# zincir.py — M20'de isabetlerin zincir kümelenmesi (ONKAYIT §4.2): her bölüt yeni 64 zincirle başlar; zincir kimliği =
# (bölüt, sıra mod 64). Bilgi puanı (v0, nokta ≥ S_Q; sınama) ve sayım puanı (kapı G, C ≥ C_Q; referans + sınama) isabetleri için
# isabet sayısı, farklı zincir sayısı, zincir başına en çok isabet. Kullanım: python3 -I src/zincir.py <sıfırkodeks>
import os, sys, glob, struct, collections
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); sk = sys.argv[1]
SQ = [float(l.split()[2]) for l in open(f'{KOK}/sonuc/esik_nihai_{sk}.txt') if l.startswith('SQ 0 ')][0]
CQ = [int(l.split()[2]) for l in open(f'{KOK}/sonuc/esik_nihai_{sk}.txt') if l.startswith('CQ 0 ')][0]
bilgi = set(); sayim = set()
for fn in sorted(glob.glob(f'{KOK}/tab/T_{sk}/b*.aday')):
    d = open(fn, 'rb').read(); b = os.path.basename(fn)
    for o in range(0, len(d) - 361, 362):
        s, = struct.unpack_from('<Q', d, o + 354); pt, = struct.unpack_from('<f', d, o + 228); C = d[o + 348]
        if pt >= SQ - 1e-6: bilgi.add(('T', b, s))
        if C >= CQ: sayim.add(('T', b, s))
for fn in sorted(glob.glob(f'{KOK}/tab/R_{sk}/b*.aday')):
    d = open(fn, 'rb').read(); b = os.path.basename(fn)
    for o in range(0, len(d) - 241, 242):
        s, = struct.unpack_from('<Q', d, o + 234); C = d[o + 228]
        if C >= CQ: sayim.add(('R', b, s))
for ad, H in (('bilgi (v0, nokta ≥ S_Q; sınama)', bilgi), (f'sayım (kapı G, C ≥ {CQ}; referans + sınama)', sayim)):
    z = collections.Counter((t, b, s % 64) for t, b, s in H)
    print(f'{sk} {ad}: isabet {len(H)}, farklı zincir {len(z)}, zincir başına en çok {max(z.values()) if z else 0}; '
          f'≥ 2 isabetli zincir {sum(1 for v in z.values() if v >= 2)}')
