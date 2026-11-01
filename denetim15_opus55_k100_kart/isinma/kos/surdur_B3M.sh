#!/bin/bash
# surdur_B3M.sh — (B) 3.000.000 ısınma koşusunu sürdürür. İlk koşuda yarış kilidi bütün sıfırlarda orn_init'i sıraya soktuğu için
# (uzun ısınmada) tek çekirdekte yürüyordu; kilit yalnız L için olacak biçimde yeniden derlendi. Sonuçları etkilemez; biten bölütler atlanır.
set -u
cd ~/hrf_olcum; I=isinma; LOG=$I/sonuc/gunluk.txt; log(){ echo "[$(date -u +%FT%TZ)] $*" >> $LOG; }
gcc -O3 -std=gnu11 -w -DDMAX=64 -DKAPI_ESIK=0 -DKAPI_HUCRE=2 -DISINMA=3000000 -I $I/src -o $I/bin/kontrol_h_Gc_3000000 $I/src/kontrol_h.c -lm -lpthread || { log "derleme hatası"; exit 2; }
D=$I/sonuc/B_3000000
log "(B) 3000000 durdurulmuştu ($(ls $D | grep -c '^b.*txt$') bölüt bitmişti): kilit tüm sıfırlarda orn_init'i sıraya sokuyordu; kilit yalnız L için yeniden derlendi (sonuçları etkilemez), kalan bölütlerle sürüyor"
R="girdi/ref/M/ref_1111.ref girdi/ref/M/ref_2222.ref girdi/ref/M/ref_0444.ref girdi/ref/M/ref_0888.ref"
ESZAMANLI=12 nice -n 10 $I/bin/kontrol_h_Gc_3000000 M girdi/M/hedef.bin girdi/M/diller_Q.txt 1 4000 $D $R 2>> $D/hata.txt
log "(B) 3000000 bitti: $(cat $D/b*.txt | awk '$1=="D"{k[$2]+=$3; kb[$2]+=$4} END{for(x in k) printf "%s k=%d kB=%d ", x, k[x], kb[x]}')"
log "İSINMA BİTTİ"
