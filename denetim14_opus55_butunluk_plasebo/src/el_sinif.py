# el_sinif.py — el dilinde (20 yüklem) sınıf kuralının betimsel özeti (§9 a ve c'nin kesişimi; ön-kayıtta kalibrasyon olarak tanımlı değil).
# Girdi: sonuc/<S>/analiz.json. Kullanım: python3 -I src/el_sinif.py <S>
import os, sys, json, math
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from analiz import cp as _cp
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
S = sys.argv[1]
d = json.load(open(os.path.join(KOK, 'sonuc', S, 'analiz.json')))
K = d['K']; el = d['el']


def cp(k, n):
    return tuple(_cp(k, n))


q = el['0']
print(f'sıfır {S}, K = {K}; el dili')
print(f"Kur'an: eşik {q['esik']:.3f} bit, sınıf {q['sinif']}, k = {q['k']}, k^c = {q['kc']} / N = {q['N']:.0e} → p = {q['p']:.3e}, "
      f"p^c = {q['pc']:.3e} [{cp(q['kc'], q['N'])[0]:.2e}; {cp(q['kc'], q['N'])[1]:.2e}]")
pl = [el[str(i)] for i in range(1, K + 1)]
sin = [x for x in pl if x['sinif'] == 1]
print(f'el dilinde sınıfta plasebo: {len(sin)} / {K}')
ps = sorted(x['p'] for x in pl)
med = math.sqrt(ps[K // 2 - 1] * ps[K // 2])
print(f'plasebo el dili p: en küçük {ps[0]:.3e}, medyan {med:.3e}, p = 1 olan {sum(1 for x in pl if x["k"] + 1 == x["N"] + 1)}')
qlo, qhi = cp(q['kc'], q['N'])
Rc = 0
for x in sorted(sin, key=lambda x: x['pc']):
    lo, hi = cp(x['kc'], x['N'])
    ulas = x['pc'] <= q['pc'] or lo <= qhi
    Rc += ulas
    isaret = "  (Kur'an'a ulaşır)" if ulas else ''
    print(f"  sınıfta x{x['hedef']:03d}: eşik {x['esik']:.3f}, k = {x['k']}, k^c = {x['kc']} / {x['N']:.0e} → p = {x['p']:.3e}, p^c = {x['pc']:.3e}"
          f" [{lo:.2e}; {hi:.2e}]{isaret}")
if q['sinif'] == 1:
    print(f"betimsel: R^c_el = {Rc} → (1 + R^c)/(K + 1) = {(1 + Rc) / (K + 1):.4f}")
# Kur'an'ın el dili p'si, plaseboların el dili p'leri arasında (12. den.'in sorusu: Kur'an'a göre yazılmış dilde rastgele kitaplar)
qhi_el = cp(q['k'], q['N'])[1]
# k ≥ 10'da CP alt sınırı ≥ 0,479·k/N; k/N > 3·hi_Q olan plasebo aralık çakışmasıyla da ulaşamaz (pahalı CP hesabı atlanır)
Rel = sum(1 for x in pl if x['p'] <= q['p'] or
          (not (x['k'] >= 10 and x['k'] / x['N'] > 3 * qhi_el) and cp(x['k'], x['N'])[0] <= qhi_el))
print(f"betimsel: el dilinde Kur'an'ın p'sine ulaşan plasebo {Rel} / {K}")
