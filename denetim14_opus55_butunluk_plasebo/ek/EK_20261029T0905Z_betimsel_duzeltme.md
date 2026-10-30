# EK — betimsel düzeltme: "dili plasebo12'dekiyle aynı" sayımı (`src/analiz_b.py`)

Yazıldı: 2026-10-29T09:05Z. Düzeltmenin yapıldığı an: 2026-10-29T05:13:13Z (dosya zamanı; `sonuc/ara_notlar.md`, 05:13:30Z kaydı).
Tür: kod düzeltmesi (ONKAYIT §12), yalnız bir betimsel sayıda. Kararlara giren hiçbir sayı (k, N, p_x, aralıklar, R, p_kal, okuma
sınıfı, model-tabanlı fark, tahmin hükümleri) bu sayıyı kullanmaz ve değişmedi.

**Hata.** Kitap kitap karşılaştırmada (ONKAYIT §9, Ö4) "dili plasebo12'dekiyle aynı kalan plasebo" sayısı bütün plasebolar üzerinden
alınıyordu. plasebo12'nin `analiz.json`'unda dili boş liste olan plasebolar bu görevde de dilsizse, iki boş liste "aynı dil" sayılıyordu.
ONKAYIT §9 bu sayıyı dilli plasebolar için tanımlar ("dilli plasebolarda log₁₀(p^A_x / p^12_x)'in medyanı …, dili plasebo12'dekiyle aynı
kalanlar").

**Düzeltme** (satır 208): `ayni_dil = sum(1 for d in dl if d['ayni_dil'])`; `dl` dilli plasebolardır.

**Düzeltme öncesi ve sonrası.** M20 Kol A kümesinin analizi (A, A−, A+) düzeltmeden önce koşmuştu (05:12Z); 08:59Z'de, hiçbir hesap
koşmazken, düzeltilmiş betikle yeniden koşturuldu. Tekdüze ve Mallows Kol A ile bütün Kol B analizleri baştan düzeltilmiş betikle koştu.

| Sıfır, kol | dilli plasebo | düzeltme öncesi | düzeltme sonrası | plasebo12'de dili boş | ikisinde de boş |
|---|---|---|---|---|---|
| M20, A | 35 | 14 | **0** | 16 | 14 |
| M20, A− | 60 | 13 | **0** | 16 | 13 |
| M20, A+ | 2 | 16 | **0** | 16 | 16 |
| tekdüze, A | 18 | (10) | **0** | 11 | 10 |
| Mallows, A | 15 | (11) | **0** | 12 | 11 |
| üç sıfır, B | 200 / 100 / 100 | (0) | **0** | 16 / 11 / 12 | 0 |

(Ayraç içindekiler, düzeltilmemiş sayımın vereceği değerlerdir; bu analizler o sürümle hiç koşmadı.) Düzeltilmiş değer her kolda 0'dır:
bütünlüklü dili olan hiçbir plasebonun dili plasebo12'deki diliyle aynı değildir.

**Yeniden koşturmanın etkisi.** Öncesi ve sonrası arasında `sonuc/M/analiz_{A,Am,Ap}.json`'da alan alan karşılaştırmada değişen tek alan
`kitap_kitap/ayni_dil`'dir; `.txt` çıktılarında yalnız bu alanı taşıyan satır değişti (`analiz_A_cikti.txt`'de ayrıca aynı sekiz numpy
uyarı satırının yeri: yeniden koşuda stderr ayrı yakalandı ve dosyanın sonuna eklendi). Düzeltme öncesi dosyalar `sonuc/M/duzeltme_oncesi/`'de salt okunur olarak duruyor (SHA-256 önekleri: analiz_A.json
9e17e14d…, analiz_Am.json 3be3818f…, analiz_Ap.json e22d85a1…).

**Dondurma anı özeti.** `src/analiz_b.py`'nin SHA-256'sı bu yüzden `sonuc/onkayit_kod_sha256.txt`'teki değerden farklıdır.
