# Independent review of the fixed SMC production

## Reviewer and scope

Run the independent review with:

```bash
python rare_event/review/production_review.py
```

The script reads the registered plan, completed replicate JSON files and saved
particle binaries. A file still being written is marked pending. Pending
replicates are never substituted with zero estimates. Every completed zero
estimate remains in the arithmetic mean and variance calculation.

Use `--pilot` to exercise the same reviewer on the six completed guided pilots.
Those pilot estimates are excluded from production summaries.

## Checks of saved states

The separate C++ checker in `review_saved_states.cpp` uses the unchanged
original `Extractor` and `Scorer`, without the auxiliary guide. For every saved
particle it checks:

- The identities form a valid permutation of all 114 items.
- In the block model, every identity remains in its original six-item block.
- In the inversion model, direct pair enumeration gives exactly 993 and 994
  under the respective count conventions.
- All six stored scores agree bit for bit with the original scorer.
- The four natural-event flags are rebuilt from the original feature masks.

The checker also records exact counts of distinct saved permutations and
primary programs. These counts are compared with the final replicate log,
along with score minima, means, maxima, and target-hit counts.

## Estimator reconstruction

The final registered lambda is the exact-hard endpoint, so the final target
has unnormalized density

\[
\gamma(x)=\exp[-\beta\max(0,t-T(x))].
\]

For every policy event and every natural event, the reviewer reconstructs

\[
\widehat Z\,\frac1N\sum_i\frac{\mathbf1_E(X_i)}{\gamma(X_i)}.
\]

Log-sum-exp arithmetic avoids numerical underflow when the normalizer and
inverse target weights have very different sizes. The primary event has
gamma equal to one on every hit, reproducing the production expression
`exp(log_z) * hits / N`. Other policies and natural events retain the inverse
gamma factor. Raw counts among tilted final particles are not reported as
their null probabilities.

Each of the six policies receives its own replicate summary. The primary
guide can leave secondary-event regions poorly explored; formal correctness
of the weighting does not establish small variance or mode coverage.

## Replicate uncertainty and reporting gates

For completed replicate estimates `p_1,...,p_R`, including zeros, the reviewer
reports the arithmetic mean and the standard error

\[
\operatorname{SE}(\bar p)=
\sqrt{\frac{\sum_r(p_r-\bar p)^2}{R(R-1)}}.
\]

It also reports the relative standard error, the largest replicate's share
of the sum, and the number of nonzero replicates. All-zero results have
undefined relative standard error and fail the precision gate. A partial
collection cannot pass the complete-production gate.

The registered primary gates require all eight planned replicates, at least
six nonzero estimates, relative standard error at most 0.35, and maximum
single-replicate share at most 0.35. Passing these empirical diagnostics does
not prove that every rare mode was sampled. The same numerical diagnostics
are displayed for secondary policies, with their distinct values preserved.
No binomial confidence interval is constructed from dependent SMC particles.

## Source integrity

Every source hash explicitly recorded in the production plan is checked
before and after review. Additional current hashes are recorded for counts,
training tables and transitive headers omitted from the plan's hash list.
Those supplementary hashes are identified as audit-time snapshots, not
prelaunch attestations. State-file hashes and reviewer dependency hashes allow
unchanged reviews to be cached without silently accepting modified inputs.

## RNG stream audit

`review_seed_streams.py` independently reconstructs the seed derivation for:

- The original 48 training and testing worker streams.
- The 32 conditional pilot and production streams with recorded or exactly
  replayed call counts.
- The hard-score and guided SMC pilots.
- The four streams in each of the 24 new production replicates: initial
  sampling, local proposals, independent refreshes, and resampling/MH draws.

The audit maps seeds to positions on the SplitMix64 counter cycle, checks
distinct starts, and computes exact circular separations. All 32 conditional
final states are reconciled with their recorded call counts.

The other programs did not record final RNG states or exact call counts.
Their start-state separation is exact, but most consumption windows use
explicit conservative caps: eight attempts per bounded integer draw and
1,024 attempts per selected-position retry. Under ideal independent uniform
words, the corresponding union bounds are extremely small; these assumptions
are stated explicitly. Fixed PRNG streams are deterministic, so these bounds
are not described as an exact nonoverlap proof or a proof of mathematical
independence. Completed proposal totals tighten the bounds for rejection
samplers; unfinished replicates remain pending.

## Reviewer validation

The guided-pilot validation checked all six replicates, 1,536 saved
permutations and 9,216 score values. Every saved score and required null
constraint matched. All policy estimates reconstructed correctly.

`rare_event/tests/test_production_review.py` separately tests five exact
replicate-summary examples, including all-zero results, dominance by one
replicate, and incomplete output. Three inverse-gamma examples exercise
nontrivial weights and underflow-resistant evaluation. Four deliberately
invalid binary inputs were rejected: duplicate identity, crossing a block
boundary, changed saved score and truncated record.

The machine-readable validation outputs are
`pilot_production_reviewer_validation.json` and
`rare_event/tests/production_reviewer_validation.json`. The completed
production audit is recorded in `production_review.json`.

## Completed production conclusions

All 24 planned replicates were reviewed. The checker verified 12,288 saved
permutations and compared 73,728 score values bit for bit with the unchanged
original scorer. All required null constraints, all inverse-gamma estimates,
all published aggregate means and all eight registered source hashes passed.
The inversion model's 4,096 saved states had exactly 993 and 994 inversions
under the respective count conventions.

| Model | Primary mean | Standard error | Relative standard error | Largest replicate share | Nonzero replicates | Precision gate |
|---|---:|---:|---:|---:|---:|---|
| Uniform | 1.0390173543897488e-12 | 5.289200914876519e-13 | 0.509058 | 0.548433 | 8/8 | Failed |
| Block6 | 1.5521569811888627e-12 | 6.049590245003466e-13 | 0.389754 | 0.420290 | 8/8 | Failed |
| Inversion exact | 3.438405867121971e-13 | 2.233805988255121e-13 | 0.649663 | 0.677404 | 7/8 | Failed |

Every secondary policy also failed the same numerical precision checks. All
natural-event and conditional-19 SMC estimates were zero in these saved
samples. Those zeros do not establish zero null probability and must not
replace the separate conditional estimators.

The smallest circular separation between the 224 audited RNG start states
was 78,985,953,964,597 words. All 32 recorded conditional windows reconciled
exactly. The remaining windows passed the documented conservative caps;
their interpretation remains conditional because the SMC programs did not
record exact final call totals. This is not a proof of independence of fixed
PRNG streams.

The computational output is internally consistent, but all full-tail SMC
point estimates remain exploratory. Passing the state and source audit does
not override the failed precision gates or prove coverage of rare modes.
