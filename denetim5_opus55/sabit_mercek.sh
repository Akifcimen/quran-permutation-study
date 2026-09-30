#!/bin/bash
D="Σw=m·kG|Σv(U) kare/asal-kuvvet"
run(){ # null seed tab
  echo "[$1 iddia-bataryası] $(ROBOT_DISABLE="$D" ./robot demofix $1 $2 40000000 $3 1 2>/dev/null | sed -n '2p')"
  echo "[$1 tam-batarya]     $(./robot demofix $1 $2 40000000 $3 1 2>/dev/null | sed -n '2p')"
}
run U 3001 tab/U_2M.bin
run J10 3002 tab/J10_2M.bin
run M20 3003 tab/M20_2M.bin
