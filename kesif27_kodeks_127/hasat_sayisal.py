#!/usr/bin/env python3
# Halife-tarzi sayisal gerekceler: "son iki ayeti sil" ligi.
# ON-KAYIT: sablon ailesi asagida sonuca bakilmadan tanimlandi; kosudan sonra degistirilmez.
# Sablon = (alt kume, nicelik). Isabet = deger 19'a bolunur (bos kume ve 0 degerler sayilmaz).
# Duzeltme = standart metinde (Tevbe 129) isabet degil, duzeltilmis metinde isabet.
# Bozma    = standart metinde isabet, duzeltilmis metinde degil.
import json, statistics as st
import os
REPO = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
d = json.load(open(REPO + '/quran_meta.json'))
A0 = {s['number']: s['numberOfAyahs'] for s in d['data']['surahs']['references']}
INIT = {2,3,7,10,11,12,13,14,15,19,20,26,27,28,29,30,31,32,36,38,40,41,42,43,44,45,46,50,68}
assert len(INIT) == 29 and A0[9] == 129
def ds(x): return sum(int(c) for c in str(x))
def asal(x):
    if x < 2: return False
    k = 2
    while k * k <= x:
        if x % k == 0: return False
        k += 1
    return True
# ayet numaralari 1..a icin onhesap
MAXA = 400
BLK = {}
for a in range(0, MAXA + 1):
    s = ''.join(str(v) for v in range(1, a + 1))
    BLK[a] = dict(rtop=sum(ds(v) for v in range(1, a + 1)), bas=len(s),
                  rk=[s.count(str(q)) for q in range(10)], s=s)
def cat_mod(parts, m):
    v = 0
    for p in parts:
        for c in p: v = (v * 10 + ord(c) - 48) % m
    return v
ALT = {
    'hepsi': lambda n, a: True,
    'n tek': lambda n, a: n % 2 == 1, 'n cift': lambda n, a: n % 2 == 0,
    'harfli': lambda n, a: n in INIT, 'harfsiz': lambda n, a: n not in INIT,
    'a tek': lambda n, a: a % 2 == 1, 'a cift': lambda n, a: a % 2 == 0,
    'n tek a tek': lambda n, a: n % 2 == 1 and a % 2 == 1, 'n tek a cift': lambda n, a: n % 2 == 1 and a % 2 == 0,
    'n cift a tek': lambda n, a: n % 2 == 0 and a % 2 == 1, 'n cift a cift': lambda n, a: n % 2 == 0 and a % 2 == 0,
    'sure 1-9': lambda n, a: n <= 9, 'sure 9-114': lambda n, a: n >= 9, 'sure 9-27': lambda n, a: 9 <= n <= 27,
    'sure 27-114': lambda n, a: n >= 27, 'sure 1-27': lambda n, a: n <= 27,
    'a tek basamak': lambda n, a: a < 10, 'a iki basamak': lambda n, a: 10 <= a < 100, 'a uc basamak': lambda n, a: a >= 100,
    'a asal': lambda n, a: asal(a), 'n asal': lambda n, a: asal(n),
    'harfsiz n tek': lambda n, a: n not in INIT and n % 2 == 1, 'harfsiz n cift': lambda n, a: n not in INIT and n % 2 == 0,
    'harfli n tek': lambda n, a: n in INIT and n % 2 == 1, 'harfli n cift': lambda n, a: n in INIT and n % 2 == 0,
    'rakam(n)+rakam(a)=19': lambda n, a: ds(n) + ds(a) == 19,
}
for q in range(10):
    ALT[f'a son rakam {q}'] = (lambda q: lambda n, a: a % 10 == q)(q)
    ALT[f'n son rakam {q}'] = (lambda q: lambda n, a: n % 10 == q)(q)
for k in range(1, 20):
    ALT[f'rakam(a)={k}'] = (lambda k: lambda n, a: ds(a) == k)(k)
BES = lambda n: 0 if n in (1, 9) else 1   # numarasiz besmele
NIC = {
    'adet': lambda L: len(L),
    'Σn': lambda L: sum(n for n, a in L),
    'Σa': lambda L: sum(a for n, a in L),
    'Σa+besmele': lambda L: sum(a + BES(n) for n, a in L),
    'Σ(n+a)': lambda L: sum(n + a for n, a in L),
    'Σayetno': lambda L: sum(a * (a + 1) // 2 for n, a in L),
    'kumulatif': lambda L: sum(n + a + a * (a + 1) // 2 for n, a in L),
    'kumulatif+besmele': lambda L: sum(n + a + BES(n) + a * (a + 1) // 2 for n, a in L),
    'Σrakam(n)': lambda L: sum(ds(n) for n, a in L),
    'Σrakam(a)': lambda L: sum(ds(a) for n, a in L),
    'Σrakam(n)+rakam(a)': lambda L: sum(ds(n) + ds(a) for n, a in L),
    'Σrakam(ayetno)': lambda L: sum(BLK[a]['rtop'] for n, a in L),
    'basamak(ayetno)': lambda L: sum(BLK[a]['bas'] for n, a in L),
    'Σson rakam(a)': lambda L: sum(a % 10 for n, a in L),
    'Σn·a': lambda L: sum(n * a for n, a in L),
}
for q in range(10):
    NIC[f'"{q}" sayisi(ayetno)'] = (lambda q: lambda L: sum(BLK[a]['rk'][q] for n, a in L))(q)
BIT = {  # bitisik yazim: deger yerine (mod19, mod361) doner
    'bitisik(n a)': lambda L: [str(n) + str(a) for n, a in L],
    'bitisik(1..a)': lambda L: [BLK[a]['s'] for n, a in L],
    'bitisik(n 1..a)': lambda L: [str(n) + BLK[a]['s'] for n, a in L],
}
def degerler(A):
    out = {}
    for an, f in ALT.items():
        L = [(n, A[n]) for n in range(1, 115) if f(n, A[n])]
        if not L: continue
        for qn, g in NIC.items():
            v = g(L)
            out[(an, qn)] = None if v == 0 else (v % 19 == 0, v % 361 == 0)
        for qn, g in BIT.items():
            p = g(L)
            out[(an, qn)] = (cat_mod(p, 19) == 0, cat_mod(p, 361) == 0)
    return out
def isabet(V, k): return {t for t, x in V.items() if x is not None and x[k]}
V0 = degerler(A0); H0 = isabet(V0, 0); H0q = isabet(V0, 1)
nsab = sum(1 for x in V0.values() if x is not None)
print(f"sablon sayisi (standart metinde tanimli): {nsab}  | alt kume {len(ALT)}, nicelik {len(NIC)+len(BIT)}")
print(f"standart metin (Tevbe 129): 19-isabeti {len(H0)}, 361-isabeti {len(H0q)}")
sonuc = []
for i in range(1, 115):
    A = dict(A0); A[i] -= 2
    V = degerler(A); H = isabet(V, 0); Hq = isabet(V, 1)
    duz = len(H - H0); boz = len(H0 - H); duzq = len(Hq - H0q)
    sonuc.append((i, duz, boz, duz - boz, len(H), duzq))
    if i == 9: H9 = H - H0
sonuc_d = {r[0]: r for r in sonuc}
def sira(col):
    vals = sorted((r[col] for r in sonuc), reverse=True)
    v9 = sonuc_d[9][col]
    ust = sum(1 for v in vals if v > v9); esit = sum(1 for v in vals if v == v9)
    return v9, ust + 1, ust + esit, st.median(vals), min(vals), max(vals)
print("\n'Son iki ayeti sil' ligi, 114 sure (Tevbe = 9:128-129):")
for col, ad in [(1, 'duzelttigi (19)'), (3, 'net = duzeltme - bozma'), (5, 'duzelttigi (19x19=361)')]:
    v9, r1, r2, med, mn, mx = sira(col)
    print(f"  {ad:26}: Tevbe {v9} | sira {r1}-{r2}/114 | lig medyani {med} | en az {mn} | en cok {mx}")
top = sorted(sonuc, key=lambda r: -r[1])[:8]
print("  en cok duzelten 8 sure (sure, duzeltme, bozma, net):", [(r[0], r[1], r[2], r[3]) for r in top])
print(f"\n  Tevbe'nin 'duzelttigi' {len(H9)} sablondan ornekler:")
for t in sorted(H9)[:12]: print("    ", t)
# Halife'nin iki unlu maddesini yeniden uret
A127 = dict(A0); A127[9] = 127
tot_bes = sum(A127[n] + BES(n) for n in A127)
kum = sum(n + A127[n] + BES(n) + A127[n] * (A127[n] + 1) // 2 for n in A127)
print(f"\nkontrol: 127 ile Σa+besmele = {tot_bes} = 19x{tot_bes/19:g}; kumulatif+besmele = {kum} = 361x{kum/361:g}")
