# rakipler.py — Kur'an dillerinde (Q, QP2, EL) eşiğe 2 bitten yakın kontroller (ONKAYIT §5.4, §9): en yüksek 20 kontrolün puanı,
# merceği, sınıfı ve Λ'nın orada sağlanan yüklemleri; eşiği aşanların zincir kümelenmesi (M20: bölüt içi sıra mod 64).
# Kullanım: python3 -I src/rakipler.py <sifir>   → sonuc/<sifir>/rakipler.txt
import sys, os, glob, struct, json
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(KOK, 'src'))
import kutuphane as KU
S = sys.argv[1]; TD = os.path.join(KOK, 'tab', S); OUT = os.path.join(KOK, 'sonuc', S); os.makedirs(OUT, exist_ok=True)
REC = struct.Struct('<IfhBB114H')
esik = {}; dil_g = {}
for dz in sorted(glob.glob(os.path.join(TD, 'k*'))):
    for s in open(os.path.join(dz, 'esikler.txt')):
        w = s.split()
        if w and w[0] == 'D': esik[w[1]] = float(w[3])
        if w and w[0] == 'E' and w[1] == '0': esik['EL'] = float(w[2])
for s in open(os.path.join(TD, 'diller.txt')):
    w = s.split(); dil_g[w[0]] = [int(x) for x in w[5:5 + int(w[4])]]
aday = {'Q': [], 'QP2': [], 'EL': []}
for dz in sorted(glob.glob(os.path.join(TD, 'k*'))):
    adlar = [s.split()[0] for s in open(os.path.join(dz, 'diller.txt')) if s.strip()]
    for fn in glob.glob(os.path.join(dz, 'b*.q')):
        b = int(os.path.basename(fn)[1:5]); d = open(fn, 'rb').read()
        for o in range(0, len(d), REC.size):
            r = REC.unpack_from(d, o); ad = adlar[r[4]]
            if ad in aday: aday[ad].append(dict(bolut=b, sira=r[0], puan=r[1], L=r[2], sinif=r[3], a=list(r[5:])))
with open(os.path.join(OUT, 'rakipler.txt'), 'w') as f:
    for ad in ('Q', 'QP2', 'EL'):
        A = aday[ad]; e = esik.get(ad)
        if e is None: continue
        A.sort(key=lambda x: (-x['puan'], x['bolut'], x['sira']))
        ust = [x for x in A if x['puan'] >= e - 1e-6]
        zincir = {}
        for x in ust: zincir[x['sira'] % 64] = zincir.get(x['sira'] % 64, 0) + 1
        ayni = {}
        for x in ust: ayni[(x['bolut'], x['sira'] % 64)] = ayni.get((x['bolut'], x['sira'] % 64), 0) + 1
        f.write(f"== {ad}: eşik {e:.4f} bit; eşiği aşan/eşleyen {len(ust)}; 2 bit yakın aday {len(A)}\n")
        if S == 'M': f.write(f"   zincir kümelenmesi: farklı (bölüt, zincir) {len(ayni)}, en çok aynı (bölüt, zincir) {max(ayni.values()) if ayni else 0}\n")
        g = dil_g[ad]; adl = [KU.TUM[i] for i in g]
        for x in A[:20]:
            f_, pi = divmod(x['L'], 29); p = KU.PRIMES[pi]
            v, bil = KU.profil(x['a'], f_, p)
            sag = [y for y in adl if v[y]]
            f.write(f"   {x['puan']:.4f} bit  bölüt {x['bolut']} sıra {x['sira']}  ({KU.AL[f_]},{KU.BE[f_]:+d},{p}) kF={bil['kF']} kH={bil['kH']} sınıf {x['sinif']}  Λ'dan sağlanan: {' '.join(sag)}\n")
print(open(os.path.join(OUT, 'rakipler.txt')).read()[:3000])
