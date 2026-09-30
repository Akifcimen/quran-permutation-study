#!/bin/bash
# "tam belgeli arama": defter bataryası (99 olgu) + 646 mercek (doğrusal + kesif24 doğrusal-olmayan), iddia bataryası, matris kapalı
D="Σw=m·kG|Σv(U) kare/asal-kuvvet|MX:"
./robot4 table J10 41 2000000 8 tab/T_J10_2M.bin
ROBOT_MINTR=1000 ROBOT_DISABLE="$D" ./robot4 score J10 42 2000000 8 tab/T_J10_2M.bin puan/T_J10_iddia_nomx 2>/dev/null
./robot4 table U 43 2000000 8 tab/T_U_2M.bin
ROBOT_MINTR=1000 ROBOT_DISABLE="$D" ./robot4 score U 44 2000000 8 tab/T_U_2M.bin puan/T_U_iddia_nomx 2>/dev/null
echo TAM_ARAMA_BITTI
