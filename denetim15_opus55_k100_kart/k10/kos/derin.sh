#!/bin/bash
# derin.sh — ön-kayıt §3 ikincil derinleştirme: birincil kuralda R'ye giren plasebolar düzey 4'e (bölüt 401–4000, 10⁹ kontrol) indirilir.
# Diller kA/diller.txt'ten aynen alınır; sayaç düzey 1–3'tekinin aynısı (Gc, yarış düzeltmeli, kilit yalnız L). Sıra M → L → U, 12 iş parçacığı (ESZAMANLI ile değiştirilebilir; 31 Eki 20:30Z: kart sınaması için 11).
set -u
H=~/hrf_olcum; K=$H/k10; LOG=$K/sonuc/kosu_gunlugu.txt; log(){ echo "[$(date -u +%FT%TZ)] $*" >> $LOG; }
ref(){ echo $H/girdi/ref/$1/ref_1111.ref $H/girdi/ref/$1/ref_2222.ref $H/girdi/ref/$1/ref_0444.ref $H/girdi/ref/$1/ref_0888.ref; }
declare -A RL=( [M]="A535 A1645 A690" [L]="A142 A786 A777 A891" [U]="A088 A358 A593 A925 A315 A491 A398" )
for S in M L U; do D=$K/tab/$S/kA/d4_0; mkdir -p $D
  [ -f $D/diller.txt ] || { for a in ${RL[$S]}; do grep "^$a " $K/tab/$S/kA/diller.txt; done > $D/diller.txt; chmod 444 $D/diller.txt; }
  log "derinleştirme $S düzey 4 (bölüt 401–4000) başladı: $(wc -l < $D/diller.txt) dil: ${RL[$S]}"
  ESZAMANLI=${ESZAMANLI:-12} nice -n 10 $K/src/kontrol_b $S $K/tab/$S/hedef.bin $D/diller.txt 401 4000 $D $(ref $S) 2>> $D/hata.txt || { log "derinleştirme $S hata"; exit 2; }
  log "derinleştirme $S bitti: $(ls $D | grep -c '^b.*txt$') bölüt"
done
log "DERİNLEŞTİRME BİTTİ"
