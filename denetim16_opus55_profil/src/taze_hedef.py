# taze_hedef.py <tam_hedef.bin> <K0> <cikti.bin> <harita.txt> — tam hedef dosyasından (kayıt 0 = Kur'an, 1..K) yalnız K0'dan sonraki
# kayıtları alır: çıktıda kayıt 0 = Kur'an, kayıt i = özgün kayıt K0 + i (yeniden numaralanır). Harita: yeni_no özgün_no tohum_sırası.
import struct, sys
src, K0, out, harita = sys.argv[1], int(sys.argv[2]), sys.argv[3], sys.argv[4]
d = open(src, 'rb').read(); n = len(d) // 232; assert len(d) == n * 232
rec = lambda i: d[i * 232 + 4:(i + 1) * 232]
for i in range(n): assert struct.unpack_from('<i', d, i * 232)[0] == i
with open(out, 'wb') as o, open(harita, 'w') as h:
    o.write(struct.pack('<i', 0) + rec(0)); h.write('0 0 kuran\n')
    for j, i in enumerate(range(K0 + 1, n), start=1):
        o.write(struct.pack('<i', j) + rec(i)); h.write(f'{j} {i} tohum(s,3,{i})\n')
print(f'{n - 1 - K0} taze hedef ({K0 + 1}..{n - 1}) → 1..{n - 1 - K0}')
