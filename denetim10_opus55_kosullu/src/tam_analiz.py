#!/usr/bin/env python3
"""Tam birlikte-olasılık sırası (ONKAYIT §4.6): <prefix>.tam.txt → k_tam ve Jeffreys sonsalından 4.000 çekilişle aralık.
Kullanım: python3 -I src/tam_analiz.py <prefix>.tam.txt [N_havuz]"""
import sys, math, numpy as np
fn = sys.argv[1]
rows = []; head = ''
for line in open(fn):
    if line.startswith('#'):
        head += line; continue
    left, hik = line.split('|')
    a = left.split()
    rows.append(dict(sahip=int(a[0]), t2t=float(a[1]), t2r=float(a[2]), duzey=int(a[3]), h1=int(a[4]), nF=int(a[5]), m=int(a[6]),
                     h2=float(a[7]), h2q=float(a[8]), h2n=int(a[9]), P=float(a[10]), S=float(a[11]), hik=hik.strip()))
N = int(head.split('N=')[1].split()[0]); R = int(head.split('R=')[1].split(',')[0])
rng = np.random.default_rng(20261023)
B = 4000
def ceken(r):
    Ns = N - 1 if r['sahip'] >= 0 else N
    if r['duzey'] == 1:
        return rng.gamma(r['h1'] + 0.5, 1.0, B) / Ns
    if r['m'] == 0 or r['nF'] == 0:
        return np.full(B, 0.5 / Ns)
    frac = rng.gamma(r['nF'] + 0.5, 1.0, B) / Ns
    RR = r.get('R', R)
    if r['h2n'] < 30:   # az isabet: Poisson/Jeffreys (iskeletler arası sapma ihmal; uyarı yazılır)
        mean = rng.gamma(r['h2n'] + 0.5, 1.0, B) / (RR * r['m'])
    else:
        mu = r['h2'] / r['m']; var = max(r['h2q'] / r['m'] - mu * mu, 0) / r['m']
        mean = np.clip(rng.normal(mu, math.sqrt(var), B), 1e-300, None)
    return frac * mean
Q = [r for r in rows if r['sahip'] < 0][0]; riv = [r for r in rows if r['sahip'] >= 0]
# isteğe bağlı: ek dosyalardan rakip satırları (ör. bütün bant) ve Kur'an'ın çözünürlüğü artırılmış satırı
for extra in sys.argv[2:]:
    if extra.startswith('kuran='):
        for line in open(extra[6:]):
            if line.startswith('#'):
                if 'R=' in line: Rq = int(line.split('R=')[1].split(',')[0])
                continue
            l, h = line.split('|'); a = l.split()
            if int(a[0]) < 0: Q = dict(sahip=-1, t2t=float(a[1]), t2r=float(a[2]), duzey=int(a[3]), h1=int(a[4]), nF=int(a[5]), m=int(a[6]), h2=float(a[7]), h2q=float(a[8]), h2n=int(a[9]), P=float(a[10]), S=float(a[11]), hik=h.strip(), R=Rq)
    elif extra.startswith('ek='):
        var = {r['sahip'] for r in riv}
        for line in open(extra[3:]):
            if line.startswith('#'): continue
            l, h = line.split('|'); a = l.split()
            if int(a[0]) >= 0 and int(a[0]) not in var: riv.append(dict(sahip=int(a[0]), t2t=float(a[1]), t2r=float(a[2]), duzey=int(a[3]), h1=int(a[4]), nF=int(a[5]), m=int(a[6]), h2=float(a[7]), h2q=float(a[8]), h2n=int(a[9]), P=float(a[10]), S=float(a[11]), hik=h.strip()))
dQ = ceken(Q)
D = np.array([ceken(r) for r in riv]) if riv else np.zeros((0, B))
Sq = -np.log10(dQ); S = -np.log10(D) if len(riv) else np.zeros((0, B))
k = (S >= Sq[None, :]).sum(0) if len(riv) else np.zeros(B)
kp = sum(1 for r in riv if r['S'] >= Q['S'])
DEL = float(head.split('≥Q−')[1].split(')')[0]); yakin = [r for r in riv if r['t2t'] >= Q['t2t'] - DEL]
print(head.strip())
print(f"Kur'an: T2*={Q['t2t']:.3f}  düzey {Q['duzey']}  P_tam={Q['P']:.3e}  S_tam={Q['S']:.3f}  (h1={Q['h1']}, nF={Q['nF']}, m={Q['m']}, isabet={Q['h2n']})  D=S_tam−T2*={Q['S']-Q['t2t']:+.3f}")
print(f"değerlendirilen rakip {len(riv)} (yakın {len(yakin)}); nokta kestirimle Kur'an'ı tam puanla geçen/eşleyen k_tam = {kp}")
print(f"sonsal (4.000): k_tam medyan {np.median(k):.0f}, %95 [{np.percentile(k,2.5):.0f}, {np.percentile(k,97.5):.0f}] → p_tam = (k+1)/(N+1) medyan {(np.median(k)+1)/(N+1):.3e}")
bant = [r for r in riv if r not in yakin]
gec = [r for r in bant if r['S'] >= Q['S']]
print(f"denetim bandı: {len(bant)} kitap, Kur'an'ı tam puanla geçen {len(gec)}")
az = [r for r in rows if r['duzey'] == 2 and r['h2n'] < 30]
print(f"düzey-2'de 30'dan az isabetli hikâye: {len(az)} (bunlarda iskeletler arası sapma aralığa girmedi)")
Dr = np.array([r['S'] - r['t2t'] for r in riv]) if riv else np.array([])
if len(Dr): print(f"rakiplerin D = S_tam − T2* dağılımı: medyan {np.median(Dr):+.2f}, çeyrekler [{np.percentile(Dr,25):+.2f}, {np.percentile(Dr,75):+.2f}]")
