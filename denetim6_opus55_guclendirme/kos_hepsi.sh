#!/bin/bash
# Ön-kayıtlı bütün koşular (ONKAYIT.md §3–§4). Sıra: tablolar → A1 (birincil) → B1 → B5 → B2 → B4.
cd "$(dirname "$0")"
D2="Σw=m·kG|Σv(U) kare/asal-kuvvet"; DX="$D2|MX:"; DK="$D2|hücreler dengeli"
METIN="3,4,5,6,7,8,9,10,11,12,19,20,30,40"
log(){ echo "[$(date '+%H:%M:%S')] $*" | tee -a sonuc/kosu.log; }
sc(){ # sc <etiket> <robot> <mods> <disable> <null> <seed> <N> <tablo>
  ROBOT_MODS="$3" ROBOT_MINTR=1000 ROBOT_DISABLE="$4" ./$2 score $5 $6 $7 8 $8 puan/$1 2>/dev/null | tee -a sonuc/kosu.log; }
log "BAŞLADI"
# --- yeni M20 tabloları (denetçide yoktu) ---
ROBOT_MODS=19     ./robot2_m table M20 1920 2000000 8 tab/D_M20_m19_2M.bin      | tee -a sonuc/kosu.log
ROBOT_MODS=$METIN ./robot4_m table M20 1930 2000000 8 tab/T_M20_metin14_2M.bin  | tee -a sonuc/kosu.log
# --- A1: birincil (10 × 2e6 = 2e7 kitap / hücre) ---
for k in 1 2 3 4 5 6 7 8 9 10; do
  sU=$((19000+k)); sJ=$((19100+k)); sM=$((19200+k))
  sc A1a_U_$k   robot_m  19 "$D2" U   $sU 2000000 tab/U_2M.bin
  sc A1a_J10_$k robot_m  19 "$D2" J10 $sJ 2000000 tab/J10_2M.bin
  sc A1a_M20_$k robot_m  19 "$D2" M20 $sM 2000000 tab/M20_2M.bin
  sc A1b_U_$k   robot2_m 19 "$DX" U   $sU 2000000 tab/D_U_2M.bin
  sc A1b_J10_$k robot2_m 19 "$DX" J10 $sJ 2000000 tab/D_J10_2M.bin
  sc A1b_M20_$k robot2_m 19 "$DX" M20 $sM 2000000 tab/D_M20_m19_2M.bin
  sc A1c_U_$k   robot4_m 19 "$DX" U   $sU 2000000 tab/T_U_2M.bin
  sc A1c_J10_$k robot4_m 19 "$DX" J10 $sJ 2000000 tab/T_J10_2M.bin
  sc A1c_M20_$k robot4_m 19 "$DX" M20 $sM 2000000 tab/T_M20_metin14_2M.bin
  log "A1 parça $k bitti"
done
log "A1_BITTI"
# --- B1: metnin adlandırdığı 14 modül ---
sc B1a_U   robot_m  $METIN "$D2" U   19401 2000000 tab/U_2M.bin
sc B1a_J10 robot_m  $METIN "$D2" J10 19402 2000000 tab/J10_2M.bin
sc B1a_M20 robot_m  $METIN "$D2" M20 19403 2000000 tab/M20_2M.bin
sc B1c_U   robot4_m $METIN "$DX" U   19401 2000000 tab/T_U_2M.bin
sc B1c_J10 robot4_m $METIN "$DX" J10 19402 2000000 tab/T_J10_2M.bin
sc B1c_M20 robot4_m $METIN "$DX" M20 19403 2000000 tab/T_M20_metin14_2M.bin
log "B1_BITTI"
# --- B5: kör batarya (hücreler de kapalı), A1a biçiminde ---
sc B5_U   robot_m 19 "$DK" U   19801 2000000 tab/U_2M.bin
sc B5_J10 robot_m 19 "$DK" J10 19802 2000000 tab/J10_2M.bin
log "B5_BITTI"
# --- B2: büyük tablolar (m=19, 2e7 tablo kitabı) + 2e7 yeni kitap ---
ROBOT_MODS=19 ./robot4_m table U   1940 20000000 8 tab/T_U_m19_20M.bin   | tee -a sonuc/kosu.log
ROBOT_MODS=19 ./robot4_m table J10 1941 20000000 8 tab/T_J10_m19_20M.bin | tee -a sonuc/kosu.log
ROBOT_MODS=19 ./robot4_m table M20 1942 20000000 8 tab/T_M20_m19_20M.bin | tee -a sonuc/kosu.log
for k in 1 2 3 4 5 6 7 8 9 10; do
  sc B2_U_$k   robot4_m 19 "$DX" U   $((19500+k)) 2000000 tab/T_U_m19_20M.bin
  sc B2_J10_$k robot4_m 19 "$DX" J10 $((19600+k)) 2000000 tab/T_J10_m19_20M.bin
  sc B2_M20_$k robot4_m 19 "$DX" M20 $((19700+k)) 2000000 tab/T_M20_m19_20M.bin
done
log "B2_BITTI"
# --- B4: denetçinin atladığı M20'de tam belgeli arama (orijinal robot4, 646 mercek) ---
./robot4 table M20 1950 2000000 8 tab/T_M20_2M.bin | tee -a sonuc/kosu.log
ROBOT_MINTR=1000 ROBOT_DISABLE="$DX" ./robot4 score M20 1951 2000000 8 tab/T_M20_2M.bin puan/B4_M20_tam 2>/dev/null | tee -a sonuc/kosu.log
log "B4_BITTI"
log "HEPSI_BITTI"
