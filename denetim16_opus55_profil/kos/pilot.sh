#!/bin/bash
# pilot.sh — geliştirme denemesi (ONKAYIT §8'de beyan edilir): K×100'ün görülmüş hedefleri, bütün olaylar, yalnız düzey 1 (bölüt 1–4)
# ve düzey 2 (bölüt 5–40). Amaç olay sayımlarının dağılımını ve model bilgisinin sırasını görmek; sonuçlar kararlarda kullanılmaz.
set -u; export CUDA_DEVICE_ORDER=PCI_BUS_ID
H=~/hrf_olcum; K=$H/profil; P=$K/pilot; mkdir -p $P/sonuc $P/bin $P/kos $P/dogrulama
cp -p $K/bin/kart_olay $P/bin/; cp -p $K/kos/olay_kos.py $P/kos/; cp -p $K/dogrulama/olay_say.py $P/dogrulama/
ref(){ echo $H/girdi/ref/$1/ref_1111.ref $H/girdi/ref/$1/ref_2222.ref $H/girdi/ref/$1/ref_0444.ref $H/girdi/ref/$1/ref_0888.ref; }
for S in M L U; do T=$P/tab/$S; mkdir -p $T; cp -p $H/k100/tab/$S/hedef.bin $T/; cp -p $H/k100/tab/$S/diller_A.txt $T/
  ( DIL_ASAL=19 nice -n 10 $K/bin/dil_b A $T/hedef.bin x $T/diller_A19.txt $T/ayrinti_A19.txt $(ref $S) && nice -n 10 $K/bin/olay_kur $T/hedef.bin $T/diller_A.txt $T/diller_A19.txt $T/olaylar.txt $(ref $S) 2> $T/olay_kur.err ) &
done; wait
for S in M L U; do for d in 1 2; do python3 -I $P/kos/olay_kos.py $S $d || exit 2; done; done
echo PILOT BITTI >> $P/sonuc/kosu_gunlugu.txt
