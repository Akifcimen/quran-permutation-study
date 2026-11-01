# K×100 plasebo kalibrasyonu, eşit derinlik — sonuç (1 Kasım 2026)

**Ön-kayıt:** `ONKAYIT.md`, donduruldu 2026-10-31T20:55:57Z, SHA-256 `b25e450e9660bcd7f9cb59cc3e326c32170daab98428df3b4760c9d341621055`. Sapma yok.

**Hesap:** workstation, `~/hrf_olcum/k100`, 21:05–23:35Z.
- İşlemci tarafı (hedef, dil arama): ~9 dk.
- Kart tarafı (RTX 4090 + RTX 3090): ~2 sa 20 dk. Kart sayacının kabul sınaması koşudan önce v3 olarak iki kartta 7/7, fark 0.

**Yeniden sayım:** `dogrulama/yeniden_say_k100.py` → `sonuc/yeniden_sayim_{M,L,U}.txt`.

## Karar

**Okuma (R_eşit): üç sıfırda da "arada".** Eşit derinlikte (10¹⁰ kontrol; Kur'an'la aynı bölütler) her sıfırda Kur'an'dan ayrılamayan plasebolar var. Her sıfırda yordam, en az bir rastgele kitapta Kur'an'da bulduğu kadar ya da daha düşük olasılıklı bir örüntü yakalıyor.

| Sıfır | K (dilli) | Kur'an (10¹⁰) | R₁ | R_derin | **R_eşit** | p_kal (R_eşit) | **R_nokta** | **π̂ = R_nokta/K** [%95] |
|---|---|---|---|---|---|---|---|---|
| M20 | 20.000 (19.856) | 47 → 4,7×10⁻⁹ | 58 | 12 | **8** | 9/20.001 ≈ 1/2.200 | **5** | 2,5×10⁻⁴ ≈ **1/4.000** [1/12.000; 1/1.700] |
| Mallows | 10.000 (9.939) | 22 → 2,2×10⁻⁹ | 20 | 6 | **3** | 4/10.001 ≈ 1/2.500 | **1** | 1,0×10⁻⁴ ≈ **1/10.000** [1/400.000; 1/1.800] |
| tekdüze | 10.000 (9.921) | 12 → 1,2×10⁻⁹ | 29 | 5 | **2** | 3/10.001 ≈ 1/3.300 | **1** | 1,0×10⁻⁴ ≈ **1/10.000** [1/400.000; 1/1.800] |

**Yordamın Kur'an'daki kadar ya da daha düşük olasılıklı örüntü yakaladığı kitaplar (R_nokta), 10¹⁰'da:**

| Sıfır | Plasebo | Mercek | İsabet / 10¹⁰ | Dil |
|---|---|---|---|---|
| M20 | A19926 | 2n − a, 7 | 3 | FAd2, FSC, WEq, HK2 |
| M20 | A14175 | 2n − a, 13 | 7 | FSd2, FCK, FKS, WEq |
| M20 | A14080 | n − a, 7 | 11 | R0e, Qc, Qh, FSC, WEq, HR1e |
| M20 | A7676 | n + a, 23 | 18 | FNd2, WEq, HR1e |
| M20 | A535 | n − a, 13 | 44 | B1, FAd2, FKS, WUNd2, WUT2p |
| Mallows | A7192 | 2n + a, 17 | 0 | BMt, FAd2, FSC, WI1, WUAd2, WHNp |
| tekdüze | A5225 | 2n + a, 17 | 1 | BMt, FCd, FAd2, FSC, WI1, WHRd2 |

**R_eşit'te kalıp nokta kestiriminde Kur'an'dan sık olanlar:**
- M20: A4111 48, A4268 50, A12732 60.
- Mallows: A142 24, A6806 27.
- tekdüze: A358 19.

## Ön-kayıtlı tahminler (§4)

| | M20 tahmin → gerçek | Mallows tahmin → gerçek | tekdüze tahmin → gerçek |
|---|---|---|---|
| R₁ | 18–45 → **58** (üstünde) | 25–60 → **20** (altında) | 45–100 → **29** (altında) |
| R_derin | 4–20 → 12 ✓ | 4–20 → 6 ✓ | 8–35 → **5** (altında) |
| R_eşit | 3–15 → 8 ✓ | 3–15 → 3 ✓ | 3–18 → **2** (altında) |
| R_nokta | 1–12 → 5 ✓ | 1–12 → 1 ✓ | 1–14 → 1 ✓ |
| π̂ | ~1/4.000 → 1/4.000 ✓ | ~1/2.000 → 1/10.000 | ~1/1.700 → 1/10.000 |
| Okuma | arada → arada ✓ | arada → arada ✓ | arada → arada ✓ |

- Okuma sınıfı üçünde de tuttu. M20 sayıları tahmine çok yakın çıktı.
- Mallows ve tekdüzede Kur'an tahminden iyi çıktı. Bu, ana hattın Kur'an'ın aleyhine yanıldığı ilk durumdur. Ön-kayıttaki uyarı ("sapma büyük olasılıkla Kur'an'ın lehine") burada tutmadı.

## Doğrulama (§5)

1. **Bağımsız yeniden sayım:** bütün partiler okundu, eksik bölüt yok.
2. **Diller:** her sıfırda en küçük p'li beş plasebonun, R_nokta üyelerinin ve Kur'an'ın dili `plasebo_dil_sina.py` ile kendi merceklerinde sınandı; hepsi TAMAM (M20 11, Mallows 6, tekdüze 6). Betiğe FH2p, WHNp, FCd ve QH eklendi.
3. **K×10 ile aynılık:** İlk 2.000 / 1.000 hedef ve dilleri K×10 ile bayt bayt aynı. Her iki çalışmada sayılmış bütün (dil, bölüt) çiftleri aynı çıktı (k ve kB):
   - M20: 29.552 çift;
   - Mallows: 26.044 çift;
   - tekdüze: 37.812 çift;
   - toplam fark 0.
   
   K×10'un düzey 1–3'ünü işlemci, K×100'ünkünü kart saydı.
4. **Kur'an'ın sayımı:** Bölüt 1–40.000 toplamları kapı çalışmasınınkilerle aynı (47, 22, 12). Bölüt bölüt karşılaştırmada (`kos/k100_dogrula.sh`; k ve kB satırları) üç sıfırda da 40.000 bölütün hepsi aynı; fark 0. Karşılaştırılanlar:
   - M20: `kapi/kuran/Gc/M`;
   - Mallows: `yaris/kuran/Gc/L`, yarış düzeltmeli;
   - tekdüze: `kapi/kuran/Gc/U`.
   
   Böylece kart sayacının kip 0 yolu ile işlemci sayacının kip 1 yolu (`kontrol_h`) Kur'an dilinde 3×10¹⁰ kontrol boyunca aynı sonucu verdi.
5. **Çapraz sınama:** Kartın yazdığı 10 bölüt işlemci sayacıyla (tek iş parçacığı, ayrı dizin) yeniden hesaplandı; hepsi `sure_s` dışında bayt bayt aynı. Bölütler düzey 1, 2, 3, 4 ve 5'ten, her iki karttan seçildi:
   - M20: d1_1 b3, d2_0 b17 (1.014 dil), d5_0 b17113, b39998;
   - Mallows: d3_0 b222, d5_0 b4004, b25003;
   - tekdüze: d4_0 b1919, d5_0 b33337, b40000.

## Betimsel: yakalanan örüntüler

- **Gerçekten rastgeleler.** 114 sûreden 3–9'u Kur'an'daki yerinde.
- **M20'nin dördü** küçük asallarda (7, 13) ve 23'te. Dördünün de dilinde "iki ailenin boyu eşit" (WEq) var. Hiçbirinin dili kitap düzeyine uzanmıyor.
- **Mallows'ta ve tekdüzede en düşük olasılıklı örüntüler** (A7192, A5225) aynı mercekte, 2n + a, 17, ve neredeyse aynı dili kuruyor. İkisinin de karşı ailesi çok küçük (1 ve 3 sûre). İkisi de kitap düzeyine bir bölünebilirlik cümlesiyle uzanıyor: iki yarının t-toplamları 17'nin katı.
  - A7192'nin tek karşı aile üyesi 17. sûre (34 ayet): 2·17 − 34 = 0 ve 2·17 + 34 = 68 = 4·17.
  - A5225'te ailenin ayet toplamı 289 = 17², karşı ailenin sûre numaraları toplamı 114.
- **Merceklerinde aynı anda doğru olan yüklem sayısı:** Kur'an 21, M20'nin dördü 8–13, A7192 18, A5225 14.
- Kur'an'ın dilindeki terazi türünden bir kitap düzeyi denkliği (B2: 3303 = 3303) bu yedi kitabın hiçbirinin dilinde yok.

Bu gözlemler sonuçlardan sonra yapıldı; kararlarda kullanılmaz.

## Kesinlik ve sınıflandırma (1 Kasım 2026 eki; dış inceleme)

- Ön-kayıttaki ±%30 kesinlik hedefi sağlanmadı: R_nokta 5 (M20) ve 1 (Mallows, tekdüze) için bağıl hata ~%45 ve ~%100'dür. π̂'nin aralıkları bu yüzden geniştir.
- Kur'an'ın sınırındaki sınıflandırma (M20'de Kur'an 47; A535 44, A4111 48, A4268 50) bu hassasiyette kesin değildir. π̂'nin Poisson aralığı bu iç belirsizliği içermez. R_eşit onu temkinli biçimde kapsamaya çalışır, ama kesin bir permütasyon testi değildir.
- Kur'an'ın iç sayımında puanına ulaşan 81 kontrolün yalnız 12'si dilin dört yüklemini birden taşır (`../dis_inceleme/`). Kalibre edilen şey bu puandır, nesnenin tam profili değil.

## Anlamı ve sınırı

Bu dilbilgisinde (109 yüklem, kapı Gc, ≥ 3 tür / ≥ 3 düzey, 406 mercek), kendi en iyi bütünlük dilini kuran rastgele kitaplar arasında Kur'an'ın yeri şöyledir:
- M20'de kabaca 4.000'de bir;
- Mallows ve tekdüzede kabaca 10.000'de bir (aralıklar geniş: 1/1.800 – 1/400.000);
- eşit derinlikte ayrılamayanlar sayılınca 1/2.200 – 1/3.300.

Her sıfırda yordam, içine yapı konmamış rastgele kitapların bir kısmında Kur'an'da bulduğu kadar düşük olasılıklı bir örüntü yakalıyor. Bu, kitapların değil, aramanın kalibrasyonudur. Yakalanan örüntüler Kur'an'ınkinden başka türdendir; ölçü onları yalnız olasılıkla karşılaştırır.

Kur'an'ın kendi ham değeri (4,7 / 2,2 / 1,2 ×10⁻⁹) ile kalibre değeri arasındaki fark, dil ve merceğin kitaba bakılarak seçilmesinin bedelidir: ~10⁴–10⁵ kat. Aynı bedel, aramayla bulunmuş eski sayılara (10⁻¹³'lük çekirdekler) uygulanmamıştır. Onlar bu dilbilgisinin dışındadır ve kalibre edilmemiştir.

Bu çalışmanın ölçmediği şeyler (araştırmacının itirazları; kural menüsüne aday):
- 19'a öncel: metin 19'u anıyor; burada bütün asallar eşit bedelli;
- kitap düzeyinin şart olması;
- merceğin bütün profili (aynı anda tutanların çokluğu).

Bunlar ayrı bir ön-kayıtla, aynı plasebolarla ölçülebilir. Hepsi önceden yazılmalı, hepsinin sonucu raporlanmalı ve seçimin bedeli ödenmelidir.
