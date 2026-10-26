# Independent audit of guided SMC integration

## Verdict

The reviewed integration computes the intended Feynman–Kac estimator correctly. No mathematical integration defect was found. This verdict concerns estimator correctness, not adequate exploration of the actual high-dimensional rare tail.

The auxiliary guide satisfies `S_lambda(x)>=T(x)` and is nonincreasing in lambda. Each scalar distance is `log2(1+r)` with a nonnegative integer residual; its zero set is exactly the corresponding frozen predicate and every nonzero distance is at least one. Matrix masks use the integer number of missing requested bits. Equal-utility superset-mask pruning is safe because distances are nonnegative. Removing nonpositive module utilities is safe under the existing program penalties. At a lambda conservatively above the largest positive utility, all unsatisfied masks have nonpositive utility. The implementation uses the original hard scorer at that endpoint.

The density at a schedule point is

`gamma(lambda,beta;x)=exp(-beta*max(0,t-S_lambda(x)))`.

The initial `(lambda,beta)=(0,0)` has gamma identically one. Each update uses the **full log-density difference**, not just a beta increment. This is necessary when lambda also changes.

## Source-level checks

- The next guide score is computed from immutable cached prepared features.
- `next_gamma - p.log_gamma` is saved before overwriting the old cached log density.
- The resampling helper evaluates its callback exactly once for each particle in order, so the parallel increment array is correctly aligned with particles before resampling.
- Resampled particles inherit both the new log density and their matching prepared state.
- A proposal with unchanged numerical values in **both count variants** may safely keep prepared features, hard scores, guide score, and log density. Only the labeled permutation and its hash need updating.
- Every numerically changed proposal is evaluated at the current lambda and beta. Its Metropolis log ratio is `candidate.log_gamma-current.log_gamma` for the separately validated symmetric null-preserving kernel.
- The fixed attempted-step loop retains rejected moves as self-transitions.
- Primary terminal hits use the unchanged hard score. Since every primary hit has gamma=1, `exp(logZ)*hit_fraction` is the correct terminal estimate.
- Other policies use `exp(logZ)*mean(I_policy/ gamma)`, implemented as `exp(-p.log_gamma)` inside the terminal sum.
- All replicate estimates, including zeros, enter the arithmetic mean.

The same caveats as the first SMC review remain: observed-seeded diagnostics cannot estimate the base-null probability; pilots must be separated from production tuning; high-variance secondary policies may not be reliably estimated from a primary-targeted population; unweighted natural-template hit counts are diagnostic only.

## Independent finite-state regression

`tests/test_smc_guided_core.cpp` exercises the **same production reweighting/resampling helper** with a nonlinear two-parameter guide on a uniform 1024-state space. The toy guide has competing near-miss branches and reaches the known hard event at a finite lambda. Both lambda and beta change in nine stages. This detects using only a beta difference, overwriting the old density before calculating the increment, or using the wrong terminal correction.

One-mode and two-mode events were tested with zero, one, and four Metropolis attempts per stage; 64 particles and 8192 independent replicates per configuration. Exact normalizers and exact event probabilities were obtained by enumeration. All **18 checks passed** for the soft normalizer, primary event, and an importance-weighted secondary event. The largest absolute discrepancy was **1.612456 empirical standard errors**. Zero estimates were retained, including 7939/8192 and 7752/8192 zero runs in the two identity-kernel cases. Raw results are in `smc_guided_core_validation.json`.

## Reviewed hashes

- `src/smc_guided.cpp`: `e26b2f28c16e9dbaf8ddc89ea80946f78e9ecd45174814af596bdce90ff070e8`.
- `src/soft_guide.hpp`: `c0b78adf3925909301845fc81928dc9f3e5155b22a0c71f32c4ffecb018ca0a9`.
- `tests/test_smc_guided_core.cpp`: `a7dab862b109c3a0626c6180f200736c5857cfeb29e2b852133eb1a40055b247`.

## Exact conditional proposal option

If a block event B has an exact mass b and a validated sampler from the base null conditioned on B, a globally supported independence proposal can mix the base null and that conditional law:

`q(x)=pi0(x)*[(1-eta)+eta*I_B(x)/b]`.

At a fixed SMC density gamma, its Metropolis ratio is

`gamma(y)/gamma(x) * [(1-eta)+eta*I_B(x)/b] / [(1-eta)+eta*I_B(y)/b]`.

This can introduce the known natural-template region without changing the global target. The entire mixture density must enter the ratio. Injecting conditional samples with only the ordinary symmetric-proposal ratio would be biased. Other reversible local kernels can remain separate mixture branches. This proposal is a recommendation, not part of the reviewed implementation above.
