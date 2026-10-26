# Observed score decomposition after freezing the study

This is a descriptive diagnostic. It does not modify any feature, weight, score, selection rule, test budget, or null model.

Each variant-specific maximum retains the original one-bit variant-selection charge, making the two columns comparable to the registered combined score. These are surrogate scores, not new p values.

## Maximum primary score by variant

| Null training model | Variant | Maximum score | Winning program |
|---|---:|---:|---|
| uniform | 129 | 19.885967 | G+C+W: (1,1), p=19 |
| uniform | 127 | 23.940436 | G+C+W+M: (1,1), p=19 |
| block6 | 129 | 18.482053 | G+C+W: (1,1), p=19 |
| block6 | 127 | 22.694183 | G+C+W+M: (1,1), p=19 |
| inversion_exact | 129 | 19.105280 | G+C+W: (1,1), p=19 |
| inversion_exact | 127 | 23.203248 | G+C+W+M: (1,1), p=19 |

## Natural tuple: full positive conjunctions

The natural tuple is form (1,1), p=19, matrix view 0. A conjunction is eligible only when its full mask is true in that variant. Fitted probabilities use the frozen Jeffreys pseudocount; a zero training count is not a claim of zero true probability.

| Model | Variant | Module | Full mask true? | Training successes / n | Fitted probability | Fitted surprise, bits |
|---|---:|---|---|---:|---:|---:|
| uniform | 129 | G | yes | 58 / 200000 | 0.00029249854 | 11.739283 |
| uniform | 129 | C | yes | 0 / 200000 | 2.4999875e-06 | 18.609648 |
| uniform | 129 | W | yes | 79 / 200000 | 0.00039749801 | 11.296765 |
| uniform | 129 | M | no | 0 / 200000 | 2.4999875e-06 | 18.609648 |
| uniform | 127 | G | yes | 67 / 200000 | 0.00033749831 | 11.532832 |
| uniform | 127 | C | yes | 0 / 200000 | 2.4999875e-06 | 18.609648 |
| uniform | 127 | W | yes | 81 / 200000 | 0.00040749796 | 11.260920 |
| uniform | 127 | M | yes | 26 / 200000 | 0.00013249934 | 12.881727 |
| block6 | 129 | G | yes | 91 / 200000 | 0.00045749771 | 11.093948 |
| block6 | 129 | C | yes | 0 / 200000 | 2.4999875e-06 | 18.609648 |
| block6 | 129 | W | yes | 134 / 200000 | 0.00067249664 | 10.538185 |
| block6 | 129 | M | no | 0 / 200000 | 2.4999875e-06 | 18.609648 |
| block6 | 127 | G | yes | 93 / 200000 | 0.00046749766 | 11.062753 |
| block6 | 127 | C | yes | 0 / 200000 | 2.4999875e-06 | 18.609648 |
| block6 | 127 | W | yes | 134 / 200000 | 0.00067249664 | 10.538185 |
| block6 | 127 | M | yes | 27 / 200000 | 0.00013749931 | 12.828288 |
| inversion_exact | 129 | G | yes | 100 / 200000 | 0.00050249749 | 10.958596 |
| inversion_exact | 129 | C | yes | 0 / 200000 | 2.4999875e-06 | 18.609648 |
| inversion_exact | 129 | W | yes | 79 / 200000 | 0.00039749801 | 11.296765 |
| inversion_exact | 129 | M | no | 0 / 200000 | 2.4999875e-06 | 18.609648 |
| inversion_exact | 127 | G | yes | 87 / 200000 | 0.00043749781 | 11.158437 |
| inversion_exact | 127 | C | yes | 0 / 200000 | 2.4999875e-06 | 18.609648 |
| inversion_exact | 127 | W | yes | 80 / 200000 | 0.00040249799 | 11.278731 |
| inversion_exact | 127 | M | yes | 34 / 200000 | 0.00017249914 | 12.501123 |

## Four-module sharing versus separate bindings

All comparisons below keep the four modules activated. The separately optimized program may use different forms, primes and positive-predicate masks for the modules. It therefore does not isolate a pure reuse effect; the fixed-predicate comparison does.

| Model | Variant | Best shared tuple | Best fully separate bindings | Natural identical-predicate shared | Natural identical-predicate separate |
|---|---:|---:|---:|---:|---:|
| uniform | 129 | 14.252636 | 3.484461 | not eligible | not eligible |
| uniform | 127 | 23.940436 | 7.257480 | 23.940436 | 7.257480 |
| block6 | 129 | 12.835556 | 2.128641 | not eligible | not eligible |
| block6 | 127 | 22.694183 | 6.011228 | 22.694183 | 6.011228 |
| inversion_exact | 129 | 13.482535 | 2.801622 | not eligible | not eligible |
| inversion_exact | 127 | 23.203248 | 6.520292 | 23.203248 | 6.520292 |

## Isolated code-cost saving

For all four modules with the same natural predicates and parameters, sharing saves **16.682955 ideal code bits** under the primary mixture policy:

- Three repeated form bindings: 3 log2(14) = 11.422065 bits.
- Two repeated prime bindings: 2[-log2(q(19))] = 5.260891 bits. G alone does not introduce a prime.

Predicate-mask costs, matrix-view cost, activation cost, variant cost, and the partition code are unchanged in this fixed comparison. The saving is a property of the declared code and grammar. It is **not** an independent probability multiplier, and exponentiating it would not produce additional statistical evidence.

Precisely, this saving measures reuse of the form parameters (alpha,beta) and the prime p. L(K), the plus/minus construction, and the module templates are fixed primitives of the grammar. They are reused by construction for every null dataset too. This diagnostic does not compare arbitrary aggregation operators or assign literal source-code lengths to alternative definitions of L.

The condition b=p contributes through the joint C-module frequency. It is not counted again as a separate binding-cost bonus.

For a one-prime natural program, changing the uniform prime policy to the frozen primary mixture changes the prime cost by 2.227536 bits. This is a score-policy effect, not an independent event probability.

## Matrix extension within the 127 convention

Keeping the same 127 data and natural G+C+W program fixed isolates the fitted score contribution of adding M. The additional costs are 4 predicate-mask bits, 3 view bits, and log2(3) partition bits; no extra form or prime is introduced.

| Model | Natural 127 G+C+W score | Natural 127 G+C+W+M score | Net matrix score increment |
|---|---:|---:|---:|
| uniform | 19.643671 | 23.940436 | 4.296765 |
| block6 | 18.450858 | 22.694183 | 4.243325 |
| inversion_exact | 19.287087 | 23.203248 | 3.916161 |

These are within-score contributions. They are not independent probabilities. The 129 variant preserves G, C, and W at the natural tuple but fails the full M conjunction; it therefore does not have the same four-module program.

## Direct verification of strongest saved controls

- uniform: pass. All 6 policy traces reconstructed from 1 direct arithmetic contexts.
- block6: pass. All 6 policy traces reconstructed from 1 direct arithmetic contexts.
- inversion_exact: pass. All 6 policy traces reconstructed from 1 direct arithmetic contexts.

Full module counts, cost ledgers, variant-specific winners, independent-binding programs, and control checks are in observed_decomposition.json. The accompanying script regenerates this diagnostic from the frozen inputs.
