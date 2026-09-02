#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""KESIF-21: sınıf-düzeyi kristal G* (bant konumu serbest) ile çekirdek. Ön-kayıt: kesif21_sinif_kristal_prereg.md
kesif18 makinesinin MUTLAK-olasılık sürümü; çapa: σ₀ için P(G)=7.14e-10 (GPU)."""
import json, sys
from collections import Counter
from itertools import combinations_with_replacement as cwr
from math import lgamma, log, exp
import numpy as np

rng = np.random.default_rng(2121)
N = np.arange(1, 115)
K0 = np.array([s['numberOfAyahs'] for s in json.load(open('quran_meta.json'))['data']['surahs']['references']], dtype=np.int64)
A_ALL = np.where(N == 9, 127, K0)
CNT = Counter(A_ALL.tolist())
LOG_REST = sum(log(x) for x in range(103, 115))     # log(114!/102!)

def triples(par, s):
    vals = [v for v in range(1, 41) if v % 2 == par]
    return sorted({tuple(sorted(t)) for t in cwr(vals, 3) if sum(t) == s})
E18, E20, O19 = triples(0, 18), triples(0, 20), triples(1, 19)
STRUCTS = set()
for a in E18:
    for b in E20:
        for c in O19:
            for d in O19:
                if c > d: continue
                vals = sorted(a + b + c + d)
                if set(range(vals[0], vals[-1] + 1)) <= set(vals):
                    STRUCTS.add((a, b, tuple(sorted((c, d)))))
STRUCTS = sorted(STRUCTS)
REAL = ((6, 6, 6), (6, 6, 8), ((5, 5, 9), (5, 7, 7)))
assert REAL in STRUCTS
print(f"G* yapı sayısı: {len(STRUCTS)}", flush=True)

def orientations(sig):
    e1, e2, (o1, o2) = sig
    ev = [(e1, e2), (e2, e1)] if e1 != e2 else [(e1, e2)]
    od = [(o1, o2), (o2, o1)] if o1 != o2 else [(o1, o2)]
    # (içerik, n%2): ilk çift hücre n-tek(1), ikinci n-çift(0); tek hücreler: ilk n-çift, ikinci n-tek (gerçek yönle uyumlu sıra)
    return [[(a, 1), (b, 0), (c, 0), (d, 1)] for (a, b) in ev for (c, d) in od]

def log_M(sig):
    m = 0.0
    for cell in (sig[0], sig[1], sig[2][0], sig[2][1]):
        for v, k in Counter(cell).items(): m += lgamma(k + 1)
    return m

def draw(sig, oris):
    """Bir SIS çekilişi. Döner (pairs, avail, used_n, logq) ya da None (çıkmaz)."""
    cells = oris[rng.integers(len(oris))]
    logq = -log(len(oris))
    used = set(); avail = Counter(CNT); pairs = []
    for content, npar in cells:
        for c in content:
            cand = [(n, 19 * c - n) for n in range(1, 115) if n % 2 == npar and n not in used and avail[19 * c - n] > 0]
            if not cand: return None
            n_, a_ = cand[rng.integers(len(cand))]
            logq -= log(len(cand)); used.add(n_); avail[a_] -= 1; pairs.append((n_, a_))
    return pairs, avail, used, logq

def log_L(pairs):
    u = Counter(a for _, a in pairs); s = 0.0
    for v, k in u.items(): s += lgamma(CNT[v] + 1) - lgamma(CNT[v] - k + 1)
    return s

def evaluate(pairs, avail, used, n_shuffle, batch=150_000):
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

S_PER, SHUF = (int(sys.argv[1]) if len(sys.argv) > 1 else 200), (int(sys.argv[2]) if len(sys.argv) > 2 else 150_000)
results = []
for si, sig in enumerate(STRUCTS):
    oris = orientations(sig); lM = log_M(sig)
    attempts = 0; rows = []
    while len(rows) < S_PER:
        attempts += 1
        d = draw(sig, oris)
        if d is None: continue
        pairs, avail, used, logq = d
        tot, acc, hA, hW, hAW = evaluate(pairs, avail, used, SHUF)
        p_avoid = acc / tot
        logw = log_L(pairs) + (log(p_avoid) if p_avoid > 0 else -1e9) - lM - logq   # f/(M q)
        rows.append((logw, acc, hA, hW, hAW))
    Z = S_PER / attempts
    lw = np.array([r[0] for r in rows]); mx = lw.max()
    est = Z * exp(mx) * np.mean(np.exp(lw - mx))            # Σ_S L·P_avoid
    pG = est * exp(-LOG_REST)
    w = np.exp(lw - mx); w /= w.sum(); ess = 1 / np.sum(w ** 2)
    acc = np.array([r[1] for r in rows], float); hA = np.array([r[2] for r in rows], float); hW = np.array([r[3] for r in rows], float)
    m = acc > 0
    pA = float(np.sum(w[m] * hA[m] / acc[m]) / np.sum(w[m])); pW = float(np.sum(w[m] * hW[m] / acc[m]) / np.sum(w[m]))
    nAW = int(sum(r[4] for r in rows))
    tag = " ← GERÇEK (çapa 7.14e-10)" if sig == REAL else ""
    print(f"[{si+1:2d}/{len(STRUCTS)}] {sig[0]}{sig[1]}|{sig[2][0]}{sig[2][1]}  P(G_σ)={pG:.3e}  Z={Z:.2f} ESS={ess:.0f}  P(A|σ)={pA:.2e} P(W|σ)={pW:.2e}  A∧W={nAW}{tag}", flush=True)
    results.append((sig, pG, pA, pW, ess, nAW, int(acc.sum())))

PG_GPU = 7.14e-10
pG0 = [r[1] for r in results if r[0] == REAL][0]
PGstar = sum(r[1] for r in results)
wsig = np.array([r[1] for r in results]) / PGstar
pA_star = float(np.sum(wsig * np.array([r[2] for r in results]))); pW_star = float(np.sum(wsig * np.array([r[3] for r in results])))
kappa = 1.6
print(f"\n=== ÇAPA: P(G_σ₀) SIS = {pG0:.3e}  vs GPU 7.14e-10 → oran {pG0/PG_GPU:.2f}")
print(f"=== P(G*) = Σ_σ P(G_σ) = {PGstar:.3e}   (P(G*)/P(G_σ₀) = {PGstar/pG0:.1f}; en ağır 3 yapı: " +
      ", ".join(f"{r[0][0]}{r[0][1]}|{r[0][2][0]}{r[0][2][1]}={r[1]/PGstar:.0%}" for r in sorted(results, key=lambda r: -r[1])[:3]) + ")")
print(f"=== P(A|G*) = {pA_star:.3e}  P(W|G*) = {pW_star:.3e}  (σ₀: P(A|G)=3.39e-4, P(W|G)=4.47e-3 kesif18)")
core_star = PGstar * pA_star * pW_star * kappa
print(f"=== ÇEKİRDEK* = P(G*)·P(A|G*)·P(W|G*)·κ(1.6) = {core_star:.2e}   [GPU-çapalı: {PGstar/pG0*PG_GPU*pA_star*pW_star*kappa:.2e}]")
print(f"    toplam doğrudan A∧W isabeti (tüm yapılar): {sum(r[5] for r in results)} / {sum(r[6] for r in results):,} G*-örneği")
