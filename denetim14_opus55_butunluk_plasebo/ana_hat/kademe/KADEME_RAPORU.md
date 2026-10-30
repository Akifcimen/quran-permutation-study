# KADEME RAPORU — Kur'an'ın plasebo14 Kol A dilindeki kendi sırası, derin sayım

**Soru.** plasebo14'te Kur'an'ın kendi sırası üç sıfırda da 0 / 10⁸ çıktı (%95 üst sınır 3,7×10⁻⁸). Aynı donmuş dil, eşik, tohum akışı ve sayım tanımıyla kontrol sayısı artırılınca sıra ne çıkar?

| | |
|---|---|
| Ön-kayıt | `ONKAYIT.md`, donduruldu **2026-10-29T09:11:16Z**, SHA-256 `db06c6f9…` (`sonuc/onkayit_ozet.txt`) |
| Hızlı sayaç | `src/kontrol_h.c`, SHA-256 `33856c3b…ba1e`; bölüt 1–400'ü üç sıfırda plasebo14'le birebir verdi (1.200 bölüt, FARK 0; ön-kayıt §3) |
| Hesap | workstation (i9-10920X), `nice -n 10`, 12 iş parçacığı; S2 09:12–09:23Z, S3 09:24–11:29Z |
| Sapma | üç ek (`ek/`): kapı G çekincesi yüzünden **S4 ertelendi**, S3 nihai kademe (09:41Z, S3 sonuçları görülmeden); kod denetiminin bulduğu Mallows iş parçacığı yarışı (19:02Z) ve düzeltilmiş yeniden sayım (23:40Z): Mallows toplamları değişmedi (§3.4) |
| Doğrulama | ön-kayıt §6'nın üç sınamasının hepsi geçti (§3) |

## 0. Kısa cevaplar

### 0.1 Sonuç (S3, N = 10¹⁰, kapı G2E)

| Dil, sıfır | k (küme) | p̂ | %95 | kB / N | durum |
|---|---|---|---|---|---|
| **A, M20** (birincil) | **4 (4)** | **4×10⁻¹⁰** | [1,1×10⁻¹⁰; 1,0×10⁻⁹] | 1,16×10⁻⁷ | k < 10; S4 ertelendi |
| A, tekdüze | 0 | — | < 3,7×10⁻¹⁰ | 7,6×10⁻⁸ | k < 10; S4 ertelendi |
| A, Mallows | 1 | 1×10⁻¹⁰ | [2,5×10⁻¹²; 5,6×10⁻¹⁰] | 1,21×10⁻⁷ | k < 10; S4 ertelendi |
| A−, M20 | 15 (15) | 1,5×10⁻⁹ | [8,4×10⁻¹⁰; 2,5×10⁻⁹] | 1,43×10⁻⁶ | **k ≥ 10, ölçüldü** |
| A+, M20 | 5 (5) | 5×10⁻¹⁰ | [1,6×10⁻¹⁰; 1,2×10⁻⁹] | 5×10⁻¹⁰ | k < 10; S4 ertelendi |

A = {B2, K1, Qc, WSum} @ (1,+1,19); A− = {B2, K1, Qc}; A+ = {B2, K1, Qh, WSum}. M20 isabetleri zincir kümesi oluşturmuyor (k = k_küme).

### 0.2 İsabetler ne?

- **A, M20:** dört isabetin üçü Kur'an'ın merceğinde, (1,+1,19), Kur'an'la tam eşit puanda (18,80 bit). Kur'an'ın iskeletinin kopyalarıdır: 12 üyeli aile, katsayı toplamları 18/19/19/20, terazi, k_H = 7; hücreler ikisinde 3/3/3/3, birinde 2/3/3/4. Dördüncüsü (1,+1,17)'de, 20,63 bit.
- **A, Mallows:** tek isabet yine (1,+1,19)'da, Kur'an'la eşit puanda.
- Bu kopyaların ayrıntılı karşılaştırması (Kur'an'la ortak konum sayısı, eksik yükleme göre) kapı çalışmasının raporunda (`~/opus5.5/kapi/KAPI_RAPORU.md` §3.3) ve karşılaştırma tablosunda.

### 0.3 Kapı çekincesi ve Gc ile derin sayım

Bu ölçünün tanımındaki kapı G (k_F·p ≥ 228) Kur'an'ın sınırındadır (12·19 = 228). Ek, S4'ü bu yüzden erteledi ve Kur'an'ın sırasını kapı çalışmasının birincil kapısı Gc ile (yalnız her hücre ≥ 2) aynı kademe ve durma kuralıyla ölçtü. Sonuç (10¹⁰, k ≥ 10 üç sıfırda): **M20 47 → 4,7×10⁻⁹, Mallows 22 → 2,2×10⁻⁹, tekdüze 12 → 1,2×10⁻⁹.** Ayrıntı `~/opus5.5/kapi/KAPI_RAPORU.md` §3. Kapı Gc'ye gevşeyince Kur'an'ın kendi sırası M20'de ~12, Mallows'ta ~22 kat sıklaşır. Bu raporun G2E sayıları, Kur'an'ın sınırına denk gelen bir kapıyla ölçüldüğü için birincil sonuç olarak kullanılmaz; birincil sonuç Gc sayılarıdır.

### 0.4 Okuma sınırı (ön-kayıt §1)

p_Q, Kur'an'ın kendi bütünlüklü dilindeki sırasıdır. Dilin Kur'an'a göre yazılmasının bedeli plasebo kalibrasyonudur (plasebo14 ve kapı çalışması: M20 1/201, Mallows 1/101). p_Q'nun derinleşmesi kalibre p'yi küçültmez. p_Q "Kur'an'ın rastlantı olma olasılığı" değildir.

## 1. Kademeler

| Kademe | bölüt | N | durum |
|---|---|---|---|
| S1 | 1–400 | 10⁸ | plasebo14; kontrol_h ile birebir yeniden sayıldı |
| S2 | 401–4.000 | 10⁹ | bitti, doğrulandı |
| S3 | 4.001–40.000 | 10¹⁰ | bitti, doğrulandı |
| S4 | 40.001–400.000 | 10¹¹ | **ertelendi** (ek) |
| S5 | — | 10¹² | koşulmadı |

Birikimli sayımlar:

| Dil, sıfır | S1 k (kB) | S2 k (kB) | S3 k (kB) |
|---|---|---|---|
| A, M20 | 0 (8) | 0 (124) | 4 (1.163) |
| A−, M20 | 0 (132) | 1 (1.397) | 15 (14.298) |
| A+, M20 | 0 (0) | 0 (0) | 5 (5) |
| A, tekdüze | 0 (6) | 0 (84) | 0 (755) |
| A, Mallows | 0 (10) | 0 (120) | 1 (1.211) |

S2'deki tek A− isabeti, Kur'an'ın çekirdeğinin birebir aynısını taşır (12 üye, 3/3/3/3, 20/18/19/19, B2), WSum'u taşımaz. Durma kuralına göre yalnız A−, M20 S3'te durdu. Öteki dört çift kural gereği S4'e geçecekti; S4 ek ile ertelendiği için S3 değerleriyle raporlanır.

## 2. Ana hattın tahmini (§7)

| Dil, sıfır | tahmin: merkez [%90] | S2'de k | gerçekleşen (S3) | hüküm |
|---|---|---|---|---|
| A, M20 | 3×10⁻¹⁰ [10⁻¹²; 3×10⁻⁹] | 0 (P ≈ 0,7) → 0 | 4×10⁻¹⁰ | **tuttu** |
| A, tekdüze | 10⁻¹¹ [10⁻¹³; 10⁻⁹] | 0 → 0 | 0 / 10¹⁰ (< 3,7×10⁻¹⁰) | tutarlı; k = 0 olduğu için hüküm verilemez |
| A, Mallows | 3×10⁻¹⁰ [10⁻¹²; 3×10⁻⁹] | 0 → 0 | 1×10⁻¹⁰ | **tuttu** |
| A−, M20 | 10⁻⁹ [10⁻¹¹; 10⁻⁸] | 0–2 → 1 | 1,5×10⁻⁹ | **tuttu** |
| A+, M20 | 10⁻¹¹ [10⁻¹⁴; 10⁻⁹] | 0 → 0 | 5×10⁻¹⁰ | **aralıkta** (merkezden 1,7 mertebe sık) |

kB oranları: M20 A için S2'de ~80 / 10⁹ (50–120) beklenmişti; 124 / 10⁹ çıktı, **aralığın az üstünde**. A− için ~1.300 / 10⁹ beklenmişti; 1.397 / 10⁹.

## 3. Doğrulama (§6)

1. **Her isabet ve her kayıt yeniden puanlandı.** S3'ün bütün kayıtları (M20 13.945, tekdüze 671, Mallows 1.091) donmuş tam yolla (`kdegerlendir` 0x3FFF, `kapi_listesi`, `puan_B`; `src/yeniden_puan.c`) yeniden puanlandı: mercek farkı 0, 10⁻⁵'ten büyük puan farkı 0 (`sonuc/dogrulama/S3_<S>.txt`). S1–S2 kayıtları için de aynısı (`S12_<S>.txt`).
2. **Tanımlardan sınama.** `kos/bagimsiz_sina.py`, C kodundan bağımsız olarak kapı G, B2, K1, Qc, Qh, WSum ve sınıf şartını sınar ve kaydın merceğinin bütünlüklü mercekler arasında olduğunu doğrular. S3 isabetleri (M20 üç dilde 23, Mallows 1) ve S2 isabeti: uyuşmazlık 0. Her sıfırda rastgele 20 bütünlüklü kayıt (tohum 20261029): uyuşmazlık 0.
3. **Özgün sayaçla örneklem.** Her kademede dört bölüt (ilk, son, %25, %75) plasebo14'ün özgün `kontrol_b`'siyle de sayıldı: M20 24 satır, tekdüze 8, Mallows 8; fark 0.

4. **Kod denetimi.** Kapı çalışmasıyla birlikte, sonuçları bilmeyen ayrı bir ajan denetledi (`~/opus5.5/kapi/KAPI_RAPORU.md` §4.4). Kısayolun güvenliği, tohumlar, bölüt sınırları ve kayıt biçimi doğru bulundu; kısayol, tam yolla kitap kitap aynı. Bir hata buldu: §3.4.

### 3.4 Mallows iş parçacığı yarışı
Mallows örnekleyicisinin kurulumu küresel bir referans dizisini yeniden yazıyordu ve `kontrol_h` bu kurulumu iş parçacıklarından kilitsiz çağırıyordu. Aynı anda kurulan bölütlerin referansı ya da q'su bozulabiliyordu. M20 ve tekdüze etkilenmez. Kurulum kilit altına alındı. Mallows bölüt 1–40.000 düzeltilmiş sayaçla yeniden sayıldı. Düzeltilmiş çıktının tek iş parçacıklı sayımla aynı olduğu sınandı.

Özgün sayımla bölüt bölüt karşılaştırma: **30 / 40.000 bölüt farklı**. Farklılar çoğunlukla çağrıların aynı anda başlayan ilk bölütleri (2, 4, 10, 401–407, 4008) ve aynı anda başlayan bölüt çiftleridir (ör. 837–838). Bunların çoğunda `b2` ~1.000'e çıkmıştı. **Toplamlar değişmedi:** A, Mallows k = 1, kB = 1.211. Ayrıntı `ek/EK_20261029T2340Z_mallows_yeniden_sayim_sonucu.md`.

## 4. Kaynaklar

| Yol | İçerik |
|---|---|
| `ONKAYIT.md`, `sonuc/onkayit_ozet.txt`, `sonuc/onkayit_kod_sha256.txt` | ön-kayıt ve donmuş kod özetleri |
| `src/kontrol_h.c`, `src/yeniden_puan.c`, `src/akis_ozet.c` | hızlı sayaç, yeniden puanlayıcı, akış özeti |
| `kos/kademe.sh`, `kos/karsilastir.sh`, `kos/kos_sina_h.sh`, `kos/ozet.py`, `kos/bagimsiz_sina.py` | akış, eşdeğerlik, özet, bağımsız sınama |
| `sonuc/kademe_gunlugu.txt`, `sonuc/ozet_S3.txt` | günlük ve özet |
| `sonuc/S2/`, `sonuc/S3/` | parça sayımları ve özgün sayaç örneklemleri |
| `sonuc/dogrulama/` | isabet ve kayıtların yeniden puanlanması |
| `ek/` | S4'ün ertelenmesi |
