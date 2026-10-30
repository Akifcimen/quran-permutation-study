#!/bin/zsh
# yeniden_sina.sh <geçici_dizin> — oturum sonu yeniden derleme sınaması (ONKAYIT §7.1; dondurmadan sonra eklendi, sonucu değiştirmez):
# kos/derle.sh ile geçici dizine yeniden derlenmiş ikililer (a) Ö0'ı yineler: kısıtsız D üç sıfırda plasebo12'nin diller.txt/ayrinti.txt'si,
# kontrol (kol 0) bölüt 1'de plasebo12'nin esikler/b0001.txt/b0001.q'su; (b) bu oturumun üretim çıktılarını yeniden üretir: üç sıfırın
# bütün kol dilleri ve ayrıntıları, M20 Kol A kümesinin düzey-1 bölüt 1'i (b0001.txt başlık hariç, b0001.q, esikler.txt).
# Tablolar ve hedefler yerinde okunur; hiçbir sonuç dosyası yazılmaz. Sonuç: <geçici_dizin>/rapor.txt. En çok 4 iş parçacığı.
cd "${0:A:h}/.."
G=${1:?geçici dizin}; mkdir -p $G; I=$G/ikili; R=$G/rapor.txt; : > $R; P12=~/opus5.5/plasebo12
./kos/derle.sh $I > /dev/null || exit 2
ok(){ print -r -- "$1: $2" >> $R }
ref(){ print -r -- $P12/tab/$1/ref_1111.ref $P12/tab/$1/ref_2222.ref $P12/tab/$1/ref_0444.ref $P12/tab/$1/ref_0888.ref }
# (a) Ö0
for S in M U L; do ( $I/dil_b P tab/$S/hedef.bin 0 $G/d0_$S.txt $G/a0_$S.txt ${=$(ref $S)}; HEDEF_SON=0 $I/dil_b P tab/$S/hedef.bin 1 $G/dP_$S.txt $G/aP_$S.txt ${=$(ref $S)} ) & done; wait
for S in M U L; do { cat $G/d0_$S.txt $G/dP_$S.txt; print "EL -1 0 -1 20 0 1 7 11 12 13 14 15 16 17 18 19 20 21 65 66 67 68 69 70" } > $G/dil_$S.txt; cat $G/a0_$S.txt $G/aP_$S.txt > $G/ay_$S.txt
  cmp -s $G/dil_$S.txt $P12/tab/$S/diller.txt && cmp -s $G/ay_$S.txt $P12/tab/$S/ayrinti.txt && ok "Ö0 kısıtsız D $S" AYNI || ok "Ö0 kısıtsız D $S" FARKLI; done
for S in M U L; do ( mkdir -p $G/k1_$S; ESZAMANLI=1 $I/kontrol_b $S tab/$S/hedef.bin $P12/tab/$S/k1/diller.txt 1 1 $G/k1_$S ${=$(ref $S)} 2> /dev/null ) & done; wait
for S in M U L; do D=$G/k1_$S
  cmp -s $D/esikler.txt $P12/tab/$S/k1/esikler.txt && diff -q <(sed 1d $D/b0001.txt) <(sed 1d $P12/tab/$S/k1/b0001.txt) > /dev/null && cmp -s $D/b0001.q $P12/tab/$S/k1/b0001.q && ok "Ö0 kontrol bölüt 1 $S" AYNI || ok "Ö0 kontrol bölüt 1 $S" FARKLI; done
# (b) bu oturumun çıktıları
for S in M U L; do ( for k in $( [[ $S == M ]] && print "A Am Ap B" || print "A B" ); do $I/dil_b $k tab/$S/hedef.bin x $G/dl_${k}_$S.txt $G/ay_${k}_$S.txt ${=$(ref $S)}; done ) & done; wait
for S in M U L; do for k in $( [[ $S == M ]] && print "A Am Ap B" || print "A B" ); do
  cmp -s $G/dl_${k}_$S.txt tab/$S/diller_$k.txt && cmp -s $G/ay_${k}_$S.txt tab/$S/ayrinti_$k.txt && ok "diller $S $k" AYNI || ok "diller $S $k" FARKLI; done; done
mkdir -p $G/kA1; ESZAMANLI=4 $I/kontrol_b M tab/M/hedef.bin tab/M/kA/d1_0/diller.txt 1 1 $G/kA1 ${=$(ref M)} 2> /dev/null
cmp -s $G/kA1/esikler.txt tab/M/kA/d1_0/esikler.txt && diff -q <(sed 1d $G/kA1/b0001.txt) <(sed 1d tab/M/kA/d1_0/b0001.txt) > /dev/null && cmp -s $G/kA1/b0001.q tab/M/kA/d1_0/b0001.q && ok "M20 Kol A düzey 1 bölüt 1" AYNI || ok "M20 Kol A düzey 1 bölüt 1" FARKLI
if [[ -d tab/M/kB/d1_0 ]]; then mkdir -p $G/kB1; ESZAMANLI=4 $I/kontrol_b M tab/M/hedef.bin tab/M/kB/d1_0/diller.txt 1 1 $G/kB1 ${=$(ref M)} 2> /dev/null
  cmp -s $G/kB1/esikler.txt tab/M/kB/d1_0/esikler.txt && diff -q <(sed 1d $G/kB1/b0001.txt) <(sed 1d tab/M/kB/d1_0/b0001.txt) > /dev/null && cmp -s $G/kB1/b0001.q tab/M/kB/d1_0/b0001.q && ok "M20 Kol B düzey 1 bölüt 1" AYNI || ok "M20 Kol B düzey 1 bölüt 1" FARKLI; fi
cat $R
