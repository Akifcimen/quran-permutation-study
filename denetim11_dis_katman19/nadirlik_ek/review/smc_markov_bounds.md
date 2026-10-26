# Conservative probability bounds from the fixed SMC production

## Scope and verdict

The fixed production estimator is nonnegative and unbiased for the frozen
primary-score tail under the usual ideal-random, exact-arithmetic SMC model.
This property permits a conservative lower confidence bound even though
all three empirical precision gates failed. It does not turn the replicate
means into precise point estimates or establish exploration of every mode.

This bound is a follow-up analytical consequence of the already fixed
production. It was not listed in the original reporting plan. No sampling
budget, target, schedule, output selection, or stopping rule was changed.

## Estimator proof

Let the original null be the probability measure π₀. The fixed schedule has
positive unnormalized densities γₜ and γ₀=1. Define

\[
\mu_t(f)=\int\pi_0(dx)\,\gamma_t(x)f(x),\qquad
G_t(x)=\gamma_t(x)/\gamma_{t-1}(x).
\]

The algorithm starts with independent draws from π₀ and Ẑ₀=1. At a stage it
multiplies Ẑ by the empirical average of Gₜ, resamples with conditional
expected offspring N times normalized weights, and applies a transition Kₜ
invariant for the normalized measure proportional to π₀γₜ. For a test function
f, the conditional expectation after resampling and mutation is therefore

\[
\widehat Z_{t-1}\,\eta^N_{t-1}(G_tK_tf).
\]

By induction, its unconditional expectation is

\[
\mu_{t-1}(G_tK_tf)=\mu_t(K_tf)=\mu_t(f).
\]

The final test function is f=1_E/γ_final. Thus the expectation of the final
unnormalized particle estimator is π₀(E)=p. The production primary event is
E={T≥T_observed}; at the exact-hard endpoint γ_final=1 throughout E. The code's
primary expression Ẑ×hits/N is exactly the corresponding expression in this
algorithm. Averaging all eight fixed replicates, including zero results,
preserves nonnegativity and expectation p. Replicate independence is not
needed for this expectation argument or for Markov's inequality.

The source-level requirements are satisfied:

- The registered initialization is independent sampling from the original
  null, with unnormalized starting density one.
- The 26-stage schedule is fixed before production and uses the full
  difference of log densities, including changes to both λ and β.
- The stratified resampler has conditionally unbiased offspring counts.
- The separately audited proposal kernels are reversible for the null. The
  Metropolis acceptance ratio uses γ(new)/γ(old), giving an invariant Kₜ.
- Every stage uses a fixed number of attempted transitions; rejection is a
  self-transition. Numerical equality shortcuts preserve both variants'
  features and the same density.
- The final score is the unchanged original scorer. Secondary events retain
  the inverse-γ correction; the primary event has γ=1 on every hit.

These are the same production sources whose eight registered hashes and
all 12,288 saved states passed `production_review.json`.

## Markov and Bonferroni construction

For any nonnegative estimate P̂ with E[P̂]=p and a fixed α∈(0,1),

\[
\Pr\{\alpha\widehat P>p\}\le\alpha.
\]

Accordingly L=0.025 P̂ is a one-sided 97.5% lower confidence bound for p.
This statement does not require a finite variance, a successful precision
gate, a normal approximation, or independence among SMC particles.

The separate original test saw zero hits in N₀=1,000,000 independent null
draws. Its one-sided 97.5% exact binomial upper endpoint is

\[
U=1-0.025^{1/N_0}=3.688872650206489\times10^{-6}.
\]

The pair [L,U] has coverage at least 95% by Bonferroni. Independence of the
lower and upper bound procedures is not required. The statement is pointwise
for each named null model. It is not a simultaneous 95% claim across all
models, and it is not a conditional coverage claim given failed diagnostics.

| Model | All-eight SMC mean P̂ | Markov lower L | Upper U | Upper bound on expected waiting 1/p |
|---|---:|---:|---:|---:|
| Uniform | 1.0390173543897488e-12 | 2.597543385974372e-14 | 3.688872650206489e-6 | 3.849791327450291e13 |
| Block6 | 1.5521569811888627e-12 | 3.880392452972157e-14 | 3.688872650206489e-6 | 2.577058924114899e13 |
| Inversion exact | 3.438405867121971e-13 | 8.596014667804929e-15 | 3.688872650206489e-6 | 1.1633297971737398e14 |

For Block6 the separate exact-anchor component lower bound
1.9093313674451053e-12 is substantially stronger than its Markov lower bound.
The final report uses that separately justified component bound for Block6.
It does not choose the largest of several unadjusted random confidence
bounds. Each displayed model's chosen lower procedure is identified.

These are intervals for a null probability and its reciprocal expected
waiting parameter. They do not predict the realized waiting time of a future
sequence of draws.

## Numerical range and simulation idealization

The implementation stores γ in log form. Across the completed production,
the minimum logged normalizers were −31.3504 (uniform), −29.5715 (Block6),
and −278.8748 (inversion exact). Exponentiating these values does not approach
double-precision underflow. The zero inversion estimate in replicate 7 had
zero target hits and log Ẑ=−278.8748; it was not a numerical underflow zero.

At fixed β=2, all λ increments have log-weight range at most 2T_observed.
After the hard endpoint, the largest β increment is 16, giving range at most
16T_observed. The maximum bounds are 383.047, 363.107, and 371.252 for the
three models. Subtracting the maximum log weight before exponentiation thus
keeps every incremental weight safely representable in these calculations.

As with the other Monte Carlo statements in this study, exact probability
coverage is a property of the mathematical random algorithm. Finite PRNG
streams and floating-point rounding are its computational approximation.
Source review, disjoint-start audits, finite-state checks, and the numerical
range checks above support that approximation; they do not assert literal
infinite-precision unbiasedness of C++ arithmetic or prove mathematical
independence of a deterministic PRNG.
