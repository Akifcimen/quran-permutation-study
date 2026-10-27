# ozet.py — üretim çıktılarının özeti (ONKAYIT §4, §5): sayım puanı sırası (referans ∪ sınama), bilgi puanı sırası (sınama),
# geçici/nihai Kur'an eşikleriyle. Kullanım: python3 -I src/ozet.py [sıfırkodeks ...]  (ör. U129 M129)
import os, sys, glob, math, struct
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
NV, HB = 8, 1200

def say_oku(fn):
    satirlar = open(fn).read().strip().split('\n'); w = satirlar[0].split()
    d = {'N': int(w[w.index('N') + 1]), 'sure': float(w[w.index('sure_s') + 1])}
    for s in satirlar[1:]:
        t = s.split()
        if t[0].startswith('kapi') and 'hist' in t:            # referans: kapiG k(C>=C_Q) x hist ...
            g = int(t[0][4:]); d.setdefault('k', {})[g] = int(t[2]); d.setdefault('hist', {})[g] = [int(x) for x in t[4:]]
        elif t[0] == 'kapi':                                     # sınama: kapi g CQ c k x
            d.setdefault('k', {})[int(t[1])] = int(t[5])
        elif t[0] == 'degiske':
            d.setdefault('v', {})[int(t[1])] = dict(SQ=float(t[3]), alt=int(t[5]), nokta=int(t[7]), ust=int(t[9]))
    return d

def cp(k, n, a=0.05):
    """Clopper–Pearson iki yanlı %95 (beta yerine ikiye bölme ile)."""
    from math import lgamma, exp, log
    def bt(x, k, n):  # P(Bin(n,x) >= k)
        if k <= 0: return 1.0
        s = 0.0
        for j in range(k):
            s += exp(lgamma(n + 1) - lgamma(j + 1) - lgamma(n - j + 1) + j * log(x) + (n - j) * math.log1p(-x)) if 0 < x < 1 else (1.0 if j == 0 and x == 0 else 0.0)
        return 1 - s
    def kok(f, lo, hi):
        for _ in range(100):
            m = (lo * hi) ** 0.5 if lo > 0 else (lo + hi) / 2
            if f(m) > 0: hi = m
            else: lo = m
        return (lo * hi) ** 0.5 if lo > 0 else (lo + hi) / 2
    alt = 0.0 if k == 0 else kok(lambda x: bt(x, k, n) - a / 2, 1e-300, k / n)
    ust = 1 - (a / 2) ** (1 / n) if k == 0 else kok(lambda x: (a / 2) - (1 - bt(x, k + 1, n)), k / n, 1.0)
    return alt, ust

def ozet(ad):
    s, kod = ad[0], int(ad[1:])
    R = [say_oku(f) for f in sorted(glob.glob(os.path.join(KOK, 'tab', f'R_{ad}', '*.say')))]
    T = [say_oku(f) for f in sorted(glob.glob(os.path.join(KOK, 'tab', f'T_{ad}', '*.say')))]
    out = [f'### {ad}: referans bölüt {len(R)} (N = {sum(r["N"] for r in R):,}), sınama bölüt {len(T)} (N = {sum(t["N"] for t in T):,}); '
           f'süre {(sum(r["sure"] for r in R) + sum(t["sure"] for t in T)) / 3600:.2f} çekirdek-saat']
    kapi_ad = ['G', 'GE', 'Gc', 'G0']
    for g in range(4):
        kR = sum(r['k'][g] for r in R); kT = sum(t['k'].get(g, 0) for t in T)
        NR = sum(r['N'] for r in R); NT = sum(t['N'] for t in T); N = NR + NT; k = kR + kT
        if N == 0: continue
        lo, hi = cp(k, N)
        out.append(f'- sayım (kapı {kapi_ad[g]}): k = {k} (referans {kR}, sınama {kT}) / N = {N:,} → p = {(k + 1) / (N + 1):.3g}  [k/N %95: {lo:.2g}–{hi:.2g}]')
    if T and 'v' in T[0]:
        NT = sum(t['N'] for t in T)
        for v in range(NV):
            SQ = T[0]['v'][v]['SQ']; ka = sum(t['v'][v]['alt'] for t in T); kn = sum(t['v'][v]['nokta'] for t in T); ku = sum(t['v'][v]['ust'] for t in T)
            out.append(f'- bilgi değişke {v} (kapı {"G" if v >= 4 else "yok"}, önsel P{v & 3}): geçici S_Q = {SQ:.3f} bit; k_alt {ka}, k_nokta {kn}, k_üst {ku} / N_T = {NT:,} '
                       f'→ p_nokta = {(kn + 1) / (NT + 1):.3g} [{(ka + 1) / (NT + 1):.3g}, {(ku + 1) / (NT + 1):.3g}]')
    return '\n'.join(out)

if __name__ == '__main__':
    adlar = sys.argv[1:] or ['U129', 'M129', 'L129', 'J129', 'U127', 'M127', 'L127']
    for a in adlar:
        if glob.glob(os.path.join(KOK, 'tab', f'R_{a}', '*.say')):
            print(ozet(a)); print()
