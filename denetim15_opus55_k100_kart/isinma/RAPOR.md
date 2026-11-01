# M20 ısınma tanısı — sonuç (31 Ekim 2026)

**Ön-kayıt:** `ONKAYIT.md`, donduruldu 2026-10-31T15:47:59Z, SHA-256 `a93fdc09d1d056bfdb0e07234ecefc1428de3ba4696d66c1df5806b175491f53`. **Sapma:** bir ek (`ek/`), sonuçları etkilemez. **Hesap:** workstation, 12 iş parçacığı, 15:48–16:21Z.

## Karar: ısınma yanlılığı ihmal edilebilir

Ön-kaydın üç ölçütü de sağlandı. İncelenen göstergelerde ısınmayı yüz katına çıkarmanın belirgin bir etkisi görülmedi; M20 sayılarının yeniden sayılmasını gerektiren bir bulgu yok. Nadir kuyruk için (Kur'an'a ulaşan kontroller: 9 / 6 / 5 isabet) kalan yanlılığa kesin bir sınır elde edilmedi. *(1 Kasım 2026'da bir dış incelemenin uyarısıyla yumuşatıldı; ilk sürüm "30.000 takaslık ısınma yeterlidir" diyordu.)*

| Ölçüt (§3) | eşik | ISINMA 300.000 / 30.000 | ISINMA 3.000.000 / 30.000 |
|---|---|---|---|
| `b2` oranı (bölüt 1–4.000; ~1,23 milyon kitap) | %95 aralık ⊂ [0,97; 1,03] | 0,9995 [0,9970; 1,0021] | 0,9997 [0,9971; 1,0022] |
| Am000 kB oranı (A−, bütünlüklü kontroller; ~26.000) | ⊂ [0,90; 1,10] | 1,011 [0,993; 1,028] | 0,982 [0,965; 0,999] |
| (A) ilk − son dilim, ISINMA 30.000 (bölüt 1–2.000) | ≤ 3 SH | S: −0,21 ± 0,11 (z = −1,9); B2: z = −0,8 | — |

Kur'an'a ulaşan kontroller (betimsel; bu karar için fazla seyrek) — M20, bölüt 1–4.000, 10⁹ kontrol:

| Isınma | A000 k | Am000 k |
|---|---|---|
| 30.000 | 9 | 129 |
| 300.000 | 6 | 116 |
| 3.000.000 | 5 | 130 |

Gc 10¹⁰ sayımının ortalaması 10⁹ başına 4,7'dir. Üç değer de onunla tutarlıdır.

## Ayrıntı

**(A) Bölüt içi kayma** (`sonuc/A_<ısınma>.txt`). Zincirler örneklemenin başından itibaren dengededir.
- Ortalama S, on dilimin hepsinde 208.477'dir (S_obs = 209.029).
- 30.000 ısınmada ilk ve son dilim arasındaki fark S için −0,21 ± 0,11, B2 oranı için −5×10⁻⁶ ± 7×10⁻⁶'dır.
- 3.000.000 ısınmada aynı farklar −0,15 ± 0,11 ve −2×10⁻⁶ ± 7×10⁻⁶'dır.
- Yüz kat uzun ısınmayla B2 oranlarının oranı 0,9993 [0,9957; 1,0029].

**(B) Isınma uzunluğu** (`sonuc/B_<ısınma>/`; 30.000 = kapı çalışmasının Kur'an tarafı Gc bölüt 1–4.000).
- `b2` üç ısınmada da %0,05'ten az farklıdır.
- Am000 kB 300.000'de %1,1 yukarıda, 3.000.000'de %1,8 aşağıdadır. Sonuncusunun aralığı 1'i kıl payı dışlar. Ama iki karşılaştırma ters yönlerdedir; bu, sistematik bir ısınma etkisinden çok örnekleme gürültüsüne uyar. İkisi de ön-kayıtlı eşiğin çok içindedir.

**Eşdeğerlik.** Isınma parametreli kod, ISINMA = 30.000 ile M20 bölüt 1–24'te kapı çalışmasının çıktısıyla süre alanı dışında birebir aynıdır.

## Tahmin

Ana hat şunu öngörmüştü: yanlılık ihmal edilebilir; `b2` farkı %1'den, Am000 kB farkı %5'ten az; bölüt içinde kayma yok. **Tuttu.**

## Anlamı ve sınırı

M20 sayılarının ısınmadan kaynaklanan bir yanlılığı, bu ölçülerin duyarlılığında yoktur: `b2` için ~%0,3, bütünlüklü kontroller için ~%2. Kapı çalışmasının M20 sonuçları (plasebo kalibrasyonu, Kur'an'ın Gc 10¹⁰ sırası) ve plasebo hedefleri bu bakımdan sağlamdır. Hedefler 30.000 + 2.000 takas sonra çekilir.

Bu tanı, zincirin hedef dağılıma (S ≤ S_obs olan dizilişler üzerinde tekdüze) ulaştığını doğrudan kanıtlamaz. Yalnız ısınma uzunluğunu yüz katına çıkarmanın sonuçları değiştirmediğini gösterir.
