#ifndef JOINT_RARE_EVENT_KERNELS_HPP
#define JOINT_RARE_EVENT_KERNELS_HPP

#include "../../src/sampler.hpp"
#include <array>
#include <cstdint>
#include <numeric>
#include <stdexcept>
#include <utility>

namespace rare {

using Ids = std::array<int, study::N>;

// Strict ascending pairs: the study's disorder relative to descending lengths.
// The independent validator compares this O(distance) update to O(M^2) counts.
template <std::size_t M>
int swap_inversion_delta(const std::array<int,M>& ids,
                         const std::array<int,M>& values,
                         int left, int right) {
    if (left == right) return 0;
    if (left > right) std::swap(left, right);
    const int x = values[ids[left]], y = values[ids[right]];
    int delta = int(y < x) - int(x < y);
    for (int k = left + 1; k < right; ++k) {
        const int z = values[ids[k]];
        delta += int(y < z) + int(z < x) - int(x < z) - int(z < y);
    }
    return delta;
}

template <std::size_t M>
std::array<int,M> value_ranks(const std::array<int,M>& values) {
    auto sorted = values;
    std::sort(sorted.begin(), sorted.end());
    std::array<int,M> ranks{};
    for (std::size_t id = 0; id < M; ++id)
        ranks[id] = int(std::lower_bound(sorted.begin(), sorted.end(), values[id]) - sorted.begin());
    return ranks;
}

// Prefix[position][rank] counts preceding values with value-rank < rank.
// This permits every possible swap delta to be queried in constant time.
template <std::size_t M>
struct RankPrefix {
    std::array<std::array<unsigned short,M+1>,M+1> prefix{};
    std::array<int,M> rank_at{};
    RankPrefix(const std::array<int,M>& ids, const std::array<int,M>& ranks) {
        for (std::size_t pos = 0; pos < M; ++pos) {
            rank_at[pos] = ranks[ids[pos]];
            prefix[pos+1] = prefix[pos];
            for (int rank = rank_at[pos]+1; rank <= int(M); ++rank)
                ++prefix[pos+1][rank];
        }
    }
    int delta(int left, int right) const {
        if (left == right) return 0;
        if (left > right) std::swap(left,right);
        const int x = rank_at[left], y = rank_at[right];
        if (x == y) return 0;
        const int low = std::min(x,y), high = std::max(x,y);
        const auto less = [&](int r) { return int(prefix[right][r])-int(prefix[left+1][r]); };
        const int inclusive = less(high+1)-less(low);
        const int strict = less(high)-less(low+1);
        return (x > y ? 1 : -1) * (1+inclusive+strict);
    }
};

enum class MoveKind : unsigned {
    Independent = 0,
    Transposition = 1,
    TwoTranspositions = 2,
    ThreeCycle = 3,
    SixShuffle = 4,
    AdjacentTwo = 5,
    AdjacentThreeCycle = 6,
    FourTranspositions = 7,
    ParityTransposition = 8,
    ParityTwo = 9,
    ResidueTransposition = 10,
    ResidueTwo = 11,
    PairedRepair = 12
};

inline const char* move_name(MoveKind kind) {
    static constexpr const char* names[] = {
        "independent", "transposition", "two_transpositions", "three_cycle",
        "six_shuffle", "adjacent_two", "adjacent_three_cycle",
        "four_transpositions", "parity_transposition", "parity_two",
        "residue_transposition", "residue_two", "paired_repair"
    };
    return names[static_cast<unsigned>(kind)];
}

struct Proposal {
    study::Draw draw{};
    MoveKind kind = MoveKind::Independent;
    bool null_ok = true;
    bool changed = false;          // Any labeled ID changed position.
    bool numeric_changed = false;  // At least one 129/127 numeric value changed.
    int delta129 = 0;              // Computed for inversion-fiber local moves.
    int delta127 = 0;              // Computed for inversion-fiber local moves.
    std::uint64_t refresh_proposals = 0; // Mallows attempts for an independent refresh.
    int repair_alternatives = 0; // Valid second swaps other than undoing the first.
    int filter_prime = 0;        // Prime selected independently for residue moves.
};

// Each local branch is symmetric on labeled permutations:
//
// * a transposition is its own inverse;
// * a fixed number of iid transpositions has the same probability as its
//   reverse sequence;
// * both orientations of a selected three-cycle have equal probability;
// * a selected six-position set is shuffled uniformly.
//
// Position/set choices and mixture weights are independent of the state.
// Rejecting proposals outside the joint inversion fiber therefore preserves
// its uniform distribution. The block model chooses positions within its fixed
// six-position blocks. Independent refreshes use the validated uniform null
// sampler; positive refresh weight gives full support on each null's support.
//
// A caller may additionally reject candidates outside a splitting-level set,
// or apply a Metropolis ratio for a soft density. Do not retry proposals until
// one passes: a rejection must be an actual self-transition.
//
// Full support is a mathematical property, not a finite-mixing guarantee.
// In a deep rare-event set, independent refreshes will usually be rejected.
// Keep level-specific numeric movement and genealogy diagnostics.
//
// Construct once per worker, not per particle: the inversion refresh sampler
// builds a Mallows alias table at construction.
class MutationKernel {
public:
    MutationKernel(const Ids& base129, int model, std::uint64_t seed,
                   unsigned refresh_per_mille = 10)
        : base129_(base129), base127_(base129), model_(model),
          refresh_per_mille_(refresh_per_mille),
          rng_(mix(seed ^ UINT64_C(0x848b50ccb25c0e7d))),
          refresh_(base129, model, mix(seed ^ UINT64_C(0x53e9f55e0d02b4cb))) {
        if (model < 0 || model > 2)
            throw std::invalid_argument("Unknown mutation null model");
        if (refresh_per_mille > 1000)
            throw std::invalid_argument("Refresh weight must be in [0,1000]");
        base127_[8] = 127;
        ranks129_ = value_ranks(base129_);
        ranks127_ = value_ranks(base127_);
    }

    Proposal propose(const Ids& current) {
        MoveKind kind;
        if (rng_.bounded(1000) < refresh_per_mille_) {
            kind = MoveKind::Independent;
        } else {
            const unsigned choice = rng_.bounded(100);
            if (model_ == 2) {
                // A single unequal-value swap cannot preserve inversion count.
                if (choice < 30) kind = MoveKind::AdjacentTwo;
                else if (choice < 40) kind = MoveKind::AdjacentThreeCycle;
                else if (choice < 60) kind = MoveKind::TwoTranspositions;
                else if (choice < 65) kind = MoveKind::FourTranspositions;
                else if (choice < 75) kind = MoveKind::ParityTwo;
                else if (choice < 85) kind = MoveKind::ResidueTwo;
                else kind = MoveKind::PairedRepair;
            } else {
                if (choice < 45) kind = MoveKind::Transposition;
                else if (choice < 60) kind = MoveKind::TwoTranspositions;
                else if (choice < 70) kind = MoveKind::ThreeCycle;
                else if (choice < 75) kind = MoveKind::SixShuffle;
                else if (choice < 90) kind = MoveKind::ParityTransposition;
                else kind = MoveKind::ResidueTransposition;
            }
        }
        return propose_kind(current, kind);
    }

    // Exposed for exact primitive validation and declared pilot comparisons.
    // Choosing a kind using the current state generally destroys symmetry.
    Proposal propose_kind(const Ids& current, MoveKind kind) {
        Proposal result;
        result.kind = kind;
        result.draw.ids = current;
        result.draw.proposals = 1;
        if (kind == MoveKind::Independent) {
            result.draw = refresh_.next();
            result.refresh_proposals = result.draw.proposals;
        } else if (kind == MoveKind::PairedRepair) {
            if (model_ != 2) throw std::invalid_argument("Paired repair requires inversion model");
            const auto first = pair();
            swap(result,first.first,first.second);
            const int wanted129 = -result.delta129, wanted127 = -result.delta127;
            const RankPrefix<study::N> prefix129(result.draw.ids,ranks129_);
            const RankPrefix<study::N> prefix127(result.draw.ids,ranks127_);
            std::pair<int,int> selected = first; // The inverse always restores x.
            const int first_lo=std::min(first.first,first.second), first_hi=std::max(first.first,first.second);
            for (int a=0; a<study::N; ++a) for (int b=a+1; b<study::N; ++b) {
                if (a==first_lo && b==first_hi) continue;
                if (prefix129.delta(a,b)!=wanted129 || prefix127.delta(a,b)!=wanted127) continue;
                ++result.repair_alternatives;
                if (rng_.bounded(result.repair_alternatives)==0) selected={a,b};
            }
            swap(result,selected.first,selected.second);
            if (result.delta129 || result.delta127)
                throw std::logic_error("Paired repair failed to restore the inversion fiber");
        } else if (kind == MoveKind::ParityTransposition || kind == MoveKind::ParityTwo ||
                   kind == MoveKind::ResidueTransposition || kind == MoveKind::ResidueTwo) {
            const bool residue = kind==MoveKind::ResidueTransposition || kind==MoveKind::ResidueTwo;
            const int number = kind==MoveKind::ParityTwo || kind==MoveKind::ResidueTwo ? 2 : 1;
            const int modulus = residue ? 2*(result.filter_prime=prime()) : 2;
            for (int k=0; k<number; ++k) {
                const auto positions=parity_pair();
                const int a=result.draw.ids[positions.first], b=result.draw.ids[positions.second];
                // The filter is invariant to reversing this same swap. A failed
                // filter is a self-transition, never a retry-until-accepted loop.
                if ((base129_[a]-base129_[b])%modulus==0 &&
                    (base127_[a]-base127_[b])%modulus==0)
                    swap(result,positions.first,positions.second);
            }
        } else if (kind == MoveKind::Transposition ||
                   kind == MoveKind::TwoTranspositions ||
                   kind == MoveKind::FourTranspositions) {
            const int number = kind == MoveKind::Transposition ? 1 :
                               kind == MoveKind::TwoTranspositions ? 2 : 4;
            for (int k = 0; k < number; ++k) {
                const auto positions = pair();
                swap(result, positions.first, positions.second);
            }
        } else if (kind == MoveKind::AdjacentTwo) {
            for (int k = 0; k < 2; ++k) {
                int position;
                if (model_ == 1)
                    position = 6 * int(rng_.bounded(study::N / 6)) + int(rng_.bounded(5));
                else
                    position = int(rng_.bounded(study::N - 1));
                swap(result, position, position + 1);
            }
        } else if (kind == MoveKind::ThreeCycle ||
                   kind == MoveKind::AdjacentThreeCycle) {
            std::array<int,3> positions;
            if (kind == MoveKind::AdjacentThreeCycle) {
                int start;
                if (model_ == 1)
                    start = 6 * int(rng_.bounded(study::N / 6)) + int(rng_.bounded(4));
                else
                    start = int(rng_.bounded(study::N - 2));
                positions = {start, start + 1, start + 2};
            } else {
                positions = subset<3>();
            }
            const bool reverse = rng_.bounded(2);
            if (!reverse) {
                swap(result, positions[0], positions[1]);
                swap(result, positions[1], positions[2]);
            } else {
                swap(result, positions[1], positions[2]);
                swap(result, positions[0], positions[1]);
            }
        } else if (kind == MoveKind::SixShuffle) {
            auto positions = subset<6>();
            for (int k = 5; k > 0; --k)
                swap(result, positions[k], positions[rng_.bounded(k + 1)]);
        } else {
            throw std::invalid_argument("Unknown mutation kind");
        }

        if (model_ == 2 && kind != MoveKind::Independent)
            result.null_ok = result.delta129 == 0 && result.delta127 == 0;
        result.changed = result.draw.ids != current;
        for (int n = 0; n < study::N; ++n) {
            const int before = current[n], after = result.draw.ids[n];
            result.numeric_changed |= base129_[before] != base129_[after] ||
                                      base127_[before] != base127_[after];
        }
        ++proposed;
        null_valid += result.null_ok;
        null_valid_numeric_moves += result.null_ok && result.numeric_changed;
        independent_attempts += result.refresh_proposals;
        return result;
    }

    std::uint64_t proposed = 0;
    std::uint64_t null_valid = 0;
    std::uint64_t null_valid_numeric_moves = 0;
    std::uint64_t independent_attempts = 0;

private:
    Ids base129_{}, base127_{}, ranks129_{}, ranks127_{};
    int model_;
    unsigned refresh_per_mille_;
    study::RNG rng_;
    study::Sampler refresh_;

    static std::uint64_t mix(std::uint64_t seed) {
        study::RNG mixer(seed);
        return mixer.next();
    }

    std::pair<int,int> pair() {
        const int width = model_ == 1 ? 6 : study::N;
        const int offset = model_ == 1 ? 6 * int(rng_.bounded(study::N/6)) : 0;
        const int a = int(rng_.bounded(width));
        int b = int(rng_.bounded(width - 1));
        if (b >= a) ++b;
        return {offset + a, offset + b};
    }

    std::pair<int,int> parity_pair() {
        const int width=model_==1 ? 3 : study::N/2;
        const int offset=(model_==1 ? 6*int(rng_.bounded(study::N/6)) : 0)+int(rng_.bounded(2));
        const int a=int(rng_.bounded(width));
        int b=int(rng_.bounded(width-1));
        if (b>=a) ++b;
        return {offset+2*a,offset+2*b};
    }

    int prime() {
        static constexpr std::array<int,29> all={3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113};
        static constexpr std::array<int,5> literal={3,5,7,11,19};
        static constexpr std::array<int,4> discussion={3,5,7,19};
        const int branch=int(rng_.bounded(3));
        if (branch==0) return all[rng_.bounded(all.size())];
        if (branch==1) return literal[rng_.bounded(literal.size())];
        return discussion[rng_.bounded(discussion.size())];
    }

    template <std::size_t M>
    std::array<int,M> subset() {
        const int width = model_ == 1 ? 6 : study::N;
        const int offset = model_ == 1 ? 6 * int(rng_.bounded(study::N/6)) : 0;
        std::array<int,M> result{};
        for (std::size_t k = 0; k < M; ++k) {
            bool duplicate;
            do {
                result[k] = offset + int(rng_.bounded(width));
                duplicate = false;
                for (std::size_t j = 0; j < k; ++j)
                    duplicate |= result[j] == result[k];
            } while (duplicate);
        }
        return result;
    }

    void swap(Proposal& proposal, int a, int b) {
        if (model_ == 2) {
            proposal.delta129 += swap_inversion_delta(proposal.draw.ids, base129_, a, b);
            proposal.delta127 += swap_inversion_delta(proposal.draw.ids, base127_, a, b);
        }
        std::swap(proposal.draw.ids[a], proposal.draw.ids[b]);
    }
};

} // namespace rare
#endif

