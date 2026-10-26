# Can the frozen global tail be restricted to the natural C15 crystal?

**No for uniform and exact-inversion nulls. Explicit valid counterexamples exist.** The block6 natural C15 count remains a useful component calculation, but its identity with the entire global tail needs a separate proof.

## Counterexamples

Starting with the original labeled order, cyclically replace the identities at positions 50,88,107 so their verse counts `(45,26,7)` become `(26,7,45)`. All other identities remain fixed. The resulting uniform-null state has:

- natural G=3 and W=3 in both variants;
- natural C=11, with cell counts `(3,3,3,3)` and sums `(18,20,19,19)`;
- natural 127-variant M=15 in the natural matrix view;
- **no C15 candidate anywhere** in the full scan over both variants and all 406 form/prime combinations;
- primary uniform score exactly `23.940435757549533`, equal to the observed threshold.

Its natural quotient support is contiguous but none of the four cells is constant, so C bit2 fails. Both C11 and C15 have zero successes in every relevant natural-tuple training histogram. Their frozen fitted probabilities and code costs coincide. The score therefore remains unchanged when C15 becomes C11. This is an actual permutation counterexample, not merely an unattainable residue upper bound.

For an exact-inversion counterexample, make three further swaps of positions `(8,26)`, `(65,77)`, `(1,35)`. Each pair has positions of the same parity and verse counts differing by a multiple of38; all six positions lie outside F. These changes preserve the natural G and C statements, W congruence and cardinality, and matrix residues. They restore strict inversion counts to **993 and994** for the two variants. The full frozen scan again finds no C15 candidate, and the primary exact-inversion score is exactly the observed `23.203247516669524`.

Reproducible verification: `c15_counterexample.cpp`; output `c15_counterexample.json` for the cycle alone and `c15_inversion_counterexample.json` with the three repairs. Neither example belongs to the block6 null.

## Safe relaxed score bounds

`score_gate_bounds.py` computes upper bounds by relaxing cross-module dependence. It retains:

- exact G parity classes;
- assignment extrema respecting fixed block membership in block6;
- C impossibility of equal counts/pattern when beta is even, and assignment bounds for F counts and the centre equation;
- assignment feasibility of disjoint F/H union cardinality p;
- exact modular matrix feasibility with total columns fixed and `alpha*F_n+beta*F_a=0`;
- the additional matrix restriction from G balance whenever its bit is selected.

Matrix residue combinations are enumerated in `matrix_balance_masks.cpp` and `matrix_free_masks.cpp`. The free enumeration may stop once every view attains mask15, which maximizes that view's fitted conjunction utility; this early exit therefore does not lower the utility upper bound.

The final bound when the **C module is entirely omitted** is:

| Null | Relaxed upper bound without C | Observed primary threshold | Consequence |
|---|---:|---:|---|
| Uniform | 23.797284932250957 | 23.940435757549533 | Every exceedance must use some C module |
| Block6 | 14.531563346975258 | 22.69418342742572 | Every exceedance must use some C module |
| Exact inversion | 23.556276832747162 | 23.203247516669524 | This bound does not settle necessity of C |

The relaxed bounds when C15 is excluded everywhere remain above the observed thresholds. A bound above a threshold is not evidence of a realizable counterexample; the explicit constructions above provide that evidence for two models.

Further block6 exclusions certify that G and W must also participate: no-G upper bound `22.086791549445945`, no-W bound `17.74101671260421`, both below `22.69418342742572`. A G-count-only mask cannot help any positive winning program: its largest feasible utility is `0.7115423357920321`, below the minimum added partition penalty of one bit. Thus the G balance predicate is necessary. The largest W-divisibility-only utility is `4.184169500248655`; even adding this to the no-W upper bound, without charging any extra code cost, is below the target. Therefore a W disjointness/union-cardinality predicate is necessary as well. Their bindings may differ.

`block6_cover.py/json` turns the safe bounds into a finite covering union of positive C conjunctions, allowing every partition and all other bindings. It retains **429 variant/form/prime tuples and852 minimal conjunction components**. The cover is still too broad to identify the entire tail with one natural component. At the natural `(n+a,p=19)` tuple, retained alternatives are C7, C9, or C10 in both variants. No claim of realizability or probability is made for each retained component. A further integer enumeration proves C14 and W0 cannot coexist at p=3,5,7; equal-count and constant-cell restrictions give additional safe C/W pruning.

## Implication for conditional probability estimation

An exact probability and sampler for natural C15 gives a valid way to estimate `P(C15)*P(global exceedance | C15)`. It does not, by itself, estimate the entire global exceedance probability. A complete calculation must establish a covering union of conditional components, or estimate the complement as well. The stronger C11 counterexamples show why this distinction is substantive.
