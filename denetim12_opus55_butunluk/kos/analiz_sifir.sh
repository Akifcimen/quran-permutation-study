#!/bin/bash
# analiz_sifir.sh — bir sıfır×kodeksin üretim çıktılarının standart analizi (sıra özetleri, rakipler, S0, sınıf-kısıtlı keşif).
# Kısa analizler kos/duraklat.sh altında koşar (4-süreç sınırı). Kullanım: kos/analiz_sifir.sh <sıfırkodeks> <N_T> <N_R>
cd "$(dirname "$0")/.."; s=$1; NT=$2; NR=$3; D=kos/duraklat.sh; kod=${s:1:3}
mkdir -p sonuc/sira sonuc/rakipler sonuc/s0 sonuc/sinif
$D python3 -I src/ozet.py $s > sonuc/sira/${s}_ozet.txt 2>&1
$D python3 -I src/bilgi_sira.py $s 40 > sonuc/sira/${s}_bilgi.txt 2>&1
$D python3 -I src/rakipler.py $s bilgi 20 > sonuc/rakipler/${s}_bilgi.txt 2>&1
$D python3 -I src/rakipler.py $s sayim 20 > sonuc/rakipler/${s}_sayim.txt 2>&1
$D ./src/sinif $kod tab/$s.tab T sonuc/sinif/${s}_T.txt tab/T_$s/b0*.aday
$D ./src/sinif $kod - R sonuc/sinif/${s}_R.txt tab/R_$s/b0*.aday
$D ./src/s0 $kod T sonuc/s0/${s}_T.txt tab/T_$s/b0*.aday
$D ./src/s0 $kod R sonuc/s0/${s}_R.txt tab/R_$s/b0*.aday
$D python3 -I src/s0_sinif_ozet.py $s $NT $NR > sonuc/s0/${s}_ozet.txt 2>&1
$D python3 -I src/s0_detay.py $s 11 > sonuc/s0/${s}_S0_11.txt 2>&1
echo "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] analiz $s bitti" >> sonuc/kosu_gunlugu.txt
