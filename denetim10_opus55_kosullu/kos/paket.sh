#!/bin/bash
# Paket (GOREV.md): kod + koşu betikleri + sonuçlar + rapor; büyük tablolar (koşullu kitap havuzları, tarama tabloları) ve derlenmiş
# ikililer hariç — hepsi tohumlarla yeniden üretilir (kos/hepsi.sh). Havuzların sayaç (.say) ve sürücü günlükleri pakete girer.
cd "$(dirname "$0")/.."
mkdir -p paket
liste=$(mktemp)
{
  ls GOREV.md ONKAYIT.md KOSULLU_RAPORU.md baslat.command
  find src kos kontrol ref ek sonuc -type f ! -name '.DS_Store' ! -name '*.yazılıyor' | while read f; do
    file -b "$f" | grep -q 'Mach-O' && continue          # derlenmiş ikililer
    echo "$f"
  done
  find tab -type f \( -name '*.say' -o -name '*.log' -o -name '*.txt' \)   # havuz sayaçları, sürücü günlükleri, Ö0(a) Kur'an satırları
} | LC_ALL=C sort > "$liste"
( while read f; do shasum -a 256 "$f"; done < "$liste" ) > paket/MANIFEST.sha256
echo paket/MANIFEST.sha256 >> "$liste"
tar -czf paket/kosullu_paket.tar.gz -T "$liste"
rm -f "$liste"
shasum -a 256 paket/kosullu_paket.tar.gz | tee paket/kosullu_paket.sha256
ls -la paket/
