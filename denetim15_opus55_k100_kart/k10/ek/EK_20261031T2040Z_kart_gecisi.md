# Ek — derinleştirmenin kalanı ekran kartında (31 Ekim 2026, 20:40Z; sonuçlar bu ekten önce görülmedi, yalnız tamlık sınandı)

**Sapma:** Ön-kayıt §3'ün ikincil derinleştirmesinin (bölüt 401–4.000) bir kısmını işlemci sayacı değil, bayt bayt eşdeğerliği önceden kabul edilmiş kart sayacı hesapladı:
- M20: 981 bölüt (kalan 2.619'u işlemci yazdı);
- Mallows: 3.600 bölütün tamamı;
- tekdüze: 3.600 bölütün tamamı.

Diller, tohumlar, dizinler ve sayım kuralı değişmedi. Hangi bölütü kartın yazdığı `tab/<S>/kA/d4_0/kart_yazdigi_bolutler.txt` dosyasında (değişiklik zamanı 20:25Z'den sonra), kartın kimliği `kart_gunlugu.txt` dosyasındadır.

**Onay:** Araştırmacı ~20:00Z: "onay veriyorum yazalım; kod biterse ve taşıma uygunsa taşıyalım". ~20:12Z: "neden kod tamamsa başlatmıyoruz?"

## Akış (UTC)

| Saat | Olay |
|---|---|
| 20:15 | İşlemci derinleştirmesi, kart sınamasına bir iş parçacığı açmak için 11 iş parçacığıyla yeniden başlatıldı. Var olan bölütler atlandı; sonuç iş parçacığı sayısına bağlı değildir. |
| 20:17–20:22 | **Kabul sınaması v1** (`~/hrf_olcum/kart/ek/EK_20261031T2010Z_kart_tasarim.md` §3; sınamadan önce yazıldı). RTX 4090 ve RTX 3090'da 7/7 karşılaştırma, fark 0. |
| 20:24 | İşlemci derinleştirmesi durduruldu (M20 2.619 bölüt). `kart_ana.c`'ye bir değişiklik yapıldı: dizinde `esikler.txt` varsa bayt bayt aynı olmasını şart koşar, üzerine yazmaz. Sayım kodu değişmedi. |
| 20:25 | Yeniden derlendi. **Kabul sınaması v2:** iki kartta 7/7, fark 0. |
| 20:27–20:37 | `kos/kart_derin.sh`. Bölüt b mod 5 ∈ {0, 1, 2} RTX 4090'a, {3, 4} RTX 3090'a. Süreler: M20'nin kalan 981 bölütü 59 sn, Mallows 3.600 bölüt 4 dk 43 sn, tekdüze 3.600 bölüt 3 dk 30 sn. Eksik bölüt yok, yarım dosya yok, hata çıktısı boş. |

Aynı iş, işlemcinin 12 iş parçacığında ~7 saat sürerdi. İki kart birlikte ~45–60 kat hızlı çıktı.

## Üretim dosyalarının çapraz sınaması (ön-kayıt §6.4'ü de karşılar)

Kartın yazdığı 10 bölüt, işlemci sayacıyla (`k10/src/kontrol_b`, tek iş parçacığı, ayrı dizinde) yeniden hesaplandı. Hepsi `sure_s` dışında bayt bayt aynı çıktı (`.txt` ve `.q`):

| Sıfır | Kart | Bölütler |
|---|---|---|
| M20 | RTX 4090 | 3987 |
| M20 | RTX 3090 | 3988 |
| Mallows | RTX 4090 | 401, 2200, 4000 |
| Mallows | RTX 3090 | 2203 |
| tekdüze | RTX 4090 | 401, 2200, 4000 |
| tekdüze | RTX 3090 | 3999 |

Mallows bölütlerinin tek iş parçacıklı sayımla aynı çıkması, ön-kaydın 4. doğrulama maddesidir. Kart sayacında, yarışa yol açan küresel durum hiç yoktur.
