# s0_sinif_ozet.py — sonradan S0 duyarlılığı ve keşif (sınıf-kısıtlı) sayılarının özeti (ek/EK_..._sifir_toplam_yozlasmasi.md,
# ek/EK_..._sinif_kisitli_kesif.md). Girdi: sonuc/s0/<sk>_{R,T}.txt, sonuc/sinif/<sk>_{R,T}.txt (src/s0, src/sinif çıktıları).
# Kullanım: python3 -I src/s0_sinif_ozet.py <sıfırkodeks> <N_T> <N_R>
import sys, os, collections
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); sk = sys.argv[1]; NT = int(float(sys.argv[2])); NR = int(float(sys.argv[3]))
def oku(fn, s0):
    d = {}; kq = None
    for l in open(fn):
        t = l.split()
        if t[0] == 'KURAN': kq = t; continue
        d[(t[0], t[1])] = t
    return d, kq
s0 = {}; sn = {}
for tur in 'RT':
    a, kq0 = oku(f'{KOK}/sonuc/s0/{sk}_{tur}.txt', True); s0.update({(tur,) + k: v for k, v in a.items()})
    b, kq1 = oku(f'{KOK}/sonuc/sinif/{sk}_{tur}.txt', False); sn.update({(tur,) + k: v for k, v in b.items()})
CQ = int(kq0[2]); SQ = float(kq1[1])
fn_n = f'{KOK}/sonuc/esik_nihai_{sk}.txt'   # nihai eşik (adanmış kestirimle düzeltilmiş) varsa o kullanılır
if os.path.exists(fn_n):
    for l in open(fn_n):
        if l.startswith('SQ 0 '): SQ = float(l.split()[2])
S0 = collections.Counter(int(v[5]) for v in s0.values())
print(f'{sk}: Kur\'an C = {CQ}, S0 = {kq0[2]}; sınıf-kısıtlı bilgi {SQ:.4f}, sayım {kq1[2]}, S0 {kq1[3]}')
print(f'  S0 (bütün mercekler; R+T, N = {NR+NT:.3g}): dağılım (≥ {CQ-2}): ' + ', '.join(f'{s}: {S0[s]}' for s in sorted(S0, reverse=True) if s >= CQ - 2))
k = sum(c for s, c in S0.items() if s >= CQ); print(f'  S0 ≥ {CQ}: k = {k} → p = {(k+1)/(NR+NT+1):.3g}')
ki = sum(1 for kk, v in sn.items() if kk[0] == 'T' and float(v[2]) >= SQ - 5e-5)
kc = sum(1 for v in sn.values() if int(v[3]) >= CQ); ks = sum(1 for v in sn.values() if int(v[4]) >= CQ)
print(f'  keşif (sınıf-kısıtlı): bilgi ≥ S_Q: {ki} / {NT:.3g} (T) → {(ki+1)/(NT+1):.3g}; sayım ≥ {CQ}: {kc} / {NR+NT:.3g} → {(kc+1)/(NR+NT+1):.3g}; '
      f'S0 ≥ {CQ}: {ks} → {(ks+1)/(NR+NT+1):.3g}')
top = sorted(((float(v[2]), kk) for kk, v in sn.items() if kk[0] == 'T'), reverse=True)[:5]
print('  sınıf-kısıtlı bilgi en yüksek 5: ' + '; '.join(f'{x:.2f} ({os.path.basename(kk[1])} {kk[2]}, mercek {sn[kk][5]})' for x, kk in top))
