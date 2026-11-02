# Ara tahmin (Kur'an'ın kestirimlerinden sonra, plasebo kestirimleri ve doğrulama okunmadan önce)

Kayıt zamanı aşağıda, `date -u` ile.

Bilinen: Kur'an'ın O2 = O3 kestirimi M20 6,0e-13, Mallows 3,4e-13, tekdüze 2,1e-15.

Kur'an'ın merceğinde, model bilgisinin ötesinde kalan bit:

| | Kalan bit | Karşılaştırma: O2 medyanı | Karşılaştırma: O3 medyanı |
|---|---|---|---|
| M20 | +6,4 | +1,3 | +4,1 |
| Mallows | +5,9 | +1,3 | +4,1 |
| tekdüze | +9,8 | +1,3 | +4,1 |

| | M20 | Mallows | tekdüze |
|---|---|---|---|
| R_O3 | 0 (olasılık ~0,7; aday A6053, 6,1 bit ister); 1 (~0,25) | 0 | 0 |
| R_O2 | 3–15 | 1–5 | 0–1 |
| Menü (3 × en küçük p_kal), R_O3 = 0 ise | 3/20.001 ≈ 1/6.700 | 3/10.001 ≈ 1/3.300 | 3/10.001 ≈ 1/3.300 |

- R_O3 = 0 olursa okuma "aşar" olur ve değer K'nın tabanıdır.
- Doğrulamanın geçme olasılığı:

  | | Olasılık |
  |---|---|
  | M20 | ~0,8 |
  | Mallows | ~0,65 |
  | tekdüze | ~0,55 (yakınsama sorunu; şart (e)) |
2026-11-01T17:52:01Z
