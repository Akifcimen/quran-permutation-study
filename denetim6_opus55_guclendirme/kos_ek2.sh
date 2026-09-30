#!/bin/bash
# KEŞİF AMAÇLI sağlamlık: bir 19-analisti 38 = 2×19'a da bakabilirdi → A1c bataryası m ∈ {19,38}. Yalnız zayıflatabilir.
# M20 için 38'i içeren bir defter tablosu yok → yalnız m ∈ {19,38} merceklerinde yeni tablo (tohum 1960).
cd "$(dirname "$0")"
DX="Σw=m·kG|Σv(U) kare/asal-kuvvet|MX:"
until grep -q "EK_BITTI" sonuc/kosu_ek.log 2>/dev/null; do sleep 5; done
ROBOT_MODS=19,38 ./robot4_m table M20 1960 2000000 8 tab/T_M20_m19_38_2M.bin | tee -a sonuc/kosu_ek.log
sc(){ ROBOT_MODS=19,38 ROBOT_MINTR=1000 ROBOT_DISABLE="$DX" ./robot4_m score $2 $3 2000000 8 $4 puan/$1 2>/dev/null | tee -a sonuc/kosu_ek.log; }
sc X38_U   U   19921 tab/T_U_2M.bin
sc X38_J10 J10 19922 tab/T_J10_2M.bin
sc X38_M20 M20 19923 tab/T_M20_m19_38_2M.bin
echo "EK2_BITTI" | tee -a sonuc/kosu_ek.log
