# ÖN-KAYIT — plasebo12: bütünlük dilini her hedef kendisi kursa, Kur'an'ın ayrışması ne kadar sıra dışı?

**Oturum:** Claude (Opus 5.5), `~/opus5.5/plasebo12`, görev `GOREV.md` (29 Ekim 2026; SHA-256 `da4604f1…ef08`, `gorev_sha256.txt` ile
eşleşti). `~/Desktop/hrf2`, `~/opus5.5/butunluk`, `~/opus5.5/kosullu` yalnız okundu; `~/opus5.5/plasebo` hiç açılmadı (görevin girdisi değil).
**Bu belge hiçbir hedefin (Kur'an dahil) kütüphane profiline, D sonucuna ya da kontrol puanına bakılmadan yazıldı ve donduruldu.**
Dondurma anı ve SHA-256: `sonuc/onkayit_ozet.txt`; dondurma anındaki kod özetleri: `sonuc/onkayit_kod_sha256.txt`. Sapmalar yalnız
`ek/` altındaki zaman damgalı eklerde yazılır; bu dosya değiştirilmez.

Kısaltmalar: on ikinci denetim = 12. den. (`~/opus5.5/butunluk`, ön-kayıt SHA-256 `d213dc89…fd513`); el dili = 12. den.'in 20 yüklemli dili.

---

## 0. Dondurmadan önce yapılanlar (ve yapılmayanlar)

Yapıldı (görevin izin verdiği biçimde; çıktılar yalnız uyuşmazlık sayıları, bayraklar ve süre/bellek):
- **Ö0'ın rastgele-kitap kısmı** (`kontrol/o0_karsilastir.c`): kod-doğrulama tohumlarından (tür 4) U, M20 ve Mallows'tan 2.000'er kitap.
  (i) Örnekleyiciler 12. den.'in `butunluk.h`'sinin değiştirilmemiş kopyasıyla (`src/b12/`) aynı tohumda **bit-bit aynı** (0 uyuşmazlık);
  (ii) kütüphane özgün 20 yükleme sınırlandırılınca maskeler, k_F, k_H ve en küçük hücre 406 mercekte **bit-bit aynı** (6.000 kitap ×
  406 mercek, 0 uyuşmazlık). **Kur'an bu sınamaya dondurmadan önce girmedi** (kütüphane kodu Kur'an'da hiç çalıştırılmadı; §7.1).
- **Kütüphanenin C ↔ Python eşitliği** (`kontrol/c_dok*.c`, `kontrol/c_py_esit.py`): 112 aday yüklem, 406 mercek, k_F, k_H, en
  küçük hücre; tür 4 tohumlarından 1.200 rastgele kitap (U, M20, Mallows) ve 10. denetimin O-havuzlarından (n + a, 19)'da eski
  literatür yapısını taşıyan 1.200 rastgele kitap (U129O, M129O ×2, L129O; `~/opus5.5/kosullu/tab/havuz`, salt okunur): **0 uyuşmazlık**.
  Bozma sınaması (tek bit çevrildi) uyuşmazlığı buldu.
- **Sabit / hep-doğru yüklem taraması** (`kontrol/sabit_tarama.c`, `sonuc/sabit_tarama_bayraklar.txt`): tür 5 tohumlarından
  6×10⁶ rastgele kitap (U, M20, Mallows'tan 2×10⁶'şar), dışlama uygulanmadan ham maskeler; yalnız bayraklar basıldı. İlk turda bir
  bayrak: FC1 p = 3, |β| = 3'te hep doğru (F = {3 | n}, sütunlar 19/19) → analitik listeye eklendi (§2.5). Son durum: analitik
  listede olmayan hep-doğru çift **0**; analitik listede olup sabit olmayan çift **0**; hiçbir mercekte hiç doğru olmayan yüklemler
  Bt, FHp, FKp — üçünün her mercekte imkânsız olduğu analitik olarak gösterildi ve L* dışında bırakıldı (§2.2).
- **D yordamının mekanik sınamaları** (`kontrol/dil_mekanik.c`, `kontrol/dil_kaba.c`): ölçüm referansıyla (tür 6, 2×10⁶ M20 kitabı)
  ve hedef olmayan tür 4 kitaplarıyla: üst-küme sayımları 32.272 sınamada doğrudan sayımla aynı; D'nin amacı 300/300 kitapta dilin
  doğrudan yeniden hesaplanan puanıyla aynı; sağlanan yüklem birleşimi ≤ 14 olan 121.579 mercekte kaba-kuvvet taraması D'den iyi bir
  alt küme bulmadı (0/300). Dil, puan ya da eşik basılmadı.
- **Kontrol programının mekanik sınaması** (`kontrol/kontrol_sinama.c`): tür 8 tohumlarından 16.000 kontrol, hedef olmayan 31
  yedek kitabın (tür 4) D dilleri ve el dili; programın sayımları bağımsız, doğrudan seyrek listelerden (double) yeniden sayımla
  karşılaştırıldı: **0 uyuşmazlık**. Sayımlar ve eşikler basılmadı.
- **Hız ve bellek ölçümü** (`src/referans`, tür 6 tohumları; yalnız süre ve seyrek girdi sayısı): referans kitap başına ~83 µs
  (bütün biçimler; 4 süreçle duvar saati ~24 µs/kitap); seyrek (mercek, k-kutusu, grup, maske) girdileri 5×10⁵ kitapta 1,56×10⁶,
  2×10⁶ kitapta 2,36×10⁶ (büyüme ~N^0,30; 10⁸'de beklenen ~10⁷ mertebesi). Kontrol başına yalın değerlendirme 70,9 µs (`kontrol/hiz.c`),
  dil başına puanlama birkaç µs.
- **Bedel ağacı** (`src/agac.py`): L*'ın 109 yüklemi için c(π) ve el dilinin bedeli (§4.1).

**Açıklama (kendi davranışım hakkında).** D'nin aday sınırını (§4.3) boyutlandırırken, yayımlanmış 12. den. raporundaki Kur'an
sayılarından (hücreler 3/3/3/3, toplamlar 20/18/19/19, yarılar 38/38, Σq = 76) birkaç kardeşin doğruluk değerini zihnen çıkardım
(ör. eşit sütun ve köşegen sayıları 3/3/3/3'ten, "her yarı toplamı = 2p" 38/38'den). Bu, görevin "Kur'an'ın kütüphane kardeşlerinden
hangilerini sağladığı dondurmadan önce hesaplanmaz" kuralının ruhuna aykırı bir düşünce adımıdır; kütüphane kodu Kur'an'da
çalıştırılmadı ve Kur'an'ın profili hesaplanmadı. Bu adımdan **önce** kütüphane, üretim kuralı, analitik dışlama listesi ve bedel ağacı
yazılmış ve sınanmıştı; adımdan sonra hiçbiri değiştirilmedi. O sırada değiştirilen tek şey D'nin grup başına aday sınırıdır (20 → 24):
sınır büyüdükçe D her kitap için tam en iyiye daha yakın olur; Kur'an'a özgü bir seçim değildir. Bu not raporda da yer alır.

Yapılmadı: hiçbir hedef (Kur'an dahil) çekilmedi ya da kütüphaneyle değerlendirilmedi; hiçbir hedefin dili kurulmadı; üretim referans
ve kontrol tohumları (tür 1–3) hiç kullanılmadı; hiçbir kontrol puanı hesaplanmadı; Kur'an'ın el dilindeki bilgi puanı yeni tabloyla
hesaplanmadı.

---

## 1. Ana hattın önceden kaydettiği tahmin ve okuma kuralı (aynen)

**Okuma kuralı** (görevden aynen; "Depoda 27 Ekim'de önceden ilan edilen okuma kuralı (not 25; ön-kayda aynen yaz, değiştirme)"):
> Kur'an'ın ayrışması bütün plasebo kitaplarınkini aşarsa, bu basamağın sırası dilin Kur'an'a göre yazılmasıyla açıklanamaz; kalibre p en küçük ~1/(K + 1) olabilir (K, plasebo kitap sayısı) ve aradaki fark ayrıca, model-tabanlı olarak verilir. Plasebo kitaplar Kur'an'ın ayrışmasına sık ulaşırsa, bu basamağın nadirliği büyük ölçüde dilin Kur'an'a göre yazılmasından gelir ve öyle yazılır. Arada kalan sonuçlar kalibre p'leriyle aynen yazılır.

**Ana hattın önceden kaydettiği tahmin** (görevden aynen; "ön-kayda aynen yaz, sonda değerlendir"):

Birincil sıfır M20'de:
- D(Kur'an) diliyle Kur'an'ın kendi kontrollerine karşı sırası p_Q için merkez **10⁻⁷**, aralık **10⁻⁹–10⁻⁶**.
- 200 plasebo kitabın p_x medyanı **10⁻³–10⁻¹**, en küçük p_x **10⁻⁶–10⁻⁴**.
- Kur'an'ın ayrışması 200 plasebonun **hepsini aşar** (kalibre p = 1/201).

Okuma kuralının işletilmesi §8.2'de, tahminin değerlendirme kuralları §10'dadır.

---

## 2. Kütüphane L*

Tanımların başvurusu `src/kutuphane.py`'dir; `src/kutuphane.h` onunla bit-bit eşittir (§0). Kodeks 129 (Σa = 6236); 127 ölçülmez.

### 2.1 Mercekler, bölme, kümeler (12. den. ile aynı)
Kitap = konum n = 1..114'e atanmış ayet sayıları a_n. Mercek λ = (α, β, p): t = αn + βa, u = αn − βa; 14 biçim (α, |β| ∈ {1,2,3},
aralarında asal, β'nin iki işareti; (1,±1), (1,±2), (1,±3), (2,±1), (2,±3), (3,±1), (3,±2)) × 29 asal (3…113) = 406 mercek.
Bölme çifti (D1, D2) = (t-paritesi, n-paritesi), birincilde sabit (bölme seçimi yalnız ikincil; §9 f). E⁰ = {D1 = 0}, E¹ = tümleyen.
F = {p | t}, q = t/p; H = {p | u}, r = u/p; U = F ∪ H; k_F = |F|, k_H = |H|. Hücreler F^{ij} = {n ∈ F : D1 = i, D2 = j}, sayıları c_ij,
q-toplamları s_ij. **Satırlar** (= aile yarıları) F^{i·}, **sütunlar** F^{·j} = F^{0j} ∪ F^{1j}, **köşegenler** F^{=} = F⁰⁰ ∪ F¹¹ ve
F^{≠} = F⁰¹ ∪ F¹⁰. H için aynı satır ve hücreler (r ile). t ≡ u (mod 2) olduğundan H'nin satırları da t-paritesine göredir.

### 2.2 Üretim kuralı (düzey × tür × nicelik × ilişki)
Kütüphane tek tek seçilmedi; görevdeki (a)–(d) listesi aşağıdaki kurala çevrildi ve **bütün** nicelik–ilişki çiftleri üretildi:

- **Düzeyler (4; 12. den.'in sınıf kuralındaki gibi):** kitap, aile (F ya da H ailesi ve yarıları; katsayılar), hücre, birleşim (iki el, U, H'nin toplamları).
- **Türler (4):** (a) denge, (b) modülün geri dönüşü, (c) iki el, (d) ardışıklık.
- **(a) Denge — nicelik = bölme, ilişki ∈ {eşit sayı, eşit toplam}:** kitapta E bölmesi (eşit sayı B1, eşit toplam = 12. den.'in B2'si,
  eşit-toplam kardeşleri: eşit Σa, eşit Σn, eşit Σt); ailede F satırları, F sütunları, F köşegenleri, H satırları; hücrede F satır-0 ve
  satır-1 hücreleri, H satır-0 ve satır-1 hücreleri. "Eşit sayı": iki parçanın boyları eşit ve ≥ 1; "eşit toplam": iki parça dolu ve
  q- (H'de r-) toplamları eşit.
- **(b) Modülün geri dönüşü — nicelik × ilişki ∈ {= p, = 2p, merkez = p, p | ·, p² | ·}:** skaler niceliklerde = p, = 2p, p |, p² |;
  demet niceliklerde (bütün parçalar dolu olmak koşuluyla) her biri = p, her biri = 2p, merkez = p (parçaların ortalaması p, yani
  toplam = parça sayısı × p), p | her biri, p² | her biri. Skaler toplamlarda ilgili küme dolu olmalıdır. Nicelikler (görevin listesi):
  boylar k_F, k_H, |U|, |F ∩ H|, yarı boyları (|F⁰|, |F¹|), hücre boyları (c_ij); toplamlar Σq, yarı toplamları (satır toplamlarıyla
  aynı nicelik — F'nin satırları t-paritesi yarılarıdır, tek kez sayılır), sütun toplamları, hücre toplamları, Σ_U a, Σ_U n, Σ_U t,
  Σ_F a, Σ_F n, Σ_H a, Σ_H n, Σ_H r ve kitap yarılarının toplamları (Σ_{E^i} n, Σ_{E^i} a, Σ_{E^i} t).
- **(c) İki el:** F ∩ H = ∅ (W1, 12. den.: k_F, k_H ≥ 1), |F ∩ H| = 1, k_F = k_H (≥ 1), k_F + k_H = p (ikisi ≥ 1).
- **(d) Ardışıklık:** F hücre toplamlarının farklı değerleri kesintisiz ve ≥ 2 farklı (K1), F katsayılarının bandı (K2), H katsayı
  bandı (r'ler; k_H ≥ 2), satır toplamlarının ardışıklığı (iki yarı dolu, |Σq(F⁰) − Σq(F¹)| = 1), sütun toplamlarının ardışıklığı
  (aynı tanım), hücre toplamlarının aritmetik dizi oluşu (dört hücre dolu; sıralı toplamlar ortak farkı ≥ 1 olan dizi), hücre
  boylarının ardışıklığı (dört hücre dolu; farklı boylar kesintisiz, ≥ 2 farklı), sabit hücre (en az bir hücre ≥ 2 üyeli ve bütün
  q'ları eşit; dış çalışmanın tanımı).

**Üretilip L* dışında bırakılanlar (her mercekte imkânsız; kodeks 129):** eşit Σn (Σn = 6555 tek); eşit Σt [Bt] (α tekte Σt tek;
α = 2'de E⁰ = {a çift} ve 4·Σ_{E⁰} n = 13110 + β(6236 − 2·Σ_{a çift} a) ≡ 2 (mod 4)); E yarılarının a-toplamlarının bütün ilişkileri
(p | ikisi ⇒ p | 6236 = 2²·1559, 3 ≤ p ≤ 113'te yok); n-yarılarında p² | ikisi (6555 = 3·5·19·23 kare çarpansız); kitap yarılarında
= p, = 2p, merkez = p (toplamlar ≫ 4p); F yarı toplamlarında "her biri = p" [FHp] ve F hücre toplamlarında "her biri = p" [FKp]
(t-çift satırda q = t/p çift, p tek). **L* = 109 yüklem** (hedef aralık 60–150). Değerlendirme kodu Bt, FHp, FKp bitlerini taşır ama
her mercekte sıfırlar.

### 2.3 Yüklem listesi ve gruplar
Bilgi puanının grupları (12. den. gibi; H-içi grup eklendi; 12. den.'in koşulsuz "boy" terimi Z grubuna genelleşti):
**B** kitap (mercek başına, bağlam yok), **Z** aile boyu (yalnız k_F'nin işlevleri; koşulsuz), **F** aile-içi | k_F, **W** iki el ve birleşim
| k_F, **H** karşı aile içi | k_H. Grup içinde tam birlikte sıklık; gruplar arası çarpım bir yaklaşımdır.

| Grup | Yüklemler (kod: tanım) |
|---|---|
| B (6) | **B1** \|E⁰\| = \|E¹\|; **B2** 12. den.'in B2'si (E⁰, E¹ dolu ve αΣ_{E⁰}n = \|β\|Σ_{E¹}a ∨ αΣ_{E¹}n = \|β\|Σ_{E⁰}a ∨ Σ_{E⁰}t = Σ_{E¹}t); Ba Σ_{E⁰}a = Σ_{E¹}a; BMn p \| Σ_{E⁰}n ve p \| Σ_{E¹}n; BMt p \| Σ_{E⁰}t ve p \| Σ_{E¹}t; BMt2 p² \| ikisi |
| Z (4) | **QkF** k_F = p; Zk2p k_F = 2p; Zkd p \| k_F (k_F ≥ 1); Zkd2 p² \| k_F |
| F (52) | **F1 F2** (satırlar: eşit sayı / toplam); **R0c R0e R1c R1e** (satır hücreleri); **K1** (hücre toplamları ardışık); **K2** (katsayı bandı); **Qc** (dört hücre dolu, Σq = 4p); **QS** (p \| Σq); **Qh** (p \| iki yarı toplamı); FC1 FC2 (sütunlar); FD1 FD2 (köşegenler); FSp FS2p FSd2 (Σq = p, = 2p, p² \|); FH2p FHm FHd2 (yarı toplamları her biri 2p, merkez p, p² \| her biri); FCp FC2p FCm FCd FCd2 (sütun toplamları); FHSp FHS2p FHSm FHSd FHSd2 (yarı boyları); FAp FA2p FAd FAd2 (Σ_F a); FNp FN2p FNd FNd2 (Σ_F n); FRK FCK (satır / sütun toplamları ardışık); FK2p FKd FKd2 (hücre toplamları her biri 2p, p \|, p² \|); FKSp FKS2p FKSm FKSd FKSd2 (hücre boyları); FAP (hücre toplamları aritmetik dizi); FKS (hücre boyları ardışık); FSC (sabit hücre) |
| W (40) | **W1** (F ∩ H = ∅); **QU** (\|U\| = p); **QkH** (k_H = p); **Qa** (p \| Σ_U a); **Qn** (p \| Σ_U n); **QH** (p \| Σ_H r); WI1 (\|F∩H\| = 1); WEq (k_F = k_H); WSum (k_F + k_H = p); WU2p WUd WUd2 (\|U\|); WH2p WHd WHd2 (k_H); WIp WI2p WId WId2 (\|F∩H\|); WUAp WUA2p WUAd2 (Σ_U a); WUNp WUN2p WUNd2 (Σ_U n); WUTp WUT2p WUTd WUTd2 (Σ_U t); WHAp WHA2p WHAd WHAd2 (Σ_H a); WHNp WHN2p WHNd WHNd2 (Σ_H n); WHRp WHR2p WHRd2 (Σ_H r) |
| H (7) | HH1 HH2 (H satırları: eşit sayı / toplam); HR0c HR0e HR1c HR1e (H satır hücreleri); HK2 (H katsayı bandı) |

Kalın = özgün 20 yüklem (tanımları ve bit sıraları 12. den. ile aynı).

### 2.4 Özgün yüklemlerin genelleştirilmesi
| Özgün | Genelleştirme (aynı tür, aynı düzey) | Kardeş sayısı |
|---|---|---|
| B1, B2 (kitap dengesi) | eşit-toplam kardeşi Ba (Σn ve Σt kardeşleri imkânsız) | 1 |
| F1, F2 (aile yarıları) | aynı iki ilişki F sütunlarında, F köşegenlerinde ve H satırlarında | 6 |
| R0c, R0e, R1c, R1e (satır hücreleri) | aynı dört yüklem H'nin satır hücrelerinde | 4 |
| K1 (hücre toplamları ardışık) | aritmetik dizi, hücre boyları ardışık, sabit hücre; satır ve sütun toplamları ardışık | 5 |
| K2 (katsayı bandı) | H katsayı bandı | 1 |
| Qc (hücre merkezi p) | hücre toplamları her biri 2p, p \|, p² \|; hücre boyları 5 ilişki | 8 |
| QS (p \| Σq) | Σq = p, = 2p, p² \| | 3 |
| Qh (p \| yarı toplamları) | yarı toplamları 3 ilişki; sütun toplamları 5; yarı boyları 5 | 13 |
| QkF (k_F = p) | k_F = 2p, p \|, p² \| | 3 |
| W1 (iki elin ayrıklığı) | \|F∩H\| = 1, k_F = k_H, k_F + k_H = p | 3 |
| QU (\|U\| = p) | \|U\| = 2p, p \|, p² \| | 3 |
| QkH (k_H = p) | k_H üç ilişki; \|F∩H\| dört ilişki | 7 |
| Qa (p \| Σ_U a) | Σ_U a üç ilişki; Σ_H a dört; Σ_F a dört | 11 |
| Qn (p \| Σ_U n) | Σ_U n üç; Σ_H n dört; Σ_F n dört; Σ_U t dört | 15 |
| QH (p \| Σ_H r) | Σ_H r üç ilişki | 3 |
| — (yeni nicelik: kitap yarılarının toplamları) | BMn, BMt, BMt2 | 3 |
Toplam 20 özgün + 89 kardeş = 109.

### 2.5 Sabit (dışlanan) yüklem–mercek çiftleri
Permütasyonla değişmeyen çiftler hiçbir kitapta puan almaz (`dislama_hazirla`, `dislanan`); özgün yüklemler için 12. den.'in listesi aynen:
- **α ya da β çift:** B1 (12. den.). **α çift:** Ba (E⁰ = {a çift} ⇒ Σ_{E⁰}a sabit). **β çift:** BMn (E⁰ = {n çift}, toplamlar sabit 3306/3249);
  D1 = D2 olduğundan F⁰¹ = F¹⁰ = ∅ ⇒ FD1, FD2, FK2p, FKd, FKd2, FKS*, FAP, FKS ve HR0c, HR0e, HR1c, HR1e sabit-yanlış.
- **p ∤ 6555:** BMn; **p ∤ Σt** (Σt = 6555α + 6236β): BMt; **p² ∤ Σt:** BMt2.
- **p = 3, |β| = 3 (F = H = {3 | n}; k = 38, Σn = 2223 = 3²·13·19):** QU, QkF, QkH, Qn, W1 (12. den.); FC1 (sütunlar 19/19, tarama
  bayrağı); Z'nin dördü; WI1, WEq, WSum, \|U\|, k_H, \|F∩H\| ilişkileri; Σ_U n, Σ_H n, Σ_F n ilişkileri; WUTd; yarı ve hücre boyu ilişkileri.
- **p = 3, α = 3 (F = H = {3 | a}; k = 39 = 3·13, Σa = 2193 = 3·17·43):** QU, QkF, QkH, Qa, W1 (12. den.); Z'nin dördü; WI1, WEq, WSum,
  \|U\|, k_H, \|F∩H\| ilişkileri; Σ_U a, Σ_H a, Σ_F a ilişkileri; WUTd; yarı ve hücre boyu ilişkilerinden "= p", "= 2p", merkez, p² \|.
- Bt, FHp, FKp her mercekte (L* dışı).
Tarama (§0): 6×10⁶ kitapta dışlanmamış hep-doğru çift 0; dışlanan çiftlerin hepsi taramada sabit.

---

## 3. Bilgi puanı S_x(Λ) (12. den.'in V0 puanı; mercek önseli P0)

### 3.1 Referans örneklemi
Her sıfır için, hedeflerden ve kontrollerden **ayrı tohumlu** 10⁸ kitap (20 bölüt × 5×10⁶; tohum türü 1). Her kitapta 406 mercekte
tam kütüphane maskeleri. Biriktirilenler: B maskesi sayımları (mercek başına), kesin k_F ve k_H dağılımları (mercek başına, 0..114),
H maskesi sayımları (mercek, k_H kutusu), F ve W maskelerinin **seyrek tam sayımları** (mercek, k_F kutusu, grup, tam maske). Kutular
0..79 (k ≥ 79 tek kutu). Biçimler dört sürece bölünür ({1,5,9,13}, {2,6,10,14}, {3,7,11}, {4,8,12}; 1'den sayılınca) ve her süreç aynı
kitap akışını üretir; sayımlar süreçler arasında ayrıktır. Kontrol noktası beş bölütte bir.

### 3.2 Bağlamlar (12. den. ile aynı kural)
F ve W: her mercekte k_F kutularının referans sayılarından; modu içeren ve her kutusunda ≥ 10⁴ kitap bulunan kesintisiz çekirdek
aralıkta her kutu ayrı bağlam; dışındaki kutular alt/üst kuyrukta k mod 4 havuzları. H: aynı kural k_H ile. B ve Z: bağlamsız
(n = referans büyüklüğü).

### 3.3 Arama ve bit değeri
Λ'nın g grubundaki yüklemlerinden kitabın mercek λ'da sağladıkları S olsun. S = ∅ ise grup katkısı 0. Değilse c = bağlamdaki referans
kitaplarından maskesi S'nin üst kümesi olanların sayısı (en-az sayısı; seyrek sayımların izdüşümü ve üst-küme toplamı), n = bağlam
büyüklüğü ve katkı b(c, n):
- c ≥ 10 ise b = log₂ n − log₂ c (12. den.'in çözülmüş değeri);
- 0 ≤ c < 10 ise b = log₂(n + 1) − log₂(c + 0,5) (12. den.'in nokta değeri); n = 0 ise 0.
**Çözülmemiş aramalar ve puan doyması için tek kural:** bu nokta değeri **bütün** aramalarda — D'de, hedefin kendi puanında, her
kontrolde, her sıfırda — aynen kullanılır. Aralık taşınmaz, adanmış kestirim yapılmaz, taban yoktur; bir aramanın alabileceği en
büyük değer (c = 0) log₂(2(n + 1))'dir (doyma). Doymuş değerler hedefte ve kontrolde aynıdır; eşitlik "≥" sayılır (hedefin aleyhine).
Hedefin en iyi merceğindeki çözülmemiş aramalar (c < 10) bayrak olarak raporlanır.

### 3.4 Puan
I_Λ(λ) = b_B + b_Z + b_F + b_W + b_H; c(λ) = P0 mercek bedeli (biçim ağırlığı ∝ 2^{−(α+|β|−2)} 14 biçimde normalize; asal eşit 1/29;
(1,±1) için 2,459 + 4,858 = 7,317 bit); S_x(Λ) = max_λ [I_Λ(λ) − c(λ)]. En iyi mercek eşitlikte küçük mercek indeksi.
Hedeflerin ve kontrollerin puanları aynı tablolar ve aynı işlevle (bit değerleri float, toplam double) hesaplanır.

---

## 4. Dil bedeli c(Λ) ve keşif yordamı D(x)

### 4.1 Dil bedeli
Yüklem bedeli = üretim ağacındaki kod uzunluğu: c(π) = log₂(4 düzey) + log₂(#tür | düzey) + log₂(#nicelik | düzey, tür) +
log₂(#ilişki | nicelik); her düğümde L*'da kalan seçenekler üzerinde tekdüze (Kraft toplamı 1,000000000000). Dil bedeli
c(Λ) = log₂ 21 + Σ_{π∈Λ} c(π) − log₂(|Λ|!) (sıralı olmayan küme kodu; |Λ| ≤ 20 boyu log₂ 21 bitle). Değerler (bit; `sonuc/agac_bedeller.txt`):
kitap (a) 4,585 (B1, B2, Ba); kitap (b) BMn 4,000, BMt/BMt2 5,000; aile (a) 6,585; aile (b) 8,392 (dört ilişkili nicelikler: k_F, Σq,
yarı toplamları, Σ_F a, Σ_F n), 8,714 (beş ilişkili: sütun toplamları, yarı boyları); aile (d) 5,585 (FRK, FCK, K2, HK2); hücre (a) 6,585;
hücre (b) 6,585 (hücre toplamları), 6,907 (hücre boyları); hücre (d) 6,170 (K1, FAP), 5,170 (FKS, FSC); birleşim (b) 8,170; birleşim (c) 5,000.
**El dilinin 20 yüklemi aynı kuralla:** Σc(π) = 138,046 bit; **c(Λ_el) = 81,361 bit.**

### 4.2 D(x) — tanım
D(x) = argmax_{Λ ⊆ L*, |Λ| ≤ 20} [S_x(Λ) − c(Λ)]. D'ye ne Kur'an'ın metni, ne el dili, ne herhangi bir sonuç girer; girdileri hedefin
kendi dizilişi, sıfırın referans tabloları, L* ve c'dir. Her hedefe aynen uygulanır.

### 4.3 Arama yöntemi (tam)
S_x(Λ) − c(Λ) = max_λ [I_Λ(λ) − c(λ) − c(Λ)] ve c(Λ) Λ'ya yüklem eklendikçe artar, I_Λ(λ) yalnız Λ ∩ (x'in λ'da sağladıkları)'na
bağlıdır; bu yüzden en iyi Λ en iyi mercekte x'in sağladığı yüklemlerin bir alt kümesidir. Yöntem: her mercek λ ve her grup g için
x'in sağladığı L* yüklemleri üzerinde **bütün alt kümeler** taranır (en-az sayıları üst-küme toplamıyla); her boyut j için en iyi
b_g(S) − Σ_{π∈S} c(π) tutulur; gruplar |Λ| ≤ 20 altında boyut bileşimleri üzerinden tam olarak birleştirilir (amaç terimi
log₂(J!) yalnız toplam boy J'ye bağlıdır, ayrışım kesindir). Bir grupta sağlanan yüklem 24'ten fazlaysa adaylar, o bağlamdaki tekil
bitleri en yüksek 24 yüklemdir (eşitlikte küçük indeks) — tek yaklaşık adım budur ve uygulandığı hedef-mercekler raporlanır.
**Eşitlik bozma** (amaçlar 10⁻⁹ bit içinde eşitse): küçük |Λ|; sonra küçük mercek indeksi (biçim × 29 + asal sırası); sonra sıralı
global yüklem indeks listesinin sözlükte küçüğü. Çıktı: Λ_x, λ*_x, amaç, bilgi bileşenleri.

### 4.4 Dondurma
Bir sıfırın bütün hedef dilleri kontrol geçişinden önce `tab/<sıfır>/diller.txt` dosyasına yazılır, salt okunur yapılır ve SHA-256'sı
`sonuc/diller_<sıfır>_sha256.txt`'e yazılır (Λ_x = D(x) donar). Hedefin eşiği S_x(Λ_x), kontrol programında aynı tablolarla hesaplanır.

---

## 5. Hedefler, kontroller ve ayrışma

### 5.1 Hedefler
Kayıt 0 = Kur'an (129). **M20 (birincil):** K = 200 plasebo; plasebo i, tohum(M, 3, i) ile kurulan **kendi** M20 örnekleyicisinden
(64 yeni zincir, 30.000 adım yakma) gelir; ilk 6.400 emisyon atılır, sonraki emisyon hedeftir (zincir 0'ın 101. emisyonu). **Tekdüze
(ikincil):** K = 100; plasebo i, tohum(U, 3, i) ile kurulan örnekleyicinin ilk kitabı. **Mallows (bütçe kalırsa, §6.3):** K = 100,
tohum(L, 3, i), ilk kitap. Hedefler zenginliklerine göre süzülmez; çekilen her plasebo raporlanır.

### 5.2 Kontroller
Her sıfırda tek bir kontrol akışı: bölütler 2,5×10⁵ kitap, tohum(sıfır, 2, b), b = 1..400 (M20'de her bölüt yeni 64 zincirle başlar).
Kontroller aynı sıfırdan, referans örnekleminden ve hedeflerden bağımsız tohumludur. Her kontrol, dil Λ_x sabitken kendi en iyi
merceğini seçer. Bütün hedefler ortak akışı paylaşır. Dil tabloları birlikte belleğe (≤ 3,5 GB) sığmazsa diller gruplara bölünür ve
aynı kontrol bölütleri her grup için aynı tohumla yeniden üretilir (aynı kitaplar).

### 5.3 Derinlik kuralı
Düzey 1: N₁ = 10⁶ (bölüt 1–4), bütün plasebolar. k < 10 kalanlar düzey 2: N₂ = 10⁷ (bölüt 1–40, ilk 10⁶ dahil). Orada da k < 10
kalanlar düzey 3: N₃ = 10⁸ (bölüt 1–400). Kur'an'ın dili (her sıfırda), Kur'an'ın P2 dili (§9 e) ve el dili (§9 a) doğrudan 10⁸
(bölüt 1–400). En çok 10⁸; daha derine gidilmez.

### 5.4 Ayrışma istatistiği
k_x = #{kontrol : S_c(Λ_x) ≥ S_x(Λ_x) − 10⁻⁶ bit} (doğrudan sayım; eşitlik hedefin aleyhine), **p_x = (k_x + 1)/(N_x + 1)**, N_x hedefin
son derinliği. Belirsizlik: k_x/N_x için iki yanlı %95 Clopper–Pearson [lo_x, hi_x] (k = 0'da lo = 0). Kuyruk dış değerlemesi yapılmaz.
M20'de Kur'an'ın isabetlerinin zincir kümelenmesi (bölüt içi sıra mod 64) raporlanır.

---

## 6. Örneklem bütçeleri, tohumlar, sıra, hesap

### 6.1 Tohumlar
tohum = 29102600000 + sıfır·10⁸ + tür·10⁷ + sıra; sıfır U = 1, M = 2, L = 3; tür 1 referans bölütü, 2 kontrol bölütü, 3 hedef, 4 kod
doğrulama, 5 sabit-yüklem taraması, 6 hız/bellek ölçümü, 7 (ayrılmış), 8 mekanik boru hattı sınaması. 12. den.'in (26102600000 + …)
ve 10. denetimin tohumlarıyla çakışmaz.

### 6.2 Bütçeler
| Sıfır | referans | hedef | kontrol |
|---|---|---|---|
| M20 (birincil) | 10⁸ | Kur'an + 200 | düzeyli 10⁶ / 10⁷ / 10⁸; Kur'an 10⁸ |
| Tekdüze | 10⁸ | Kur'an + 100 | aynı |
| Mallows (bütçe kalırsa) | 10⁸ | Kur'an + 100 | aynı |
Hesap tavanı 50 çekirdek-saat (hedef ~30); aynı anda en çok 4 iş parçacığı/süreç; bellek ≤ 6 GB; disk ≤ 8 GB. Kestirim: sıfır başına
referans ~2,3, kontroller ~3 çekirdek-saat. Bir koşu bütçeyi aşacaksa yeni koşu başlatılmaz, eksik kalan yazılır (ek).

### 6.3 Sıra
M20: Ö0'ın Kur'an kısmı (§7.1) → hedefler → referans → Ö0 bilgi çapası (§7.2) → D → dillerin dondurulması → kontroller (düzey 1, 2, 3) →
analiz. Sonra tekdüze aynı sırayla. Mallows yalnız, M20 ve tekdüze bittikten sonra günlüğe kayıtlı toplam hesap + Mallows kestirimi
≤ 40 çekirdek-saat ise; değilse "ölçülmedi".

### 6.4 Sürdürülebilirlik ve kayıt
Referans: süreç başına kontrol noktası (atomik). Kontrol: bölüt çıktıları atomik; var olan bölüt atlanır. Günlük `sonuc/kosu_gunlugu.txt`,
ara notlar `sonuc/ara_notlar.md`. Sonda `paket/plasebo12_paket.tar.gz` (≤ 200 MB) ve SHA-256'sı.

---

## 7. Ö0 çapalarının dondurma sonrası kısmı

### 7.1 Kur'an'da maske eşitliği ve el dili profili
Dondurmadan sonra ilk iş: kütüphane 20 özgün yükleme sınırlandırılınca Kur'an'ın 406 mercekteki maskeleri `butunluk.h` ile bit-bit aynı
olmalı ve el dili profili (1, +1, 19)'da 15 yüklem olmalı (B1 B2 F1 F2 R0c R1c R1e K1 K2 Qc QS Qh W1 QU Qa). Tutmazsa sebep bulunur,
yazılır ve Ö1'e geçmeden çözülür.

### 7.2 Bilgi puanı çapası
Yeni M20 ve tekdüze referans tablolarıyla Kur'an'ın el dilindeki bilgi puanı (aynı V0/P0 tanımı; bu ön-kaydın §3.3 kuralı) 12. den.'in
değerine tablo gürültüsü içinde eşit olmalı (M20 27,41; U 31,06 bit). Ölçüt: Kur'an'ın en iyi merceğindeki her arama (B, F, W) için
yeni en-az sayısı, 12. den.'in tablo sayısıyla (`~/opus5.5/butunluk/tab/{M,U}129.tab`, salt okunur) iki-örneklem koşullu binom
sınamasında (iki yanlı) p ≥ 0,001 ile tutarlı olmalı; U'nun F araması ayrıca 12. den.'in adanmış kestirimiyle (2,307×10⁻⁶) Poisson
tutarlılığı ile sınanır. Puan farkı ve gürültü (σ ≈ 1/(ln 2 · √c) arama başına) raporlanır. Tutmazsa sebebi bulunur ve yazılır.

---

## 8. Kalibre cevap

### 8.1 Kalibre p
R = #{plasebo x : p_x ≤ p_Q ya da [lo_x, hi_x] ile [lo_Q, hi_Q] çakışır (lo_x ≤ hi_Q)} (eşitlikler ve aralık çakışmaları Kur'an'ın
aleyhine); **p_kal = (1 + R)/(K + 1)**. Her sıfır ayrı (M20 birincil; K = 200).

### 8.2 Okuma kuralının işletilmesi
- R = 0 → "Kur'an'ın ayrışması bütün plasebo kitaplarınkini aşar"; okuma kuralının ilk cümlesi uygulanır; p_kal = 1/(K + 1); fark ayrıca
  model-tabanlı verilir (§8.3).
- R/K ≥ 0,05 (M20'de R ≥ 10; K = 100'de R ≥ 5) → "plasebo kitaplar Kur'an'ın ayrışmasına sık ulaşır"; ikinci cümle uygulanır.
- Aradaki her sonuç (M20'de 1 ≤ R ≤ 9) p_kal ile aynen yazılır.

### 8.3 Model-tabanlı fark (önceden yazılmış model; kararlarda kullanılmaz)
y_x = −log₁₀ p_x (plasebolar), y_Q = −log₁₀ p_Q. **Birincil model:** plasebo y'lerinin üst %20'sine (M20'de en büyük 40, K = 100'de 20;
eşik u = bu kümenin en küçüğünün hemen altındaki sıra istatistiği) genelleştirilmiş Pareto (GPD) uyumu, en çok olabilirlik (σ, ξ);
P_model(Y ≥ y_Q) = 0,2 · [1 + ξ(y_Q − u)/σ]^{−1/ξ} (ξ < 0 ve y_Q uç noktanın ötesindeyse 0). Belirsizlik: parametrik önyükleme
(2.000 yeniden örneklem, yeniden uyum) %95 aralığı. 10⁸'de k = 0 kalan plasebolar sağdan sansürlü (y = log₁₀(10⁸ + 1)) olarak olabilirliğe
girer. **İkincil:** bütün y'lere normal uyum (aynı sansürle), P = 1 − Φ((y_Q − μ)/σ). Ayrıca betimsel fark Δ = y_Q − max_x y_x
(mertebe). Hepsi "model-tabanlı" etiketiyle verilir.

---

## 9. İkinciller

(a) **El dili plasebolarda:** el dili (20 yüklem) bütün hedeflere aynı kontrol akışında uygulanır; plasebo başına p_x^el aynı derinlik
kuralıyla (el dili sayımlarına uygulanmış); **dil yazımının kazancı** G_x = log₁₀ p_x^el − log₁₀ p_x (mertebe); dağılımı (medyan, çeyrekler)
ve Kur'an'ın G_Q'su.
(b) **D(Kur'an) ile el dili:** ortak yüklemler; Kur'an'ın el dilindeki p'si (10⁸) ve kendi dilindeki p'si; Kur'an'ın (1,+1,19)'daki L*
profili (dondurmadan sonra).
(c) **Bütünlük şartlı ayrışma:** 12. den. §7'nin sınıf kuralı kardeşlere genelleştirilir. Bir kitap (hedef ya da kontrol), dil Λ'daki en
iyi merceğinde (i) kapı G tutuyorsa (k_F ≥ 2E = 228/p ve her F hücresi ≥ 2) ve (ii) Λ'nın orada sağlanan yüklemleri ≥ 3 motif
türünden ve (iii) ≥ 3 düzeyden geliyorsa "sınıfta"dır (tür ve düzey etiketleri §2.2–2.3; 12. den.'in 20 yüklemdeki etiketleriyle aynı).
12. den.'den farkı: kütüphanenin gerektirme yapısı türetilmediği için "kredi alan" yerine "sağlanan" yüklemler sayılır (kural daha
geniştir). Hedef sınıftaysa k^c_x = #{kontrol : sınıfta ve S_c ≥ S_x}, p^c_x = (k^c_x + 1)/(N_x + 1); sınıf dışı hedef "sınıf dışı"dır (p^c = 1
sayılır). Raporlananlar: Kur'an'ın ve plaseboların p^c'leri, sınıftaki plasebo dili sayısı, p^c_kal = (1 + R^c)/(K + 1) (§8.1 kuralıyla,
yalnız sınıftaki plasebolar). Kur'an kendi dilinde sınıf dışıysa sınıf şartlı kalibrasyon tanımsızdır ve öyle yazılır.
(d) **Tekdüze** (K = 100) ve bütçe kalırsa **Mallows** (K = 100) plasebo kalibrasyonu, aynı kurallarla.
(e) **Kur'an için P2** (12. den.'in metin ağırlıklı asal önseli: 1/3 eşit + 1/3 {3,5,7,11,19} + 1/3 {3,5,7,19}): D(Kur'an) P2 mercek
bedeliyle yeniden kurulur, kontroller de P2 bedeliyle puanlanır; yalnız ek satır, kalibrasyonsuz (plaseboların metni yoktur).
(f) **Bölme seçimi kütüphaneye** (menünün 5 bölmesi): her referans ve kontrol kitabı için 20 bölme çiftinin maskeleri gerekir (~20 kat
hesap; sıfır başına ~45 çekirdek-saat); bütçeye sığmaz → **ölçülmeyecek**, "ölçülmedi" yazılacak.

---

## 10. Tahminin değerlendirilmesi (karar kuralları; M20)

- **p_Q:** k_Q ≥ 1 ise p̂ = (k_Q + 1)/(N + 1) ve %95 CP aralığı; "tuttu" = p̂ ∈ [10⁻⁹, 10⁻⁶]; "tutmadı" = %95 aralığı tümüyle [10⁻⁹, 10⁻⁶]
  dışında; öteki durumlar "karar verilemedi". k_Q = 0 ise yalnız tek yanlı %95 üst sınır U = 1 − 0,05^{1/N} vardır: U < 10⁻⁹ ise
  "tutmadı (daha nadir)", aksi halde "karar verilemedi". |log₁₀ p̂ + 7| ayrıca yazılır.
- **Plasebo p_x medyanı:** nokta = 200 p_x'in medyanı (100. ve 101. sıra istatistiklerinin geometrik ortalaması); %95 aralık = sıra istatistikleri [p_(86), p_(115)] (kapsama 0,960); "tuttu" = nokta
  ∈ [10⁻³, 10⁻¹]; "tutmadı" = aralık tümüyle dışında; öteki "karar verilemedi".
- **En küçük p_x:** nokta = min_x p_x; aralık = [min_x lo_x, min_x hi_x]; aynı üç hüküm, [10⁻⁶, 10⁻⁴] ile.
- **"Hepsini aşar":** R = 0 ise "tuttu", R ≥ 1 ise "tutmadı".
- **Bütün tahmin:** dört parça da tuttuysa "tuttu"; hiçbiri tutmayıp en az biri tutmadıysa "tutmadı"; öteki durumlar "kısmen tuttu"
  (ayrıntısıyla).

---

## 11. Rapor kuralları

`PLASEBO12_RAPORU.md` (Türkçe; sonunda İngilizce özet). Her sayının yanında sıfır, örneklem, isabet ve belirsizlik; senaryolar bir aralığın
uçları gibi sunulmaz; üst sınırlar sıklık gibi okunmaz. Cevaplanacak sorular görevdeki beş sorudur. **Ölçülmeyenler** "ölçülmedi" diye
yazılır, en azından: kütüphanenin sınırları (L*'ın dışındaki yüklem aileleri, 406 mercekli mercek ailesi, bölme seçimi); literatürün on
yıllar süren araması; nesnenin (n, a) seçimi; sayım okulu; kültürel havuz; gruplar arası bağımsızlık yaklaşımının etkisi; referans
tablosu gürültüsünün p_x'e etkisi (puan işlevi tabloya koşulludur). Raporun depoya ekleneceği ve ana hattın onu bağımsız
doğrulayacağı bilinerek yazılır: bütün dil dosyaları, eşikler, bölüt sayımları ve yeniden üretim komutları pakettedir.

## 12. Sapma politikası

Her sapma (bütçe, kod düzeltmesi, tanım netleştirmesi) `ek/EK_<UTC zaman>_<konu>.md` dosyasına gerekçesi ve sonuca etkisiyle yazılır.
Herhangi bir hedefin profiline, diline ya da kontrol puanına bakıldıktan sonra tanım, ağırlık, eşik, bedel ya da kural değiştirilmez;
değiştirilmesi zorunlu bir hata bulunursa düzeltme ve düzeltme öncesi sonuç birlikte raporlanır.
