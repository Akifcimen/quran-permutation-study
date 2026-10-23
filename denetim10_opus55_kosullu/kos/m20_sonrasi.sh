#!/bin/bash
# M20 129 havuzu bitince: puanlama (bir kez) → Ç4(b) → tam birlikte-olasılık → O' → Ö2. (ONKAYIT §4.4–4.8, §5)
cd "$(dirname "$0")/.."
until grep -q "BİTTİ" tab/havuz/M129O/surucu.log; do sleep 30; done
bash kos/puanla.sh 129 O tab/havuz/M129O M129O
bash kos/uretim_B.sh c4m
bash kos/tam.sh 129 O M 6292001 tab/havuz/M129O M129O 2
bash kos/o_ussu.sh M 2
bash kos/puanla.sh 129 P tab/havuz/M129P M129P
nice -n 10 ./src/cekirdek 129 M 7292001 10000000 1 sonuc/o2/M129_cekirdek.txt $(ls tab/havuz/M129O/*.bin | sort)
until grep -q "BİTTİ" tab/havuz/M127G/surucu.log; do sleep 30; done
nice -n 10 ./src/cekirdek 127 M 7272001 10000000 1 sonuc/o2/M127_cekirdek.txt $(ls tab/havuz/M127G/*.bin | sort)
echo "[$(date -u +%FT%TZ)] M20 sonrası bitti" >> sonuc/o1/kosu.log
