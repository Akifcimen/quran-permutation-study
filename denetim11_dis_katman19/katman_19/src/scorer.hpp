#ifndef JOINT_STUDY_SCORER_HPP
#define JOINT_STUDY_SCORER_HPP

#include "hist.hpp"
#include "matrix.hpp"

#include <array>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace study {

struct ScoreBlock {
  int modules = 0;
  int form = -1;
  int prime_index = -1;
  int matrix_view = -1;
  std::array<uint8_t, 4> positive_masks{};
  double value = -std::numeric_limits<double>::infinity();
};

struct ScoreTrace {
  int variant = -1;
  int active = 0;
  int block_count = 0;
  std::array<ScoreBlock, 4> blocks{};
  double blocks_value = 0;
  double program_penalty = 0;
};

struct Scores {
  std::array<double, K> value{};
  std::array<ScoreTrace, K> trace{};
  uint64_t training_n = 0;
};

// A trained SURROGATE ranking score. Its sum of module surprisals is not
// asserted to be an exact joint log probability. Inference requires an
// independent full-search Monte Carlo calibration.
class Scorer {
 public:
  inline static constexpr std::array<const char*, 6> kPolicyNames = {
      "uniform_29", "primary_mixture", "literal_integer_values",
      "number_discussion_values", "conditional_19", "literal_frequency"};

  explicit Scorer(const Hist& hist) : training_n_(hist.n) {
    static_assert(V == 2 && F == 14 && P == 29 && C == 406 && K == 6,
                  "Scorer requires the frozen finite grammar dimensions");
    if (hist.n == 0) throw std::invalid_argument("Scorer needs nonempty training histograms");
    build_utilities(hist.g, V * F, 4, 2, hist.n, ug_);
    build_utilities(hist.c, V * C, 16, 4, hist.n, uc_);
    build_utilities(hist.w, V * C, 4, 2, hist.n, uw_);
    build_utilities(hist.m, V * C * 8, 16, 7, hist.n, um_);
    for (int pi = 0; pi < P; ++pi) {
      const int p = MatrixEngine::kPrimes[pi];
      const double uniform = 1.0 / P;
      const bool literal = p == 3 || p == 5 || p == 7 || p == 11 || p == 19;
      const bool discussion = p == 3 || p == 5 || p == 7 || p == 19;
      const double literal_weight = literal ? 1.0 / 5 : 0;
      const double discussion_weight = discussion ? 1.0 / 4 : 0;
      const int frequency = p == 3 ? 19 : p == 5 ? 2 : p == 7 ? 24 :
                            (p == 11 || p == 19) ? 1 : 0;
      const double weights[K] = {
          uniform, (uniform + literal_weight + discussion_weight) / 3,
          literal_weight, discussion_weight, p == 19 ? 1.0 : 0.0,
          static_cast<double>(frequency) / 47};
      for (int k = 0; k < K; ++k)
        logq_[k][pi] = weights[k] > 0 ? std::log2(weights[k]) : neg_inf();
    }
  }

  Scores operator()(const Features& features) const {
    Scores result;
    result.training_n = training_n_;
    const double form_cost = std::log2(static_cast<double>(F));
    constexpr int bell[5] = {1, 1, 2, 5, 15};

    for (int v = 0; v < V; ++v) {
      // Prime-specific maxima over forms are policy-independent. Optimizing
      // them once avoids rerunning the form search for each textual policy.
      std::array<std::array<ScoreBlock, 16>, P> prime_best;
      ScoreBlock g_only;
      for (int f = 0; f < F; ++f) {
        const uint8_t gm = features.g[v][f];
        if (gm >= 4) throw std::invalid_argument("G feature mask exceeds two bits");
        const double gu = ug_[(v * F + f) * 4 + gm];
        if (gu > 0 && gu - form_cost > g_only.value) {
          g_only = make_block(1, f, -1, -1, {gm, 0, 0, 0}, gu - form_cost);
        }
        for (int pi = 0; pi < P; ++pi) {
          const int cx = f * P + pi;
          const uint8_t cm = features.c[v][cx], wm = features.w[v][cx];
          if (cm >= 16 || wm >= 4)
            throw std::invalid_argument("C or W feature mask exceeds declared width");
          const double cu = uc_[(v * C + cx) * 16 + cm];
          const double wu = uw_[(v * C + cx) * 4 + wm];
          const uint32_t packed = features.m[v][cx];
          const int m_base = (v * C + cx) * 8 * 16;
          double mu = neg_inf();
          int m_view = -1;
          uint8_t mm = 0;
          for (int view = 0; view < 8; ++view) {
            const uint8_t mask = (packed >> (4 * view)) & 15;
            const double candidate = um_[m_base + view * 16 + mask];
            if (candidate > mu) {
              mu = candidate;
              m_view = view;
              mm = mask;
            }
          }
          const double u[4] = {gu, cu, wu, mu};
          int available = 0;
          for (int j = 0; j < 4; ++j) if (u[j] > 0) available |= 1 << j;

          // Exact pruning: deleting a module with nonpositive utility cannot
          // worsen binding costs and cannot increase the Bell/activation cost.
          // With T clamped below at zero, such a module cannot improve a winner.
          for (int block = available; block != 0; block = (block - 1) & available) {
            if (block == 1) continue;  // G alone has no prime parameter.
            double sum = 0;
            for (int j = 0; j < 4; ++j) if (block & (1 << j)) sum += u[j];
            const double candidate = sum - form_cost;
            if (candidate > prime_best[pi][block].value) {
              prime_best[pi][block] = make_block(
                  block, f, pi, (block & 8) ? m_view : -1,
                  {gm, cm, wm, mm}, candidate);
            }
          }
        }
      }

      for (int policy = 0; policy < K; ++policy) {
        std::array<ScoreBlock, 16> block_best;
        block_best[1] = g_only;
        for (int block = 2; block < 16; ++block) {
          for (int pi = 0; pi < P; ++pi) {
            if (!std::isfinite(logq_[policy][pi])) continue;
            const double candidate = prime_best[pi][block].value + logq_[policy][pi];
            if (candidate > block_best[block].value) {
              block_best[block] = prime_best[pi][block];
              block_best[block].value = candidate;
            }
          }
        }

        std::array<double, 16> best;
        best.fill(neg_inf());
        best[0] = 0;
        std::array<int, 16> choice{};
        for (int active = 1; active < 16; ++active) {
          const int anchor = active & -active;
          for (int block = active; block != 0; block = (block - 1) & active) {
            if ((block & anchor) == 0) continue;
            const double candidate = block_best[block].value + best[active ^ block];
            if (candidate > best[active]) {
              best[active] = candidate;
              choice[active] = block;
            }
          }
          const double penalty = 5 + std::log2(static_cast<double>(bell[popcount(active)]));
          const double value = best[active] - penalty;
          if (value > result.value[policy]) {
            result.value[policy] = value;
            ScoreTrace trace;
            trace.variant = v;
            trace.active = active;
            trace.blocks_value = best[active];
            trace.program_penalty = penalty;
            for (int remaining = active; remaining != 0;) {
              const int block = choice[remaining];
              if (block == 0) throw std::logic_error("Missing partition-DP predecessor");
              trace.blocks[trace.block_count++] = block_best[block];
              remaining ^= block;
            }
            result.trace[policy] = trace;
          }
        }
      }
    }
    return result;
  }

  uint64_t training_n() const { return training_n_; }

 private:
  uint64_t training_n_;
  std::vector<double> ug_, uc_, uw_, um_;
  std::array<std::array<double, P>, K> logq_{};

  static constexpr double neg_inf() { return -std::numeric_limits<double>::infinity(); }

  static int popcount(int mask) {
    int count = 0;
    for (; mask; mask &= mask - 1) ++count;
    return count;
  }

  static ScoreBlock make_block(int modules, int form, int prime, int view,
                               std::array<uint8_t, 4> masks, double value) {
    ScoreBlock result;
    result.modules = modules;
    result.form = form;
    result.prime_index = prime;
    result.matrix_view = view;
    for (int j = 0; j < 4; ++j) if ((modules & (1 << j)) == 0) masks[j] = 0;
    result.positive_masks = masks;
    result.value = value;
    return result;
  }

  static void build_utilities(const std::vector<uint64_t>& counts, int groups,
                              int states, int code_cost, uint64_t n,
                              std::vector<double>& output) {
    if (counts.size() != static_cast<size_t>(groups * states))
      throw std::invalid_argument("Training histogram has the wrong shape");
    output.assign(counts.size(), neg_inf());
    for (int group = 0; group < groups; ++group) {
      const size_t offset = static_cast<size_t>(group) * states;
      uint64_t total = 0;
      for (int state = 0; state < states; ++state) total += counts[offset + state];
      if (total != n) throw std::invalid_argument("Training histogram group does not sum to n");
      for (int mask = 1; mask < states; ++mask) {
        uint64_t superset = 0;
        for (int state = 0; state < states; ++state)
          if ((state & mask) == mask) superset += counts[offset + state];
        const double probability = (static_cast<double>(superset) + 0.5) /
                                   (static_cast<double>(n) + 1.0);
        output[offset + mask] = -std::log2(probability) - code_cost;
      }
    }
  }
};

inline void write_scores_json(std::ostream& out, const Scores& scores) {
  const auto old_precision = out.precision();
  out << std::setprecision(17);
  out << "{\"training_n\":" << scores.training_n << ",\"policies\":[";
  for (int k = 0; k < K; ++k) {
    if (k) out << ',';
    const ScoreTrace& trace = scores.trace[k];
    out << "{\"policy\":" << k << ",\"name\":\"" << Scorer::kPolicyNames[k]
        << "\",\"value\":" << scores.value[k] << ",\"variant_index\":";
    if (trace.variant < 0) out << "null"; else out << trace.variant;
    out << ",\"active_mask\":" << trace.active
        << ",\"blocks_value\":" << trace.blocks_value
        << ",\"program_penalty\":" << trace.program_penalty << ",\"blocks\":[";
    for (int j = 0; j < trace.block_count; ++j) {
      if (j) out << ',';
      const ScoreBlock& block = trace.blocks[j];
      out << "{\"module_mask\":" << block.modules << ",\"form_index\":" << block.form
          << ",\"prime_index\":";
      if (block.prime_index < 0) out << "null"; else out << block.prime_index;
      out << ",\"prime\":";
      if (block.prime_index < 0) out << "null";
      else out << MatrixEngine::kPrimes[block.prime_index];
      out << ",\"matrix_view\":";
      if (block.matrix_view < 0) out << "null"; else out << block.matrix_view;
      out << ",\"positive_masks\":[";
      for (int module = 0; module < 4; ++module) {
        if (module) out << ',';
        out << static_cast<unsigned>(block.positive_masks[module]);
      }
      out << "],\"value\":" << block.value << '}';
    }
    out << "]}";
  }
  out << "]}";
  out.precision(old_precision);
}

}  // namespace study

#endif
