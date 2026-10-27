#!/bin/bash
# menu_kos.sh — bölme menüsü koşusu (ONKAYIT §8.4): U, M20, Mallows (129) × 10 bölüt × 10^6 kitap; çağrı başına TEK süreç.
# İsteğe bağlı argüman: sıfır listesi (ör. ./kos/menu_kos.sh M) — ayrı çağrılar ayrı sıfırlarda koşturulabilir.
# Tohum = 26102600000 + sıfır·10^8 + 3·10^7 + bölüt. kos/DUR dosyası varsa bölüt aralarında durur. Sürdürülebilir (.say = tamam).
cd "$(dirname "$0")/.."
for s in ${@:-U M L}; do
  case $s in U) n=1;; M) n=2;; L) n=3;; esac
  d=tab/D_${s}129; mkdir -p $d
  for b in $(seq 1 10); do
    on=$(printf "%s/b%03d" $d $b)
    [ -f $on.say ] && continue
    [ -f kos/DUR ] && { echo "DUR dosyası: duruldu"; exit 0; }
    rm -f $on.*
    seed=$((26102600000 + n*100000000 + 3*10000000 + b))
    ./src/menu 129 $s $seed 1000000 $on
    echo "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] menü ${s}129 bölüt $b bitti" >> sonuc/kosu_gunlugu.txt
  done
done
echo "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] menü koşusu BİTTİ" >> sonuc/kosu_gunlugu.txt
