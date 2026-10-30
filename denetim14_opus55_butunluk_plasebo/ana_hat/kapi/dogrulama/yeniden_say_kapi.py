# Ana hattın plasebo14 için bağımsız yeniden sayımı (oturumun analiz_b.py'si kullanılmadı): bölüt dosyalarından k_x (derinlik kuralı:
# k(1–4) ≥ 10 → dur, k(1–40) ≥ 10 → dur, yoksa 1–400), p_x = (k+1)/(N+1), Poisson-CP aralığı, R (p ≤ p_Q ya da aralık çakışması) ve p_kal.
# Dilsiz plasebolar (dil dosyasında olmayanlar) p = 1. Kullanım: python3 -I yeniden_say14.py <plasebo14_kökü>
import sys, os, glob, math
KOK = sys.argv[1]
def pois_cdf(k, lam):
    if lam == 0: return 1.0
    t = -lam; s = math.exp(t)
    for i in range(1, k + 1): t += math.log(lam) - math.log(i); s += math.exp(t)
    return min(s, 1.0)
def cp(k, n, a=0.05):
    def kok(f, lo, hi):
        for _ in range(200):
            m = (lo + hi) / 2
            if f(m): hi = m
            else: lo = m
        return (lo + hi) / 2
    hi = kok(lambda l: pois_cdf(k, l) <= a / 2, 0, 10 * k + 50) / n
    lo = 0.0 if k == 0 else kok(lambda l: 1 - pois_cdf(k - 1, l) >= a / 2, 0, 10 * k + 50) / n
    return lo, hi
for S, K, kume, kollar in [(s, k, 'kA', ['A']) for s, k in (('M', 200), ('U', 100), ('L', 100)) if os.path.isdir(os.path.join(KOK, 'tab', s, 'kA', 'd3_0'))]:
    say, nb = {}, {}
    for d in ('d1_0', 'd2_0', 'd3_0'):
        for fn in glob.glob(os.path.join(KOK, 'tab', S, kume, d, 'b*.txt')):
            b = None
            for s in open(fn):
                f = s.split()
                if s.startswith('#'): b = int(f[f.index('bolut') + 1]); nb[b] = int(f[f.index('N') + 1])
                elif f and f[0] == 'D': say.setdefault(f[1], {})[b] = int(f[2])
    for kol in kollar:
        Q = kol + '000'; dilli = [x for x in say if x.startswith(kol) and x[len(kol):].isdigit() and x != Q and len(x) == len(kol) + 3]
        def k_ara(ad, son):
            bs = say[ad]; eks = [b for b in range(1, son + 1) if b not in bs]
            return sum(bs.get(b, 0) for b in range(1, son + 1)), sum(nb[b] for b in range(1, son + 1)), eks
        res = {}
        for ad in dilli + [Q]:
            for son in ((4, 40, 400) if ad != Q else (400,)):
                k, n, e = k_ara(ad, son)
                if k >= 10 or son == 400: break
            if e: print(S, ad, 'EKSİK BÖLÜT', len(e))
            res[ad] = (k, n, (k + 1) / (n + 1), cp(k, n) if k <= 3000 else ((k - 2 * k ** .5) / n, (k + 2 * k ** .5) / n))
        kQ, nQ, pQ, (lQ, hQ) = res[Q]
        R = sorted(x for x in dilli if res[x][2] <= pQ or res[x][3][0] <= hQ)
        enk = sorted(dilli, key=lambda x: res[x][2])[:3]
        print(f'{S} kol {kol}: dilli {len(dilli)}/{K}; Kur\'an k {kQ}/{nQ:.0e} p {pQ:.2e} [{lQ:.1e}, {hQ:.2e}]; en küçük ' +
              ', '.join(f'{x} {res[x][0]}/{res[x][1]:.0e} p {res[x][2]:.2e} [{res[x][3][0]:.1e}, {res[x][3][1]:.1e}]' for x in enk) +
              f'; R = {len(R)} {R}; p_kal = {(1 + len(R))}/{K + 1} = {(1 + len(R)) / (K + 1):.4f}')
