#!/usr/bin/env python3
"""Ö1 sonuç tablosu: sonuc/o1/<etiket>.ozet.txt dosyalarından (ve havuz .say dosyalarından) tek tablo.
Kullanım: python3 -I src/o1_ozet.py <etiket>=<havuz_dizini> ..."""
import sys, re, glob
def say_topla(d):
    T = H = B = C = G = 0; sure = 0.0; nb = 0
    for f in glob.glob(f'{d}/*.say'):
        s = open(f).read(); nb += 1
        T += int(re.search(r'deneme (\d+)', s).group(1)); H += int(re.search(r'isabet (\d+)', s).group(1))
        B += int(re.search(r' B (\d+)', s).group(1)); C += int(re.search(r' C (\d+)', s).group(1)); G += int(re.search(r' G (\d+)', s.split(' kosul_gecen')[0]).group(1))
        sure += float(re.search(r'sure_s ([\d.]+)', s).group(1))
    return dict(T=T, H=H, B=B, C=C, G=G, sure=sure, nb=nb)
print("| havuz | N | Kur'an T2* | k (T2*) | p = (k+1)/(N+1) | k/N %95 | Kur'an T2 | k (T2) | p (T2) | tekrarı ayıklanan | deneme | P(koşul|sıfır) | G-kitap | çekirdek-saat |")
print("|---|---|---|---|---|---|---|---|---|---|---|---|---|---|")
for arg in sys.argv[1:]:
    tag, d = arg.split('=')
    try: s = open(f'sonuc/o1/{tag}.ozet.txt').read()
    except FileNotFoundError: print(f"| {tag} | (puanlanmadı) |"); continue
    N = int(re.search(r'N = (\d+)', s).group(1))
    q2r, q2t = map(float, re.search(r'robot T2 = ([\d.]+), tekrarsız T2\* = ([\d.]+)', s).groups())
    kt, pt, lo, hi = re.search(r'T2\* \(birincil\): k = (\d+) → p = \(k\+1\)/\(N\+1\) = ([\de.+-]+) ; k/N %95 Clopper–Pearson \[([\de.+-]+), ([\de.+-]+)\]', s).groups()
    kr, pr = re.search(r'robot T2 \(karşılaştırma\): k = (\d+) → p = ([\de.+-]+)', s).groups()
    nd = re.search(r'tekrarı ayıklanan\) kitap: (\d+) \(([\d.]+)%\)', s).groups()
    sy = say_topla(d) if d and d != '-' else None
    pk = f"{sy['H']/sy['T']:.3e}" if sy and sy['T'] else ''
    print(f"| {tag} | {N:,} | {q2t:.3f} | {kt} | {float(pt):.3e} | [{float(lo):.2e}, {float(hi):.2e}] | {q2r:.3f} | {kr} | {float(pr):.3e} | {nd[0]} (%{nd[1]}) | {sy['T'] if sy else ''} | {pk} | {sy['G'] if sy else ''} | {sy['sure']/3600:.2f} |" if sy else
          f"| {tag} | {N:,} | {q2t:.3f} | {kt} | {float(pt):.3e} | [{float(lo):.2e}, {float(hi):.2e}] | {q2r:.3f} | {kr} | {float(pr):.3e} | {nd[0]} (%{nd[1]}) | | | | |")
