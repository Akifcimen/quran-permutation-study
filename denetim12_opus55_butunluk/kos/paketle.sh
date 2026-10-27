#!/bin/bash
# paketle.sh — paket/butunluk_paket.tar.gz ve SHA-256'sı (paket/butunluk_paket.sha256).
# Pakete girenler: ön-kayıt, görev, rapor, ekler, rapor taslakları, bütün sonuçlar (sonuc/), kaynaklar ve betikler (src, kontrol, kos),
# küçük tablolar (bölüt sayımları .say/.hist, aday dosyaları .aday, Kur'an başvuruları .ref, menü bölütleri, kural sayımları, günlükler).
# Girmeyenler: derlenmiş ikililer (kos/derle.sh yeniden derler), büyük sıklık tabloları tab/*.tab (~430 MB; tohumlarla R bölütlerinden
# yeniden kurulur), dondurma öncesi çıkarım taraması örneklemleri tab/cikarim (~80 MB) ve dış girdiler girdi/ (yalnız SHA-256 listesi
# sonuc/girdi_sha256.txt). Pakete giren her dosyanın SHA-256'sı paket/PAKET_ICERIGI.sha256'dadır (o da pakettedir).
set -euo pipefail
cd "$(dirname "$0")/.."
export COPYFILE_DISABLE=1                     # macOS: ._ (AppleDouble) dosyaları yazılmasın
shasum -a 256 girdi/* > sonuc/girdi_sha256.txt
liste=paket/.liste
{
  ls ONKAYIT.md GOREV.md BUTUNLUK_RAPORU.md baslat.command
  find ek rapor_taslak sonuc -type f
  find src kontrol -type f \( -name '*.c' -o -name '*.h' -o -name '*.py' -o -name '*.txt' -o -name '*.md' \) -not -path '*/__pycache__/*'
  find kos -type f \( -name '*.sh' -o -name '*.py' -o -name '*.txt' \)
  find tab -type f \( -name '*.say' -o -name '*.hist' -o -name '*.aday' -o -name '*.ref' -o -name '*.txt' -o -name '*.out' \
                      -o -name '*.bitti' \) -not -path 'tab/cikarim/*'
  find tab/kural -type f
} | grep -v '^paket/' | LC_ALL=C sort -u > $liste
xargs -I{} shasum -a 256 "{}" < $liste > paket/PAKET_ICERIGI.sha256
echo paket/PAKET_ICERIGI.sha256 >> $liste
tar --no-mac-metadata -czf paket/butunluk_paket.tar.gz -T $liste
rm -f $liste
( cd paket && shasum -a 256 butunluk_paket.tar.gz > butunluk_paket.sha256 )
echo "dosya: $(wc -l < paket/PAKET_ICERIGI.sha256 | tr -d ' ') ; boyut: $(du -h paket/butunluk_paket.tar.gz | cut -f1)"
cat paket/butunluk_paket.sha256
