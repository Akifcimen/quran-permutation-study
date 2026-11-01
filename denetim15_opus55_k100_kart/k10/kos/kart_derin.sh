#!/bin/bash
# kart_derin.sh — ön-kayıt §3 ikincil derinleştirmesinin (derin.sh) kalan bölütleri, eşdeğerliği kabul edilmiş kart sayacıyla
# (~/hrf_olcum/kart/ek/EK_20261031T2010Z_kart_tasarim.md; kabul sınaması sina_4090.txt, sina_3090.txt). Diller, dizinler, bölüt aralığı
# ve sıra derin.sh ile aynıdır; var olan bölütler atlanır (işlemcinin yazdıkları korunur). İki kart aynı anda: bölüt b mod 5 ∈ {0,1,2}
# RTX 4090'a, {3,4} RTX 3090'a. Kartları süren 2 işlemci iş parçacığı (bloklayan eşitleme) 12 sınırına sayılır.
set -u
H=~/hrf_olcum; K=$H/k10; LOG=$K/sonuc/kosu_gunlugu.txt; log(){ echo "[$(date -u +%FT%TZ)] $*" >> $LOG; }
export CUDA_DEVICE_ORDER=PCI_BUS_ID
ref(){ echo $H/girdi/ref/$1/ref_1111.ref $H/girdi/ref/$1/ref_2222.ref $H/girdi/ref/$1/ref_0444.ref $H/girdi/ref/$1/ref_0888.ref; }
declare -A RL=( [M]="A535 A1645 A690" [L]="A142 A786 A777 A891" [U]="A088 A358 A593 A925 A315 A491 A398" )
for S in M L U; do D=$K/tab/$S/kA/d4_0; mkdir -p $D
  [ -f $D/diller.txt ] || { for a in ${RL[$S]}; do grep "^$a " $K/tab/$S/kA/diller.txt; done > $D/diller.txt; chmod 444 $D/diller.txt; }
  log "kart derinleştirme $S düzey 4 (bölüt 401–4000; var olan $(ls $D | grep -c '^b.*txt$') bölüt atlanır) başladı: $(wc -l < $D/diller.txt) dil: ${RL[$S]}"
  CUDA_VISIBLE_DEVICES=1 KART_MOD=5 KART_ALT=0 KART_UST=2 KART_PARTI=${KART_PARTI:-720} nice -n 10 $H/kart/bin/kart_b $S $K/tab/$S/hedef.bin $D/diller.txt 401 4000 $D $(ref $S) 2>> $D/hata_kart1.txt & p1=$!
  CUDA_VISIBLE_DEVICES=0 KART_MOD=5 KART_ALT=3 KART_UST=4 KART_PARTI=${KART_PARTI:-720} nice -n 10 $H/kart/bin/kart_b $S $K/tab/$S/hedef.bin $D/diller.txt 401 4000 $D $(ref $S) 2>> $D/hata_kart0.txt & p0=$!
  wait $p1; r1=$?; wait $p0; r0=$?
  [ $r1 = 0 ] && [ $r0 = 0 ] || { log "kart derinleştirme $S hata (4090 rc=$r1, 3090 rc=$r0)"; exit 2; }
  log "kart derinleştirme $S bitti: $(ls $D | grep -c '^b.*txt$') bölüt"
done
log "DERİNLEŞTİRME BİTTİ"
