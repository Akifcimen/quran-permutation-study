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
| **Combined core** | **≈ 1×10⁻¹⁵ (about 1 in a quadrillion)** — by two independent routes: fully direct 8.4×10⁻¹⁶; dependence-corrected chain 1.02×10⁻¹⁵ |

The summit event (parity balance ∧ full crystal) was **directly observed 5 times in 24 trillion trials** (two independent RNG seeds: 3+2), confirming the chained estimate (ratio 0.93). The only inter-layer dependence found (×1.126±0.033 at one mid-level joint) is measured and included.

**Specificity:** the same 8 structural criteria applied to every modulus 3–40: only m = 19 scores 8/8 (nearest rival 3/8). In 500,000 synthetic arrangements given free choice of modulus, none reached even 7/8.

An optional matrix-theoretic layer (both group matrices are primitive elements of GL₂(F₁₉), full 360-cycle with −I at step 180) multiplies the composite to ≈1.2×10⁻¹⁷, but requires the contested 127-verse count for surah 9 and is therefore reported separately, never in the headline.

## Honesty ledger (read this before the results)

- All event definitions were frozen before their measurement runs (`kesif*_prereg.md` files).
- ~75 candidate statistic families were examined across the study; **negative results are reported** (Meccan/Medinan split, muqatta'at set, window/interval structures, multiplicity and parity lenses — all null). Worst-case Bonferroni over the ledger leaves the core at ~10⁻¹³.
- Automatic (algebraically forced) structures were identified and **excluded** from evidence (documented inside `KESIF.md`).
- Decorative near-patterns ("flourishes") are recorded with their measured, unimpressive probabilities and **not multiplied** into the composite.
- This is a post-hoc discovery: the definitive epistemic step is **pre-registered independent replication** — which this repository exists to enable. Please try to break it.

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
| `KESIF.md` | Main report (Turkish): layers, measurements, specificity, ledger |
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
| **Birleşik çekirdek** | **≈ 1×10⁻¹⁵ (yaklaşık katrilyonda 1)** — iki bağımsız yolla: tamamen doğrudan 8,4×10⁻¹⁶; bağımlılık-düzeltmeli zincir 1,02×10⁻¹⁵ |

Zirvedeki olay (parite terazisi ∧ tam kristal) **24 trilyon denemede 5 kez doğrudan gözlenmiştir** (iki bağımsız RNG tohumu: 3+2) ve zincirleme tahmini doğrular (oran 0,93). Bulunan tek katmanlar-arası bağımlılık (bir ara eklemde ×1,126±0,033) ölçülmüş ve hesaba dahil edilmiştir.

**Özgüllük:** aynı 8 yapısal ölçüt 3–40 arası her modüle uygulandı: yalnız m = 19 tam puan alır (en yakın rakip 3/8). Modül seçiminde tam serbestlik verilen 500.000 sentetik dizilişte hiçbiri 7/8'e bile ulaşamadı.

İsteğe bağlı bir matris-kuramsal katman (iki grup matrisi de GL₂(F₁₉)'un ilkel elemanıdır; 180. adımda −I ile tam 360'lık çevrim) bileşiği ≈1,2×10⁻¹⁷'ye taşır; ama sûre 9 için tartışmalı 127 sayımını gerektirdiğinden ayrı raporlanır, manşete asla girmez.

## Dürüstlük defteri (sonuçlardan önce okuyun)

- Tüm olay tanımları, ölçüm koşularından önce donduruldu (`kesif*_prereg.md` dosyaları).
- Çalışma boyunca ~75 aday istatistik ailesi incelendi; **negatif sonuçlar raporlanmıştır** (Mekkî/Medenî ayrımı, mukattaa kümesi, pencere/aralık yapıları, çokluk ve parite mercekleri — hepsi boş). Defter üzerinden en kötü durum Bonferroni düzeltmesi çekirdeği ~10⁻¹³'te bırakır.
- Otomatik (cebirsel olarak zorunlu) yapılar tespit edilip kanıttan **dışlanmıştır** (KESIF.md içinde belgelidir).
- Dekoratif yarı-desenler ("süsler") ölçülmüş, etkileyici olmayan olasılıklarıyla kayda geçirilmiş ve bileşiğe **çarpılmamıştır**.
- Bu, sonradan yapılmış (post-hoc) bir keşiftir: kesin epistemik adım **önceden kayıtlı bağımsız tekrardır** — bu depo tam da bunu mümkün kılmak için vardır. Lütfen kırmayı deneyin.

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
| `KESIF.md` | Ana rapor (Türkçe): katmanlar, ölçümler, özgüllük, defter |
| `RAPOR.md` | Önceki kaynak dokümanın matris iddialarının doğrulama raporu |
| `test_oruntular.py` | Tüm iddiaların iki kodeks altında tek seferde doğrulanması |
| `kesif2_motor.c` → `kesif4_motor.c` | CPU permütasyon motorları (v4 = tam sayaç seti) |
| `kesif_cuda.cu`, `README_4090.md` | GPU motoru + doğrulama protokolü |
| `kesif*.py` | Taramalar, mercekler, kontroller (yüzer-modül, sıkı-S4 vb.) |
| `kesif*_prereg.md` | Dondurulmuş ön-kayıtlar |
| `quran_meta.json`, `quran_chapters_qcom.json` | Ham veri (iki bağımsız kaynak) |

## Durum

Ölçüm aşaması tamamlandı (Temmuz 2026). Bağımsız istatistiksel inceleme aranıyor. Olay tanımlarında, motorlarda ya da çıkarımda bir hata bulursanız lütfen issue açın — doğrulanmış bir çürütme, bu çalışmanın memnuniyetle karşılanacak bir sonucudur.
