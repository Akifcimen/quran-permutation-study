#!/bin/bash
# adanmis_U.sh — tekdüze (129) en üstteki belirsiz kitapların çözülmemiş aramaları için adanmış kestirim (ONKAYIT §4.3); tek süreç.
# Tohum = 26142600000 + sıra (tür 4 = adanmış; 1 ve 2 Kur'an'ın aramasında kullanıldı). Çıktılar sonuc/adanmis/.
cd "$(dirname "$0")/.."; o=sonuc/adanmis; B=26142600000
run(){ local ad=$1; shift; [ -s $o/$ad.txt ] && return; "$@" > $o/$ad.tmp 2>&1 && mv $o/$ad.tmp $o/$ad.txt; echo "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] adanmış $ad bitti" >> sonuc/kosu_gunlugu.txt; }
run U129_B_1p3          ./src/adanmis_s 129 U 4 11 0     2000000000 $((B+3))  B 2 3
run U129_W_1p1_19_K6_7  ./src/adanmis_s 129 U 0 19 6,7   2000000000 $((B+4))  W 37 13
run U129_F_1m1_19_K19   ./src/adanmis_u 129 1 19 19      20000000   $((B+5))  F 544
run U129_F_2p1_17_K17   ./src/adanmis_u 129 6 17 17      20000000   $((B+6))  F 1548
run U129_F_1m1_3_K36    ./src/adanmis_s 129 U 1 3 36     2000000000 $((B+7))  F 593
run U129_W_1m2_5_K26    ./src/adanmis_s 129 U 3 5 26     2000000000 $((B+8))  W 57
run U129_W_1p3_5_K26    ./src/adanmis_s 129 U 4 5 26     2000000000 $((B+9))  W 57
run U129_W_2p1_5_K26    ./src/adanmis_s 129 U 6 5 26     2000000000 $((B+10)) W 57
run U129_W_3m1_5_K26    ./src/adanmis_s 129 U 11 5 26    2000000000 $((B+11)) W 57
echo "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] adanmış U129 dizisi BİTTİ" >> sonuc/kosu_gunlugu.txt
