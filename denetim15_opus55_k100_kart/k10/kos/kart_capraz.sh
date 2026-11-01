#!/bin/bash
# kart_capraz.sh — kartın yazdığı derinleştirme bölütlerinden seçilenleri işlemci sayacı (k10/src/kontrol_b, tek iş parçacığı)
# ayrı bir dizinde yeniden hesaplar ve karşılaştırır (sure_s dışında bayt bayt). Seçim: her sıfırdan ilk, orta, son bölüt + M'de kartın yazdığı biri.
set -u
H=~/hrf_olcum; K=$H/k10; O=$H/kart/capraz; rm -rf $O; mkdir -p $O
ref(){ echo $H/girdi/ref/$1/ref_1111.ref $H/girdi/ref/$1/ref_2222.ref $H/girdi/ref/$1/ref_0444.ref $H/girdi/ref/$1/ref_0888.ref; }
Mk=$(cd $K/tab/M/kA/d4_0 && for f in $(ls -tr b*.txt | tail -400); do grep -q . $f && { echo ${f:1:4}; break; }; done)
is(){ S=$1; b=$2; d=$O/${S}_$b; mkdir -p $d; ESZAMANLI=1 nice -n 10 $K/src/kontrol_b $S $K/tab/$S/hedef.bin $K/tab/$S/kA/d4_0/diller.txt $((10#$b)) $((10#$b)) $d $(ref $S) 2> $d/hata.txt; }
for x in "M $Mk" "L 0401" "L 2200" "L 4000" "U 0401" "U 2200" "U 4000"; do is $x & done; wait
fark=0
for x in "M $Mk" "L 0401" "L 2200" "L 4000" "U 0401" "U 2200" "U 4000"; do set -- $x; d=$O/${1}_$2; k=$K/tab/$1/kA/d4_0
  diff <(sed 's/ sure_s [0-9.]*//' $d/b$2.txt) <(sed 's/ sure_s [0-9.]*//' $k/b$2.txt) > /dev/null && cmp -s $d/b$2.q $k/b$2.q && echo "$1 b$2 aynı ($(grep '^D' $k/b$2.txt | tr '\n' ' '))" || { echo "$1 b$2 FARK"; fark=1; }; done
echo "çapraz sınama fark $fark"
