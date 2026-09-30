#!/bin/bash
cd "$(dirname "$0")"; T=../denetim5_opus55/tab
export ROBOT_MINTR=1000 ROBOT_DISABLE="Σw=m·kG|Σv(U) kare/asal-kuvvet|MX:" ROBOT_COHERENT=1
for NUL in J10 U; do ROBOT_MSET=19 ./robot4_tutarli score $NUL 919 2000000 8 $T/T_${NUL}_2M.bin tt19_${NUL} 2>&1 | tail -1; done
echo M19_BITTI
for NUL in J10 U; do ./robot4_tutarli score $NUL 919 2000000 8 $T/T_${NUL}_2M.bin tt_${NUL} 2>&1 | tail -1; done
echo SERBEST_BITTI
