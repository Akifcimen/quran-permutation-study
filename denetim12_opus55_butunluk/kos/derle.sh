#!/bin/bash
# derle.sh — bütün C programlarını derler (macOS clang; -O3). Kullanım: kos/derle.sh [çıktı_dizini]
# (çıktı dizini verilmezse src/ programları src/'ye, kontrol/ programları kontrol/'e yazılır.)
# kontrol/kredi9_dok.c eskidir (dondurmadan önce KREDI9 tablosu kesin kredi tablosu RED17 ile değiştirildi); derlenmez.
cd "$(dirname "$0")/.."; O=$1
for f in src/*.c; do b=$(basename $f .c); cc -O3 -o ${O:-src}/$b $f -lm || { echo "HATA: $f"; exit 1; }; done
for f in kontrol/*.c; do b=$(basename $f .c); [ $b = kredi9_dok ] && continue; cc -O3 -o ${O:-kontrol}/$b $f -lm || { echo "HATA: $f"; exit 1; }; done
echo "derleme tamam"
