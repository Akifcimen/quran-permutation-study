#!/bin/bash
# adanmis_M.sh — M20 (129) belirsiz kitaplar için adanmış kestirim (ONKAYIT §4.3: tek mercekli doğrudan örnekleme), S_alt'a göre
# azalan tek dizi. Kur'an'ın M20 aramalarının hepsi çözülmüş (c ≥ 10). Kitap sayısı: beklenen isabet ≥ 30 ise 1e9, değilse 2e9 (tavan).
# Tohumlar 26242600000 + i (ek/EK_..._adanmis_tohumlari.md). Toplam 1,9e10 kitap ≈ 3,4 çekirdek-saat (≤ 4). Tek süreç.
cd "$(dirname "$0")/.."; o=sonuc/adanmis; B=26242600000
run(){ local ad=$1; shift; [ -s $o/$ad.txt ] && return; if "$@" > $o/$ad.tmp 2>&1; then mv $o/$ad.tmp $o/$ad.txt; echo "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] adanmış $ad bitti" >> sonuc/kosu_gunlugu.txt; else echo "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] adanmış $ad HATA" >> sonuc/kosu_gunlugu.txt; fi; }
run M129_W_1m1_5_K20    ./src/adanmis_s 129 M 1 5 20    2000000000 $((B+2))  W 20     # b002/3449001  S_alt 27,29
run M129_F_1m1_7_K23    ./src/adanmis_s 129 M 1 7 23    1000000000 $((B+3))  F 580    # b014/1713494  26,86
run M129_F_1m1_11_K11   ./src/adanmis_s 129 M 1 11 11   2000000000 $((B+4))  F 204    # b005/11350    26,69
run M129_W_2m1_17_K7    ./src/adanmis_s 129 M 7 17 7    2000000000 $((B+5))  W 29     # b008/2421252  26,62
run M129_W_1p1_17_K17   ./src/adanmis_s 129 M 0 17 17   1000000000 $((B+6))  W 4      # b020/4411008  26,45
run M129_F_3m1_7_K14    ./src/adanmis_s 129 M 11 7 14   2000000000 $((B+7))  F 1582   # b017/3690548  26,32
run M129_F_1m3_13_K9    ./src/adanmis_s 129 M 5 13 9    1000000000 $((B+8))  F 632    # b013/3447723  26,25
run M129_B_1p3          ./src/adanmis_s 129 M 4 0 0     2000000000 $((B+9))  B 2      # b016/1210550  25,99 (B2 @ (1,±3))
run M129_F_1m1_13_K8    ./src/adanmis_s 129 M 1 13 8    2000000000 $((B+10)) F 242    # b004/1315858  25,82
run M129_W_1m3_19_K6    ./src/adanmis_s 129 M 5 19 6    1000000000 $((B+11)) W 58     # b007/3025535  25,77
run M129_F_2p1_13_K13   ./src/adanmis_s 129 M 6 13 13   1000000000 $((B+12)) F 1714   # b008/2217288  25,62
run M129_F_2m1_7_K17    ./src/adanmis_s 129 M 7 7 17    2000000000 $((B+13)) F 1650   # b012/2173162  25,52
echo "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] adanmış M129 dizisi BİTTİ" >> sonuc/kosu_gunlugu.txt
