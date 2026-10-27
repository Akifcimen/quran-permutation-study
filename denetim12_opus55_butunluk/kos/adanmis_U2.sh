#!/bin/bash
# adanmis_U2.sh — tekdüze (129) tam sınama örnekleminde belirsizlerin yeni en üsttekileri için adanmış kestirim (ONKAYIT §4.3).
# adanmis_U.sh bitince başlar. Tohum = 26142600000 + 12…17.
cd "$(dirname "$0")/.."; o=sonuc/adanmis; B=26142600000
while pgrep -f "kos/adanmis_U.sh" >/dev/null; do sleep 20; done
run(){ local ad=$1; shift; [ -s $o/$ad.txt ] && return; if "$@" > $o/$ad.tmp 2>&1; then mv $o/$ad.tmp $o/$ad.txt; echo "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] adanmış $ad bitti" >> sonuc/kosu_gunlugu.txt; else echo "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] adanmış $ad HATA" >> sonuc/kosu_gunlugu.txt; fi; }
run U129_F_1m1_19_K19   ./src/adanmis_u 129 1 19 19 20000000   $((B+5))  F 544
run U129_F_2p1_17_K17   ./src/adanmis_u 129 6 17 17 20000000   $((B+6))  F 1548
run U129_F_3m1_17_K17   ./src/adanmis_u 129 11 17 17 20000000   $((B+12)) F 144
run U129_W_3m1_17_K17   ./src/adanmis_u 129 11 17 17 10000000   $((B+13)) W 34
run U129_F_1m1_11_K11   ./src/adanmis_u 129 1 11 11  100000000  $((B+14)) F 1738
run U129_W_2p1_19_K6    ./src/adanmis_s 129 U 6 19 6 2000000000 $((B+15)) W 29
run U129_W_2m1_19_K19   ./src/adanmis_u 129 7 19 19  10000000   $((B+16)) W 57
run U129_F_1m1_19_K19b  ./src/adanmis_u 129 1 19 19  20000000   $((B+17)) F 640 544
echo "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] adanmış U129 ikinci dizi BİTTİ" >> sonuc/kosu_gunlugu.txt
