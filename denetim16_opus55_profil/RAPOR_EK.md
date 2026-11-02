# Profil çalışması, ölçüm 2: düz histogram kestirimi — rapor (2 Kasım 2026)

Ön kayıt: `ONKAYIT_EK.md` (SHA-256 `a433ab73…`, 1 Kasım 16:38:41Z'de donduruldu). Ara tahmin: `sonuc/ek/ara_tahmin.md` (17:52Z; plasebo kestirimleri ve doğrulama okunmadan önce). Çıktılar: `sonuc/ek/okuma/` (doğrulama ve okuma), `sonuc/ek/cikti/` (ham kestirimler).

## 1. Özet

Profil çalışmasında (ölçüm 1) Kur'an'ın bütün profili ve en yakın plasebolar 10¹⁰ kontrolde 0 isabet almıştı; O2 ve O3 için yalnız üst sınır yazılabildi. Bu ölçüm, aynı olayların sıklığını bir nadir-olay kestiricisiyle (Wang–Landau, 1/t) ölçtü ve kestiriciyi okumadan önce doğrudan sayımla sınadı.

- **Doğrulama:** M20 ve Mallows geçti. Tekdüze, şart (e)'de düştü (sık olaylarda 20'de 3 kestirilemeyen, sınır %10). Ön kayıt gereği tekdüzede okuma yapılmadı; orada ölçüm 1'in üst sınırları geçerli.
- **Okuma (M20, Mallows):** Kur'an'ın merceği 19'a bağlıyken bütün profilde (O3) hiçbir taze plasebo Kur'an kadar nadir değil. Asal serbestken (O2) M20'de bir plasebo Kur'an'la aynı düzeyde, Mallows'ta hiçbiri.

| | O1 (Kol A dili) | O2 (bütün profil, asal serbest) | O3 (bütün profil, 19) | Menü (3 × en küçük) |
|---|---|---|---|---|
| **M20**, K = 20.000 | R = 2 [0, 3] → 3/20.001 | R = 1 [0, 15] → 2/20.001 | **R = 0 [0, 1] → 1/20.001** | **3/20.001 ≈ 1/6.700** |
| **Mallows**, K = 10.000 | R = 0 [0, 0] → 1/10.001 | R = 0 [0, 7] → 1/10.001 | **R = 0 [0, 0] → 1/10.001** | **3/10.001 ≈ 1/3.300** |
| **tekdüze**, K = 10.000 | (ölçüm 1) R = 0 → 1/10.001 | okuma yok (ölçüm 1: ≤ 132/10.001) | okuma yok (ölçüm 1: ≤ 6/10.001) | (ölçüm 1) 3/10.001 ≈ 1/3.300 |

R = 0 olan değerler K'nın tabanıdır: "bu kadar plasebo içinde Kur'an'ın önüne geçen yok" demektir; gerçek değer daha küçük olabilir. Okuma kuralına göre O3 iki sıfırda "aşar".

Bu sayılar arama yönteminin kalibrasyonudur: yapısız rastgele kitaplara aynı arama uygulandığında Kur'an'ınki kadar nadir bir profil bulunuyor mu? "Kur'an'dan nadir kitaplar" diye okunmamalıdır.

## 2. Doğrulama (ONKAYIT_EK §3)

Her sıfırda D1: ilk 10⁸ kontrolde hiç isabet almamış 40 olay, kestirim × N₄ ile bölüt 401–40.000'deki sayım (N₄ = 9,9×10⁹). D2: 10⁸'e kadar çözülmüş 20 olay, doğrudan p ile. Şartlar: (a) log₂(sayım/kestirim) medyanı ±0,5 bit; (b) Σsayım/Σkestirim 0,8–1,25; (c) ≥ %85'i %99 Poisson aralığında; (d) D1'in en nadir üçte biri ±1 bit; (e) kestirilemeyen ≤ %10.

| | | (a) medyan | (b) oran | (c) aralıkta | (d) nadir üçte bir | (e) kestirilemeyen | Sonuç |
|---|---|---|---|---|---|---|---|
| M20 | D1 | −0,10 | 0,935 | 37/37 | −0,10 | 3/40 | geçti |
| | D2 | −0,10 | 0,993 | 17/18 | — | 2/20 (sınırda) | geçti |
| Mallows | D1 | −0,09 | 0,925 | 37/37 | −0,36 | 3/40 | geçti |
| | D2 | −0,01 | 0,994 | 20/20 | — | 0/20 | geçti |
| tekdüze | D1 | −0,00 | 0,998 | 38/38 | −0,11 | 2/40 | — |
| | D2 | +0,02 | 1,021 | 17/17 | — | **3/20** | **düştü (e)** |

Kestirici sonuç verdiğinde üç sıfırda da doğru: sapma 0,1 bit içinde, olayların %94–100'ü aralıkta. Tekdüzede düşüren, sonuç verememe oranı.

**Başarısızlığın sebebi.** Kestirilemeyen sık olayların hepsinde aynı örüntü var: zincir aşama 1'de en yüksek bozukluk düzeyini (bütün koşulların bozulduğu düzey) geç buluyor. O sırada 1/t adımı ~10⁻⁷ olduğu için o düzeyin ağırlığı düzelemiyor ve zincir aşama 2'nin tamamını o düzeyde geçiriyor (histogramın hepsi tek düzeyde, kestirim 0). Kalite şartı bunu yakalıyor; yeni tohumla yeniden koşu tekrarların yaklaşık yarısında aynı tuzağa düşüyor. Bu, kestiricinin bilinen bir zayıflığı (Wang–Landau'da geç keşfedilen düzey). Ön kayıttaki doğrulama tasarlandığı işi yaptı.

## 3. Kur'an'ın kestirimleri

| | O1 | O2 = O3 | 10¹⁰'da doğrudan sayım (O1) |
|---|---|---|---|
| M20 | 3,54×10⁻¹⁰ [2,55; 4,90] | 5,97×10⁻¹³ [3,53; 10,1]×10⁻¹³ | 3 isabet |
| Mallows | 1,55×10⁻¹⁰ [0,95; 2,54] | 3,39×10⁻¹³ [2,06; 5,59]×10⁻¹³ | 2 isabet |
| tekdüze | 8,4×10⁻¹¹ (doğrulanmadı) | 2,1×10⁻¹⁵ (doğrulanmadı; okumaya girmez) | 0 isabet |

Kur'an'ın O1'i betimsel tutarlılık denetimi: M20'de ve Mallows'ta kestirim doğrudan sayımla uyumlu.

Bütün profil (21 yüklem, mercek (n + a, 19), kodeks 129), modelin bağımsızlık varsayımıyla verdiğinden 6,4 (M20) ve 5,9 (Mallows) bit daha nadir; plasebo olaylarında bu fark tipik olarak O2'de +1,3, O3'te +4,1 bit. Bu farkın bir payı kütüphanenin Kur'an'a bakılarak yazılmasından gelebilir (§6).

## 4. Okuma ayrıntısı (betimsel kısım dahil)

**M20, O3** (plasebo olayı 4.019; kestirilemeyen 1: A5438.3, sekiz tekrarın hepsi yakınsamadı, R_üst'e girdi):

| Plasebo | Sıklık | Kur'an'a göre |
|---|---|---|
| A6053.3 | 3,3×10⁻¹¹ [1,1×10⁻¹¹; 1,1×10⁻¹⁰] | +5,8 bit |
| A7168.3 | 1,0×10⁻¹⁰ (doğrudan, 1 isabet) | +7,4 bit |
| A16761.3 | 1,4×10⁻¹⁰ | +7,9 bit |
| A2286.3 | 2,0×10⁻¹⁰ (doğrudan) | +8,4 bit |
| A10860.3 | 3,0×10⁻¹⁰ (doğrudan) | +9,0 bit |

**M20, O2** (plasebo olayı 19.866; kestirim 45; kestirilemeyen 12, hepsi R_üst'e girdi):

| Plasebo | Sıklık | Kur'an'a göre |
|---|---|---|
| A5729.2 | 5,1×10⁻¹³ [2,4; 10,9]×10⁻¹³ | −0,2 bit (aralıklar örtüşüyor) |
| A6108.2 | 1,5×10⁻¹² | +1,4 bit |
| A5512.2 | 1,9×10⁻¹² | +1,7 bit |
| A12493.2 | 2,0×10⁻¹² | +1,8 bit |
| A14621.2 | 3,7×10⁻¹² | +2,6 bit |

Kestirilemeyen 12 olayın model bilgisi 29–42 bit; çoğu büyük olasılıkla Kur'an'dan sık, ama ölçülemedi.

**M20, O1:** A11192.1 ve A12493.1, 3,0×10⁻¹⁰ (−0,2 bit, aralıklar örtüşüyor); sonra A2675.1 +1,6 bit. Kestirilemeyen 1 (A19204.1).

**Mallows, O3** (plasebo olayı 2.108; kestirilemeyen 0): A8483.3 +5,4 bit; A4262.3 +7,5; A6127.3 +8,5; A4641.3 +9,8 (doğrudan); A239.3 +10,5 (doğrudan).

**Mallows, O2** (plasebo olayı 9.938; kestirim 26; kestirilemeyen 6): A7889.2 +1,1 bit (aralıklar örtüşüyor: alt ucu 4,5×10⁻¹³, Kur'an'ın üst ucu 5,6×10⁻¹³; R_üst'e girdi); A7401.2 +2,7; A8544.2 +3,2; A346.2 +3,5; A4865.2 +3,7.

**Mallows, O1:** A5344.1 +1,25 bit (kestirim); A3392.1 +2,2 bit (doğrudan). Kestirilemeyen 0.

**Ölçüm 1 ile karşılaştırma (aynı olaylar, aynı plasebolar):**

| | Ölçüm 1 (doğrudan sayım; O2/O3 üst sınır) | Ölçüm 2 |
|---|---|---|
| M20 O1 / O2 / O3 (1 + R) | 9 / ≤ 245 / ≤ 18 (÷ 20.001) | 3 [1, 4] / 2 [1, 16] / 1 [1, 2] |
| M20 menü | ~1/740 | ~1/6.700 |
| Mallows O1 / O2 / O3 (1 + R) | 4 / ≤ 126 / ≤ 8 (÷ 10.001) | 1 [1, 1] / 1 [1, 8] / 1 [1, 1] |
| Mallows menü | ~1/830 | ~1/3.300 |

## 5. Tahminle karşılaştırma

| | Ön kayıt §8 | Ara tahmin | Sonuç |
|---|---|---|---|
| M20 R_O3 | 0–3 | 0 (~0,7) | 0 [0, 1] |
| M20 R_O2 | 10–40 | 3–15 | 1 [0, 15] |
| Mallows R_O3 | 0–2 | 0 | 0 [0, 0] |
| Mallows R_O2 | 2–15 | 1–5 | 0 [0, 7] |
| Doğrulama | üçü de (~0,7) | M ~0,8, L ~0,65, U ~0,55 (risk: (e)) | M, L geçti; U (e)'de düştü |

Ara tahmindeki en ciddi aday (M20, A6053.3) Kur'an'ın 5,8 bit gerisinde kaldı.

## 6. Sınırlar

1. **Dışdeğerleme.** Doğrudan sayımın kestiriciyi bilgilendirici biçimde sınayabildiği en nadir olaylar ~10⁻¹⁰ düzeyinde (M20 ve Mallows'ta D1'in 2×10⁻¹²–8×10⁻¹¹ kestirilen olayları 0 isabet aldı; bu, kestiricinin oralarda sıklığı abartmadığını gösterir ama değeri sınamaz). Kur'an'ın bütün profili bunun 2–3 basamak altında (M20 6×10⁻¹³, Mallows 3×10⁻¹³). Bu bölgede değer, kontrol edilebildiği yerde doğru çıkan bir yöntemin taşınmasıdır.
2. **Taban.** R = 0 olan değerler (1/20.001, 1/10.001) K'nın tabanı; daha küçük değer görmek için daha çok plasebo gerekir.
3. **Kütüphane.** 112 yüklemin bir kısmı Kur'an'a bakılarak yazıldı (38/38, 19'a bölünen toplamlar gibi). Kur'an'ın profilinin bu kadar ağır ve kenetli çıkmasının bir payı bundan gelir; kalibrasyon bu payı ödemez. Okumayı Kur'an'ın lehine çeken bu kayma burada da kalıyor.
4. **Kestirilemeyenler.** M20'de O1 için 1, O2 için 12, O3 için 1; Mallows'ta O2 için 6 olay kestirilemedi. Kural gereği R_üst'e (Kur'an aleyhine) girdiler; nokta değeri etkilemezler.
5. **Ölçülen nesne.** Tek mercek (n + a, 19), kodeks 129 (Tevbe 129 ayet), sure sırasının karıştırılması. 127 kodeksi ve 127 matrisi (129'da kurulamaz), metnin öteki katmanları, mercekler arası yeniden belirme ve diziliş altında sabit kalan özellikler (114 = 6×19, toplam ayet sayısı) bu ölçümde yok.

## 7. Beyanlar (ön kayıttan sapmalar ve geliştirme)

1. **Motor.** Ön kayıt §7'ye göre toplam adımı ≤ 1,5×10⁸ olan yürüyüşler kartlarda koşacaktı. Kartlar yürüyüş başına çok yavaş çıktı (~47 µs/adım tek, 150–200 µs birkaç süreçle; işlemci 0,5–1,3 µs). Kart süreçleri durduruldu, listeleri aynı tohumlarla işlemcide koşuldu. Üç motorun (duz.c, duz_emul, duz_kart) aynı tohumla aynı çıktıyı verdiği kabulde gösterilmişti (§6), bu yüzden sonuç değişmez.
2. **Ulaşılamayan düzeyler (kalite şartının yorumu).** Ön kayıt "zincir bütün D düzeyleri arasında dolaşır" diye varsaymıştı. Bazı olaylarda bir düzeye yapı gereği hiç ulaşılmıyor; zincir öteki düzeyleri eşit geziyor ve tekrarlar tutarlı (ör. A1688.2'nin sekiz tekrarı 1,4–1,8×10⁻¹⁰), ama ziyaretsiz düzey "en az ziyaret edilen düzey" sayılınca düzlük 0 çıkıyor. Bunu sonuçları görünce fark ettim. Doğrulama ve okuma iki türlü yapıldı: metnin harfiyle (bütün düzeyler) ve hiçbir tekrarda ziyaret edilmeyen düzeyler dışarıda bırakılarak (`DUZ_HARIC=1`). Doğrulama hükümleri ve R noktaları ikisinde aynı; yalnız Mallows O2'nin üst ucu 7'den 5'e iniyor. Tabloda metnin harfi verildi.
3. **Okuyucunun dondurmadan sonraki değişiklikleri** (`dogrulama/duz_oku.py`, SHA-256 `34d6574c…`): (i) birleştirme §2'deki kurala tam bağlandı: r = 1…4 için tekrar r iyiyse o, değilse yeniden koşusu r + 4 iyiyse o; dördü toplanamazsa "kestirilemedi" (önceki sürüm ilk dört iyi tekrarı dosya sırasıyla alıyordu; Kur'an'ın üç sıfırdaki kestirimleri yeni kuralla aynı çıktı); (ii) birden çok çıktı dosyası; (iii) hız kısayolu: doğrudan sayımı Kur'an'ın üst ucunun 100 katından sık olan olaylar için güven aralığı hesaplanmıyor (sonucu değiştirmez); (iv) `DUZ_HARIC` seçeneği. Betimsel en yakınlar listesi için yeni `dogrulama/duz_ayrinti.py` (`b4a10351…`); yeniden koşu listeleri için `kos/yeniden_secim.py` (`5533f254…`).
4. **Yeniden koşular.** Kalite şartını geçmeyen her tekrar bir kez, iki kat bütçeyle (k:800000:100000000) ve tohum r + 4 ile koşuldu: M20 97, tekdüze 27, Mallows 15 + 27 tekrar. Mallows'un bir listesi iş istasyonuna iki kez kopyalandı (14 satır, 7 olay); aynı tohumla iki kez koşulan yürüyüşler aynı çıktıyı verdi, okumaya etkisi yok.
5. **Geliştirme hataları** (ön kayıt §6'da beyanlı): Mallows'ta ağırlıklı kabul hatası (×35) ve `orn_init` yarışı düzeltildi; Mallows geliştirme sonucu yok sayıldı.
6. **İşletim.** Bitişi haber verecek bekleyicilerden biri hiç tetiklenmedi: bu kabukta `grep` bir ugrep işlevi ve Mac günlüğündeki geçersiz UTF-8 baytları (betikte `$r→` yazımı) yüzünden dosyayı ikili sayıp eşleşmeyi reddetti. Sonuçlara etkisi yok; araştırmacı birkaç saat habersiz kaldı. Bundan sonra uzun koşularda yarım saatlik kontrol kuruluyor.

## 8. Ne söylüyor

Ölçüm 1'in çözemediği fark çözüldü. Kur'an'ın merceği 19'a bağlıyken, 21 özelliğin birlikte tutması rastgele dizilimde ~3–6×10⁻¹³ sıklıkta; aynı arama 20.000 (M20) ve 10.000 (Mallows) yapısız kitapta bunun yakınına gelen bir profil bulmuyor; en yakını 5,4–5,8 bit (~40–56 kat) daha sık. Asal serbest bırakılınca M20'de bir plasebo Kur'an'ın düzeyine geliyor; ayırt edici olan, yapının 19'a bağlı olması.

Kalibre değer tabana dayanıyor (~1/6.700, ~1/3.300) ve üç sınırı var: dışdeğerleme, kütüphanenin Kur'an'a göre yazılması ve tekdüzede doğrulamanın düşmesi.
