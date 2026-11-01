# yeniden_say_k10.py — K×10 plasebo çalışmasının bağımsız yeniden sayımı. Kapı çalışmasının yeniden_say_kapi.py'sinden türetildi; farklar:
#   (1) bir düzeyin bütün bellek partileri okunur (d{düzey}_0, d{düzey}_1, …); kod denetiminin 2. bulgusu;
#   (2) bir dilin gereken aralığında eksik bölüt varsa sayım hata verip durur (N fazla sayılmaz); 3. bulgu;
#   (3) plasebo adları her basamak uzunluğunda kabul edilir (A0001…A2000 değil, dil_b'nin "A%03d"si: A001…A2000);
#   (4) K komut satırından; isteğe bağlı derin düzey d4 (bölüt 401–4000) yalnız orada sayılmış diller için.
# Derinlik kuralı: k(1–4) ≥ 10 → dur, k(1–40) ≥ 10 → dur, k(1–400) ≥ 10 ya da d4 yoksa → 1–400, d4 varsa → 1–4000.
# p_x = (k+1)/(N+1); %95 Poisson-Garwood aralığı; R = p ≤ p_Q ya da aralığı Kur'an'ınkiyle çakışan plasebolar; p_kal = (1+R)/(K+1).
# Kullanım: python3 -I yeniden_say_k10.py <kök> <S> <K> [kur'an_p kur'an_alt kur'an_üst]   (son üçü verilirse Kur'an'ın derin değeri kullanılır)
import sys, os, glob, math
KOK, S, K = sys.argv[1], sys.argv[2], int(sys.argv[3])
QDERIN = tuple(float(x) for x in sys.argv[4:7]) if len(sys.argv) >= 7 else None
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
say, nb = {}, {}
for d in sorted(glob.glob(os.path.join(KOK, 'tab', S, 'kA', 'd[1-4]_*'))):
    for fn in glob.glob(os.path.join(d, 'b*.txt')):
        b = None
        for s in open(fn):
            f = s.split()
            if s.startswith('#'): b = int(f[f.index('bolut') + 1]); nb[b] = int(f[f.index('N') + 1])
            elif f and f[0] == 'D':
                if b in say.setdefault(f[1], {}): sys.exit(f'HATA: {f[1]} bölüt {b} iki kez sayılmış')
                say[f[1]][b] = int(f[2])
Q = 'A000'; dilli = sorted(x for x in say if x.startswith('A') and x[1:].isdigit() and x != Q)
def k_ara(ad, son):
    bs = say[ad]; eks = [b for b in range(1, son + 1) if b not in bs]
    if eks: sys.exit(f'HATA: {ad} için 1–{son} aralığında {len(eks)} eksik bölüt (ilk {eks[:5]})')
    return sum(bs[b] for b in range(1, son + 1)), sum(nb[b] for b in range(1, son + 1))
res = {}
for ad in dilli + [Q]:
    sonlar = (4, 40, 400, 4000) if ad != Q else (400,)
    for son in sonlar:
        if son == 4000 and 401 not in say[ad]: break
        k, n = k_ara(ad, son); kk, nn = k, n
        if k >= 10 or son == sonlar[-1]: break
        if son == 400 and 401 not in say[ad]: break
    res[ad] = (kk, nn, (kk + 1) / (nn + 1), cp(kk, nn) if kk <= 3000 else ((kk - 2 * kk ** .5) / nn, (kk + 2 * kk ** .5) / nn))
kQ, nQ, pQ, (lQ, hQ) = res[Q]
if QDERIN: pQ, lQ, hQ = QDERIN
R = sorted(x for x in dilli if res[x][2] <= pQ or res[x][3][0] <= hQ)
enk = sorted(dilli, key=lambda x: res[x][2])[:10]
print(f'{S}: dilli {len(dilli)}/{K}; Kur\'an p {pQ:.2e} [{lQ:.1e}, {hQ:.2e}]' + (' (derin)' if QDERIN else f' (k {kQ}/{nQ:.0e})'))
print('  en küçük 10: ' + ', '.join(f'{x} {res[x][0]}/{res[x][1]:.0e} p {res[x][2]:.2e} [{res[x][3][0]:.1e}, {res[x][3][1]:.1e}]' for x in enk))
print(f'  R = {len(R)} {R}; p_kal = {1 + len(R)}/{K + 1} = {(1 + len(R)) / (K + 1):.5f}')
