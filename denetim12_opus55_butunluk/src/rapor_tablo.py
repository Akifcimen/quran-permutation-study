# rapor_tablo.py — sıra sonuçlarını rapor tablolarına dönüştürür (elle aktarım hatası olmasın diye).
# Girdi: sonuc/sira/<sk>_ozet.txt (src/ozet.py), sonuc/sira/<sk>_bilgi.txt (src/bilgi_sira.py), sonuc/kuran_<sk>.txt,
# varsa sonuc/belirsiz/<sk>_duzelt.txt (src/sira_duzelt.py: adanmış kestirimle düzeltilmiş v0 sayıları).
# Kullanım: python3 -I src/rapor_tablo.py <ana|duyarlilik> <sk> [<sk> ...]
import sys, os, re, math
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
AD = {'U': 'Tekdüze', 'M': 'M20', 'L': 'Mallows', 'J': 'J10'}
def g(x):  # bilimsel gösterim, Türkçe ondalık
    if x == 0: return '0'
    e = math.floor(math.log10(x)); m = x / 10 ** e
    if round(m, 1) >= 10: m /= 10; e += 1
    ust = str(e).translate(str.maketrans('-0123456789', '⁻⁰¹²³⁴⁵⁶⁷⁸⁹'))
    return f'{m:.1f}'.replace('.', ',') + '×10' + ust
def cp(k, N):  # Clopper–Pearson %95 (beta dağılımı yerine normal-olmayan kesin hesap: ikili arama)
    from math import lgamma, exp, log
    def bin_cdf(k, n, p):  # P(X ≤ k), Poisson yaklaşımı (N büyük, p küçük); her terim log uzayında (λ > ~745'te exp(−λ) taşmasın)
        lam = n * p
        if lam <= 0: return 1.0
        ll = log(lam)
        return sum(exp(-lam + i * ll - lgamma(i + 1)) for i in range(k + 1))
    def ara(f, lo, hi):
        for _ in range(200):
            mid = (lo + hi) / 2
            if f(mid): hi = mid
            else: lo = mid
        return (lo + hi) / 2
    lo = 0.0 if k == 0 else ara(lambda p: 1 - bin_cdf(k - 1, N, p) >= 0.025, 0.0, 1.0)
    hi = ara(lambda p: bin_cdf(k, N, p) <= 0.025, 0.0, 1.0)
    return lo, hi
def oku_ozet(sk):
    s = open(f'{KOK}/sonuc/sira/{sk}_ozet.txt').read(); d = {}
    for kapi in ('G', 'GE', 'Gc', 'G0'):
        m = re.search(rf'sayım \(kapı {kapi}\): k = (\d+) .*?/ N = ([\d,]+)', s)
        if m: d[kapi] = (int(m.group(1)), int(m.group(2).replace(',', '')))
    return d
def oku_bilgi(sk):
    s = open(f'{KOK}/sonuc/sira/{sk}_bilgi.txt').read(); d = {}
    N = int(re.search(r'sınama N = ([\d,]+)', s).group(1).replace(',', ''))
    for m in re.finditer(r'v(\d): nihai S_Q ([\d.]+): k_alt (\d+), k_nokta (\d+), k_üst (\d+)', s):
        d[int(m.group(1))] = (float(m.group(2)), int(m.group(3)), int(m.group(4)), int(m.group(5)))
    return N, d
def duzelt(sk):
    fn = f'{KOK}/sonuc/belirsiz/{sk}_duzelt.txt'
    if not os.path.exists(fn): return None
    m = re.search(r'k_alt (\d+), k_nokta (\d+), k_üst (\d+)', open(fn).read())
    return tuple(int(x) for x in m.groups())
def sayim_satir(k, N):
    lo, hi = cp(k, N); p = (k + 1) / (N + 1)
    if k == 0: return f'k = 0 / {g(N)} → {g(p)} (tek yanlı %95 üst sınır {g(1 - 0.05 ** (1 / N))})'
    return f'{k} / {g(N)} → **{g(p)}** [{g(lo)}; {g(hi)}]'
def bilgi_satir(N, ka, kn, ku):
    p = (kn + 1) / (N + 1); lo, hi = cp(kn, N)
    s = f'{ka} / {kn} / {ku} → **{g(p)}**'
    s += f' [k_nokta %95: {g(lo)}; {g(hi)}]' if kn > 0 else f' (k_nokta = 0: tek yanlı %95 üst sınır {g(1 - 0.05 ** (1 / N))})'
    s += f'; belirsizlik aralığı [{g((ka + 1) / (N + 1))}; {g((ku + 1) / (N + 1))}]'
    return s
if __name__ == '__main__':
    kip = sys.argv[1]
    if kip == 'ana':
        print('| Sıfır (kodeks) | S_Q (bit) | **bilgi (V0, P0)**: k_alt / k_nokta / k_üst → p_nokta | sayım (kapı G): k / N → p [%95] |')
        print('|---|---|---|---|')
        for sk in sys.argv[2:]:
            oz = oku_ozet(sk); N, b = oku_bilgi(sk); SQ, ka, kn, ku = b[0]; dz = duzelt(sk); not_ = ''
            if dz: ka, kn, ku = dz; not_ = ' (adanmış kestirimle düzeltilmiş)'
            print(f'| {AD[sk[0]]} ({sk[1:]}) | {SQ:.2f}'.replace('.', ',') + f' | {bilgi_satir(N, ka, kn, ku)}{not_} | {sayim_satir(*oz["G"])} |')
    else:
        print('| Sıfır (kodeks) | sayım G | GE | Gc | G0 | V0 P0 | V0 P1 | V0 P2 | V0 P3 | V1 P0 | V1 P1 | V1 P2 | V1 P3 |')
        print('|---|---|---|---|---|---|---|---|---|---|---|---|---|')
        for sk in sys.argv[2:]:
            oz = oku_ozet(sk); N, b = oku_bilgi(sk); h = []; dz = duzelt(sk)
            for kapi in ('G', 'GE', 'Gc', 'G0'):
                k, n = oz[kapi]; h.append(g((k + 1) / (n + 1)))
            for v in range(8):
                SQ, ka, kn, ku = b[v]
                if v == 0 and dz: ka, kn, ku = dz   # V0 P0: adanmış kestirimle düzeltilmiş (ana tabloyla aynı)
                h.append(g((kn + 1) / (N + 1)) + ('*' if kn == 0 else ''))
            print(f'| {AD[sk[0]]} ({sk[1:]}) | ' + ' | '.join(h) + ' |')
