# EK — dondurmadan sonra değişen ya da eklenen kod (özet denetimi)

`sonuc/onkayit_kod_sha256.txt`'teki 34 dosyanın özeti bu ekin zamanında yeniden denetlendi (`shasum -a 256 -c`): **32'si aynı**, ikisi
farklı:
- `src/test.c` — aday dosyası ölçütü değişti (pay 6 → 2 bit; sayım ölçütünden G0'ın çıkarılması). Gerekçe ve etki:
  `EK_20261026T174901Z_aday_payi.md`, `EK_20261026T180313Z_aday_G0.md`. Sayılan k'ler (.say) ve puan tanımları değişmedi; yalnız
  hangi kitapların ayrıntılı kaydının tutulduğu değişti.
- `src/dil.py` — Python başvuru uygulamasına **bölme menüsü bölümü** eklendi (menü C kodunun `kontrol/menu_esit.py` ile denetimi için).
  Bütün üretim puanları C puanlayıcısından (`src/butunluk.h`, `src/tablo.h`, `src/bilgi.h`; özetleri dondurma anındakiyle **aynı**)
  hesaplandı. Güncel `dil.py`'nin birincil tanımlarının C ile eşitliği yeniden sınandı (`kontrol/c_dok` + `kontrol/c_py_esit.py`;
  yeni, üretim dışı tohumlar 26902600001–2): tekdüze 150 + M20 150 kitap × 406 mercek: maske, boy/hücre ve kredi uyuşmazlığı **0**.

Dondurmadan sonra **eklenen** programlar (ön-kayıt §0'da "dondurmadan sonra, ön-kayıttaki tanıma göre" yazılacağı belirtilenler ve
analiz araçları): `src/adanmis_u.c`, `src/adanmis_s.c`, `src/kosullu.c`, `src/kuyruk.c`, `src/menu.c`/`menu.h`, `src/ayrinti.c`,
`src/puanla_liste.c`, `src/s0.c`, `src/sinif.c`, `src/menu_s0.c` (sonradan S0), Python özet/rapor betikleri (`src/*.py`) ve koşu
betikleri (`kos/*.sh`). Hiçbiri puan tanımlarını değiştirmez; her birinin denetimi `sonuc/ara_notlar.md`'de yazılıdır.
