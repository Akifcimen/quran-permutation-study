# Independent re-implementation of the bütünlük language L (ONKAYIT §2.1–2.9, count score) written from the prereg text only,
# without reading the session's dil.py / butunluk.h. Used to re-check the Quran's profile and the stored candidate books.
import itertools, json, os

REPO = os.path.expanduser('~/Desktop/hrf2')
K129 = [s['numberOfAyahs'] for s in json.load(open(f'{REPO}/quran_meta.json'))['data']['surahs']['references']]
PR = [3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113]
AL = [1,1,1,1,1,1,2,2,2,2,3,3,3,3]; BE = [1,-1,2,-2,3,-3,1,-1,3,-3,1,-1,2,-2]
NAMES = ['B1','B2','F1','F2','R0c','R0e','R1c','R1e','K1','K2','Qc','QS','Qh','QkF','W1','QU','QkH','Qa','Qn','QH']
FAM = {'F1','F2','R0c','R0e','R1c','R1e','K1','K2','Qc','QS','Qh','QkF'}      # gated family/cell predicates (§2.8)

def lens(L): f, i = divmod(L, 29); return AL[f], BE[f], PR[i]

def consecutive(vals):
    v = sorted(set(vals)); return len(v) >= 2 and v == list(range(v[0], v[-1] + 1))

def profile(a, al, be, p):
    """Satisfied predicates (before constant-exclusion and gate) plus structure info."""
    N = range(1, 115)
    t = [al*n + be*x for n, x in zip(N, a)]; u = [al*n - be*x for n, x in zip(N, a)]
    E0 = [i for i in range(114) if t[i] % 2 == 0]; E1 = [i for i in range(114) if t[i] % 2 != 0]
    F = [i for i in range(114) if t[i] % p == 0]; H = [i for i in range(114) if u[i] % p == 0]
    q = {i: t[i] // p for i in F}
    F0 = [i for i in F if t[i] % 2 == 0]; F1_ = [i for i in F if t[i] % 2 != 0]
    cell = {(r, c): [i for i in (F0 if r == 0 else F1_) if (i + 1) % 2 == c] for r in (0, 1) for c in (0, 1)}
    s = {k: sum(q[i] for i in v) for k, v in cell.items()}
    full = all(len(v) > 0 for v in cell.values())
    U = sorted(set(F) | set(H)); Sq = sum(q.values())
    sn = lambda S: sum(i + 1 for i in S); sa = lambda S: sum(a[i] for i in S); st = lambda S: sum(t[i] for i in S)
    P = {}
    P['B1'] = len(E0) == len(E1)
    P['B2'] = bool(E0) and bool(E1) and (al*sn(E0) == abs(be)*sa(E1) or al*sn(E1) == abs(be)*sa(E0) or st(E0) == st(E1))
    P['F1'] = len(F0) == len(F1_) >= 1
    P['F2'] = bool(F0) and bool(F1_) and sum(q[i] for i in F0) == sum(q[i] for i in F1_)
    P['R0c'] = len(cell[0, 0]) == len(cell[0, 1]) >= 1
    P['R0e'] = bool(cell[0, 0]) and bool(cell[0, 1]) and s[0, 0] == s[0, 1]
    P['R1c'] = len(cell[1, 0]) == len(cell[1, 1]) >= 1
    P['R1e'] = bool(cell[1, 0]) and bool(cell[1, 1]) and s[1, 0] == s[1, 1]
    P['K1'] = full and consecutive(s.values())
    P['K2'] = len(F) >= 2 and consecutive(q.values())
    P['Qc'] = full and Sq == 4*p
    P['QS'] = len(F) >= 1 and Sq % p == 0
    P['Qh'] = bool(F0) and bool(F1_) and sum(q[i] for i in F0) % p == 0 and sum(q[i] for i in F1_) % p == 0
    P['QkF'] = len(F) == p
    P['W1'] = len(F) >= 1 and len(H) >= 1 and not (set(F) & set(H))
    P['QU'] = len(U) == p
    P['QkH'] = len(H) == p
    P['Qa'] = len(U) >= 1 and sa(U) % p == 0
    P['Qn'] = len(U) >= 1 and sn(U) % p == 0
    P['QH'] = len(H) >= 1 and sum(u[i] // p for i in H) % p == 0
    info = dict(kF=len(F), kH=len(H), U=len(U), cnt=[len(cell[k]) for k in sorted(cell)], s=[s[k] for k in sorted(s)],
                q=sorted(q.values()), Sq=Sq, gate_G=len(F) >= 2*114/p and all(len(v) >= 2 for v in cell.values()),
                E0=len(E0),
                zero={k for k, z in (('F2', sum(q[i] for i in F0) == 0 and sum(q[i] for i in F1_) == 0),
                                     ('R0e', s[0, 0] == 0 and s[0, 1] == 0), ('R1e', s[1, 0] == 0 and s[1, 1] == 0),
                                     ('QS', Sq == 0),
                                     ('Qh', sum(q[i] for i in F0) == 0 or sum(q[i] for i in F1_) == 0),
                                     ('QH', sum(u[i] // p for i in H) == 0)) if z})
    return P, info

def constant_excluded(al, be, p):
    ex = set()
    if al % 2 == 0 or be % 2 == 0: ex.add('B1')
    if p == 3 and abs(be) == 3: ex |= {'QU', 'QkF', 'QkH', 'Qn', 'W1'}
    if p == 3 and al == 3: ex |= {'QU', 'QkF', 'QkH', 'Qa', 'W1'}
    return ex

def rules(al, be, p):
    R = [({'Qc'}, {'QS'}), ({'Qh'}, {'QS'}), ({'F2', 'QS'}, {'Qh'}), ({'K1', 'Qc'}, {'F2', 'R1e'})]
    if (al*be) % p != 0: R.append(({'QkF', 'QkH', 'QU'}, {'Qn', 'Qa'}))
    if p >= 5: R.append(({'K1', 'Qh'}, {'F2'}))
    if p == 3: R += [({'R1c', 'K1', 'Qh'}, {'F2'}), ({'R1e', 'K1', 'Qh'}, {'F2'})]
    if be > 0: R.append(({'Qc', 'Qh'}, {'F2'}))
    if p == 3 and be > 0:
        R += [({'Qc','F1','F2'}, {'R0c','R1c'}), ({'Qc','F1','R0e'}, {'R0c'}), ({'Qc','F1','R1e'}, {'R1c'}),
              ({'Qc','F1','K1'}, {'R0c','R1c','K2'}), ({'Qc','F1','Qh'}, {'R0c','R1c'}), ({'Qc','F2','R1e'}, {'K1'}),
              ({'Qc','R1e','Qh'}, {'K1'})]
    return R

def closure(S, R):
    S = set(S); ch = True
    while ch:
        ch = False
        for pre, post in R:
            if pre <= S and not post <= S: S |= post; ch = True
    return S

def credit(S, R):
    """Size of the smallest subset of S whose closure covers S. Members not derivable from the rest are in every generating set."""
    S = set(S)
    must = {x for x in S if x not in closure(S - {x}, R)}
    rest = sorted(S - must)
    for k in range(len(rest) + 1):
        for B in itertools.combinations(rest, k):
            if S <= closure(must | set(B), R): return len(must) + k
    return len(S)

def all_scores(a):
    """Max credit over lenses for gates G, GE, Gc, G0 and post-hoc S0 (gate G), with the best lens for G and S0."""
    best = {g: (-1, None) for g in ('G', 'GE', 'Gc', 'G0', 'S0')}
    for L in range(406):
        al, be, p = lens(L)
        P, info = profile(a, al, be, p)
        sat = {k for k, v in P.items() if v} - constant_excluded(al, be, p)
        R = rules(al, be, p)
        cells_ok = min(info['cnt']) >= 2
        gates = {'G': info['gate_G'], 'GE': info['kF'] >= 114/p and cells_ok, 'Gc': cells_ok, 'G0': True}
        for g, ok in gates.items():
            c = credit(sat if ok else sat - FAM, R)
            if c > best[g][0]: best[g] = (c, L)
        z = info['zero']
        s0 = (sat if gates['G'] else sat - FAM) - z
        c = credit(s0, R)
        if c > best['S0'][0]: best['S0'] = (c, L)
    return best

def count_lens(a, L, gate='G'):
    al, be, p = lens(L)
    P, info = profile(a, al, be, p)
    sat = {k for k, v in P.items() if v} - constant_excluded(al, be, p)
    if gate == 'G' and not info['gate_G']: sat -= FAM
    return credit(sat, rules(al, be, p)), sat, info

def count_score(a, gate='G'):
    best = (-1, None)
    for L in range(406):
        c, _, _ = count_lens(a, L, gate)
        if c > best[0]: best = (c, L)
    return best

if __name__ == '__main__':
    for name, a in (('129', K129), ('127', [127 if n == 9 else K129[n-1] for n in range(1, 115)])):
        res = sorted(((count_lens(a, L)[0], L) for L in range(406)), reverse=True)[:4]
        c, sat, info = count_lens(a, res[0][1])
        print(name, 'en iyi:', [(cc, lens(L)) for cc, L in res], '| sağlanan', len(sat), sorted(sat, key=NAMES.index), info)
