# K1 ∧ Qc ∧ K2 altında (pozitif q, D1 = t2) hücre sayılarının zorlanıp zorlanmadığını KESİN sayar.
# K1 ∧ Qc ⇒ hücre toplamları: satır 0 = {p−1, p+1} (çift parçalar), satır 1 = (p, p) (tek parçalar).
# Her hücre için bütün bölüntüler sayılır; K2 = bütün farklı değerler kesintisiz bant.
import sys, itertools
from collections import defaultdict
def bolunumler(n, parcalar):
    parcalar = sorted(parcalar, reverse=True); out = []
    def rec(kalan, i, cur):
        if kalan == 0: out.append(tuple(cur)); return
        for j in range(i, len(parcalar)):
            x = parcalar[j]
            if x <= kalan: cur.append(x); rec(kalan - x, j, cur); cur.pop()
    rec(n, 0, []); return out
def analiz(p):
    ciftler = list(range(2, p + 2, 2)); tekler = list(range(1, p + 1, 2))
    A = bolunumler(p - 1, ciftler); B = bolunumler(p + 1, ciftler); C = bolunumler(p, tekler)
    # değer kümesi → olası sayılar
    def grupla(L):
        g = defaultdict(set)
        for b in L: g[frozenset(b)].add(len(b))
        return g
    gA, gB, gC = grupla(A), grupla(B), grupla(C)
    vektorler = set()
    for sa, na in gA.items():
        for sb, nb in gB.items():
            sab = sa | sb
            for sc, nc in gC.items():
                for sd, nd in gC.items():
                    S = sab | sc | sd
                    if len(S) >= 2 and max(S) - min(S) + 1 == len(S):
                        for x in na:
                            for y in nb:
                                for z in nc:
                                    for w in nd: vektorler.add((x, y, z, w))
    return len(A), len(B), len(C), vektorler
for p in [int(x) for x in sys.argv[1:]]:
    a, b, c, V = analiz(p)
    F1 = all(x + y == z + w for x, y, z, w in V); R0c = all(x == y for x, y, z, w in V); R1c = all(z == w for x, y, z, w in V)
    print(f'p={p}: bölüntü sayıları {a},{b},{c}; K2 ile olası (c00,c01,c10,c11) vektörleri {len(V)}; '
          f'F1 zorunlu: {F1}, R0c zorunlu: {R0c}, R1c zorunlu: {R1c}; örnek: {sorted(V)[:6]}')
