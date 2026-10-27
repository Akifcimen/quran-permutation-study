# ayrinti_yaz.py — src/ayrinti TSV'sini okunur biçimde yazar: her kitap ve S_Q'ya ulaşabilecek mercekleri; c < 10 aramalar '*'.
# Kullanım: python3 -I src/ayrinti_yaz.py <tsv> <S_Q>
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import dil
SQ = float(sys.argv[2])
for line in open(sys.argv[1]):
    t = line.rstrip('\n').split('\t')
    if t[0] == 'KITAP': print(f"KİTAP {os.path.basename(t[1])} sıra {t[2]}: nokta {t[3]}, alt {t[4]}, üst {t[5]}, sayım {t[6]}"); continue
    L, f, p, kF, ctx, n = map(int, t[1:7]); sp, sl, sh = map(float, t[7:10])
    cB, cS, fm, cF, wm, cW = int(t[10]), int(t[11]), int(t[12]), int(t[13]), int(t[14]), int(t[15]); m = int(t[16], 16)
    if sh < SQ: continue
    yz = lambda c: '*' if c < 10 else ''
    print(f"   f={f} ({dil.AL[f]},{dil.BE[f]:+d},{p}) kF={kF} n={n}: nokta {sp:.2f} alt {sl:.2f} üst {'∞' if sh > 1e8 else round(sh, 2)}"
          f" | B={m & 3} c={cB}{yz(cB)} | boy(QkF={(m >> 13) & 1}) c={cS}{yz(cS)} | F={fm} c={cF}{yz(cF)} | W={wm} c={cW}{yz(cW)} | kapı {t[17]}")
