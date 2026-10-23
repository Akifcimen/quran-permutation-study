#!/bin/bash
# Tam birlikte-olasılık (ONKAYIT §4.6). kullanım: kos/tam.sh <kodeks> <O|P> <U|M|L|J> <seed> <havuz_dizini> <etiket> <parça_sayısı>
cd "$(dirname "$0")/.."
kod=$1; kos=$2; nul=$3; seed=$4; d=$5; tag=$6; n=$7
files=$(ls $d/*.bin | sort)
for ((i=0;i<n;i++)); do
  PARCA=$i/$n nice -n 15 ./src/tam_ortak $kod $kos $nul 1 $seed 50 20 2.0 500 2000 sonuc/o1/$tag sonuc/o1/$tag.puan.bin $files > sonuc/o1/${tag}_tam_parca$i.log 2>&1 &
done
wait
python3 -I src/tam_birlestir.py sonuc/o1/$tag $n && python3 -I src/tam_analiz.py sonuc/o1/$tag.tam.txt > sonuc/o1/${tag}_tam_analiz.txt
echo "[$(date -u +%FT%TZ)] $tag tam birlikte-olasılık bitti" >> sonuc/o1/kosu.log
