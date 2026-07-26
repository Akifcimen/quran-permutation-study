#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""KESIF-18: P(A∧W | G) koşullu doğrudan ölçümü. Ön-kayıt: kesif18_kosullu_cekirdek_prereg.md
Tasarım: SIS ile aile ataması + bileşen içinde kesin-düzgün ret örneklemesi (i.i.d.; MCMC yok).
Kodeks: sûre 9 = 127 (motorla aynı)."""
import json
from collections import Counter
import numpy as np

rng = np.random.default_rng(42)
N = np.arange(1, 115)
K0 = np.array([s['numberOfAyahs'] for s in json.load(open('quran_meta.json'))['data']['surahs']['references']], dtype=np.int64)
A_ALL = np.where(N == 9, 127, K0)
assert int(A_ALL.sum()) == 6234

# ---- gerçek veri sağlaması: G motor tanımıyla tutuyor mu ----
def g_check(a):
    t = N + a; fam = (t % 19 == 0)
    if fam.sum() != 12: return False
    c = (t[fam] // 19); npar = (N[fam] % 2 == 1); ceven = (c % 2 == 0)
    if sorted(c.tolist()) != [5,5,5,6,6,6,6,6,7,7,8,9]: return False
    cells = {}
    for key_ce in (True, False):
        for key_no in (True, False):
            cells[(key_ce, key_no)] = sorted(c[(ceven == key_ce) & (npar == key_no)].tolist())
    ev = {tuple(cells[(True, True)]), tuple(cells[(True, False)])}
    od = {tuple(cells[(False, True)]), tuple(cells[(False, False)])}
    return ev == {(6,6,6), (6,6,8)} and od == {(5,5,9), (5,7,7)} and \
           all(len(v) == 3 for v in cells.values())
assert g_check(A_ALL), "gerçek veri G tanımını sağlamalı"

CNT = Counter(A_ALL.tolist())

# ---- SIS: geçerli aile ataması üret (q kaydıyla) ----
# yuva sırası: 4 hücre × 3 katsayı; oryantasyon (2×2) düzgün seçilir (sabit çarpan, öz-normda düşer)
def draw_assignment():
    for _ in range(200):  # çıkmaz-yeniden-deneme
        ori_even = rng.integers(2)   # 0: {666}→n-tek, {668}→n-çift (gerçek yön); 1: tersi
        ori_odd = rng.integers(2)    # 0: {559}→n-çift, {577}→n-tek (gerçek yön); 1: tersi
        cells = [((6,6,6), 1 - ori_even), ((6,6,8), ori_even),
                 ((5,5,9), ori_odd), ((5,7,7), 1 - ori_odd)]
        # (içerik, n%2 değeri): n%2==1 tek konum
        used_n = set(); avail = Counter(CNT); pairs = []; logq = 0.0
        ok = True
        for content, npar in cells:
            for c in content:
                cand = [(n, 19 * c - n) for n in range(1, 115)
                        if n % 2 == npar and n not in used_n and avail[19 * c - n] > 0]
                if not cand: ok = False; break
                n_, a_ = cand[rng.integers(len(cand))]
                logq -= np.log(len(cand))
                used_n.add(n_); avail[a_] -= 1; pairs.append((n_, a_, c))
            if not ok: break
        if ok:
            return pairs, avail, used_n, logq
    raise RuntimeError("SIS: 200 denemede geçerli atama yok")

def evaluate(assign_pairs, rest_counts, rest_pos, n_shuffle, batch=400_000):
    """Atama sabit; kalan sayıları düzgün karıştır, yeni 19-katı üretenleri ele.
    Dönen: (toplam karışım, kabul, W isabet, A isabet, A∧W isabet)"""
    rp = np.array(sorted(rest_pos)); rc = np.array(rest_counts, dtype=np.int64)
    fam_n = np.array([p[0] for p in assign_pairs]); fam_a = np.array([p[1] for p in assign_pairs])
    # sabit katkılar (aile):
    fam_t = fam_n + fam_a
    fam_even_cnt = int((fam_t % 2 == 0).sum()); fam_even_sum = int(fam_t[fam_t % 2 == 0].sum())
    fam_u = fam_n - fam_a
    fam_fu = (fam_u % 19 == 0)          # ailede u-katı da olan üyeler (kesişim → W'de birleşim sayımı)
    fam_sa = int(fam_a.sum())
    tot = acc = hW = hA = hAW = 0
    done = 0
    while done < n_shuffle:
        b = min(batch, n_shuffle - done); done += b
        P = rng.permuted(np.tile(rc, (b, 1)), axis=1)
        T = rp[None, :] + P
        new_fam = (T % 19 == 0)
        keep = ~new_fam.any(axis=1)
        tot += b; k = int(keep.sum()); acc += k
        if k == 0: continue
        Pk = P[keep]; Tk = T[keep]
        # A: çift-t sayısı 57 ve toplamı 6234 (aile katkısı dahil)
        te = (Tk % 2 == 0)
        A_ = (te.sum(1) + fam_even_cnt == 57) & ((Tk * te).sum(1) + fam_even_sum == 6234)
        # W: u-katları
        U = rp[None, :] - Pk
        fu = (U % 19 == 0)
        nu = fu.sum(1) + int(fam_fu.sum())
        # birleşim: aile 12 (tamamı t-katı) + kalan konumlarda fu olanlar (t-katı yok → kesişim yalnız ailede)
        uniCnt = 12 + fu.sum(1)
        uniSa = fam_sa + (Pk * fu).sum(1)
        W_ = (nu == 7) & (uniCnt == 19) & (uniSa % 19 == 0)
        hW += int(W_.sum()); hA += int(A_.sum()); hAW += int((A_ & W_).sum())
    return tot, acc, hW, hA, hAW

S_ASSIGN, SHUF_PER = 600, 4_000_000
res = []
for j in range(S_ASSIGN):
    pairs, avail, used_n, logq = draw_assignment()
    rest_counts = list(avail.elements())
    rest_pos = [n for n in range(1, 115) if n not in used_n]
    assert len(rest_counts) == 102 and len(rest_pos) == 102
    tot, acc, hW, hA, hAW = evaluate(pairs, rest_counts, rest_pos, SHUF_PER)
    res.append((logq, tot, acc, hW, hA, hAW))
    if (j + 1) % 25 == 0:
        ta = sum(r[2] for r in res); tw = sum(r[3] for r in res); taa = sum(r[4] for r in res); taw = sum(r[5] for r in res)
        print(f"atama {j+1}/{S_ASSIGN} | G-örneği {ta:,} | W:{tw} A:{taa} A∧W:{taw}", flush=True)

# ---- kestirimler ----
logq = np.array([r[0] for r in res]); tot = np.array([r[1] for r in res], dtype=float)
acc = np.array([r[2] for r in res], dtype=float)
hW = np.array([r[3] for r in res], dtype=float); hA = np.array([r[4] for r in res], dtype=float)
hAW = np.array([r[5] for r in res], dtype=float)
p_avoid = acc / tot
w = p_avoid * np.exp(-(logq - logq.max()))          # ∝ bileşen kütlesi / örnekleme olasılığı
w /= w.sum()
ess = 1.0 / np.sum(w ** 2)

def rate(h):
    unw = h.sum() / acc.sum()
    m = acc > 0
    wg = np.sum(w[m] * (h[m] / acc[m])) / np.sum(w[m])
    return unw, wg

for ad, h in (("W|G", hW), ("A|G", hA), ("A∧W|G", hAW)):
    unw, wg = rate(h)
    print(f"P({ad}): havuz {unw:.3e} | ağırlıklı {wg:.3e} | isabet {int(h.sum())}")
nAW = hAW.sum()
lo, hi = (0.5 * np.array([np.percentile(rng.chisquare(2 * nAW, 200_000), 2.5),
                          np.percentile(rng.chisquare(2 * (nAW + 1), 200_000), 97.5)]) if nAW > 0 else (0.0, 3.69))
pAW_unw = nAW / acc.sum()
print(f"\nÖRNEK: {int(acc.sum()):,} i.i.d. G-örneği | ağırlık ESS = {ess:.0f}/{S_ASSIGN}")
print(f"P(A∧W|G) = {pAW_unw:.3e}  (Poisson %95: [{lo/acc.sum():.2e}, {hi/acc.sum():.2e}])")
PG = 7.14e-10
print(f"P(A∧G∧W) = P(G)×P(A∧W|G) = {PG * pAW_unw:.2e}  GA: [{PG*lo/acc.sum():.1e}, {PG*hi/acc.sum():.1e}]")
print(f"kıyas — zincir: 1.02e-15 / 'doğrudan' etiketlisi: 8.4e-16 | GPU W|G çapası: 62/17204 = 3.60e-03")
