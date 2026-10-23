#!/usr/bin/env python3
"""M20 O-havuzunda kümelenme: aynı bölüt + aynı zincirden, ardışık emisyonlarda (global sıra farkı 64) gelen isabetler.
Kullanım: python3 -I src/kume_m20.py <havuz_dizini>"""
import sys, glob, struct
d = sys.argv[1]; tot = 0; ardisik = 0; kume = 0; maxrun = 0
for f in sorted(glob.glob(f'{d}/*.bin')):
    b = open(f, 'rb').read(); recs = []
    for o in range(0, len(b), 244):
        z, s = struct.unpack_from('<IQ', b, o + 232); recs.append((z, s))
    tot += len(recs); son = {}; run = {}
    for z, s in recs:
        if z in son and s - son[z] == 64:
            ardisik += 1; run[z] = run.get(z, 1) + 1; maxrun = max(maxrun, run[z])
        else:
            kume += 1; run[z] = 1
        son[z] = s
print(f"{d}: isabet {tot:,}; bir önceki emisyonunda da O olan isabet {ardisik:,} (%{100*ardisik/tot:.2f}); bağımsız küme sayısı {kume:,}; en uzun ardışık dizi {maxrun}")
