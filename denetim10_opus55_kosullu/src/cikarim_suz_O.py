#!/usr/bin/env python3
"""O-koşullu kitaplarda (kaynak O) ek gerektirme adayları: O'nun gerektirdiği (her O-kitapta doğru) olgular ve hiç görülmeyenler
çıkarılır; düğüm-arası ikililer ve puanlamayı etkileyebilecek üçlüler (üç farklı düğüm, kanıtlı ikililerle bağlı olmayan) listelenir.
Kullanım: python3 -I src/cikarim_suz_O.py <kodeks> [kaynak=O]"""
import sys, struct, numpy as np, json
kod = sys.argv[1]; kaynaklar = sys.argv[2:] if len(sys.argv) > 2 else ['O']; kay = '+'.join(kaynaklar)
NF = 99; meta = json.load(open('sonuc/olgu_meta.json')); FN = meta['ad']; FD = meta['dugum']; ix = {n: i for i, n in enumerate(FN)}
N = 0; C1 = C2 = C3 = None
for k in kaynaklar:
    b = open(f'tab/cikarim/c{kod}_{k}.bin', 'rb').read(); n, nO, nf = struct.unpack('<3Q', b[:24]); o = 24; N += n
    a1 = np.frombuffer(b, '<u8', NF, o).astype(np.int64); o += 8*NF
    a2 = np.frombuffer(b, '<u8', NF*NF, o).reshape(NF, NF).astype(np.int64); o += 8*NF*NF
    a3 = np.frombuffer(b, '<u8', NF**3, o).reshape(NF, NF, NF).astype(np.int64)
    C1 = a1 if C1 is None else C1 + a1; C2 = a2 if C2 is None else C2 + a2; C3 = a3 if C3 is None else C3 + a3
P2 = C2 + C2.T; np.fill_diagonal(P2, C1)
def c3(i, j, k): a = sorted((i, j, k)); return C3[a[0], a[1], a[2]]
hep = [f for f in range(NF) if C1[f] == N]; hic = [f for f in range(NF) if C1[f] == 0]
print(f"kaynak {kay}: N={N:,} O-kitap. Her kitapta doğru (O'nun gerektirdiği adaylar): {[FN[f] for f in hep]}")
print(f"Hiç görülmeyen: {[FN[f] for f in hic]}")
KANITLI = [("F: Σn=Σa","U: m|Σw(U)"),("F: m|Σn","U: m|Σw(U)"),("G: m|Σn","U: m|Σv(U)"),("G: Σn=Σa","G: m²|Σq"),
  ("G: v-yarılar m²|","G: m²|Σq"),("G: n-yarılar m²|","G: m²|Σq"),("G: Σq kare","G: m²|Σq"),("F: Σn=Σa","F: m|Σn"),("G: k=m","G: k≥2E"),
  ("F: hücreler dengeli","F: v-yarılar eşit"),("G: hücreler dengeli","G: v-yarılar eşit")]
bag = set()
for x, y in KANITLI: bag.add((ix[x], ix[y])); bag.add((ix[y], ix[x]))
skip = set(hep) | set(hic) | {f for f in range(NF) if FN[f].startswith('MX:')}
ikili = {(x, y) for x in range(NF) if x not in skip for y in range(NF) if y != x and y not in skip and P2[x, y] == C1[x]}
print("\nDüğüm-arası ikili adaylar (O içinde):")
for x, y in sorted(ikili):
    if FD[x] != FD[y]: print(f"   [{FD[x]}] {FN[x]} → [{FD[y]}] {FN[y]}   n(X)={C1[x]:,}  {'(kanıtlı)' if (x,y) in bag else ''}")
out = []
for x1 in range(NF):
    if x1 in skip: continue
    for x2 in range(x1+1, NF):
        if x2 in skip or FD[x1] == FD[x2]: continue
        n12 = P2[x1, x2]
        if n12 == 0: continue
        for y in range(NF):
            if y in skip or y in (x1, x2) or FD[y] in (FD[x1], FD[x2]): continue
            if (x1, y) in ikili or (x2, y) in ikili or (x1, y) in bag or (x2, y) in bag: continue
            if c3(x1, x2, y) == n12: out.append((n12, x1, x2, y))
out.sort(reverse=True)
print(f"\nPuanlamayı etkileyebilecek üçlü adaylar (O içinde): {len(out)}")
for n12, x1, x2, y in out: print(f"   {{[{FD[x1]}] {FN[x1]} , [{FD[x2]}] {FN[x2]}}} → [{FD[y]}] {FN[y]}   n(X1∧X2)={n12:,}")
