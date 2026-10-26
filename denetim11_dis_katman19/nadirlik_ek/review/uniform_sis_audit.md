# Independent audit of the uniform natural-127 SIS calculation

## Scope

The target is the fixed natural-form, p=19, 127-count event C15∩W3∩G3∩M15
with matrix view 0. It is not the complete scanned-score tail. The recorded
score components additionally intersect the CW127 anchor and natural G in
at least one count variant. No extrapolation from that component to the
full score tail is justified.

The production sampler adds RNG counters and stored H-row witnesses to the
already audited integer-alias sampler. The diff leaves the F distribution,
H proposal, completion rule and importance weight unchanged.

## Support and trace weight

The independent support check enumerated all 20 possible coefficient bags,
matched every exact F-matching count with the forward enumerator, and checked
the Hall completion rule on 9,631 feasible small count states with 17,559
allowed transitions. The exact total is

\[
M_F=985178375935386285.
\]

Integer alias tables preserve each node's exact outcome masses; their
integer mass identities are checked during construction. Thus each complete
F partial matching has probability 1/M_F. A valid final anchored permutation
has a unique F matching. It has exactly 7! possible orders of its seven H
edges. Every such order is supported: its first six edges are eligible, and
the seventh satisfies the residue condition implied by W3.

After F and H, the remaining 95-by-95 matching forbids equal unsigned
residue colors. Hall feasibility is equivalent to R_g+C_g≤L for each color.
Choosing a row from a color maximizing R_g+C_g and any allowed column
preserves this inequality. Every valid final matching remains reachable
along its uniquely determined completion trace for a fixed H ordering.

For a trace τ ending at a valid permutation x, let A_j be the numbers of
eligible H choices and B_k the numbers of eligible completion columns. Then

\[
q(\tau)=\frac1{M_F}\prod_{j=1}^{7}\frac1{A_j}
                       \prod_{k=1}^{95}\frac1{B_k}.
\]

Assigning each of the 7! traces the extended target mass 1/(114!7!) gives

\[
w(\tau)=\frac{M_F\prod_j A_j\prod_k B_k}{114!7!}.
\]

Summing trace target masses returns the intended uniform labeled-permutation
probability. The 7! denominator is essential. H deadends contribute zero
and remain in the fixed proposal denominator.

## Completed production review

`review_uniform_production.py` independently reconstructs every aggregate
from the two completed 50-million files. Its output is
`uniform_production_review.json`. It verifies the eleven registered source
hashes, all 16 worker sums, all 20 coefficient-bag sums, every raw hit count,
every weight-square sum, and every maximum contribution. All 100 million
proposals, including deadends, are retained.

For each worker's largest target contribution, the reviewer reconstructs
the actual H and completion trace and recomputes the weight using exact
integer products divided by 114!7!. It directly checks the scalar
conditions and matrix orders by matrix exponentiation, independently of
the residue shortcut. All 16 witnesses pass. Their 96 stored score values
also match the unchanged original scorer bit for bit.

All 16 recorded final RNG states reconcile with their exact call totals.
The 16 production counter windows are pairwise disjoint and their initial
states do not coincide with recorded pilot starts. Exact call windows are
not fabricated for older pilots that did not record them. This check does
not establish mathematical independence of deterministic PRNG streams.

## Probability and uncertainty

The independently reconstructed primary result is

\[
\widehat p=7.780716111327623\times10^{-18},\qquad
\operatorname{SE}=9.82735257742\times10^{-19}.
\]

There are 83 nonzero target proposals; their importance weights vary.
Therefore 83/100,000,000 is not the target null probability. The weighted
normal approximation gives the 95% interval

\[
[5.854590399815615,\ 9.70684182283963]\times10^{-18}.
\]

The separate Student-t interval across all 16 equal-size worker batches is
[5.91543159266235, 9.646000629992896]×10⁻¹⁸. These are asymptotic Monte Carlo
intervals, not exact binomial intervals. All 16 workers contributed target
hits; target ESS is 62.6853, relative standard error is 0.126304, and the
largest individual contribution is 3.46220% of the total. All four
predeclared precision diagnostics pass. Passing those diagnostics is
evidence of usable precision under this proposal, not a proof of exact
finite-sample interval coverage.

The point reciprocal is 1.2852287446192021×10¹⁷. Inverting the weighted
normal endpoints gives [1.0302011903058507, 1.7080614213959254]×10¹⁷ for the
expected waiting parameter. It is not a prediction interval for realized
waiting time, and it applies only to the stated fixed natural event under
the uniform null.
