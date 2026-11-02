#!/bin/bash
# kabul.sh — kart_olay'ın kabul sınaması (ONKAYIT §6; geliştirme verisi: K×100'ün ilk 200 hedefi). Her sıfırda, her kartta, bölüt 1–2:
# kart_olay (kart) = kart_olay_emul (aynı çekirdek, işlemci) = olay_ozgun (özgün kutuphane.h yolu). .q dosyaları kart = öykünme.
set -u; export CUDA_DEVICE_ORDER=PCI_BUS_ID
H=~/hrf_olcum; K=$H/profil; O=$K/kabul; mkdir -p $O
ref(){ echo $H/girdi/ref/$1/ref_1111.ref $H/girdi/ref/$1/ref_2222.ref $H/girdi/ref/$1/ref_0444.ref $H/girdi/ref/$1/ref_0888.ref; }
for S in M L U; do D=$O/$S; mkdir -p $D; Hd=$H/k100/tab/$S/hedef.bin
  HEDEF_SON=200 nice -n 10 $K/bin/dil_b A $Hd x $D/dA.txt $D/aA.txt $(ref $S); HEDEF_SON=200 DIL_ASAL=19 nice -n 10 $K/bin/dil_b A $Hd x $D/dA19.txt $D/aA19.txt $(ref $S)
  head -201 $H/k100/tab/$S/diller_A.txt | cmp -s - $D/dA.txt && echo "$S: yamalı dil_b = K×100 diller_A (ilk 201)" || echo "$S: FARK dil_b"
  nice -n 10 $K/bin/olay_kur $Hd $D/dA.txt $D/dA19.txt $D/olaylar.txt $(ref $S) 2> $D/olay_kur.err; cat $D/olay_kur.err
  for b in 1 2; do nice -n 10 $K/bin/olay_ozgun $S $D/olaylar.txt $b > $D/ozgun_$b.txt & done
  mkdir -p $D/emul; nice -n 10 $K/bin/kart_olay_emul $S $Hd $D/olaylar.txt 1 2 $D/emul > /dev/null 2>&1 &
  for G in 1 0; do mkdir -p $D/kart$G; CUDA_VISIBLE_DEVICES=$G KART_PARTI=2 nice -n 10 $K/bin/kart_olay $S $Hd $D/olaylar.txt 1 2 $D/kart$G 2> $D/kart$G.err; done; wait
  for b in 1 2; do n=$(printf b%04d $b)
    a=$(diff <(grep '^D' $D/emul/$n.txt) $D/ozgun_$b.txt > /dev/null && echo aynı || echo FARK)
    for G in 1 0; do c=$(diff <(sed 's/ sure_s [0-9.]*//' $D/kart$G/$n.txt) <(sed 's/ sure_s [0-9.]*//' $D/emul/$n.txt) > /dev/null && cmp -s $D/kart$G/$n.q $D/emul/$n.q && echo aynı || echo FARK); a="$a kart$G:$c"; done
    echo "$S bölüt $b: öykünme=özgün $a ($(grep -c '^D' $D/emul/$n.txt) olayda isabet / $(wc -l < $D/olaylar.txt) olay)"; done
done
echo KABUL BITTI
