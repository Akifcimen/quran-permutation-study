#!/bin/bash
# k10_akis.sh "<sıfırlar>" — K×10 plasebo kalibrasyonu (ONKAYIT §2): kapı Gc ile hedef üretimi (M20 2000, Mallows ve tekdüze 1000),
# ilk 200/100 hedefin kapı çalışmasıyla bayt bayt aynılık sınaması, Kol A dilleri, kontrol düzey 1–3 (kos/kontrol_b.py; derinlik kuralı aynı).
# Kaynak: kapı çalışmasının donmuş kaynakları; kontrol_b.c yarış düzeltmeli, kilit yalnız L için (yama/); hedef.c plasebo14'ün üreticisi (yama/).
set -u
H=~/hrf_olcum; K=$H/k10; mkdir -p $K/src $K/kos $K/sonuc
log(){ echo "[$(date -u +%FT%TZ)] $*" >> $K/sonuc/kosu_gunlugu.txt; }
cp -p $H/kapi/kaynak/src/*.c $H/kapi/kaynak/src/*.h $H/kapi/kaynak/src/*.py $K/src/
cp -p $K/yama/kontrol_b.c $K/src/kontrol_b.c; cp -p $H/kapi/kaynak/kos/kontrol_b.py $K/kos/
cp -p $K/yama/hedef.c $K/src/hedef.c
CF="-O3 -std=gnu11 -w -DKAPI_ESIK=0 -DKAPI_HUCRE=2"
gcc $CF -o $K/src/hedef $K/src/hedef.c -lm && gcc $CF -o $K/src/dil_b $K/src/dil_b.c -lm -lpthread \
  && gcc $CF -DDMAX=64 -o $K/src/kontrol_b $K/src/kontrol_b.c -lm -lpthread && gcc $CF -DDMAX=64 -o $K/src/hedef_ayrinti_b $K/src/hedef_ayrinti_b.c -lm -lpthread \
  || { log "derleme hatası"; exit 2; }
( cd $K && sha256sum src/*.c src/*.h src/*.py kos/*.py > sonuc/kod_sha256.txt )
log "derlendi (Gc; kontrol_b yarış düzeltmeli); kod özeti $(sha256sum $K/sonuc/kod_sha256.txt | cut -c1-16)"
export KAPI_REF=$H/girdi/ref
ref(){ echo $H/girdi/ref/$1/ref_1111.ref $H/girdi/ref/$1/ref_2222.ref $H/girdi/ref/$1/ref_0444.ref $H/girdi/ref/$1/ref_0888.ref; }
for S in $1; do case $S in M) KK=2000; K0=200;; *) KK=1000; K0=100;; esac
  T=$K/tab/$S; mkdir -p $T/kA
  if [ ! -f $T/hedef.bin ]; then $K/src/hedef $S $KK $T/hedef.bin || { log "hedef hatası $S"; exit 2; }; chmod 444 $T/hedef.bin; fi
  cmp -n $(( (K0+1)*232 )) $T/hedef.bin $H/girdi/$S/hedef.bin || { log "HATA $S: ilk $K0 hedef kapı çalışmasınınkilerle aynı değil"; exit 2; }
  log "hedef $S: $(( $(stat -c %s $T/hedef.bin) / 232 - 1 )) plasebo; ilk $K0 kapı çalışmasıyla bayt bayt aynı"
  if [ ! -f $T/diller_A.txt ]; then
    $K/src/dil_b A $T/hedef.bin x $T/diller_A.txt $T/ayrinti_A.txt $(ref $S) || { log "dil_b hatası $S"; exit 2; }
    chmod 444 $T/diller_A.txt $T/ayrinti_A.txt; fi
  log "dil $S A: $(wc -l < $T/diller_A.txt) satır, $(awk '$4>=0 && $5>0' $T/diller_A.txt | wc -l) dilli"
  if [ ! -f $T/kA/diller.txt ]; then awk '$4>=0 && $5>0' $T/diller_A.txt > $T/kA/diller.txt; chmod 444 $T/kA/diller.txt
    $K/src/hedef_ayrinti_b $T/hedef.bin $T/diller_A.txt $T/hedef_ayrinti_A.txt $(ref $S); fi
done
( cd $K && sha256sum tab/*/hedef.bin tab/*/diller_A.txt tab/*/ayrinti_A.txt > sonuc/diller_sha256.txt )
log "diller yazıldı: $(sha256sum $K/sonuc/diller_sha256.txt | cut -c1-16)…"
for S in $1; do for d in 1 2 3; do ESZAMANLI=12 python3 -I $K/kos/kontrol_b.py $S A $d || { log "kontrol hatası $S $d"; exit 2; }; done; log "kontrol $S A bitti"; done
log "AKIŞ BİTTİ"
