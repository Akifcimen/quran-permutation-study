#!/usr/bin/env python3
# DUYARLILIK (kosudan SONRA eklendi, on-kayitli degil): sablon ailesindeki Halife'ye ozgu capalari
# (sure 1-9, 9-114, 9-27, 1-27, 27-114 ve rakam(n)+rakam(a)=19) cikar; yerine her sure icin simetrik
# araliklar koy: [1,k] ve [k,114], k=1..114. Boylece hicbir sure capa ayricaligi tasimaz.
import importlib.util, sys, io, contextlib, statistics as st
spec = importlib.util.spec_from_file_location('h', sys.path[0] + '/hasat_sayisal.py')
with contextlib.redirect_stdout(io.StringIO()):
    h = importlib.util.module_from_spec(spec); spec.loader.exec_module(h)
ALT = {k: v for k, v in h.ALT.items() if not (k.startswith('sure ') or k.startswith('rakam(n)+rakam(a)'))}
for k in range(1, 115):
    ALT[f'[1,{k}]'] = (lambda k: lambda n, a: n <= k)(k)
    ALT[f'[{k},114]'] = (lambda k: lambda n, a: n >= k)(k)
def mod19(A):
    out = {}
    for an, f in ALT.items():
        L = [(n, A[n]) for n in range(1, 115) if f(n, A[n])]
        if not L: continue
        for qn, g in h.NIC.items():
            v = g(L); out[(an, qn)] = None if v == 0 else v % 19
        for qn, g in h.BIT.items():
            out[(an, qn)] = h.cat_mod(g(L), 19)
    return out
A0 = h.A0; R0 = mod19(A0)
print(f"simetrik aile: {sum(1 for v in R0.values() if v is not None)} sablon; standart metinde 19-isabeti {sum(1 for v in R0.values() if v == 0)}")
rows = []
for i in range(1, 115):
    A = dict(A0); A[i] -= 2; R = mod19(A)
    dok = [k for k in set(R0) | set(R) if R0.get(k) != R.get(k)]
    duz = sum(1 for k in dok if R.get(k) == 0 and R0.get(k) != 0)
    boz = sum(1 for k in dok if R0.get(k) == 0 and R.get(k) != 0)
    bek = sum(1 for k in dok if R0.get(k) not in (0, None) and R.get(k) is not None) / 18
    rows.append((i, len(dok), duz, boz, duz - boz, round(duz - bek, 1)))
def sira(col):
    v9 = next(r[col] for r in rows if r[0] == 9)
    ust = sum(1 for r in rows if r[col] > v9); es = sum(1 for r in rows if r[col] == v9)
    return v9, ust + 1, ust + es, st.median(r[col] for r in rows), max(r[col] for r in rows)
for col, ad in [(2, 'duzeltme'), (4, 'net'), (5, 'sansin ustundeki fazla')]:
    v9, a, b, med, mx = sira(col)
    print(f"  {ad:24}: Tevbe {v9} | sira {a}-{b}/114 | medyan {med} | en cok {mx}")
print("  net'e gore ilk 8 (sure, dokunulan, duzeltme, bozma, net, fazla):")
for r in sorted(rows, key=lambda r: -r[4])[:8]: print("    ", r)
