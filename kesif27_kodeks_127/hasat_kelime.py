#!/usr/bin/env python3
# Kelime sayimi ligi: her surenin son iki ayeti silinince, o iki ayette gecen kelime turlerinden
# kacinin Kur'an genelindeki sayisi 19'un katina iner (duzeltme) ya da katindan cikar (bozma)?
# Iki sozlesme: A = tam yazim bicimi; B = basindaki vav/fa, bi/li/ka ve el- takisi soyulmus kaba govde.
# Metin: api.alquran.cloud quran-simple-clean; numarasiz besmeleler cikarildi (1:1 ve 27:30 kalir).
import json, re, statistics as st
from collections import Counter
d = json.load(open('simple_clean.json'))
BES = 'بسم الله الرحمن الرحيم'
def temiz(s):
    s = s.replace('﻿', '')
    s = re.sub(r'[ۖ-ۭؐ-ًؚ-ٰٟ]', '', s)
    return re.sub(r'\s+', ' ', s).strip()
V = {}
for su in d['data']['surahs']:
    n = su['number']
    for ay in su['ayahs']:
        t = temiz(ay['text']); k = ay['numberInSurah']
        if k == 1 and n not in (1, 9) and t.startswith(BES):
            t = t[len(BES):].strip()
        V[(n, k)] = t.split(' ') if t else []
A0 = {}
for (n, k) in V: A0[n] = max(A0.get(n, 0), k)
assert sum(A0.values()) == 6236 and A0[9] == 129
def govde(w):
    for p in ('و', 'ف'):
        if w.startswith(p) and len(w) > 3: w = w[1:]; break
    for p in ('ب', 'ل', 'ك'):
        if w.startswith(p) and len(w) > 3: w = w[1:]; break
    if w.startswith('ال') and len(w) > 3: w = w[2:]
    return w
ALLAH = re.compile(r'^(?:[وفبتأ]|أف|أب|وب|فب|وت)?الله$|^(?:[وفأ])?لله$|^آلله$')
tum = [w for k in sorted(V) for w in V[k]]
nallah = sum(1 for w in tum if ALLAH.match(w))
n9 = sum(1 for k in ((9, 128), (9, 129)) for w in V[k] if ALLAH.match(w))
print(f"'Allah' (on ekli bicimler dahil, Allahumma haric): {nallah}; 9:128-129 icinde {n9}; cikarinca {nallah-n9} (19'a kalan {(nallah-n9)%19})")
def toplam_ayetno(pred):
    return sum(k for (n, k), ws in V.items() for w in ws if pred(w))
s1 = toplam_ayetno(lambda w: ALLAH.match(w))
print(f"'Allah' gecen ayet numaralari toplami (her gecis icin): {s1}; 9:129 cikinca {s1-129} (19'a kalan {(s1-129)%19})")
metin = ' '.join(' '.join(V[k]) for k in sorted(V))
print(f"'لا إله إلا هو' gecis sayisi: {metin.count('لا إله إلا هو')}")
def lig(ad, f):
    C = Counter(f(w) for w in tum)
    rows = []
    for n in range(1, 115):
        a = A0[n]
        R = Counter(f(w) for k in (a - 1, a) for w in V[(n, k)])
        duz = boz = uyg = 0
        for t, r in R.items():
            c = C[t]
            if c - r <= 0: continue
            uyg += 1
            if c % 19 != 0 and (c - r) % 19 == 0: duz += 1
            if c % 19 == 0 and (c - r) % 19 != 0: boz += 1
        rows.append((n, len(R), duz, boz, duz - boz))
    r9 = next(r for r in rows if r[0] == 9)
    def sira(col):
        ust = sum(1 for r in rows if r[col] > r9[col]); es = sum(1 for r in rows if r[col] == r9[col])
        return f"{r9[col]} (sira {ust+1}-{ust+es}/114, medyan {st.median(r[col] for r in rows)}, en cok {max(r[col] for r in rows)})"
    print(f"\n[{ad}] Tevbe'nin son iki ayetinde {r9[1]} kelime turu")
    print(f"   duzeltme: {sira(2)}")
    print(f"   net     : {sira(4)}")
    top = sorted(rows, key=lambda r: (-r[2], -r[4]))[:6]
    print("   en cok duzelten 6 (sure, tur, duzeltme, bozma, net):", top)
    # tur basina duzeltme orani
    oran = sorted(((r[2] / r[1]) if r[1] else 0, r[0]) for r in rows)
    o9 = r9[2] / r9[1]
    print(f"   tur basina duzeltme orani: Tevbe {o9:.3f}; ust siradan {sum(1 for o,_ in oran if o > o9)+1}. / 114; sansa gore ~{1/19:.3f}")
lig('A tam bicim', lambda w: w)
lig('B kaba govde', govde)
