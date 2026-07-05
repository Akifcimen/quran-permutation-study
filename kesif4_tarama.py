#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Konum-bazlı ve ikinci-tur tarama: kümülatif 19-duraklar, a mod 19 histogramı,
ardışık eşit t'ler, u-ailesi iç anatomi, İki El birleşiminin iç yapısı."""
import json
import numpy as np
from collections import Counter

d = json.load(open('/Users/dejuremacminim1/Desktop/hrf2/quran_meta.json'))
A0 = np.array([s['numberOfAyahs'] for s in d['data']['surahs']['references']])
N = np.arange(1, 115)
A = np.where(N == 9, 127, A0)
t = N + A; u = N - A

print("== KÜMÜLATİF DURAKLAR ==")
Sa = np.cumsum(A); St = np.cumsum(t)
stopsA = [int(k+1) for k in range(114) if Sa[k] % 19 == 0]
stopsT = [int(k+1) for k in range(114) if St[k] % 19 == 0]
print(f"19 | Σa(1..k) olan k'ler ({len(stopsA)}): {stopsA}")
print(f"19 | Σt(1..k) olan k'ler ({len(stopsT)}): {stopsT}")
print(f"38 | Σt(1..k) olan k'ler: {[int(k+1) for k in range(114) if St[k] % 38 == 0]}")

print("\n== a mod 19 HİSTOGRAMI (sınıf: üye) ==")
h = Counter((A % 19).tolist())
print(" ", dict(sorted(h.items())))
print("  boy dağılımı:", sorted(Counter(h.values()).items()))

print("\n== ARDIŞIK KOMŞULAR ==")
eq = [(int(n), int(t[n-1])) for n in range(1, 114) if t[n-1] == t[n]]
print(f"t(n) = t(n+1) olan n'ler: {eq}")
dt = np.diff(t)
print(f"t farkı 19'un katı olan komşu sayısı: {int((dt % 19 == 0).sum())} (beklenen ~{113/19:.1f})")
du = np.diff(u)
print(f"u farkı 19'un katı olan komşu sayısı: {int((du % 19 == 0).sum())}")

print("\n== U-AİLESİ İÇ ANATOMİ ==")
fu = (u % 19 == 0); ft = (t % 19 == 0)
nu = N[fu]
print(f"üyeler: {list(nu)}, u-katsayıları: {list(u[fu]//19)}")
print(f"n-parite: çift {int((nu%2==0).sum())} / tek {int((nu%2==1).sum())}")
print(f"t-parite: çift {int((t[fu]%2==0).sum())} / tek {int((t[fu]%2==1).sum())}")
print(f"Σt = {int(t[fu].sum())}, Σn = {int(nu.sum())}, Σa = {int(A[fu].sum())}")
print(f"|u| toplamı = {int(abs(u[fu]).sum())} = 19×{int(abs(u[fu]).sum())//19}")
print(f"pozitif u: {list(u[fu][u[fu]>0])} (Σ={int(u[fu][u[fu]>0].sum())}), negatif: {list(u[fu][u[fu]<0])} (Σ={int(u[fu][u[fu]<0].sum())})")

print("\n== İKİ EL BİRLEŞİMİ İÇ YAPI ==")
bir = ft | fu
tb = t[bir]
print(f"Σt = {int(tb.sum())} (= 3^7 = {3**7}); çift-t Σ = {int(tb[tb%2==0].sum())}, tek-t Σ = {int(tb[tb%2==1].sum())}")
print(f"üye t-pariteleri: çift {int((tb%2==0).sum())} / tek {int((tb%2==1).sum())}")
print(f"Σn = {int(N[bir].sum())}, Σa = {int(A[bir].sum())} = 19×{int(A[bir].sum())//19}")
print(f"Σa(t-eli) = {int(A[ft].sum())} = 61×{int(A[ft].sum())//61}, Σa(u-eli) = {int(A[fu].sum())} = 61×{int(A[fu].sum())//61}")

print("\n== BLOK YAPILARI (19'luk 6 blok) ==")
for isim, dizi in [("aile üyesi", ft), ("u-aile üyesi", fu), ("birleşim", bir)]:
    per = [int(dizi[i*19:(i+1)*19].sum()) for i in range(6)]
    print(f"  {isim} blok dağılımı: {per}")

print("\n== 38'lik 3 blok ==")
for isim, dizi in [("birleşim", bir)]:
    per = [int(dizi[i*38:(i+1)*38].sum()) for i in range(3)]
    print(f"  {isim}: {per}")
sa3 = [int(A[i*38:(i+1)*38].sum()) for i in range(3)]
st3 = [int(t[i*38:(i+1)*38].sum()) for i in range(3)]
print(f"  Σa üçte birler: {sa3} (mod 19: {[x%19 for x in sa3]})")
print(f"  Σt üçte birler: {st3} (mod 19: {[x%19 for x in st3]})")
