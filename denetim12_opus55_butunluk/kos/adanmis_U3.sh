#!/bin/bash
# adanmis_U3.sh — tekdüze (129) belirsiz kitaplar için adanmış kestirim, ÖNCELİK SIRALI tek dizi (ONKAYIT §4.3: S_alt'a göre azalan).
# adanmis_U.sh ve adanmis_U2.sh'ın yerini alır (aynı tohumlar). Tek süreç. Başarısızlık günlüğe HATA olarak yazılır.
cd "$(dirname "$0")/.."; o=sonuc/adanmis; B=26142600000
run(){ local ad=$1; shift; [ -s $o/$ad.txt ] && return; if "$@" > $o/$ad.tmp 2>&1; then mv $o/$ad.tmp $o/$ad.txt; echo "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] adanmış $ad bitti" >> sonuc/kosu_gunlugu.txt; else echo "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] adanmış $ad HATA" >> sonuc/kosu_gunlugu.txt; fi; }
# 1) S_alt 30,75: (3,-1,17) k=17
run U129_F_3m1_17_K17   ./src/adanmis_u 129 11 17 17 20000000   $((B+12)) F 144
run U129_W_3m1_17_K17   ./src/adanmis_u 129 11 17 17 10000000   $((B+13)) W 34
# 2) S_alt 30,55: (1,-1,11) k=11
run U129_F_1m1_11_K11   ./src/adanmis_u 129 1 11 11  100000000  $((B+14)) F 1738
# 3) S_alt 28,11: (2,+1,19) k=6 ve (2,-1,19) k=19
run U129_W_2p1_19_K6    ./src/adanmis_s 129 U 6 19 6 2000000000 $((B+15)) W 29
run U129_W_2m1_19_K19   ./src/adanmis_u 129 7 19 19  10000000   $((B+16)) W 57
# 4) S_alt 28,01 ve 27,14: (1,-1,19) k=19
run U129_F_1m1_19_K19b  ./src/adanmis_u 129 1 19 19  20000000   $((B+17)) F 640 544
# 5) S_alt 26,6: (2,+1,17) k=17
run U129_F_2p1_17_K17   ./src/adanmis_u 129 6 17 17  20000000   $((B+6))  F 1548
# 6) S_alt 26,88: (1,-1,3) k=36 ve p=5 iki-el maskeleri (k=26)
run U129_F_1m1_3_K36    ./src/adanmis_s 129 U 1 3 36     2000000000 $((B+7))  F 593
run U129_W_1m2_5_K26    ./src/adanmis_s 129 U 3 5 26     2000000000 $((B+8))  W 57
run U129_W_1p3_5_K26    ./src/adanmis_s 129 U 4 5 26     2000000000 $((B+9))  W 57
run U129_W_2p1_5_K26    ./src/adanmis_s 129 U 6 5 26     2000000000 $((B+10)) W 57
run U129_W_3m1_5_K26    ./src/adanmis_s 129 U 11 5 26    2000000000 $((B+11)) W 57
echo "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] adanmış U129 dizisi (U3) BİTTİ" >> sonuc/kosu_gunlugu.txt
