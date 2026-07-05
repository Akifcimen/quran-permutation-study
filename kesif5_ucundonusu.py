#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""'Üçün Dönüşü' ölçümü: iki elin katsayı cetvelinde 3 boşluğu ve birleşim toplamının 3^7 olması.
Önceden tanımlı olaylar: N1: Σt(birleşim) 3'ün tam kuvveti; N2: 27|Σt; N3: N1 ∧ parite yarıları 81'e bölünür.
Koşullar: C1: birleşim=19; C2: C1 ∧ Nu=7; C3: W (İki El olayı)."""
import json
import numpy as np
d=json.load(open('/Users/dejuremacminim1/Desktop/hrf2/quran_meta.json'))
A0=np.array([s['numberOfAyahs'] for s in d['data']['surahs']['references']])
N=np.arange(1,115)
A=np.where(N==9,127,A0)
rng=np.random.default_rng(13)
TR=20_000_000; CH=100_000
c=dict(C1=0,C2=0,C3=0,N1c1=0,N1c2=0,N1c3=0,N2c1=0,N3c2=0)
POW3={3**k for k in range(3,9)}
for i in range(TR//CH):
    P=rng.permuted(np.tile(A,(CH,1)),axis=1)
    T=P+N; U=N-P
    FT=(T%19==0); FU=(U%19==0); B=FT|FU
    uni=B.sum(1); nu=FU.sum(1)
    stU=(T*B).sum(1); saU=(P*B).sum(1)
    c1=(uni==19); c2=c1&(nu==7); c3=c2&(saU%19==0)
    n1=np.isin(stU,list(POW3)); n2=(stU%27==0)
    se=(T*(B&(T%2==0))).sum(1)
    n3=n1&(se%81==0)
    c['C1']+=int(c1.sum()); c['C2']+=int(c2.sum()); c['C3']+=int(c3.sum())
    c['N1c1']+=int((n1&c1).sum()); c['N1c2']+=int((n1&c2).sum()); c['N1c3']+=int((n1&c3).sum())
    c['N2c1']+=int((n2&c1).sum()); c['N3c2']+=int((n3&c2).sum())
for k,v in c.items(): print(k,v)
