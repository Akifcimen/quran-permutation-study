#!/usr/bin/env python3
# hasat_sayisal.py'nin ayni sablon ailesi; ek olarak: dokunulan sablon sayisi ve sansa gore beklenen duzeltme.
# Beklenen: degeri degisen ve standartta isabet olmayan her sablon yeni degerde 0'a 1/18 olasilikla duser
# (0 disindaki 18 kalintidan birinden rastgele kalintiya gecis); degeri degisen isabetli sablon kesin bozulur.
import importlib.util, statistics as st, sys
spec = importlib.util.spec_from_file_location('h', sys.path[0] + '/hasat_sayisal.py')
import io, contextlib
buf = io.StringIO()
with contextlib.redirect_stdout(buf):
    h = importlib.util.module_from_spec(spec); spec.loader.exec_module(h)
A0, degerler, V0 = h.A0, h.degerler, h.V0
def mod19(A):  # sablon -> kalinti (None: tanimsiz)
    out = {}
    for an, f in h.ALT.items():
        L = [(n, A[n]) for n in range(1, 115) if f(n, A[n])]
        if not L: continue
        for qn, g in h.NIC.items():
            v = g(L); out[(an, qn)] = None if v == 0 else v % 19
        for qn, g in h.BIT.items():
            out[(an, qn)] = h.cat_mod(g(L), 19)
    return out
R0 = mod19(A0)
rows = []
for i in range(1, 115):
    A = dict(A0); A[i] -= 2
    R = mod19(A)
    keys = set(R0) | set(R)
    dok = [k for k in keys if R0.get(k) != R.get(k)]
    dok_non = [k for k in dok if R0.get(k) not in (0,)]
    duz = sum(1 for k in dok if R.get(k) == 0 and R0.get(k) != 0)
    boz = sum(1 for k in dok if R0.get(k) == 0 and R.get(k) != 0)
    bek = sum(1 for k in dok_non if R.get(k) is not None) / 18
    rows.append((i, len(dok), duz, round(bek, 1), round(duz - bek, 1), boz))
d = {r[0]: r for r in rows}
print("sure, dokunulan sablon, duzeltme, sansa gore beklenen, fazla, bozma")
for r in sorted(rows, key=lambda r: -r[1])[:10]: print("  ", r)
fz = sorted((r[4] for r in rows), reverse=True)
t = d[9]
ust = sum(1 for v in fz if v > t[4])
print(f"\nTevbe: dokunulan {t[1]} (lig medyani {st.median(r[1] for r in rows)}), duzeltme {t[2]}, beklenen {t[3]}, fazla {t[4]}")
print(f"'fazla' siralamasi: Tevbe {ust+1}/114 | lig medyani {st.median(fz)} | en cok {fz[0]}")
import math
cor = st.correlation([r[1] for r in rows], [r[2] for r in rows])
print(f"dokunulan ile duzeltme arasindaki korelasyon: {cor:.2f}")
# Tevbe'nin bozduklari: standart metnin '129 lehine' kanitlari
R9 = dict(A0); R9[9] = 127; RR = mod19(R9)
boz9 = sorted(k for k in R0 if R0[k] == 0 and RR.get(k) != 0)
print(f"\nTevbe'yi 127 yapinca bozulan {len(boz9)} sablon (129'da 19'a bolunen) ornekleri:")
for k in boz9[:10]: print("    ", k)
Hq = [k for k in RR if RR[k] == 0]
