#!/bin/bash
# is_derle.sh — workstation: profil çalışmasının ikilileri (kapı Gc). Ana programlar özgün sayaçlarla aynı gcc bayraklarıyla; kart --fmad=false.
set -eu; K=~/hrf_olcum/profil; cd $K; mkdir -p bin
CF="-O3 -std=gnu11 -w -DKAPI_ESIK=0 -DKAPI_HUCRE=2 -DDMAX=64 -Isrc"
nice -n 10 gcc $CF -o bin/hedef src/hedef.c -lm
nice -n 10 gcc $CF -o bin/dil_b src/dil_b.c -lm -lpthread
nice -n 10 gcc $CF -o bin/olay_kur src/olay_kur.c -lm
nice -n 10 gcc $CF -o bin/olay_ozgun src/olay_ozgun.c -lm
nice -n 10 nvcc -O3 --fmad=false -gencode arch=compute_86,code=sm_86 -gencode arch=compute_89,code=sm_89 -Isrc -c src/kart_cihaz.cu -o bin/kart_cihaz.o
nice -n 10 gcc $CF -c src/kart_olay_ana.c -o bin/kart_olay_ana.o && nice -n 10 nvcc bin/kart_olay_ana.o bin/kart_cihaz.o -o bin/kart_olay -lm
nice -n 10 gcc $CF -o bin/kart_olay_emul src/kart_olay_ana.c src/kart_emul.c -lm
sha256sum src/* kos/* dogrulama/* > bin/kaynak_sha256.txt; sha256sum bin/kart_olay bin/kart_olay_emul bin/olay_ozgun bin/olay_kur bin/dil_b bin/hedef > bin/ikili_sha256.txt; ls bin
