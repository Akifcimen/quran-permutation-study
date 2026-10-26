# Exact conditional calculation for the block6 W7 union

## Target and scope

The target is the **unchanged primary mixture-score exceedance**, intersected with a specified union of six auxiliary events under the existing block6 null. This calculates a contribution to the global score-tail probability. It does not claim that the complement of the union contributes zero.

The block6 null independently permutes the six labeled chapter identities inside each of the 19 original blocks. Its sample-space size is exactly720^19. The same permutation is used for the 129 and127 count variants, with the change attached to original chapter identity8 (zero-based).

For each variant and each ratio r in{1,2,3}, define B_(v,r) by:

- No record has both `a == r*n (mod7)` and `a == -r*n (mod7)`.
- The union of those two selectors contains exactly7 records.

These six anchors cover every p=7 W0 predicate in the frozen14-form family: multiplying either selector by a nonzero residue and exchanging the signs reduces all coefficient ratios to one of±1,±2,±3.

## Exact counts and exact conditional draws

For each block, enumerate all720 labeled permutations. Retain permutations with zero overlap and group them by their contribution u to the union count. The backward integer recurrence is

`N_j(block,k)=sum_u local_multiplicity_j(block,u)*N_j(block+1,k-u)`.

The terminal count is one at k=0 and zero otherwise. The exact anchor numerator is N_j(0,7). An integer rank uniformly sampled below this numerator is unranked through the recurrence, selecting both the local contribution and a labeled block permutation. No approximate floating-point categorical probabilities are used in conditional sampling. The integer implementation and its corrected quotient operation were independently validated in the earlier conditional sampler.

Independent Python enumeration matches all six C++ anchor counts, all114 local histograms, and all798 histogram coefficients. Probabilities are approximately:

| Variant | Ratio1 | Ratio2 | Ratio3 |
|---|---:|---:|---:|
|129|8.8528219859e-9|1.2466087197e-9|2.9117795812e-8|
|127|1.2786332560e-8|1.6031462987e-9|2.6504664437e-8|

The sum of the six masses, counting overlaps repeatedly, is `B=8.01113698139678138527e-8`. Exact inclusion–exclusion independently gives union mass `5.78110132042282645839e-8`. Thus the expected thinning acceptance below is `0.7216330633026902542`.

## Size-weighted mixture and exact binomial inference

Choose anchor j with probability b_j/B, where b_j is its exact base-null mass, and draw uniformly conditional on that anchor. One integer rank below the sum of all anchor numerators implements both choices jointly. If a sampled permutation lies in K anchors, its proposal density is `pi0(x)*K/B`.

Independently retain it with probability1/K, implemented by the exact bounded-integer RNG. For the event E defined by the frozen primary threshold,

`P_proposal(retained AND E)=P0(E intersect union B_j)/B`.

Therefore, over a **fixed number n of proposals**, all failed thinning attempts count as zeros and the estimator is

`B * number_of_retained_primary_hits / n`.

The retained-event indicators are iid Bernoulli. Multiplying their exact Clopper–Pearson interval by B yields an exact-coverage binomial interval for this component probability. Sampling does not stop at a requested number of accepted points or hits. The alternative importance estimator `mean(B/K * I_E)` is reported as an efficiency cross-check, with its ordinary iid variance estimate.

## Fast complete screen for the primary event

A certified relaxed-score upper bound shows that a block6 primary exceedance must use G. A G-count-only mask cannot improve a positive winning program, because its maximum feasible utility is below the minimum added partition penalty. Hence some G balance predicate must hold. All14 forms reduce to three parity selectors: n+a even, n even, or a even. All three selectors are checked in both variants from additive block profiles before invoking the full frozen extractor and scorer. This is an exact necessary-condition screen for the primary event.

The five secondary policy counters are explicitly labeled as additionally intersecting this G-balance screen. Completeness of that screen has not been established for those separate policy thresholds. Their retained-event indicators still support binomial intervals for the events as labeled.

## Validation and production separation

`test_sampler.cpp` exhaustively enumerates518400 two-block states and verifies all44064 anchor ranks bijectively, including direct selector arithmetic. A200000-draw toy mixture checks thinning acceptance, an event probability, and the importance estimator; all discrepancies are below1.06 standard errors. First125 draws per production worker are also checked against the original full extractor for anchor membership, G screening, and block preservation.

The1m-proposal pilot is excluded from the estimate. Its29 retained primary hits motivated a fixed100m-proposal production budget, split into two50m batches with new independent seeds. The budget, hashes, estimator, and stopping rule were recorded in `production_plan.json` before either production result was inspected.

Files include the exact-count implementations, independent intersection calculation, sampling source, toy validation, pilot, frozen plan, two production batches, and their combined analysis. The original score and original study outputs are unchanged.
