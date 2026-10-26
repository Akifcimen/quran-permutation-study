# Stratified resampling validation

`src/smc_math_stratified.hpp` is an API-compatible alternative to the original multinomial helper. It evaluates weight callbacks once in input order, retains the same stable log-mean, ESS, and maximum-weight calculations, and draws one independent uniform variable in each CDF stratum. Include exactly one of the two headers.

For normalized weight w_j, the expected number of selected ancestors j is the sum of N stratum intersection lengths, namely N*w_j. This conditional expected-offspring property preserves the SMC normalizer induction. Independence of all offspring is not required. Equal weights select each input particle exactly once, avoiding genealogy loss at uninformative stages. A floating-point upper-endpoint guard keeps each draw in its own half-open stratum.

Independent checks passed:

- 14 equal-weight cases, sizes1,2,3,7,64,256,513 and common log shifts0 and-1000, including an RNG returning the largest representable grid value below one.
- 7 expected-offspring checks over200000 repetitions with unequal weights and two zero-weight particles.
- 18 two-parameter SMC checks, 8192 replicates each in one-mode/two-mode finite spaces and with0/1/4 mutation attempts. Exact soft normalizers, primary tails, and secondary inverse-density corrections all matched within2.453409 empirical standard errors. All zero runs were retained.

Raw outputs: `stratified_resampling_validation.json` and `smc_guided_stratified_validation.json`. Test sources: `tests/test_stratified_resampling.cpp`, and `tests/test_smc_guided_core.cpp` compiled with `-DRARE_USE_STRATIFIED`.

Validated header SHA256: `c9b0adf84714456ddff2f5b654163c683a1fedfa35133800290be671d13b43f8`.

Stratification reduces avoidable resampling variation; it does not establish adequate mutation, discovery of every rare mode, or precision of a particular run.
