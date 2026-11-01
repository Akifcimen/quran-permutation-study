# §5.3: ortak plasebolar (ve A000) için bölüt başına "D ad k kB" satırları K×10 ile K×100'de aynı mı (ikisinde de sayılmış her (dil, bölüt) çifti)
import glob, os, sys
def oku(kok, S):
    r = {}
    for d in glob.glob(os.path.join(kok, 'tab', S, 'kA', 'd[1-5]_*')):
        for fn in glob.glob(os.path.join(d, 'b*.txt')):
            b = None
            for s in open(fn):
                f = s.split()
                if s.startswith('#'): b = int(f[f.index('bolut') + 1])
                elif f and f[0] == 'D': r[(f[1], b)] = (f[2], f[3])
    return r
for S in 'MLU':
    a = oku(os.path.expanduser('~/opus5.5/k10/sonuc'), S); b = oku(os.path.expanduser('~/opus5.5/k100/sonuc'), S)
    ort = set(a) & set(b); fark = [x for x in ort if a[x] != b[x]]
    dil = len(set(x[0] for x in ort)); print(f'{S}: ortak (dil, bölüt) {len(ort)} ({dil} dil), fark {len(fark)} {sorted(fark)[:3]}')
