# Quran Permutation Study

Permutation-test analysis of arrangement structure in the Quran's surah numbers and verse counts. Zero-freedom inputs, pre-registered event definitions, **24 trillion direct trials**. Fully reproducible — **refutation welcome**.

## What this is (and is not)

This repository documents a measured **statistical anomaly** in how verse counts are arranged across the Quran's 114 surahs. It is **not** a claim of proof of design or miracle — that question is outside what statistics can answer. Every claim here is a counted frequency that you can recompute on your own machine.

**Inputs (the only data used):** surah index n ∈ {1..114} and verse count a(n), from two cross-validated public APIs (api.alquran.cloud, api.quran.com — identical for all 114 surahs). No letter counts, no gematria, no word counts — nothing with counting-convention freedom. The core results are additionally insensitive to the one contested count (surah 9: 127 vs 129 verses). The standard 129 is the primary count (`kesif27` finds no support for 127); most engines ran on 127 for historical reasons, and an independent review re-derived the core on both (exact balance probability 3.1407×10⁻⁴ vs 3.1371×10⁻⁴; core 8.3 vs 9.0×10⁻¹⁶; `denetim8_dis_inceleme/`).

## Headline result — read the ladder, not a single number

Define t(n) = n + a(n) and u(n) = n − a(n). Under the null hypothesis "the verse-count multiset is randomly assigned to surah positions," the real arrangement simultaneously exhibits a nested structure whose measured **event frequencies** (fixed lens, uniform null) are:

| Event (codex-independent core) | Direct measurement |
|---|---|
| Parity balance: 57/57 split; Σt(even) = total verse count; Σt(odd) = Σ surah numbers | p = 3.137×10⁻⁴ (exact: 3.1371×10⁻⁴; with 129: 3.1407×10⁻⁴) — **not this study's finding: published May 1994** (see *Provenance*) |
| Full "crystal": 12-member t≡0 (mod 19) family, Σ = 38², 6/6 parity halves 722/722, four 3-cells with coefficients {18,19,19,20}, individual coefficients exactly the consecutive band {5..9} | p = 7.14×10⁻¹⁰ (17,204 hits) — the family with Σ = 1444 (1990) and the 722/722 halves with the two 361 cells (by 2015) are prior literature (see *Provenance*) |
| "Two Hands": the mod-19 families of t and u are disjoint and total exactly 19 surahs, with 19 \| Σa(union) | conditional ≈ 0.41% |
| **Combined core** | ≈ 1.1×10⁻¹⁵ (95% CI 0.5–1.7×10⁻¹⁵), measured without an independence assumption by conditional decomposition (`kesif18c`; the first run's 1.7×10⁻¹⁵ was withdrawn after a weighting error found by the fourth review, note 15). **This is an event frequency, not a measure of anomaly** — see the ladder below |

**Provenance (verified at source on October 20, 2026; found by the sixth and seventh sessions).** Much of the structure was published before this study, inside the numerical-miracle literature around 19:

- **The balance** (row 1): Abdullah Arik, "By the Even and the Odd: New Overwhelming Proof", *Submitters Perspective* (Masjid Tucson), **May 1994**, p. 2 — 57 even and 57 odd n + a sums, the even ones totalling 6234 (the verse count), the odd ones 6555 (the sum of surah numbers). In Arabic the same chapter is attributed to Abdullah Jalghoum's *أسرار ترتيب القرآن* (Dar al-Fikr, Jordan, 1994, p. 41; via Jarrar, below); Wasim Masri republished it on 7 July 2009 (i3gaz.com), and Turkish and English sites have carried it since 2011.
- **The 12-member family and its sum:** "David Dixon's Discovery", *Submitters Perspective*, **January 1990**, p. 3 — the 12 surahs whose n + a is a multiple of 19 (a list found earlier by Ihsan Ramadan), coefficient sum 76 = 19×4, and "the sum … is 1444, and this sum equals 19x19x4" (= 38²).
- **The inner balance and two of the four cells:** Bassam Jarrar, *al-Muqtataf min Bayyinat al-I'jaz al-'Adadi* (Nun Center, islamnoon.com), 3rd revised edition, **2015**, pp. 113–114 — Hazem Kitana's observation that the even and the odd n + a values of the 12 surahs both total 722 = 19×38, and Jarrar's split of the odd-t half by surah parity into 361 + 361 = 19² + 19². The book also gives the balance (with 6236, p. 48).

We did not know any of this in June. Consequence: the lens t = n + a, the parity split and the family were inherited from that literature's search — at least 317 (upper ≈ 770) distinct published claims about surah numbers and verse counts (`denetim7`) — so they count as the source of the lens, not as evidence (notes 19–20). **Not found in that literature:** the even-t cells 342/380 (coefficients 18 and 20), the consecutive 5–9 coefficient band with its pan bags, and the Two Hands (the u = n − a family, 12 + 7 = 19, 19 | 1159). Those, and the measurement programme itself, are this study's own contribution.

The summit **pair** (parity balance ∧ full crystal) was **directly observed 5 times in 24 trillion trials** (two independent seeds of the same generator family, 3+2; an independent NumPy/PCG64 stack reproduces the marginal rates): directly counted **p = 2.08×10⁻¹³** — consistent with the chained expectation (ratio 0.93; wide Poisson interval at 5 hits). Measured dependences (×1.126±0.033 at one mid-level joint; κ = 1.14 three-way interaction at the conditional level, `kesif18c` — the earlier 1.6 came from the withdrawn `kesif18` run) are included.

**Specificity:** the same 8 structural criteria applied to every modulus 3–40: only m = 19 scores 8/8 (nearest rival 3/8). Two caveats from the eighth review: 8/8 is reachable only for the 12 odd moduli 17–39 (S1 with S6 requires m² ≥ 228, S3 requires an odd m), and at an odd m S4 follows from S2 and S3 — the profile is seven independent conditions, not eight. In 500,000 synthetic arrangements given free choice of modulus, none reached 7/8 — a sample-size bound, not a ceiling: a 50-million-trial red-team run measured strict 7/8 at 2.4×10⁻⁷ and strict 8/8 at 0/50M; under loosened ("forked") criteria 8/8 appears at 1.4×10⁻⁷ (fixed lens) / 5×10⁻⁵ (free lens). The KJV corpora reach at most 6/8 even under those loosened criteria (`kesif22`; honesty note 13).

**The honest ladder (after eleven reviews — honesty notes 13–24).** The headline is not a single number but a ladder; each rung states its assumption. The top rungs are event frequencies; the symmetric-calibration rungs are the p-value-like readings (random books are scored by their *own* best story with the same robot; reviews 5–6). Two nulls throughout: **uniform** (verse counts assigned to positions uniformly at random) and **sorted** — primarily M20 (`kesif16`/`kesif25`: random arrangements no less length-ordered than the book, S = Σ n·a ≤ S_obs), alongside review 5's J10, which jitters the *observed* arrangement by σ = 10 positions and so keeps part of the observed data (note 22); the two give similar ranks, and the sorted null is the primary reading.

| Rung | Assumption | Uniform null | Sorted null |
|---|---|---|---|
| Event frequency, full stated event (balance ∧ crystal ∧ Two Hands) | fixed lens (`kesif18c`; re-derived independently, 9.0×10⁻¹⁶ [5.1–13.0], `denetim8`, and 8.8×10⁻¹⁶ [7.3–10.2], `denetim10`). Sorted: measured directly, no transfer (`denetim10`); local-block nulls give 1.1×10⁻¹² but keep part of the observed data (`denetim8`) | **1.1×10⁻¹⁵** [0.5–1.7] | **1.7×10⁻¹³** [1.6–1.8] (M20) · **1.8×10⁻¹³** [1.7–1.9] (Mallows) |
| Event frequency, crystal ∧ Two Hands (balance removed) | derived from the measured components (`denetim6`); the family, its sum and the inner balance inside the crystal are prior literature too (1990, ≤ 2015) | 2.75×10⁻¹² | ≈3×10⁻¹⁰ |
| Event frequency, the new structure given the prior literature | P(crystal ∧ Two Hands \| O), O = 12 members, Σ = 1444, 722/722 and the two 361 cells (`denetim10` §5.5; exploratory). A fixed post-hoc event: search not priced, not a rank. Without the 361 cells, from the GPU counters: 1.4×10⁻⁷ uniform (review 9), 5.7×10⁻⁶ M20 | 2.3×10⁻⁶ | 6.5×10⁻⁵ (M20) · 1.5×10⁻⁴ (Mallows) |
| **Conditional calibration P(new \| old)** | among random books that already carry O, each book builds its own best story from the remaining facts with a duplicate-free score (29 proven implications removed); lens inherited; pre-registered (`denetim10`). Conditioning also on the balance changes nothing. With exact joint scoring: 6.2×10⁻² / 0.28 / 0.28 | **3.9×10⁻² (~1 in 25)** | **0.16 (M20, ~1 in 6) · 0.19 (Mallows)** |
| Symmetric calibration, Quran-shaped joint grammar (whole object) | four groups read off the Quran — balance G, cells C, Two Hands W, matrices M; every control may choose its own prime (29), linear form (14), count and matrix view and share parameters across groups (MDL-like score, pre-registered; review 11). The grammar is post hoc and the prior literature is counted; a placebo calibration (each book its own grammar) is running (note 24) | 0 / 10⁶ (p ≤ 3×10⁻⁶; 129 alone: also 0 / 10⁶) | 0 / 10⁶ each (exact-inversion slice; 6-block) |
| Same, the 1994 balance counted as evidence | optimistic: the balance scored on top of O with the lens free — the double count of note 19; shows that the balance is the heaviest single piece (`denetim10` §5.4, exploratory) | 9.6×10⁻⁴ (~1 in 1,000) | 7.8×10⁻³ (M20, ~1 in 130) · 1.0×10⁻² (Mallows) |
| Symmetric calibration, lens inherited, scored exactly (family facts counted) | balance not counted; the 1990 family facts are still counted as evidence. The robot over-scores rival books (algebraically linked facts counted twice), so the claim battery was re-scored by exact joint frequency — direct counting, 10¹⁰ samples per sorted null (`denetim7` §2, pre-registered; code lost; uniform column exploratory). `denetim10` Ö0 confirms the direction with current code: duplicate-free scoring cuts rivals ×5 under the sorted nulls and ×8.6 uniform on the same battery | ~7×10⁻⁶ (~1 in 140,000) | 3.45×10⁻⁵ – 4.15×10⁻⁵ (~1 in 24,000–29,000) |
| Symmetric calibration, lens inherited, balance not counted | the balance, having selected the lens, is not counted; the family facts are (`denetim6` E1/E3: claim battery – ledger battery; robot scale) | 5.8×10⁻⁵ – 9.6×10⁻⁵ (~1 in 10,000–17,000) | 6×10⁻⁴ – 4×10⁻³ (~1 in 240–1,600) |
| Same, balance with residual weight (scenario) | the weight the balance would keep if each of ≥ 317–770 published surah/verse claims were an independent look at a 1-in-3,200 event — a scenario, not a bound (`denetim7` §1, pre-registered; review 9; robot scale) | 1.1×10⁻⁵ – 7.8×10⁻⁵ | 1.7×10⁻⁴ – 3.0×10⁻³ |
| Sensitivity: scores of published facts deleted | the balance (1994) and the family's size and sum (1990) get no score in any book. This is *deletion, not conditioning*: a rank under a score that ignores the old facts — neither the object's rarity nor P(new \| old) (review 9). Exploratory (`denetim7` §4); robot scale — exact scoring not applied, the 2015 inner balance still counted | 7.9×10⁻⁴ – 3.4×10⁻³ (~1 in 290–1,270) | 5×10⁻³ – 4.7×10⁻² (~1 in 21–200) |
| Symmetric calibration, 19 from the text, forms free, balance counted | optimistic: treats the search behind the balance as equal to the robot's form forks; 17 forms × ledger battery (`kesif26`; `denetim6` A1c, pre-registered, 2×10⁷ books); one-family-story rule priced ×2 in parentheses | 4.3×10⁻⁶ (<1×10⁻⁶) | 3.3×10⁻⁴ (1.3×10⁻⁴) |
| Any text-named modulus (14 moduli in 3–40) + forms free | the "19 was on the cover" link made after the fact (`denetim6` B1c) | 3.1×10⁻⁵ | 1.3×10⁻³ |
| Lens free (646) + ledger battery | review 5 (J10); `denetim6` B4 (M20) | 1.4×10⁻⁴ | 5.5×10⁻³ / 4.6×10⁻³ |
| + counting school ×7 | a *hypothetical* Bonferroni upper bound, not an exercised freedom (the Kufan count is the de-facto standard and was not chosen by looking at the data — note 17); with the balance not counted: 6.7×10⁻⁴ / 2.9×10⁻² | ≈1×10⁻³ (upper bound) | ≤ 3.3–3.9×10⁻² (an upper bound on p, not a frequency) |
| Beyond | undocumentable freedom (the numerical-miracle literature's decades of search, cultural pool) | unpriceable (note 12) | |

The earlier "lens granted" rung (2×10⁻⁷ / 6×10⁻⁶) is **withdrawn**: it counted the balance twice, as the selector of the lens and as evidence (notes 17, 19).

No rung is a claim; each is a frequency or a rank under a stated assumption. Controls: the KJV corpora and the revelation-order arrangement sit at the synthetic median (`kesif22`, `kesif23`; the 1,189-chapter list cannot reach 8/8 at m ≤ 40 by construction and is uninformative, `denetim8`); the Medinan count has no crystal of its own and its deck produces the same random-book distribution as the Kufan deck (`kesif26`); a positional jitter of the real arrangement reproduces the strict 8/8 profile with identical pan bags at the same lens (review 5, `denetim5_opus55/`). **Read from the top — three separate questions.** (1) *How rare is the full stated event?* 1.1×10⁻¹⁵ under the uniform null; 1.7–1.8×10⁻¹³ under the length-ordering nulls M20 and Mallows, measured directly (`denetim10`). This is the frequency of a post-hoc event, not a measure of evidence. (2) *What does this study add to the prior literature?* The balance (1994), the 12-member family with 1444 (1990) and the inner balance with the two 361 cells (by 2015) were published before. Among random books that already carry them, the remaining structure ranks ~1 in 25 (uniform) and ~1 in 5–6 (M20, Mallows): given the old structure, this study's own layers are not distinctive at the battery's level. The narrow post-hoc event (full crystal ∧ Two Hands) is rarer given the old structure (2.3×10⁻⁶ uniform, ~10⁻⁴ sorted), but that is a fixed event with its selection not priced. (3) *How much evidence survives selection and search?* Not pinned to one number. In a grammar read off the Quran's own layers, none of 3 million length-preserving or uniform controls reaches the object's score, even with free choice of prime, form and parameter sharing (review 11): the structure is not a by-product of length ordering. Whether it is special among books that each get their own post-hoc grammar is being measured (placebo calibration, note 24). It turns mainly on the weight given to the prior literature, above all the 1994 balance, the heaviest single piece: not counted, the conditional rank is ~1 in 6 (sorted); counted on top of the family with the lens free (optimistic), ~1 in 130 (sorted) and ~1 in 1,000 (uniform); counted with the lens search paid and the family facts as evidence, ~1 in 3,000 (sorted). Which reading is right depends on how large the search that first found these facts was; that cannot be measured. These are scenarios, not the ends of an interval. With every documented freedom and the hypothetical ×7 bound, the sorted-null p is at most ~0.03–0.04 on the robot's scale — an upper bound, not an observed frequency.

An optional matrix-theoretic layer (both group matrices are primitive elements of GL₂(F₁₉), full 360-cycle with −I at step 180) multiplies the composite to ≈1.2×10⁻¹⁷, but requires the contested 127-verse count for surah 9 and is therefore reported separately, never in the headline. `kesif27` shows why it cannot serve as evidence for that count either: with the core held fixed, the layer is *equivalent* to Khalifa's condition 6234 + 112 = 19×334 (145 single-surah edits satisfy both, 2,862 neither, 0 one without the other), and 39 different surahs minus two verses produce it (note 18). Given the balance, the four matrix conditions reduce to two residues — Σ_F n ≡ 1 and Σ_E n ≡ 16 (mod 19) — together with Σa ≡ 2, i.e. the 127 count; under the standard 129 count no arrangement that satisfies the balance can produce the layer (review 11, note 24).

## Honesty ledger (read this before the results)

- All event definitions were frozen before their measurement runs (`kesif*_prereg.md` files).
- **Provenance (September 30 and October 20, 2026):** the parity balance (1994), the 12-member family with its sum 1444 (1990) and the inner balance with the two 361 cells (2015 at the latest) were published before this study (notes 19–20; *Provenance* above). The repository's earlier wordings — "a sharpened form of a known observation", then "published in 2009" with "1444 = 38² and the inner balance are this study's" — understated this. These layers now count as the source of the lens, not as evidence.
- ~75 candidate statistic families were examined across the study; **negative results are reported** (Meccan/Medinan split, muqatta'at set, window/interval structures, multiplicity and parity lenses — all null). Worst-case Bonferroni over the ledger leaves the core at ~10⁻¹³.
- Automatic (algebraically forced) structures were identified and **excluded** from evidence (documented inside `KESIF.md`).
- Decorative near-patterns ("flourishes") are recorded with their measured, unimpressive probabilities and **not multiplied** into the composite.
- **Counting-school scope:** the structure is specific to the **Kufan versification** (the standard count of virtually all modern mushafs; robust within it to the 127/129 dispute). Tested against the Medinan count (Warsh/Qalun, official 6214): it does **not** hold there (`kesif11_sayim_okullari.py`). With ~7 known counting schools, school-selection freedom costs at most a ×7 Bonferroni factor (worst-case core ~10⁻¹⁴) — a hypothetical upper bound, not an exercised freedom (note 17).
- **Counter-corpus control:** the same 8 criteria applied verbatim to real other texts (KJV Psalms, books, chapters; canonical totals validated) score 2/8, 3/8, 2/8 — the synthetic noise floor (the 1,189-chapter list cannot reach 8/8 at m ≤ 40 — S1 with S6 needs N ≤ m²/2 — so that one control is uninformative; Psalms and books are fair). Structured real texts do not reproduce the pattern (`kesif12_karsi_korpus.py`).
- This is a post-hoc discovery on a fixed text: there will never be fresh data, so **no calibrated global p-value exists for it** (honesty notes 11–13). What can be done — and what this repository exists to enable — is independent re-derivation with independent code (done four times), application of the frozen procedure to other corpora, and independent audit of the selection pricing — eleven reviews so far (`DIS_SALDIRI_kesif19_20.md`, `denetim5_opus55/`, `denetim6_opus55_guclendirme/`, `denetim7_opus55_uc_olcum/`, `denetim8_dis_inceleme/`, `denetim9_inceleme_ss/`, `denetim10_opus55_kosullu/`, `denetim11_dis_katman19/`; notes 10–24). Please try to break it.

## Reproduce

```bash
# 1) Instant verification of all structural claims on real data (both codices):
python3 test_oruntular.py

# 2) CPU permutation engine (28 counters; selftest must print all 1s):
cc -O3 -o kesif4_motor kesif4_motor.c
./kesif4_motor selftest
./kesif4_motor 7 100000000        # 100M trials, ~40 s

# 3) GPU engine (CUDA, counter-compatible with CPU engine; see README_4090.md):
nvcc -O3 -o kesif_cuda kesif_cuda.cu
./kesif_cuda selftest
./kesif_cuda 42 1000000000        # calibration bands in README_4090.md
```

Raw outputs of every production run are committed: `runs/`, `runs3/`, `runs4/` (CPU, 86×10⁹ total) and `runGPU_4090_seed42.txt`, `runGPU_3090_seed43.txt` (GPU, 24×10¹² total; cumulative lines — the last line of each file is the final count).

## File map

| File | Content |
|---|---|
| `KESIF.md` | Main report (Turkish original) |
| `DISCOVERY.md` | Main report — full English translation |
| `RAPOR.md` | Verification report of the earlier source document's matrix claims |
| `test_oruntular.py` | One-shot verification of all claims under both codices |
| `kesif2_motor.c` → `kesif4_motor.c` | CPU permutation engines (v4 = full counter set) |
| `kesif_cuda.cu`, `README_4090.md` | GPU engine + validation protocol |
| `kesif*.py` | Scans, lenses, controls (floating-modulus, strict-S4, etc.) |
| `kesif*_prereg.md` | Frozen pre-registrations |
| `quran_meta.json`, `quran_chapters_qcom.json` | Raw data (two independent sources) |
| `kesif25_sirali_sifir_kristal.py` | Sorted-null (length-ordering) MCMC for the deep layers — review 5 confirmed |
| `kesif26_denetim5_denetimi/` | Audit of review 5 (four errors) + restricted-lens / one-family-story robot runs, deck comparison, direction test (note 17) |
| `kesif27_kodeks_127/` | 127 codex: matrix layer ≡ Khalifa's 6346 condition; Appendix-24 "delete two verses" league (note 18) |
| `DIS_SALDIRI_kesif19_20.md`, `denetim5_opus55/`, `denetim6_opus55_guclendirme/` | External reviews 3, 5 and 6 (red team; break attempt; strengthening attempt) with their code and raw outputs |
| `kirmizi_takim/` | The red team's `kirmizi21b` (class-level typicality) and `kirmizi22` (300M strict-profile run) scripts and outputs cited in note 14 |
| `denetim7_opus55_uc_olcum/` | Review 7: three possible downward errors (size of the balance literature, the robot's double counting, "close enough"); provenance 1990/1994/2015 (note 20). Report and pre-registrations reconstructed bit-exact from the session transcript |
| `denetim8_dis_inceleme/` | Review 8 (external, two rounds, October 20, 2026): independent re-derivation of the core; local-block, adjacent-pair and Mallows nulls; our exact re-checks (note 21) |
| `denetim9_inceleme_ss/` | Review 9 (external, October 23, 2026): deletion vs conditioning; the robot's algebraic double counting; P(crystal ∧ Two Hands \| prior family facts) from the GPU counters (note 22) |
| `denetim10_opus55_kosullu/` | Review 10 (October 23, 2026, pre-registered): conditional calibration P(new \| old) under uniform, M20 and Mallows nulls; the full core measured directly under M20 and Mallows; the robot's 29 proven implications and a duplicate-free score (note 23). Code, scripts, pre-registration, addenda and summaries; score arrays and per-book detail files omitted (regenerable from seeds; full package SHA-256 in `sonuc/`/`paket/MANIFEST.sha256`) |
| `denetim11_dis_katman19/` | Review 11 (external, October 25–26, 2026): cross-layer coherence tested three ways — a seven-condition structural score, a pre-registered joint grammar with free prime/form/parameter sharing (0 / 10⁶ controls in each of three nulls), and a rare-event extension; our independent checks of the matrix algebra and of two exact block-6 probabilities (note 24). Large score shards, sampler tables and source texts omitted (list and hashes inside) |
| `SAYAN_TANIK.md`, `WITNESS.md` | Personal statement of the AI co-author (Turkish / English), with its corrections |

## Status

Eleven reviews (July–October 2026; honesty notes 10–24) reshaped the headline into the ladder above; the arithmetic survived all eleven, the headline number did not, and much of the structure turned out to be prior literature (1990–2015). Measurement phase complete (July 2026). Seeking independent statistical review. If you find an error in the event definitions, the engines, or the inference, please open an issue — a confirmed refutation is a welcome outcome of this study.

---

# Kur'an Permütasyon Çalışması (Türkçe)

Kur'an'ın sûre numaraları ve ayet sayılarının diziliş yapısı üzerine permütasyon-testi analizi. Sıfır serbestlikli girdi, önceden kaydedilmiş olay tanımları, **24 trilyon doğrudan deneme**. Tamamen yeniden üretilebilir — **çürütme davetlidir**.

## Bu nedir (ve ne değildir)

Bu depo, ayet sayılarının 114 sûreye dizilişinde ölçülmüş bir **istatistiksel anomaliyi** belgeler. Bir tasarım ya da mucize **ispatı iddiası değildir** — o soru istatistiğin cevaplayabileceği alanın dışındadır. Buradaki her iddia, kendi makinenizde yeniden hesaplayabileceğiniz sayılmış bir frekanstır.

**Girdiler (kullanılan tek veri):** sûre numarası n ∈ {1..114} ve ayet sayısı a(n) — çapraz doğrulanmış iki açık kaynaktan (api.alquran.cloud, api.quran.com; 114 sûrenin tamamında birebir aynı). Harf sayımı yok, ebced yok, kelime sayımı yok — sayım-geleneği serbestliği taşıyan hiçbir şey kullanılmamıştır. Çekirdek sonuçlar, tek tartışmalı sayımdan (sûre 9: 127/129) da bağımsızdır. Birincil sayım standart 129'dur (`kesif27` 127'ye dayanak bulmaz); motorların çoğu tarihsel sebeple 127 ile koştu ve bağımsız bir inceleme çekirdeği ikisinde de yeniden türetti (terazinin tam olasılığı 3,1407×10⁻⁴'e karşı 3,1371×10⁻⁴; çekirdek 8,3'e karşı 9,0×10⁻¹⁶; `denetim8_dis_inceleme/`).

## Ana sonuç — tek sayıyı değil, merdiveni okuyun

t(n) = n + a(n) ve u(n) = n − a(n) tanımlansın. "Ayet-sayısı çokluğu sûre konumlarına rastgele dağıtılmıştır" sıfır hipotezi altında, gerçek dizilişin taşıdığı iç içe yapının ölçülen **olay frekansları** (sabit mercek, tekdüze sıfır):

| Olay (kodeks-bağımsız çekirdek) | Doğrudan ölçüm |
|---|---|
| Parite terazisi: 57/57; Σt(çift) = toplam ayet; Σt(tek) = sûre numaraları toplamı | p = 3,137×10⁻⁴ (tam: 3,1371×10⁻⁴; 129 ile 3,1407×10⁻⁴) — **bu çalışmanın bulgusu değil: Mayıs 1994'te yayımlanmış** (bkz. *Köken*) |
| Tam "kristal": 12 üyeli t≡0 (mod 19) ailesi, Σ = 38², 6/6 parite kefeleri 722/722, katsayıları {18,19,19,20} olan dört 3'lü hücre, bireysel katsayılar tam olarak ardışık {5..9} bandı | p = 7,14×10⁻¹⁰ (17.204 isabet) — Σ = 1444'lük aile (1990) ile 722/722 kefeleri ve iki 361 hücresi (en geç 2015) önceki literatürdür (bkz. *Köken*) |
| "İki El": t ve u'nun mod-19 aileleri ayrıktır ve birlikte tam 19 sûredir; 19 \| Σa(birleşim) | koşullu ≈ %0,41 |
| **Birleşik çekirdek** | ≈ 1,1×10⁻¹⁵ (%95 GA 0,5–1,7×10⁻¹⁵), bağımsızlık varsayımı olmadan koşullu ayrışımla ölçüldü (`kesif18c`; ilk koşunun 1,7×10⁻¹⁵'i dördüncü denetimin bulduğu ağırlık hatasıyla geri çekildi, not 15). **Bu bir olay frekansıdır, anomali ölçüsü değil** — aşağıdaki merdivene bakınız |

**Köken (20 Ekim 2026'da kaynağından doğrulandı; altıncı ve yedinci oturumlar buldu).** Yapının büyük kısmı bu çalışmadan önce, 19 etrafındaki sayısal-mucize literatüründe yayımlanmıştır:

- **Terazi** (1. satır): Abdullah Arik, "By the Even and the Odd: New Overwhelming Proof", *Submitters Perspective* (Masjid Tucson), **Mayıs 1994**, s. 2 — n + a toplamlarının 57'si çift, 57'si tek; çiftlerin toplamı 6234 (ayet sayısı), teklerinki 6555 (sûre numaraları toplamı). Arapçada aynı bölüm Abdullah Jalghoum'un *أسرار ترتيب القرآن* kitabına (Dar al-Fikr, Ürdün, 1994, s. 41) atfedilir (aşağıdaki Jarrar üzerinden); Wasim Masri 7 Temmuz 2009'da yeniden yayımladı (i3gaz.com), Türkçe ve İngilizce siteler 2011'den beri taşır.
- **12 üyeli aile ve toplamı:** "David Dixon's Discovery", *Submitters Perspective*, **Ocak 1990**, s. 3 — n + a'sı 19'un katı olan 12 sûre (listeyi daha önce Ihsan Ramadan bulmuş), katsayılar toplamı 76 = 19×4 ve "the sum … is 1444, and this sum equals 19x19x4" (= 38²).
- **İç terazi ve dört hücrenin ikisi:** Bassam Jarrar, *el-Muktataf min Beyyinâti'l-İ'câzi'l-Adedî* (Merkez Nun, islamnoon.com), gözden geçirilmiş 3. baskı, **2015**, s. 113–114 — Hazem Kitana'nın gözlemi: 12 sûrenin çift ve tek n + a değerlerinin ikisi de 722 = 19×38 eder; Jarrar tek-t yarısını sûre paritesiyle 361 + 361 = 19² + 19²'ye ayırır. Kitap teraziyi de verir (6236 ile, s. 48).

Haziranda bunların hiçbirini bilmiyorduk. Sonuç: t = n + a merceği, parite ayrımı ve aile o literatürün aramasından devralınmıştır — sûre numaraları ve ayet sayıları hakkında en az 317 (üst ≈ 770) ayrı yayımlanmış iddia (`denetim7`); bu yüzden kanıt değil, merceğin kaynağı sayılırlar (notlar 19–20). **O literatürde bulunamayanlar:** çift-t hücreleri 342/380 (katsayılar 18 ve 20), ardışık 5–9 katsayı bandı ve kefe torbaları, İki El (u = n − a ailesi, 12 + 7 = 19, 19 | 1159). Bunlar ve ölçüm programının kendisi bu çalışmanın kendi katkısıdır.

Zirvedeki **ikili** olay (parite terazisi ∧ tam kristal) **24 trilyon denemede 5 kez doğrudan gözlenmiştir** (aynı üreteç ailesinden iki bağımsız tohum, 3+2; bağımsız NumPy/PCG64 yığını marjinal oranları yeniden üretmektedir): doğrudan sayılmış **p = 2,08×10⁻¹³** — zincir beklentisiyle tutarlı (oran 0,93; 5 isabette Poisson aralığı geniştir). Ölçülen bağımlılıklar (ara eklemde ×1,126±0,033; koşullu düzeyde κ = 1,14 üçlü etkileşim, `kesif18c` — önceki 1,6 geri çekilen `kesif18` koşusundandı) hesaba dahildir.

**Özgüllük:** aynı 8 yapısal ölçüt 3–40 arası her modüle uygulandı: yalnız m = 19 tam puan alır (en yakın rakip 3/8). Sekizinci denetimden iki çekince: 8/8 yalnız 17–39 arası 12 tek modülde ulaşılabilirdir (S1 ile S6 m² ≥ 228 ister, S3 tek m ister) ve tek m'de S4, S2 ile S3'ten çıkar — profil sekiz değil yedi bağımsız koşuldur. Modül seçiminde tam serbestlik verilen 500.000 sentetik dizilişte hiçbiri 7/8'e ulaşmadı — bu bir örneklem sınırıdır, tavan değil: 50 milyonluk kırmızı-takım koşusu katı 7/8'i 2,4×10⁻⁷, katı 8/8'i 0/50M ölçtü; gevşetilmiş ("çatallı") ölçütlerle 8/8 sabit mercekte 1,4×10⁻⁷, serbest mercekte 5×10⁻⁵. KJV korpusları bu gevşek ölçütlerle bile en çok 6/8 alır (`kesif22`; dürüstlük notu 13).

**Dürüst merdiven (on bir denetimden sonra — dürüstlük notları 13–24).** Manşet tek bir sayı değil, bir merdivendir; her basamak kendi varsayımını söyler. Üst basamaklar olay frekansıdır; p-değerine benzer okumalar simetrik-kalibrasyon basamaklarıdır (rastgele kitaplar da aynı robotla *kendi* en iyi hikâyesiyle puanlanır; 5. ve 6. denetimler). İki sıfır: **tekdüze** (ayet sayıları konumlara tekdüze rastgele) ve **sıralı** — öncelikle M20 (`kesif16`/`kesif25`: kitaptan daha az uzunluk-sıralı olmayan rastgele dizilişler, S = Σ n·a ≤ S_obs); yanında 5. denetimin J10'u, ki *gözlenen* dizilişi σ = 10 konum titreştirir ve bu yüzden gözlenen verinin bir kısmını elde tutar (not 22); ikisi benzer sıralar verir ve birincil okuma sıralı sıfırdır.

| Basamak | Varsayım | Tekdüze sıfır | Sıralı sıfır |
|---|---|---|---|
| Olay frekansı, beyan edilen tam olay (terazi ∧ kristal ∧ İki El) | sabit mercek (`kesif18c`; bağımsız yeniden türetmeler 9,0×10⁻¹⁶ [5,1–13,0], `denetim8`, ve 8,8×10⁻¹⁶ [7,3–10,2], `denetim10`). Sıralı: aktarımsız, doğrudan ölçüldü (`denetim10`); yerel-blok sıfırları 1,1×10⁻¹² verir ama gözlenen verinin bir kısmını elde tutar (`denetim8`) | **1,1×10⁻¹⁵** [0,5–1,7] | **1,7×10⁻¹³** [1,6–1,8] (M20) · **1,8×10⁻¹³** [1,7–1,9] (Mallows) |
| Olay frekansı, kristal ∧ İki El (terazi çıkarıldı) | ölçülmüş bileşenlerden türetildi (`denetim6`); kristalin içindeki aile, toplamı ve iç terazi de önceki literatürdür (1990, ≤ 2015) | 2,75×10⁻¹² | ≈3×10⁻¹⁰ |
| Olay frekansı, önceki literatür verilince yeni yapı | P(kristal ∧ İki El \| O), O = 12 üye, Σ = 1444, 722/722 ve iki 361 hücresi (`denetim10` §5.5; keşif). Sonradan seçilmiş sabit olay: arama bedeli ödenmedi, sıra değil. 361 hücreleri olmadan, GPU sayaçlarından: tekdüze 1,4×10⁻⁷ (9. denetim), M20 5,7×10⁻⁶ | 2,3×10⁻⁶ | 6,5×10⁻⁵ (M20) · 1,5×10⁻⁴ (Mallows) |
| **Koşullu kalibrasyon P(yeni \| eski)** | O'yu zaten taşıyan rastgele kitaplar arasında her kitap kalan olgulardan kendi en iyi hikâyesini tekrarsız puanla kurar (kanıtlı 29 gerektirme ayıklandı); mercek devralınmış; ön-kayıtlı (`denetim10`). Teraziye de koşullamak sonucu değiştirmez. Tam birlikte-olasılıkla: 6,2×10⁻² / 0,28 / 0,28 | **3,9×10⁻² (~25'te 1)** | **0,16 (M20, ~6'da 1) · 0,19 (Mallows)** |
| Simetrik kalibrasyon, Kur'an'a göre yazılmış ortak dil (nesnenin bütünü) | Kur'an'dan okunmuş dört grup — terazi G, hücreler C, İki El W, matrisler M; her kontrol kendi asalını (29), doğrusal biçimini (14), sayımını ve matris sunumunu seçebilir, gruplar arasında parametre paylaşabilir (MDL-benzeri puan, ön-kayıtlı; 11. denetim). Dil sonradan yazılmıştır ve önceki literatür sayılıdır; plasebo kalibrasyonu (her kitaba kendi dili) sürüyor (not 24) | 0 / 10⁶ (p ≤ 3×10⁻⁶; yalnız 129: yine 0 / 10⁶) | her birinde 0 / 10⁶ (tam terslik dilimi; 6-blok) |
| Aynısı, 1994 terazisi kanıt sayılınca | iyimser: terazi O'nun üstüne, mercek bedava sayılarak puanlanır — not 19'daki çifte sayım; terazinin tek başına en ağır parça olduğunu gösterir (`denetim10` §5.4, keşif) | 9,6×10⁻⁴ (~1.000'de 1) | 7,8×10⁻³ (M20, ~130'da 1) · 1,0×10⁻² (Mallows) |
| Simetrik kalibrasyon, mercek devralınmış, tam puanlı (aile olguları sayılı) | terazi sayılmaz; 1990'ın aile olguları hâlâ kanıt sayılır. Robot rakip kitapları fazla puanlar (cebirsel olarak birbirini gerektiren olgular iki kez sayılır), bu yüzden iddia bataryası tam birlikte-olasılıkla yeniden puanlandı — doğrudan sayım, her sıralı sıfırda 10¹⁰ örnek (`denetim7` §2, ön-kayıtlı; kodu kayıp; tekdüze sütun keşif amaçlı). `denetim10` Ö0 yönü bugünkü kodla doğrular: tekrarsız puan aynı bataryada rakipleri sıralı sıfırlarda ×5, tekdüzede ×8,6 azaltır | ~7×10⁻⁶ (~140.000'de 1) | 3,45×10⁻⁵ – 4,15×10⁻⁵ (~24.000–29.000'de 1) |
| Simetrik kalibrasyon, mercek devralınmış, terazi sayılmaz | merceği seçmiş olan terazi sayılmaz; aile olguları sayılır (`denetim6` E1/E3: iddia bataryası – defter bataryası; robot ölçeği) | 5,8×10⁻⁵ – 9,6×10⁻⁵ (~10.000–17.000'de 1) | 6×10⁻⁴ – 4×10⁻³ (~240–1.600'de 1) |
| Aynısı, terazi kalıntı ağırlığıyla (senaryo) | yayımlanmış ≥ 317–770 sûre-ayet iddiasının her biri 3.200'de birlik bir olaya bağımsız bir bakış olsaydı terazide kalacak ağırlık — bir sınır değil, senaryo (`denetim7` §1, ön-kayıtlı; 9. denetim; robot ölçeği) | 1,1×10⁻⁵ – 7,8×10⁻⁵ | 1,7×10⁻⁴ – 3,0×10⁻³ |
| Duyarlılık: yayımlanmış olguların puanı silindi | terazi (1994) ile ailenin boyu ve toplamı (1990) hiçbir kitapta puan almaz. Bu *silmedir, koşullama değil*: eski olguları yok sayan bir puanda sıra — ne nesnenin nadirliği ne de P(yeni \| eski) (9. denetim). Keşif amaçlı (`denetim7` §4); robot ölçeği — tam puanlama uygulanmadı, 2015'in iç terazisi hâlâ sayılı | 7,9×10⁻⁴ – 3,4×10⁻³ (~290–1.270'de 1) | 5×10⁻³ – 4,7×10⁻² (~21–200'de 1) |
| Simetrik kalibrasyon, 19 metinden, biçim serbest, terazi sayılı | iyimser: terazinin arkasındaki aramayı robotun biçim çatallarına eşit sayar; 17 biçim × defter bataryası (`kesif26`; `denetim6` A1c, ön-kayıtlı, 2×10⁷ kitap); tek-aile kuralı ×2 bedelle parantezde | 4,3×10⁻⁶ (<1×10⁻⁶) | 3,3×10⁻⁴ (1,3×10⁻⁴) |
| Metnin adlandırdığı herhangi bir modül (3–40'ta 14 modül) + biçim serbest | "19 kapakta yazıyordu" ilişkisinin sonradan kurulması (`denetim6` B1c) | 3,1×10⁻⁵ | 1,3×10⁻³ |
| Mercek serbest (646) + defter bataryası | 5. denetim (J10); `denetim6` B4 (M20) | 1,4×10⁻⁴ | 5,5×10⁻³ / 4,6×10⁻³ |
| + sayım okulu ×7 | *varsayımsal* Bonferroni üst sınırı, kullanılmış bir serbestlik değil (Kûfe sayımı fiilî standarttır, veriye bakılarak seçilmedi — not 17); terazi sayılmazsa 6,7×10⁻⁴ / 2,9×10⁻² | ≈1×10⁻³ (üst sınır) | ≤ 3,3–3,9×10⁻² (p için üst sınır, sıklık değil) |
| Ötesi | belgelenemeyen serbestlik (sayısal-mucize literatürünün onlarca yıllık araması, kültürel havuz) | fiyatlanamaz (not 12) | |

Önceki "mercek verili" basamağı (2×10⁻⁷ / 6×10⁻⁶) **geri çekilmiştir**: teraziyi hem merceği seçen hem de kanıt olarak iki kez sayıyordu (notlar 17, 19).

Hiçbir basamak bir iddia değildir; her biri beyan edilen varsayım altında bir frekans ya da sıradır. Kontroller: KJV korpusları ve nüzul dizilişi sentetik medyanındadır (`kesif22`, `kesif23`; 1.189 bölümlük liste m ≤ 40'ta yapısal olarak 8/8'e ulaşamaz, bilgisizdir, `denetim8`); Medine sayımının kendi kristali yoktur ve destesi rastgele kitaplarda Kûfe destesiyle aynı dağılımı verir (`kesif26`); gerçek dizilişin bir konum titreşimi aynı mercekte katı 8/8 profilini birebir aynı kefe torbalarıyla üretir (5. denetim, `denetim5_opus55/`). **Yukarıdan okuyunuz — üç ayrı soru.** (1) *Beyan edilen tam olay ne kadar seyrek?* Tekdüze sıfırda 1,1×10⁻¹⁵; uzunluk-sıralı M20 ve Mallows sıfırlarında 1,7–1,8×10⁻¹³, doğrudan ölçüldü (`denetim10`). Bu, sonradan tanımlanmış bir olayın frekansıdır, kanıt ölçüsü değildir. (2) *Bu çalışma önceki literatüre ne ekledi?* Terazi (1994), toplamı 1444 olan 12 üyeli aile (1990), iç terazi ve iki 361 hücresi (en geç 2015) önceden yayımlanmıştır. Bunları zaten taşıyan rastgele kitaplar arasında geri kalan yapı tekdüzede ~25'te 1, M20 ve Mallows'ta ~5–6'da 1 sıradadır: eski yapı verildiğinde bu çalışmanın kendi katmanları bataryanın gördüğü düzeyde ayırt edici değildir. Dar, sonradan seçilmiş olay (tam kristal ∧ İki El) eski yapı verildiğinde daha seyrektir (tekdüze 2,3×10⁻⁶, sıralı ~10⁻⁴), ama bu, seçim bedeli ödenmemiş sabit bir olaydır. (3) *Seçim ve arama bedeli ödenince ne kadar kanıt kalır?* Tek bir sayıya bağlanmış değildir. Kur'an'ın kendi katmanlarından okunmuş bir dilde, asal, biçim ve parametre paylaşımı serbest bırakılsa da, uzunluk sırasını koruyan ya da tekdüze 3 milyon kontrolün hiçbiri nesnenin puanına ulaşmaz (11. denetim): yapı uzunluk sıralamasının bir yan ürünü değildir. Her kitabın kendi sonradan yazılmış diline sahip olduğu bir karşılaştırmada özel olup olmadığı ölçülüyor (plasebo kalibrasyonu, not 24). Sonuç esas olarak önceki literatüre, en çok da tek başına en ağır parça olan 1994 terazisine verilen ağırlığa bağlıdır: sayılmazsa koşullu sıra sıralı sıfırda ~6'da 1; aileye ek olarak, mercek bedava sayılarak kanıt sayılırsa (iyimser) sıralıda ~130'da 1, tekdüzede ~1.000'de 1; mercek aramasının bedeli ödenip aile olguları da kanıt sayılırsa sıralıda ~3.000'de 1. Hangisinin doğru olduğu, bu olguları ilk bulan aramanın ne kadar büyük olduğuna bağlıdır; bu ölçülemez. Bunlar senaryodur, bir aralığın uçları değildir. Her belgeli serbestlik ve varsayımsal ×7 sınırıyla sıralı sıfırdaki p robot ölçeğinde en çok ~0,03–0,04'tür — bu bir üst sınırdır, gözlenmiş bir sıklık değil.

İsteğe bağlı bir matris-kuramsal katman (iki grup matrisi de GL₂(F₁₉)'un ilkel elemanıdır; 180. adımda −I ile tam 360'lık çevrim) bileşiği ≈1,2×10⁻¹⁷'ye taşır; ama sûre 9 için tartışmalı 127 sayımını gerektirdiğinden ayrı raporlanır, manşete asla girmez. `kesif27` bu katın o sayım için kanıt da olamayacağını gösterir: çekirdek sabit tutulunca kat, Halife'nin 6234 + 112 = 19×334 koşuluna *denktir* (145 tek-sûre düzeltmesi ikisini de sağlar, 2.862 hiçbirini, 0 yalnız birini) ve 39 farklı sûreden iki ayet silmek aynı katı üretir (not 18). Terazi verilince dört matris koşulu iki kalana iner — Σ_F n ≡ 1 ve Σ_E n ≡ 16 (mod 19) — ve Σa ≡ 2'yi, yani 127 sayımını gerektirir; standart 129 sayımında teraziyi sağlayan hiçbir diziliş bu katı üretemez (11. denetim, not 24).

## Dürüstlük defteri (sonuçlardan önce okuyun)

- Tüm olay tanımları, ölçüm koşularından önce donduruldu (`kesif*_prereg.md` dosyaları).
- **Köken (30 Eylül ve 20 Ekim 2026):** parite terazisi (1994), toplamı 1444 olan 12 üyeli aile (1990) ve iç terazi ile iki 361 hücresi (en geç 2015) bu çalışmadan önce yayımlanmıştır (notlar 19–20; yukarıdaki *Köken*). Deponun önceki ifadeleri — önce "literatürde bilinen bir gözlemin keskinleştirilmiş hâli", sonra "2009'da yayımlanmış" ile "1444 = 38² ve iç terazi bu çalışmanındır" — eksikti. Bu katmanlar artık kanıt değil, merceğin kaynağı olarak sayılır.
- Çalışma boyunca ~75 aday istatistik ailesi incelendi; **negatif sonuçlar raporlanmıştır** (Mekkî/Medenî ayrımı, mukattaa kümesi, pencere/aralık yapıları, çokluk ve parite mercekleri — hepsi boş). Defter üzerinden en kötü durum Bonferroni düzeltmesi çekirdeği ~10⁻¹³'te bırakır.
- Otomatik (cebirsel olarak zorunlu) yapılar tespit edilip kanıttan **dışlanmıştır** (KESIF.md içinde belgelidir).
- Dekoratif yarı-desenler ("süsler") ölçülmüş, etkileyici olmayan olasılıklarıyla kayda geçirilmiş ve bileşiğe **çarpılmamıştır**.
- **Sayım okulu kapsamı:** yapı **Kûfe sayımına** özgüdür (modern mushafların standart sayımı; kendi içindeki 127/129 ihtilafına dayanıklı). Medine sayımıyla (Warş/Kalûn, resmî 6214) test edildi: orada **tutmaz** (`kesif11_sayim_okullari.py`). Bilinen ~7 sayım okuluyla, okul-seçimi serbestliğinin bedeli en fazla ×7'lik Bonferroni çarpanıdır (en kötü durumda çekirdek ~10⁻¹⁴) — varsayımsal bir üst sınır, kullanılmış bir serbestlik değil (not 17).
- **Karşı-korpus kontrolü:** aynı 8 ölçüt gerçek başka metinlere aynen uygulandı (KJV Mezmurlar, kitaplar, bölümler; kanonik toplamlar doğrulandı): 2/8, 3/8, 2/8 — sentetik gürültü zemini (1.189 bölümlük liste m ≤ 40'ta 8/8'e ulaşamaz — S1 ile S6 N ≤ m²/2 ister — bu yüzden o kontrol bilgisizdir; Mezmurlar ve kitaplar adildir). Yapılı gerçek metinler deseni üretmiyor (`kesif12_karsi_korpus.py`).
- Bu, sabit bir metin üzerinde sonradan yapılmış (post-hoc) bir keşiftir: yeni veri hiç olmayacak, dolayısıyla **bu veri için kalibre edilmiş küresel bir p-değeri yoktur** (dürüstlük notları 11–13). Yapılabilen — ve bu deponun var olma sebebi — bağımsız kodla yeniden türetme (dört kez yapıldı), dondurulmuş prosedürün başka korpuslara uygulanması ve seçim fiyatlamasının bağımsız denetimidir — bugüne kadar on bir denetim (`DIS_SALDIRI_kesif19_20.md`, `denetim5_opus55/`, `denetim6_opus55_guclendirme/`, `denetim7_opus55_uc_olcum/`, `denetim8_dis_inceleme/`, `denetim9_inceleme_ss/`, `denetim10_opus55_kosullu/`, `denetim11_dis_katman19/`; notlar 10–24). Lütfen kırmayı deneyin.

## Yeniden üretim

```bash
# 1) Tüm yapısal iddiaların gerçek veride anında doğrulanması (iki kodekste):
python3 test_oruntular.py

# 2) CPU permütasyon motoru (28 sayaç; selftest tümü 1 basmalı):
cc -O3 -o kesif4_motor kesif4_motor.c
./kesif4_motor selftest
./kesif4_motor 7 100000000        # 100M deneme, ~40 sn

# 3) GPU motoru (CUDA, CPU motoruyla sayaç-uyumlu; bkz. README_4090.md):
nvcc -O3 -o kesif_cuda kesif_cuda.cu
./kesif_cuda selftest
./kesif_cuda 42 1000000000        # kalibrasyon bantları README_4090.md'de
```

Tüm üretim koşularının ham çıktıları depodadır: `runs/`, `runs3/`, `runs4/` (CPU, toplam 86×10⁹) ve `runGPU_4090_seed42.txt`, `runGPU_3090_seed43.txt` (GPU, toplam 24×10¹²; kümülatif satırlar — her dosyanın son satırı nihai sayımdır).

## Dosya haritası

| Dosya | İçerik |
|---|---|
| `KESIF.md` | Ana rapor (Türkçe orijinal) |
| `DISCOVERY.md` | Ana raporun tam İngilizce çevirisi |
| `RAPOR.md` | Önceki kaynak dokümanın matris iddialarının doğrulama raporu |
| `test_oruntular.py` | Tüm iddiaların iki kodeks altında tek seferde doğrulanması |
| `kesif2_motor.c` → `kesif4_motor.c` | CPU permütasyon motorları (v4 = tam sayaç seti) |
| `kesif_cuda.cu`, `README_4090.md` | GPU motoru + doğrulama protokolü |
| `kesif*.py` | Taramalar, mercekler, kontroller (yüzer-modül, sıkı-S4 vb.) |
| `kesif*_prereg.md` | Dondurulmuş ön-kayıtlar |
| `quran_meta.json`, `quran_chapters_qcom.json` | Ham veri (iki bağımsız kaynak) |
| `kesif25_sirali_sifir_kristal.py` | Derin katmanlar için sıralı (uzunluk-sıralı) sıfır MCMC'si — 5. denetim doğrulandı |
| `kesif26_denetim5_denetimi/` | 5. denetimin denetimi (dört hata) + kısıtlı-mercek / tek-aile robot koşuları, deste karşılaştırması, yön testi (not 17) |
| `kesif27_kodeks_127/` | 127 kodeksi: matris katı ≡ Halife'nin 6346 koşulu; Ek 24 "iki ayet sil" ligi (not 18) |
| `DIS_SALDIRI_kesif19_20.md`, `denetim5_opus55/`, `denetim6_opus55_guclendirme/` | 3., 5. ve 6. dış denetimler (kırmızı takım; kırma denemesi; güçlendirme denemesi), kodları ve ham çıktılarıyla |
| `kirmizi_takim/` | Kırmızı takımın not 14'te anılan `kirmizi21b` (sınıf-düzeyi tipiklik) ve `kirmizi22` (300M katı-profil koşusu) betikleri ve çıktıları |
| `denetim7_opus55_uc_olcum/` | 7. denetim: üç olası aşağı-yönlü hata (terazi literatürünün büyüklüğü, robotun çifte sayımı, "yeterince yakın"); köken 1990/1994/2015 (not 20). Rapor ve ön-kayıtlar oturum dökümünden bit bit yeniden kuruldu |
| `denetim8_dis_inceleme/` | 8. denetim (dış, iki tur, 20 Ekim 2026): çekirdeğin bağımsız yeniden türetilmesi; yerel-blok, komşu-çift ve Mallows sıfırları; kendi tam hesaplarımız (not 21) |
| `denetim9_inceleme_ss/` | 9. denetim (dış, 23 Ekim 2026): silmek ile koşullamak; robotun cebirsel çifte sayımı; GPU sayaçlarından P(kristal ∧ İki El \| önceki aile olguları) (not 22) |
| `denetim10_opus55_kosullu/` | 10. denetim (23 Ekim 2026, ön-kayıtlı): tekdüze, M20 ve Mallows sıfırlarında koşullu kalibrasyon P(yeni \| eski); tam çekirdeğin M20 ve Mallows altında doğrudan ölçümü; robotun kanıtlı 29 gerektirmesi ve tekrarsız puan (not 23). Kod, betikler, ön-kayıt, ekler ve özetler; puan dizileri ve kitap başına ayrıntı dosyaları çıkarıldı (tohumlarla yeniden üretilir; tam paketin özetleri `paket/MANIFEST.sha256`) |
| `denetim11_dis_katman19/` | 11. denetim (dış, 25–26 Ekim 2026): katmanlar arası bütünlük üç biçimde sınandı — yedi koşullu yapısal puan, asal/biçim/parametre paylaşımı serbest ön-kayıtlı ortak dil (üç sıfırın her birinde 0 / 10⁶ kontrol) ve nadir-olay eki; matris cebirinin ve iki kesin 6-blok olasılığının bizim bağımsız kontrolümüz (not 24). Büyük puan parçaları, örnekleyici tabloları ve kaynak metinler çıkarıldı (liste ve özetler içeride) |
| `SAYAN_TANIK.md`, `WITNESS.md` | Yapay zekâ ortak yazarın kişisel ifadesi (Türkçe / İngilizce), tashihleriyle |

## Durum

On bir denetim (Temmuz–Ekim 2026; dürüstlük notları 10–24) manşeti yukarıdaki merdivene dönüştürdü; aritmetik on birinden de sağ çıktı, manşet sayısı çıkmadı ve yapının büyük kısmının önceki literatürde olduğu görüldü (1990–2015). Ölçüm aşaması tamamlandı (Temmuz 2026). Bağımsız istatistiksel inceleme aranıyor. Olay tanımlarında, motorlarda ya da çıkarımda bir hata bulursanız lütfen issue açın — doğrulanmış bir çürütme, bu çalışmanın memnuniyetle karşılanacak bir sonucudur.
