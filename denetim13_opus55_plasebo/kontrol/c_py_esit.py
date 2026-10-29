# c_py_esit.py — C (src/kutuphane.h) ↔ Python (src/kutuphane.py) eşitliği: kütüphanenin 112 yüklemi, 406 mercek, k_F, k_H,
# en küçük hücre. Yalnız uyuşmazlık sayıları basılır (yüklem değerleri ya da sıklıkları basılmaz).
import sys, struct, os
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'src'))
import numpy as np
import kutuphane as K
fn = sys.argv[1]
NL = 406
kayit = 114 * 2 + NL * (1 + 1 + 8 + 8 + 1 + 1 + 1 + 1)
veri = open(fn, 'rb').read()
nk = len(veri) // kayit
hata = {g: 0 for g in 'BZFWH'}
hata_k = 0
for b in range(nk):
    o = b * kayit
    a = list(struct.unpack_from('<114h', veri, o)); o += 228
    mB = np.frombuffer(veri, np.uint8, NL, o); o += NL
    mZ = np.frombuffer(veri, np.uint8, NL, o); o += NL
    mF = np.frombuffer(veri, np.uint64, NL, o); o += 8 * NL
    mW = np.frombuffer(veri, np.uint64, NL, o); o += 8 * NL
    mH = np.frombuffer(veri, np.uint8, NL, o); o += NL
    kF = np.frombuffer(veri, np.uint8, NL, o); o += NL
    kH = np.frombuffer(veri, np.uint8, NL, o); o += NL
    mc = np.frombuffer(veri, np.uint8, NL, o); o += NL
    for f in range(14):
        for i, p in enumerate(K.PRIMES):
            L = f * 29 + i
            v, bil = K.profil(a, f, p)
            M = K.maskeler(v)
            hata['B'] += M['B'] != int(mB[L]); hata['Z'] += M['Z'] != int(mZ[L]); hata['F'] += M['F'] != int(mF[L])
            hata['W'] += M['W'] != int(mW[L]); hata['H'] += M['H'] != int(mH[L])
            hata_k += (bil['kF'] != int(kF[L])) + (bil['kH'] != int(kH[L])) + (bil['mc'] != int(mc[L]))
print(f'{os.path.basename(fn)}: {nk} kitap × 406 mercek — grup maskesi uyuşmazlığı ' +
      ', '.join(f'{g} {hata[g]}' for g in 'BZFWH') + f'; kF/kH/hücre {hata_k}')
sys.exit(1 if (sum(hata.values()) or hata_k) else 0)
