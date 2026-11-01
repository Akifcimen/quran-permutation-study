#!/bin/bash
# k100_dogrula.sh — ONKAYIT §5.4 (Kur'an'ın bölüt 1–40.000 sayımı kapı Gc ile bölüt bölüt aynı mı) ve §5.5 (kartın yazdığı 10 bölüt
# işlemci sayacıyla, tek iş parçacığı, ayrı dizinde yeniden hesaplanır; sure_s dışında bayt bayt karşılaştırılır).
set -u
H=~/hrf_olcum; K=$H/k100
ref(){ echo $H/girdi/ref/$1/ref_1111.ref $H/girdi/ref/$1/ref_2222.ref $H/girdi/ref/$1/ref_0444.ref $H/girdi/ref/$1/ref_0888.ref; }
for x in "M kapi/kuran/Gc/M" "L yaris/kuran/Gc/L" "U kapi/kuran/Gc/U"; do set -- $x; f=0
  for b in $(seq 1 40000); do n=$(printf b%04d.txt $b); a=$(grep -h "^D A000 " $K/tab/$1/kA/d*_*/$n 2>/dev/null); c=$(grep -h "^D A000 " $H/$2/$n); [ "$a" = "$c" ] || { f=$((f+1)); [ $f -le 3 ] && echo "  FARK $1 $b [$a] [$c]"; }; done
  echo "§5.4 $1: A000 bölüt 1–40000, fark $f"; done
O=$H/kart/capraz_k100; rm -rf $O; mkdir -p $O
SEC="M d1_1 3|M d2_0 17|M d5_0 17113|M d5_0 39998|L d3_0 222|L d5_0 4004|L d5_0 25003|U d4_0 1919|U d5_0 33337|U d5_0 40000"
IFS='|'; for x in $SEC; do IFS=' '; set -- $x; d=$O/${1}_${2}_$3; mkdir -p $d
  ESZAMANLI=1 nice -n 10 $K/src/kontrol_b $1 $K/tab/$1/hedef.bin $K/tab/$1/kA/$2/diller.txt $3 $3 $d $(ref $1) 2> $d/hata.txt & IFS='|'; done; IFS=' '; wait
f=0; IFS='|'; for x in $SEC; do IFS=' '; set -- $x; d=$O/${1}_${2}_$3; k=$K/tab/$1/kA/$2; n=$(printf b%04d $3)
  if diff <(sed 's/ sure_s [0-9.]*//' $d/$n.txt) <(sed 's/ sure_s [0-9.]*//' $k/$n.txt) > /dev/null && cmp -s $d/$n.q $k/$n.q; then echo "§5.5 $1 $2 $n aynı ($(grep -c '^D' $k/$n.txt) dil, kart $( [ $(( $3 % 5 )) -le 2 ] && echo 4090 || echo 3090 ))"; else echo "§5.5 $1 $2 $n FARK"; f=1; fi; IFS='|'; done; IFS=' '
echo "§5.5 fark $f"
