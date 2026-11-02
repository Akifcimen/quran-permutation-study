# olay_say.py — profil çalışmasının bağımsız yeniden sayımı ve okuması (ONKAYIT §5). Kullanım: python3 -I olay_say.py <kök> <S> <K>
# Bölüt dosyalarını (tab/<S>/d<düzey>/b*.txt) okur; her olayın sayımını derinlik kuralıyla alır (k(1–4) ≥ 10 → 1–4; k(1–40) ≥ 10 → 1–40;
# k(1–400) ≥ 10 → 1–400; değilse 1–40.000; Kur'an'ın olayları her zaman 1–40.000). Gereken aralıkta eksik bölüt varsa durur.
# Üç kural (tür 1, 2, 3): R = p ≤ p_Q ya da aralığı çakışan plasebo; R_nokta = k/N ≤ k_Q/N_Q; p_kal = (1+R)/(K+1); menü: 3 × en küçük p_kal.
# Model (betimsel): R_S = S ≥ S_Q olan plasebo; doğrulama: çözülmüş olaylarda (k ≥ 10) −log₂(k/N) ile model bilgisi arasında sıra ilişkisi.
import sys, os, glob, math
KOK, S, K = sys.argv[1], sys.argv[2], int(sys.argv[3])
TD = os.path.join(KOK, 'tab', S)
def pois_cdf(k, lam):
    if lam == 0: return 1.0
    t = -lam; s = math.exp(t)
    for i in range(1, k + 1): t += math.log(lam) - math.log(i); s += math.exp(t)
    return min(s, 1.0)
def cp(k, n, a=0.05):
    if k > 3000: return ((k - 2 * k ** .5) / n, (k + 2 * k ** .5) / n)
    def kok(f, lo, hi):
        for _ in range(200):
            m = (lo + hi) / 2
            if f(m): hi = m
            else: lo = m
        return (lo + hi) / 2
    hi = kok(lambda l: pois_cdf(k, l) <= a / 2, 0, 10 * k + 50) / n
    lo = 0.0 if k == 0 else kok(lambda l: 1 - pois_cdf(k - 1, l) >= a / 2, 0, 10 * k + 50) / n
    return lo, hi
def alt_le(k, n, h):
    if k == 0: return True
    lam = h * n
    if k - 1 > lam + 40 * math.sqrt(lam) + 200: return False
    return 1 - pois_cdf(k - 1, lam) >= 0.025
olay = {}
for s in open(os.path.join(TD, 'olaylar.txt')):
    w = s.split(); olay[w[0]] = dict(hed=int(w[1]), L=int(w[2]), S=float(w[8]), bilgi=float(w[9]), n=int(w[10]))
say, nb, uye = {}, {}, {}
for dz in sorted(glob.glob(os.path.join(TD, 'd[1-4]'))):
    j = int(os.path.basename(dz)[1]); uye[j] = set(s.split()[0] for s in open(os.path.join(dz, 'olaylar.txt')))
    for fn in glob.glob(os.path.join(dz, 'b*.txt')):
        b = None
        for s in open(fn):
            f = s.split()
            if s.startswith('#'): b = int(f[f.index('bolut') + 1]); nb[b] = int(f[f.index('N') + 1]); nolay = int(f[f.index('olay') + 1])
            elif f and f[0] == 'D': say.setdefault(f[1], {})[b] = int(f[2])
        if nolay != len(uye[j]): sys.exit(f'HATA: {fn} olay sayısı {nolay}, düzey listesi {len(uye[j])}')
ARALIK = {1: (1, 4), 2: (5, 40), 3: (41, 400), 4: (401, 40000)}
if os.environ.get('OLAY_ARALIK'): ARALIK = {j + 1: tuple(int(x) for x in a.split('-')) for j, a in enumerate(os.environ['OLAY_ARALIK'].split(','))}   # yalnız sınama
SON = ARALIK[4][1]
def eksik(j):
    b1, b2 = ARALIK[j]; e = [b for b in range(b1, b2 + 1) if b not in nb]
    if e: sys.exit(f'HATA: düzey {j} bölütleri eksik ({len(e)}; ilk {e[:5]})')
for j in uye: eksik(j)
def k_ara(ad, son):
    for j in range(1, 5):
        if ARALIK[j][1] <= son and ad not in uye.get(j, ()): sys.exit(f'HATA: {ad} düzey {j}de sayılmamış')
    bs = say.get(ad, {}); return sum(bs.get(b, 0) for b in range(1, son + 1)), sum(nb[b] for b in range(1, son + 1))
def derin(ad):
    if olay[ad]['hed'] == 0: return k_ara(ad, SON)
    for j in (1, 2, 3):
        k, n = k_ara(ad, ARALIK[j][1])
        if k >= 10: return k, n
    return k_ara(ad, SON)
print(f'{S}: K = {K} taze plasebo; olay {len(olay)}')
pk = {}
for tur in (1, 2, 3):
    adlar = [a for a in olay if a.endswith(f'.{tur}')]; Q = f'A000.{tur}'
    if Q not in olay: print(f'  tür {tur}: Kur\'an\'ın olayı yok'); continue
    kQ, nQ = derin(Q); pQ = (kQ + 1) / (nQ + 1); lQ, hQ = cp(kQ, nQ)
    pl = [a for a in adlar if a != Q]; res = {a: derin(a) for a in pl}
    R = [a for a in pl if (res[a][0] + 1) / (res[a][1] + 1) <= pQ or alt_le(res[a][0], res[a][1], hQ)]
    Rn = [a for a in pl if res[a][0] / res[a][1] <= kQ / nQ]
    SQ = olay[Q]['S']; RS = [a for a in pl if olay[a]['S'] >= SQ]
    pk[tur] = (1 + len(R)) / (K + 1)
    print(f'  tür {tur}: Kur\'an {kQ}/{nQ:.0e} p {pQ:.2e} [{lQ:.1e}, {hQ:.2e}], {olay[Q]["n"]} yüklem, S {SQ:.3f}; olaylı plasebo {len(pl)}')
    enk = sorted(pl, key=lambda a: (res[a][0] + 1) / (res[a][1] + 1))[:8]
    print('    en küçük 8: ' + ', '.join(f'{a} {res[a][0]}/{res[a][1]:.0e} ({olay[a]["n"]} yük.)' for a in enk))
    print(f'    R = {len(R)}; p_kal = {1 + len(R)}/{K + 1} = {pk[tur]:.5f}; R_nokta = {len(Rn)} {Rn[:12]}; okuma: ' + ('aşar' if not R else ('sık' if len(R) / K >= 0.05 else 'arada')))
    cz = [(olay[a]['bilgi'], -math.log2(res[a][0] / res[a][1])) for a in pl if res[a][0] >= 10]
    if len(cz) > 20:
        def sira(v):
            o = sorted(range(len(v)), key=lambda i: v[i]); r = [0] * len(v)
            for i, x in enumerate(o): r[x] = i
            return r
        ra, rb = sira([x for x, _ in cz]), sira([y for _, y in cz]); m = len(cz)
        rho = 1 - 6 * sum((x - y) ** 2 for x, y in zip(ra, rb)) / (m * (m * m - 1))
        fark = sorted(y - x for x, y in cz)
        print(f'    model (betimsel): R_S = {len(RS)} (S ≥ Kur\'an\'ınki); çözülmüş {m} olayda Spearman ρ(bilgi, −log₂ p̂) = {rho:.3f}, (−log₂ p̂ − bilgi) medyanı {fark[m // 2]:+.2f} bit [%10 {fark[m // 10]:+.2f}, %90 {fark[9 * m // 10]:+.2f}]')
    else: print(f'    model (betimsel): R_S = {len(RS)}')
if pk: print(f'  menü: en küçük p_kal × 3 = {min(1.0, 3 * min(pk.values())):.5f}')
