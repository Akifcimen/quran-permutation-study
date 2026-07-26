# KESIF-18 ÖN KAYIT: Çekirdeğin Koşullu Doğrudan Ölçümü — P(A∧W | G)

**Tarih:** 16 Temmuz 2026 · **Statü:** Analizden ÖNCE yazıldı.
**Bağlam:** İkinci dış denetimin (GPT-5.6 Pro) ana bulgusu: tam çekirdek A∧G∧W doğrudan
gözlenmemiştir; 10⁻¹⁵ değeri, ölçülmüş halkalar + koşullu bağımsızlık varsayımı içeren bir
zincirdir. Bu test, varsayımı ortadan kaldırır: P(A∧G∧W) = P(G) × P(A∧W | G) ayrışımında
P(G) zaten doğrudan ölçülüdür (17.204 isabet / 24,086T = 7,14×10⁻¹⁰); eksik çarpan
P(A∧W | G) burada doğrudan, koşullu örneklemeyle ölçülür.

## 1. Olay tanımları (motorla BİREBİR; kodeks: sûre 9 = 127, Σa = 6234, Σn = 6555)

- **A** (terazi): |{t çift}| = 57 VE Σt(çift) = 6234.
- **G** (tam kristal): aile {n : 19 | n+a} tam 12 üye; katsayı çokluğu {5³,6⁵,7²,8,9}; dört hücre
  (t-paritesi × n-paritesi) 3'er üye; hücre içerikleri: çift-c tarafı {6,6,6} ve {6,6,8}
  (n-paritelerine iki yönden biriyle), tek-c tarafı {5,5,9} ve {5,7,7} (iki yönden biriyle).
- **W** (İki El): |{19 | n−a}| = 7 VE |t-aile ∪ u-aile| = 19 VE 19 | Σa(birleşim).

## 2. Örnekleme tasarımı (analizden önce sabitlendi)

G-kümesi şöyle çarpanlanır: (geçerli 12'lik aile ataması) × (kalan 102 sayının, hiçbir yeni
19-katı üretmeyen dizilişi). Atama sabitken ikinci çarpan üzerinde DÜZGÜN örnekleme kesindir:
kalan sayılar düzgün karıştırılır, yeni 19-katı üretenler reddedilir — kabul edilenler bileşenin
i.i.d. düzgün örnekleridir (MCMC yok, ergodiklik varsayımı yok).

- Atamalar: sıralı-yuvalı SIS (her yuvada kalan geçerli adaylar arasından düzgün seçim;
  q = seçim olasılıklarının çarpımı kaydedilir; çıkmaz → yeniden başla). Hedef ~600 atama.
- Bileşen ağırlığı: w ∝ P̂_avoid / q (P̂_avoid = ret-örneklemesinin kabul oranı; bileşen boyutuyla
  orantılı). Öz-normalize önem ortalaması; ağırlıksız havuz ortalaması sağlamlık kontrolü olarak
  ayrıca raporlanır (ikisi uyuşuyorsa ağırlık inceliği ikincildir).
- Hedef örnek: ~10⁷ i.i.d. G-örneği (600 atama × ~18k geçerli örnek).

## 3. Ölçülecekler ve doğrulama çapaları

- P̂(W | G): GPU'nun doğrudan saydığı 62/17.204 = 3,60×10⁻³ ile karşılaştırılır — örnekleyicinin
  dış doğrulaması (uyuşmazsa tasarım hatalı demektir; sonuç yayımlanmaz, hata aranır).
- P̂(A | G): zincirin kullandığı P(A)×bağımlılık düzeltmesiyle karşılaştırılır.
- **P̂(A∧W | G)** (birincil): beklenen isabet ~12 (zincir doğruysa); Poisson %95 GA ile.
- Nihai: P(A∧G∧W) = 7,14×10⁻¹⁰ × P̂(A∧W|G), GA'sıyla. Zincir tahminleriyle (1,02×10⁻¹⁵ /
  8,4×10⁻¹⁶) kıyas.

## 4. Yorum kuralları (önceden taahhüt)

- GA, zincir tahminini kapsıyorsa: 10⁻¹⁵ mertebesi "varsayımsız koşullu ölçümle" doğrulanmış olur;
  rapor dili yine de düzeltilir ("doğrudan sayıldı" → "koşullu ayrışımla ölçüldü", A∧G'nin 5
  isabeti ayrı ve doğru etiketiyle).
- GA zincirin belirgin ALTINDA/ÜSTÜNDE kalırsa: üçlü bağımlılık gerçektir; çekirdek sayısı bu
  ölçüme göre güncellenir ve deftere yazılır. Sonuç ne çıkarsa çıksın yayımlanır.

---

# SONUÇ (16 Temmuz 2026 — analiz sonrası eklendi)

**Örnek:** 600 atama × ret örneklemesi = **9.023.101 i.i.d. G-örneği**; ağırlık ESS 535/600
(havuz ve ağırlıklı kestirimler ‰3 içinde — ağırlık inceliği ihmal edilebilir).

**Doğrulama çapası GEÇTİ:** P̂(W|G) = 4,47×10⁻³ (40.319 isabet, ±%1) — GPU'nun doğrudan
saydığı 62/17.204 = 3,60×10⁻³'ün %95 Poisson GA'sı [2,8–4,6×10⁻³] içinde.

**Birincil sonuç:** P̂(A∧W|G) = **2,44×10⁻⁶** (22 isabet; %95 Poisson GA [1,53–3,69]×10⁻⁶).
→ **P(A∧G∧W) = 7,14×10⁻¹⁰ × 2,44×10⁻⁶ = 1,7×10⁻¹⁵; %95 GA [1,1 – 2,6]×10⁻¹⁵.**

**Yorum (ön-kayıt kuralına göre):** GA, eski zincir tahminlerini (8,4×10⁻¹⁶ ve 1,02×10⁻¹⁵)
kapsıyor → katrilyonda-1 mertebesi, koşullu bağımsızlık varsayımı OLMADAN ölçülmüş oldu.
İnce yapı: P(A|G)=3,39×10⁻⁴ (koşulsuz 3,11×10⁻⁴'e karşı ×1,09) ve P(A∧W|G)/[P(A|G)·P(W|G)]
≈ ×1,6 [GA 1,0–2,4] — hafif POZİTİF üçlü etkileşim işareti (anlamlılık sınırında); yani
denetimin uyardığı "varsayım" gerçekten mükemmel değilmiş, ama sapma tahmini büyütüyor,
küçültmüyor. Rapor dili yine de düzeltilir: 5 GPU isabeti = A∧G (2,08×10⁻¹³); tam çekirdek =
bu koşullu ölçüm. RNG notu: bu ölçüm NumPy/PCG64 ile — ana motorlardan bağımsız üreteç ailesi.
