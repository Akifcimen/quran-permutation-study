#!/bin/bash
cd ~/hrf_olcum; mkdir -p sonuc/sina_h
for s in M U L; do mkdir -p sonuc/sina_h/$s; R="girdi/ref/$s/ref_1111.ref girdi/ref/$s/ref_2222.ref girdi/ref/$s/ref_0444.ref girdi/ref/$s/ref_0888.ref"
  /usr/bin/time -f "$s %e s" env ESZAMANLI=12 nice -n 10 bin/kontrol_h $s girdi/$s/hedef.bin girdi/$s/diller_Q.txt 1 400 sonuc/sina_h/$s $R; done > sonuc/sina_h/gunluk.txt 2>&1
echo BITTI >> sonuc/sina_h/gunluk.txt
