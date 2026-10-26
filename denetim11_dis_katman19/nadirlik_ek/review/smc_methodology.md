# Rare-event estimator: methodology and independent audit

This extension estimates the probability of the **existing frozen primary score tail** in each existing null model separately. It does not change the score, training tables, count variants, semantic weighting, or historical scope of the original retrospective analysis.

## Recommended estimator

Write `t` for the observed primary score, `T(x)` for the entire frozen search, and `d(x)=max(0,t-T(x))`. The exact event is `E={d=0}`. For a deterministic temperature schedule `0=beta[0]<...<beta[L]`, use

`gamma[l](x)=exp(-beta[l]*d(x))`, `pi[l](x) proportional to pi[0](x)*gamma[l](x)`.

Initialize particles independently with the validated exact base-null sampler. At each stage, compute incremental weights `exp(-(beta[l]-beta[l-1])*d)`, multiply the mean weight into the normalizer estimate, resample multinomially, and apply a fixed number of Metropolis attempts invariant for `pi[l]`. The terminal contribution is the fraction satisfying `d=0`: on this event the last soft weight is exactly one. Thus

`p_hat = product(mean incremental weights) * mean(1_E)`.

The resulting estimator is nonnegative and at most one. Each entire run is one replicate. Average these full-run estimates arithmetically, including all zero runs.

## Why finite mixing does not by itself introduce normalizer bias

Let `Z_hat[l]` be the estimated normalizer and `eta_N[l]` the empirical distribution after resampling and mutation. An induction gives

`E[Z_hat[l] eta_N[l](f)] = pi[0](gamma[l] f)`.

Conditional expectation over unbiased resampling and mutation changes the left side to `E[Z_hat[l-1] eta_N[l-1](G[l] M[l] f)]`. The induction hypothesis changes this into `pi[0](gamma[l] M[l] f)`, which equals `pi[0](gamma[l] f)` because `M[l]` preserves the normalized current target. Applying the final factor `1_E/gamma[L]=1_E` gives `E[p_hat]=pi[0](E)`.

This argument needs fixed kernels and temperatures, or additional valid adaptive-SMC theory. Choosing temperatures and move counts in an **independent pilot**, then freezing them before fresh production replicates, is the uncomplicated option. A kernel being invariant for each population-dependent parameter value does not, alone, justify arbitrary adaptation using the same particles.

No claim follows that one realized run is accurate. Identity mutation is an extreme example: the estimator remains unbiased, but rare initial hits can dominate its mean. Short or disconnected chains can similarly hide high-variance modes. Formal unbiasedness is not a precision certificate.

Primary methodological sources: [Del Moral, Doucet and Jasra, 2006, *Sequential Monte Carlo samplers*](https://www.stats.ox.ac.uk/~doucet/delmoral_doucet_jasra_sequentialmontecarlosamplersJRSSB.pdf); [Dau and Chopin, 2022, *Waste-Free Sequential Monte Carlo*, especially the Feynman–Kac formulation and Proposition 2](https://academic.oup.com/jrsssb/article/84/1/114/7056097). The present implementation uses standard resample–move SMC, not the waste-free variant.

## Kernel requirements and inversion-fiber repair

The state is a permutation of **labeled chapter identities**; the same permutation defines both count variants. Local proposals must be symmetric on the appropriate null support, or include the full proposal-density ratio. Null constraints are checked after a whole compound proposal, not after each intermediate transposition. Each rejected proposal is a self-transition; do not retry until a move is accepted.

A single swap of unequal numerical values always changes the strict inversion count. Therefore a single-swap kernel restricted to the exact inversion fiber only rearranges numerical ties. Compound swaps or another valid fiber kernel are necessary for numeric movement.

A useful exact repair kernel is: choose a uniform transposition `u`, pass through an unrestricted intermediate state `z=u*x`, then choose uniformly among transpositions `v` returning `z` to both required inversion counts. The undo move always exists. The forward and reverse paths pass through the same `z` and each have probability `1/M/|A(z)|`, so the proposal is symmetric. Excluding the undo move when alternatives exist remains symmetric, with denominator `|A(z)|-1`. Prefix rank counts allow all second-swap deltas to be enumerated in quadratic time. An exact-null independent-refresh component supplies full support, although its high-temperature acceptance can be negligible.

## Production diagnostics and uncertainty

Record independent-replicate estimates, zero-run fraction, maximum replicate contribution to the total, relative standard error, stage ESS, maximum weight, ancestor counts, distinct permutations, feature-changing acceptance, aggregate changes, and winning-program changes. Accepted label-only or irrelevant swaps do not establish exploration of the rare constraints.

Validate low-threshold probabilities against independent ordinary Monte Carlo; increase particle count and mutation length in an independently specified comparison; compare outcomes from independent initial populations. An observed-seeded run is useful only as a mixing diagnostic. It is not a valid probability estimator from the base null.

An approximate confidence interval can use variation of independent full-run estimates when their distribution is sufficiently stable. It is not a binomial interval based on terminal particle hits: those particles are dependent and drawn from a tilted distribution. Ordinary replicate bootstrap or normal approximations can still be misleading if a rare mode dominates variance but has not been observed. Report instability instead of presenting a resolved point estimate in that case.

The six-policy terminal correction `Z_hat*mean(1_{E_k}*exp(beta[L]*d))` is algebraically valid. It may have much larger variance than the primary event if that policy's events have low primary scores. Equal precision across policies does not follow. Unweighted natural-template counts among terminal particles are tilted-distribution diagnostics, not unconditional probabilities.

If a natural-template event `C` has an exact probability and conditional sampler, its contribution `P(C)*P(E|C)` can be estimated separately. It equals the whole tail only after proving `E` is contained in `C`. Otherwise the complementary contribution must also be estimated. Restricting to familiar 19 templates without this proof is not an estimate of the complete frozen search.

## Implementation review and regression check

The reviewed `smc.cpp` and `smc_math.hpp` use the correct soft incremental weights, multinomial resampling, fixed attempted Metropolis steps, terminal event fraction, and arithmetic averaging including zero runs. Initialization is iid unless explicitly labeled `observed_diagnostic`. The paired count variants are preserved by the mutation kernel's numeric-change check.

`tests/test_smc_core.cpp` uses the actual production resampling core on a known uniform 1024-state bit space, with one-mode and two-mode events. It tests zero, one, and four mutation attempts per temperature, 64 particles, nine temperatures, and 8192 independent replicates per configuration. All **18** checks passed: soft normalizers, primary-event probabilities, and secondary importance-weighted events. The largest absolute deviation from an exact value was **2.014682 empirical standard errors**. Identity-kernel cases retained thousands of zero runs and correctly exhibited much greater variance. Raw results are in `review/smc_core_validation.json`.

This is a correctness check of estimator mechanics. It does not certify mixing, precision, or mode coverage in the 114-chapter problem.

Reviewed source SHA256 values:

- `smc.cpp`: `4ec278b7f422fbdec23e202cbbbadabd4f2c44aa6d9a1f6a6417693cab02d69a`.
- `smc_math.hpp`: `e0a6f2eac083d660c2e9724ea2e0ad13217bd3f2f32013fa0a44bba3e87e2b66`.
- `test_smc_core.cpp`: `e5f4c7baf88746bb8e86f3bbbc2144aa7430d2aaf98f1d18505ff32adeb27ef0`.

The metadata name `is_production_estimator` currently means valid iid initialization, so it is also true in an iid pilot. A separate pilot/production phase label is advisable before interpreting files. Reproducibility validation can be run with `python rare_event/tests/test_smc_core.py` from the study directory.
