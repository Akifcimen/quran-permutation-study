#!/bin/bash
# ek_mac_yeniden.sh — Mac (4 iş parçacığı): kalite şartını geçmeyen tekrarların yeniden koşusu (ONKAYIT_EK §2): iki kat bütçe, tohum r+4.
# Mac'in ana kuyruğu (ek_mac_kuyruk.sh) bitince başlar. Kullanım: ek_mac_yeniden.sh <S>...
cd ~/opus5.5/profil; O=sonuc/ek/cikti; G=$O/gunluk_mac.txt; B2=k:800000:100000000
until grep -q "ana U kart bitti" $G; do sleep 60; done
for S in "$@"; do for r in 1 2 3 4; do L=sonuc/ek/yen_${S}_r$r.txt; [ -s $L ] || continue
  ./bin_mac_duz $S sonuc/tab/$S/olaylar.txt $L $B2 0 1 4 $O/Y_${S}_r$r.txt $((r+4)); echo "[$(date -u +%FT%TZ)] yeniden $S r$r→$((r+4)) bitti rc=$?" >> $G; done; done
echo "[$(date -u +%FT%TZ)] yeniden bitti: $*" >> $G
