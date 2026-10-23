#!/bin/bash
# Ö1 puanlaması (ONKAYIT §4.4–4.5, §4.8): bir havuz bütünüyle bir kez. kullanım: kos/puanla.sh <kodeks> <O|P> <havuz_dizini> <etiket>
cd "$(dirname "$0")/.."
kod=$1; kos=$2; d=$3; tag=$4
files=$(ls $d/*.bin | sort)
nice -n 10 ./src/kosullu_puan $kod $kos 1 sonuc/o1/${tag} $files > sonuc/o1/${tag}.log 2>&1
nice -n 10 ./src/kosullu_puan $kod $kos 0 sonuc/o1/${tag}_teraziacik $files > sonuc/o1/${tag}_teraziacik.log 2>&1
echo "[$(date -u +%FT%TZ)] $tag puanlandı ($(echo $files | wc -w) dosya)" >> sonuc/o1/kosu.log
