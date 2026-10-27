# EK — `EK_20261026T200253Z_dondurma_sonrasi_kod.md`'den sonra değişen ya da eklenen kod

**Denetim (bu ekin zamanında):**
- `ONKAYIT.md`'nin SHA-256'sı hâlâ `d213dc89…fd513`'tür (salt okunur).
- `sonuc/onkayit_kod_sha256.txt` denetiminde yine yalnız `src/test.c` ve `src/dil.py` farklıdır (önceki ek). Puanlayıcının özetleri
  (`src/butunluk.h`, `src/tablo.h`, `src/bilgi.h`) dondurma anındakiyle aynıdır.
- `~/Desktop/hrf2`'de `GOREV.md`'den yeni hiçbir dosya yoktur.

**Önceki ekten (2026-10-26T20:02:53Z) sonra değişen ya da eklenen dosyalar.** Hepsi koşu düzeni ya da özet/rapor aracıdır; hiçbiri
puan tanımlarını, örnekleyicileri ya da karar kurallarını değiştirmez.
- Koşu düzeni: `kos/is_kuyrugu.py` (küresel 4-süreç sınırı), `kos/is_listesi_1.txt`, `kos/is_listesi_2.txt`, `kos/duraklat.sh`
  (genelleştirildi), `kos/derle.sh`, `kos/paketle.sh`.
- Özet ve rapor: `src/bilgi_sira.py` (Δ = S_Q − S_alt dağılımının dökümü eklendi), `src/zincir.py`, `src/belirsiz_kimlik.py`,
  `src/sira_duzelt.py`, `src/karar.py`, `src/menu_ozet.py`, `src/rapor_kur.py`, `src/rapor_tablo.py`, `src/hesap_saati.py`.
- `src/sira_duzelt.py`: yeniden puanlanmış kitabın "sonra" alt/üst sınıfı artık `src/belirsiz_coz.py`'nin tam duyarlıklı kararından
  alınır (önce 2 ondalıklı yazılmış değerden okunuyordu). Değerler S_Q'ya yuvarlama payından yakınsa uyarı basılır. Üç sıfırda uyarı
  yoktur; tekdüze sonucu önceki sürümle aynıdır.
- `src/rapor_tablo.py`:
  - **Hata düzeltmesi.** `cp()` Poisson toplamını exp(−λ)'dan başlatıyordu; λ > ~745'te bu değer taşıyordu ve k ≳ 745 için aralık
    anlamsız çıkıyordu (ör. [7,45×10⁻⁵; 7,45×10⁻⁵]). Hata yalnız menü (d3) sütunlarında görüldü ve bu değerler rapora hiç girmedi.
    Terimler artık log uzayında hesaplanır. Küçük k'de eski sonuçla göreli fark 2×10⁻¹³'tür; büyük k'de sonuç `src/ozet.py`'nin kesin
    binom Clopper–Pearson hesabıyla aynıdır.
  - Duyarlılık tablosunun V0 P0 sütunu, ana tabloyla aynı olsun diye adanmış kestirimle düzeltilmiş k_nokta'yı kullanır.
