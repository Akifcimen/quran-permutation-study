# K×100 plasebo kalibrasyonu, eşit derinlik — ön-kayıt (ana hat, 31 Ekim / 1 Kasım 2026)

## 1. Soru

K×10 çalışmasında (`~/opus5.5/k10/RAPOR.md`) üç sıfırın üçünde de birincil okuma "arada" çıktı: 4/2001, 5/1001, 8/1001. İkincil derinleştirmede birer ikişer plasebo kaldı (R_derin 1, 1, 2). Bu plasebolar Kur'an'a 2–3 kat yakın; 10⁹ kontrolde Kur'an'dan ayrılamadılar. Kur'an'ın kendi değeri ise 10¹⁰ kontroldendir.

İki zayıflık var:
1. Kalibre p, R = 1–2'ye dayanıyor; Poisson belirsizliği 3–5 kat.
2. Plasebolar Kur'an'dan daha sığ sayıldı.

Bu çalışmanın soruları:
- **(a)** Kur'an'ınki kadar nadir bir bütünlük dili kaç rastgele kitapta bir çıkar? Hedef ±%30 kesinlik.
- **(b)** Eşit derinlikte (10¹⁰) Kur'an'dan ayrılamayan kaç plasebo kalır?
- **(c)** Nokta kestiriminde Kur'an'dan daha nadir dil kuran plasebo var mı?

## 2. Yöntem

K×10'un aynısıdır (`~/opus5.5/k10/ONKAYIT.md` §2–§3, `~/opus5.5/kapi/ONKAYIT.md`). Aynı kalanlar:
- kapı Gc (her hücre ≥ 2);
- 109 yüklemlik kütüphane;
- bütünlük kuralı (≥ 3 tür, ≥ 3 düzey), dil bedeli, Kol A dil arama yordamı (`dil_b A`);
- kontrol tohumları (tür 2, bölüt numarası);
- hedef üreticisi (`hedef.c`, tohum(s, 3, h));
- M20 ısınması 30.000 (ısınma tanısı: ihmal edilebilir).

Değişenler:

| | K×10 | K×100 |
|---|---|---|
| Plasebo sayısı K | M20 2.000; Mallows, tekdüze 1.000 | **M20 20.000; Mallows 10.000; tekdüze 10.000** |
| Hedefler | h = 1..K | h = 1..K. İlk 2.000 / 1.000 hedef K×10'unkilerle bayt bayt aynı olmalı; koşudan önce sınanır |
| Sayaç | işlemci (`kontrol_b`); derinleştirmenin kalanı kart | **kart sayacı** (`~/hrf_olcum/kart/bin/kart_b`). Eşdeğerliği kabul edildi (`kart/ek/EK_20261031T2010Z_kart_tasarim.md`; v1, v2 iki kartta 7/7, fark 0). Dil sınırı 4.096'dan 32.768'e çıkar; kabul sınaması koşudan önce v3 olarak tekrarlanır |
| Bellek ölçümü | `kontrol_b` (KONTROL_BELLEK) | aynı (işlemci ikilisi; yalnız tablo boyu basar) |

**Düzeyler.** Bölüt başına 250.000 kontrol.

| Düzey | Bölüt | Kümülatif kontrol | Seçim |
|---|---|---|---|
| 1 | 1–4 | 10⁶ | bütün diller |
| 2 | 5–40 | 10⁷ | hedef 0 ∪ {k(1–4) < 10} |
| 3 | 41–400 | 10⁸ | hedef 0 ∪ {düzey 2'de olup k(1–40) < 10} |
| 4 | 401–4.000 | 10⁹ | hedef 0 ∪ {birincil kuralda R'ye girenler} (K×10 §3 ikincil derinleştirmesi) |
| 5 | 4.001–40.000 | **10¹⁰** | hedef 0 ∪ {R_derin'e girenler} (yeni) |

Düzey 1–3 K×10 ile aynıdır. Hedef 0'ın (Kur'an) dilleri her düzeyde sayılır. Kur'an'ın düzey 4–5 sayımları, kapı çalışmasının `kontrol_h` ile yaptığı Gc 10¹⁰ sayımıyla (aynı tohumlar) bölüt bölüt aynı çıkmalıdır. Bu, iki sayaç yolu arasında bir eşdeğerlik sınamasıdır; Mallows için yarış düzeltmeli sayım (`yaris/kuran/Gc/L`) kullanılır.

## 3. Okuma kuralları

- p_x = (k + 1)/(N + 1), %95 Poisson-Garwood aralığı.
- **Birincil derinlik:** k(1–4) ≥ 10 ise 1–4; değilse k(1–40) ≥ 10 ise 1–40; değilse 1–400.
- **En derin:** dilin sayıldığı en derin düzey (d5 varsa 1–40.000; d4 varsa 1–4.000; yoksa birincil).

| Ölçü | Plasebo | Kur'an | Sayılan |
|---|---|---|---|
| **R₁** (birincil, K×10 kuralının aynısı) | birincil derinlik | 1–400 sayımı (10⁸) | p_x ≤ p_Q ya da aralık çakışıyor |
| **R_derin** (K×10 ikincilinin aynısı) | d4'e kadar en derin | Gc 10¹⁰: M20 4,7×10⁻⁹ [3,5; 6,3], Mallows 2,2×10⁻⁹ [1,4; 3,3], tekdüze 1,2×10⁻⁹ [0,6; 2,1] | aynı ölçüt |
| **R_eşit — karar ölçüsü** | d5'e kadar en derin; R_eşit'e girebilecek her plasebo 10¹⁰'da, Kur'an'la aynı derinlik ve aynı kontrol bölütlerinde | Gc 10¹⁰ | aynı ölçüt |
| **R_nokta** (nokta kestirimi) | en derin, p̂_x = k/N | p̂_Q = k/N (Gc 10¹⁰) | p̂_x ≤ p̂_Q |

- p_kal = (1 + R)/(K + 1). Her R için yazılır.
- **Okuma (R_eşit ile):** R_eşit = 0 ise aşar; R_eşit/K ≥ 0,05 ise sık; arası arada. Birincil karar sıralı sıfırlarda (M20, Mallows); tekdüze ikincil.
- **(a)'nın kestirimi:** π̂ = R_nokta/K, Poisson %95 aralığıyla. Bu, "Kur'an'ınki kadar nadir dil kuran rastgele kitapların oranı"dır. R_eşit/K bunun temkinli (Kur'an'ın aleyhine) üst ucudur.
- **(c) için:** R_nokta > 0 ise "Kur'an her sıfırda en nadir dildir" cümlesi o sıfırda düşer.

**Betimsel:** her sıfırda en küçük p'li on plasebo; mercekleri ve dilleri; R_nokta üyelerinin dilleri; ilk 2.000 / 1.000 plasebonun K×10 sayımlarıyla bölüt bölüt aynılığı.

**Betimsel ek ölçüm (zaman kalırsa, kararlarda kullanılmaz):** Kur'an'ın Gc sayımı `kart_h` ile 10¹¹'e (bölüt 1–400.000). p̂_Q'nun kesinliği için.

## 4. Ana hattın tahmini (sonuçlardan önce)

K×10'un en küçük p'li on plasebosuna kuyruk uydurmasıyla, p_(r) ∝ r^α (α ≈ 1,45 / 1,8 / 1,9).

| | M20 (K = 20.000) | Mallows (K = 10.000) | tekdüze (K = 10.000) |
|---|---|---|---|
| R₁ | ~30 (18–45) | ~40 (25–60) | ~70 (45–100) |
| R_derin | ~10 (4–20) | ~10 (4–20) | ~20 (8–35) |
| R_eşit | ~8 (3–15) | ~8 (3–15) | ~9 (3–18) |
| R_nokta | ~5 (1–12) | ~5 (1–12) | ~6 (1–14) |
| π̂ | ~1/4.000 | ~1/2.000 | ~1/1.700 |
| P(R_nokta ≥ 1) | ≈ 0,95 | ≈ 0,95 | ≈ 0,95 |

Okuma: üçünde de "arada".

**Uyarı:** Ana hattın K×10 ve kapı çalışmasındaki tahminleri, Kur'an'ın plasebolardan ayrışmasını her seferinde büyük gördü. Burada da sapma varsa büyük olasılıkla aynı yöndedir: R'ler daha büyük çıkar. Tahmin hükmü: okuma sınıfı tutarsa "tuttu", sayılar aralıktaysa "aralıkta".

## 5. Kaynak ve doğrulama

- **Kaynak:** workstation, `~/hrf_olcum/k100`. İşlemci tarafı (hedef, dil arama, bellek ölçümü) en çok 12 iş parçacığı, `nice -n 10`. Kartlar: RTX 4090 bölüt b mod 5 ∈ {0, 1, 2}, RTX 3090 {3, 4}. Kartları süren 2 iş parçacığı 12'ye sayılır; işlemci tarafı kartlarla aynı anda çalışırken 10 iş parçacığını aşmaz.
- **Tahmini süre:** işlemci ~15–30 dk; kartlar ~3–5 saat (düzey 5'in payı ~2 saat).

**Doğrulama:**
1. Bağımsız yeniden sayım: `dogrulama/yeniden_say_k100.py`. Bütün partiler okunur; eksik bölütte durur.
2. Her sıfırda en küçük p'li beş plasebonun, R_nokta üyelerinin ve Kur'an'ın dili `plasebo_dil_sina.py` ile sınanır.
3. İlk 2.000 / 1.000 hedef K×10 ile bayt bayt aynı olmalı. Onların düzey 1–3 sayımları K×10'unkilerle aynı olmalı.
4. Kur'an'ın (hedef 0) bölüt 1–40.000 sayımları (k ve kB) kapı çalışmasının `kontrol_h` Gc sayımlarıyla aynı olmalı.
5. Kartın yazdığı rastgele 10 bölüt, işlemci sayacıyla (tek iş parçacığı) yeniden hesaplanıp karşılaştırılır.

Sapmalar `ek/` altında zaman damgalı yazılır.
