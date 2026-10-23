#!/usr/bin/env python3
"""Ö2 birleştirme (ONKAYIT §5): P(A∩G∩W | sıfır) = P(G | sıfır) × ort_iskelet P(A∧W | iskelet).
P(G | sıfır) = (havuzdaki G-kitap sayısı / havuz N) × (havuz N / toplam deneme)  [= G isabeti / deneme]; Poisson.
İskelet ortalaması için iskeletler üzerinde bootstrap (4.000).
Kullanım: python3 -I src/cekirdek_analiz.py <cekirdek_cikti.txt> <say_dizini_ya_da_->  [--pG deger alt ust]"""
import sys, glob, re, math, numpy as np
fn = sys.argv[1]; sd = sys.argv[2]
rows = [list(map(float, l.split()[3:8])) for l in open(fn) if not l.startswith('#')]   # sayim A W AW kabul
a = np.array(rows) if rows else np.zeros((0, 5))
ns, nA, nW, nAW = a[:, 0], a[:, 1], a[:, 2], a[:, 3]
r = nAW / ns; rA = nA / ns; rW = nW / ns
rng = np.random.default_rng(5)
B = 4000; idx = rng.integers(0, len(r), (B, len(r)))
boot = r[idx].mean(1)
print(f"iskelet {len(r)}; toplam A∧W isabeti {int(nAW.sum())} / {int(ns.sum()):,} adım; P(A|G)={rA.mean():.4e}  P(W|G)={rW.mean():.4e}  P(A∧W|G)={r.mean():.4e} "
      f"[%95 {np.percentile(boot,2.5):.3e}, {np.percentile(boot,97.5):.3e}]  κ = P(A∧W)/(P(A)P(W)) = {r.mean()/(rA.mean()*rW.mean()):.2f}")
if '--pG' in sys.argv:
    i = sys.argv.index('--pG'); pG, lo, hi = map(float, sys.argv[i+1:i+4])
    print(f"P(G) verilen {pG:.4e} [{lo:.3e}, {hi:.3e}]")
else:
    G = 0; T = 0
    for f in glob.glob(f'{sd}/*.say'):
        s = open(f).read(); T += int(re.search(r'deneme (\d+)', s).group(1)); G += int(re.search(r' G (\d+)\s*$', s.strip().split(' kosul_gecen')[0]).group(1))
    pG = G / T
    lo = (G - 1.96*math.sqrt(G)) / T; hi = (G + 1.96*math.sqrt(G)) / T
    print(f"P(G | sıfır) = {G} / {T:,} = {pG:.4e} [%95 ≈ {lo:.3e}, {hi:.3e}]")
# birleşik: P(G) için log-normal yaklaşık çekiliş × bootstrap
sd_log = (math.log(hi) - math.log(lo)) / (2 * 1.96) if lo > 0 else 0.0
pg = np.exp(np.log(pG) + sd_log * rng.standard_normal(B))
core = pg * boot
print(f"P(A∩G∩W | sıfır) = {pG*r.mean():.3e}  [%95 {np.percentile(core,2.5):.2e}, {np.percentile(core,97.5):.2e}]")
