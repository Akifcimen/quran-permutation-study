#!/bin/bash
# profil_akis.sh "<sıfırlar>" — profil çalışması (ONKAYIT §2–§4): taze hedefler, Kol A dilleri (bütün asallar ve yalnız 19), olaylar,
# kart_olay ile düzey 1–4. İkililer kos/is_derle.sh ile derlenmiş, kabul sınamasından geçmiş olanlardır (bin/ikili_sha256.txt).
set -u; export CUDA_DEVICE_ORDER=PCI_BUS_ID
H=~/hrf_olcum; K=$H/profil; LOG=$K/sonuc/kosu_gunlugu.txt; log(){ echo "[$(date -u +%FT%TZ)] $*" >> $LOG; }
ref(){ echo $H/girdi/ref/$1/ref_1111.ref $H/girdi/ref/$1/ref_2222.ref $H/girdi/ref/$1/ref_0444.ref $H/girdi/ref/$1/ref_0888.ref; }
( cd $K && sha256sum ONKAYIT.md src/* kos/* dogrulama/* bin/kart_olay bin/kart_olay_emul bin/olay_ozgun bin/olay_kur bin/dil_b bin/hedef > sonuc/kod_sha256.txt )
log "başladı; ön-kayıt $(sha256sum $K/ONKAYIT.md | cut -c1-16); kod özeti $(sha256sum $K/sonuc/kod_sha256.txt | cut -c1-16)"
hazirla(){ S=$1; case $S in M) KK=40000; K0=20000;; *) KK=20000; K0=10000;; esac
  T=$K/tab/$S; mkdir -p $T
  nice -n 10 $K/bin/hedef $S $KK $T/hedef_tam.bin || { log "hedef hatası $S"; return 2; }
  cmp -n $(( (K0+1)*232 )) $T/hedef_tam.bin $H/k100/tab/$S/hedef.bin || { log "HATA $S: ilk $K0 hedef K×100 ile aynı değil"; return 2; }
  python3 -I $K/src/taze_hedef.py $T/hedef_tam.bin $K0 $T/hedef.bin $T/harita.txt >> $LOG; chmod 444 $T/hedef.bin; rm -f $T/hedef_tam.bin
  log "hedef $S: ilk $K0 K×100 ile bayt bayt aynı; taze $((KK-K0)) hedef (h = $((K0+1))…$KK)"
  nice -n 10 $K/bin/dil_b A $T/hedef.bin x $T/diller_A.txt $T/ayrinti_A.txt $(ref $S) || { log "dil_b hatası $S"; return 2; }
  DIL_ASAL=19 nice -n 10 $K/bin/dil_b A $T/hedef.bin x $T/diller_A19.txt $T/ayrinti_A19.txt $(ref $S) || { log "dil_b 19 hatası $S"; return 2; }
  nice -n 10 $K/bin/olay_kur $T/hedef.bin $T/diller_A.txt $T/diller_A19.txt $T/olaylar.txt $(ref $S) 2> $T/olay_kur.err || { log "olay_kur hatası $S"; return 2; }
  chmod 444 $T/diller_A.txt $T/diller_A19.txt $T/olaylar.txt
  log "dil ve olay $S: $(awk '$4>=0' $T/diller_A.txt | wc -l) / $(awk '$4>=0' $T/diller_A19.txt | wc -l) dilli (bütün asallar / 19); $(cat $T/olay_kur.err)"; }
for S in $1; do hazirla $S & done; wait
for S in $1; do [ -f $K/tab/$S/olaylar.txt ] || { log "HATA $S hazırlanamadı"; exit 2; }; done
( cd $K && sha256sum tab/*/hedef.bin tab/*/diller_A.txt tab/*/diller_A19.txt tab/*/olaylar.txt > sonuc/girdi_sha256.txt )
log "girdiler: $(sha256sum $K/sonuc/girdi_sha256.txt | cut -c1-16)"
for S in $1; do for d in 1 2 3 4; do python3 -I $K/kos/olay_kos.py $S $d || { log "olay hatası $S $d"; exit 2; }; done; log "olay $S bitti"; done
log "AKIŞ BİTTİ"
