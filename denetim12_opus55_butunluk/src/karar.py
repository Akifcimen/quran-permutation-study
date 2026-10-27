# karar.py — ana hattın tahmininin ön-kayıt §9 kurallarıyla değerlendirilmesi (ve ek/EK_..._O_ve_Ousu_tahmin.md: O ve O' ayrı).
# Sayılar doğrudan sonuç dosyalarından okunur: sonuc/sira/<sk>_{ozet,bilgi}.txt, sonuc/belirsiz/<sk>_duzelt.txt (varsa),
# sonuc/kosullu/<sk>{O,P}.txt.
# Kural: k ≥ 1 → p̂ = (k+1)/(N+1); %95 aralığı = CP(k/N), belirsizlik aralığı [(k_alt+1)/(N+1), (k_üst+1)/(N+1)] ile birleştirilir.
#   "tuttu": p̂ tahmin aralığında; "tutmadı": birleşik aralık tümüyle tahmin aralığının dışında; aksi "karar verilemedi".
#   k = 0 → U = 1 − 0,05^{1/N} (belirsizlik varsa üst uç (k_üst+1)/(N+1) ile büyüğü); U < aralığın alt ucu → "tutmadı (daha nadir)";
#   aksi "karar verilemedi".
# Kullanım: python3 -I src/karar.py [sıfırkodeks ...]   (varsayılan U129 M129 L129)
import os, sys, re, math
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); sys.path.insert(0, os.path.join(KOK, 'src'))
from rapor_tablo import g, cp, oku_ozet, oku_bilgi, duzelt
AD = {'U': 'Tekdüze', 'M': 'M20', 'L': 'Mallows'}
ARALIK = {'U': (1e-12, 1e-10), 'M': (1e-10, 1e-6), 'L': (1e-10, 1e-6)}; KOSULLU = (1e-4, 1e-2)
def hukum(k, N, lo_t, hi_t, ka=None, ku=None):
    p = (k + 1) / (N + 1)
    if k == 0:
        U = 1 - 0.05 ** (1 / N)
        if ku is not None: U = max(U, (ku + 1) / (N + 1))
        h = 'tutmadı (daha nadir)' if U < lo_t else 'karar verilemedi'
        return h, f'k = 0 / {g(N)}: artı-bir sıra {g(p)}, üst sınır {g(U)}'
    lo, hi = cp(k, N)
    if ka is not None: lo = min(lo, (ka + 1) / (N + 1)) if ka == 0 else min(lo, cp(ka, N)[0])
    if ku is not None: hi = max(hi, (ku + 1) / (N + 1))
    if lo_t <= p <= hi_t: h = 'tuttu'
    elif hi < lo_t: h = 'tutmadı (daha nadir)'
    elif lo > hi_t: h = 'tutmadı (daha az nadir)'
    else: h = 'karar verilemedi'
    return h, f'p̂ = {g(p)}, birleşik aralık [{g(lo)}; {g(hi)}]'
def kosullu(fn):
    s = open(fn).read(); N = int(re.search(r'havuz N (\d+)', s).group(1))
    m = re.search(r'k_alt (\d+) k_nokta (\d+) k_ust (\d+)', s); ka, kn, ku = map(int, m.groups())
    ks = int(re.search(r'sayim sabit: k (\d+)', s).group(1))
    return N, ka, kn, ku, ks
sk_l = sys.argv[1:] or ['U129', 'M129', 'L129']
print('| Parça | Sıfır | Puan | Ölçülen | Tahmin aralığı | Hüküm |')
print('|---|---|---|---|---|---|')
for sk in sk_l:
    if not os.path.exists(f'{KOK}/sonuc/sira/{sk}_bilgi.txt'): continue
    lo_t, hi_t = ARALIK[sk[0]]; N, b = oku_bilgi(sk); SQ, ka, kn, ku = b[0]; dz = duzelt(sk)
    if dz: ka, kn, ku = dz
    h, ac = hukum(kn, N, lo_t, hi_t, ka, ku)
    ek = f'; \\|log₁₀ p̂ + 8\\| = {abs(math.log10((kn + 1) / (N + 1)) + 8):.2f}'.replace('.', ',') if sk[0] in 'ML' else ''
    print(f'| nesnenin bütünü | {AD[sk[0]]} | bilgi (V0, P0) | {ac}{ek} | [{g(lo_t)}; {g(hi_t)}] | **{h}** |')
    k, n = oku_ozet(sk)['G']; h, ac = hukum(k, n, lo_t, hi_t)
    ek = f'; \\|log₁₀ p̂ + 8\\| = {abs(math.log10((k + 1) / (n + 1)) + 8):.2f}'.replace('.', ',') if sk[0] in 'ML' else ''
    print(f'| nesnenin bütünü | {AD[sk[0]]} | sayım (G) | {ac}{ek} | [{g(lo_t)}; {g(hi_t)}] | **{h}** |')
for kos, ad in (('O', "O (görevin birincil koşulu)"), ('P', "O' = O ∧ terazi (tahminin ifadesi)")):
    for sk in sk_l:
        fn = f'{KOK}/sonuc/kosullu/{sk}{kos}.txt'
        if not os.path.exists(fn): continue
        N, ka, kn, ku, ks = kosullu(fn); Nt = f'{N:,}'.replace(',', '.')
        h, ac = hukum(kn, N, *KOSULLU, ka, ku)
        print(f'| eski literatür koşullu: {ad} | {AD[sk[0]]} | bilgi | {ac} (N = {Nt}) | [{g(KOSULLU[0])}; {g(KOSULLU[1])}] | **{h}** |')
        h, ac = hukum(ks, N, *KOSULLU)
        print(f'| eski literatür koşullu: {ad} | {AD[sk[0]]} | sayım | {ac} (N = {Nt}) | [{g(KOSULLU[0])}; {g(KOSULLU[1])}] | **{h}** |')
