#!/bin/bash
# Ö0(a) — dokuzuncu incelemenin deneyinin yeniden üretimi (ONKAYIT §3.1). Robotun kendi ikilisiyle tablo + puan; bizim tekrar9 ile
# aynı kitaplarda özgün / tek-düzeltme / tekrarsız sayım. Büyük ara dosyalar (tablo, f32) kullanıldıktan sonra silinir (tohumla yeniden üretilir).
cd "$(dirname "$0")/.."
B129="7,286,200,176,120,165,206,75,129,109,123,111,43,52,99,128,111,110,98,135,112,78,118,64,77,227,93,88,69,60,34,30,73,54,45,83,182,88,75,85,54,53,89,59,37,35,38,29,18,45,60,49,62,55,78,96,29,22,24,13,14,11,11,18,12,12,30,52,52,44,28,28,20,56,40,31,50,40,46,42,29,19,36,25,22,17,19,26,30,20,15,21,11,8,8,19,5,8,8,11,11,8,3,9,5,4,7,3,6,3,5,4,5,6"
DK="Σw=m·kG|Σv(U) kare/asal-kuvvet|MX:"
for kod in 129 127; do for r in 1 2 3; do for nul in U J10 M20; do
  case $nul in U) o=1;; J10) o=2;; M20) o=3;; esac
  ek=0; [ $kod = 127 ] && ek=50; ts=$((9100*r+o+ek)); es=$((ts+1000)); tag=${kod}_${nul}_${r}
  [ -f sonuc/t9/${tag}_kapali.txt ] && [ -f sonuc/t9/${tag}_acik.txt ] && continue
  if [ $kod = 129 ]; then export ROBOT_BASE=$B129; else unset ROBOT_BASE; fi
  ROBOT_MODS=19 nice -n 10 ./ref/robot4_m table $nul $ts 2000000 8 tab/t9/tab_$tag.bin > /dev/null 2>&1
  ROBOT_MODS=19 ROBOT_MINTR=1000 ROBOT_DISABLE="$DK|kitap:" nice -n 10 ./ref/robot4_m score $nul $es 2000000 8 tab/t9/tab_$tag.bin tab/t9/sk_$tag > /dev/null 2>&1
  nice -n 10 ./src/tekrar9 $kod $nul $es 2000000 8 tab/t9/tab_$tag.bin tab/t9/sk_${tag}_sifir.f32 1 sonuc/t9/${tag}_kapali
  cp tab/t9/sk_${tag}_kuran.txt sonuc/t9/${tag}_kapali_robot_kuran.txt; rm -f tab/t9/sk_${tag}_sifir.f32
  ROBOT_MODS=19 ROBOT_MINTR=1000 ROBOT_DISABLE="$DK" nice -n 10 ./ref/robot4_m score $nul $es 2000000 8 tab/t9/tab_$tag.bin tab/t9/sa_$tag > /dev/null 2>&1
  nice -n 10 ./src/tekrar9 $kod $nul $es 2000000 8 tab/t9/tab_$tag.bin tab/t9/sa_${tag}_sifir.f32 0 sonuc/t9/${tag}_acik
  cp tab/t9/sa_${tag}_kuran.txt sonuc/t9/${tag}_acik_robot_kuran.txt; rm -f tab/t9/sa_${tag}_sifir.f32 tab/t9/tab_$tag.bin
  echo "[$(date -u +%FT%TZ)] $tag bitti" >> sonuc/t9/kosu.log
done; done; done
echo "[$(date -u +%FT%TZ)] HEPSI_BITTI" >> sonuc/t9/kosu.log
