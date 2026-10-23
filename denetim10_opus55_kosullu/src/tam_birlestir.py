#!/usr/bin/env python3
"""tam_ortak parçalarını (PARCA=i/n) birleştirir: düzey-2 toplamları (h2, h2kare, h2isabet) parçalar üzerinden toplanır, P_tam yeniden.
Kullanım: python3 -I src/tam_birlestir.py <prefix> <n>  → <prefix>.tam.txt"""
import sys, math
pre, n = sys.argv[1], int(sys.argv[2])
parts = [open(f'{pre}.tam.parca{i}.txt').read().splitlines() for i in range(n)]
head = [l for l in parts[0] if l.startswith('#')]
rows = [[l for l in p if not l.startswith('#')] for p in parts]
N = int(head[0].split('N=')[1].split()[0])
out = head[:]
for k in range(len(rows[0])):
    L = [r[k].split('|')[0].split() for r in rows]; hik = rows[0][k].split('|')[1].strip()
    a = L[0]; assert all(x[0] == a[0] for x in L)
    h2 = sum(float(x[7]) for x in L); h2q = sum(float(x[8]) for x in L); h2n = sum(int(x[9]) for x in L)
    sahip, duzey, h1, nF, m = int(a[0]), int(a[3]), int(a[4]), int(a[5]), int(a[6])
    Ns = N - 1 if sahip >= 0 else N
    P = h1 / Ns if duzey == 1 else ((nF / Ns) * (h2 / m) if m > 0 else 0.0)
    S = -math.log10(P) if P > 0 else 99.0
    out.append(f"{a[0]} {a[1]} {a[2]} {duzey} {h1} {nF} {m} {h2:.6f} {h2q:.6f} {h2n} {P:.4e} {S:.4f} | {hik}")
open(f'{pre}.tam.txt', 'w').write('\n'.join(out) + '\n'); print(f'birleştirildi: {len(rows[0])} hikâye → {pre}.tam.txt')
