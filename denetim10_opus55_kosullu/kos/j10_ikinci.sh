#!/bin/bash
# J10 129 O havuzuna ikinci sürücü (sondan başa; yazılmakta olan bölütleri atlar; aynı 6 çekirdek-saat tavanı). Mallows havuzu bitince başlar.
cd "$(dirname "$0")/.."
until [ "$(grep -c "BİTTİ" tab/havuz/L129O/surucu.log 2>/dev/null)" -ge 2 ] && ! pgrep -f "kosullu_uret 129 L O" > /dev/null; do sleep 60; done
TERS=1 python3 -I kos/havuz.py 129 J O 4291001 20 1000 tab/havuz/J129O 3 6
