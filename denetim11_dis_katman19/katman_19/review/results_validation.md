# Independent final-results validation

Completed: 2026-10-25T13:59:01.837422+00:00

## Scope

This audit read the 24 raw binary score shards, three observed-score files, six final run manifests, three training histograms and three diagnostic aggregates. It did not import or read the main analyzer or its summary before completing these calculations. The analyst had been informed that the main analysis found zero exceedances; the calculations themselves were recomputed from the raw shards.

## Raw-score results

Each model has 200,000 training draws and exactly 1,000,000 fresh test draws. Each of its eight score shards contains 125,000 rows of six float64 values, or 6,000,000 bytes. All values are finite. Every training categorical histogram sums to 200,000 and all binary headers, file lengths and runtime totals agree.

| Model | Primary observed score | Largest primary control | Exceedances / test draws | Plus-one p |
| --- | ---: | ---: | ---: | ---: |
| uniform | 23.9404357575 | 9.08496226914 | 0 / 1,000,000 | 9.99999000001e-07 |
| block6 | 22.6941834274 | 6.04235212793 | 0 / 1,000,000 | 9.99999000001e-07 |
| inversion_exact | 23.2032475167 | 7.5093242673 | 0 / 1,000,000 | 9.99999000001e-07 |

All six declared policies have zero exceedances in each of the three models. Comparisons use score >= observed score, including ties. The saved top 50 primary scores match the 50 largest scores recomputed from every raw test row.

For each individual zero-hit run, the plus-one Monte Carlo value is 9.99999000001e-07; the exact two-sided 95% Clopper–Pearson interval for the conditional tail probability is [0,3.68887265021e-06]; the one-sided 95% upper bound is2.99572778635e-06. Zero hits do not mean zero probability or an arbitrarily tiny estimated tail probability.

The three null models are not pooled into a 3,000,000-draw denominator. Robustness is summarized by the maximum model-specific p-value. The six related policies are not treated as six independent confirmations.

## 20 diagnostics

| Diagnostic | Uniform | Block6 | Exact inversion |
| --- | ---: | ---: | ---: |
| E1_modules_exist_separately_within_one_variant | 0 | 0 | 0 |
| E2_same_form_and_prime_G_Cbase_W_M | 0 | 0 | 0 |
| E3_same_form_and_prime_G_Ccenter_W_M | 0 | 0 | 0 |
| E4_E3_and_number_discussion_prime | 0 | 0 | 0 |
| any_Cbase | 0 | 48 | 75 |
| any_Ccenter | 0 | 0 | 4 |
| any_shared_Cbase_M | 0 | 0 | 0 |
| any_shared_Ccenter_M | 0 | 0 | 0 |
| canonical_129_Cbase | 0 | 16 | 10 |
| canonical_129_Ccenter | 0 | 0 | 0 |
| canonical_129_M | 1390 | 1471 | 1494 |
| canonical_129_Cbase_M | 0 | 0 | 0 |
| canonical_129_G | 300 | 435 | 447 |
| canonical_129_W | 416 | 634 | 406 |
| canonical_127_Cbase | 0 | 16 | 10 |
| canonical_127_Ccenter | 0 | 0 | 0 |
| canonical_127_M | 1450 | 1426 | 1482 |
| canonical_127_Cbase_M | 0 | 0 | 0 |
| canonical_127_G | 302 | 472 | 424 |
| canonical_127_W | 402 | 634 | 411 |

E1 through E4 all have count 0 under every model. Thus E2/E1, E3/E2 and E4/E3 are 0/0 and **unestimable**. The incremental rarity of common-rule, center and textual-anchor constraints within the complete four-module family cannot be estimated from these zero denominators.

The simpler C-only event has nonzero counts in two models. For centered-C given any bare-C, uniform is 0/0 (unestimable), block6 is 0/48, and exact inversion is 4/75 = 0.0533333. Those limited conditional samples support the intervals in the machine-readable report; they must not be confused with the unobserved complete-family conditioning events.

The 20 diagnostic numbers are producer aggregates. Their bounds, nesting and conditional-denominator calculations were independently checked. They cannot all be reconstructed from the six saved score columns; this audit did not replay all 3,000,000 feature evaluations.

## Seeds and runtime bookkeeping

All 48 final worker seeds match an independent implementation of the hashed seed formula and are globally distinct. Minimum circular distance between starting states in SplitMix's counter coordinates is 27,188,455,771,641,243 outputs.

For ranges 1..114 the union bound on all potentially rejected 32-bit inputs is 2,947. Because SplitMix's full output map is bijective, this bounds retry states across an entire 64-bit cycle by 12,657,268,621,312. Adding 342 times the largest whole-run proposal count bounds any worker's total consumed counter interval by 12,819,487,675,210, below the minimum starting-state gap. Thus the final worker counter intervals cannot overlap under the audited code and recorded proposal counts. This establishes stream separation, not mathematical independence of a deterministic PRNG.

Training and test manifests use different master seeds, with no worker-state collision across phases or models. The initial defective arithmetic seed spacing is not present in the final manifests.

## Reproduction

Run `python semantic/analyze_results_independent.py` from any working directory. Machine-readable details, confidence bounds, policy-specific maxima, shard hashes and metadata checks are saved in `review/results_validation.json`.

## Comparison after independent completion

After saving the independent calculations, the main analyzer summary was opened for comparison. All18 model/policy results agree exactly on observed scores, exceedance counts, sample sizes, plus-one values, control maxima and confidence bounds. All60 diagnostic counts and all six run manifests also agree. See `review/results_crosscheck.json`.
