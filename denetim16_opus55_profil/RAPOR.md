# Profil çalışması — sonuç (1 Kasım 2026)

Ön-kayıt `ONKAYIT.md`, 2026-11-01T13:41:29Z'de donduruldu (SHA-256 `754c3bb1…`; koşudan sonra değişmedi). Koşu 13:41:32Z – 15:22:16Z, iş istasyonunda, iki kartta (RTX 4090 + 3090). Kod özeti `2f05d1fe…`, girdi özeti `d547b925…`.

## 1. Ne ölçüldü

Soru, 15. denetimin dış incelemesinin açık bıraktığı sorudur: Kur'an'daki ilişkilerin **hepsi birden**, aynı türden ilişkileri koruyan bir karşılaştırmada ne kadar nadirdir? Önceki kalibrasyon bir puanı ölçüyordu. O puana dilin bir kısmıyla da ulaşılabiliyordu.

Bu çalışmada bir kontrol bir hedefe ancak şu durumda yetişmiş sayılır: hedefin merceğinde kapı Gc tutar ve hedefin olayındaki her yüklem o mercekte doğrudur. Kısmi puan, bit modeli ve kontrol için mercek araması yoktur.

Üç kural, hiçbiri birincil değil:

- **O1:** hedefin bütünlük dili, merceğinde, hepsi birden. Kur'an için (n + a, 19)'da B2 K1 Qc WSum: terazi, ardışık hücre toplamları, hücre katsayıları toplamı 4·19, iki ailenin boyları toplamı 19.
- **O2:** hedefin merceğindeki bütün profil. Kur'an için 21 yüklem: B1 B2 F1 F2 R0c R1c R1e K1 K2 Qc QS Qh FC1 FD1 FH2p FSC W1 QU Qa WSum WUd.
- **O3:** bütün profil, mercek yalnız p = 19 mercekleri arasından seçilerek. Kur'an'da aynı mercek, aynı 21 yüklem.

Hedefler taze plasebolardır; hiçbiri daha önce üretilmemiş, görülmemiştir:

| | M20 | Mallows | tekdüze |
|---|---|---|---|
| Plasebo K | 20.000 | 10.000 | 10.000 |
| Dil kurabilen (O1, O2) | 19.866 | 9.938 | 9.905 |
| p = 19'da dil kurabilen (O3) | 4.019 (%20) | 2.108 (%21) | 2.011 (%20) |

Kontroller 15. denetimdekilerle aynıdır: aynı tohumlar, sıfır başına 10¹⁰. Kur'an'ın olayları ve Kur'an'la yarışabilecek her plasebo olayı 10¹⁰'a kadar sayıldı.

## 2. Doğrulama (ön-kayıt §6)

1. **Bütünlük.** 3 × 40.000 bölüt dosyasının hepsi tam (N = 250.000), aday taşması 0, kart hata dosyaları boş, her dosyanın olay sayısı düzey listesiyle aynı.
2. **Yeniden sayım.** `olay_say.py`, bölüt dosyalarından bağımsız topladı (`sonuc/okuma_*.txt`).
3. **Kur'an'ın isabetleri.** Bütün `.q` kayıtları (M20 3, Mallows 2, tekdüze 0) tanımlardan bağımsız bir Python hesabıyla yeniden hesaplandı (`dogrulama/q_yeniden.py`). Her kayıt Kur'an'ın 114 sayısının bir permütasyonu, mercek (n + a, 19), kapı Gc ve dört yüklemin dördü de doğru: 5/5.
4. **Özgün yol.** Her sıfırda ön-kaydın özetinden tohumlanan 3 rastgele bölüt ve Kur'an'ın isabet aldığı 5 bölüt `olay_ozgun` ile yeniden sayıldı. Bu, özgün `kutuphane.h` yolu ve tek iş parçacığıdır. 14/14 satır satır aynı (`sonuc/ozgun_sina/`).
5. **İlk K kaydı.** İlk 20.000 / 10.000 / 10.000 kayıt K×100'ünkilerle bayt bayt aynı; koşudan önce sınandı.

Çapraz tutarlılık: Kur'an'ın O1 isabetlerinden ikisi (M20 bölüt 7237 sıra 41379; Mallows bölüt 14209 sıra 94893) dış incelemenin incelediği iki karşıörnektir. Kontroller aynı tohumlardan geldiği için aynı kitaplar yeniden çıktı.

## 3. Sonuçlar

İsabet sayısı 10¹⁰ kontrolde. p = (k + 1)/(N + 1); aralıklar %95.

| Kural | | M20 (K = 20.000) | Mallows (10.000) | tekdüze (10.000) |
|---|---|---|---|---|
| **O1** | Kur'an | 3 → 4,0×10⁻¹⁰ [0,6; 8,8] | 2 → 3,0×10⁻¹⁰ [0,2; 7,2] | 0 → < 3,7×10⁻¹⁰ |
| | R / p_kal | 8 → 9/20.001 ≈ **1/2.200** | 3 → 4/10.001 ≈ **1/2.500** | 0 → **≤ 1/10.001** |
| | R_nokta | 3 | 1 | 0 |
| | okuma | arada | arada | **aşar** |
| **O2** | Kur'an | 0 | 0 | 0 |
| | R / p_kal (üst sınır) | ≤ 244 → ≤ 1/82 | ≤ 125 → ≤ 1/79 | ≤ 131 → ≤ 1/76 |
| | R_nokta | 57 | 32 | 31 |
| | okuma | arada, çözünürlük sınırında | aynı | aynı |
| **O3** | Kur'an | 0 | 0 | 0 |
| | R / p_kal (üst sınır) | ≤ 17 → ≤ 1/1.100 | ≤ 7 → ≤ 1/1.250 | ≤ 5 → ≤ 1/1.670 |
| | R_nokta | 3 | 3 | 1 |
| | okuma | arada, çözünürlük sınırında | aynı | aynı |
| **Menü** | 3 × en küçük p_kal | **0,00135 ≈ 1/740** | **0,0012 ≈ 1/830** | **0,0003 ≈ 1/3.300** |

**O1.** En küçük p_kal üç sıfırda da bu kuraldan geliyor. Kur'an'ın dilini o mercekte taşıyan, 10¹⁰ kontrolde 3, 2 ve 0 kitap çıktı.
- **M20:** Plasebolardan üçü kendi dilinde 2 isabet aldı: A12493 (n + a, 13), A11192 (3n − a, 23), A19204 (n − a, 23). Beşi de 11–14 isabetle aralık çakışmasından R'ye girdi.
- **Mallows:** Biri 1 isabet aldı: A5344 (3n + a, 5). İkisi 7 ve 12 isabetle R'ye girdi.
- **Tekdüze:** En yakını 21 isabet. Hiçbir plasebo olayı Kur'an'ınkine aralıkla bile yaklaşmıyor.

K×100'ün puana dayalı eşit derinlik sayımı (R_eşit 8 / 3 / 2) taze plasebolarda, kısmi puan olmadan 8 / 3 / 0 olarak tekrarlandı.

**O2 ve O3.** Kur'an'ın bütün profili 3 × 10¹⁰ kontrolün hiçbirinde, Kur'an'ın merceğinde görülmedi.
- Bu tek başına ayırt edici değildir. Bütün profiller rastgele kitaplarda da çok seyrektir: taze plaseboların ~%0,3'ünün bütün profili de 10¹⁰'da hiç görülmedi (R_nokta 57 / 32 / 31).
- Doğrudan sayım bu düzeyde Kur'an'ı onlardan ayıramaz. Yazılan R, sıranın Kur'an aleyhine bir üst sınırıdır: aralığı Kur'an'ınkiyle çakışan, yani 10¹⁰'da ≲ 8 isabet alan bütün plasebolar R'ye girer.
- O3'te p = 19'da bütün profili hiç görülmeyen plasebo 3 / 3 / 1'dir. Bunların O3'teki ağırlığının bir kısmı şuradan gelir: plaseboların ~%80'i p = 19'da dil kuramıyor. Kural bunu "yetişmedi" sayar (ön-kayıt §3).

## 4. Model sırası (ikincil)

**Doğrulama şartı üç kuralda da sağlandı.** Çözülmüş olaylarda (k ≥ 10):

| | ρ (M20 / Mallows / tekdüze) | (−log₂ p̂ − bilgi) medyanı | %10 – %90 |
|---|---|---|---|
| O1 | 0,90 / 0,90 / 0,90 | +1,0 / +1,0 / +1,0 bit | −0,4 … +3,3 |
| O2 | 0,92 / 0,92 / 0,91 | +1,3 / +1,3 / +1,3 | −0,5 … +3,7 |
| O3 | 0,96 / 0,96 / 0,96 | +4,2 / +4,1 / +4,3 | +2,9 … +5,8 |

Model nadirliği abartmıyor, küçümsüyor; yani temkinli. Bu yüzden R_S "model kalibrasyonu" olarak yazılır:

| | M20 | Mallows | tekdüze |
|---|---|---|---|
| R_S, O1 / O2 / O3 | 12 / 21 / 0 | 3 / 4 / 0 | 2 / 2 / 0 |
| O3 model p_kal | 1/20.001 | 1/10.001 | 1/10.001 |
| Menüyle (× 3) | ≈ 1/6.700 | ≈ 1/3.300 | ≈ 1/3.300 |

Modele göre Kur'an'ın 19'daki bütün profili, üç sıfırda da bütün taze plaseboların 19'daki bütün profillerinden nadirdir. Ama aradaki fark sıfırdan sıfıra çok değişiyor:

| | Kur'an S | En yakın plasebo S | Fark |
|---|---|---|---|
| M20 | 26,88 | 26,21 (A6053, (2n + a, 19), 15 yüklem) | **0,7 bit** |
| Mallows | 28,18 | 23,47 | 4,7 bit |
| tekdüze | 31,64 | 23,62 | 8,0 bit |

- **M20:** 0,7 bitlik fark, modelin olaydan olaya saçılımından (%10–%90 aralığı ~3 bit) küçüktür. Burada R_S = 0 kırılgandır. A6053'ün mercek bedeli düşülmemiş bilgisi (34,53) Kur'an'ınkinden (34,20) biraz yüksektir.
- **Mallows ve tekdüze:** Fark sağlamdır.

Model ikinci ölçüdür; asıl okuma doğrudan sayımdır.

## 5. Tahminlerin denetimi (ön-kayıt §9)

| | Tahmin (M20 / Mallows / tekdüze) | Gerçek | |
|---|---|---|---|
| Kur'an O1 isabeti | 3–20 / 1–10 / 1–8 | 3 / 2 / 0 | 2/3; tekdüze altında |
| R_O1 | 10–60 / 3–25 / 3–25 | 8 / 3 / 0 | 1/3; ikisi altında |
| Kur'an O2 = O3 isabeti | 0 / 0 / 0 | 0 / 0 / 0 | 3/3 |
| R_O2 | 300–3.000 / 150–1.500 / 150–1.500 | 244 / 125 / 131 | 0/3; hepsi altında |
| R_O3 | 50–300 / 20–150 / 20–150 | 17 / 7 / 5 | 0/3; hepsi altında |
| R_S (O1 / O2 / O3) | aralıklar | 9 değerin 9'u içinde | 9/9 |
| Doğrulama şartı | sağlanır (olasılık ≈ 0,8) | sağlandı | ✓ |

- **Yanılan 9 tahmin:** Hepsi aynı yönde yanıldı. Sonuç, Kur'an için tahminden daha ayırt edici çıktı; yani tahminler karamsardı. R_O2 ve R_O3 tahminlerimi 10⁷'deki çözülmemiş olay sayılarından çıkarmıştım. 10¹⁰'da bu olayların çoğu 9'dan fazla isabetle çözüldü ve R'den çıktı.
- **Önceki denetimlerin tahminleri:** Bunlar iki yönde de yanılmıştı: K×10'da iyimser, K×100'de Mallows ve tekdüze için karamsar. Bu sefer yanılmaların hepsi karamsar yönde.

## 6. Okuma

**Ne söylüyor:**

1. **Kısmi puan kaldırılınca okuma değişmiyor.** Bir kontrolün Kur'an'a yetişmesi için Kur'an'ın bütünlük dilinin dört cümlesini merceğinde birden taşıması gerekiyor. Bunu 10¹⁰ rastgele kitapta 3 (M20), 2 (Mallows) ve 0 (tekdüze) kitap yapıyor.
2. **Arama kalibrasyonu taze plasebolarda tekrarlandı.** Aynı yordam taze rastgele kitaplara uygulandığında, onlar için de o kadar seyrek bir olay kuruyor:
   - M20'de 20.000'de 8 kitapta;
   - Mallows'ta 10.000'de 3 kitapta;
   - tekdüzede 10.000'de hiçbirinde.

   Bunlar Kur'an'dan nadir kitaplar değildir. Bunlar, yordamın kendi verisinde Kur'an'ınki kadar seyrek bir örüntü bulduğu, yapısız rastgele kitaplardır. Örüntüleri başka türdendir: küçük asallar (5, 13, 23), başka düzeyler. M20'nin üç en yakınının hiçbiri 19'da değildir.
3. **Bütün profil.** Kur'an'ın bütün profili (21 yüklem) Kur'an'ın merceğinde 3 × 10¹⁰ kontrolün hiçbirinde görülmedi. Ama bütün profiller genel olarak bu düzeyde çözülemeyecek kadar seyrek. Doğrudan sayım yalnız üst sınır veriyor: M20 ≤ 1/82; 19 önceliğiyle ≤ 1/1.100. Doğrulanmış ve temkinli model ise Kur'an'ın 19'daki bütün profilini üç sıfırda da birinci sıraya koyuyor; M20'de bu sıra kırılgan.
4. **Kural seçiminin bedeli ödendikten sonra:** M20 ≈ 1/740, Mallows ≈ 1/830, tekdüze ≈ 1/3.300.

**Ne söylemiyor:**

- **Bu bir tasarım olasılığı değildir.** Bir p değeri de değildir. Ölçülen şey, bu örüntü arama yordamının rastgele kitaplarda Kur'an'ınki kadar seyrek bir olayı ne sıklıkla kurduğudur.
- **Kütüphane Kur'an görülerek yazıldı.** 109 yüklem, Kur'an'daki ilişkiler bilinerek seçildi. Bu, Kur'an'ın merceğinde daha çok yüklemin doğru çıkmasını kolaylaştırır. Plasebolar aynı kütüphaneyle ölçüldüğü için bu bedelin bir kısmı ödenir, tamamı ödenmez. Kütüphanenin Kur'an'a göre biçilmesi okumayı Kur'an'ın lehine çeker. Bu kalibrasyon o kaymayı ölçmez.
- **Önceki literatür.** Yapının büyük kısmı önceki literatürdedir (1990–2015). Bu çalışma onu yeniden ölçer, keşfetmez.

## 7. Sınırlar ve açık işler

- **Çözünürlük.** O2 ve O3'te Kur'an da en yakın plasebolar da 10¹⁰'da 0 isabet alıyor. Daha derin bir sayım (10¹¹), R'deki plasebo olaylarının çoğunu çözerdi; R_O3'ü R_nokta'ya (3 / 3 / 1) doğru indirirdi. Kartlarda tahminen sıfır başına ~5 saat, toplam ~15 saat. Ön-kayda bir ek gerektirir; başlatılmadı.
- **19 önceliği.** O3'ün gücünün bir kısmı, plaseboların ~%80'inin p = 19'da hiç dil kuramamasından gelir. Bu, metnin andığı sayıya bağlanmanın bedelidir. Menü bu bedelin yalnız kural seçimi kısmını öder.
- **Model.** Model gruplar arasında bağımsızlık varsayar. Aynı toplamın iki adı modelde çifte sayılabilir. O2'de 10¹⁰'da hiç görülmeyen plasebo olaylarından 23 / 4 / 2'sinin bilgisi Kur'an'ınkinden yüksektir: d-türü bölünebilirlik yüklemlerinin yığıldığı profiller.
- **Mercek dışı.** "Hiçbir mercekte" sorusu ölçülmedi. Bir kontrolün Kur'an'ın 21 yüklemini başka bir mercekte taşıyıp taşımadığına bakılmadı.

## 8. Depodaki düzeltme (2 Kasım 2026'da uygulandı)

`8a28fd5`'te şu cümle fazla ileri gidiyordu: "Tam profili taşıyan her kontrol isabetler arasında görüneceğinden, 3×10¹⁰ kontrolün hiçbirinde, hiçbir mercekte bu profil yoktur."

Neden fazla ileri gidiyordu:
- Tam profili taşıyan bir kontrol Kur'an'ın puanına ulaşmayabilir: bit, kontrolün kendi bağlamına (k_F kutusu) bağlıdır.
- Başka bir mercekte mercek bedeli ve bağlam başkadır.
- Bu yüzden cümle yalnız puana ulaşan 81 kontrol için kanıtlıydı.

Bu çalışma, cümlenin Kur'an'ın merceğindeki kısmını doğrudan ölçtü (§3, O2):
- **Düzeltilmiş cümle:** "Kur'an'ın merceğinde (n + a, 19), 3×10¹⁰ kontrolün hiçbiri onun bütün profilini (21 yüklem) taşımıyor. Bu tek başına ayırt edici değil: taze plaseboların ~%0,3'ünün bütün profili de 10¹⁰'da hiç görülmedi. Başka mercekler bu sayımın dışında."
- **Düzeltilen yerler:** KESİF / DISCOVERY not 28'in eki, `denetim15…/dis_inceleme/BENIOKU.md` madde 1 ve ortak yazarın değerlendirmesi (TR/EN), 37. satır.

**Uygulanan biçim (2 Kasım 2026).** Yukarıdaki taslak ölçüm 1'den kalmadır; "tek başına ayırt edici değil" kısmını ölçüm 2 (`RAPOR_EK.md`) çözdü. Depoya şu biçimde girdi: "Kur'an'ın merceğinde (n + a, 19) 3×10¹⁰ kontrolün hiçbiri onun bütün profilini (21 yüklem) taşımıyor; kestirimle bu profilin sıklığı ~3–6×10⁻¹³. Aynı ölçüyle taze plasebolarda (M20 20.000, Mallows 10.000) 19'a bağlı hiçbir bütün profil bu kadar nadir çıkmadı; tekdüzede kestirici doğrulamayı geçemedi. Başka mercekler bu sayımın dışında."

## Dosyalar

- `ONKAYIT.md`
- `src/`, `kos/`, `dogrulama/`:
  - `olay_say.py`
  - `q_yeniden.py`
  - `profil_ozet.py`
  - `ozgun_sina.sh`
- `sonuc/`:
  - `kosu_gunlugu.txt`
  - `okuma_{M,L,U}.txt`
  - `ozet.txt`
  - `ozgun_sina/`
  - `tab/`: 518 MB; bölüt dosyaları ve `.q` kayıtları.
