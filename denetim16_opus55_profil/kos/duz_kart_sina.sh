#!/bin/bash
# duz_kart_sina.sh — workstation: duz (işlemci, özgün kutuphane.h yolu), duz_emul (duz_ortak.h, işlemci) ve duz_kart (iki kart) aynı tohumla
# aynı çıktıyı mı veriyor? Ayrıca tek merceklik profilin eşdeğerliği ve ΔI formülü.
set -eu; K=~/hrf_olcum/profil; cd $K
CF="-O3 -std=gnu11 -w -DKAPI_ESIK=0 -DKAPI_HUCRE=2 -DDMAX=64 -Isrc -ffp-contract=off"
nice -n 10 gcc $CF -o bin/tek_sina src/tek_sina.c -lm
nice -n 10 gcc $CF -o bin/dI_sina src/dI_sina.c -lm
nice -n 10 gcc $CF -o bin/duz src/duz.c -lm -lpthread
nice -n 10 gcc $CF -o bin/duz_emul src/duz_kart_ana.c src/duz_kart_emul.c -lm
nice -n 10 nvcc -O3 --fmad=false -gencode arch=compute_86,code=sm_86 -gencode arch=compute_89,code=sm_89 -Isrc -c src/duz_kart_cihaz.cu -o bin/duz_kart_cihaz.o
nice -n 10 gcc $CF -c src/duz_kart_ana.c -o bin/duz_kart_ana.o && nice -n 10 nvcc bin/duz_kart_ana.o bin/duz_kart_cihaz.o -o bin/duz_kart -lm
O=duz_sina; mkdir -p $O; rm -f $O/*.txt
nice -n 10 bin/tek_sina 100000 | tee $O/tek_sina.txt
nice -n 10 bin/dI_sina | tee $O/dI_sina.txt
printf 'A000.1\nA000.2\n' > $O/sec.txt
for S in M L U; do
  nice -n 10 bin/duz $S tab/$S/olaylar.txt $O/sec.txt ${1:-k:1000:200000} ${2:-0} 2 4 $O/r_$S.txt
  nice -n 10 bin/duz_emul $S tab/$S/olaylar.txt $O/sec.txt ${1:-k:1000:200000} ${2:-0} 2 0/1 $O/e_$S.txt 2>/dev/null
  CUDA_DEVICE_ORDER=PCI_BUS_ID CUDA_VISIBLE_DEVICES=1 nice -n 10 bin/duz_kart $S tab/$S/olaylar.txt $O/sec.txt ${1:-k:1000:200000} ${2:-0} 2 0/2 $O/k_$S.txt 2>/dev/null
  CUDA_DEVICE_ORDER=PCI_BUS_ID CUDA_VISIBLE_DEVICES=0 nice -n 10 bin/duz_kart $S tab/$S/olaylar.txt $O/sec.txt ${1:-k:1000:200000} ${2:-0} 2 1/2 $O/k_$S.txt 2>/dev/null
  for x in e k; do if diff <(grep -v '^#' $O/${x}_$S.txt | sort) <(grep -v '^#' $O/r_$S.txt | sort) > /dev/null; then echo "$S: $x = duz.c ($(grep -vc '^#' $O/${x}_$S.txt) yürüyüş)"; else echo "$S: $x FARKLI"; fi; done
done | tee $O/sonuc.txt
grep -h '^#' $O/k_*.txt | head -2
