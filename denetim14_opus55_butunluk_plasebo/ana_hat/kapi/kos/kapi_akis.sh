#!/bin/bash
# kapi_akis.sh <ad> <KAPI_ESIK> <KAPI_HUCRE> <sıfırlar> [ö0] — kapı çalışması (ONKAYIT §3): plasebo14 Kol A akışını verilen kapıyla yürütür.
#   1. derleme (gcc; kapı -D ile), 2. Kol A dilleri (dil_b A; hedefler plasebo14/plasebo12 ile aynı), salt okunur + SHA-256,
#   3. kontrol kümesi (dilsiz hedefler hariç) ve hedef ayrıntıları, 4. kontrol geçişi düzey 1–3 (kos/kontrol_b.py; derinlik kuralı aynı).
#   "ö0" verilirse yalnız 1–2 ve düzey 1 koşar (eşdeğerlik sınaması).
# Ağaç: ~/hrf_olcum/kapi/<ad>/{src,kos,tab/<S>,sonuc}. Referans tabloları ~/hrf_olcum/girdi/ref/<S>/.
set -u
AD=$1; KE=$2; KH=$3; SIFIRLAR=$4; KIP=${5:-tam}
H=~/hrf_olcum; K=$H/kapi/$AD; KAYNAK=$H/kapi/kaynak
mkdir -p $K/src $K/kos $K/sonuc
log(){ echo "[$(date -u +%FT%TZ)] $*" >> $K/sonuc/kosu_gunlugu.txt; }
cp -p $KAYNAK/src/*.c $KAYNAK/src/*.h $KAYNAK/src/*.py $K/src/; cp -p $KAYNAK/kos/kontrol_b.py $K/kos/
CF="-O3 -std=gnu11 -w -DKAPI_ESIK=$KE -DKAPI_HUCRE=$KH"
gcc $CF -o $K/src/dil_b $K/src/dil_b.c -lm -lpthread && gcc $CF -DDMAX=64 -o $K/src/kontrol_b $K/src/kontrol_b.c -lm -lpthread \
  && gcc $CF -DDMAX=64 -o $K/src/hedef_ayrinti_b $K/src/hedef_ayrinti_b.c -lm -lpthread && gcc $CF -DDMAX=64 -o $K/src/kontrol_h $K/src/kontrol_h.c -lm -lpthread \
  || { log "derleme hatası"; exit 2; }
log "derlendi: KAPI_ESIK=$KE KAPI_HUCRE=$KH ($AD, $KIP)"
export KAPI_REF=$H/girdi/ref
ref(){ echo $H/girdi/ref/$1/ref_1111.ref $H/girdi/ref/$1/ref_2222.ref $H/girdi/ref/$1/ref_0444.ref $H/girdi/ref/$1/ref_0888.ref; }
for S in $SIFIRLAR; do mkdir -p $K/tab/$S/kA; cp -p $H/girdi/$S/hedef.bin $K/tab/$S/
  if [ ! -f $K/tab/$S/diller_A.txt ]; then
    $K/src/dil_b A $K/tab/$S/hedef.bin x $K/tab/$S/diller_A.txt $K/tab/$S/ayrinti_A.txt $(ref $S) || { log "dil_b hatası $S"; exit 2; }
    chmod 444 $K/tab/$S/diller_A.txt $K/tab/$S/ayrinti_A.txt
    log "dil $S A: $(wc -l < $K/tab/$S/diller_A.txt) satır, $(awk '$4>=0 && $5>0' $K/tab/$S/diller_A.txt | wc -l) dilli"
  fi
  if [ ! -f $K/tab/$S/kA/diller.txt ]; then awk '$4>=0 && $5>0' $K/tab/$S/diller_A.txt > $K/tab/$S/kA/diller.txt; chmod 444 $K/tab/$S/kA/diller.txt
    $K/src/hedef_ayrinti_b $K/tab/$S/hedef.bin $K/tab/$S/diller_A.txt $K/tab/$S/hedef_ayrinti_A.txt $(ref $S); fi
done
( cd $K && sha256sum tab/*/diller_A.txt tab/*/ayrinti_A.txt > sonuc/diller_sha256.txt )
log "diller yazıldı: $(sha256sum $K/sonuc/diller_sha256.txt | cut -c1-16)…"
if [ "$KIP" = "ö0" ]; then python3 -I $K/kos/kontrol_b.py M A 1 || { log "kontrol hatası"; exit 2; }; log "Ö0 bitti"; exit 0; fi
for S in $SIFIRLAR; do for d in 1 2 3; do python3 -I $K/kos/kontrol_b.py $S A $d || { log "kontrol hatası $S $d"; exit 2; }; done; log "kontrol $S A bitti"; done
log "AKIŞ BİTTİ"
