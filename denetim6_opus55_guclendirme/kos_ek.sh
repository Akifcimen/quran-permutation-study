#!/bin/bash
# ONKAYIT_EK.md — keşif amaçlı çift-sayma kontrolü (terazi merceği "satın aldıysa").
cd "$(dirname "$0")"
D2="Σw=m·kG|Σv(U) kare/asal-kuvvet"; DX="$D2|MX:"
sc(){ ROBOT_MODS=19 ROBOT_MINTR=1000 ROBOT_DISABLE="$4" ./$2 score $5 $6 2000000 8 $7 puan/$1 2>/dev/null | tee -a sonuc/kosu_ek.log; }
sc E1_U   robot_m  19 "$D2|kitap:" U   19901 tab/U_2M.bin
sc E1_J10 robot_m  19 "$D2|kitap:" J10 19902 tab/J10_2M.bin
sc E1_M20 robot_m  19 "$D2|kitap:" M20 19903 tab/M20_2M.bin
sc E3_U   robot4_m 19 "$DX|kitap:" U   19911 tab/T_U_2M.bin
sc E3_J10 robot4_m 19 "$DX|kitap:" J10 19912 tab/T_J10_2M.bin
sc E3_M20 robot4_m 19 "$DX|kitap:" M20 19913 tab/T_M20_metin14_2M.bin
echo "EK_BITTI" | tee -a sonuc/kosu_ek.log
