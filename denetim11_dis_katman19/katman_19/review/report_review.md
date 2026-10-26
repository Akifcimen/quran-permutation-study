# Final numerical and statistical consistency review

Reviewed `katman_19_raporu.md` against the frozen protocol, production summary and diagnostic counters, saved top controls, and independently generated observed decomposition. This review concerns material numerical/statistical interpretation, not stylistic editing.

## Result

**No material numerical or statistical-result discrepancy was found.** One table-heading clarification was requested: the section 7 column showing 1/5 is the equal weighting *within the literal-number branch*, not the overall primary mixture weight. The heading `Birincil aday değeri ağırlığı` should therefore be replaced by `Açık sayı sınıfındaki eşit ağırlık` or equivalent. The numerical entries remain correct for that branch; no test or result changes are needed.

## Checks passed

- The 127/129 scalar invariants and natural matrix contrast agree with the direct structure calculations.
- The 29 primes, 14 signed forms, two variants, eight canonical matrix views, and 51 activation/partition shapes match the implemented finite family.
- The 16.682955-bit saving is correctly limited to common form/prime parameter bindings. It is not described as a universal operator-description length or an independent probability multiplier.
- The separate-versus-shared score table and all six variant-specific primary maxima agree with `observed_decomposition.json`.
- The net matrix increments 4.296765, 4.243325, and 3.916161 include the additional predicate, view, and partition costs and agree with the independent decomposition.
- The primary mixture weight for 19, 0.1614942529, and the 2.227535725-bit one-prime score shift are correct.
- All 18 model/policy rows in the production summary contain zero exceedances among exactly 1,000,000 test draws.
- The observed primary scores and primary control maxima match the stored summaries and strongest-control traces.
- The plus-one Monte Carlo value, one-sided 95% bound, and two-sided Clopper–Pearson upper endpoint are numerically correct and clearly distinguished.
- Different null models are not pooled. The maximum-primary-p robustness summary is described with the correct scope, and model-wise confidence bounds are not presented as simultaneous bounds.
- The nested event definitions and zero denominators agree with the production diagnostics. The 48, 75, and 4 more-permissive cell-pattern counts and the descriptive 4/75 and zero-event upper bounds are correct.
- The strongest-control descriptions agree with their saved forms, primes, activated groups, and independently reconstructed traces.
- The report correctly distinguishes a trained surrogate from exact joint-event surprisal, textual conditioning from a new independent Bernoulli event, and retrospective finite-family calibration from complete correction for historical discovery choices.
- The worker-seed defect, retirement of all initial training tables, and correction before final testing are described consistently with the audit records.

The requested heading clarification is the only remaining review item; it does not change the scientific result.


## Root resolution before packaging

The literal-number table heading was changed to “Açık sayı sınıfındaki eşit ağırlık”. No data or result changed. The report also now explicitly distinguishes the independent raw-score audit from validation of producer diagnostic aggregates.
