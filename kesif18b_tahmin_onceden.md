# kesif18b — koşu bitmeden yazılmış tahmin (2 Eylül 2026, atama 100/1200 iken)

Gerekçe: (1) eski kesif18'de p_avoid/q-ağırlıklı ile havuz kestirimleri ‰3 içinde — koşullu oran
atamaya zayıf bağlı; L(S) ağırlıkları bazı atamaları ×2–4 değiştirir ama oran atama-bağımsızsa
etki küçük. (2) kesif21 2. geçişin L-ağırlıklı σ₀ oranları (A 2,0e-4, W 2,8e-3) kesif18'den
düşük ama gürültülü (ESS 103; 1. geçiş 3,7e-4 / 4,4e-3 vermişti). (3) GPU çapası P(W|G)=3,6e-3
[2,8–4,6]; kesif18'in 4,47e-3'ü üst kenardaydı — düzeltme aşağı yönde çapaya YAKLAŞTIRABİLİR.

TAHMİN: P(W|G) ≈ 3,5–4,3×10⁻³ · P(A|G) ≈ 2,8–3,5×10⁻⁴ · κ ≈ 1,4–1,9 ·
P(A∧W|G) ≈ 1,6–2,4×10⁻⁶ (eski 2,44) · bootstrap %95 GA ≈ [0,9–3,6]×10⁻⁶ (eskisinden ~%50 geniş) ·
ÇEKİRDEK ≈ 1,1–1,7×10⁻¹⁵ (eski 1,74); mertebe değişmez.
Beni şaşırtacak sonuç: iki yönde de >×2 değişim (koşullu oranların değer-çokluğuna güçlü
bağımlılığı demek olur; şimdiye dek hiçbir ölçüm bunu göstermedi). Çapa P(G): 0,9–1,1.

## SONUÇ (koşu bitti 23:28) — TAHMİN TUTMADI
Çapa 1,64 (üç koşu: 0,69 / 0,99 / 1,64 → kestirici kararsız); **ESS 6/1200** — önem ağırlıkları
dejenere. Ağırlıklı P(W|G) 2,28e-3 (GPU çapası [2,8–4,6]e-3'ün ALTINDA); ağırlıklı P(A∧W|G) 5,5e-7
(havuz 2,1e-6) → çekirdek 3,9e-16 [1,1e-16, 1,0e-15]; κ 0,72. Tahmin (%0–35 düşüş) ×4 düşüşle
tutmadı — denetçinin uyardığı sebeple: seyrek ortak isabet + değişken önem ağırlıkları. Bu koşunun
hiçbir sayısı güvenilir değildir (ne eskisi ne yenisi). Teşhis: tekdüze aday önerisi L/q'yu
ağır-kuyruklu kılıyor. Çözüm (kırmızı takım 2. tur önerisi): çokluk-orantılı önerici → kesif18c.
