#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Özgüllük testi: fraktal ölçütler TÜM modüllere (3..40) eşit uygulanır.
Ölçütler modülden bağımsız, önceden tanımlı:
 S1: aile boyu >= 2×(114/m)          (beklenenin en az iki katı)
 S2: m² | Σt(aile)
 S3: aile t-paritesine 50/50 bölünür VE iki yarının Σt'si eşit
 S4: yarı toplamların her biri m²'nin katı
 S5: katsayı kümesi ardışık tam bant (boşluksuz)
 S6: |t-aile ∪ u-aile| == m  (İki El: birleşim üye sayısı modülün kendisi)
 S7: m | Σa(birleşim)
 S8: t-aile ile u-aile ayrık
Not: her ölçütün taban olasılığı m'ye göre değişir (ör. S6, m≈15 için kolay,
m=19 için beklenen birleşim ~12'dir); amaç mutlak p değil, profil karşılaştırması.
"""
import json
import numpy as np

d = json.load(open('/Users/dejuremacminim1/Desktop/hrf2/quran_meta.json'))
A0 = np.array([s['numberOfAyahs'] for s in d['data']['surahs']['references']])
N = np.arange(1, 115)
A = np.where(N == 9, 127, A0)
t = N + A; u = N - A

print(f"{'m':>3} {'aile':>4} {'bekl':>5} {'S1':>3} {'S2':>3} {'S3':>3} {'S4':>3} {'S5':>3} {'S6':>3} {'S7':>3} {'S8':>3}  {'skor':>4}   detay")
best = []
for m in range(3, 41):
    ft = (t % m == 0); fu = (u % m == 0)
    k = int(ft.sum())
    exp = 114 / m
    st = int(t[ft].sum())
    s1 = k >= 2 * exp
    s2 = (st % (m * m) == 0) and k > 0
    te = (t % 2 == 0)
    ke, ko = int((ft & te).sum()), int((ft & ~te).sum())
    se, so = int(t[ft & te].sum()), int(t[ft & ~te].sum())
    s3 = (ke == ko) and (se == so) and k > 0
    s4 = k > 0 and se % (m * m) == 0 and so % (m * m) == 0
    co = np.sort(t[ft] // m) if k else np.array([])
    s5 = k > 0 and len(set(co.tolist())) == (int(co.max()) - int(co.min()) + 1)
    uni = ft | fu
    s6 = int(uni.sum()) == m
    s7 = int(A[uni].sum()) % m == 0 if uni.any() else False
    s8 = not bool((ft & fu).any())
    skor = sum([s1, s2, s3, s4, s5, s6, s7, s8])
    def b(x): return ' ✓' if x else ' ·'
    print(f"{m:>3} {k:>4} {exp:>5.1f} {b(s1)} {b(s2)} {b(s3)} {b(s4)} {b(s5)} {b(s6)} {b(s7)} {b(s8)}  {skor:>4}   Σt={st}")
    best.append((skor, m))
best.sort(reverse=True)
print("\nEn yüksek skorlar:", best[:6])
