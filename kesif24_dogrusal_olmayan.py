#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""KESIF-24: Ölçülmemiş son mercek ekseni — DOĞRUSAL OLMAYAN doğal biçimler.
kesif9/17/19 yalnız αn+βa biçimlerini yarıştırdı. Burada ilkokul aritmetiğiyle iki listeden
üretilebilecek doğrusal-olmayan doğal fonksiyonlar (çarpım, kareler, ebob, kare farkı, ...) aynı
S1–S8 katı/çatallı puanlamasıyla (kesif19 mantığı) gerçek kitapta, tüm modüllerde denenir.
İkinci el (w) her biçim için ayrıca belirtilir; kanonik eşleniği olmayanlar için w = n−a alınır."""
import json
import numpy as np
from math import gcd
from kesif22_incil_catalli import MODS
from kesif19_catallanma import pretty, is_square, rowstats, BIG

N = np.arange(1, 115, dtype=np.int64)
K0 = np.array([s['numberOfAyahs'] for s in json.load(open('quran_meta.json'))['data']['surahs']['references']], dtype=np.int64)
A = np.where(N == 9, 127, K0)
G = np.array([gcd(int(n), int(a)) for n, a in zip(N, A)], dtype=np.int64)

FORMS = {
    "n+a (çekirdek)":      (N + A,           N - A),
    "n·a":                 (N * A,           N - A),
    "n²+a":                (N * N + A,       N - A),
    "n+a²":                (N + A * A,       N - A),
    "n²+a²":               (N * N + A * A,   N * N - A * A),
    "n²−a² (t·u)":         (N * N - A * A,   N * N + A * A),
    "ebob(n,a)":           (G,               N - A),
    "n·a+n+a":             (N * A + N + A,   N * A - N - A),
    "(n+a)²":              ((N + A) ** 2,    (N - A) ** 2),
    "a² − n":              (A * A - N,       A * A + N),
    "n² − a":              (N * N - A,       N * N + A),
    "n·a − (n+a)":         (N * A - N - A,   N * A + N + A),
    "|n−a|":               (np.abs(N - A),   N + A),
}

def score(V, W, m):
    V = V[None, :]; W = W[None, :]; P = A[None, :]; Nn = N[None, :]; E = 114 / m
    F = (V % m == 0); k = F.sum(1); kpos = k > 0
    C = np.where(F, V // m, 0)
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
    kk, cmin, cmax, nd = rowstats(np.where(F, V // m, BIG), F)
    band = kpos & (nd == cmax - cmin + 1)
    s5 = band; f5 = band  # adım-2 AD çatalı burada atlandı (muhafazakâr: çatallı puanı düşürür)
    U = (W % m == 0); ku = U.sum(1); uni = F | U; kuni = uni.sum(1); inter = (F & U).sum(1)
    s6 = kuni == m; f6 = s6 | (kuni == 2 * m) | ((ku == k) & kpos) | (ku == m) | (ku >= 2 * E)
    sa_u = (P * uni).sum(1); sn_u = (Nn * uni).sum(1); sv_u = (V * uni).sum(1); sw_u = (W * uni).sum(1)
    s7 = sa_u % m == 0
    f7 = s7 | (sn_u % m == 0) | (sv_u % m == 0) | (sw_u % m == 0) | pretty(sa_u, m) | pretty(sn_u, m) | pretty(sv_u, m) | pretty(sw_u, m)
    s8 = inter == 0; f8 = s8 | (inter == 1)
    return int((s1.astype(np.int8) + s2 + s3 + s4 + s5 + s6 + s7 + s8)[0]), int((f1.astype(np.int8) + f2 + f3 + f4 + f5 + f6 + f7 + f8)[0])

print(f"{'biçim':18s} {'katı':>6s} {'çatallı':>8s}   en iyi modül(ler)")
for ad, (V, W) in FORMS.items():
    best_s = best_f = 0; where = []
    for m in MODS:
        for v, w in ((V, W), (W, V)):
            s, f = score(v.astype(np.int64), w.astype(np.int64), m)
            best_s = max(best_s, s)
            if f > best_f: best_f, where = f, [m]
            elif f == best_f and m not in where: where.append(m)
    print(f"{ad:18s} {best_s:>5d}/8 {best_f:>7d}/8   {where[:6]}{' …' if len(where) > 6 else ''}")
