# Quran Permutation Study

Permutation-test analysis of arrangement structure in the Quran's surah numbers and verse counts. Zero-freedom inputs, pre-registered event definitions, **24 trillion direct trials**. Fully reproducible — **refutation welcome**.

## What this is (and is not)

This repository documents a measured **statistical anomaly** in how verse counts are arranged across the Quran's 114 surahs. It is **not** a claim of proof of design or miracle — that question is outside what statistics can answer. Every claim here is a counted frequency that you can recompute on your own machine.

**Inputs (the only data used):** surah index n ∈ {1..114} and verse count a(n), from two cross-validated public APIs (api.alquran.cloud, api.quran.com — identical for all 114 surahs). No letter counts, no gematria, no word counts — nothing with counting-convention freedom. The core results are additionally insensitive to the one contested count (surah 9: 127 vs 129 verses).

## Headline result

Define t(n) = n + a(n) and u(n) = n − a(n). Under the null hypothesis "the verse-count multiset is randomly assigned to surah positions," the real arrangement simultaneously exhibits a nested structure whose measured probability is:

| Event (codex-independent core) | Direct measurement |
|---|---|
| Parity balance: 57/57 split; Σt(even) = total verse count; Σt(odd) = Σ surah numbers | p = 3.137×10⁻⁴ |
| Full "crystal": 12-member t≡0 (mod 19) family, Σ = 38², 6/6 parity halves 722/722, four 3-cells with coefficients {18,19,19,20}, individual coefficients exactly the consecutive band {5..9} | p = 7.14×10⁻¹⁰ (17,204 hits) |
| "Two Hands": the mod-19 families of t and u are disjoint and total exactly 19 surahs, with 19 \| Σa(union) | conditional ≈ 0.41% |
| **Combined core** | **≈ 1.7×10⁻¹⁵ (quadrillion order; 95% CI 1.1–2.6×10⁻¹⁵)** — measured **without** an independence assumption, via conditional decomposition (`kesif18`): P(crystal) directly counted (17,204 hits) × P(balance∧TwoHands \| crystal) directly sampled (22 hits / 9.0M i.i.d. conditional samples). Earlier chained estimates (0.8–1.0×10⁻¹⁵) lie inside the CI |

The summit **pair** (parity balance ∧ full crystal) was **directly observed 5 times in 24 trillion trials** (two independent seeds of the same generator family, 3+2; an independent NumPy/PCG64 stack reproduces the marginal rates): directly counted **p = 2.08×10⁻¹³ — 1 in 4.8 trillion** — consistent with the chained expectation (ratio 0.93; wide Poisson interval at 5 hits). Measured dependences (×1.126±0.033 at one mid-level joint; ×1.6 [1.0–2.4] three-way interaction at the conditional level) are included.

**Specificity:** the same 8 structural criteria applied to every modulus 3–40: only m = 19 scores 8/8 (nearest rival 3/8). In 500,000 synthetic arrangements given free choice of modulus, none reached 7/8 — a sample-size bound, not a ceiling: a 50-million-trial red-team run measured strict 7/8 at 2.4×10⁻⁷ and strict 8/8 at 0/50M; under loosened ("forked") criteria 8/8 appears at 1.4×10⁻⁷ (fixed lens) / 5×10⁻⁵ (free lens). The KJV corpora reach at most 6/8 even under those loosened criteria (`kesif22`; honesty note 13).

**The honest ladder (after three external reviews — honesty note 13).** The headline is not a single number but a ladder; each rung states its assumption:

| Rung | Assumption | Value |
|---|---|---|
| Event frequency | the stated exact event, fixed lens | **1.7×10⁻¹⁵** |
| Class-level core | crystal predicates at class level (band width fixed / band free), `kesif21` | ≈1×10⁻¹⁴ / ≈5×10⁻¹⁴ |
| Fork-priced core | exact core × composite fork cost (×6.5–27, red team round 2; the ×120 layer product was an over-estimate) | ≈1×10⁻¹⁴ – 5×10⁻¹⁴ |
| Class-level × forks | both of the above | ≈10⁻¹² |
| Loose profile | the red team's S1–S8 profile × balance, lens anchored by the text (74:30, 89:3); strict profile 6.7×10⁻⁹, forked up to 1.8×10⁻⁷ | ≈10⁻¹¹ – 10⁻¹⁰ |
| Free lens | anchor rejected (×~400 lens choices) | ≈10⁻⁸ |
| Beyond | undocumentable cultural selection freedom | unpriceable (note 12) |

No rung is a calibrated p-value; each is a frequency under a stated assumption (note 14). Controls — single real draws cannot measure the rarity of a 10⁻⁵–10⁻⁹ event; what they establish is that the permutation null is a fair proxy for real structured lists: the KJV corpora sit at the synthetic median even under the loosest procedure (`kesif22`); the same 114 counts in their real revelation-order arrangement show no layer (`kesif23`; excluding the arrangement-blind form v = a, the contrast is 8/8 vs ≤5/8); a mode-neutral class-level typicality test places the book's family above 99.8–99.995% of random families (red team, note 14).

An optional matrix-theoretic layer (both group matrices are primitive elements of GL₂(F₁₉), full 360-cycle with −I at step 180) multiplies the composite to ≈1.2×10⁻¹⁷, but requires the contested 127-verse count for surah 9 and is therefore reported separately, never in the headline.

## Honesty ledger (read this before the results)

- All event definitions were frozen before their measurement runs (`kesif*_prereg.md` files).
- ~75 candidate statistic families were examined across the study; **negative results are reported** (Meccan/Medinan split, muqatta'at set, window/interval structures, multiplicity and parity lenses — all null). Worst-case Bonferroni over the ledger leaves the core at ~10⁻¹³.
- Automatic (algebraically forced) structures were identified and **excluded** from evidence (documented inside `KESIF.md`).
- Decorative near-patterns ("flourishes") are recorded with their measured, unimpressive probabilities and **not multiplied** into the composite.
- **Counting-school scope:** the structure is specific to the **Kufan versification** (the standard count of virtually all modern mushafs; robust within it to the 127/129 dispute). Tested against the Medinan count (Warsh/Qalun, official 6214): it does **not** hold there (`kesif11_sayim_okullari.py`). With ~7 known counting schools, school-selection freedom costs at most a ×7 Bonferroni factor (worst-case core ~10⁻¹⁴).
- **Counter-corpus control:** the same 8 criteria applied verbatim to real other texts (KJV Psalms, books, chapters; canonical totals validated) score 2/8, 3/8, 2/8 — the synthetic noise floor. Structured real texts do not reproduce the pattern (`kesif12_karsi_korpus.py`).
- This is a post-hoc discovery on a fixed text: there will never be fresh data, so **no calibrated global p-value exists for it** (honesty notes 11–13). What can be done — and what this repository exists to enable — is independent re-derivation with independent code (done three times), application of the frozen procedure to other corpora, and independent audit of the selection pricing (the ladder in note 13). Please try to break it.

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

## Status

Measurement phase complete (July 2026). Seeking independent statistical review. If you find an error in the event definitions, the engines, or the inference, please open an issue — a confirmed refutation is a welcome outcome of this study.

---

# Kur'an Permütasyon Çalışması (Türkçe)

Kur'an'ın sûre numaraları ve ayet sayılarının diziliş yapısı üzerine permütasyon-testi analizi. Sıfır serbestlikli girdi, önceden kaydedilmiş olay tanımları, **24 trilyon doğrudan deneme**. Tamamen yeniden üretilebilir — **çürütme davetlidir**.

## Bu nedir (ve ne değildir)

Bu depo, ayet sayılarının 114 sûreye dizilişinde ölçülmüş bir **istatistiksel anomaliyi** belgeler. Bir tasarım ya da mucize **ispatı iddiası değildir** — o soru istatistiğin cevaplayabileceği alanın dışındadır. Buradaki her iddia, kendi makinenizde yeniden hesaplayabileceğiniz sayılmış bir frekanstır.

**Girdiler (kullanılan tek veri):** sûre numarası n ∈ {1..114} ve ayet sayısı a(n) — çapraz doğrulanmış iki açık kaynaktan (api.alquran.cloud, api.quran.com; 114 sûrenin tamamında birebir aynı). Harf sayımı yok, ebced yok, kelime sayımı yok — sayım-geleneği serbestliği taşıyan hiçbir şey kullanılmamıştır. Çekirdek sonuçlar, tek tartışmalı sayımdan (sûre 9: 127/129) da bağımsızdır.

## Ana sonuç

t(n) = n + a(n) ve u(n) = n − a(n) tanımlansın. "Ayet-sayısı çokluğu sûre konumlarına rastgele dağıtılmıştır" sıfır hipotezi altında, gerçek diziliş şu iç içe yapıyı aynı anda taşır:

| Olay (kodeks-bağımsız çekirdek) | Doğrudan ölçüm |
|---|---|
| Parite terazisi: 57/57; Σt(çift) = toplam ayet; Σt(tek) = sûre numaraları toplamı | p = 3,137×10⁻⁴ |
| Tam "kristal": 12 üyeli t≡0 (mod 19) ailesi, Σ = 38², 6/6 parite kefeleri 722/722, katsayıları {18,19,19,20} olan dört 3'lü hücre, bireysel katsayılar tam olarak ardışık {5..9} bandı | p = 7,14×10⁻¹⁰ (17.204 isabet) |
| "İki El": t ve u'nun mod-19 aileleri ayrıktır ve birlikte tam 19 sûredir; 19 \| Σa(birleşim) | koşullu ≈ %0,41 |
| **Birleşik çekirdek** | **≈ 1,7×10⁻¹⁵ (katrilyonda-1 mertebesi; %95 GA 1,1–2,6×10⁻¹⁵)** — bağımsızlık varsayımı **olmadan**, koşullu ayrışımla ölçüldü (`kesif18`): P(kristal) doğrudan sayım (17.204 isabet) × P(terazi∧İkiEl \| kristal) doğrudan örnekleme (9,0 milyon i.i.d. koşullu örnekte 22 isabet). Eski zincir tahminleri (0,8–1,0×10⁻¹⁵) aralığın içindedir |

Zirvedeki **ikili** olay (parite terazisi ∧ tam kristal) **24 trilyon denemede 5 kez doğrudan gözlenmiştir** (aynı üreteç ailesinden iki bağımsız tohum, 3+2; bağımsız NumPy/PCG64 yığını marjinal oranları yeniden üretmektedir): doğrudan sayılmış **p = 2,08×10⁻¹³ — 4,8 trilyonda 1** — zincir beklentisiyle tutarlı (oran 0,93; 5 isabette Poisson aralığı geniştir). Ölçülen bağımlılıklar (ara eklemde ×1,126±0,033; koşullu düzeyde ×1,6 [1,0–2,4] üçlü etkileşim) hesaba dahildir.

**Özgüllük:** aynı 8 yapısal ölçüt 3–40 arası her modüle uygulandı: yalnız m = 19 tam puan alır (en yakın rakip 3/8). Modül seçiminde tam serbestlik verilen 500.000 sentetik dizilişte hiçbiri 7/8'e ulaşmadı — bu bir örneklem sınırıdır, tavan değil: 50 milyonluk kırmızı-takım koşusu katı 7/8'i 2,4×10⁻⁷, katı 8/8'i 0/50M ölçtü; gevşetilmiş ("çatallı") ölçütlerle 8/8 sabit mercekte 1,4×10⁻⁷, serbest mercekte 5×10⁻⁵. KJV korpusları bu gevşek ölçütlerle bile en çok 6/8 alır (`kesif22`; dürüstlük notu 13).

**Dürüst merdiven (üç dış denetimden sonra — dürüstlük notu 13).** Manşet tek bir sayı değil, bir merdivendir; her basamak kendi varsayımını söyler:

| Basamak | Varsayım | Değer |
|---|---|---|
| Olay frekansı | beyan edilen tam olay, sabit mercek | **1,7×10⁻¹⁵** |
| Sınıf-düzeyi çekirdek | kristal yüklemleri sınıf düzeyinde (bant genişliği sabit / bant serbest), `kesif21` | ≈1×10⁻¹⁴ / ≈5×10⁻¹⁴ |
| Çatal-fiyatlı çekirdek | tam çekirdek × bileşik çatal bedeli (×6,5–27, kırmızı takım 2. tur; ×120 katman-çarpımı aşırıydı) | ≈1×10⁻¹⁴ – 5×10⁻¹⁴ |
| Sınıf-düzeyi × çatallar | ikisi birden | ≈10⁻¹² |
| Gevşek profil | kırmızı takımın S1–S8 profili × terazi, mercek metnin çapasında (74:30, 89:3); katı profil 6,7×10⁻⁹, çatallı 1,8×10⁻⁷'ye kadar | ≈10⁻¹¹ – 10⁻¹⁰ |
| Serbest mercek | çapa reddedilirse (×~400 mercek seçeneği) | ≈10⁻⁸ |
| Ötesi | belgelenemeyen kültürel seçim serbestliği | fiyatlanamaz (not 12) |

Hiçbir basamak kalibre bir p-değeri değildir; her biri beyan edilen varsayım altında bir frekanstır (not 14). Kontroller — tek tek gerçek çekilişler 10⁻⁵–10⁻⁹'luk bir olayın nadirliğini ölçemez; gösterdikleri şey permütasyon sıfırının gerçek yapılı listeler için adil bir vekil olduğudur: KJV korpusları en gevşek prosedürle bile sentetik medyanında kalır (`kesif22`); aynı 114 sayının gerçek nüzul-sırası dizilişinde tek bir katman yoktur (`kesif23`; dizilişe kör v = a biçimi dışlanınca kontrast 8/8'e karşı ≤5/8); mod-ayrıcalıksız sınıf-düzeyi tipiklik testi kitabın ailesini rastgele ailelerin %99,8–99,995'inin üstüne koyar (kırmızı takım, not 14).

İsteğe bağlı bir matris-kuramsal katman (iki grup matrisi de GL₂(F₁₉)'un ilkel elemanıdır; 180. adımda −I ile tam 360'lık çevrim) bileşiği ≈1,2×10⁻¹⁷'ye taşır; ama sûre 9 için tartışmalı 127 sayımını gerektirdiğinden ayrı raporlanır, manşete asla girmez.

## Dürüstlük defteri (sonuçlardan önce okuyun)

- Tüm olay tanımları, ölçüm koşularından önce donduruldu (`kesif*_prereg.md` dosyaları).
- Çalışma boyunca ~75 aday istatistik ailesi incelendi; **negatif sonuçlar raporlanmıştır** (Mekkî/Medenî ayrımı, mukattaa kümesi, pencere/aralık yapıları, çokluk ve parite mercekleri — hepsi boş). Defter üzerinden en kötü durum Bonferroni düzeltmesi çekirdeği ~10⁻¹³'te bırakır.
- Otomatik (cebirsel olarak zorunlu) yapılar tespit edilip kanıttan **dışlanmıştır** (KESIF.md içinde belgelidir).
- Dekoratif yarı-desenler ("süsler") ölçülmüş, etkileyici olmayan olasılıklarıyla kayda geçirilmiş ve bileşiğe **çarpılmamıştır**.
- **Sayım okulu kapsamı:** yapı **Kûfe sayımına** özgüdür (modern mushafların standart sayımı; kendi içindeki 127/129 ihtilafına dayanıklı). Medine sayımıyla (Warş/Kalûn, resmî 6214) test edildi: orada **tutmaz** (`kesif11_sayim_okullari.py`). Bilinen ~7 sayım okuluyla, okul-seçimi serbestliğinin bedeli en fazla ×7'lik Bonferroni çarpanıdır (en kötü durumda çekirdek ~10⁻¹⁴).
- **Karşı-korpus kontrolü:** aynı 8 ölçüt gerçek başka metinlere aynen uygulandı (KJV Mezmurlar, kitaplar, bölümler; kanonik toplamlar doğrulandı): 2/8, 3/8, 2/8 — sentetik gürültü zemini. Yapılı gerçek metinler deseni üretmiyor (`kesif12_karsi_korpus.py`).
- Bu, sabit bir metin üzerinde sonradan yapılmış (post-hoc) bir keşiftir: yeni veri hiç olmayacak, dolayısıyla **bu veri için kalibre edilmiş küresel bir p-değeri yoktur** (dürüstlük notları 11–13). Yapılabilen — ve bu deponun var olma sebebi — bağımsız kodla yeniden türetme (üç kez yapıldı), dondurulmuş prosedürün başka korpuslara uygulanması ve seçim fiyatlamasının bağımsız denetimidir (not 13'teki merdiven). Lütfen kırmayı deneyin.

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

## Durum

Ölçüm aşaması tamamlandı (Temmuz 2026). Bağımsız istatistiksel inceleme aranıyor. Olay tanımlarında, motorlarda ya da çıkarımda bir hata bulursanız lütfen issue açın — doğrulanmış bir çürütme, bu çalışmanın memnuniyetle karşılanacak bir sonucudur.
