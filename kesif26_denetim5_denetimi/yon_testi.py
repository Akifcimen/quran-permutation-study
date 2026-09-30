#!/usr/bin/env python3
# Yön testi: uzundan kısaya (gerçek yön) ve kısadan uzuna sıralı kitaplarda katmanlar.
# Uzundan kısaya örnekler: kesif16/kesif25 sıfırı (S = Σ n·a ≤ S_obs, çok zincirli Metropolis).
# Kısadan uzuna örnekler: her örneğin konum sırası tersine çevrilir (aynı sıralılık derecesi, ters yön).
import json, numpy as np
N = np.arange(1, 115, dtype=np.int64)
K0 = np.array([s['numberOfAyahs'] for s in json.load(open('../quran_meta.json'))['data']['surahs']['references']], dtype=np.int64)
A = np.where(N == 9, 127, K0); TOTAL = int(A.sum()); S_obs = int((N * A).sum())
rng = np.random.default_rng(1150)

def katmanlar(P):
    T = N[None, :] + P; te = (T % 2 == 0)
    A_ = (te.sum(1) == 57) & ((T * te).sum(1) == TOTAL)
    F = (T % 19 == 0); C = np.where(F, T // 19, 0); k = F.sum(1)
    B_ = (k == 12) & ((C * F).sum(1) == 76)
    FE = F & te; C_ = B_ & (FE.sum(1) == 6) & ((C * FE).sum(1) == 38)
    bant = ((T >= 95) & (T <= 171)).mean(1)          # t'lerin 5·19…9·19 aralığındaki payı
    return A_, B_, C_, bant, T.std(1)

CH, BURN, STEPS, THIN = 10_000, 30_000, 20_000, 100
P = np.tile(np.sort(A)[::-1], (CH, 1)); S = P @ N; rows = np.arange(CH)
say = {"uzundan kısaya": np.zeros(3), "kısadan uzuna": np.zeros(3)}; bantlar = {k: [] for k in say}; sdler = {k: [] for k in say}; n = 0
for step in range(BURN + STEPS):
    i = rng.integers(0, 114, CH); j = rng.integers(0, 114, CH)
    Pi = P[rows, i]; Pj = P[rows, j]; dS = (N[i] - N[j]) * (Pj - Pi)
    ok = (S + dS) <= S_obs
    P[rows[ok], i[ok]] = Pj[ok]; P[rows[ok], j[ok]] = Pi[ok]; S = S + dS * ok
    if step >= BURN and (step - BURN) % THIN == 0:
        for ad, X in (("uzundan kısaya", P), ("kısadan uzuna", P[:, ::-1])):
            a_, b_, c_, bt, sd = katmanlar(X)
            say[ad] += [a_.sum(), b_.sum(), c_.sum()]; bantlar[ad].append(bt.mean()); sdler[ad].append(sd.mean())
        n += CH
print(f"örnek sayısı: {n:,} (her yön için)")
print(f"{'yön':16s} {'t std':>6s} {'95–171 payı':>11s} {'A terazi':>10s} {'B aile':>10s} {'C iç terazi':>11s}")
ref = {"A": 3.19e-4, "B": 3.21e-4, "C": 1.64e-5}
for ad in say:
    a, b, c = say[ad] / n
    print(f"{ad:16s} {np.mean(sdler[ad]):6.1f} {np.mean(bantlar[ad]):11.2f} {a:10.2e} {b:10.2e} {c:11.2e}")
print(f"{'tekdüze (ref.)':16s} {62.4:6.1f} {0.43:11.2f} {ref['A']:10.2e} {ref['B']:10.2e} {ref['C']:11.2e}")
# gerçek kitap ve nüzul sırası
t = N + A; print(f"\ngerçek kitap: t std {t.std():.1f}, 95–171 payı {((t>=95)&(t<=171)).mean():.2f}")
ch = json.load(open('../quran_chapters_qcom.json'))['chapters']
rev = np.array([c['revelation_order'] for c in ch]); a_nuz = A[np.argsort(rev)]
tn = N + a_nuz
def kendall(x, y):
    c = d = 0
    for p in range(len(x)):
        s = np.sign((x[p+1:] - x[p]) * (y[p+1:] - y[p])); c += (s > 0).sum(); d += (s < 0).sum()
    return (c - d) / (len(x) * (len(x) - 1) / 2)
print(f"nüzul sırası: konum–ayet Kendall τ = {kendall(N, a_nuz):+.2f} (pozitif = kısadan uzuna) | t std {tn.std():.1f}, 95–171 payı {((tn>=95)&(tn<=171)).mean():.2f}")
print(f"mushaf sırası: konum–ayet Kendall τ = {kendall(N, A):+.2f}")
