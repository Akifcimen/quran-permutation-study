---

## English summary

**Question.** How rare is the Quran structure's *cross-layer recurrence* — the same balanced binary split at every level, the same
modulus returning outside its definition, two disjoint "hands", consecutiveness at two levels — measured fairly against random
arrangements? (Task `GOREV.md`; study folder `~/opus5.5/butunluk`; `~/Desktop/hrf2` was only read.)

**Pre-registration.** `ONKAYIT.md` was written and frozen (2026-10-26T17:13:24Z, SHA-256 `d213dc89…fd513`) **before any random book's
integrity score was computed or seen**; only the Quran's own profile/count score and code validations were done before freezing.
Deviations are in timestamped addenda (`ek/`); no definition, weight, threshold or budget was changed after scores were seen.
One procedural deviation is documented: in the uniform null, one ambiguous book was resolved out of S_alt order, and the five
lookups of two higher-ranked books were not run (effect: upper ambiguity count −1).

**Language L.** Lenses (α, β, p): t = αn + βa, u = αn − βa, the external study's 14 forms × 29 primes 3–113 (406 lenses). Primary
split: t-parity × n-parity. 20 predicates in four motif types — (a) split balance (book 57/57 and book-level cross balance; family
halves equal count/sum; cells equal count/sum per row), (b) modulus return (centre Σq = 4p, p | Σq, p | half sums, family/H/union
size = p, p | Σa(U), p | Σn(U), p | Σ(u/p)), (c) two hands (F ∩ H = ∅), (d) consecutiveness (cell sums, coefficient band). No
Quran-specific constant appears. Proven logical implications are pruned (e.g. K1 ∧ Qc ⇒ F2, R1e); no unproven candidate rule remained.
**Count score:** number of non-implied predicates at the best lens, family/cell predicates only if k_F ≥ 2E and every cell ≥ 2 (gate G).
**Information score (primary):** −log₂ of the joint frequency of the satisfied predicate masks (book; within-family | k_F; two hands |
k_F; size event k_F = p), from an independent 10⁸-book reference sample per null, minus a pre-registered lens description cost
(simplicity prior). Unresolved lookups (< 10 reference hits) are carried as [lower, upper] bounds — no floor saturation — and the
highest-ranked ambiguous books were re-scored by dedicated estimation (exact conditional importance sampling for the uniform null, direct
single-lens sampling for the ordered nulls).

**Quran.** Best lens in every null, score and variant: **(1, +1, 19)** (t = n + a); 15 of 20 predicates; **count score 11**;
information score 31.06 bits (uniform), 27.41 (M20), 27.27 (Mallows), 25.98 (J10).

**(1) Rank** p = (k+1)/(N+1), 10⁸ test books per primary null (count score: 2×10⁸ incl. reference books):

| Null | information score: p_point [ambiguity interval] | count score (gate G) |
|---|---|---|
| M20 (primary ordered) | [M129_BILGI] [M129_BILGI_ARALIK] | [M129_SAYIM] [M129_SAYIM_CI] |
| Mallows | [L129_BILGI] [L129_BILGI_ARALIK] | [L129_SAYIM] [L129_SAYIM_CI] |
| Uniform | [U129_BILGI] [U129_BILGI_ARALIK] | [U129_SAYIM] [U129_SAYIM_CI] |
| J10 (secondary, 10⁷) | [J129_BILGI] [J129_BILGI_ARALIK] | [J129_SAYIM] [J129_SAYIM_CI] |

In the length-ordered nulls the Quran's integrity is at the **~3×10⁻⁷ level** under both scores; uniform ~4×10⁻⁸ (information).
The upper end of the ambiguity interval (~1.6–1.8×10⁻⁴) comes from 16–18 thousand books carrying masks never seen in the reference
sample (infinite upper bounds); almost all of them lie far below the threshold (median 19–22 bits short) — it is not a frequency
estimate. Codex 127: same order of magnitude.

**(2) Rivals.** Information score: only 0–2 of the top 20 per null pass the pre-registered "Quran class" rule; the top is held by
single rare events (family size = p, book-level cross balance) — scattered coincidences. Count score: all top-20 pass the class rule,
but in the uniform null they are degenerate (8-member zero-sum families at large p — the rule does not catch coefficient degeneracy;
a post-hoc zero-sum-excluded score S0 is reported: uniform 3×10⁻⁸, M20 9×10⁻⁸, Mallows 6×10⁻⁸). In the ordered nulls part of the
rivals are genuinely of the Quran's kind: **12-member 3/3/3/3 families at the Quran's own lens (1, +1, 19)**, with consecutive cell
sums, a coefficient band, mostly k_H = 7 and |U| = 19 — but **none has the centre Σq = 4·19 or the book-level cross balance (terazi)**.

**(3) Given the old literature.** Given O (12-member family, 1444, 6/6, 722/722, 361/361) the remaining layers rank 5.0×10⁻⁶ (uniform,
k = 0) to 4.8×10⁻⁵ (Mallows), mostly due to the 1994 balance (terazi). Given O' = O ∧ terazi — only this study's additions remain
(even-t cells 18/20, the 5–9 band, 3/3/3/3, the two hands) — 1.0×10⁻³ (uniform) to 3.4×10⁻³ (ordered) by information score,
4.1×10⁻⁴ – 1.6×10⁻³ by count; ~3×10⁻² with a free lens (sensitivity).

**(4) The main line's pre-registered prediction — partly held.** Ordered nulls (M20, Mallows): **held** under both scores (inside
[10⁻¹⁰, 10⁻⁶], but 1.4–1.7 orders of magnitude above the predicted centre 10⁻⁸). Uniform: **did not hold** (2.6–3.7 orders less rare than
the predicted 10⁻¹⁰–10⁻¹²). Old-literature conditional: with the task's primary condition O mostly **did not hold (rarer)**; with O',
which matches the prediction's wording ("only this study's additions"), **held**.

**(5) Not measured:** the effect of having written the language after seeing the Quran (the predicates name its known layers; lens
cost and size-conditioning do not price predicate choice); the choice of lens family and modulus range; the effect of the
between-group independence approximation for books other than the Quran; literature search, counting school, cultural pool; the
partition menu with the information score; rare-event (subset-simulation) tails (not needed: k ≥ 1 everywhere); dedicated resolution
of all ambiguous books (budget; reported as intervals).

**(d) Partition menu** (count score only; N = 10⁷ per null). Letting every random book choose the best of 20 ordered partition pairs
raises the Quran's rank by only a factor of 1.3–2.5, compared with the fixed (t-parity, n-parity) pair on the same sample: uniform
5.0×10⁻⁷ → 9.0×10⁻⁷, M20 4.0×10⁻⁷ → 5.0×10⁻⁷, Mallows 4.0×10⁻⁷ → 1.0×10⁻⁶ (3–9 hits, so the ratio is uncertain). The Quran's best pair
is already the primary one. If t-parity is removed from the menu for everyone, the Quran's best scores 9 and its rank rises to
1.6–2.7×10⁻⁴. So the Quran's advantage is tied to its own partition type, but it is not an artefact of choosing that type from the menu.
Predicate choice remains uncalibrated.
