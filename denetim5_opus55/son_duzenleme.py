# robot4 sonuçları geldikten sonra raporun özet bölümlerini günceller
import numpy as np, sys
def pv(tag):
    q=np.loadtxt(f'puan/{tag}_kuran.txt'); x=np.fromfile(f'puan/{tag}_sifir.f32',dtype=np.float32).reshape(-1,9); n=len(x)
    kb=int(np.sum(x[:,1]>=q[1]-1e-6)); kf=int(np.sum(x[:,4]>=q[4]-1e-6))
    return q[1],(kb+1)/(n+1),kb,(kf+1)/(n+1),kf,n
def fmt(p):
    m,e=f"{p:.1e}".split('e'); return f"{m.replace('.',',')}×10⁻{str(int(e[1:])).translate(str.maketrans('0123456789','⁰¹²³⁴⁵⁶⁷⁸⁹'))}" if e.startswith('-') else f"{p:.2g}"
TJ=pv('T_J10_iddia_nomx'); TU=pv('T_U_iddia_nomx')
print("T_J10:",TJ); print("T_U:",TU)
s=open('KIRMA_RAPORU.md').read()
row=(f"| **Tam belgeli arama**: defter bataryası (99 olgu, matris kapalı) + 646 mercek (doğrusal + kesif24) | — | "
     f"**{fmt(TJ[1])}** serbest ({TJ[2]}/2×10⁶), {fmt(TJ[3])} sabit (J10) · tekdüzede **{fmt(TU[1])}** serbest ({TU[2]}/2×10⁶), {fmt(TU[3])} sabit |\n"
     f"| … × sayım okulu (×7) | ×7 | **~{fmt(min(1,7*TJ[1]))}** (J10; ~{round(1/min(1,7*TJ[1]))}'de 1) · tekdüzede **~{fmt(7*TU[1])}** (~{format(round(1/(7*TU[1])),',').replace(',','.')}'de 1) |\n")
anchor="| Ötesi: 19 geleneğinin onlarca yıllık araması"
assert anchor in s; s=s.replace(anchor,row+anchor,1)
open('KIRMA_RAPORU.md','w').write(s)
print("eklendi")
