#!/bin/bash
# Mac: özgün sayaçlar ve öykünme arka ucu (eşdeğerlik sınaması); BN küçük.
set -e
cd ~/opus5.5/kart; mkdir -p bin
BNV=${1:-5000}
CF="-O2 -std=gnu11 -w -ffp-contract=off -DKAPI_ESIK=0 -DKAPI_HUCRE=2 -DDMAX=64 -DBN=$BNV -Isrc"
cc $CF -o bin/kontrol_b src/kontrol_b.c -lm -lpthread
cc $CF -o bin/kontrol_h src/kontrol_h.c -lm -lpthread
cc $CF -DKART_KIP=0 -o bin/kart_b_emul src/kart_ana.c src/kart_emul.c -lm
cc $CF -DKART_KIP=1 -o bin/kart_h_emul src/kart_ana.c src/kart_emul.c -lm
ls bin
