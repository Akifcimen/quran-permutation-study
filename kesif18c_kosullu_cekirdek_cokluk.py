#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""KESIF-18c: kesif18b.nin ÖNERİCİSİ DÜZELTİLMİŞ sürümü — aday seçimi kalan çokluğuyla orantılı (kırmızı takım 2. tur önerisi); L/q analitik olarak sadeleşir, ağırlıklar düzleşir. kesif18b (dördüncü dış denetim, 6astraultra, 2 Eylül 2026).
Hata: kesif18'in önem ağırlığı p_avoid/q idi; etiketli-çokluk çarpanı L(S)=Π m_v!/(m_v−k_v)! eksikti
(kesif21'de vardı, kesif18'de yoktu). Ayrıca nihai P(A∧W|G) ağırlıksız havuz oranıydı ve Poisson GA
atama-seçimi belirsizliğini içermiyordu.
Bu sürüm: (1) ağırlık = L·p_avoid/(M·q), Z düzeltmesi; (2) mutlak P(G) çapası (GPU 7,14e-10);
(3) ağırlıklı P(A|G), P(W|G), P(A∧W|G); (4) atama-düzeyi ağırlıklı bootstrap GA; (5) κ yeniden."""
import json, sys
from collections import Counter
from math import lgamma, log, exp
import numpy as np

rng = np.random.default_rng(1819)
N = np.arange(1, 115)
K0 = np.array([s['numberOfAyahs'] for s in json.load(open('quran_meta.json'))['data']['surahs']['references']], dtype=np.int64)
A_ALL = np.where(N == 9, 127, K0); assert int(A_ALL.sum()) == 6234
CNT = Counter(A_ALL.tolist())
LOG_REST = sum(log(x) for x in range(103, 115))   # log(114!/102!)
CELLS_ORI = [[((6,6,6),1),((6,6,8),0),((5,5,9),0),((5,7,7),1)], [((6,6,6),0),((6,6,8),1),((5,5,9),0),((5,7,7),1)],
             [((6,6,6),1),((6,6,8),0),((5,5,9),1),((5,7,7),0)], [((6,6,6),0),((6,6,8),1),((5,5,9),1),((5,7,7),0)]]
LOG_M = lgamma(4) + lgamma(3) + lgamma(3) + lgamma(3)   # 3!·2!·2!·2! özdeş-katsayı yuva sıralamaları

def draw():
    cells = CELLS_ORI[rng.integers(4)]; logq = -log(4)
    used = set(); avail = Counter(CNT); pairs = []
    for content, npar in cells:
        for c in content:
            cand = [(n, 19*c - n) for n in range(1, 115) if n % 2 == npar and n not in used and avail[19*c - n] > 0]
            if not cand: return None
            wts = np.array([avail[a] for _, a in cand], dtype=float); tot = wts.sum()
            i = rng.choice(len(cand), p=wts / tot); n_, a_ = cand[i]
            logq += log(wts[i] / tot); used.add(n_); avail[a_] -= 1; pairs.append((n_, a_))
    return pairs, avail, used, logq

def log_L(pairs):
    u = Counter(a for _, a in pairs); return sum(lgamma(CNT[v]+1) - lgamma(CNT[v]-k+1) for v, k in u.items())

def evaluate(pairs, avail, used, n_shuffle, batch=400_000):
    rp = np.array(sorted(n for n in range(1, 115) if n not in used)); rc = np.array(list(avail.elements()), dtype=np.int64)
    fn = np.array([p[0] for p in pairs]); fa = np.array([p[1] for p in pairs]); ft = fn + fa
    fe_cnt = int((ft % 2 == 0).sum()); fe_sum = int(ft[ft % 2 == 0].sum()); f_fu = int(((fn - fa) % 19 == 0).sum()); f_sa = int(fa.sum())
    tot = acc = hA = hW = hAW = 0; done = 0
    while done < n_shuffle:
        b = min(batch, n_shuffle - done); done += b
        P = rng.permuted(np.tile(rc, (b, 1)), axis=1); T = rp[None, :] + P
        keep = ~((T % 19 == 0).any(1)); tot += b; k = int(keep.sum()); acc += k
        if k == 0: continue
        Pk, Tk = P[keep], T[keep]; te = (Tk % 2 == 0)
        A_ = (te.sum(1) + fe_cnt == 57) & ((Tk * te).sum(1) + fe_sum == 6234)
        fu = ((rp[None, :] - Pk) % 19 == 0)
        W_ = (fu.sum(1) + f_fu == 7) & (12 + fu.sum(1) == 19) & ((f_sa + (Pk * fu).sum(1)) % 19 == 0)
        hA += int(A_.sum()); hW += int(W_.sum()); hAW += int((A_ & W_).sum())
    return tot, acc, hA, hW, hAW

S_ASSIGN = int(sys.argv[1]) if len(sys.argv) > 1 else 1200
SHUF = int(sys.argv[2]) if len(sys.argv) > 2 else 2_000_000
rows = []; attempts = 0
while len(rows) < S_ASSIGN:
    attempts += 1
    d = draw()
    if d is None: continue
    pairs, avail, used, logq = d
    tot, acc, hA, hW, hAW = evaluate(pairs, avail, used, SHUF)
    p_avoid = acc / tot
    logw = log_L(pairs) + (log(p_avoid) if p_avoid > 0 else -1e9) - LOG_M - logq
    rows.append((logw, acc, hA, hW, hAW))
    if len(rows) % 50 == 0:
        print(f"atama {len(rows)}/{S_ASSIGN} | kabul {sum(r[1] for r in rows):,} | A {sum(r[2] for r in rows)} W {sum(r[3] for r in rows)} A∧W {sum(r[4] for r in rows)}", flush=True)

Z = S_ASSIGN / attempts
lw = np.array([r[0] for r in rows]); mx = lw.max()
pG = Z * exp(mx) * np.mean(np.exp(lw - mx)) * exp(-LOG_REST)
w = np.exp(lw - mx); w /= w.sum(); ess = 1 / np.sum(w**2)
acc = np.array([r[1] for r in rows], float); hA = np.array([r[2] for r in rows], float)
hW = np.array([r[3] for r in rows], float); hAW = np.array([r[4] for r in rows], float)
m = acc > 0
def wrate(h): return float(np.sum(w[m] * h[m] / acc[m]) / np.sum(w[m]))
def pooled(h): return float(h.sum() / acc.sum())
pA, pW, pAW = wrate(hA), wrate(hW), wrate(hAW)
# atama-düzeyi ağırlıklı bootstrap (atama seçimi + önem-ağırlık + Poisson gürültüsü birlikte)
B = 4000; idx = np.arange(len(rows)); boots = []
for _ in range(B):
    s = rng.choice(idx, size=len(idx), replace=True)
    ws = w[s]; a = acc[s]; h = hAW[s]; mm = a > 0
    boots.append(float(np.sum(ws[mm] * h[mm] / a[mm]) / np.sum(ws[mm])))
lo, hi = np.percentile(boots, [2.5, 97.5])
PG_GPU = 7.14e-10
print(f"\n=== ÇAPA: P(G) SIS = {pG:.3e} vs GPU 7.14e-10 → oran {pG/PG_GPU:.2f} | Z={Z:.3f} | ESS={ess:.0f}/{S_ASSIGN}")
print(f"=== toplam kabul (G-örneği) = {int(acc.sum()):,} | isabet A={int(hA.sum())} W={int(hW.sum())} A∧W={int(hAW.sum())}")
print(f"=== P(A|G): ağırlıklı {pA:.3e} | havuz {pooled(hA):.3e}")
print(f"=== P(W|G): ağırlıklı {pW:.3e} | havuz {pooled(hW):.3e}   (GPU çapası 62/17204 = 3.60e-3 [2.8, 4.6] — İKİNCİ ÇAPA)")
print(f"=== P(A∧W|G): ağırlıklı {pAW:.3e} | havuz {pooled(hAW):.3e}  | bootstrap %95 GA [{lo:.2e}, {hi:.2e}]")
print(f"=== κ = P(A∧W|G)/[P(A|G)·P(W|G)] = {pAW/(pA*pW):.2f}")
print(f"=== P(A∧G∧W) = 7.14e-10 × P(A∧W|G) = {PG_GPU*pAW:.2e}   GA [{PG_GPU*lo:.1e}, {PG_GPU*hi:.1e}]   (kesif18 eski: 1.74e-15 [1.1, 2.6])")
