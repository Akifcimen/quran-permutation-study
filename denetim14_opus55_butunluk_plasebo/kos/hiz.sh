#!/bin/zsh
# hiz.sh <geçici_dizin> — dondurma öncesi hız ve bellek ölçümü (ONKAYIT §0, §8.2). Kontroller tür 6 tohumlarından (kontrol_b_hiz: bölüt 250.000 kitap,
# bölüt 1–4, 4 iş parçacığı); diller kos/dogrulama.sh'in hedef olmayan 201 kitabından. Yalnız süreler ve bellek basılır.
cd "${0:A:h}/.."
G=${1:?geçici dizin}; P12=~/opus5.5/plasebo12; O=sonuc/dogrulama/hiz.txt; : > $O
ref(){ print -r -- $P12/tab/$1/ref_1111.ref $P12/tab/$1/ref_2222.ref $P12/tab/$1/ref_0444.ref $P12/tab/$1/ref_0888.ref }
kos(){ local S=$1 D=$2 F=$3; mkdir -p $D; find $D -name "b*.txt" -delete; find $D -name "b*.q" -delete; local t0=$(date +%s)
  ESZAMANLI=4 ./kontrol/kontrol_b_hiz $S $G/dog_$S.bin $F 1 4 $D ${=$(ref $S)} 2> $D/hata.txt
  print -r -- "$(( $(date +%s) - t0 )) $(awk '{for(i=1;i<=NF;i++) if($i=="sure_s") s+=$(i+1)} END{printf "%.3f", s/4/250000*1e6}' $D/b000[1-4].txt)" }
nd(){ awk '$4>=0 && $5>0' $1 | wc -l | tr -d ' ' }
for S in M U L; do : > $G/bos.txt; r=($(kos $S $G/hiz0_$S $G/bos.txt)); print -r -- "taban (profil + kapı listesi) $S: ${r[2]} µs/kontrol (çekirdek), duvar ${r[1]} s / 10⁶ kontrol" >> $O; done
S=M; cat $G/dil_A_$S.txt $G/dil_Am_$S.txt $G/dil_Ap_$S.txt > $G/hizA.txt; b0=$(grep "taban.* $S:" $O | awk '{print $(NF-6)}')
r=($(kos $S $G/hizA_$S $G/hizA.txt)); n=$(nd $G/hizA.txt)
print -r -- "Kol A/A−/A+ $S: dil başına ek $(awk -v t=${r[2]} -v b=$b0 -v n=$n 'BEGIN{printf "%.4f", (t-b)/n}') µs/kontrol" >> $O
KONTROL_BELLEK=1 ./kontrol/kontrol_b_hiz $S $G/dog_$S.bin $G/dil_B_$S.txt 1 1 $G/hizB_$S ${=$(ref $S)} > $G/bellekB.txt
awk -v lim=$((3500*1024*1024)) '{b=$3; if(b>m) m=b; t+=b; if(cur+b>lim){ np++; cur=0 } cur+=b; if(np==0) n1++} END{ printf "Kol B bellek: dil başına en çok %.0f MB, toplam %.0f MB, 3,5 GB partileri %d\n", m/1048576, t/1048576, np+1 }' $G/bellekB.txt >> $O
awk -v lim=$((3500*1024*1024)) '{b=$3; if(cur+b>lim) exit; cur+=b; print $2}' $G/bellekB.txt > $G/parti1.txt
awk 'NR==FNR{a[$1]=1; next} ($1 in a)' $G/parti1.txt $G/dil_B_$S.txt > $G/hizB.txt
r=($(kos $S $G/hizB_$S $G/hizB.txt)); n=$(nd $G/hizB.txt)
print -r -- "Kol B $S (ilk parti): dil başına ek $(awk -v t=${r[2]} -v b=$b0 -v n=$n 'BEGIN{printf "%.3f", (t-b)/n}') µs/kontrol; parti kuruluşu + 10⁶ kontrol duvar ${r[1]} s" >> $O
cat $O
