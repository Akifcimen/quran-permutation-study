# hesap_saati.py — günlüğe kayıtlı hesap (çekirdek-saat): referans süreçlerinin .say süreleri, kontrol bölütlerinin süre_s alanları.
# Dondurma öncesi ölçümler ve kısa analizler dahil değildir (ayrıca kestirilir). Kullanım: python3 -I src/hesap_saati.py
import os, glob
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
top = {}
for S in ('M', 'U', 'L'):
    r = 0.0
    for fn in glob.glob(os.path.join(KOK, 'tab', S, 'ref_*.say')):
        w = open(fn).read().split(); r += float(w[w.index('sure_s') + 1])
    k = 0.0
    for fn in glob.glob(os.path.join(KOK, 'tab', S, 'k*', 'b*.txt')):
        w = open(fn).readline().split()
        if 'sure_s' in w: k += float(w[w.index('sure_s') + 1])
    top[S] = (r / 3600, k / 3600)
t = sum(a + b for a, b in top.values())
for S, (a, b) in top.items(): print(f'{S}: referans {a:.2f} çekirdek-saat, kontrol {b:.2f} çekirdek-saat')
print(f'toplam (kayıtlı): {t:.2f} çekirdek-saat')
