#!/bin/bash
# J10 129 havuzu bitince: puanlama + tam birlikte-olasılık (yalnız düzey 1; ONKAYIT §4.6). İki sürücü (ileri + ters, kos/j10_ikinci.sh) bitmeli.
cd "$(dirname "$0")/.."
until [ "$(grep -c "BİTTİ" tab/havuz/J129O/surucu.log 2>/dev/null)" -ge 2 ] && ! pgrep -f "kosullu_uret 129 J O" > /dev/null; do sleep 60; done
bash kos/puanla.sh 129 O tab/havuz/J129O J129O
DUZEY1=1 nice -n 12 ./src/tam_ortak 129 O J 1 6294001 50 20 2.0 500 2000 sonuc/o1/J129O sonuc/o1/J129O.puan.bin $(ls tab/havuz/J129O/*.bin | sort) > sonuc/o1/J129O_tam.log 2>&1
python3 -I src/tam_cozunurluk.py sonuc/o1/J129O.tam.txt > sonuc/o1/J129O_tam_cozunurluk.txt; python3 -I src/tam_analiz.py sonuc/o1/J129O.tam.txt > sonuc/o1/J129O_tam_analiz.txt
echo "[$(date -u +%FT%TZ)] J10 sonrası bitti" >> sonuc/o1/kosu.log
