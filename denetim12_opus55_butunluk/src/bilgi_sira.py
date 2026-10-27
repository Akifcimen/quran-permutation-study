# bilgi_sira.py — bilgi puanının nihai sırası (ONKAYIT §4.1–4.3) sınama adaylarından: nihai eşikle k_alt, k_nokta, k_üst;
# geçici eşikle .say sayılarına tutarlılık denetimi; belirsiz kitapların S_alt'a göre azalan listesi (adanmış çözüm sırası).
# Kullanım: python3 -I src/bilgi_sira.py <sıfırkodeks> [liste_adet=30]   (nihai eşik: sonuc/esik_nihai_<ad>.txt; yoksa geçici)
import os, sys, glob, struct, math
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ad = sys.argv[1]; adet = int(sys.argv[2]) if len(sys.argv) > 2 else 30
def esik(fn):
    d = {}
    for line in open(fn):
        if line.startswith('SQ'): t = line.split(); d[int(t[1])] = float(t[2])
    return d
gec = esik(os.path.join(KOK, 'sonuc', f'esik_{ad}.txt'))
fn_n = os.path.join(KOK, 'sonuc', f'esik_nihai_{ad}.txt'); nih = esik(fn_n) if os.path.exists(fn_n) else dict(gec)
R = 362; kayit = {}; N = 0; say_k = {v: [0, 0, 0] for v in range(8)}
for f in sorted(glob.glob(os.path.join(KOK, 'tab', f'T_{ad}', '*.say'))):
    L = open(f).read().split('\n'); N += int(L[0].split()[7])
    for s in L[1:]:
        t = s.split()
        if t and t[0] == 'degiske': v = int(t[1]); say_k[v][0] += int(t[5]); say_k[v][1] += int(t[7]); say_k[v][2] += int(t[9])
    d = open(f[:-4] + '.aday', 'rb').read()
    for o in range(0, len(d) - R + 1, R):
        sira, = struct.unpack_from('<Q', d, o + 354)
        pt = struct.unpack_from('<8f', d, o + 228); lo = struct.unpack_from('<8f', d, o + 260); hi = struct.unpack_from('<8f', d, o + 292)
        kayit[(f, sira)] = (pt, lo, hi)
print(f'{ad}: sınama N = {N:,}; aday (tekil) {len(kayit):,}')
for v in range(8):
    S = nih[v]; kl = sum(1 for pt, lo, hi in kayit.values() if lo[v] >= S - 1e-6); kp = sum(1 for pt, lo, hi in kayit.values() if pt[v] >= S - 1e-6)
    kh = sum(1 for pt, lo, hi in kayit.values() if hi[v] >= S - 1e-6)
    Sg = gec[v]; gkl = sum(1 for pt, lo, hi in kayit.values() if lo[v] >= Sg - 1e-6); gkp = sum(1 for pt, lo, hi in kayit.values() if pt[v] >= Sg - 1e-6)
    gkh = sum(1 for pt, lo, hi in kayit.values() if hi[v] >= Sg - 1e-6)
    tut = '✓' if (gkl, gkp, gkh) == tuple(say_k[v]) else f'≠ .say {tuple(say_k[v])}'
    U95 = 1 - 0.05 ** (1 / N) if N else float('nan')
    print(f'  v{v}: nihai S_Q {S:.3f}: k_alt {kl}, k_nokta {kp}, k_üst {kh} → p_nokta {(kp + 1) / (N + 1):.3g}, aralık [{(kl + 1) / (N + 1):.3g}, {(kh + 1) / (N + 1):.3g}]'
          + (f'; k_nokta = 0 → tek yanlı %95 üst sınır {U95:.3g}' if kp == 0 else '') + f'  | geçici eşikle adaylardan ({gkl},{gkp},{gkh}) {tut}')
S = nih[0]
ust = sorted(((pt[0], lo[0], hi[0], k) for k, (pt, lo, hi) in kayit.items() if pt[0] >= S - 1e-6), reverse=True)
print(f'  nokta puanı ≥ S_Q olanlar (v0): {len(ust)}')
for pt, lo, hi, k in ust: print(f'    nokta {pt:.2f} [{lo:.2f}, {"∞" if hi > 1e30 else round(hi, 2)}]  {os.path.basename(k[0])} sıra {k[1]} (zincir {k[1] % 64})')
bel = sorted(((lo[0], pt[0], k) for k, (pt, lo, hi) in kayit.items() if lo[0] < S - 1e-6 <= hi[0] or (hi[0] >= S - 1e-6 and lo[0] < S - 1e-6)), reverse=True)
print(f'  belirsiz (v0): {len(bel)}; S_alt en büyük {adet}:')
for lo, pt, k in bel[:adet]: print(f'    S_alt {lo:.2f}  S_nokta {pt:.2f}  {os.path.basename(k[0])} sıra {k[1]}')
if bel:
    import statistics
    print(f'  belirsizlerin S_alt medyanı {statistics.median([b[0] for b in bel]):.2f}, S_nokta en büyük {max(b[1] for b in bel):.2f}')
    D = sorted(S - b[0] for b in bel); q = lambda x: D[min(len(D) - 1, int(x * len(D)))]
    print(f'  belirsizlerde Δ = S_Q − S_alt (bit; eşiği geçmek için çözülmemiş aramanın %95 üst sınırından 2^Δ kat nadir olması gerekir): '
          f'en küçük {D[0]:.2f}, %10 {q(0.1):.2f}, medyan {q(0.5):.2f}, %90 {q(0.9):.2f}; Δ ≤ 1: {sum(1 for x in D if x <= 1)}, Δ ≤ 3: {sum(1 for x in D if x <= 3)}')
