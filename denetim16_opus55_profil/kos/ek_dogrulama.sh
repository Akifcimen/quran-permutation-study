#!/bin/bash
# ek_dogrulama.sh — workstation: ONKAYIT_EK §3 doğrulama koşuları. Kartlar: her sıfırda D1+D2'nin kart listesi, iki kartta yarı yarıya
# (6 süreç, engelleyici bekleme). İşlemci: her sıfırda D1'in işlemci listesi, 2'şer iş parçacığı (toplam 6). Bütçe k:4e5:5e7, 4 tekrar.
set -eu; K=~/hrf_olcum/profil; cd $K; O=ek/cikti; mkdir -p $O
CF="-O3 -std=gnu11 -w -DKAPI_ESIK=0 -DKAPI_HUCRE=2 -DDMAX=64 -Isrc -ffp-contract=off"
nice -n 10 nvcc -O3 --fmad=false -gencode arch=compute_86,code=sm_86 -gencode arch=compute_89,code=sm_89 -Isrc -c src/duz_kart_cihaz.cu -o bin/duz_kart_cihaz.o 2>/dev/null
nice -n 10 gcc $CF -c src/duz_kart_ana.c -o bin/duz_kart_ana.o && nice -n 10 nvcc bin/duz_kart_ana.o bin/duz_kart_cihaz.o -o bin/duz_kart -lm
sha256sum src/duz.c src/duz_ortak.h src/duz_kart_cihaz.cu src/duz_kart_ana.c src/duz_kart_emul.c src/tek_mercek.h src/kutuphane.h src/kart_ortak.h bin/duz bin/duz_kart > $O/kod_sha256.txt
echo "[$(date -u +%FT%TZ)] doğrulama başladı; ek $(sha256sum ONKAYIT_EK.md | cut -c1-16)" >> $O/gunluk.txt
for S in M L U; do cat ek/sec_D1_${S}_kart.txt ek/sec_D2_${S}_kart.txt > ek/sec_D_${S}_kart.txt
  for k in 0 1; do dev=$((1-k))   # parça 0 → RTX 4090 (PCI 1), parça 1 → RTX 3090 (PCI 0)
    (CUDA_DEVICE_ORDER=PCI_BUS_ID CUDA_VISIBLE_DEVICES=$dev setsid nohup nice -n 10 bin/duz_kart $S tab/$S/olaylar.txt ek/sec_D_${S}_kart.txt k:400000:50000000 0 4 $k/2 $O/D_${S}_kart$k.txt > $O/D_${S}_kart$k.log 2>&1; echo "[$(date -u +%FT%TZ)] kart $S $k bitti rc=$?" >> $O/gunluk.txt) &
  done
  (setsid nohup nice -n 10 bin/duz $S tab/$S/olaylar.txt ek/sec_D1_${S}_isl.txt k:400000:50000000 0 4 2 $O/D_${S}_isl.txt > $O/D_${S}_isl.log 2>&1; echo "[$(date -u +%FT%TZ)] işlemci $S bitti rc=$?" >> $O/gunluk.txt) &
done
sleep 2; ps -eo pid,args | grep -E "[b]in/duz(_kart)? " | wc -l
