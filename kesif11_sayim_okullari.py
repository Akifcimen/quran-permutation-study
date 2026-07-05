#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Sayım okulları sağlamlık testi: çekirdek katmanlar Medine sayımında (Warş/Kalûn,
resmî 6214) tutuyor mu? Veri: sayim_medine_warsh.json (kaynağı içinde; Hafs çaprazı doğrulandı).
SONUÇ: TUTMUYOR — terazi 61/53, aile 7 üye, İki El birleşimi 18. Yapı Kûfe sayımına özgüdür.
Okul-seçimi serbestliğinin en kötü Bonferroni bedeli ~×7 (bilinen okul sayısı) → çekirdek ~10⁻¹⁴."""
import json
import numpy as np
N=np.arange(1,115)
def test(A, ad):
    A=np.asarray(A); t=N+A; u=N-A; total=int(A.sum()); te=(t%2==0)
    A1=int(te.sum())==57 and int(t[te].sum())==total
    fam=(t%19==0); B_=int(fam.sum())==12 and int(t[fam].sum())==1444
    C_=B_ and int((fam&te).sum())==6 and int(t[fam&te].sum())==722
    fu=(u%19==0); uni=fam|fu
    W_=int(fu.sum())==7 and int(uni.sum())==19 and int(A[uni].sum())%19==0
    print(f"{ad}: toplam={total} | terazi {'✓' if A1 else '✗'} | aile {'✓' if B_ else '✗'} | iç terazi {'✓' if C_ else '✗'} | İki El {'✓' if W_ else '✗'}")
d=json.load(open('quran_meta.json'))
K=np.array([s['numberOfAyahs'] for s in d['data']['surahs']['references']])
test(np.where(N==9,127,K),"Kûfe-127")
test(K,"Kûfe-129")
M=json.load(open('sayim_medine_warsh.json'))['ayetler']
test(M,"Medine (Warş/Kalûn 6214)")
