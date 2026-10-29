# sabit_birlestir.py — sabit_tarama çıktılarını birleştirir; yalnız bayrakları basar (ONKAYIT §2.5).
import sys, os
import numpy as np
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'src'))
import kutuphane as K
NL = 406
def oku(fn):
    d = open(fn, 'rb').read(); o = 0; r = {}
    for ad, w in [('orB',1),('anB',1),('orZ',1),('anZ',1),('orH',1),('anH',1),('orF',8),('anF',8),('orW',8),('anW',8),('xB',1),('xZ',1),('xH',1),('xF',8),('xW',8)]:
        dt = np.uint8 if w == 1 else np.uint64
        r[ad] = np.frombuffer(d, dt, NL, o).copy(); o += w * NL
    return r
R = [oku(f) for f in sys.argv[1:]]
top = {}
for g in 'BZHFW':
    top['or' + g] = np.bitwise_or.reduce([r['or' + g] for r in R]); top['an' + g] = np.bitwise_and.reduce([r['an' + g] for r in R]); top['x' + g] = R[-1]['x' + g]
isim = {'B': K.GB, 'Z': K.GZ, 'F': K.GF, 'W': K.GW, 'H': K.GH}
hep_dogru_dislanmamis = []; dislanan_sabit_degil = []; hic_dogru = {}
for g in 'BZFWH':
    for i, ad in enumerate(isim[g]):
        hic = True
        for L in range(NL):
            f, pi = divmod(L, 29); p = K.PRIMES[pi]; mer = f'({K.AL[f]},{K.BE[f]:+d},{p})'
            o = (int(top['or' + g][L]) >> i) & 1; a = (int(top['an' + g][L]) >> i) & 1; x = (int(top['x' + g][L]) >> i) & 1
            if a and not x: hep_dogru_dislanmamis.append(f'{ad}@{mer}')
            if x and o and not a: dislanan_sabit_degil.append(f'{ad}@{mer}')
            if o and not x: hic = False
        hic_dogru[ad] = hic
print('taranan dosya:', len(R))
print('(1) analitik listede olmayan hep-doğru çiftler:', len(hep_dogru_dislanmamis), hep_dogru_dislanmamis[:50])
print('(2) analitik listede olup sabit olmayan çiftler:', len(dislanan_sabit_degil), dislanan_sabit_degil[:50])
print('(3) dışlanmamış hiçbir mercekte hiç doğru görülmeyen yüklemler:', [ad for ad, h in hic_dogru.items() if h])
