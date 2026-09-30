#!/bin/bash
cd "$(dirname "$0")"; T=../denetim5_opus55/tab
export ROBOT_MINTR=1000 ROBOT_DISABLE="Σw=m·kG|Σv(U) kare/asal-kuvvet"
ROBOT_COHERENT=1 ./robot_tutarli demobest J10 919 2000000 8 $T/J10_2M.bin > best_J10_tutarli.txt 2>/dev/null
ROBOT_COHERENT=0 ./robot_tutarli demobest U 919 2000000 8 $T/U_2M.bin > best_U_karisik.txt 2>/dev/null
echo DEMOBEST_BITTI
