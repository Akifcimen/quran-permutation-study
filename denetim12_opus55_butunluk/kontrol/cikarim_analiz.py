# cikarim_analiz — tarama histogramlarından gerektirme ADAYLARI (karşı-örneği sıfır, desteği ≥ 30) ve sabit yüklemler.
# Yalnız aday kuralları ve sabit-doğru yüklemleri basar; sıklık/puan basmaz.
import sys, numpy as np
sys.path.insert(0, 'src'); import dil
NP = 20; M = 1 << NP
def oku(fn):
    d = np.fromfile(fn, dtype=np.uint8)
    Ht = d[:8*M].view(np.uint64).astype(np.float64); Hg = d[8*M:16*M].view(np.uint64).astype(np.float64)
    o = 16*M; ever = d[o:o+4*406].view(np.uint32); always = d[o+4*406:o+8*406].view(np.uint32)
    return Ht, Hg, ever, always
def ust_kume_toplami(H):
    S = H.copy()
    for b in range(NP):
        bit = 1 << b
        idx = np.arange(M); low = idx[(idx & bit) == 0]
        S[low] += S[low | bit]
    return S
dosyalar = sys.argv[1:]
Ht = Hg = None; ever = None; always = None
for fn in dosyalar:
    a, g, e, al = oku(fn)
    Ht = a if Ht is None else Ht + a; Hg = g if Hg is None else Hg + g
    ever = e if ever is None else (ever | e); always = al if always is None else (always & al)
for ad, H in (('bütün mercekler', Ht), ('kapı G tutan mercekler', Hg)):
    S = ust_kume_toplami(H)
    aday = []
    for x in range(NP):
        for y in range(NP):
            if x == y: continue
            sup = S[1 << x]; ce = sup - S[(1 << x) | (1 << y)]
            if sup >= 30 and ce == 0: aday.append((dil.PRED[x],), )
            if sup >= 30 and ce == 0: aday[-1] = ((dil.PRED[x],), dil.PRED[y], int(sup))
    ikili = {(a[0], a[1]) for a in aday}
    for x1 in range(NP):
        for x2 in range(x1+1, NP):
            for y in range(NP):
                if y in (x1, x2): continue
                if ((dil.PRED[x1],), dil.PRED[y]) in ikili or ((dil.PRED[x2],), dil.PRED[y]) in ikili: continue
                m = (1 << x1) | (1 << x2); sup = S[m]; ce = sup - S[m | (1 << y)]
                if sup >= 30 and ce == 0: aday.append(((dil.PRED[x1], dil.PRED[x2]), dil.PRED[y], int(sup)))
    print(f'== {ad}: aday gerektirmeler (karşı-örnek 0, destek ≥ 30) ==')
    for onc, son, sup in aday: print('   ', ' ∧ '.join(onc), '→', son, f'(destek {sup:.0f})')
# sabit-doğru: herhangi bir mercekte hep doğru olan yüklem
sabit = []
for L in range(406):
    for j in range(NP):
        if always[L] >> j & 1: sabit.append((L, dil.PRED[j]))
print('== hep doğru (dışlanmamış) yüklem–mercek çiftleri:', len(sabit), sabit[:20])
hic = sum(1 for L in range(406) for j in range(NP) if not (ever[L] >> j & 1))
print('== hiç doğru olmayan yüklem–mercek çifti sayısı (zararsız):', hic)
