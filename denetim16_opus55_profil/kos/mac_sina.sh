#!/bin/bash
# Mac (geliştirme verisi: K×100'ün görülmüş hedefleri): yamalı dil_b = özgün; olay_kur; kart_olay öykünmesi = olay_ozgun (bağımsız yol).
cd ~/opus5.5/profil/sina; S=${1:-U}; NS=${2:-60}; B=${3:-1}
R=~/opus5.5/plasebo12/tab/$S; REF="$R/ref_1111.ref $R/ref_2222.ref $R/ref_0444.ref $R/ref_0888.ref"; H=~/opus5.5/k100/sonuc/tab/$S/hedef.bin
HEDEF_SON=$NS ../bin/dil_b A $H x dA.txt aA.txt $REF; HEDEF_SON=$NS ../bin/dil_b_ozgun A $H x dA_oz.txt aA_oz.txt $REF; HEDEF_SON=$NS DIL_ASAL=19 ../bin/dil_b A $H x dA19.txt aA19.txt $REF
cmp dA.txt dA_oz.txt && cmp aA.txt aA_oz.txt && echo "yamalı dil_b (DIL_ASAL yok) = özgün dil_b"
head -$((NS+1)) ~/opus5.5/k100/sonuc/tab/$S/diller_A.txt | cmp - dA.txt && echo "= K×100 diller_A ilk $((NS+1)) satır"
echo "dilli: tüm asallar $(awk '$4>=0' dA.txt | wc -l), yalnız 19 $(awk '$4>=0' dA19.txt | wc -l)"
../bin/olay_kur $H dA.txt dA19.txt olaylar.txt $REF; grep "^A000" olaylar.txt; echo "Kur'an Kol A eşiği (ayrıntı): $(grep '^A000 ' aA.txt | grep -o 'esik [0-9.]*')"
mkdir -p emul; rm -f emul/*; ../bin/kart_olay_emul $S $H olaylar.txt $B $B emul > /dev/null; ../bin/olay_ozgun $S olaylar.txt $B > ozgun.txt
diff <(grep '^D' emul/b$(printf %04d $B).txt) ozgun.txt > /dev/null && echo "kart_olay öykünmesi = olay_ozgun (bölüt $B, $(wc -l < ozgun.txt) olayda isabet, $(wc -l < olaylar.txt) olay)" || echo "FARK"
head -1 emul/b$(printf %04d $B).txt
