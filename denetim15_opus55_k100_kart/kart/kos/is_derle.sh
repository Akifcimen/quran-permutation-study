#!/bin/bash
# is_derle.sh — workstation: kart sayacını derler (kip 0 = kontrol_b kol 1–3, kip 1 = kontrol_h; kapı Gc) ve işlemci öykünmesini.
# Ana program, özgün sayaçlarla aynı gcc bayraklarıyla derlenir (tablolar bit-bit aynı olsun diye); kart kodu --fmad=false.
set -eu
K=~/hrf_olcum/kart; cd $K; mkdir -p bin
CF="-O3 -std=gnu11 -w -DKAPI_ESIK=0 -DKAPI_HUCRE=2 -DDMAX=64"
nice -n 10 nvcc -O3 --fmad=false -gencode arch=compute_86,code=sm_86 -gencode arch=compute_89,code=sm_89 -Isrc -c src/kart_cihaz.cu -o bin/kart_cihaz.o
for KIP in 0 1; do ad=$([ $KIP = 0 ] && echo b || echo h)
  nice -n 10 gcc $CF -DKART_KIP=$KIP -Isrc -c src/kart_ana.c -o bin/kart_ana_$ad.o
  nice -n 10 nvcc bin/kart_ana_$ad.o bin/kart_cihaz.o -o bin/kart_$ad -lm
  nice -n 10 gcc $CF -DKART_KIP=$KIP -Isrc -o bin/kart_${ad}_emul src/kart_ana.c src/kart_emul.c -lm; done
sha256sum src/* > bin/kaynak_sha256.txt; ls -la bin
