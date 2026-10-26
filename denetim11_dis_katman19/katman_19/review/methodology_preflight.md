# Independent methodology preflight

Status: written before inspecting any new simulation outcomes. This is an audit and design note, not a claim that the final implementation has already passed review. Historical source definitions were inspected; new outcome files were not.

## Study scope

A defensible primary study can be a finite, retrospective replication of a declared shared algebraic grammar. It cannot be described as a correction for every possible historical search. The proposed joint field grammar covers 29 odd primes from 3 through 113, the two selectors n+a and n-a, the two supplied count variants, and 16 independently chosen row/column views of two 2-by-2 matrices. A historical source also searched eight linear forms against every modulus from 3 through 40. Therefore a prime-only, plus/minus study is narrower along those dimensions even if it is broader along other dimensions.

All transformations, feature definitions, score construction, conditional diagnostics, randomization models, seeds, training sizes, test sizes, and stopping rules must be frozen before the fresh outcome files are inspected. The existing observed pattern is already known, so the study is retrospective, not a first discovery on untouched data.

## Reusable objects

- t_sigma = n + sigma*a; u_sigma = n - sigma*a.
- E = {n : n+a is even}. Using n-a gives the identical set because their difference is 2a.
- F = {n : p divides t_sigma}; H = {n : p divides u_sigma}.
- U = F union H, with overlap recorded separately.
- Four cells of F, indexed by t_sigma parity and n parity.
- q = t_sigma/p on F; Q is the 2-by-2 table of cell sums of q.
- L(K) has row one (sum_K n, sum_K a) and row two (sum_notK n, sum_notK a).
- P = L(E) modulo p and R = L(F) modulo p, followed by explicitly coded row/column views.

The common p, common E split, and common L definition are references reused by the program. They are not independently charged at every use. Conversely, unused parameters should not silently generate duplicate candidates or artificial multiplicity penalties.

## Historical arithmetic predicates

The definitions below were read from sources/repo/kesif4_ozgulluk.py and sources/repo/kesif17_mercek_uzayi.py, without running them.

1. |F| >= 2*N/p.
2. F nonempty and p^2 divides sum_F t_sigma.
3. F nonempty; its two t-parity halves have equal cardinality and equal t_sigma sums.
4. F nonempty; both t-parity-half sums are divisible by p^2.
5. F nonempty; the distinct q values form a contiguous integer interval.
6. |F union H| = p.
7. Sum of a over F union H is divisible by p; the nonempty guard should be explicit.
8. F and H are disjoint. The historical code sometimes reports this as an eighth criterion.

The score should identify these by their definitions rather than call an unspecified collection “the seven features.”

## Logical closure that must not be sold as independent evidence

- For odd p, predicates 2 and 3 imply predicate 4. Predicate 4 already implies predicate 2.
- Equal occupancies in all four cells plus Q = [[p+1,p-1],[p,p]], allowing the declared cell orientation, imply predicates 2, 3, and 4.
- A q table centred on p implies sum_F t_sigma = 4*p^2; this is a consequence of the table, not an additional success.
- t_sigma and u_sigma always have the same parity.
- If sum_E n = sum_notE a, P has equal diagonal entries before modular reduction. That matrix symmetry is forced by the scalar balance.
- Every 2-by-2 matrix commutator has trace zero and its square is scalar. The actual extra matrix predicate is that this scalar equals one.

A joint-event frequency handles these implications automatically. A predictive score based on feature weights must either account for this closure or explicitly avoid interpreting its sum as independent evidence or an exact joint log probability.

## Matrix reduction and feasible lookup table

For the scalar core B: sum_E n = sum_notE a, put d = sum n modulo p, s = sum a modulo p, y = sum_E n modulo p, and x = sum_F n modulo p. Then

P = [[y,s-y],[d-y,y]],
R_plus = [[x,-x],[d-x,s+x]],
R_minus = [[x,x],[d-x,s-x]].

The four matrix flags can therefore be precomputed over (p, variant, sign, view, x, y) if the matrix extension explicitly requires B. This is a permissible finite-grammar restriction and must be named. It is not an exhaustive test of matrices formed when B fails.

An exact order-p^2-1 check requires M^(p^2-1)=I and M^((p^2-1)/r)!=I for every distinct prime factor r of p^2-1. Merely testing M^(p^2-1)=I is insufficient.

## Shared centre versus free centre

To measure reuse of p beyond membership selection, distinguish the branch Q = [[p+1,p-1],[p,p]] from Q = [[b+1,b-1],[b,b]] with free integer b. Both branches should have explicitly declared orientation handling. The latter's b can be inferred from Q, so enumeration over every possible b is unnecessary. If an MDL interpretation is used, the free literal b must carry a valid integer code, whereas b=p is a reference to an already introduced parameter.

The existing pattern motivated this grammar; the code cost is therefore a finite within-study comparison, not an objective universal description length or a complete bill for historical human choices.

## Score and Monte Carlo calibration

For a fixed predicate program h and a valid conditional prefix code, the ideal event scan is

T(D) = max_{h: D in A_h} [-log2 P0(A_h|Z,O) - L(h|Z,O)].

If sum_h 2^(-L(h|Z,O)) <= 1, then P0(T>=t|Z,O) <= 2^(-t). No independence assumption is needed. Only satisfied predicates are eligible. Unknown event probabilities cannot be replaced by unconstrained plug-in estimates while retaining this analytic guarantee.

With finite training data, complete rare conjunctions can all be unseen. Assigning all of them the same pseudocount probability makes their information score saturate and can cause the winning program to ignore later layers. A feasible alternative is a clearly labelled predictive ranking score learned from independent training simulations, using module or prefix conditional frequencies. Its inferential calibration is the independent full-search Monte Carlo test, not an analytic joint-probability claim.

p_global = (1 + number of independent test scores at least the observed score)/(B+1).

The same fixed search algorithm and computational effort must be applied to observed and null data. Ties use >=. Training and test streams must be distinct. A zero exceedance count does not imply a zero probability. Fixed budgets must be declared; ordinary rank formulas do not automatically permit outcome-driven stopping.

## Textual side information

Textual salience can determine a normalised weight q_Z(p), or select a prespecified target p. The mention of 19 is constant under a length-assignment null and is not an additional Bernoulli success to multiply into the structural result. If 19 was already fixed on textual grounds, assigning that same reason again cannot create additional independent evidence.

Z must be a defined semantic feature profile invariant under the randomization, not the complete ordered and segmented text. Conditioning on the latter would determine the count sequence and leave no nontrivial permutation distribution. If chapter/verse coordinates are themselves tested, they must be preserved or recomputed consistently.

Weights chosen after looking at structural success are another search choice. They need explicit accounting or a sensitivity analysis; they are not automatically data-independent merely because they have a textual interpretation.

## Null models and variants

- A uniform shuffle is an assignment-exchangeability null conditional on the preserved multiset.
- Exact inversion matching requires uniform sampling on the exact statistic fibre, or a justified exact conditional procedure. “Similar inversion count” does not suffice. Prespecified bins are preferable to an observed-centred tolerance window.
- Fixed-block shuffles assume exchangeability within the declared blocks. Data-chosen boundaries require accounting.
- Apply the same underlying record permutation to the 127 and 129 variants so their dependence is preserved.
- If testing robustness to a union of justified null models, max_j p_j is a conservative intersection-union p value. Taking the minimum gives a different, selection-sensitive claim.

## Required secondary diagnostics

Report the global complete-search p value, nested conditional extension rates, and sensitivity across variants and justified null models. Conditional matrix-extension rates answer a different question from total-event rarity and must not replace it. Uniform counting over the algebraic (x,y) grid is not a probability model for the actual permutation-induced residues.

When the conditional denominator is small, report it and the interval or resolution. A computational limit is a resolution limit, not evidence of impossibility or an invitation to extrapolate a smaller p value.
