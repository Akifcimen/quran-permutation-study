#!/usr/bin/env python3
"""Kanıt yardımcısı (kombinatorik, tam tarama): k < 12 (çift) katsayılı bir ailede
  (i) katsayılar ardışık bant (min..max arası her tamsayı en az bir kez), (ii) tam k/2 katsayı çift (yarılar eşit boy),
  (iii) |Σçift − Σtek| ∈ {19, 38} (katsayı biriminde; v-yarılar Δ ∈ {m², 2m²}) birlikte mümkün mü?
Bant genişliği w ≤ k−1. Yalnız katsayı çokluğuna bakar (konum/değer kısıtları gevşetilir → 'mümkün değil' sonucu kesin kanıttır).
Ayrıca Δ'nın her k için ulaşılabilir en büyük değeri yazılır."""
from itertools import combinations_with_replacement
def maks_delta(k):
    best = 0
    for w in range(0, k):            # bant genişliği
        # başlangıç paritesi önemli: m çift ya da tek (mutlak konum önemsiz, yalnız parite)
        for m0 in (0, 1):
            vals = list(range(m0, m0 + w + 1))
            extra = k - (w + 1)
            if extra < 0: continue
            for add in combinations_with_replacement(vals, extra):
                ms = vals + list(add)
                ev = [v for v in ms if v % 2 == 0]
                if len(ev) * 2 != k: continue
                d = abs(sum(ev) - sum(v for v in ms if v % 2))
                best = max(best, d)
    return best
for k in (2, 4, 6, 8, 10):
    print(f"k={k:2d}: bant + eşit-boy yarılar ile ulaşılabilir en büyük |Σçift−Σtek| = {maks_delta(k)}  (gereken ≥ 19)")
