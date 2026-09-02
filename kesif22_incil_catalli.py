#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""KESIF-22: "İncil'de de bulunan türden mi?" — saldırganın (kesif19) EN GEVŞEK puanlaması
(çatallı S1–S8, 304 mercek × 2 el, serbest) gerçek KJV korpuslarına ve Kur'an'a aynen uygulanır.
Soru: kitabın 8/8'i, çatallı-serbest prosedürle başka gerçek metinlerde de çıkıyor mu?"""
import json
import numpy as np
from kesif19_catallanma import FORMS, MODS, pretty, is_square, rowstats, BIG

def score_n(a, al, be, m, primary_sum=True):
    n = len(a); Nn = np.arange(1, n + 1, dtype=np.int64)[None, :]; P = np.asarray(a, dtype=np.int64)[None, :]
    V = al * Nn + be * P; W = al * Nn - be * P
    if not primary_sum: V, W = W, V
    E = n / m
    F = (V % m == 0); k = F.sum(1); kpos = k > 0
    C = V // m
    sv = (V * F).sum(1); sc = (C * F).sum(1)
    s1 = k >= 2 * E; f1 = s1 | (k == m)
    s2 = kpos & (sv % (m * m) == 0); f2 = kpos & pretty(sv, m)
    VE = (V % 2 == 0); FE = F & VE
    ke = FE.sum(1); sve = (V * FE).sum(1); svo = sv - sve
    eqcnt = (2 * ke == k) & kpos
    s3 = eqcnt & (sve == svo); d = np.abs(sve - svo)
    f3v = eqcnt & ((d == 0) | (d == m * m) | (d == 2 * m * m))
    NE = (Nn % 2 == 0); FN = F & NE
    kn = FN.sum(1); svn = (V * FN).sum(1); svn2 = sv - svn
    eqn = (2 * kn == k) & kpos; dn = np.abs(svn - svn2)
    f3 = f3v | (eqn & ((dn == 0) | (dn == m * m) | (dn == 2 * m * m)))
    s4 = kpos & (sve % (m * m) == 0) & (svo % (m * m) == 0)
    f4 = s4 | (kpos & is_square(sve) & is_square(svo)) | (kpos & (svn % (m * m) == 0) & (svn2 % (m * m) == 0))
    kk, cmin, cmax, nd = rowstats(C, F)
    band = kpos & (nd == cmax - cmin + 1)
    ap2 = kpos & ((cmax - cmin) % 2 == 0) & (nd == (cmax - cmin) // 2 + 1) & (nd >= 2) & \
          (((C * F) % 2 == (cmin[:, None] % 2)) | ~F).all(1)
    s5 = band; f5 = band | ap2
    U = (W % m == 0); ku = U.sum(1); uni = F | U; kuni = uni.sum(1); inter = (F & U).sum(1)
    s6 = kuni == m
    f6 = s6 | (kuni == 2 * m) | ((ku == k) & kpos) | (ku == m) | (ku >= 2 * E)
    sa_u = (P * uni).sum(1); sn_u = (Nn * uni).sum(1); sv_u = (V * uni).sum(1); sw_u = (W * uni).sum(1)
    s7 = sa_u % m == 0
    f7 = s7 | (sn_u % m == 0) | (sv_u % m == 0) | (sw_u % m == 0) | \
         pretty(sa_u, m) | pretty(sn_u, m) | pretty(sv_u, m) | pretty(sw_u, m) | \
         ((P * U).sum(1) % m == 0) | ((Nn * U).sum(1) % m == 0) | ((ku > 0) & ((W * U).sum(1) == m * ku))
    s8 = inter == 0; f8 = s8 | (inter == 1)
    strict = int((s1.astype(np.int8) + s2 + s3 + s4 + s5 + s6 + s7 + s8)[0])
    forked = int((f1.astype(np.int8) + f2 + f3 + f4 + f5 + f6 + f7 + f8)[0])
    return strict, forked

kjv = json.load(open('korpus_kjv.json'))
qd = json.load(open('quran_meta.json'))
K0 = np.array([s['numberOfAyahs'] for s in qd['data']['surahs']['references']], dtype=np.int64)
N114 = np.arange(1, 115)
corpora = {
    "Kur'an (114 sûre, kodeks 127)": np.where(N114 == 9, 127, K0),
    "KJV Mezmurlar (150 mezmur, ayet sayıları)": np.array(kjv['psalms_verses']),
    "KJV kitaplar (66 kitap, bölüm sayıları)": np.array(kjv['book_chapters']),
    "KJV bölümler (1189 bölüm, ayet sayıları)": np.array(kjv['chapter_verses']),
}
print("Çatallı-serbest prosedür (kesif19 puanlaması: 304 mercek × 2 el) → her korpusun EN İYİ merceği")
print(f"{'korpus':46s} {'katı':>6s} {'çatallı':>8s}   en iyi mercek(ler)")
for ad, a in corpora.items():
    best_s = best_f = 0; where_f = []
    for al, be in FORMS:
        for m in MODS:
            for prim in (True, False):
                s, f = score_n(a, al, be, m, prim)
                best_s = max(best_s, s)
                if f > best_f: best_f, where_f = f, [(al, be, m, 't' if prim else 'u')]
                elif f == best_f: where_f.append((al, be, m, 't' if prim else 'u'))
    print(f"{ad:46s} {best_s:>5d}/8 {best_f:>7d}/8   {where_f[:4]}{' …' if len(where_f) > 4 else ''}")
