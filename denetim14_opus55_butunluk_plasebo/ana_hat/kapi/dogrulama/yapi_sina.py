# Kur'an ve 19'daki kopyalar için yapı olgularını tanımlardan yeniden say (lens t = n + a, p = 19)
import json, struct, sys
d = json.load(open(sys.argv[1]))['data']
sur = d['surahs']['references'] if 'surahs' in d else d
a_q = [s['numberOfAyahs'] for s in sur]
def olgular(a, al=1, be=1, p=19):
    N = range(1, 115); t = {n: al*n + be*a[n-1] for n in N}; u = {n: al*n - be*a[n-1] for n in N}
    E0 = [n for n in N if t[n] % 2 == 0]; E1 = [n for n in N if t[n] % 2]
    F = [n for n in N if t[n] % p == 0]; H = [n for n in N if u[n] % p == 0]
    F0 = [n for n in F if t[n] % 2 == 0]; F1 = [n for n in F if t[n] % 2]
    hucre = {}
    for n in F: hucre.setdefault((t[n] % 2, n % 2), []).append(t[n] // p)
    q = sorted(t[n] // p for n in F); U = set(F) | set(H)
    return dict(B1=f'{len(E0)}/{len(E1)}', sn0=sum(E0), sa1=sum(a[n-1] for n in E1), tE0=sum(t[n] for n in E0), tE1=sum(t[n] for n in E1),
                kF=len(F), kH=len(H), ayrik=not (set(F) & set(H)), aileT=sum(t[n] for n in F),
                yarilar=f'{sum(t[n] for n in F0)}/{sum(t[n] for n in F1)}',
                hucreSay=sorted(len(v) for v in hucre.values()), hucreQ=sorted(sum(v) for v in hucre.values()),
                q=q, U=len(U), Uayet=sum(a[n-1] for n in U), Uayet_mod=sum(a[n-1] for n in U) % p)
print('KURAN', olgular(a_q))
K = struct.Struct('<IfhBB114H'); FORMS = [(1,1),(1,-1),(1,2),(1,-2),(1,3),(1,-3),(2,1),(2,-1),(2,3),(2,-3),(3,1),(3,-1),(3,2),(3,-2)]
PR = [3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113]
for f in sys.argv[2:]:
    data = open(f, 'rb').read()
    for o in range(0, len(data), K.size):
        r = K.unpack_from(data, o); L = r[2]; fo, i = divmod(L, 29)
        if PR[i] != 19: continue
        a = list(r[5:]); ortak = sum(x == y for x, y in zip(a, a_q))
        print(f.split('/')[-2], 'sira', r[0], 'mercek', FORMS[fo], 'ortak konum', ortak, olgular(a, *FORMS[fo]))
