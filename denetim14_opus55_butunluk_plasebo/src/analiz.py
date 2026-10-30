# analiz.py — ONKAYIT §5.3–§10'un uygulaması: derinlik kuralı, p_x ve Clopper–Pearson, kalibre p ve okuma kuralı, model-tabanlı
# fark (GPD birincil, normal ikincil), ikinciller (el dili kazancı, sınıf şartlı ayrışma, P2), tahmin hükümleri (yalnız M20).
# Kullanım: python3 -I src/analiz.py <sifir U|M|L>   → sonuc/<sifir>/analiz.json ve analiz.txt
import sys, os, math, json, glob, struct
import numpy as np

KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(KOK, 'src'))
import kutuphane as KU
import agac as AG

S = sys.argv[1]
K = {'M': 200, 'U': 100, 'L': 100}[S]
TD = os.path.join(KOK, 'tab', S)
OUT = os.path.join(KOK, 'sonuc', S)
os.makedirs(OUT, exist_ok=True)
BN = 250000
SEVIYE = [(1, 4), (5, 40), (41, 400)]          # bölüt aralıkları: düzey 1, 2, 3
NSEV = [4 * BN, 40 * BN, 400 * BN]
TUM = KU.TUM


# ---------------- Clopper–Pearson (iki yanlı %95) ----------------
def _logpmf(N, j, x):
    return math.lgamma(N + 1) - math.lgamma(j + 1) - math.lgamma(N - j + 1) + j * math.log(x) + (N - j) * math.log1p(-x)


def _kuyruk_ust(N, k, x):
    """P(Bin(N, x) ≥ k)"""
    if k <= 0: return 1.0
    if x <= 0: return 0.0
    if x >= 1: return 1.0
    mu = N * x; sd = math.sqrt(max(N * x * (1 - x), 1.0)); w = int(60 * sd + 200)
    a = max(k, 0); b = min(N, max(k, int(mu)) + w)
    if b < a: return 0.0
    js = np.arange(a, b + 1, dtype=np.float64)
    from math import lgamma
    lg = np.vectorize(lgamma)
    lp = (lgamma(N + 1) - lg(js + 1) - lg(N - js + 1) + js * math.log(x) + (N - js) * math.log1p(-x))
    m = lp.max(); return float(min(1.0, math.exp(m) * np.exp(lp - m).sum()))


def _kuyruk_alt(N, k, x):
    """P(Bin(N, x) ≤ k)"""
    if k >= N: return 1.0
    return 1.0 - _kuyruk_ust(N, k + 1, x)


def cp(k, N, alfa=0.05):
    if N <= 0: return (0.0, 1.0)
    def bul(f, hedef, lo, hi):
        for _ in range(200):
            mid = math.sqrt(lo * hi) if lo > 0 else hi / 2
            if f(mid) < hedef: lo = mid
            else: hi = mid
            if hi / max(lo, 1e-300) < 1 + 1e-10: break
        return math.sqrt(lo * hi) if lo > 0 else hi
    if k == 0: alt = 0.0
    else: alt = bul(lambda x: _kuyruk_ust(N, k, x), alfa / 2, 1e-300, k / N)
    if k == N: ust = 1.0
    else:
        # P(X ≤ k | x) azalan: hedef α/2
        lo, hi = max(k / N, 1e-300), 1.0
        for _ in range(200):
            mid = math.sqrt(lo * hi)
            if _kuyruk_alt(N, k, mid) > alfa / 2: lo = mid
            else: hi = mid
            if hi / lo < 1 + 1e-10: break
        ust = math.sqrt(lo * hi)
    return (alt, ust)


def ust_tek(N, alfa=0.05):
    return 1 - alfa ** (1.0 / N)


# ---------------- girdiler ----------------
def diller_oku(fn):
    L = []
    for s in open(fn):
        w = s.split()
        if not w: continue
        ad, hed, ons, Ld, n = w[0], int(w[1]), int(w[2]), int(w[3]), int(w[4])
        g = [int(x) for x in w[5:5 + n]]
        L.append(dict(ad=ad, hedef=hed, onsel=ons, L=Ld, g=g))
    return L


def ayrinti_oku(fn):
    A = {}
    if not os.path.exists(fn): return A
    for s in open(fn):
        w = s.split()
        A[w[0]] = s.strip()
    return A


def bolut_oku(dizin):
    """b<sıra>.txt → {sıra: {'D': {ad: (k, kc)}, 'E': {h: (k, kc)}, 'sure': s}}"""
    R = {}
    for fn in glob.glob(os.path.join(dizin, 'b*.txt')):
        b = int(os.path.basename(fn)[1:5]); d = {'D': {}, 'E': {}, 'sure': 0.0}
        for s in open(fn):
            w = s.split()
            if not w: continue
            if w[0] == '#':
                if 'sure_s' in w: d['sure'] = float(w[w.index('sure_s') + 1])
                if 'N' in w: assert int(w[w.index('N') + 1]) == BN
            elif w[0] == 'D': d['D'][w[1]] = (int(w[2]), int(w[3]))
            elif w[0] == 'E': d['E'][int(w[1])] = (int(w[2]), int(w[3]))
        R[b] = d
    return R


def esik_oku(dizin):
    E = {'D': {}, 'E': {}}
    fn = os.path.join(dizin, 'esikler.txt')
    if not os.path.exists(fn): return E
    for s in open(fn):
        w = s.split()
        if not w or w[0] == '#': continue
        if w[0] == 'D': E['D'][w[1]] = (int(w[2]), float(w[3]), int(w[4]))
        else: E['E'][int(w[1])] = (float(w[2]), int(w[3]))
    return E


# ---------------- model-tabanlı fark ----------------
def nelder_mead(f, x0, adim=0.3, iter_=4000, tol=1e-10):
    n = len(x0); P = [np.array(x0, float)]
    for i in range(n):
        y = np.array(x0, float); y[i] += adim; P.append(y)
    F = [f(p) for p in P]
    for _ in range(iter_):
        o = np.argsort(F); P = [P[i] for i in o]; F = [F[i] for i in o]
        if abs(F[-1] - F[0]) < tol: break
        c = np.mean(P[:-1], axis=0); xr = c + (c - P[-1]); fr = f(xr)
        if fr < F[0]:
            xe = c + 2 * (c - P[-1]); fe = f(xe)
            if fe < fr: P[-1], F[-1] = xe, fe
            else: P[-1], F[-1] = xr, fr
        elif fr < F[-2]: P[-1], F[-1] = xr, fr
        else:
            xc = c + 0.5 * (P[-1] - c); fc = f(xc)
            if fc < F[-1]: P[-1], F[-1] = xc, fc
            else:
                for i in range(1, n + 1): P[i] = P[0] + 0.5 * (P[i] - P[0]); F[i] = f(P[i])
    o = int(np.argmin(F)); return P[o], F[o]


def gpd_nll(th, z, zc):
    ls, xi = th; s = math.exp(ls)
    def lg(zz):
        t = 1 + xi * zz / s
        if np.any(t <= 0): return None
        return np.log(t)
    v = 0.0
    if len(z):
        L = lg(z)
        if L is None: return 1e30
        v += len(z) * ls + (1 + 1 / xi) * L.sum() if abs(xi) > 1e-8 else len(z) * ls + z.sum() / s
    if len(zc):
        L = lg(zc)
        if L is None: return 1e30
        v += (1 / xi) * L.sum() if abs(xi) > 1e-8 else zc.sum() / s
    return v


def gpd_uyum(z, zc):
    best = None
    for xi0 in (-0.3, 0.1, 0.5):
        th, f = nelder_mead(lambda t: gpd_nll(t, z, zc), [math.log(max(np.mean(np.concatenate([z, zc])) if len(z) + len(zc) else 1, 1e-3)), xi0])
        if best is None or f < best[1]: best = (th, f)
    return math.exp(best[0][0]), best[0][1]


def gpd_sf(z, s, xi):
    if abs(xi) < 1e-8: return math.exp(-z / s)
    t = 1 + xi * z / s
    if t <= 0: return 0.0
    return t ** (-1 / xi)


def model_fark(y, cens, yQ, oran=0.2, nboot=2000, tohum=29102600):
    """y: plasebo −log10 p (dizi), cens: sağdan sansür bayrakları; yQ: Kur'an. GPD (üst %20) birincil; normal ikincil."""
    y = np.asarray(y, float); cens = np.asarray(cens, bool); n = len(y)
    o = np.argsort(y, kind='stable'); ys = y[o]; cs = cens[o]
    m = int(round(oran * n)); u = ys[n - m - 1]
    top = ys[n - m:]; ctop = cs[n - m:]
    z = top[~ctop] - u; zc = top[ctop] - u
    s, xi = gpd_uyum(z, zc)
    P = oran * gpd_sf(yQ - u, s, xi) if yQ > u else None
    rng = np.random.default_rng(tohum); Pb = []
    for _ in range(nboot):
        # parametrik önyükleme: GPD'den m aşım, sansür sınırı aynı
        uu = rng.random(m)
        if abs(xi) < 1e-8: zz = -s * np.log(uu)
        else: zz = s / xi * (uu ** (-xi) - 1)
        ymax = math.log10(400 * BN + 1)
        cc = (u + zz) >= ymax
        zz2 = np.where(cc, ymax - u, zz)
        sb, xb = gpd_uyum(zz2[~cc], zz2[cc])
        Pb.append(oran * gpd_sf(yQ - u, sb, xb))
    Pb = np.array(Pb)
    # normal (sansürlü en çok olabilirlik)
    def nll(th):
        mu, ls = th; sg = math.exp(ls); v = 0.0
        for yy, c in zip(y, cens):
            zz = (yy - mu) / sg
            if c: v -= math.log(max(0.5 * math.erfc(zz / math.sqrt(2)), 1e-300))
            else: v += ls + 0.5 * zz * zz
        return v
    th, _ = nelder_mead(nll, [float(np.mean(y)), math.log(float(np.std(y)) + 1e-6)])
    mu, sg = th[0], math.exp(th[1]); Pn = 0.5 * math.erfc((yQ - mu) / sg / math.sqrt(2))
    return dict(esik_u=float(u), m=m, gpd_sigma=float(s), gpd_xi=float(xi), P_gpd=None if P is None else float(P),
                P_gpd_95=[float(np.quantile(Pb, 0.025)), float(np.quantile(Pb, 0.975))], normal_mu=float(mu), normal_sd=float(sg),
                P_normal=float(Pn), delta_mertebe=float(yQ - y.max()))


# ---------------- ana akış ----------------
def main():
    diller = diller_oku(os.path.join(TD, 'diller.txt'))
    adlar = [d['ad'] for d in diller if d['ad'] != 'EL']
    el = [d for d in diller if d['ad'] == 'EL']
    ayr = ayrinti_oku(os.path.join(TD, 'ayrinti.txt'))
    hay = ayrinti_oku(os.path.join(TD, 'hedef_ayrinti.txt'))
    # bütün bölütler (düzey dizinlerinden)
    B = {}
    for dz in sorted(glob.glob(os.path.join(TD, 'k*'))):
        for b, d in bolut_oku(dz).items():
            if b in B:   # aynı bölüt birden çok dil grubunda koşulmuş olabilir: birleştir
                B[b]['D'].update(d['D']); B[b]['E'].update(d['E']); B[b]['sure'] += d['sure']
            else: B[b] = d
    E = {'D': {}, 'E': {}}
    for dz in sorted(glob.glob(os.path.join(TD, 'k*'))):
        e = esik_oku(dz); E['D'].update(e['D']); E['E'].update(e['E'])

    def say(ad, son):
        k = kc = 0; eksik = []
        for b in range(1, son + 1):
            if b not in B or ad not in B[b]['D']: eksik.append(b); continue
            k += B[b]['D'][ad][0]; kc += B[b]['D'][ad][1]
        return k, kc, eksik

    def say_el(h, son):
        k = kc = 0; eksik = []
        for b in range(1, son + 1):
            if b not in B or h not in B[b]['E']: eksik.append(b); continue
            k += B[b]['E'][h][0]; kc += B[b]['E'][h][1]
        return k, kc, eksik

    def derinlik(fn_say, dogrudan=False):
        """ONKAYIT §5.3: düzey 1 (10⁶) → k < 10 ise düzey 2 (10⁷) → k < 10 ise düzey 3 (10⁸); doğrudan=True → 10⁸"""
        if dogrudan:
            k, kc, ek = fn_say(400); return dict(k=k, kc=kc, N=None if ek else 400 * BN, duzey=3, eksik=ek)
        for i, son in enumerate((4, 40, 400)):
            k, kc, ek = fn_say(son)
            if ek: return dict(k=k, kc=kc, N=None, duzey=i + 1, eksik=ek)
            if k >= 10 or son == 400: return dict(k=k, kc=kc, N=son * BN, duzey=i + 1, eksik=[])
        raise RuntimeError

    sonuc = {'sifir': S, 'K': K, 'hedefler': {}}
    for d in diller:
        if d['ad'] == 'EL': continue
        ad = d['ad']; dog = ad in ('Q', 'QP2')
        r = derinlik(lambda son: say(ad, son), dogrudan=dog)
        hed, esk, snf = E['D'].get(ad, (d['hedef'], None, None))
        r.update(ad=ad, hedef=d['hedef'], onsel=d['onsel'], Ldil=d['L'], dil=[TUM[i] for i in d['g']], esik=esk, sinif=snf,
                 dil_bedeli=AG.dil_bedeli([TUM[i] for i in d['g']]) if d['g'] else math.log2(21))
        if r['N']:
            r['p'] = (r['k'] + 1) / (r['N'] + 1); r['ci'] = list(cp(r['k'], r['N']))
            r['pc'] = (r['kc'] + 1) / (r['N'] + 1) if snf else 1.0
            r['ci_c'] = list(cp(r['kc'], r['N'])) if snf else [1.0, 1.0]
        r['ayrinti'] = ayr.get(ad, ''); r['hedef_ayrinti'] = hay.get(ad, '')
        sonuc['hedefler'][ad] = r
    # el dili (bütün hedefler)
    sonuc['el'] = {}
    if el:
        for h, (esk, snf) in sorted(E['E'].items()):
            r = derinlik(lambda son: say_el(h, son), dogrudan=(h == 0))
            r.update(hedef=h, esik=esk, sinif=snf)
            if r['N']:
                r['p'] = (r['k'] + 1) / (r['N'] + 1); r['ci'] = list(cp(r['k'], r['N']))
                r['pc'] = (r['kc'] + 1) / (r['N'] + 1) if snf else 1.0
            sonuc['el'][h] = r
    H = sonuc['hedefler']
    Q = H.get('Q')
    plas = [H[a] for a in sorted(H) if a not in ('Q', 'QP2')]
    tamam = Q is not None and Q.get('N') and all(x.get('N') for x in plas) and len(plas) == K
    sonuc['tamam'] = bool(tamam)
    if not tamam:
        json.dump(sonuc, open(os.path.join(OUT, 'analiz.json'), 'w'), ensure_ascii=False, indent=1); print('eksik: analiz tamamlanmadı'); return
    # ---- kalibre p (§8.1) ----
    loQ, hiQ = Q['ci']; pQ = Q['p']
    R = [x['ad'] for x in plas if x['p'] <= pQ or x['ci'][0] <= hiQ]
    pkal = (1 + len(R)) / (K + 1)
    oku = 'aşar' if len(R) == 0 else ('sık ulaşır' if len(R) / K >= 0.05 else 'arada')
    y = [-math.log10(x['p']) for x in plas]; cens = [x['k'] == 0 and x['N'] == 400 * BN for x in plas]
    mf = model_fark(y, cens, -math.log10(pQ), oran=0.2)
    sonuc['kalibre'] = dict(pQ=pQ, ciQ=Q['ci'], kQ=Q['k'], NQ=Q['N'], R=len(R), ulasanlar=R, p_kal=pkal, okuma=oku, model=mf)
    # ---- sınıf şartlı (§9 c) ----
    if Q['sinif']:
        Rc = [x['ad'] for x in plas if x['sinif'] and (x['pc'] <= Q['pc'] or x['ci_c'][0] <= Q['ci_c'][1])]
        sonuc['sinif'] = dict(Q_sinif=1, pcQ=Q['pc'], ci_cQ=Q['ci_c'], kcQ=Q['kc'], sinifta_plasebo=sum(1 for x in plas if x['sinif']), Rc=len(Rc),
                              ulasanlar=Rc, pc_kal=(1 + len(Rc)) / (K + 1))
    else:
        sonuc['sinif'] = dict(Q_sinif=0, sinifta_plasebo=sum(1 for x in plas if x['sinif']), not_='Kur\'an kendi dilinde sınıf dışı; sınıf şartlı kalibrasyon tanımsız')
    # ---- el dili kazancı (§9 a) ----
    G = []
    for x in plas:
        e = sonuc['el'].get(x['hedef'])
        if e and e.get('N'): x['p_el'] = e['p']; x['G'] = math.log10(e['p']) - math.log10(x['p']); G.append(x['G'])
    eQ = sonuc['el'].get(0)
    sonuc['el_ozet'] = dict(G_medyan=float(np.median(G)), G_ceyrek=[float(np.quantile(G, 0.25)), float(np.quantile(G, 0.75))],
                            G_min=float(min(G)), G_maks=float(max(G)), pQ_el=eQ['p'] if eQ else None, kQ_el=eQ['k'] if eQ else None,
                            ciQ_el=eQ['ci'] if eQ else None, G_Q=(math.log10(eQ['p']) - math.log10(pQ)) if eQ else None,
                            ortak=[g for g in Q['dil'] if g in KU.HAND])
    # ---- plasebo özeti ----
    ps = np.array([x['p'] for x in plas]); o = np.sort(ps)
    med = math.sqrt(o[K // 2 - 1] * o[K // 2])
    ranks = (86, 115) if K == 200 else (40, 61)
    sonuc['plasebo'] = dict(medyan=med, medyan_95=[float(o[ranks[0] - 1]), float(o[ranks[1] - 1])], min=float(o[0]),
                            min_aralik=[min(x['ci'][0] for x in plas), min(x['ci'][1] for x in plas)],
                            ceyrekler=[float(np.quantile(ps, q)) for q in (0.1, 0.25, 0.5, 0.75, 0.9)],
                            dil_boyu=[len(x['dil']) for x in plas], duzeyler={str(i): sum(1 for x in plas if x['duzey'] == i) for i in (1, 2, 3)})
    # ---- tahmin (yalnız M20; §10) ----
    if S == 'M':
        def hukum(nokta, aralik, a, b):
            if a <= nokta <= b: return 'tuttu'
            if aralik[1] < a or aralik[0] > b: return 'tutmadı'
            return 'karar verilemedi'
        th = {}
        if Q['k'] >= 1: th['p_Q'] = hukum(pQ, Q['ci'], 1e-9, 1e-6)
        else:
            U = ust_tek(Q['N']); th['p_Q'] = 'tutmadı (daha nadir)' if U < 1e-9 else 'karar verilemedi'
        th['p_Q_merkez_uzaklik'] = abs(math.log10(pQ) + 7)
        th['medyan'] = hukum(med, sonuc['plasebo']['medyan_95'], 1e-3, 1e-1)
        th['en_kucuk'] = hukum(float(o[0]), sonuc['plasebo']['min_aralik'], 1e-6, 1e-4)
        th['hepsini_asar'] = 'tuttu' if len(R) == 0 else 'tutmadı'
        parca = [th['p_Q'], th['medyan'], th['en_kucuk'], th['hepsini_asar']]
        if all(p == 'tuttu' for p in parca): th['butun'] = 'tuttu'
        elif not any(p == 'tuttu' for p in parca) and any(p.startswith('tutmadı') for p in parca): th['butun'] = 'tutmadı'
        else: th['butun'] = 'kısmen tuttu'
        sonuc['tahmin'] = th
    sonuc['hesap_sn'] = sum(d['sure'] for d in B.values())
    json.dump(sonuc, open(os.path.join(OUT, 'analiz.json'), 'w'), ensure_ascii=False, indent=1)
    # metin özeti
    with open(os.path.join(OUT, 'analiz.txt'), 'w') as f:
        f.write(f'sıfır {S}, K = {K}\n')
        f.write(f"Kur'an: dil {Q['dil']} (|Λ| = {len(Q['dil'])}), eşik {Q['esik']:.4f} bit, sınıf {Q['sinif']}, k = {Q['k']} / N = {Q['N']:.0f} → p_Q = {pQ:.3e} [{Q['ci'][0]:.2e}; {Q['ci'][1]:.2e}]\n")
        kq = sonuc['kalibre']
        f.write(f"kalibre: R = {kq['R']} → p_kal = {kq['p_kal']:.4f}; okuma: {kq['okuma']}\n")
        f.write(f"model-tabanlı: {json.dumps(kq['model'], ensure_ascii=False)}\n")
        f.write(f"plasebo: {json.dumps(sonuc['plasebo'], ensure_ascii=False)[:600]}\n")
        f.write(f"sınıf: {json.dumps(sonuc['sinif'], ensure_ascii=False)[:600]}\n")
        f.write(f"el: {json.dumps(sonuc['el_ozet'], ensure_ascii=False)}\n")
        if 'tahmin' in sonuc: f.write(f"tahmin: {json.dumps(sonuc['tahmin'], ensure_ascii=False)}\n")
    print(open(os.path.join(OUT, 'analiz.txt')).read())


if __name__ == '__main__':
    main()
