# menu_ozet.py — bölme menüsü (ONKAYIT §8.4) özeti: (d1) Kur'an'ın en iyi (çift, mercek) ve puanı; (d2) her kitap çift seçerken
# (tum) sıra; (d3) t2 menüden herkes için çıkarılınca (t2siz) sıra; (d4) yalnız (t2, n2) (bir) sıra. Ayrıca sonradan S0 sürümleri
# (src/menu_s0 çıktısı varsa: sonuc/menu/<sk>_s0.txt). Sıra p = (k+1)/(N+1), %95 Clopper–Pearson (k/N).
# Kullanım: python3 -I src/menu_ozet.py <sk> [<sk> ...]   (ör. U129 M129 L129)
import os, sys, glob, re
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); sys.path.insert(0, os.path.join(KOK, 'src'))
from rapor_tablo import g, cp
AD = {'U': 'Tekdüze', 'M': 'M20', 'L': 'Mallows'}
def satir(k, N):
    lo, hi = cp(k, N); p = (k + 1) / (N + 1)
    if k == 0: return f'0 / {g(N)} → {g(p)} (tek yanlı %95 üst sınır {g(1 - 0.05 ** (1 / N))})'
    return f'{k} / {g(N)} → {g(p)} [{g(lo)}; {g(hi)}]'
TABLO = os.environ.get('MENU_TABLO') == '1'; SAT = []
_print = print
if TABLO: print = lambda *a, **k: None
for sk in sys.argv[1:]:
    N = 0; k = {'tum': 0, 't2siz': 0, 'bir': 0}; Q = None; nb = 0; hist = {}
    for fn in sorted(glob.glob(f'{KOK}/tab/D_{sk}/b*.say')):
        s = open(fn).read(); nb += 1
        N += int(re.search(r' N (\d+)', s).group(1)); Q = re.search(r'Kuran tum .*', s).group(0)
        for ad in k:
            m = re.search(rf'^{ad} k (\d+) hist (.*)$', s, re.M); k[ad] += int(m.group(1))
            h = [int(x) for x in m.group(2).split()]; hist[ad] = [a + b for a, b in zip(hist.get(ad, [0] * len(h)), h)]
    if nb == 0: print(f'### {AD[sk[0]]} ({sk[1:]}): menü bölütü yok (ölçülmedi)'); continue
    print(f'### {AD[sk[0]]} ({sk[1:]}): menü bölütü {nb}, N = {N:,}')
    print(f'- (d1) {Q}')
    for ad, ac in (('tum', '(d2) her kitap 20 çiftten seçer'), ('t2siz', '(d3) t2 herkes için menü dışı (12 çift)'), ('bir', '(d4) yalnız (t2, n2)')):
        top = [(c, n) for c, n in enumerate(hist[ad]) if n][-4:]
        print(f'- {ac}: k = {satir(k[ad], N)}; en üst puanlar {", ".join(f"{c}: {n}" for c, n in top)}')
    fs = f'{KOK}/sonuc/menu/{sk}_s0.txt'
    if os.path.exists(fs):
        L = open(fs).read().split('\n'); q = re.search(r'KURAN S0 tum (\d+) .* t2siz (\d+) bir (\d+)', L[0]); qt, qs, qb = map(int, q.groups())
        d = {}
        for l in L[1:]:
            x = l.split()
            if len(x) < 10 or x[0] == 'DENETIM': continue
            d[(x[0], x[1])] = (int(x[7]), int(x[8]), int(x[9]))
        kt = sum(1 for v in d.values() if v[0] >= qt); ks = sum(1 for v in d.values() if v[1] >= qs); kb = sum(1 for v in d.values() if v[2] >= qb)
        print(f'- sonradan S0 (Kur\'an S0: tum {qt}, t2siz {qs}, bir {qb}; {len(d):,} tekil aday): tum {satir(kt, N)}; t2siz {satir(ks, N)}; bir {satir(kb, N)}')
    else: kt = ks = kb = None
    SAT.append((sk, N, k, kt, ks, kb))
# --- rapor tablosu kipi: MENU_TABLO=1 ise yukarıdaki dökümün yerine tek bir markdown tablosu basılır (rapor_kur.py kullanır)
if TABLO:
    _print('| Sıfır (N) | (d2) her kitap 20 çiftten seçer (Kur\'an 11) | (d3) t2 menü dışı (Kur\'an 9) | (d4) yalnız (t2, n2) (Kur\'an 11) | S0 (d2) | S0 (d3) | S0 (d4) |')
    _print('|---|---|---|---|---|---|---|')
    for sk, N, k, kt, ks, kb in SAT:
        f = lambda x: '—' if x is None else satir(x, N)
        _print(f'| {AD[sk[0]]} ({g(N)}) | {satir(k["tum"], N)} | {satir(k["t2siz"], N)} | {satir(k["bir"], N)} | {f(kt)} | {f(ks)} | {f(kb)} |')
