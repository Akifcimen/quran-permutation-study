# belirsiz_coz.py — belirsiz kitapların adanmış kestirimlerle yeniden puanlanması (ONKAYIT §4.3).
# Girdi: ayrinti TSV'si (src/ayrinti çıktısı; KITAP + S_Q'ya ulaşabilecek MERCEK satırları), adanmış sonuç dosyaları (sonuc/adanmis/*.txt),
# S_Q. Bir arama için adanmış kestirim varsa (aynı grup, biçim, p, kitabın k_F'si, aynı maske) o kullanılır; yoksa referans tablosu
# sayısı (c ≥ 10 nokta; c < 10 Poisson–CP sınırları). Karar: mercek puanlarının en büyüğü için alt ≥ S_Q → "ÜSTTE";
# üst < S_Q → "ALTTA"; aksi halde "BELİRSİZ". Kullanım: python3 -I src/belirsiz_coz.py <tsv> <S_Q> <adanmış dosyaları...>
import sys, re, math, os
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)))); import dil
CP_UST = [2.995732, 4.743865, 6.295794, 7.753657, 9.153519, 10.513035, 11.842396, 13.148114, 14.434650, 15.705216]
CP_ALT = [0.0, 0.051293, 0.355362, 0.817691, 1.366318, 1.970150, 2.613015, 3.285316, 3.980823, 4.695228]
def yuzey(c, n):
    if n == 0: return (0.0, 0.0, math.inf)
    if c >= 10: v = math.log2(n) - math.log2(c); return (v, v, v)
    lo = max(0.0, math.log2(n) - math.log2(CP_UST[c])); hi = math.inf if c == 0 else math.log2(n) - math.log2(CP_ALT[c])
    return (math.log2(n + 1) - math.log2(c + 0.5), lo, hi)
def yuzey_adanmis(h, n):   # isabet h / n; nokta h/n, %95 iki yanlı Poisson yaklaşık sınırlar
    if h >= 10:
        pt = math.log2(n) - math.log2(h); se = 1.96 / math.sqrt(h) / math.log(2)
        return (pt, pt - se, pt + se)
    return yuzey(h, n)
# adanmış kestirimleri oku
AD = {}
for fn in sys.argv[3:]:
    L = open(fn).read().split('\n'); bas = L[0]
    m = re.search(r'bicim \((\d),([+-]\d)\)', bas); al, be = int(m.group(1)), int(m.group(2)); f = [i for i in range(14) if dil.AL[i] == al and dil.BE[i] == be][0]
    if 'grup B' in bas:
        for l in L[1:]:
            mm = re.search(r'maske (\d+): isabet (\d+) / (\d+)', l)
            if mm:
                fe = [i for i in range(14) if dil.AL[i] == al and dil.BE[i] == -be][0]   # B olayı (α, β) ve (α, −β) için aynıdır
                for ff in (f, fe): AD[('B', ff, 0, 0, int(mm.group(1)))] = (int(mm.group(2)), int(mm.group(3)))
        continue
    p = int(re.search(r' p (\d+)', bas).group(1)); grp = re.search(r'grup (\w)', bas).group(1)
    if bas.startswith('adanmis_u'):
        K = int(re.search(r' K (\d+)', bas).group(1)); ess = float(re.search(r'ESS (\d+)', bas).group(1))
        for l in L[1:]:
            mm = re.search(r'maske (\d+): isabet (\d+), P\^ = ([0-9.e+-]+)', l)
            if mm: ph = float(mm.group(3)); AD[(grp, f, p, K, int(mm.group(1)))] = (ph * ess, ess)
    else:
        for l in L[1:]:
            mm = re.search(r'K=(\d+) maske (\d+): isabet (\d+) / (\d+)', l)
            if mm: AD[(grp, f, p, int(mm.group(1)), int(mm.group(2)))] = (int(mm.group(3)), int(mm.group(4)))
SQ = float(sys.argv[2]); N_R = 100_000_000
def bedel(f, p): return dil.bicim_bedeli(f) + dil.asal_bedeli(p)
kitaplar = []; cur = None
for line in open(sys.argv[1]):
    t = line.rstrip('\n').split('\t')
    if t[0] == 'KITAP': cur = dict(ad=t[1], sira=t[2], pt=float(t[3]), lo=float(t[4]), hi=float(t[5]), m=[]); kitaplar.append(cur)
    else:
        L_, f, p, kF, ctx, n = map(int, t[1:7]); cB, cS, fm, cF, wm, cW = int(t[10]), int(t[11]), int(t[12]), int(t[13]), int(t[14]), int(t[15]); m = int(t[16], 16)
        cur['m'].append(dict(L=L_, f=f, p=p, kF=kF, n=n, bm=m & 3, cB=cB, cS=cS, fm=fm, cF=cF, wm=wm, cW=cW, kq=(m >> 13) & 1))
for k in kitaplar:
    sonuc = []
    for m in k['m']:
        f, p, kF = m['f'], m['p'], m['kF']; parca = []; kullan = []
        for grp, mask, c, n, key in (('B', m['bm'], m['cB'], N_R, ('B', f, 0, 0, m['bm'])), ('F', m['fm'], m['cF'], m['n'], ('F', f, p, kF, m['fm'])),
                                     ('W', m['wm'], m['cW'], m['n'], ('W', f, p, kF, m['wm']))):
            if mask == 0: parca.append((0, 0, 0)); continue
            if key in AD: h, nn = AD[key]; parca.append(yuzey_adanmis(h, nn)); kullan.append(f'{grp}:adanmış {h:.0f}/{nn:.3g}')
            else: parca.append(yuzey(c, n)); kullan.append(f'{grp}:tablo {c}/{n}')
        if m['kq']: parca.append(yuzey(m['cS'], N_R)); kullan.append(f'boy:tablo {m["cS"]}')
        cst = bedel(f, p); pt = sum(x[0] for x in parca) - cst; lo = sum(x[1] for x in parca) - cst; hi = sum(x[2] for x in parca) - cst
        sonuc.append((pt, lo, hi, f'({dil.AL[f]},{dil.BE[f]:+d},{p}) kF={kF}', kullan))
    best = max(sonuc, key=lambda x: x[0]); LO = max(x[1] for x in sonuc); HI = max(x[2] for x in sonuc)
    karar = 'ÜSTTE' if LO >= SQ - 1e-6 else ('ALTTA' if HI < SQ - 1e-6 else 'BELİRSİZ')
    print(f"{os.path.basename(k['ad'])} {k['sira']}: önce nokta {k['pt']:.2f} [{k['lo']:.2f}, {'∞' if k['hi'] > 1e8 else round(k['hi'], 2)}] → "
          f"sonra nokta {best[0]:.2f} [{LO:.2f}, {'∞' if math.isinf(HI) else round(HI, 2)}] → {karar}  ({best[3]}; {', '.join(best[4])})")
