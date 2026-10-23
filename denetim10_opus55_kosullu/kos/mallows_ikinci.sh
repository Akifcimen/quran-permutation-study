#!/bin/bash
# Mallows 129 O havuzuna ikinci sürücü (sondan başa; yazılmakta olan bölütleri atlar; aynı tavan). Tekdüze O' bitince başlar.
cd "$(dirname "$0")/.."
until grep -q "U129P puanlandı" sonuc/o1/kosu.log 2>/dev/null; do sleep 60; done
TERS=1 python3 -I kos/havuz.py 129 L O 3291001 100 1000 tab/havuz/L129O 2 16
