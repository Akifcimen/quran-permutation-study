# rakipler.py — en güçlü 20 rakip ve ön-kayıtlı "Kur'an sınıfı" sınıflandırması (ONKAYIT §7).
# Sayım puanı: referans (242 B) + sınama (362 B) adayları; bilgi puanı: sınama adayları (birincil değişke v = 0, nokta puanı).
# Kullanım: python3 -I src/rakipler.py <sıfırkodeks> <sayim|bilgi> [adet=20]
import os, sys, glob, struct
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(KOK, 'src')); import dil

TUR = {'denge': {'B1', 'B2', 'F1', 'F2', 'R0c', 'R0e', 'R1c', 'R1e'}, 'modül': {'Qc', 'QS', 'Qh', 'QkF', 'QU', 'QkH', 'Qa', 'Qn', 'QH'},
       'iki el': {'W1'}, 'ardışıklık': {'K1', 'K2'}}
DUZ = {'kitap': {'B1', 'B2'}, 'aile': {'F1', 'F2', 'QS', 'Qh', 'QkF', 'K2'}, 'hücre': {'R0c', 'R0e', 'R1c', 'R1e', 'K1', 'Qc'},
       'birleşim': {'W1', 'QU', 'QkH', 'Qa', 'Qn', 'QH'}}

def sinif(a, L):
    f, i = divmod(L, 29); p = dil.PRIMES[i]
    v, b = dil.profil(a, f, p); g = dil.kapi(b, p, 'G')
    S = [k for k in dil.PRED if v[k] and (g or k not in dil.GATED)]
    c = dil.kredi(S, p, dil.BE[f])
    nt = sum(1 for t in TUR.values() if t & set(S)); nd = sum(1 for d in DUZ.values() if d & set(S))
    kq = max([abs(x) for x in b['katsayilar']] or [0])
    return dict(mercek=f'({dil.AL[f]},{dil.BE[f]:+d},{p})', kredi=c, S=S, kapi=g, tur=nt, duzey=nd, sinif=(g and nt >= 3 and nd >= 3),
                kF=b['kF'], kH=b['kH'], kU=b['kU'], hucre=b['hucre_sayi'], toplam=b['hucre_toplam'], maxq=kq, sq=b['sq'])

def oku_R(fn):
    d = open(fn, 'rb').read(); out = []
    for o in range(0, len(d) - 241, 242):
        a = list(struct.unpack_from('<114H', d, o)); C = list(d[o + 228:o + 232]); L, = struct.unpack_from('<h', d, o + 232); s, = struct.unpack_from('<Q', d, o + 234)
        out.append(dict(a=a, C=C[0], L=L, sira=s, kaynak=fn))
    return out

def oku_T(fn):
    d = open(fn, 'rb').read(); out = []; R = 362
    for o in range(0, len(d) - R + 1, R):
        a = list(struct.unpack_from('<114H', d, o)); pt = struct.unpack_from('<8f', d, o + 228); lo = struct.unpack_from('<8f', d, o + 260); hi = struct.unpack_from('<8f', d, o + 292)
        Lv = struct.unpack_from('<8h', d, o + 324); coz = d[o + 340:o + 348]; C = list(d[o + 348:o + 352]); CL, = struct.unpack_from('<h', d, o + 352); s, = struct.unpack_from('<Q', d, o + 354)
        out.append(dict(a=a, pt=pt, lo=lo, hi=hi, Lv=Lv, coz=list(coz), C=C[0], CL=CL, sira=s, kaynak=fn))
    return out

def tekil(L):
    gor = {};
    for r in L: gor[(r['kaynak'], r['sira'])] = r
    return list(gor.values())

if __name__ == '__main__':
    ad, tur = sys.argv[1], sys.argv[2]; adet = int(sys.argv[3]) if len(sys.argv) > 3 else 20
    if tur == 'sayim':
        K = tekil(sum([oku_R(f) for f in sorted(glob.glob(os.path.join(KOK, 'tab', f'R_{ad}', '*.aday')))], []) +
                  [dict(a=r['a'], C=r['C'], L=r['CL'], sira=r['sira'], kaynak=r['kaynak']) for r in tekil(sum([oku_T(f) for f in sorted(glob.glob(os.path.join(KOK, 'tab', f'T_{ad}', '*.aday')))], []))])
        K.sort(key=lambda r: -r['C']); ust = K[:adet]
        print(f'| # | sayım | mercek | kF | kH | \\|U\\| | hücre sayıları | hücre toplamları | maks\\|q\\| | sağlanan (kapılı) | tür | düzey | Kur\'an sınıfı |')
        print('|---|---|---|---|---|---|---|---|---|---|---|---|---|')
        for j, r in enumerate(ust, 1):
            s = sinif(r['a'], r['L'])
            print(f"| {j} | {r['C']} | {s['mercek']} | {s['kF']} | {s['kH']} | {s['kU']} | {s['hucre']} | {s['toplam']} | {s['maxq']} | {' '.join(s['S'])} | {s['tur']} | {s['duzey']} | {'evet' if s['sinif'] else 'hayır'} |")
    else:
        K = tekil(sum([oku_T(f) for f in sorted(glob.glob(os.path.join(KOK, 'tab', f'T_{ad}', '*.aday')))], []))
        K.sort(key=lambda r: -r['pt'][0]); ust = K[:adet]
        print(f'| # | bilgi (nokta) [alt, üst] | çözülmüş | mercek | kF | kH | \\|U\\| | hücre sayıları | hücre toplamları | maks\\|q\\| | sağlanan | sayım | Kur\'an sınıfı |')
        print('|---|---|---|---|---|---|---|---|---|---|---|---|---|')
        for j, r in enumerate(ust, 1):
            L = r['Lv'][0]; s = sinif(r['a'], L)
            f, i = divmod(L, 29); p = dil.PRIMES[i]; v, b = dil.profil(r['a'], f, p)
            sag = [k for k in dil.PRED if v[k]]
            hi = r['hi'][0]; his = '∞' if hi > 1e30 else f'{hi:.2f}'
            print(f"| {j} | {r['pt'][0]:.2f} [{r['lo'][0]:.2f}, {his}] | {'evet' if r['coz'][0] else 'hayır'} | {s['mercek']} | {s['kF']} | {s['kH']} | {s['kU']} | {s['hucre']} | {s['toplam']} | {s['maxq']} | {' '.join(sag)} | {r['C']} | {'evet' if s['sinif'] else 'hayır'} |")
