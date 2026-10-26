# Statistical interpretation review of the draft report

Reviewed `rare_event/build_deliverables.py`, primarily its `report()` text.
No report or statistical code was changed by this review.

## Necessary corrections

1. **State failed global precision explicitly in the direct answer.** If the
   production gates fail, section 1 must state that a reliable numerical value
   for the full-search tail has not yet been established. Section 7's means
   remain exploratory diagnostics; they must not become an asserted global
   rarity or an advertised reciprocal waiting count. Point estimates should
   not be selected from successful replicates or from whichever method gives
   the smallest probability.

2. **Describe the textual weighting's provenance and scope.** Section 8's
   wording about prior selection should state that the 19 hypothesis was
   already known when the semantic candidate sets were constructed and that
   these sets were frozen before the simulations. `semantic/salience_audit.json`
   explicitly records that access. These weights are not independently
   discovered prior evidence. They encode which prime candidates receive a
   lower code cost; they do not estimate the probability of the specific
   74:30 location, the full surrounding meaning, or the existence of that
   textual reference. The text is held fixed in these permutations.

3. **Narrow the inversion-normalizer validation claim.** Section 9's claim
   that the normalizer confirms exactly which distribution the sampler
   generates is stronger than the calculation establishes. The normalizer
   independently checks support mass and a necessary acceptance-rate
   prediction. Uniformity within that support rests on the separate sampler
   construction, tie-label argument and validation.

4. **Keep the reciprocal interval a parameter interval.** The report's main
   paragraph correctly labels the interval as uncertainty in the expected
   repeat count. The notebook headline's shorter phrase “%95 tekrar aralığı”
   should also say it is a confidence interval for `1/p`, the expected waiting
   count. It is not a prediction interval for one realized waiting time.

## Scope constraints when pending results are inserted

- Keep **block6, the 127 convention, p19, the fixed natural definitions, and
  the natural matrix view** attached to the 165.5-trillion result wherever it
  is repeated. The current draft does this correctly in its main sections.
- The block7 union estimator measures `P(full-score tail AND anchor union)`.
  It is a component and hence a lower contribution to the global tail. A
  confidence interval for that component supplies no upper confidence bound
  for the complete tail.
- Uniform natural-event SIS must retain its own importance-weighted
  uncertainty calculation; an ordinary binomial interval for weighted
  successes is not justified.
- Each of the six policy tails requires its own variance and mode-coverage
  discussion. The primary guide's diagnostic status cannot be transferred
  automatically to another policy.

## Substantive claims already correct

The fixed natural-event estimate is clearly separated from the full-search
tail. Its confidence interval scales the single joint conditional binomial
count by the exactly known anchor probability. The report does not multiply
separate marginal confidence intervals or assume independent layers. The
36/189 comparison uses one sample and correctly measures the conditional
contribution of the two additional C conditions. The report also correctly
limits its sampling interval to the specified model and fixed event, without
claiming that it covers model selection or historical searches.
