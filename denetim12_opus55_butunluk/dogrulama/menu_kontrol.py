# Independent check of the split-menu analysis (ONKAYIT §8.4), written from the prereg text; reuses my primary predicates' logic.
import runpy, struct, glob, random, sys, collections, os
B = runpy.run_path(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'bagimsiz.py'), run_name='lib')
lens, consecutive, closure, credit, FAM, K129 = B['lens'], B['consecutive'], B['closure'], B['credit'], B['FAM'], B['K129']
SPL = ['t2', 'n2', 'a2', 'h', 't4']
PAIRS = [(d1, d2) for d1 in SPL for d2 in SPL if d1 != d2]

def cls(name, n, a, t):
    return {'t2': t % 2, 'n2': n % 2, 'a2': a % 2, 'h': 0 if n <= 57 else 1, 't4': 0 if t % 4 in (0, 1) else 1}[name]

def structure(d1, d2, al, be):
    teq = {'t2'} | ({'a2'} if al % 2 == 0 else set()) | ({'n2'} if be % 2 == 0 else set())
    if d1 in teq: return 'satir'
    if d2 in teq: return 'sutun'
    if {d1, d2} == {'n2', 'a2'} and al % 2 and be % 2: return 'kosegen'
    return 'paritesiz'

def menu_rules(st, al, be, p):
    if st == 'satir': return B['rules'](al, be, p)
    R = [({'Qc'}, {'QS'}), ({'Qh'}, {'QS'}), ({'F2', 'QS'}, {'Qh'})]
    if (al*be) % p: R.append(({'QkF', 'QkH', 'QU'}, {'Qn', 'Qa'}))
    if p >= 5: R.append(({'K1', 'Qh'}, {'F2'}))
    if st == 'paritesiz':
        R += [({'R0e', 'K1', 'Qc'}, {'F2'}), ({'R1e', 'K1', 'Qc'}, {'F2'})]
        if be > 0: R += [({'R0e', 'Qc', 'Qh'}, {'F2'}), ({'R1e', 'Qc', 'Qh'}, {'F2'})]
        if p == 3: R.append(({'K1', 'Qc', 'Qh'}, {'F2'}))
    return R

def menu_scores(a):
    best = {'tum': (-1, None, None), 't2siz': (-1, None, None), 'bir': (-1, None, None)}
    N = range(1, 115)
    for L in range(406):
        al, be, p = lens(L)
        t = [al*n + be*x for n, x in zip(N, a)]; u = [al*n - be*x for n, x in zip(N, a)]
        F = [i for i in range(114) if t[i] % p == 0]; H = [i for i in range(114) if u[i] % p == 0]
        q = {i: t[i] // p for i in F}; Sq = sum(q.values()); U = set(F) | set(H)
        lensP = {'K2': len(F) >= 2 and consecutive(q.values()), 'QS': len(F) >= 1 and Sq % p == 0, 'QkF': len(F) == p,
                 'W1': len(F) >= 1 and len(H) >= 1 and not (set(F) & set(H)), 'QU': len(U) == p, 'QkH': len(H) == p,
                 'Qa': len(U) >= 1 and sum(a[i] for i in U) % p == 0, 'Qn': len(U) >= 1 and sum(i + 1 for i in U) % p == 0,
                 'QH': len(H) >= 1 and sum(u[i] // p for i in H) % p == 0}
        ex = set()
        if p == 3 and abs(be) == 3: ex |= {'QU', 'QkF', 'QkH', 'Qn', 'W1'}
        if p == 3 and al == 3: ex |= {'QU', 'QkF', 'QkH', 'Qa', 'W1'}
        for d1, d2 in PAIRS:
            c1 = [cls(d1, i + 1, a[i], t[i]) for i in range(114)]; c2 = [cls(d2, i + 1, a[i], t[i]) for i in range(114)]
            E0 = [i for i in range(114) if c1[i] == 0]; E1 = [i for i in range(114) if c1[i] == 1]
            F0 = [i for i in F if c1[i] == 0]; F1_ = [i for i in F if c1[i] == 1]
            cell = {(r, c): [i for i in (F0 if r == 0 else F1_) if c2[i] == c] for r in (0, 1) for c in (0, 1)}
            s = {k: sum(q[i] for i in v) for k, v in cell.items()}; full = all(cell.values())
            sn = lambda S: sum(i + 1 for i in S); sa = lambda S: sum(a[i] for i in S); st_ = lambda S: sum(t[i] for i in S)
            h0, h1 = sum(q[i] for i in F0), sum(q[i] for i in F1_)
            P = dict(lensP)
            P['B1'] = len(E0) == len(E1)
            P['B2'] = bool(E0) and bool(E1) and (al*sn(E0) == abs(be)*sa(E1) or al*sn(E1) == abs(be)*sa(E0) or st_(E0) == st_(E1))
            P['F1'] = len(F0) == len(F1_) >= 1
            P['F2'] = bool(F0) and bool(F1_) and h0 == h1
            P['R0c'] = len(cell[0, 0]) == len(cell[0, 1]) >= 1
            P['R0e'] = bool(cell[0, 0]) and bool(cell[0, 1]) and s[0, 0] == s[0, 1]
            P['R1c'] = len(cell[1, 0]) == len(cell[1, 1]) >= 1
            P['R1e'] = bool(cell[1, 0]) and bool(cell[1, 1]) and s[1, 0] == s[1, 1]
            P['K1'] = full and consecutive(s.values())
            P['Qc'] = full and Sq == 4*p
            P['Qh'] = bool(F0) and bool(F1_) and h0 % p == 0 and h1 % p == 0
            exc = set(ex)
            if d1 in ('n2', 'a2', 'h') or (d1 == 't2' and (al % 2 == 0 or be % 2 == 0)): exc.add('B1')
            sat = {k for k, v in P.items() if v} - exc
            if not (len(F) >= 2*114/p and all(len(v) >= 2 for v in cell.values())): sat -= FAM
            c = credit(sat, menu_rules(structure(d1, d2, al, be), al, be, p))
            for key, ok in (('tum', True), ('t2siz', 't2' not in (d1, d2)), ('bir', (d1, d2) == ('t2', 'n2'))):
                if ok and c > best[key][0]: best[key] = (c, (d1, d2), (al, be, p))
    return best

MF = struct.Struct('<114H3B2B2hQ')
assert MF.size == 245
if __name__ == '__main__':
    q = menu_scores(K129)
    print("Kur'an (129):", q)
    S = os.environ.get('BUTUNLUK', os.path.expanduser('~/opus5.5/butunluk'))  # oturumun tam (açılmış) paketi
    random.seed(271027)
    for X in sys.argv[1:]:
        recs = {}
        for fn in sorted(glob.glob(f'{S}/tab/D_{X}/b*.aday')):
            b = open(fn, 'rb').read(); seg = fn.split('/')[-1][:4]
            for o in range(0, len(b) - len(b) % MF.size, MF.size):
                r = MF.unpack_from(b, o); recs.setdefault((seg, r[-1]), r)
        tum = [k for k, r in recs.items() if r[114] >= 11]; t2s = [k for k, r in recs.items() if r[115] >= 9]
        bir = [k for k, r in recs.items() if r[116] >= 11]
        print(f'== {X}: aday {len(recs)}; tum ≥ 11: {len(tum)}, t2siz ≥ 9: {len(t2s)}, bir ≥ 11: {len(bir)}')
        check = sorted(set(tum) | set(random.sample(t2s, min(15, len(t2s))) )
                       | set(random.sample(list(recs), min(15, len(recs)))))
        mism = 0
        for k in check:
            r = recs[k]; m = menu_scores(list(r[:114]))
            mine = (m['tum'][0], m['t2siz'][0], m['bir'][0])
            if mine != tuple(r[114:117]): mism += 1; print('   UYUŞMAZLIK', k, 'oturum', r[114:117], 'benim', mine, m)
        print(f'   yeniden puanlanan {len(check)} kitap (tum ≥ 11 hepsi + örneklem): uyuşmazlık {mism}')
