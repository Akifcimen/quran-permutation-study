# Auxiliary guide for the frozen primary score

## Purpose and interface

`rare_event/src/soft_guide.hpp` supplies a monotone envelope of the original
primary score, to guide rare-event simulation toward near misses. It does not
change the event whose probability is being estimated. The final event uses
`guide.hard(prepared) >= observed`, where `hard()` calls the unchanged original
`study::Scorer` and preserves its exact floating-point evaluation order.

```cpp
rare::SoftGuide guide(base129, histogram, matrix_engine);
rare::SoftPrepared prepared = guide.prepare(draw);
double s = guide.evaluate(prepared, lambda);
double t = guide.hard(prepared);
double finite_endpoint = guide.exact_hard_lambda();
```

The prepared structure caches the original hard features and all needed
distances, so multiple lambda values do not repeat feature extraction. The
guide retains all 14 forms, 29 primes, both count conventions, eight matrix
views, all module activation sets and all partitions, with the frozen primary
text mixture and every original code cost.

## Definition and guarantees

For each trained positive predicate mask, replace its utility `u` by
`u - lambda * d`, with `d` the sum of its selected component distances. All
distances are nonnegative. Zero means the original hard predicate is true;
the implementation uses exact zero comparisons, without a tolerance.

Every hard-valid program remains available with its original utility, so
`S_lambda >= T`. Every candidate program has nonincreasing utility as lambda
increases; maximizing over the same finite family preserves monotonicity.

All nonzero distances are at least one: the residuals below are integers and
use `log2(1 + residual)`, and the matrix distance is an integer missing-bit
count. Let `Gamma` be the largest retained positive module utility. Once
`lambda > Gamma`, all unsatisfied masks have negative utility. A nonpositive
module cannot improve the optimum: deleting it removes its utility, does not
increase any form or prime cost, and cannot increase the activation/partition
penalty. Therefore `S_lambda = T` at this finite endpoint, as well as at
infinity. `exact_hard_lambda()` returns the next representable double above
Gamma, conservatively avoiding an equality-rounding ambiguity.

For all three current training tables, that endpoint is
**16.609647687893986**. At lambda zero the guide is constant across states:
**44.093899963394634**.

## Component distances

Write `L(r) = log2(1+r)` for a nonnegative integer residual.

| Predicate | Distance |
|---|---|
| G count | `L(abs(even_count-57))` |
| G balance | `L(abs(even_n-(total-even_a)))` |
| C equal positive cells | `L(max(celln)-min(celln)+(min(celln)==0))` |
| C sum pattern | `L(minimum L1 distance to the frozen row/orientation patterns (b,b,b-1,b+1)))`; integer b is optimized by a median |
| C contiguous band with a repeated constant cell | `L(number of support holes + min_cell(range+max(0,2-celln)))`; empty support has one hole |
| C centre | `L(abs(sum(cell sums)-4*p))` |
| W disjoint union of p items | `L(abs(union_count-p)+overlap)` |
| W divisibility | `L(min(union_sum mod p,p-(union_sum mod p)))` |
| Each M bit | Zero if the original matrix bit is present, otherwise one |

The four C sum patterns exactly match the original hard predicate; the guide
does not add further patterns. Empty-union divisibility also retains the
original hard definition.

For speed, a positive mask is omitted if a proper subset has equal or greater
utility: the subset also has an equal or smaller penalty for every state and
lambda. The matrix envelopes precompute the best utility for each possible
number of missing bits, then maximize over the same eight views.

## Validation performed

The C++ test is `rare_event/tests/test_soft_guide.cpp`; its machine-readable
result is `rare_event/tests/soft_guide_validation.json`. It passed:

- 50,000 random-family checks, including an independent exhaustive integer
  search for the nearest C sum pattern.
- 14,641 exhaustive small-integer four-cell sum patterns, comparing the
  optimized integer formula with an independent search over integer centres.
- Explicit full-crystal, empty-family and gapped-support examples.
- 8,885,184 exact-zero equivalence checks against original G/C/W predicate bits.
- 4,442,592 original feature-mask comparisons.
- 5,409 bit-exact hard endpoint checks at the finite endpoint, lambda one
  million, and infinity.
- 23,439 upper-envelope checks and 23,439 monotonicity checks.

The permutation cases consist of 500 independent null draws plus 101 observed
or near-observed permutations for each of the three training tables. The full
form/prime/view optimizer is exercised in every case. Near-observed cases are
feature and optimizer checks; they are not asserted to belong to every null
support. There are no additional training or outcome claims from these tests.

Snapshots are recorded in `rare_event/tests/soft_guide_snapshot_MODEL.csv`.
Mean guide values over the 500 independent draws illustrate a usable bridge:

| Model | lambda 1 | lambda 2 | lambda 4 | lambda 8 |
|---|---:|---:|---:|---:|
| Uniform | 32.59875 | 21.25793 | 8.52438 | 0.00974 |
| Block6 | 32.65258 | 21.60145 | 9.98146 | 0.00868 |
| Exact inversion | 32.92441 | 21.83035 | 8.62008 | 0.02738 |

## Use in an estimator

A fixed sequence of nested sets `{S_lambda >= observed}` ends at the frozen
event because the guide equals the hard score at the finite endpoint. An
indicator SMC product then targets the event probability when mutation steps
preserve the corresponding restricted null distributions. Alternatively,
soft-density SMC must retain the explicit final event/target density
correction. Pilot-adapted levels must be frozen before independent production
runs unless a separate valid adaptive estimator is used.

The envelope proof and tests establish the guide's definition and endpoint.
They do not establish particle diversity, useful mixing, or the precision of
any rarity estimate.

## Equivalent performance optimization

The production header replaces the four median searches for the C pattern
distance with the exact integer expression

```text
max(abs(a+b-c-d), abs(abs(a+c-b-d)-2), abs(abs(a+d-b-c)-2)).
```

For four integers, the minimum sum of absolute distances to a common centre
equals the largest absolute balanced two-versus-two contrast. The four
permitted pattern orientations leave the row contrast unchanged and shift
the two diagonal contrasts independently by either +2 or −2. Minimizing over
these four orientations gives the expression above. This changes only the
arithmetic used to evaluate the same distance.

The second optimization caches `log2(1+r)` for integer r between zero and
32,768, using exactly the same floating-point expression as before. Larger
residuals retain the original direct calculation. Neither optimization changes
the guide definition, endpoint, statistic, or proposal kernel.

The exact pilot header is preserved in
`rare_event/src/soft_guide_pilot_baseline.hpp`. A separately namespaced copy in
`rare_event/tests/soft_guide_reference.hpp` permits side-by-side comparison.
The benchmark checks all prepared fields and guide values bit for bit on
768 independent-null and final-pilot states before measuring their costs.
The measured overall candidate-evaluation speedup is approximately 1.27–1.43
across the six cohorts. Machine-readable timings are in
`rare_event/tests/soft_guide_fast_benchmark.json`; exact timings depend on CPU
load. The unchanged original six-policy scorer remains responsible for the
final event.

## Reproduction

```bash
g++ -O3 -std=c++17 -march=native -Wall -Wextra -Wpedantic rare_event/tests/test_soft_guide.cpp -o rare_event/tests/test_soft_guide
./rare_event/tests/test_soft_guide /workspace/scratch/6c9672781e7f/joint_study
```

Header SHA-256:
`3fbe927f0e97af5d1152ae570313185dc78be8e6f4bf6662598bc69fd906da36`.

Preserved pilot header SHA-256:
`c0b78adf3925909301845fc81928dc9f3e5155b22a0c71f32c4ffecb018ca0a9`.

Test source SHA-256:
`a2c863cdbaeee4fb59fcfeaff032c329a47fcaa4b9cdf217b6f2d308b0326446`.
