#include "sampler.hpp"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace {
void check(bool ok, const std::string& message) {
    if (!ok) throw std::runtime_error(message);
}

template <std::size_t M>
int reference_inversions(const std::array<int, M>& ids, const std::array<int, M>& values) {
    // Deliberately slow independent definition, with no rank/tie shortcuts.
    int result = 0;
    for (std::size_t i = 0; i < M; ++i)
        for (std::size_t j = i + 1; j < M; ++j)
            result += values[ids[i]] < values[ids[j]];
    return result;
}

template <std::size_t M>
void check_labels(const std::array<int, M>& ids) {
    std::array<bool, M> seen{};
    for (int id : ids) {
        check(id >= 0 && id < static_cast<int>(M), "ID outside range");
        check(!seen[id], "Repeated ID");
        seen[id] = true;
    }
}

double statistic(const std::map<std::array<int, 4>, std::uint64_t>& observed,
                 const std::map<std::array<int, 4>, long double>& probabilities,
                 std::uint64_t n) {
    double chi2 = 0;
    for (const auto& item : probabilities) {
        const double expectation = static_cast<double>(item.second * n);
        const auto found = observed.find(item.first);
        const double count = found == observed.end() ? 0 : static_cast<double>(found->second);
        chi2 += (count - expectation) * (count - expectation) / expectation;
    }
    return chi2;
}

void normalize(std::map<std::array<int, 4>, long double>& weights) {
    long double sum = 0;
    for (const auto& p : weights) sum += p.second;
    for (auto& p : weights) p.second /= sum;
}
} // namespace

int main(int argc, char** argv) {
    try {
        if (argc != 2) throw std::runtime_error("Usage: test_sampler counts.txt");
        const auto start = std::chrono::steady_clock::now();
        std::array<int, study::N> base{};
        std::ifstream input(argv[1]);
        for (int& value : base) check(bool(input >> value), "Failed to read114 baseline counts");
        std::array<int, study::N> identity{};
        std::iota(identity.begin(), identity.end(), 0);
        auto changed = base;
        changed[8] = 127;
        check(reference_inversions(identity, base) == 993, "Baseline observed inversion count mismatch");
        check(reference_inversions(identity, changed) == 994, "Edited observed inversion count mismatch");

        study::RNG golden(0);
        check(golden.next() == UINT64_C(0xe220a8397b1dcdaf), "SplitMix64 known first word mismatch");
        study::RNG bounded(3141592653ULL);
        double bounded_chi2 = 0;
        std::array<int, 3> bins{};
        constexpr int BOUND_DRAWS = 150000;
        for (int i = 0; i < BOUND_DRAWS; ++i) ++bins[bounded.bounded(3)];
        for (int count : bins) bounded_chi2 += (count - BOUND_DRAWS / 3.0) * (count - BOUND_DRAWS / 3.0) / (BOUND_DRAWS / 3.0);
        check(bounded_chi2 < 35, "bounded(3) distribution check failed");
        for (std::uint32_t r : {std::uint32_t(1), std::uint32_t(114), UINT32_C(2147483649), UINT32_C(4294967295)}) {
            long double mean = 0;
            for (int i = 0; i < 30000; ++i) {
                const auto k = bounded.bounded(r);
                check(k < r, "Bounded RNG returned out-of-range result");
                if (r > 1) mean += static_cast<long double>(k) / (r - 1);
                const double u = bounded.unif();
                check(u >= 0 && u < 1, "Uniform floating draw outside[0,1)");
            }
            if (r > 1) check(std::fabs(mean / 30000 - 0.5L) < 0.015L, "Large-range bounded RNG moment check failed");
        }

        // Exhaustive toy reference probabilities are computed from O(n^2)
        // pair comparisons, independently of the production rank sampler.
        const std::array<int, 4> toy = {3, 3, 2, 1};
        std::map<std::array<int, 4>, long double> expected_labels, expected_raw;
        std::map<std::array<int, 4>, std::uint64_t> labels, raw;
        std::array<int, 4> permutation = {0, 1, 2, 3};
        const std::array<int, 4> descending_reference_rank = {1, 0, 2, 3};
        do {
            expected_labels[permutation] = std::pow(2.0L / 3.0L, reference_inversions(permutation, toy));
            int labeled_inversions = 0;
            for (int i = 0; i < 4; ++i)
                for (int j = i + 1; j < 4; ++j)
                    labeled_inversions += descending_reference_rank[permutation[i]] > descending_reference_rank[permutation[j]];
            expected_raw[permutation] = std::pow(2.0L / 3.0L, labeled_inversions);
            for (int special = 0; special < 4; ++special) {
                for (int replacement = -1; replacement <= 4; ++replacement) {
                    auto altered = toy;
                    altered[special] = replacement;
                    const int actual_delta = reference_inversions(permutation, altered) - reference_inversions(permutation, toy);
                    const int fast_delta = study::sampling_detail::changed_value_inversion_delta(permutation, toy, special, replacement);
                    check(actual_delta == fast_delta, "Edited-value delta failed exhaustive tie/multiple-crossing test");
                }
            }
        } while (std::next_permutation(permutation.begin(), permutation.end()));
        normalize(expected_labels);
        normalize(expected_raw);
        const long double exact_toy_mean = 2164.0L / 1235.0L;
        check(std::fabs(study::sampling_detail::expected_strict_inversions(toy, 2.0L / 3.0L) - exact_toy_mean) < 1e-12L,
              "Projected mean differs from exact four-label enumeration");
        study::sampling_detail::MallowsCore<4> core(toy, 2.0 / 3.0);
        study::RNG small_rng(20261025);
        constexpr std::uint64_t TOY_DRAWS = 240000;
        long double empirical_mean = 0;
        for (std::uint64_t i = 0; i < TOY_DRAWS; ++i) {
            int inversions = -1;
            check(core.generate(small_rng, permutation, inversions), "Uncapped toy generation rejected");
            check_labels(permutation);
            check(inversions == reference_inversions(permutation, toy), "On-the-fly inversion count wrong");
            ++raw[permutation];
            empirical_mean += inversions;
            core.uniformize_tie_labels(small_rng, permutation);
            check(inversions == reference_inversions(permutation, toy), "Tie relabeling changed numeric inversions");
            ++labels[permutation];
        }
        empirical_mean /= TOY_DRAWS;
        const double toy_raw_chi2 = statistic(raw, expected_raw, TOY_DRAWS);
        const double toy_labeled_chi2 = statistic(labels, expected_labels, TOY_DRAWS);
        check(toy_raw_chi2 < 95, "Raw labeled Mallows distribution check failed");
        check(toy_labeled_chi2 < 95, "Projected uniformly labeled Mallows distribution check failed");
        check(std::fabs(empirical_mean - exact_toy_mean) < 0.02, "Projected inversion mean sampling check failed");

        // Joint constraint changes label2 from2 to0. Exactly four orders qualify;
        // raw Mallows weights are unequal, but relabeling makes all four uniform.
        auto toy_changed = toy;
        toy_changed[2] = 0;
        std::map<std::array<int, 4>, long double> conditional_probabilities;
        permutation = {0, 1, 2, 3};
        do {
            if (reference_inversions(permutation, toy) == 2 && reference_inversions(permutation, toy_changed) == 3)
                conditional_probabilities[permutation] = 1;
        } while (std::next_permutation(permutation.begin(), permutation.end()));
        check(conditional_probabilities.size() == 4, "Unexpected exact toy joint support");
        normalize(conditional_probabilities);
        std::map<std::array<int, 4>, std::uint64_t> conditional_counts;
        constexpr std::uint64_t CONDITIONAL_TOY_DRAWS = 60000;
        std::uint64_t toy_attempts = 0;
        for (std::uint64_t accepted = 0; accepted < CONDITIONAL_TOY_DRAWS;) {
            ++toy_attempts;
            int inversions = -1;
            if (!core.generate(small_rng, permutation, inversions, 2) || inversions != 2) continue;
            core.uniformize_tie_labels(small_rng, permutation);
            const int delta = study::sampling_detail::changed_value_inversion_delta(permutation, toy, 2, 0);
            if (inversions + delta != 3) continue;
            check(reference_inversions(permutation, toy_changed) == 3, "Toy second-condition count wrong");
            ++conditional_counts[permutation];
            ++accepted;
        }
        const double conditional_chi2 = statistic(conditional_counts, conditional_probabilities, CONDITIONAL_TOY_DRAWS);
        check(conditional_chi2 < 35, "Conditional toy law not uniform on exact support");

        for (int model = 0; model < 2; ++model) {
            study::Sampler sampler(base, model, 1001 + model);
            bool moved = false;
            for (int i = 0; i < 1000; ++i) {
                const auto draw = sampler.next();
                check_labels(draw.ids);
                check(draw.proposals == 1, "Direct model proposal accounting wrong");
                moved = moved || draw.ids != identity;
                if (model == 1)
                    for (int position = 0; position < study::N; ++position)
                        check(draw.ids[position] / 6 == position / 6, "Block model moved a surah outside its fixed block");
            }
            check(moved, "Direct model never reordered labels");
            check(sampler.proposals == 1000, "Cumulative direct proposals wrong");
        }

        study::Sampler conditional(base, 2, 88172645463325252ULL);
        check(std::fabs(study::sampling_detail::expected_strict_inversions(base, conditional.q) - 993) < 1e-8,
              "Fitted Mallows mean differs from993");
        std::uint64_t summed_attempts = 0;
        std::uint64_t max_attempts = 0;
        constexpr int ACTUAL_DRAWS = 1000;
        for (int i = 0; i < ACTUAL_DRAWS; ++i) {
            const auto draw = conditional.next();
            check_labels(draw.ids);
            check(reference_inversions(draw.ids, base) == 993, "Accepted actual draw fails I129=993");
            check(reference_inversions(draw.ids, changed) == 994, "Accepted actual draw fails I127=994");
            check(draw.proposals >= 1, "Empty rejection proposal count");
            summed_attempts += draw.proposals;
            max_attempts = std::max(max_attempts, draw.proposals);
        }
        check(summed_attempts == conditional.proposals, "Conditional cumulative proposal accounting wrong");
        for (int model = 0; model < 3; ++model) {
            study::Sampler first(base, model, 20261101), second(base, model, 20261101);
            for (int i = 0; i < 5; ++i) {
                const auto a = first.next(), b = second.next();
                check(a.ids == b.ids && a.proposals == b.proposals, "Seeded reproducibility failure");
            }
        }
        const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        std::cout << std::setprecision(17)
                  << "{\n  \"status\":\"passed\",\n"
                  << "  \"bounded_rng_chi2_df2\":" << bounded_chi2 << ",\n"
                  << "  \"toy_proposals\":" << TOY_DRAWS << ",\n"
                  << "  \"toy_raw_mallows_chi2_df23\":" << toy_raw_chi2 << ",\n"
                  << "  \"toy_uniformly_labeled_projection_chi2_df23\":" << toy_labeled_chi2 << ",\n"
                  << "  \"toy_mean_exact\":" << static_cast<double>(exact_toy_mean) << ",\n"
                  << "  \"toy_mean_empirical\":" << static_cast<double>(empirical_mean) << ",\n"
                  << "  \"toy_conditional_accepted\":" << CONDITIONAL_TOY_DRAWS << ",\n"
                  << "  \"toy_conditional_attempts\":" << toy_attempts << ",\n"
                  << "  \"toy_conditional_uniform_chi2_df3\":" << conditional_chi2 << ",\n"
                  << "  \"uniform_draws_checked\":1000,\n  \"fixed_block_draws_checked\":1000,\n"
                  << "  \"actual_conditional_draws_checked\":" << ACTUAL_DRAWS << ",\n"
                  << "  \"actual_conditional_proposals\":" << conditional.proposals << ",\n"
                  << "  \"actual_conditional_max_proposals_per_draw\":" << max_attempts << ",\n"
                  << "  \"q\":" << conditional.q << ",\n"
                  << "  \"elapsed_seconds\":" << seconds << "\n}\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Sampler validation failed: " << error.what() << '\n';
        return 1;
    }
}
