# C dökümünü Python başvurusuyla karşılaştırır. YALNIZ uyuşmazlık sayılarını basar (rastgele kitapların puanı basılmaz).
import sys, struct
sys.path.insert(0, sys.argv[2])
import dil
fn = sys.argv[1]
REC = 2*114 + 4*406 + 4*406
dat = open(fn, 'rb').read()
nb = len(dat) // REC
hata_m = hata_k = hata_c = 0; kontrol = 0
for b in range(nb):
    o = b*REC
    a = list(struct.unpack_from('<114h', dat, o)); o += 228
    m = struct.unpack_from('<406I', dat, o); o += 1624
    kF = dat[o:o+406]; o += 406
    kH = dat[o:o+406]; o += 406
    mc = dat[o:o+406]; o += 406
    krC = dat[o:o+406]
    for f in range(14):
        for i, p in enumerate(dil.PRIMES):
            L = f*29 + i
            v, bi = dil.profil(a, f, p)
            mp = sum(1 << j for j, k in enumerate(dil.PRED) if v[k])
            kontrol += 1
            if mp != m[L]: hata_m += 1
            if bi['kF'] != kF[L] or bi['kH'] != kH[L] or min(bi['hucre_sayi']) != mc[L]: hata_k += 1
            # sayım puanı (kapı G): C'nin maskesinden Python kredisiyle
            g = dil.kapi(bi, p, 'G')
            S = [k for k in dil.PRED if v[k] and (g or k not in dil.GATED)]
            c_py = dil.kredi(S, p, dil.BE[f])
            if c_py != krC[L]: hata_c += 1
print(f'{fn}: kitap {nb}, mercek-kitap {kontrol}, maske uyuşmazlığı {hata_m}, boy/hücre uyuşmazlığı {hata_k}, kredi uyuşmazlığı {hata_c}')
