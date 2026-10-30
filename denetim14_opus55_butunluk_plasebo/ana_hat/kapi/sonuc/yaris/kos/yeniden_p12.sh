#!/bin/bash
# yeniden_p12.sh — 13. denetimin (plasebo12) Mallows kolunun yarış düzeltmeli kontrol.c ile yeniden sayımı.
# yeniden_L.sh bitince başlar. kontrol.c en çok 4 iş parçacığı kullanır; düzey 3 üç ayrı süreçte (3 × 4 = 12 iş parçacığı) koşar.
set -u
cd ~/hrf_olcum; Y=yaris; LOG=$Y/sonuc/gunluk.txt
log(){ echo "[$(date -u +%FT%TZ)] $*" >> $LOG; }
ref(){ echo girdi/ref/L/ref_1111.ref girdi/ref/L/ref_2222.ref girdi/ref/L/ref_0444.ref girdi/ref/L/ref_0888.ref; }
until grep -q "YENİDEN L BİTTİ" $LOG 2>/dev/null; do sleep 60; done
k(){ # $1 düzey dizini, $2 b1, $3 b2
  local D=$Y/p12yeni/$1; mkdir -p $D
  ESZAMANLI=4 nice -n 10 $Y/bin/kontrol_p12 L girdi/L/hedef.bin $Y/p12/L/$1/diller.txt $2 $3 $D $(ref) 2>> $D/hata.txt || log "p12 $1 $2–$3 hata"; }
log "p12 L başladı"
k k1 1 4 & k k2 5 22 & k k2 23 40 & wait
k k3 41 160 & k k3 161 280 & k k3 281 400 & wait
log "p12 L BİTTİ"
