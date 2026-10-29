# Ana hattın bağımsız yeniden sayımı (oturumun analiz.py'si kullanılmadı): bölüt dosyalarından k_x, derinlik kuralı (ONKAYIT §5.3), p_x, Poisson-CP aralığı, R ve p_kal (§8.1).
import sys, os, glob, math, json
KOK = sys.argv[1]  # Kullanım: python3 -I dogrulama/yeniden_say.py .  (bu klasörden; yalnız metin dosyalarını okur)

def pois_cdf(k, lam):
    # P(X <= k), log-uzayda toplam
    if lam == 0: return 1.0
    lt = -lam; s = math.exp(lt); t = lt
    for i in range(1, k + 1):
        t += math.log(lam) - math.log(i); s += math.exp(t)
    return min(s, 1.0)

def cp(k, n, a=0.05):
    # k/n için Poisson yaklaşımıyla iki yanlı Clopper–Pearson (n büyük, k/n küçük)
    def kok(f, lo, hi):
        for _ in range(200):
            m = (lo + hi) / 2
            if f(m): hi = m
            else: lo = m
        return (lo + hi) / 2
    hi = kok(lambda l: pois_cdf(k, l) <= a / 2, 0, 10 * k + 50) / n
    lo = 0.0 if k == 0 else kok(lambda l: 1 - pois_cdf(k - 1, l) >= a / 2, 0, 10 * k + 50) / n
    return lo, hi

for S in ['M', 'U', 'L']:
    say = {}   # ad -> {bölüt: k}
    nb = {}
    for d in ['k1', 'k2', 'k3']:
        for fn in glob.glob(os.path.join(KOK, 'tab', S, d, 'b*.txt')):
            b = None
            for s in open(fn):
                f = s.split()
                if s.startswith('#'):
                    b = int(f[f.index('bolut') + 1]); nb[b] = int(f[f.index('N') + 1])
                elif f[0] == 'D':
                    say.setdefault(f[1], {})[b] = int(f[2])
    diller = [s.split()[0] for s in open(os.path.join(KOK, 'tab', S, 'diller.txt')) if s.strip()]
    plasebo = [x for x in diller if x.startswith('x')]
    def k_ara(ad, son):
        bs = say.get(ad, {})
        eksik = [b for b in range(1, son + 1) if b not in bs]
        return sum(bs.get(b, 0) for b in range(1, son + 1)), sum(nb[b] for b in range(1, son + 1)), eksik
    sonuc = {}
    for ad in diller:
        if ad == 'EL': continue  # el dili kitap başına 'E' satırlarında sayılır (ONKAYIT §9 a); burada yeniden sayılmadı
        if ad.startswith('x'):
            for son in (4, 40, 400):
                k, n, e = k_ara(ad, son)
                if k >= 10 or son == 400: break
        else:
            k, n, e = k_ara(ad, 400)
        if e: print(S, ad, 'EKSİK BÖLÜT', e[:5], len(e))
        # kesin CP yalnız Kur'an'a yakın olanlarda gerekir (k ≤ 2000); ötekilerde normal yaklaşımı yeter
        ci = cp(k, n) if k <= 2000 else ((k - 1.96 * k ** 0.5) / n, (k + 1.96 * k ** 0.5) / n)
        sonuc[ad] = (k, n, (k + 1) / (n + 1), ci)
    kQ, nQ, pQ, (loQ, hiQ) = sonuc['Q']
    R = [x for x in plasebo if sonuc[x][2] <= pQ or sonuc[x][3][0] <= hiQ]
    K = len(plasebo)
    ps = sorted(sonuc[x][2] for x in plasebo)
    med = ps[K // 2] if K % 2 else (ps[K // 2 - 1] + ps[K // 2]) / 2
    en = min(plasebo, key=lambda x: sonuc[x][2])
    print(f"{S}: K={K}  Kur'an k={kQ}/N={nQ:.0e} p_Q={pQ:.3e} [{loQ:.2e}; {hiQ:.2e}]  QP2 p={sonuc['QP2'][2]:.2e}")
    print(f"   en iyi plasebo {en}: k={sonuc[en][0]}/N={sonuc[en][1]:.0e} p={sonuc[en][2]:.3e} [{sonuc[en][3][0]:.2e}; {sonuc[en][3][1]:.2e}]  medyan p={med:.4g}")
    print(f"   R={len(R)} {R} → p_kal={(1 + len(R)) / (K + 1):.4f}")
    a = json.load(open(os.path.join(KOK, 'sonuc', S, 'analiz.json')))['kalibre']
    print(f"   oturum: p_Q={a['pQ']:.3e} k={a['kQ']} R={a['R']} p_kal={a['p_kal']:.4f}  ci={a['ciQ']}")
