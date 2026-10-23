#!/usr/bin/env python3
"""O-koşullu havuzlarda olay düzeyinde koşullu frekanslar (sabit olay; arama bedeli yok; kalibre sıra DEĞİL):
P(D|O) (dört hücre 3/3/3/3 + çift hücreler 18/20), P(E|O) (kefe torbaları), P(G|O) (tam kristal), P(W|O) (İki El), P(A|O) (terazi),
P(G∧W|O), P(D∧W|O). Kayıtlardaki olay bayraklarından (kesif4 tanımları) sayılır.
Kullanım: python3 -I src/olay_frekans.py <havuz_dizini>..."""
import sys, glob, struct, math
EV = dict(A=1, B=2, C=4, D=8, E=16, G=32, W=64, O3=128)
def cp(k, n):
    # Wilson yaklaşık %95 (k küçükken uyarı)
    if n == 0: return (0, 0)
    p = k / n; z = 1.96; d = 1 + z*z/n; c = (p + z*z/(2*n)) / d; h = z*math.sqrt(p*(1-p)/n + z*z/(4*n*n)) / d
    return max(0, c - h), c + h
for d in sys.argv[1:]:
    n = 0; cnt = dict(A=0, D=0, E=0, G=0, W=0, GW=0, DW=0, AW=0, EW=0)
    for f in sorted(glob.glob(f'{d}/*.bin')):
        b = open(f, 'rb').read()
        for o in range(0, len(b), 244):
            fl = struct.unpack_from('<I', b, o + 228)[0]; n += 1
            for k in ('A', 'D', 'E', 'G', 'W'):
                if fl & EV[k]: cnt[k] += 1
            if fl & EV['G'] and fl & EV['W']: cnt['GW'] += 1
            if fl & EV['D'] and fl & EV['W']: cnt['DW'] += 1
            if fl & EV['A'] and fl & EV['W']: cnt['AW'] += 1
            if fl & EV['E'] and fl & EV['W']: cnt['EW'] += 1
    print(f"{d}: N = {n:,}")
    for k, v in cnt.items():
        lo, hi = cp(v, n); print(f"   P({k}|koşul) = {v}/{n} = {v/n:.4e}  [%95 ≈ {lo:.2e}, {hi:.2e}]")
