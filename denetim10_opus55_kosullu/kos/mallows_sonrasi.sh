#!/bin/bash
# Mallows 129 havuzu bitince (tamamı ya da tavan): puanlama → Ç4(b) → tam birlikte-olasılık → O' → Ö2. (ONKAYIT §4.4–4.8, §5)
cd "$(dirname "$0")/.."
until [ "$(grep -c "BİTTİ" tab/havuz/L129O/surucu.log 2>/dev/null)" -ge 2 ] && ! pgrep -f "kosullu_uret 129 L O" > /dev/null; do sleep 60; done   # iki sürücü de bitmeli
bash kos/puanla.sh 129 O tab/havuz/L129O L129O
bash kos/uretim_B.sh c4l
bash kos/tam.sh 129 O L 6293001 tab/havuz/L129O L129O 2
bash kos/o_ussu.sh L 3
bash kos/puanla.sh 129 P tab/havuz/L129P L129P
DUZEY1=1 nice -n 12 ./src/tam_ortak 129 P L 1 6295003 50 20 2.0 500 2000 sonuc/o1/L129P sonuc/o1/L129P.puan.bin $(ls tab/havuz/L129P/*.bin | sort) > sonuc/o1/L129P_tam.log 2>&1
python3 -I src/tam_cozunurluk.py sonuc/o1/L129P.tam.txt > sonuc/o1/L129P_tam_cozunurluk.txt; python3 -I src/tam_analiz.py sonuc/o1/L129P.tam.txt > sonuc/o1/L129P_tam_analiz.txt
nice -n 10 ./src/cekirdek 129 L 7293001 10000000 1 sonuc/o2/L129_cekirdek.txt $(ls tab/havuz/L129O/*.bin | sort)
until grep -q "BİTTİ" tab/havuz/L127G/surucu.log; do sleep 60; done
nice -n 10 ./src/cekirdek 127 L 7273001 10000000 1 sonuc/o2/L127_cekirdek.txt $(ls tab/havuz/L127G/*.bin | sort)
echo "[$(date -u +%FT%TZ)] Mallows sonrası bitti" >> sonuc/o1/kosu.log
