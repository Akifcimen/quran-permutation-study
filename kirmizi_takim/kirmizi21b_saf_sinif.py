#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""KIRMIZI-21b (v2, SAF SINIF MENÜSÜ): "En iyi hikâye" tipikliği — modu ayrıcalıklandırmayan sınıf-düzeyi test.

Ana hattın itirazı (kesif20'ye): nokta-olasılık kıyası tam-denge (mod) yapılandırmasını hiçbir zaman
anormal gösteremez. Cevap: kıyası SINIF düzeyinde yap. Her 12'lik aileye aynı "güzel yüklem" menüsü
verilir; ailenin "en iyi hikâyesi" = sağladığı en nadir (≤ L yüklemli) konjonksiyon. Soru: rastgele
ailelerin yüzde kaçı, gerçek kitabınki kadar iyi (ya da daha iyi) bir hikâyeye sahip?

Menü (C düzeyi: Σc, k_e, Σc_e, Σc_o; D düzeyi: 4 hücre (t-parite × n-parite) sayı ve toplamları):
 P01 k_e = 6                         P02 Σc_e = Σc_o                  P03 19 | Σc (Σt = 19²·k)
 P04 Σc = 76 (Σt = 38², tam kare)    P05 6 | Σc (114 | Σt)            P06 |Σc_e − Σc_o| = 19
 P07 |Σc_e − Σc_o| = 38              P08 2:1 oran                     P09 k_e ∈ {0,12}
 P10 19 | Σc_e ∧ 19 | Σc_o           P11 Σc_e = 38 ∨ Σc_o = 38        P12 Σc tam kare
 P13 k_e ∈ {3,9}                     P14 19 | Σc_e ∨ 19 | Σc_o        P15 Σc = 114
 P16 katsayılar ardışık bant         P17 katsayı bandı 5..9           P18 en sık katsayı 6 (t=114) ≥ 4 kez
 Q01 hücreler 3/3/3/3                Q02 hücre sayıları çiftler hâlinde eşit   Q03 tek-t hücreleri 19/19
 Q04 sıralı hücre toplamları [18,19,19,20]   Q05 dört hücre toplamı da 19'un katı
 Q06 iki hücre toplamı eşit          Q07 köşegen dengesi              Q08 toplamlar aritmetik dizi
 Q09 toplamlar simetrik (s1+s4=s2+s3) Q10 bir hücre toplamı 19        Q11 bir hücre toplamı 38
 Q12 çift-t hücre çarpımı 360        Q13 max−min hücre sayısı ≤ 1     Q14 bir hücrede üç eşit katsayı
Hikâye = ≤ L yüklemin konjonksiyonu (L = 1..4). Sınıf olasılıkları aynı örneklemden; < 10 isabetli
hikâyeler 10 isabet olarak tabanlanır (rastgele ailelerin hikâyelerini DAHA SIK gösterir → yazar lehine).
"""
import json, os, sys, time, itertools
import numpy as np

REPO = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
N = np.arange(1, 115, dtype=np.int64)
K0 = np.array([s['numberOfAyahs'] for s in json.load(open(f'{REPO}/quran_meta.json'))['data']['surahs']['references']], dtype=np.int64)
A = np.where(N == 9, 127, K0)
n_target = int(sys.argv[1]) if len(sys.argv) > 1 else 1_500_000
rng = np.random.default_rng(778)
NE = (N % 2 == 0)

def is_sq(x):
    x = np.abs(x); r = np.floor(np.sqrt(x)).astype(np.int64)
    return (r * r == x) | ((r + 1) * (r + 1) == x)

def features(P):
    """P: (B,114), hepsi k=12. Döner (B, 32) bool yüklem matrisi."""
    T = N[None, :] + P; F = (T % 19 == 0); C = T // 19
    TE = (T % 2 == 0)
    sc = (C * F).sum(1)
    FE = F & TE; ke = FE.sum(1); sce = (C * FE).sum(1); sco = sc - sce
    d = np.abs(sce - sco)
    # hücreler
    m_ee = F & TE & NE[None, :]; m_eo = F & TE & ~NE[None, :]
    m_oe = F & ~TE & NE[None, :]; m_oo = F & ~TE & ~NE[None, :]
    cnt = np.stack([m.sum(1) for m in (m_ee, m_eo, m_oe, m_oo)], 1)
    sm = np.stack([(C * m).sum(1) for m in (m_ee, m_eo, m_oe, m_oo)], 1)
    ss = np.sort(sm, 1)
    # bant
    Cm = np.where(F, C, 10**6); S = np.sort(Cm, 1)
    cmin = S[:, 0]; cmax = np.where(F, C, -1).max(1)
    nd = ((S[:, 1:] != S[:, :-1]) & (S[:, 1:] < 10**6)).sum(1) + 1
    band = nd == cmax - cmin + 1
    # en sık katsayı 6 sayısı
    six = ((C == 6) & F).sum(1)
    # hücrede üç eşit katsayı: hücre sayısı 3 ve o hücrede max==min
    def cell_allequal(m):
        cm = np.where(m, C, -1).max(1); cn = np.where(m, C, 10**6).min(1)
        return (m.sum(1) == 3) & (cm == cn)
    q14 = cell_allequal(m_ee) | cell_allequal(m_eo) | cell_allequal(m_oe) | cell_allequal(m_oo)
    # band genişliği, tekrar sayısı, aynı-taraf eşit hücreler, m-etrafında simetri, parite-izinli en dengeli
    width = cmax - cmin + 1
    maxrep = np.zeros(C.shape[0], dtype=np.int64)
    for v in range(0, 16): maxrep = np.maximum(maxrep, ((C == v) & F).sum(1))
    same_side_eq = (sm[:, 0] == sm[:, 1]) | (sm[:, 2] == sm[:, 3])
    sym_m = (sm[:, 0] + sm[:, 1] == 38) & (sm[:, 2] + sm[:, 3] == 38)
    balanced = (ss[:, 3] - ss[:, 0]) <= 2
    preds = [
        ke == 6, sce == sco, sc % 19 == 0, is_sq(19 * sc), sc % 6 == 0, d == 19, d == 38,
        (sce == 2 * sco) | (sco == 2 * sce), (ke == 0) | (ke == 12),
        (sce % 19 == 0) & (sco % 19 == 0), is_sq(sc), (ke == 3) | (ke == 9),
        (sce % 19 == 0) | (sco % 19 == 0), sc == 114,
        band, band & (width <= 5), maxrep >= 5, maxrep >= 4,
        (cnt == 3).all(1), (cnt[:, 0] == cnt[:, 1]) & (cnt[:, 2] == cnt[:, 3]),
        same_side_eq, balanced, sym_m,
        (sm % 19 == 0).all(1),
        (ss[:, 0] == ss[:, 1]) | (ss[:, 1] == ss[:, 2]) | (ss[:, 2] == ss[:, 3]),
        sm[:, 0] + sm[:, 3] == sm[:, 1] + sm[:, 2],
        (ss[:, 1] - ss[:, 0] == ss[:, 2] - ss[:, 1]) & (ss[:, 2] - ss[:, 1] == ss[:, 3] - ss[:, 2]),
        ss[:, 0] + ss[:, 3] == ss[:, 1] + ss[:, 2],
        (sm == 19).any(1), (sm == 38).any(1), sm[:, 0] * sm[:, 1] == 360,
        cnt.max(1) - cnt.min(1) <= 1, q14,
    ]
    return np.stack(preds, 1)

NAMES = ["P01 k_e=6","P02 Σc_e=Σc_o","P03 m|Σc","P04 Σt kare","P05 114|Σt","P06 Δ=m","P07 Δ=2m","P08 2:1",
         "P09 tek parite","P10 m|iki yarı","P11 Σc kare","P12 k_e∈{3,9}","P13 m|bir yarı","P14 Σc=6m",
         "P15 bant","P16 bant≤5 geniş","P17 bir katsayı ≥5 kez","P18 bir katsayı ≥4 kez",
         "Q01 3/3/3/3","Q02 çift-eşit sayı","Q03 aynı-taraf hücreler eşit","Q04 parite-izinli en dengeli (max−min≤2)",
         "Q05 m-etrafında simetrik (çiftler 2m)","Q06 hepsi m×","Q07 iki toplam eşit","Q08 köşegen","Q09 AD",
         "Q10 simetrik","Q11 bir hücre m","Q12 bir hücre 2m","Q13 çift-taraf çarpımı m²−1","Q14 sayılar ±1","Q15 üç eşit katsayı"]
NP = len(NAMES)

real = features(A[None, :])[0]
print("Gerçek kitabın sağladığı yüklemler:", [NAMES[i] for i in range(NP) if real[i]])

# ---- örnekleme ----
rows = []; tot = 0; t0 = time.time(); CH = 200_000
while sum(r.shape[0] for r in rows) < n_target:
    P = rng.permuted(np.tile(A, (CH, 1)), axis=1)
    k = ((N[None, :] + P) % 19 == 0).sum(1)
    sel = P[k == 12]; tot += CH
    rows.append(features(sel))
X = np.concatenate(rows)[:n_target]
n = X.shape[0]
print(f"{tot:,} permütasyon → {n:,} k=12 örneği [{time.time()-t0:.0f}s]")
bits = np.zeros(n, dtype=np.int64)
for i in range(NP): bits |= (X[:, i].astype(np.int64) << i)
rbits = int(sum(int(real[i]) << i for i in range(NP)))

print("\nTekil yüklem oranları (k=12 koşullu):")
for i in range(NP):
    print(f"  {NAMES[i]:24s} {X[:, i].mean():.3e}" + ("  ← gerçek sağlıyor" if real[i] else ""))

FLOOR = 10
for L in (1, 2, 3, 4):
    t1 = time.time()
    best = np.full(n, 1.0); real_best = 1.0; real_story = None
    stories = 0
    for size in range(1, L + 1):
        for combo in itertools.combinations(range(NP), size):
            mask = sum(1 << i for i in combo)
            sat = (bits & mask) == mask
            c = int(sat.sum())
            if c == 0: continue
            stories += 1
            p = max(c, FLOOR) / n
            np.minimum(best, np.where(sat, p, 1.0), out=best)
            if (rbits & mask) == mask and p < real_best:
                real_best = p; real_story = [NAMES[i] for i in combo]
    frac = float((best <= real_best).mean())
    q = np.quantile(best, [0.1, 0.5, 0.9])
    print(f"\n[L ≤ {L}] {stories:,} hikâye [{time.time()-t1:.0f}s]")
    print(f"   gerçek kitabın en iyi hikâyesi: P = {real_best:.3e}  ← {' ∧ '.join(real_story)}")
    print(f"   rastgele ailelerin en iyi hikâyesi: %10 = {q[0]:.2e} | medyan = {q[1]:.2e} | %90 = {q[2]:.2e}")
    print(f"   gerçek kitabınki kadar iyi ya da daha iyi hikâyesi olan rastgele aile oranı: {frac:.3%}")
