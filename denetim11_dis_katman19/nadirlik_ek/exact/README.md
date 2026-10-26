# Exact conditioning and rare-event estimates under the block6 model

## What was calculated

This supplementary calculation preserves the original verse counts, paired 129/127 variants, null model, feature definitions, trained score, and six textual policies. It changes the computational method for a specified part of the probability space.

The block6 null independently permutes the six labeled suras within each of the nineteen original blocks. Every one of its `720^19` labeled orders has equal probability. Repeated verse counts retain their sura labels throughout the enumeration and sampling.

For the natural form `n+a` at prime19, an exact integer dynamic program counts the C event and its intersection with W. A uniform integer rank then samples directly from that counted subset. Two fresh, fixed-budget experiments each draw **100,000,000 independent conditional orders**. They do not use a Markov chain, burn-in, resampling, fitted importance weights, or a mixing assumption.

The central result for the natural 127-verse conjunction is:

\[
\Pr_{\text{block6}}(G_{127},C_{15},W,M_0)
\approx 6.04210\times10^{-15},
\]

with two-sided95% Clopper–Pearson interval

\[
[5.10179\times10^{-15},\;7.10550\times10^{-15}].
\]

This is an estimate of a specified natural conjunction under one null model. **It is not the probability of the complete scanned-score tail.** The latter also permits alternative forms, primes, variants, matrix views, positive predicate masks, module activations, and parameter-sharing partitions. A probability interval for a subset's contribution does not give an upper bound for that larger tail.

## Event definitions

All four C predicates are the already implemented predicates in `src/features.hpp`:

| Bit | Predicate |
|---:|---|
|1|Four nonempty parity cells have equal member counts.|
|2|One row has equal coefficient sums; the other row's sums differ from that common sum by−1 and+1.|
|4|The observed integer coefficient support is contiguous, and at least one cell with at least two members is constant.|
|8|The sum of all four cell coefficient sums equals `4p`.|

`C15` means all four predicates. The larger event called **Cpattern-centre** means `(Cmask & 10)==10`: bits2 and8 are present. It includes actual masks10,11,14,15; it does not mean only the exact mask10.

For `F={n:19 divides n+a}` and `H={n:19 divides n−a}`, W means that F and H are disjoint, their union has19 members, and the sum of its verse counts is divisible by19. G means that the even `n+a` family has57 members and that its sum of `n+a` equals the total verse count. M0 requires the four frozen matrix predicates in the natural view0; Many allows any of the eight canonical views.

The conventional total is6236 and the edited total is6234. The same labeled order is used for both variants. In the block6 model, original sura9 remains in positions7–12. Neither129 nor127 makes that sura a member of F or H in those positions. Consequently, the C and W anchor events are **identical between the two variants**, while G and M can differ.

## Exact enumeration of the C anchor

Every block has only720 labeled permutations. Enumerating these shows that the possible natural F coefficients over the entire block6 space lie in3…9. Exhaustive enumeration of the cell coefficient multisets, followed by assignment-compatible counting, proves that C15 has exactly two possible coefficient configurations:

|Cell0|Cell1|Cell2|Cell3|
|---|---|---|---|
|6,6,8|6,6,6|5,5,9|5,7,7|
|6,6,8|6,6,6|5,7,7|5,7,7|

The second configuration matters: contiguous support5…8 satisfies the frozen predicate even though coefficient9 is absent. Counting only the visually observed first configuration would undercount the event.

The exact number of C15 orders is

```
1722075030862920049254249867105596866560000000000
```

out of

```
720^19 = 1946782493550362124155303577601966080000000000000000000.
```

Thus

\[
\Pr(C_{15})=
\frac{12997308130651}{14693280768000000000}
=8.845749520391251\times10^{-7}.
\]

For Cpattern-centre there are eighteen possible coefficient configurations, all with nonzero assignment-compatible counts. They are listed with counts and example orders in `block6_C10_exact.json`. Their exact combined probability is

\[
\Pr(C_{\mathrm{pattern,centre}})=2.982418523027784\times10^{-6}.
\]

An actual counterexample in `C10_not_C15_counterexample.json` has coefficient cells `(4,4,6,6)`, `(6,6,6)`, `(5,7,7)`, `(5,7,7)`. It has mask14: pattern, centre, and band/constant-cell predicates hold, but the member counts are4,3,3,3. Pattern and centre do not imply the other C predicates.

## Adding W without any independence assumption

For every F member, `a ≡ −n (mod19)`, and for every H member, `a ≡ n (mod19)`. With overlap excluded, the union verse sum condition is therefore equivalent to

\[
\sum_{H}n-\sum_{F}n\equiv0\pmod{19}.
\]

Every C15 configuration has12 F members, so W additionally requires Hcount7. The expanded Cpattern-centre configurations have12,13,or14 F members; their H targets are respectively7,6,or5.

The exact DP augments each coefficient state with the remaining H count and the required difference of the two position sums modulo19. Local F/H overlap is excluded. Its transitions are multiplied by the exact number of compatible labeled permutations of that block. The resulting anchor counts are:

|Anchor|Exact probability|
|---|---:|
|C15 and W|4.1384256856316515×10⁻⁹|
|Cpattern-centre and W|1.7731145864366945×10⁻⁸|

For the smaller anchor,

\[
\Pr(C_{15},W)=
\frac{6052295999638228458971}{1462463376025190400000000000000},
\qquad
\Pr(W\mid C_{15})=0.004678434174618825.
\]

The C15 sampler has500 positive coefficient states. The expanded sampler has2622. This small state space makes exact conditioning practical even though direct unconditioned sampling encounters the anchors very rarely.

## Exact conditional sampling

For a state, the number of full completions is the sum, over its possible next local profiles, of the number of compatible local labeled permutations times the child's completion count. An integer selected uniformly from `[0,total_count)` uniquely identifies an order. At each block, cumulative completion counts select a profile group; integer quotient and remainder select a local permutation and the residual rank.

This is a bijective unranking of the counted labeled sample space. Consequently, independent uniform ranks give independent uniform conditional orders. No unknown normalization constant remains.

All counts fit into192 bits because even `720^19 < 2^181`. The C++ sampler uses three64-bit limbs. It may use a floating-point quotient guess for speed, but then corrects that guess using exact integer multiplication, comparison, and subtraction. No floating-point probability or cumulative distribution decides an order's weight. Random ranks use masked64-bit PRNG words and exact rejection of ranks outside the allowed interval.

A Python arbitrary-precision reference checked100,000 quotient/remainder cases, including zero remainders and remainders one below the divisor. Independent forward polynomial convolution, with a fresh enumeration of all720 permutations per block and without importing the suffix-DP implementation, reproduces **both C and CW counts exactly for all eighteen expanded configurations**.

## The matrix condition reduces to a finite residue table

Let

\[
x=\sum_E n\bmod19,\qquad y=\sum_F n\bmod19,
\]

and let `t` be the total verse count modulo19: `t=4` for129 and `t=2` for127. Since `Σn=6555` is divisible by19, G and the C centre condition imply

\[
P=\begin{pmatrix}x&t-x\\-x&x\end{pmatrix},\qquad
R=\begin{pmatrix}y&-y\\-y&t+y\end{pmatrix}\pmod{19}.
\]

All361 residue pairs can therefore be checked directly, without a statistical approximation. An independent Python implementation of matrix multiplication and exact multiplicative order gives:

|Variant|`x`|`y`|Views satisfying all four M predicates|
|---:|---:|---:|---|
|129|8|9|1|
|129|10|11|1|
|127|16|1|0,3|
|127|18|3|5,6|

Thus the natural view0 condition is impossible for129 once G and the centre condition hold. In127 it occurs exactly when `(x,y)=(16,1)`. This statement comes from exhaustive finite arithmetic; it is stronger than observing zero successes in a simulation. It does not say that all other views or other grammar branches are impossible in129.

## Fixed-budget production results

The first experiment uses the exact C15-and-W anchor, seed202610250701, and100,000,000 draws. Its plan and code/input hashes were saved before production. It took24.93 seconds on the available eight-thread execution environment.

|Event within the C15-and-W anchor|Conditional hits|Estimated unconditional probability|95% interval|
|---|---:|---:|---|
|G129|51,482|2.13054×10⁻¹²|[2.11218,2.14902]×10⁻¹²|
|G127|50,888|2.10596×10⁻¹²|[2.08771,2.12434]×10⁻¹²|
|G129 and Many|278|1.15048×10⁻¹⁴|[1.01920,1.29398]×10⁻¹⁴|
|G127 and Many|304|1.25808×10⁻¹⁴|[1.12062,1.40775]×10⁻¹⁴|
|G127 and M0|146|6.04210×10⁻¹⁵|[5.10179,7.10550]×10⁻¹⁵|

For a counted anchor A and any target B, the estimator is simply

\[
\widehat{\Pr(A\cap B)}=\Pr(A)\frac{\#\{B\text{ successes in conditional draws}\}}{100000000}.
\]

The interval is the conditional binomial Clopper–Pearson interval multiplied by the exactly known `Pr(A)`. It does not multiply marginal estimates as if the modules were independent.

The same result can be described by the valid sequential conditional factorization:

|Factor|Value|
|---|---:|
|P(C15), exact|8.845749520×10⁻⁷|
|P(W given C15), exact|0.004678434175|
|P(G127 given C15 and W)|50,888/100,000,000 =0.00050888|
|P(M0 given G127, C15, and W)|146/50,888 =0.002869045748|

The last two factors use the **same** sample and their product cancels the intermediate G count. This is conditioning, not an independence claim. Permitting any of the eight canonical matrix views gives304/50,888 =0.005973903474 for the last factor.

## Expanded anchor and the additional C restrictions

The second experiment uses Cpattern-centre and W, seed202610250801, and a fresh100,000,000 draws. Its plan was also written before production. It took44.44 seconds. Each draw was classified by its exact C mask.

|Actual C mask|Conditional draws|G127 and M0 hits|Full primary-score tail hits, also requiring Geither|
|---:|---:|---:|---:|
|10|51,545,045|100|0|
|11|6,688,559|11|11|
|14|18,428,555|42|0|
|15|23,337,841|36|36|
|Total|100,000,000|189|47|

Here `Geither` means natural G holds in at least one of the paired variants. Every such draw was evaluated by the complete frozen Extractor and Scorer, including their entire search. The full matrix residue reduction was also checked against the Extractor for every G draw.

The larger natural conjunction has estimate

\[
\Pr(C_{\mathrm{pattern,centre}},W,G_{127},M_0)
=3.35119\times10^{-14}
\]

with95% interval `[2.89043,3.86451]×10^-14`.

Within this same sample, adding equal cell sizes and the band/constant-cell requirement retains36 of the189 full natural conjunctions:

\[
\frac{\Pr(C_{15},W,G_{127},M_0)}
{\Pr(C_{\mathrm{pattern,centre}},W,G_{127},M_0)}
\approx\frac{36}{189}=0.190476,
\]

with95% interval `[0.137117,0.253814]`. The point estimate corresponds to a further reduction by a factor5.25 within this specified natural conjunction. It is not a separate multiplier to attach to a globally calibrated score probability.

The individual extra conditions are correlated: equality holds in47/189 cases and the band/constant-cell condition in78/189, while both hold in36/189. The latter is larger than the product of the two marginal fractions. A valid conditional decomposition is `(47/189)×(36/47)=36/189`.

## What the full-score evaluations establish

In the first experiment, all six policy columns had146 threshold exceedances among the C15-and-W draws with Geither. In the expanded experiment, all six had47 threshold exceedances among the Cpattern-centre-and-W draws with Geither. These identical counts are reported as observed; the policies are not treated as independent replications.

For the expanded experiment, the estimated subset contribution to each complete tail is

\[
\Pr(T_k\ge T_{k,\mathrm{obs}},\;C_{\mathrm{pattern,centre}},W,G_{\mathrm{either}})
\approx8.33364\times10^{-15},
\]

with95% interval `[6.12324×10^-15,1.10820×10^-14]`.

Because the intersection event is a subset of the complete tail, its true probability is a lower-bound contribution. **Neither this point estimate nor the upper end of its interval is an upper bound on the complete tail.** The calculation does not cover score exceedances outside this natural C/W/G family. It does not establish the full tail probability under uniform permutation or exact-inversion sampling.

Masks10/14 can satisfy the relaxed natural matrix conjunction yet fall below the full-score threshold because the frozen training frequencies assign them a different C utility. This is why the relaxed natural-conjunction probability3.35×10^-14 and the threshold-contribution probability8.33×10^-15 answer different questions.

## Validation and provenance

The machine-readable record `conditional_results.json` contains every counter, scaled interval, exact anchor fraction, and the following checks:

- Every input/source hash in both production plans still matches.
- Each experiment processed exactly100,000,000 draws with the fixed recorded seed and eight workers.
- The reconstructed observed scores exactly match the saved frozen block6 observed score file.
- Independent forward and backward integer counts agree for C15/CW and for all eighteen expanded configurations.
- All1,000 direct feature-validation draws per production satisfy the claimed C and W anchor in both variants; the expanded experiment additionally validates matrix reduction on every G draw.
- The expanded sampler's C-mask proportions match exact stratum masses: validation chi-square0.92119 with3 degrees of freedom. Its diagnostic p=0.82031 is a sampler check, not a numerical-structure p value.
-100,000 arbitrary-precision quotient/remainder reference cases pass.
- All80 worker streams comprising the earlier48 frozen-study streams and32 new conditional pilot/production streams have distinct starting states. Hashed seed formulas are recomputed. New final RNG states match exact consumed calls; C15 consumption is obtained by exact replay of rank draws. The minimum circular SplitMix counter gap is9,562,352,722,671,573, larger than the earlier conservative per-worker consumption bound12,819,487,675,210 and every newly measured consumption count. Thus the audited consumed counter intervals do not overlap. This does not purport to prove mathematical statistical independence of a deterministic PRNG.

These are additional calculations performed after the original numerical discovery. Exact arithmetic and independent conditional samples improve numerical resolution; they do not retroactively preregister the original pattern, expand the frozen grammar to every conceivable alternative, or establish a random-text or authorship probability.

## Main files and reproduction

`block6_core.py` enumerates C15 configurations, counts them exactly, and exports local profiles. `block6_cw.py` augments the DP with W and exports the conditional integer sampler. `block6_pattern_centre.py` performs the eighteen-configuration expansion. `validate_exact.py` and `validate_exact_c10.py` independently count the same spaces by forward polynomial convolution.

`sample_cw.cpp` and `sample_cw10.cpp` implement the two production experiments. `production_plan.json` and `production_C10_plan.json` retain the fixed budgets, seeds, scopes, and source hashes. `analyze_production.py` reproduces `conditional_results.json` from the completed count and production files.

From the study root:

```bash
python rare_event/exact/block6_core.py --draws 100000
python rare_event/exact/block6_cw.py
python rare_event/exact/block6_pattern_centre.py
python rare_event/exact/validate_exact.py
python rare_event/exact/validate_exact_c10.py
g++ -O3 -std=c++17 -fopenmp rare_event/exact/sample_cw.cpp -o rare_event/exact/sample_cw
g++ -O3 -std=c++17 -fopenmp rare_event/exact/sample_cw10.cpp -o rare_event/exact/sample_cw10
rare_event/exact/sample_cw "$PWD" 100000000 202610250701 8 rare_event/exact/block6_CW_production.json
rare_event/exact/sample_cw10 "$PWD" 100000000 202610250801 8 rare_event/exact/block6_C10W_production.json
python rare_event/exact/analyze_production.py
```

The saved RNG replay files are needed by the final analysis. They can be regenerated with `replay_cw_rng.cpp`; the arbitrary-precision fixture can be regenerated with `make_integer_tests.py` and checked with `test_integer.cpp`. The original matrix reference in `src/matrix_reference.py` supplies the direct finite-arithmetic crosscheck.
