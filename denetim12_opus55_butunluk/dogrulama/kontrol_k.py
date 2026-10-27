# Recount the session's ranks from its candidate files and re-score books with my independent scorer (bagimsiz.py).
import struct, glob, re, random, runpy, sys, math, os
B = runpy.run_path(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'bagimsiz.py'), run_name='lib')
S = os.environ.get('BUTUNLUK', os.path.expanduser('~/opus5.5/butunluk'))  # oturumun tam (açılmış) paketi
TF = struct.Struct('<114H8f8f8f8h8B4BhQ'); RF = struct.Struct('<114H4BhQ')
GATES = ('G', 'GE', 'Gc', 'G0')

def esik(X):
    d = {}
    for line in open(f'{S}/sonuc/esik_nihai_{X}.txt'):
        w = line.split()
        if w[:2] == ['SQ', '0']: d['SQ'] = float(w[2])
        if w[:2] == ['CQ', '0']: d['CQ'] = int(w[2])
    return d

def books(X):
    """(kind, seg, sira) -> dict(a, C[4], and for T: pt, lo, hi)"""
    out, dup = {}, 0
    for kind, st in (('R', RF), ('T', TF)):
        for fn in sorted(glob.glob(f'{S}/tab/{kind}_{X}/b*.aday')):
            seg = fn.split('/')[-1][:4]; b = open(fn, 'rb').read()
            if len(b) % st.size: print('  UYARI eksik kayıt', fn, len(b) % st.size)
            for o in range(0, len(b) - len(b) % st.size, st.size):
                r = st.unpack_from(b, o); key = (kind, seg, r[-1])
                if key in out: dup += 1; continue
                d = dict(a=list(r[:114]))
                if kind == 'T': d.update(pt=r[114], lo=r[122], hi=r[130], C=list(r[154:158]))
                else: d.update(C=list(r[114:118]))
                out[key] = d
    return out, dup

def coz(X):
    """dedicated re-scoring: list of (sira, before point, after point, after lo, after hi, verdict)"""
    fn = f'{S}/sonuc/belirsiz/{X}_coz.txt'; res = []
    try: lines = open(fn)
    except FileNotFoundError: return res
    num = r'([\d.]+|∞)'
    for line in lines:
        m = re.search(rf' (\d+): önce nokta {num} \[{num}, {num}\] → sonra nokta {num} \[{num}, {num}\] → (\S+)', line)
        if m:
            f = lambda s: math.inf if s == '∞' else float(s)
            res.append((int(m[1]), f(m[2]), f(m[5]), f(m[6]), f(m[7]), m[8]))
    return res

X = sys.argv[1]; NSAMPLE = int(sys.argv[2]) if len(sys.argv) > 2 else 200
e = esik(X); SQ, CQ = e['SQ'], e['CQ']; tol = 1e-6
bk, dup = books(X)
T = {k: v for k, v in bk.items() if k[0] == 'T'}
print(f'== {X}: S_Q {SQ}, C_Q {CQ}; aday kitap R {len(bk) - len(T)}, T {len(T)}; yinelenen kayıt {dup}')
k = lambda key: sum(1 for v in T.values() if v[key] >= SQ - tol)
print(f'  bilgi (düzeltme öncesi) k_alt/k_nokta/k_üst = {k("lo")} / {k("pt")} / {k("hi")}')
cz = coz(X)
if cz:
    for sira, before, pt, lo, hi, verdict in cz:
        hits = [key for key, v in T.items() if key[2] == sira and abs(v['pt'] - before) < 0.006]
        assert len(hits) == 1, (sira, before, hits)
        T[hits[0]].update(pt=pt, lo=lo, hi=hi)
    print(f'  bilgi (adanmış düzeltmeyle, {len(cz)} kitap) k_alt/k_nokta/k_üst = {k("lo")} / {k("pt")} / {k("hi")}')
hiC = {key: v for key, v in bk.items() if v['C'][0] >= CQ}
print(f'  sayım: C[G] ≥ {CQ}: {len(hiC)} kitap (R {sum(1 for x in hiC if x[0] == "R")}, T {sum(1 for x in hiC if x[0] == "T")})')
# independent re-score of every C >= CQ book
mism, s0hits = 0, []
for key, v in sorted(hiC.items()):
    sc = B['all_scores'](v['a'])
    mine = [sc[g][0] for g in GATES]
    if mine != v['C']: mism += 1; print('   UYUŞMAZLIK', key, 'oturum', v['C'], 'benim', mine)
    if sc['S0'][0] >= CQ: s0hits.append((key, sc))
print(f'  C ≥ C_Q kitaplarında 4 kapının hepsinde uyuşmazlık: {mism}; benim S0 ≥ {CQ}: {len(s0hits)}')
for key, sc in s0hits:
    c, L = sc['S0']; al, be, p = B['lens'](L)
    P, info = B['profile'](bk[key]['a'], al, be, p)
    print(f'   S0 {c} {key} ({al},{be:+d},{p}) kF={info["kF"]} kH={info["kH"]} |U|={info["U"]} hücre={info["cnt"]} '
          f'toplam={info["s"]} Σq={info["Sq"]} q={min(info["q"])}..{max(info["q"])} Qc={P["Qc"]} B2={P["B2"]} B1={P["B1"]} K1={P["K1"]} Qa={P["Qa"]}')
# random sample of the remaining candidates
random.seed(20261027)
rest = [key for key in bk if key not in hiC]
samp = random.sample(rest, min(NSAMPLE, len(rest))); mism = 0
for key in samp:
    sc = B['all_scores'](bk[key]['a']); mine = [sc[g][0] for g in GATES]
    if mine != bk[key]['C']: mism += 1; print('   örneklem UYUŞMAZLIK', key, 'oturum', bk[key]['C'], 'benim', mine)
print(f'  rastgele {len(samp)} aday (C < C_Q): 4 kapıda uyuşmazlık {mism}')
