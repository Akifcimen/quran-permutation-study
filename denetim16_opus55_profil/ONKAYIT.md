# Profil çalışması — ilişkileri koruyan olay kalibrasyonu, taze plasebolar — ön-kayıt (ana hat, 1 Kasım 2026)

## 1. Soru

15. denetimin dış incelemesi, şimdiye kadarki kalibrasyonun bir puanı kalibre ettiğini gösterdi. Bir kontrol, hedefin dilinin bir kısmıyla da hedefin puanına ulaşabiliyordu. Kur'an'ın puanına ulaşan 81 kontrolün yalnız 12'si dilin dört yüklemini birden taşıyordu ve hiçbiri Kur'an'ın merceğindeki olguların hepsini taşımıyordu.

Araştırmacının sorusu ise şudur: Kur'an'daki ilişkilerin birlikte bulunması, aynı türden ilişkileri koruyan bir karşılaştırmada ne kadar nadirdir?

Bu çalışma dört şeyi değiştirir:
1. **Kısmi puan yok.** Bir kontrol bir hedefe, hedefin bütün yüklemlerini hedefin merceğinde birden taşıyorsa yetişmiş sayılır.
2. **Bütün profil.** Ölçülen şey yalnız seçilmiş dil değil, hedefin merceğinde doğru olan bütün düzenliliklerdir.
3. **Metnin andığı 19.** Bir kural, mercekleri yalnız p = 19 ile sınırlar.
4. **Taze plasebolar.** Hiçbiri daha önce üretilmemiş, görülmemiştir.

Sorulan şey bir olayın sıklığıdır: aynı ilişkilerin hepsi birden. Bir puan değildir.

## 2. Hedefler

- **Kur'an:** kayıt 0; kodeks 129.
- **Taze plasebolar:** aynı üretici (`hedef.c`), aynı tohum kuralı tohum(s, 3, h).
  - M20: h = 20.001…40.000, yani 20.000 kitap.
  - Mallows ve tekdüze: h = 10.001…20.000, yani 10.000'er kitap.
  - Bu tohumlarla hiçbir kitap daha önce üretilmedi.
  - Tam üretimin ilk 20.000 / 10.000 kaydı K×100'ünkilerle bayt bayt aynı olmalıdır; koşudan önce sınanır.
- **Isınma:** M20 30.000 (ısınma tanısının incelenen göstergelerde etki görmediği değer).

## 3. Mercek, dil ve olaylar

Her hedefe değiştirilmemiş dil arama yordamı uygulanır: `dil_b A`, Kol A kuralı (≥ 3 tür, ≥ 3 düzey), kapı Gc (her hücre ≥ 2), 109 yüklemlik kütüphane.

- **L_A, Λ_A:** bütün 406 mercek arasından seçilen Kol A merceği ve dili.
- **L_19:** aynı yordamın yalnız p = 19 olan 14 mercekle sınırlanmış sürümüyle seçilen mercek. Yordamın kaynağına tek satırlık bir süzgeç eklendi; ortam değişkeni verilmezse çıktı özgün yordamla bayt bayt aynıdır (sınandı).
- **Π(L):** hedefin L merceğinde doğru olan bütün kütüphane yüklemleri, analitik dışlamalardan sonra.

Her hedefin en çok üç olayı vardır:

| Kural | Olay | Ne ölçer |
|---|---|---|
| **O1** | (L_A, Λ_A) | Hedefin bütünlük dilinin dört (ya da üç, beş, altı) cümlesinin hepsi birden |
| **O2** | (L_A, Π(L_A)) | Hedefin merceğindeki bütün düzenlilikler birden. Kur'an için (n + a, 19)'da 21 yüklem |
| **O3** | (L_19, Π(L_19)) | Aynısı, mercek metnin andığı 19'dan seçilerek. Kur'an'da L_19 = L_A |

Bir kural için dil kuramayan hedefin o kuralda olayı yoktur ve Kur'an'a yetişmemiş sayılır.

Bir kontrol bir olayı şu durumda taşır: olayın merceğinde kapı Gc tutar ve olayın her yüklemi o mercekte doğrudur. Bit modeli, kısmi puan ve kontrol için mercek araması yoktur.

Mercek seçiminin bedeli plasebo kalibrasyonuyla ödenir: her plasebo kendi merceğini Kur'an'la aynı yordamla seçti. Kural seçiminin bedeli §5'teki menüyle ödenir. Zorunlu sonuçlar ve aynı olgunun iki adı çifte sayılmaz, çünkü ölçülen şey bir olaydır, bir toplam değil.

## 4. Kontroller ve derinlik

- Aynı sıfırlar (M20, Mallows, tekdüze) ve aynı tohumlar (tür 2). Bölüt b, 250.000 kontroldür.
- Sayaç: kart sayacının olay kipi (`kart_olay`). Profil, bayt bayt eşdeğerliği kabul edilmiş çekirdekle hesaplanır.

| Düzey | Bölüt | Kümülatif | Seçim |
|---|---|---|---|
| 1 | 1–4 | 10⁶ | bütün olaylar |
| 2 | 5–40 | 10⁷ | Kur'an'ın olayları ∪ {k(1–4) < 10} |
| 3 | 41–400 | 10⁸ | Kur'an'ın olayları ∪ {düzey 2'de olup k(1–40) < 10} |
| 4 | 401–40.000 | **10¹⁰** | Kur'an'ın olayları ∪ {düzey 3'te olup k(1–400) < 10} |

Kur'an'ın olayları her zaman 10¹⁰'a kadar sayılır. Kur'an'la yarışabilecek her plasebo olayı da Kur'an'la aynı derinlikte ve aynı bölütlerde sayılır.

## 5. Okuma kuralları

Her kural t ∈ {O1, O2, O3} için:

- **Derinlik:** Plasebo olayının sayımı derinlik kuralıyla alınır: k(1–4) ≥ 10 ise 1–4; değilse k(1–40) ≥ 10 ise 1–40; değilse k(1–400) ≥ 10 ise 1–400; değilse 1–40.000.
- **p ve aralık:** p = (k + 1)/(N + 1), %95 Poisson-Garwood aralığı.
- **R_t:** p ≤ p_Q olan ya da aralığı Kur'an'ınkiyle çakışan plasebo sayısı. p_kal,t = (1 + R_t)/(K + 1).
- **Okuma:** R = 0 ise aşar; R/K ≥ 0,05 ise sık; arası arada.
- **R_nokta,t:** k/N ≤ k_Q/N_Q olan plasebo sayısı.
- **Çözünürlük kuralı:** Kur'an'ın olayı 10¹⁰'da 10'dan az isabet alırsa, Kur'an'ın değeri yalnız bir üst sınırdır. Bu durumda R_t'ye 10¹⁰'da kabaca 8 ya da daha az isabet alan bütün plasebolar girer; R_t sıranın Kur'an aleyhine bir üst sınırıdır. Okuma "sık" çıksa bile "çözünürlük sınırında" diye yazılır.
- **Menü:** Üç kural birlikte raporlanır; hiçbiri birincil değildir. Kural seçiminin bedeli: menüye göre düzeltilmiş kalibre değer = min(1, 3 × en küçük p_kal,t).

**Model sırası (ikincil, doğrulama şartıyla).**
- Her olayın model bilgisi: olayın grup maskelerinin referans tablolarındaki üst-küme sayımlarından, 13–15. denetimlerin bağlam ve bit kuralıyla (`olay_kur.c`). S = bilgi − c(L).
- R_S,t, S ≥ S_Q olan plasebo sayısıdır.
- **Doğrulama:** Taze verinin doğrudan sayımla çözülmüş olaylarında (k ≥ 10) şunlara bakılır: model bilgisi ile −log₂ p̂ arasındaki Spearman ρ ve (−log₂ p̂ − bilgi) farkının dağılımı.
- **Şart:** her kuralda ρ ≥ 0,7 ve farkın medyanı ≥ −1 bit. Yani model, nadirliği medyanda 2 kattan fazla abartmamalıdır.
- Şart sağlanırsa R_S "model kalibrasyonu" olarak yazılır; sağlanmazsa yalnız betimseldir.
- Model, gruplar arasında bağımsızlık varsayar. Aynı toplamın iki grupta görünmesi modelde çifte sayılabilir; doğrudan sayımda sayılmaz.

## 6. Kabul ve doğrulama

**Koşudan önce yapıldı.** Geliştirme verisi K×100'ün ilk 200 hedefiydi; üç sıfır, iki kart, bölüt 1–2. `kart_olay` (kart), `kart_olay_emul` (aynı çekirdek, işlemci) ve `olay_ozgun` (özgün `kutuphane.h` yolu, tek iş parçacığı) 18 karşılaştırmanın hepsinde aynı çıktı. `.q` kayıtları kartla öykünmede aynı. Yamalı `dil_b` (süzgeçsiz) üç sıfırda özgünle aynı (`kabul/`).

**Koşudan sonra:**
1. `olay_say.py` ile bağımsız yeniden sayım.
2. Kur'an'ın olaylarının bütün isabetleri (`.q`), O1 için bağımsız bir Python yeniden hesabıyla (`isabet_yeniden.py`'nin yüklemleri) sınanır.
3. Her sıfırda rastgele 3 bölüt `olay_ozgun` ile yeniden hesaplanır.
4. İlk K kaydın K×100 ile aynılığı.

## 7. Kaynak

- Workstation, `~/hrf_olcum/profil`.
- İşlemci: en çok 12 iş parçacığı, `nice -n 10`.
- Kartlar: RTX 4090 bölüt b mod 5 ∈ {0, 1, 2}, RTX 3090 {3, 4}.
- Tahmini süre: 3–6 saat.

## 8. Geliştirme denemesi (beyan)

Bu ön-kayıttan önce, tasarımın işleyip işlemediğini görmek için aynı ölçüm K×100'ün görülmüş hedeflerinde (20.000 / 10.000 / 10.000) yalnız düzey 1–2'de (10⁷) koşuldu (`pilot/`). Bu sonuçlar kararlarda kullanılmaz. Görülenler:

- **O1:** M20'de 19.856 olaydan 732'si 10⁷'den sonra çözülmemiş kaldı (179'u 0 isabet). Mallows 320, tekdüze 351.
- **O2:** Bütün profiller çoğu rastgele kitapta da çok seyrektir. M20'de 5.940 olay çözülmedi (2.384'ü 0 isabet); Mallows 3.001, tekdüze 2.928. 10¹⁰'da bile yüzlercesinin çözülmemiş kalması beklenir.
- **O3:** p = 19'da dil kurabilen hedef ~%20. M20'de 746 olay çözülmedi.
- **Kur'an:** üç olayında da 10⁷'de 0 isabet.
- **Model ve doğrudan sayım:**
  - Çözülmüş olaylarda ρ: O1 0,86, O2 0,78–0,79, O3 0,90–0,91.
  - (−log₂ p̂ − bilgi) medyanı: O1 ve O2'de ~+1 bit, O3'te ~+4 bit. Yani model nadirliği ortalamada küçümser; temkinlidir. Bunun bir kısmı, modelin bağlamın, yani k_F kutusunun, kendi olasılığını içermemesidir.
- **Model sırası R_S:**
  - O1: M20 22, Mallows 5, tekdüze 4.
  - O2: 24, 6, 1.
  - O3: 0, 0, 0.

## 9. Ana hattın tahmini (geliştirme denemesinden sonra, taze sonuçlardan önce)

| | M20 (K = 20.000) | Mallows (10.000) | tekdüze (10.000) |
|---|---|---|---|
| Kur'an O1 isabeti, 10¹⁰ | 3–20 | 1–10 | 1–8 |
| R_O1 | 10–60 | 3–25 | 3–25 |
| Kur'an O2 = O3 isabeti, 10¹⁰ | 0 | 0 | 0 |
| R_O2 | 300–3.000 (çözünürlük sınırında) | 150–1.500 | 150–1.500 |
| R_O3 | 50–300 (çözünürlük sınırında) | 20–150 | 20–150 |
| Model R_S (O1 / O2 / O3) | 10–40 / 10–40 / 0–3 | 2–15 / 2–15 / 0–3 | 2–15 / 0–10 / 0–3 |

Doğrulama şartının üç kuralda da sağlanacağını bekliyorum (olasılık ≈ 0,8).

**Uyarı:** Ana hattın geçmiş tahminleri bir kez Kur'an'ın aleyhine, çoğunlukla lehine yanıldı. Bu tablo geliştirme verisine dayanır; taze veride sapma iki yönde de olabilir.
