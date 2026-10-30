#!/bin/zsh
# o0_d.sh — Ö0 (ONKAYIT §7.1): kısıtsız D (dil_b P kipi) üç sıfırda plasebo12'nin tab/<sıfır>/diller.txt ve ayrinti.txt dosyalarını
# bit-bit vermeli. Dosyalar sonuc/o0/ altına yazılır; yalnız karşılaştırma sonucu basılır (dil içerikleri basılmaz).
cd "${0:A:h}/.."
P12=~/opus5.5/plasebo12; O=sonuc/o0; mkdir -p $O; : > $O/o0_d.txt
for S in M U L; do (
  R=($P12/tab/$S/ref_1111.ref $P12/tab/$S/ref_2222.ref $P12/tab/$S/ref_0444.ref $P12/tab/$S/ref_0888.ref)
  ./src/dil_b P tab/$S/hedef.bin 0 $O/diller0_$S.txt $O/ayrinti0_$S.txt $R
  HEDEF_SON=0 ./src/dil_b P tab/$S/hedef.bin 1 $O/dillerP2_$S.txt $O/ayrintiP2_$S.txt $R
  { cat $O/diller0_$S.txt $O/dillerP2_$S.txt; print "EL -1 0 -1 20 0 1 7 11 12 13 14 15 16 17 18 19 20 21 65 66 67 68 69 70" } > $O/diller_$S.txt
  cat $O/ayrinti0_$S.txt $O/ayrintiP2_$S.txt > $O/ayrinti_$S.txt ) &
done; wait
for S in M U L; do
  d=$(cmp -s $O/diller_$S.txt $P12/tab/$S/diller.txt && echo AYNI || echo FARKLI)
  a=$(cmp -s $O/ayrinti_$S.txt $P12/tab/$S/ayrinti.txt && echo AYNI || echo FARKLI)
  nd=$(diff $O/diller_$S.txt $P12/tab/$S/diller.txt | grep -c '^<')
  print -r -- "$S: diller.txt $d (farklı satır $nd; satır $(wc -l < $O/diller_$S.txt | tr -d ' ')), ayrinti.txt $a, SHA-256 $(shasum -a 256 $O/diller_$S.txt | cut -c1-16)… / plasebo12 $(cut -c1-16 <<< $(grep "tab/$S/diller.txt" $P12/sonuc/diller_${S}_sha256.txt))…" >> $O/o0_d.txt
done
cat $O/o0_d.txt
