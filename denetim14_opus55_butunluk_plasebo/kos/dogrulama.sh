#!/bin/zsh
# dogrulama.sh <geçici_dizin> — dondurma öncesi kod doğrulamaları (ONKAYIT §0, §7.3). Yalnız HEDEF OLMAYAN kitaplar (tür 4, sıra ≥ 100)
# ve mekanik kontrol tohumları (tür 8, bölüt 101–104) kullanılır; yalnız uyuşmazlık sayıları ve önceden yazılmış kapsam bayrakları
# basılır (sonuc/dogrulama/). Diller, eşikler ve sayımlar geçici dizinde kalır, basılmaz. En çok 3 süreç aynı anda.
cd "${0:A:h}/.."
G=${1:?geçici dizin}; mkdir -p $G sonuc/dogrulama; P12=~/opus5.5/plasebo12
ref(){ print -r -- $P12/tab/$1/ref_1111.ref $P12/tab/$1/ref_2222.ref $P12/tab/$1/ref_0444.ref $P12/tab/$1/ref_0888.ref }
# (1) kaba-kuvvet D_B ve en geniş dil: 2.000 kitap / sıfır
for S in M U L; do ./kontrol/hedef_dogrulama $S 2000 $G/dog2000_$S.bin & done; wait
for S in M U L; do ( ./kontrol/dilb_kaba $G/dog2000_$S.bin 0 1999 ${=$(ref $S)} > sonuc/dogrulama/dilb_kaba_$S.txt; print "rc=$?" >> sonuc/dogrulama/dilb_kaba_$S.txt ) & done; wait
# (2) hedef olmayan 201 kitabın dilleri (dört kol)
for S in M U L; do ./kontrol/hedef_dogrulama $S 201 $G/dog_$S.bin; done
for S in M U L; do ( for k in A Am Ap B; do ./src/dil_b $k $G/dog_$S.bin x $G/dil_${k}_$S.txt $G/ay_${k}_$S.txt ${=$(ref $S)}; done ) & done; wait
# (3) mekanik kontrol sınaması: kitap 0–29'un A, A−, A+ dilleri ve kitap 0–9'un Kol B dili; tür-8 bölütler 101–104 (16.000 kontrol)
for S in M U L; do ( D=$G/mek_$S; mkdir -p $D; find $D -name "b*.txt" -delete; find $D -name "b*.q" -delete
  { for k in A Am Ap; do awk '$2<=29' $G/dil_${k}_$S.txt; done; awk '$2<=9' $G/dil_B_$S.txt } > $D/diller.txt
  cat $G/ay_A_$S.txt $G/ay_Am_$S.txt $G/ay_Ap_$S.txt $G/ay_B_$S.txt > $D/ayrinti.txt
  ESZAMANLI=1 ./kontrol/kontrol_b_test $S $G/dog_$S.bin $D/diller.txt 101 104 $D ${=$(ref $S)} 2> $D/hata.txt
  ./kontrol/kontrolb_sinama $S $G/dog_$S.bin $D/diller.txt $D/ayrinti.txt 101 104 $D ${=$(ref $S)} > sonuc/dogrulama/kontrolb_sinama_$S.txt; print "rc=$?" >> sonuc/dogrulama/kontrolb_sinama_$S.txt ) & done; wait
# (4) hedef_ayrinti_b çalışırlık sınaması (çıktı basılmaz; yalnız satır sayısı eşitliği)
for S in M U L; do ( D=$G/mek_$S; ./src/hedef_ayrinti_b $G/dog_$S.bin $D/diller.txt $D/hedef_ayrinti.txt ${=$(ref $S)}; rc=$?
  print -r -- "hedef_ayrinti_b $S: girdi $(wc -l < $D/diller.txt | tr -d ' ') dil satırı, çıktı $(wc -l < $D/hedef_ayrinti.txt | tr -d ' ') satır, rc=$rc" > sonuc/dogrulama/hedef_ayrinti_$S.txt ) & done; wait
cat sonuc/dogrulama/*.txt
