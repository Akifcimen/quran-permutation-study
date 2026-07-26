#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Yüzer-modül sıfır testi (DÜZELTİLMİŞ, 16 Temmuz 2026): sentetik dizilişlerde HERHANGİ bir m
(3..40) kaç/8 puana ulaşıyor? (S1-S8 tanımları kesif4_ozgulluk.py ile aynı.)

DÜZELTME NOTU: İlk sürüm yalnız "S5-dışı 7 ölçütün TÜMÜ geçerse S5'e bak" yolunu sayıyordu;
"herhangi 7/8" (S5 doğru + tek başka ölçüt yanlış) vakalarını taramıyordu. Kusuru ikinci dış
denetim bildirdi (bkz. KESIF.md dürüstlük notu 11); denetçinin kendi düzeltilmiş tekrarında da
tavan 6/8 çıkmıştı. Bu sürüm tam puanı hesaplar: 7 ucuz ölçüt + S5 (ucuz-puan ≥ 5 olan her
adayda değerlendirilir; ≥6 toplam puanlar böylece kesindir)."""
import json
import numpy as np

d = json.load(open('quran_meta.json'))
A0 = np.array([s['numberOfAyahs'] for s in d['data']['surahs']['references']], dtype=np.int64)
N = np.arange(1, 115, dtype=np.int64)
A = np.where(N == 9, 127, A0)
rng = np.random.default_rng(41)
TR, CH = 500_000, 5_000

def s5_band(vrow, fvrow, m):
    co = np.sort(vrow[fvrow] // m)
    return len(co) > 0 and len(set(co.tolist())) == int(co[-1] - co[0] + 1)

hist = np.zeros(9, dtype=np.int64)
for b in range(TR // CH):
    P = rng.permuted(np.tile(A, (CH, 1)), axis=1)
    T = N[None, :] + P
    U = N[None, :] - P
    TE = (T % 2 == 0)
    best = np.zeros(CH, dtype=np.int8)
    for m in range(3, 41):
        FT = (T % m == 0); k = FT.sum(1)
        s1 = k >= 2 * 114 / m
        st = (T * FT).sum(1)
        s2 = (st % (m * m) == 0) & (k > 0)
        ke = (FT & TE).sum(1); se = (T * (FT & TE)).sum(1)
        s3 = (2 * ke == k) & (2 * se == st) & (k > 0)
        s4 = (se % (m * m) == 0) & ((st - se) % (m * m) == 0) & (k > 0)
        FU = (U % m == 0); uni = FT | FU
        s6 = uni.sum(1) == m
        s7 = ((P * uni).sum(1) % m == 0)
        s8 = ~((FT & FU).any(1))
        sc = (s1.astype(np.int8) + s2 + s3 + s4 + s6 + s7 + s8)
        for r in np.where(sc >= 5)[0]:
            if s5_band(T[r], FT[r], m):
                sc[r] += 1
        np.maximum(best, sc, out=best)
    hist += np.bincount(best, minlength=9)
    if (b + 1) % 20 == 0:
        print(f"{(b+1)*CH:,}/{TR:,} | {dict((i,int(c)) for i,c in enumerate(hist) if c)}", flush=True)

print(f"\nSONUÇ — sentetik başına azami puan (modül serbest, tam 8-ölçüt mantığı):")
for i, c in enumerate(hist):
    if c: print(f"  puan {i}: {c:,} ({c/TR:.3%})")
print(f"  ≥7: {int(hist[7:].sum())} | tam profil: {int(hist[8])}")
