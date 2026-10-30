#!/bin/bash
# yeniden_L.sh — Mallows (L) sayımlarının yarış düzeltmeli sayaçlarla (orn_init kilit altında) yeniden koşusu ve özgünlerle bölüt bölüt karşılaştırması.
# Yarış: kutuphane.h'de orn_init(L) küresel REF_DESC'i yeniden yazar; iş parçacıkları aynı anda çağırınca bölütün referansı ve q'su bozulabilir.
set -u
cd ~/hrf_olcum; Y=yaris; LOG=$Y/sonuc/gunluk.txt; FARK=$Y/sonuc/farklar.txt
log(){ echo "[$(date -u +%FT%TZ)] $*" >> $LOG; }
ref(){ echo girdi/ref/L/ref_1111.ref girdi/ref/L/ref_2222.ref girdi/ref/L/ref_0444.ref girdi/ref/L/ref_0888.ref; }
norm(){ sed 's/ sure_s [0-9.]*//' "$1"; }
karsilastir(){ # $1 ad, $2 yeni dizin, $3.. özgün dizinler
  local ad=$1 yeni=$2; shift 2; local n=0 f=0 e=0 o b
  for od in "$@"; do for o in "$od"/b*.txt; do [ -f "$o" ] || continue; b=$(basename "$o"); n=$((n+1))
    if [ ! -f "$yeni/$b" ]; then e=$((e+1)); echo "$ad EKSİK $b" >> $FARK; continue; fi
    if ! cmp -s <(norm "$o") <(norm "$yeni/$b"); then f=$((f+1))
      echo "$ad FARK $b | özgün: $(head -1 "$o" | grep -o 'b2 [0-9]*') $(grep '^D' "$o" | tr '\n' ' ' | cut -c1-300) | yeni: $(head -1 "$yeni/$b" | grep -o 'b2 [0-9]*') $(grep '^D' "$yeni/$b" | tr '\n' ' ' | cut -c1-300)" >> $FARK; fi
  done; done; log "$ad karşılaştırma: $n bölüt, $f farklı, $e eksik"; }
toplam(){ cat "$1"/b*.txt 2>/dev/null | awk '$1=="D"{k[$2]+=$3; kb[$2]+=$4} END{for(x in k) printf "%s k=%d kB=%d ", x, k[x], kb[x]}'; }
kh(){ # $1 kapı, $2 b1, $3 b2
  local D=$Y/kuran/$1/L; mkdir -p $D; log "kontrol_h $1 L $2–$3 başladı"
  ESZAMANLI=12 nice -n 10 $Y/bin/kontrol_h_$1 L girdi/L/hedef.bin girdi/L/diller_Q.txt $2 $3 $D $(ref) 2>> $D/hata.txt || log "kontrol_h $1 hata"
  log "kontrol_h $1 L bitti: $(toplam $D)"; }
kb(){ # $1 kapı (ikili), $2 ad, $3 özgün k dizini (d*_0 içerir; diller oradan), $4 yeni kök
  local d r D
  for d in d1_0 d2_0 d3_0; do [ -f $3/$d/diller.txt ] || continue
    case $d in d1_0) r="1 4";; d2_0) r="5 40";; d3_0) r="41 400";; esac
    D=$4/$d; mkdir -p $D; log "kontrol_b $2 $d ($r) başladı: $(wc -l < $3/$d/diller.txt) dil"
    ESZAMANLI=12 nice -n 10 $Y/bin/kontrol_b_$1 L girdi/L/hedef.bin $3/$d/diller.txt $r $D $(ref) 2>> $D/hata.txt || log "kontrol_b $2 $d hata"
    log "kontrol_b $2 $d bitti"; done; }
log "YENİDEN L BAŞLADI"
kh Gc 1 40000;  karsilastir kuran_Gc $Y/kuran/Gc/L kapi/kuran/Gc/L
kh G2E 1 40000; karsilastir kademe_G2E $Y/kuran/G2E/L $Y/kademe_ozgun/L   # S1 sina_h + S2/S3 parca.txt bölütlere ayrılmış
kh GE 1 4000;   karsilastir kuran_GE $Y/kuran/GE/L kapi/kuran/GE/L
kh G0 1 4000;   karsilastir kuran_G0 $Y/kuran/G0/L kapi/kuran/G0/L
for G in Gc GE G0; do kb $G plasebo_$G kapi/$G/tab/L/kA $Y/plasebo/$G
  for d in d1_0 d2_0 d3_0; do karsilastir plasebo_${G}_$d $Y/plasebo/$G/$d kapi/$G/tab/L/kA/$d; done; done
kb G2E p14_kA $Y/p14/L/kA $Y/p14yeni/kA
kb G2E p14_kB $Y/p14/L/kB $Y/p14yeni/kB
log "YENİDEN L BİTTİ"
