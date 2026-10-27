# p = 3, pozitif q, D1 = t2: Qc (Σq = 12, dört hücre dolu) uzayının KESİN sayımı; bu uzayda geçerli bütün kurallar (Qc öncüllü).
import itertools
from collections import defaultdict
def bolunumler(n, parcalar):
    parcalar = sorted(parcalar, reverse=True); out = []
    def rec(k, i, cur):
        if k == 0: out.append(tuple(cur)); return
        for j in range(i, len(parcalar)):
            if parcalar[j] <= k: cur.append(parcalar[j]); rec(k - parcalar[j], j, cur); cur.pop()
    rec(n, 0, []); return out
p = 3; T = 4 * p
ev = list(range(2, T + 1, 2)); od = list(range(1, T + 1, 2))
AD = ['F1','F2','R0c','R0e','R1c','R1e','K1','K2','Qc','QS','Qh']
def kosu(v):
    s = sorted(set(v)); return len(s) >= 2 and s[-1] - s[0] + 1 == len(s)
maskeler = set()
for s0 in range(2, T + 1, 2):
    for s1 in range(2, T + 1 - s0, 2):
        for s2 in range(1, T - s0 - s1):
            s3 = T - s0 - s1 - s2
            if s3 < 1: continue
            for A in bolunumler(s0, ev):
                for B in bolunumler(s1, ev):
                    for C in bolunumler(s2, od):
                        for D in bolunumler(s3, od):
                            c = [len(A), len(B), len(C), len(D)]; s = [s0, s1, s2, s3]
                            h0, h1 = c[0] + c[1], c[2] + c[3]; q0, q1 = s0 + s1, s2 + s3
                            v = dict(F1=h0 == h1, F2=q0 == q1, R0c=c[0] == c[1], R0e=s0 == s1, R1c=c[2] == c[3], R1e=s2 == s3,
                                     K1=kosu(s), K2=kosu(list(A + B + C + D)), Qc=True, QS=True, Qh=(q0 % p == 0 and q1 % p == 0))
                            maskeler.add(tuple(v[k] for k in AD))
print('p=3 pozitif Qc uzayında farklı maske sayısı:', len(maskeler))
kural = []
for r in (0, 1, 2):
    for onc in itertools.combinations([a for a in AD if a not in ('Qc', 'QS')], r):
        dest = [m for m in maskeler if all(m[AD.index(x)] for x in onc)]
        if not dest: continue
        for y in AD:
            if y in onc or y in ('Qc', 'QS'): continue
            if all(m[AD.index(y)] for m in dest):
                kucuk = any(all(mm[AD.index(y)] for mm in maskeler if all(mm[AD.index(x)] for x in alt)) for rr in range(r) for alt in itertools.combinations(onc, rr))
                if not kucuk: kural.append(' ∧ '.join(('Qc',) + onc) + ' → ' + y)
print('Kesin kurallar (Qc öncüllü, p = 3, pozitif):'); [print('   ', k) for k in kural]
