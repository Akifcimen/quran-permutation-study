#!/bin/bash
# Ö1 birincil havuzları (ONKAYIT §4.3). Her satır ayrı arka plan sürücüsü; bölütler sürdürülebilir (havuz.py).
cd "$(dirname "$0")/.."
H="python3 -I kos/havuz.py"
case "$1" in
  mallows) $H 129 L O 3291001 100 1000 tab/havuz/L129O 3 16 ;;
  m20)     $H 129 M O 2291001 50 2000 tab/havuz/M129O 1 8 ;;
  u129)    $H 129 U O 1291001 20 10000 tab/havuz/U129O 1 4 ;;
  u127)    $H 127 U O 1271001 20 10000 tab/havuz/U127O 1 4 ;;
  j10)     $H 129 J O 4291001 20 1000 tab/havuz/J129O 1 6 ;;
  m20g127) $H 127 M G 2271001 4 50 tab/havuz/M127G 1 4 ;;
  lg127)   $H 127 L G 3271001 4 25 tab/havuz/L127G 1 6 ;;
  *) echo "bilinmeyen: $1"; exit 1 ;;
esac
