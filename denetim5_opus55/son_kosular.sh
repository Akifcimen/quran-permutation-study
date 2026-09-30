#!/bin/bash
# defter_kos.sh J10 bölümünden sonra: tekdüze sıfırda defter bataryası + doğrusal-olmayan mercekli robot3 (J10)
D="Σw=m·kG|Σv(U) kare/asal-kuvvet"
./robot2 table U 25 2000000 8 tab/D_U_2M.bin
ROBOT_MINTR=1000 ROBOT_DISABLE="$D" ./robot2 score U 26 2000000 8 tab/D_U_2M.bin puan/D_U_iddia 2>/dev/null
./robot3 table J10 31 2000000 8 tab/N_J10_2M.bin
ROBOT_MINTR=1000 ROBOT_DISABLE="$D" ./robot3 score J10 32 2000000 8 tab/N_J10_2M.bin puan/N_J10_iddia 2>/dev/null
./robot3 table U 33 2000000 8 tab/N_U_2M.bin
ROBOT_MINTR=1000 ROBOT_DISABLE="$D" ./robot3 score U 34 2000000 8 tab/N_U_2M.bin puan/N_U_iddia 2>/dev/null
# matris düğümü kapalı (yazar matris katmanını çekirdek dışı tutar; standart Tevbe=129 kabulüne denk)
ROBOT_MINTR=1000 ROBOT_DISABLE="$D|MX:" ./robot2 score J10 22 2000000 8 tab/D_J10_2M.bin puan/D_J10_iddia_nomx 2>/dev/null
ROBOT_MINTR=1000 ROBOT_DISABLE="$D|MX:" ./robot2 score U 26 2000000 8 tab/D_U_2M.bin puan/D_U_iddia_nomx 2>/dev/null
echo SON_KOSULAR_BITTI
