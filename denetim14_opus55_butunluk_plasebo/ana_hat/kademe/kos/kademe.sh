#!/bin/bash
# kademe.sh <S2|S3|S4|S5> — ONKAYIT §4: bir kademenin yeni bölütlerini M → U → L sırasıyla sayar (kontrol_h, 12 iş parçacığı, nice 10).
# Bölütler 10.000lik parçalara bölünür; parça bitince bölüt .txt dosyaları sırayla parca.txt içine birleştirilir (.q/.s dosyaları kalır).
# Sonra her sıfırda özgün kontrol_b ile 4 bölütlük örneklem sınaması (ilk, %25, %75, son; ONKAYIT §6.2).
set -u
cd ~/hrf_olcum
K=$1
case $K in S2) B1=401; B2=4000;; S3) B1=4001; B2=40000;; S4) B1=40001; B2=400000;; S5) B1=400001; B2=4000000;; *) echo "kademe?"; exit 1;; esac
G=sonuc/kademe_gunlugu.txt
log(){ echo "[$(date -u +%FT%TZ)] $*" >> $G; }
log "$K başladı: bölüt $B1–$B2"
for s in M U L; do
  R="girdi/ref/$s/ref_1111.ref girdi/ref/$s/ref_2222.ref girdi/ref/$s/ref_0444.ref girdi/ref/$s/ref_0888.ref"
  b=$B1
  while [ $b -le $B2 ]; do
    e=$(( (b-1)/10000*10000 + 10000 )); [ $e -gt $B2 ] && e=$B2
    D=sonuc/$K/$s/p$(printf %07d $b); mkdir -p $D
    if [ ! -f $D/parca.txt ]; then
      ESZAMANLI=12 nice -n 10 bin/kontrol_h $s girdi/$s/hedef.bin girdi/$s/diller_Q.txt $b $e $D $R 2>> $D/hata.txt || { log "$K $s $b–$e: kontrol_h hatası"; exit 2; }
      n=$(find $D -maxdepth 1 -name "b*.txt" | wc -l); [ $n -eq $((e-b+1)) ] || { log "$K $s $b–$e: eksik bölüt ($n)"; exit 2; }
      find $D -maxdepth 1 -name "b*.txt" | sort | xargs cat > $D/parca.yaziliyor && mv $D/parca.yaziliyor $D/parca.txt && find $D -maxdepth 1 -name "b*.txt" -delete
      log "$K $s $b–$e bitti: $(awk "\$1==\"D\"{k[\$2]+=\$3; kb[\$2]+=\$4} END{for(x in k) printf \"%s k=%d kB=%d \", x, k[x], kb[x]}" $D/parca.txt)"
    fi
    b=$((e+1))
  done
done
log "$K sayım bitti; örneklem sınaması (kontrol_b) başlıyor"
for s in M U L; do
  R="girdi/ref/$s/ref_1111.ref girdi/ref/$s/ref_2222.ref girdi/ref/$s/ref_0444.ref girdi/ref/$s/ref_0888.ref"
  D=sonuc/$K/ornek_b/$s; mkdir -p $D
  for b in $B1 $((B1+(B2-B1)/4)) $((B1+3*(B2-B1)/4)) $B2; do
    ESZAMANLI=1 nice -n 10 bin/kontrol_b $s girdi/$s/hedef.bin girdi/$s/diller_Q.txt $b $b $D $R 2>> $D/hata.txt &
  done; wait
done
log "$K BİTTİ"
