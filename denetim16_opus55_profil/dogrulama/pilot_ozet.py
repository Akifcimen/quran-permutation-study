# pilot_ozet.py <pilot_kök> — geliştirme denemesinin özeti (düzey 1–2): olayların çözülme dağılımı, Kur'an'ın sayımı, model sırası.
import sys, os, glob, math
P = sys.argv[1]
for S in 'MLU':
    T = os.path.join(P, 'tab', S); olay = {}
    for s in open(os.path.join(T, 'olaylar.txt')):
        w = s.split(); olay[w[0]] = (int(w[1]), float(w[8]), float(w[9]), int(w[10]))
    k = {}; n = {1: 0, 2: 0}
    for j, (b1, b2) in ((1, (1, 4)), (2, (5, 40))):
        for b in range(b1, b2 + 1):
            fn = os.path.join(T, f'd{j}', f'b{b:04d}.txt')
            if not os.path.exists(fn): print(S, 'eksik', fn); break
            for s in open(fn):
                w = s.split()
                if s.startswith('#'): n[j] += int(w[w.index('N') + 1])
                elif w and w[0] == 'D': k.setdefault(w[1], {1: 0, 2: 0})[j] += int(w[2])
    print(f'{S}: olay {len(olay)}; N düzey1 {n[1]:.0e}, düzey1+2 {n[1] + n[2]:.0e}')
    for t in (1, 2, 3):
        ad = [a for a in olay if a.endswith(f'.{t}') and olay[a][0] != 0]; Q = f'A000.{t}'
        k1 = lambda a: k.get(a, {1: 0})[1]; k12 = lambda a: k.get(a, {1: 0, 2: 0})[1] + k.get(a, {1: 0, 2: 0})[2]
        r1 = sum(1 for a in ad if k1(a) >= 10); kal = [a for a in ad if k1(a) < 10]; r2 = sum(1 for a in kal if k12(a) >= 10); kal2 = [a for a in kal if k12(a) < 10]
        sifir = sum(1 for a in kal2 if k12(a) == 0)
        SQ = olay[Q][1] if Q in olay else float('nan'); RS = sum(1 for a in ad if olay[a][1] >= SQ)
        cz = [(olay[a][2], -math.log2(k1(a) / n[1])) for a in ad if k1(a) >= 10]
        m = len(cz); rho = float('nan')
        if m > 20:
            def sira(v):
                o = sorted(range(len(v)), key=lambda i: v[i]); r = [0] * len(v)
                for i, x in enumerate(o): r[x] = i
                return r
            ra, rb = sira([x for x, _ in cz]), sira([y for _, y in cz]); rho = 1 - 6 * sum((x - y) ** 2 for x, y in zip(ra, rb)) / (m * (m * m - 1))
            fark = sorted(y - x for x, y in cz)
        print(f'  tür {t}: olaylı plasebo {len(ad)}; 10⁶\'da çözülen {r1}, 10⁷\'de {r2}, 10⁷ sonrası kalan {len(kal2)} (bunların {sifir}\'ı 0 isabet); '
              f'Kur\'an {Q} 10⁷\'de {k12(Q) if Q in olay else "-"} isabet, {olay[Q][3] if Q in olay else "-"} yüklem, S {SQ:.2f}; model R_S {RS}; '
              f'çözülmüş {m} olayda ρ {rho:.3f}' + (f', (−log₂p̂ − bilgi) medyan {fark[m // 2]:+.2f} [%10 {fark[m // 10]:+.2f}, %90 {fark[9 * m // 10]:+.2f}]' if m > 20 else ''))
