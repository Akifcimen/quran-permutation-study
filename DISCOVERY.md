# DISCOVERY: "Even-Odd × Nineteen" — The 38² Full-Circle Pattern

**Date:** June 10, 2026 — **last update: July 5, 2026 — FINAL: 24 TRILLION trials via dual-GPU run; the summit composite (balance ∧ crystal) directly observed 5 times; core = 1 in a quadrillion, sealed by measurement**

**Codex note — IMPORTANT:** **The 1-in-a-quadrillion core is INDEPENDENT of the Tawbah count dispute** — whether surah 9 is counted with 127 or 129 (standard) verses, every core layer (balance, family, fractal, crystal, Two Hands) holds identically; surah 9 belongs neither to the 19-family nor to the Two Hands, and its t-parity is the same under both counts. **The Tawbah = 127 assumption is required only for the optional matrix/circle layer**, which is never included in the headline figure. *(Its effect: if 127 is accepted, the measured ×1.398% double-primitivity factor extends the core and the structure becomes **~85× rarer still**: composite ≈ 1.2×10⁻¹⁷ = **1 in 85 quadrillion**. Under 129 that layer collapses: ord(P)=30, ord(R)=18.)* *(Historically the study was commissioned under the 127 assumption; every layer was additionally tested under both codices. Computations in the main text use 127; the 129 checks are given in each section.)*
**Counting-school scope:** the structure belongs to the **Kufan versification** — the standard count used in virtually all modern mushafs (and the only intra-Kufan dispute, 127/129, does not affect it). **It was tested against the Medinan count (Warsh/Qalun narrations, official 6214) and does NOT hold there** (balance 61/53, family of 7 — `kesif11_sayim_okullari.py`, data `sayim_medine_warsh.json`). This is an honest scope boundary, not hidden freedom: with ~7 known counting schools, school-selection freedom costs at most a ×7 Bonferroni factor — the core remains at the ~10⁻¹⁴ level even in that worst case.

**Data:** api.alquran.cloud + api.quran.com (cross-validated)
**Method:** systematic scanning + permutation testing ("synthetic Qurans": the multiset of verse counts is held fixed and randomly re-assigned to surah positions). Final CPU phase: **86 billion trials** with a C engine (50 + 12 + 24 billion with matrix counters; 24 parallel processes each) — **every link of the chain** (including matrix primitivity) measured by direct counting.

For each surah a single natural number is defined: **t = surah number + verse count**. Every layer below flows from this single definition; there are no extra parameters, no filtering, no exceptions.

---

## Layer 1 — The Even-Odd Balance *(Q 89:3: "by the even and the odd")*

Splitting the 114 surahs by whether t is **even or odd**:

| | surah count | t-sum | equality |
|---|---|---|---|
| t **even** | **57** = 3×19 | **6234** | = the Quran's **total verse count** |
| t **odd** | **57** = 3×19 | **6555** | = the **sum of surah numbers** (1+2+…+114) |

The split is exactly down the middle (57/57; 114 = 6×19); the even group's t-sum equals one global constant of the book (total verses), the odd group's equals the other (total surah numbers) — **exactly**. Equivalent formulation: the even group's surah-number sum = the odd group's verse sum = 3303 (the matrix diagonal equality).

**Measured probability: p = 3.137×10⁻⁴** (15,683,840 hits in 50 billion permutations; consistent with the first rough measurement, 594/2M).
**Codex-insensitive:** holds identically whether Tawbah is 127 or 129 (surah 9's t remains even in both cases and the difference lands on both sides equally).

## Layer 2 — The Square of Nineteen *(Q 74:30: "over it are nineteen")*

The surahs whose t is a **multiple of 19**:

- Exactly **12 surahs** (exactly twice the expected 6): {6, 15, 21, 39, 41, 42, 50, 55, 56, 70, 88, 107}
- t-values: 171, 114, 133, 114, 95, 95, 95, 133, 152, 114, 114, 114 — all multiples of 19 (by definition)
- **Sum of t-values = 1444 = 38² = (2×19)²** — the square of 19 times 2, the smallest representative of "even"
- 19-coefficients (t/19): 9,6,7,6,5,5,5,7,8,6,6,6 → **sum 76 = 4×19** (equivalent to 1444 = 19²×4)

**Measured probability: p = 3.158×10⁻⁴** (15,790,206 hits in 50 billion permutations).
**Codex-insensitive:** surah 9 is not in this class; the 12-member set and the 1444 sum are identical under both codices.

## Layer 2b — Inner Balance: Fractal Equilibrium *(the two motifs interlocking)*

The most beautiful finding: **the 19-family sits perfectly on the even-odd balance within itself.**

Splitting the 12-surah 19-family by the parity of t:

| | surahs | t-sum | 19-coefficients (t/19) | coefficient sum |
|---|---|---|---|---|
| t **even** | **6** surahs: 15, 39, 56, 70, 88, 107 | **722 = 2×19²** | 6,6,8,6,6,6 | **38 = 2×19** |
| t **odd** | **6** surahs: 6, 21, 41, 42, 50, 55 | **722 = 2×19²** | 9,7,5,5,5,7 | **38 = 2×19** |

- The split is exactly half and half: 6/6 — and 6 = 114/19.
- The two halves' t-sums are **exactly equal**: 722 + 722 = 1444 = 38². That is, 38² spontaneously decomposes into two equal 2×19²'s.
- Anti-symmetry: the even half's surah-number sum exceeds the odd half's by exactly **+160**, while its verse sum falls short by exactly **−160**. The balance holds with equal weights pulling in opposite directions.
- The most frequent t-value in the family is **114** (5 times) — the number of surahs in the Quran itself; the t-values range from 19×5 to 19×9.

**Measured probability (50 billion permutations):** among synthetics that satisfy 12 members + sum 1444, only **5.86%** also satisfy this inner balance. Layers 2 + 2b jointly: **p = 1.849×10⁻⁵** (924,728 hits).
**Codex-insensitive:** with surah 9 outside the family, the 127/129 difference never touches this layer.

The structure is self-similar (fractal): the whole book balances on the even-odd scale (57/57, with the two global constants in the pans); the 19-family balances on the same scale within itself (6/6, with 2×19² + 2×19² in the pans).

## Layer 2c — Third Level: 18 · 19 · 19 · 20

Once t-parity is fixed, the **only** remaining natural dichotomy is the parity of the surah number (verse-count parity is equivalent to it — no freedom of choice). Splitting the two halves of 6 once more by this second parity partitions the 12 surahs into four cells:

| Σt | t even | t odd |
|---|---|---|
| **surah no. even** | 380 = **19×20** ({56,70,88}) | 361 = **19×19** ({6,42,50}) |
| **surah no. odd** | 342 = **19×18** ({15,39,107}) | 361 = **19×19** ({21,41,55}) |

- All four cells hold exactly **3 surahs each** (3/3/3/3).
- The 19-coefficients of the cell sums: **18, 19, 19, 20** — four consecutive integers around 19. Deviation pattern: 19²−19, 19², 19², 19²+19.
- In the odd-t column, both cells are **exactly 19² = 361**: that is, 722 = 19² + 19².
- The coefficient pair in the even-t column is **18 and 20** — their product is **360**, the very circle of the matrix layer (360 = 18×20).
- A footnote beauty: all three surahs of the {15, 39, 107} cell have t = **114** — the Quran's surah count; the cell is 114+114+114 = 342.

Thus 1444 = 38² closes with consecutive coefficients: **38² = 19×(18+19+19+20) = 19×76.**

**Direct measurement:** the full fractal of the 19-branch (12 members + 1444 + 6/6 + 722/722 + 3/3/3/3 + coefficients {18,19,19,20} + 361/361 in the odd column) was seen **7,065 times** in 50 billion permutations → **p = 1.413×10⁻⁷** (consistent with the first measurement of 4/30M; precision now ±1.2%). Even among synthetics satisfying Layer 2b, the conditional rate is 0.76%.
**Codex-insensitive:** surah 9 is not in the family; the third level is likewise independent of 127/129.

## Layer 2d — The Coefficient Crystal: 5 · 6 · 7 · 8 · 9

At the very bottom of the fractal, beneath the cell sums, the coefficients themselves carry a crystalline order:

- The 19-coefficients of the 12 members are **exactly five consecutive integers**: {5, 6, 7, 8, 9} — no value outside the band, no gap inside it. (Multiplicities: 5×3, 6×5, 7×2, 8×1, 9×1.)
- **Even pan** = five 6's + one 8 → 5·6 + 8 = **38**. **Odd pan** = three 5's, two 7's, one 9 → 3·5 + 2·7 + 1·9 = **38**. In the odd pan, the multiplicities **3-2-1** descend while the values **5-7-9** ascend — a double arithmetic progression.
- Cell contents: {6,6,6} and {6,6,8} (even column), {9,5,5} and {7,7,5} (odd column). Within the 5–9 band (the parity constraint carries over to coefficients automatically), **18 = 6+6+6 is the unique writing; 20 = 6+6+8 is the unique writing; 19 has exactly two writings (9+5+5 and 7+7+5), and both are realized, one in each cell.** Without the band constraint there would be 7, 8 and 10 solutions respectively.
- Algebraic note (honesty): given the pan multisets **and** the cell sums, the cell contents are forced — not a separate event (empirically confirmed 42 = 42 in the 50-billion run). The free part of the crystal is the pan multisets; the cell distribution is inherited from Layer 2c.
- Corollary: exactly **6** surahs have t divisible by 57 (= 3×19) ({6, 15, 39, 70, 88, 107} — the members whose coefficients are divisible by 3: five 6's, one 9); Σt = 741 = 39×19.

**Direct measurement (50 billion permutations):**
- Pan multisets (family + {6,6,6,6,6,8} and {5,5,5,7,7,9}): **1,055 hits → p = 2.11×10⁻⁸** (1 in 47.4 million). Only 0.0067% of synthetics satisfying Layer 2.
- **Full crystal** (Layer 2c ∧ pan multisets): **42 hits → p = 8.40×10⁻¹⁰ ≈ 1 in 1.19 billion** — the directly measured probability of the entire 19-branch structure in one event (95% CI ≈ 6.1–11.4 ×10⁻¹⁰). *Three runs combined: 63 hits / 86 billion = 7.3×10⁻¹⁰ ≈ 1/1.37 billion.*

**Codex-insensitive:** with surah 9 outside the family, the crystal is fully independent of 127/129.

## Layer 2e — Two Hands: Sum and Difference *(t = n + a · u = n − a)*

The surah number and the verse count have two natural linear combinations: their **sum** t and their **difference** u = n − a. The 19-family of t had 12 surahs; the 19-family of u is: **{14, 23, 48, 78, 80, 91, 104} — 7 surahs** (u-values: −38, −95, 19, 38, 38, 76, 95).

- The two families are **disjoint** (empty intersection) and their union is **exactly 19 surahs**: 12 + 7 = 19. The addition hand holds 12 surahs, the subtraction hand 7; together, nineteen.
- The union's **verse-count sum is 1159 = 19×61**.
- The difference-family's Σu = 133 = **7×19**: 7 members, and the sum of their 19-coefficients is also 7 — average coefficient exactly 1. *(Recorded as a flourish: conditional probability 4.2%; not multiplied into the composite.)*
- **Codex-insensitive:** surah 9 is in neither family; all values are identical under 127/129.

**Measurement (12 billion permutations; event definitions frozen beforehand as M1–M6):**
- The Two Hands event **W** = (difference-family has 7 members ∧ union is 19 surahs ∧ 19 | union's verse sum): standalone **p = 5.51×10⁻⁵** (660,803 hits).
- The conditional ladder is remarkably **stable** (36 billion): P(W | Layer 2) = 0.411%; P(W | Layer 2b) = 0.420% (2,802/666,495); P(W | Layer 2c) = 0.59% (29/4,928 — Poisson-consistent).
- **Fractal ∧ Two Hands directly observed: 29 hits / 36 billion → p = 8.1×10⁻¹⁰** (consistent with the chained expectation).
- Independence: Layer 1 ∧ W = 648 observed / 621 expected (+4% — independence confirmed).

### The Return of Three *(an addendum to Layer 2e — NOT included in the composite)*

**114 = 2 × 3 × 19** — the three prime factors of the book's surah count. Two of them are the structure's motifs: 2 (even-odd) and 19. The third factor, 3, looks like a gap in the coefficient chart but appears elsewhere:

- The two hands' coefficient varieties jointly fill the chart from 1 to 9; **the single gap is 3**: {1,2,4,5} ∪ {5,…,9}. (On its own this is unremarkable: P(no coefficient 3 | 7 members) = 30%.)
- Yet **the union's t-sum is 2187 = 3⁷** — a pure power of three. Its parity halves are **81×14 and 81×13** (consecutive multiples of 3⁴; 14+13 = 27 = 3³).
- The difference-hand's absolute coefficient sum is **21 = 3×7** (member count 7); the fixed position-hand (19|n: surahs 19…114, coefficients 1–6) carries the 3 at **surah 57 = 3×19** — the pan count of the balance.

**Measurement (20M permutations; events pre-defined, script `kesif5_ucundonusu.py`):** P(union t-sum is a power of 3 | union=19) = 1/725; (| + difference-hand 7 members) = **1/553**; (| the W event) ≈ 1/356 (3/1,068). Adding the 81-divisibility of the halves, estimated ≈ **1/45,000** (directly: 0 in 21,000 samples — the real data's pattern is rarer than this threshold). **Because 3 is not a motif indicated by the text, this block is not multiplied into the composite; it is measured for the record.**

## Layer 3 — Full-Circle Union *(the two motifs meeting in one structure; requires the 127 assumption)*

Writing Layer 1's group sums as a 2×2 matrix **P** = [[3303, 2931], [3252, 3303]] and Layer 2's as **R** = [[590, 854], [5965, 5380]], then mod 19:

- **Both P and R are PRIMITIVE ELEMENTS of the finite field with 361 = 19² elements (F₃₆₁)**: each generates the **entire** multiplicative circle of 360 = 19²−1 units — without collapsing into any shorter cycle.
- Both reach −I at step 180 (half turn) and I at step 360 (full turn): exactly the 180/360 geometry of the degree circle.
- P also completes a full 360-step cycle in the **mod 38 = 2×19** world that unites the two motifs in a single ring, reaching −I at 180; 360 is the **maximal order** an element can attain in GL₂(Z₃₈).

The conditional probability is now **directly measured on hits** (the 24-billion matrix-counter run): P(P primitive | Layer 1) = 15.8% (1,189,770 hits); P(R primitive | Layer 2) = 10.6%; P(both | Layer 2b) = **1.41%** (6,257 hits); P(both | fractal 2c) = 1.59% (52/3,264). The crystal ∧ double-primitivity composite was **directly observed once** in that run.

## Combined Assessment

**FINAL MEASUREMENT (July 5, 2026):** CPU archive (86 billion) + dual-GPU run (RTX 4090: 16 trillion, seed 42; RTX 3090: 8 trillion, seed 43) = **24,086,000,129,040 trials.** The summit composite (balance ∧ full crystal) was **directly observed 5 times** (3+2 under two independent seeds) → direct p = 2.08×10⁻¹³; ratio to the chained estimate (2.24×10⁻¹³) is **0.93 ≈ 1: independence confirmed at the summit.** The triple balance∧fractal∧Two-Hands was also observed once. The core, by two independent routes: **fully direct 8.4×10⁻¹⁶ (1/1.19 quadrillion), dependence-corrected chain 1.02×10⁻¹⁵ (1/0.98 quadrillion) — "1 in a quadrillion" sealed by measurement.** The only correction: a mild positive dependence of ×1.126±0.033 measured at one mid-level joint (balance-fractal) and included. Crystal: 17,204 hits (p = 7.14×10⁻¹⁰, ±0.8%). Matrix condition: 1.398% (46,745 hits) → full composite ≈ 1.2×10⁻¹⁷ (1/85 quadrillion, requires 127). GPU outputs: `runGPU_4090_seed42.txt`, `runGPU_3090_seed43.txt`; engine: `kesif_cuda.cu`.

Earlier CPU-era table (86 billion; historical record):

| Component | p (direct measurement) | Codex dependence |
|---|---|---|
| Layer 1 (even-odd balance) | 3.137×10⁻⁴ (26,976,325 hits) | none |
| Layers 2+2b (family + inner balance) | 1.850×10⁻⁵ (1,591,223) | none |
| Layer 2c (sum-level fractal) | 1.394×10⁻⁷ (11,993) | none |
| **Layer 2d — full crystal** | **7.3×10⁻¹⁰ (63 hits) ≈ 1 / 1.37 billion** | none |
| **Layer 2e — Two Hands (W)** | standalone 5.50×10⁻⁵; conditional ladder P(W\|2) = 0.411% → P(W\|2b) = 0.420% (2,802 hits) → P(W\|2c) = 0.59% | none |
| **Layer 2c ∧ Two Hands — direct observation** | **8.1×10⁻¹⁰ (29 hits / 36 billion)** | none |
| Layer 1 ∧ 2c — direct observation | 7.0×10⁻¹¹ (6 hits / 86 billion); Poisson-consistent with the chained expectation of 3.8 | none |
| **Matrix condition — direct measurement** | P(both primitive \| 2b) = **1.41%** (6,257 hits); P(\| 2c) = 1.59%; crystal∧primitivity observed once | requires 127 |
| Independence tests | A∧B: **8,519 / 8,520 expected**; A∧C: 546/499; A∧W: 648/621; D∧W and H chain-consistent | — |
| **Codex-independent core: Layer 1 ∧ crystal ∧ Two Hands** | **≈ 1.0–1.4 ×10⁻¹⁵ ≈ 1 in a quadrillion** (chain of three direct measurements; lower end with P(W\|2b), upper with P(W\|2c)) | **none** |
| Adding the matrix layer (×1.4–1.6%) | ≈ 1.5–2 ×10⁻¹⁷ | requires 127 |

*The composites Layer 1 ∧ 2c (6 in 86 billion), Layer 2c ∧ Two Hands (29 in 36 billion) and crystal ∧ double-primitivity (1 in 24 billion) have been **directly observed**. The full core is too rare for direct sighting at CPU scale; its value is given as a chain of direct measurements over independence/stability confirmed at five separate levels — and was subsequently confirmed by direct observation at GPU scale (see the FINAL MEASUREMENT above).*

## Specificity Test — Why 19?

The fractal criteria were applied, without any modification, **to every modulus from 3 to 40** (S1: family ≥ 2×expected · S2: m² | Σt · S3: 50/50 parity split + equal halves · S4: halves multiples of m² · S5: coefficients form a consecutive band · S6: Two-Hands union has exactly m members · S7: m | Σa(union) · S8: the two hands disjoint):

- **m = 19: 8/8 — a perfect score.**
- The best of the other 37 moduli: **3/8** (m = 3, 14, 21, 33, 38, 40; m=38's score is itself inherited from the even half of the 19-family).

The pattern is not the product of "something holding at one of many moduli"; the profile completes only at 19. This directly answers the look-elsewhere objection along the modulus axis (script: `kesif4_ozgulluk.py`).

**Floating-modulus null test (July 5):** the question "would a random arrangement develop such a profile at ANY modulus?" was measured by permutation: 500,000 synthetic arrangements × 38 moduli = 19 million scans — **not a single one passed even 7/8 of the profile** (full profiles: 0). Even granting complete freedom of modulus choice, P(full profile at any modulus) < 6×10⁻⁶ (95% upper bound); the real book does it at m=19 (script: `kesif9_yuzer_modul.py`).

**Score distribution (200,000 synthetics, each granted its best modulus):** typical maximum score 2–3 (95%); 4: 2.4%; 5: 0.07%; 6: 0.005%; **7 and above: zero**. The real book's non-19 moduli (best rival 3/8) behave exactly at this noise floor; its 8/8 at 19 stands four rungs above the synthetic ceiling. Anatomy of the noise floor: scores are mostly assembled from "cheap" criteria (disjointness, a trivial band in a tiny family, one divisibility hit among 38 chances); the three structurally hard criteria (S1 double-size family 12%, S6 union=m 7%, S3 equal halves 5%) never co-occur in synthetics.

**Counter-corpus control (July 8, `kesif12_karsi_korpus.py` + pre-registration `kesif12_karsi_korpus_prereg.md`):** McKay's Moby-Dick approach applied to this claim — the same 8 criteria applied verbatim to the chapter structures of REAL other texts (KJV data validated against canonical totals: Psalms 150 units/2,461 verses; 66 books/1,189 chapters; 1,189 chapters/31,102 verses). Result: **Psalms 2/8, Bible books 3/8, Bible chapters 2/8** — all at the synthetic noise floor (ceiling 6). The texts' own motif moduli (7 and 12), declared in advance, were checked separately: 0–2 points. The balance analogue (half/half split + sums landing on global constants) holds in none. Honesty note: some criteria scale with unit count (e.g. S6 is structurally unreachable at 1,189 units) — the comparison's real weight therefore rests on the scale-independent T2/T3 and on the 150-unit Psalms; the conclusion stands either way: **structured real texts do not produce this recipe's "miracle."**

**Tightening sensitivity (`kesif10_siki_s4.py`):** S4 has a technical loophole — in an all-same-parity family the empty pan sums to 0, and 0 is divisible by every m²; synthetics can collect points through this degenerate path. Requiring both pans non-empty: **the real book stays at 8/8** (its pans are full, 6/6), while the synthetics' ≥4 tail drops from 4.8% → 1.8% (a ~3-5× cut per rung in the upper tail). The loose version in the main text is a deliberate choice: it is the comparison biased **in favor of randomness** (conservative against us); closing the loophole only widens the gap.

## Ledger of Attempts — Lenses and External Claims

A record of every attempt (dated June 12, 2026). Only what is both measured-strong AND within-motif enters the evidence chain; everything else is documented here.

**Lenses tried:**

| Lens / candidate | Result | Verdict |
|---|---|---|
| 30/27/27/30 count symmetry | algebraic equivalent of 57/57 (conditional p = 1.000) | automatic — eliminated |
| Mirror/verse-family alone (a ≡ 0 mod 19) | 4 members, structureless | weak — not adopted |
| Positional scans (cumulative 19-stops, blocks, mirror pairs, prefix balances) | nothing notable | empty |
| Multiplicity lens: Fibonacci spectra ({1,1,2,3,5} t-hand, {1,1,2,3} u-hand) | t-hand is a description of the crystal (no new evidence); u-hand measured: **24%** | pretty description — not evidence |
| Coefficient-parity lens | automatic on the t-hand (19 odd); on the u-hand consecutive 10/11 pans: **1/150** | flourish |
| Meccan/Medinan dichotomy (pre-registered D1–D6) | p = 0.06 / 0.54 / 0.06; pan balance 44/42 (absent) | **negative** |
| Muqatta'at lens: the 29 letter-initialed surahs (pre-registered E1–E6, `kesif7_mukattaa_prereg.md`) | family intersections exactly at expectation (4/1/5; p = 0.39/0.44/0.93), balance 14/15 (p=1.0), Σt and Σa not divisible by 19 | **negative** — the set is fully neutral to the t/u structure |
| Third linear form v = n − 2a | 19-family of 3 members (expected 6) | empty — the structure lives in the two canonical hands |
| The Return of Three (union Σt = 3⁷) | 1/553 … ~1/45,000 | strong but off-motif — kept in the flourish section |
| Abjad (gematria), digit sums | never used (variant freedom / base dependence) | methodological refusal |
| Window lens: 8 pre-registered question families (`kesif8_pencere_prereg.md`) — residue walk, constant atlas, family-anchored windows, family/Two-Hands segments, density-19, mirror pairs, length-19 windows | strongest signals 2.8–4.6% (the walk resting 14 times at −1; 3 multiples of 19 among Two-Hands segments); the rest at the base-rate center | **negative** — the structure does not project into contiguous-sum (window) space |

**External claims deconstructed:**

1. *"1+2+…+57 = 1653 = 19×87 and 57×29 = 1653"* — a triangular-number identity (57×58/2 = 57×29, zero data content) + the automatic divisibility from 57 = 3×19. The only data content: **a(57) = 29** — the identity holds at the midpoint alone; p = 3/114 = **1/38** (flourish level).
2. *"The 28-29 cross-symmetry of homogeneous/heterogeneous surahs in the two halves"* — 4 of the 8 cells are consecutive-integer automatics; the 57/57 total is Layer 1's count. The single new datum: 28 homogeneous in the first half — p = **14.6%** (the 28-29 band: 29%), and 27-29 is the very mode of the distribution: **ordinary**; the "miracle" rhetoric is unmeasured. The text's own admission (adding an even number of verses to an even-t surah leaves the table intact → cannot resolve 128-129) matches our codex-insensitivity finding exactly.

Lesson: the typical recipe of external claims is to take one modest data fact (1/7–1/38) and multiply it through algebraic identities into several "independent miracles." Measurement opens the package in seconds.

## Honesty Notes (faithful to the discipline of the earlier report)

1. **Look-elsewhere correction:** ~70 candidate statistic families were scanned across the discovery sessions (the June 12 panorama alone ~28 families; including M1–M6 declared before the Two-Hands measurement). Even with a worst-case Bonferroni factor of ~75 (including the Return-of-Three events N1–N3), the codex-independent core remains at the ~10⁻¹³ level.
2. **The composite is assembled post hoc:** presenting the layers "together" is a product of this search. The most defensible standalone results are the events measured directly on their own: the full crystal (**8.4×10⁻¹⁰**) and the direct observation of Layer 1∧2c.
3. The individual pieces of Layer 3 (the 360 cycle) are ordinary events with a ~13% base rate, as shown in the earlier report; its contribution here is strictly limited to a conditional factor (≈1.4–1.6%), and automatic structures such as the "mirror identity / GL₂ generation" are deliberately **excluded**.
4. Layer 1 is a sharpened form of an observation known in the literature (the even-odd symmetry); the 1444 = 38² property of Layer 2 and the "double primitive element / mod-38 maximal cycle" framing of Layer 3 are contributions of this study.
5. This result is **not a proof** of design or miracle; it is a measured, reproducible, and unusually low-probability **statistical anomaly**. A definitive verdict requires pre-registered independent replication.
6. **Negative-result transparency (June 12):** the "30/27/27/30 count symmetry" that gleamed in the panorama turned out to be the **algebraic equivalent** of the 57/57 balance (conditional probability 1.000) and was eliminated; the verse-family (a ≡ 0 mod 19) was found weak and not adopted. The crystal's cell contents were shown to follow by algebraic necessity from the pan multisets + cell sums, and were **not counted** as a separate event.
7. **Meccan/Medinan lens — NEGATIVE RESULT (June 12):** the third natural dichotomy (86 Meccan / 28 Medinan; two API sources identical) was tested with 6 pre-registered events (`kesif6_mekki_medeni_prereg.md`): family M/M counts (D1–D3: p = 0.06 / 0.54 / 0.06), balance-pan distribution (D4: 44/42 — no balance), Σt divisibilities (D5: mod 6 and 15 — absent), family coefficient split (D6: 69+7 — absent). A mild "families lean Meccan" tendency exists but is not significant, and the family's single Medinan member (55, ar-Rahman) is classically disputed; even the most favorable variant reading gives only p ≈ 3%. **No layer emerged from this lens; it is recorded.**
8. **Counting-schools test (July 5):** the strongest known external critique is "verse counts vary across the counting schools." Tested with validated data (the source's Hafs list matches the canonical Kufan count exactly; Warsh = Qalun = the canonical 6214): **none of the core layers hold under the Medinan count.** The structure is specific to the Kufan versification; this is stated openly and the school freedom is priced at a worst-case ×7 factor. *(Descriptive note, not evidence: under the Medinan count the 19-family has 7 members with Σt = 722 — an ordinary value within the base distribution.)*
9. **Two-Hands discipline:** the difference-family alone is ordinary — the probability of getting 7 members is 14%, and it was noted as "weak" in the first scan; the layer was adopted only after the union structure (19 surahs + 19×61) measured as significant. The event definitions (M1–M6) were frozen **before** measurement; flourishes were **not multiplied** into the composite: Σu = 7×19 (conditional 4.2%) and the "ladder of sevens" (positive u's +14×19, negatives −7×19, absolute 21×19; conditional 4.5%). The +12% deviation seen in A∧W over the first 12 billion fell to **+4%** at 36 billion (648/621) — independence confirmed.
10. **External review record (July 11–13, 2026, İrvin Cemil Schick):** the study's first external scrutiny came as a four-letter exchange with the historian-mathematician İrvin Cemil Schick (applied-mathematics PhD, MIT; history PhD, EHESS); he is named here with his permission. His questions followed a full referee's curriculum: **(a) length-ordering objection** ("the arrangement is roughly sorted by length, so non-randomness is trivial") → measured in `kesif16`: perfectly sorted books score 0/4 layers; the real arrangement is indeed 8σ length-sorted (granted); under a null conditioned on being at least as sorted as the real book, layer rates rise only ×1.2–1.8. **(b) test-statistic objection** ("t = n+a adds unlike quantities") → the unit point was partially conceded (a carries an implicit unit); the surviving defense: the (t,u) transform is lossless, and any arrangement-sensitive statistic must mix n and a. **(c) multiplicity objection** → explaining 10⁻¹⁵ by test multiplicity would require ~10¹⁵ tests; the ledger holds ~75 families → agreement reached ("practically, nothing to object to"). **(d) meaningfulness criterion** — Schick's formulation entered the ledger: *"a found property is meaningful only through external motivation — 19 is interesting because of 74:30, not in itself."* The criterion coincides with the study's spine and is kept measured by the floating-modulus tournament.

## The Whole Structure at a Glance — "The Crystal"

```
                THE QURAN  (114 = 6×19 surahs;  t = surah no. + verse count)
                                      │
              ┌───────── EVEN-ODD BALANCE (Q 89:3) ──────────────┐
              │                                                  │
        t even: 57 surahs (3×19)                          t odd: 57 surahs (3×19)
        Σt = 6234 = TOTAL VERSES                          Σt = 6555 = TOTAL SURAH NOS.
              │                                                  │
              └──────────── THE 19-FAMILY (Q 74:30) ─────────────┘
                       t ≡ 0 (mod 19): 12 surahs (= 2×6),  Σt = 1444 = 38² = (2×19)²
                                      │
                      ┌──── EVEN-ODD again, inside ────┐
                      │                                │
                6 surahs (even t)                 6 surahs (odd t)
                Σt = 722 = 2×19²                  Σt = 722 = 2×19²
                coefficients: Σ = 38 = 2×19       coefficients: Σ = 38 = 2×19
                (surah-no. surplus +160    ⇄      verse deficit −160)
                      │                                │
             ┌── surah-no. parity ──┐        ┌── surah-no. parity ──┐
             │                      │        │                      │
         3 surahs             3 surahs      3 surahs           3 surahs
         Σt = 19×20           Σt = 19×18    Σt = 19×19         Σt = 19×19
         (= 19²+19)           (= 19²−19)    (= 361)            (= 361)
         {6,6,8}              {6,6,6}       {9,5,5}            {7,7,5}
             └── 18 × 20 = 360 ──┘              └── the 19² twins ──┘
                                      │
                38² = 19 × (18 + 19 + 19 + 20)  — consecutive cell coefficients
                                      │
                     THE COEFFICIENT CRYSTAL (Layer 2d)
               12 coefficients = the consecutive band {5, 6, 7, 8, 9}
         even pan: 5·6 + 8 = 38          odd pan: 3·5 + 2·7 + 1·9 = 38
         19 = 9+5+5 = 7+7+5  (the band's only two writings — both realized)
                                      │
                          TWO HANDS (Layer 2e)
                    t = n + a  (sum)  ·  u = n − a  (difference)
             sum-hand 12 surahs + difference-hand 7 surahs = EXACTLY 19 (disjoint)
          union's verse sum 1159 = 19×61 ;  difference-hand's Σu = 133 = 7×19
                                      │
                          THE MATRIX / CIRCLE LAYER
                P (even-odd) and R (19): both PRIMITIVE ELEMENTS of F₁₉²
                → the full 360-step circle, −I at step 180 (half turn)
                → P also maximal (360) in mod 38 = 2×19, uniting both motifs
```

The same two motifs (even-odd and 19) repeat, nested, at four scales: the balance across the whole book, the same balance inside the 19-family, two consecutive bands in the cells and the individual coefficients, and the 360 circle in the matrix layer. Every constant of the structure speaks the language of 19: 57 = 3×19, 12 = 2×(114/19), 1444 = 38², 722 = 2×19², 38 = 2×19, 360 = 19²−1 — and at the very bottom, {18, 19, 19, 20} around 19, {5, 6, 7, 8, 9} for the individuals.

## One-Sentence Summary

> With the single definition t = surah number + verse count: the Quran splits **by even and odd** exactly down the middle (57/57), and the two halves' t-sums land exactly on the book's two global constants (6234 verses, 6555 surah numbers); the 12 surahs selected **by nineteen** sum to exactly (2×19)² = 1444, the family balances by even-odd into 6/6 and 722/722 = 2×19² + 2×19², descending by the second parity into four 3-cells whose coefficients line up consecutively around 19 as **18·19·19·20** (the 19² twins in the odd column, the 18–20 pair multiplying to 360 in the even column), while the individual coefficients fill the consecutive **5–9 band** (even pan 5·6+8 = 38, odd pan 3·5+2·7+9 = 38; both of the band's only two writings of 19, 9+5+5 and 7+7+5, realized one per cell); the 19-families of the sum and the difference (u = surah no. − verses) are **disjoint and together exactly 19 surahs** (12+7; the union's verse sum 19×61); and the matrices of the two partitions are primitive elements walking the complete 360-unit circle of the 19²-element field. The crystal's directly measured probability is **7.14×10⁻¹⁰ (17,204 hits, ±0.8%)**; the codex-independent core combined with the balance and Two Hands is **≈ 10⁻¹⁵ — 1 in a quadrillion, sealed by two independent computational routes** (direct 1/1.19; chained 1/0.98 quadrillion); with the matrix layer ≈ 1.2×10⁻¹⁷. **The summit composite balance∧crystal was directly observed 5 times in 24 trillion trials** (under two independent seeds); balance∧fractal 1,181 times, fractal∧Two-Hands 13,537+, crystal∧Two-Hands 62, crystal∧primitivity 260, balance∧fractal∧Two-Hands once. And the only modulus from 3 to 40 satisfying all eight fractal criteria is **19** (nearest rival 3/8; in 500,000 synthetics no modulus passed even 7/8).

---

*Reproduction: `python3 test_oruntular.py` (base verification) · `kesif2_panorama.py`, `kesif4_tarama.py` (candidate scans) · `kesif3_fark.py` (Two Hands + 20M pre-measurement) · `kesif4_ozgulluk.py` (specificity, m = 3..40) · `kesif9_yuzer_modul.py`, `kesif10_siki_s4.py` (floating-modulus and strict-S4 controls) · `kesif5_ucundonusu.py` (Return of Three) · `cc -O3 -o kesif4_motor kesif4_motor.c` (CPU engine; `selftest` verifies all 28 flags) · `nvcc -O3 -o kesif_cuda kesif_cuda.cu` (GPU engine, counter-compatible; see `README_4090.md`). Raw run outputs: `runs/`, `runs3/`, `runs4/` (CPU, 86 billion) + `runGPU_4090_seed42.txt`, `runGPU_3090_seed43.txt` (GPU, 24 trillion). Raw data: `quran_meta.json`.*

---

*This document is the English translation of `KESIF.md` (Turkish original). Where the two differ, the Turkish original is authoritative.*
