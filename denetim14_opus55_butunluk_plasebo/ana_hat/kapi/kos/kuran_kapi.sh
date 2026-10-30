#!/bin/bash
# kuran_kapi.sh <kapı> <ESIK> <HUCRE> <son_bolut> — kapı çalışması ön-kaydı §6: Kur an in dilleri (girdi/<S>/diller_Q.txt) kontrol_h ile
# verilen kapıyla bölüt 1..son sayılır (M → U → L; 12 iş parçacığı, nice 10). Var olan bölütler atlanır (sürdürülebilir).
set -u
cd ~/hrf_olcum
G=$1; KE=$2; KH=$3; SON=$4; B=kapi/kuran/bin; mkdir -p $B
[ -x $B/kontrol_h_$G ] || gcc -O3 -std=gnu11 -w -DDMAX=64 -DKAPI_ESIK=$KE -DKAPI_HUCRE=$KH -I kapi/kaynak/src -o $B/kontrol_h_$G kapi/kaynak/src/kontrol_h.c -lm -lpthread || exit 2
L=kapi/kuran/gunluk.txt; log(){ echo "[$(date -u +%FT%TZ)] $*" >> $L; }
for S in M U L; do D=kapi/kuran/$G/$S; mkdir -p $D; R="girdi/ref/$S/ref_1111.ref girdi/ref/$S/ref_2222.ref girdi/ref/$S/ref_0444.ref girdi/ref/$S/ref_0888.ref"
  ESZAMANLI=12 nice -n 10 $B/kontrol_h_$G $S girdi/$S/hedef.bin girdi/$S/diller_Q.txt 1 $SON $D $R 2>> $D/hata.txt || { log "$G $S hata"; exit 2; }
  log "$G $S 1–$SON bitti: $(cat $D/b*.txt | awk "\$1==\"D\"{k[\$2]+=\$3; kb[\$2]+=\$4} END{for(x in k) printf \"%s k=%d kB=%d \", x, k[x], kb[x]}") | eşik: $(grep "^D A000" $D/esikler.txt | cut -d" " -f4)"
done
