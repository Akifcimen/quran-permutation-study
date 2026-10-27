## 10. Sapmalar ve ekler

Ön-kayıt dondurulduktan sonra yazılan bütün ekler (`ek/`), zamanlarıyla:

| Ek | Konu | Sonuca etkisi |
|---|---|---|
| `EK_20261026T171720Z_O_ve_Ousu_tahmin.md` | Eski literatür koşullu tahmin parçası O ve O' ile **ayrı ayrı** değerlendirilir (sonuç görüldükten sonra yazıldı) | yalnız hükmün yazılışı |
| `EK_20261026T172158Z_sifir_toplam_yozlasmasi.md` | Sayım puanında sıfır-toplam yozlaşması gözlendi; **sonradan** S0 duyarlılığı tanımlandı (sonuçlarına bakılmadan önce) | birincil sayılar değişmez; S0 ayrıca |
| `EK_20261026T174901Z_aday_payi.md` | Aday dosyası payı 6 → 2 bit (disk) | k sayıları değişmez |
| `EK_20261026T175853Z_R2_bant_yonu.md` | R2 kalibrasyon bandının yönü (R2/oran) | R2 kararda kullanılmadı |
| `EK_20261026T180313Z_aday_G0.md` | Sayım aday ölçütünden G0 çıkarıldı (disk) | k sayıları değişmez |
| `EK_20261026T181111Z_sinif_kisitli_kesif.md` | Keşif: yalnız Kur'an-sınıfı mercekleriyle sıra | keşif; karar dışı |
| `EK_20261026T194928Z_adanmis_tohumlari.md` | Adanmış kestirim tohumlarında sıfır numarası bir kaydırılmış | yok |
| `EK_20261026T200253Z_dondurma_sonrasi_kod.md` | Dondurmadan sonra değişen/eklenen kod; `dil.py` ↔ C yeniden sınandı (0 uyuşmazlık) | yok |
| `EK_20261026T224527Z_tekduze_adanmis_sirasi.md` | Tekdüze adanmış kestirimde S_alt sırasından sapma: bir kitap (S_alt 24,93) sırası gelmeden çözüldü; S_alt 26,88 olan iki kitabın beş araması koşulmadı | k_üst −1; k_alt, k_nokta değişmez |
| `EK_20261026T234102Z_son_kod_degisiklikleri.md` | Önceki ekten sonra değişen/eklenen koşu ve rapor kodu; `src/rapor_tablo.py` aralık hesabında büyük k taşma hatası (rapora girmeden) düzeltildi | yok |

Puanlara bakıldıktan sonra hiçbir tanım, ağırlık, eşik ya da bütçe değiştirilmedi. Tek yordam sapması tekdüze adanmış kestirimin
sırasıdır (`EK_20261026T224527Z_tekduze_adanmis_sirasi.md`).

## 11. Yeniden üretim

- **Ön-kayıt:** `ONKAYIT.md` (salt okunur), `sonuc/onkayit_ozet.txt` (UTC + SHA-256), `sonuc/onkayit_kod_sha256.txt`.
- **Tanım:** `src/dil.py` (Python başvuru), `src/butunluk.h` (C puanlayıcı; bit-bit eşit), `src/tablo.h`, `src/bilgi.h` (bilgi puanı).
- **Üretim:** `kos/orkestra.py kos/plan_ana.txt` — R (referans bölütü, `src/referans`) → B (`src/birlestir`, `src/tablo_kur`) → Q
  (`src/kuran_puan`, Kur'an eşiği) → T (sınama bölütü, `src/test`); sürdürülebilir (`.say` = bölüt tamam). Tohum =
  26102600000 + sıfır·10⁸ + tür·10⁷ + kodeks·10⁶ + bölüt (ön-kayıt §5.3; adanmış koşular için ek). Derleme: `kos/derle.sh`.
- **Adanmış kestirim:** `src/adanmis_u` (tekdüze, kesin koşullu önem örneklemesi), `src/adanmis_s` (tek mercekli doğrudan örnekleme);
  diziler `kos/adanmis_U3.sh`, `kos/adanmis_M.sh` ve iş kuyruğu `kos/is_listesi_2.txt` (Mallows, 127 Kur'an eşikleri; küresel
  4-süreç sınırıyla `kos/is_kuyrugu.py`); sonuçlar `sonuc/adanmis/`; yeniden puanlama
  `src/ayrinti` + `src/belirsiz_coz.py` + `src/sira_duzelt.py` (`sonuc/belirsiz/`).
- **Analiz:** `kos/analiz_sifir.sh <sıfırkodeks>` (sıra özetleri `src/ozet.py`, `src/bilgi_sira.py`; rakipler `src/rakipler.py`;
  S0 `src/s0`; keşif `src/sinif`); rapor tabloları `src/rapor_tablo.py`.
- **İkincil:** koşullu (b) `src/kosullu` (`sonuc/kosullu/`); menü (d) `src/menu` (`tab/D_*`; ilk dört tekdüze bölütü `kos/menu_kos.sh`,
  kalanı iş kuyruğuyla, aynı tohum yasasıyla), S0 `src/menu_s0`,
  özet `src/menu_ozet.py`; ana hat rakipleri `src/puanla_liste`.
- **Denetimler:** `kontrol/` (C ↔ Python eşitliği, örnekleyici bit-eşitliği, gerektirme taramaları, kural sayımları).
- **Kısa analizler** `kos/duraklat.sh` altında koşar (4 ağır süreç çalışıyorsa birini geçici durdurur).
- **Günlükler:** `sonuc/kosu_gunlugu.txt` (her koşunun bitişi), `sonuc/ara_notlar.md` (ara notlar).
- **Rapor:** `python3 -I src/rapor_kur.py` (bu dosyayı `rapor_taslak/` bölümlerinden kurar; tablolar ve hükümler sonuç dosyalarından).
- **Hesap ve disk:** günlüğe kayıtlı koşular 24,3 çekirdek-saattir: üretim 12,6, adanmış kestirim 7,8, menü 3,9 (`src/hesap_saati.py`,
  `sonuc/kosu_gunlugu.txt`; sürenin yazılmadığı sıralı dizilerde duvar saati, yani üst sınır). Kayıt dışı kısa analizler, dondurma öncesi
  doğrulamalar ve ilk adanmış denemeler ölçülmedi (kestirim: birkaç çekirdek-saat; toplam ~40 çekirdek-saatlik bütçenin altında).
  Aynı anda en çok 4 ağır süreç koştu (`kos/is_kuyrugu.py`, `kos/duraklat.sh`). Çalışma klasörü ~0,8 GB, paket ~60 MB.
- **Paket:** `paket/butunluk_paket.tar.gz` ve SHA-256'sı `paket/butunluk_paket.sha256`. Pakette derlenmiş ikililer, büyük sıklık
  tabloları (`tab/*.tab`; R bölütlerinden aynı tohumlarla yeniden kurulur), dondurma öncesi çıkarım taraması örneklemleri (`tab/cikarim`)
  ve dış girdiler (`girdi/`; yalnız SHA-256 listesi `sonuc/girdi_sha256.txt`) yoktur. Pakete giren her dosyanın SHA-256'sı
  `paket/PAKET_ICERIGI.sha256`'dadır.
