#!/usr/bin/env python3
# 127 "iki yol" testi: matris mimarisi, cekirdegi koruyan duzeltmelerde neye duyarli?
import json, os, sys
from collections import Counter, defaultdict
import os
REPO = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
d = json.load(open(os.path.join(REPO, 'quran_meta.json')))
A0 = {s['number']: s['numberOfAyahs'] for s in d['data']['surahs']['references']}
assert A0[9] == 129 and sum(A0.values()) == 6236
M = 19
I = ((1, 0), (0, 1))
def mul(X, Y):
    return (((X[0][0]*Y[0][0] + X[0][1]*Y[1][0]) % M, (X[0][0]*Y[0][1] + X[0][1]*Y[1][1]) % M),
            ((X[1][0]*Y[0][0] + X[1][1]*Y[1][0]) % M, (X[1][0]*Y[0][1] + X[1][1]*Y[1][1]) % M))
def det(X): return (X[0][0]*X[1][1] - X[0][1]*X[1][0]) % M
_oc = {}
def order(X):
    if X in _oc: return _oc[X]
    if det(X) == 0: _oc[X] = None; return None
    Y = X; k = 1
    while Y != I:
        Y = mul(Y, X); k += 1
        if k > 400: _oc[X] = -1; return -1
    _oc[X] = k; return k
def gm(a, pred):
    s = [n for n in range(1, 115) if pred(n, a[n])]
    r = [n for n in range(1, 115) if not pred(n, a[n])]
    return ((sum(s) % M, sum(a[n] for n in s) % M), (sum(r) % M, sum(a[n] for n in r) % M))
def mimari(a):
    P = gm(a, lambda n, x: (n + x) % 2 == 0)
    R = gm(a, lambda n, x: (n + x) % 19 == 0)
    PR, RP = mul(P, R), mul(R, P)
    S = tuple(tuple((PR[i][j] + RP[i][j]) % M for j in range(2)) for i in range(2))
    D = tuple(tuple((PR[i][j] - RP[i][j]) % M for j in range(2)) for i in range(2))
    return order(P) == 360 and order(R) == 360 and order(S) == 360 and mul(D, D) == I
E0 = {n for n in A0 if (n + A0[n]) % 2 == 0}
F0 = {n for n in A0 if (n + A0[n]) % 19 == 0}
U0 = {n for n in A0 if (n - A0[n]) % 19 == 0}
print(f"cekirdek: cift-t {len(E0)} sure, t-ailesi {len(F0)}, u-ailesi {len(U0)}, birlesim {len(F0 | U0)}")
def cekirdek(a):
    E = {n for n in a if (n + a[n]) % 2 == 0}
    F = {n for n in a if (n + a[n]) % 19 == 0}
    U = {n for n in a if (n - a[n]) % 19 == 0}
    St_E = sum(n + a[n] for n in E)
    return (len(E) == 57 and St_E == sum(a.values()) and F == F0 and U == U0
            and all(a[n] == A0[n] for n in F0 | U0))
def halife(a): return (sum(a.values()) + 112) % 19 == 0
t127 = dict(A0); t127[9] = 127
print(f"129: cekirdek={cekirdek(A0)} mimari={mimari(A0)} halife(6236+112)={halife(A0)}")
print(f"127: cekirdek={cekirdek(t127)} mimari={mimari(t127)} halife(6234+112=6346=19x334)={halife(t127)}")
# 1) Tevbe taramasi 1..400
tv = []
for x in range(1, 401):
    a = dict(A0); a[9] = x
    if mimari(a): tv.append((x, cekirdek(a), halife(a)))
print("\n[1] Tevbe=1..400 icinde tam mimariyi veren degerler (deger, cekirdek, halife):")
print("   ", tv)
# 2) tum sureler x delta -100..100
tab = Counter(); res = defaultdict(lambda: [0, 0]); sure_set = set(); dis_hit = []
n_all = 0
for i in range(1, 115):
    for dl in range(-100, 101):
        if dl == 0 or A0[i] + dl < 1: continue
        a = dict(A0); a[i] += dl; n_all += 1
        c, m, h = cekirdek(a), mimari(a), halife(a)
        tab[(c, m, h)] += 1
        if c:
            res[dl % 19][0] += 1; res[dl % 19][1] += m
            if m: sure_set.add(i)
        elif m:
            dis_hit.append((i, dl, h))
print(f"\n[2] tek-sure duzeltmeleri: {n_all} (114 sure x delta -100..+100)")
print("    cekirdegi koruyan duzeltmeler icinde, mimari x halife capraz tablosu:")
for m in (True, False):
    for h in (True, False):
        print(f"      mimari={m!s:5} halife={h!s:5}: {tab[(True, m, h)]}")
print(f"    cekirdegi koruyan ve mimariyi veren farkli sure sayisi: {len(sure_set)}")
print("    delta mod 19 basina mimari orani (cekirdegi koruyanlar):")
print("     ", ", ".join(f"{r}:{res[r][1]}/{res[r][0]}" for r in range(19) if res[r][0]))
print(f"    cekirdegi BOZARAK mimariyi veren duzeltmeler: {len(dis_hit)}; bunlarin halife-uyumlu olani: {sum(1 for x in dis_hit if x[2])}")
# 3) 2 ayet silme: -2 ile kac sure ayni mimariyi verir
iki = [i for i in range(1, 115) if A0[i] > 2 and (lambda a: (a.__setitem__(i, a[i] - 2), mimari(a) and cekirdek(a))[1])(dict(A0))]
print(f"\n[3] bir sureden 2 ayet silmek: cekirdek + tam mimari veren sure sayisi = {len(iki)}; Tevbe icinde mi: {9 in iki}")
print("    ", iki)
