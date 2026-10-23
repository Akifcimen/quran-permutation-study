#!/bin/bash
# Ö0(b) aday taraması: altı kaynak, 129 kodeksi (127 için ayrıca U ve O). Çıktılar tab/cikarim/ (büyük; paket dışı).
cd "$(dirname "$0")/.."
for spec in "U 101 10000000" "M 102 10000000" "J 103 2000000" "L 104 2000000" "O 105 100000" "X 106 10000000"; do
  set -- $spec; nice -n 8 ./src/cikarim 129 $1 $2 $3 tab/cikarim/c129_$1.bin
done
nice -n 8 ./src/cikarim 127 U 201 5000000 tab/cikarim/c127_U.bin
nice -n 8 ./src/cikarim 127 O 205 50000 tab/cikarim/c127_O.bin
echo BITTI
