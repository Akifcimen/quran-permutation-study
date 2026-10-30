# s_metne.py — ikili bütünlüklü kayıtları (.s; '<IfhBB114H': sıra, puan, mercek L, ayrılmış, dil d, 114 ayet sayısı) metne çevirir.
# Kullanım: python3 -I s_metne.py <kayıt.s>                          → bütün kayıtlar
#           python3 -I s_metne.py <kayıt.s> isabet <esikler.txt> <S>  → yalnız puanı dilinin eşiğine ulaşanlar (eşik − 1e-5)
import struct, sys
FORMS = [(1, 1), (1, -1), (1, 2), (1, -2), (1, 3), (1, -3), (2, 1), (2, -1), (2, 3), (2, -3), (3, 1), (3, -1), (3, 2), (3, -2)]
PR = [3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113]
K = struct.Struct('<IfhBB114H'); data = open(sys.argv[1], 'rb').read()
ad = esik = None
if len(sys.argv) > 2:
    ad = [ln.split()[1] for ln in open(sys.argv[3]) if ln.startswith('D ')]; esik = [float(ln.split()[3]) for ln in open(sys.argv[3]) if ln.startswith('D ')]
print('# dil sira puan alpha beta p ayet_sayilari(1..114)')
for o in range(0, len(data), K.size):
    r = K.unpack_from(data, o); sira, puan, L, _, d = r[:5]
    if esik is not None and puan < esik[d] - 1e-5: continue
    f, i = divmod(L, 29); a, b = FORMS[f]
    print(ad[d] if ad else d, sira, f'{puan:.6f}', a, b, PR[i], ' '.join(map(str, r[5:])))
