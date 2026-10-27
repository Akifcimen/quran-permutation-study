#!/bin/bash
# duraklat.sh — 4-süreç sınırını korumak için: ağır süreçlerden BİRİNİ (önce adanmış kestirim, yoksa menü/sınama/referans) SIGSTOP
# ile durdurur, verilen kısa analiz komutunu çalıştırır, sonra yalnız kendi durdurduğunu SIGCONT ile sürdürür. Ağır süreç sayısı
# zaten 4'ün altındaysa hiçbir şeyi durdurmaz. (kos/is_kuyrugu.py durdurulmuş süreçleri de sayar; yarış olmaz.)
# Kullanım: kos/duraklat.sh <komut ...>
calisan=$(ps -axo stat=,comm= | awk '$1 !~ /^T/ && $2 ~ /src\/(referans|test|menu|adanmis_s|adanmis_u)$/' | wc -l)
P=""
if [ "$calisan" -ge 4 ]; then
  for desen in "src/adanmis_[us] " "src/menu " "src/test " "src/referans "; do
    for x in $(pgrep -f "$desen"); do st=$(ps -o stat= -p $x 2>/dev/null); [[ -n "$st" && "$st" != T* ]] && { P=$x; break; }; done
    [ -n "$P" ] && break
  done
  [ -n "$P" ] && kill -STOP $P
fi
"$@"; rc=$?
[ -n "$P" ] && kill -CONT $P
exit $rc
