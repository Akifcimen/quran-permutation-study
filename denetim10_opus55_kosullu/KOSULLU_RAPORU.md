# KOŞULLU ÖLÇÜM RAPORU — önceki literatürü taşıyan kitaplar arasında yeni yapı ne kadar sıra dışı?

**Tarih:** 23 Ekim 2026 · **Yazan:** Claude (Opus 5.5), `GOREV.md` gereği, `~/opus5.5/kosullu` klasöründe (depo `~/Desktop/hrf2` yalnız
okundu, hiçbir dosyası değiştirilmedi).
**Ön-kayıt:** `ONKAYIT.md`, hiçbir rastgele kitabın koşullu puanı hesaplanmadan yazıldı ve **2026-10-23T02:48:18Z**'de donduruldu (SHA-256
`4608859bd46f5951c8eac47a0a3901f4fac6747295fe7360d0b8ecc35ec71159`, `sonuc/onkayit_ozet.txt`; dondurma anındaki kod özetleri
`sonuc/onkayit_kod_sha256.txt`). Sapmalar `ek/` altındaki zaman damgalı dosyalardadır (§10).
**Birincil sayım:** Tevbe = 129 (Σa = 6236); çapa karşılaştırmaları 127 ile de koşuldu.
**Terimler.** *Araştırma*: depodaki çalışma (merdiven, kristal, İki El…); *bu ölçüm*: bu rapor. *O* = O1 ∧ O2 ∧ O3 (önceki literatür:
12 üyeli t ≡ 0 ailesi ve Σt = 1444 [1990]; 6/6 ve 722/722 [en geç 2015]; tek-t kolunun iki hücresi 361 ve 361 [en geç 2015]); *O'* = O ∧
terazi [1994]. Sıfırlar: *U* tekdüze; *M20* birincil sıralı (robotun uzunluk-sıralı zinciri); *Mallows* (ayet sayılarının büyükten küçüğe
sırasına merkezli); *J10* ikincil, **veriye merkezli** (gerçek dizilişin σ = 10 konumluk titreşimi).

> Bu rapor bir olay frekansını, bir kalibre sırayı ve ölçülmeyen bedelleri **ayrı ayrı** verir. Bu sayılar birbiriyle çarpılmamalı;
> farklı sıfırların ya da senaryoların sayıları bir aralığın uçları gibi okunmamalıdır. Her sayının yanında sıfır modeli, örneklem,
> isabet sayısı ve belirsizlik vardır; köşeli parantezler %95 aralığıdır.

---

## 0. Kısa cevaplar

### (1) Beyan edilen tam olay (terazi ∧ tam kristal ∧ İki El) ne kadar seyrek?

Sabit mercekte, arama bedeli olmadan, olayın frekansı:

| Sıfır, kodeks | P(A ∩ G ∩ W) | nasıl |
|---|---|---|
| Tekdüze, 127 | 8,8×10⁻¹⁶ [7,3–10,2] | bu ölçüm (Ç5): GPU'nun P(G)'si × 114 G-iskeletinde P(A∧W\|G) (2.796 isabet / 2,28×10⁹ adım) |
| Tekdüze, 127 (mevcut) | 1,1×10⁻¹⁵ [0,5–1,7] ve bağımsız 9,05×10⁻¹⁶ [5,1–13,0] | `kesif18c`; sekizinci denetim — bu ölçümle uyumlu |
| Tekdüze, 129 (mevcut) | 8,29×10⁻¹⁶ | sekizinci denetim (ön-kayıt gereği yeniden koşulmadı) |
| **M20, 129** | **1,69×10⁻¹³ [1,57–1,82]** | doğrudan, aktarımsız: P(G) = 1.490 / 2,21×10¹⁰ emisyon × P(A∧W\|G) (1.000 iskelet, 25.063 isabet / 10¹⁰ adım) |
| M20, 127 | 1,88×10⁻¹³ [1,56–2,23] | 200 / 2,65×10⁹ × 200 iskelet (4.975 isabet) |
| **Mallows, 129** | **1,79×10⁻¹³ [1,68–1,90]** | doğrudan, aktarımsız: P(G) = 2.948 / 3,81×10¹⁰ örnek × P(A∧W\|G) (1.000 iskelet, 23.095 isabet / 10¹⁰ adım) |
| Mallows, 127 | 2,14×10⁻¹³ [1,66–2,72] | 100 / 1,09×10⁹ × 100 iskelet (2.331 isabet) |

Merdivendeki sıralı sütunun "≈10⁻¹³ – 10⁻¹²"si (kristal şişmesinin aktarımı ve yerel-blok sıfırları) artık iki sıralı sıfırda doğrudan
ölçümdür. **Bu bir olay frekansıdır; olay veriye bakılarak tanımlandığından kanıt gücü ölçüsü değildir.**

### (2) Araştırma önceki literatüre ne ekledi? (Ö1, asıl ölçüm)

O'yu zaten taşıyan rastgele kitaplar arasında, aynı robot ve aynı defter bataryasıyla (82 olgu, sabit mercek, terazi kapalı, O'nun
gerektirdiği olgular herkes için kapalı, kesin gerektirmeler ayıklanmış), her kitabın kendi en iyi hikâyesine karşı Kur'an'ın kalan
yapısının sırası p = (k+1)/(N+1):

| Sıfır (129) | N koşullu kitap | **p — tekrarsız T2\* (birincil)** | p — robot T2 | p — tam birlikte-olasılık (sonsal medyan [%95]) |
|---|---|---|---|---|
| Tekdüze | 200.000 | **3,95×10⁻²** (k = 7.896; ~1/25) | 6,56×10⁻² | 6,2×10⁻² [5,8–6,6] |
| **M20 (birincil sıralı)** | 100.000 | **0,164** (k = 16.392; ~1/6) | 0,209 | 0,28 [0,27–0,29] |
| Mallows | 83.000 | **0,191** (k = 15.832; ~1/5) | 0,236 | 0,28 [0,27–0,30] |
| J10 (ikincil, veriye merkezli) | 20.000 | 0,263 (k = 5.263) | 0,308 | 0,30 [0,20–0,39] (yalnız düzey 1; zayıf çözünürlük) |

Teraziye de koşullayınca (O') sıra değişmez: tekdüze 3,94×10⁻² (N = 104.514), M20 0,185 (N = 150.660), Mallows 0,206 (N = 126.963).

**Robotun bataryasının görebildiği düzeyde, araştırmanın önceki literatüre eklediği yapı, o literatürü taşıyan kitaplar arasında
tekdüzede ~25'te 1, birincil sıralı sıfırda ~6'da 1, Mallows'ta ~5'te 1 sıklıkta ya da daha güçlü biçimde görülür.** Kur'an'ın O altında
kalan hikâyesi altı sınıf olgusundan oluşur (hücre dengesi, katsayı bandı, İki El'in iki olgusu, mukattaa paritesi, tekrarlı t); bunların
her birinin O altındaki olasılığı (Kur'an'ın k_F/k_G bağlamında) en az 0,02'dir (§5.2). Veriye bakılarak tanımlanmış **dar** yeni olayın
(tam kristal ∧ İki El) O'ya koşullu frekansı ayrıca: tekdüzede 2,3×10⁻⁶ [1,8–2,9], M20'de 6,5×10⁻⁵ [6,0–6,9], Mallows'ta 1,5×10⁻⁴
[1,5–1,6] (keşif; arama bedeli yok, sıra değil; §5.5).

### (3) Seçim ve arama bedelleri ödenince ne kadar kanıt kalır?

Ö1'in sırası **yalnız batarya aramasını** öder: her rastgele kitap aynı 82 olgudan kendi en iyi sekiz düğümlük hikâyesini kurar. Şunlar
**ölçülmedi**: merceğin (n ± a) seçimi (görev gereği devralınmış sayıldı), modülün (19) seçimi, bataryanın Kur'an görüldükten sonra
kurulması, dar olayın seçimi, 19 literatürünün onlarca yıllık araması (≥ 317–770 yayımlanmış sûre-ayet iddiası), sayım okulunun seçimi
(README'nin ×7'si varsayımsal bir Bonferroni **üst sınırıdır**, bir sıklık değildir; burada uygulanmadı), O'nun hangi olgulardan kurulacağı
(KESIF not 20'nin tarihlemesi devralındı), kültürel havuz. Bu bedeller simetrik fiyatlansaydı rakip kitaplar ek serbestlikten kazanırdı;
Kur'an ise ancak sabit seçimlerden daha iyi bir seçenek bulursa kazanırdı. Koşullu sıradaki net etkileri ölçülmedi. **Bu ölçümün
verebildiği kalan kanıt, yukarıdaki batarya-bedelli koşullu sıradır; birincil sıralı sıfırda bu sıra zaten sıradandır (p = 0,16; Mallows'ta 0,19).**

---

## 1. Görev ve yapılanlar

`GOREV.md` üç ölçüm ister: Ö0 (robotun cebirsel tekrarlarının temizlenmesi), Ö1 (önceki literatüre koşullu kalibrasyon: P(yeni | eski))
ve Ö2 (tam çekirdeğin sıralı sıfırlarda doğrudan ölçümü). Ön-kayıt bütün tanımları, örneklem hedeflerini, tohumları, bütçe tavanlarını,
doğrulama ölçütlerini ve rapor kurallarını dondurmadan önce sabitledi. Dondurmadan önce yalnız kod ve örnekleyici doğrulamaları, Ç1 çapası,
olgu birlikte-görülme taraması ve puan içermeyen oran yoklamaları yapıldı (ONKAYIT §9). Kod, betikler, ön-kayıt, ekler ve küçük çıktılar
bu klasörde ve `paket/kosullu_paket.tar.gz`'tedir; koşullu kitap havuzları tohumlarla yeniden üretilir (§11).

---

## 2. Ne ölçüldü, nasıl

**Mercek ve olgular.** Mercek sabittir: t = n + a ve u = n − a, modül 19. F = {n : 19 | t} (aile), G = {n : 19 | u} (fark eli), U = F ∪ G.
Batarya robot4_m'nin defter bataryasıdır (99 olgu, 23 düğüm); matris olguları, "G: Σw=m·kG", "U: Σv(U) kare/asal-kuvvet" ve terazi
olguları ("kitap:" ×12) herkes için kapalıdır: **82 etkin olgu**. Robotun puanı korunur: olgu olasılığı (isabet + 0,5)/(deneme + 1),
(k_F, k_G) bağlamına koşullu (MINTR = 1.000), düğüm değeri düğümdeki en nadir doğru olgunun −log10 p'si, **T2 = en büyük 8 düğüm değerinin
toplamı**. Kodum robotun sabit-mercek olgularının ve U/J10/M20 örnekleyicilerinin bit-birebir portudur (129 ve 127'de 6 × 100.001 kitapta
örnekleyici ve olgu biti farkı 0); Ö0(a)'nın 36 koşusunda robotun T2'si her kitapta ≤ 6,8×10⁻⁷ farkla yeniden üretildi.

**Koşul.** O1 = 12 üyeli t ≡ 0 ailesi ve Σt(F) = 1444; O2 = çift-t yarısı 6 üye ve Σt(çift-t) = 722 (⇒ 6/6, 722/722); O3 = tek-t kolunun
iki hücresi (sûre no paritesine göre) 361 ve 361; O4 = terazi (|{t çift}| = 57 ∧ Σt(çift) = Σa). **Birincil O = O1 ∧ O2 ∧ O3; ikincil
O' = O ∧ O4.** O'nun gerektirdiği beş olgu (F: k≥2E, F: m²|Σq, F: Σq kare, F: v-yarılar eşit, F: v-yarılar m²|) herkes için kapalıdır;
havuzdaki her kitapta doğrulukları ayrıca denetlendi.

**Koşullu sıfırlar.**
- *Tekdüze (U), kesin.* Etiketli permütasyon = aile yapısı γ × tamamlama τ; P(γ) ∝ Φ(k), Φ = yeni aile üyesi oluşturmayan tamamlama
  sayısı (19 ayrık bloklu yasak tahtanın kale polinomu). Öneri k ~ Π C(6,k)·C(c_r,k)·k!, γ'de koşul, Φ/(M!·B) ile kabul (B, Σk = 12'li
  8,3×10⁷ vektörün tam taramasıyla), τ düzgün (ret). MCMC ve ağırlık yoktur; çıktılar bağımsızdır.
- *M20 (birincil sıralı).* Robotun M sıfırı birebir (64 zincir, tam azalan diziden 30.000 adım yakma, 20 önerilik emisyon, S = Σn·a ≤
  S_obs kısıtlı takaslar); koşullu sıfır = **O'yu sağlayan emisyonlar** (zincir O içinde dolaştırılmaz; donma sorunu yoktur).
- *Mallows.* P ∝ q^I, I = büyükten küçüğe sıraya aykırı kesin çift sayısı (eşitler terslik değil); **q yalnız E[I] = I_gözlenen** için
  ayarlıdır (129: I = 993, q = 0,9145731; 127: q sekizinci denetimin değeriyle 13 haneye kadar aynı). Kesin Lehmer örneklemesi + ret.
- *J10 (ikincil, veriye merkezli).* Robotun J10'unun dağılımca aynısı (anahtar = konum + 10·Z) + ret.

**Puan ve sıra.** Olgu olasılıkları aynı koşullu havuzdan, **biri-dışarıda (LOO)** tabloyla tahmin edilir (Kur'an tam tabloyla). Birincil
istatistik **tekrarsız T2\***: aynı düğümden en çok bir olgu içeren ve listedeki hiçbir kesin gerektirmeyi (öncül ve sonuç birlikte)
içermeyen hikâyeler üzerinde en büyük 8 olgu toplamının en büyüğü (§4.3). Kur'an'ın sırası p = (k+1)/(N+1), k = #{T2\*_i ≥ T2\*_Kur'an}.
Kur'an'ı geçen ya da ona yakın (T2\* ≥ Q − 2) bütün kitaplar ve [Q − 3, Q − 2) bandından 500 kitap için hikâyenin **tam
birlikte-olasılığı** koşullu sıfırda doğrudan sayıldı (iki düzeyli: aileye bağlı olgular iskelet sayımıyla, kalanlar iskelet başına 50
tamamlamayla; LOO; J10'da ve O' havuzlarında yalnız düzey 1).

**Ö2.** P(A∩G∩W | sıfır) = P(G | sıfır) × ort_iskelet P(A∧W | G-iskeleti): P(G) o sıfırın kendi havuz sayaçlarından (G ⊂ O), P(A∧W |
iskelet) her G-iskeleti için iskelet sabit tamamlama zinciriyle (her adımda sayım; iskelet başına 10⁷ adım, en çok 1.000 iskelet).
Aktarım yoktur.

**Bütçe.** Havuzlar sabit tohumlu bölütlerle üretildi (atomik yazım, sürdürülebilir); her havuzun çekirdek-saat tavanı önceden
yazılıydı. Tavana ulaşılınca yeni bölüt başlatılmadı (§9, ekler).

---

## 3. Doğrulamalar ve çapalar

| Çapa | Ölçüt (ön-kayıt) | Sonuç | Hüküm |
|---|---|---|---|
| **Ç1** tekdüze, 127: P(G∩W \| O1∧O2) | GPU 62/444.513.933 = 1,395×10⁻⁷ [1,07–1,79] | P(G\|C) = 965/2,6×10⁷ = 3,71×10⁻⁵ (GPU 3,86×10⁻⁵; −1,2σ) × P(W\|G) = 7.572/1,93×10⁶ = 3,92×10⁻³ ⇒ **1,456×10⁻⁷** (oran 1,04); doğrudan G∩W 5 isabet (beklenen 3,6) | **tuttu** (dondurmadan önce) |
| **Ç2** Mallows, 127: P(G) | sekizinci denetim 18/2×10⁸ = 9,0×10⁻⁸ [5,3–14,2]; şişme ×126 [75–199] | 100/1,09×10⁹ = **9,17×10⁻⁸** [7,4–11,0]; şişme **×128** [103–154] | **tuttu** |
| **Ç3** M20, 127: P(B), P(C), şişme | `kesif25` (bağımsız MCMC) 5,49×10⁻⁴, 5,16×10⁻⁵; şişme: 5. denetimin M20/J10 için verdiği ×88–160 ile örtüşme | 5,458×10⁻⁴ (z −0,85), 5,138×10⁻⁵ (z −0,22) (2,65×10⁹ emisyon); şişme **×106** [91–120] | **tuttu** |
| Ç3 başlangıç denetimi (129) | gerçek dizilişten başlayan zincirler 3σ içinde | P(O) 5.000/1,10×10⁹ (beklenen 4.981; z +0,3); B z +0,7; C z +2,5 (Poisson; zincir içi kümelenme hesaba katılmadan) | **tuttu** |
| **Ç4** tamamlama zincirleri | ön-kayıtlı beş frekans (\|U\|=m, m\|Σa(U), MQ 14/15, A, W) 3σ içinde (eşleşmiş z) | U (200 iskelet × 2.000, kesin tamamlamaya karşı): +1,33 / +1,71 / −2,84 / −0,90 / +0,99; M20 (5.000 × 200, havuzun doğrudan frekansına karşı): +0,83 / +0,62 / −1,26 / +1,62 / −0,39; Mallows (5.000 × 200, havuzun doğrudan frekansına karşı): −1,91 / −0,44 / −0,81 / −1,20 / +0,62. Tamamlamayla değişen ve beklenen sayısı ≥ 50 olan bütün olgularda en büyük \|z\|: U 2,84, M20 2,34, Mallows 1,91 | **tuttu** (üç sıfırda) |
| **Ç5** tekdüze, 127: P(A∧W \| G), çekirdek | `kesif18c` 1,51×10⁻⁶ [0,69–2,45]; 1,1×10⁻¹⁵ [0,5–1,7]; bağımsız 9,05×10⁻¹⁶ [5,1–13,0] | 114 G-iskeleti × 2×10⁷ adım, 2.796 isabet: **1,23×10⁻⁶** [1,03–1,43] (κ = 0,99) ⇒ çekirdek **8,8×10⁻¹⁶** [7,3–10,2] | **tuttu** |

Tekdüze kesin örnekleyicinin ayrıca: kesin P(|F| = 12) = 0,0101534219 (127; yedinci oturumun büyük-tamsayı değeri 1,015342×10⁻²) ve
0,0101497464 (129); öneri k-marjinalleri kesin dağılımla uyumlu (χ² 113 / ≈103 hücre; Φ-kabullü 101); 4×10⁹'dan fazla öneride Φ ≤ B
ihlali 0.

**Açık gözlem — GPU'nun D sayacı.** Aynı C-koşullu tekdüze örneklemde (2,6×10⁷ C-kitap) P(D | C) = 196.317/2,6×10⁷ = 7,551×10⁻³, GPU'nun
7,494×10⁻³'ünden **+%0,76 (3,3σ)** yüksek; P(E|C) (+%0,2), P(W|C) (+%0,5), P(A|C) (+%0,0), P(DW|C) (+%2; 0,7σ) ve P(G|C) GPU ile
uyumlu. 86 milyarlık CPU arşivi (aynı tanımlar, ayrı tohumlama) 7,537±0,069×10⁻³ ile hem bu ölçümle hem GPU ile uyumludur. Örnekleyici
tasarım gereği kesin ve öneri dağılımı sınandı; farkın kaynağı bulunamadı. Ç1'in ölçütü G∩W'dir; D farkı hiçbir sonucu etkilemez
(Kur'an'ın "hücreler dengeli" olgusunun puanında 0,003 mertebe).

---

## 4. Ö0 — tekrarların temizlenmesi

### 4.1 Ö0(a) Dokuzuncu incelemenin deneyi

Robotun kendi ikilisiyle (robot4_m, `ROBOT_MODS=19`, `ROBOT_MINTR=1000`) her sıfır × kodeks için üç bağımsız tablo (2×10⁶) ve ayrı
tohumlu 2×10⁶ değerlendirme kitabı; aynı kitaplar bizim portla yeniden kuruldu ve robotun sabit-mercek T2'si 36 koşunun hepsinde her
kitapta ≤ 6,8×10⁻⁷ farkla yeniden üretildi. Sayılar: k = Kur'an'ı geçen/eşleyen kitap (N = 2×10⁶); üç tekrarın değerleri ve (ortalaması).
"Tek düzeltme" incelemenin kaldırdığı tek tekrardır (R1: G: Σn=Σa → G: m²|Σq); "T2*" §4.3'teki tam tekrarsız puandır (koşulsuz liste L0).

| Batarya | Kodeks | Sıfır | k özgün | k tek düzeltme | k tam tekrarsız T2* | inceleme (özgün → tek düzeltme) |
|---|---|---|---|---|---|---|
| **defter (82 olgu; ön-kayıtlı ölçüt)**, terazi kapalı | 129 | U | 126 / 380 / 302 (269) | 75 / 248 / 181 (168) | 97 / 291 / 253 (214) | 116 → 53 |
| | 129 | J10 | 6.551 / 7.536 / 7.792 (7.293) | 4.442 / 5.237 / 5.408 (5.029) | 2.813 / 3.315 / 3.505 (3.211) | 1.135 → 573 |
| | 129 | M20 | 7.183 / 5.631 / 5.661 (6.158) | 5.061 / 3.872 / 3.835 (4.256) | 3.048 / 2.282 / 2.208 (2.513) | 827 → 393 |
| | 127 | U | 220 / 194 / 173 (196) | 141 / 122 / 110 (124) | 177 / 166 / 138 (160) | |
| | 127 | J10 | 8.761 / 7.688 / 8.118 (8.189) | (5.747) | (3.644) | |
| | 127 | M20 | 7.773 / 5.252 / 7.372 (6.799) | (4.724) | (2.840) | |
| defter, terazi açık (üç tekrarın ortalaması) | 129 | U / J10 / M20 | 1 / 59 / 54 | 1 / 35 / 35 | 1 / 20 / 21 | |
| **iddia (60 olgu; keşif, EK)**, terazi kapalı | 129 | U | 123 / 274 / 249 (215) | 48 / 101 / 95 (81) | 13 / 34 / 28 (25) | 116 → 53 |
| | 129 | J10 | 953 / 1.134 / 1.212 (1.100) | 478 / 527 / 570 (525) | 182 / 226 / 239 (216) | 1.135 → 573 |
| | 129 | M20 | 1.177 / 933 / 952 (1.021) | 561 / 474 / 479 (505) | 235 / 208 / 176 (206) | 827 → 393 |

Okuma. (i) Ön-kayıtlı ölçütle (defter bataryası) inceleme **yeniden üretilemedi** (`ek/EK_20261023T034012Z_o0a_iddia_bataryasi.md`):
defter bataryasında sıralı sıfırların sayıları incelemeninkinin J10'da 6,4, M20'de 7,4 katıdır. İncelemenin sayıları **iddia
bataryasında** (robot_m'nin 60 olgusu; altıncı denetimin E1 satırı) yeniden üretilir: J10 1.100 → 525, M20 1.021 → 505 (inceleme 1.135 → 573, 827 → 393); tekdüzede
yalnız ilk tekrar (123 → 48) eşleşir. İncelemenin büyük olasılıkla iddia bataryasıyla koştuğu sonucuna varıyorum (keşif). (ii) Koşulsuz tek
tablolu sayımlar Poisson'dan çok daha geniş bir **tablo belirsizliği** taşır: Kur'an'ın en güçlü olgusu ("hücreler dengeli", k_F = 12
bağlamında ~10⁻³·⁶) 2×10⁶'lık tabloda birkaç isabetle tahmin edildiğinden Kur'an'ın T2'si tablodan tabloya 0,5 mertebe oynar (U, 129:
12,28 / 11,75 / 11,82) ve k üç kat değişir. (iii) **Tam tekrarsız puan tek düzeltmenin ötesine geçer.** Özgünden T2*'a: iddia bataryasında
J10 1.100 → 216 (×5,1), M20 1.021 → 206 (×5,0), tekdüze 215 → 25 (×8,6); defter bataryasında J10 7.293 → 3.211 (×2,3), M20 6.158 →
2.513 (×2,5), tekdüze 269 → 214 (×1,3). Tekdüzede (defter) Kur'an'ın koşulsuz hikâyesinde de bir tekrar vardır (R19: hücreler dengeli ∧ F'de aynı
t ≥ 5 kez → F: k≥2E); T2* Kur'an'ın puanını da düşürür (12,28 → 11,81), bu yüzden orada T2* sayısı tek düzeltmeninkinden büyüktür
(214 > 168). Tek düzeltmenin kendi etkisi (iddia bataryasında J10 ×2,1, M20 ×2,0, tekdüze ×2,7) incelemeyle uyumludur.

### 4.2 Ö0(b) Kesin gerektirmeler (sabit mercek (1,1,19))

**Yöntem.** (i) Her olgunun tanımı robot kaynağından okunup cebirsel ilişkiler türetildi; (ii) tarama: 129'da U (10⁷), M20 (10⁷), J10
(2×10⁶), Mallows (2×10⁶), kesin tekdüze O-koşullu (10⁵) ve aile boyları zorlanmış "çeşitli" kitaplar (10⁷); 127'de U (5×10⁶) ve
O-koşullu (5×10⁴); ayrıca M20 (5.000) ve Mallows (1.500) O-kitapları — toplam ~4,1×10⁷ kitapta bütün olgu çiftlerinin ve üçlülerinin
birlikte-görülme sayıları; *aday* = örneklemde karşı-örneği olmayan çift X → Y ya da üçlü {X1, X2} → Y. (iii) Her aday ya kanıtlandı ya
düşürüldü: **listeye yalnız tanımdan ya da her kitapta geçerli bir kanıtla gösterilenler girdi.** Kanıtlanamayan adaylar (örneklem
yanılsaması olabilir; ör. G'nin iki u-yarısının da 0 olduğu sık durumdan doğan "→ G: Σn=Σa" üçlüleri) kullanılmadı ve desteğiyle
`sonuc/cikarim_adaylar_*.txt`'te listelendi.

**Yardımcı önermeler.** L1: n ∈ F ∩ G ⟺ 19 | n ∧ 19 | a. L2: x = Σn(F) mod 19, y = Σn(G) mod 19 ise Σn(U) ≡ x + y, Σa(U) ≡ y − x,
Σt(U) ≡ 2y, Σu(U) ≡ 2x (mod 19). L3: Σt(F) ve Σu(G) her zaman 19'a bölünür; 19'a bölünen bir tam kare 361'e bölünür. L4: F'de t çift ⟺
c = t/19 çift. L5: kitabın Σt'si tektir (12791 / 12789); çoklukta 54 tek değer vardır.

**Düğüm-arası kesin gerektirmeler L0** (puanlamayı etkileyen; `src/kurallar.h`):

| Kural | Öncül → sonuç | Kanıt |
|---|---|---|
| R1 | G: Σn=Σa → G: m²\|Σq | Σu(G) = 0 (incelemenin örneği) |
| R2, R3 | F: v-yarılar m²\| ; F: n-yarılar m²\| → F: m²\|Σq | iki yarı 361'e bölünür ⇒ toplamı |
| R4, R5 | G: v-yarılar m²\| ; G: n-yarılar m²\| → G: m²\|Σq | aynı |
| R6 | F: m\|Σn → U: m\|Σw(U) | L2: Σu(U) ≡ 2x |
| R7 | F: Σn=Σa → U: m\|Σw(U) | Σt(F) = 2Σn(F), L3 ⇒ x ≡ 0 |
| R8 | G: m\|Σn → U: m\|Σv(U) | L2: Σt(U) ≡ 2y |
| R9–R12 | {F: m\|Σn ya da F: Σn=Σa} ∧ G: m\|Σn → U: m\|Σn(U) ; U: m\|Σa(U) | x ≡ y ≡ 0, L2 |
| R13–R16 | R: kF=kG ∧ F: k≥2E → G: k≥2E (ve simetriği; k=m için de) | tanım |
| R17 | F: k=m ∧ G: k=m → R: kF=kG | tanım |
| R18 | F: v-yarılar eşit ∧ RP: F'de aynı v ≥5 kez → F: k≥2E | eşit yarılar ⇒ 4 \| k; beş eş t aynı paritededir ⇒ yarı ≥ 5 ⇒ k ≥ 12 |
| R19 | F: hücreler dengeli ∧ RP: F'de aynı v ≥5 → F: k≥2E | "hücreler dengeli" "v-yarılar eşit"i içerir ⇒ R18 |
| R20, R21 | F: k=m ∧ U: \|U\|=2m → G: k≥2E (ve simetriği) | k_G = 38 − 19 + \|F∩G\| ≥ 19 |
| R22–R25 | G: k=m ∧ U: \|U\|=m → U: m\|Σw(U) ve F: m\|Σn (ve F/G simetriği) | \|U\| = k_G ⇒ F ⊆ G ⇒ F üyelerinde 19 \| n (L1) ⇒ x ≡ 0 |
| R26, R27 | v-yarılar Δ∈{m²,2m²} ∧ katsayı bandı → k≥2E (F ve G) | Δ katsayı biriminde 19 ya da 38; bant + eşit-boy parite yarılarıyla ulaşılabilecek en büyük \|Σçift − Σtek\| k = 2…10 için 1, 2, 5, 8, 13 (tam tarama, `kontrol/bant_kanit.py`) < 19 ⇒ k ≥ 12 |

Eşdeğerlikler aynı kurallarla örtülür (k_F ≥ 1 iken U: m|Σw(U) ⟺ F: m|Σn; k_G ≥ 1 iken U: m|Σv(U) ⟺ G: m|Σn). Düğüm-içi gerektirmeler
(K2 → K1; Σq kare → m²|Σq; F: Σn=Σa → F: m|Σn; hücreler dengeli → v-yarılar eşit; …) robotun "düğümde en nadir olgu" kuralıyla zaten
tekrarsız sayılır. **Hiçbir kitapta doğru olamayan olgular** (iki kodekste): kitap: vE=Σn, vO=Σa, vE=vO, nE=nO, nO=aO, nO=aE; kitap:
aE=aO (129'da); BL: Σv(1..57)=Σv(58..114); BL: üç 38'lik blok m|Σv; W: m-bölünen pencere az. Kanıtlar `ONKAYIT.md` §3.2'dedir.

**O altında ek kesin sonuçlar L_O.** O'nun gerektirdiği (herkes için kapalı): F: k≥2E, F: m²|Σq (361 | 1444), F: Σq kare (1444 = 38²),
F: v-yarılar eşit, F: v-yarılar m²| (722 = 2·361). O altında imkânsız: F: k=m, F: 114|Σq, F: v-yarılar Δ∈{m²,2m²}, F: katsayı adım-2,
F: n-yarılar eşit, F: n-yarılar Δ∈{m²,2m²}. **RO1:** R: kF=kG → G: k≥2E (k_F = 12 ⇒ k_G = 12). **RO2:** U: |U|=2m → G: k≥2E
(k_G = 26 + |F∩G|). O' altında ek kural yoktur. Tarama, listedeki kurallar dışında yeni kesin gerektirme bulmadı.

### 4.3 Ö0(c) Tekrarsız puan T2*

Bir kitabın hikâyesi S: doğru ve kapalı olmayan olgulardan, düğüm başına en çok bir olgu içeren küme. S **gereksiz**dir eğer etkin
listede öncülü P ve sonucu c olan bir kural için P ∪ {c} ⊆ S ise (gerektirilen olgu, gerektirenin bulunduğu hikâyede ek puan almaz;
karşılıklı gerektirmelerde ikisinden yalnız biri sayılır). **T2\* = gereksiz olmayan hikâyeler üzerinde en büyük 8 olgu değerinin
toplamının en büyüğü** (kesin numaralandırmayla, `src/puan.h`). Hiçbir kural uygulanmıyorsa T2* = T2. Etkin liste koşulsuzda L0, O ve O'
altında L0 ∪ L_O. Kesin gerektirme olmayan kısmi bağımlılıklar T2*'da kalır; onları tam birlikte-olasılık (§5.3) düzeltir.

---

## 5. Ö1 — koşullu kalibrasyon (asıl ölçüm)

### 5.1 Birincil sıra (koşul O, kodeks 129)

| Sıfır | N koşullu kitap | Kur'an T2* | k (T2* ≥ Kur'an) | **p = (k+1)/(N+1)** | k/N %95 (Clopper–Pearson) | robot T2 ile p | tekrarı ayıklanan kitap |
|---|---|---|---|---|---|---|---|
| Tekdüze | 200.000 (bağımsız) | 6,273 | 7.896 | **3,95×10⁻²** | [3,86; 4,03]×10⁻² | 6,56×10⁻² (k = 13.122) | %15,1 |
| **M20 (birincil sıralı)** | 100.000 (85.335 bağımsız küme) | 4,761 | 16.392 | **0,164** | [0,162; 0,166] | 0,209 (k = 20.876) | %13,9 |
| Mallows | 83.000 (bağımsız; hedef 10⁵, bütçe tavanı) | 4,524 | 15.832 | **0,191** | [0,188; 0,193] | 0,236 (k = 19.560) | %13,8 |
| J10 (ikincil, veriye merkezli) | 20.000 (bağımsız; hedef 2×10⁴) | 3,971 | 5.263 | 0,263 | [0,257; 0,269] | 0,308 (k = 6.161) | %13,7 |

Kodeks denetimi (tekdüze, 127, 2×10⁵ bağımsız): T2* 6,295, k = 7.590 → 3,80×10⁻² (robot T2 ile 6,37×10⁻²). M20'de isabetlerin %14,7'si
aynı zincirin bir önceki emisyonu da O iken gelir (en uzun dizi 8); sıra kestiricisi yansız, etkin örneklem biraz küçüktür.

### 5.2 Kur'an'ın O altındaki hikâyesi

O'nun gerektirdiği beş olgu herkes için kapalı olunca Kur'an'da altı açık olgu kalır; hiçbir gerektirme kuralı uygulanmaz (T2* = T2).
Olgu olasılıkları koşullu havuzlardan (robotun tablosu: Kur'an'ın k_F/k_G bağlamında, (isabet + 0,5)/(deneme + 1)):

| Olgu (düğüm) | tekdüze p | M20 p | Mallows p |
|---|---|---|---|
| F: hücreler dengeli (3/3/3/3; O altında çift-t hücreleri 18/20, yani D) | 0,123 | 0,159 | 0,183 |
| F: katsayı bandı (ailenin farklı katsayıları kesintisiz: 5–9) | 0,113 | 0,284 | 0,303 |
| U: \|U\| = m (birleşim 19 sûre) | 0,125 | 0,125 | 0,118 |
| U: m \| Σa(U) (19 \| 1159) | 0,053 | 0,053 | 0,052 |
| MQ: 29 mukattaa sûresi t-paritesine 14/15 bölünür | 0,293 | 0,292 | 0,291 |
| RP: F'de aynı t ≥ 5 kez (t = 114 beş kez) | 0,020 | 0,200 | 0,300 |
| **T2\*** | **6,27** | **4,76** | **4,52** |

Önceki literatürün yapısı verildiğinde araştırmanın kendi katmanları **sınıf düzeyinde** sık görülür: çift-t hücrelerinin dengesi
O-kitaplarının tekdüzede %12,3'ünde, M20'de %15,9'unda, Mallows'ta %18,3'ünde; kesintisiz katsayı bandı tekdüzede %11,3'ünde, M20'de
%28,4'ünde, Mallows'ta %30,3'ünde vardır. Kur'an'ın puanına en çok katkıyı İki El'in iki olgusu verir (birlikte 2,2 mertebe; İki El
olayının tamamı W, O-kitaplarında tekdüzede 4,1×10⁻³, M20'de 4,0×10⁻³, Mallows'ta 4,1×10⁻³; §5.5) ve tekdüzede tekrarlı t (1,7 mertebe;
M20'de 0,7, Mallows'ta 0,5). Rastgele O-kitapları 82 olgudan sekiz düğümlük hikâyelerini kurarken Kur'an altı düğüm doldurur: defter bataryasının araştırma boyunca "boş çıkan" ailelerinde (Mekkî/Medenî, pencere, ayna, blok…)
Kur'an'ın mukattaa paritesi dışında olgusu yoktur.

### 5.3 Tam birlikte-olasılık (Kur'an'ı geçen ya da ona yakın kitaplar)

T2* bağımsız olmayan olguları çarpar (yalnız kesin gerektirmeler ayıklanır). Hikâyenin bütün olgularının birlikte görülme sıklığı koşullu
sıfırda doğrudan sayıldı.

| Sıfır | değerlendirilen kitap | Kur'an P_tam (isabet) | S_tam (T2*'a göre) | kesin sınırlar | **p_tam (sonsal medyan [%95])** |
|---|---|---|---|---|---|
| Tekdüze | 41.574 yakın + bütün bant 36.730 (ön-kayıt kuralı: denetim bandında 2/500 geçti) | 1,98×10⁻⁶ (1.976 isabet; R = 5.000 çözünürlük eki) | 5,70 (−0,57) | k ∈ [3.729; 23.545] → p ∈ [1,9×10⁻²; 1,2×10⁻¹] | **6,2×10⁻²** [5,8–6,6] |
| M20 | 56.809 yakın + 500 bant (0 geçen) | 1,14×10⁻⁴ [0,99–1,30] (212) | 3,94 (−0,82) | k ∈ [23.528; 31.548] → p ∈ [0,24; 0,32] | **0,28** [0,27–0,29] |
| Mallows | 51.391 yakın + 500 bant (0 geçen) | 1,38×10⁻⁴ [1,19–1,60] (184) | 3,86 (−0,67) | k ∈ [20.076; 26.481] → p ∈ [0,24; 0,32] | **0,28** [0,27–0,30] |
| J10 (yalnız düzey 1) | 15.033 yakın + 500 bant (0 geçen) | 2,0×10⁻⁴ [0,5–5,1] (4) | 3,70 (−0,27) | k ∈ [0; 9.121] → p ∈ [5×10⁻⁵; 0,46] (zayıf çözünürlük) | 0,30 [0,20–0,39] |

Kur'an'ın olguları O altında birbirine pozitif bağlıdır (hücre dengesi, katsayı bandı ve tekrarlı t'nin üçü de F ailesinin katsayı
yapısının işlevidir): tam olasılığı çarpımdan tekdüzede 0,57, M20'de 0,82, Mallows'ta 0,67 mertebe daha sıktır; rakiplerde bu fark medyanda
~0'dır (tekdüze +0,02, M20 −0,02, Mallows −0,05). Tam puanlama bu yüzden Kur'an'ı rakiplerine göre geriletir. (Yedinci oturumun koşulsuz tam puanlamada bulduğu
×15–16'lık rakip azalması O altında Kur'an lehine işlemez.) Tekdüzede rakip hikâyelerinin bir kısmı az isabetlidir; kesin sınırlar
geniştir, sonsal medyan Jeffreys önseline dayanır.

### 5.4 İkincil analizler

| Analiz | Tekdüze | M20 | Mallows |
|---|---|---|---|
| **O' = O ∧ terazi** (iskelet sabit yeniden tamamlama; N) | 3,94×10⁻² (104.514; robot T2 6,65×10⁻²) | 0,185 (150.660; robot T2 0,230) | 0,206 (126.963; robot T2 0,252) |
| O' tam birlikte-olasılık (yalnız düzey 1) | çözülemedi (Kur'an'ın hikâyesi 0 isabet; kesin sınırlar p ∈ [10⁻⁵; 0,20]; `ek/EK_20261023T050132Z_U_Ousu_bant.md`) | 0,31 [0,27–0,34] (kesin sınırlar 0,22–0,37) | 0,31 [0,28–0,35] (kesin sınırlar 0,22–0,38) |
| **Terazi açık (keşif)** — 1994 terazisi araştırmanın katkısı sayılsaydı | 9,6×10⁻⁴ (T2* 9,77; k = 191 / 2×10⁵) | 7,8×10⁻³ (T2* 8,01; k = 776 / 10⁵) | 1,0×10⁻² (T2* 7,76; k = 842 / 83.000) |

P(terazi | O): tekdüzede 3,2×10⁻⁴ (64/2×10⁵ doğrudan) ve 3,48×10⁻⁴ (iskelet başına kesin tamamlamayla), M20'de 5,6×10⁻⁴ (56/10⁵) ve
5,02×10⁻⁴; terazi aile yapısından neredeyse bağımsızdır, bu yüzden teraziye koşullamak sırayı değiştirmez. Terazi açık okuma, 1994'te
yayımlanmış bir olguyu araştırmanın katkısı sayacağı için birincil değildir; yalnız terazinin puanda ne kadar ağır bastığını gösterir.
J10'da (veriye merkezli) terazi açık: T2* 7,62, k = 215 / 20.000 → 1,08×10⁻².

### 5.5 Olay düzeyinde koşullu frekanslar (sabit olay; arama bedeli yok; sıra değil)

| P(· \| O), 129 | tekdüze (2×10⁵) | M20 (10⁵) | Mallows (83.000) |
|---|---|---|---|
| D: dört hücre 3/3/3/3, çift hücreler 18/20 | 0,123 (24.675) | 0,159 (15.881) | 0,183 (15.208) |
| E: kefe torbaları {6⁵,8}/{5³,7²,9} | 1,9×10⁻³ (381) | 4,6×10⁻² (4.574) | 0,107 (8.845) |
| G: tam kristal | 5,85×10⁻⁴ (117) | 1,49×10⁻² (1.490) | 3,55×10⁻² (2.948) |
| W: İki El | 4,1×10⁻³ (817) | 4,0×10⁻³ (400) | 4,1×10⁻³ (338) |
| A: terazi | 3,2×10⁻⁴ (64) | 5,6×10⁻⁴ (56) | 5,8×10⁻⁴ (48) |
| **G ∧ W** = P(G\|O) · ort P(W\|G-iskeleti) (keşif) | **2,3×10⁻⁶** [1,8–2,9] (P(W\|G) = 3,98×10⁻³, 117 iskelet; doğrudan 0) | **6,5×10⁻⁵** [6,0–6,9] (4,34×10⁻³, 1.000 iskelet; doğrudan 4) | **1,5×10⁻⁴** [1,46–1,62] (4,33×10⁻³, 1.000 iskelet; doğrudan 15) |

Görevin çapasının (P(G∩W | O1∧O2) = 1,39×10⁻⁷, tekdüze, 127) sıralı karşılığı da havuz sayaçlarından çıkar (keşif): **M20, 127:
P(G∩W | O1∧O2) = 6,7×10⁻⁶ [5,6–7,8]** (P(G|C) = 200/136.181 C-emisyonu × P(W|G) = 4,55×10⁻³, 200 iskelet); 129'da 5,7×10⁻⁶ [5,3–6,0];
Mallows, 127: 1,0×10⁻⁵ [0,80–1,27] (P(G|C) = 100/45.167 × P(W|G) = 4,66×10⁻³, 100 iskelet); 129'da 8,0×10⁻⁶ [7,6–8,4]. Bu sayılar
veriye bakılarak tanımlanmış dar olayın eski olguları taşıyan kitaplar arasındaki sıklığıdır; arama bedeli içermezler ve kalibre sıra
(§5.1) ile karıştırılmamalıdırlar. (M20 aralıkları zincir içi kümelenmeyi hesaba katmaz.)
İkincil J10'da (veriye merkezli, 20.000 O-kitap): D 0,231 (4.619), E 0,157 (3.131), G 4,9×10⁻² (989), W 4,1×10⁻³ (81), A 2,0×10⁻⁴ (4),
G ∧ W doğrudan 4.

---

## 6. Ö2 — tam çekirdeğin sıralı sıfırlarda doğrudan ölçümü

| Sıfır, kodeks | P(G) (isabet / deneme) | iskelet | P(A\|G) | P(W\|G) | P(A∧W\|G) [%95] (isabet) | κ | **P(A∩G∩W)** [%95] |
|---|---|---|---|---|---|---|---|
| Tekdüze, 127 (Ç5) | 7,142×10⁻¹⁰ (GPU, 17.141 / 2,4×10¹³) | 114 | 3,43×10⁻⁴ | 3,62×10⁻³ | 1,23×10⁻⁶ [1,03–1,43] (2.796) | 0,99 | **8,8×10⁻¹⁶** [7,3–10,2] |
| **M20, 129** | 6,74×10⁻⁸ (1.490 / 2,21×10¹⁰) | 1.000 / 1.490 | 5,48×10⁻⁴ | 4,34×10⁻³ | 2,51×10⁻⁶ [2,37–2,65] (25.063) | 1,05 | **1,69×10⁻¹³** [1,57–1,82] |
| M20, 127 | 7,55×10⁻⁸ (200 / 2,65×10⁹) | 200 | 5,46×10⁻⁴ | 4,55×10⁻³ | 2,49×10⁻⁶ [2,20–2,77] (4.975) | 1,00 | 1,88×10⁻¹³ [1,56–2,23] |
| **Mallows, 129** | 7,73×10⁻⁸ (2.948 / 3,81×10¹⁰) | 1.000 / 2.948 | 5,18×10⁻⁴ | 4,33×10⁻³ | 2,31×10⁻⁶ [2,20–2,42] (23.095) | 1,03 | **1,79×10⁻¹³** [1,68–1,90] |
| Mallows, 127 | 9,17×10⁻⁸ (100 / 1,09×10⁹) | 100 | 5,19×10⁻⁴ | 4,66×10⁻³ | 2,33×10⁻⁶ [1,99–2,66] (2.331) | 0,97 | 2,14×10⁻¹³ [1,66–2,72] |

Okuma: kristal sıralı sıfırlarda tekdüzeye göre çok sıklaşır (127'de M20 ×106 [91–120], Mallows ×128 [103–154]; Ç2, Ç3); 127'de
kristal verildiğinde terazi M20'de ×1,6, Mallows'ta ×1,5, İki El ikisinde de ×1,3 sıklaşır; A ile W kristal verildiğinde neredeyse
bağımsızdır (κ ≈ 1). Çekirdek iki sıralı sıfırda yakındır (129'da M20 1,69×10⁻¹³, Mallows 1,79×10⁻¹³) ama iki ayrı ölçümdür. Tekdüze
129 için Ö2 ön-kayıt gereği koşulmadı; keşif olarak O-havuzunun 117 G-iskeletinde P(A∧W|G) = 1,36×10⁻⁶ [1,15–1,57] (3.189 isabet / 2,34×10⁹
adım; κ = 1,00) — 127 değeriyle uyumlu.

---

## 7. Üç soru, ayrıntılı

### (1) Beyan edilen tam olay ne kadar seyrek? — olay frekansı

Tekdüze sıfırda 127'de bu ölçüm 8,8×10⁻¹⁶ [7,3–10,2] verir; mevcut 1,1×10⁻¹⁵ [0,5–1,7] (`kesif18c`) ve 9,05×10⁻¹⁶ [5,1–13,0] (bağımsız)
ile uyumludur; 129'un mevcut değeri 8,29×10⁻¹⁶'dır. Sıralı sıfırlarda olay ilk kez aktarım yapılmadan ölçüldü: **M20'de 1,69×10⁻¹³
[1,57–1,82]** (129) ve 1,88×10⁻¹³ [1,56–2,23] (127); **Mallows'ta 1,79×10⁻¹³ [1,68–1,90]** (129) ve 2,14×10⁻¹³ [1,66–2,72] (127). Sıralı değerlerin tekdüzeden
büyüklüğü çoğunlukla kristalin şişmesinden gelir (127'de M20 ×106, Mallows ×128); kristal verildiğinde terazi ile İki El'in birlikte
görülmesi sıralı sıfırlarda ayrıca ~×2 daha sıktır (P(A∧W|G) tekdüzede 1,23×10⁻⁶, M20'de 2,49×10⁻⁶, Mallows'ta 2,33×10⁻⁶; 127). Bu sayılar sabit, veriye bakılarak tanımlanmış bir olayın frekansıdır: hiçbir
arama bedeli içermez ve "kanıt gücü" olarak okunamaz. Merdivendeki yerel-blok sıfırı değeri (1,1×10⁻¹², `denetim8`) gözlenen verinin bir
kısmını elde tutan başka bir sıfıra aittir; bu ölçümün değerleriyle bir aralık oluşturmaz.

### (2) Araştırma önceki literatüre ne ekledi? — koşullu kalibrasyon

**Ölçülen soru:** O'yu (1990 ve en geç 2015 yayımlanmış aile, toplam, 6/6, 722/722 ve 361/361 hücreleri) zaten taşıyan bir rastgele kitap,
aynı bataryadan O'nun gerektirmediği olgularla kendi en iyi hikâyesini kurduğunda, Kur'an'ınkine eşit ya da daha güçlü bir hikâye ne
sıklıkla çıkar? Cevap (§5.1): **tekdüzede 3,95×10⁻²** (k = 7.896 / 2×10⁵), **M20'de 0,164** (16.392 / 10⁵), **Mallows'ta 0,191**
(15.832 / 83.000); ikincil J10 (veriye merkezli) 0,263 (5.263 / 20.000). Tam birlikte-olasılıkla puanlama Kur'an'ı geriletir (tekdüze 6,2×10⁻², M20 0,28,
Mallows 0,28); teraziye de koşullamak sırayı değiştirmez (tekdüze 3,94×10⁻², M20 0,185, Mallows 0,206).

**Nasıl okunmalı.** Robotun bataryası sınıf olgularıyla puanlar ("katsayı bandı", "hücreler dengeli", "|U| = m"…); kristalin hücre
içerikleri ve kefe torbaları bataryada ayrı olgu değildir. Bu yüzden Ö1, araştırmanın eklediği yapının **bataryanın görebildiği kısmını**
kalibre eder: o kısım O-kitapları arasında tekdüzede ~25'te 1, M20'de ~6'da 1, Mallows'ta ~5'te 1 sıklıktadır. Daha dar, veriye
bakılarak tanımlanmış yeni olay (tam kristal ∧ İki El) O'ya koşullu olarak tekdüzede 2,3×10⁻⁶, M20'de 6,5×10⁻⁵, Mallows'ta 1,5×10⁻⁴
sıklıktadır (§5.5) — ama bu sayı, aynı bataryaya kalibre edilmiş bir sıra değil, Kur'an'a bakılarak seçilmiş tek bir olayın arama bedeli
ödenmemiş sıklığıdır. İkisi farklı sorulara cevap verir; aralarındaki farkın ne kadarının bataryanın kabalığından, ne kadarının dar
olayın sonradan seçilmesinden geldiği ölçülmedi.

**Silme ile koşullama.** README'nin silme basamağı (1990 ve 1994 olgularının puanı silinmiş, 2015'in iç terazisi hâlâ sayılı, robot puanı;
tekdüze sütunda 7,9×10⁻⁴ ve 3,4×10⁻³, sıralı sütunda 5×10⁻³ ve 4,7×10⁻², iki batarya) nesneyi daha sıra dışı gösteriyordu. Koşullu biçim
robotun kendi T2'siyle bile (tekdüze 6,6×10⁻², M20 0,21, Mallows 0,24) daha sıradandır. Fark hem silme yerine koşullamaktan hem de 2015 olgularının da
koşula alınmasından gelir; bu ikisi burada ayrıştırılmadı.

### (3) Seçim ve arama bedelleri ödenince ne kadar kanıt kalır?

| Bedel | Bu ölçümde |
|---|---|
| Batarya araması (her kitap 82 olgudan kendi en iyi 8 düğümlük hikâyesi) | **ödendi** (Ö1 sırası) |
| Cebirsel tekrarlar | **ayıklandı** (T2*, kanıtlı 29 kural); kısmi bağımlılıklar tam birlikte-olasılıkla (§5.3) |
| Merceğin (n ± a) seçimi | görev gereği devralınmış sayıldı — **ölçülmedi** |
| Modülün (19) seçimi | **ölçülmedi** |
| Bataryanın Kur'an görüldükten sonra kurulması | **ölçülmedi** |
| Dar olayın (tam kristal ∧ İki El) seçimi | **ölçülmedi** (§5.5 sayıları bu bedeli içermez) |
| 19 literatürünün araması (≥ 317–770 yayımlanmış iddia) | **ölçülmedi** |
| Sayım okulu | **ölçülmedi**; README'nin ×7'si varsayımsal bir Bonferroni üst sınırıdır, sıklık değildir |
| O'nun kapsamı (hangi olgular "önceki literatür") | KESIF not 20'nin tarihlemesi devralındı — **ölçülmedi** |
| Kültürel havuz | fiyatlanamaz — **ölçülmedi** |

Bu ölçümün verebildiği kalan kanıt, batarya-bedelli koşullu sıradır: **birincil sıralı sıfırda p = 0,164, tekdüzede 3,95×10⁻²,
Mallows'ta 0,191.** Ödenmeyen bedeller simetrik fiyatlansaydı rakip kitaplar ek serbestlikten kazanırdı; Kur'an ancak sabit seçimlerden
daha iyi bir seçenek bulursa kazanırdı. Bu iki etkinin koşullu sıradaki net büyüklüğü ölçülmedi.

---

## 8. Merdivene etkisi (ana hat için öneri)

- **"Olay frekansı, beyan edilen tam olay"** basamağının sıralı sütunu: "≈10⁻¹³ – 10⁻¹²" yerine doğrudan ölçüm — M20 1,7×10⁻¹³ [1,6–1,8]
  (129; 127'de 1,9×10⁻¹³), Mallows 1,8×10⁻¹³ [1,7–1,9] (129; 127'de 2,1×10⁻¹³). İki sıfır ayrı yazılmalı, aralık uçları gibi değil.
- **"Olay frekansı, önceki aile olguları verilince yeni yapı"** basamağının sıralı sütunu ("ölçülmedi"): M20 6,7×10⁻⁶ [5,6–7,8]
  (127; tekdüze çapa 1,4×10⁻⁷ ile aynı kodeks), Mallows 1,0×10⁻⁵ [0,8–1,3] (127) (keşif, §5.5). O3 de koşula alınınca (129): tekdüze 2,3×10⁻⁶,
  M20 6,5×10⁻⁵, Mallows 1,5×10⁻⁴.
- **Yeni basamak — "Koşullu kalibrasyon P(yeni | eski), mercek devralınmış, tekrarsız puan":** tekdüze 3,9×10⁻² (~1/25), sıralı M20 0,16
  (~1/6), Mallows 0,19 (~1/5); tam birlikte-olasılıkla tekdüze 6,2×10⁻², M20 0,28, Mallows 0,28. Bu, dokuzuncu incelemenin istediği biçimdir; silme
  basamağı yalnız duyarlılık olarak kalmalı.
- Robot puanına (T2) dayanan basamaklar cebirsel tekrarları çift sayıyordu. Ö0(a): özgünden tam tekrarsız puana rakip sayısı iddia bataryasında J10
  ×5,1, M20 ×5,0, tekdüze ×8,6; defter bataryasında J10 ×2,3, M20 ×2,5, tekdüze ×1,3 azalır. Tek tablolu koşulsuz sayımlar ayrıca tablodan
  tabloya ×3 oynar; tek bir tabloya dayanan basamaklar bu belirsizliği taşır.
- Ö1'in sayıları merceği sabit (devralınmış) sayar; mercek, modül ve literatür araması fiyatlanmadı.

---

## 9. Sınırlılıklar

- **Batarya kabadır.** Robot sınıf olgularıyla puanlar; 5–9 bandının kendisi, kefe torbaları ve hücre içerikleri (tam kristal) bataryada
  ayrı olgu değildir. Ö1 "robotun görebildiği" yeni yapıyı ölçer; dar olayın koşullu frekansı ayrıca verildi (§5.5), ama o sayı arama
  bedeli içermez.
- **M20 bir zincirdir.** Koşullu sıfır M20 akışının O'ya süzülmesidir; isabetlerin %14,7'si aynı zincirin ardışık emisyonundadır (85.335
  bağımsız küme / 10⁵). Sıra kestiricisi yansız, etkin örneklem biraz küçüktür; Poisson aralıkları kümelenmeyi hesaba katmaz.
- **Tam birlikte-olasılık çözünürlüğü sınırlıdır.** Tekdüzede rakip hikâyelerinin bir kısmı Kur'an'ınkiyle çözülemeyecek kadar az
  isabetlidir (kesin sınırlar p ∈ [1,9×10⁻²; 1,2×10⁻¹]); sonsal medyan Jeffreys önseline dayanır. M20'de çözünürlük yeterlidir. O'
  havuzlarında yalnız düzey 1 izinliydi; tekdüze O''de Kur'an'ın hikâyesi çözülemedi.
- **Mallows bütçe tavanında kaldı:** 83.000 O-kitap (hedef 10⁵; 16 çekirdek-saat tavanı; `ek/EK_20261023T070712Z_mallows_butce.md`). **J10** ikincildir: 20.000 kitap (hedef 2×10⁴; 5,33 çekirdek-saat, tavan 6); tam birlikte-olasılığı yalnız düzey 1'dir
  ve Kur'an'ın hikâyesi 4 isabetle zayıf çözülür.
- **Ö0(a)** inceleme defter bataryasıyla yeniden üretilemedi; iddia bataryasıyla (keşif) üretildi. Tek tablolu koşulsuz sayımlar büyük
  tablo belirsizliği taşır.
- **Açık gözlem:** GPU'nun D sayacıyla +%0,76 (3,3σ) fark (§3); sonuçları etkilemiyor, kaynağı bulunamadı.
- Mercek, modül, batarya tasarımı, dar olayın seçimi, literatür araması, sayım okulu ve kültürel havuz ölçülmedi.

## 10. Sapmalar ve ekler (`ek/`)

- `EK_20261023T025113Z_c4_eslesmis_z.md` — Ç4 z istatistiği eşleşmiş farka çevrildi (daha sıkı; sonuç değişmedi).
- `EK_20261023T034012Z_o0a_iddia_bataryasi.md` — Ö0(a) ön-kayıtlı ölçütle yeniden üretilemedi; iddia bataryasıyla keşif sınaması.
- `EK_20261023T041241Z_c4_tamsayi.md` — Ç4 aracında tamsayı sayım (özet satırı düzeltmesi).
- `EK_20261023T050132Z_U_Ousu_bant.md` — tekdüze O' düzey-1'de Kur'an çözülemedi; bant kuralı uygulanamadı.
- `EK_20261023T070712Z_mallows_butce.md` — Mallows havuzu 16 çekirdek-saat tavanında kaldı: 83 bölüt, N = 83.000 (hedef 10⁵); O' tohum
  kuralı notu.
- `EK_20261023T034012Z`'de M20'nin defter bataryası ortalaması (6.407) yazıldığı anda biten iki tekrarındır; üç tekrarın ortalaması 6.158'dir (§4.1).

Ön-kayıt dışı (keşif) analizler: Ö0(a)'nın iddia bataryası sınaması; tekdüze 129 için P(A∧W|G) ve P(G∧W|O) (§5.5, §6); C-koşullu sıralı
olay frekansları (§5.5). Bunlar birincil sayıların yerine geçmez.

## 11. Yeniden üretim

Bütün sayılar sabit tohumlarla yeniden üretilir (macOS, Apple clang; Python 3.11 + NumPy). Koşulan bütün adımlar, koşuldukları sırayla
ve aynı tohumlarla `kos/hepsi.sh`'tedir (`bash kos/hepsi.sh <adım>`; adımlar: derle, sinir, c1, o0b, o0a, havuz, u, m20, mallows, j10,
kesif). Havuzlar sürdürülebilir bölütlerdir (`kos/havuz.py`): tamamlanmış bölüt atlanır, yarıda kalan baştan koşulur; eşzamanlı bölüt
sayısı sonucu etkilemez. Sınama: iki bölüt (tekdüze 1291007, M20 2291013) aynı tohumla yeniden koşuldu, çıktılar bit-birebir aynı.

| Sonuç | Dosya |
|---|---|
| Ö1 sıra, dağılım, en yüksek 40 kitap, Kur'an'ın hikâyesi | `sonuc/o1/<etiket>.ozet.txt`, `.kuran.txt` (etiketler U129O, U127O, M129O, L129O, J129O, U129P, M129P, L129P); terazi açık `_teraziacik.*` |
| Tam birlikte-olasılık | `sonuc/o1/*.tam.txt`, `*_tam_analiz*.txt`, `*_tam_cozunurluk.txt` |
| Ö2 ve olay düzeyi | `sonuc/o2/*_cekirdek*.txt`, `sonuc/o2/*_olay_gw*.txt` |
| Ö0(a) | `sonuc/t9/` (`ozet_ana.txt`; her koşunun Kur'an satırı ve sayımları) |
| Ö0(b) | kurallar `src/kurallar.h`; adaylar `sonuc/cikarim_adaylar_*.txt`; bant kanıtı `kontrol/bant_kanit.py` |
| Çapalar | Ç1 `sonuc/u_capa127_*.txt`; Ç3 `tab/havuz/M127G/*.say`, `tab/havuz/M129O_gercek/*.say`; Ç4 `sonuc/c4_*.txt`; tekdüze sınır `sonuc/u_B_*_K12.txt`; port eşitliği `sonuc/dogrulama_bit_esitligi.txt` |
| Havuz sayaçları | `tab/havuz/*/*.say` (deneme/emisyon, B, C, G sayıları, süre), `tab/havuz/*/surucu.log` |
| Çalışma günlüğü | `sonuc/ara_notlar.md`, `sonuc/o1/kosu.log` |

Koşullu kitap havuzları (`tab/havuz/*/*.bin`) ve tarama tabloları (`tab/cikarim/`) pakete konmadı; her bölüt
`src/kosullu_uret <kodeks> <sıfır> <koşul> <tohum> <N> <çıktı>` ile yeniden üretilir (tekdüze, Mallows ve J10 bağımsız örnekler; M20
robotun zinciri). Paket: `paket/kosullu_paket.tar.gz` (≈ 55 MB; SHA-256 `paket/kosullu_paket.sha256`; içerik özetleri
`paket/MANIFEST.sha256`). Paket sınaması: boş bir dizine açılıp derlendi; tekdüze (1291007) ve Mallows (3291005) bölütlerinin ilk kayıtları havuzdakilerle
bit-birebir aynı çıktı; içerik özetleri MANIFEST ile tuttu.

---

## English summary

**Question.** For the Quran's surah-number / verse-count structure under the inherited lens (t = n + a, u = n − a, modulus 19), how
unusual is the structure the research added **among random books that already carry the prior literature** — O = the 12-member t ≡ 0
family with Σt = 1444 (1990), 6/6 and 722/722 (by 2015), the two odd-t cells 361/361 (by 2015); secondary O' = O ∧ the 1994 parity
balance? That is P(new | old), instead of deleting the old facts' scores. Pre-registered before any conditional score was computed
(`ONKAYIT.md`, frozen 2026-10-23T02:48:18Z, SHA-256 `4608859b…`); deviations are in time-stamped addenda (`ek/`).

**Method.** The ninth review's robot (robot4_m ledger battery, 82 active facts at the fixed lens; matrix and balance facts closed for
everyone; the facts implied by O closed for everyone; leave-one-out fact probabilities from the conditional pool), with one change: a
**duplicate-free score T2\*** that never credits a fact implied by other facts of the same story (27 proven cross-node implications + 2
under O, derived from the definitions and screened for counterexamples in ~4.1×10⁷ books; unproven candidates are listed, not used).
Conditional books: uniform — an *exact* sampler (family structure weighted by the rook-polynomial count of completions, rejection;
validated against the exact P(|F| = 12) and the GPU anchor P(G∩W | O1∧O2): 1.46×10⁻⁷ vs 1.39×10⁻⁷); M20 (primary sorted) — the robot's
own chain, emissions filtered on O; Mallows (q tuned only to the observed inversion count) — exact Lehmer sampling with rejection; J10
(secondary, data-centred). Rank p = (k+1)/(N+1). Exact joint probabilities of the stories of all books near or above the Quran were
counted directly in the conditional null.

**Results.**
1. *Declared full event* (balance ∧ full crystal ∧ Two Hands), event frequency at the fixed lens, no search cost: uniform 8.8×10⁻¹⁶
   [7.3–10.2] (127; consistent with 1.1×10⁻¹⁵ and 9.05×10⁻¹⁶); sorted nulls, measured directly for the first time (no transfer): **M20
   1.69×10⁻¹³ [1.57–1.82]** and **Mallows 1.79×10⁻¹³ [1.68–1.90]** (129; at 127: 1.88×10⁻¹³ and 2.14×10⁻¹³). This is the frequency of a post-hoc event, not a measure of evidence.
2. *What the research added beyond the prior literature* (calibrated; the battery search is paid): the Quran's duplicate-free score among
   O-books ranks at **p = 3.95×10⁻² (uniform, N = 2×10⁵), 0.164 (M20, N = 10⁵), 0.191 (Mallows, N = 83,000)**; J10 (secondary)
   0.263 (N = 20,000). With exact joint scoring: 6.2×10⁻², 0.28, 0.28. Conditioning also on the balance (O') changes nothing (3.94×10⁻², 0.185,
   0.206). Given the old structure, the research's own layers are common at class level (balanced even-t cells in 12% (uniform), 16%
   (M20) and 18% (Mallows) of O-books; a consecutive coefficient band in 11%, 28% and 30%); the Quran's strongest remaining piece is the
   Two Hands (the full event W in 0.4% of O-books in all three nulls). The narrow post-hoc event full crystal ∧ Two Hands has
   conditional frequency 2.3×10⁻⁶ (uniform), 6.5×10⁻⁵ (M20) and 1.5×10⁻⁴ (Mallows) given O — exploratory, no search cost, not a rank. The README's *deletion* rung had
   made the object look more unusual; the conditional form is more ordinary even with the robot's own score.
3. *After selection and search costs:* only the battery search is priced. The choice of lens and modulus, the post-hoc battery design, the
   choice of the narrow event, the literature's search (≥ 317–770 published claims), the counting school (the README's ×7 is a
   hypothetical Bonferroni upper bound, not a frequency) and the cultural pool were **not measured**; their net effect on the conditional
   rank is unknown. Under the primary sorted null the measured rank is already ordinary (p = 0.16; Mallows 0.19).

**Side results.** Ö0(a): with the pre-registered ledger battery the ninth review's counts were not reproduced; they are reproduced with
the 60-fact claim battery (J10 1,100 → 525, M20 1,021 → 505 vs 1,135 → 573 and 827 → 393; exploratory, addendum). Removing all exact
implications (T2*) cuts rival counts beyond the single fix (claim battery, from the original score: J10 ×5.1, M20 ×5.0, uniform ×8.6).
Single-table unconditional counts carry a large table uncertainty (×3 across table seeds). An unexplained +0.76% (3.3σ) excess of
P(D | C) relative to the GPU counters is reported as an open observation; it affects no result. Mallows reached N = 83,000 of its 10⁵ target within the pre-registered 16 core-hour budget (addendum); J10 reached its
2×10⁴ target.
