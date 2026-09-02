#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""KESIF-19 (dış saldırı): Çatallanmış mercek turnuvası — "ölçütler veriden okundu" itirazının fiyatı.

kesif4/kesif9/kesif17'deki S1–S8 profili, gerçek kitabın m=19'daki gözlenen özelliklerinden yazılmıştır.
Bu betik her ölçüt için, depoda YAZARIN KENDİSİNİN başka yerde kutladığı/etiketlediği eşdeğer
alternatifleri ("çatallar") ekler ve aynı turnuvayı yeniden koşar. Soru: ölçütler gözlenen değere
değil, "aynı derecede güzel herhangi bir değere" göre yazılsaydı, sentetik kitaplar 7/8–8/8'e ulaşır mıydı?

Çatal gerekçeleri (depodan kanıt):
 S1  k ≥ 2E  |  k == m                       [S6'da "birleşim = m" kutlanıyor; aynı güzellik k için]
 S2  m² | Σv | Σv tam kare | 114 | Σv | Σv asal kuvveti (p≤7, üs≥3)
                                              [kesif2_panorama.pretty(): 19×, 38×, kare, 114× etiketleri;
                                               KESIF.md "Üçün Dönüşü": Σt(birleşim)=3⁷ kutlanıyor]
 S3  v-paritesi eşit sayı ∧ eşit toplam | n-paritesi ile aynı | eşit sayı ∧ |Δ| ∈ {m², 2m²}
                                              [2c: "18 ve 20, 19'un etrafında" (Δ=2m²); ikinci bölme ekseni n-paritesi]
 S4  m | Σc(her yarı) | iki yarı tam kare       [722=2×19² ve "38=2×19" katsayı toplamı]
 S5  ardışık bant | adım-2 aritmetik dizi       ["5,7,9 çıkıyor" (adım 2) kutlanıyor]
 S6  |F∪U| ∈ {m, 2m} | k_u == k | k_u == m | k_u ≥ 2E
 S7  {Σa,Σn,Σv,Σw}(birleşim) m'ye bölünür ya da güzel (kare/asal kuvveti/114×) |
     Σa(U) veya Σn(U) m'ye bölünür | Σw(U) == m·k_u   [Σt(birleşim)=3⁷; Σu=7×19 "ortalama katsayı 1"; |Σu|=21×19]
 S8  ayrık | kesişim tam 1
 EL  her mercek iki el sırasıyla değerlendirilir (v birincil / w birincil)  [u-eli kristal taşısaydı raporlanırdı]

Katı puan (çatalsız, kesif17 ile birebir) aynı koşuda ayrıca hesaplanır — karşılaştırma tabanı.
"""
import json, sys, time
import numpy as np

N = np.arange(1, 115, dtype=np.int64)
K0 = np.array([s['numberOfAyahs'] for s in json.load(open('quran_meta.json'))['data']['surahs']['references']], dtype=np.int64)
A = np.where(N == 9, 127, K0)                      # motorla aynı kodeks
FORMS = [(0, 1), (1, 1), (1, 2), (1, 3), (2, 1), (2, 3), (3, 1), (3, 2)]
MODS = list(range(3, 41))
PP = np.array(sorted({p**j for p in (2, 3, 5, 7) for j in range(3, 20) if p**j < 10**6}), dtype=np.int64)
BIG = 10**7

def is_square(x):
    x = np.abs(x); r = np.floor(np.sqrt(x)).astype(np.int64)
    return (r * r == x) | ((r + 1) * (r + 1) == x)
def is_pp(x): return np.isin(np.abs(x), PP)
def pretty(x, m): return (x % (m * m) == 0) | is_square(x) | (x % 114 == 0) | is_pp(x)

def rowstats(V, F):
    """F maskeli satırlarda katsayı bandı / adım-2 AD için (nd, cmin, cmax, k)."""
    k = F.sum(1)
    C = np.where(F, V, BIG)
    S = np.sort(C, axis=1)
    cmin = S[:, 0]
    cmax = np.where(F, V, -BIG).max(1)
    nd = ((S[:, 1:] != S[:, :-1]) & (S[:, 1:] < BIG)).sum(1) + (k > 0)
    return k, cmin, cmax, nd

def score(P, al, be, m, primary_sum=True):
    """P:(B,114). Döner (katı_puan, çatallı_puan) int8 dizileri."""
    Nn = N[None, :]
    V = al * Nn + be * P; W = al * Nn - be * P
    if not primary_sum: V, W = W, V
    E = 114 / m
    F = (V % m == 0); k = F.sum(1); kpos = k > 0
    C = V // m                                     # katsayılar (tam bölünen yerlerde anlamlı)
    sv = (V * F).sum(1); sc = (C * F).sum(1)
    # --- S1
    s1 = k >= 2 * E
    f1 = s1 | (k == m)
    # --- S2
    s2 = kpos & (sv % (m * m) == 0)
    f2 = kpos & pretty(sv, m)
    # --- S3 / S4 (v-paritesi)
    VE = (V % 2 == 0); FE = F & VE
    ke = FE.sum(1); sve = (V * FE).sum(1); svo = sv - sve
    sce = (C * FE).sum(1); sco = sc - sce
    eqcnt = (2 * ke == k) & kpos
    s3 = eqcnt & (sve == svo)
    d = np.abs(sve - svo)
    f3v = eqcnt & ((d == 0) | (d == m * m) | (d == 2 * m * m))
    # n-paritesi ile
    NE = (Nn % 2 == 0); FN = F & NE
    kn = FN.sum(1); svn = (V * FN).sum(1); svn2 = sv - svn
    eqn = (2 * kn == k) & kpos
    dn = np.abs(svn - svn2)
    f3n = eqn & ((dn == 0) | (dn == m * m) | (dn == 2 * m * m))
    f3 = f3v | f3n
    s4 = kpos & (sve % (m * m) == 0) & (svo % (m * m) == 0)
    f4 = s4 | (kpos & is_square(sve) & is_square(svo)) | \
         (kpos & (svn % (m * m) == 0) & (svn2 % (m * m) == 0))
    # --- S5
    kk, cmin, cmax, nd = rowstats(C, F)
    band = kpos & (nd == cmax - cmin + 1)
    ap2 = kpos & ((cmax - cmin) % 2 == 0) & (nd == (cmax - cmin) // 2 + 1) & (nd >= 2) & \
          (((C * F) % 2 == (cmin[:, None] % 2)) | ~F).all(1)
    s5 = band; f5 = band | ap2
    # --- S6 / S7 / S8 (İki El)
    U = (W % m == 0); ku = U.sum(1); uni = F | U; kuni = uni.sum(1)
    inter = (F & U).sum(1)
    s6 = kuni == m
    f6 = s6 | (kuni == 2 * m) | ((ku == k) & kpos) | (ku == m) | (ku >= 2 * E)
    sa_u = (P * uni).sum(1); sn_u = (Nn * uni).sum(1); sv_u = (V * uni).sum(1); sw_u = (W * uni).sum(1)
    s7 = sa_u % m == 0
    f7 = s7 | (sn_u % m == 0) | (sv_u % m == 0) | (sw_u % m == 0) | \
         pretty(sa_u, m) | pretty(sn_u, m) | pretty(sv_u, m) | pretty(sw_u, m) | \
         ((P * U).sum(1) % m == 0) | ((Nn * U).sum(1) % m == 0) | ((ku > 0) & ((W * U).sum(1) == m * ku))
    s8 = inter == 0
    f8 = s8 | (inter == 1)
    strict = (s1.astype(np.int8) + s2 + s3 + s4 + s5 + s6 + s7 + s8)
    forked = (f1.astype(np.int8) + f2 + f3 + f4 + f5 + f6 + f7 + f8)
    return strict, forked

def real_map():
    P = A[None, :]
    rows = []
    for al, be in FORMS:
        for m in MODS:
            for prim in (True, False):
                s, f = score(P, al, be, m, prim)
                rows.append((int(f[0]), int(s[0]), al, be, m, 't' if prim else 'u'))
    rows.sort(reverse=True)
    print("=== GERÇEK KİTAP: katı ve çatallı puan (en yüksek 12 mercek) ===")
    for f, s, al, be, m, h in rows[:12]:
        tag = "  ← çekirdek" if (al, be, m, h) == (1, 1, 19, 't') else ""
        print(f"  çatallı {f}/8  katı {s}/8   v={al}n+{be}a  m={m}  birincil el={h}{tag}")
    return rows

def run_fixed(n_syn, seed, chunk=100_000):
    rng = np.random.default_rng(seed)
    hs = np.zeros(9, np.int64); hf = np.zeros(9, np.int64); hf_t = np.zeros(9, np.int64)
    done = 0; t0 = time.time()
    while done < n_syn:
        b = min(chunk, n_syn - done); done += b
        P = rng.permuted(np.tile(A, (b, 1)), axis=1)
        s_t, f_t = score(P, 1, 1, 19, True)
        s_u, f_u = score(P, 1, 1, 19, False)
        hs += np.bincount(s_t, minlength=9)
        hf_t += np.bincount(f_t, minlength=9)
        hf += np.bincount(np.maximum(f_t, f_u), minlength=9)
    print(f"\n=== SABİT MERCEK (1,1,19): {n_syn:,} sentetik, {time.time()-t0:.0f}s ===")
    def show(tag, h):
        tot = h.sum()
        print(f"  {tag}: " + "  ".join(f"{i}:{int(c):,}" for i, c in enumerate(h) if c) +
              f"   | ≥7: {int(h[7:].sum()):,} ({h[7:].sum()/tot:.2e})  8/8: {int(h[8]):,} ({h[8]/tot:.2e})")
    show("katı (yazarın ölçütleri, t-eli)      ", hs)
    show("çatallı, t-eli                        ", hf_t)
    show("çatallı, iki el (maks)                ", hf)

def run_free(n_syn, seed, chunk=5_000):
    rng = np.random.default_rng(seed)
    hs = np.zeros(9, np.int64); hf = np.zeros(9, np.int64)
    done = 0; t0 = time.time()
    while done < n_syn:
        b = min(chunk, n_syn - done); done += b
        P = rng.permuted(np.tile(A, (b, 1)), axis=1)
        bs = np.zeros(b, np.int8); bf = np.zeros(b, np.int8)
        for al, be in FORMS:
            for m in MODS:
                for prim in (True, False):
                    s, f = score(P, al, be, m, prim)
                    if prim: np.maximum(bs, s, out=bs)       # katı: yalnız t-eli (kesif17 ile aynı)
                    np.maximum(bf, f, out=bf)
        hs += np.bincount(bs, minlength=9); hf += np.bincount(bf, minlength=9)
        print(f"  {done:,}/{n_syn:,}  katı: {dict((i,int(c)) for i,c in enumerate(hs) if c)}  "
              f"çatallı: {dict((i,int(c)) for i,c in enumerate(hf) if c)}  [{time.time()-t0:.0f}s]", flush=True)
    print(f"\n=== SERBEST MERCEK (304 mercek × 2 el): {n_syn:,} sentetik ===")
    for tag, h in (("katı (kesif17 tekrarı)", hs), ("çatallı              ", hf)):
        tot = h.sum()
        print(f"  {tag}: " + "  ".join(f"{i}:{int(c):,}" for i, c in enumerate(h) if c) +
              f"   | ≥7: {int(h[7:].sum()):,} ({h[7:].sum()/tot:.2e})  8/8: {int(h[8]):,} ({h[8]/tot:.2e})")

if __name__ == "__main__":
    mode = sys.argv[1] if len(sys.argv) > 1 else "all"
    n = int(sys.argv[2]) if len(sys.argv) > 2 else None
    real_map()
    if mode in ("fixed", "all"): run_fixed(n or 5_000_000, seed=19)
    if mode in ("free", "all"):  run_free(n or 100_000, seed=23)
