#!/bin/zsh
# derle.sh [çıktı_dizini] — bütün C programlarını derler (Apple clang 17, arm64; macOS). Çıktı dizini verilirse ikililer oraya,
# verilmezse yerlerine (src/, kontrol/) yazılır. Sonuç dosyaları derleme bayraklarına bağlı olmamalıdır: bu betikle yeniden derlenmiş
# ikililerin çıktıları oturumda kullanılan ikililerinkiyle karşılaştırıldı (rapor §11).
cd "${0:A:h}/.."
O=${1:-}
[[ -n $O ]] && mkdir -p $O
CF=(-O3 -std=gnu11 -w)
b(){ local out=$1; shift; [[ -n $O ]] && out=$O/${out:t}; cc $CF -o $out "$@" -lm -lpthread || { print -u2 "derleme hatası: $out"; exit 2 } }
# üretim hattı
b src/referans        src/referans.c
b src/hedef           src/hedef.c
b src/dil_cikar       src/dil_cikar.c
b src/kontrol         src/kontrol.c
b src/hedef_ayrinti   src/hedef_ayrinti.c
b src/kuran_profil    src/kuran_profil.c
# Ö0 çapaları
b kontrol/o0_karsilastir kontrol/o0_karsilastir.c kontrol/o0_b12.c kontrol/o0_kut.c
b kontrol/o0_kuran       kontrol/o0_kuran.c kontrol/o0_b12.c kontrol/o0_kut.c
b kontrol/o0_bilgi       kontrol/o0_bilgi.c kontrol/o0b_b12.c kontrol/o0b_yeni.c
# dondurma öncesi doğrulamalar ve raporlama
b kontrol/c_dok          kontrol/c_dok.c
b kontrol/c_dok_havuz    kontrol/c_dok_havuz.c
b kontrol/sabit_tarama   kontrol/sabit_tarama.c
b kontrol/dil_mekanik    kontrol/dil_mekanik.c
b kontrol/dil_kaba       kontrol/dil_kaba.c
b kontrol/kontrol_sinama kontrol/kontrol_sinama.c
b kontrol/hedef_test     kontrol/hedef_test.c
b kontrol/hiz            kontrol/hiz.c
b kontrol/aday_siniri    kontrol/aday_siniri.c
# kontrol programının mekanik sınama derlemesi (tür 8 tohumları, bölüt 4.000 kitap; kontrol_sinama ile karşılaştırılır)
CF+=(-DKONTROL_TUR=8 -DBN=4000)
b kontrol/kontrol_test   src/kontrol.c
print "derlendi${O:+ → $O}"
