#!/bin/bash
# ek_kuran_mac.sh — Mac (4 iş parçacığı): Kur'an'ın O1 ve O2 olayları, üç sıfır sırayla. Bütçe k:4e5:5e7, 4 tekrar.
set -eu; cd ~/opus5.5/profil; O=sonuc/ek/cikti; mkdir -p $O
gcc -O3 -std=gnu11 -w -DKAPI_ESIK=0 -DKAPI_HUCRE=2 -DDMAX=64 -Isrc -ffp-contract=off -o bin_mac_duz src/duz.c -lm -lpthread
shasum -a 256 src/duz.c src/tek_mercek.h src/kutuphane.h bin_mac_duz > $O/kod_sha256_mac.txt
for S in M L U; do ./bin_mac_duz $S sonuc/tab/$S/olaylar.txt sonuc/ek/sec_Q_$S.txt k:400000:50000000 0 4 4 $O/Q_$S.txt; echo "[$(date -u +%FT%TZ)] Kur'an $S bitti" >> $O/gunluk_mac.txt; done
