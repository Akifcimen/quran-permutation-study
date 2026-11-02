# duz_oku.py — düz histogram kestiriminin (ölçüm 2; ONKAYIT_EK) okuması: tekrarların birleştirilmesi, kalite şartı, doğrulama (D1, D2)
# ve ana okuma (R, p_kal, menü). Kullanım:
#   python3 -I duz_oku.py dogrula <tab> <S> <duz_çıktısı[,…]> <secim_D1.txt> <secim_D2.txt>
#   python3 -I duz_oku.py oku <tab> <S> <K> <duz_çıktısı>...
# DUZ_HARIC=1 ortam değişkeniyle düzlük, olayın hiçbir tekrarında ziyaret edilmemiş (ulaşılamayan) düzeyler dışarıda bırakılarak hesaplanır
# (beyanlı yorum; ön-kayıt metni bütün düzeylerin ulaşılabilir olduğunu varsaymıştı). Varsayılan: metnin harfi (bütün düzeyler).
import sys, os, glob, math
T3 = 3.182   # t(0,975; 3 sd), dört tekrar
N4 = 39600 * 250000; ARALIK = {1: (1, 4), 2: (5, 40), 3: (41, 400), 4: (401, 40000)}
def sayimlar(TD):
    k = {}
    for j in (1, 2, 3, 4):
        for fn in glob.glob(os.path.join(TD, f'd{j}', 'b*.txt')):
            for s in open(fn):
                if s.startswith('D '): w = s.split(); k.setdefault(w[1], [0, 0, 0, 0])[j - 1] += int(w[2])
    return k
def olaylar(TD):
    o = {}
    for s in open(os.path.join(TD, 'olaylar.txt')):
        w = s.split(); o[w[0]] = dict(hed=int(w[1]), L=int(w[2]), S=float(w[8]), bilgi=float(w[9]), n=int(w[10]))
    return o
def derin(kk, hed):   # ONKAYIT §5 derinlik kuralı: (k, N)
    c = 0
    for j, n in ((1, 1e6), (2, 1e7), (3, 1e8)):
        c += kk[j - 1]
        if c >= 10 and hed: return c, n
    return c + kk[3], 1e10
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
def pois_q(lam, q):   # Poisson nicel değeri
    k, c = 0, math.exp(-lam) if lam < 700 else 0.0
    if lam >= 700: return max(0, int(lam + math.sqrt(lam) * (2.576 if q > 0.5 else -2.576)))
    while c < q: k += 1; c += math.exp(-lam + k * math.log(lam) - math.lgamma(k + 1))
    return k
HARIC = os.environ.get('DUZ_HARIC') == '1'
def duz_oku(dosyalar):
    r = {}
    for fn in dosyalar:
        for s in open(fn):
            if s.startswith('#') or not s.strip(): continue
            w = s.split(); ad, rep, p = w[0], int(w[2]), float(w[4]); asama = int(w[8])
            V = [int(x) for x in s.split('| V')[1].split('|')[0].split()]
            r.setdefault(ad, {})[rep] = dict(p=p, asama=asama, V=V)
    for ad, reps in r.items():
        nl = len(next(iter(reps.values()))['V'])
        gor = [d for d in range(nl) if any(x['V'][d] for x in reps.values())] if HARIC else list(range(nl))
        for x in reps.values():
            V = [x['V'][d] for d in gor]; x['duz'] = min(V) / (sum(V) / len(V)) if V and sum(V) else 0.0
            x['iyi'] = x['asama'] == 1 and x['duz'] >= 0.3 and x['p'] > 0
    return r
def birlestir(reps):   # §2: r = 1…4 için tekrar r iyiyse o, değilse yeniden koşusu r+4 iyiyse o; dördü toplanmazsa None
    iyi = []
    for k in (1, 2, 3, 4):
        x = reps.get(k) if reps.get(k, {}).get('iyi') else reps.get(k + 4)
        if x is None or not x['iyi']: return None
        iyi.append(x['p'])
    m = sum(iyi) / 4; lg = [math.log(x) for x in iyi]; ml = sum(lg) / 4
    sd = math.sqrt(sum((x - ml) ** 2 for x in lg) / 3); f = math.exp(T3 * sd / 2)
    return m, m / f, m * f
def dogrula(TD, S, fn, d1, d2):
    k = sayimlar(TD); o = olaylar(TD); r = duz_oku(fn.split(',')); sonuc = {}
    for ad_dos, ad in ((d1, 'D1'), (d2, 'D2')):
        sec = [s.split()[0] for s in open(ad_dos) if s.strip()]; sat = []; eksik = 0
        for a in sec:
            b = birlestir(r.get(a, {}))
            if b is None: eksik += 1; continue
            m, lo, hi = b; kk = k.get(a, [0, 0, 0, 0])
            if ad == 'D1': kd, nd = kk[3], N4
            else: kd, nd = derin(kk, 1)
            sat.append((a, kd, nd, m, lo, hi))
        lr = sorted(math.log2((kd + 0.5) / (m * nd + 0.5)) for a, kd, nd, m, lo, hi in sat)
        ic = sum(1 for a, kd, nd, m, lo, hi in sat if pois_q(lo * nd, 0.005) <= kd <= pois_q(hi * nd, 0.995))
        top_k = sum(x[1] for x in sat); top_m = sum(x[3] * x[2] for x in sat)
        med = lr[len(lr) // 2] if lr else float('nan')
        print(f'{S} {ad}: {len(sat)} olay (kestirilemeyen {eksik}); log₂(sayım/kestirim) medyanı {med:+.2f} bit; %10 {lr[len(lr) // 10]:+.2f}, %90 {lr[9 * len(lr) // 10]:+.2f}; '
              f'Σ sayım / Σ kestirim = {top_k}/{top_m:.1f} = {top_k / top_m:.3f}; %99 Poisson aralığında {ic}/{len(sat)} ({ic / max(1, len(sat)):.0%})')
        sonuc[ad] = (med, top_k / top_m if top_m else float('nan'), ic / max(1, len(sat)), eksik)
        if ad == 'D1':
            sira = sorted(sat, key=lambda x: x[3])[: len(sat) // 3]
            lr3 = sorted(math.log2((kd + 0.5) / (m * nd + 0.5)) for a, kd, nd, m, lo, hi in sira)
            m3 = lr3[len(lr3) // 2] if lr3 else float('nan'); print(f'    en nadir üçte bir ({len(sira)} olay): medyan {m3:+.2f} bit'); sonuc['D1_3'] = m3
    return sonuc
def oku(TD, S, K, dosyalar):
    k = sayimlar(TD); o = olaylar(TD); r = duz_oku(dosyalar); pk = {}
    print(f'{S}: K = {K}')
    for t in (1, 2, 3):
        Q = f'A000.{t}'; bQ = birlestir(r.get(Q, {})) or (birlestir(r.get('A000.2', {})) if t == 3 else None)
        if bQ is None: print(f'  O{t}: Kur\'an kestirilemedi'); continue
        mQ, loQ, hiQ = bQ; pl = [a for a in o if a.endswith(f'.{t}') and a != Q]
        R, Ra, Ru, kes, cozul = [], [], [], 0, 0
        for a in pl:
            kk, n = derin(k.get(a, [0, 0, 0, 0]), 1)
            if kk >= 10 and kk / n > 100 * hiQ: cozul += 1; continue   # hız: doğrudan sayım Kur'an'dan açıkça sık; aralık hesabı gereksiz
            lo_d, hi_d = cp(kk, n)
            if kk >= 10 or lo_d > hiQ:   # doğrudan sayım yeter (çözülmüş ya da alt sınırı Kur'an'ın üst sınırının üstünde)
                p, lo, hi = kk / n, lo_d, hi_d; cozul += 1
            else:
                b = birlestir(r.get(a, {}))
                if b is None: Ru.append(a); continue   # kestirilemedi: Kur'an aleyhine üst sınıra girer
                p, lo, hi = b; kes += 1
            if p <= mQ: R.append(a)
            if hi <= loQ: Ra.append(a)
            if lo <= hiQ: Ru.append(a)
        pk[t] = (1 + len(R)) / (K + 1)
        print(f'  O{t}: Kur\'an {mQ:.2e} [{loQ:.2e}, {hiQ:.2e}]; doğrudan {cozul}, kestirim {kes}; R = {len(R)} [{len(Ra)}, {len(Ru)}]; '
              f'p_kal = {1 + len(R)}/{K + 1} = {pk[t]:.5f} [{(1 + len(Ra)) / (K + 1):.5f}, {(1 + len(Ru)) / (K + 1):.5f}]; R üyeleri: {sorted(R)[:12]}')
    if pk: print(f'  menü: 3 × en küçük p_kal = {min(1.0, 3 * min(pk.values())):.5f}')
if __name__ == '__main__':
    if sys.argv[1] == 'dogrula': dogrula(sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6])
    else: oku(sys.argv[2], sys.argv[3], int(sys.argv[4]), sys.argv[5:])
