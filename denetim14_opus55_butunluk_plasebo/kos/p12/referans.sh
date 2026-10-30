#!/bin/zsh
# referans.sh <sifir> — ONKAYIT §3.1: 10⁸ referans kitabı (20 bölüt × 5×10⁶, tohum türü 1), biçimler 4 sürece bölünmüş; kontrol noktası 5 bölütte bir.
# Sürdürülebilir: tamamlanmış süreç (.say) atlanır; yarım süreç kontrol noktasından devam eder.
cd "${0:A:h}/.."
S=$1; mkdir -p tab/$S
log(){ print -r -- "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] $*" >> sonuc/kosu_gunlugu.txt }
log "referans $S başladı"
for g in 1111 2222 0444 0888; do
  if [[ -f tab/$S/ref_$g.say ]]; then log "referans $S $g zaten tamam"; continue; fi
  ./src/referans $S 1 1 20 5000000 $g tab/$S/ref_$g 5 2> tab/$S/ref_$g.err &
done
wait
for g in 1111 2222 0444 0888; do log "referans $S $g: $(cat tab/$S/ref_$g.say 2>/dev/null || echo EKSİK)"; done
log "referans $S bitti"
