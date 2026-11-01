#!/bin/bash
# is_sina.sh — eşdeğerlik sınaması (EK_…_kart_tasarim.md §3): kart çıktıları, işlemci sayaçlarının iş istasyonunda zaten yazdığı
# bölüt dosyalarıyla karşılaştırılır (sure_s alanı dışında bayt bayt). Tek kart; host iş parçacığı bloklayan eşitlemeyle uyur.
set -u; export CUDA_DEVICE_ORDER=PCI_BUS_ID   # nvidia-smi sırası: 0 = RTX 3090, 1 = RTX 4090
H=~/hrf_olcum; K=$H/kart; G=${KART_GPU:-1}; O=$K/sina_kart$G; mkdir -p $O
ref(){ echo $H/girdi/ref/$1/ref_1111.ref $H/girdi/ref/$1/ref_2222.ref $H/girdi/ref/$1/ref_0444.ref $H/girdi/ref/$1/ref_0888.ref; }
karsilastir(){ # <kart dizini> <işlemci dizini> <ilk> <son> <kip>
  local fark=0 n=0
  for b in $(seq $3 $4); do f=$(printf b%04d $b); n=$((n+1))
    diff <(sed 's/ sure_s [0-9.]*//' $1/$f.txt) <(sed 's/ sure_s [0-9.]*//' $2/$f.txt) >/dev/null || { echo "  FARK $f.txt"; fark=$((fark+1)); }
    for e in q s; do if [ -e $2/$f.$e ] || [ -e $1/$f.$e ]; then cmp -s $1/$f.$e $2/$f.$e || { echo "  FARK $f.$e"; fark=$((fark+1)); }; fi; done; done
  echo "  $n bölüt, fark $fark"; }
kos(){ # <ad> <kip b|h> <S> <hedef> <diller> <ilk> <son> <işlemci dizini>
  local D=$O/$1; rm -rf $D; mkdir -p $D; local t0=$(date +%s.%N)
  CUDA_VISIBLE_DEVICES=$G KART_PARTI=${KART_PARTI:-4096} nice -n 10 $K/bin/kart_$2 $3 $4 $5 $6 $7 $D $(ref $3) 2> $D/hata.txt || echo "  HATA rc=$?"
  local t1=$(date +%s.%N); echo "$1: $(echo "$t1 - $t0" | bc) sn"; karsilastir $D $8 $6 $7; }
T=$H/k10/tab
kos M_b_d1 b M $T/M/hedef.bin $T/M/kA/d1_0/diller.txt 1 4 $T/M/kA/d1_0
kos M_b_d4 b M $T/M/hedef.bin $T/M/kA/d4_0/diller.txt 401 424 $T/M/kA/d4_0
kos L_b_d2 b L $T/L/hedef.bin $T/L/kA/d2_0/diller.txt 5 28 $T/L/kA/d2_0
kos U_b_d2 b U $T/U/hedef.bin $T/U/kA/d2_0/diller.txt 5 28 $T/U/kA/d2_0
kos M_h_Gc h M $H/girdi/M/hedef.bin $H/girdi/M/diller_Q.txt 1 24 $H/kapi/kuran/Gc/M
kos U_h_Gc h U $H/girdi/U/hedef.bin $H/girdi/U/diller_Q.txt 1 24 $H/kapi/kuran/Gc/U
kos L_h_Gc h L $H/girdi/L/hedef.bin $H/girdi/L/diller_Q.txt 1 24 $H/yaris/kuran/Gc/L
