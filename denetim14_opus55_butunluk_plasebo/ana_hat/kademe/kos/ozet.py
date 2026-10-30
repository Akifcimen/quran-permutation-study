# ozet.py — kademeli ölçümün özeti (ONKAYIT §5): birikimli k, kB, p̂, (k+1)/(N+1), %95 aralık (Poisson-kesin; N ≥ 10⁸, p küçük),
# M20 zincir kümeleri, bütünlüklü kayıtların en iyi puanları ve özgün kontrol_b örneklem sınaması.
# Kullanım: python3 -I kos/ozet.py <kademe_kökü>   (kök altında sina_h/<s>/b*.txt = S1, S2/<s>/p*/parca.txt …, S*/ornek_b/<s>/b*.txt)
import glob, math, os, struct, sys
KOK = sys.argv[1]
KADEMELER = ['S1', 'S2', 'S3', 'S4', 'S5']
ESIK = {('M', 'A000'): 18.801347495, ('M', 'Am000'): 15.946826697, ('M', 'Ap000'): 18.801347495,
        ('U', 'A000'): 20.830033303, ('L', 'A000'): 19.997884751}
DILNO = {'M': ['A000', 'Am000', 'Ap000'], 'U': ['A000'], 'L': ['A000']}
KAYIT = struct.Struct('<IfhBB228s')

def pois_cdf(k, m):
    if m == 0: return 1.0
    t = math.exp(-m); s = t
    for i in range(1, k + 1): t *= m / i; s += t
    return min(s, 1.0)

def aralik(k, N):                       # Poisson-kesin %95 (Garwood); k ≤ 5000
    def kok(f, lo, hi):
        for _ in range(200):
            mid = math.sqrt(lo * hi) if lo > 0 else hi / 2
            if f(mid): hi = mid
            else: lo = mid
        return hi
    ust = kok(lambda m: pois_cdf(k, m) <= 0.025, 1e-12, 1e7)
    alt = 0.0 if k == 0 else kok(lambda m: 1 - pois_cdf(k - 1, m) >= 0.025, 1e-12, 1e7)
    return alt / N, ust / N

def satirlar(s, kad):
    if kad == 'S1':
        fs = sorted(glob.glob(os.path.join(KOK, 'sina_h', s, 'b*.txt')))
    else:
        fs = sorted(glob.glob(os.path.join(KOK, kad, s, 'p*', 'parca.txt')))
    bolut = None
    for fn in fs:
        for ln in open(fn):
            if ln.startswith('#'):
                bolut = int(ln.split()[4]); yield ('#', bolut, ln)
            elif ln.startswith('D '):
                _, ad, k, kb = ln.split(); yield ('D', bolut, (ad, int(k), int(kb)))

def kayitlar(s, kad, uzanti):
    if kad == 'S1': fs = glob.glob(os.path.join(KOK, 'sina_h', s, 'b*' + uzanti))
    else: fs = glob.glob(os.path.join(KOK, kad, s, 'p*', 'b*' + uzanti))
    for fn in fs:
        b = int(os.path.basename(fn)[1:-len(uzanti)]); data = open(fn, 'rb').read()
        for o in range(0, len(data), KAYIT.size):
            sira, puan, L, sinif, dil, a = KAYIT.unpack_from(data, o)
            yield b, sira, puan, L, dil

for s in 'MUL':
    print(f'=== sıfır {s}')
    bir = {ad: [0, 0] for ad in DILNO[s]}; N = 0; bolutler = set(); ornek_fark = 0; ornek_n = 0
    for kad in KADEMELER:
        if kad != 'S1' and not glob.glob(os.path.join(KOK, kad, s, 'p*', 'parca.txt')): continue
        bk = {}
        for tur, b, x in satirlar(s, kad):
            if tur == '#':
                if b in bolutler: print('YİNELENEN BÖLÜT', b); sys.exit(2)
                t = x.split(); bolutler.add(b); N += int(t[t.index('N') + 1])
            else:
                ad, k, kb = x; bir[ad][0] += k; bir[ad][1] += kb; bk[(b, ad)] = (k, kb)
        # özgün kontrol_b örneklemi
        for fn in sorted(glob.glob(os.path.join(KOK, kad, 'ornek_b', s, 'b*.txt'))):
            b = int(os.path.basename(fn)[1:-4])
            for ln in open(fn):
                if ln.startswith('D '):
                    _, ad, k, kb = ln.split(); ornek_n += 1
                    if bk.get((b, ad)) != (int(k), int(kb)): ornek_fark += 1; print('ÖRNEK FARKI', kad, s, b, ad, (k, kb), bk.get((b, ad)))
        if len(bolutler) != max(bolutler) or min(bolutler) != 1: print('BÖLÜT KÜMESİ BİTİŞİK DEĞİL'); sys.exit(2)
        print(f'  {kad}: bölüt 1–{max(bolutler)}, N = {N:.3e}')
        for ad in DILNO[s]:
            k, kb = bir[ad]; lo, hi = aralik(k, N)
            print(f'    {ad}: k = {k}  p̂ = {k / N:.3e}  (k+1)/(N+1) = {(k + 1) / (N + 1):.3e}  %95 [{lo:.2e}, {hi:.2e}]  |  kB = {kb} ({kb / N:.2e})')
    print(f'  özgün kontrol_b örneklemi: {ornek_n} satır, fark {ornek_fark}')
    # bütünlüklü kayıtlar ve isabetler (M20 zincir kümeleri)
    tum = []
    for kad in KADEMELER: tum += [(kad,) + r for r in kayitlar(s, kad, '.s')]
    for d, ad in enumerate(DILNO[s]):
        r = sorted([x for x in tum if x[5] == d], key=lambda x: -x[3])
        es = ESIK[(s, ad)]; isabet = [x for x in r if x[3] >= es - 1e-6]
        kume = 0
        if s == 'M':
            gor = []
            for x in sorted(isabet, key=lambda x: (x[1], x[2])):
                if not any(g[0] == x[1] and g[1] % 64 == x[2] % 64 and abs(g[1] - x[2]) <= 96000 for g in gor): kume += 1
                gor.append((x[1], x[2]))
        else: kume = len(isabet)
        print(f'  {ad}: bütünlüklü kayıt {len(r)}, isabet {len(isabet)} (küme {kume}); en iyi puanlar (eşik {es:.3f}):',
              ', '.join(f'{x[3]:.2f}@L{x[4]}' for x in r[:8]))
