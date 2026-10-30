# KAPI RAPORU — plasebo14'ün sonucu kapı G'nin yerleşimine mi bağlı?

**Soru.** plasebo14'te Kur'an'ın bütünlüklü ayrışması bütün plaseboları aştı. Ama bütünlük kuralının içindeki kapı G (k_F·p ≥ 228, her hücre ≥ 2) Kur'an'ın tam sınırına denk geliyordu: 12·19 = 228. Kapıyı Kur'an'dan bağımsız kurallarla değiştirince sonuç değişiyor mu?

| | |
|---|---|
| Ön-kayıt | `ONKAYIT.md`, donduruldu **2026-10-29T09:41:23Z**, SHA-256 `6d8db06b45532c28568b71bbd92906999b60f06f6840ab32509480ca9c8b2e09` |
| Kod | plasebo14'ün donmuş kaynakları ve üç değişiklik (§1). Donmuş kod listesi (`sonuc/kod_sha256.txt`, SHA-256 `d7d4745d…3073`) koşulardan sonra yerelde ve workstation'da yeniden sınandı: hepsi aynı. |
| Hesap | workstation (i9-10920X), `nice -n 10`, en çok 12 iş parçacığı. Gc 09:41–12:48Z, GE 12:48–15:34Z, G0 15:34–18:12Z; kapı başına ≈ 13–15 çekirdek-saat. Kur'an tarafı: `kontrol_h`, 12 iş parçacığı. Proje diski < 2 GB. |
| Sapmalar | üç ek (`ek/`): dondurmadan sonra yazılan çalıştırma ve sınama kodu ile üç betimsel analiz (18:35Z); kod denetiminin bulduğu Mallows iş parçacığı yarışı (19:02Z) ve düzeltilmiş yeniden sayımın sonucu (23:40Z). Yarışın düzeltmesi dışında hesap kodu değişmedi. **Bütün Mallows sayıları bu raporda düzeltilmiş yeniden sayımdandır** (§4.5). |
| Doğrulama | bağımsız yeniden sayım; Kur'an'ın ve her kapı-sıfırda en yakın üç plasebonun dili Python'la sınandı (36/36); Kur'an tarafı isabetleri yeniden puanlandı ve tanımlardan sınandı. Kod farkı sonuçları bilmeyen bir ajanla denetlendi (§4.4); bulduğu Mallows yarışı düzeltildi ve bütün Mallows sayımları yeniden yapıldı (§4.5). |

**Gösterim** plasebo14 ile aynı. p = (k + 1)/(N + 1), köşeli ayraç %95 Clopper–Pearson. R: p'si Kur'an'ınkinden büyük olmayan ya da aralığı Kur'an'ınkiyle çakışan plasebo sayısı. **p_kal = (1 + R)/(K + 1).** K = 200 (M20), 100 (tekdüze, Mallows).

---

## 0. Kısa cevaplar

### 0.1 Birincil karar

**Gc'de iki sıralı sıfırda da "aşar".** Ön-kayıt §5'in ilk maddesi uygulanır: plasebo14'ün sonucu kapı G'nin yerleşimine bağlı değildir. Bütünlük okuması README'nin ana okuması olur. Çekince şöyle yazılır: "kapı G Kur'an'ın sınırındaydı; sonuç kapıdan bağımsız". GE ve G0 yanında verilir.

### 0.2 Tablo (Kol A)

| Kapı | Sıfır | dilli plasebo | Kur'an p (k/N) | en küçük plasebo p (ad, mercek) | R | p_kal | okuma |
|---|---|---|---|---|---|---|---|
| **Gc** (birincil) | M20 | 199/200 | 1,0×10⁻⁸ (0/10⁸) | 2,5×10⁻⁷ (A047) | 0 | **1/201** | **aşar** |
| | Mallows | 99/100 | 1,0×10⁻⁸ (0/10⁸) | 3,4×10⁻⁷ (A047) | 0 | **1/101** | **aşar** |
| | tekdüze | 99/100 | 1,0×10⁻⁸ (0/10⁸) | 1,0×10⁻⁸ (A088, (1,+3,3)) | 1 | 2/101 | arada |
| GE | M20 | 195/200 | 1,0×10⁻⁸ (0/10⁸) | 1,7×10⁻⁷ (A071) | 0 | 1/201 | aşar |
| | Mallows | 99/100 | 1,0×10⁻⁸ (0/10⁸) | 3,4×10⁻⁷ (A047) | 0 | 1/101 | aşar |
| | tekdüze | 97/100 | 1,0×10⁻⁸ (0/10⁸) | 1,0×10⁻⁸ (A088) | 1 | 2/101 | arada |
| G0 | M20 | 200/200 | 2,0×10⁻⁸ (1/10⁸) | 1,4×10⁻⁷ (A181) | 0 | 1/201 | aşar |
| | Mallows | 100/100 | 1,0×10⁻⁸ (0/10⁸) | 3,4×10⁻⁷ (A047) | 0 | 1/101 | aşar |
| | tekdüze | 100/100 | 1,0×10⁻⁸ (0/10⁸) | 1,0×10⁻⁸ (A088); 5,0×10⁻⁸ (A068) | 2 | 3/101 | arada |
| G2E (plasebo14, başvuru) | M20 / tekdüze / Mallows | 35 / 18 / 15 | 1,0×10⁻⁸ | 1,1×10⁻⁶ / 4,1×10⁻⁶ / 3,4×10⁻⁷ | 0 | 1/201, 1/101, 1/101 | aşar |

Kapının etkisi büyük ama tek yönlü. G2E'de plaseboların %82–85'i bütünlüklü dil kuramıyordu; ayrışmanın çoğu oradan geliyordu. Gc'de neredeyse hepsi kuruyor: 199/200, 99/100, 99/100. Plasebolar Kur'an'a yaklaşıyor (M20'de en yakın 1,1×10⁻⁶'dan 2,5×10⁻⁷'ye). Yine de sıralı iki sıfırda hiçbiri Kur'an'a yetişmiyor. Kapı tamamen kalkınca (G0) da durum aynı.

### 0.3 Tekdüzede neden "arada"?

Her kapıda aynı plasebo yetişiyor: **A088**, mercek (1,+3,3). Bu mercekte |β| = p = 3'tür, yani t = n + 3a ≡ n (mod 3): aile {3 | n}'dir ve dizilişe hiç bağlı değildir (k_F = k_H = 38, hücreler 9/10/10/9). Dili {F1, K1, WHRd2}'dir: aile yarıları eşit sayıda (19/19), hücre toplamları 728/728/728/729 ardışık, karşı ailenin katsayı toplamı p²'ye bölünür. On ikinci denetimin p = 3 dışlama listesi bu üç yüklemi kapsamıyordu; plasebo14'ten devralınan bir boşluktur. G0'da ikinci plasebo **A068** eklenir: (3,+2,43), beş üyeli bir aile, iki hücresi boş (k_F = 5, hücreler 3/0/0/2). Bunu ancak kapısız kural geçirir. Her iki durumda da yetişen yapı Kur'an türünden değildir. Kalibre p yine de kurala göre yazılır: 2/101, 3/101.

### 0.4 Kur'an'ın kendi sırası (§6)

Kur'an'ın Kol A dili üç kapıda da plasebo14'ünkiyle aynıdır: {B2, K1, Qc, WSum} @ (1,+1,19). Dil B2'yi gerektirdiği için `kontrol_h` kısayolu kullanıldı.

| Kapı | M20 | Mallows | tekdüze | N |
|---|---|---|---|---|
| G2E (kademe S3, başvuru) | 4 → 4×10⁻¹⁰ | 1 → 1×10⁻¹⁰ | 0 → < 3,7×10⁻¹⁰ | 10¹⁰ |
| **Gc** | **47 → 4,7×10⁻⁹ [3,5; 6,3]** | **22 → 2,2×10⁻⁹ [1,4; 3,3]** | **12 → 1,2×10⁻⁹ [0,6; 2,1]** | **10¹⁰** |
| Gc (betimsel) | 9 | 3 | 1 | 10⁹ |
| GE (betimsel) | 9 | 2 | 1 | 10⁹ |
| G0 (betimsel) | 23 | 9 | 1 | 10⁹ |

Gc 10¹⁰'da kademe durma kuralı (k ≥ 10) üç sıfırda da sağlandı: sayılar **ölçülmüştür**, üst sınır değildir. Kapı G2E'den Gc'ye gevşeyince Kur'an'ın kendi sırası M20'de ~12, Mallows'ta ~22 kat sıklaşır. Bunun nedeni, rastgele kitapların bütünlüğü en çok küçük asallarda kurması: M20'deki 47 isabetin 34'ü p = 13'te, yalnız 3'ü p = 19'da. G0'da sıklaşma 10⁹'da bir 2,5–3 kat daha.

### 0.5 Ana hattın tahmini (§8) tuttu mu?

| | tahmin | gerçekleşen | hüküm |
|---|---|---|---|
| Gc, M20 | dilli 120–200; en küçük p 10⁻⁸–10⁻⁶; R 0–5; en olası "arada" (0,40), "aşar" 0,35 | 199; 2,5×10⁻⁷; 0; **aşar** | sayılar **aralıkta**; okuma sınıfı **tutmadı** (en olası sınıf değildi) |
| Gc, Mallows | aynı | 99; 3,4×10⁻⁷; 0; **aşar** | **aralıkta**; okuma sınıfı **tutmadı** |
| GE, M20 | dilli 60–150; en küçük p 10⁻⁷–10⁻⁵; R 0–2; "aşar" ya da "arada" | 195; 1,7×10⁻⁷; 0; aşar | dilli sayısı **aralık dışında** (daha çok); öteki üçü **tuttu** |
| G0, M20 | Gc'ye yakın | 200; 1,4×10⁻⁷; 0; aşar | **tuttu** |
| Kur'an Gc / G2E | 3–30 kat artar | M20 ~12, Mallows ~22; tekdüze tanımsız (G2E'de 0) | **aralıkta** |

Ana hat Gc'de plasebolara daha çok şans tanımıştı. Plasebolar gerçekten yaklaştı, ama yetişmedi.

### 0.6 Ölçülmeyenler

- Bütünlük kuralının kendisinin seçimi (≥ 3 tür, ≥ 3 düzey) bu kalibrasyonla bedellendirilmedi. Kural, Kur'an'ın biçimi bilinerek yazıldı. Bu çalışma yalnız kapıyı değiştirdi.
- K = 200 / 100 plasebo, kalibre p'nin tabanını 1/201 ve 1/101'e bağlar. Kur'an'ın en yakın plasebodan ne kadar uzak olduğu (M20'de Kur'an 10¹⁰'da 4,7×10⁻⁹, en yakın plasebo 2,5×10⁻⁷) kalibre p'ye girmez.
- Malzemenin seçimi (sûre numarası, ayet sayısı, toplama) ve o seçimi yapan literatür araması.
- Kur'an tarafında 10¹¹ kademesi; Gc 10¹⁰ durma kuralını sağladığı için gerekmedi.

---

## 1. Neden ve ne değişti

plasebo14 Kol A'nın bütünlük şartı üç parçadır: kapı G, ≥ 3 motif türü, ≥ 3 düzey. Kapı G'nin aile boyu şartı (k_F ≥ 2E) Kur'an'ın aile boyuna tam denk düşer. On ikinci denetimin ön-kaydı bu şartı tam bu yüzden birincil puandan çıkarmıştı (§2.8). Ana hat plasebo14'ü yazarken onu bütünlük kuralının içinde bıraktı. Bu bir tasarım hatasıydı. Kapının etkisi büyüktü: plasebo12'de Kur'an'ın çekirdek dilinde Kur'an'a ulaşan 33 kontrolün hepsi kapı G'yi sağlamayan merceklerdeydi.

Bu çalışma Kol A'yı on ikinci denetimin duyarlılık kapılarıyla tekrarlar. Yeni bir kapı tanımı yoktur.

| Ad | Tanım | Durum |
|---|---|---|
| **Gc** | dört hücrenin her biri ≥ 2 üye; aile boyu şartı yok | **birincil**, sonuçlardan önce sabitlendi |
| GE | k_F·p ≥ 114 ve her hücre ≥ 2 | duyarlılık |
| G0 | kapı yok | duyarlılık |
| G2E | plasebo14'ün kapısı | başvuru |

Kapı dışındaki her şey plasebo14'tür: aynı 109 yüklemlik kütüphane, aynı bütünlük kuralı, aynı hedefler (`hedef.bin` aynı dosyalar), aynı kontrol tohumları, aynı derinlik kuralı. Koddaki üç değişiklik:
1. `kutuphane.h`: `kapiG` iki derleme parametresine bağlandı (KAPI_ESIK, KAPI_HUCRE). Varsayılan (228, 2) plasebo14'tür.
2. `kontrol_b.c`: iş parçacığı tavanı 4'ten 24'e.
3. `kos/kontrol_b.py`: referans yolu ve iş parçacığı sayısı ortam değişkenlerinden.

**Ö0.** Dondurmadan önce G2E ile Linux'ta derlenen hat plasebo14'ü birebir verdi: üç sıfırda `diller_A.txt`, `ayrinti_A.txt`, `hedef_ayrinti_A.txt`; M20 Kol A'nın 36 dilinin bölüt 1–4 sayımları (FARK 0) ve eşikler.

## 2. Plasebo sonuçları — en yakın üç plasebo

Her kapı ve sıfırda en küçük p'li üç plasebo. Diller bağımsız Python koduyla kendi merceklerinde sınandı (§4.2).

| Kapı, sıfır | 1. | 2. | 3. |
|---|---|---|---|
| Gc, M20 | A047 24/10⁸, (1,+1,19), {BMn, FRK, WUAd2, HR1c} | A161 10/10⁷ | A018 11/10⁷ |
| Gc, Mallows | A047 33/10⁸, (1,+3,11), {Zk2p, R0c, FKS, WUTd2} | A090 69/10⁸, (3,+1,5) | A060 32/10⁷ |
| Gc, tekdüze | A088 0/10⁸, (1,+3,3), {F1, K1, WHRd2} | A051 16/10⁸ | A039 20/10⁸ |
| GE, M20 | A071 16/10⁸, (1,−1,11), {BMt, R0c, WI1, WEq, WHNd2} | A047 24/10⁸ | A018 48/10⁸, (1,+1,7) |
| GE, Mallows | A047 33/10⁸ | A090 69/10⁸ | A016 43/10⁷ |
| GE, tekdüze | A088 0/10⁸ | A039 13/10⁸, (1,−1,7) | A051 14/10⁸, (1,−3,17) |
| G0, M20 | A181 13/10⁸, (2,+1,17), {BMt, FC2, WI1, WUNd2}; hücreler 2/1/1/3 | A183 33/10⁸, (1,+2,19); hücreler 4/0/0/2 | A086 54/10⁸, (1,+1,13) |
| G0, Mallows | A047 33/10⁸ | A090 13/10⁷ | A046 26/10⁷ |
| G0, tekdüze | A088 0/10⁸ | A068 4/10⁸, (3,+2,43); hücreler 3/0/0/2 | A098 48/10⁸ |

Gözlemler:
- M20'de Gc'nin en yakın plasebosu A047, Kur'an'ın merceğinde, (1,+1,19), bütünlük kuruyor. Ailesi 10 üyeli (10·19 = 190 < 228; G2E'de bu mercek kapalıydı). Dili Kur'an'ınkinden başka: iki kitap yarısının sûre numarası toplamlarının p'ye bölünmesi (BMn), aile yarılarının katsayı toplamlarının ardışık olması (FRK), birleşimin ayet toplamının p²'ye bölünmesi (WUAd2), karşı ailenin tek-t satırındaki iki hücrenin eşit sayıda üyesi olması (HR1c). Kur'an'ın dilindeki dört yüklemin (B2, K1, Qc, WSum) hiçbiri bu dilde yok.
- Kapı gevşedikçe yetişen ya da yaklaşan plasebolar yozlaşmaya doğru kayıyor: G0'da boş hücreli aileler (A183, A068) ve tek üyeli hücreler (A181) en yakınlar arasına giriyor.
- Mallows'ta üç kapıda da en yakın aynı plasebo (A047, 3,4×10⁻⁷): onun en iyi merceği her kapıyı zaten geçiyor.

## 3. Kur'an'ın kendi sırası

### 3.1 Betimsel sayım, 10⁹ (§6, ikinci madde)

plasebo14'ün donmuş Kur'an dilleri, `kontrol_h` ile her kapıda bölüt 1–4000. Kol A dilinin eşikleri kapıdan bağımsız çıktı (M20 18,80, tekdüze 20,83, Mallows 20,00 bit), çünkü Kur'an'ın kendi puanı kapıdan etkilenmez. M20 için kademe ön-kaydının duyarlılık dilleri de sayıldı:

| Kapı | A000 (Kol A) M / U / L | A− {B2, K1, Qc}, M20 | A+ {B2, K1, Qh, WSum}, M20 |
|---|---|---|---|
| Gc | 9 / 1 / 3 | 129 | 0 |
| GE | 9 / 1 / 2 | 64 | 0 |
| G0 | 23 / 1 / 9 | 181 | 3 |

GE, Kol A'da M20 ve tekdüzede Gc ile aynı sayıyı, Mallows'ta bir eksiğini verdi. WSum içermeyen A− dilinde ise GE, Gc'nin yarısını bıraktı (64'e karşı 129).

### 3.2 Derin sayım, Gc, 10¹⁰ (§6, ilk madde)

Bölüt 1–40000, her sıfırda 10¹⁰ kontrol (12:15–14:24Z).

| Sıfır | k | p | %95 | eşik |
|---|---|---|---|---|
| M20 | 47 | 4,7×10⁻⁹ | [3,5; 6,3]×10⁻⁹ | 18,80 |
| Mallows | 22 | 2,2×10⁻⁹ | [1,4; 3,3]×10⁻⁹ | 20,00 |
| tekdüze | 12 | 1,2×10⁻⁹ | [0,6; 2,1]×10⁻⁹ | 20,83 |

Aynı sayımda M20'nin duyarlılık dilleri: A− 1.204 (1,2×10⁻⁷), A+ 12 (1,2×10⁻⁹). Bölüt dosyalarından toplamlar yeniden sayıldı: N = 10¹⁰ her sıfırda, k'ler günlükle aynı.

İsabetlerin en iyi merceğinin asalı: M20 {11: 3, 13: 34, 17: 7, 19: 3}; Mallows {11: 5, 13: 15, 19: 2}; tekdüze {11: 2, 13: 9, 17: 1}. M20'de isabetler zincir ya da bölüt kümesi oluşturmuyor (kademe ön-kaydının küme kuralı: aynı bölüt, aynı zincir, |Δsıra| ≤ 96000).

p = 19'daki beş isabetin dördü (M20'deki üçü ve Mallows'taki biri) Kur'an'la tam eşit puanlıdır; eşitlik kuralı gereği Kur'an'ın aleyhine sayılmıştır. Beşincisinin puanı Kur'an'ınkinden yüksektir.

### 3.3 Sonradan analizler (ek; ön kayıtta yok, betimsel)

1. **19-öncel.** Merceği metnin andığı asal 19'da sabit tutunca Kur'an düzeyine ulaşan kitaplar: M20 3 (3×10⁻¹⁰ [0,6; 8,8]), Mallows 2 (2×10⁻¹⁰ [0,2; 7,2]), tekdüze 0 (< 3,7×10⁻¹⁰). Metnin andığı asallar {3, 5, 7, 11, 19} birlikte: 6, 7, 2. Alt sınır (en iyi mercek kümede) ile üst sınır (kümede bütünlüklü mercek var) her sıfırda eşit; sayılar kesin. Çıktı: `sonuc/kuran/Gc10/on_dokuz_sayimi.txt`.
2. **19'daki beş isabetin yapısı.** Hiçbiri Kur'an'ın olgularının hepsini taşımıyor. En yakını (M20 sıra 14644): 57/57, terazi, 12 + 7 ayrık, 1444, 722/722, hücreler 3/3/3/3 ve 18/19/19/20, katsayı bandı 5–8, |U| = 19; eksik olan yalnız Qa (birleşimin ayet toplamı 785 ≡ 6 mod 19). Kur'an'la ortak konum sayıları 4–9. Çıktı: `sonuc/kuran/Gc10/yapi_karsilastirma.txt`.

## 4. Doğrulama

### 4.1 Yeniden sayım (§7.1)
`dogrulama/yeniden_say_kapi.py`, plasebo14'te birebir doğrulanmış betik. Bölüt dosyalarından k, p, R ve p_kal'ı her kapı ve sıfır için yeniden hesaplar. Çıktılar `sonuc/<kapı>/yeniden_sayim.txt`; §0.2 tablosu bu çıktılardır.

### 4.2 Diller (§7.2)
`dogrulama/plasebo_dil_sina.py`: yüklemler `kutuphane.h`'deki tanımlardan ve dışlama kurallarından Python'a yazıldı. Her kapı ve sıfırda Kur'an ile en küçük p'li üç plasebo kendi merceğinde sınandı: kapı, her yüklem, tür ve düzey sayısı. **36 / 36 TAMAM** (`sonuc/plasebo_dil_sinamasi.txt`). Ön-kayıt üç plasebo diyordu; dokuz kapı-sıfır çiftinin hepsinde yapıldı.

### 4.3 Kur'an tarafı isabetleri
- **Yeniden puanlama.** Gc 10¹⁰'un bütün aday kayıtları (M20 268.133, tekdüze 5.221, Mallows 6.279; düzeltilmiş yeniden sayımda 6.277) `yeniden_puan` ile donmuş yoldan baştan puanlandı. Mercekler birebir aynı. Puan farkları ≤ 1×10⁻⁶: kayıt puanı tek duyarlıkla saklanır, yeniden puan çift duyarlıkla hesaplanır. Yeniden puanla sayılan isabetler aynı: 47 / 12 / 22.
- **Tanımlardan sınama.** `dogrulama/bagimsiz_sina_kapi.py` her isabeti C'den bağımsız olarak tanımlardan sınar: kapı, B2, K1, Qc, WSum, sınıf ve kaydın merceğinin bütünlüklü mercekler arasında olması. 47 + 12 + 22 kayıt, **uyuşmazlık 0**.

### 4.4 Kod denetimi (§7.3)
Kod farkını, `kontrol_h` kısayolunu, yeniden sayım betiğini ve Python sınayıcılarını sonuçları bilmeyen ayrı bir ajan denetledi (29 Ekim akşamı; okudu, derledi, küçük sınamalar koştu).
- **Doğru bulunanlar:** fark yalnız üç amaçlanan değişiklikten ibaret; kapı her yolda tek makrodan geliyor; `kontrol_h` kısayolu tam yolla kitap kitap aynı (6 dil × 4 kapı, 40.000 kitap, FARK 0); tohumlar, bölüt sınırları ve kayıt biçimi doğru; Python yüklemleri C ile 220 kitap × 406 mercekte uyuşuyor.
- **Bulgu 1 (yüksek):** Mallows örnekleyicisinde iş parçacığı yarışı. §4.5.
- **Bulgu 2:** yeniden sayım betiği bellek partilerinden (`d*_1`, …) yalnız ilkini okur. Bu koşularda hiç parti bölünmesi olmadı: `_1` dizini yok ve dilli sayıları dil dosyalarıyla tutuyor. Etkisiz.
- **Bulgu 3:** eksik bölütte betik N'yi yine sayar. Özgün sayımda tetiklenmedi. Düzeltilmiş sayımda bir kez tetiklendi (§4.5) ve eksik düzey sayılarak giderildi.
- **Küçük bulgular:** `bagimsiz_sina_kapi.py` yalnız donmuş Kur'an dilleri için kullanılabilir; burada yalnız onlar için kullanıldı. `kuran_kapi.sh` aynı adlı eski ikiliyi yeniden kullanabilir. Bir yorum eskimiş. Aralıklar Clopper–Pearson değil Poisson-Garwood; bu p'lerde eşdeğer.

### 4.5 Mallows yarışı ve düzeltilmiş yeniden sayım
`kutuphane.h`'de Mallows kurulumu (`orn_init`, 'L') küresel bir referans dizisini her çağrıda yeniden yazar ve sıralar. Sayaçlar bu kurulumu iş parçacıklarından kilitsiz çağırıyordu. İki iş parçacığı aynı anda kurarsa bir bölütün referansı ya da q'su bozulur. O bölütün 250.000 kitabı yanlış dağılımdan gelir, kimi permütasyon bile değildir. M20 ve tekdüze etkilenmez.

Düzeltme: kurulum mevcut kilidin altına alındı (tek satır; tek iş parçacıklı çıktı değişmez). Bütün Mallows sayımları düzeltilmiş sayaçla yeniden yapıldı ve özgünlerle bölüt bölüt karşılaştırıldı. Düzeltilmiş 12 iş parçacıklı çıktı, tek iş parçacıklı sayımla birebir aynıdır.

| Koşu | farklı bölüt | etkisi |
|---|---|---|
| Kur'an tarafı Gc 10¹⁰ | 13 / 40.000 | k değişmedi (22) |
| Kur'an tarafı GE 10⁹ | 14 / 4.000 | k 3 → 2 (bozuk bir kitap 31,6 bitle isabet sayılmıştı) |
| Kur'an tarafı G0 10⁹ | 3 / 4.000 | k değişmedi (9) |
| Plasebo Gc / GE / G0, Mallows | 3 / 5 / 1 (400'er bölüt) | R ve p_kal değişmedi |
| plasebo14, Mallows | 1 / 800 | değişmedi |
| plasebo12 (13. denetim), Mallows | 0 / 400 | değişmedi |

GE'de bir plasebo dilinin (A012) düzey 1 sayımı 11'den 9'a indi, bu yüzden derinlik kuralı onu düzey 2'ye gönderir. Düzey 2 düzeltilmiş sayaçla sayıldı: k(1–40) = 73, p = 7,4×10⁻⁶. Ayrıntı ve bozuk bölütlerin listesi: `ek/EK_20261029T1902Z_mallows_yarisi.md`, `ek/EK_20261029T2340Z_mallows_yeniden_sayim_sonucu.md`, `sonuc/yaris/`.

## 5. Okuma

Ön-kayıtlı kural (§5) uygulanır. Birincil kapı Gc'de iki sıralı sıfırda da R = 0: Kur'an'ın bütünlüklü ayrışması, kendi bütünlüklü dilini kuran bütün plasebo kitapları aşar. Bu, plasebo14'ün sonucunun kapı G'nin Kur'an'ın sınırına denk gelmesinden kaynaklanmadığını gösterir. Sonuç GE'de ve hiç kapı olmadan (G0) da aynıdır. Tekdüzede her kapıda "arada"dır. Yetişen plasebo dizilişe bağlı olmayan, yozlaşmış bir p = 3 merceğidir, G0'da buna boş hücreli bir aile eklenir.

README'ye önerilen cümle:
> Kur'an kendi bütünlüklü dilini kuran 200 sıralı plasebo kitabın hepsini aşar (kalibre p 1/201 M20, 1/101 Mallows; tekdüze 2/101). Kural Kur'an'ın biçimi bilinerek yazıldı ve ilk sürümünde bir kapı Kur'an'ın tam sınırındaydı; o kapı Kur'an'dan bağımsız kurallarla değiştirildiğinde ya da tamamen kaldırıldığında sonuç aynı kalır. Kur'an'ın kendi sırası (Gc, 10¹⁰ kontrol): M20 4,7×10⁻⁹, Mallows 2,2×10⁻⁹, tekdüze 1,2×10⁻⁹.

## 6. Kaynaklar

| Yol | İçerik |
|---|---|
| `ONKAYIT.md`, `sonuc/onkayit_ozet.txt`, `sonuc/kod_sha256.txt` | ön-kayıt ve donmuş kod özetleri |
| `src/`, `kos/kapi_akis.sh`, `kos/kontrol_b.py`, `kos/kuran_kapi.sh` | kod ve akışlar |
| `sonuc/<kapı>/tab/<S>/` | diller, ayrıntılar, kontrol bölüt sayımları (metin) |
| `sonuc/<kapı>/sonuc/kosu_gunlugu.txt`, `diller_sha256.txt` | koşu günlükleri, dil dosyalarının özetleri |
| `sonuc/<kapı>/yeniden_sayim.txt` | bağımsız yeniden sayım (düzeltilmiş Mallows bölütleriyle; düzeltme öncesi `yeniden_sayim_yaris_oncesi.txt`) |
| `sonuc/yaris/` | Mallows yarışı: düzeltilmiş sayaç kodu, yeniden koşu betikleri ve günlüğü, bölüt bölüt farklar |
| `sonuc/plasebo_dil_sinamasi.txt` | dil sınaması, 36 hedef |
| `sonuc/kuran/` | Kur'an tarafı: 10⁹ betimsel (Gc, GE, G0), Gc 10¹⁰ isabetleri, sınamalar, sonradan analizler |
| `dogrulama/` | yeniden sayım, dil ve isabet sınayıcıları, 19-öncel ve yapı karşılaştırma betikleri |
| `ek/` | sapmalar ve sonradan analizler |
