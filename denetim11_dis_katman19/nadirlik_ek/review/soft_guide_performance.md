# Soft-guide performance assessment

The optimized guide retains bit-exact values relative to the pilot guide.
Its speedup comes from an exact integer simplification of the C pattern
distance and a lookup table for the existing log distances. No feature,
score, distance, lambda endpoint, proposal, or random-number decision changed.

## Benchmark

`rare_event/tests/benchmark_soft_guide.cpp` compares the pre-optimization
reference with the production header. Each of the three models uses two
128-state cohorts: fresh independent null draws and saved final states from
guided pilot replicate zero. Every prepared field is checked exactly, and
every state is compared at lambda 0, 1, 2, 4, 8 and 32. The 768-state comparison
passed. Timings below are process CPU microseconds per preparation, averaged
over repeated evaluations. Process CPU timing reduces distortion from waiting
for other processes; frequency and cache variation remain possible.

| Model | Cohort | Previous prepare | Optimized prepare | Estimated total candidate speedup |
|---|---|---:|---:|---:|
| Uniform | IID | 318.58 µs | 192.51 µs | 1.42× |
| Uniform | Guided final | 296.80 µs | 217.63 µs | 1.27× |
| Block6 | IID | 282.05 µs | 167.26 µs | 1.43× |
| Block6 | Guided final | 241.93 µs | 157.35 µs | 1.38× |
| Exact inversion | IID | 308.29 µs | 223.45 µs | 1.29× |
| Exact inversion | Guided final | 226.53 µs | 146.20 µs | 1.38× |

The total candidate estimate includes preparation, the original six-policy
scorer and guide evaluation at lambda 2. It does not include proposal
generation, resampling, diagnostics or file output; therefore it should not
be read as an exact wall-time speedup for an entire SMC run. Earlier and later
lambda values also have different guide-evaluation costs.

## Secondary option considered

Before the final lambda, the MH ratio needs the guide score but does not need
the original six-policy score. Scoring only accepted candidates would preserve
the transition law if diagnostic bookkeeping were deferred consistently. In
the initial profile, however, the original scorer took approximately 19–59 µs
while preparation took 264–378 µs, and candidate acceptance was approximately
one half. The projected saving was only a few percent of total evaluation
cost. That optimization was not applied, preserving the simple existing
scoring and diagnostic path.

The mutation kernels were also left unchanged. Their mathematical symmetry
and validation remain those of the stable kernel report. Improvements in
guide arithmetic should not be mistaken for evidence of sufficient mixing
within rare modes; production diagnostics remain necessary.

## Reproduction and source preservation

```bash
g++ -O3 -std=c++17 -march=native -Wall -Wextra -Wpedantic rare_event/tests/benchmark_soft_guide.cpp -o rare_event/tests/benchmark_soft_guide
./rare_event/tests/benchmark_soft_guide /workspace/scratch/6c9672781e7f/joint_study
```

The exact prior header is preserved at
`rare_event/src/soft_guide_pilot_baseline.hpp`, SHA-256
`c0b78adf3925909301845fc81928dc9f3e5155b22a0c71f32c4ffecb018ca0a9`.

The optimized header is `rare_event/src/soft_guide.hpp`, SHA-256
`3fbe927f0e97af5d1152ae570313185dc78be8e6f4bf6662598bc69fd906da36`.

The full validation was rerun after optimization, including 14,641 exhaustive
four-cell integer patterns, the random-family tests, all original-feature
comparisons, hard endpoints, monotonicity and upper-envelope checks. Results
are recorded in `rare_event/tests/soft_guide_validation.json` and timings in
`rare_event/tests/soft_guide_fast_benchmark.json`.
