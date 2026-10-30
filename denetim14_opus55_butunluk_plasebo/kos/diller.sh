#!/bin/zsh
# diller.sh — ONKAYIT §4.5: ön-kayıt dondurulduktan sonra ve HİÇBİR kontrol geçişinden önce bütün kolların dilleri kurulur, salt okunur
# yapılır ve SHA-256'ları sonuc/diller_sha256.txt'e yazılır. Kollar: M20: A, A−, A+, B; tekdüze ve Mallows: A, B. Sonra kontrol kümeleri
# (kA, kB; dilsiz hedefler hariç) ve betimsel hedef ayrıntıları yazılır. En çok 3 süreç aynı anda.
cd "${0:A:h}/.."
P12=~/opus5.5/plasebo12
log(){ print -r -- "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] $*" >> sonuc/kosu_gunlugu.txt }
ref(){ print -r -- $P12/tab/$1/ref_1111.ref $P12/tab/$1/ref_2222.ref $P12/tab/$1/ref_0444.ref $P12/tab/$1/ref_0888.ref }
kollar(){ [[ $1 == M ]] && print -r -- "A Am Ap B" || print -r -- "A B" }
if [[ -f sonuc/diller_sha256.txt ]]; then log "diller zaten donmuş"; exit 0; fi
log "diller başladı"
for S in M U L; do (
  for k in ${=$(kollar $S)}; do t0=$(date +%s)
    ./src/dil_b $k tab/$S/hedef.bin x tab/$S/diller_$k.txt tab/$S/ayrinti_$k.txt ${=$(ref $S)} || exit 2
    chmod 444 tab/$S/diller_$k.txt tab/$S/ayrinti_$k.txt
    log "dil $S $k: $(wc -l < tab/$S/diller_$k.txt | tr -d ' ') satır, $(awk '$4>=0 && $5>0' tab/$S/diller_$k.txt | wc -l | tr -d ' ') dilli, $(( $(date +%s) - t0 )) s"
  done ) & done; wait
shasum -a 256 tab/*/diller_*.txt tab/*/ayrinti_*.txt > sonuc/diller_sha256.txt; chmod 444 sonuc/diller_sha256.txt
log "diller donduruldu: $(wc -l < sonuc/diller_sha256.txt | tr -d ' ') dosya; sonuc/diller_sha256.txt SHA-256 $(shasum -a 256 sonuc/diller_sha256.txt | cut -c1-16)…"
for S in M U L; do mkdir -p tab/$S/kA tab/$S/kB
  if [[ $S == M ]]; then cat tab/$S/diller_A.txt tab/$S/diller_Am.txt tab/$S/diller_Ap.txt; else cat tab/$S/diller_A.txt; fi | awk '$4>=0 && $5>0' > tab/$S/kA/diller.txt
  awk '$4>=0 && $5>0' tab/$S/diller_B.txt > tab/$S/kB/diller.txt
  chmod 444 tab/$S/kA/diller.txt tab/$S/kB/diller.txt; done
for S in M U L; do ( for k in ${=$(kollar $S)}; do ./src/hedef_ayrinti_b tab/$S/hedef.bin tab/$S/diller_$k.txt tab/$S/hedef_ayrinti_$k.txt ${=$(ref $S)}; done ) & done; wait
log "diller bitti (kontrol kümeleri ve hedef ayrıntıları yazıldı)"
