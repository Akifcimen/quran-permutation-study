#!/bin/bash
cd "$(dirname "$0")"; T=../denetim5_opus55/tab
export ROBOT_MINTR=1000 ROBOT_DISABLE="Σw=m·kG|Σv(U) kare/asal-kuvvet" ROBOT_MSET=19
for C in 0 1; do for NUL in J10 U; do ROBOT_COHERENT=$C ./robot4_tutarli score $NUL 919 2000000 8 $T/T_${NUL}_2M.bin mx19_c${C}_${NUL} 2>&1 | tail -1; done; done
echo KOS127_BITTI
