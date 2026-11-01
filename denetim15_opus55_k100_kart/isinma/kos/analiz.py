# analiz.py — M20 ısınma tanısının çözümlemesi (ONKAYIT §2–§3). Standart hatalar bölütler arası değişimden (bölütler bağımsız).
# Kullanım: python3 -I analiz.py <sonuc_dizini> <özgün_30000_bölüt_dizini>
import glob, math, os, sys
SON, OZGUN = sys.argv[1], sys.argv[2]
BN, DIL = 250000, 10; M = BN // DIL

def ort_se(x):
    n = len(x); m = sum(x) / n; v = sum((a - m) ** 2 for a in x) / (n - 1); return m, math.sqrt(v / n)

def oran(x, y):   # ortalama(x) / ortalama(y), delta yöntemiyle %95 aralık (bağımsız örneklemler)
    mx, sx = ort_se(x); my, sy = ort_se(y); r = mx / my; s = r * math.sqrt((sx / mx) ** 2 + (sy / my) ** 2)
    return r, r - 1.96 * s, r + 1.96 * s

print('# (A) bölüt içi kayma: dilim başına ortalama S ve B2 oranı (kitap başına)')
A = {}
for N in ('30000', '3000000'):
    fn = os.path.join(SON, f'A_{N}.txt')
    if not os.path.exists(fn): print(N, 'yok'); continue
    rows = [l.split() for l in open(fn) if l.startswith('b ')]
    S = [[int(w[3 + d]) / M for d in range(DIL)] for w in rows]; B = [[int(w[3 + DIL + 1 + d]) / M for d in range(DIL)] for w in rows]
    A[N] = (S, B); print(f'ISINMA {N}: {len(rows)} bölüt')
    for ad, X in (('S', S), ('B2', B)):
        ms = [ort_se([r[d] for r in X]) for d in range(DIL)]
        print(f'  {ad} dilimler: ' + ' '.join(f'{m:.6g}' for m, _ in ms))
        fark = [r[0] - r[DIL - 1] for r in X]; m, s = ort_se(fark)
        print(f'  {ad} ilk − son dilim: {m:.6g} ± {s:.3g}  (z = {m / s if s else float("nan"):.2f})')
if len(A) == 2:
    for i, ad in ((0, 'S'), (1, 'B2')):
        x = [sum(r) / DIL for r in A['3000000'][i]]; y = [sum(r) / DIL for r in A['30000'][i]]
        r, lo, hi = oran(x, y); print(f'  {ad} oranı 3.000.000 / 30.000: {r:.5f} [{lo:.5f}; {hi:.5f}]')

print('# (B) ısınma uzunluğunun sayımlara etkisi (M20, bölüt 1–4000)')
def bolutler(d):
    out = {}
    for fn in glob.glob(os.path.join(d, 'b*.txt')):
        b = int(os.path.basename(fn)[1:-4]); v = {'b2': 0}
        for l in open(fn):
            w = l.split()
            if not w: continue
            if w[0] == '#':
                for i, x in enumerate(w):
                    if x == 'b2': v['b2'] = int(w[i + 1])
            elif w[0] == 'D': v['k_' + w[1]] = int(w[2]); v['kB_' + w[1]] = int(w[3])
        if b <= 4000: out[b] = v
    return out
B30 = bolutler(OZGUN)
for N in ('300000', '3000000'):
    d = os.path.join(SON, f'B_{N}')
    if not os.path.isdir(d): print(N, 'yok'); continue
    BN_ = bolutler(d); ortak = sorted(set(B30) & set(BN_)); print(f'ISINMA {N}: {len(BN_)} bölüt (ortak {len(ortak)})')
    for ad in ('b2', 'kB_Am000', 'kB_A000', 'k_Am000', 'k_A000'):
        x = [BN_[b].get(ad, 0) for b in ortak]; y = [B30[b].get(ad, 0) for b in ortak]
        if sum(y) == 0: continue
        r, lo, hi = oran(x, y); print(f'  {ad}: toplam {sum(x)} / {sum(y)}; oran {r:.4f} [{lo:.4f}; {hi:.4f}]')
