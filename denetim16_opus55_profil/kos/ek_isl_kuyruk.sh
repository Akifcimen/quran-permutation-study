#!/bin/bash
# ek_isl_kuyruk.sh — workstation (12 iş parçacığı, yalnız işlemci): kart yürüyüşleri beklenenden yavaş çıktığı için (adım başına ~150–200 µs)
# kart listeleri aynı tohumlarla duz.c'de koşulur (motorlar aynı sonucu verir). Kol A (6 iş parçacığı, hemen): doğrulamanın kart listeleri.
# Kol B (6 iş parçacığı, doğrulamanın işlemci işleri bitince): Mallows ana kestirimi (işlemci ve kart listeleri).
K=~/hrf_olcum/profil; cd $K; O=ek/cikti; G=$O/gunluk.txt; B=k:400000:50000000
( echo "[$(date -u +%FT%TZ)] kol A başladı (doğrulama kart listeleri işlemcide)" >> $G
  for S in M L U; do nice -n 10 bin/duz $S tab/$S/olaylar.txt ek/sec_D_${S}_kart.txt $B 0 4 6 $O/D_${S}_kartisl.txt > $O/D_${S}_kartisl.log 2>&1; echo "[$(date -u +%FT%TZ)] kol A $S bitti rc=$?" >> $G; done ) &
( while [ "$(ps -eo args | grep -cE '^bin/duz . tab/./olaylar.txt ek/sec_D1_')" != "0" ]; do sleep 30; done
  echo "[$(date -u +%FT%TZ)] kol B başladı (Mallows ana)" >> $G
  for L in isl kart; do nice -n 10 bin/duz L tab/L/olaylar.txt ek/sec_A_L_$L.txt $B 0 4 6 $O/A_L_${L}isl.txt > $O/A_L_${L}isl.log 2>&1; echo "[$(date -u +%FT%TZ)] kol B L $L bitti rc=$?" >> $G; done ) &
wait
