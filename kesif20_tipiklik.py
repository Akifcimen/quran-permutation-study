#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""KESIF-20 (dış saldırı): Kristal yapılandırmasının TİPİKLİĞİ.

Soru: Kristalin katmanlı p-değerleri (B→C→D→G) "gözlenen yapılandırmanın olasılığı"dır.
Ama 12 üyeli rastgele bir 19-ailesinin HERHANGİ bir yapılandırması da düşük olasılıklıdır.
Gerçek kitabın yapılandırması, aynı ayrıntı düzeyinde rastgele yapılandırmalar arasında
ne kadar "nadir"? Kütle-ağırlıklı yüzdelik: rastgele bir 12'lik aile, kendi yapılandırmasının
olasılığı gerçek kitabınkinden KÜÇÜK olan bir yapılandırmaya ne sıklıkla düşer?

Düzeyler (motorun sayaçlarıyla aynı taneciklik):
  B: Σc                       (gerçek: 76)
  C: (Σc, k_e, Σc_e)          (gerçek: 76, 6, 38)
  D: C + hücre (sayı,toplam)  (gerçek: çift-t hücreleri {(3,18),(3,20)}, tek-t {(3,19),(3,19)})
  G: D + torbalar/hücre içerikleri (gerçek: {666},{668} | {559},{577})
Yöntem: k=12 koşullu örnekleme (ret), her düzeyde yapılandırma sıklık tablosu; gerçek
yapılandırmanın sıklığı ile kütle-ağırlıklı dağılımın karşılaştırılması.
"""
import json, sys, time
from collections import Counter
import numpy as np

N = np.arange(1, 115, dtype=np.int64)
K0 = np.array([s['numberOfAyahs'] for s in json.load(open('quran_meta.json'))['data']['surahs']['references']], dtype=np.int64)
A = np.where(N == 9, 127, K0)
n_target = int(sys.argv[1]) if len(sys.argv) > 1 else 1_000_000
rng = np.random.default_rng(2026)

def configs(P):
    """P: (B,114) satırlar, hepsi k=12. Döner B/C/D/G düzeyi anahtar listeleri."""
    T = N[None, :] + P
    F = (T % 19 == 0)
    C = T // 19
    TE = (T % 2 == 0); NE = (N % 2 == 0)
    out = [[], [], [], []]
    for r in range(P.shape[0]):
        f = F[r]; c = C[r][f]; te = TE[r][f]; ne = NE[f]
        sc = int(c.sum()); ke = int(te.sum()); sce = int(c[te].sum())
        cells = []
        for pe in (True, False):
            pair = []
            for pn in (True, False):
                mm = (te == pe) & (ne == pn)
                pair.append((int(mm.sum()), int(c[mm].sum()), tuple(sorted(c[mm].tolist()))))
            cells.append(tuple(sorted(pair)))
        keyB = sc
        keyC = (sc, ke, sce)
        keyD = (keyC, tuple(tuple((x[0], x[1]) for x in pr) for pr in cells))
        keyG = (keyC, tuple(tuple(x[2] for x in pr) for pr in cells))
        out[0].append(keyB); out[1].append(keyC); out[2].append(keyD); out[3].append(keyG)
    return out

realB, realC, realD, realG = [x[0] for x in configs(A[None, :])]
print("gerçek anahtarlar:", realB, realC, realD, realG)

cnt = [Counter(), Counter(), Counter(), Counter()]
tot_perm = 0; got = 0; t0 = time.time(); CH = 200_000
while got < n_target:
    P = rng.permuted(np.tile(A, (CH, 1)), axis=1)
    T = N[None, :] + P
    k = (T % 19 == 0).sum(1)
    sel = P[k == 12]
    tot_perm += CH; got += sel.shape[0]
    for lvl, keys in enumerate(configs(sel)):
        cnt[lvl].update(keys)
    if tot_perm % 2_000_000 == 0:
        print(f"  {tot_perm:,} perm → {got:,} k=12 örneği  [{time.time()-t0:.0f}s]", flush=True)

pk12 = got / tot_perm
print(f"\nP(k=12) = {pk12:.4e}   ({got:,} örnek / {tot_perm:,} perm)")

def report(name, counter, realkey, level_p_report):
    n = sum(counter.values()); c_real = counter.get(realkey, 0)
    # kütle-ağırlıklı: rastgele bir örnek, sıklığı c_real'den küçük/eşit/büyük bir anahtara düşme oranı
    lo = sum(c for c in counter.values() if c < c_real) / n
    eq = sum(c for c in counter.values() if c == c_real) / n
    hi = sum(c for c in counter.values() if c > c_real) / n
    singles = sum(1 for c in counter.values() if c == 1)
    p_real_cond = c_real / n
    # kütle-ağırlıklı medyan anahtar sıklığı
    freqs = np.array(sorted(counter.values()))
    cum = np.cumsum(freqs) / n
    med = freqs[np.searchsorted(cum, 0.5)]
    # plug-in entropi (nat) → tipik yapılandırma olasılığı ~ e^-H (alt sınır: görülmemiş kütle yok sayıldı)
    p = freqs / n; H = float(-(p * np.log(p)).sum())
    print(f"\n[{name}] farklı yapılandırma: {len(counter):,} | tekil (1 kez görülen): {singles:,} | görülmemiş kütle (Good-Turing) ≈ {singles/n:.3f}")
    print(f"   gerçek yapılandırma: {c_real:,} kez → P(·|k=12) = {p_real_cond:.3e} → P = {p_real_cond*pk12:.3e}  (motor: {level_p_report})")
    print(f"   kütle-ağırlıklı medyan yapılandırma: {med} kez → P(·|k=12) = {med/n:.3e} → P = {med/n*pk12:.3e}")
    print(f"   rastgele bir 12'lik ailenin kendi yapılandırması gerçekten DAHA NADİR: {lo:.1%} | eşit: {eq:.1%} | DAHA SIK: {hi:.1%}")
    print(f"   plug-in entropi H = {H:.2f} nat → e^-H = {np.exp(-H):.2e} (koşullu; gerçek {p_real_cond:.2e})")

report("B düzeyi: Σc", cnt[0], realB, "3.16e-4 (B)")
report("C düzeyi: (Σc,k_e,Σc_e)", cnt[1], realC, "1.85e-5 (C)")
report("D düzeyi: + hücre sayı/toplam", cnt[2], realD, "1.39e-7 (D)")
report("G düzeyi: + hücre içerikleri", cnt[3], realG, "7.14e-10 (G)")
