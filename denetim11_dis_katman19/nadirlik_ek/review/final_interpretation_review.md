# Final independent interpretation review

Reviewed on 26 October 2026. This review does not alter any simulation, target,
budget, training histogram, or original result. It reads the original frozen
protocol and the completed block7, exact-block, SMC, and complement-cover
outputs. The completed uniform natural-event SIS production retains its own
importance-weighted uncertainty analysis.

## Numerically confirmed results

The block7 result was independently recomputed from the exact fraction for B,
the retained-hit count 2480, and the fixed proposal count 100000000. Beta
quantiles were evaluated independently of the stored final-summary values.

| Quantity | Recomputed result |
|---|---:|
| Block7 component probability | 1.9867619713864016e-12 |
| Component two-sided Clopper–Pearson 95% interval | [1.9093313674451053e-12, 2.06652684749066e-12] |
| Full block6 primary-tail interval, coverage at least 95% | [1.9093313674451053e-12, 3.688872650206489e-6] |
| Corresponding confidence interval for expected waiting count 1/p | [271085.5306821242, 523743556016.73773] |
| Component/natural19 point ratio | 328.8196947784274 |
| One-sided 95% lower confidence bound for full-tail/natural19 ratio | 268.7116322530126 |

The full-tail lower endpoint uses the component's one-sided 97.5% lower
bound. The upper endpoint is `1 - .025**(1/1000000)` from the original zero-hit
direct sample. Bonferroni gives at least 95% simultaneous coverage of these
two endpoints without assuming independence. The ratio bound divides the
component's one-sided 97.5% lower endpoint by the natural-event one-sided
97.5% upper endpoint and uses the same union-bound argument.

These are model-specific statements for the frozen primary score. They are
not simultaneous intervals over every null model, every text policy, or all
possible historical searches.

## Full-tail scope is genuinely incomplete

The block7 sampler calculates `P(T >= observed AND U)`, where U is the union
of six W0 anchors for prime7. Its certified G screen is complete for the
primary event within U. The five secondary policy outputs additionally
intersect that screen and have narrower stated events.

The complement is not merely an unverified hypothetical possibility. The
canonical observed labeled ordering belongs to the block6 support and meets
its own observed threshold. Direct recomputation of the six W7 anchors gives:

| Count convention | Ratio | F/H overlap | F/H union |
|---|---:|---:|---:|
| 129 | 1 | 1 | 26 |
| 129 | 2 | 1 | 38 |
| 129 | 3 | 1 | 24 |
| 127 | 1 | 1 | 26 |
| 127 | 2 | 1 | 37 |
| 127 | 3 | 1 | 25 |

All six anchors fail, because they require zero overlap and union size7.
Consequently `P(T >= observed AND complement(U))` is strictly positive on
this finite support. The single canonical ordering supplies a mathematically
valid but practically negligible lower mass of `1 / 720**19`.

`block6_outside_w7_cover.json` retains 407 variant/form/prime tuples with803
minimal positive C conjunctions as a necessary covering union for the
complement. Its relaxed upper scores remain27.5562485341 and26.7574718585,
above the observed22.6941834274. At the natural p19 tuple, C7, C9, and C10
remain in both variants. These are necessary relaxed possibilities, not803
known realizable modes and not803 measured probabilities. They do not prove
that C10, C15, or the W7 union covers the entire full-score event.

## SMC precision diagnosis

All eight independent production replicates, including the zero replicate,
were retained in these independent arithmetic calculations.

| Model | Arithmetic mean | Replicate SE | Relative SE | Largest replicate share | Nonzero | Predeclared gates |
|---|---:|---:|---:|---:|---:|---|
| Uniform | 1.0390173543897488e-12 | 5.289200914876519e-13 | 0.5090579953 | 0.5484334782 | 8/8 | Fail |
| Block6 | 1.5521569811888627e-12 | 6.049590245003467e-13 | 0.3897537632 | 0.4202898299 | 8/8 | Fail |
| Exact inversion | 3.438405867121971e-13 | 2.2338059882551207e-13 | 0.6496632668 | 0.6774043326 | 7/8 | Fail |

Every model fails both the relative-SE and maximum-contribution gates. The
nonzero-repeat gate alone passes. These means may remain visible as
exploratory estimates with their diagnosis; they should not be advertised
as reliable final full-tail rarity estimates or converted to confident
reciprocal waiting times. Finite-repeat Student intervals are only
approximations, especially with this skew and mode sensitivity.

The block6 SMC mean is below the independently estimated block7 component.
It is0.7184 SMC standard errors below that component estimate. This is not a
logical contradiction of an unbiased estimator, because realizations need
not preserve probability inclusions; it reinforces that the current SMC
precision does not establish the full-tail point value. It would be wrong to
replace the mean with the component, to discard low runs, or to select the
smallest result.

## Exact / binomial / weighted distinctions

- The block C and C-intersect-W anchor masses are exact counts. Their later
  natural-pattern estimates scale one joint binomial proportion by one known
  anchor mass. The scaled Clopper–Pearson interval is justified for those
  independent exact-conditional draws.
- The block7 thinned-hit estimator uses the fixed proposal denominator;
  rejected thinning attempts remain zeros. Its scaled binomial interval is
  justified. The alternative weighted estimator is a cross-check with an iid
  weight-variance calculation, not the source of the binomial interval.
- Uniform SIS must use weighted moments, contribution diagnostics, and
  qualified asymptotic or replicate uncertainty. Its raw positive-hit count
  cannot be substituted into an ordinary binomial confidence interval for
  the unconditional probability.
- SMC particles are dependent. Binomial intervals on final particle hits
  are not valid uncertainty intervals for the normalizing-constant estimator.
- Reciprocals of probability confidence limits are confidence limits on the
  expected count1/p. They are not prediction limits on one realized waiting
  time.

## Textual19 provenance and scope

`FROZEN_PROTOCOL.md` explicitly states that the19 pattern was known before
the finite grammar and semantic candidate sets were constructed.
`semantic/salience_audit.json` independently records that access. The sets
were then frozen before the simulations. This is not a blind prior discovery
or a retrospective preregistration of the original observation.

The mixture weight is `q(19)=(1/29+1/5+1/4)/3`, giving the fixed natural
binding2.2275357246 extra ideal code bits relative to uniform prime weights.
The shared-binding saving is `3*log2(14)+2*(-log2(q(19)))` =16.6829553072 bits;
G alone requires no prime, hence two saved prime choices, not three.

These are code-cost comparisons. Neither is an independent probability
factor. Text content is fixed in these permutations, and this procedure does
not estimate the probability of the particular74:30 location, all of its
meaning, or the presence of the textual reference. A distinct textual null
would be required for that different question; no such null has been defined
or silently supplied here.

## Concrete report corrections

1. Use26 October2026 in the new report, matching the client date; preserve
   original source-study dates.
2. Replace “tam olay ... en az bu kadar sık olabilir” with the deterministic
   inclusion `P(full event) >= P(component)`, then state the numerical bound
   with its confidence level.
3. Prefer “oran için tek yönlü %95 alt güven sınırı268,71” over saying that
   the event merely “could be269 times more frequent.” If rounded to269,
   retain “yaklaşık”;268.71 is the actual bound.
4. Keep the failed full-tail precision status in the opening answer. The
   completed narrow natural-pattern calculation does not complete the
   different full-tail probability problem.
5. Keep the block6 /127 / natural view qualifiers attached to the165.5-trillion
   result. The original standard129 observed natural matrices do not satisfy
   the same four M predicates.
6. After uniform SIS finishes, insert its result under its own null and
   event label, with weighted uncertainty. Do not pool it with block6 or
   treat its anchor-restricted full-score component as a full-tail result.

No further inexpensive scalar calculation can convert the 803-component
cover into a complete probability. A precise full-tail estimate would need a
new validated complement estimator or substantially better global rare-event
coverage. Reporting this unresolved scope is a necessary part of the
scientific result.

## Additional valid full-tail bounds from the SMC estimates

A source-level check by the independent production auditor confirms that the
fixed production estimator is nonnegative and unbiased for the frozen full
primary-tail probability in the usual ideal-random/arithmetic SMC model. Its
fixed schedule, original-null initialization, unbiased stratified offspring,
invariant MH transitions, fixed proposal attempts, exact endpoint, and
terminal inverse-density correction satisfy the required construction.

Thus the mean of all eight retained production estimates, denoted X, obeys
`E[X]=p` and `X>=0`. Markov's inequality gives

`P(.025 * X > p) <= .025`.

This supplies a one-sided 97.5% lower confidence bound without a variance or
mixing-precision claim. Combining it with the original direct-test one-sided
97.5% upper bound via Bonferroni gives the following conservative full-tail
intervals:

| Model | Probability interval, coverage at least 95% | Expected waiting count interval |
|---|---:|---:|
| Uniform | [2.597543385974372e-14, 3.688872650206489e-6] | [271085.5306821242, 38497913274502.91] |
| Exact inversion | [8.596014667804929e-15, 3.688872650206489e-6] | [271085.5306821242, 116332979717373.98] |

These bounds do not make the SMC point estimates precise. Their coverage is
unconditional over production randomness and pointwise for each model; it is
not simultaneous across all models and not conditional on the observed
precision gates failing. Replicate independence is not needed for this
expectation-based inequality, although the implementation uses independent
starts. Keep the separately validated and much stronger block7-based interval
for block6; do not take a post hoc maximum of several 95% lower bounds without
adjusting the total error budget.

## Completed uniform natural-event result

The uniform natural-127 estimate is `7.780716111327623e-18`. Its importance-
weighted normal-approximation 95% interval is
`[5.854590399815615e-18, 9.70684182283963e-18]`. The reciprocal estimate is
`1.2852287446192021e17` independent permutations, approximately 128.5
quadrillion in English or 128.5 katrilyon in Turkish. The corresponding
approximate interval for the expected waiting parameter is
`[1.0302011903058507e17, 1.7080614213959254e17]`.

The production has 83 positive draws, target effective sample size62.6853,
relative standard error12.6304%, maximum single-draw contribution3.4622%,
and nonzero target contributions in all16 worker batches. Those diagnostics
support the declared approximate uncertainty; they do not transform it into
an exact-coverage binomial interval. Its event remains the fixed natural
form, prime19, count127, and natural matrix view. The broader score output
from this sampler is explicitly restricted to its C15W3/natural-G anchor
intersection and remains a component.
