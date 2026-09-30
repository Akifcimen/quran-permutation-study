import numpy as np, sys
def lens_name(li):
    FORMS=[(1,1),(1,2),(1,3),(2,1),(2,3),(3,1),(3,2)]; f=li//38; m=3+li%38; return f"({FORMS[f][0]},{FORMS[f][1]},{m})"
for tag in sys.argv[1:]:
    q=np.loadtxt(f'puan/{tag}_kuran.txt')
    x=np.fromfile(f'puan/{tag}_sifir.f32',dtype=np.float32).reshape(-1,9)
    n=len(x)
    print(f"=== sıfır={tag}  N={n:,}")
    for j,name in enumerate(['T1 (düğüm toplamı)','T2 (en iyi 8 düğüm)','T3 (çatal cezalı)']):
        qb, qf = q[j], q[3+j]
        pb = (np.sum(x[:,j]>=qb-1e-6)+1)/(n+1); pf=(np.sum(x[:,3+j]>=qf-1e-6)+1)/(n+1)
        print(f"  {name}: Kur'an serbest={qb:.2f}  p_serbest={pb:.2e} ({int(np.sum(x[:,j]>=qb-1e-6))} kitap ≥)  |  sabit(1,1,19)={qf:.2f} p_sabit={pf:.2e} ({int(np.sum(x[:,3+j]>=qf-1e-6))} ≥)")
        qs=np.quantile(x[:,j],[0.5,0.9,0.99,0.999,0.9999]); print(f"     sıfır dağılımı (serbest) medyan/90/99/99.9/99.99%: "+" ".join(f"{v:.2f}" for v in qs), f" maks={x[:,j].max():.2f}")
    # en iyi merceği Kur'an'ı geçen kitaplar hangi merceklerde?
    top=np.argsort(-x[:,1])[:5]
    print("  T2'de en yüksek 5 sıfır kitabı:", [(round(float(x[i,1]),2), lens_name(int(x[i,7]))) for i in top])
