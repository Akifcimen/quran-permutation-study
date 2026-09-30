#!/bin/bash
# D_J10_tam puanlaması bitince defter_kos.sh'yi (M20 bölümüne geçmeden) durdur, son_kosular.sh'yi başlat
while [ ! -f puan/D_J10_tam_sifir.f32 ]; do sleep 3; done
sleep 2
pkill -f defter_kos.sh; sleep 1; pkill -f "robot2 table M20"; sleep 1
echo "defter J10 tamam; M20 atlandı; son koşular başlıyor"
./son_kosular.sh > sonuc/son_kosular.log 2>&1
echo "gecis bitti"
