#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""KESIF-16: Boy-sırası itirazı (Schick) — ön-kayıt: kesif16_boy_sirasi_prereg.md
A1: saf-sıralı kitaplar bataryada. A2: koşulsuz MC, katman oranı × S-dilimi. A3: koşullu MCMC (S ≤ S_obs)."""
import json
import numpy as np

N = np.arange(1, 115)
K = np.array([s['numberOfAyahs'] for s in json.load(open('quran_meta.json'))['data']['surahs']['references']], dtype=np.int64)
TOTAL = int(K.sum())
S_obs = int((N * K).sum())

def battery_rows(A):
    """A: (B,114) permütasyon matrisi → dört katman bool dizileri"""
    t = N[None, :] + A
    te = (t % 2 == 0)
    ter = (te.sum(1) == 57) & ((t * te).sum(1) == TOTAL)
    fam = (t % 19 == 0)
    B_ = (fam.sum(1) == 12) & ((t * fam).sum(1) == 1444)
    C_ = B_ & ((fam & te).sum(1) == 6) & ((t * (fam & te)).sum(1) == 722)
    u = N[None, :] - A
    fu = (u % 19 == 0); uni = fam | fu
    W_ = (fu.sum(1) == 7) & (uni.sum(1) == 19) & ((A * uni).sum(1) % 19 == 0)
    return ter, B_, C_, W_

def show(tag, A):
    ter, B_, C_, W_ = battery_rows(A[None, :])
    print(f" {tag}: terazi {'✓' if ter[0] else '✗'} | aile {'✓' if B_[0] else '✗'} | iç {'✓' if C_[0] else '✗'} | İkiEl {'✓' if W_[0] else '✗'}"
          f" | S={int((N*A).sum())}")

print(f"Gerçek veri: S_obs={S_obs} | toplam={TOTAL}")
show("GERÇEK KİTAP     ", K)

# ---- A1: saf boy-sırası kitapları ----
print("\n=== A1: saf-sıralı kitaplar (itirazın limit hâli) ===")
show("TAM AZALAN (boy) ", np.sort(K)[::-1])
show("TAM ARTAN        ", np.sort(K))

# ---- A2: koşulsuz MC — S dilimlerine göre katman oranları ----
print("\n=== A2: koşulsuz MC (10^7), katman oranı × sıralılık dilimi ===")
rng = np.random.default_rng(42)
S_all, hits = [], {k: [] for k in ('ter', 'B', 'W')}
for _ in range(10):
    A = rng.permuted(np.tile(K, (1_000_000, 1)), axis=1)
    ter, B_, C_, W_ = battery_rows(A)
    S = A @ N
    S_all.append(S); hits['ter'].append(ter); hits['B'].append(B_); hits['W'].append(W_)
S_all = np.concatenate(S_all)
ter = np.concatenate(hits['ter']); B_ = np.concatenate(hits['B']); W_ = np.concatenate(hits['W'])
mu, sd = S_all.mean(), S_all.std()
print(f" S dağılımı: ort={mu:.0f} std={sd:.0f} → S_obs z-skoru = {(S_obs-mu)/sd:+.1f}"
      f"  (İ1: dizilişin boy-sıralılığı gerçekten uç — kabul)")
print(f" koşulsuz oranlar: terazi {ter.mean():.2e} | aile {B_.mean():.2e} | İkiEl {W_.mean():.2e}")
q = np.quantile(S_all, np.linspace(0, 1, 11))
print(" S-dilimi (1=en sıralı) → katman oranları:")
for d in range(10):
    m = (S_all >= q[d]) & (S_all < q[d+1] + (d == 9))
    print(f"  dilim {d+1:2d}: terazi {ter[m].mean():.2e} | aile {B_[m].mean():.2e} | İkiEl {W_[m].mean():.2e} (n={m.sum()})")

# ---- A3: koşullu MCMC — S ≤ S_obs üzerinde tekdüze ----
print("\n=== A3: koşullu MCMC (S ≤ S_obs; 20k zincir) ===")
CH, BURN, STEPS, THIN = 20_000, 30_000, 60_000, 150
A = np.tile(np.sort(K)[::-1], (CH, 1))          # başlangıç: tam-azalan (S minimum, küme içinde)
S = A @ N
rows = np.arange(CH)
acc = 0
cond = {k: [] for k in ('ter', 'B', 'C', 'W')}
n_snap = 0
for step in range(BURN + STEPS):
    i = rng.integers(0, 114, CH); j = rng.integers(0, 114, CH)
    Ai = A[rows, i]; Aj = A[rows, j]
    dS = (N[i] - N[j]) * (Aj - Ai)
    ok = (S + dS) <= S_obs
    A[rows[ok], i[ok]] = Aj[ok]; A[rows[ok], j[ok]] = Ai[ok]
    S = S + dS * ok
    acc += ok.mean()
    if step >= BURN and (step - BURN) % THIN == 0:
        ter_c, B_c, C_c, W_c = battery_rows(A)
        cond['ter'].append(ter_c); cond['B'].append(B_c); cond['C'].append(C_c); cond['W'].append(W_c)
        n_snap += 1
assert (S <= S_obs).all() and ((A @ N) == S).all()
tot = n_snap * CH
print(f" kabul oranı {acc/(BURN+STEPS):.2f} | örnek: {n_snap} kesit × {CH} zincir = {tot:.0f}")
for k, ad, unc in (('ter', 'terazi', ter.mean()), ('B', 'aile', B_.mean()), ('W', 'İkiEl', W_.mean())):
    r = np.concatenate(cond[k]).mean()
    ratio = r / unc if unc > 0 else float('nan')
    print(f" P({ad} | S≤S_obs) = {r:.2e}  vs koşulsuz {unc:.2e}  → oran {ratio:.2f}")
print(f" P(iç terazi | koşullu) = {np.concatenate(cond['C']).mean():.2e}")
