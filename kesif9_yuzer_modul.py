#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Yüzer-modül sıfır testi: sentetik dizilişlerde HERHANGİ bir m (3..40) tam 8/8
özgüllük profili üretebiliyor mu? (S1-S8 tanımları kesif4_ozgulluk.py ile aynı;
S5 yalnız diğer 7'yi geçen adaylara uygulanır — hız için.)
Sonuç (500k diziliş, tohum 41): 7/8 geçen aday 0; tam profil 0 → p < 6e-6 (%95)."""
import json
import numpy as np
d=json.load(open('/Users/dejuremacminim1/Desktop/hrf2/quran_meta.json'))
A0=np.array([s['numberOfAyahs'] for s in d['data']['surahs']['references']])
N=np.arange(1,115)
A=np.where(N==9,127,A0)
rng=np.random.default_rng(41)
TR=500_000; CH=5_000
aday=tam=0
for _ in range(TR//CH):
    P=rng.permuted(np.tile(A,(CH,1)),axis=1)
    T=P+N; U=N-P
    for m in range(3,41):
        FT=(T%m==0); FU=(U%m==0)
        k=FT.sum(1); s1=k>=2*114/m
        if not s1.any(): continue
        st=(T*FT).sum(1); s2=(st%(m*m)==0)&(k>0)
        TE=(T%2==0); ke=(FT&TE).sum(1); se=(T*(FT&TE)).sum(1)
        s3=(2*ke==k)&(2*se==st); s4=(se%(m*m)==0)&((st-se)%(m*m)==0)
        uni=(FT|FU); s6=uni.sum(1)==m; s7=((P*uni).sum(1)%m==0); s8=~((FT&FU).any(1))
        ok=s1&s2&s3&s4&s6&s7&s8
        for r in np.where(ok)[0]:
            aday+=1
            trow=T[r]; f=trow%m==0
            co=np.sort(trow[f]//m)
            if len(co) and len(set(co.tolist()))==int(co.max()-co.min()+1):
                tam+=1; print("TAM PROFİL: m=",m)
print(f"{TR:,}×38: aday={aday}, tam={tam}")
