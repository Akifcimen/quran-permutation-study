#!/bin/zsh
# dil.sh <sifir> — ONKAYIT §4: her hedefe D(x) (P0), Kur'an'a ayrıca P2 (§9 e); el dili satırı eklenir; diller dondurulur (§4.4).
cd "${0:A:h}/.."
S=$1; T=tab/$S; R=($T/ref_1111.ref $T/ref_2222.ref $T/ref_0444.ref $T/ref_0888.ref)
log(){ print -r -- "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] $*" >> sonuc/kosu_gunlugu.txt }
if [[ -f $T/diller.txt ]]; then log "dil $S: diller zaten donmuş"; exit 0; fi
log "dil $S başladı"
./src/dil_cikar $T/hedef.bin 0 x $T/diller0.txt $T/ayrinti0.txt $R || exit 2
HEDEF_SON=0 ./src/dil_cikar $T/hedef.bin 1 x $T/dillerP2.txt $T/ayrintiP2.txt $R || exit 2
{ cat $T/diller0.txt $T/dillerP2.txt; print "EL -1 0 -1 20 0 1 7 11 12 13 14 15 16 17 18 19 20 21 65 66 67 68 69 70" } > $T/diller.txt
cat $T/ayrinti0.txt $T/ayrintiP2.txt > $T/ayrinti.txt
chmod 444 $T/diller.txt $T/ayrinti.txt
shasum -a 256 $T/diller.txt $T/ayrinti.txt > sonuc/diller_${S}_sha256.txt
log "dil $S: $(wc -l < $T/diller.txt) satır donduruldu, SHA-256 $(shasum -a 256 $T/diller.txt | cut -c1-16)…"
./src/hedef_ayrinti $T/hedef.bin $T/diller.txt $T/hedef_ayrinti.txt $R || exit 2
log "dil $S bitti"
