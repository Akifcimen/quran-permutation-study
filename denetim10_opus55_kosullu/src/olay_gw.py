#!/usr/bin/env python3
"""Keşif (ön-kayıt dışı): olay düzeyinde P(G ∧ W | O) = P(G | O) × ort_iskelet P(W | G-iskeleti).
P(G | O) koşullu havuzdan (G isabeti / O-kitap; Poisson), P(W | iskelet) çekirdek çıktısından (iskeletler üzerinde bootstrap 4.000).
Kullanım: python3 -I src/olay_gw.py <cekirdek_cikti.txt> <G_isabeti> <kosul_kitap_sayisi> [kosul_adi, varsayılan O]"""
import sys, math, numpy as np
fn, G, N = sys.argv[1], int(sys.argv[2]), int(sys.argv[3])
K = sys.argv[4] if len(sys.argv) > 4 else 'O'
a = np.array([list(map(float, l.split()[3:7])) for l in open(fn) if not l.startswith('#')])
ns, nW = a[:, 0], a[:, 2]
rW = nW / ns
rng = np.random.default_rng(5)
B = 4000
bootW = rW[rng.integers(0, len(rW), (B, len(rW)))].mean(1)
pG = G / N
pg = rng.poisson(G, B) / N
gw = pg * bootW
print(f"P(G|{K}) = {G}/{N:,} = {pG:.3e} [%95 {(G-1.96*math.sqrt(G))/N:.2e}, {(G+1.96*math.sqrt(G))/N:.2e}]")
print(f"P(W|G) = {rW.mean():.3e} [%95 {np.percentile(bootW,2.5):.2e}, {np.percentile(bootW,97.5):.2e}]  ({len(rW)} iskelet, W isabeti {int(nW.sum()):,} / {int(ns.sum()):,} adım)")
print(f"P(G∧W|{K}) = {pG*rW.mean():.3e} [%95 {np.percentile(gw,2.5):.2e}, {np.percentile(gw,97.5):.2e}]")
