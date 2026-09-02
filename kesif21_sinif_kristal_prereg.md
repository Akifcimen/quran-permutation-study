# KESIF-21 ÖN KAYIT: Sınıf-Düzeyi Kristal G* ile Çekirdeğin Yeniden Ölçümü

**Tarih:** 2 Eylül 2026 · **Statü:** Analizden ÖNCE yazıldı.
**Bağlam:** Üçüncü dış denetim (kırmızı takım, `DIS_SALDIRI_kesif19_20.md`) kristalin G düzeyinin
"nokta-içerik" (tarif inceliği) taşıdığını ileri sürdü. Cevabımız: G'nin her adımı adlandırılabilir
bir sınıf yüklemidir; G, D'den yalnız "katsayılar ardışık bir bantta" yüklemiyle ayrılır ve bant
verilince torbalar cebirsel olarak zorunludur. Ama G'nin bant KONUMU (5–9) veriden okunmuş bir
ayrıntıdır. Bu test, bant konumunu serbest bırakan sınıf-düzeyi kristal G* ile çekirdeği yeniden
ölçer — merdivenin "sınıf-düzeyi" basamağını tahminle değil ölçümle doldurmak için.

## 1. G* tanımı (analizden önce sabit)
Aile tam 12 üye (2× beklenen); Σc = 76 (Σt = 38², tam kare); t-paritesine 6/6 ve 38/38 tam denge;
dört hücre (t-paritesi × n-paritesi) 3'er üye; hücre toplamları paritenin izin verdiği en dengeli
hâlde (çift-c tarafı 18+20, tek-c tarafı 19+19 — tek olasılık); 12 katsayının tamamı ardışık bir
tamsayı bandı oluşturur (min..max arası her değer mevcut), KONUM SERBEST. Sayım: **31 yapı**
(gerçek kitabınki: {6,6,6}{6,6,8} | {5,5,9}{5,7,7}, bant 5–9).

## 2. Yöntem
- kesif18'in SIS + ret örneklemesi, MUTLAK olasılık kestirimi verecek biçimde: P(G_σ) =
  (102!/114!) × Z_σ × E_SIS[ L(S)·P_avoid(S) / (M_σ·q(S)) ]; L(S) = etiketli sayım çarpanı
  (Π_v mult(v)!/(mult(v)−u_v)!), M_σ = özdeş katsayılı yuva sıralamalarının çokluğu, Z_σ = SIS
  başarı oranı (çıkmaz-yeniden-deneme düzeltmesi), q = çekiliş olasılığı (yönelim dahil).
- **Doğrulama çapası:** gerçek yapı σ₀ için bu kestirim, GPU'nun doğrudan saydığı
  P(G) = 7,14×10⁻¹⁰ (17.204 isabet, ±%0,8) ile uyuşmalı. Uyuşmazsa sonuç yayımlanmaz, hata aranır.
- P(G*) = Σ_σ P(G_σ). Her yapı için P(A|G_σ), P(W|G_σ) kabul edilen örneklerden (ağırlıklı).
- P(A∧W|G*): yapılar arası A∧W doğrudan isabeti seyrek olacağından, beyan edilen yaklaşıklık:
  P(A∧W|G*) ≈ Σ_σ w_σ P(A|σ)P(W|σ) × κ, κ = kesif18'de σ₀ için ölçülen etkileşim (×1,6 [1,0–2,4]).
  Sınırlılık olarak raporlanır.
- Çekirdek* = P(G*) × P(A∧W|G*). Bütçe: yapı başına 200 SIS ataması × 150k karışım.

## 3. Yorum kuralları (önceden)
- P(G*)/P(G) oranı, kristalin bant-konumu "nokta içeriğinin" fiyatıdır; çekirdek* buna göre
  1,7×10⁻¹⁵'ten yukarı kayar. Çıkan değer merdivene "sınıf-düzeyi çekirdek" basamağı olarak yazılır.
- Çapa uyuşmazsa ya da ağırlık ESS'i düşükse (< yapı başına 50), sonuç "ön" diye etiketlenir.

---

# SONUÇ (2 Eylül 2026 — analiz sonrası; ÖN: ESS eşiği altında, ikinci geçiş sürüyor)

- **Çapa:** P(G_σ₀)_SIS = 4,9×10⁻¹⁰ vs GPU 7,14×10⁻¹⁰ → oran 0,69. Kestirici ~%30 düşük (yapı başına
  ESS 8–46 < 50; ağır kuyruklu önem ağırlıkları, tipik alt-yanlılık). Mutlak değerler GPU çapasına
  oranlanarak kullanılır; sonuç "ön" etiketlidir, 1500×30k'lık ikinci geçiş hassasiyeti artıracak.
- **P(G*)/P(G_σ₀) ≈ 46** (bant konumu VE genişliği serbest, 31 yapı). Bant genişliği ≤ 5 tutulup
  yalnız konum serbest bırakılırsa (5 yapı): **≈ 6,5**. Geniş bantlı yapılar dar bantlıdan daha
  olası — yani kristalin "5–9" bandı ~1,7 basamaklık veriden-okunmuş içerik taşır: üçüncü denetimin
  G-düzeyi eleştirisi bu ölçüde HAKLIDIR ve deftere böyle yazılır.
- **Koşullu oranlar sınıf genelinde kararlı:** P(A|G*) = 3,1×10⁻⁴ (σ₀: 3,4×10⁻⁴), P(W|G*) = 3,9×10⁻³
  (σ₀: 4,5×10⁻³); doğrudan A∧W isabeti 7 / 3,49M G*-örneği = 2,0×10⁻⁶ [0,8–4,1] — κ-çarpımı
  (3,1×10⁻⁴ × 3,9×10⁻³ × 1,6 = 1,96×10⁻⁶) ile uyumlu; yaklaşıklık doğrulandı.
- **Sınıf-düzeyi çekirdek (GPU-çapalı):** bant tamamen serbest ≈ **6×10⁻¹⁴**; bant genişliği sabit
  (konum serbest) ≈ **1×10⁻¹⁴**. Merdivene bu iki basamak yazılır.
