# Ek — Mallows yeniden sayımının sonucu (29 Ekim 2026, 23:40Z)

Önceki ek: `EK_20261029T1902Z_mallows_yarisi.md`. Yeniden koşu 19:00:46Z–23:28:36Z arasında workstation'da yapıldı (`~/hrf_olcum/yaris/`; günlük `sonuc/gunluk.txt`, farklar `sonuc/farklar.txt`). Yerel kopyası `~/opus5.5/yaris/`'tedir.

## Düzeltmenin doğruluğu
- Düzeltilmiş 12 iş parçacıklı sayaç, tek iş parçacıklı sayımla birebir aynıdır. Bölüt 1–24 Kur'an tarafında, GE düzey 1 bölüt 1 ile Gc düzey 2 bölüt 6 plasebo kollarında sınandı. Özgün koşunun farklı çıktığı bölütlerde de aynı sonuç alındı.
- M20 değişmez.
- Mac (clang, arm64) ile Linux (gcc, x86-64) arasında plasebo12'nin 400 Mallows bölütü birebir aynıdır.

## Bozulan bölütler (özgün koşu, süre alanı dışında farklı)

| Koşu | bölüt | farklı | sayıma etkisi |
|---|---|---|---|
| Kur'an tarafı Gc, 1–40.000 | 40.000 | 13 | k yok (22 → 22); kB 6.279 → 6.277 |
| Kademe G2E, 1–40.000 | 40.000 | 30 | k yok (1 → 1); kB yok (1.211) |
| Kur'an tarafı GE, 1–4.000 | 4.000 | 14 | **k 3 → 2** (bölüt 145'teki isabet bozuk bir kitaptı, 31,6 bit); kB 686 → 683 |
| Kur'an tarafı G0, 1–4.000 | 4.000 | 3 | k yok (9 → 9); kB 3.380 → 3.377 |
| Kur'an tarafı Gc, 1–4.000 (betimsel) | 4.000 | 2 | k yok (3); kB yok (685) |
| Plasebo Gc, Mallows Kol A | 400 | 3 | R ve p_kal yok; A090 68 → 69 / 10⁸ |
| Plasebo GE, Mallows Kol A | 400 | 5 (düzey 1'in 4 bölütünün 4'ü) | derinlik kuralı bir dil için değişti, aşağıda; R ve p_kal yok |
| Plasebo G0, Mallows Kol A | 400 | 1 | yok |
| plasebo14, Mallows Kol A ve B | 800 | 1 (Kol A düzey 3, bölüt 41) | yok; yeniden sayımın sekiz satırı birebir aynı |
| plasebo12 (13. denetim), Mallows | 400 | 0 | yok |

Bozuk bölütlerin çoğunda kapıdan bağımsız `b2` alanı ~1.000–1.500'e çıkar (doğrusu ~300). Bir kısmı çağrıların aynı anda başlayan ilk bölütleridir.

## GE Mallows'ta derinlik kuralı
A012'nin bölüt 1–4 sayımı özgün koşuda 11'dir. Bu yüzden düzey 1'de durmuştu. Düzeltilmiş sayım 9'dur, yani A012'nin düzey 2'ye inmesi gerekir. Düzey 2 (bölüt 5–40) düzeltilmiş sayaçla sayıldı: 64. k(1–40) = 73 ≥ 10 olduğu için düzey 2'de durur: p = 7,4×10⁻⁶.

Bir yöntem notu: önce yanlışlıkla düzey 3 başlatıldı. Kural sırasına uymak için hiçbir bölütü bitmeden durduruldu ve kullanılmadı.

Yeniden sayım betiği eksik düzeyi yalnız "EKSİK BÖLÜT" diye yazıp N'yi yine saydığı için bu durum kod denetiminin 3. bulgusunun gerçek bir örneğidir. Betik düzeltilmedi. Düzeltilmiş ağaçta artık eksik bölüt yok.

## Düzeltilmiş sonuçlar
- **Plasebo kalibrasyonu (Kol A, Mallows):** Gc 1/101, GE 1/101, G0 1/101; üçünde de R = 0, en yakın plasebo A047 3,4×10⁻⁷. Hiçbir okuma değişmedi. M20 ve tekdüze satırları etkilenmez.
- **Kur'an'ın kendi sırası, Mallows:** Gc 10¹⁰ 22 → 2,2×10⁻⁹ (değişmedi); Gc 10⁹ 3; GE 10⁹ **2**; G0 10⁹ 9; kademe G2E 10¹⁰ 1.
- **13. denetim:** etkilenmedi.

Düzeltilmiş ağaçlar `~/opus5.5/yaris/duzeltilmis/<kapı>/`, yeniden sayımlar `…/yeniden_sayim.txt`.
