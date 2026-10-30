#!/bin/zsh
# akis_b.sh — plasebo14'ün tam akışı (ONKAYIT §8.1 sırası): diller (bütün kollar, bütün sıfırlar; dondurulur) → Kol A kümesi (M20'de A, A−,
# A+; tekdüze ve Mallows'ta A) düzey 1–3 ve analiz, sıfır sırası M20, tekdüze, Mallows → Kol B M20 düzey 1–3 (bütçe kuralıyla) → Kol B
# tekdüze ve Mallows (yalnız Kol B M20 tamamlandıysa; bütçe kuralıyla). Sürdürülebilir (var olan bölütler atlanır).
cd "${0:A:h}/.."
log(){ print -r -- "[$(date -u +%Y-%m-%dT%H:%M:%SZ)] $*" >> sonuc/kosu_gunlugu.txt }
./kos/diller.sh || { log "akış: diller hatası"; exit 2 }
mkdir -p sonuc/M sonuc/U sonuc/L
for S in M U L; do
  for d in 1 2 3; do python3 -I kos/kontrol_b.py $S A $d || { log "akış: kontrol $S A $d hatası"; exit 2 }; done
  python3 -I src/analiz_b.py $S A > sonuc/$S/analiz_A_cikti.txt 2>&1; log "analiz $S A: rc=$?"
done
for S in M U L; do
  if [[ $S != M ]] && ! grep -q '"tamam": true' sonuc/M/analiz_B.json 2>/dev/null; then log "Kol B $S: Kol B M20 tamamlanmadı — koşulmadı (ONKAYIT §8.2)"; continue; fi
  for d in 1 2 3; do python3 -I kos/kontrol_b.py $S B $d || { log "akış: kontrol $S B $d hatası"; exit 2 }; done
  python3 -I src/analiz_b.py $S B > sonuc/$S/analiz_B_cikti.txt 2>&1; log "analiz $S B: rc=$?"
done
log "akış bitti"
