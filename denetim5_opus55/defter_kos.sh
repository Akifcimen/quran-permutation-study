#!/bin/bash
D="Σw=m·kG|Σv(U) kare/asal-kuvvet"
for spec in "J10 21 22" "M20 23 24" "U 25 26"; do
  set -- $spec
  ./robot2 table $1 $2 2000000 8 tab/D_$1_2M.bin
  ROBOT_MINTR=1000 ROBOT_DISABLE="$D" ./robot2 score $1 $3 2000000 8 tab/D_$1_2M.bin puan/D_$1_iddia 2>/dev/null
  ROBOT_MINTR=1000 ./robot2 score $1 $3 2000000 8 tab/D_$1_2M.bin puan/D_$1_tam
done
