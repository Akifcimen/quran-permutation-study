#!/bin/bash
# ek_mac_kuyruk.sh — Mac (4 iş parçacığı): M20 ana kestirimi (işlemci ve kart listeleri, ikisi de işlemcide), sonra tekdüze ana kestirimi.
cd ~/opus5.5/profil; O=sonuc/ek/cikti; B=k:400000:50000000
for S in M U; do for L in isl kart; do ./bin_mac_duz $S sonuc/tab/$S/olaylar.txt sonuc/ek/sec_A_${S}_$L.txt $B 0 4 4 $O/A_${S}_${L}isl.txt; echo "[$(date -u +%FT%TZ)] ana $S $L bitti rc=$?" >> $O/gunluk_mac.txt; done; done
