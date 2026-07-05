#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Geniş tarama: şimdiye dek test edilmemiş doğal istatistiklerin panoraması.
Amaç: daha güçlü/güzel bir örüntü adayı bulmak. Tüm adaylar sonra permütasyonla ölçülecek."""
import json
import numpy as np
from collections import Counter

d = json.load(open('/Users/dejuremacminim1/Desktop/hrf2/quran_meta.json'))
A0 = np.array([s['numberOfAyahs'] for s in d['data']['surahs']['references']])
N = np.arange(1, 115)

def pretty(x):
    tags = []
    if x % 19 == 0: tags.append(f"19×{x//19}")
    if x % 38 == 0: tags.append(f"38×{x//38}")
    r = int(x ** 0.5 + 0.5)
    if r*r == x: tags.append(f"{r}²")
    if x % 114 == 0: tags.append(f"114×{x//114}")
    return (" [" + ", ".join(tags) + "]") if tags else ""

def stats(tag, A):
    t = N + A; u = A - N
    print(f"\n{'='*78}\n===== {tag}  (toplam ayet={A.sum()}) =====")
    te = (t % 2 == 0); ne = (N % 2 == 0); ae = (A % 2 == 0)

    print("\n-- KİTAP DÜZEYİ 4 HÜCRE (t-paritesi × n-paritesi) --")
    for tp, l1 in [(te, 't çift'), (~te, 't tek ')]:
        for np_, l2 in [(ne, 'n çift'), (~ne, 'n tek ')]:
            m = tp & np_
            st, sn, sa = int(t[m].sum()), int(N[m].sum()), int(A[m].sum())
            print(f"  {l1} & {l2}: say={int(m.sum()):3d}  Σt={st}{pretty(st)}  Σn={sn}{pretty(sn)}  Σa={sa}{pretty(sa)}")

    print("\n-- ÖNEK DENGELERİ (ilk k sûrede t-çift sayısı; denge = k/2) --")
    for k in (19, 38, 57, 76, 95, 114):
        c = int(te[:k].sum()); st = int(t[:k].sum())
        print(f"  ilk {k:3d}: çift={c:2d}/{k}  {'<<< DENGE' if 2*c == k else ''}  Σt={st}{pretty(st)}")
    bal = [k for k in range(2, 115, 2) if 2*int(te[:k].sum()) == k]
    print("  tam denge noktaları (tümü):", bal)

    print("\n-- t mod 19 SINIF DAĞILIMI (r: boy, Σt) --")
    sizes = [(r, int((t % 19 == r).sum()), int(t[t % 19 == r].sum())) for r in range(19)]
    print("  " + "  ".join(f"r{r}:{b}/{s}" for r, b, s in sizes))
    print("  boy dağılımı:", sorted(Counter(b for _, b, _ in sizes).items()), " (beklenen boy=6)")

    print("\n-- AYNA AİLE: u = a − n ≡ 0 (mod 19) --")
    m = (u % 19 == 0)
    print(f"  say={int(m.sum())}, sûreler={list(N[m])}")
    print(f"  Σt={int(t[m].sum())}{pretty(int(t[m].sum()))}, Σu={int(u[m].sum())}, Σn={int(N[m].sum())}, Σa={int(A[m].sum())}")
    print(f"  u değerleri: {sorted(u[m].tolist())}")
    me = m & te
    print(f"  parite bölünmesi: çift-t {int(me.sum())} (Σt={int(t[me].sum())}{pretty(int(t[me].sum()))}) / tek-t {int((m&~te).sum())} (Σt={int(t[m&~te].sum())}{pretty(int(t[m&~te].sum()))})")

    print("\n-- AYET-AİLESİ: a ≡ 0 (mod 19) --")
    m = (A % 19 == 0)
    print(f"  say={int(m.sum())}, sûreler={list(N[m])}, a={A[m].tolist()}, Σa={int(A[m].sum())}{pretty(int(A[m].sum()))}, Σt={int(t[m].sum())}{pretty(int(t[m].sum()))}")

    print("\n-- t TEKRARLARI --")
    c = Counter(t.tolist())
    rep = {v: k for v, k in sorted(c.items()) if k > 1}
    print("  tekrarlanan t değerleri:", rep)
    print("  farklı t değeri sayısı:", len(c), "; tekrar eden sûre sayısı:", sum(k for k in c.values() if k > 1))

    print("\n-- 19-AİLESİ KATSAYI YAPISI --")
    fam = (t % 19 == 0)
    print("  katsayı multiset:", sorted((t[fam] // 19).tolist()))
    print("  çift-t yarı:", sorted((t[fam & te] // 19).tolist()), "  tek-t yarı:", sorted((t[fam & ~te] // 19).tolist()))

    print("\n-- AYNA ÇİFTLERİ (n, 115−n) --")
    print(f"  a(n)=a(115−n) olan çift sayısı: {int((A == A[::-1]).sum()) // 2}")
    print(f"  t(n)=t(115−n) olan çift sayısı: {int((t == t[::-1]).sum()) // 2}")
    ts = t + t[::-1]
    print(f"  t(n)+t(115−n) 19'a bölünen çift sayısı: {int((ts[:57] % 19 == 0).sum())}")

    print("\n-- BLOK TOPLAMLARI --")
    print(f"  Σt yarılar (1-57 / 58-114): {int(t[:57].sum())}{pretty(int(t[:57].sum()))} / {int(t[57:].sum())}{pretty(int(t[57:].sum()))}")
    print(f"  Σt üçte birler (38'lik): {int(t[:38].sum())}{pretty(int(t[:38].sum()))} / {int(t[38:76].sum())}{pretty(int(t[38:76].sum()))} / {int(t[76:].sum())}{pretty(int(t[76:].sum()))}")
    print(f"  Σt altıda birler (19'luk): " + " / ".join(f"{int(t[i*19:(i+1)*19].sum())}{pretty(int(t[i*19:(i+1)*19].sum()))}" for i in range(6)))

    print("\n-- MARJİNALLER --")
    print(f"  Σa(n çift)={int(A[ne].sum())}{pretty(int(A[ne].sum()))}  Σa(n tek)={int(A[~ne].sum())}{pretty(int(A[~ne].sum()))}")
    print(f"  Σn(a çift)={int(N[ae].sum())}{pretty(int(N[ae].sum()))}  Σn(a tek)={int(N[~ae].sum())}{pretty(int(N[~ae].sum()))}")
    print(f"  a-çift sûre sayısı: {int(ae.sum())} / a-tek: {int((~ae).sum())}")
    print(f"  19|n konumlarındaki a değerleri: {A[N % 19 == 0].tolist()}  Σ={int(A[N % 19 == 0].sum())}{pretty(int(A[N % 19 == 0].sum()))}")
    print(f"  19|n konumlarındaki t değerleri: {t[N % 19 == 0].tolist()}  Σ={int(t[N % 19 == 0].sum())}{pretty(int(t[N % 19 == 0].sum()))}")

stats("RK-127 (Tevbe=127)", np.where(N == 9, 127, A0))
stats("STD-129 (Tevbe=129)", A0)
