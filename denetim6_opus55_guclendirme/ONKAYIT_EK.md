# ÖN-KAYIT EKİ — keşif amaçlı çift-sayma kontrolü

**Tarih:** 29 Eylül 2026. A1 sonuçları görülmeden yazıldı. Özet `sonuc/onkayit_ek_ozet.txt` dosyasında.
**Statü:** KEŞİF AMAÇLI. Ana ön-kayıttan (`ONKAYIT.md`) sonra akla geldi. Manşete girmez, yalnız raporlanır.

## Soru

Denetçinin "mercek verili" basamağı (tekdüze 2,3×10⁻⁷, J10 6×10⁻⁶) hem merceği bedava sayıyor hem de teraziyi kanıt
olarak sayıyor. t = n + a biçimi, terazinin bulunduğu çift-tek (homojen/heterojen) literatüründen geldiyse, yani merceği
terazi seçtiyse, bu **çift saymadır**. Tutarlı iki seçenek var:

- **(i)** teraziyi say, biçim serbestliğini öde. Bu ana ön-kayıttaki A1'dir.
- **(ii)** merceği bedava say, teraziyi sayma. Terazi merceği "satın almış" olur.

(ii)'yi ölçüyorum. Bu kontrol yazarın **aleyhine** çıkabilir.

## Tanım

- E1 (sabit mercek, terazisiz): `robot_m`, iddia bataryası + `kitap:` olguları da kapalı
  (`ROBOT_DISABLE="Σw=m·kG|Σv(U) kare/asal-kuvvet|kitap:"`). Sütun 4, yani (1,1,19). U / J10 / M20, N = 2×10⁶,
  tohumlar 19901 / 19902 / 19903, tablolar A1a ile aynı.
- E2 (m = 19, biçim serbest, terazisiz): aynı koşunun 1. sütunu. En muhafazakâr "19-analisti" okuması.
- E3: E1–E2'nin defter bataryasıyla aynısı (`robot4_m`, `…|MX:|kitap:`), tablolar A1c ile aynı, tohumlar 19911 / 19912 / 19913.

## Yorum

- (ii) (i)'den kötü çıkarsa (daha büyük p), "mercek verili" basamağı yazar lehine bir üst sınır olarak okunmalı, tutarlı
  basamak A1'dir.
- (ii) (i)'den iyi çıkarsa, tutarlı iki okuma arasındaki aralık raporlanır. Hiçbiri tek başına manşet olmaz.
