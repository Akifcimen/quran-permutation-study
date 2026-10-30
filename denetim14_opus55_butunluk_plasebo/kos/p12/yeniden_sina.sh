#!/bin/zsh
# yeniden_sina.sh <geçici_dizin> — kos/derle.sh ile yeniden derlenmiş ikililerin çıktılarını oturumun sonuç dosyalarıyla karşılaştırır
# (rapor §11). Tablolar ve hedefler yerinde okunur; hiçbir sonuç dosyası yazılmaz. Sonuç: <geçici_dizin>/rapor.txt.
# En çok 4 iş parçacığı: adımlar sırayla koşar; kontrol adımı ESZAMANLI=4.
cd "${0:A:h}/.."
G=${1:?geçici dizin}; mkdir -p $G; I=$G/ikili; R=$G/rapor.txt; : > $R
./kos/derle.sh $I > /dev/null || exit 2
ok(){ print -r -- "$1: $2" >> $R }
# (a) Ö0'ın Kur'an kısmı
$I/o0_kuran > $G/o0_kuran.txt; cmp -s $G/o0_kuran.txt sonuc/o0_kuran.txt && ok o0_kuran AYNI || ok o0_kuran FARKLI
# (b) hedefler
for S K in U 100 L 100 M 200; do $I/hedef $S $K $G/hedef_$S.bin
  [[ $(shasum -a 256 $G/hedef_$S.bin | cut -d' ' -f1) == $(grep "tab/$S/hedef.bin" sonuc/hedef_sha256.txt | cut -d' ' -f1) ]] && ok "hedef $S" AYNI || ok "hedef $S" FARKLI; done
RU=(tab/U/ref_1111.ref tab/U/ref_2222.ref tab/U/ref_0444.ref tab/U/ref_0888.ref)
# (c) D (tekdüze; P0 ve P2)
$I/dil_cikar tab/U/hedef.bin 0 x $G/diller0.txt $G/ayrinti0.txt $RU
cmp -s $G/diller0.txt tab/U/diller0.txt && cmp -s $G/ayrinti0.txt tab/U/ayrinti0.txt && ok "D U (P0)" AYNI || ok "D U (P0)" FARKLI
HEDEF_SON=0 $I/dil_cikar tab/U/hedef.bin 1 x $G/dillerP2.txt $G/ayrintiP2.txt $RU
cmp -s $G/dillerP2.txt tab/U/dillerP2.txt && cmp -s $G/ayrintiP2.txt tab/U/ayrintiP2.txt && ok "D U (P2)" AYNI || ok "D U (P2)" FARKLI
# (d) hedef ayrıntısı (tekdüze)
$I/hedef_ayrinti tab/U/hedef.bin tab/U/diller.txt $G/hedef_ayrinti.txt $RU
cmp -s $G/hedef_ayrinti.txt tab/U/hedef_ayrinti.txt && ok "hedef_ayrinti U" AYNI || ok "hedef_ayrinti U" FARKLI
# (e) kontrol: tekdüze bölüt 1, düzey-1 dilleri
mkdir -p $G/k1; ESZAMANLI=4 $I/kontrol U tab/U/hedef.bin tab/U/k1/diller.txt 1 1 $G/k1 $RU 2> $G/k1/hata.txt
diff <(sed 1d $G/k1/b0001.txt) <(sed 1d tab/U/k1/b0001.txt) > /dev/null && cmp -s $G/k1/b0001.q tab/U/k1/b0001.q && ok "kontrol U bölüt 1" AYNI || ok "kontrol U bölüt 1" FARKLI
# (f) referans: küçük bir tür-6 geçişi, oturumun ikilisiyle ve yeni ikiliyle
./src/referans U 6 1 1 100000 1111 $G/ref_eski 1 2> /dev/null; $I/referans U 6 1 1 100000 1111 $G/ref_yeni 1 2> /dev/null
cmp -s $G/ref_eski.ref $G/ref_yeni.ref && ok "referans (tür 6, 10⁵ kitap)" AYNI || ok "referans (tür 6, 10⁵ kitap)" FARKLI
# (g) Ö0 bilgi çapası (tekdüze)
$I/o0_bilgi ~/opus5.5/butunluk/tab/U129.tab 31.057354 2.30712e-06 46 20000000 -- $RU > $G/o0_bilgi_U.txt
cmp -s $G/o0_bilgi_U.txt sonuc/o0_bilgi_U.txt && ok "o0_bilgi U" AYNI || ok "o0_bilgi U" FARKLI
cat $R
