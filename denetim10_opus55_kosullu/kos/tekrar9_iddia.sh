#!/bin/bash
# EK (keşif): Ö0(a) iddia bataryasıyla (60 olgu; defterin 0–14. düğümleri), 129, terazi kapalı, aynı tablo tohumları.
cd "$(dirname "$0")/.."
B129="7,286,200,176,120,165,206,75,129,109,123,111,43,52,99,128,111,110,98,135,112,78,118,64,77,227,93,88,69,60,34,30,73,54,45,83,182,88,75,85,54,53,89,59,37,35,38,29,18,45,60,49,62,55,78,96,29,22,24,13,14,11,11,18,12,12,30,52,52,44,28,28,20,56,40,31,50,40,46,42,29,19,36,25,22,17,19,26,30,20,15,21,11,8,8,19,5,8,8,11,11,8,3,9,5,4,7,3,6,3,5,4,5,6"
for r in 1 2 3; do for nul in U J10 M20; do
  case $nul in U) o=1;; J10) o=2;; M20) o=3;; esac
  ts=$((9100*r+o)); es=$((ts+1000)); tag=129_${nul}_${r}
  [ -f sonuc/t9/iddia_${tag}.txt ] && continue
  ROBOT_BASE=$B129 ROBOT_MODS=19 nice -n 10 ./ref/robot4_m table $nul $ts 2000000 8 tab/t9/tabI_$tag.bin > /dev/null 2>&1
  IDDIA=1 nice -n 10 ./src/tekrar9 129 $nul $es 2000000 8 tab/t9/tabI_$tag.bin - 1 sonuc/t9/iddia_${tag}
  rm -f tab/t9/tabI_$tag.bin; echo "[$(date -u +%FT%TZ)] iddia $tag bitti" >> sonuc/t9/kosu.log
done; done
