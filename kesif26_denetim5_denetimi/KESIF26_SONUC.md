# kesif26 — Beşinci denetimin denetimi ve kısıtlı-mercek ölçümleri

**Tarih:** 28–29 Eylül 2026 · **Girdi:** beşinci dış denetimin robotu ve tabloları (`denetim5_opus55/`) · **Sonuç:** dürüstlük notu 17 (`KESIF.md`).

## Ne yapıldı

Denetçinin puanlayıcısı (`robot.c`, `robot4.c`) kopyalandı; olgular, düğümler, puanlar ve aynı 2×10⁶ rastgele kitap (tohum 919) korundu. Yalnız iki şey eklendi:

- **Mercek maskesi** (`robot_kisit.c`, `robot4_kisit.c`): `ROBOT_MSET` izinli modülleri, `ROBOT_FSET` izinli biçimleri kısıtlar. Kur'an'ın puanı değişmez; rastgele kitaplar yalnız izinli merceklerde kendi en iyi hikâyesini arar.
- **Tek-aile kuralı** (`robot_tutarli.c`, `robot4_tutarli.c`, `ROBOT_COHERENT=1`): bir hikâye ya toplam (F) ya fark (G) ailesi hakkında olmalı; kitap-bütünü ve birleşim olguları iki hikâyeye de açık. Kur'an'ın hikâyesi zaten tek aileli olduğundan puanı değişmez. Kural sonradan eklendiği için ×2 bedelle raporlanır.
- `demobest` modu: Kur'an'ı geçen kitapların en iyi merceğini, olgularını ve dizilişini döker (`best_J10_tutarli.txt`, `best_U_karisik.txt`); `hikaye_analiz.py` bu dökümleri önceden yazılmış beş özellikle (modül 19, tek aile, kitap-toplamı öz-dengesi, tam eşitlikler, kısa sabitler) sınıflar.

Tablolar (`../denetim5_opus55/tab/*.bin`, ikili, ~260 MB) depoda değildir; `denetim5_opus55/` betikleriyle aynı tohumlarla yeniden üretilir. Koşu betikleri `kos*.sh`, logları `kos*.log`, Kur'an puanları `*_kuran.txt`.

## Denetim raporunda bulunan dört hata

1. **§7b'nin kronolojisi ters.** `dokuman.md`'de t-paritesi (P) ve t ≡ 0 (mod 19) (R) bölümlemeleri belgenin başında tanımlı başlangıç kurallarıdır; kalıntı taraması sonradan gelir ve r = 0'ı "zaten beklediğimiz ana sınıf" diye anar. 127-bağımlılık yalnız 360 matris özelliğindedir; o kat zaten manşet dışıdır. (Altıncı oturum aynı sonuca vardı; gerçek köken daha eskidir, bkz. not 19.)
2. **×7 okul çarpanı kullanılmış bir serbestlik değildir.** Not 8'deki varsayımsal Bonferroni üst sınırıdır. Kûfe sayımı veriye bakılarak seçilmedi; basılı mushafların fiilî standardıdır. "26'da 1" bu çarpanla kurulmuş en-kötü-durum satırıdır.
3. **"Yaklaşıklık Kur'an lehine" yanlış.** Robot Kur'an'ı tam olay frekansına göre 0,5–0,9 mertebe eksik puanlar (−log₁₀; tekdüze 14,06'ya karşı 14,97, J10 12,03'e karşı 12,54, M 12,08'e karşı 12,82).
4. **Karşı-örnek miras taşır.** σ = 10 titreşim kitabı gerçek dizilişin 11/114 konumunu ve 12 aile üyesinden ikisini (6, 50) birebir miras alır.

**Aynen geçerli olanlar:** sıralı sıfırda kristal şişmesi (`kesif25` tutarlı) ve nominal ≠ kalibre (sabit mercek J10 6,0×10⁻⁶, yeni tohumla 6,1×10⁻⁶'ya karşı).

## Ölçümler (T2 = en iyi 8 düğüm; 2×10⁶ kitap; p = (k+1)/(N+1))

| Basamak | Sıralı J10 | Tekdüze U |
|---|---|---|
| İddia bataryası, sabit (1,1,19) | 6,0×10⁻⁶ | 2,3×10⁻⁷ |
| İddia, modül 19 sabit, biçim serbest | 2,4×10⁻⁵ | 1,0×10⁻⁶ |
| İddia, biçim n+a sabit, modül serbest | 7,8×10⁻⁵ | 1,5×10⁻⁶ |
| İddia, 266 mercek serbest | 2,7×10⁻⁴ | 1,0×10⁻⁵ |
| Tam defter (99 olgu, matris kapalı), sabit | 3,5×10⁻⁵ | — |
| **Tam defter, modül 19 + 17 biçim** | **3,15×10⁻⁴ (~1/3.200)** | **5,5×10⁻⁶ (~1/180.000)** |
| Tam defter, 646 mercek serbest | 5,5×10⁻³ | 1,44×10⁻⁴ |
| Aynısı ×7 (varsayımsal) | 3,9×10⁻² | 1,0×10⁻³ |

Tek-aile kuralı (×2 bedeli parantezde): iddia bataryası serbest 1,8×10⁻⁵ / 0÷2M, modül 19 1,0×10⁻⁶ / —; tam defter modül 19 6,25×10⁻⁵ (1,25×10⁻⁴, ~1/8.000) / 0÷2M (<1×10⁻⁶); tam defter serbest 1,21×10⁻³ (2,4×10⁻³, ~1/410) / 6,8×10⁻⁵ (1,4×10⁻⁴); sabit 7×10⁻⁶ / —. Kur'an'ı geçen karışık-hikâye kitapları düşer (ör. #1101626: 13,56 → 8,85). Kural mercek çapalıyken güçlü (×5–25), mercekler serbestken zayıf (×2–4,5).

127 kodeksi (matris düğümü açık; `kos_127.sh`, `mx19_*`): tam defter, modül 19 — sıralı karışık 2,6×10⁻⁵, tek aile 5,5×10⁻⁶; sabit mercek 2,5×10⁻⁶ / 1,5×10⁻⁶; tekdüze karışık 1×10⁻⁶, tek aile 0÷2M. Matris kapalıya göre ×11–12; bu kazancın Halife koşulunun yeniden sayımı olduğu `kesif27`'de gösterilir.

## Yan ölçümler

- **Deste karşılaştırması:** Kûfe ve Medine desteleri rastgele kitaplarda aynı en-iyi-hikâye dağılımını verir (tekdüze %50/%99/%99,99 nicelikleri 6,58/9,81/12,73'e karşı 6,59/9,81/12,77; sıralı 6,71/9,96/12,84'e karşı 6,73/9,98/12,88). Gerçek Medine kitabı en iyi merceğinde (3,1,4) T2 = 6,75 ile 56. yüzdeliktedir. Nesne desteden değil, Kûfe sayımı × mushaf sırası eşleşmesinden gelir.
- **Yön** (`yon_testi.py`): kısadan uzuna sıralı kitaplar katmanları uzundan kısaya kadar üretmez (terazi 2,5×10⁻⁴'e karşı 4,6×10⁻⁴; aile 2,4×10⁻⁴'e karşı 5,7×10⁻⁴; iç terazi 1,5×10⁻⁵'e karşı 6,0×10⁻⁵). Mushaf sırası Kendall τ = −0,68, nüzul sırası +0,14.
- **Kusurlu sıra** (`kusurlu_sira.py`): gerçek kitabın sıralılığı S = 209.011, sıralı sıfırın medyanı 208.622 (%98'i gerçek kitabın %1 içinde); komşu bozulma 45/113'e karşı 54; 95–171 bandı %72'ye karşı %65.
- **Rakip hikâyelerin anatomisi:** en güçlü 24 rakipte (12 tek-aile J10, 12 karışık U) tanık yazısının dört "güzellik" özelliği — kesintisiz bant 11/24, dengeli hücreler 1/24, iki el 3/24, kitabın kendi toplamlarına öz-denge 2/24, dördü birden 0/24. Tek-aile J10 rakiplerinin 1/12'si modül 19'da; kısa sabitler ayırt edici değil (ortalama 1,8'e karşı Kur'an 3). Bant, uzunluk sıralı bir kitabın parmak izidir; öz-denge sıralılıkla açıklanmaz.
- **Örnekler:** tipik rastgele kitap (`tipik_kitap.txt`) en iyi merceğinde (2,1,34) T2 = 8,80 (96. yüzdelik); nüzul dizilişi (`nuzul_kitap.txt`) en iyi merceğinde (3,1,19) T2 = 9,45 (tekdüzede 1/59, sıralıda 1/93).
