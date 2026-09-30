import json, numpy as np
N = np.arange(1, 115, dtype=np.int64)
K0 = np.array([s['numberOfAyahs'] for s in json.load(open('../quran_meta.json'))['data']['surahs']['references']], dtype=np.int64)
A = np.where(N == 9, 127, K0); S_obs = int((N * A).sum()); rng = np.random.default_rng(3)
def ozet(P):
    S = P @ N; t = N[None, :] + P
    ters = (np.diff(P, axis=1) > 0).sum(1)                 # komşu çiftlerde ayet sayısının ARTTIĞI yer: sıranın bozulduğu yer
    return S, ters, t.std(1), ((t >= 95) & (t <= 171)).mean(1)
S0, ters0, sd0, b0 = ozet(A[None, :])
CH, BURN, STEPS, THIN = 4000, 30000, 20000, 200
P = np.tile(np.sort(A)[::-1], (CH, 1)); S = P @ N; rows = np.arange(CH); acc = {k: [] for k in ("S", "ters", "sd", "b")}
for step in range(BURN + STEPS):
    i = rng.integers(0, 114, CH); j = rng.integers(0, 114, CH)
    Pi = P[rows, i]; Pj = P[rows, j]; dS = (N[i] - N[j]) * (Pj - Pi); ok = (S + dS) <= S_obs
    P[rows[ok], i[ok]] = Pj[ok]; P[rows[ok], j[ok]] = Pi[ok]; S = S + dS * ok
    if step >= BURN and (step - BURN) % THIN == 0:
        s_, t_, d_, b_ = ozet(P); acc["S"] += s_.tolist(); acc["ters"] += t_.tolist(); acc["sd"] += d_.tolist(); acc["b"] += b_.tolist()
Sx = np.array(acc["S"]); tx = np.array(acc["ters"])
ref = []; 
for _ in range(20): ref += (rng.permuted(np.tile(A, (5000, 1)), axis=1) @ N).tolist()
ref = np.array(ref)
print(f"{'':34s} {'gerçek kitap':>12s} {'sıralı sıfır':>13s} {'tekdüze':>9s}")
print(f"{'sıralılık S (küçük = daha sıralı)':34s} {S0[0]:12,d} {np.median(Sx):13,.0f} {np.median(ref):9,.0f}")
print(f"{'S, gerçek kitaba göre %1 içinde':34s} {'':12s} {np.mean(Sx >= 0.99*S_obs):12.0%} {'':9s}")
print(f"{'sıranın bozulduğu komşu çift / 113':34s} {ters0[0]:12d} {np.median(tx):13.0f} {'~56':>9s}")
print(f"{'t yayılımı, std':34s} {sd0[0]:12.1f} {np.mean(acc['sd']):13.1f} {'62,4':>9s}")
print(f"{'t değerlerinin 95–171 payı':34s} {b0[0]:12.0%} {np.mean(acc['b']):13.0%} {'%43':>9s}")
