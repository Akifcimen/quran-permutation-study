#!/usr/bin/env python3
"""Ö0(a) özeti: sonuc/t9/<kodeks>_<sıfır>_<r>_<kapali|acik>.txt → tekrar ortalamaları, incelemeyle karşılaştırma (ONKAYIT §3.1)."""
import glob, re, statistics as st
INC = {'U': (116, 53), 'J10': (1135, 573), 'M20': (827, 393)}
rows = {}
for f in sorted(glob.glob('sonuc/t9/*_*.txt')):
    m = re.match(r'sonuc/t9/(\d+)_(\w+?)_(\d)_(kapali|acik)\.txt$', f)
    if not m: continue
    kod, nul, r, b = m.groups(); s = open(f).read()
    g = lambda k: float(re.search(k + r'[ =]+([-\d.e+]+)', s).group(1))
    rows.setdefault((kod, b, nul), []).append(dict(r=int(r), k0=int(g('k_ozgun')), k1=int(g('k_fix9')), k2=int(g('k_tekrarsiz')), N=int(g(' N')),
        q0=g('Kuran T2'), q2=g('T2\\*'), fark=g('robot farkı maks'), nf=int(re.search(r'>1e-3: (\d+)', s).group(1)), ndup=int(g('T2\\*<T2 kitap'))))
print("kodeks | batarya | sıfır | tekrar: k_özgün / k_tekdüzeltme / k_tekrarsız (N=2e6) | ortalama | inceleme (129, terazi kapalı) | ölçüt")
for (kod, b, nul), L in sorted(rows.items()):
    L.sort(key=lambda x: x['r'])
    k0 = st.mean(x['k0'] for x in L); k1 = st.mean(x['k1'] for x in L); k2 = st.mean(x['k2'] for x in L)
    s = ' ; '.join(f"r{x['r']}: {x['k0']}/{x['k1']}/{x['k2']}" for x in L)
    inc = INC.get(nul); oran = k0 / k1 if k1 else float('nan'); ok = ''
    if kod == '129' and b == 'kapali' and inc:
        ok = 'YENİDEN ÜRETİLDİ' if (abs(k0 - inc[0]) <= 0.3 * inc[0] and 1.6 <= oran <= 2.6) else 'yeniden üretilemedi'
    rob = max(x['fark'] for x in L); nf = sum(x['nf'] for x in L)
    print(f"{kod} | {b} | {nul} | {s} | {k0:.0f}/{k1:.0f}/{k2:.0f} (özgün/düzeltme oranı {oran:.2f}) | {inc if inc else ''} | {ok} | Kur'an T2={L[0]['q0']:.3f} T2*={L[0]['q2']:.3f} | robot farkı maks {rob:.1e} (>1e-3: {nf})")
