import numpy as np, os, sys
rows=[("Tam (60 olgu)","U","U_tam_mt"),("Tam (60 olgu)","J10","J10_tam_mt"),("Tam (60 olgu)","M20","M20_tam_mt"),
      ("İddia (60−2 süs)","U","U_iddia_mt"),("İddia (60−2 süs)","J10","J10_iddia_mt"),("İddia (60−2 süs)","M20","M20_iddia_mt"),
      ("Kör (−hücre −2 süs)","U","U_kor"),("Kör (−hücre −2 süs)","J10","J10_kor"),("Kör (−hücre −2 süs)","M20","M20_kor"),
      ("Defter-iddia (99−2)","J10","D_J10_iddia"),("Defter-iddia (99−2)","M20","D_M20_iddia"),("Defter-iddia (99−2)","U","D_U_iddia"),
      ("Defter-tam (99)","J10","D_J10_tam"),("Defter-tam (99)","M20","D_M20_tam"),("Defter-tam (99)","U","D_U_tam")]
print(f"| Batarya | Sıfır | Kur'an T2 (serbest) | p serbest | Kur'an T2 (1,1,19) | p sabit (2M) | sıfır medyan / %99,99 |")
print("|---|---|---|---|---|---|---|")
for bat,nul,tag in rows:
    fq=f'puan/{tag}_kuran.txt'; fs=f'puan/{tag}_sifir.f32'
    if not (os.path.exists(fq) and os.path.exists(fs)): continue
    q=np.loadtxt(fq); x=np.fromfile(fs,dtype=np.float32).reshape(-1,9); n=len(x)
    kb=np.sum(x[:,1]>=q[1]-1e-6); kf=np.sum(x[:,4]>=q[4]-1e-6)
    pb=(kb+1)/(n+1); pf=(kf+1)/(n+1)
    med,q4=np.quantile(x[:,1],[0.5,0.9999])
    print(f"| {bat} | {nul} | {q[1]:.2f} | {pb:.1e} ({kb}/{n:,}) | {q[4]:.2f} | {pf:.1e} ({kf}) | {med:.2f} / {q4:.2f} |")
