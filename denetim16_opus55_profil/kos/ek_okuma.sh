#!/bin/bash
# ek_okuma.sh — ölçüm 2'nin doğrulaması ve okuması (sonuc/ek/okuma/ dosyalarını üretir). Önce sonuc/tab/<S>/bolutler_profil_<S>.tar.gz
# kendi klasöründe açılmış olmalı (d1–d4). Kök klasörde çalıştırın: bash kos/ek_okuma.sh
set -e; C=sonuc/ek/cikti; O=sonuc/ek/okuma; mkdir -p $O
for S in M L U; do F="$C/ws/D_${S}_isl.txt,$C/ws/D_${S}_kartisl.txt,$(ls $C/Y_${S}_r*.txt | tr '\n' ',' | sed 's/,$//')"
  for H in 0 1; do echo "== DUZ_HARIC=$H"; DUZ_HARIC=$H python3 -I dogrulama/duz_oku.py dogrula sonuc/tab/$S $S "$F" sonuc/ek/sec_D1_$S.txt sonuc/ek/sec_D2_$S.txt; done > $O/dogrulama_$S.txt; done
# tekdüze doğrulamanın (e) şartında düştüğü için okunmaz (ONKAYIT_EK §3)
FM="$C/Q_M.txt $C/A_M_islisl.txt $C/A_M_kartisl.txt $C/Y_M_r*.txt $C/ws/D_M_isl.txt $C/ws/D_M_kartisl.txt"
FL="$C/Q_L.txt $C/ws/A_L_islisl.txt $C/ws/A_L_kartisl.txt $C/Y_L_r*.txt $C/ws/Y_L_r1_ws.txt $C/ws/Y_L_r2_ws.txt $C/ws/D_L_isl.txt $C/ws/D_L_kartisl.txt"
for X in "M 20000 $FM" "L 10000 $FL"; do set -- $X; S=$1; K=$2; shift 2
  (date -u +%FT%TZ; for H in 0 1; do echo "== DUZ_HARIC=$H"; DUZ_HARIC=$H python3 -I dogrulama/duz_oku.py oku sonuc/tab/$S $S $K "$@"; done
   python3 -I dogrulama/duz_ayrinti.py sonuc/tab/$S $S "$@") > $O/okuma_$S.txt; done
