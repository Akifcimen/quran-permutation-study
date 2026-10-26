#ifndef JOINT_STUDY_SAMPLER_HPP
#define JOINT_STUDY_SAMPLER_HPP

#include "common.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <numeric>
#include <stdexcept>
#include <utility>

namespace study {

// SplitMix64. bounded() uses a 32x32 -> 64 bit multiply/reject construction;
// unlike remainder reduction without rejection it has no range/modulo bias.
struct RNG {
    std::uint64_t state;
    explicit RNG(std::uint64_t seed) : state(seed) {}

    std::uint64_t next() {
        std::uint64_t z = (state += UINT64_C(0x9e3779b97f4a7c15));
        z = (z ^ (z >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
        z = (z ^ (z >> 27)) * UINT64_C(0x94d049bb133111eb);
        return z ^ (z >> 31);
    }

    std::uint32_t bounded(std::uint32_t r) {
        if (r == 0) throw std::invalid_argument("RNG::bounded requires r > 0");
        std::uint32_t x = static_cast<std::uint32_t>(next() >> 32);
        std::uint64_t product = static_cast<std::uint64_t>(x) * r;
        std::uint32_t low = static_cast<std::uint32_t>(product);
        if (low < r) {
            const std::uint32_t threshold = (std::uint32_t(0) - r) % r;
            while (low < threshold) {
                x = static_cast<std::uint32_t>(next() >> 32);
                product = static_cast<std::uint64_t>(x) * r;
                low = static_cast<std::uint32_t>(product);
            }
        }
        return static_cast<std::uint32_t>(product >> 32);
    }

    double unif() {
        return static_cast<double>(next() >> 11) * 0x1.0p-53;
    }
};

struct Draw {
    std::array<int, N> ids{};
    // Attempts consumed by this accepted draw, not the cumulative total.
    std::uint64_t proposals = 0;
};

namespace sampling_detail {

inline long double truncated_geometric_mean(long double q, int m) {
    if (m <= 1) return 0;
    if (q == 1) return static_cast<long double>(m - 1) / 2;
    long double numerator = 0, denominator = 0, w = 1;
    for (int k = 0; k < m; ++k) {
        numerator += k * w;
        denominator += w;
        w *= q;
    }
    return numerator / denominator;
}

template <std::size_t M>
long double expected_strict_inversions(const std::array<int, M>& values,
                                      long double q) {
    std::array<int, M> sorted = values;
    std::sort(sorted.begin(), sorted.end());
    long double mean = 0;
    for (int m = 1; m <= static_cast<int>(M); ++m)
        mean += truncated_geometric_mean(q, m);
    // A labeled Mallows law contains independent internal tie permutations.
    // Subtract their means to obtain strict numerical, rather than labeled,
    // inversions. The tie q-factorial partition factors are constant over words.
    for (int begin = 0; begin < static_cast<int>(M);) {
        int end = begin + 1;
        while (end < static_cast<int>(M) && sorted[end] == sorted[begin]) ++end;
        for (int m = 1; m <= end - begin; ++m)
            mean -= truncated_geometric_mean(q, m);
        begin = end;
    }
    return mean;
}

template <std::size_t M>
double fit_projected_q(const std::array<int, M>& values, int target) {
    const long double at_one = expected_strict_inversions(values, 1);
    if (target <= 0 || target >= at_one)
        throw std::invalid_argument("Requested inversion mean must lie between 0 and the uniform mean");
    long double lo = 0, hi = 1;
    for (int iteration = 0; iteration < 80; ++iteration) {
        const long double mid = (lo + hi) / 2;
        if (expected_strict_inversions(values, mid) < target) lo = mid;
        else hi = mid;
    }
    return static_cast<double>((lo + hi) / 2);
}

// The size template permits an exhaustive small-multiset validator to exercise
// the exact production proposal code, without depending on the study features.
template <std::size_t M>
class MallowsCore {
    static_assert(M > 0 && M < 65536, "Unsupported alias-table dimension");
    static constexpr std::size_t TABLE = M * (M + 1) / 2;
    std::array<int, M> values_{};
    std::array<int, M> reference_{}; // ascending; largest remaining item at end
    std::array<int, M> group_of_id_{};
    std::array<int, M> group_start_{};
    std::array<int, M> group_size_{};
    int groups_ = 0;
    std::array<std::size_t, M + 1> offsets_{};
    std::array<double, TABLE> probability_{};
    std::array<std::uint16_t, TABLE> alias_{};

public:
    MallowsCore() = default;
    MallowsCore(const std::array<int, M>& values, double q) { initialize(values, q); }

    void initialize(const std::array<int, M>& values, double q) {
        if (!(q >= 0 && q <= 1)) throw std::invalid_argument("Mallows q must be in [0,1]");
        values_ = values;
        std::iota(reference_.begin(), reference_.end(), 0);
        std::sort(reference_.begin(), reference_.end(), [&](int a, int b) {
            return values_[a] == values_[b] ? a < b : values_[a] < values_[b];
        });
        groups_ = 0;
        for (int start = 0; start < static_cast<int>(M);) {
            int end = start + 1;
            while (end < static_cast<int>(M) && values_[reference_[end]] == values_[reference_[start]]) ++end;
            group_start_[groups_] = start;
            group_size_[groups_] = end - start;
            for (int i = start; i < end; ++i) group_of_id_[reference_[i]] = groups_;
            ++groups_;
            start = end;
        }
        std::size_t offset = 0;
        for (int r = 1; r <= static_cast<int>(M); ++r) {
            offsets_[r] = offset;
            std::array<long double, M> weights{};
            std::array<double, M> scaled{};
            std::array<int, M> small{}, large{};
            int ns = 0, nl = 0;
            long double sum = 0, w = 1;
            for (int j = 0; j < r; ++j) {
                weights[j] = w;
                sum += w;
                w *= static_cast<long double>(q);
            }
            for (int j = 0; j < r; ++j) {
                scaled[j] = static_cast<double>(weights[j] / sum * r);
                if (scaled[j] < 1) small[ns++] = j;
                else large[nl++] = j;
            }
            while (ns && nl) {
                const int s = small[--ns], l = large[--nl];
                probability_[offset + s] = std::max(0.0, scaled[s]);
                alias_[offset + s] = static_cast<std::uint16_t>(l);
                scaled[l] = scaled[l] + scaled[s] - 1;
                if (scaled[l] < 1) small[ns++] = l;
                else large[nl++] = l;
            }
            while (nl) {
                const int l = large[--nl];
                probability_[offset + l] = 1;
                alias_[offset + l] = static_cast<std::uint16_t>(l);
            }
            while (ns) {
                const int s = small[--ns];
                probability_[offset + s] = 1;
                alias_[offset + s] = static_cast<std::uint16_t>(s);
            }
            offset += r;
        }
    }

    int rank(RNG& rng, int remaining) const {
        if (remaining == 1) return 0;
        const auto column = rng.bounded(static_cast<std::uint32_t>(remaining));
        const auto k = offsets_[remaining] + column;
        return rng.unif() < probability_[k] ? static_cast<int>(column) : alias_[k];
    }

    // P(numeric word) is proportional to q^strict_inversions. Internal label
    // orders are still Mallows-biased until uniformize_tie_labels() is called.
    // Early rejection is valid because each added strict-pair count is >= 0.
    bool generate(RNG& rng, std::array<int, M>& ids, int& inversions,
                  int maximum_inversions = static_cast<int>(M * (M - 1) / 2)) const {
        std::array<int, M> remaining_ids = reference_;
        inversions = 0;
        for (int position = 0; position < static_cast<int>(M); ++position) {
            const int remaining = static_cast<int>(M) - position;
            const int k = rank(rng, remaining);
            const int ix = remaining - 1 - k;
            const int id = remaining_ids[ix];
            int end_of_equal = ix + 1;
            while (end_of_equal < remaining && values_[remaining_ids[end_of_equal]] == values_[id]) ++end_of_equal;
            inversions += remaining - end_of_equal;
            if (inversions > maximum_inversions) return false;
            ids[position] = id;
            // Ascending storage shifts k items (typically small when q < 1).
            std::memmove(remaining_ids.data() + ix, remaining_ids.data() + ix + 1,
                         static_cast<std::size_t>(k) * sizeof(int));
        }
        return true;
    }

    void uniformize_tie_labels(RNG& rng, std::array<int, M>& ids) const {
        std::array<int, M> positions{}, used{};
        for (int position = 0; position < static_cast<int>(M); ++position) {
            const int group = group_of_id_[ids[position]];
            positions[group_start_[group] + used[group]++] = position;
        }
        for (int group = 0; group < groups_; ++group) {
            const int start = group_start_[group];
            for (int j = group_size_[group] - 1; j > 0; --j) {
                const auto other = rng.bounded(static_cast<std::uint32_t>(j + 1));
                std::swap(ids[positions[start + j]], ids[positions[start + other]]);
            }
        }
    }
};

template <std::size_t M>
int changed_value_inversion_delta(const std::array<int, M>& ids,
                                  const std::array<int, M>& values,
                                  int changed_id, int replacement) {
    const int original = values[changed_id];
    bool before_changed = true;
    int delta = 0;
    for (int id : ids) {
        if (id == changed_id) {
            before_changed = false;
            continue;
        }
        const int v = values[id];
        if (before_changed) delta += int(v < replacement) - int(v < original);
        else delta += int(replacement < v) - int(original < v);
    }
    return delta;
}

} // namespace sampling_detail

struct Sampler {
    std::uint64_t proposals = 0;
    double q = 1;

    Sampler(const std::array<int, N>& base129, int model, std::uint64_t seed)
        : base_(base129), model_(model), rng_(seed) {
        if (model < 0 || model > 2) throw std::invalid_argument("Unknown sampling model");
        if (base_[8] != 129) throw std::invalid_argument("Original surah ID8 must have129 verses");
        if (model_ == 2) {
            q = sampling_detail::fit_projected_q(base_, 993);
            mallows_.initialize(base_, q);
        }
    }

    Draw next() {
        Draw draw;
        if (model_ < 2) {
            std::iota(draw.ids.begin(), draw.ids.end(), 0);
            if (model_ == 0) {
                for (int i = N - 1; i > 0; --i)
                    std::swap(draw.ids[i], draw.ids[rng_.bounded(static_cast<std::uint32_t>(i + 1))]);
            } else {
                static_assert(N % 6 == 0, "Block model requires blocks of six");
                for (int block = 0; block < N; block += 6)
                    for (int i = 5; i > 0; --i)
                        std::swap(draw.ids[block + i], draw.ids[block + rng_.bounded(static_cast<std::uint32_t>(i + 1))]);
            }
            ++proposals;
            draw.proposals = 1;
            return draw;
        }

        for (;;) {
            ++proposals;
            ++draw.proposals;
            int inversions = 0;
            if (!mallows_.generate(rng_, draw.ids, inversions, 993) || inversions != 993) continue;
            // Uniform relabeling matters: merely projecting labeled Mallows
            // produces the right numeric law but not uniform whole-sura labels.
            // Apply it BEFORE the second constraint, even if changed_id had ties.
            mallows_.uniformize_tie_labels(rng_, draw.ids);
            const int delta = sampling_detail::changed_value_inversion_delta(draw.ids, base_, 8, 127);
            if (inversions + delta == 994) return draw;
        }
    }

private:
    std::array<int, N> base_{};
    int model_;
    RNG rng_;
    sampling_detail::MallowsCore<N> mallows_;
};

} // namespace study

#endif
