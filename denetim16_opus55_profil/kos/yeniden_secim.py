# yeniden_secim.py — ONKAYIT_EK §2 kalite şartı: iyi olmayan (olay, r) tekrarlarını bulur; r+4 tohumuyla, iki kat bütçeyle yeniden koşulacak
# listeleri r'ye göre yazar (yen_<S>_r<r>.txt). Zaten yeniden koşulmuş (r+4 çıktısı olan) tekrarlar atlanır. Kalite şartı duz_oku'daki gibidir.
# Kullanım: python3 -I yeniden_secim.py <S> <çıktı_dizini> <duz_çıktısı>...
import sys, os
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'dogrulama'))
import duz_oku as d
S, O, F = sys.argv[1], sys.argv[2], sys.argv[3:]
r = d.duz_oku(F); g = {1: [], 2: [], 3: [], 4: []}
for a in sorted(r):
    for k in (1, 2, 3, 4):
        x = r[a].get(k)
        if x is None: print(f'uyarı: {a} r{k} yok'); continue
        if not x['iyi'] and (k + 4) not in r[a]: g[k].append(a)
for k in g:
    open(os.path.join(O, f'yen_{S}_r{k}.txt'), 'w').write(''.join(a + '\n' for a in g[k]))
print(S, 'olay', len(r), '; yeniden koşulacak tekrar:', {k: len(v) for k, v in g.items()}, 'toplam', sum(len(v) for v in g.values()))
