# yeniden_say_k100.py — K×100 çalışmasının bağımsız yeniden sayımı (ONKAYIT §3). yeniden_say_k10.py'den türetildi; farklar:
#   düzey 5 (bölüt 4.001–40.000); dört ölçü: R₁ (birincil, Kur'an 10⁸), R_derin (d4'e kadar, Kur'an 10¹⁰), R_eşit (d5'e kadar, Kur'an 10¹⁰),
#   R_nokta (k/N ≤ Kur'an'ın k/N'si); π̂ = R_nokta/K; Kur'an'ın (A000) bölüt 1–40.000 sayımı da basılır (doğrulama §5.4 için).
# Bir dilin sayıldığı aralıkta eksik bölüt varsa durur. Bir dil bir bölütte iki kez sayılmışsa durur.
# Kullanım: python3 -I yeniden_say_k100.py <kök> <S> <K> <kur'an_p> <kur'an_alt> <kur'an_üst> [--liste R1|Rderin <çıktı>]
import sys, os, glob, math
A = sys.argv[1:]; LISTE = None
if '--liste' in A: i = A.index('--liste'); LISTE = (A[i + 1], A[i + 2]); A = A[:i] + A[i + 3:]
KOK, S, K = A[0], A[1], int(A[2]); pQd, lQd, hQd = (float(x) for x in A[3:6])
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
say, nb = {}, {}
for d in sorted(glob.glob(os.path.join(KOK, 'tab', S, 'kA', 'd[1-5]_*'))):
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
def birincil(ad):
    for son in (4, 40, 400):
        k, n = k_ara(ad, son)
        if k >= 10 or son == 400: return k, n                      # derinlik kuralı; gereken düzey eksikse k_ara durur
def derin4(ad): return k_ara(ad, 4000) if 401 in say[ad] else birincil(ad)
def en_derin(ad): return k_ara(ad, 40000) if 4001 in say[ad] else derin4(ad)
def kayit(k, n): return (k, n, (k + 1) / (n + 1))
def alt_le(k, n, h):          # Garwood alt ucu ≤ h ⟺ P(X ≥ k | λ = h·n) ≥ 0,025 (alt uç λ'da artan; bisection'sız, kesin)
    if k == 0: return True
    lam = h * n
    if k - 1 > lam + 40 * math.sqrt(lam) + 200: return False      # P(X ≥ k) < 1e-300 düzeyinde
    return 1 - pois_cdf(k - 1, lam) >= 0.025
r1 = {x: kayit(*birincil(x)) for x in dilli}; rd = {x: kayit(*derin4(x)) for x in dilli}; re_ = {x: kayit(*en_derin(x)) for x in dilli}
kQ8, nQ8 = k_ara(Q, 400); pQ8 = (kQ8 + 1) / (nQ8 + 1); lQ8, hQ8 = cp(kQ8, nQ8)
def R(tab, pQ, hQ): return sorted(x for x in dilli if tab[x][2] <= pQ or alt_le(tab[x][0], tab[x][1], hQ))
R1 = R(r1, pQ8, hQ8); Rd = R(rd, pQd, hQd); Re = R(re_, pQd, hQd)
Rn = sorted(x for x in dilli if re_[x][0] / re_[x][1] <= pQd)
if LISTE:
    with open(LISTE[1], 'w') as f: f.write('\n'.join({'R1': R1, 'Rderin': Rd}[LISTE[0]]) + '\n')
def pk(r): return f'{1 + len(r)}/{K + 1} = {(1 + len(r)) / (K + 1):.5f}'
print(f'{S}: dilli {len(dilli)}/{K}')
print(f'  Kur\'an 10⁸: k {kQ8}/{nQ8:.0e} p {pQ8:.2e} [{lQ8:.1e}, {hQ8:.2e}]; derin (argüman) p {pQd:.2e} [{lQd:.1e}, {hQd:.2e}]')
if 4001 in say.get(Q, {}):
    kQ, nQ = k_ara(Q, 40000); print(f'  Kur\'an bu çalışmada bölüt 1–40.000: k {kQ}/{nQ:.0e} = {kQ / nQ:.2e} (doğrulama §5.4: kapı Gc sayımıyla aynı olmalı)')
enk = sorted(dilli, key=lambda x: re_[x][2])[:10]
print('  en küçük 10 (en derin): ' + ', '.join(f'{x} {re_[x][0]}/{re_[x][1]:.0e} p {re_[x][2]:.2e} [{cp(re_[x][0], re_[x][1])[0]:.1e}, {cp(re_[x][0], re_[x][1])[1]:.1e}]' for x in enk))
print(f'  R₁ = {len(R1)}; p_kal {pk(R1)}')
print(f'  R_derin = {len(Rd)} {Rd}; p_kal {pk(Rd)}')
print(f'  R_eşit = {len(Re)} {Re}; p_kal {pk(Re)}')
lo, hi = cp(len(Rn), K)
print(f'  R_nokta = {len(Rn)} {Rn}; π̂ = {len(Rn)}/{K} = {len(Rn) / K:.2e} [%95 {lo:.1e}, {hi:.1e}]')
print('  okuma (R_eşit): ' + ('aşar' if not Re else ('sık' if len(Re) / K >= 0.05 else 'arada')))
