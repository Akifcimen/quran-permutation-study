#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Karşı-korpus (Moby-Dick tarzı) kontrolü: aynı 8 özgüllük ölçütü, gerçek başka
metinlerin bölüm-yapılarına aynen uygulanır. Ön-kayıt: kesif12_karsi_korpus_prereg.md
Veri: korpus_kjv.json (api.getbible.net/v2/kjv.json; kanonik toplamlarla doğrulandı:
Mezmurlar 2461 ayet, 66 kitap/1189 bölüm, KJV 31102 ayet).
SONUÇ: Mezmurlar 2/8, İncil kitapları 3/8, İncil bölümleri 2/8 (motif modülleri
7/12: 0-2 puan; terazi analoğu hiçbirinde yok) — hepsi sentetik gürültü zemininde.
Kur'an aynı fonksiyonla: 8/8 (m=19)."""
import json
import numpy as np
K=json.load(open('korpus_kjv.json'))
corpora={'Mezmurlar':K['psalms_verses'],'İncil-kitaplar':K['book_chapters'],'İncil-bölümler':K['chapter_verses']}
def skorla(A):
    A=np.asarray(A); Nn=len(A); N=np.arange(1,Nn+1)
    t=N+A; u=N-A; TE=(t%2==0); best=(0,None)
    for m in range(3,41):
        FT=(t%m==0); FU=(u%m==0)
        k=int(FT.sum()); st=int(t[FT].sum())
        ke=int((FT&TE).sum()); se=int(t[FT&TE].sum()); uni=FT|FU
        f=int(k>=2*Nn/m)+int(k>0 and st%(m*m)==0)+int(k>0 and 2*ke==k and 2*se==st)
        f+=int(k>0 and se%(m*m)==0 and (st-se)%(m*m)==0)
        if k>0:
            co=np.sort(t[FT]//m)
            f+=int(len(set(co.tolist()))==int(co.max()-co.min()+1))
        f+=int(int(uni.sum())==m)+int(int(A[uni].sum())%m==0)+int(not bool((FT&FU).any()))
        if f>best[0]: best=(f,m)
    return best
for ad,A in corpora.items(): print(ad, skorla(A))
d=json.load(open('quran_meta.json'))
print('Kuran', skorla([s['numberOfAyahs'] for s in d['data']['surahs']['references']]))
