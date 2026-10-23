# EK — Ö0(a): defter bataryası incelemenin sayılarını üretmiyor; iddia bataryası keşif sınaması (yazılış: 2026-10-23T03:40:12Z)

**Tür:** doğrulama başarısızlığı (ön-kayıtlı ölçüt) + keşif amaçlı ek analiz. Ö1'in bataryası değişmez (ONKAYIT §3.1 son cümlesi).

ONKAYIT §3.1'deki ölçütle (129, terazi kapalı, üç tekrarın ortalaması incelemenin ±%30'u içinde ve oran 1,6–2,6) dokuzuncu incelemenin
sayıları **yeniden üretilemedi**: defter bataryasında (robot4_m, 82 etkin olgu) U 269 [126/380/302], J10 7.293, M20 6.407 (inceleme 116,
1.135, 827); terazi açık biçimde U 1, J10 59, M20 58. Hiçbir biçim incelemeye yakın değil. İncelemenin sayıları altıncı denetimin
**iddia bataryası, terazisiz** satırına (robot_m, 60 olgu; `denetim6` E1, 127: U 115, J10 1.250, M20 1.125) çok yakındır; inceleme
robot4_m.c'ye bağlantı verir ama G-çapraz/G-toplam düğüm düzeni iki robotta aynıdır.

**Keşif sınaması (ön-kayıt dışı):** iddia bataryası = defter bataryasının 0–14. düğümleri (robot_m'nin 60 olgusu, aynı bitler; 15–22.
düğümler kapalı). Aynı tablo tohumlarıyla (robot4_m tablosu yeniden üretilir; tohumlar §3.1) 129, terazi kapalı, üç tekrar × üç sıfır;
`src/tekrar9` IDDIA=1 seçeneğiyle. Sonuçlar `sonuc/t9/iddia_*.txt`.

**Ayrıca gözlem:** koşulsuz 2×10⁶'lık tabloda Kur'an'ın T2'si tablodan tabloya oynuyor (U: 12,28 / 11,75 / 11,82; J10 9,46–9,58; M20 9,54–9,75):
en güçlü olgusu "F: hücreler dengeli" k_F = 12 bağlamında ~10⁻³·⁶'dır ve o bağlamda ~2×10⁴ tablo kitabında yalnız birkaç isabetle
tahmin edilir. Tek tablolu sayımlar Poisson'dan çok daha geniş bir tablo belirsizliği taşır.
