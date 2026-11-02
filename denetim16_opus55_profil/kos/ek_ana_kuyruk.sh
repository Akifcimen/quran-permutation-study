#!/bin/bash
# ek_ana_kuyruk.sh — workstation: ONKAYIT_EK §4 ana kestirim kuyruğu. Doğrulamanın işlemci işleri bitince verilen sıfırların işlemci
# listeleri (6 iş parçacığı), kart işleri bitince kart listeleri (sıfır başına iki kart süreci) başlar. Bütçe k:4e5:5e7, 4 tekrar.
# Kullanım: bash kos/ek_ana_kuyruk.sh "<işlemci sıfırları>" "<kart sıfırları>"
K=~/hrf_olcum/profil; cd $K; O=ek/cikti; G=$O/gunluk.txt
calisan(){ ps -eo args | grep -cE "^bin/$1 "; }
( while [ "$(calisan duz)" != "0" ]; do sleep 60; done
  echo "[$(date -u +%FT%TZ)] ana işlemci başladı: $1" >> $G
  for S in $1; do nice -n 10 bin/duz $S tab/$S/olaylar.txt ek/sec_A_${S}_isl.txt k:400000:50000000 0 4 6 $O/A_${S}_isl.txt > $O/A_${S}_isl.log 2>&1; echo "[$(date -u +%FT%TZ)] ana işlemci $S bitti rc=$?" >> $G; done ) &
( while [ "$(calisan duz_kart)" != "0" ]; do sleep 60; done
  echo "[$(date -u +%FT%TZ)] ana kart başladı: $2" >> $G
  for S in $2; do for k in 0 1; do dev=$((1-k))
    ( CUDA_DEVICE_ORDER=PCI_BUS_ID CUDA_VISIBLE_DEVICES=$dev nice -n 10 bin/duz_kart $S tab/$S/olaylar.txt ek/sec_A_${S}_kart.txt k:400000:50000000 0 4 $k/2 $O/A_${S}_kart$k.txt > $O/A_${S}_kart$k.log 2>&1; echo "[$(date -u +%FT%TZ)] ana kart $S $k bitti rc=$?" >> $G ) &
  done; done; wait ) &
wait
