#!/bin/bash
# mac_sina.sh <S> <kip b|h> <diller> <hedef> <ilk> <son> — özgün ve öykünme aynı girdilerde; çıktılar sure_s dışında karşılaştırılır.
cd ~/opus5.5/kart; S=$1; KIP=$2; DIL=$3; HED=$4; B1=$5; B2=$6
R="$HOME/opus5.5/plasebo12/tab/$S/ref_1111.ref $HOME/opus5.5/plasebo12/tab/$S/ref_2222.ref $HOME/opus5.5/plasebo12/tab/$S/ref_0444.ref $HOME/opus5.5/plasebo12/tab/$S/ref_0888.ref"
O=sina/cpu/${S}_${KIP}_$(basename $DIL .txt); rm -rf $O; mkdir -p $O/ozgun $O/emul
t0=$(date +%s); ESZAMANLI=1 bin/kontrol_$KIP $S $HED $DIL $B1 $B2 $O/ozgun $R 2>$O/ozgun.err; t1=$(date +%s)
bin/kart_${KIP}_emul $S $HED $DIL $B1 $B2 $O/emul $R 2>$O/emul.err; t2=$(date +%s)
echo "$O: özgün $((t1-t0)) sn, öykünme $((t2-t1)) sn"
fark=0
for f in $(cd $O/ozgun && ls); do
  case $f in *.txt) if ! diff <(sed 's/ sure_s [0-9.]*//' $O/ozgun/$f) <(sed 's/ sure_s [0-9.]*//' $O/emul/$f) >/dev/null; then echo "FARK $f"; fark=1; fi;;
  *) cmp -s $O/ozgun/$f $O/emul/$f || { echo "FARK $f"; fark=1; };; esac
done
for f in $(cd $O/emul && ls); do [ -e $O/ozgun/$f ] || [ $f = kart_gunlugu.txt ] || { echo "FAZLA $f"; fark=1; }; done
echo "dosya: $(ls $O/ozgun | wc -l) | fark $fark"; head -c 300 $O/ozgun/b$(printf %04d $B1).txt
