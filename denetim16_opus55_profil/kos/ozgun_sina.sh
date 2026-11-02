#!/bin/bash
# ozgun_sina.sh — ONKAYIT §6.3: her sıfırda rastgele 3 bölüt (seçim ön-kaydın SHA-256'sından tohumlanır) ve Kur'an'ın isabet aldığı bütün
# bölütler, özgün kutuphane.h yoluyla (olay_ozgun, tek iş parçacığı) yeniden sayılır ve kartın bölüt dosyasıyla satır satır karşılaştırılır.
set -eu; K=~/hrf_olcum/profil; cd $K; O=sonuc/ozgun_sina; mkdir -p $O
python3 -I - > $O/liste.txt <<'PY'
import random, hashlib, glob, os
r = random.Random(int(hashlib.sha256(open('ONKAYIT.md', 'rb').read()).hexdigest(), 16))
A = {1: (1, 4), 2: (5, 40), 3: (41, 400), 4: (401, 40000)}
for S in 'MLU':
    bs = [(b, 'rastgele') for b in r.sample(range(1, 40001), 3)]
    bs += [(int(os.path.basename(f)[1:].split('.')[0]), 'Kur\'an isabeti') for f in sorted(glob.glob(f'tab/{S}/d4/b*.q')) if os.path.getsize(f)]
    for b, neden in bs:
        j = next(j for j, (x, y) in A.items() if x <= b <= y); print(S, b, j, neden.replace(' ', '_'))
PY
cat $O/liste.txt
while read S b j neden; do echo "$S $b $j"; done < $O/liste.txt | xargs -P 8 -L 1 bash -c 'printf -v B "%04d" $1; nice -n 10 bin/olay_ozgun $0 tab/$0/d$2/olaylar.txt $1 > sonuc/ozgun_sina/$0_b$B.txt'
while read S b j neden; do printf -v B "%04d" $b; F=tab/$S/d$j/b$B.txt
  if diff <(grep '^D ' $F) $O/${S}_b$B.txt > /dev/null; then echo "$S bölüt $b (düzey $j, $neden): $(grep -c '^D ' $F) sıfırdan büyük olay — AYNI"; else echo "$S bölüt $b: FARKLI"; fi
done < $O/liste.txt | tee $O/sonuc.txt
