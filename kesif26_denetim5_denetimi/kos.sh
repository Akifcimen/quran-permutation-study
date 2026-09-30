#!/bin/bash
cd "$(dirname "$0")"; T=../denetim5_opus55/tab; D="Σw=m·kG|Σv(U) kare/asal-kuvvet"
export ROBOT_MINTR=1000 ROBOT_DISABLE="$D"
for NUL in J10 U; do
  ROBOT_MSET=19 ./robot_kisit score $NUL 919 2000000 8 $T/${NUL}_2M.bin m19_${NUL} 2>&1 | tail -1
  ROBOT_FSET=0  ./robot_kisit score $NUL 919 2000000 8 $T/${NUL}_2M.bin f11_${NUL} 2>&1 | tail -1
done
echo KISIT_BITTI
