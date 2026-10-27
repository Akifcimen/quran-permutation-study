# aday_incele — referans bölütü aday dosyasından (aday_t, 242 bayt) en yüksek sayım puanlı kitapların profilleri.
import sys, struct
sys.path.insert(0, 'src'); import dil
REC = 242; esik = int(sys.argv[2]) if len(sys.argv) > 2 else 10
for fn in sys.argv[1:2]:
    d = open(fn, 'rb').read(); goruldu = set()
    for o in range(0, len(d), REC):
        a = list(struct.unpack_from('<114H', d, o)); C = list(d[o+228:o+232]); L, = struct.unpack_from('<h', d, o+232); sira, = struct.unpack_from('<Q', d, o+234)
        if C[0] < esik or sira in goruldu: continue
        goruldu.add(sira); f, i = divmod(L, 29); p = dil.PRIMES[i]
        v, b = dil.profil(a, f, p); c, S = dil.sayim(v, b, p, 'G', dil.BE[f])
        print(f'sira {sira}: C={C} mercek ({dil.AL[f]},{dil.BE[f]:+d},{p}) kredi {c}  kF={b["kF"]} kH={b["kH"]} |U|={b["kU"]} hücre {b["hucre_sayi"]} Σ {b["hucre_toplam"]} q {b["katsayilar"]}')
        print('     sağlanan:', S)
