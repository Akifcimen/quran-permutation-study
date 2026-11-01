# kitap_incele.py <hedef.bin> <diller.txt> <ayrinti_A.txt> <ad>... — bir hedef kitabın kendi merceğindeki yapısını somut sayılarla döker.
import struct, sys
hedef, dill, ayr = sys.argv[1:4]; adlar = sys.argv[4:]
FORMS = [(1, 1), (1, -1), (1, 2), (1, -2), (1, 3), (1, -3), (2, 1), (2, -1), (2, 3), (2, -3), (3, 1), (3, -1), (3, 2), (3, -2)]
PR = [3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113]
data = open(hedef, 'rb').read(); rec = lambda i: list(struct.unpack_from('<114h', data, i * 232 + 4))
satir = {l.split()[0]: l.split() for l in open(dill)}
ay = {l.split()[0]: l for l in open(ayr)}
Q = rec(0)
for ad in adlar:
    w = satir[ad]; h = int(w[1]); L = int(w[3]); f, i = divmod(L, 29); al, be = FORMS[f]; p = PR[i]; a = rec(h)
    t = {n: al * n + be * a[n - 1] for n in range(1, 115)}; u = {n: al * n - be * a[n - 1] for n in range(1, 115)}
    F = [n for n in t if t[n] % p == 0]; H = [n for n in u if u[n] % p == 0]
    hc = lambda n: ('t çift' if t[n] % 2 == 0 else 't tek') + '/' + ('n çift' if n % 2 == 0 else 'n tek')
    print(f'===== {ad} (hedef {h}) mercek t = {al}·n {"+" if be > 0 else "−"} {abs(be)}·a, p = {p}')
    x = ay[ad].split(); print('  ' + ' '.join(x[x.index('amac'):x.index('|Λ|')]) + ' | dil: ' + ay[ad].split(':')[-1].strip())
    yer = sum(1 for n in range(114) if a[n] == Q[n]); print(f'  Kur\'an diziliminden farkı: 114 sûreden {yer} tanesi aynı yerde')
    E0 = [n for n in t if t[n] % 2 == 0]; E1 = [n for n in t if t[n] % 2]
    print(f'  kitap: t-çift {len(E0)} sûre (Σn {sum(E0)}, Σa {sum(a[n-1] for n in E0)}, Σt {sum(t[n] for n in E0)}), t-tek {len(E1)} (Σn {sum(E1)}, Σa {sum(a[n-1] for n in E1)}, Σt {sum(t[n] for n in E1)})')
    print(f'  aile F (p | t): {len(F)} sûre; Σa {sum(a[n-1] for n in F)}, Σn {sum(F)}, Σq {sum(t[n]//p for n in F)}')
    for c in ('t çift/n çift', 't çift/n tek', 't tek/n çift', 't tek/n tek'):
        m = [n for n in F if hc(n) == c]; print(f'    hücre {c}: {len(m)} sûre, q = {[t[n]//p for n in m]}, Σq = {sum(t[n]//p for n in m)}  (sûreler {m})')
    print(f'  karşı aile H (p | u): {len(H)} sûre; Σa {sum(a[n-1] for n in H)}, Σn {sum(H)}, r = {sorted(u[n]//p for n in H)}, Σr {sum(u[n]//p for n in H)}')
    for c in ('t çift/n çift', 't çift/n tek', 't tek/n çift', 't tek/n tek'):
        m = [n for n in H if hc(n) == c]; print(f'    H hücre {c}: {len(m)}, Σr = {sum(u[n]//p for n in m)}')
    I = sorted(set(F) & set(H)); U = sorted(set(F) | set(H))
    print(f'  iki el: |F∩H| = {len(I)} {I}; birleşim {len(U)} sûre, Σa {sum(a[n-1] for n in U)}, Σn {sum(U)}, Σt {sum(t[n] for n in U)}')
