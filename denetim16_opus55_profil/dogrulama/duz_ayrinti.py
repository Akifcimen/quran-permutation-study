# duz_ayrinti.py — ONKAYIT_EK §5 betimsel kısmı: her kuralda kestirilen plasebo olayları (sıklık, aralık), kestirilemeyenler ve Kur'an'a en yakın
# beş plasebonun bit farkı (log₂(p_plasebo / p_Kur'an); eksi = Kur'an'dan nadir). Doğrudan sayımla çözülenler de en yakınlar listesine girer.
# Kullanım: python3 -I duz_ayrinti.py <tab/S> <S> <duz_çıktısı>...
import sys, os, math
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import duz_oku as d
TD, S, F = sys.argv[1], sys.argv[2], sys.argv[3:]
k = d.sayimlar(TD); o = d.olaylar(TD); r = d.duz_oku(F)
for t in (1, 2, 3):
    Q = f'A000.{t}'; bQ = d.birlestir(r.get(Q, {})) or (d.birlestir(r.get('A000.2', {})) if t == 3 else None); mQ, loQ, hiQ = bQ
    sat, kes_yok, n_t = [], [], 0
    for a in o:
        if not a.endswith(f'.{t}') or a.startswith('A000'): continue
        n_t += 1; kk, n = d.derin(k.get(a, [0, 0, 0, 0]), 1)
        if kk >= 10: sat.append((kk / n, None, None, a, f'doğrudan k={kk}/{n:.0e}')); continue
        lo_d, hi_d = d.cp(kk, n)
        if lo_d > hiQ: sat.append((kk / n, lo_d, hi_d, a, f'doğrudan k={kk}/{n:.0e} (alt sınır > Kur\'an üst ucu)')); continue
        b = d.birlestir(r.get(a, {}))
        if b is None:
            reps = r.get(a, {}); kes_yok.append((a, kk, ' '.join(f"r{x}:{'iyi' if reps[x]['iyi'] else 'kötü'}" for x in sorted(reps)))); continue
        sat.append((b[0], b[1], b[2], a, f'kestirim (10¹⁰ sayımı k={kk})'))
    sat.sort()
    print(f'{S} O{t}: plasebo olayı {n_t}; Kur\'an {mQ:.2e} [{loQ:.2e}, {hiQ:.2e}]')
    print('  Kur\'an\'a en yakın beş (ve daha nadirler):')
    for p, lo, hi, a, ne in sat[:max(5, sum(1 for x in sat if x[0] <= mQ) + 5)][:12]:
        ar = f'[{lo:.2e}, {hi:.2e}]' if lo is not None else ''
        print(f'    {a:10s} {p:.2e} {ar:24s} {math.log2(p / mQ):+6.2f} bit  {ne}  bilgi {o[a]["bilgi"]:.1f}')
    print(f'  kestirilemeyen {len(kes_yok)}:')
    for a, kk, ss in kes_yok: print(f'    {a:10s} 10¹⁰ sayımı k={kk}; bilgi {o[a]["bilgi"]:.1f}; {ss}')
