#!/bin/bash
cd "$(dirname "$0")"; T=../denetim5_opus55/tab
export ROBOT_MINTR=1000 ROBOT_DISABLE="Σw=m·kG|Σv(U) kare/asal-kuvvet" ROBOT_COHERENT=1
for NUL in J10 U; do
  ROBOT_MSET=19 ./robot_tutarli score $NUL 919 2000000 8 $T/${NUL}_2M.bin tut19_${NUL} 2>&1 | tail -1
  ./robot_tutarli score $NUL 919 2000000 8 $T/${NUL}_2M.bin tut_${NUL} 2>&1 | tail -1
done
echo TUTARLI_BITTI
