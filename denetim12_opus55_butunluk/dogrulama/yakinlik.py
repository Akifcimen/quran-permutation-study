# How close do stored random books get to the Quran's own profile at its own lens (1,+1,19)? (candidate files only:
# books with count credit >= 9 or near the info threshold, so low-credit books are missing; counts are lower bounds)
import runpy, struct, glob, sys, collections, os
B = runpy.run_path(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'bagimsiz.py'), run_name='lib')
S = os.environ.get('BUTUNLUK', os.path.expanduser('~/opus5.5/butunluk'))  # oturumun tam (açılmış) paketi
TF = struct.Struct('<114H8f8f8f8h8B4BhQ'); RF = struct.Struct('<114H4BhQ')
QSET = ['B1','B2','F1','F2','R0c','R1c','R1e','K1','K2','Qc','QS','Qh','W1','QU','Qa']
for X in sys.argv[1:]:
    seen = {}
    for kind, st in (('R', RF), ('T', TF)):
        for fn in sorted(glob.glob(f'{S}/tab/{kind}_{X}/b*.aday')):
            b = open(fn, 'rb').read(); seg = fn.split('/')[-1][:4]
            for o in range(0, len(b) - len(b) % st.size, st.size):
                r = st.unpack_from(b, o); seen.setdefault((kind, seg, r[-1]), r[:114])
    steps = [('12 üye, 3/3/3/3', lambda P, i: i['kF'] == 12 and i['cnt'] == [3,3,3,3]),
             ('+ K1 ∧ Qc (satırlar 18/20, 19/19; merkez 19)', lambda P, i: P['K1'] and P['Qc']),
             ('+ K2 (bant)', lambda P, i: P['K2']),
             ('+ W1 ∧ |U| = 19', lambda P, i: P['W1'] and P['QU']),
             ('+ Qa', lambda P, i: P['Qa']),
             ('+ B1 (57/57)', lambda P, i: P['B1']),
             ('+ B2 (terazi)', lambda P, i: P['B2'])]
    cnt = collections.Counter(); hist = collections.Counter(); best = []
    qc_any = 0
    for key, a in seen.items():
        P, info = B['profile'](list(a), 1, 1, 19)
        m = sum(P[x] for x in QSET); hist[m] += 1; best.append((m, key, info, P))
        ok = True
        for j, (name, f) in enumerate(steps):
            ok = ok and f(P, info)
            if ok: cnt[j] += 1
        if info['kF'] == 12 and info['cnt'] == [3,3,3,3] and P['Qc']: qc_any += 1
    print(f'== {X}: aday {len(seen)} kitap; mercek (1,+1,19)')
    for j, (name, _) in enumerate(steps): print(f'   {name}: {cnt[j]}')
    print(f'   (12 üye 3/3/3/3 ∧ Qc, K1 olsun olmasın: {qc_any})')
    print('   Kur\'an\'ın 15 yükleminden kaç tanesi (bu mercekte): ' + ', '.join(f'{m}:{hist[m]}' for m in sorted(hist) if m >= 10))
    for m, key, info, P in sorted(best, key=lambda x: -x[0])[:4]:
        print(f'     {m}/15 {key} kF={info["kF"]} hücre={info["cnt"]} toplam={info["s"]} Σq={info["Sq"]} kH={info["kH"]} |U|={info["U"]} eksik: {[x for x in QSET if not P[x]]}')
