# K2 içeren iki aday kural için geniş uzayda rastgele karşı-örnek araması (D1 = t2 parite yapısı: satır 0 çift q, satır 1 tek q).
import random
def kosu(v):
    s = sorted(set(v)); return len(s) >= 2 and s[-1] - s[0] + 1 == len(s)
def ara(p, isaret_serbest, deneme, tohum):
    rnd = random.Random(tohum); bul = {'F1∧K2∧Qh→F2': None, 'K2∧Qc∧Qh→F2': None}
    for _ in range(deneme):
        m = rnd.randint(-15 if isaret_serbest else 1, 12); w = rnd.randint(1, 8)    # bant [m, m+w]
        band = list(range(m, m + w + 1))
        ev = [x for x in band if x % 2 == 0]; od = [x for x in band if x % 2 != 0]
        if not ev or not od: continue
        c = [rnd.randint(1, 7) for _ in range(4)]
        cells = [[rnd.choice(ev) for _ in range(c[0])], [rnd.choice(ev) for _ in range(c[1])],
                 [rnd.choice(od) for _ in range(c[2])], [rnd.choice(od) for _ in range(c[3])]]
        allv = sum(cells, [])
        if not kosu(allv): continue
        s = [sum(x) for x in cells]; R0, R1 = s[0] + s[1], s[2] + s[3]
        F1 = c[0] + c[1] == c[2] + c[3]; F2 = R0 == R1; Qh = R0 % p == 0 and R1 % p == 0; Qc = R0 + R1 == 4 * p
        if F1 and Qh and not F2 and bul['F1∧K2∧Qh→F2'] is None: bul['F1∧K2∧Qh→F2'] = cells
        if Qc and Qh and not F2 and bul['K2∧Qc∧Qh→F2'] is None: bul['K2∧Qc∧Qh→F2'] = cells
    return bul
for p in (3, 5, 7, 11, 19):
    for serbest in (False, True):
        b = ara(p, serbest, 3_000_000, 1000 + p)
        print(f'p={p} {"serbest" if serbest else "pozitif"}:', {k: (v if v is None else [sorted(x) for x in v]) for k, v in b.items()})
