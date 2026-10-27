/* Independent uniform-null check of two components of the Quran's information score at lens (1,+1,19):
   P(B1 & B2) for form (1,+1), P(k_F = 12), and P(W1 & QU & Qa | k_F = 12). Own RNG and Fisher-Yates; own predicate code. */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "k129.h"
static uint64_t s[4];
static inline uint64_t rotl(uint64_t x, int k) { return (x << k) | (x >> (64 - k)); }
static inline uint64_t nx(void) { uint64_t r = rotl(s[1] * 5, 7) * 9, t = s[1] << 17; s[2] ^= s[0]; s[3] ^= s[1]; s[1] ^= s[2]; s[0] ^= s[3]; s[2] ^= t; s[3] = rotl(s[3], 45); return r; }
static inline uint32_t below(uint32_t n) { return (uint32_t)(((nx() >> 32) * (uint64_t)n) >> 32); }
int main(int argc, char **argv) {
    long N = atol(argv[1]); uint64_t seed = strtoull(argv[2], 0, 10);
    for (int i = 0; i < 4; i++) { seed += 0x9E3779B97F4A7C15ULL; uint64_t z = seed; z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL; z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL; s[i] = z ^ (z >> 31); }
    int a[114]; long nB = 0, nB1 = 0, nK12 = 0, nW = 0, nWW1 = 0, nWQU = 0;
    for (int i = 0; i < 114; i++) a[i] = K129[i];
    for (long it = 0; it < N; it++) {
        for (int i = 113; i > 0; i--) { int j = below(i + 1); int x = a[i]; a[i] = a[j]; a[j] = x; }
        int e0 = 0; long n0 = 0, n1 = 0, a0 = 0, a1 = 0, t0 = 0, t1 = 0; int kF = 0, kH = 0, both = 0, U = 0; long sa = 0;
        for (int i = 0; i < 114; i++) {
            int n = i + 1, t = n + a[i], u = n - a[i];
            if ((t & 1) == 0) { e0++; n0 += n; a0 += a[i]; t0 += t; } else { n1 += n; a1 += a[i]; t1 += t; }
            int f = (t % 19 == 0), h = (u % 19 == 0);
            kF += f; kH += h; both += f && h; if (f || h) { U++; sa += a[i]; }
        }
        int B1 = (e0 == 57), B2 = (e0 > 0 && e0 < 114) && (n0 == a1 || n1 == a0 || t0 == t1);
        nB1 += B1; nB += B1 && B2;
        if (kF == 12) { nK12++; int W1 = kH >= 1 && both == 0, QU = (U == 19), Qa = (U >= 1 && sa % 19 == 0);
                        nWW1 += W1; nWQU += W1 && QU; nW += W1 && QU && Qa; }
    }
    printf("N %ld  B1 %ld  B1&B2 %ld  kF=12 %ld  W1|k12 %ld  W1&QU|k12 %ld  W1&QU&Qa|k12 %ld\n", N, nB1, nB, nK12, nWW1, nWQU, nW);
    return 0;
}
