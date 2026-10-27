#!/bin/bash
# sira_M_once.sh — 4. yuvanın önceliği: tekdüze dizisinin (U3) yüksek öncelikli işleri bitince U3 (bash + o anki çocuğu) SIGSTOP ile
# dondurulur, M20 adanmış dizisi koşar, sonra U3 SIGCONT ile kaldığı yerden sürer. Aynı anda en çok bir adanmış süreç çalışır.
cd "$(dirname "$0")/.."
until grep -q "adanmış U129_F_2p1_17_K17 \(bitti\|HATA\)" sonuc/kosu_gunlugu.txt; do sleep 5; done
U3=$(pgrep -f "kos/adanmis_U3.sh"); CH=$(pgrep -P $U3 | tr '\n' ' ')
kill -STOP $U3 $CH
echo "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] U3 donduruldu (M20 adanmış dizisi öne alındı; çocuk $CH)" >> sonuc/kosu_gunlugu.txt
./kos/adanmis_M.sh
kill -CONT $CH $U3
echo "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] U3 sürdürüldü" >> sonuc/kosu_gunlugu.txt
