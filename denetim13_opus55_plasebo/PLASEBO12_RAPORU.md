# PLASEBO12 — Bütünlük dilinin plasebo kalibrasyonu

**Soru (görev; not 25, çekince 1).** 12. denetimin 20 yüklemli dili Kur'an görüldükten sonra yazıldı. O denetimde mercek seçimi
bedellendi, yüklem seçimi bedellenmedi. Bu oturum şu soruyu cevaplar: *her hedef kendi dilini aynı otomatik yordamla kursa, Kur'an'ın
ayrışması ne kadar sıra dışı kalır?*

**Oturum.** Claude (Opus 5.5), `~/opus5.5/plasebo12`, 28–29 Ekim 2026 (zamanlar UTC). Görev `GOREV.md`, SHA-256 `da4604f1…ef08`
(`gorev_sha256.txt` ile eşleşti). Ön-kayıt `ONKAYIT.md` **2026-10-28T22:33:04Z**'de donduruldu ve salt okunur yapıldı. SHA-256'sı
`3837c9d819f63b027e89b5e21f413c94766c440cea20d981b068fba269771a5a` (`sonuc/onkayit_ozet.txt`). Ön-kayıt hiçbir hedefin (Kur'an
dahil) kütüphane profiline, D sonucuna ya da kontrol puanına bakılmadan yazıldı; dondurmadan önceki bir düşünce adımım §1.1'de
açıklanmıştır. `~/Desktop/hrf2`, `~/opus5.5/butunluk` ve `~/opus5.5/kosullu` yalnız okundu. `~/opus5.5/plasebo` hiç açılmadı (görevin
girdisi değil). Kodeks 129 (Σa = 6236); 127 ölçülmedi.

Kısaltmalar: **12. den.** = on ikinci denetim (`~/opus5.5/butunluk`). **El dili** = 12. den.'in 20 yüklemli dili. **D** = dil kurma
yordamı. **p_x** = hedef x'in kendi dilinde kendi kontrollerine karşı sırası. **p_kal** = kalibre p. **[a; b]** = iki yanlı %95
Clopper–Pearson aralığı. **k / N** = isabet / kontrol sayısı.

---

## 0. Kısa cevaplar

**1. Kendi dilini kuran rastgele kitaplar kendi kontrollerinden ne kadar ayrışıyor? Kur'an nerede, kalibre p nedir?**

| Sıfır | K | plasebo p_x medyanı | en küçük plasebo p_x | Kur'an'ın dili | Kur'an p_Q (k / 10⁸) | R | **p_kal** | okuma |
|---|---|---|---|---|---|---|---|---|
| **M20 (birincil)** | 200 | 1,35×10⁻² [9,9×10⁻³; 1,8×10⁻²] | 3,0×10⁻⁶ (x183; 29 / 10⁷) | {B2, K1, Qc} @ (1,+1,19) | **3,40×10⁻⁷** (33) [2,27; 4,63]×10⁻⁷ | 0 | **1/201 = 0,0050** | hepsini aşar |
| Tekdüze | 100 | 1,06×10⁻² [6,2×10⁻³; 2,4×10⁻²] | 1,0×10⁻⁷ (x068; 9 / 10⁸) | {B2, K1, Qc} @ (1,+1,19) | 5,0×10⁻⁸ (4) [1,1×10⁻⁸; 1,0×10⁻⁷] | 1 | 2/101 = 0,020 | arada |
| Mallows | 100 | 1,40×10⁻² [6,5×10⁻³; 1,8×10⁻²] | 5,8×10⁻⁶ (x046; 57 / 10⁷) | {B2, K1, Qc} @ (1,+1,19) | 8,0×10⁻⁸ (7) [2,8×10⁻⁸; 1,4×10⁻⁷] | 0 | 1/101 = 0,0099 | hepsini aşar |

Plasebolar kendi dillerinde tipik olarak ~10⁻² düzeyinde ayrışır; 200 M20 plasebosundan 8'i 10⁻⁴'ün, 1'i 10⁻⁵'in altına iner, hiçbiri
10⁻⁶'nın altına inmez. Birincil sıfırda Kur'an'ın ayrışması 200 plasebonun hepsini aşar. Kalibre p, K = 200 ile olabilecek en küçük
değerdir: **1/201**. Okuma kuralının ilk cümlesi uygulanır (§8). Aradaki fark model-tabanlıdır ve kararlarda kullanılmaz. Kur'an'ın
p'si en uç plasebonunkinin ~1/9'udur (betimsel fark 0,95 mertebe). GPD kuyruk uyumu Kur'an'ı uyumun uç noktasının ötesine koyar
(P_model = 0; parametrik önyükleme %95 üst ucu 3,1×10⁻⁴). İkincil normal model P = 1,1×10⁻⁵ verir. Tekdüzede bir plasebo (x068)
Kur'an'ın aralığıyla çakışır. Nokta tahmini Kur'an'ınkinin iki katıdır, ama çakışma ön-kayıt gereği Kur'an'ın aleyhine sayılır:
p_kal = 2/101, sonuç "arada" kalır ve kalibre p'siyle aynen yazılır. Mallows'ta Kur'an'ın ayrışması (8,0×10⁻⁸) 100 plasebonun hepsini
aşar. En yakın plasebo 5,8×10⁻⁶'dır ve aralıklar çakışmaz: R = 0 → p_kal = 1/101 = 0,0099, yani K = 100 ile olabilecek en küçük değer.
Okuma kuralının ilk cümlesi bu sıfırda da uygulanır (betimsel fark 1,86 mertebe).

**2. D(Kur'an) el dilini buluyor mu? Dil yazımının kazancı ne kadar?** Hayır; yalnız el dilinin bir çekirdeğini bulur. D(Kur'an), el
dilinin 20 yükleminden 3'ünü seçer: B2 (çapraz terazi), K1 (hücre toplamları ardışık) ve Qc (hücre merkezi 19). Seçim bütün
sıfırlarda aynıdır (M20, tekdüze ve Mallows). Kur'an (1,+1,19)'da 21 L\* yüklemi sağlar; geri kalan 18'i bedelini karşılayacak ek bilgi
taşımaz. El dilinin aynı kuralla bedeli **81,36 bittir**; Kur'an'ın el dilindeki bilgisi ise 33,9 bittir (M20). El dili D'nin
seçebileceği bir dil değildir. **Dil yazımının kazancı** G = log₁₀ p^el − log₁₀ p^kendi (mertebe). M20 plasebolarında medyan **1,47**
(çeyrekler 0,97–2,03; en küçük −0,94, en büyük 4,75); tekdüzede 1,38; Mallows'ta 1,43. Kur'an'da kazanç küçüktür: G_Q = 0,27
(M20; el dilinde 6,4×10⁻⁷, kendi dilinde 3,4×10⁻⁷), tekdüzede 0,08, Mallows'ta 0,27 (el dilinde 1,5×10⁻⁷, kendi dilinde
8,0×10⁻⁸). Plasebolar kendi dillerini yazınca el diline göre tipik olarak ~1,5 mertebe kazanır. Kur'an'ın el dilindeki sırası ise kendi otomatik dilindekiyle hemen hemen aynıdır.

**3. Bütünlük şartlı ayrışmada tablo değişiyor mu?** Ön-kayıtlı biçimiyle **ölçülemedi; koşul tanımsız kaldı.** D(Kur'an)'ın üç
yüklemi iki düzeyden gelir: kitap (B2) ve hücre (K1, Qc). Sınıf kuralı ≥ 3 düzey ister, bu yüzden Kur'an kendi dilinde sınıf dışıdır.
Ön-kayıt bu durumda sınıf şartlı kalibrasyonun tanımsız olduğunu ve öyle yazılacağını söyler (§9 c). Hiçbir plasebo dili de sınıfa
girmez: M20'de 0/200, tekdüzede 0/100, Mallows'ta 0/100. D'nin dilleri bunun için fazla küçüktür (plasebolarda ortalama |Λ| ≈ 1,0–1,1).
Betimsel ek, el dilinde: Kur'an sınıftadır; sınıf şartıyla M20'de 4 / 10⁸ kontrol kalır, p^c = 5,0×10⁻⁸ [1,1×10⁻⁸; 1,0×10⁻⁷]
(şartsız 6,4×10⁻⁷). El dilinde sınıfa giren plasebo M20'de 0/200, tekdüzede 2/100, Mallows'ta 1/100. Sınıftaki bu üç plasebonun
en küçük p^c'si 7,5×10⁻⁴'tür; Kur'an'ınki tekdüzede 3,0×10⁻⁸, Mallows'ta 1,0×10⁻⁸'dir (0 / 10⁸). Bu ek, plasebo kalibrasyonu
değildir: plasebolar burada Kur'an'ın dilinde ölçülür.

**4. Ana hattın tahmini tuttu mu?** **Evet; dört parçası da tuttu** (M20; ön-kayıt §10 kurallarıyla):

| Parça | Tahmin | Ölçülen | Hüküm |
|---|---|---|---|
| p_Q | merkez 10⁻⁷, aralık 10⁻⁹–10⁻⁶ | 3,40×10⁻⁷ [2,27; 4,63]×10⁻⁷ (33 / 10⁸); merkezden 0,53 mertebe | tuttu |
| plasebo p_x medyanı | 10⁻³–10⁻¹ | 1,35×10⁻² [9,9×10⁻³; 1,8×10⁻²] | tuttu |
| en küçük plasebo p_x | 10⁻⁶–10⁻⁴ | 3,0×10⁻⁶ [1,9×10⁻⁶; 4,2×10⁻⁶] | tuttu |
| hepsini aşar | p_kal = 1/201 | R = 0 → 1/201 | tuttu |

**5. Ölçülmeyenler** (§9): kütüphanenin sınırları (L\*'ın dışındaki yüklem aileleri, 406 mercekli mercek ailesi, bölme seçimi);
literatürün on yıllar süren araması; nesnenin (n, a) seçimi; sayım okulu; kültürel havuz; gruplar arası bağımsızlık yaklaşımının etkisi;
referans tablosu gürültüsünün p_x'e etkisi; dil bedelinin biçimi. Hepsi **ölçülmedi**.

---

## 1. Ön-kayıt, tahmin ve okuma kuralı

### 1.1 Dondurma ve dondurmadan önce yapılanlar
Ön-kayıt kütüphaneyi ve üretim kuralını, dil bedelini, D'yi, hedef sayılarını, bütçeleri, derinlik kuralını, tohumları, ayrışma
istatistiğini, çözülmemiş aramaların kuralını, model-tabanlı fark modelini ve karar kurallarını sabitledi. Dondurma anındaki 30 kaynak
dosyanın SHA-256 özetleri `sonuc/onkayit_kod_sha256.txt`'tedir (oturum sonunda 30/30 değişmemiş; §10). Dondurmadan önce yalnız görevin
izin verdikleri yapıldı (ONKAYIT §0): kod doğrulamaları, örnekleyici eşitlikleri, sabit/hep-doğru yüklem taraması ve hız ölçümü. Bunlardan
yalnız uyuşmazlık sayıları, bayraklar ve süreler basıldı. Kütüphane kodu Kur'an'da dondurmadan sonra ilk kez çalıştırıldı.

**Açıklama (ön-kayıt §0'dan aynen aktarılan bir not).** D'nin grup başına aday sınırını boyutlandırırken, yayımlanmış 12. den.
raporundaki Kur'an sayılarından birkaç kardeşin doğruluk değerini zihnen çıkardım. Örneğin eşit sütun ve köşegen sayılarını hücre
boylarından (3/3/3/3), "her yarı toplamı = 2p" kardeşini yarılardan (38/38) çıkardım. Bu, görevin "Kur'an'ın kütüphane kardeşlerinden
hangilerini sağladığı dondurmadan önce hesaplanmaz" kuralının ruhuna aykırı bir düşünce adımıdır. Kütüphane kodu Kur'an'da
çalıştırılmadı ve Kur'an'ın profili hesaplanmadı. Kütüphane, üretim kuralı, dışlama listesi ve bedel ağacı bu adımdan önce yazılmış ve
sınanmıştı; adımdan sonra hiçbiri değişmedi. O sırada değişen tek şey D'nin grup başına aday sınırıdır (20 → 24). Sınır büyüdükçe D her
kitapta tam en iyiye yaklaşır; bu, Kur'an'a özgü bir seçim değildir. Sınırın sonuçta hiç devreye girip girmediği §4.3'tedir.

### 1.2 Ana hattın tahmini (ön-kayda aynen yazıldı)
Birincil sıfır M20'de:
- D(Kur'an) diliyle Kur'an'ın kendi kontrollerine karşı sırası p_Q için merkez **10⁻⁷**, aralık **10⁻⁹–10⁻⁶**.
- 200 plasebo kitabın p_x medyanı **10⁻³–10⁻¹**, en küçük p_x **10⁻⁶–10⁻⁴**.
- Kur'an'ın ayrışması 200 plasebonun **hepsini aşar** (kalibre p = 1/201).

### 1.3 Okuma kuralı (ön-kayda aynen yazıldı)
> Kur'an'ın ayrışması bütün plasebo kitaplarınkini aşarsa, bu basamağın sırası dilin Kur'an'a göre yazılmasıyla açıklanamaz; kalibre p en küçük ~1/(K + 1) olabilir (K, plasebo kitap sayısı) ve aradaki fark ayrıca, model-tabanlı olarak verilir. Plasebo kitaplar Kur'an'ın ayrışmasına sık ulaşırsa, bu basamağın nadirliği büyük ölçüde dilin Kur'an'a göre yazılmasından gelir ve öyle yazılır. Arada kalan sonuçlar kalibre p'leriyle aynen yazılır.

İşletimi (ONKAYIT §8.2): R = 0 → ilk cümle; R/K ≥ 0,05 → ikinci cümle; arada kalan her sonuç p_kal ile aynen yazılır.

---

## 2. Yöntem (özet; tam tanımlar `ONKAYIT.md`'de)

### 2.1 Kütüphane L\* (109 yüklem)
Görevdeki (a)–(d) listesi bir üretim kuralına çevrildi: **düzey** (kitap, aile, hücre, birleşim) × **tür** (denge, modülün geri dönüşü,
iki el, ardışıklık) × **nicelik** × **ilişki**. Bütün nicelik–ilişki çiftleri üretildi. Her mercekte imkânsız olanlar analitik gerekçeyle
düşüldü: eşit Σn, eşit Σt (Bt), kitabın a-yarılarının bütün ilişkileri, n-yarılarında p² |, kitap yarılarında = p, = 2p ve merkez = p,
FHp ve FKp. Özgün 20 yüklem tanımları ve bit sıralarıyla aynen kütüphanededir. Tanımların başvurusu `src/kutuphane.py`'dir;
`src/kutuphane.h` onunla bit-bit eşittir (§3).

**Özgün yüklemlerin genelleştirilmesi** (aynı tür, aynı düzey; ONKAYIT §2.4):

| Özgün | Genelleştirme | Kardeş |
|---|---|---|
| B1, B2 (kitap dengesi) | eşit-toplam kardeşi Ba (Σn ve Σt kardeşleri imkânsız) | 1 |
| F1, F2 (aile yarıları) | aynı iki ilişki F sütunlarında, F köşegenlerinde ve H satırlarında | 6 |
| R0c, R0e, R1c, R1e (satır hücreleri) | aynı dört yüklem H'nin satır hücrelerinde | 4 |
| K1 (hücre toplamları ardışık) | aritmetik dizi, hücre boyları ardışık, sabit hücre; satır ve sütun toplamları ardışık | 5 |
| K2 (katsayı bandı) | H katsayı bandı | 1 |
| Qc (hücre merkezi p) | hücre toplamları her biri 2p, p \|, p² \|; hücre boylarında 5 ilişki | 8 |
| QS (p \| Σq) | Σq = p, = 2p, p² \| | 3 |
| Qh (p \| yarı toplamları) | yarı toplamlarında 3 ilişki; sütun toplamlarında 5; yarı boylarında 5 | 13 |
| QkF (k_F = p) | k_F = 2p, p \|, p² \| | 3 |
| W1 (iki elin ayrıklığı) | \|F∩H\| = 1, k_F = k_H, k_F + k_H = p | 3 |
| QU (\|U\| = p) | \|U\| = 2p, p \|, p² \| | 3 |
| QkH (k_H = p) | k_H'de üç ilişki; \|F∩H\|'de dört | 7 |
| Qa (p \| Σ_U a) | Σ_U a'da üç ilişki; Σ_H a'da dört; Σ_F a'da dört | 11 |
| Qn (p \| Σ_U n) | Σ_U n'de üç; Σ_H n'de dört; Σ_F n'de dört; Σ_U t'de dört | 15 |
| QH (p \| Σ_H r) | Σ_H r'de üç ilişki | 3 |
| — (yeni nicelik: kitap yarılarının toplamları) | BMn, BMt, BMt2 | 3 |
| **Toplam** | 20 özgün + 89 kardeş | **109** |

Gruplar (bilgi puanı için): B kitap (7 bit, 6'sı L\*'da), Z aile boyu (4), F aile-içi | k_F (52), W iki el ve birleşim | k_F (40), H karşı
aile içi | k_H (7). Permütasyonla değişmeyen yüklem–mercek çiftleri analitik listeyle dışlandı (ONKAYIT §2.5). Tarama bu listeyi doğruladı:
6×10⁶ kitapta dışlanmamış hep-doğru çift 0.

### 2.2 Bilgi puanı
S_x(Λ) = max_λ [I_Λ(λ) − c(λ)]. Bu, 12. den.'in V0 puanıdır, mercek önseli P0 ile. Mercekler λ = (α, β, p): 14 doğrusal biçim × 29 asal
(3–113) = 406. t = αn + βa, u = αn − βa; F = {p | t}, H = {p | u}. Bölme çifti (t-paritesi, n-paritesi) sabittir. I_Λ beş grubun grup
içi tam birlikte "en-az" sıklıklarının −log₂ toplamıdır. Sıklıklar her sıfır için ayrı tohumlu 10⁸ kitaplık bir referans örnekleminin
seyrek tam-maske sayımlarından okunur. Bağlamlar 12. den.'in kuralıyla kurulur (k_F ve k_H kutuları; çekirdekte ≥ 10⁴ kitap, kuyruklar
k mod 4 havuzu). Gruplar arası çarpım bir yaklaşımdır (ölçülmedi; §9).

**Çözülmemiş aramalar ve doyma için tek kural.** c ≥ 10 ise b = log₂ n − log₂ c. c < 10 ise b = log₂(n + 1) − log₂(c + 0,5)
(12. den.'in nokta değeri). Kural D'de, hedefte ve her kontrolde aynen kullanılır. Eşitlik hedefin aleyhine sayılır (≥ − 10⁻⁶ bit).

### 2.3 Dil bedeli ve D
Yüklem bedeli c(π), üretim ağacındaki kod uzunluğudur: her düğümde (düzey → tür → nicelik → ilişki) L\*'da kalan seçenekler
üzerinde tekdüze; Kraft toplamı 1. Dil bedeli c(Λ) = log₂ 21 + Σ c(π) − log₂ |Λ|! (sırasız küme kodu). Yüklem başına bedel 4,0–8,7
bittir (`sonuc/agac_bedeller.txt`). **El dilinin 20 yüklemi aynı kuralla: Σ c(π) = 138,05 bit, c(Λ_el) = 81,36 bit.**
D(x) = argmax_{Λ⊆L\*, |Λ|≤20} [S_x(Λ) − c(Λ)]. Arama tamdır: en iyi Λ, en iyi mercekte x'in sağladığı yüklemlerin bir alt kümesidir.
Her mercek ve her grup için bütün alt kümeler taranır (grup başına en çok 24 aday). Gruplar |Λ| ≤ 20 altında boyut bileşimleriyle tam
birleştirilir. Eşitlik bozma: küçük |Λ|, sonra küçük mercek indeksi, sonra sözlükte küçük yüklem listesi. D'ye ne Kur'an'ın metni, ne el
dili, ne herhangi bir sonuç girer.

### 2.4 Hedefler, kontroller, ayrışma
- **Hedefler.** Kayıt 0 Kur'an'dır. M20'de K = 200 plasebo vardır; her biri kendi M20 örnekleyicisinden gelir (64 yeni zincir, 30.000 adım
  yakma, ilk 6.400 emisyon atılır, sonraki hedeftir). Tekdüzede ve Mallows'ta K = 100'dür; her biri kendi tohumunun ilk kitabıdır.
  Hedefler süzülmedi; çekilen her plasebo raporlanır (dil dosyaları pakette).
- **Kontroller.** Her sıfırda tek akış vardır: 400 bölüt × 2,5×10⁵ kitap (M20'de her bölüt yeni 64 zincirle başlar). Tohumlar
  referanstan ve hedeflerden bağımsızdır. Her kontrol, hedefin donmuş dilinde kendi en iyi merceğini seçer.
- **Derinlik.** Bütün plasebolar 10⁶ kontrolle sınanır; k < 10 kalanlar 10⁷'ye, orada da k < 10 kalanlar 10⁸'e iner. Kur'an'ın dili, P2
  dili ve el dili doğrudan 10⁸ kontrolle sınanır.
- **Ayrışma.** k_x = #{kontrol : S_c(Λ_x) ≥ S_x(Λ_x) − 10⁻⁶}, p_x = (k_x + 1)/(N_x + 1). Kuyruk dış değerlemesi yapılmaz.
- **Kalibre p.** R = #{plasebo : p_x ≤ p_Q ya da aralığı Kur'an'ınkiyle çakışır}; p_kal = (1 + R)/(K + 1).
- **Model-tabanlı fark** (kararlarda kullanılmaz). y = −log₁₀ p. Birincil model, plasebo y'lerinin üst %20'sine GPD uyumudur (en çok
  olabilirlik; 2.000 parametrik önyükleme). İkincil model, bütün y'lere normal uyumdur. Betimsel fark Δ = y_Q − max y_x.

Tohumlar: tohum = 29102600000 + sıfır·10⁸ + tür·10⁷ + sıra (U = 1, M = 2, L = 3; tür 1 referans, 2 kontrol, 3 hedef, 4 doğrulama,
5 tarama, 6 hız, 8 mekanik sınama).

---

## 3. Ö0 — çapalar

| Çapa | Sonuç |
|---|---|
| Örnekleyiciler (U, M20, Mallows), 12. den.'in `butunluk.h`'sinin değiştirilmemiş kopyasıyla aynı tohumda | 3 × 2.000 kitapta **bit-bit aynı** (0 uyuşmazlık) |
| Kütüphane 20 özgün yükleme sınırlandırılınca maskeler, k_F, k_H, en küçük hücre (406 mercek) | rastgele 6.000 kitapta 0 uyuşmazlık (dondurmadan önce); **Kur'an'da 0 uyuşmazlık** (dondurmadan sonra ilk iş; `sonuc/o0_kuran.txt`) |
| Kur'an'ın el dili profili | (1,+1,19)'da **15 yüklem**: B1 B2 F1 F2 R0c R1c R1e K1 K2 Qc QS Qh W1 QU Qa; k_F = 12, k_H = 7, en küçük hücre 3 |
| Kütüphanenin C ↔ Python eşitliği (112 aday yüklem) | 1.200 rastgele + 1.200 O-havuzu kitabında 0 uyuşmazlık |
| Sabit / hep-doğru yüklem taraması | 6×10⁶ kitapta dışlanmamış hep-doğru çift 0 (tek bayrak FC1 @ p = 3, \|β\| = 3 analitik listeye eklendi); Bt, FHp, FKp her mercekte imkânsız (L\* dışı) |
| Yeni referans tablosuyla Kur'an'ın el dilindeki bilgi puanı — M20 | **tuttu**: 26,58 bit (12. den. 27,41) |
| aynısı — tekdüze | **tuttu**: 30,06 bit (12. den. 31,06) |

Ön-kayıtlı ölçüt şudur: Kur'an'ın en iyi merceğindeki her arama için (B, F, W) yeni en-az sayısı, 12. den.'in tablo sayısıyla
iki-örneklem koşullu binom sınamasında (iki yanlı) p ≥ 0,001 vermelidir. Tekdüzenin F araması ayrıca 12. den.'in adanmış kestirimiyle
Poisson tutarlılığına bakılarak sınanır. Kur'an'ın en iyi merceği (1,+1,19), k_F = 12'dir (`sonuc/o0_bilgi_{M,U}.txt`).

| Sıfır | Arama | 12. den. tablosu | yeni tablo | koşullu binom p |
|---|---|---|---|---|
| M20 | B (B1 ∧ B2) | 46.833 / 10⁸ | 46.672 / 10⁸ | 0,60 |
| M20 | F (11 bitlik el dili maskesi), k_F = 12 | 24 / 1.202.540 | 42 / 1.203.170 | 0,036 |
| M20 | W (6 bitlik) | 4.529 / 1.202.540 | 4.621 / 1.203.170 | 0,35 |
| Tekdüze | B | 31.637 / 10⁸ | 31.288 / 10⁸ | 0,17 |
| Tekdüze | F | 2 / 1.014.579 | 4 / 1.014.992 | 0,69 |
| Tekdüze | F, adanmış kestirimle | P = 2,307×10⁻⁶ (46 / 2×10⁷) → beklenen 2,34 | 4 | Poisson 0,31 |
| Tekdüze | W | 3.900 / 1.014.579 | 4.110 / 1.014.992 | 0,020 |

Puan farkları M20'de −0,83 bit, tekdüzede −1,00 bittir. Aramaların Poisson gürültüsü M20'de σ ≈ 0,37 bit, tekdüzede σ ≈ 1,25 bittir.
M20'deki fark neredeyse tümüyle aile-içi aramadan gelir (24 → 42 isabet). Bu iki bağımsız 10⁸'lik örneklemin gürültüsüdür. Bu
oturumdaki bütün puanlar yeni tablolarla hesaplandı. Bu yüzden Kur'an'ın el dilindeki M20 sırası burada 12. den.'inkinden yüksektir:
6,4×10⁻⁷ (63 / 10⁸), 12. den.'de 2,3×10⁻⁷. Tekdüzede 6,0×10⁻⁸ (5 / 10⁸), 12. den.'de 4,0×10⁻⁸. Mallows için ön-kayıtlı bir bilgi
çapası yoktur; Mallows örnekleyicisi bit-bit çapadadır. Betimsel olarak, Kur'an'ın el dilindeki Mallows sırası burada 1,5×10⁻⁷
(14 / 10⁸), 12. den.'de 4,7×10⁻⁷'dir.

---

## 4. Diller

### 4.1 Kur'an'ın dili
D(Kur'an) = **{B2, K1, Qc} @ (1,+1,19)**, üç sıfırda da. D her sıfırda o sıfırın kendi referans tablosuyla ayrı ayrı uygulandı.

| Bileşen (M20) | Arama | Bit |
|---|---|---|
| kitap: B2 (çapraz terazi), (1,+1) biçiminde | 123.118 / 10⁸ | 9,666 |
| aile-içi \| k_F = 12: K1 ∧ Qc | 97 / 1.203.170 | 13,599 |
| toplam bilgi | | 23,264 |
| dil bedeli: log₂ 21 + 4,585 (B2) + 6,170 (K1) + 6,585 (Qc) − log₂ 3! | | 19,147 |
| mercek bedeli (P0): (1,+1) biçimi + eşit asal | | 7,317 |
| amaç | 23,264 − 19,147 − 7,317 | −3,200 |
| **eşik** S_Q(Λ_Q) = bilgi − mercek bedeli | | **15,947** |

Tekdüzede aramalar B 112.329 / 10⁸ (9,798 bit) ve F 22 / 1.014.992 (15,494 bit) çıkar; amaç −1,173, eşik **17,974 bit**.
Mallows'ta aramalar B 122.713 / 10⁸ (9,671 bit) ve F 48 / 1.307.394 (14,733 bit) çıkar; amaç −2,061, eşik **17,086 bit**. Aramaların
hepsi çözülmüştür (c ≥ 10).

Kur'an (1,+1,19)'da 21 L\* yüklemi sağlar: el dilinin 15'i ile FC1, FD1, FH2p, FSC, WSum, WUd. D bunlardan 3'ünü seçti. Kalan 18
yüklemin her biri bedelini (4,6–8,7 bit) karşılayacak kadar ek bilgi taşımaz. Bilgileri seçilmiş yüklemlerle büyük ölçüde örtüşür;
örneğin K1 ∧ Qc, F2'yi ve R1e'yi cebirsel olarak gerektirir.

**Köken notu.** Seçilen üç yüklemin Kur'an'daki içeriği büyük ölçüde önceki literatürdür (ana hattın *Köken* bölümü,
`~/Desktop/hrf2/README.md`). **B2**, (n + a)'nın 1994'te yayımlanmış parite terazisidir: Σ_{t çift} n = Σ_{t tek} a ⇔ çift
t'lerin toplamı ayet sayısına eşittir. **Qc**'nin p = 19'daki içeriği (dört hücre dolu ve Σq = 4·19 = 76), 1990'da yayımlanmış
"katsayılar toplamı 76, aile toplamı 1444"tür. **K1**'in Kur'an'daki hücre toplamları 18/19/19/20'dir. Bunların iki 19'u (2×361) en geç
2015'te yayımlanmıştı; 18/20 bu çalışmanındır. D bu yüklemleri literatürü bilmeden, yalnız bilgi − bedel ölçütüyle seçti. Ama bu
kalibrasyon literatürün aramasını bedellendirmez (§8, §9).

### 4.2 Plasebo dilleri

| | M20 (K = 200) | Tekdüze (K = 100) | Mallows (K = 100) |
|---|---|---|---|
| \|Λ\| dağılımı | 0: 16, 1: 156, 2: 26, 3: 1, 5: 1 | 0: 11, 1: 72, 2: 14, 3: 1, 4: 2 | 0: 12, 1: 75, 2: 12, 3: 1 |
| ortalama \|Λ\| | 1,08 | 1,11 | 1,02 |
| grup kullanımı (yüklem) | B 9, F 110, W 87, H 10 | B 5, F 58, W 41, H 7 | B 2, Z 1, F 48, W 46, H 5 |
| el dili yüklemi içeren dil | 32 / 200 | 18 / 100 | 12 / 100 |
| eşik S_x(Λ_x): medyan; en büyük | 1,25; 23,42 bit | 1,06; 22,79 bit | 1,47; 11,64 bit |
| amaç: medyan (en küçük; en büyük) | −10,30 (−11,71; −4,62) | −10,21 (−11,71; +2,89) | −10,23 (−11,71; −4,93) |
| en iyi mercekte çözülmemiş arama (c < 10) | 1 / 200 (x021: F 3 / 165.976) | 0 / 100 | 1 / 100 (x046: F 7 / 5.610.919) |
| kendi dilinde sınıfta | 0 / 200 | 0 / 100 | 0 / 100 |

Boş dil (|Λ| = 0), hiçbir yüklemin bedelini karşılamadığı anlamına gelir. Boş dilde bütün kontroller hedefle eşit puan alır ve p_x = 1
olur. Plasebo dillerinin hepsi `tab/<sıfır>/diller.txt`'te, ayrıntıları `tab/<sıfır>/ayrinti.txt` ve `hedef_ayrinti.txt`'tedir.
Diller kontrol geçişinden önce donduruldu; SHA-256'ları `sonuc/diller_<sıfır>_sha256.txt`'tedir (M20 `a55236f9…`, tekdüze
`61c82759…`, Mallows `ec36dd60…`).

### 4.3 Aday sınırı (ön-kayıt §4.3'ün raporlama yükümlülüğü)
D, bir grupta sağlanan yüklem 24'ü aşarsa yalnız tekil bilgisi en yüksek 24 yüklemi tarar; tek yaklaşık adım budur.
Sınır **hiçbir hedefte devreye girmedi**. Üç sıfırın 403 hedef kaydında (her sıfırda Kur'an ve plasebolar), 406 mercekte ve 5
grupta, bir grupta sağlanan L\* yüklemi en çok 15'tir (M20: B 4, Z 2, F 14, W 15, H 6; tekdüze: 3, 2, 15, 11, 6; Mallows: 4, 2,
15, 13, 6). 24'ü aşan (hedef, mercek, grup) üçlüsü yoktur (`kontrol/aday_siniri.c`, `sonuc/<sıfır>/aday_siniri.txt`). Sayım, D'nin
aday seçimine giren maskenin aynısını kullanır. Bu yüzden D her hedefte tam en iyiyi buldu. Dondurmadan önce sınırın 20'den 24'e
çıkarılması (§1.1) da hiçbir sonucu etkilemedi: sınır 20 olsaydı da hiçbir grupta aşılmazdı.

---

## 5. Sonuçlar

### 5.1 M20 (birincil)
**Plasebolar (K = 200).** 198'i 10⁶ kontrolde (k ≥ 10), 2'si 10⁷'de durdu; 10⁸'e inen olmadı.

| İstatistik | Değer |
|---|---|
| p_x medyanı (100. ve 101. sıra istatistiğinin geometrik ortalaması) | **1,35×10⁻²** [%95: p_(86) – p_(115) = 9,9×10⁻³; 1,8×10⁻²] |
| %10 / %25 / %75 / %90 | 4,0×10⁻⁴ / 3,6×10⁻³ / 4,5×10⁻² / 9,7×10⁻² |
| en küçük p_x | **3,0×10⁻⁶**: x183 {WSum, WHRd2, HH2} @ (1,+2,19); 29 / 10⁷; [1,9×10⁻⁶; 4,2×10⁻⁶] |
| sonraki dört | 1,3×10⁻⁵ (x028, 130 / 10⁷), 1,4×10⁻⁵ (x074, 13 / 10⁶), 1,6×10⁻⁵ (x152, 15 / 10⁶), 2,4×10⁻⁵ (x120, 23 / 10⁶) |
| p_x < 10⁻⁴ / 10⁻⁵ / 10⁻⁶ | 8 / 1 / 0 |
| p_x = 1 (boş dil) | 16 / 200 |

Dil boyu ile ayrışma güçlü biçimde ilişkilidir. |Λ| = 1 olan 156 dilin medyanı 1,5×10⁻², |Λ| = 2 olan 26 dilinki 4,2×10⁻⁴'tür. Tek
|Λ| = 3 dili x183'tür (3,0×10⁻⁶); tek |Λ| = 5 dili x120'dir (2,4×10⁻⁵).

**Kur'an.** Eşik 15,947 bit. 10⁸ kontrolde (bölüt 1–400) **k = 33** → **p_Q = 3,40×10⁻⁷ [2,27×10⁻⁷; 4,63×10⁻⁷]**. İsabetler 32 farklı
(bölüt, zincir) çiftine dağılır (en çok 2'si aynı zincirde); zincir kümelenmesi yoktur. Eşiği aşan ya da eşleyen 33 kontrolün hiçbiri üç
yüklemi birden taşımaz. En yüksekleri iki yüklem taşır: (1,+1,13)'te B2 ∧ Qc (20,41 bit); küçük asallarda (p = 5, 7) B2 ∧ K1 ya da
K1 ∧ Qc (16,6–18,2 bit). Küçük asallarda büyük ailelerde (k_F = 18–26) ardışık hücre toplamları çok nadirdir ve tek başına çok bit
verir (`sonuc/M/rakipler.txt`).

**Kalibre p.** R = 0. En yakın plasebonun aralığı [1,9×10⁻⁶; 4,2×10⁻⁶], Kur'an'ınki [2,3×10⁻⁷; 4,6×10⁻⁷]; çakışma yoktur.
→ **p_kal = 1/201 = 4,98×10⁻³**. Okuma: **"Kur'an'ın ayrışması bütün plasebo kitaplarınkini aşar"**; ilk cümle uygulanır (§8).

**Model-tabanlı fark** (önceden yazılmış; kararlarda kullanılmaz). y_Q = 6,47, en uç plasebo y = 5,52; betimsel fark **0,95 mertebe**.
GPD uyumu üst 40 plaseboyla yapıldı (eşik u = 2,72): σ = 1,21, ξ = −0,36. Bu sınırlı bir kuyruktur, uç noktası y = 6,05'tir. Kur'an
uç noktanın ötesindedir → P_model = 0; parametrik önyükleme %95 aralığı [0; 3,1×10⁻⁴]. İkincil normal model (μ = 1,98, σ = 1,06)
P = 1,1×10⁻⁵ verir. İki model birbirinden çok farklıdır. "0" bir olanaksızlık değildir, uyumun uç noktasıdır. Model-tabanlı okuma en çok
şunu söyler: Kur'an'ın ayrışması, plasebo dağılımının uzantısında 10⁻⁴'ten seyrek bir yerdedir; bu sayı modele bağlıdır.

### 5.2 Tekdüze (ikincil)
**Plasebolar (K = 100).** 97'si 10⁶'da, 1'i 10⁷'de, 2'si 10⁸'de durdu.

| İstatistik | Değer |
|---|---|
| p_x medyanı | **1,06×10⁻²** [6,2×10⁻³; 2,4×10⁻²] |
| %10 / %25 / %75 / %90 | 2,8×10⁻⁴ / 3,2×10⁻³ / 4,9×10⁻² / 1 |
| en küçük p_x | **1,0×10⁻⁷**: x068 {F2, K2, FSC, WUN2p} @ (3,+2,43); 9 / 10⁸; [4,1×10⁻⁸; 1,7×10⁻⁷] |
| sonraki iki | 5,7×10⁻⁷ (x088 {K1} @ (1,+3,3); 56 / 10⁸), 6,1×10⁻⁶ (x098 {FSC, HK2} @ (1,−2,11); 60 / 10⁷) |
| p_x < 10⁻⁴ / 10⁻⁵ / 10⁻⁶ | 7 / 3 / 2 |
| p_x = 1 (boş dil) | 11 / 100 |

x068'in puanı küçük bir ailede (k_F = 5, k_H = 1) iki çözülmüş aramadan gelir. Aile-içi F2 ∧ K2 ∧ FSC 76 / 7.732.562 isabet verir
(16,63 bit); birleşim WUN2p (Σ_U n = 2p = 86) 85 / 7.732.562 isabet verir (16,47 bit). Amacı −2,53, eşiği 22,79 bittir. Kapı G tutmaz
(k_F·p = 215 < 228, boş hücre); x068 sınıf dışıdır.

**Kur'an.** Eşik 17,974 bit. **k = 4 / 10⁸** → **p_Q = 5,0×10⁻⁸ [1,1×10⁻⁸; 1,0×10⁻⁷]**. Eşiği aşan dört kontrolden biri üç yüklemi
birden taşır: bölüt 388, (1,+1,17), k_F = 10, 18,68 bit. Ötekiler iki yüklem taşır: (1,−1,3)'te B2 ∧ K1 (20,10 ve 19,02 bit) ve
(2,−3,5)'te B2 ∧ Qc (19,29 bit) (`sonuc/U/rakipler.txt`).

**Kalibre p.** x068'in nokta tahmini (1,0×10⁻⁷) Kur'an'ınkinden (5,0×10⁻⁸) büyüktür. Ama aralığının alt ucu (4,1×10⁻⁸), Kur'an'ın
aralığının üst ucunun (1,0×10⁻⁷) altındadır; aralıklar çakışır. Ön-kayıt gereği Kur'an'ın aleyhine sayılır: **R = 1 → p_kal = 2/101 =
0,0198**. R/K = 0,01 < 0,05 olduğundan sonuç **"arada"**dır ve kalibre p'siyle aynen yazılır. (Yalnız nokta tahminleri sayılsaydı R = 0
olurdu. Bu, ön-kayıtlı okuma değildir.)

**Model-tabanlı fark.** y_Q = 7,30, en uç plasebo 7,00; Δ = **0,30 mertebe**. GPD uyumu üst 20 plaseboyla yapıldı (u = 2,65):
σ = 1,32, ξ = −0,09 → P_model = **3,0×10⁻³** [önyükleme %95: 0; 1,4×10⁻²]. Normal model (μ = 2,03, σ = 1,26) P = 1,5×10⁻⁵ verir.

### 5.3 Mallows (ikincil; bütçe kuralıyla koşuldu)
**Plasebolar (K = 100).** 99'u 10⁶'da, 1'i (x046) 10⁷'de durdu; 10⁸'e inen olmadı.

| İstatistik | Değer |
|---|---|
| p_x medyanı | **1,40×10⁻²** [6,5×10⁻³; 1,8×10⁻²] |
| %10 / %25 / %75 / %90 | 6,3×10⁻⁴ / 2,5×10⁻³ / 5,2×10⁻² / 1 |
| en küçük p_x | **5,8×10⁻⁶**: x046 {FA2p, FAP} @ (2,+1,37); 57 / 10⁷; [4,3×10⁻⁶; 7,4×10⁻⁶] |
| sonraki iki | 3,5×10⁻⁵ (x090 {FSC, WI1} @ (3,+1,5); 34 / 10⁶), 5,8×10⁻⁵ (x060 {FC2, FSC, HK2} @ (1,−1,7); 57 / 10⁶) |
| p_x < 10⁻⁴ / 10⁻⁵ / 10⁻⁶ | 3 / 1 / 0 |
| p_x = 1 (boş dil) | 12 / 100 |

x046'nın puanı küçük bir ailedeki (k_F = 6, k_H = 1) tek bir aile-içi aramadan gelir: ailenin ayet toplamı 2p = 74'tür (FA2p) ve hücre
toplamları aritmetik dizidir (FAP). Arama 7 / 5.610.919 isabet verir, yani çözülmemiştir (c < 10); ön-kayıtlı kural
(b = log₂(n + 1) − log₂(c + 0,5)) 19,51 bit verir. Amacı −6,76, eşiği 11,20 bittir. Kapı G tutmaz (k_F·p = 222 < 228, en küçük hücre 1);
x046 sınıf dışıdır.

**Kur'an.** Eşik 17,086 bit. **k = 7 / 10⁸** → **p_Q = 8,0×10⁻⁸ [2,8×10⁻⁸; 1,4×10⁻⁷]**. Eşiği aşan ya da eşleyen 7 kontrolün hiçbiri
üç yüklemi birden taşımaz. Altısı B2 ∧ K1 taşır: ikisi p = 113'te küçük ailelerde (k_F = 4–5; en yükseği (1,+3,113)'te 19,40 bit),
dördü küçük asallarda büyük ailelerde (p = 5, 7; k_F = 14–30). Biri (1,+1,13)'te K1 ∧ Qc taşır (17,37 bit) (`sonuc/L/rakipler.txt`).

**Kalibre p.** R = 0. En yakın plasebonun aralığı [4,3×10⁻⁶; 7,4×10⁻⁶], Kur'an'ınki [2,8×10⁻⁸; 1,4×10⁻⁷]; çakışma yoktur.
→ **p_kal = 1/101 = 9,90×10⁻³**. Okuma: **"Kur'an'ın ayrışması bütün plasebo kitaplarınkini aşar"**; ilk cümle uygulanır (§8).

**Model-tabanlı fark** (kararlarda kullanılmaz). y_Q = 7,10, en uç plasebo 5,24; Δ = **1,86 mertebe**. GPD uyumu üst 20 plaseboyla
yapıldı (u = 2,73): σ = 0,75, ξ = −0,10 → P_model = **4,0×10⁻⁵** [önyükleme %95: 0; 2,6×10⁻³]. Normal model (μ = 1,93, σ = 1,07)
P = 6,9×10⁻⁷ verir.

**El dili ve P2.** Kur'an'ın el dilindeki eşiği 28,09 bittir: 14 / 10⁸ → 1,5×10⁻⁷ [7,7×10⁻⁸; 2,3×10⁻⁷]. 12. den.'in Mallows sırası
4,7×10⁻⁷'dir. Mallows için ön-kayıtlı bir bilgi çapası yoktur; fark, iki bağımsız referans tablosunun farkıdır (§8.4). P2 ile
4 / 10⁸ → 5,0×10⁻⁸ (§6.5).

**Bütçe.** Mallows ön-kayıtlı kuralla koşuldu (§10): karar anında kestirim ≈ 17–18 ≤ 40 çekirdek-saat. Gerçekleşen hesap 4,88
çekirdek-saattir.

### 5.4 Özet tablo
`src/rapor_tablo.py` bu tabloları sonuç dosyalarından üretir (`sonuc/rapor_tablolar.md`).

| Sıfır | K | Kur'an dili Λ_Q (mercek) | eşik (bit) | k_Q / N | p_Q [%95] | plasebo p medyanı [%95] | en küçük plasebo p [aralık] | R | p_kal | okuma |
|---|---|---|---|---|---|---|---|---|---|---|
| M20 (birincil) | 200 | {B2, K1, Qc} (1,+1,19) | 15,947 | 33 / 10⁸ | **3,40×10⁻⁷** [2,27×10⁻⁷; 4,63×10⁻⁷] | 1,35×10⁻² [9,87×10⁻³; 1,79×10⁻²] | 3,00×10⁻⁶ [1,94×10⁻⁶; 4,16×10⁻⁶] | 0 | **4,975×10⁻³** | aşar |
| Tekdüze | 100 | {B2, K1, Qc} (1,+1,19) | 17,974 | 4 / 10⁸ | **5,00×10⁻⁸** [1,09×10⁻⁸; 1,02×10⁻⁷] | 1,06×10⁻² [6,19×10⁻³; 2,43×10⁻²] | 1,00×10⁻⁷ [4,12×10⁻⁸; 1,71×10⁻⁷] | 1 | **1,980×10⁻²** | arada |
| Mallows | 100 | {B2, K1, Qc} (1,+1,19) | 17,086 | 7 / 10⁸ | **8,00×10⁻⁸** [2,81×10⁻⁸; 1,44×10⁻⁷] | 1,40×10⁻² [6,51×10⁻³; 1,83×10⁻²] | 5,80×10⁻⁶ [4,32×10⁻⁶; 7,39×10⁻⁶] | 0 | **9,901×10⁻³** | aşar |

| Sıfır | model-tabanlı: GPD P [önyükleme %95] | GPD (u, σ, ξ) | normal P (μ, σ) | betimsel fark Δ (mertebe) |
|---|---|---|---|---|
| M20 (birincil) | 0 [0; 3,08×10⁻⁴] | (2,72; 1,21; −0,36) | 1,12×10⁻⁵ (1,98; 1,06) | 0,95 |
| Tekdüze | 2,99×10⁻³ [0; 1,39×10⁻²] | (2,65; 1,32; −0,09) | 1,52×10⁻⁵ (2,03; 1,26) | 0,30 |
| Mallows | 4,01×10⁻⁵ [0; 2,58×10⁻³] | (2,73; 0,75; −0,10) | 6,91×10⁻⁷ (1,93; 1,07) | 1,86 |

| Sıfır | Kur'an el dilinde: k / N → p [%95] | dil yazımının kazancı G (mertebe): plasebo medyanı [çeyrekler]; Kur'an | P2 ek satırı: Λ, k / N → p |
|---|---|---|---|
| M20 (birincil) | 63 / 10⁸ → 6,40×10⁻⁷ [4,84×10⁻⁷; 8,06×10⁻⁷] | 1,47 [0,97; 2,03]; 0,27 | {B2, K1, Qc}, 24 / 10⁸ → 2,50×10⁻⁷ [1,54×10⁻⁷; 3,57×10⁻⁷] |
| Tekdüze | 5 / 10⁸ → 6,00×10⁻⁸ [1,62×10⁻⁸; 1,17×10⁻⁷] | 1,38 [0,87; 2,23]; 0,08 | {B2, K1, Qc}, 3 / 10⁸ → 4,00×10⁻⁸ [6,19×10⁻⁹; 8,77×10⁻⁸] |
| Mallows | 14 / 10⁸ → 1,50×10⁻⁷ [7,65×10⁻⁸; 2,35×10⁻⁷] | 1,43 [0,90; 1,97]; 0,27 | {B2, K1, Qc}, 4 / 10⁸ → 5,00×10⁻⁸ [1,09×10⁻⁸; 1,02×10⁻⁷] |

---

## 6. İkinciller

### 6.1 (a) El dili plasebolarda; dil yazımının kazancı
El dili (20 yüklem) bütün hedeflere aynı kontrol akışında, aynı derinlik kuralıyla uygulandı. Kazanç G_x = log₁₀ p_x^el − log₁₀ p_x
(mertebe); pozitif değer, kendi dilin el dilinden daha ayırt edici olduğunu gösterir.

| Sıfır | plasebo p^el: en küçük; medyan | G: medyan [çeyrekler]; en küçük; en büyük | Kur'an p^el (k / 10⁸) | Kur'an p^kendi | G_Q |
|---|---|---|---|---|---|
| M20 | 1,87×10⁻³ (x114); 0,445 | **1,47** [0,97; 2,03]; −0,94; 4,75 | 6,4×10⁻⁷ (63) [4,8; 8,1]×10⁻⁷ | 3,4×10⁻⁷ | **0,27** |
| Tekdüze | 7,1×10⁻⁵ (x088); 0,524 | 1,38 [0,87; 2,23]; −0,58; 5,05 | 6,0×10⁻⁸ (5) [1,6×10⁻⁸; 1,2×10⁻⁷] | 5,0×10⁻⁸ | 0,08 |
| Mallows | 1,34×10⁻² (x092); 0,470 | 1,43 [0,90; 1,97]; −1,53; 4,84 | 1,5×10⁻⁷ (14) [7,7×10⁻⁸; 2,3×10⁻⁷] | 8,0×10⁻⁸ | 0,27 |

El dilinde, yani 12. den.'in sorusunda ("rastgele kitaplar Kur'an'a göre yazılmış dilde ne kadar puan alıyor?"), M20'de hiçbir plasebo
Kur'an'ın p'sine ulaşmaz (0/200). En iyisi 1,87×10⁻³'tür. Tekdüzede (0/100; en iyisi 7,1×10⁻⁵) ve Mallows'ta (0/100; en iyisi
1,34×10⁻²) da ulaşan yoktur.

### 6.2 (b) D(Kur'an) ile el dili
Ortak yüklemler: B2, K1, Qc (el dilinin 20'sinden 3'ü). D(Kur'an)'da olmayan el dili yüklemleri: B1, QkF, F1, F2, R0c, R0e, R1c, R1e,
K2, QS, Qh, W1, QU, QkH, Qa, Qn, QH. Kur'an'ın (1,+1,19)'daki L\* profili 21 yüklemdir: el dilinin 15'i ile FC1, FD1, FH2p, FSC, WSum,
WUd (`sonuc/kuran_Lstar_profili.txt`). Kur'an'ın p'si:

| Sıfır | kendi dili {B2, K1, Qc} | el dili (20 yüklem) |
|---|---|---|
| M20 | 3,40×10⁻⁷ (33 / 10⁸) | 6,4×10⁻⁷ (63 / 10⁸) |
| Tekdüze | 5,0×10⁻⁸ (4 / 10⁸) | 6,0×10⁻⁸ (5 / 10⁸) |
| Mallows | 8,0×10⁻⁸ (7 / 10⁸) | 1,5×10⁻⁷ (14 / 10⁸) |

### 6.3 (c) Bütünlük şartlı ayrışma
Kural (ONKAYIT §9 c; 12. den. §7'nin genelleştirilmesi) şöyledir: en iyi mercekte (i) kapı G tutmalı (k_F·p ≥ 228 ve her F hücresi ≥ 2),
(ii) Λ'nın orada sağlanan yüklemleri ≥ 3 türden ve (iii) ≥ 3 düzeyden gelmelidir.
- **Kendi dillerinde.** Kur'an sınıf dışıdır: üç yüklem üç türden gelir (a: B2, d: K1, b: Qc), ama yalnız iki düzeyden (kitap, hücre).
  Ön-kayıt gereği **sınıf şartlı kalibrasyon tanımsızdır**. Kendi dilinde sınıfa giren plasebo dili: M20 0/200, tekdüze 0/100, Mallows
  0/100.
- **Betimsel ek (el dilinde; kalibrasyon değil).** Kur'an el dilinde sınıftadır. Sınıf şartıyla M20'de 4 / 10⁸ kontrol kalır →
  p^c = 5,0×10⁻⁸ [1,1×10⁻⁸; 1,0×10⁻⁷]. Tekdüzede 2 / 10⁸ → 3,0×10⁻⁸ [2,4×10⁻⁹; 7,2×10⁻⁸]. Mallows'ta 0 / 10⁸ →
  p^c = 1,0×10⁻⁸ [0; 3,7×10⁻⁸]; el dilinde eşiği aşan 14 kontrolün hiçbiri sınıfta değildir. El dilinde sınıfa giren plasebo: M20
  0/200, tekdüze 2/100 (x054: p^c = 7,5×10⁻⁴; x043: 3,3×10⁻³), Mallows 1/100 (x019: 1,6×10⁻³). Hiçbiri Kur'an'ın p^c'sine ulaşmaz;
  betimsel R^c her sıfırda 0'dır (`sonuc/<sıfır>/el_sinif.txt`).

Görevin amacı yüksek puanlı ama parçalı rakipleri bütünlüklü olanlardan ayırmaktı. Bu amaç D'nin dilleriyle gerçekleşemedi. MDL bedeli
altında diller 0–5 yüklemdir ve üç düzeye yayılamaz. Kur'an'ın kendi dili de buna dahildir. Sınıf şartı yalnız el dili gibi büyük bir
dilde anlam taşır; el dili ise D'nin seçebileceği bir dil değildir (§4.1).

### 6.4 (d) Tekdüze ve Mallows
§5.2 ve §5.3.

### 6.5 (e) Kur'an için P2 (ek satır; kalibrasyonsuz)
P2, 12. den.'in metin ağırlıklı asal önselidir: 1/3 eşit + 1/3 {3, 5, 7, 11, 19} + 1/3 {3, 5, 7, 19}. D(Kur'an) P2 mercek bedeliyle
yeniden kuruldu: dil aynıdır, {B2, K1, Qc} @ (1,+1,19); mercek bedeli 5,090 bittir. Kontroller P2 bedeliyle puanlandı.

| Sıfır | eşik | k / 10⁸ | p [%95] |
|---|---|---|---|
| M20 | 18,174 bit | 24 | 2,5×10⁻⁷ [1,5×10⁻⁷; 3,6×10⁻⁷] |
| Tekdüze | 20,202 bit | 3 | 4,0×10⁻⁸ [6,2×10⁻⁹; 8,8×10⁻⁸] |
| Mallows | 19,314 bit | 4 | 5,0×10⁻⁸ [1,1×10⁻⁸; 1,0×10⁻⁷] |

Plaseboların metni olmadığı için P2'nin kalibrasyonu yoktur.

### 6.6 (f) Bölme seçimi kütüphaneye
**Ölçülmedi.** Ön-kayıt bunu bütçe gerekçesiyle önceden "ölçülmeyecek" diye yazmıştı (§9 f). Her referans ve kontrol kitabı için 20
bölme çiftinin maskeleri gerekir; bu yaklaşık 20 kat hesap, sıfır başına ~45 çekirdek-saat demektir.

---

## 7. Tahminin değerlendirilmesi (M20; ön-kayıt §10)

| Parça | Kural | Ölçülen | Hüküm |
|---|---|---|---|
| p_Q | p̂ ∈ [10⁻⁹, 10⁻⁶] → tuttu | p̂ = 3,40×10⁻⁷ [2,27; 4,63]×10⁻⁷; \|log₁₀ p̂ + 7\| = 0,53 | **tuttu** |
| plasebo medyanı | nokta ∈ [10⁻³, 10⁻¹] → tuttu | 1,35×10⁻² [9,9×10⁻³; 1,8×10⁻²] | **tuttu** |
| en küçük p_x | nokta ∈ [10⁻⁶, 10⁻⁴] → tuttu | 3,0×10⁻⁶ [1,9×10⁻⁶; 4,2×10⁻⁶] | **tuttu** |
| hepsini aşar | R = 0 → tuttu | R = 0 | **tuttu** |
| **bütün tahmin** | dördü de tuttuysa "tuttu" | | **tuttu** |

Tahmin yalnız M20 içindi. Tekdüzede "hepsini aşar" parçası tutmazdı (R = 1), Mallows'ta tutardı (R = 0). Bunlar hüküm değil,
betimsel notlardır.

---

## 8. Okuma

**Birincil sıfır (M20), okuma kuralının ilk cümlesiyle:** Kur'an'ın ayrışması bütün plasebo kitaplarınkini aşar; bu basamağın sırası
dilin Kur'an'a göre yazılmasıyla açıklanamaz. Kalibre p = 1/201, yani K = 200 ile olabilecek en küçük değerdir. Aradaki fark
model-tabanlı olarak §5.1'dedir: betimsel 0,95 mertebe, GPD'de uç noktanın ötesi, normal modelde 1,1×10⁻⁵.

**İkincil sıfırlar.** Tekdüzede sonuç **arada** kalır: p_kal = 2/101 = 0,020 (R = 1, aralık çakışmasıyla). Mallows'ta Kur'an'ın
ayrışması 100 plasebonun hepsini aşar (R = 0). Okuma kuralının ilk cümlesi uygulanır: p_kal = 1/101 = 0,0099, K = 100 ile olabilecek
en küçük değer. Aradaki fark model-tabanlı olarak §5.3'tedir: betimsel 1,86 mertebe, GPD'de 4,0×10⁻⁵, normal modelde 6,9×10⁻⁷. Üç
sıfırın ikisinde R = 0, birinde (tekdüze) R = 1 çıkar. Tekdüzede çakışan plasebonun (x068) nokta tahmini Kur'an'ınkinin iki katıdır.

**Bu okumanın kapsamı.** Aşağıdakiler okuma kuralını değiştirmez; neyin kalibre edildiğini sınırlar.
1. **Bedellenen şey D'nin seçimidir.** Kalibrasyon, *bu* kütüphaneden (L\*), *bu* bedelle (c), *bu* mercek ailesiyle ve *bu* puanla
   yüklem seçmenin bedelini ölçer. L\*'ın üretim kuralı el dilinin 20 yükleminden genelleştirildi; el dili de Kur'an görüldükten sonra
   yazılmıştı. Kütüphanenin kendisi bu anlamda Kur'an'dan esinlenmiştir. L\*'ın dışında kalan yapılar (başka nicelikler, ilişkiler,
   bölmeler) plasebolara açık değildi. Bunun etkisi ölçülmedi (§9).
2. **D'nin dili, nesnenin bütünü değil, çekirdeğidir.** MDL bedeli altında D, Kur'an'ın sağladığı 21 yüklemden 3'ünü seçer. 12. den.'in
   "nadirlik tek bir parçada değil, bütündedir" okuması bu kalibrasyonla sınanmış olmaz. Bütün (el dili) D'nin seçebileceği bir dil
   değildir; el dili bedelini (81,36 bit) karşılamaz.
3. **Literatür.** D'nin seçtiği üç yüklemin Kur'an'daki içeriği büyük ölçüde 1990, 1994 ve en geç 2015 tarihli yayımlanmış
   gözlemlerdir (§4.1). Kalibrasyon bu gözlemleri ilk bulan insan aramasının bedelini ölçmez (ölçülmedi). Bu yüzden p_kal, "eski
   literatür verilince" sorusunun cevabı değildir.
4. **Tablo koşulu.** Puan işlevi sıfır başına tek bir referans tablosuna koşulludur. Ö0 çapaları tutsa da tablolar arası fark puanda
   ~1 bit, Kur'an'ın sırasında 2–3 kat oynayabilir (el dilinde M20: burada 6,4×10⁻⁷, 12. den.'de 2,3×10⁻⁷). p_x'ler aynı tabloyla
   hedefe ve kontrollere uygulandığı için geçerli sıralardır. Etkinin büyüklüğü ölçülmedi.

---

## 9. Ölçülmeyenler ve sınırlılıklar

**Ölçülmedi:**
- **Kütüphanenin sınırları.** L\* görevdeki (a)–(d) listesinin üretim kuralıyla kuruldu (109 yüklem). Dışında kalan aileler ölçülmedi:
  başka nicelikler (en büyük/en küçük q, farklı q sayısı, çarpımlar), başka ilişkiler (= 3p, p³ |, kalan sınıfları) ve başka bölmeler.
  406 mercekli mercek ailesi (14 doğrusal biçim × 29 asal, 3–113) değiştirilmedi. Bölme seçimi (Ö5 f) ölçülmedi (§6.6).
- **Literatürün on yıllar süren araması** — yayımlanmış iddiaların ve denenmiş örüntülerin sayısı ölçülmedi. Plasebo kalibrasyonu
  yalnız D'nin seçim bedelini ölçer.
- **Nesnenin (n, a) seçimi** — sûre numaraları ve ayet sayıları dışındaki girdiler ölçülmedi.
- **Sayım okulu** — Kûfe dışındaki ayet sayımları ölçülmedi; 127/129 ikiliğinden 127 ölçülmedi. Yalnız kodeks 129 kullanıldı.
- **Kültürel havuz** — başka metinler ve başka iddialar ölçülmedi.
- **Gruplar arası bağımsızlık yaklaşımının etkisi** — kitap ⊥ boy ⊥ aile-içi ⊥ iki el ⊥ karşı aile | bağlam varsayımının etkisi
  ölçülmedi. Yaklaşım bütün hedeflere ve kontrollere aynen uygulanır. Kalibrasyonu bozmaz, ama puanın "olasılık" yorumunu yaklaşık
  yapar.
- **Referans tablosu gürültüsünün p_x'e etkisi** — ölçülmedi (§8.4).
- **Dil bedelinin biçimi** — c(Λ) önceden yazılmış tek bir koddur. Başka makul kodların (ör. yüklem başına düz log₂ 109 bit, ya da
  daha ucuz bir dil bedeli) sonuca etkisi ölçülmedi. Daha ucuz bir bedel daha büyük diller seçtirir. Bu hem plasebolara hem Kur'an'a
  daha çok yüklem kazandırır, ama hangisine ne kadar kazandıracağı ölçülmedi.

**Sınırlılıklar:**
- p_Q, aynı sıfırdan 10⁸ kontrolle ölçüldü. Plasebolar en çok 10⁸'e indi. p < 10⁻⁸ düzeyindeki ayrışmalar ayırt edilemez.
- Kalibre p'nin çözünürlüğü 1/(K + 1)'dir: M20'de 1/201, ikincillerde 1/101. Daha küçük bir kalibre p için daha çok plasebo gerekir.
  Model-tabanlı fark bu sınırın ötesini yalnız bir modelle söyler ve iki model çok farklıdır (§5.1).
- D'nin dilleri küçük olduğundan sınıf kuralı uygulanamadı (§6.3).

---

## 10. Sapmalar, ekler, bütünlük ve hesap

**Ekler** (`ek/`):
- `EK_20261028T232150Z_dondurma_sonrasi_kod.md` — dondurmadan sonra eklenen kod (Ö0'ın Kur'an kısmı, bilgi çapası, ayrıntı ve analiz
  betikleri, yürütücüler). Hiçbiri tanım, ağırlık, eşik, bedel ya da kural değiştirmez. Sonuca etkisi yoktur.
- `EK_20261029T011946Z_eszamanlilik_asimi.md` — Mallows referans geçişi 4 süreçle koşarken bir betimsel özet betiği ön planda
  çalıştırıldı. Yaklaşık 3–4 dakika boyunca 5 hesap süreci koştu; "aynı anda en çok 4 iş parçacığı" sınırı aşıldı. Fark edilince betik
  durduruldu. Sonuçlara etkisi yoktur (deterministik tohumlar); yalnız kayıtlı süreler birkaç çekirdek-dakika şişmiş olabilir.
- `EK_20261029T013043Z_rapor_betikleri.md` — raporlama ve yeniden üretim için dondurmadan sonra eklenen betikler (`src/el_sinif.py`,
  `kontrol/aday_siniri.c`, `kos/derle.sh`, `kos/yeniden_sina.sh`). Sonuca etkisi yoktur.
- `EK_20261029T024324Z_kapanis.md` — Mallows sonrası raporlama adımları. Aday sayacına D'nin sabitini veren başlık eklendi.
  `src/rapor_tablo.py`'nin çıktı yeri değişti; bu betik önceki ekte unutulmuştu. Bölüm taslakları (`rapor_taslak/`) pakete konmadı.
  `~/Desktop/hrf2`'de salt okuma amaçlı bir `git status`, `.git` dizininin zamanını değiştirdi; hiçbir dosyanın içeriği değişmedi.
  Yerel ayar (`tr_TR`) yalnız bir konsol denetimini etkiledi. Sonuca etkisi yoktur.

**Kural değişikliği yok.** Herhangi bir hedefin profiline, diline ya da kontrol puanına bakıldıktan sonra hiçbir tanım, ağırlık, eşik,
bedel ya da kural değiştirilmedi. Düzeltme gerektiren bir hata bulunmadı.

**Bütünlük denetimi** (oturum sonunda): `ONKAYIT.md`'nin SHA-256'sı `sonuc/onkayit_ozet.txt`'tekiyle aynıdır ve dosya salt okunurdur.
Dondurma anındaki 30 kaynak dosya `shasum -a 256 -c sonuc/onkayit_kod_sha256.txt` ile 30/30 değişmemiştir; `GOREV.md`
`gorev_sha256.txt` ile eşleşir. Donmuş diller (`tab/<sıfır>/diller.txt`, `ayrinti.txt`) ve hedefler (`hedef.bin`) özetleriyle
eşleşir. Yeniden derleme sınamasında (`kos/yeniden_sina.sh` → `sonuc/yeniden_derleme.txt`) 10 karşılaştırmanın 10'u bit-bit aynıdır:
Ö0'ın Kur'an kısmı, üç sıfırın hedefleri, tekdüzede D (P0 ve P2), hedef ayrıntısı, bir kontrol bölütü, küçük bir referans geçişi ve
bilgi çapası. Kontrol geçişinin aday tamponu üç sıfırda da taşmadı (eşiğe 2 bitten yakın aday kaydı M20'de 494, tekdüzede 72,
Mallows'ta 185). `~/Desktop/hrf2`'nin çalışma ağacı temizdir (`git status --porcelain` boş). `~/opus5.5/butunluk`, `~/opus5.5/kosullu`
ve `~/opus5.5/plasebo`'da oturum başından daha yeni dosya yoktur.

**Hesap** (`src/hesap_saati.py`; kayıtlı süreler):

| Sıfır | referans (10⁸) | kontroller | toplam |
|---|---|---|---|
| M20 | 2,27 | 2,50 | 4,77 çekirdek-saat |
| Tekdüze | 2,29 | 2,45 | 4,74 |
| Mallows | 2,45 | 2,43 | 4,88 |
| kayıtsız (dondurma öncesi doğrulamalar, Ö0, hedef üretimi, analizler; kestirim) | | | ≤ 2 |
| **toplam** | | | **≤ 16,4** (kayıtlı 14,40 + kayıtsız ≤ 2) |

Tavan 50 çekirdek-saattir (hedef ~30). Mallows, ön-kayıtlı kuralla (kayıtlı toplam + kestirim ≤ 40) koşuldu: karar anında kayıtlı
9,51 + kayıtsız ≤ 2 + Mallows kestirimi ~5–6 ≈ 17–18 çekirdek-saat. Bellek, kontrol geçişinde dil tabloları için ≤ 3,5 GB idi. Disk
kullanımı oturum sonunda ~0,7 GB idi (`du -sh`: 676 MB; neredeyse tamamı seyrek referans tablolarıdır); tavan 8 GB.

---

## 11. Yeniden üretim ve paket

**Paket.** `paket/plasebo12_paket.tar.gz` (≈ 2,0 MB). SHA-256'sı `paket/plasebo12_paket.sha256`'dadır; paket bu raporu da
içerdiği için özet raporun içine yazılamaz. Paket `plasebo12/` klasörü olarak açılır. İçerik:
- bütün kaynaklar ve oturumda kullanılan ikililer (`src/`, `kontrol/`, `kos/`);
- `GOREV.md`, `gorev_sha256.txt`, `baslat.command`, `ONKAYIT.md`, bu rapor ve `ek/`;
- `sonuc/`: çapalar, analizler, özetler, özet listeleri, koşu günlüğü, ara notlar;
- her sıfırın `tab/<sıfır>/` dosyaları: hedefler `hedef.bin`, donmuş diller `diller.txt`, D ayrıntıları, hedef ayrıntıları, referans
  `.say` özetleri, eşikler `esikler.txt`, bütün kontrol bölütlerinin sayım dosyaları `b*.txt` ve aday dosyaları `b*.q`;
- `tab/tarama/` ve `tab/olcum/*.say` (dondurma öncesi tarama ve hız ölçümü).

Seyrek referans tabloları (`ref_*.ref`; sıfır başına ~165 MB) boyut sınırı yüzünden pakette yoktur. Tohumlardan deterministik olarak
yeniden üretilirler; SHA-256'ları `sonuc/ref_sha256.txt`'tedir.

**Komutlar** (Apple clang 17, arm64; `kos/derle.sh` bütün C programlarını derler):
```
cd ~/opus5.5/plasebo12
./kos/derle.sh                      # bütün C programları (src/, kontrol/)
mkdir -p tab/M tab/U tab/L sonuc
./kontrol/o0_kuran > sonuc/o0_kuran.txt         # Ö0, Kur'an kısmı (§3)
./src/hedef M 200 tab/M/hedef.bin; ./src/hedef U 100 tab/U/hedef.bin; ./src/hedef L 100 tab/L/hedef.bin   # SHA-256: sonuc/hedef_sha256.txt
./kos/akis.sh M                     # referans → Ö0 bilgi çapası → D ve dondurma → kontrol düzeyleri 1–3 → analiz
./kos/akis.sh U
./kos/akis.sh L
for S in M U L; do python3 -I src/rakipler.py $S; python3 -I src/dil_ozet.py $S; python3 -I src/el_sinif.py $S > sonuc/$S/el_sinif.txt
  ./kontrol/aday_siniri tab/$S/hedef.bin tab/$S/diller.txt > sonuc/$S/aday_siniri.txt; done
python3 -I src/hesap_saati.py; python3 -I src/rapor_tablo.py
./kos/yeniden_sina.sh /tmp/plasebo12_yeniden   # yeniden derleme sınaması (§10)
```
Yeniden üretim sıfır başına ~5 çekirdek-saattir (4 süreçle ~1,3 saat duvar saati). Python betikleri standart kitaplığın
yanında yalnız numpy kullanır. Yeniden üretilen referans tabloları `sonuc/ref_sha256.txt` ile karşılaştırılabilir. Oturumdaki
ikililerin derleme bayrakları kaydedilmemişti. `kos/derle.sh` ile yeniden derlenmiş ikililerin çıktıları oturumun çıktılarıyla
bit-bit aynıdır (`./kos/yeniden_sina.sh <geçici_dizin>`, ~72 s; `sonuc/yeniden_derleme.txt`: 10/10 AYNI). Oturumda kullanılan
ikililer de pakettedir (arm64, macOS).

---

## English summary

**Question.** The review-12 "integrity language" (20 predicates) was written after seeing the Quran. Its predicate choice was not
priced. This session prices it with a **placebo test**. Language writing becomes an automatic, deterministic procedure D. D picks,
from a pre-registered library L\* of 109 predicates, the subset that maximises information score minus description length. L\* holds
the 20 originals plus 89 siblings, generated by a level × type × quantity × relation rule. D is applied identically to the Quran and to
placebo books drawn from the null. Each target is tested in its own frozen language against its own 10⁶–10⁸ controls:
p_x = (k + 1)/(N + 1). The Quran's separation is then ranked among the placebos': p_cal = (1 + R)/(K + 1), with ties and interval
overlaps counted against the Quran. The pre-registration was frozen before any target's library profile, language or control score
was computed (SHA-256 `3837c9d8…71a5a`, 2026-10-28T22:33:04Z). One pre-freeze lapse is disclosed in §1.1: a mental derivation of a
few sibling truth values from published numbers.

**Results.** Primary null M20, K = 200. The placebos' own-language separations have median p_x = 1.35×10⁻² [9.9×10⁻³; 1.8×10⁻²]. The
smallest is 3.0×10⁻⁶ (29 / 10⁷); none of 200 is below 10⁻⁶. D(Quran) = {B2, K1, Qc} at (1,+1,19), i.e. the cross parity balance,
consecutive cell sums and centre 19. In this language the Quran has p_Q = 3.40×10⁻⁷ (33 / 10⁸; [2.27; 4.63]×10⁻⁷). **It exceeds all
200 placebos: R = 0, p_cal = 1/201 = 0.0050.** This is the smallest value possible with K = 200, so the first sentence of the
pre-registered reading rule applies: the rank of this rung cannot be explained by the language having been written for the Quran.
The model-based gap is not used for decisions: 0.95 orders of magnitude beyond the most extreme placebo; beyond the endpoint of a GPD
tail fit (bootstrap 95% upper bound 3.1×10⁻⁴); 1.1×10⁻⁵ under a normal fit. Uniform null, K = 100: p_Q = 5.0×10⁻⁸ (4 / 10⁸). One
placebo (1.0×10⁻⁷, 9 / 10⁸) overlaps the Quran's interval, so R = 1 and p_cal = 2/101 = 0.020, an in-between result reported as such.
Mallows null, K = 100: p_Q = 8.0×10⁻⁸ (7 / 10⁸). The closest placebo is 5.8×10⁻⁶ (57 / 10⁷) with no interval overlap, so **R = 0
and p_cal = 1/101 = 0.0099**, the smallest value possible with K = 100, and the first sentence of the reading rule applies. The
model-based gap is 1.86 orders of magnitude descriptively, 4.0×10⁻⁵ under GPD and 6.9×10⁻⁷ under a normal fit. D(Quran) is the same
language and lens under all three nulls.

**D does not recover the hand language.** It keeps 3 of its 20 predicates. Under the same code the hand language costs 81.4 bits,
more than its information on the Quran (33.9 bits). Language writing gains the placebos a median 1.47 orders of magnitude over the
hand language (M20); the Quran gains only 0.27. **The integrity (class) condition could not be evaluated** in the pre-registered
form. D's languages are too small (mean |Λ| ≈ 1.0–1.1 for placebos; 3 predicates on 2 levels for the Quran) to span ≥ 3 levels, so the
class-conditional calibration is undefined. Descriptively, in the hand language the Quran is in class (p^c = 5.0×10⁻⁸ under M20).
No M20 placebo is; the 3 in-class placebos under the secondary nulls have p^c ≥ 7.5×10⁻⁴. **The main line's prediction held in all
four parts** (p_Q in 10⁻⁹–10⁻⁶, placebo median in 10⁻³–10⁻¹, minimum in 10⁻⁶–10⁻⁴, all exceeded).

**Scope.** The calibration prices D's selection from L\*. L\* itself was generalised from the Quran-derived hand language. D's three
predicates are largely prior literature (1990, 1994, by 2015), and the decades-long human search that found them is not priced.
**Not measured:** the limits of the library (other predicate families, the 406-lens family, the choice of split); the literature's
search; the choice of the object (n, a); the counting school (codex 127); the cultural pool; the effect of the between-group
independence approximation; the effect of reference-table noise on p_x; the form of the language cost.

**Compute and package.** 14.40 logged core-hours (M20 4.77, uniform 4.74, Mallows 4.88) plus at most 2 unlogged. Binaries rebuilt
with `kos/derle.sh` reproduce the session outputs bit for bit (10/10 checks). The package is `paket/plasebo12_paket.tar.gz`; its
SHA-256 is in `paket/plasebo12_paket.sha256`.
