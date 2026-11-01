#!/bin/bash
# k100_akis.sh "<sıfırlar>" — K×100 eşit derinlik kalibrasyonu (ONKAYIT §2): hedef üretimi (M20 20.000, Mallows ve tekdüze 10.000),
# ilk 2.000 / 1.000 hedefin K×10 ile bayt bayt aynılık sınaması, Kol A dilleri (dil_b A; üç sıfır aynı anda, 3 iş parçacığı),
# kontrol düzey 1–5 kart sayacıyla (kos/kontrol_kart.py). Kaynaklar k10'un derlenmiş kaynaklarının kopyasıdır; kart_b kabul v3 ikilisidir.
set -u
H=~/hrf_olcum; K=$H/k100; mkdir -p $K/src $K/kos $K/sonuc $K/bin
log(){ echo "[$(date -u +%FT%TZ)] $*" >> $K/sonuc/kosu_gunlugu.txt; }
cp -p $H/k10/src/*.c $H/k10/src/*.h $H/k10/src/*.py $K/src/
CF="-O3 -std=gnu11 -w -DKAPI_ESIK=0 -DKAPI_HUCRE=2"
gcc $CF -o $K/src/hedef $K/src/hedef.c -lm && gcc $CF -o $K/src/dil_b $K/src/dil_b.c -lm -lpthread \
  && gcc $CF -DDMAX=64 -o $K/src/kontrol_b $K/src/kontrol_b.c -lm -lpthread && gcc $CF -DDMAX=64 -o $K/src/hedef_ayrinti_b $K/src/hedef_ayrinti_b.c -lm -lpthread \
  || { log "derleme hatası"; exit 2; }
cp -p $H/kart/bin/kart_b $K/bin/kart_b
( cd $K && sha256sum src/*.c src/*.h src/*.py kos/*.py kos/*.sh dogrulama/*.py bin/kart_b $H/kart/src/* > sonuc/kod_sha256.txt )
log "derlendi; kod özeti $(sha256sum $K/sonuc/kod_sha256.txt | cut -c1-16); kart_b $(sha256sum $K/bin/kart_b | cut -c1-16)"
export KAPI_REF=$H/girdi/ref
ref(){ echo $H/girdi/ref/$1/ref_1111.ref $H/girdi/ref/$1/ref_2222.ref $H/girdi/ref/$1/ref_0444.ref $H/girdi/ref/$1/ref_0888.ref; }
hazirla(){ S=$1; case $S in M) KK=20000; K0=2000;; *) KK=10000; K0=1000;; esac
  T=$K/tab/$S; mkdir -p $T/kA
  if [ ! -f $T/hedef.bin ]; then nice -n 10 $K/src/hedef $S $KK $T/hedef.bin || { log "hedef hatası $S"; return 2; }; chmod 444 $T/hedef.bin; fi
  cmp -n $(( (K0+1)*232 )) $T/hedef.bin $H/k10/tab/$S/hedef.bin || { log "HATA $S: ilk $K0 hedef K×10'unkilerle aynı değil"; return 2; }
  log "hedef $S: $(( $(stat -c %s $T/hedef.bin) / 232 - 1 )) plasebo; ilk $K0 K×10 ile bayt bayt aynı"
  if [ ! -f $T/diller_A.txt ]; then nice -n 10 $K/src/dil_b A $T/hedef.bin x $T/diller_A.txt $T/ayrinti_A.txt $(ref $S) || { log "dil_b hatası $S"; return 2; }
    chmod 444 $T/diller_A.txt $T/ayrinti_A.txt; fi
  head -$((K0+1)) $T/diller_A.txt | cmp -s - $H/k10/tab/$S/diller_A.txt || { log "HATA $S: ilk $K0 dil K×10'unkilerle aynı değil"; return 2; }
  log "dil $S A: $(wc -l < $T/diller_A.txt) satır, $(awk '$4>=0 && $5>0' $T/diller_A.txt | wc -l) dilli; ilk $K0 K×10 ile aynı"
  if [ ! -f $T/kA/diller.txt ]; then awk '$4>=0 && $5>0' $T/diller_A.txt > $T/kA/diller.txt; chmod 444 $T/kA/diller.txt
    nice -n 10 $K/src/hedef_ayrinti_b $T/hedef.bin $T/diller_A.txt $T/hedef_ayrinti_A.txt $(ref $S); fi; }
for S in $1; do hazirla $S & done; wait
for S in $1; do [ -f $K/tab/$S/kA/diller.txt ] || { log "HATA $S hazırlanamadı"; exit 2; }; done
( cd $K && sha256sum tab/*/hedef.bin tab/*/diller_A.txt > sonuc/diller_sha256.txt )
log "diller yazıldı: $(sha256sum $K/sonuc/diller_sha256.txt | cut -c1-16)…"
for S in $1; do for d in 1 2 3 4 5; do python3 -I $K/kos/kontrol_kart.py $S $d || { log "kontrol hatası $S $d"; exit 2; }; done; log "kontrol $S bitti"; done
log "AKIŞ BİTTİ"
