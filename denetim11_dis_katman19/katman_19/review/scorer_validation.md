# Independent scorer validation

Validated with artificial histograms and feature masks before inspecting any new study simulation outcomes.

## Implemented score

`src/scorer.hpp` implements the frozen four-module surrogate. The 12 positive predicates are grouped into G (2), C (4), W (2), and M (4). Each conjunction probability is estimated from the independent training histogram by `(superset_count+0.5)/(n+1)`. A module with an empty true mask cannot be activated. All nonempty predicate masks in a module have the same declared code cost, so the all-true mask attains the maximum utility among its eligible submasks.

The scorer includes all nonempty activation subsets, every set partition of the activated modules, both global count variants, all 14 forms, all 29 prime values permitted by a policy, and eight canonical matrix views. Each partition block shares one form and prime, except a G-only block does not introduce a prime. The six text policies match the frozen uniform, equal mixture, literal-value, discussion-value, conditional-19, and literal-frequency definitions.

The search is exact within this finite family. Two efficiency changes do not alter the optimum:

1. A block is first maximized over forms at each fixed prime; textual prime weights are applied afterward. Form ranking at a fixed prime is independent of those weights.
2. Nonpositive-utility modules are excluded from candidate blocks. Deleting such a module cannot worsen binding costs, cannot increase the Bell-number partition penalty, and cannot reduce the optimum when the empty baseline has score zero.

The partition dynamic program fixes the least set bit in one block, enumerating each partition without depending on an ordering of blocks. It is compared against direct enumeration in the independent reference.

## Code weights and their limit

Activation uses 4 ideal code bits, variant 1 bit, each form `log2(14)`, each prime `-log2(q(p))`, each matrix view 3 bits, and a partition `log2(Bell(number_of_active_modules))`. Positive-predicate masks use 2, 4, 2, and 4 bits for G, C, W, and M respectively.

The ideal weights sum to 0.6560211181640625 over the declared nonempty program family, hence satisfy the Kraft weight bound. This is a check on the finite coding scheme, **not** a proof that `2^(-score)` is a p value. The sum of fitted module surprisals is a predictive surrogate; modules remain dependent. The independent full-search Monte Carlo ranking supplies the inferential p value.

Real-valued negative log weights are ideal code lengths. They need not be integer lengths of a literal binary source-code file.

## Independent checks actually run

Command: `python joint_study/src/test_scorer_kernel.py`.

- 114 policy scores, from 19 artificial feature cases and all six policies, matched independent exhaustive form/prime/partition enumeration.
- Every one of the 16 activation masks was exercised.
- Winners with zero, one, two, three, and four blocks occurred.
- All 108 nonzero winning traces were reconstructed from independently calculated module utilities and code penalties.
- The Python reference retained negative utilities, independently checking their pruning by the C++ implementation.
- The Python reference enumerated set partitions directly rather than using the production dynamic program.
- All policy weight vectors summed to one; the full ideal code mass was at most one.
- Compilation used C++17 with `-O2 -Wall -Wextra -pedantic` and emitted no compiler warning.

The artificial training counts deliberately included rare, common, and zero-count conjunctions. Neither the observed corpus nor any study null outcomes were used by this test.

## Output API

`Scorer(Hist)` checks that each histogram group has the declared shape and sums to `Hist.n`. The call operator returns `Scores.value[6]`, `Scores.trace[6]`, and the training sample count. Each trace records the global variant index, active modules, partition blocks, form and prime choices, matrix view, selected positive masks, and the score reconstruction. `write_scores_json` emits the complete structure.

The variant index follows `protocol.json`: zero is the 129 count convention, one is the 127 convention.

## File hashes at validation

| File | SHA-256 |
|---|---|
| src/scorer.hpp | ae07964b5e6a22a6f3bb032bb689863adc04cb96626a985c29928486a83d8929 |
| src/scorer_driver.cpp | 1c18c03333e4615aa9d4e303803675ab78e903670666ac7c05a83f7de004a68d |
| src/test_scorer_kernel.py | 24ac29a2f737093d08e89af32cd5eaa4688ddbafd2abd7bef94a3d6d60b2a32d |

This note validates the scorer and finite search. It does not certify that the complete study sampling/output pipeline has passed every review item; that requires the separate pretest code audit.
