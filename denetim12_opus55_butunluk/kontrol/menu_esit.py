# menu_esit — C menü kredileri ↔ Python (dil.menu_kredi); yalnız uyuşmazlık sayısı.
import sys, struct
sys.path.insert(0, sys.argv[2]); import dil
dat = open(sys.argv[1], 'rb').read(); REC = 228 + 20*406; nb = len(dat)//REC; hata = 0; n = 0
for b in range(nb):
    o = b*REC; a = list(struct.unpack_from('<114h', dat, o)); kr = dat[o+228:o+REC]
    for c, (d1, d2) in enumerate(dil.CIFTLER):
        for f in range(14):
            for i, p in enumerate(dil.PRIMES):
                n += 1
                if dil.menu_kredi(a, f, p, d1, d2) != kr[c*406 + f*29 + i]: hata += 1
print(f'{sys.argv[1]}: kitap {nb}, (çift, mercek) {n}, uyuşmazlık {hata}')
