#!/bin/bash
# Denetim bandında geçen çıkınca bütün bandın tam birlikte-olasılığı (ONKAYIT §4.6). kullanım: kos/tam_bant.sh <kodeks> <O|P> <U|M|L|J> <seed> <havuz> <etiket> <parça>
cd "$(dirname "$0")/.."
kod=$1; kos=$2; nul=$3; seed=$4; d=$5; tag=$6; n=$7
files=$(ls $d/*.bin | sort)
for ((i=0;i<n;i++)); do
  BANT_TAMAMI=1 PARCA=$i/$n nice -n 15 ./src/tam_ortak $kod $kos $nul 1 $seed 50 20 2.0 0 2000 sonuc/o1/${tag}_bant sonuc/o1/$tag.puan.bin $files > sonuc/o1/${tag}_bant_parca$i.log 2>&1 &
done
wait
python3 -I src/tam_birlestir.py sonuc/o1/${tag}_bant $n && python3 -I src/tam_cozunurluk.py sonuc/o1/${tag}_bant.tam.txt > sonuc/o1/${tag}_bant_cozunurluk.txt
echo "[$(date -u +%FT%TZ)] $tag bütün bant bitti" >> sonuc/o1/kosu.log
