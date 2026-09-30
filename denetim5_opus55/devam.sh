#!/bin/bash
# Kalan tek adım: tam belgeli arama (defter bataryası, matris kapalı, 646 mercek), TEKDÜZE sıfır, doğrudan ölçüm.
cd "$(dirname "$0")"
D="Σw=m·kG|Σv(U) kare/asal-kuvvet|MX:"
ROBOT_MINTR=1000 ROBOT_DISABLE="$D" ./robot4 score U 44 2000000 8 tab/T_U_2M.bin puan/T_U_iddia_nomx 2>/dev/null
python3 - <<'EOF'
import numpy as np
tag='T_U_iddia_nomx'
q=np.loadtxt(f'puan/{tag}_kuran.txt'); x=np.fromfile(f'puan/{tag}_sifir.f32',dtype=np.float32).reshape(-1,9); n=len(x)
kb=int(np.sum(x[:,1]>=q[1]-1e-6)); kf=int(np.sum(x[:,4]>=q[4]-1e-6)); p=(kb+1)/(n+1)
print(f"Tam belgeli arama, TEKDÜZE: Kur'an T2={q[1]:.2f} | serbest p={p:.2e} ({kb}/{n}) | sabit p={(kf+1)/(n+1):.2e} ({kf}) | ×7 okul → {min(1,7*p):.2e} (~{round(1/min(1,7*p))}'de 1)")
EOF
