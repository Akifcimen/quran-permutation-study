#!/usr/bin/env python3
"""Ö0(b) aday süzgeci: üçlü adaylardan yalnız PUANLAMAYI ETKİLEYEBİLENLERİ bırakır:
öncüller ve sonuç üç farklı düğümde; ve sonuç–öncül çifti zaten kanıtlı bir ikili gerektirmeyle bağlı değil.
Kullanım: python3 -I src/cikarim_suz.py <kodeks> <kaynaklar...>"""
import sys, struct, numpy as np, json
kod = sys.argv[1]; kaynaklar = sys.argv[2:]
NF = 99; meta = json.load(open('sonuc/olgu_meta.json')); FN = meta['ad']; FD = meta['dugum']
ix = {n: i for i, n in enumerate(FN)}
def oku(k):
    b = open(f'tab/cikarim/c{kod}_{k}.bin', 'rb').read()
    N, nO, nf = struct.unpack('<3Q', b[:24]); o = 24
    C1 = np.frombuffer(b, '<u8', NF, o).astype(np.int64); o += 8*NF
    C2 = np.frombuffer(b, '<u8', NF*NF, o).reshape(NF, NF).astype(np.int64); o += 8*NF*NF
    C3 = np.frombuffer(b, '<u8', NF**3, o).reshape(NF, NF, NF).astype(np.int64)
    return N, C1, C2, C3
N = 0; C1 = C2 = C3 = None
for k in kaynaklar:
    n, a, b, c = oku(k); N += n
    C1 = a if C1 is None else C1 + a; C2 = b if C2 is None else C2 + b; C3 = c if C3 is None else C3 + c
P2 = C2 + C2.T; np.fill_diagonal(P2, C1)
def c3(i, j, k): a = sorted((i, j, k)); return C3[a[0], a[1], a[2]]
# kanıtlı ikili gerektirmeler (düğüm içi ve düğüm arası), ad çiftleri:
KANITLI = [("F: Σn=Σa","U: m|Σw(U)"),("F: m|Σn","U: m|Σw(U)"),("G: m|Σn","U: m|Σv(U)"),("G: Σn=Σa","G: m²|Σq"),
  ("F: v-yarılar m²|","F: m²|Σq"),("F: n-yarılar m²|","F: m²|Σq"),("G: v-yarılar m²|","G: m²|Σq"),("G: n-yarılar m²|","G: m²|Σq"),
  ("F: Σq kare","F: m²|Σq"),("G: Σq kare","G: m²|Σq"),("F: Σn=Σa","F: m|Σn"),("F: k=m","F: k≥2E"),("G: k=m","G: k≥2E"),
  ("F: hücreler dengeli","F: v-yarılar eşit"),("G: hücreler dengeli","G: v-yarılar eşit")]
bag = set()
for x, y in KANITLI: bag.add((ix[x], ix[y])); bag.add((ix[y], ix[x]))
ikili = {(x, y) for x in range(NF) if C1[x] for y in range(NF) if y != x and P2[x, y] == C1[x]}
out = []
for x1 in range(NF):
    for x2 in range(x1+1, NF):
        n12 = P2[x1, x2]
        if n12 == 0 or FD[x1] == FD[x2]: continue
        for y in range(NF):
            if y in (x1, x2) or FD[y] in (FD[x1], FD[x2]): continue
            if (x1, y) in ikili or (x2, y) in ikili: continue
            if (x1, y) in bag or (x2, y) in bag: continue
            if c3(x1, x2, y) == n12: out.append((n12, x1, x2, y))
out.sort(reverse=True)
print(f"kaynaklar {kaynaklar}: N={N:,}; puanlamayı etkileyebilecek üçlü aday: {len(out)}")
for n12, x1, x2, y in out:
    print(f"  {{[{FD[x1]}] {FN[x1]} , [{FD[x2]}] {FN[x2]}}} → [{FD[y]}] {FN[y]}   n(X1∧X2)={n12:,}")
