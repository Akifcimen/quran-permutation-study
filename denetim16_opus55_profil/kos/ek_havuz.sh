#!/bin/bash
# ek_havuz.sh — workstation: iki kol (6'şar iş parçacığı) ortak iş listesinden (ek/cikti/isler.txt, flock ile) sıradaki işi çeker.
# Kol A, koşmakta olan doğrulama M kart listesi bitince; kol B, doğrulamanın işlemci işleri bitince başlar. İş satırı: <sıfır> <seçim> <çıktı>
K=~/hrf_olcum/profil; cd $K; O=ek/cikti; G=$O/gunluk.txt; B=k:400000:50000000; J=$O/isler.txt
al(){ ( flock 9; l=$(head -1 $J); [ -n "$l" ] && sed -i 1d $J; echo "$l" ) 9>$J.kilit; }
kol(){ while true; do l=$(al); [ -z "$l" ] && break; set -- $l
  echo "[$(date -u +%FT%TZ)] kol $K_AD: $2 başladı" >> $G
  nice -n 10 bin/duz $1 tab/$1/olaylar.txt $2 $B 0 4 6 $3 > $3.log 2>&1; echo "[$(date -u +%FT%TZ)] kol $K_AD: $2 bitti rc=$?" >> $G; done; }
( while [ "$(ps -eo args | grep -cE '^bin/duz M tab/M/olaylar.txt ek/sec_D_M_kart')" != "0" ]; do sleep 30; done; K_AD=A kol ) &
( while [ "$(ps -eo args | grep -cE '^bin/duz . tab/./olaylar.txt ek/sec_D1_')" != "0" ]; do sleep 30; done; K_AD=B kol ) &
wait; echo "[$(date -u +%FT%TZ)] havuz boş" >> $G
