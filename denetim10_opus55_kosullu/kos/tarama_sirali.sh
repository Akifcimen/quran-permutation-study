#!/bin/bash
# Ö0(b) için ek tarama kaynağı: sıralı sıfırlardan O-kitaplar (yalnız olgu birlikte-görülmeleri; puan hesaplanmaz).
cd "$(dirname "$0")/.."
nice -n 8 ./src/kosullu_uret 129 M O 9101 5000 tab/tarama/M_O_9101.bin &
nice -n 8 ./src/kosullu_uret 129 L O 9201 1500 tab/tarama/L_O_9201.bin &
wait
for s in M_O_9101 L_O_9201; do KAYIT=tab/tarama/$s.bin nice -n 8 ./src/cikarim 129 R 0 100000000 tab/cikarim/c129_$s.bin; done
echo BITTI
