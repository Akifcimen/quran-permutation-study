#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""KESIF-15: Ayarcı Uzayı — insan-eli hipotezinin ölçümü (ön-kayıt: kesif15_ayarci_uzayi_prereg.md)

Uzay: her sûre için ayet sayısı ∈ {Kûfe, Medine(Warş/Kalûn)}; ayrışan k=50 sûre → 2^50 yapılandırma.
Puanlayıcı: kesif11 bataryası BİREBİR (terazi A1, aile B, iç terazi C, İki El W).
A1: tam-yapı (4/4) yapılandırmalarının KESİN sayısı (aktif küme tam sayımı × pasif küme DP'si).
A2: Kûfe köşesi Hamming halkaları r≤3.  A3: MC skor merdiveni (10^7, tohum 42).  A4: katman marjinalleri.
"""
import json, itertools
import numpy as np

N = np.arange(1, 115)
d = json.load(open('quran_meta.json'))
K = np.array([s['numberOfAyahs'] for s in d['data']['surahs']['references']], dtype=np.int64)
M = np.array(json.load(open('sayim_medine_warsh.json'))['ayetler'], dtype=np.int64)

# ---- kesif11 bataryası (birebir) ----
def battery(A):
    A = np.asarray(A, dtype=np.int64); t = N + A; u = N - A
    total = int(A.sum()); te = (t % 2 == 0)
    A1 = int(te.sum()) == 57 and int(t[te].sum()) == total
    fam = (t % 19 == 0)
    B_ = int(fam.sum()) == 12 and int(t[fam].sum()) == 1444
    C_ = B_ and int((fam & te).sum()) == 6 and int(t[fam & te].sum()) == 722
    fu = (u % 19 == 0); uni = fam | fu
    W_ = int(fu.sum()) == 7 and int(uni.sum()) == 19 and int(A[uni].sum()) % 19 == 0
    return (A1, B_, C_, W_)

def score(A):
    return sum(battery(A))

# ---- sûre-yerel istatistik katkısı: (e, D, f, F, g, G, h, j, m) ----
# e: t çift mi; D = (t if çift) − a  [ΣD = Σt(çift) − Σa; hedef 0]
# f/F: aile üyeliği/t'si; g/G: aile∧çift; h: u-ailesi; j: birleşim; m: birleşim a'sı (mod 19 hedefi)
def contrib(n, a):
    t = n + a
    e = 1 if t % 2 == 0 else 0
    D = (t if e else 0) - a
    f = 1 if t % 19 == 0 else 0
    F = t if f else 0
    g = 1 if (f and e) else 0
    G = t if g else 0
    h = 1 if (n - a) % 19 == 0 else 0
    j = 1 if (f or h) else 0
    m = a if j else 0
    return np.array([e, D, f, F, g, G, h, j, m], dtype=np.int64)

TARGETS = dict(e=57, D=0, f=12, F=1444, g=6, G=722, h=7, j=19)  # m: mod 19 == 0

V = np.where(K != M)[0]                       # ayrışan sûreler (0-indeksli)
X = np.where(K == M)[0]                       # sabit sûreler
kk = len(V)
SPACE = 2 ** kk
print(f"Ayrışan sûre k={kk} → uzay 2^{kk} = {SPACE:.3e}")

fixed = sum(contrib(int(N[i]), int(K[i])) for i in X)          # sabit taban
cK = np.array([contrib(int(N[i]), int(K[i])) for i in V])      # (50,9) Kûfe seçeneği
cM = np.array([contrib(int(N[i]), int(M[i])) for i in V])      # (50,9) Medine seçeneği

# ---- sağlamalar ----
bK, bM = battery(K), battery(M)
print(f"Sağlama — saf Kûfe:   terazi {bK[0]} aile {bK[1]} iç {bK[2]} İkiEl {bK[3]}  (4/4 beklenir)")
print(f"Sağlama — saf Medine: terazi {bM[0]} aile {bM[1]} iç {bM[2]} İkiEl {bM[3]}  (kesif11 ile aynı olmalı)")
assert sum(bK) == 4, "Kûfe köşesi 4/4 değil!"

# ---- aktif/pasif ayrımı ----
# pasif: her iki seçenekte de aile/u-ailesi/birleşim bileşenleri (indeks 2..8) tamamen sıfır
nz = (cK[:, 2:] != 0).any(axis=1) | (cM[:, 2:] != 0).any(axis=1)
ACT = np.where(nz)[0]; PAS = np.where(~nz)[0]
print(f"Aktif sûreler ({len(ACT)}): {[int(N[V[i]]) for i in ACT]}")
print(f"Pasif sûreler ({len(PAS)}): yalnız (e,D) katkısı")

# ---- pasif DP: (Σe, ΣD) ortak dağılımı ----
def dp_eD(indices):
    tab = {(0, 0): 1}
    for i in indices:
        e0, D0 = int(cK[i, 0]), int(cK[i, 1])
        e1, D1 = int(cM[i, 0]), int(cM[i, 1])
        nt = {}
        for (e, D), c in tab.items():
            for de, dD in ((e0, D0), (e1, D1)):
                key = (e + de, D + dD)
                nt[key] = nt.get(key, 0) + c
        tab = nt
    return tab

pas_tab = dp_eD(PAS)
assert sum(pas_tab.values()) == 2 ** len(PAS)

# ---- A1: tam-yapı kesin sayımı ----
na = len(ACT)
bits = ((np.arange(2 ** na)[:, None] >> np.arange(na)[None, :]) & 1).astype(np.int64)  # (2^na, na)
act_stats = bits @ (cM[ACT] - cK[ACT]) + cK[ACT].sum(axis=0)[None, :]                  # (2^na, 9)
need = fixed + act_stats  # sabit + aktif toplam; pasif tamamlayacak
ok = (need[:, 2] == TARGETS['f']) & (need[:, 3] == TARGETS['F']) \
   & (need[:, 4] == TARGETS['g']) & (need[:, 5] == TARGETS['G']) \
   & (need[:, 6] == TARGETS['h']) & (need[:, 7] == TARGETS['j']) \
   & (need[:, 8] % 19 == 0)
cand = np.where(ok)[0]
print(f"Aile+İkiEl hedeflerini tutturan aktif atama: {len(cand)} / {2**na}")

total_solutions = 0
sol_details = []
for idx in cand:
    re_, rD = TARGETS['e'] - int(need[idx, 0]), TARGETS['D'] - int(need[idx, 1])
    c = pas_tab.get((re_, rD), 0)
    if c:
        total_solutions += c
        sol_details.append((int(idx), c))
print(f"\n=== A1 SONUÇ: tam-yapı (4/4) çözüm sayısı 2^{kk} uzayında KESİN = {total_solutions} ===")
if total_solutions > 0:
    print(f"    yoğunluk = {total_solutions/SPACE:.3e}  |  ayar hassasiyeti = {np.log2(SPACE/total_solutions):.1f} bit")

# çözümleri çıkar (az sayıdaysa): pasif tarafta geri-izleme
def extract_passive(indices, re_, rD, limit):
    sols, choice = [], []
    suff = [None] * (len(indices) + 1)
    suff[len(indices)] = {(0, 0): 1}
    for pos in range(len(indices) - 1, -1, -1):
        i = indices[pos]; nt = {}
        for (e, D), c in suff[pos + 1].items():
            for opt, (de, dD) in enumerate(((int(cK[i,0]), int(cK[i,1])), (int(cM[i,0]), int(cM[i,1])))):
                key = (e + de, D + dD)
                nt[key] = nt.get(key, 0) + c
        suff[pos] = nt
    def walk(pos, e, D):
        if len(sols) >= limit: return
        if pos == len(indices):
            if e == re_ and D == rD: sols.append(list(choice))
            return
        i = indices[pos]
        for opt, (de, dD) in enumerate(((int(cK[i,0]), int(cK[i,1])), (int(cM[i,0]), int(cM[i,1])))):
            if suff[pos + 1].get((re_ - e - de, rD - D - dD), 0) > 0:
                choice.append(opt); walk(pos + 1, e + de, D + dD); choice.pop()
    walk(0, 0, 0)
    return sols

if 0 < total_solutions <= 20:
    print("Çözümlerin dökümü (Medine seçilen sûreler):")
    for idx, c in sol_details:
        act_choice = [(int(N[V[ACT[b]]])) for b in range(na) if (idx >> b) & 1]
        re_, rD = TARGETS['e'] - int(need[idx, 0]), TARGETS['D'] - int(need[idx, 1])
        for pas_sol in extract_passive(list(PAS), re_, rD, limit=20):
            pas_choice = [int(N[V[PAS[p]]]) for p, o in enumerate(pas_sol) if o == 1]
            med = sorted(act_choice + pas_choice)
            print(f"  → Medine değeri seçilen sûreler: {med if med else 'YOK (saf Kûfe köşesi)'}")

# ---- A2: Hamming halkaları r ≤ 3 ----
print("\n=== A2: Kûfe köşesi çevresi (halka başına en iyi skor) ===")
for r in (1, 2, 3):
    best, hist = 0, {}
    best_combo = None
    for combo in itertools.combinations(range(kk), r):
        A = K.copy()
        for c in combo: A[V[c]] = M[V[c]]
        s = score(A)
        hist[s] = hist.get(s, 0) + 1
        if s > best: best, best_combo = s, combo
    n_tot = sum(hist.values())
    print(f" r={r}: {n_tot} yapılandırma | skor dağılımı {dict(sorted(hist.items()))} | en iyi {best}"
          + (f" (sûre {[int(N[V[c]]) for c in best_combo]})" if best >= 3 else ""))

# ---- A3: MC skor merdiveni ----
print("\n=== A3: MC skor merdiveni (10^7 örnek, tohum 42) ===")
rng = np.random.default_rng(42)
hist = np.zeros(5, dtype=np.int64)
dK, dM = cK, cM
delta = dM - dK
base = fixed + dK.sum(axis=0)
for _ in range(10):
    b = rng.integers(0, 2, size=(1_000_000, kk), dtype=np.int64)
    S = b @ delta + base[None, :]
    A1 = (S[:, 0] == 57) & (S[:, 1] == 0)
    B_ = (S[:, 2] == 12) & (S[:, 3] == 1444)
    C_ = B_ & (S[:, 4] == 6) & (S[:, 5] == 722)
    W_ = (S[:, 6] == 7) & (S[:, 7] == 19) & (S[:, 8] % 19 == 0)
    sc = A1.astype(int) + B_.astype(int) + C_.astype(int) + W_.astype(int)
    hist += np.bincount(sc, minlength=5)
print(f" skor dağılımı (10^7): {dict(enumerate(hist.tolist()))}")

# ---- A4: katman marjinalleri (kesin) ----
print("\n=== A4: katman-başına kesin marjinal sayımlar (2^50 içinde) ===")
def exact_marginal(stat_idx, target_check, label):
    rel = np.where((cK[:, stat_idx] != 0).any(axis=1) | (cM[:, stat_idx] != 0).any(axis=1))[0]
    if len(rel) > 24:
        print(f" {label}: ilgili sûre {len(rel)} > 24, atlandı"); return
    nb = len(rel)
    bb = ((np.arange(2 ** nb)[:, None] >> np.arange(nb)[None, :]) & 1).astype(np.int64)
    st = bb @ (cM[rel][:, stat_idx] - cK[rel][:, stat_idx]) + cK[rel][:, stat_idx].sum(axis=0)[None, :] \
         + fixed[stat_idx][None, :]
    hits = int(target_check(st).sum()) * 2 ** (kk - nb)
    print(f" {label}: {hits} / 2^{kk}  ({hits/SPACE:.3e})")

# terazi (tüm 50 sûre ilgili) → DP ile
ter_tab = dp_eD(list(range(kk)))
ter_hits = ter_tab.get((TARGETS['e'] - int(fixed[0]), TARGETS['D'] - int(fixed[1])), 0)
print(f" Terazi A1: {ter_hits} / 2^{kk}  ({ter_hits/SPACE:.3e})")
exact_marginal([2, 3], lambda s: (s[:, 0] == 12) & (s[:, 1] == 1444), "Aile B")
exact_marginal([2, 3, 4, 5], lambda s: (s[:, 0] == 12) & (s[:, 1] == 1444) & (s[:, 2] == 6) & (s[:, 3] == 722), "İç terazi C")
exact_marginal([6, 7, 8], lambda s: (s[:, 0] == 7) & (s[:, 1] == 19) & (s[:, 2] % 19 == 0), "İki El W")
