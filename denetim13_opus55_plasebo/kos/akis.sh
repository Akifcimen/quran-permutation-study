#!/bin/zsh
# akis.sh <sifir> — bir sıfırın tam akışı (ONKAYIT §6.3): referans → Ö0 bilgi çapası (U ve M) → D ve dondurma → kontrol düzeyleri → analiz.
cd "${0:A:h}/.."
S=$1; T=tab/$S
log(){ print -r -- "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] $*" >> sonuc/kosu_gunlugu.txt }
./kos/referans.sh $S
for g in 1111 2222 0444 0888; do [[ -f $T/ref_$g.say ]] || { log "akış $S: referans eksik"; exit 2 } done
if [[ $S == M ]]; then ./kontrol/o0_bilgi ~/opus5.5/butunluk/tab/M129.tab 27.408142 -- $T/ref_*.ref > sonuc/o0_bilgi_M.txt; rc=$?; fi
if [[ $S == U ]]; then ./kontrol/o0_bilgi ~/opus5.5/butunluk/tab/U129.tab 31.057354 2.30712e-06 46 20000000 -- $T/ref_*.ref > sonuc/o0_bilgi_U.txt; rc=$?; fi
if [[ $S == M || $S == U ]]; then log "Ö0 bilgi çapası $S: rc=$rc"; [[ $rc == 0 ]] || { log "akış $S: Ö0 çapası tutmadı — durdu"; exit 3 } fi
./kos/dil.sh $S || exit 2
for d in 1 2 3; do python3 -I kos/kontrol.py $S $d || exit 2; done
mkdir -p sonuc/$S; python3 -I src/analiz.py $S > sonuc/$S/analiz_cikti.txt 2>&1
log "akış $S bitti"
