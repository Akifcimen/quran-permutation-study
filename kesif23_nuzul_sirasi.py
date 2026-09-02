#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""KESIF-23: Kırmızı takımın kontrol önerisi — aynı 114 ayet sayısının GERÇEK bir başka dizilişi:
nüzul (iniş) sırası. Sentetik değil, tarihsel bir alternatif π. Mushaf dizilişindeki yapı
(t = mushaf sırası + ayet) nüzul dizilişinde de var mı? kesif11 bataryası + kesif19 katı/çatallı
serbest-mercek puanlaması uygulanır."""
import json
import numpy as np
from kesif22_incil_catalli import score_n, FORMS, MODS

qd = json.load(open('quran_meta.json'))
K0 = np.array([s['numberOfAyahs'] for s in qd['data']['surahs']['references']], dtype=np.int64)
N = np.arange(1, 115)
A_mus = np.where(N == 9, 127, K0)
ch = json.load(open('quran_chapters_qcom.json'))['chapters']
rev = {c['id']: c['revelation_order'] for c in ch}
order = sorted(range(1, 115), key=lambda s: rev[s])          # nüzul sırasına göre sûre id'leri
A_nuz = np.array([A_mus[s - 1] for s in order])              # konum i = i'nci inen sûrenin ayet sayısı
assert sorted(A_nuz.tolist()) == sorted(A_mus.tolist())

def battery(A):
    A = np.asarray(A, dtype=np.int64); t = N + A; u = N - A; total = int(A.sum()); te = (t % 2 == 0)
    A1 = int(te.sum()) == 57 and int(t[te].sum()) == total
    fam = (t % 19 == 0); B_ = int(fam.sum()) == 12 and int(t[fam].sum()) == 1444
    C_ = B_ and int((fam & te).sum()) == 6 and int(t[fam & te].sum()) == 722
    fu = (u % 19 == 0); uni = fam | fu
    W_ = int(fu.sum()) == 7 and int(uni.sum()) == 19 and int(A[uni].sum()) % 19 == 0
    return A1, B_, C_, W_, int(te.sum()), int(fam.sum()), int(t[fam].sum()), int(fu.sum())

for ad, A in (("MUSHAF dizilişi", A_mus), ("NÜZUL dizilişi ", A_nuz)):
    A1, B_, C_, W_, ne, nf, sf, nu = battery(A)
    best_s = best_f = 0; where = None
    for al, be in FORMS:
        for m in MODS:
            for prim in (True, False):
                s, f = score_n(A, al, be, m, prim)
                best_s = max(best_s, s)
                if f > best_f: best_f, where = f, (al, be, m, 't' if prim else 'u')
    print(f"{ad}: batarya terazi {'✓' if A1 else '✗'}({ne} çift) aile {'✓' if B_ else '✗'}({nf} üye, Σt={sf}) iç {'✓' if C_ else '✗'} İkiEl {'✓' if W_ else '✗'}({nu} u-üye) | serbest mercek: katı {best_s}/8, çatallı {best_f}/8 @ {where}")
