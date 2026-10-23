#!/usr/bin/env python3
"""Ö0(b) aday analizi: tab/cikarim/c<kodeks>_<kaynak>.bin sayılarından ikili (X→Y) ve üçlü ({X1,X2}→Y) gerektirme ADAYLARI.
Aday = örneklemde karşı-örneği olmayan çift/üçlü. Kesinlik bu betiğin işi değildir: her aday ya kanıtlanır ya da
karşı-örnekle düşürülür (ONKAYIT §Ö0(b)). Kullanım: python3 -I src/cikarim_analiz.py <kodeks> <kaynaklar...>"""
import sys, struct, numpy as np, json
kod = sys.argv[1]; kaynaklar = sys.argv[2:]
NF = 99
# olgu adları ve düğümleri: ortak.h init_facts() sırasıyla (C programından dökülür)
meta = json.load(open('sonuc/olgu_meta.json'))
FN = meta['ad']; FD = meta['dugum']
def oku(k):
    b = open(f'tab/cikarim/c{kod}_{k}.bin', 'rb').read()
    N, nO, nf = struct.unpack('<3Q', b[:24]); assert nf == NF
    o = 24; C1 = np.frombuffer(b, '<u8', NF, o); o += 8*NF
    C2 = np.frombuffer(b, '<u8', NF*NF, o).reshape(NF, NF); o += 8*NF*NF
    C3 = np.frombuffer(b, '<u8', NF**3, o).reshape(NF, NF, NF)
    return N, nO, C1.astype(np.int64), C2.astype(np.int64), C3.astype(np.int64)
toplam = None
for k in kaynaklar:
    N, nO, C1, C2, C3 = oku(k)
    if toplam is None: toplam = [N, nO, C1.copy(), C2.copy(), C3.copy()]
    else:
        toplam[0] += N; toplam[1] += nO; toplam[2] += C1; toplam[3] += C2; toplam[4] += C3
N, nO, C1, C2, C3 = toplam
# simetrik tam matrisler
P2 = C2 + C2.T; np.fill_diagonal(P2, C1)
def c3(i, j, k):
    a = sorted((i, j, k)); return C3[a[0], a[1], a[2]]
print(f"kaynaklar {kaynaklar}: N={N:,} kitap (O-kitap {nO:,})")
hic = [f for f in range(NF) if C1[f] == 0]
print(f"\nHİÇ GÖRÜLMEYEN olgular ({len(hic)}):")
for f in hic: print(f"   [{FD[f]:>2}] {FN[f]}")
# ikili adaylar: X→Y (X≠Y), n(X)>0, n(X∧Y)=n(X)
ikili = []
for x in range(NF):
    if C1[x] == 0: continue
    for y in range(NF):
        if y == x: continue
        if P2[x, y] == C1[x]: ikili.append((x, y))
print(f"\nİKİLİ ADAYLAR X→Y ({len(ikili)}); düğüm-arası olanlar işaretli (*):")
for x, y in ikili:
    tag = '*' if FD[x] != FD[y] else ' '
    print(f" {tag} [{FD[x]:>2}] {FN[x]:<42} → [{FD[y]:>2}] {FN[y]:<42} n(X)={C1[x]:,}")
imp = set(ikili)
# üçlü adaylar: {X1,X2}→Y, n(X1∧X2)>0, n(X1∧X2∧Y)=n(X1∧X2), Y ne X1'den ne X2'den ikili olarak çıkmıyor; Y∉{X1,X2}
uclu = []
for x1 in range(NF):
    for x2 in range(x1+1, NF):
        n12 = P2[x1, x2]
        if n12 == 0: continue
        for y in range(NF):
            if y in (x1, x2): continue
            if (x1, y) in imp or (x2, y) in imp: continue
            if c3(x1, x2, y) == n12: uclu.append((x1, x2, y, n12))
print(f"\nÜÇLÜ ADAYLAR {{X1,X2}}→Y ({len(uclu)}), Y'nin düğümü X1 ve X2'ninkinden farklı olanlar (*):")
for x1, x2, y, n12 in uclu:
    tag = '*' if (FD[y] != FD[x1] and FD[y] != FD[x2]) else ' '
    print(f" {tag} {{[{FD[x1]}] {FN[x1]} , [{FD[x2]}] {FN[x2]}}} → [{FD[y]}] {FN[y]}   n(X1∧X2)={n12:,}")
