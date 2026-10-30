#!/bin/zsh
# o0_k.sh — Ö0 (ONKAYIT §7.1): kontrol_b, kol 0 kipinde plasebo12'nin düzey-1 dilleriyle (tab/<sıfır>/k1/diller.txt; Kur'an'ın Q,
# QP2 ve el dili eşikleri dahil) bölüt 1'de plasebo12'nin esikler.txt, b0001.txt (başlık satırı hariç) ve b0001.q dosyalarını bit-bit
# vermeli. Üç sıfır paralel, her biri tek iş parçacığı (toplam 3). Yalnız karşılaştırma sonucu basılır.
cd "${0:A:h}/.."
P12=~/opus5.5/plasebo12; O=sonuc/o0; mkdir -p $O; : > $O/o0_k.txt
for S in M U L; do (
  R=($P12/tab/$S/ref_1111.ref $P12/tab/$S/ref_2222.ref $P12/tab/$S/ref_0444.ref $P12/tab/$S/ref_0888.ref)
  D=$O/k1_$S; mkdir -p $D; rm -f $D/b0001.txt $D/b0001.q
  ESZAMANLI=1 ./src/kontrol_b $S tab/$S/hedef.bin $P12/tab/$S/k1/diller.txt 1 1 $D $R 2> $D/hata.txt ) &
done; wait
for S in M U L; do D=$O/k1_$S
  e=$(cmp -s $D/esikler.txt $P12/tab/$S/k1/esikler.txt && echo AYNI || echo FARKLI)
  q=$(diff -q <(grep -E '^(D Q |D QP2 |E 0 )' $D/esikler.txt) <(grep -E '^(D Q |D QP2 |E 0 )' $P12/tab/$S/k1/esikler.txt) > /dev/null && echo AYNI || echo FARKLI)
  b=$(diff <(sed 1d $D/b0001.txt) <(sed 1d $P12/tab/$S/k1/b0001.txt) > /dev/null && echo AYNI || echo FARKLI)
  nb=$(diff <(sed 1d $D/b0001.txt) <(sed 1d $P12/tab/$S/k1/b0001.txt) | grep -c '^<')
  a=$(cmp -s $D/b0001.q $P12/tab/$S/k1/b0001.q && echo AYNI || echo FARKLI)
  print -r -- "$S: esikler.txt $e (Kur'an'ın Q/QP2/el dili eşikleri $q); bölüt 1 sayımları $b (farklı satır $nb / $(sed 1d $D/b0001.txt | wc -l | tr -d ' ')); b0001.q $a; süre $(head -1 $D/b0001.txt | awk '{for(i=1;i<=NF;i++) if($i=="sure_s") print $(i+1)}') s" >> $O/o0_k.txt
done
cat $O/o0_k.txt
