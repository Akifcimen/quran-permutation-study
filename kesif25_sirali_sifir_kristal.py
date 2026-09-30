#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""KESIF-25: Beşinci denetimin ana iddiasının BAĞIMSIZ sınaması — kesif16'nın kendi koşullu sıfırı
(S = Σ n·a ≤ S_obs, transpozisyon-Metropolis) kristalin DERİN katmanlarına (E kefe torbaları, G tam
kristal) uygulanınca oran ×~100 çıkıyor mu? Saldırganın kodu değil, kesif16'nın kendi MCMC'si; motor
yüklemleri kesif4_motor.c ile birebir (127 kodeksi). Çok-zincir (20k), yakma 30k, seyreltme 100."""
import json, sys
import numpy as np
N = np.arange(1, 115, dtype=np.int64)
K0 = np.array([s['numberOfAyahs'] for s in json.load(open('quran_meta.json'))['data']['surahs']['references']], dtype=np.int64)
A = np.where(N == 9, 127, K0); TOTAL = int(A.sum()); S_obs = int((N * A).sum())
rng = np.random.default_rng(2525)

def layers(P):
    """P:(B,114) → A,B,C,D,E,G,W bool dizileri (motor tanımları)."""
    T = N[None, :] + P; te = (T % 2 == 0)
    A_ = (te.sum(1) == 57) & ((T * te).sum(1) == TOTAL)
    F = (T % 19 == 0); C = np.where(F, T // 19, 0); k = F.sum(1)
    B_ = (k == 12) & ((C * F).sum(1) == 76)
    FE = F & te; C_ = B_ & (FE.sum(1) == 6) & ((C * FE).sum(1) == 38)
    NE = (N % 2 == 0)[None, :]
    cnt = [ (F & te & ~NE).sum(1), (F & te & NE).sum(1), (F & ~te & ~NE).sum(1), (F & ~te & NE).sum(1) ]  # idx: te?0:2 + n odd?1:0 → (te,n-odd)=0,(te,n-even)=1? motor: idx=(te?0:2)+(n&1?1:0)
    # motor: idx0 = te & n-even, idx1 = te & n-odd, idx2 = odd-t & n-even, idx3 = odd-t & n-odd
    m0 = F & te & NE; m1 = F & te & ~NE; m2 = F & ~te & NE; m3 = F & ~te & ~NE
    s0 = (C * m0).sum(1); s1 = (C * m1).sum(1); s2 = (C * m2).sum(1); s3 = (C * m3).sum(1)
    D_ = C_ & (m0.sum(1) == 3) & (m1.sum(1) == 3) & (m2.sum(1) == 3) & (m3.sum(1) == 3) & (s2 == 19) & (s3 == 19) & (((s0 == 18) & (s1 == 20)) | ((s0 == 20) & (s1 == 18)))
    # E: hist even side {6:5, 8:1}, odd side {5:3, 7:2, 9:1}
    def hist(mask, v): return ((C == v) & mask).sum(1)
    E_ = (hist(FE, 6) == 5) & (hist(FE, 8) == 1) & (hist(F & ~te, 5) == 3) & (hist(F & ~te, 7) == 2) & (hist(F & ~te, 9) == 1)
    # G: D & E & cell identities: even pair cells {6,6,6}&{6,6,8} (sum,sumsq)=(18,108),(20,136); odd {5,5,9}=(19,131),{5,7,7}=(19,123)
    q0 = (C * C * m0).sum(1); q1 = (C * C * m1).sum(1); q2 = (C * C * m2).sum(1); q3 = (C * C * m3).sum(1)
    evenpair = ((s0 == 18) & (q0 == 108) & (s1 == 20) & (q1 == 136)) | ((s0 == 20) & (q0 == 136) & (s1 == 18) & (q1 == 108))
    oddpair = ((s2 == 19) & (q2 == 131) & (s3 == 19) & (q3 == 123)) | ((s2 == 19) & (q2 == 123) & (s3 == 19) & (q3 == 131))
    G_ = D_ & E_ & evenpair & oddpair
    U = N[None, :] - P; FU = (U % 19 == 0); uni = F | FU
    W_ = (FU.sum(1) == 7) & (uni.sum(1) == 19) & ((P * uni).sum(1) % 19 == 0)
    return A_, B_, C_, D_, E_, G_, W_

names = ["A", "B", "C", "D", "E", "G", "W"]
assert all(x[0] for x in layers(A[None, :])), "gerçek kitap tüm katmanları sağlamalı"

# --- koşulsuz referans (10M) ---
ref = np.zeros(7, dtype=np.int64); NREF = 10_000_000
for _ in range(NREF // 500_000):
    P = rng.permuted(np.tile(A, (500_000, 1)), axis=1)
    ref += np.array([int(x.sum()) for x in layers(P)])
print("koşulsuz (10M):", {n: f"{c/NREF:.2e}" for n, c in zip(names, ref)}, flush=True)

# --- koşullu MCMC: S ≤ S_obs, çok zincir ---
CH, BURN, STEPS, THIN = int(sys.argv[1]) if len(sys.argv) > 1 else 20_000, 30_000, int(sys.argv[2]) if len(sys.argv) > 2 else 200_000, 100
P = np.tile(np.sort(A)[::-1], (CH, 1)); S = P @ N; rows = np.arange(CH)
hit = np.zeros(7, dtype=np.int64); nsamp = 0
for step in range(BURN + STEPS):
    i = rng.integers(0, 114, CH); j = rng.integers(0, 114, CH)
    Pi = P[rows, i]; Pj = P[rows, j]; dS = (N[i] - N[j]) * (Pj - Pi)
    ok = (S + dS) <= S_obs
    P[rows[ok], i[ok]] = Pj[ok]; P[rows[ok], j[ok]] = Pi[ok]; S = S + dS * ok
    if step >= BURN and (step - BURN) % THIN == 0:
        hit += np.array([int(x.sum()) for x in layers(P)]); nsamp += CH
        if ((step - BURN) // THIN) % 400 == 0 and step > BURN:
            print(f"  koşullu örnek {nsamp:,}: " + " ".join(f"{n}={c}" for n, c in zip(names, hit)), flush=True)
assert (S <= S_obs).all()
print(f"\nkoşullu S≤S_obs ({nsamp:,} örnek):")
for n, c, r in zip(names, hit, ref):
    pc = c / nsamp; pr = r / NREF
    print(f"  {n}: {pc:.2e} ({c} isabet) | koşulsuz {pr:.2e} | oran ×{pc/pr if pr>0 else float('nan'):.1f}")
