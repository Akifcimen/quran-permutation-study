#include "scorer.hpp"
#include <cstdint>
#include <iostream>

namespace {
void fill(std::vector<uint64_t>& histogram, int states, int salt, uint64_t n) {
  constexpr int scale[5] = {1, 10, 100, 1000, 2000};
  const int groups = static_cast<int>(histogram.size()) / states;
  for (int group = 0; group < groups; ++group) {
    uint64_t total = 0;
    for (int state = 1; state < states; ++state) {
      const uint64_t count = ((group * 17 + state * 13 + salt) % 31) * scale[group % 5];
      histogram[group * states + state] = count;
      total += count;
    }
    if (total > n) throw std::logic_error("Invalid synthetic fixture");
    histogram[group * states] = n - total;
  }
}

uint32_t state_hash(uint32_t seed, int variant, int index, int module, int view) {
  uint32_t x = seed + 104729u * (variant + 1) + 7919u * (index + 1) +
               1543u * (module + 1) + 32749u * (view + 1);
  x = (x ^ (x >> 16)) * 0x7feb352du;
  x = (x ^ (x >> 15)) * 0x846ca68bu;
  return x ^ (x >> 16);
}
}  // namespace

int main() {
  study::Hist hist;
  hist.n = hist.proposals = 1000000;
  fill(hist.g, 4, 3, hist.n);
  fill(hist.c, 16, 5, hist.n);
  fill(hist.w, 4, 7, hist.n);
  fill(hist.m, 16, 11, hist.n);
  study::Scorer scorer(hist);
  uint32_t seed;
  int allowed;
  while (std::cin >> seed >> allowed) {
    study::Features features{};
    for (int v = 0; v < study::V; ++v) {
      for (int f = 0; f < study::F; ++f)
        features.g[v][f] = (allowed & 1) ? state_hash(seed, v, f, 0, 0) & 3 : 0;
      for (int cx = 0; cx < study::C; ++cx) {
        features.c[v][cx] = (allowed & 2) ? state_hash(seed, v, cx, 1, 0) & 15 : 0;
        features.w[v][cx] = (allowed & 4) ? state_hash(seed, v, cx, 2, 0) & 3 : 0;
        if (allowed & 8)
          for (int view = 0; view < 8; ++view)
            features.m[v][cx] |= (state_hash(seed, v, cx, 3, view) & 15) << (4 * view);
      }
    }
    study::write_scores_json(std::cout, scorer(features));
    std::cout << '\n';
  }
}
