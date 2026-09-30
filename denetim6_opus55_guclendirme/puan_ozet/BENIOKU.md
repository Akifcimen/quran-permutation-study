# puan_ozet — sıkıştırılmış rastgele-kitap puanları

Tam puan dizileri (toplam 9,9 GB, 9 sütun float32) disk dolduğu için silindi. Bütün koşular tohumlu ve deterministik:
`kos_hepsi.sh`, `kos_ek.sh` ve `kos_ek2.sh` onları bit bit yeniden üretir.

Her `.npz` dosyasında şunlar var:
- `N`: kitap sayısı
- `Q`: Kur'an'ın puanları
- `idx`/`tail`: her sütunda Kur'an eşiğinin 1,5 altına kadar olan bütün satırlar, tam hassasiyetle ve satır indeksleriyle
- `quantiles`: sütun 0–5 için %50/90/99/99,9/99,99 nicelikleri
- `k`: sütun başına aşan kitap sayısı

`analiz.py` bu dosyalardan raporun bütün p-değerlerini birebir üretir. Doğrulama `kontrol/sikistir.py` içinde. Tam
dizilerden alınan çıktı `sonuc/ONKAYITLI_SONUCLAR_tam_dizilerden.md` dosyasında duruyor.
