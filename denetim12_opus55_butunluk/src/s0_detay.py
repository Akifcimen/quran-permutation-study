# s0_detay.py — S0 ≥ eşik (sonradan; sıfır-toplam dışlamalı sayım) olan kitapların S0 merceğindeki profili (rapor için).
# Kullanım: python3 -I src/s0_detay.py <sıfırkodeks> [eşik=11]
import sys, os, struct
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); sys.path.insert(0, os.path.join(KOK, 'src')); import dil
sk = sys.argv[1]; E = int(sys.argv[2]) if len(sys.argv) > 2 else 11
def kayit(fn, sira, R):
    d = open(fn, 'rb').read()
    for o in range(0, len(d) - R + 1, R):
        if struct.unpack_from('<Q', d, o + R - 8)[0] == sira: return list(struct.unpack_from('<114H', d, o))
gor = {}
for t, R in (('R', 242), ('T', 362)):
    for l in open(f'{KOK}/sonuc/s0/{sk}_{t}.txt'):
        x = l.split()
        if x[0] == 'KURAN' or int(x[5]) < E: continue
        gor[(x[0], int(x[1]))] = (R, int(x[3]), int(x[5]), int(x[7]))
print(f'| kaynak | sayım C | S0 | mercek | kF | kH | \\|U\\| | hücre sayıları | hücre toplamları | Σq | katsayılar | sağlanan (kapılı) |')
print('|---|---|---|---|---|---|---|---|---|---|---|---|')
for (fn, s), (R, C, S0, L) in sorted(gor.items(), key=lambda z: -z[1][2]):
    a = kayit(os.path.join(KOK, fn) if not os.path.isabs(fn) else fn, s, R); f, i = divmod(L, 29); p = dil.PRIMES[i]
    v, b = dil.profil(a, f, p); g = dil.kapi(b, p, 'G'); S = [k for k in dil.PRED if v[k] and (g or k not in dil.GATED)]
    print(f"| {os.path.basename(os.path.dirname(fn))}/{os.path.basename(fn)}:{s} | {C} | {S0} | ({dil.AL[f]},{dil.BE[f]:+d},{p}) | {b['kF']} | {b['kH']} | {b['kU']} | "
          f"{b['hucre_sayi']} | {b['hucre_toplam']} | {b['sq']} | {sorted(set(b['katsayilar']))} | {' '.join(S)} |")
