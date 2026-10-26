# Uniform natural-19 anchor: fixed production result

This calculation targets the **fixed natural form `(1,1)`, prime 19, variant 127,
and natural matrix view** under uniform labeled permutations of all 114 IDs.
It does not estimate the entire frozen score tail.

The predetermined production budget was **100,000,000 proposals** in two independently
seeded 50-million runs and 16 worker streams. Every proposal is retained in the
denominator, including H deadends. All pilots are excluded.

## Primary result

For `C15 & W3 & G3 & M15(view0)`, the estimate is
**p = 7.78071611133e-18**, with approximate weighted-moment 95% Monte Carlo
interval **[5.85459039982e-18, 9.70684182284e-18]**.

The expected waiting parameter is **1/p = 1.28522874462e+17**,
with inverse-endpoint interval **[1.03020119031e+17, 1.7080614214e+17]**.
This is uncertainty in the expected waiting parameter, not a prediction interval
for an actual geometrically distributed waiting time.

There were 83 positive importance contributions, target ESS
62.685, relative standard error
12.63040%, largest individual contribution
3.46220%, and
16/16 positive worker batches.
The predeclared precision diagnostics all passed: **True**.

The independent-worker Student t check gives
[5.91543159266235e-18, 9.646000629992896e-18]; its standard error is
0.8905 times the iid weighted-moment SE.
The two 50-million estimates differ by
-0.9851 estimated standard errors.
Both interval methods are approximations. **Neither is a Clopper–Pearson interval.**

## Event-specific estimates

All these are unconditional probabilities under the uniform permutation null,
even though the efficient proposal concentrates on the C/W anchor.

| Event | Positive contributions | Probability | Approximate 95% MC interval | Target ESS |
|---|---:|---:|---|---:|
| anchor_C15_W_127 | 94728465 | 9.36771359052e-12 | [9.36647151337e-12, 9.36895566767e-12] | 68603560.190 |
| G127 | 32642 | 3.22562760767e-15 | [3.18441305813e-15, 3.26684215722e-15] | 23524.551 |
| G127_Mview0 | 83 | 7.78071611133e-18 | [5.85459039982e-18, 9.70684182284e-18] | 62.685 |
| G127_Manyview | 174 | 1.56679267026e-17 | [1.29733502443e-17, 1.83625031609e-17] | 129.879 |
| tail_mixture | 83 | 7.78071611133e-18 | [5.85459039982e-18, 9.70684182284e-18] | 62.685 |

`tail_mixture` is only the intersection of the C15/W3 variant-127 anchor, natural
G3 in either variant, and the complete frozen score exceeding its observed
threshold. It is a component of the full tail. Equal point estimates for this
component and the natural event, if observed, are not an equivalence proof.

## Proposal and checks

Exactly **985178375935386285** eligible labeled F partial matchings were counted with integer
dynamic programming. The 20 coefficient bags were independently enumerated and
their counts verified by a separate forward algorithm. Exact integer alias
tables preserve that distribution. Seven H edges are sampled in order, with
the last edge enforcing the residue sum. A deterministic Hall-preserving row
choice then fills the remaining 95 positions with uniformly chosen eligible
labels. A final valid permutation has one F matching and 7! ordered H traces.

For each successful trace the weight is

`M_F × product(H edge counts) × product(completion label counts) / (114! × 7!)`.

The production F-bag count check has chi-square 23.28659,
19 degrees of freedom, p=0.22495. Every bag was sampled.
An independently constructed marked-F rejection pilot gives anchor probability
8.90012345461e-12, exact scaled binomial 95% interval [7.807455326015576e-12, 1.010290517860571e-11]; the
production SIS anchor estimate lies inside this interval:
True.
The comparator is retained only as validation and is not pooled with production.

All frozen source hashes match, all aggregate worker/bag moments reconcile,
and all 16 measured production random-number counter windows are disjoint.
There were 2000 direct feature checks and
50441 full score evaluations. All retained maximum
target witnesses include their ordered H rows for independent trace replay.

The calculation concerns these exact predicates and null model. It does not
correct for every historical choice that led to proposing them, it does not
assign a probability to the textual position or meaning of the number 19, and
it does not turn any score in bits into a p-value.
