# r2_dogrula — ONKAYIT §6.2 doğrulaması: R2'nin P(S ≥ s) eğrisi ↔ sınama örnekleminin doğrudan sıklığı (nokta puanı histogramları).
# Doğrulama bölgesi: doğrudan isabet ≥ 30 olan eşikler; en yüksek dördünde oran R2/doğrudan ∈ [0,5; 2,0] ise "doğrulanmış".
# Sayım puanı için aynı karşılaştırma referans+sınama sayım histogramlarıyla. Kullanım: python3 -I src/r2_dogrula.py <sıfırkodeks> [S_Q] [C_Q]
import os, sys, glob, numpy as np
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ad = sys.argv[1]; SQ = float(sys.argv[2]) if len(sys.argv) > 2 else None; CQ = int(sys.argv[3]) if len(sys.argv) > 3 else 11
NV, HB, HMIN, HW = 8, 1200, -20.0, 0.1
H = np.zeros(HB); N = 0
for f in sorted(glob.glob(os.path.join(KOK, 'tab', f'T_{ad}', '*.hist'))):
    d = np.fromfile(f, dtype=np.int64); H += d[:HB]                     # v = 0 nokta puanı histogramı
    N += int(open(f[:-5] + '.say').read().split()[7])
r2 = {}; r2c = {}
for line in open(os.path.join(KOK, 'sonuc', f'kuyruk_{ad}.txt')):
    t = line.split()
    if t[0] == 'bilgi': r2[float(t[1])] = float(t[2])
    elif t[0] == 'sayim': r2c[int(t[1])] = float(t[2])
kum = np.cumsum(H[::-1])[::-1]                                          # kum[b] = #(S ≥ alt kenar b)
satir = []
for s in sorted(r2):
    b = int(round((s - HMIN) / HW))
    if 0 <= b < HB:
        k = kum[b]; satir.append((s, int(k), k / N if N else 0, r2[s]))
gecerli = [x for x in satir if x[1] >= 30]
print(f'{ad}: sınama N = {N:,}. Bilgi (v=0, nokta): doğrudan isabet ≥ 30 olan en yüksek eşikler:')
son4 = gecerli[-4:]
for s, k, pd, pr in son4: print(f'   s = {s:5.2f}: doğrudan {k} ({pd:.3g})  R2 {pr:.3g}  oran {pr / pd:.2f}')
ok = len(son4) == 4 and all(0.5 <= pr / pd <= 2.0 for s, k, pd, pr in son4)
oran = [pr / pd for s, k, pd, pr in son4]
print(f'   R2 doğrulanmış: {"EVET" if ok else "HAYIR"}' + (f'; bant oranları [{min(oran):.2f}, {max(oran):.2f}]' if son4 else ''))
if SQ is not None:
    s_alt = max(s for s in r2 if s <= SQ); s_ust = min(s for s in r2 if s >= SQ)
    v = r2[s_alt] + (r2[s_ust] - r2[s_alt]) * ((SQ - s_alt) / (s_ust - s_alt) if s_ust > s_alt else 0)
    print(f'   R2 @ S_Q = {SQ:.3f}: ham {v:.3g}' + (f'; doğrudana kalibre (R2 / oran, dört oranın aralığı): {v / max(oran):.3g} – {v / min(oran):.3g}' if son4 else ''))
# sayım: referans + sınama
Hc = np.zeros(21); Nc = 0
for f in sorted(glob.glob(os.path.join(KOK, 'tab', f'R_{ad}', '*.say'))):
    L = open(f).read().split('\n'); Nc += int(L[0].split()[7]); Hc += np.array([int(x) for x in L[1].split()[4:]])
for f in sorted(glob.glob(os.path.join(KOK, 'tab', f'T_{ad}', '*.hist'))):
    d = np.fromfile(f, dtype=np.int64); Hc += d[2 * NV * HB:2 * NV * HB + 21]; Nc += int(open(f[:-5] + '.say').read().split()[7])
kc = np.cumsum(Hc[::-1])[::-1]
print(f'Sayım (kapı G): N = {Nc:,}; c, doğrudan ≥c, R2')
for c in range(6, 14):
    print(f'   c = {c}: doğrudan {int(kc[c])} ({kc[c] / Nc:.3g})  R2 {r2c.get(c, 0):.3g}' + (f'  oran {r2c[c] / (kc[c] / Nc):.2f}' if kc[c] > 0 else ''))
