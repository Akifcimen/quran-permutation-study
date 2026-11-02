#!/bin/bash
set -e; cd ~/opus5.5/profil; BNV=${1:-2000}
CF="-O2 -std=gnu11 -w -ffp-contract=off -DKAPI_ESIK=0 -DKAPI_HUCRE=2 -DDMAX=64 -Isrc"
cc $CF -o bin/dil_b src/dil_b.c -lm -lpthread
cc $CF -o bin/olay_kur src/olay_kur.c -lm
cc $CF -DBN=$BNV -o bin/olay_ozgun src/olay_ozgun.c -lm
cc $CF -DBN=$BNV -o bin/kart_olay_emul src/kart_olay_ana.c src/kart_emul.c -lm
cc $CF -o bin/dil_b_ozgun ~/opus5.5/kapi/src/dil_b.c -I ~/opus5.5/kapi/src -lm -lpthread
echo derlendi
