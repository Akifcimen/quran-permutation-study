#!/bin/zsh
# derle.sh [çıktı_dizini] — plasebo14'ün bütün C programlarını derler (Apple clang, arm64; macOS). Bayraklar plasebo12'ninkiyle aynı
# (-O3 -std=gnu11 -w). Çıktı dizini verilirse ikililer oraya, verilmezse yerlerine (src/, kontrol/) yazılır.
cd "${0:A:h}/.."
O=${1:-}
[[ -n $O ]] && mkdir -p $O
CF=(-O3 -std=gnu11 -w)
b(){ local out=$1; shift; [[ -n $O ]] && out=$O/${out:t}; cc $CF -o $out "$@" -lm -lpthread || { print -u2 "derleme hatası: $out"; exit 2 } }
# üretim hattı
b src/dil_b            src/dil_b.c
b src/kontrol_b        -DDMAX=64 src/kontrol_b.c
b src/hedef_ayrinti_b  -DDMAX=64 src/hedef_ayrinti_b.c
# plasebo12'nin değiştirilmemiş programları (Ö0 karşılaştırmaları için)
b src/dil_cikar        src/dil_cikar.c
b src/kontrol          src/kontrol.c
# dondurma öncesi doğrulamalar (hedef olmayan kitaplar; yalnız uyuşmazlık sayıları)
b kontrol/hedef_dogrulama kontrol/hedef_dogrulama.c
b kontrol/dilb_kaba       kontrol/dilb_kaba.c
b kontrol/kontrolb_sinama -DDMAX=64 kontrol/kontrolb_sinama.c
# kontrol programının mekanik sınama derlemesi (tür 8 tohumları, bölüt 4.000 kitap) ve hız ölçümü derlemesi (tür 6)
b kontrol/kontrol_b_test  -DDMAX=64 -DKONTROL_TUR=8 -DBN=4000 src/kontrol_b.c
b kontrol/kontrol_b_hiz   -DDMAX=64 -DKONTROL_TUR=6 -DBN=250000 src/kontrol_b.c
print "derlendi${O:+ → $O}"
