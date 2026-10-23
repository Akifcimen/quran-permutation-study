#!/bin/bash
# O' havuzları (ONKAYIT §4.8). kullanım: kos/o_ussu.sh <U|M|L> <paralel>
# U: U129O'nun ilk 10 bölütü, iskelet başına 3.000 kesin tamamlama, tohum 1292001+b. M/L: bütün bölütler, 3.000 kayıt × 20 adım, tohum 2292001+b / 3292001+b.
cd "$(dirname "$0")/.."
nul=$1; par=$2
case $nul in
  U) src=tab/havuz/U129O; dst=tab/havuz/U129P; s0=1292001; R=3000; K=1; nmax=10 ;;
  M) src=tab/havuz/M129O; dst=tab/havuz/M129P; s0=2292001; R=3000; K=20; nmax=1000 ;;
  L) src=tab/havuz/L129O; dst=tab/havuz/L129P; s0=3292001; R=3000; K=20; nmax=1000 ;;
esac
mkdir -p $dst
ls $src/*.bin | sort | head -n $nmax | awk '{print NR-1, $0}' | while read b f; do
  out=$dst/$(basename $f .bin | sed 's/_O_/_P_/').bin
  [ -f $out ] && continue
  echo "$b $f $out"
done | xargs -P $par -L 1 bash -c 'nice -n 12 ./src/o_ussu 129 '$nul' $(('$s0'+$0)) '$R' '$K' $2 $1'
echo "[$(date -u +%FT%TZ)] O' $nul bitti" >> $dst/kosu.log
