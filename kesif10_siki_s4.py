#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Sıkı-S4 duyarlılık ölçümü: S4'teki 'boş kefe' dejenereliği (boş toplam = 0,
her m²'ye bölünür) kapatılırsa özgüllük karşılaştırması nasıl değişir?
Sıkı kural: iki parite kefesi de DOLU olacak (ke>0 ve k-ke>0).
Sonuç (3000 sentetik, tohum 53): gerçek kitap 8/8 → 8/8 (etkilenmez);
sentetiklerde ≥4 kuyruğu %4,8 → %1,8 (~2,7×), puan-5 5→1 (~5×).
Yorum: ana metindeki gevşek versiyon rastgeleliğin LEHİNE (tutucu) seçimdir."""
import json
import numpy as np
from collections import Counter
d=json.load(open('/Users/dejuremacminim1/Desktop/hrf2/quran_meta.json'))
A0=np.array([s['numberOfAyahs'] for s in d['data']['surahs']['references']])
N=np.arange(1,115)
Areal=np.where(N==9,127,A0)
def skorla(P,siki):
    T=P+N; U=N-P; TE=(T%2==0); best=0
    for m in range(3,41):
        FT=(T%m==0); FU=(U%m==0)
        k=int(FT.sum()); st=int(T[FT].sum())
        ke=int((FT&TE).sum()); se=int(T[FT&TE].sum())
        uni=FT|FU; f=0
        f+=int(k>=2*114/m)
        f+=int(k>0 and st%(m*m)==0)
        f+=int(k>0 and 2*ke==k and 2*se==st)
        s4=k>0 and se%(m*m)==0 and (st-se)%(m*m)==0
        f+=int(s4 and ((ke>0 and k-ke>0) if siki else True))
        if k>0:
            co=np.sort(T[FT]//m)
            f+=int(len(set(co.tolist()))==int(co.max()-co.min()+1))
        f+=int(int(uni.sum())==m)
        f+=int(int(P[uni].sum())%m==0)
        f+=int(not bool((FT&FU).any()))
        best=max(best,f)
    return best
print("gerçek:", skorla(Areal,False), skorla(Areal,True))
rng=np.random.default_rng(53)
g=Counter(); s=Counter()
for i in range(3000):
    P=rng.permutation(Areal)
    g[skorla(P,False)]+=1; s[skorla(P,True)]+=1
print("gevşek:",dict(sorted(g.items())),"\nsıkı:",dict(sorted(s.items())))
