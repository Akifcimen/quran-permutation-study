#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""KESIF-17: Genişletilmiş mercek turnuvası — biçim (α,β) × modül (m) serbestliği.
Ön-kayıt: kesif17_mercek_uzayi_prereg.md
A1: 100k sentetik × 304 mercek → azami puan dağılımı (S5 tembel: yalnız 7-puan ≥ 5 adaylarda).
A2: gerçek kitap × 304 mercek → tam harita."""
import json, sys
import numpy as np

N = np.arange(1, 115, dtype=np.int64)
K = np.array([s['numberOfAyahs'] for s in json.load(open('quran_meta.json'))['data']['surahs']['references']], dtype=np.int64)
FORMS = [(0, 1), (1, 1), (1, 2), (1, 3), (2, 1), (2, 3), (3, 1), (3, 2)]
MODS = range(3, 41)

def s5_band(vrow, fvrow, m):
    co = np.sort(vrow[fvrow] // m)
    return len(co) > 0 and len(set(co.tolist())) == int(co[-1] - co[0] + 1)

def scores_batch(P):
    """P: (B,114) dizilişler → (B,) azami puan (304 mercek üzerinden)."""
    B = P.shape[0]
    best = np.zeros(B, dtype=np.int8)
    for al, be in FORMS:
        V = al * N[None, :] + be * P
        W = al * N[None, :] - be * P
        VE = (V % 2 == 0)
        for m in MODS:
            FV = (V % m == 0); k = FV.sum(1)
            s1 = k >= 2 * 114 / m
            st = (V * FV).sum(1)
            s2 = (st % (m * m) == 0) & (k > 0)
            ke = (FV & VE).sum(1); se = (V * (FV & VE)).sum(1)
            s3 = (2 * ke == k) & (2 * se == st) & (k > 0)
            s4 = (se % (m * m) == 0) & ((st - se) % (m * m) == 0) & (k > 0)
            FW = (W % m == 0); uni = FV | FW
            s6 = uni.sum(1) == m
            s7 = (P * uni).sum(1) % m == 0
            s8 = ~((FV & FW).any(1))
            sc7 = (s1.astype(np.int8) + s2 + s3 + s4 + s6 + s7 + s8)
            hi = np.where(sc7 >= 5)[0]
            for r in hi:
                if s5_band(V[r], FV[r], m):
                    sc7[r] += 1
            np.maximum(best, sc7, out=best)
    return best

def full_map_real():
    print("=== A2: GERÇEK KİTAP × 304 mercek (tam 8 ölçüt) ===")
    rows = []
    for al, be in FORMS:
        V = (al * N + be * K); W = (al * N - be * K)
        VE = (V % 2 == 0)
        for m in MODS:
            FV = (V % m == 0); k = int(FV.sum())
            if k == 0: continue
            st = int(V[FV].sum())
            s1 = k >= 2 * 114 / m
            s2 = st % (m * m) == 0
            ke = int((FV & VE).sum()); se = int(V[FV & VE].sum())
            s3 = (2 * ke == k) and (2 * se == st)
            s4 = (se % (m * m) == 0) and ((st - se) % (m * m) == 0)
            FW = (W % m == 0); uni = FV | FW
            s6 = int(uni.sum()) == m
            s7 = int(K[uni].sum()) % m == 0
            s8 = not bool((FV & FW).any())
            s5 = s5_band(V, FV, m)
            sc = sum([s1, s2, s3, s4, s5, s6, s7, s8])
            rows.append((sc, al, be, m))
    rows.sort(reverse=True)
    for sc, al, be, m in rows[:10]:
        tag = "  ← ÇEKİRDEK" if (al, be, m) == (1, 1, 19) else ""
        print(f"  puan {sc}/8: v={al}n+{be}a, m={m}{tag}")
    return rows

full_map_real()

print("\n=== A1: 100.000 sentetik × 304 mercek (tohum 42) ===", flush=True)
rng = np.random.default_rng(42)
TR, CH = 100_000, 2_000
hist = np.zeros(9, dtype=np.int64)
for b in range(TR // CH):
    P = rng.permuted(np.tile(K, (CH, 1)), axis=1)
    best = scores_batch(P)
    hist += np.bincount(best, minlength=9)
    if (b + 1) % 5 == 0:
        print(f"  {(b+1)*CH:,}/{TR:,} | dağılım şu ana dek: {dict((i,int(c)) for i,c in enumerate(hist) if c)}", flush=True)
print(f"\nSONUÇ — sentetik başına azami puan (biçim+modül serbest):")
for i, c in enumerate(hist):
    if c: print(f"  puan {i}: {c:,}  ({c/TR:.2%})")
print(f"  ≥7: {int(hist[7:].sum())} | tam profil (8): {int(hist[8])}")
