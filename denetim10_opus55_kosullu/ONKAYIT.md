# ÖN-KAYIT — koşullu ölçüm: önceki literatürü taşıyan kitaplar arasında yeni yapı ne kadar sıra dışı?

**Tarih:** 23 Ekim 2026 · **Yazan:** Claude (Opus 5.5), `GOREV.md` gereği · **Klasör:** `~/opus5.5/kosullu` (depo `~/Desktop/hrf2` yalnız okundu)
**Statü:** Bu metin, **hiçbir rastgele kitabın koşullu puanı hesaplanmadan ve görülmeden** yazıldı ve donduruldu. SHA-256 özeti ve UTC
zamanı `sonuc/onkayit_ozet.txt`'tedir; dosya salt okunurdur. Dondurmadan önce yapılan her şey §9'da açıkça listelidir (örnekleyici ve kod
doğrulamaları, olgu birlikte-görülme taraması; hiçbiri koşullu puan ya da Kur'an'ın koşullu sırası içermez).
**Kural:** ölçmek için hatalı davranmamak; sonuç hangi yöne çıkarsa çıksın aynen yazılır. Sapmalar (doğrulama başarısızlığı, bütçe kısıtı,
hata düzeltmesi) `ek/` altında ayrı, zaman damgalı dosyalardadır; bu metin sonradan değiştirilmez.

---

## 1. Girdiler, kod ve kodeks

- Veri: `quran_meta.json` (depo; SHA-256 `sonuc/sha256_girdiler.txt`). **Birincil sayım Tevbe = 129 (Σa = 6236).** GPU çapaları 127 ile
  koşulduğu için çapa karşılaştırmaları 127 ile de koşulur (§4.7, §5).
- Robot: `denetim6_opus55_guclendirme/src/robot4_m.c` (kopyası `ref/robot4_m.c`, değiştirilmedi; SHA-256 kayıtlı).
- Kendi kodum (`src/`): `ortak.h` robotun sabit mercek (1,1,19) olgularının (`facts()`, aynı olgu sırası, aynı bitler) ve U/J10/M20
  örnekleyicilerinin **bit-birebir portudur**: 129 ve 127 kodekslerinde, U/J10/M20'de 6 × 100.001 kitapta örnekleyici farkı 0, olgu biti
  farkı 0 (`sonuc/dogrulama_bit_esitligi.txt`). Puanlayıcım robotun sabit-mercek T2'sini aynı kitaplarda float hassasiyetinde (en büyük
  fark 4,8×10⁻⁷) yeniden üretir (§9).
- Dondurma anında `src/`, `kontrol/`, `kos/` dosyalarının SHA-256 listesi `sonuc/onkayit_kod_sha256.txt`'e yazılır.

## 2. Tanımlar

**Mercek** sabittir: t = n + a, u = n − a, modül 19 (`GOREV.md`: devralınmış sayılır; bedeli bu ölçümün konusu değildir).
F = {n : 19 | t}, G = {n : 19 | u} (kod adlarıyla F ve G; kesif4'ün "aile" ve "fark eli"), U = F ∪ G, k_F = |F|, k_G = |G|.

**Olaylar** (`kesif4_motor.c`/`kesif18c` tanımları; Σa kodeksten):
- **O1** = B: k_F = 12 ∧ Σt(F) = 1444 (1990). **O2**: O1 ∧ çift-t yarısı 6 üye ∧ Σt(çift-t) = 722 (⇒ 722/722) (en geç 2015).
- **O3**: O2 ∧ tek-t kolunun iki hücresi (sûre no paritesine göre) Σt = 361 ve 361 (en geç 2015).
- **O4** = A (terazi): |{n : t çift}| = 57 ∧ Σt(çift) = Σa (1994).
- **Birincil koşul O = O1 ∧ O2 ∧ O3; ikincil O' = O ∧ O4.** C = O1 ∧ O2 (çapa koşulu).
- D, E, G (tam kristal), W (İki El) `kesif4_motor.c`'deki gibi; G ⊂ O.

**Batarya:** robot4_m'nin defter bataryası (99 olgu, 23 düğüm), sabit mercekte. Kapalı (herkes için): matris (MX ×3), "G: Σw=m·kG",
"U: Σv(U) kare/asal-kuvvet" (bütün önceki defter koşularındaki `ROBOT_DISABLE`), ve **terazi olguları ("kitap:" ×12)**. Gerekçe:
(i) terazi O4'tür (önceki literatür) ve merceğin kaynağıdır; README merdiveni not 19'dan beri teraziyi saymaz; (ii) dokuzuncu incelemenin
tekdüzede 2×10⁶'da 116 sayısı yalnız terazi sayılmadığında anlamlıdır (terazi sayılınca aynı batarya sabit mercekte 2×10⁷'de 6 kitap verir,
`denetim6` A1c); (iii) birincil ile ikincil koşul arasındaki fark böylece yalnız **koşullamadır** (O' teraziyi koşul olarak taşır).
Ö0(a) iki biçimi de (terazi açık/kapalı) yeniden üretir. Terazi açık sürüm Ö1'de yalnız **duyarlılık (keşif)** olarak raporlanır (§4.8).
Etkin olgu sayısı: 99 − 5 − 12 = **82**.

**Olgu olasılıkları** robotunkiyle aynı: p = (isabet + 0,5)/(deneme + 1), olgunun bağlamında (bağlamsız; k_F'ye; k_G'ye koşullu; 41 kova),
bağlam denemesi < 1000 ise havuzlanmış olasılık (`ROBOT_MINTR=1000`). Düğüm değeri = düğümdeki en nadir doğru olgunun −log10 p'si;
**T2 = en büyük 8 düğüm değerinin toplamı**; F ve G küme olarak aynıysa 6–11 ve 14. düğümler sıfırlanır (robotun kuralı).

---

## 3. Ö0 — tekrarların temizlenmesi

### 3.1 Ö0(a): dokuzuncu incelemenin deneyinin yeniden üretimi

- Robotun kendi ikilisiyle (`ref/robot4_m`, `ROBOT_MODS=19`, `ROBOT_MINTR=1000`) her (sıfır ∈ {U, J10, M20}) × (kodeks ∈ {129, 127}) ×
  (tekrar r ∈ {1, 2, 3}) için **2×10⁶'lık tablo** (tohum 9100·r + {1: U, 2: J10, 3: M20} + kodeks farkı: 129 → +0, 127 → +50) ve **ayrı
  tohumla 2×10⁶ değerlendirme kitabı** (tablo tohumu + 1000), 8 iş parçacığı düzeniyle (robotun tohum şeması). Puanlar: sabit mercek T2.
- Benim tarafım (`src/tekrar9`) aynı değerlendirme kitaplarını bit-birebir yeniden kurar, robotun T2'sini doğrular (fark ≤ 10⁻³ zorunlu),
  ve aynı kitaplarda üç sayım verir: (1) **özgün T2**, (2) **dokuzuncu incelemenin tek düzeltmesi** (G-çapraz düğümünde kazanan olgu
  "G: Σn=Σa" ise G-toplam düğümünde "G: m²|Σq" puan almaz; düğüm o olgu olmadan yeniden hesaplanır), (3) **Ö0(c)'nin tekrarsız T2*'ı**
  (kural listesi L0). Sayım: k = #{T2_i ≥ T2_Kur'an − 10⁻⁶}, p = (k+1)/(N+1).
- İki batarya biçimi: terazi kapalı (birincil yeniden üretim) ve terazi açık.
- **Yeniden üretim ölçütü:** 129 ve terazi kapalı biçimde, üç tekrarın her sıfırdaki k_özgün ortalaması incelemenin sayısının (U 116,
  J10 1.135, M20 827) ±%30'u içinde ve k_özgün/k_düzeltme oranı [1,6; 2,6] aralığında (inceleme 1,98–2,19) ise "yeniden üretildi";
  değilse "yeniden üretilemedi" yazılır, iki biçimin hangisinin incelemeye yakın olduğu raporlanır (Ö1 bataryası buna göre değişmez).

### 3.2 Ö0(b): kesin gerektirmeler (sabit mercek (1,1,19))

**Yöntem (sistematik).** (i) Her olgunun tanımı robot kaynağından okunup cebirsel ilişkiler türetildi; (ii) tarama: U (10⁷), M20 (10⁷),
J10 (2×10⁶), Mallows (2×10⁶), kesin tekdüze O-koşullu (10⁵), aile boyları zorlanmış "çeşitli" kitaplar X (10⁷) — 129; ayrıca 127'de U
(5×10⁶) ve O-koşullu (5×10⁴); M20 O-kitapları (5.000) ve Mallows O-kitapları (1.500) — kitaplarında bütün olgu çiftlerinin ve üçlülerinin
birlikte-görülme sayıları; **aday** = örneklemde karşı-örneği olmayan çift X→Y ya da üçlü {X1,X2}→Y. (iii) Her aday ya kanıtlandı ya
düşürüldü: **listeye yalnız tanımdan ya da her kitapta geçerli bir kanıtla gösterilenler girer.** Kanıtlanamayan adaylar (örneklem
yanılsaması olabilir; ör. G'nin u-yarılarının ikisi de 0 olduğu sık durumdan doğan "→ G: Σn=Σa" üçlüleri) **kullanılmaz** ve
`sonuc/cikarim_adaylar_*.txt` dosyalarında desteğiyle listelenir. Aday üretimi: `src/cikarim.c`, `src/cikarim_analiz.py`,
`src/cikarim_suz.py`, `src/cikarim_suz_O.py`.

**Yardımcı önermeler.** L1: n ∈ F ∩ G ⟺ 19 | n ∧ 19 | a (t + u = 2n, t − u = 2a, 19 tek). L2: x = Σn(F) mod 19, y = Σn(G) mod 19 ise
Σn(U) ≡ x + y, Σa(U) ≡ y − x, Σt(U) ≡ 2y, Σu(U) ≡ 2x (mod 19) (F'de a ≡ −n, G'de a ≡ n; F ∩ G üyeleri 0 katkı verir). L3: Σt(F) ve
Σu(G) her zaman 19'a bölünür; 19'a bölünen bir tam kare 361'e bölünür. L4: F'de t çift ⟺ c = t/19 çift; G'de u çift ⟺ c = u/19 çift.
L5: kitabın Σt'si 6555 + Σa = 12791 (129) / 12789 (127): tek; Σt ≡ 4 / 2 (mod 19). Çoklukta 54 tek değer var; |E| = 2x + 3 (x = tek
konumdaki tek değer sayısı), |E| = 57 ⟺ x = 27.

**Düğüm-arası kesin gerektirmeler L0 (puanlamayı etkileyen; `src/kurallar.h`):**

| Kural | Öncül → sonuç | Kanıt |
|---|---|---|
| R1 | G: Σn=Σa → G: m²\|Σq | Σu(G) = 0 |
| R2, R3 | F: v-yarılar m²\| ; F: n-yarılar m²\| → F: m²\|Σq | iki yarı 361'e bölünür ⇒ toplamı |
| R4, R5 | G: v-yarılar m²\| ; G: n-yarılar m²\| → G: m²\|Σq | aynı |
| R6 | F: m\|Σn → U: m\|Σw(U) | L2: Σu(U) ≡ 2x |
| R7 | F: Σn=Σa → U: m\|Σw(U) | Σt(F) = 2Σn(F), L3 ⇒ x ≡ 0 |
| R8 | G: m\|Σn → U: m\|Σv(U) | L2: Σt(U) ≡ 2y |
| R9–R12 | {F: m\|Σn ya da F: Σn=Σa} ∧ G: m\|Σn → U: m\|Σn(U) ; → U: m\|Σa(U) | x ≡ y ≡ 0, L2 |
| R13–R16 | R: kF=kG ∧ F: k≥2E → G: k≥2E (ve simetriği; k=m için de) | tanım |
| R17 | F: k=m ∧ G: k=m → R: kF=kG | tanım |
| R18 | F: v-yarılar eşit ∧ RP: F'de aynı v ≥5 kez → F: k≥2E | eşit yarılar: tek-t yarısının toplamı k/2 tek sayının toplamı, çift yarınınki çift ⇒ 4 \| k; beş eş t aynı paritededir ⇒ yarı ≥ 5 ⇒ k/2 ≥ 6 |
| R19 | F: hücreler dengeli ∧ RP: F'de aynı v ≥5 → F: k≥2E | "hücreler dengeli" tanımı "v-yarılar eşit"i içerir ⇒ R18 |
| R20, R21 | F: k=m ∧ U: \|U\|=2m → G: k≥2E (ve simetriği) | k_G = 38 − 19 + \|F∩G\| ≥ 19 |
| R22–R25 | G: k=m ∧ U: \|U\|=m → U: m\|Σw(U) ve F: m\|Σn (ve F/G simetriği) | \|U\| = k_G ⇒ F ⊆ G ⇒ F üyelerinde 19 \| n (L1) ⇒ x ≡ 0 |
| R26, R27 | v-yarılar Δ∈{m²,2m²} ∧ katsayı bandı → k≥2E (F ve G) | Δ katsayı biriminde 19 ya da 38; bant + eşit-boy parite yarılarıyla ulaşılabilecek en büyük \|Σçift − Σtek\| çift k = 2, 4, 6, 8, 10 için 1, 2, 5, 8, 13 (tam tarama, `kontrol/bant_kanit.py`) < 19 ⇒ k ≥ 12 |

Eşdeğerlikler (aynı kurallarla örtülür): k_F ≥ 1 iken U: m|Σw(U) ⟺ F: m|Σn; k_G ≥ 1 iken U: m|Σv(U) ⟺ G: m|Σn; U-bölünebilirlik düğümünün
dört olgusundan herhangi ikisi öbür ikisini gerektirir (aynı düğüm).

**Düğüm-içi kesin gerektirmeler** (robotun "düğümde en nadir olgu" kuralı bunları zaten tekrarsız sayar; listede tamlık için):
K2 → K1 (F, G); Σq kare → m²|Σq (F, G; L3); F: Σn=Σa → F: m|Σn; hücreler dengeli → v-yarılar eşit (F, G); kitap: & (her eşitlik) →
kitap: |E|=|O|; vE=Σa ⟺ vO=Σn ⟺ nE=aO; MD: U∩Medenî=∅ → MD: F∩Medenî=∅ (k_F ≥ 4) ve G'de (k_G ≥ 4); MQ için aynısı; {v-yarılar eşit
ya da Δ∈{m²,2m²}} ∧ m²|Σq → v-yarılar m²| (sonuç öncülün düğümünde); vb.
**Hiçbir kitapta doğru olamayan olgular** (iki kodekste): kitap: vE=Σn (vE çift, 6555 tek), vO=Σa (vO 57 tek sayının toplamı), vE=vO
(Σt tek), nE=nO (6555 tek), nO=aO (nO çift, aO tek; x = 27), nO=aE (⟺ vE=Σn); kitap: aE=aO 129'da (aE tek, 3118 çift; 127'de olası);
BL: Σv(1..57)=Σv(58..114) (Σt tek); BL: üç 38'lik blok m|Σv (19 ∤ Σt); W: m-bölünen pencere az (Σ C(c_r,2) ≥ 291 > 207).

**O altında ek kesin sonuçlar L_O** (O: k_F = 12, Σt(F) = 1444, 6/6, 722/722, tek-t hücreleri 361/361):
- **O'nun gerektirdiği (herkes için kapalı):** F: k≥2E, F: m²|Σq (361 | 1444), F: Σq kare (1444 = 38²), F: v-yarılar eşit,
  F: v-yarılar m²| (722 = 2·361, iki yarı da dolu). (Her O-kitabında doğruluğu puanlayıcıda ayrıca denetlenir.)
- **O altında imkânsız:** F: k=m, F: 114|Σq (114 ∤ 1444), F: v-yarılar Δ∈{m²,2m²} (Δ = 0), F: katsayı adım-2 (6 çift, 6 tek katsayı),
  F: n-yarılar eşit ve F: n-yarılar Δ∈{m²,2m²} (çift-t hücrelerinin toplamı 19 × (çift sayıların toplamı); n-yarıları = hücre_çift + 361;
  "eşit" iki çift-t hücresinin 361 olmasını ister — 361 = 19·19 tek kat, imkânsız; Δ = 361 tek kat, imkânsız; Δ = 722 bir çift-t hücresini
  boşaltır, o zaman o n-yarısı 6 + (tek sayıda) üye olur ve yarılar eşit boy olamaz).
- **RO1:** R: kF=kG → G: k≥2E (k_F = 12 ⇒ k_G = 12). **RO2:** U: |U|=2m → G: k≥2E (k_G = 26 + |F∩G|).
- O' altında ek kural yok (terazi olguları zaten kapalı).

### 3.3 Ö0(c): tekrarsız puan T2*

Bir kitabın **hikâyesi** S: doğru ve kapalı olmayan olgulardan, düğüm başına en çok bir olgu içeren küme. S **gereksiz**dir eğer etkin
listede öncülü P ve sonucu c olan bir kural için P ∪ {c} ⊆ S ise (gerektirilen olgu, gerektirenin bulunduğu hikâyede ek puan almaz;
karşılıklı gerektirmelerde ikisinden yalnız biri sayılır). **T2\* = gereksiz olmayan hikâyeler üzerinde en büyük 8 olgu değerinin
toplamının en büyüğü.** Hiçbir kural uygulanmıyorsa T2* = robotun T2'si (robotun T2'si bütün hikâyeler üzerindeki en büyük değerdir).
Hesap kesin numaralandırmayla yapılır (`src/puan.h`): kurallara bağlı düğümlerde her seçenek (bağsız en iyi olgu ya da hiç / her bağlı
olgu) denenir. **Etkin liste:** koşulsuz kullanımda L0; O ve O' altında L0 ∪ L_O. Kitabın **T2\*-hikâyesi** (en iyi 8 olgu) tam
birlikte-olasılık hesabında kullanılır (§4.6). Kısmi örtüşmeler (kesin gerektirme olmayan bağımlılıklar) T2*'da kalır; onları tam
birlikte-olasılık düzeltir.

---

## 4. Ö1 — koşullu kalibrasyon (asıl ölçüm)

### 4.1 Soru ve biçim
Eski olguları (O; ikincilde O') zaten taşıyan kitaplar arasında, O'nun gerektirmediği olgulardan kurulan en iyi hikâye (aynı batarya,
matris ve terazi kapalı, O'nun gerektirdiği olgular kapalı) Kur'an'da ne kadar sıra dışı: **P(T2\* ≥ T2\*_Kur'an | O, sıfır)**.

### 4.2 Sıfırlar ve koşullu örnekleyiciler
- **U (tekdüze), kesin:** `src/kosul_u.h`. Etiketli permütasyon = (yapı γ: aile konumları + değerleri) × (tamamlama τ). P(γ) ∝ Φ(k(γ)),
  Φ = yeni aile üyesi oluşturmayan tamamlama sayısı, yalnız sınıf-sayısı vektörü k'ye bağlıdır (19 ayrık blokluk yasak tahtanın kale
  polinomu). Öneri k ~ Π w_r(k_r) (Σk = 12, w_r(k) = C(6,k)·C(c_r,k)·k!), sınıf içinde düzgün; γ'de koşul; Φ(k)/(M!·B) olasılıkla kabul
  (B = max Φ/M!, Σk = 12 bütün 8,3×10⁷ vektörün tam taramasıyla: 129: 0,00457773644911964; 127: 0,00458061425217835; çalışma sırasında
  Φ/M! ≤ B ihlali sayılır, 0 olmalı); τ geçerli tamamlamalar arasından düzgün (ret). Çıktılar bağımsız ve tam dağılımlıdır.
- **M20 (birincil sıralı):** robotun M sıfırı birebir (64 zincir, tam azalan diziden 30.000 adım yakma, 20 önerilik emisyon,
  S = Σ n·a ≤ S_obs kısıtlı takaslar; 129'da S_obs = 209029). Koşullu sıfır = **O'yu sağlayan emisyonlar** (M20 akışının O'ya süzülmesi;
  zincir O kümesinde dolaştırılmaz, donma sorunu yoktur). Her bölüt taze zincirlerle başlar; zincir no ve emisyon sırası kaydedilir,
  aynı zincirden ardışık isabetler (küme) raporlanır.
- **Mallows:** P(diziliş) ∝ q^I, I = büyükten küçüğe sıraya aykırı kesin çift sayısı (eşitler terslik değil); **q yalnız E[I] = I_gözlenen
  olacak biçimde** ayarlanır (129: I = 993, q = 0,9145731284624; 127: I = 994, q = 0,9146671727169 — sekizinci denetimin q'suyla 13
  haneye kadar aynı). Örnekleme kesin: etiketli Lehmer kodu (V_p bağımsız kesik geometrik; alias eşiği 32-bit nicemli), eşit değerlerin
  iç terslikleri her dizilişe aynı çarpanı verdiğinden izdüşüm tam ∝ q^I'dır. **Koşullu kitaplar ret örneklemesiyle** (kesin).
- **J10 (yalnız ikincil, "veriye merkezli"):** robotun J10'unun dağılımca aynısı (gerçek dizilişin σ = 10 konum titreşimi; anahtar =
  konum + 10·Z; Box–Muller'in iki çıktısı da kullanılır, sıralama kova yöntemiyle — ekleme sıralamasıyla aynı sırayı verdiği 2×10⁶
  anahtar kümesinde sınandı; robot J10'uyla P(B), P(C), ortalama terslik uyumlu, `kontrol/jz_sina.c`); ret örneklemesi.

### 4.3 Örneklem hedefleri, tohumlar, bütçe
Bölütler bağımsız tohumludur, tamamlanınca atomik yazılır (yarıda kalan bölüt baştan koşulur); koşu betikleri `kos/`.

| Havuz | Hedef N | Bölüt × kitap | Tohumlar | Bütçe tavanı |
|---|---|---|---|---|
| U, 129, O | 2×10⁵ | 20 × 10.000 | 1291001–1291020 | 4 çekirdek-saat |
| U, 127, O | 2×10⁵ | 20 × 10.000 | 1271001–1271020 | 4 çekirdek-saat |
| M20, 129, O | 10⁵ | 50 × 2.000 | 2291001–2291050 | 8 çekirdek-saat |
| Mallows, 129, O | 10⁵ | 100 × 1.000 | 3291001–3291100 | 16 çekirdek-saat |
| J10, 129, O (ikincil) | 2×10⁴ | 20 × 1.000 | 4291001–4291020 | 6 çekirdek-saat |
| M20, 127, G (çapa) | 200 G-kitabı | 4 × 50 | 2271001–2271004 | 4 çekirdek-saat |
| Mallows, 127, G (çapa) | 100 G-kitabı | 4 × 25 | 3271001–3271004 | 6 çekirdek-saat |

Tavana ulaşılırsa yeni bölüt başlatılmaz; tamamlanmış bölütlerle ulaşılan N yazılır (hedef ≥ 10⁵: ulaşılamazsa ulaşılan sayı yazılır).
Puanlama yalnız tamamlanmış bölütlerle ve bütün bölütler bittiğinde (ya da tavanda) bir kez yapılır; ara puanlama yoktur. Paralellik
(eşzamanlı bölüt sayısı) makine yüküne göre değiştirilebilir; sonucu etkilemez (bölütler sabit tohumlu ve bağımsız). Ö0(b) taraması için
üretilen O-kitapları (M20 tohum 9101, Mallows 9201) havuzlara katılmaz.

Diğer tohumlar: Ç3 başlangıç denetimi M20 129 O, `M20_BASLANGIC=gercek`, 5.000 O-kitap (≈1,1×10⁹ emisyon), tohum 2299001. Ç4: U 200 iskelet
(U 129 havuzunun ilk bölütü) × R = 2.000, K = 20, tohum 5291001; M20 ve Mallows havuzlarının ilk 5.000 iskeleti × R = 200, K = 20,
tohumlar 5292001, 5293001. Tam birlikte-olasılık: 6291001 (U), 6292001 (M20), 6293001 (Mallows), 6294001 (J10), O'-havuzları 6295001–6295003.
Ö2: 7271001 (U 127), 7292001 (M20 129), 7272001 (M20 127), 7293001 (Mallows 129), 7273001 (Mallows 127).

### 4.4 Tablo ve kapalılar
Olgu olasılıkları **aynı koşullu havuzdan** tahmin edilir (koşullu sıfır). Bağımsızlık için **biri-dışarıda (LOO)**: i. kitap, kendi
katkısı çıkarılmış tabloyla (N − 1 kitap) puanlanır; Kur'an tam tabloyla (N kitap). Bağlamlar ve MINTR robotla aynı. Kapalı olgular §2
ve §3.2 (O'nun gerektirdikleri).

### 4.5 Sıra istatistiği
**Birincil:** k = #{i : T2\*_i ≥ T2\*_Kur'an − 10⁻⁶}, **p = (k + 1)/(N + 1)**, k/N için %95 Clopper–Pearson aralığı. Robotun T2'siyle aynı
sayım karşılaştırma olarak verilir. Her sıfır ayrı raporlanır; sıfırlar birleştirilmez, aralık uçları gibi sunulmaz. Birincil sıralı
okuma M20'dir; U ve Mallows yanında verilir; J10 ikincildir.

### 4.6 Tam birlikte-olasılık (Kur'an'ı geçen ya da ona yakın kitaplar)
- Değerlendirilen kitaplar: T2\*_i ≥ T2\*_Kur'an − 2 olan bütün kitaplar ve [T2\*_Kur'an − 3, T2\*_Kur'an − 2) bandından 500 rastgele
  kitap (denetim bandı); her birinin ve Kur'an'ın **T2\*-hikâyesi** S için S_tam = −log10 P(S'nin bütün olguları | koşul, sıfır).
- Kestirim (`src/tam_ortak.c`), doğrudan sayım, bağımsızlık varsayımı yok: düzey 1 — havuzda (LOO) S'yi taşıyan kitap sayısı h1 ≥ 30
  ise P = h1/(N−1); değilse düzey 2 — S = S_F (yalnız aileye bağlı olgular: düğüm 1–5, RP: F'de…, RP: F'nin tepe değeri, MD/MQ'nun F
  olguları) ∪ S_R; P = (n_F/N)·ort_s[h_s/R], n_F = S_F'yi taşıyan iskelet sayısı, s bunlardan en çok 2.000'lik düzgün örnek, her
  iskelet için R = 50 tamamlama (iskelet sabit tamamlama zinciri `src/tamamla.h`, kitabın kendi tamamlamasından başlar, kayıtlar 20
  adım arayla; U'da tekdüze, M20'de S ≤ S_obs kısıtlı, Mallows'ta q^ΔI kabullü; yeni aile üyesi yasak).
- **Tam sıra:** k_tam = #{değerlendirilen i : S_tam,i ≥ S_tam,Kur'an}; belirsizlik her hikâyenin isabet sayısının Jeffreys sonsalından
  4.000 çekilişle. Denetim bandında Kur'an'ı tam puanla geçen kitap çıkarsa bütün bant değerlendirilir ve raporlanır; bandın altındaki
  kitapların geçmediği varsayımı bant sonucuyla birlikte yazılır.
- J10'da tamamlama dağılımının açık biçimi olmadığından yalnız düzey 1 uygulanır.

### 4.7 Çapalar ve doğrulamalar (ölçütler önceden)
- **Ç1 (U, 127):** P(G∩W | C) = P(G | C)·P(W | G) GPU'nun 62/444.513.933 = 1,39478×10⁻⁷'sinin %95 aralığıyla ([1,07; 1,79]×10⁻⁷)
  uyumlu olmalı. *Dondurmadan önce koşuldu (örnekleyici doğrulaması; puan yok) — sonucu §9'da; son değeri raporda.*
- **Ç2 (Mallows, 127):** P(G | Mallows) sekizinci denetimin 18/2×10⁸'iyle ([5,3; 14,2]×10⁻⁸) %95 aralıkları örtüşmeli; şişme P(G|Mallows)/
  P(G|U) (P(G|U) = 17.141/2,4×10¹³, GPU) ×126 [75–199] ile örtüşmeli.
- **Ç3 (M20, 127):** P(B | M20) ve P(C | M20) `kesif25`'in bağımsız MCMC'siyle (5,49×10⁻⁴ [21.957 isabet], 5,16×10⁻⁵ [2.065]) 3σ içinde;
  kristal şişmesi P(G|M20)/P(G|U) %95 aralığı ×88–160 ile örtüşmeli. Başlangıç denetimi (129): zincirler gerçek dizilişten (S = S_obs)
  30.000 adım yakılarak başlatıldığında P(B), P(C), P(O) azalan-başlangıçlı değerlerle 3σ içinde (en az 10⁹ emisyon, `M20_BASLANGIC=gercek`).
- **Ç4 (tamamlama zincirleri):** (a) U: 200 O-iskeletinde zincir (K = 20) ile kesin tamamlamanın P(A|iskelet), P(W|iskelet), P(U: |U|=m),
  P(U: m|Σa(U)), P(MQ: MQ v-yarılara 14/15) toplu frekansları 3σ içinde; (b) M20 ve Mallows: aynı olguların zincir-ortalamalı frekansları
  (havuzun bütün iskeletleri) havuzun doğrudan frekanslarıyla 3σ içinde. Bir sıfırda tutmazsa o sıfırda düzey-2 tam birlikte-olasılık,
  O' ve Ö2 "doğrulanmadı" diye işaretlenir ve ölçüm sayılmaz.
- **Ç5 (Ö2, U, 127):** P(A∧W | G) `kesif18c`'nin 1,51×10⁻⁶ [0,69–2,45]'iyle, çekirdek 7,142×10⁻¹⁰ × P(A∧W|G) ise 1,1×10⁻¹⁵ [0,5–1,7] ve
  bağımsız 9,05×10⁻¹⁶ [5,1–13,0] ile %95 aralığı örtüşmeli.
- Tutmayan her çapa için sebep aranır ve ayrı ek dosyada yazılır.

### 4.8 İkincil ve duyarlılık analizleri
- **O' (ikincil koşul):** O-havuzundaki her iskelet sabit tutulur, tamamlaması koşullu dağılımından yeniden çekilir, terazi tutanlar
  kaydedilir (`src/o_ussu.c`); iskelet başına sabit emek (U: 3.000 kesin tamamlama; M20, Mallows: 3.000 kayıt × 20 zincir adımı) ⇒ iskeletin
  beklenen katkısı P(A | iskelet) ile orantılı ⇒ çıktı akışı P(· | O') dağılımındadır. Kaynak: U 129 havuzunun ilk 10 bölütü (10⁵ iskelet),
  M20 ve Mallows havuzlarının hepsi; tohumlar 1292001+b, 2292001+b, 3292001+b. O'-havuzları §4.4–4.5 ile puanlanır (LOO); tam
  birlikte-olasılık O'-havuzlarında **yalnız düzey 1** ile (düzey-2 tamamlamaları teraziye koşullu olmadığından). J10'da yok.
- **Kodeks denetimi:** U 127 havuzu da aynı biçimde puanlanır (yalnız rapor).
- **Terazi açık (keşif, duyarlılık):** O altında terazi olguları açık bırakılarak aynı havuzlar yeniden puanlanır (görevin "O'nun
  gerektirmediği her olgu açık" okuması); birincil değildir.

---

## 5. Ö2 — tam çekirdeğin sıralı sıfırlarda doğrudan ölçümü
- Olay A ∩ G ∩ W (terazi ∧ tam kristal ∧ İki El). **P(A∩G∩W | sıfır) = P(G | sıfır) × ort_iskelet P(A∧W | G-iskeleti)**; iki çarpan da o
  sıfırda ölçülür (aktarım yok).
- P(G | sıfır): M20 ve Mallows 129 O-havuzlarından P(O | sıfır)·P(G | O, sıfır) (deneme/emisyon sayıları `.say` dosyalarında); 127 için
  çapa G-koşuları (§4.3), iskeletleri de onlardan. Tekdüzede P(G) GPU'nun ölçülmüş değeridir (127: 17.141/2,4×10¹³); tekdüze 127 koşusu
  yalnız Ç5 çapasıdır, tekdüze 129 için Ö2 koşulmaz (mevcut 129 değeri: sekizinci denetim 8,29×10⁻¹⁶).
- P(A∧W | iskelet): her G-iskeleti (havuzlardan; G ⊂ O, o sıfırın P(iskelet | G) çekilişleri) için iskelet sabit tamamlama zinciri
  (`src/cekirdek.c`), her adımda sayım; iskelet başına L = 10⁷ adım (U 127'de 2×10⁷), en çok 1.000 iskelet (fazlası düzgün örneklenir).
- Belirsizlik: iskeletler üzerinde bootstrap (4.000) × P(G)'nin Poisson aralığı. U 127 sonucu Ç5 çapasıdır; M20 ve Mallows 129 birincil,
  127 karşılaştırma.

## 6. Önceden tahmin
Sayısal tahmin yazmıyorum: koşullu sıranın büyüklüğü hakkında savunulabilir bir önsel bilgim yok ve bir tahmin yazmak yorumu
etkileyebilir. Yalnız nitel beklenti: tekrar temizliği rakipleri kırpar (Kur'an'ın O altındaki hikâyesinde hiçbir kural uygulanmaz),
koşullama ise O'nun gerektirdiği olguları herkesten aldığı için Kur'an'ın puanını düşürür; net yön bilinmiyor.

## 7. Rapor kuralları (`KOSULLU_RAPORU.md`, Türkçe; sonunda İngilizce özet)
1. Üç soru ayrı cevaplanır: (1) beyan edilen tam olay ne kadar seyrek (Ö2 ve mevcut tekdüze değer); (2) bu çalışma önceki literatüre ne
   ekledi (Ö1); (3) seçim ve arama bedelleri ödenince ne kadar kanıt kalır — ölçülmediği yerde "ölçülmedi".
2. Her sayının yanında sıfır, örneklem, isabet sayısı ve belirsizlik. Farklı senaryolar bir aralığın uçları gibi sunulmaz; ×7 okul çarpanı
   gibi üst sınırlar sıklık gibi okunmaz.
3. Birincil sayılar: Ö1'de T2* sırası (M20, U, Mallows ayrı ayrı); ikinciller: robot T2'si, tam birlikte-olasılık sırası, O', J10, 127,
   terazi açık. Ön-kayıt dışı her analiz "keşif" etiketlidir.
4. Ana hattın raporu depoya ekleyeceği bilinerek yazılır.

## 8. Sapma kuralları
Doğrulama başarısızlığı, bütçe kısıtı ya da hata düzeltmesi `ek/EK_<UTC>_<konu>.md` dosyasında, sebebi ve etkisiyle; düzeltilen kod
yeniden koşulur, eski ve yeni sonuç birlikte yazılır. Kodda bulunan bir hata (puanlayıcı, örnekleyici) düzeltmesi puanlara bakılmadan
önce bulunmuşsa da kayda geçer.

## 9. Dondurmadan önce yapılanlar (şeffaflık)
- Kod doğrulamaları: port bit-birebir (§1); puanlayıcı robot T2'sini yeniden üretir (U, M20; 127, 129; terazi açık/kapalı; 20.000'lik
  küçük tablolarla, yalnız fark denetlendi). Puanlayıcının (LOO) çalıştığı **koşulsuz** 2×10⁵ U kitabında sınandı; o sınamada Kur'an'ın
  koşulsuz T2 = 11,81, T2* = 11,41 (R19 Kur'an'ın koşulsuz hikâyesinde K1[F]'yi yineleme sayar), koşulsuz sırası 29/2×10⁵ görüldü
  (koşulsuz, sınama amaçlı; Ö0(a)'nın parçası değildir).
- Örnekleyici doğrulamaları (puan yok): kesin P(|F| = 12) = 0,0101534219 (127; yedinci oturumun büyük-tamsayı değeri 1,015342×10⁻²),
  129'da 0,0101497464; öneri k-marjinalleri kesin dağılımla uyumlu (χ² 113/≈103 hücre; Φ-kabullü 101); Mallows q (127) sekizinci
  denetimle aynı; ortalama I = 992,7 (hedef 993; 2×10⁵ örnek); M20 hızlı portu robot akışıyla birebir.
- **Ç1 sonucu (dondurmadan önce tamamlandı; 2,6×10⁷ C-kitap, 127, tekdüze, 4 bağımsız tohum 7101–7104, `sonuc/u_capa127_*.txt`):**
  P(G|C) = 965/2,6×10⁷ = 3,71×10⁻⁵ (GPU 17.141/444.513.933 = 3,856×10⁻⁵; −1,2σ); P(W|G) = 7.572/1,93×10⁶ tamamlama = 3,92×10⁻³ (965
  G-yapısı × 2.000; GPU 62/17.141 = 3,62×10⁻³; kesif18c 3,85×10⁻³) ⇒ **P(G∩W|C) = 1,456×10⁻⁷ (GPU 1,395×10⁻⁷; oran 1,04) — Ç1 tuttu.**
  Doğrudan G∩W isabeti 5 (GPU oranıyla beklenen 3,6). P(E|C) +%0,2, P(W|C) +%0,5 (1,5σ), P(A|C) +%0,0, P(DW|C) +%2 (0,7σ) — GPU ile uyumlu.
  **P(D|C) = 196.317/2,6×10⁷ = 7,551×10⁻³, GPU'nun 7,494×10⁻³'ünden +%0,76 (3,3σ) yüksek**; 86 milyarlık CPU arşivi (aynı tanımlar, ayrı
  tohumlama) 7,537±0,069×10⁻³ ile ikisiyle de uyumlu; öneri dağılımı kesin dağılıma uyuyor (yukarıda). Ç1'in ölçütü G∩W'dir; D farkı
  raporda açık gözlem olarak yazılacak. Φ/M! ≤ B ihlali 0; Φ kabul oranı %84,2.
- Olasılık yoklamaları (129; puan yok): P(O) ≈ U 1,3×10⁻⁶ (26/2×10⁷), J10 2,35×10⁻⁶ (47/2×10⁷); tarama koşularından M20
  5.000/1,103×10⁹ = 4,53×10⁻⁶ (emisyon başına; G 71 → 6,4×10⁻⁸), Mallows 1.500/7,22×10⁸ = 2,08×10⁻⁶ (G 45 → 6,2×10⁻⁸; ayrıca 2×10⁸
  örneklik yoklamada O 451, G 14). Hız (hafif yük): Mallows 1,21 µs/örnek (kaydırma vektörleştirildi; eski sürümle 2×10⁶ kitapta
  birebir aynı çıktı), J10 2,1 µs, M20 153 ns/emisyon.
- Ö0(b) taraması (yalnız olgu birlikte-görülmeleri; puan yok); aday dosyaları `sonuc/cikarim_adaylar_kosulsuz_129.txt`, `_127.txt`,
  `_kosulsuz_129_tum.txt` (bütün ikili/üçlü adaylar), `sonuc/cikarim_adaylar_O_129.txt` (U, M20 ve Mallows O-kitapları, 106.500),
  `_O_127.txt`. Listedeki kanıtlı kurallar dışında yeni kesin gerektirme bulunmadı.
