# istat.py — plasebo12'nin src/analiz.py'sinden AYNEN kopyalanmış istatistik işlevleri (Clopper–Pearson, tek yanlı üst sınır, GPD ve
# normal model, model-tabanlı fark). Değiştirilmedi; plasebo14'ün analizi (src/analiz_b.py) bunları içe aktarır.
import math
import numpy as np
BN = 250000

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


