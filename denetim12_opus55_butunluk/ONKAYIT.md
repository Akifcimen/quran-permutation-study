# ÖN-KAYIT — bütünlük ölçümü: aynı unsurların katmanlar arasında yeniden belirmesi ne kadar nadir?

**Oturum:** Claude (Opus 5.5), `~/opus5.5/butunluk`, görev `GOREV.md` (26 Ekim 2026). `~/Desktop/hrf2` yalnız okundu.
**Bu belge hiçbir rastgele kitabın bütünlük puanına (sayım ya da bilgi) bakılmadan yazıldı ve donduruldu.** Dondurma anı ve
SHA-256: `sonuc/onkayit_ozet.txt`; dondurma anındaki kod özetleri: `sonuc/onkayit_kod_sha256.txt`. Sapmalar yalnız `ek/`
altındaki zaman damgalı eklerde yazılır; bu dosya değiştirilmez.

---

## 0. Dondurmadan önce yapılanlar (ve yapılmayanlar)

Yapıldı (görevin izin verdiği biçimde):
- **Kur'an'ın kendi profili ve sayım puanı** (bütün 406 mercekte; kesin, rastgelelik içermez): §2.10.
- **Kod doğrulamaları:** C puanlayıcısı (`src/butunluk.h`) ile Python tanım uygulamasının (`src/dil.py`) eşitliği — Kur'an, dört
  sıfırdan 150–300'er rastgele kitap ve 300 O-havuzu kitabı × 406 mercekte yüklem maskesi, aile boyları, hücre sayıları ve
  sayım kredileri; **yalnız uyuşmazlık sayıları basıldı (hepsi 0); hiçbir puan değeri görülmedi.**
- **Örnekleyiciler:** U, M20, Mallows, J10 örnekleyicileri `denetim10_opus55_kosullu/src/{ortak.h,hizli.h}`'den kopyalandı;
  aynı tohumla 129 ve 127'de 200.000'er kitapta onlarınkiyle **bit-bit aynı** (orada çapalarla doğrulanmışlardı).
- **10. denetimin O-havuzları** (`~/opus5.5/kosullu/tab/havuz`): U ve M20'den birer bölüt depodaki kodla aynı tohumdan yeniden
  üretildi, bit-bit aynı.
- **Gerektirme ve sabit-yüklem taramaları** (§2.7): 3,5×10⁶ rastgele kitap ve 4,03×10⁵ O-havuzu kitabında yüklem maskelerinin
  birlikte-görülme sayıları (puan değil); yalnız karşı-örneksiz aday kurallar ve "hep doğru" yüklemler basıldı. Ayrıca soyut
  yapılandırma uzaylarının kapsamlı sayımı.
- **Mekanik sınamalar:** bütün hat (referans → birleştirme → tablo → Kur'an puanı → sınama bölütü → orkestra) yalnız Kur'an'ı
  tekrarlayan 'K' kipiyle; referans tablosu kurulumu rastgele kitaplarla **puansız kipte** (`PUANSIZ=1`: puan hesaplanmaz) yalnız
  yapısal olarak (toplamlar, en-az dönüşümü bağımsız Python hesabıyla eşit) sınandı; tablo içerikleri incelenmedi.
- Hız ölçümü (yalnız süre basıldı): rastgele kitap başına değerlendirme + sayım ~42 µs; referans biriktirmeyle ~62 µs.

Yapılmadı: hiçbir rastgele kitabın sayım ya da bilgi puanı hesaplanıp görülmedi; üretim referans tabloları kurulmadı;
**Kur'an'ın bilgi puanı hesaplanmadı** (sıfır tablolarını gerektirir; dondurmadan sonra, rastgele kitaplarla aynı yordamla).

---

## 1. Ana hattın önceden kaydettiği tahmin (aynen)

> **Ana hattın önceden kaydettiği tahmin** (ön-kayda aynen yaz, sonda değerlendir): nesnenin bütününün bütünlük sırası uzunluk-sıralı sıfırlarda (M20, Mallows) merkez **10⁻⁸**, aralık **10⁻⁶–10⁻¹⁰**; tekdüze sıfırda **10⁻¹⁰–10⁻¹²**; eski literatür koşullu (yalnız bu çalışmanın eklediği) bütünlük **10⁻²–10⁻⁴**.

Değerlendirme kuralları §9'dadır.

---

## 2. Bütünlük dili L

Dilde Kur'an'a özgü sabit yoktur (19, 12, 1444, 722, 5–9 geçmez). Tanımların tek başvurusu `src/dil.py`'dir; `src/butunluk.h`
onunla bit-bit eşittir.

### 2.1 Mercek
Kitap = konum n = 1..114'e atanmış ayet sayıları a_n. Mercek λ = (α, β, p):
- **Biçimler (14):** t = αn + βa, u = αn − βa; α, |β| ∈ {1,2,3}, aralarında asal, β'nin iki işareti: (1,±1), (1,±2), (1,±3),
  (2,±1), (2,±3), (3,±1), (3,±2) (dış çalışmanın 14 biçimi, aynı sıra).
- **Asallar (29):** 3, 5, 7, …, 113. Toplam 406 mercek.

### 2.2 Bölme
Birincil bölme çifti (D1, D2) = (t-paritesi, n-paritesi). Sınıf 0 = çift. (Menü §8.4.)

### 2.3 Düzeyler ve kümeler
E⁰ = {n : D1 = 0}, E¹ = tümleyen (kitap düzeyi). F = {n : p | t_n}, H = {n : p | u_n}, U = F ∪ H; k_F = |F|, k_H = |H|;
E = 114/p (beklenen aile boyu). q_n = t_n/p (n ∈ F; tam sayı, sıfır ya da negatif olabilir); u-katsayısı u_n/p (n ∈ H).
Aile yarıları F⁰ = F ∩ E⁰, F¹ = F ∩ E¹; hücreler F^{ij} = {n ∈ F^i : D2 = j}; hücre toplamları s_ij = Σ q (F^{ij}).
Satır 0 = (s00, s01) (t çift), satır 1 = (s10, s11) (t tek). t = 0 ise n her asalın ailesindedir (0 her p'ye bölünür).

### 2.4 Yüklemler (20)
Motif türleri: (a) bölme dengesi, (b) modülün geri dönüşü, (c) iki el, (d) ardışıklık.

| Kod | Düzey | Tür | Tanım |
|---|---|---|---|
| B1 | kitap | a | \|E⁰\| = \|E¹\| (= 57) |
| B2 | kitap | a | E⁰, E¹ boş değil ve [α·Σ_{E⁰} n = \|β\|·Σ_{E¹} a] ∨ [α·Σ_{E¹} n = \|β\|·Σ_{E⁰} a] ∨ [Σ_{E⁰} t = Σ_{E¹} t] (eşit toplam; kitap düzeyinde toplamın doğal küresel sabite — n- ve a-bileşenine — eşitliği dahil) |
| F1 | aile | a | \|F⁰\| = \|F¹\| ≥ 1 |
| F2 | aile | a | F⁰, F¹ boş değil ve Σq(F⁰) = Σq(F¹) |
| R0c | hücre | a | \|F⁰⁰\| = \|F⁰¹\| ≥ 1 |
| R0e | hücre | a | F⁰⁰, F⁰¹ boş değil ve s00 = s01 |
| R1c | hücre | a | \|F¹⁰\| = \|F¹¹\| ≥ 1 |
| R1e | hücre | a | F¹⁰, F¹¹ boş değil ve s10 = s11 |
| K1 | hücre | d | dört hücre dolu; {s00, s01, s10, s11}'in farklı değerleri kesintisiz ardışık ve en az 2 farklı değer |
| K2 | katsayı | d | k_F ≥ 2; ailenin farklı q değerleri kesintisiz ardışık ve en az 2 farklı değer |
| Qc | hücre | b | dört hücre dolu ve Σq(F) = 4p (hücre toplamlarının merkezi p) |
| QS | aile | b | k_F ≥ 1 ve p \| Σq(F) |
| Qh | aile | b | F⁰, F¹ boş değil ve p \| Σq(F⁰) ve p \| Σq(F¹) (yarı toplamları) |
| QkF | aile | b | k_F = p |
| W1 | birleşim | c | k_F ≥ 1, k_H ≥ 1, F ∩ H = ∅ |
| QU | birleşim | b | \|U\| = p |
| QkH | birleşim | b | k_H = p |
| Qa | birleşim | b | \|U\| ≥ 1 ve p \| Σ_{U} a |
| Qn | birleşim | b | \|U\| ≥ 1 ve p \| Σ_{U} n |
| QH | birleşim | b | k_H ≥ 1 ve p \| Σ_{H} u/p |

Gerekçe: (a) her düzeyde aynı tanım (eşit sayı, eşit toplam; kitap düzeyinde eşit toplam tek toplamda imkânsız olduğundan
doğal sabit seçeneği); (b) önceden listelenmiş doğal nicelikler: birleşim boyu, iki aile boyu, Σq, merkez, yarı toplamları,
birleşimin ayet ve konum toplamları, ikinci elin katsayı toplamı — her nicelik bir yüklem; boyut/merkez "= p", toplamlar "p |";
(c) iki elin ayrıklığı (birleşim ilişkisi QU/Qa/Qn ile (b)'de); (d) iki düzeyde aynı ardışıklık tanımı (hücre toplamları, katsayılar).
"Sabit hücre" (≥ 2 eşit q) görevin (a)–(d) türlerinde olmadığı için dilde yoktur. H'nin iç yapısı ayrıca yüklenmez: (α, −β)
biçiminde H, F'nin rolündedir (aynı t-paritesi bölmesiyle).

### 2.5 Kitap maskesinin biçim düzeyinde olması
B1, B2 yalnız biçime (ve D1'e) bağlıdır; (α, β) ve (α, −β) aynı E-bölmesini ve aynı B2'yi verir.

### 2.6 Sabit (dışlanan) yüklemler
Permütasyonla değişmeyen yüklemler hiçbir kitapta puan almaz (analitik liste; `sabit_dislanan`):
- B1: D1 yalnız konuma ya da yalnız değere bağlıysa (birincilde α ya da β çift: t ≡ a ya da t ≡ n (mod 2)).
- p = 3 ve |β| = 3 (biçimler (1,±3), (2,±3)): F = H = {3 | n} sabit → QU, QkF, QkH, Qn (Σn = 2223, 3'e bölünür: sabit-doğru), W1.
- p = 3 ve α = 3 (biçimler (3,±1), (3,±2)): F = H = {n : 3 | a_n}, sabit boy 39, sabit Σa = 2193 (3'e bölünür) → QU, QkF, QkH, Qa, W1.
Tarama: dışlanmamış hep-doğru yüklem–mercek çifti 0 (3,5×10⁶ rastgele kitap).

### 2.7 Kanıtlı gerektirmeler (sayım puanının ayıklaması)
Bir yüklem, aynı profildeki başka yüklemlerden zorunlu çıkıyorsa ek kredi almaz. Kredi(S) = kanıtlı kuralların kapanışıyla S'yi
üreten en küçük alt kümenin boyu (kesin; `kredi_kesin`). Kurallar (D1 = t2; satır 0'da q çift, satır 1'de q tek, çünkü p tek):

| Kural | Geçerlilik | Kanıt (özet) |
|---|---|---|
| P1 Qc → QS | her mercek | Σq = 4p ⇒ p \| Σq |
| P2 Qh → QS | her mercek | iki yarı p'ye bölünür ⇒ toplamı |
| P3 F2 ∧ QS → Qh | her mercek | yarılar eşit = Σq/2; p tek, p \| Σq ⇒ p \| Σq/2 |
| P4 QkF ∧ QkH ∧ QU → Qn, Qa | p ∤ αβ | \|F\| = \|H\| = \|F∪H\| ⇒ F = H = U; F∩H'de p \| 2αn, p \| 2βa ⇒ p \| n, p \| a |
| P5 K1 ∧ Qc → F2 | her mercek | satır-0 toplamları çift; ardışık değerler ve toplam 4p (p tek) için vaka analizi: 2 değer (çift+tek) ⇒ toplam ≢ 0 (mod 4); 3 değer {e−1,e,e+1}, e çift ⇒ 4e = 4p imkânsız; {e,e+1,e+2} ⇒ yalnız satır 0 = {e,e+2}, satır 1 = (e+1,e+1), e+1 = p; 4 değer ⇒ toplam ≡ 2 (mod 4). Tek çözüm: satır 0 = {p−1,p+1}, satır 1 = (p,p) ⇒ F2 |
| P6 K1 ∧ Qc → R1e | her mercek | aynı vaka analizi |
| P7 K1 ∧ Qh → F2 | p ≥ 5 | ardışık ≤ 4 değerde \|Σq(F⁰) − Σq(F¹)\| ≤ 4 (4 değer ⇒ = 2, p ∤ 2); p \| fark, p ≥ 5 ⇒ fark 0 |
| P7a R1c ∧ K1 ∧ Qh → F2; P7b R1e ∧ K1 ∧ Qh → F2 | p = 3 | K1 en az bir tek hücre toplamı ister ⇒ satır 1'in iki toplamı aynı paritede ⇒ fark çift, \|fark\| ≤ 4, 3 \| fark ⇒ 0 |
| P8 Qc ∧ Qh → F2 | β > 0 | q > 0; Σq(F⁰) = jp çift, 0 < j < 4 ⇒ j = 2 |
| P9 (7 kural) | p = 3 ∧ β > 0 | Qc ⇒ Σq = 12 (q ≥ 1): uzay sonlu; kesin sayımla (`kontrol/p3_qc_kesin.py`): Qc∧F1∧F2→R0c,R1c; Qc∧F1∧R0e→R0c; Qc∧F1∧R1e→R1c; Qc∧F1∧K1→R0c,R1c,K2; Qc∧F1∧Qh→R0c,R1c; Qc∧F2∧R1e→K1; Qc∧R1e∧Qh→K1 |

Arama yöntemi: (1) 3,5×10⁶ rastgele + 4,03×10⁵ O-havuzu kitabı × 406 mercekte birlikte-görülme; (2) soyut kapsamlı sayım
(hücre başına ≤ 5 üye, toplamlar [−3p−6, 5p+6], p ∈ {3,5,7,11} ve p = 19'da ≤ 4 üye, iki işaret kipi): çıkan bütün ≤ 3 öncüllü
kurallar yukarıdadır; (3) K2 için açık katsayılı sayım; (4) K1∧Qc∧K2 altında bölüntülerin kesin sayımı (p ≤ 23).
**Çürütülen adaylar (kullanılmaz):** sınırlı pencerelerin yapıtları — F1∧R1c∧K1→R0c, F2∧R0e∧R1e→R1c, R0e∧R1e∧Qc→R1c (≥ 4 üyeli
hücrelerle karşı-örnek); K2 içeren bütün adaylar (F1∧K1∧Qc→K2, K1∧K2∧Qc→F1/R0c [p = 19'da 4.015 farklı sayı vektörü mümkün],
F1∧K2∧Qh→F2 [her p'de karşı-örnek], K2∧Qc∧Qh→F2 [serbest işaretli biçimlerde karşı-örnek]). O-havuzunun kapılı mercekleri
koşullamanın yarattığı yapay ilişkiler verdi (ör. B2∧K1→F1); bunlar gerektirme değildir. **Kanıtlanamamış aday kalmadı.**
Bilgi puanında gerektirmeler kendiliğinden etkisizdir (birlikte olasılık).

### 2.8 Yozlaşma kuralı
- **Sayım puanı (birincil kapı G, görevin önerisi):** aile ve hücre düzeyindeki yüklemler (F1, F2, R0c, R0e, R1c, R1e, K1, K2,
  Qc, QS, Qh, QkF) yalnız k_F ≥ 2E ve dört hücrenin her biri ≥ 2 üyeliyse kredi alır. Kitap ve birleşim yüklemleri kapısızdır.
- **Bilgi puanı (birincil):** kapı yok; aile-içi ve iki-el yüklemlerinin olasılıkları **aile boyuna (k_F) koşulludur**. Gerekçe:
  küçük ailede kendiliğinden tutan bir motif, o boyda olası olduğu ölçüde az bit verir (küçük aileler açıkça bedellendirilir);
  aile boyunun kendisi (ör. büyük aile) bir yeniden-belirme olayı olmadığından ödüllendirilmez (QkF dışında). Bu, rakiplere 2E
  kapısından adildir: normal boylu ailelerde de yapı puanlanır. 2E kapısı Kur'an'ın aile boyuna tam denk düştüğü (12 = 2·6) için
  birincil bilgi puanında kullanılmaz; duyarlılık olarak raporlanır (§8.3).
- Duyarlılık kapıları (sayım): GE (k_F ≥ E ∧ hücre ≥ 2), Gc (hücre ≥ 2), G0 (kapı yok). Bilgi: V1 (G kapısı tutmazsa aile-içi ve
  boy katkısı 0).

### 2.9 Puanlar
**(i) Sayım puanı:** C_λ = kredi(kapıdan geçen sağlanmış yüklemler); C = max_λ C_λ (mercek bedeli yok).

**(ii) Bilgi-ağırlıklı puan (birincil):**
I(λ) = I_B + I_boy + I_F + I_W − c(λ), S = max_λ I(λ), burada (en-az olasılık = sağlanan yüklemlerin hepsinin birlikte tutma olasılığı):
- I_B = −log₂ P(B-maskesi | biçim) (kitap grubu, 2 bit);
- I_boy = −log₂ P(k_F = p) yalnız QkF doğruysa (koşulsuz; boy olayı);
- I_F = −log₂ P(aile-içi maske | k_F) (11 bit: F1 F2 R0c R0e R1c R1e K1 K2 Qc QS Qh; grup içi **birlikte** sıklık);
- I_W = −log₂ P(iki-el maskesi | k_F) (6 bit: W1 QU QkH Qa Qn QH; grup içi birlikte sıklık);
- c(λ) = biçim bedeli + asal bedeli.
Gruplar arası çarpım bir yaklaşımdır (kitap ⊥ aile-içi ⊥ iki-el | k_F); grup içi bağımlılıklar (hücre, bant, merkez, yarılar —
bütün cebirsel bağlar) tam birlikte sıklıkla hesaplanır. Kur'an'ın merceğinde gruplar arası bağımsızlık önceki ölçümlerle
uyumludur (10. denetim: κ ≈ 1); bu yaklaşımın kendisi raporda sınırlılık olarak yazılır (§10).

**Mercek tarif bedeli (sadelik önseli, önceden yazılmış):** birincil P0: biçim ağırlığı ∝ 2^{−(α+|β|−2)} (14 biçimde normalize:
(1,±1) 2,459 bit; (1,±2),(2,±1) 3,459; (1,±3),(3,±1) 4,459; (2,±3),(3,±2) 5,459), asal eşit 1/29 (4,858 bit).
Duyarlılık: P1 biçim eşit (log₂14 = 3,807 bit); P2 asal metin ağırlıklı (dış çalışmanın karışımı: 1/3 eşit + 1/3 {3,5,7,11,19}
+ 1/3 {3,5,7,19}; q(19) = 0,1615 → 2,63 bit); P3 biçim ∝ 4^{−(α+|β|−2)}.

### 2.10 Kur'an'ın profili ve sayım puanı (dondurmadan önce hesaplandı)
En iyi mercek **(α, β, p) = (1, +1, 19)**; k_F = 12 (= 2E), k_H = 7, \|U\| = 19, F ∩ H = ∅; hücre sayıları 3/3/3/3, hücre toplamları
(s00, s01, s10, s11) = (20, 18, 19, 19); katsayılar {5,…,9}; Σq = 76; yarılar 38/38.
Sağlanan 15 yüklem: B1 B2 F1 F2 R0c R1c R1e K1 K2 Qc QS Qh W1 QU Qa (sağlanmayan: R0e, QkF, QkH, Qn, QH).
Ayıklanan 4: F2 ve R1e (K1∧Qc'den), QS (Qc'den), Qh (F2∧QS'den). **Sayım puanı (kapı G) = 11.** GE, Gc, G0 kapılarında da 11.
İkinci en iyi mercek (1,−1,19): 6. 127 sayımında profil aynıdır (sûre 9 ailede değil; terazi 6234 ile tutar).
Ana hattın 13'lük tablosuyla fark: o tablo "dört hücre eşit" ve "b,b / b−1,b+1 deseni"ni tek yüklem sayar ve "sabit hücre"yi içerir;
burada eşit hücreler üç ayrı denge (F1, R0c, R1c) olarak, desen ise K1 ∧ Qc'nin zorunlu sonucu olarak yer alır.

---

## 3. Bilgi puanının olasılık tabloları (yeterli örneklem, taban doygunluğu yok)

### 3.1 Referans örneklemi
Her sıfır × kodeks için sınama örnekleminden **ayrı tohumlu** bir referans örneklemi; her kitapta 406 mercekte yüklem maskesi.
Biriktirilenler: (mercek, k_F) başına aile-içi maskenin (11 bit) ve iki-el maskesinin (6 bit) **kesin** maske sayıları;
biçim başına kitap maskesi; mercek başına k_F dağılımı; kapı G'yi geçen mercek-kitaplarda aile maskesi (+QkF) sayıları (yalnız
§6.2 için). k_F kutuları 0..79 (≥ 79 tek kutu).

### 3.2 Arama (en-az olasılık) ve bağlam
P̂(en az M | bağlam) = (M'nin üst-kümelerinin sayısı) / n_bağlam. Bağlam: her mercekte, modu içeren ve her k'de referans sayısı
≥ 10⁴ olan kesintisiz **çekirdek** aralıkta kesin k_F; dışındaki k'ler alt/üst kuyrukta **k_F mod 4** havuzlarında (paritenin
ve 4'e bölünebilirliğin gerektirdiği yüklemler için). Boş maske 0 bit.

### 3.3 Çözünürlük ve sınırlar (taban doygunluğu yok)
Bir arama, en-az sayısı c ≥ 10 ise **çözülmüş** sayılır: nokta = log₂ n − log₂ c. c < 10 ise **çözülmemiş**: puanın alt sınırına
olasılığın tek yanlı %95 Poisson–Clopper–Pearson üst sınırı (c = 0: 2,996/n …), üst sınırına alt sınırı (c = 0 ise ∞) girer;
nokta değeri yalnız geçici sıralama içindir: log₂(n+1) − log₂(c+0,5). Her kitap için S_alt, S_nokta, S_üst hesaplanır.
Taban değerde doyma yoktur: çözülmemiş arama puanı bir tabana kırpılmaz, aralık olarak taşınır ve §4.3'teki adanmış kestirimle
çözülür.

### 3.4 Kur'an'ın bilgi puanı
Aynı tablolar ve aynı yordamla hesaplanır (`src/kuran_puan.c`). En iyi merceğindeki çözülmemiş aramalar §4.3'ün adanmış
kestirimiyle çözülür; nihai S_Q bundan sonra sabitlenir.

---

## 4. Sıra

### 4.1 Tanım
Kur'an'ın sırası p = (k + 1)/(N + 1), k = #{sınama kitabı : puan ≥ Kur'an'ın puanı} (eşitlik ≥ sayılır; bilgi puanında 10⁻⁶ bit
tolerans). Sayım puanında örneklem = referans ∪ sınama kitapları (ikisi de bağımsız tohumlu); bilgi puanında yalnız sınama kitapları
(tablolar referanstan).

### 4.2 Belirsizlik
k ≥ 1: k/N için Clopper–Pearson %95. k = 0: tek yanlı %95 üst sınır 1 − 0,05^{1/N} ve artı-bir sıra 1/(N+1); üst sınır sıklık
gibi okunmaz. M20'de isabetlerin zincir kümelenmesi (sıra numarası mod 64) raporlanır.

### 4.3 Bilgi puanında belirsiz kitaplar ve adanmış kestirim
k_alt = #{S_alt ≥ S_Q}, k_üst = #{S_üst ≥ S_Q}. Belirsiz kitaplar (S_alt < S_Q ≤ S_üst) çözülmemiş aramaları için **adanmış
kestirim** alır; sıra: önce Kur'an'ın aramaları, sonra belirsiz kitaplar S_alt'a göre azalan. Adanmış kestirim (tohum türü 4):
- **Tekdüze:** kesin koşullu örnekleme — ailenin sınıf-sayı vektörü öneriden çekilir ve **önem ağırlığı Φ(k)/M!** (yeni aile üyesi
  doğurmayan tamamlama sayısı; ayrık blok yasak tahtasının kale polinomu; 10. denetim `kosul_u.h`'nin genellemesi) ile ağırlıklanır;
  aile içi maske γ'den hesaplanır; iki-el maskesi için tamamlama düzgün ret örneklemesiyle çekilir; P(k_F = p) gerektiğinde kesin
  formülle. MCMC yoktur.
- **M20, Mallows, J10:** tek mercekli doğrudan örnekleme (sıfırın kendi örnekleyicisi, yeni tohum); bağlamdaki kitaplarda hedef
  maskelerin en-az isabetleri sayılır.
Hedef: arama başına ≥ 30 (en az 10) isabet. Bütçe: arama bağlamı başına tekdüzede ≤ 2×10⁸ öneri, sıralı sıfırlarda ≤ 2×10⁹ kitap;
sıfır başına adanmış toplam ≤ 4 çekirdek-saat. Çözülemeyen belirsiz kitaplar k için aralık olarak raporlanır: p ∈
[(k_alt+1)/(N+1), (k_üst+1)/(N+1)].
Sınama koşusu geçici S_Q (nokta) ile k sayar ve S_üst ≥ S_Q,geçici − 6 bit olan bütün kitapları (her değişke) aday dosyasına yazar;
nihai k adaylardan nihai S_Q ile yeniden sayılır (S_Q 6 bitten fazla düşerse 0,1 bitlik histogramlar kullanılır; rapor edilir).

---

## 5. Sıfır modelleri, örneklem bütçeleri, tohumlar

### 5.1 Sıfırlar (kodeks 129 birincil)
- **U** tekdüze: ayet sayıları çokluğunun düzgün permütasyonu (Fisher–Yates; robotun U akışı).
- **M20 (birincil sıralı):** robotun zinciri — 64 zincir, tam azalan diziden 30.000 öneri yakma, emisyon başına 20 takas önerisi,
  S = Σ n·a ≤ S_gözlenen kısıtı; zincirler sırayla emisyon verir. Her bölüt yeni 64 zincirle başlar.
- **Mallows:** P ∝ q^I, I = büyükten küçüğe sıraya aykırı kesin çift sayısı; q yalnız E[I] = I_gözlenen için ayarlı (129: I = 993,
  q = 0,914573128462; 127: 0,914667172717); kesin Lehmer + alias örneklemesi.
- **J10 (yalnız ikincil, "veriye merkezli"):** gerçek dizilişin σ = 10 konumluk titreşimi.
Dört örnekleyici denetim10'unkilerle bit-bit aynıdır (§0).

### 5.2 Bütçeler
| Sıfır, kodeks | referans N_R (bölüt × N) | sınama N_T | sayım puanı örneklemi |
|---|---|---|---|
| U, 129 | 10⁸ (20 × 5×10⁶) | 10⁸ (20 × 5×10⁶) | 2×10⁸ |
| M20, 129 | 10⁸ | 10⁸ | 2×10⁸ |
| Mallows, 129 | 10⁸ | 10⁸ | 2×10⁸ |
| J10, 129 (ikincil) | 2×10⁷ (4 × 5×10⁶) | 10⁷ (2 × 5×10⁶) | 3×10⁷ |
| U, M20, Mallows, 127 (ikincil) | 3×10⁷ (6 × 5×10⁶) | 10⁷ (2 × 5×10⁶) | 4×10⁷ |
Menü (§8.4): 10⁷ / sıfır (U, M20, Mallows; 129). Toplam hesap tavanı **40 çekirdek-saat**; ana koşular ~15 çekirdek-saat
beklenir. Bir koşu bütçesini aşarsa yeni bölüt başlatılmaz, eksik N raporlanır (ek). Disk ≤ 8 GB (hedef ≤ 3 GB): referans deltaları
birleştirilince silinir; puan dizileri saklanmaz (histogramlar + adaylar). **Aynı anda en çok 4 süreç.**

### 5.3 Tohumlar ve sürdürülebilirlik
Tohum = 26102600000 + sıfır·10⁸ + tür·10⁷ + kodeks·10⁶ + bölüt (sıfır U=1, M=2, L=3, J=4; tür: referans 1, sınama 2, menü 3,
adanmış 4, nadir-olay 5; kodeks 129 → 0, 127 → 1; bölüt 1'den). Bölütler atomik yazılır; `.say` dosyası bölütün tamam işaretidir;
yarım bölüt baştan koşulur (`kos/orkestra.py`). Çalışma günlüğü `sonuc/kosu_gunlugu.txt`, ara notlar `sonuc/ara_notlar.md`.

---

## 6. k = 0 (ya da çok küçük) çıkarsa: kuyruk

### 6.1 Sınır (her zaman)
§4.2'deki tek yanlı üst sınır ve artı-bir sıra.

### 6.2 Model-tabanlı kuyruk (R2; bölmeli ayrışım)
P(S ≥ s) ≈ Σ_λ P(S_λ ≥ s) (mercekler üzerinde birleşim; yüksek s'de örtüşme ihmal edilir, üst sınır yönünde), her mercekte
P(S_λ ≥ s) = Σ_bağlam π(bağlam) Σ_{b,f,w} P̂_B(b) P̂_F(f | bağlam) P̂_W(w | bağlam) 1{I_B(b) + I_boy + I_F(f) + I_W(w) − c(λ) ≥ s}
(grupların bağlam verildiğinde bağımsızlığı varsayımıyla; kesin maske olasılıkları referans tablosundan). Sayım puanında aynı
ayrışım kapı G'li aile tablosuyla (§3.1) ve birleşik maskenin kredisiyle.
**Doğrulama (önceden yazılmış):** R2'nin P(S ≥ s) eğrisi, sınama örnekleminin doğrudan sıklığıyla, doğrudan isabetin ≥ 30 olduğu
en yüksek dört eşikte karşılaştırılır; oranların hepsi [0,5; 2,0] içindeyse R2 "doğrulanmış" sayılır ve S_Q'daki değeri,
bu dört orandan çıkan bantla **model-tabanlı kestirim** olarak raporlanır (bir dış değerlemedir, doğrudan ölçüm değildir).
Doğrulanmazsa R2 değeri yalnız "doğrulanmamış" olarak yazılır, kararlarda kullanılmaz.

### 6.3 Bölmeli örnekleme (alt küme benzetimi)
Zorunlu değildir; yapılırsa yalnız bağımsız çapalarla (doğrudan ölçülebilen ara eşikler) doğrulanmış olarak raporlanır, aksi
halde "ölçülmedi". Kur'an merceğinde tek-mercek kuyruğu (alt sınır) da bu kapsamdadır.

---

## 7. En güçlü rakipler

Her sıfırda ve her iki puanda en yüksek puanlı 20 sınama kitabının (sayımda referans + sınama) merceği, profili, k_F, hücre
sayıları ve toplamları, k_H, \|U\| raporlanır. **Kur'an sınıfı** (önceden yazılmış): en iyi mercekte (i) kapı G tutar (yozlaşmamış:
k_F ≥ 2E, her hücre ≥ 2) ve (ii) kredi alan yüklemler 4 motif türünün (denge {B1,B2,F1,F2,R0c,R0e,R1c,R1e}, modül
{Qc,QS,Qh,QkF,QU,QkH,Qa,Qn,QH}, iki el {W1}, ardışıklık {K1,K2}) en az 3'ünden ve (iii) 4 düzeyin (kitap {B1,B2}; aile
{F1,F2,QS,Qh,QkF,K2}; hücre {R0c,R0e,R1c,R1e,K1,Qc}; birleşim {W1,QU,QkH,Qa,Qn,QH}) en az 3'ünden gelir. Değilse "dağınık
denk gelme". Kur'an bu sınıftadır (4 tür, 4 düzey). Ana hattın tablosundaki bilinen rakipler (J10 sahte 8/8, robot rakipleri, 7'de
rakipler) dondurmadan sonra aynı dille puanlanır.

---

## 8. İkincil analizler

### 8.1 (a) 127
Kur'an'ın 127 profili ve puanları; U, M20, Mallows'ta 127 sıfırı (§5.2 bütçeleri; kendi tablolarıyla).

### 8.2 (b) Eski literatüre koşullu sürüm
O = (n+a, 19) merceğinde 12 üyeli aile ve Σt = 1444, 6/6 ve 722/722, tek-t hücreleri 361/361; O' = O ∧ terazi. Koşullu kitaplar
10. denetimin havuzlarıdır (U129O 2×10⁵, M129O 10⁵, L129O 83.000, J129O 2×10⁴; O': U129P, M129P, L129P), örnekleyicileri ve
tohumlarıyla; bölütleri bit-bit yeniden üretilebilir (§0). **Mercek devralınmıştır** ((1,+1,19) sabit). O'nun kapattığı yüklemler
herkes için kapalıdır: F1, F2, R1e, Qc, QS, Qh (ve aile boyu); O ∧ kapı G altında R1c de zorunludur (tek-t hücreleri 19'ar, tek
q'lar ⇒ tek sayılar; ≥ 2 ⇒ 3/3) — sayımda kapalı. O' altında B1, B2 de kapalı.
Kalan yüklemler: B1, B2, R0c, R1c (yalnız bilgi puanında), K1, K2, W1, QU, QkH, Qa, Qn, QH (R0e O altında imkânsız).
- Sayım: kalan yüklemlerin kredisi (kapı G). Kur'an: O'da 8 (B1 B2 R0c K1 K2 W1 QU Qa), O''de 6.
- Bilgi: −log₂ P̂(B | O) − log₂ P̂(F-kalan | O) − log₂ P̂(W | O); olasılıklar aynı havuzdan **biri-dışarıda** (kitabın kendi katkısı
  çıkarılarak); grup çarpımı §2.9'daki gibi.
- Sıra p = (k+1)/(N+1) havuz içinde. Duyarlılık (yalnız sayım): serbest mercek — her O-kitabı (1,+1,19)'un kalanıyla öteki 405
  merceğin tam kredisinin büyüğünü alır.

### 8.3 (c) Duyarlılık
Sayım: kapılar G (birincil), GE, Gc, G0. Bilgi: V0 (birincil) / V1 (G kapısı) × önseller P0 (birincil), P1, P2, P3 — sekiz değişke
aynı sınama kitaplarında; her biri kendi Kur'an eşiğiyle.

### 8.4 (d) Bölme menüsü (dil seçiminin kalibrasyonu)
Menü (yoğunluğu 1/2 olan ikili bölmeler): t2 (t mod 2), n2 (n mod 2), a2 (a mod 2), h (n ≤ 57 / > 57), t4 (t mod 4 ∈ {0,1} / {2,3}).
"t mod 3 = 0" menüde yoktur: yoğunluğu 1/3'tür, "eşit sayı" onun için doğal bir denge değildir (57/57'ye zorlamak doğal olmayan
bir olay olur); yerine yoğunluğu 1/2 olan iki ek bölme (h, t4) konmuştur. Her kitap sıralı çift (D1, D2), D1 ≠ D2 (20 çift) ve mercek
seçer; yüklemler §2.4'teki gibi (D1, D2) ile. B1, D1 yalnız konuma/değere bağlıysa sabittir. Kurallar parite yapısına göre
(soyut kapsamlı sayımla bulundu, `kontrol/kural_sayim.c` desen kipi):
- satır paritesi (D1 ≡ t-paritesi): §2.7'nin tamamı;
- sütun paritesi (D2 ≡ t-paritesi) ya da köşegen parite ({D1, D2} = {n2, a2}, α ve β tek): P1, P2, P3, P4; p ≥ 5'te K1∧Qh→F2;
- paritesiz: P1–P4; p ≥ 5'te K1∧Qh→F2; R0e∧K1∧Qc→F2 ve R1e∧K1∧Qc→F2 (her p; ardışık ve toplam 4p ⇒ eşit satır p,p); β > 0'da
  R0e∧Qc∧Qh→F2, R1e∧Qc∧Qh→F2; p = 3'te K1∧Qc∧Qh→F2.
  (p = 3 ∧ β > 0'daki ek sonlu-uzay kuralları birincil dışı yapılarda kullanılmaz; Qc'yi p = 3'te gerçekleştiren aile yoktur.)
Yalnız **sayım puanı** (kapı G; kapı hücreleri seçilen çiftin hücreleridir). N = 10⁷ / sıfır. Raporlananlar: (d1) Kur'an'ın en iyi
(çift, mercek) ve puanı; (d2) her kitap çift seçerken Kur'an'ın sırası; (d3) t2 menüden herkes için çıkarılınca (Kur'an da t2'siz)
sıra; (d4) yalnız t2 ile (birincil sayım, karşılaştırma). Bilgi puanıyla menü analizi **ölçülmeyecek**.

---

## 9. Karar kuralları (ana hattın tahmininin değerlendirilmesi)

Birincil puan: **bilgi-ağırlıklı (V0, P0)**; sayım puanı (kapı G) her yerde yanında raporlanır ve aynı kurallarla ayrıca değerlendirilir.
Her sıfır için ölçülen sıra p̂:
- k ≥ 1 ise doğrudan p̂ = (k+1)/(N+1) ve %95 aralığı (belirsiz kitaplar varsa §4.3'teki aralık birleştirilir);
- k = 0 ise üst sınır U = 1 − 0,05^{1/N}; R2 doğrulanmışsa model-tabanlı kestirim ve bandı.
Hüküm (her sıfır): **"tuttu"** — nokta kestirimi (doğrudan ya da doğrulanmış R2) tahmin aralığında; **"tutmadı"** — %95 aralığı
(ya da R2 bandı) tümüyle aralığın dışında, ya da yalnız üst sınır varken U aralığın alt ucunun altında ("daha nadir"); **"karar
verilemedi"** — öteki durumlar (ör. yalnız aralık içinde ya da üstünde kalan bir üst sınır). Aralıklar: M20 ve Mallows
[10⁻¹⁰, 10⁻⁶] (merkez 10⁻⁸; \|log₁₀ p̂ + 8\| ayrıca yazılır), U [10⁻¹², 10⁻¹⁰]; eski literatür koşullu [10⁻⁴, 10⁻²] (U, M20, Mallows
ayrı ayrı; bilgi ve sayım). Bütün tahmin: bütün parçalar tuttuysa "tuttu", hiçbiri tutmayıp en az biri tutmadıysa "tutmadı",
öteki durumlarda "kısmen tuttu / karar verilemedi" ayrıntısıyla. J10 ve 127 tahmine dahil değildir (ikincil).

---

## 10. Rapor kuralları
`BUTUNLUK_RAPORU.md` (Türkçe; sonunda İngilizce özet). Her sayının yanında sıfır, örneklem, isabet ve belirsizlik; senaryolar
aralık ucu gibi sunulmaz; üst sınırlar sıklık gibi okunmaz. Ölçülmeyenler "ölçülmedi" diye yazılır (en azından: mercek ailesi ve
modül aralığı seçimi, dilin Kur'an görüldükten sonra yazılması, gruplar arası bağımsızlık yaklaşımının etkisi [Kur'an dışında],
literatür araması, sayım okulu, kültürel havuz, bilgi puanıyla menü analizi). Raporun depoya ekleneceği bilinerek yazılır.
Sonda `paket/butunluk_paket.tar.gz` (≤ 200 MB) ve SHA-256'sı.

## 11. Sapma politikası
Her sapma (bütçe, kod düzeltmesi, tanım netleştirmesi) `ek/EK_<UTC zaman>_<konu>.md` dosyasına, gerekçesi ve sonuca etkisiyle
yazılır. Puanlara bakıldıktan sonra tanım, ağırlık ya da eşik değiştirilmez; değiştirilmesi zorunlu bir hata bulunursa düzeltme ve
düzeltme öncesi sonuç birlikte raporlanır.
