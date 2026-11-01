# K×10 plasebo kalibrasyonu — sonuç (31 Ekim 2026)

**Ön-kayıt:** `ONKAYIT.md`, donduruldu 2026-10-31T16:22:46Z, SHA-256 `b394af2dd09c0bede9c46b2d3a5174f0642a1a0d62918d891beed8b4a345b081`.

**Sapmalar (`ek/`):**
- T1626Z: dil kapasitesi 1.024'ten 4.096'ya çıkarıldı.
- T2040Z: derinleştirmenin kalanı, eşdeğerliği kabul edilmiş kart sayacında hesaplandı.

İkisi de sonucu değiştirmez. Ara öngörüler (T1700Z, T1900Z) sonuçlardan önce yazıldı.

**Hesap:** workstation.
- Birincil: işlemci, 12 iş parçacığı, 16:24–17:49Z.
- Derinleştirme: işlemci 17:53–20:24Z, kart 20:27–20:37Z.

**Yeniden sayım:** `dogrulama/yeniden_say_k10.py`. Çıktılar: `sonuc/yeniden_sayim_{M,L,U}.txt` (birincil) ve `sonuc/yeniden_sayim_derin_{M,L,U}.txt`.

## Karar

**Birincil (ön-kayıt §3): üç sıfırda da "arada".** Kapı çalışmasında sıralı sıfırlarda "aşar" çıkmıştı (1/201, 1/101). Bu sonuç on kat plaseboyla tutmadı.

| Sıfır | K | Dilli | R | p_kal | Okuma |
|---|---|---|---|---|---|
| M20 | 2.000 | 1.987 | 3 (A535, A1645, A690) | 4/2001 = 0,0020 | arada |
| Mallows | 1.000 | 993 | 4 (A142, A786, A777, A891) | 5/1001 = 0,0050 | arada |
| tekdüze | 1.000 | 992 | 7 (A088, A358, A593, A925, A315, A491, A398) | 8/1001 = 0,0080 | arada |

Birincil kuralda Kur'an'ın değeri plasebolarla aynı derinlikten (10⁸) alınır. Orada Kur'an 0 isabetle [0; 3,7×10⁻⁸] aralığındadır. R'ye girenlerin çoğu bu geniş aralık yüzünden girer.

**İkincil derinleştirme (R_derin; plasebolar 10⁹, Kur'an Gc 10¹⁰):**

| Sıfır | Kur'an | R_derin'de kalan | Oran (nokta) | Ayrışan | R_derin | p_kal_derin |
|---|---|---|---|---|---|---|
| M20 | 4,7×10⁻⁹ [3,5; 6,3] | A535 10/10⁹: 1,1×10⁻⁸ [4,8×10⁻⁹; 1,8×10⁻⁸] | 2,3 | A1645 3,0×10⁻⁸; A690 6,4×10⁻⁸ | **1** | 2/2001 = 0,0010 |
| Mallows | 2,2×10⁻⁹ [1,4; 3,3] | A142 6/10⁹: 7,0×10⁻⁹ [2,2×10⁻⁹; 1,3×10⁻⁸] | 3,2 | A777 3,5×10⁻⁸; A891 3,9×10⁻⁸; A786 4,5×10⁻⁸ | **1** | 2/1001 = 0,0020 |
| tekdüze | 1,2×10⁻⁹ [0,6; 2,1] | A358 2/10⁹: 3,0×10⁻⁹ [2,4×10⁻¹⁰; 7,2×10⁻⁹]; A088 5/10⁹: 6,0×10⁻⁹ [1,6×10⁻⁹; 1,2×10⁻⁸] | 2,5; 5,0 | A593 1,1×10⁻⁸; A925 2,6×10⁻⁸; A491 4,1×10⁻⁸; A315 5,1×10⁻⁸; A398 5,7×10⁻⁸ | **2** | 3/1001 = 0,0030 |

Plasebo p'leri kural gereği (k + 1)/(N + 1) ile hesaplanır. k/N ile oranlar şöyle olur: M20 2,1; Mallows 2,7; tekdüze A358 1,7.

**Okuma.**
- Her üç sıfırda Kur'an'ın nokta kestirimi, 1.000–2.000 plasebonun hepsinden küçüktür; hiçbir plasebo kendi dilinde Kur'an'dan daha nadir değildir.
- Ama her sıfırda bir ya da iki plasebo Kur'an'a 2–3 kat yakındır. Bunlar bu derinlikte Kur'an'dan ayrılamaz.
- Kalibre nadirlik kabaca şudur:
  - M20: 2.000 rastgele kitapta 1–2 (1/1.000 – 1/500);
  - Mallows: 1/500 – 1/200;
  - tekdüze: 1/330 – 1/125.
- Tekdüzedeki ikinci plasebo A088, p = 3'ün sabit aileli merceğidir. Ailesi dizilişe bağlı değildir, ama yakalanan iç yapı dizilişe bağlıdır (kapı raporu düzeltmesi).

## Ön-kayıtlı tahminler

| Tahmin | Gerçekleşen | Hüküm |
|---|---|---|
| §5 M20: R 0–4, okuma "arada" 0,55 / "aşar" 0,45 | R 3, arada | tuttu, aralıkta |
| §5 M20: en küçük plasebo p 2×10⁻⁸ – 2×10⁻⁷ | 2,0×10⁻⁸ | aralıkta (alt uçta) |
| §5 M20: R > 0 ise R_derin 0–1 | 1 | aralıkta |
| §5 Mallows: R 0–3 | 4 | **aralık dışı** |
| §5 Mallows: en küçük plasebo 3×10⁻⁸ – 3×10⁻⁷ | 3,0×10⁻⁸ | aralıkta (alt uçta) |
| §5 tekdüze: R 5–25, "arada" | 7, arada | tuttu |
| §5 tekdüze, gerekçe: yetişenler "A088 türünden yozlaşmış mercekler" | 7'den yalnız A088 yozlaşmış | **gerekçe yanlış** |
| T1700Z: M20 P(R_derin = 0) ≈ 0,85 | 1 | **tutmadı** (T1900Z'de aralık çakışmasıyla düzeltildi: P(R_derin = 1) ≈ 0,6; tuttu) |
| T1700Z: Mallows R 1–3, R_derin büyük olasılıkla 0 | 4, 1 | **tutmadı** |
| T1700Z: tekdüze R_derin birkaç | 2 | tuttu |
| T1700Z: en yakın plasebo / Kur'an (M20) 3–10 kat | 2,3 | **aralık dışı** |

Desen tek yönlü. Ana hattın tahminleri Kur'an'ın plasebolardan ayrışmasını sürekli büyük gördü. Bu, ön-kaydın asıl gerekçesidir: sayılar tahmine göre değil, kurala göre okunur.

## Doğrulama (ön-kayıt §6)

1. Bağımsız yeniden sayım: `yeniden_say_k10.py`. Bütün bellek partileri okundu, eksik bölüt yok.
2. Her sıfırda en küçük p'li beş plasebonun ve Kur'an'ın dili `plasebo_dil_sina.py` ile kendi merceklerinde sınandı: 16/16 TAMAM. Betiğe FC1, FNd2 ve FCK eklendi.
3. İlk 200 / 100 hedef kapı çalışmasıyla bayt bayt aynı (koşu günlüğü). Sayımlar kapı çalışmasının düzeltilmiş sayımlarıyla aynı.
4. Mallows'ta tek iş parçacıklı eşdeğerlik sağlandı. Kartın yazdığı 4 Mallows bölütü (ve 6 M20/tekdüze bölütü), işlemci sayacının tek iş parçacıklı hesabıyla bayt bayt aynı (ek T2040Z).

## Betimsel: en yakın plasebolar

| Plasebo | Sıfır | Mercek | Dil | Not |
|---|---|---|---|---|
| A535 | M20 | (1, −1), 13 | {B1, FAd2, FKS, WUNd2, WUT2p} | "13 her yerde": aile ayet toplamı 676 = 4·13², birleşim n toplamı 1521 = 9·13² |
| A142 | Mallows | (1, −1), 5 | {Qh, FSC, WH2p, HR0e} | küçük asalda üst üste binen tesadüfler |
| A358 | tekdüze | (1, −1), 19 | {BMn, FSC, W1, WUAd2, WHA2p} | metnin andığı asal |
| A519 | M20, 5. sırada (1,6×10⁻⁷) | (3, −1), 17 | {K1, FNd2, WSum} | Kur'an'ın dört yükleminden ikisini taşır; hücre toplamları 18, 16, 17, 18 |

## Anlamı ve sınırı

Kur'an'ın bütünlük dili hâlâ her sıfırda en nadir dildir (nokta kestiriminde). Ama "rastgele kitapların hiçbiri yetişemiyor" iddiası on kat plaseboyla düştü. Yaklaşık iki bin rastgele kitaptan biri, kendi tarifiyle Kur'an'ın 2–3 kat yakınında bir bütünlük kuruyor.

Kalibre sayı bu yüzden kapı çalışmasındakinden (1/201, 1/101) kötü değildir; o sayılar plasebo sayısının tabanıydı. Bu çalışma ilk kez tabanın üstünde bir ölçü verir: M20'de birincil 1/500, ikincil 1/1.000.

R_derin'de kalan dört plasebo (A535, A142, A358, A088) 10⁹'da Kur'an'dan ayrılamıyor. Kur'an'ın değeri 10¹⁰'dandır. Eşit derinlikte (10¹⁰–10¹¹) çözülmeleri kart sayacıyla dakikalar sürer. Ayrı bir ön-kayıt gerektirir.

Depodaki metinler (README, KESIF not 27, ortak yazarın değerlendirmesi) bugün "Kur'an her kapıda bütün plaseboları aşar (1/201, 1/101)" diyor. Bu sonuçla güncellenmeleri gerekir. Araştırmacının onayıyla yapılır.
