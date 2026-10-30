# PLASEBO14 RAPORU — bütünlük plasebosu

**Soru.** Her hedef kendi en iyi **bütünlüklü** dilini kursa, yani kendi en iyi merceğinde en az üç düzeye ve üç motif türüne yayılan
bir yapı seçmek zorunda olsa, Kur'an'ın bütünlüklü ayrışması rastgele kitaplarınkine göre ne kadar sıra dışıdır?

| | |
|---|---|
| Görev | `GOREV.md` (SHA-256 `f4224b6c…5b13`, `gorev_sha256.txt` ile doğrulandı) |
| Ön-kayıt | `ONKAYIT.md`, donduruldu **2026-10-29T04:38:14Z**, SHA-256 `6080c6b17c4bf38184c11482b3931d4804633ee23d190d69d252e03926e1f748` |
| Diller | ön-kayıttan sonra, bütün kontrol geçişlerinden önce donduruldu (04:38:39Z; `sonuc/diller_sha256.txt`, 16 dosya) |
| Hesap | kayıtlı 18,10 çekirdek-saat (tavan 40); en çok 4 iş parçacığı; en büyük süreç 1,87 GB; proje diski 14 MB |
| Sapmalar | iki ek (`ek/`): bir betimsel sayımın düzeltilmesi ve dondurmadan sonra eklenen raporlama/sınama kodu (§12) |
| Yeniden üretim | oturum sonu yeniden derleme sınaması 16 / 16 bit-bit aynı (`sonuc/yeniden_sina_rapor.txt`) |

**Gösterim.** p = (k + 1)/(N + 1): k, puanı hedefinkine eşit ya da büyük olan kontrol sayısı (eşitlik hedefin aleyhine), N kontrol
sayısı; köşeli ayraç iki yanlı %95 Clopper–Pearson aralığıdır (k = 0'da alt uç 0). K: plasebo sayısı; K_d: bütünlüklü dili olan plasebo
sayısı; R: p'si Kur'an'ınkinden büyük olmayan ya da aralığı Kur'an'ınkiyle çakışan plasebo sayısı; **p_kal = (1 + R)/(K + 1)**. Sıfırlar:
**M20** (birincil, K = 200), **tekdüze** ve **Mallows** (K = 100). Kontrol kitapları her sıfırda plasebo12'ninkilerle aynıdır (aynı tohum
ve kod; Ö0, §3). plasebo12'nin sonucu ile bu testin sonucu yan yana yazılır; **ikincisi birincinin yerine geçmez.**

---

## 0. Kısa cevaplar

### 0.1 Kendi bütünlüklü dilini kuran rastgele kitaplar ne kadar ayrışıyor? Kur'an nerede, kalibre p nedir?

Kol A (birincil: ≥ 3 tür, ≥ 3 düzey; dil bedeli plasebo12'ninki):

| Sıfır (K) | bütünlüklü dili olan plasebo | plasebo p medyanı (dilsiz = 1) | dilli plasebo p medyanı | en küçük plasebo p (k / N) [%95] | Kur'an p_Q (k / N) [%95] | R | p_kal | okuma |
|---|---|---|---|---|---|---|---|---|
| M20 (200) | 35 | 1 | 7,1×10⁻⁴ | 1,1×10⁻⁶ (A195; 10 / 10⁷) [4,8×10⁻⁷; 1,8×10⁻⁶] | 1,0×10⁻⁸ (0 / 10⁸) [0; 3,7×10⁻⁸] | 0 | **1/201 = 0,0050** | **aşar** |
| Tekdüze (100) | 18 | 1 | 5,9×10⁻⁴ | 4,1×10⁻⁶ (A018 ve A045; 40 / 10⁷) [2,9×10⁻⁶; 5,4×10⁻⁶] | 1,0×10⁻⁸ (0 / 10⁸) [0; 3,7×10⁻⁸] | 0 | **1/101 = 0,0099** | **aşar** |
| Mallows (100) | 15 | 1 | 1,7×10⁻⁴ | 3,4×10⁻⁷ (A047; 33 / 10⁸) [2,3×10⁻⁷; 4,6×10⁻⁷] | 1,0×10⁻⁸ (0 / 10⁸) [0; 3,7×10⁻⁸] | 0 | **1/101 = 0,0099** | **aşar** |

Kur'an'ın Kol A dili üç sıfırda da **{B2, K1, Qc, WSum} @ (1,+1,19)**'dur. 10⁸ kontrolün hiçbiri onun bütünlüklü puanına (eşik
18,80 / 20,83 / 20,00 bit) ulaşmaz; eşiğe 2 bitten yakın gelen kontrol de yoktur. Üç sıfırda da **R = 0**: hiçbir plasebonun p'si
Kur'an'ınkinden küçük ya da ona eşit değildir ve hiçbirinin aralığı Kur'an'ınkiyle çakışmaz. Okuma kuralının ilk cümlesi uygulanır (§10):
**Kur'an'ın bütünlüklü ayrışması bütün plasebo kitaplarınkini aşar.** Kalibre p tasarımın verebileceği en küçük değerdedir, 1/(K + 1).
Kur'an'ın p'si üç sıfırda da çözünürlük sınırındadır (k = 0).

Model-tabanlı fark (etiketli; kararlarda kullanılmaz): betimsel fark Δ ≥ 2,04 (M20), 2,61 (tekdüze), 1,53 (Mallows) mertebe. Model P'leri
modele çok bağlıdır. Dilli plasebolara koşullu karışımda normal model 9,2×10⁻⁸ / 1,3×10⁻⁶ / 2,9×10⁻⁴ verir; GPD M20'de 0
[önyükleme %95: 0; 2,0×10⁻⁴] verir, öteki iki sıfırda tanımsızdır (§5).

Ayrışmanın büyük kısmı, plasebo kitapların bütünlüklü dil kuramamasından gelir: %82,5'i, %82'si ve %85'i p = 1 alır. Kurabilenlerin
en yakını da Kur'an'a ulaşmaz. Sonuç **bütünlük kuralının içinde** geçerlidir. Kural Kur'an'ın biçimi bilinerek yazıldı; Kur'an kapı G'yi
tam sınırda sağlar (k_F·p = 12·19 = 228; 406 mercekten yalnız birinde). Kuralın kendisinin seçimi bu kalibrasyonla bedellendirilmedi
(**ölçülmedi**, §11).

### 0.2 Kaç plasebonun bütünlüklü bir dili var, bu diller neye benziyor?

Bütünlüklü dili olan plasebolar M20'de **35 / 200** (%17,5), tekdüzede **18 / 100**, Mallows'ta **15 / 100**'dür. Duyarlılık kollarında
M20'de A−'de 60 / 200, A+'da 2 / 200. Dilsiz plasebolarda çoğunlukla hiçbir mercekte kapı G tutmaz; kapı G, k_F·p ≥ 228 ve dört F
hücresinin her birinin ≥ 2 üyeli olmasıdır. Bu durum M20'de 165 dilsizin 138'inde, tekdüzede 82'nin 73'ünde, Mallows'ta 85'in 72'sinde
görülür.

Plasebo dilleri kısadır: 3 yüklem; yalnız M20'de 4 ve Mallows'ta 4 dil 4 yüklemlidir. Hepsi bütünlük şartının alt sınırındadır: tam
üç tür, tam üç düzey. Kur'an'ın dili ise dört türe yayılır. Tipik plasebo dili şu parçalardan oluşur:
- iki elin ayrıklığı (W1; M20'deki 35 dilin 24'ünde) ya da iki elin tek ortak üyesi (WI1),
- hücre boylarının ardışıklığı (FKS) ya da sabitliği (FSC),
- bir aile ya da hücre dengesi.

En iyi plasebolar p = 11–29 merceklerindedir (§4.2, §8.2). Plasebo eşikleri (bütünlüklü puan) −7,6 ile +14,6 bit arasındadır; Kur'an'ınki
18,8–20,8 bittir.

### 0.3 Bedelsiz en geniş dilde (Kol B) tablo nedir?

| Sıfır (K) | Kur'an eşiği (bit) | Kur'an p_Q (k / N) [%95] | plasebo p medyanı [%95] | p < 10⁻⁶ / < 10⁻⁷ | Kur'an'ın aralığına giren plasebolar (k / 10⁸) | R | p_kal | sınıf |
|---|---|---|---|---|---|---|---|---|
| M20 (200) | 31,784 | 1,0×10⁻⁸ (0 / 10⁸) [0; 3,7×10⁻⁸] | 8,4×10⁻⁵ [5,2×10⁻⁵; 1,15×10⁻⁴] | 8 / 2 | B183 (1)\*, B087 (7) | 2 | **3/201 = 0,0149** | arada |
| Tekdüze (100) | 35,814 | 1,0×10⁻⁸ (0 / 10⁸) [0; 3,7×10⁻⁸] | 9,9×10⁻⁵ [4,3×10⁻⁵; 2,3×10⁻⁴] | 8 / 2 | B080 (1), B068 (4)\* | 2 | **3/101 = 0,0297** | arada |
| Mallows (100) | 31,769 | 1,0×10⁻⁸ (0 / 10⁸) [0; 3,7×10⁻⁸] | 1,8×10⁻⁴ [9,5×10⁻⁵; 3,9×10⁻⁴] | 4 / 2 | B090 (0; eşit), B061 (2)\* | 2 | **3/101 = 0,0297** | arada |

\* Puanı, en iyi merceğindeki çözülmemiş bir aramaya (c < 10) dayanır; değeri ön-kayıtlı tek kuralla hesaplanmıştır (§7).

Bütün plasebolar dillidir (5–21 yüklem). Kur'an'ın en geniş dili üç sıfırda da aynıdır: 15 yüklem @ (1,−1,19). Hiçbir sıfırda, 10⁸
kontrolden hiçbiri Kur'an'ın eşiğine 2 bitten yakın gelmez. Ama her sıfırda iki plasebonun kendi tarifi Kur'an'ınkinin aralığına girer:
R = 2. Sınıf "arada"dır; bu sınıf betimseldir, çünkü okuma kuralı Kol A için işletilir. Her çiftin birinin puanı çözülmemiş bir aramaya
dayanır. Öteki (B087, B080, B090) çözülmüş aramalarla Kur'an'ın aralığına girer; Mallows'ta B090 Kur'an'la aynı çözünürlük sınırındadır
(0 / 10⁸).

Okuma: her kitap sahip olduğu her şeyle tarif edildiğinde Kur'an'ın tarifi en seyreklerdendir; plasebolardan 198 / 200, 98 / 100 ve
98 / 100'ünü aşar, ama hepsini aşmaz. Kol A'daki "aşar" sonucu bütünlük şartıyla gelir.

### 0.4 Duyarlılık kolları sonucu değiştiriyor mu?

**Hayır.** M20'de iki duyarlılık kolu da aynı sonucu verir: R = 0, p_kal = 1/201, sınıf "aşar".

| Kol | Kur'an'ın dili | eşik | k_Q / N | dilli plasebo | en küçük plasebo p |
|---|---|---|---|---|---|
| A− (≥ 2 tür, ≥ 2 düzey) | {B2, K1, Qc}; plasebo12'nin diliyle aynı | 15,947 bit | 0 / 10⁸ | 60 | 4,7×10⁻⁵ (Am012; 46 / 10⁶) |
| A+ (4 tür, 4 düzey) | {B2, K1, Qh, WSum} | 18,801 bit | 0 / 10⁸ | 2 | 2,6×10⁻⁴ (Ap120; 255 / 10⁶) |

Kol B (bedelsiz en geniş dil, bütünlük kuralı yok) bir duyarlılık kolu değil, ayrı bir sorudur. O kolda sınıf "arada"dır (0.3).

### 0.5 Ana hattın tahmini tuttu mu?

**Kısmen tuttu** (ön-kayıtlı karar kurallarıyla, Kol A, M20; §9):

| Tahmin parçası | Hüküm | Gerekçe |
|---|---|---|
| p_Q ~10⁻⁷ (10⁻⁹–10⁻⁶) | karar verilemedi | k = 0 / 10⁸; nokta 1,0×10⁻⁸ aralığın içinde, ama tek yanlı %95 üst sınır 3,0×10⁻⁸ ≥ 10⁻⁹ |
| plasebolarının en az yarısında bütünlüklü dil | tutmadı | 35 / 200 |
| plasebo p medyanı 10⁻³–10⁻¹ | tutmadı | 1, çünkü dilsizler p = 1 alır; dilli 35 plasebonun medyanı 7,1×10⁻⁴ |
| en küçük plasebo p 10⁻⁶–10⁻⁴ | tuttu | 1,1×10⁻⁶ [4,8×10⁻⁷; 1,8×10⁻⁶] |
| hepsini aşar (p_kal = 1/201) | tuttu | R = 0 |

### 0.6 Ölçülemeyenler

Ayrıntılar §11'dedir. Şunlar **ölçülmedi**:
- kütüphanenin ve bütünlük kuralının sınırları; kural Kur'an'ın biçimi bilinerek yazıldı;
- literatürün araması;
- nesnenin (n, a) seçimi;
- sayım okulu;
- gruplar arası bağımsızlık yaklaşımı;
- kültürel havuz;
- referans tablosu gürültüsünün p_x'e etkisi;
- dil bedelinin biçimi.

---

## 1. Ön-kayıt, tahmin ve okuma kuralı

### 1.1 Dondurma ve dondurmadan önce yapılanlar
`ONKAYIT.md` **2026-10-29T04:38:14Z**'de donduruldu ve salt okunur yapıldı. SHA-256'sı
`6080c6b17c4bf38184c11482b3931d4804633ee23d190d69d252e03926e1f748`'dir (`sonuc/onkayit_ozet.txt`). Dondurma anındaki 49 kod dosyasının ve
`GOREV.md`'nin özetleri `sonuc/onkayit_kod_sha256.txt`'tedir.

Ön-kayıt şunları sabitledi: bütünlük kuralı, D_B'nin tam arama yöntemi, kollar, hedefler, bütçeler, derinlik kuralı, tohumlar, ayrışma
istatistiği, çözülmemiş aramaların kuralı, model-tabanlı fark modeli ve karar kuralları.

Dondurmadan önce yalnız görevin izin verdikleri yapıldı (ONKAYIT §0):
- plasebo12 ile eşitlik sınamaları (Ö0, §3);
- hedef olmayan kitaplarda kod doğrulamaları (yalnız uyuşmazlık sayıları);
- hız ve bellek ölçümü.

Hiçbir hedef, Kur'an dahil, bu görevin dilleriyle değerlendirilmedi; hiçbir kontrol bu görevin dilleriyle puanlanmadı. Diller ön-kayıttan
sonra ve bütün kontrol geçişlerinden önce kuruldu ve donduruldu (04:38:39Z; `sonuc/diller_sha256.txt`, 16 dosya).

**Açıklama (ONKAYIT §0 e'den).**
1. plasebo12'nin yayımlanmış raporu, Kur'an'ın (1,+1,19)'daki 21 yüklemlik profilini ve kapı G'nin orada tuttuğunu yayımlar. Buradan iki
   şeyi önceden çıkardım ve ön-kayda yazdım: Kur'an'ın üç kolda da bütünlüklü dili olacak; A−'de D_B(Kur'an) plasebo12'nin diline eşit
   olacak. İkisi de öyle çıktı.
2. Dondurma öncesi doğrulama çıktıları, istenmeden, hedef olmayan rastgele kitapların çoğunun bütünlüklü dili olmadığını kabaca gösterdi:
   mekanik sınamada 90 kitap-kolun M'de 12'si, U'da 19'u, L'de 17'si dilliydi. Bu, tahminin "en az yarısı" parçasının tutmayacağını önceden
   düşündürüyordu. Hiçbir tanım bundan etkilenmedi: tanımlar görevde sabittir ve bütçe kuralları en kötü duruma göre yazıldı.
3. plasebo12'nin hedef başına ayrıntı dosyaları dondurmadan önce açılmadı.

### 1.2 Ana hattın tahmini (ön-kayda aynen yazıldı)
Birincil kol A, birincil sıfır M20:
- Kur'an'ın bütünlüklü dilindeki bütünlük şartlı sırası p_Q için merkez **10⁻⁷**, aralık **10⁻⁹–10⁻⁶**.
- 200 plasebonun en az yarısında bütünlüklü bir dil bulunur.
- Plasebo p_x medyanı **10⁻³–10⁻¹**, en küçük p_x **10⁻⁶–10⁻⁴**.
- Kur'an'ın ayrışması 200 plasebonun **hepsini aşar** (kalibre p = 1/201).

### 1.3 Okuma kuralı (ön-kayda aynen yazıldı)
> Kur'an'ın bütünlüklü ayrışması bütün plasebo kitaplarınkini aşarsa, nesnenin bütünlüğünün sırası dilin Kur'an'a göre yazılmasıyla açıklanamaz; kalibre p en küçük ~1/(K + 1) olabilir (K, plasebo kitap sayısı) ve aradaki fark ayrıca, model-tabanlı olarak verilir. Plasebo kitaplar Kur'an'ın bütünlüklü ayrışmasına sık ulaşırsa (R/K ≥ 0,05), bütünlüğün nadirliği büyük ölçüde dilin Kur'an'a göre yazılmasından gelir ve öyle yazılır. Arada kalan sonuçlar kalibre p'leriyle aynen yazılır.

İşletimi (ONKAYIT §5.5):
- R = 0 → ilk cümle;
- R/K ≥ 0,05 → ikinci cümle;
- arada kalan her sonuç p_kal ile aynen yazılır.

Okuma kuralı Kol A için işletilir. A−, A+ ve Kol B'de aynı sınıflar betimsel olarak yazılır.

---

## 2. Yöntem (özet; tam tanımlar `ONKAYIT.md`'de)

### 2.1 Değişmeyenler
Şunlar plasebo12'deki gibidir:
- kütüphane L\* (109 yüklem), yüklem ve dil bedelleri;
- 406 mercek ve mercek bedeli;
- bilgi puanı ve bağlamları;
- çözülmemiş aramaların tek kuralı;
- referans tabloları: sıfır başına 10⁸ kitap; plasebo12'nin tabloları, SHA-256 doğrulandı, yerinde okundu;
- hedefler: plasebo12'nin `hedef.bin`'leri; Kur'an + 200 M20, + 100 tekdüze, + 100 Mallows;
- kontrol akışı: tohum(sıfır, 2, b), b = 1..400; kontrol kitapları plasebo12'ninkilerle aynı.

**Değişen yalnız bütünlük kısıtıdır.**

### 2.2 Bütünlük kuralı ve kollar
Bir kitap bir mercekte, dil Λ altında, şu üç koşul tutarsa bütünlüklüdür (plasebo12 ön-kaydının §9 c kuralı; tür ve düzey etiketleri
plasebo12'nin §2.2–2.3'ü):
1. kapı G tutar: k_F·p ≥ 228 ve dört F hücresinin her biri ≥ 2 üyeli;
2. Λ'nın orada sağlanan yüklemleri ≥ t_min motif türünden gelir;
3. aynı yüklemler ≥ d_min düzeyden gelir.

Bütünlüklü puan, yalnız bu kuralın tuttuğu mercekler üzerinden alınan en büyük I_Λ(λ) − c(λ)'dır. Kural hiçbir mercekte tutmuyorsa kitap
puansızdır: hedefte p = 1, kontrolde "aşmaz".

| Kol | t_min / d_min | dil bedeli | sıfırlar |
|---|---|---|---|
| A (birincil) | 3 / 3 | plasebo12'nin c(Λ)'sı | M20, tekdüze, Mallows |
| A− (duyarlılık) | 2 / 2 | c(Λ) | M20 |
| A+ (duyarlılık) | 4 / 4 | c(Λ) | M20 |
| B (ikincil) | bütünlük yok | 0 (en geniş dil) | M20, tekdüze, Mallows (bütçe kuralıyla; üçü de koştu) |

### 2.3 D_B ve en geniş dil
D_B(x), |Λ| ≤ 20 olan ve x'in kendi en iyi merceğinde bütünlük kuralını sağlayan diller arasından, bütünlüklü puan − dil bedelini en büyük
yapan dili seçer. Amaç negatif olsa bile seçilir; böyle bir dil yoksa hedef dilsizdir (p = 1).

Arama tamdır:
1. Kapı G'nin tuttuğu her mercekte, her grubun sağlanan yüklemlerinin bütün alt kümeleri taranır.
2. Her (boy, düzey ∪ tür imzası) kovası için en iyisi tutulur.
3. Gruplar (toplam boy, birleşik imza) durumları üzerinde dinamik programlamayla birleştirilir.

Eşitlik bozma plasebo12'ninkidir. Tek yaklaşık adım plasebo12'nin grup başına 24 aday sınırıdır; hiçbir hedefte devreye girmedi (§8.4).
Kol B'de dil bedeli sıfırdır: dil, hedefin bilgi puanını en büyük yapan mercekte sağladığı bütün L\* yüklemleridir (boy sınırı yok).

### 2.4 Ayrışma ve kalibrasyon
- p_x = (k_x + 1)/(N_x + 1); k_x, puanı hedefinkine eşit ya da büyük olan kontrol sayısıdır.
- Derinlik 10⁶ → 10⁷ → 10⁸; k < 10 ise derinleşir. Kur'an doğrudan 10⁸.
- Kalibre p = (1 + R)/(K + 1). R, p_x ≤ p_Q olan ya da %95 Clopper–Pearson aralığı Kur'an'ınkiyle çakışan plasebolardır.
- Model-tabanlı fark plasebo12'nin modeliyle verilir (GPD üst %20, normal, Δ). Dilsiz plasebolar için önceden yazılmış koşullu karışım
  da verilir. İkisi de kararlarda kullanılmaz.

---

## 3. Ö0 — çapalar

| Çapa | Sonuç |
|---|---|
| Referans tabloları ve hedef dosyaları (plasebo12'de yerinde) | SHA-256 12/12 ve 3/3 OK (oturum sonunda yeniden: plasebo12'nin listesindeki 20/20 ve 3/3 OK); hedefler bu klasöre kopyalandı, kopyalar 3/3 OK |
| plasebo12 kaynaklarının kopyaları | 30 dosya özgünlerle aynı; 15'i plasebo12'nin dondurma anı özetleriyle de aynı (`sonuc/p12_kopya_sha256.txt`) |
| Kısıtsız D (`src/dil_b` P kipi) → plasebo12'nin `diller.txt` ve `ayrinti.txt`'si | M 203, U 103, L 103 satır: **bit-bit aynı** (SHA-256 `a55236f9…`, `61c82759…`, `ec36dd60…`) |
| Kur'an'ın plasebo12 eşikleri (Q, QP2, el dili) ve bütün eşikler | üç sıfırda `esikler.txt` **bit-bit aynı** |
| Bir kontrol bölütünün sayımları (bölüt 1, plasebo12'nin düzey-1 dilleri) | `b0001.txt` (M 403, U 203, L 203 satır) ve `b0001.q` üç sıfırda **bit-bit aynı** |
| Oturum sonu yeniden derleme sınaması (`kos/yeniden_sina.sh`; `kos/derle.sh` ile geçici dizine yeniden derlenmiş ikililer) | **16 / 16 bit-bit aynı**: Ö0 kısıtsız D (3), Ö0 kontrol bölüt 1 (3), bu oturumun bütün dil ve ayrıntı dosyaları (8 kol-sıfır), M20 Kol A ve Kol B kümelerinin düzey-1 bölüt 1'i (`sonuc/yeniden_sina_rapor.txt`) |

Şunlar plasebo12'dekiyle aynıdır: kütüphane L\*, yüklem bedelleri, bilgi puanı, kısıtsız D ve kontrol akışı (aynı tohumlar, aynı kitaplar).

**Dondurma öncesi kod doğrulamaları** (yalnız hedef olmayan kitaplar; `sonuc/dogrulama/`):
- Kaba-kuvvet D_B ve en geniş dil, üç sıfırda 2.000'er kitap ve üç kol: seçilen dilde (liste ve mercek dahil) 0 uyuşmazlık.
- Kontrol programının bağımsız yeniden sayımı (16.000 tür-8 kontrol): k, kB ve eşiklerde 0 uyuşmazlık.

---

## 4. Diller

### 4.1 Kur'an'ın dilleri
D_B, Kur'an'a üç sıfırda da aynı Kol A dilini verir: **{B2, K1, Qc, WSum} @ (1,+1,19)**. Kur'an'ın 406 mercek içinde kapı G'nin tuttuğu
tek merceği budur: k_F = 12, 12·19 = 228; hücreler 3/3/3/3; en küçük hücre 3.

Dil dört türden gelir: a (B2), d (K1), b (Qc), c (WSum). Üç düzeyden gelir: kitap (B2), hücre (K1, Qc), birleşim (WSum). WSum,
k_F + k_H = p'dir: 12 + 7 = 19, yani iki el ayrıktır ve birleşimleri 19 sûredir.

| Bileşen (M20) | Arama | Bit |
|---|---|---|
| kitap: B2, (1,+1) biçiminde | 123.118 / 10⁸ | 9,666 |
| aile-içi \| k_F = 12: K1 ∧ Qc | 97 / 1.203.170 | 13,599 |
| birleşim \| k_F = 12: WSum | 166.353 / 1.203.170 | 2,854 |
| toplam bilgi | | 26,119 |
| dil bedeli: log₂ 21 + 4,585 (B2) + 6,170 (K1) + 6,585 (Qc) + 5,000 (WSum) − log₂ 4! | | 22,147 |
| mercek bedeli (P0) | | 7,317 |
| amaç | 26,119 − 22,147 − 7,317 | −3,346 |
| **eşik** S^B_Q(Λ_Q) = bilgi − mercek bedeli | | **18,801** |

Öteki iki sıfırın eşikleri:
- tekdüze: **20,830 bit** (B 112.329 / 10⁸; F 22 / 1.014.992; W 140.212 / 1.014.992);
- Mallows: **19,998 bit** (B 122.713 / 10⁸; F 48 / 1.307.394; W 173.763 / 1.307.394).

Bütün aramalar çözülmüştür (c ≥ 10).

**plasebo12 diliyle farkı.** plasebo12'nin dili {B2, K1, Qc}'nin üç yüklemi aynen kalır; bütünlük şartı **WSum**'u ekler. plasebo12'de
WSum'un 2,85 bitlik bilgisi 5 bitlik bedelini karşılamıyordu. Bütünlük üçüncü bir düzey istediği için bu görevde dile girer; Kur'an'ın
üçüncü düzeyi için en ucuz yüklem budur.

Duyarlılık kollarında:
- **A−**: {B2, K1, Qc}. Bu, plasebo12'nin diliyle aynıdır (ön-kayıtta önceden açıklanmıştı); eşik 15,947 bit, plasebo12'nin eşiğine eşit.
- **A+**: {B2, K1, Qh, WSum}. Dördüncü düzey için aile düzeyli Qh girer, yani 19 iki yarı toplamını da böler (38 ve 38). Bilgisi Kol A
  ile aynıdır, eşik 18,801 bit; bedeli 1,8 bit fazladır.

Kur'an'ın Kol B dili §7.1'dedir.

**Köken notu.** Kol A dilinin dört yükleminin Kur'an'daki içeriği:
- B2: 1994'te yayımlanmış parite terazisi.
- Qc (Σq = 4·19 = 76): 1990'ın "katsayılar toplamı 76, aile toplamı 1444"ü.
- K1: hücre toplamları 20/18/19/19. İki 19'u (2×361) en geç 2015'te yayımlanmıştı; 18 ve 20 bu çalışmanındır.
- WSum (12 + 7 = 19): bu çalışmanın "İki El" katmanıdır (README'nin *Köken* bölümü).

D_B bunları literatürü bilmeden, yalnız bilgi − bedel ölçütüyle ve bütünlük şartıyla seçti. Bu kalibrasyon literatürün aramasını
bedellendirmez.

### 4.2 Plasebo dilleri (Kol A)

| | M20 (K = 200) | Tekdüze (K = 100) | Mallows (K = 100) |
|---|---|---|---|
| bütünlüklü dili olan plasebo (K_d) | **35 (%17,5)** | **18 (%18)** | **15 (%15)** |
| dilsiz | 165 | 82 | 85 |
| dilsizlerden hiç kapı-G merceği olmayan | 138 | 73 | 72 |
| \|Λ\| dağılımı | 3: 31, 4: 4 | 3: 18 | 3: 11, 4: 4 |
| tür / düzey sayısı | hepsi 3 / 3 | hepsi 3 / 3 | hepsi 3 / 3 |
| tür kullanımı (a, b, c, d) | 33, 9, 31, 32 | 16, 6, 14, 18 | 15, 9, 11, 10 |
| düzey kullanımı (kitap, aile, hücre, birleşim) | 7, 31, 34, 33 | 2, 16, 18, 18 | 2, 14, 14, 15 |
| en sık yüklemler | W1 24, FKS 16, B1 7, WI1 5 | W1 8, FKS 7, FSC 6, F1 5 | W1 6, FD1 5, WI1 5, FSC 5 |
| eşik S^B_x: aralık (Kur'an: 18,80 / 20,83 / 20,00) | −7,6 … +6,9 bit | −5,2 … +5,2 bit | −6,8 … +14,6 bit |
| aday sınırı (24) devreye giren | 0 | 0 | 0 |
| en iyi mercekte çözülmemiş arama | 1 (A120) | 0 | 0 |

Plasebo dillerinin hepsi tam üç türe ve üç düzeye yayılır; Kur'an'ınki dört türe yayılır. Tipik plasebo dili şunlardan oluşur: iki elin
ayrıklığı (W1) ya da tek ortak üyesi (WI1), hücre boylarının ardışıklığı (FKS) ya da sabit hücre (FSC), ve bir aile ya da hücre dengesi.

Dilsizliğin ana sebebi kapı G'dir. Dilsiz plasebolarda çoğunlukla hiçbir mercekte iki şart birlikte tutmaz: k_F·p ≥ 228 ve dört hücrenin
her biri ≥ 2.

---

## 5. Sonuçlar — Kol A (birincil)

### 5.1 M20 (birincil)
**Kur'an.**
- Eşik 18,801 bit.
- 10⁸ kontrolde (bölüt 1–400) **k = 0** → p_Q = 1/(10⁸ + 1) = **1,0×10⁻⁸ [0; 3,7×10⁻⁸]**.
- Eşiğin 2 bit yakınına gelen kontrol yoktur (aday dosyalarında 0 kayıt).
- Kur'an'ın dili altında en az bir mercekte bütünlüklü olabilen kontrol 8 / 10⁸'dir; bunların hiçbiri eşiğe 2 bitten yakın değildir.

**Plasebolar.** 35 dilli plasebodan 32'si 10⁶'da, 3'ü 10⁷'de durdu; 10⁸'e inen olmadı.

| İstatistik | Değer |
|---|---|
| p_x medyanı (200 plasebo; dilsiz = 1) | **1** [%95: p_(86) – p_(115) = 1; 1] |
| dilli 35 plasebonun p_x medyanı | 7,1×10⁻⁴ |
| p_x < 10⁻² / 10⁻³ / 10⁻⁴ / 10⁻⁵ / 10⁻⁶ | 31 / 20 / 4 / 1 / 0 |
| en küçük p_x | **1,1×10⁻⁶**: A195 {FAd, W1, HR0e, HR1e} @ (1,+3,19); 10 / 10⁷; [4,8×10⁻⁷; 1,8×10⁻⁶] |
| sonraki dört | 1,46×10⁻⁵ (A159, 145 / 10⁷), 1,50×10⁻⁵ (A146, 149 / 10⁷), 1,9×10⁻⁵ (A012, 18 / 10⁶), 1,05×10⁻⁴ (A062, 104 / 10⁶) |

**Kalibre p.** R = 0. En yakın plasebonun aralığı [4,8×10⁻⁷; 1,8×10⁻⁶], Kur'an'ınki [0; 3,7×10⁻⁸]; çakışma yoktur.
→ **p_kal = 1/201 = 4,98×10⁻³**. Okuma: **"Kur'an'ın bütünlüklü ayrışması bütün plasebo kitaplarınkini aşar"**; ilk cümle uygulanır (§10).

**Model-tabanlı fark** (önceden yazılmış; kararlarda kullanılmaz).
- y_Q = 8,00 (çözünürlük sınırında, k = 0); en uç plasebo y = 5,96. Betimsel fark **Δ ≥ 2,04 mertebe**: p_Q'nun gerçek değeri 10⁻⁸'in
  altında olabilir, Δ bu yüzden bir alt sınırdır.
- (a) plasebo12'nin modeli: üst %20 kümesinde (40 plasebo) dilsiz plasebolar bulunduğundan GPD **tanımsızdır**. Normal model P = 2,3×10⁻⁹
  verir; ama 165 dilsiz plasebonun y = 0 nokta kütlesi bu uyumu bozar ve bu sayı yorumlanmamalıdır.
- (b) Koşullu karışım (dilli 35 plasebo): GPD (üst 7) Kur'an'ı uç noktanın ötesine koyar, P = 0 [önyükleme %95: 0; 2,0×10⁻⁴]; normal
  P = 9,2×10⁻⁸.

Model-tabanlı okuma en çok şunu söyler: Kur'an'ın ayrışması, dilli plasebo dağılımının uzantısında 10⁻⁴'ten seyrek bir yerdedir; sayının
kendisi modele bağlıdır.

### 5.2 Tekdüze
**Kur'an.** Eşik 20,830 bit; **k = 0 / 10⁸** → 1,0×10⁻⁸ [0; 3,7×10⁻⁸]; aday 0; bütünlüklü olabilen kontrol 6 / 10⁸.

**Plasebolar.** 18 dilli; 15'i 10⁶'da, 3'ü 10⁷'de durdu. Dilli medyan 5,9×10⁻⁴; p_x < 10⁻⁴ / 10⁻⁵ / 10⁻⁶: 5 / 2 / 0. En küçük p_x
**4,1×10⁻⁶**, iki plaseboda: A018 {FSp, FKS, WEq} @ (1,+1,29) ve A045 {FD2, FKS, WI1} @ (1,+1,23); ikisi de 40 / 10⁷ [2,9×10⁻⁶; 5,4×10⁻⁶].

**Kalibre p.** R = 0 → **p_kal = 1/101 = 9,90×10⁻³**; okuma: **aşar**. plasebo12'de tekdüzede Kur'an'ın aralığıyla çakışan x068 burada
dilsizdir: tek kapı-G merceğinde üç tür ve üç düzeye yayılan sağlanan küme yoktur.

**Model.** Δ ≥ 2,61 mertebe. (a) GPD tanımsız; normal 5,0×10⁻⁸ (nokta kütlesiyle bozuk). (b) GPD tanımsız (dilli 18'in üst %20'si 4 < 5);
normal 1,3×10⁻⁶.

### 5.3 Mallows
**Kur'an.** Eşik 19,998 bit; **k = 0 / 10⁸** → 1,0×10⁻⁸ [0; 3,7×10⁻⁸]; aday 0; bütünlüklü olabilen kontrol 10 / 10⁸.

**Plasebolar.** 15 dilli; 11'i 10⁶'da, 2'si 10⁷'de, 2'si 10⁸'de durdu. Dilli medyan 1,7×10⁻⁴; p_x < 10⁻⁴ / 10⁻⁵ / 10⁻⁶ / 10⁻⁷:
6 / 4 / 2 / 0.
- En küçük p_x **3,4×10⁻⁷**: A047 {Zk2p, R0c, FKS, WUTd2} @ (1,+3,11); k_F = 22 = 2p, hücreler 5/5/6/6; 33 / 10⁸; [2,3×10⁻⁷; 4,6×10⁻⁷].
- Sonraki: A068 {B1, FCK, Qa, HK2} @ (1,+3,29), 6,8×10⁻⁷ (67 / 10⁸).

**Kalibre p.** A047'nin aralığının alt ucu (2,3×10⁻⁷), Kur'an'ın aralığının üst ucunun (3,7×10⁻⁸) üstündedir; çakışma yoktur. R = 0 →
**p_kal = 1/101 = 9,90×10⁻³**; okuma: **aşar**.

**Model.** Δ ≥ 1,53 mertebe. (a) GPD tanımsız; normal 4,1×10⁻⁷ (bozuk). (b) GPD tanımsız (üst %20'si 3 < 5); normal 2,9×10⁻⁴.

### 5.4 plasebo12 ile yan yana (ikincisi birincinin yerine geçmez)

| Sıfır | plasebo12: Kur'an'ın dili | p_Q (k / 10⁸) | en iyi plasebo | p_kal | plasebo14 Kol A: Kur'an'ın dili | p_Q (k / 10⁸) | en iyi plasebo | p_kal |
|---|---|---|---|---|---|---|---|---|
| M20 | {B2, K1, Qc} | 3,4×10⁻⁷ (33) | 3,0×10⁻⁶ (x183) | 1/201 | {B2, K1, Qc, WSum} | ≤ 3,7×10⁻⁸ (0) | 1,1×10⁻⁶ (A195) | 1/201 |
| Tekdüze | {B2, K1, Qc} | 5,0×10⁻⁸ (4) | 1,0×10⁻⁷ (x068; çakışma) | 2/101 | {B2, K1, Qc, WSum} | ≤ 3,7×10⁻⁸ (0) | 4,1×10⁻⁶ (A018, A045) | 1/101 |
| Mallows | {B2, K1, Qc} | 8,0×10⁻⁸ (7) | 5,8×10⁻⁶ (x046) | 1/101 | {B2, K1, Qc, WSum} | ≤ 3,7×10⁻⁸ (0) | 3,4×10⁻⁷ (A047) | 1/101 |

plasebo14'te p_Q'nun nokta değeri her sıfırda 1,0×10⁻⁸'dir; "≤" %95 üst ucunu gösterir. plasebo12'de plasebo dillerinin ortalama boyu
1,0–1,1 yüklemdi (M20'de 16, tekdüzede 11, Mallows'ta 12 plasebonun dili boştu) ve bütünlük şartlı satır tanımsız kalmıştı. Bu görevde her
hedefe bütünlüklü dil zorla verildi.

**Kitap kitap değişim** (aynı hedefler, aynı kontrol kitapları):

| Sıfır | dilli | dili plasebo12'dekiyle aynı | log₁₀(p^A / p^12) medyanı [çeyrekler] | azalan / artan | Spearman (dilliler) | dilsizlerin p^12 medyanı |
|---|---|---|---|---|---|---|
| M20 | 35 | 0 | −0,73 [−1,49; −0,47] | 31 / 4 | 0,41 | 1,4×10⁻² |
| Tekdüze | 18 | 0 | −1,46 [−2,40; −0,81] | 15 / 3 | −0,02 | 1,1×10⁻² |
| Mallows | 15 | 0 | −1,62 [−2,44; −0,93] | 14 / 1 | 0,22 | 1,4×10⁻² |

Bütünlüklü dili olan hiçbir plasebonun dili plasebo12'deki diliyle aynı değildir. Bu sayı dondurmadan sonra düzeltildi; ek:
`EK_20261029T0905Z_betimsel_duzeltme`.

Bütünlüklü puan hedefe de kontrollere de uygulanır. Kontrollerin çoğu hedefin dilinde bütünlüklü olamadığından, dilli plasebolarda p_x
plasebo12'dekinden tipik olarak 0,7–1,6 mertebe küçülür. Kur'an'ın p'si, nokta değerleriyle, M20'de 1,5, tekdüzede 0,7, Mallows'ta
0,9 mertebe küçülür; k = 0 olduğundan gerçek küçülme daha büyük olabilir. Dilsiz plasebolar (%82–85) plasebo12'de tipik plasebolardı
(p^12 medyanı ~10⁻²); bu görevde ayrışamazlar (p = 1).

---

## 6. Duyarlılık kolları (M20)

| Kol | Kur'an'ın dili | eşik | k_Q / 10⁸ (bütünlüklü olabilen kontrol) | K_d | en küçük plasebo p | R | p_kal | okuma |
|---|---|---|---|---|---|---|---|---|
| A− (≥ 2 / ≥ 2) | {B2, K1, Qc} @ (1,+1,19) | 15,947 | 0 (132) | 60 | 4,7×10⁻⁵ (Am012 {FAP, WSum}; 46 / 10⁶) [3,4; 6,1]×10⁻⁵ | 0 | 1/201 | aşar |
| **A** (≥ 3 / ≥ 3) | {B2, K1, Qc, WSum} @ (1,+1,19) | 18,801 | 0 (8) | 35 | 1,1×10⁻⁶ (A195) | 0 | 1/201 | aşar |
| A+ (4 / 4) | {B2, K1, Qh, WSum} @ (1,+1,19) | 18,801 | 0 (0) | 2 | 2,6×10⁻⁴ (Ap120; 255 / 10⁶) [2,2; 2,9]×10⁻⁴ | 0 | 1/201 | aşar |

Duyarlılık kolları okumayı değiştirmez.
- A−'de Kur'an'ın dili plasebo12'ninkiyle aynıdır. plasebo12'de bu dille eşiği aşan 33 kontrolün (33 / 10⁸) hiçbiri bütünlüklü puanda
  (kapı G ve ≥ 2 tür, ≥ 2 düzey) eşiğe ulaşmaz.
- A+'da plasebolardan yalnız ikisi dil kurabilir.
- Model-tabanlı: A−'de Δ ≥ 3,67 mertebe; (a) GPD P = 0 (uç noktanın ötesi); (b) GPD 0, normal 7,7×10⁻¹². A+'da Δ ≥ 4,41; modeller
  dejeneredir (K_d = 2; uyum yapılmadı).

---

## 7. Kol B — bedelsiz en geniş dil (ikincil)

Soru: her kitap sahip olduğu her şeyle tarif edilse, Kur'an'ın tarifi rastgele kitapların tariflerinden ne kadar seyrek? Kurallar:
- dil bedeli 0;
- dil, hedefin bilgi puanını en büyük yapan mercekte sağladığı bütün L\* yüklemleridir;
- bütünlük kuralı yoktur;
- kontroller plasebo12 puanıyla (bütün mercekler) puanlanır.

Ön-kayıtlı bütçe kuralı her düzeye izin verdi; üç sıfır da tamamlandı.

### 7.1 Kur'an'ın en geniş dili
Üç sıfırda da aynı mercek ve aynı 15 yüklem: **(1,−1,19)**, {B1, B2, FKS, W1, QU, Qa, QH, WSum, WUd, HH1, HH2, HR0c, HR1c, HR1e, HK2}.

Bu mercekte t = n − a ve u = n + a'dır:
- F = {19 | n − a}, 7 üyeli aile ("ikinci el");
- H = {19 | n + a}, 12 üyeli aile (kristal).

Kristalin yapısı H grubunda görünür: HH1 ve HH2 eşit yarılar; HR0c, HR1c, HR1e hücreler; HK2 5–9 bandı. İki elin ayrıklığı, 19'luk birleşim
ve 19 | Σa(U) W grubundadır.

| Grup (arama) | M20 | Tekdüze | Mallows |
|---|---|---|---|
| kitap: B1 ∧ B2 | 46.672 / 10⁸ → 11,07 bit | 31.288 / 10⁸ → 11,64 | 45.344 / 10⁸ → 11,11 |
| F: FKS | 6.614.532 / 14.141.747 → 1,10 | 6.718.983 / 14.110.428 → 1,07 | 6.436.486 / 13.655.297 → 1,09 |
| W: W1 ∧ QU ∧ Qa ∧ QH ∧ WSum ∧ WUd | 236 / 14.141.747 → 15,87 | 232 / 14.110.428 → 15,89 | 198 / 13.655.297 → 16,07 |
| H: altı yüklem \| k_H = 12 | 560 / 1.203.170 → 11,07 | 43 / 1.014.992 → 14,53 | 723 / 1.307.394 → 10,82 |
| toplam bilgi | 39,10 | 43,13 | 39,09 |
| **eşik** (bilgi − mercek bedeli 7,317) | **31,784 bit** | **35,814 bit** | **31,769 bit** |

Bütün aramalar çözülmüştür (c ≥ 10). (1,+1,19)'daki 21 yüklemlik küme daha az bit verir; D bu yüzden aynalı merceği seçer. Bu mercekte
k_F·p = 7·19 = 133 < 228 olduğundan kapı G tutmaz; Kol B'de bütünlük kuralı yoktur.

### 7.2 Sonuçlar
| Sıfır | K | Kur'an p_Q (k / 10⁸) | plasebo p medyanı [%95] | p < 10⁻⁶ / 10⁻⁷ | en küçük plasebo p (k / N) | R | p_kal | okuma |
|---|---|---|---|---|---|---|---|---|
| M20 | 200 | **1,0×10⁻⁸ (0)** [0; 3,7×10⁻⁸] | 8,4×10⁻⁵ [5,2×10⁻⁵; 1,15×10⁻⁴] | 8 / 2 | 2,0×10⁻⁸ (B183; 1 / 10⁸) [2,5×10⁻¹⁰; 5,6×10⁻⁸] | 2 | **3/201 = 0,0149** | arada |
| Tekdüze | 100 | **1,0×10⁻⁸ (0)** [0; 3,7×10⁻⁸] | 9,9×10⁻⁵ [4,3×10⁻⁵; 2,3×10⁻⁴] | 8 / 2 | 2,0×10⁻⁸ (B080; 1 / 10⁸) [2,5×10⁻¹⁰; 5,6×10⁻⁸] | 2 | **3/101 = 0,0297** | arada |
| Mallows | 100 | **1,0×10⁻⁸ (0)** [0; 3,7×10⁻⁸] | 1,8×10⁻⁴ [9,5×10⁻⁵; 3,9×10⁻⁴] | 4 / 2 | 1,0×10⁻⁸ (B090; 0 / 10⁸) [0; 3,7×10⁻⁸] | 2 | **3/101 = 0,0297** | arada |

Derinlik: M20'de 159 / 32 / 9 plasebo 10⁶ / 10⁷ / 10⁸'de durdu; tekdüzede 81 / 11 / 8; Mallows'ta 88 / 8 / 4. 10⁸'de k = 0 kalan
(sansürlü) plasebo yalnız Mallows'ta vardır: B090. Hiçbir sıfırda Kur'an'ın eşiğine 2 bitten yakın gelen kontrol yoktur.

**Kur'an'ın aralığına giren altı plasebo (her sıfırda iki).**

| Sıfır | Plasebo | Dil @ mercek | Yapı | Eşik | k / 10⁸ → p [%95] | Arama durumu |
|---|---|---|---|---|---|---|
| M20 | **B183** | {FSC, Qn, QH, WI1, WSum, WHRd2, HH2} @ (1,+2,19) | β çift olduğundan dört F hücresinin ikisi boş (4/0/0/2) | 21,95 bit | 1 → 2,0×10⁻⁸ [2,5×10⁻¹⁰; 5,6×10⁻⁸] | puanın 21,3 biti tek bir **çözülmemiş** W aramasından gelir (6 / 16.489.723 referans kitabı) |
| M20 | **B087** | 11 yüklem @ (1,−1,13) | — | 21,52 bit | 7 → 8,0×10⁻⁸ [2,8×10⁻⁸; 1,4×10⁻⁷] | aramaları çözülmüş |
| Tekdüze | **B080** | 15 yüklem {B1, Zkd, Zkd2, F2, QS, Qh, FSd2, FHd2, FCd, Qn, QH, WHd, WUNd2, HR0c, HR1c} @ (1,+1,5) | k_F = 25, k_H = 20; hücreler 6/5/5/9 | 28,36 bit | 1 → 2,0×10⁻⁸ [2,5×10⁻¹⁰; 5,6×10⁻⁸] | aramaları çözülmüş (en seyrek: F 211 / 7.858.432 → 15,18 bit) |
| Tekdüze | **B068** | 9 yüklem {F2, K2, FC2, FSC, W1, Qn, QH, WUN2p, WHRd2} @ (3,+2,43) | β çift; hücreler 3/0/0/2 | 25,95 bit | 4 → 5,0×10⁻⁸ [1,1×10⁻⁸; 1,0×10⁻⁷] | W araması **çözülmemiş** (9 / 7.732.562 → 19,63 bit) |
| Mallows | **B090** | 10 yüklem {B1, Zkd, Zkd2, R1c, FSC, QH, WI1, WUd, HH1, HR0c} @ (3,+1,5) | k_F = 25, k_H = 16; hücreler 11/2/6/6 | 27,81 bit | **0** → 1,0×10⁻⁸ [0; 3,7×10⁻⁸]; Kur'an'la aynı çözünürlük sınırında, eşitlik Kur'an'ın aleyhine sayılır | aramaları çözülmüş |
| Mallows | **B061** | 12 yüklem @ (1,+1,5) | k_F = k_H = 26, \|F∩H\| = 8 | 24,78 bit | 2 → 3,0×10⁻⁸ [2,4×10⁻⁹; 7,2×10⁻⁸] | F araması **çözülmemiş** (3 / 6.609.383 → 20,85 bit) |

B183, plasebo12'de de M20'nin en iyi plasebosudur (x183, 3,0×10⁻⁶).

Çözülmemiş aramaların değeri ön-kayıtlı tek kuraldır (c < 10 → log₂(n + 1) − log₂(c + 0,5)); hedefte ve kontrollerde aynen kullanılır.
Betimsel bir not (ön-kayıtlı hesap değildir): bu üç plasebo çıkarılsa da her sıfırda çözülmüş aramalarla Kur'an'ın aralığına giren bir
plasebo kalır. O durumda R = 1 olur (2/201, 2/101) ve sınıf yine "arada"dır.

Nokta değerleri Kur'an'ınkinden büyüktür (B090'da eşittir), ama aralıkları Kur'an'ınkiyle çakışır ve ön-kayıt gereği Kur'an'ın aleyhine
sayılır. Kur'an'ın nokta değeri çözünürlük sınırındadır (k = 0); 10⁸ kontrolle 0, 1, 2, 4 ve 7 isabet birbirinden kesin ayrılamaz.

**Model-tabanlı** (etiketli; kararlarda kullanılmaz). Bütün plasebolar dilli olduğundan koşullu karışım (a) ile aynıdır.

| Sıfır | GPD (üst %20) P [önyükleme %95] (u; σ; ξ) | normal P (μ; σ) | Δ (mertebe) |
|---|---|---|---|
| M20 | 7,4×10⁻⁴ [0; 5,3×10⁻³] (4,94; 0,83; −0,16) | 1,9×10⁻⁴ (4,11; 1,09) | 0,30 |
| Tekdüze | 8,8×10⁻⁴ [0; 8,9×10⁻³] (4,92; 1,27; −0,35) | 6,8×10⁻⁴ (4,06; 1,23) | 0,30 |
| Mallows | 1,0×10⁻² [0; 3,1×10⁻²] (4,66; 0,61; 0,38) | 2,3×10⁻⁴ (3,79; 1,20) | 0,00 |

**Plasebo dilleri.**

| | M20 | Tekdüze | Mallows |
|---|---|---|---|
| boy (en sık) | 5–21 (8) | 5–21 (9) | 6–18 (10) |
| tür sayısı 4 / 3 / 2 / 1 | 89 / 61 / 47 / 3 | 46 / 35 / 17 / 2 | 44 / 35 / 21 / 0 |
| düzey sayısı 4 / 3 / 2 | 47 / 129 / 24 | 18 / 72 / 10 | 17 / 74 / 9 |
| en sık yüklemler | WUTd 88, WHAd 80, WHNd 80, W1 78, QS 77 | WUTd 46, FKS 39, WHAd 39, WHNd 38, W1 35 | QS 41, FKS 36, W1 36, WUTd 36, FAd 35 |

Bütünlük kuralı yokken en geniş dil bir kitabı çok yüklemle ve çok bitle tarif eder. Plasebolar plasebo12'dekinden daha ayırt edici olur:
log₁₀(p^B / p^12) medyanı M20'de −2,22 [−2,89; −1,37] (188 / 200 plasebonun p'si küçüldü), tekdüzede −1,86 [−2,90; −0,79] (96 / 100),
Mallows'ta −1,93 [−2,79; −0,77] (90 / 100).

**Okuma (betimsel; okuma kuralı Kol A içindir).** Her kitap sahip olduğu her şeyle tarif edildiğinde Kur'an'ın tarifi şu plasebolardan
seyrek kalır: M20'de 200'ün 198'inden, tekdüzede ve Mallows'ta 100'ün 98'inden. Her sıfırda ikisi Kur'an'ın aralığına girer. p_kal 3/201 ve
3/101'dir; sınıf "arada"dır ve kalibre p'leriyle aynen yazılır. Kol A'daki "bütün plaseboları aşar" sonucu en geniş tarifte değil,
bütünlük şartlı tarifte elde edilir.

---

## 8. Betimseller (Ö4)

### 8.1 Kur'an'ın rakipleri
Her kolda ve her sıfırda, 10⁸ kontrolün (400 bölüt) hiçbiri Kur'an'ın eşiğine 2 bitten yakın gelmez:
- 2.400 bölüt dosyasının aday listeleri boştur;
- aday tamponu taşması 0'dır.

Kur'an'ın isabeti olmadığından M20'de zincir kümelenmesi boştur. Kol A kollarında Kur'an'ın dili altında en az bir mercekte bütünlüklü
olabilen kontrol sayısı (kB_Q, / 10⁸): M20 A'da 8, A−'de 132, A+'da 0; tekdüzede 6; Mallows'ta 10. Bu kontrollerin hiçbiri Kur'an'ın
puanına yaklaşmaz.

### 8.2 En iyi beş plasebo
Tablolar `src/rapor_tablo_b.py` ile analiz çıktılarından üretildi (`sonuc/rapor_tablolar.md`). "kapı G 1", bu mercekte kapı G'nin
tuttuğunu gösterir.

**M20, Kol A**

| ad | p_x [%95] | k / N | dil | türler / düzeyler | yapı (mercek, k_F, k_H, hücreler) |
|---|---|---|---|---|---|
| A195 | 1,10×10⁻⁶ [4,80×10⁻⁷; 1,84×10⁻⁶] | 10 / 10⁷ | FAd W1 HR0e HR1e | abc / aile, birleşim, hücre | (1,+3,19), k_F 12, k_H 13, \|F∩H\| 0, hücre boyları 4/2/3/3, q-toplamları 58/22/25/49, kapı G 1 |
| A159 | 1,46×10⁻⁵ [1,22×10⁻⁵; 1,71×10⁻⁵] | 145 / 10⁷ | FCK WI1 HR0e | acd / aile, birleşim, hücre | (3,+1,17), k_F 14, k_H 7, \|F∩H\| 1, hücre boyları 3/4/4/3, q-toplamları 42/48/52/45, kapı G 1 |
| A146 | 1,50×10⁻⁵ [1,26×10⁻⁵; 1,75×10⁻⁵] | 149 / 10⁷ | FCK FKS Qn WSum | bcd / aile, birleşim, hücre | (2,+3,19), k_F 12, k_H 7, \|F∩H\| 0, hücre boyları 4/3/2/3, q-toplamları 56/36/18/39, kapı G 1 |
| A012 | 1,90×10⁻⁵ [1,07×10⁻⁵; 2,84×10⁻⁵] | 18 / 10⁶ | FD2 FAP WSum | acd / aile, birleşim, hücre | (1,−3,19), k_F 13, k_H 6, \|F∩H\| 1, hücre boyları 2/5/3/3, q-toplamları −12/−42/3/−27, kapı G 1 |
| A062 | 1,05×10⁻⁴ [8,50×10⁻⁵; 1,26×10⁻⁴] | 104 / 10⁶ | R1e FD1 FKS W1 | acd / aile, birleşim, hücre | (2,+1,17), k_F 14, k_H 6, \|F∩H\| 0, hücre boyları 3/3/4/4, q-toplamları 32/30/42/42, kapı G 1 |

**Tekdüze, Kol A**

| ad | p_x [%95] | k / N | dil | türler / düzeyler | yapı |
|---|---|---|---|---|---|
| A018 | 4,10×10⁻⁶ [2,86×10⁻⁶; 5,45×10⁻⁶] | 40 / 10⁷ | FSp FKS WEq | bcd / aile, birleşim, hücre | (1,+1,29), k_F 9, k_H 9, \|F∩H\| 1, hücre boyları 2/2/2/3, q-toplamları 12/6/6/5 |
| A045 | 4,10×10⁻⁶ [2,86×10⁻⁶; 5,45×10⁻⁶] | 40 / 10⁷ | FD2 FKS WI1 | acd / aile, birleşim, hücre | (1,+1,23), k_F 12, k_H 3, \|F∩H\| 1, hücre boyları 3/3/2/4, q-toplamları 18/20/10/12 |
| A025 | 1,03×10⁻⁵ [8,32×10⁻⁶; 1,24×10⁻⁵] | 102 / 10⁷ | K2 WHAd HR0e | abd / aile, birleşim, hücre | (1,−1,23), k_F 11, k_H 4, \|F∩H\| 0, hücre boyları 2/2/3/4, q-toplamları 0/−2/−7/0 |
| A023 | 5,10×10⁻⁵ [3,71×10⁻⁵; 6,59×10⁻⁵] | 50 / 10⁶ | FC2 FSC WI1 | acd / aile, birleşim, hücre | (1,+1,23), k_F 13, k_H 7, \|F∩H\| 1, hücre boyları 4/3/2/4, q-toplamları 24/14/2/12 |
| A010 | 6,70×10⁻⁵ [5,10×10⁻⁵; 8,40×10⁻⁵] | 66 / 10⁶ | FRK Qn HR0c | abd / aile, birleşim, hücre | (1,−1,23), k_F 10, k_H 5, \|F∩H\| 0, hücre boyları 2/3/2/3, q-toplamları 2/−2/0/−1 |

**Mallows, Kol A**

| ad | p_x [%95] | k / N | dil | türler / düzeyler | yapı |
|---|---|---|---|---|---|
| A047 | 3,40×10⁻⁷ [2,27×10⁻⁷; 4,63×10⁻⁷] | 33 / 10⁸ | Zk2p R0c FKS WUTd2 | abd / aile, birleşim, hücre | (1,+3,11), k_F 22, k_H 7, \|F∩H\| 2, hücre boyları 5/5/6/6, q-toplamları 82/102/156/104 |
| A068 | 6,80×10⁻⁷ [5,19×10⁻⁷; 8,51×10⁻⁷] | 67 / 10⁸ | B1 FCK Qa HK2 | abd / aile, birleşim, kitap | (1,+3,29), k_F 9, k_H 3, \|F∩H\| 0, hücre boyları 2/2/2/3, q-toplamları 20/16/14/17 |
| A064 | 4,50×10⁻⁶ [3,20×10⁻⁶; 5,91×10⁻⁶] | 44 / 10⁷ | FRK Qn HR0c | abd / aile, birleşim, hücre | (1,+1,13), k_F 19, k_H 9, \|F∩H\| 2, hücre boyları 4/6/3/6, q-toplamları 32/56/25/62 |
| A008 | 8,90×10⁻⁶ [7,06×10⁻⁶; 1,08×10⁻⁵] | 88 / 10⁷ | R1e FD1 WI1 WHRd2 | abc / aile, birleşim, hücre | (1,+1,19), k_F 12, k_H 6, \|F∩H\| 1, hücre boyları 3/3/3/3, q-toplamları 22/18/23/23 |
| A049 | 2,10×10⁻⁵ [1,22×10⁻⁵; 3,09×10⁻⁵] | 20 / 10⁶ | QkF WI1 HR0e | abc / aile, birleşim, hücre | (1,+3,17), k_F 17, k_H 8, \|F∩H\| 1, hücre boyları 4/2/3/8, q-toplamları 50/22/25/148 |

**M20, Kol A−** (ilk üç): Am012 {FAP, WSum} 4,70×10⁻⁵ (46 / 10⁶); Am159 {FCK, HR0e} 5,20×10⁻⁵ (51 / 10⁶); Am056 {FKS, WHRp} 1,06×10⁻⁴
(105 / 10⁶).

**M20, Kol A+** (yalnız iki plasebo dilli):
- Ap120 {B1, QS, FKS, W1} @ (3,−1,37): 2,56×10⁻⁴ (255 / 10⁶);
- Ap157 {B1, FAd, FKS, W1} @ (3,−1,23): 5,30×10⁻⁴ (529 / 10⁶).

**Kol B** (her sıfırda en iyi beş; ilk ikisi §7.2'de):

| Sıfır | 3. | 4. | 5. |
|---|---|---|---|
| M20 | B093 1,50×10⁻⁷ (14 / 10⁸) @ (3,+1,13) | B162 2,10×10⁻⁷ (20 / 10⁸) @ (3,+1,13) | B044 2,20×10⁻⁷ (21 / 10⁸) @ (3,+1,7) |
| Tekdüze | B018 2,00×10⁻⁷ (19 / 10⁸) @ (2,+3,29) | B082 2,10×10⁻⁷ (20 / 10⁸) @ (1,+1,13) | B008 6,80×10⁻⁷ (67 / 10⁸) @ (2,−1,7) |
| Mallows | B011 7,70×10⁻⁷ (76 / 10⁸) @ (1,−1,7) | B016 8,50×10⁻⁷ (84 / 10⁸) @ (1,+1,5) | B002 1,20×10⁻⁶ (11 / 10⁷) @ (1,−3,5) |

Kol B'nin her sıfırdaki en iyi beşinin (15 plasebo) hiçbirinde, kendi en iyi merceğinde kapı G tutmaz. Kol A'nın plasebolarında, tanım
gereği tutar.

### 8.3 Plasebo dillerinin dağılımı
Kol A: §4.2; Kol B: §7.2. Bütün kolların p eşik altı sayıları, derinlikleri ve model ayrıntıları `sonuc/rapor_tablolar.md`'dedir.

### 8.4 Denetimler (ONKAYIT §7.2)
- (i) D_B'nin ya da Kol B'nin eşiği (double) ile kontrol programının eşiği (float tablolar) arasındaki en büyük fark: Kol A kollarında
  ≤ 8,3×10⁻⁷ bit, Kol B'de ≤ 1,4×10⁻⁶ bit. Mercekler her karşılaştırmada aynıdır (0 farklı).
- (ii) Aday sınırı (24) hiçbir hedefte devreye girmedi.
- (iii) Hedeflerin en iyi merceğinde çözülmemiş aramalar: Kur'an'da hiçbir kolda yok.
  - Plasebolarda Kol A: M20 A120 (A−'de Am120, A+'da Ap120 aynı kitap); tekdüzede ve Mallows'ta yok.
  - Plasebolarda Kol B: M20'de B067, B071, B162, B183; tekdüzede B046, B068, B088; Mallows'ta B002, B027, B061.
- (iv) Kontrol aday tamponu taşması: 0 (bütün bölütler).
- (v) Dondurma anı kod özetleri: 50 satırın 49'u aynı. `src/analiz_b.py` betimsel düzeltme yüzünden farklıdır (ek).

---

## 9. Tahminin değerlendirilmesi (Kol A, M20; ön-kayıt §10)

| Parça | Tahmin | Ölçülen | Hüküm |
|---|---|---|---|
| p_Q | merkez 10⁻⁷, aralık 10⁻⁹–10⁻⁶ | k = 0 / 10⁸: p̂ = 1,0×10⁻⁸, tek yanlı %95 üst sınır 3,0×10⁻⁸ (≥ 10⁻⁹); \|log₁₀ p̂ + 7\| = 1,0 | **karar verilemedi** |
| en az yarısı dilli | K_d ≥ 100 | K_d = 35 / 200 | **tutmadı** |
| plasebo p_x medyanı | 10⁻³–10⁻¹ | 1 [1; 1] (165 dilsiz plasebo p = 1); dilli 35'in medyanı 7,1×10⁻⁴ | **tutmadı** |
| en küçük plasebo p_x | 10⁻⁶–10⁻⁴ | 1,1×10⁻⁶ [4,8×10⁻⁷; 1,8×10⁻⁶] (A195) | **tuttu** |
| hepsini aşar | p_kal = 1/201 | R = 0 → 1/201 | **tuttu** |
| **bütün tahmin** | | iki parça tuttu, iki parça tutmadı, biri karar verilemedi | **kısmen tuttu** |

p_Q parçası için ön-kayıtlı kural, k_Q = 0'da yalnız tek yanlı üst sınıra bakar. 3,0×10⁻⁸ ≥ 10⁻⁹ olduğundan hüküm "karar
verilemedi"dir. Nokta değeri 10⁻⁸ tahmin aralığının içindedir ve merkezin bir mertebe altındadır; gerçek değer 10⁸ kontrolle ayırt
edilemez. Tutmayan iki parça aynı olgudan gelir: plasebo kitapların çoğu bütünlüklü bir dil kuramaz. Bu olgu, dondurmadan önce kabaca
görülmüş ve ön-kayıtta açıklanmıştı (§1.1).

---

## 10. Okuma

**Okuma kuralı Kol A'da, her sıfırda işletilir.** M20'de, tekdüzede ve Mallows'ta R = 0'dır; ilk cümle uygulanır:

> **Kur'an'ın bütünlüklü ayrışması bütün plasebo kitaplarınkini aşar; nesnenin bütünlüğünün sırası dilin Kur'an'a göre yazılmasıyla açıklanamaz.** Kalibre p M20'de 1/201 = 0,0050, tekdüzede ve Mallows'ta 1/101 = 0,0099'dur (K = 200 ve 100 ile en küçük olabilen değer). Aradaki fark model-tabanlıdır: betimsel fark Δ ≥ 2,04 / 2,61 / 1,53 mertebe; model P'leri §5'tedir ve modele çok bağlıdır.

Bu cümlenin kapsamı:

1. **"Dilin Kur'an'a göre yazılması" burada neyi kapsar?** Kapsadığı şey dilin seçimidir: hangi yüklemlerin ve hangi merceğin seçildiği.
   Bu seçim, her plasebonun aynı kuralla kendi dilini seçmesine izin verilerek kalibre edildi. Kapsamadığı şey kuralın kendisidir. Kapı G,
   ≥ 3 tür / ≥ 3 düzey eşikleri ve tür/düzey etiketleri Kur'an'ın biçimi bilinerek yazıldı (plasebo12 ön-kaydı §9 c). Kur'an kapı G'yi
   **tam sınırda** sağlar: k_F·p = 12·19 = 228, ve bu 406 mercekten yalnız birinde olur. Başka bir kuralın seçilmiş olabileceği
   bedellendirilmedi (**ölçülmedi**). Cümledeki "açıklanamaz", dil seçimi için geçerlidir; kural seçiminin payı ölçülmemiştir.
2. **Ayrışmanın kaynağı.** Plasebo kitapların %82,5 / %82 / %85'i bütünlüklü dil kuramaz (p = 1). Çoğunda hiçbir mercekte kapı G tutmaz.
   "Aşar" sonucu bu yüzden büyük ölçüde şunu söyler: Kur'an kuralın kapısını ve kapsamını sağlar, rastgele kitapların çoğu sağlamaz.
   Kuralı sağlayabilen plasebolar arasında da Kur'an'a ulaşan yoktur. En yakınları M20'de 1,1×10⁻⁶ (10 / 10⁷), tekdüzede 4,1×10⁻⁶
   (40 / 10⁷), Mallows'ta 3,4×10⁻⁷ (33 / 10⁸); Kur'an 0 / 10⁸.
3. **Bütünlük şartı olmadan sonuç "arada"ya iner.** Kol B'de (bedelsiz en geniş dil, bütünlük kuralı yok) üç sıfırda da R = 2; p_kal
   0,0149 / 0,0297 / 0,0297. Her kitap sahip olduğu her şeyle tarif edildiğinde, 200 M20 plasebosunun 2'si ve 100'er tekdüze ve Mallows
   plasebosunun 2'şeri, 10⁸ kontrolün çözünürlüğünde Kur'an'ın aralığına giren bir tarife ulaşır. Kol A ile Kol B arasındaki fark bütünlük
   şartıdır; bu şartın kendisi de 1. maddedeki sınırlamayı taşır.
4. **Çözünürlük.** p_Q her kolda ve her sıfırda çözünürlük sınırındadır (0 / 10⁸). Kalibre p'nin çözünürlüğü 1/(K + 1)'dir. Daha ince
   bir ifade yalnız model-tabanlıdır ve modeller birbirinden mertebelerce ayrılır.
5. **Literatürün payı.** Kur'an'ın Kol A dilinin içeriği büyük ölçüde yayımlanmış gözlemlerdir: B2 1994, Qc 1990, K1'in iki 19'u en geç
   2015; K1'in 18 ve 20'si ile WSum bu çalışmanındır. D_B bunları literatürü bilmeden seçti. Gözlemleri ilk bulan insan aramasının
   büyüklüğü ise ölçülmedi.
6. **Bu bir sıradır, nesnenin nadirliği değildir.** p_Q, Kur'an'ın kendi bütünlüklü dilindeki puanının, sıfır modelinin kitapları
   arasındaki sırasıdır. Olay frekansları değildir.

plasebo12 ile yan yana:
- plasebo12 (kısıtsız dil): M20'de 1/201 "aşar", tekdüzede 2/101 "arada", Mallows'ta 1/101 "aşar".
- plasebo14 Kol A (bütünlüklü dil): üç sıfırda da "aşar".
- plasebo14 Kol B (en geniş dil): üç sıfırda da "arada".

İkinci test birincinin yerine geçmez. Ana hattın planı (bilgi için) Kol A'nın "aşar" çıkmasına bağlıydı; bu koşul üç sıfırda da gerçekleşti.
Okumaya bu bölümün 1–6. maddeleri eşlik etmelidir.

---

## 11. Ölçülmeyenler ve sınırlılıklar

**Ölçülmedi:**
- **Kütüphanenin ve bütünlük kuralının sınırları.** Kural Kur'an'ın biçimi bilinerek yazıldı (12. den. §7, plasebo12 §9 c).
  - Kapı G k_F ≥ 2E = 228/p ister (E = 114/p, ailenin beklenen boyu). Kur'an'ın ailesi bu eşiğe tam oturur: 12·19 = 228; Kur'an'ın 406
    mercekte kapı-G merceği yalnız budur.
  - "≥ 3 tür, ≥ 3 düzey" şartı ve tür/düzey etiketleri de Kur'an'ın katmanlarından okunmuş bir sınıflamadan gelir.
  - Başka kapı eşiklerinin (ör. k_F·p > 228), başka etiketlemelerin ve başka tür/düzey sayılarının etkisi ölçülmedi. A− ve A+ yalnız
    sayıları değiştirir.
  - L\* el dilinden genelleştirildi. Dışındaki yüklem aileleri, 406 mercekli mercek ailesi ve bölme seçimi değiştirilmedi.

  Bu kalibrasyon, **kuralın içinde** D_B'nin seçimini bedellendirir; kuralın kendisini bedellendirmez.
- **Literatürün araması.** Kur'an'ın Kol A dilinin dört yükleminden üçünün içeriği 1990, 1994 ve en geç 2015 tarihli yayımlanmış
  gözlemlerdir (§4.1). Onları ilk bulan insan aramasının büyüklüğü ölçülmedi.
- **Nesnenin (n, a) seçimi.** Sûre numaraları ve ayet sayıları dışındaki girdiler ölçülmedi.
- **Sayım okulu.** Yalnız Kûfe sayımı kullanıldı (kodeks 129); 127 ve öteki okullar ölçülmedi.
- **Gruplar arası bağımsızlık yaklaşımı.** Bilgi puanı beş grubun çarpımıdır: kitap ⊥ boy ⊥ aile-içi ⊥ iki el ⊥ karşı aile | bağlam. Bu
  yaklaşımın etkisi ölçülmedi. Yaklaşım hedefe ve kontrollere aynen uygulanır; kalibrasyonu bozmaz, puanın olasılık yorumunu yaklaşık yapar.
- **Kültürel havuz**, **referans tablosu gürültüsünün p_x'e etkisi** (puan işlevi tek bir tabloya koşulludur) ve **dil bedelinin biçimi**.

**Sınırlılıklar:**
- **Çözünürlük.** p_Q bütün kollarda ve sıfırlarda çözünürlük sınırındadır (k = 0 / 10⁸). Kalibre p'nin çözünürlüğü 1/(K + 1)'dir:
  M20'de 1/201, ötekilerde 1/101. Ötesi yalnız model-tabanlıdır, ve dilsiz plasebolar bir nokta kütlesi oluşturduğu için modeller
  birbirinden çok farklıdır.
- **Dilsiz plasebolar.** Plasebo kitapların %82–85'i bütünlüklü dil kuramaz ve p_x = 1 alır. "Kur'an bütün plasebolardan çok ayrışır"
  sonucu büyük ölçüde Kur'an'ın bu kuralın kapısını ve kapsamını sağlamasından gelir. Dilli plasebolar arasında da Kur'an'a ulaşan yoktur:
  en yakın dilli plasebo M20'de 1,1×10⁻⁶, tekdüzede 4,1×10⁻⁶, Mallows'ta 3,4×10⁻⁷.
- **Kol B'de çözülmemiş aramalar.** Kur'an'ın aralığına giren altı plasebodan üçünün puanı çözülmemiş bir aramaya dayanır (§7.2). Değer
  ön-kayıtlı nokta değeridir; aralık taşınmaz.
- **Sıra, nadirlik değil.** Bu bir sıradır, nesnenin nadirliği değildir; olay frekansları merdivenin üst basamaklarındadır.

---

## 12. Sapmalar, hesap ve kaynaklar

### 12.1 Sapmalar (`ek/`)
- `EK_20261029T0905Z_betimsel_duzeltme.md`: kitap kitap karşılaştırmadaki "dili plasebo12'dekiyle aynı" sayımı, dilsiz-dilsiz çiftleri de
  sayıyordu. Sayım yalnız dilli plasebolara indirildi (ONKAYIT §9'daki tanım).
  - Önce → sonra: M20 A 14 → 0, A− 13 → 0, A+ 16 → 0.
  - M20 Kol A analizi düzeltilmiş betikle yeniden koşturuldu. Değişen tek alan budur.
  - Düzeltme öncesi çıktılar `sonuc/M/duzeltme_oncesi/`'dedir.
  - Kararlara giren hiçbir sayı etkilenmedi.
- `EK_20261029T0913Z_dondurma_sonrasi_kod.md`: dondurmadan sonra eklenen iki dosya ve süreç notları.
  - `kos/yeniden_sina.sh`: ONKAYIT §7.1'in oturum sonu sınaması.
  - `src/rapor_tablo_b.py`: yalnız tablo biçimlendirme.

Başka sapma yoktur. Ön-kaydın bütün kolları koştu: Kol A üç sıfırda; A− ve A+; Kol B üç sıfırda, çünkü bütçe kuralı her düzeye izin verdi.
Herhangi bir dile ya da kontrol puanına bakıldıktan sonra hiçbir tanım, ağırlık, eşik, bedel ya da kural değiştirilmedi.

### 12.2 Hesap (`python3 -I src/hesap_saati_b.py`; kontrol bölütlerinin süre alanları + `sonuc/hesap_ek.txt`)

| Kalem | Çekirdek-saat |
|---|---|
| M20 Kol A kümesi (A, A−, A+; Kur'an'ın dilleri 10⁸, derinleşen plasebolar) | 2,13 |
| tekdüze Kol A | 2,10 |
| Mallows Kol A | 2,17 |
| M20 Kol B | 4,01 |
| tekdüze Kol B | 3,35 |
| Mallows Kol B | 3,23 |
| ek kalemler: dondurma öncesi 3.150 ç-sn (Ö0, doğrulama, hız); sonrası 850 ç-sn (dillerin kuruluşu, analizler, oturum sonu sınaması) | 1,11 |
| **toplam (kayıtlı)** | **18,10** (tavan 40) |

- Aynı anda en çok 4 iş parçacığı kullanıldı. Kontrol programı 4 iş parçacığıyla koşarken analiz ya da derleme başlatılmadı; analizler
  geçişler arasında koştu. Geçişler sürerken yalnız izleme ve çıktı okuma komutları çalıştı: `ls`, `tail`, `grep` ve saniyelik dosya
  okumaları (ONKAYIT §8.2).
- Bellek: Kol B dil tabloları tek partide 864 MB'tır (`sonuc/dogrulama/hiz.txt`). En büyük süreç ölçümü 1,87 GB'tır: M20 Kol B düzey-1
  dilleri, oturum sonu sınamasında. Tavan 6 GB.
- Disk: proje 14 MB, tavan 8 GB. Referans tabloları plasebo12'de yerinde okundu.

---

## 13. Yeniden üretim ve paket

**Komutlar** (`~/opus5.5/plasebo14`'te; Apple M1, macOS, Apple clang arm64; Python 3 + numpy):
```
./kos/derle.sh                          # bütün C programları (-O3 -std=gnu11 -w); bayraklar plasebo12'ninkiyle aynı
./kos/o0_d.sh && ./kos/o0_k.sh          # Ö0: kısıtsız D ve kontrol bölüt 1, plasebo12 ile bit-bit
./kos/dogrulama.sh && ./kos/hiz.sh      # dondurma öncesi doğrulamalar (hedef olmayan kitaplar) ve hız/bellek ölçümü
./kos/akis_b.sh                         # diller (kos/diller.sh) → kontrol kümeleri (kos/kontrol_b.py; düzey 1–3, bütçe kuralı) → analiz (src/analiz_b.py)
python3 -I src/rapor_tablo_b.py         # sonuc/rapor_tablolar.md
python3 -I src/hesap_saati_b.py         # kayıtlı hesap
./kos/yeniden_sina.sh <geçici_dizin>    # oturum sonu: yeniden derle, Ö0'ı ve bu oturumun çıktılarını bit-bit yeniden üret
```
Kontrol programları plasebo12'nin referans tablolarını yerinde okur: `~/opus5.5/plasebo12/tab/<S>/ref_{1111,2222,0444,0888}.ref`. Özetleri
`~/opus5.5/plasebo12/sonuc/ref_sha256.txt`'tedir; tablolar pakete girmez.

**Bağımsız doğrulama için.**
1. `ONKAYIT.md`'nin SHA-256'sını `sonuc/onkayit_ozet.txt` ile karşılaştırın.
2. Dil dosyalarını `shasum -a 256 -c sonuc/diller_sha256.txt` ile doğrulayın.
3. Her hedefin k / N'sini bölüt dosyalarından yeniden sayın: `tab/<S>/k{A,B}/d<düzey>_0/b*.txt`, ilk satır başlıktır ve süre alanını
   taşır.
4. `src/analiz_b.py <S> <A|B>`'yi koşun ve `sonuc/<S>/analiz_*.json` ile karşılaştırın.
5. `kos/yeniden_sina.sh`'i koşun; 16 satırın hepsi "AYNI" olmalıdır.

**Paket.** `paket/plasebo14_paket.tar.gz`; SHA-256'sı `paket/plasebo14_paket.sha256`'dadır. Referans tabloları, derlenmiş ikililer ve
taslaklar hariçtir. İçerik:
- `GOREV.md`, `gorev_sha256.txt`, `baslat.command`, `ONKAYIT.md`, bu rapor;
- `ek/`, `src/`, `kos/`, `kontrol/` (kaynaklar);
- `tab/<S>/`: `hedef.bin`, donmuş diller ve ayrıntılar, hedef ayrıntıları, `kA/` ve `kB/` kontrol kümeleri (2.400 bölütün sayımları ve
  `.q` dosyaları, eşikler);
- `sonuc/`: analizler, günlük, ara notlar, Ö0, doğrulama, özetler, düzeltme öncesi çıktılar.

Paketteki her dosyanın SHA-256'sı `paket/plasebo14_icerik_sha256.txt`'tedir; bu liste paketin içinde de vardır.

---

## English summary

**Question.** Suppose every target must build its own best *integral* language: in its own best lens, the satisfied predicates must
span at least three motif types and three levels, and the lens must pass gate G (k_F·p ≥ 228, every F cell ≥ 2). How unusual is the
Quran's integral separation then? Everything else is plasebo12's: library L\*, costs, information score, reference tables, the 200
M20 / 100 uniform / 100 Mallows placebos and the control stream. The preregistration (`ONKAYIT.md`, frozen 2026-10-29T04:38:14Z,
SHA-256 `6080c6b1…f748`) was written and frozen before any target was evaluated with this task's languages and before any control was
scored with them. All languages were frozen before the first control pass.

**Anchors.** With the integrity constraint switched off, the code reproduces plasebo12's languages, thresholds and one control segment
bit-for-bit in all three nulls. An end-of-session rebuild reproduced all 16 checks bit-for-bit.

**Arm A (primary; ≥ 3 types, ≥ 3 levels; plasebo12's language cost).**
- The Quran's language in all three nulls is {B2, K1, Qc, WSum} @ (1,+1,19), with thresholds 18.80 / 20.83 / 20.00 bits.
  (1,+1,19) is the only one of the Quran's 406 lenses where gate G holds, and it holds exactly at the boundary, 12·19 = 228.
- No control out of 10⁸ reaches its score: p_Q = 1.0×10⁻⁸ (0 / 10⁸) [0; 3.7×10⁻⁸] in every null.
- Only 35 / 200 (M20), 18 / 100 (uniform) and 15 / 100 (Mallows) placebos can build an integral language. The rest get p = 1, mostly
  because gate G holds in none of their lenses.
- The closest placebos are 1.1×10⁻⁶ (10 / 10⁷), 4.1×10⁻⁶ (40 / 10⁷) and 3.4×10⁻⁷ (33 / 10⁸). None overlaps the Quran's interval.
- R = 0 everywhere, so p_cal = 1/201 = 0.0050 (M20) and 1/101 = 0.0099 (uniform, Mallows). The first sentence of the reading rule applies:
  *the Quran's integral separation exceeds that of every placebo book; the rank of the object's integrity cannot be explained by the
  language having been written for the Quran.*
- The model-based gap is reported separately: Δ ≥ 2.04 / 2.61 / 1.53 orders of magnitude. Model P values are strongly model-dependent.

**Sensitivity (M20).** Arm A− (≥ 2/≥ 2; the Quran's language equals plasebo12's {B2, K1, Qc}) and arm A+ (4/4) both give R = 0 and
p_cal = 1/201. The sensitivity arms do not change the reading.

**Arm B (secondary; zero language cost, broadest language, no integrity rule).**
- The Quran's broadest language is 15 predicates @ (1,−1,19), with thresholds 31.78 / 35.81 / 31.77 bits, and 0 / 10⁸ controls in each
  null.
- In each null, two placebos' self-descriptions reach the Quran's interval: M20 B183 (1 / 10⁸) and B087 (7 / 10⁸); uniform B080
  (1 / 10⁸) and B068 (4 / 10⁸); Mallows B090 (0 / 10⁸, a tie) and B061 (2 / 10⁸).
- So R = 2 and p_cal = 3/201 = 0.0149 and 3/101 = 0.0297, class "in between" (descriptive). In each pair, one score rests on an
  unresolved search; the other does not.
- Described by everything it has, the Quran is among the rarest books but not beyond all. The "exceeds all" of arm A comes with the
  integrity condition.

**The main line's prediction** was **partly confirmed**:
- p_Q: undecidable (k = 0; the upper bound 3.0×10⁻⁸ ≥ 10⁻⁹);
- "at least half of the placebos have an integral language": not confirmed (35 / 200);
- placebo median in 10⁻³–10⁻¹: not confirmed (it is 1);
- smallest placebo p in 10⁻⁶–10⁻⁴: confirmed;
- "exceeds all": confirmed.

**Scope and what was not measured.**
- The integrity rule (gate G, the type/level thresholds and labels) was written knowing the Quran's form. This calibration prices the
  *choice of language within the rule*, not the choice of the rule. Its limits were not measured.
- Not measured either: the literature search behind B2 (1994), Qc (1990) and K1 (≤ 2015, plus this study); the choice of (n, a); the
  counting school (Kufan only); the between-group independence approximation; the cultural pool; reference-table noise; the form of
  the language cost.
- p_Q sits at the resolution limit everywhere. This is a rank, not the rarity of the object. plasebo12's result stands beside this one;
  the second does not replace the first.

**Deviations and compute.**
- Two dated appendices: a descriptive count fix (no decision quantity affected) and post-freeze reporting and verification code.
- 18.10 core-hours recorded (ceiling 40); at most 4 threads.
- Package `paket/plasebo14_paket.tar.gz`, SHA-256 in `paket/plasebo14_paket.sha256`.
