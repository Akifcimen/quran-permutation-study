#!/usr/bin/env python3
"""Tam birlikte-olasılık çözünürlüğü: her rakibin P_tam'ı için %95 Poisson (Garwood) aralığı, Kur'an'ınkiyle karşılaştırma:
kesin geçen (üst sınırı Kur'an'ın alt sınırının altında), kesin geçmeyen, çözülemeyen. Kullanım: python3 -I src/tam_cozunurluk.py <prefix>.tam.txt"""
import sys, math, numpy as np
def gamma_ppf(p, a):   # Gamma(a,1) çeyreklik (Wilson–Hilferty + Newton yerine ikili arama, regularize alt gama)
    from math import lgamma, exp, log
    def P(x):  # regularize alt eksik gama
        if x <= 0: return 0.0
        if x < a + 1:
            s = 1.0 / a; t = s; n = 0
            while True:
                n += 1; t *= x / (a + n); s += t
                if t < s * 1e-14: break
            return s * exp(-x + a * log(x) - lgamma(a))
        b = x + 1 - a; c = 1e300; d = 1 / b; h = d
        for i in range(1, 300):
            an = -i * (i - a); b += 2; d = an * d + b; d = 1e-300 if abs(d) < 1e-300 else d; c = b + an / c; c = 1e-300 if abs(c) < 1e-300 else c
            d = 1 / d; de = d * c; h *= de
            if abs(de - 1) < 1e-14: break
        return 1 - exp(-x + a * log(x) - lgamma(a)) * h
    lo, hi = 0.0, max(10.0, a * 10 + 50)
    for _ in range(200):
        m = 0.5 * (lo + hi)
        if P(m) < p: lo = m
        else: hi = m
    return 0.5 * (lo + hi)
def garwood(k):
    lo = 0.0 if k == 0 else gamma_ppf(0.025, k); hi = gamma_ppf(0.975, k + 1); return lo, hi
fn = sys.argv[1]; head = ''; rows = []
for l in open(fn):
    if l.startswith('#'): head += l; continue
    a = l.split('|')[0].split(); rows.append(dict(sahip=int(a[0]), t2t=float(a[1]), duz=int(a[3]), h1=int(a[4]), nF=int(a[5]), m=int(a[6]), hn=int(a[9]), S=float(a[11])))
N = int(head.split('N=')[1].split()[0]); R = int(head.split('R=')[1].split(',')[0])
def kscale(r):
    Ns = N - 1 if r['sahip'] >= 0 else N
    if r['duz'] == 1: return r['h1'], 1.0 / Ns
    return (r['hn'], (r['nF'] / Ns) / (R * r['m'])) if r['m'] else (0, 0.0)
Q = [r for r in rows if r['sahip'] < 0][0]; riv = [r for r in rows if r['sahip'] >= 0]
kq, sq = kscale(Q); ql, qh = garwood(kq); ql *= sq; qh *= sq
print(f"Kur'an: P_tam = {kq*sq:.3e} [%95 {ql:.3e}, {qh:.3e}] ({kq} isabet), S_tam = {Q['S']:.3f}, T2* = {Q['t2t']:.3f}")
cache = {}
ab = be = un = 0; nb = 0
for r in riv:
    k, s = kscale(r)
    if k not in cache: cache[k] = garwood(k)
    lo, hi = cache[k]; lo *= s; hi *= s
    if hi < ql: ab += 1
    elif lo > qh: be += 1
    else: un += 1
print(f"rakip {len(riv)}: kesin daha nadir (Kur'an'ı geçer) {ab} | kesin daha sık {be} | çözülemeyen (aralıklar örtüşür) {un}")
print(f"→ tam puanla k_tam ∈ [{ab}, {ab+un}] (kesin sınırlar; N = {N:,} için p ∈ [{(ab+1)/(N+1):.3e}, {(ab+un+1)/(N+1):.3e}])")
t = np.array([r['t2t'] for r in riv]); S = np.array([r['S'] for r in riv])
for lo_, hi_ in [(Q['t2t']-3, Q['t2t']-2), (Q['t2t']-2, Q['t2t']-1), (Q['t2t']-1, Q['t2t']), (Q['t2t'], Q['t2t']+1), (Q['t2t']+1, 99)]:
    m = (t >= lo_) & (t < hi_); print(f"  T2* [{lo_:.2f}, {hi_:.2f}): {m.sum()} kitap; nokta kestirimle S_tam ≥ S_Kur'an: {(S[m] >= Q['S']).sum()}")
