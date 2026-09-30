#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""KIRMIZI-22: Asgarî (eşit-tarif-uzunluklu) çatal kümesi + katı 8/8 oranının çözülmesi.

Ana hattın itirazı: kesif19'un çatalları eşit ağırlıkla sayılıyor; kitap her ölçütte sınıfın en kısa
tarifli üyesinde; sentetikler daha uzun tarifli çatallarla geçiyor. Cevap: yalnız EŞİT uzunluktaki
alternatifleri tutan "asgarî" küme tanımlanır ve üç puanlama (katı / asgarî / geniş) aynı örneklemde ölçülür.

Asgarî çatallar:
 S1 k ≥ 2E | k = m                      S2 m² | Σv | Σv tam kare | 114 | Σv
 S3 v-paritesi (eşit sayı ∧ eşit toplam) | n-paritesi (aynı)
 S4 m | Σc(iki v-yarı) | iki yarı tam kare | m | Σc(iki n-yarı)
 S5 yalnız ardışık bant                 S6 |F∪U| = m | k_u = k | k_u = m
 S7 {Σa,Σn,Σv,Σw}(birleşim) ≡ 0 (mod m) S8 yalnız ayrık
 El: t birincil | u birincil (aynı uzunluk)
Geniş küme = kesif19 ile aynı.

Sabit mercek (1,1,19). Çözünürlük için koşullu örnekleme: k_t ≥ 12 VEYA k_u ≥ 12 olan satırlar tutulur
(S1'in her iki el için ön-koşulu); koşulsuz oran = P(tut) × koşullu oran.
"""
import json, os, sys, time
import numpy as np

REPO = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
N = np.arange(1, 115, dtype=np.int64)
K0 = np.array([s['numberOfAyahs'] for s in json.load(open(f'{REPO}/quran_meta.json'))['data']['surahs']['references']], dtype=np.int64)
A = np.where(N == 9, 127, K0)
PP = np.array(sorted({p**j for p in (2, 3, 5, 7) for j in range(3, 20) if p**j < 10**6}), dtype=np.int64)
BIG = 10**7

def is_square(x):
    x = np.abs(x); r = np.floor(np.sqrt(x)).astype(np.int64)
    return (r * r == x) | ((r + 1) * (r + 1) == x)
def is_pp(x): return np.isin(np.abs(x), PP)
def pretty(x, m): return (x % (m * m) == 0) | is_square(x) | (x % 114 == 0) | is_pp(x)

def rowstats(V, F):
    k = F.sum(1); S = np.sort(np.where(F, V, BIG), axis=1)
    cmin = S[:, 0]; cmax = np.where(F, V, -BIG).max(1)
    nd = ((S[:, 1:] != S[:, :-1]) & (S[:, 1:] < BIG)).sum(1) + (k > 0)
    return k, cmin, cmax, nd

def score3(P, al, be, m, primary_sum=True):
    """Döner (katı, asgarî, geniş) int8 puanlar."""
    Nn = N[None, :]
    V = al * Nn + be * P; W = al * Nn - be * P
    if not primary_sum: V, W = W, V
    E = 114 / m
    F = (V % m == 0); k = F.sum(1); kpos = k > 0
    C = V // m; sv = (V * F).sum(1); sc = (C * F).sum(1)
    s1 = k >= 2 * E; a1 = s1 | (k == m); g1 = a1
    s2 = kpos & (sv % (m * m) == 0)
    a2 = kpos & ((sv % (m * m) == 0) | is_square(sv) | (sv % 114 == 0)); g2 = kpos & pretty(sv, m)
    VE = (V % 2 == 0); FE = F & VE
    ke = FE.sum(1); sve = (V * FE).sum(1); svo = sv - sve
    eqcnt = (2 * ke == k) & kpos
    s3 = eqcnt & (sve == svo); d = np.abs(sve - svo)
    NE_ = (Nn % 2 == 0); FN = F & NE_
    kn = FN.sum(1); svn = (V * FN).sum(1); svn2 = sv - svn
    eqn = (2 * kn == k) & kpos; dn = np.abs(svn - svn2)
    a3 = s3 | (eqn & (svn == svn2))
    g3 = (eqcnt & ((d == 0) | (d == m * m) | (d == 2 * m * m))) | (eqn & ((dn == 0) | (dn == m * m) | (dn == 2 * m * m)))
    s4 = kpos & (sve % (m * m) == 0) & (svo % (m * m) == 0)
    a4 = s4 | (kpos & is_square(sve) & is_square(svo)) | (kpos & (svn % (m * m) == 0) & (svn2 % (m * m) == 0)); g4 = a4
    kk, cmin, cmax, nd = rowstats(C, F)
    band = kpos & (nd == cmax - cmin + 1)
    ap2 = kpos & ((cmax - cmin) % 2 == 0) & (nd == (cmax - cmin) // 2 + 1) & (nd >= 2) & \
          (((C * F) % 2 == (cmin[:, None] % 2)) | ~F).all(1)
    s5 = band; a5 = band; g5 = band | ap2
    U = (W % m == 0); ku = U.sum(1); uni = F | U; kuni = uni.sum(1); inter = (F & U).sum(1)
    s6 = kuni == m; a6 = s6 | ((ku == k) & kpos) | (ku == m); g6 = a6 | (kuni == 2 * m) | (ku >= 2 * E)
    sa_u = (P * uni).sum(1); sn_u = (Nn * uni).sum(1); sv_u = (V * uni).sum(1); sw_u = (W * uni).sum(1)
    s7 = sa_u % m == 0
    a7 = s7 | (sn_u % m == 0) | (sv_u % m == 0) | (sw_u % m == 0)
    g7 = a7 | pretty(sa_u, m) | pretty(sn_u, m) | pretty(sv_u, m) | pretty(sw_u, m) | \
         ((P * U).sum(1) % m == 0) | ((Nn * U).sum(1) % m == 0) | ((ku > 0) & ((W * U).sum(1) == m * ku))
    s8 = inter == 0; a8 = s8; g8 = s8 | (inter == 1)
    strict = (s1.astype(np.int8) + s2 + s3 + s4 + s5 + s6 + s7 + s8)
    minimal = (a1.astype(np.int8) + a2 + a3 + a4 + a5 + a6 + a7 + a8)
    generous = (g1.astype(np.int8) + g2 + g3 + g4 + g5 + g6 + g7 + g8)
    return strict, minimal, generous

def run_fixed(n_perm, seed=31, chunk=200_000):
    rng = np.random.default_rng(seed)
    H = {key: np.zeros(9, np.int64) for key in ('katı_t', 'asgarî_t', 'geniş_t', 'katı_2el', 'asgarî_2el', 'geniş_2el')}
    kept = 0; done = 0; t0 = time.time()
    while done < n_perm:
        b = min(chunk, n_perm - done); done += b
        P = rng.permuted(np.tile(A, (b, 1)), axis=1)
        kt = ((N[None, :] + P) % 19 == 0).sum(1); ku = ((N[None, :] - P) % 19 == 0).sum(1)
        sel = P[(kt >= 12) | (ku >= 12)]
        kept += sel.shape[0]
        if sel.shape[0] == 0: continue
        st, mt, gt = score3(sel, 1, 1, 19, True)
        su, mu, gu = score3(sel, 1, 1, 19, False)
        H['katı_t'] += np.bincount(st, minlength=9); H['asgarî_t'] += np.bincount(mt, minlength=9); H['geniş_t'] += np.bincount(gt, minlength=9)
        H['katı_2el'] += np.bincount(np.maximum(st, su), minlength=9)
        H['asgarî_2el'] += np.bincount(np.maximum(mt, mu), minlength=9)
        H['geniş_2el'] += np.bincount(np.maximum(gt, gu), minlength=9)
        if done % 20_000_000 == 0:
            print(f"  {done:,} perm, tutulan {kept:,} | 8/8 katı_t={int(H['katı_t'][8])} asgarî_2el={int(H['asgarî_2el'][8])} geniş_2el={int(H['geniş_2el'][8])} [{time.time()-t0:.0f}s]", flush=True)
    pk = kept / n_perm
    print(f"\n=== SABİT MERCEK (1,1,19), {n_perm:,} permütasyon; koşul (k_t≥12 ∨ k_u≥12): P = {pk:.4e}, {kept:,} satır ===")
    print(f"{'puanlama':12s} {'7/8':>10s} {'8/8':>8s}   {'P(8/8) koşulsuz':>16s}   {'%95 aralık':>24s}")
    for key, h in H.items():
        c7 = int(h[7]); c8 = int(h[8]); p8 = c8 / kept * pk
        if c8 == 0: lo, hi = 0.0, 3.69 / kept * pk
        else:  # Poisson %95 (karekök yaklaşımı; c≥1 için yeterli)
            lo = max(0.0, (c8 ** 0.5 - 0.98)) ** 2 / kept * pk; hi = (c8 ** 0.5 + 0.98) ** 2 / kept * pk
        print(f"{key:12s} {c7:10,d} {c8:8,d}   {p8:16.2e}   [{lo:.1e}, {hi:.1e}]")
    print("Gerçek kitap: katı 8/8, asgarî 8/8, geniş 8/8 (t-eli, (1,1,19)).")

if __name__ == "__main__":
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 300_000_000
    s, m_, g = score3(A[None, :], 1, 1, 19, True)
    print("gerçek kitap puanları (katı, asgarî, geniş):", int(s[0]), int(m_[0]), int(g[0]))
    run_fixed(n)
