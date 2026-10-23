#!/bin/bash
# İkinci aşama yardımcı koşular (ONKAYIT §4.7 Ç3 başlangıç denetimi; Ç4 zincir sınamaları).
cd "$(dirname "$0")/.."
case "$1" in
  c3gercek) mkdir -p tab/havuz/M129O_gercek; M20_BASLANGIC=gercek nice -n 10 ./src/kosullu_uret 129 M O 2299001 5000 tab/havuz/M129O_gercek/M_O_129_2299001.bin ;;
  c4u)  nice -n 10 ./src/zincir_sina 129 U U 5291001 200 2000 20 sonuc/c4_U129.txt tab/havuz/U129O/U_O_129_1291001.bin ;;
  c4m)  nice -n 10 ./src/zincir_sina 129 M S 5292001 5000 200 20 sonuc/c4_M129.txt $(ls tab/havuz/M129O/M_O_129_*.bin | sort) ;;
  c4l)  nice -n 10 ./src/zincir_sina 129 L S 5293001 5000 200 20 sonuc/c4_L129.txt $(ls tab/havuz/L129O/L_O_129_*.bin | sort) ;;
  *) echo "bilinmeyen: $1"; exit 1 ;;
esac
