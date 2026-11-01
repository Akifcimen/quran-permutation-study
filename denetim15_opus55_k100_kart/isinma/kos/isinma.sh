#!/bin/bash
# isinma.sh — M20 ısınma tanısı (ONKAYIT §2): derleme, eşdeğerlik sınaması, (A) bölüt içi kayma, (B) ısınma uzunluğunun sayımlara etkisi.
# workstation ~/hrf_olcum'da çalışır; çıktılar isinma/sonuc/. En çok 12 iş parçacığı, nice 10.
set -u
cd ~/hrf_olcum; I=isinma; LOG=$I/sonuc/gunluk.txt; mkdir -p $I/bin $I/sonuc
log(){ echo "[$(date -u +%FT%TZ)] $*" >> $LOG; }
R="girdi/ref/M/ref_1111.ref girdi/ref/M/ref_2222.ref girdi/ref/M/ref_0444.ref girdi/ref/M/ref_0888.ref"
for N in 30000 300000 3000000; do
  gcc -O3 -std=gnu11 -w -DDMAX=64 -DKAPI_ESIK=0 -DKAPI_HUCRE=2 -DISINMA=$N -I $I/src -o $I/bin/kontrol_h_Gc_$N $I/src/kontrol_h.c -lm -lpthread || { log "derleme hatası kontrol_h $N"; exit 2; }
  gcc -O3 -std=gnu11 -w -DISINMA=$N -I $I/src -o $I/bin/isinma_S_$N $I/src/isinma_S.c -lm -lpthread || { log "derleme hatası isinma_S $N"; exit 2; }
done
log "derlendi: $(sha256sum $I/src/kutuphane.h $I/src/kontrol_h.c $I/src/isinma_S.c | cut -c1-16 | tr '\n' ' ')"
# eşdeğerlik: ISINMA 30000, M20 bölüt 1–24, kapı çalışmasının özgün Kur'an tarafı Gc çıktısıyla (süre alanı dışında) birebir
D=$I/sonuc/esdeger; mkdir -p $D
ESZAMANLI=12 nice -n 10 $I/bin/kontrol_h_Gc_30000 M girdi/M/hedef.bin girdi/M/diller_Q.txt 1 24 $D $R 2>> $D/hata.txt
f=0; for b in $(seq -f %04g 1 24); do cmp -s <(sed 's/ sure_s [0-9.]*//' $D/b$b.txt) <(sed 's/ sure_s [0-9.]*//' kapi/kuran/Gc/M/b$b.txt) || f=$((f+1)); done
log "eşdeğerlik (ISINMA 30000, bölüt 1–24): $f farklı"; [ $f -eq 0 ] || { log "EŞDEĞERLİK BAŞARISIZ, durdu"; exit 2; }
# (A) bölüt içi kayma
for N in 30000 3000000; do log "(A) isinma_S $N bölüt 1–2000 başladı"
  ESZAMANLI=12 nice -n 10 $I/bin/isinma_S_$N 1 2000 > $I/sonuc/A_$N.txt 2>> $I/sonuc/A_hata.txt; log "(A) isinma_S $N bitti: $(wc -l < $I/sonuc/A_$N.txt) bölüt"; done
# (B) ısınma uzunluğu: 300000 ve 3000000 ile M20 bölüt 1–4000 (30000 = kapi/kuran/Gc/M)
for N in 300000 3000000; do D=$I/sonuc/B_$N; mkdir -p $D; log "(B) kontrol_h_Gc $N bölüt 1–4000 başladı"
  ESZAMANLI=12 nice -n 10 $I/bin/kontrol_h_Gc_$N M girdi/M/hedef.bin girdi/M/diller_Q.txt 1 4000 $D $R 2>> $D/hata.txt
  log "(B) $N bitti: $(cat $D/b*.txt | awk '$1=="D"{k[$2]+=$3; kb[$2]+=$4} END{for(x in k) printf "%s k=%d kB=%d ", x, k[x], kb[x]}')"; done
log "İSINMA BİTTİ"
