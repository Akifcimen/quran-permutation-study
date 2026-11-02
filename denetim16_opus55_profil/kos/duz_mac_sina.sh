#!/bin/bash
# duz_mac_sina.sh — Mac: duz_ortak.h (öykünme) ile duz.c'nin aynı tohumla aynı çıktıyı verdiğinin sınaması.
set -eu; cd ~/opus5.5/profil
CF="-O3 -std=gnu11 -w -DKAPI_ESIK=0 -DKAPI_HUCRE=2 -DDMAX=64 -Isrc -ffp-contract=off"
gcc $CF -o zincir_gel/duz_emul src/duz_kart_ana.c src/duz_kart_emul.c -lm
gcc $CF -o zincir_gel/duz_ref src/duz.c -lm -lpthread
cd zincir_gel; printf 'A000.1\nA000.2\n' > sec_s.txt
for S in M L U; do rm -f e_$S.txt r_$S.txt
  ./duz_emul $S ../sonuc/tab/$S/olaylar.txt sec_s.txt ${1:-k:1000:200000} ${2:-0} 2 0/1 e_$S.txt 2>/dev/null
  ./duz_ref $S ../sonuc/tab/$S/olaylar.txt sec_s.txt ${1:-k:1000:200000} ${2:-0} 2 2 r_$S.txt
  if diff <(grep -v '^#' e_$S.txt | sort) <(grep -v '^#' r_$S.txt | sort) > /dev/null; then echo "$S: öykünme = duz.c ($(grep -vc '^#' e_$S.txt) yürüyüş)"
  else echo "$S: FARKLI"; diff <(grep -v '^#' e_$S.txt | sort | cut -c1-120) <(grep -v '^#' r_$S.txt | sort | cut -c1-120) || true; fi
done
