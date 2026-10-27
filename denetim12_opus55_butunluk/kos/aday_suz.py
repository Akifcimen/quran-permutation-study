# aday_suz — ilk sürüm sınama bölütlerinin (pay 6 bit) aday dosyalarını yeni ölçüte süzer (EK_..._aday_payi.md):
# S_üst ≥ S_Q,geçici − 2 (herhangi değişke) ya da C ≥ C_Q − 2; bölüt içi en yüksek 30 kayıt (dosya sonundaki) korunur.
import sys, struct, os
fn, esik = sys.argv[1], sys.argv[2]
SQ = {}; CQ = {}
for line in open(esik):
    t = line.split()
    if t[0] == 'SQ': SQ[int(t[1])] = float(t[2])
    elif t[0] == 'CQ': CQ[int(t[1])] = int(t[2])
R = 362; d = open(fn, 'rb').read(); n = len(d) // R; out = bytearray(); kal = 0
for i in range(n):
    o = i * R
    hi = struct.unpack_from('<8f', d, o + 292); C = d[o + 348:o + 352]
    if i >= n - 30 or any(hi[v] >= SQ[v] - 2.0 for v in range(8)) or any(C[g] >= CQ[g] - 2 for g in range(3)):
        out += d[o:o + R]; kal += 1
tmp = fn + '.suzuluyor'; open(tmp, 'wb').write(out); os.replace(tmp, fn)
print(f'{fn}: {n} → {kal} kayıt')
