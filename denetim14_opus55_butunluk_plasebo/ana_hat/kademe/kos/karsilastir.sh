#!/bin/zsh
# karsilastir.sh <sonuc_dizini> — kontrol_h'nin bölüt 1–400 çıktısını plasebo14'ün Mac'teki kol A bölütleriyle (aynı tohumlar) karşılaştırır:
# her bölütte Kur'an dillerinin D satırları (k, kB) ve aday sayısı. Fark yoksa "FARK 0".
D=$1; P=~/opus5.5/plasebo14/tab; fark=0; n=0
for s in M U L; do
  for b in {1..400}; do bb=$(printf %04d $b); if (( b <= 4 )); then d=d1_0; elif (( b <= 40 )); then d=d2_0; else d=d3_0; fi
    h=$D/$s/b$bb.txt; m=$P/$s/kA/$d/b$bb.txt
    [[ -f $h && -f $m ]] || { print "EKSİK $s $bb"; fark=1; continue }
    a=$(grep '^D' $h | sort); c=$(grep -E '^D (A000|Am000|Ap000) ' $m | sort)
    [[ $a == $c ]] || { print "FARK $s $bb\n$a\n--\n$c"; fark=1 }
    ah=$(sed -n '1s/.* aday \([0-9]*\) .*/\1/p' $h); am=$(sed -n '1s/.* aday \([0-9]*\) .*/\1/p' $m)
    (( am == 0 )) || print "not: Mac aday $s $bb = $am (diller başka hedefleri de içerir)"
    (( ah == 0 )) || print "not: kontrol_h aday $s $bb = $ah"
    n=$((n+1)); done
  print "$s toplam: $(awk '$1=="D"{k[$2]+=$3; kb[$2]+=$4} END{for(x in k) printf "%s k=%d kB=%d  ", x, k[x], kb[x]}' $D/$s/b*.txt)"
done
print "karşılaştırılan bölüt $n, FARK $fark"
