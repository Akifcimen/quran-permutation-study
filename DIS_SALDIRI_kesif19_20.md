# DIŞ SALDIRI RAPORU — Ana keşif kırılabiliyor mu? (2 Eylül 2026)

**Görev:** depodaki ana keşfi (t = n + a üzerinden terazi ∧ kristal ∧ İki El; çekirdek ≈ 1,7×10⁻¹⁵) incelemek ve kırmaya çalışmak.
**Araçlar:** `kesif19_catallanma.py` (çatallanmış mercek turnuvası), `kesif20_tipiklik.py` (kristal yapılandırmasının tipikliği), 20M'lik bağımsız marjinal/çatal ölçümü, `kesif16` tekrarı. Ham çıktılar: `kesif19_catallanma_sonuc.txt`, `kesif20_tipiklik_sonuc.txt`, `kesif16_tekrar_2026-09-02.txt`.
**Kodeks:** motorla aynı (sûre 9 = 127). Üreteç: NumPy PCG64 (motorlardan bağımsız aile).

## 0. Hüküm

**Aritmetik olarak gerçek, istatistiksel anomali olarak kırık.** Veri-içi eşitliklerin hiçbiri yanlış değil; motorlar hatasız; marjinaller bağımsız üreteçle tekrar üretildi. Ama manşet rakam (10⁻¹⁵) verinin tuhaflığını değil, seçilmiş tarifin ayrıntı düzeyini ölçüyor. İki bağımsız ölçüm bunu gösteriyor:

1. **Tipiklik (kesif20):** kristalin her düzeyinde gerçek kitabın yapılandırması, rastgele 12 üyeli ailelerin *kendi* yapılandırmalarından daha nadir değil; C ve D düzeylerinde en yaygın olanlardan. Zincirin 3×10⁻⁴'ten 7×10⁻¹⁰'a inmesi tarifi incelemenin bedeli, yapının değil.
2. **Çatallanmış turnuva (kesif19):** S1–S8 profili gerçek verinin parmak izidir. Yalnız yazarın depoda kendisinin kutladığı eşdeğer alternatifler eklenince sentetik kitaplar 7/8'e %3,3, 8/8'e 1/20.000 oranında ulaşıyor. "Sentetik tavanın dört basamak üstünde" iddiası düşüyor.

Dürüst kalıntı: yalnız *belgelenebilen* tarif serbestliği altında bile çekirdeğin frekansı 10⁻¹⁵'ten 10⁻⁸–10⁻¹⁰ bandına çıkıyor; belgelenemeyen (kültürel) serbestlik not 12'de zaten fiyatlanamaz kabul edilmiş. Bu, sonradan-bulunmuş sayısal desenlerin (İncil Kodu türü) düştüğü seviyedir.

## 1. Kırılamayanlar

- **Eşitlikler:** `test_oruntular.py` ve bağımsız hesap; tamamı doğru (iki kodekste).
- **Motor:** `kesif4_motor.c` — Fisher-Yates + Lemire sınırlı-çarpım + SplitMix64; yanlılık yok; 28 sayaç tanımlarla birebir; negatif u için `%` doğru. `kesif_cuda.cu` sayaç-uyumlu.
- **Marjinaller (20M permütasyon, PCG64):**

| Olay | Bu ölçüm | Rapor |
|---|---|---|
| A terazi | 3,22×10⁻⁴ | 3,14×10⁻⁴ |
| B aile (12 ∧ 1444) | 3,24×10⁻⁴ | 3,16×10⁻⁴ |
| W İki El | 5,6×10⁻⁵ | 5,5×10⁻⁵ |

- **kesif18** koşullu ayrışım tasarımı (SIS + ret örneklemesi) geçerli; çapa testi geçmiş.
- **kesif16 tekrarı (boy-sırası):** S_obs z = −8,0; P(katman | S ≤ S_obs) / koşulsuz = terazi ×1,45, aile ×1,77, İki El ×1,16. Rapordaki ×1,2–1,8 ile uyumlu; boy-sıralılık yapıyı beslemiyor. *Bu itiraz kapalı.*

## 2. Saldırı 1 — Tipiklik: p-değerleri "yapı sınıfı"nın değil "gözlenen yapılandırma"nın frekansıdır (kesif20)

**Yöntem:** 147,6M permütasyon; k = 12 üyeli 19-ailesi olanlar seçildi (1.500.515 örnek; P(k=12) = 1,017×10⁻²). Her örnek için yapılandırma, motorun sayaç taneciğiyle aynı düzeylerde kaydedildi. Gerçek kitabın yapılandırmasının koşullu olasılığı, rastgele örneklerin *kendi* yapılandırmalarının olasılık dağılımıyla (kütle-ağırlıklı) karşılaştırıldı.

| Düzey | Gerçek yapılandırma | P(·\|k=12) | Koşulsuz P (motor) | Rastgele ailelerin yapılandırması gerçekten **daha nadir** olanlar |
|---|---|---|---|---|
| B | Σc = 76 (Σt = 1444) | 3,1×10⁻² | 3,2×10⁻⁴ (3,16×10⁻⁴) | %54 |
| C | + 6/6, Σc_e = 38 (722/722) | 1,8×10⁻³ | 1,8×10⁻⁵ (1,85×10⁻⁵) | **%87** |
| D | + hücreler (3,18)(3,20) \| (3,19)(3,19) | 1,2×10⁻⁵ | 1,2×10⁻⁷ (1,39×10⁻⁷) | **%83** |
| G | + torbalar {666}{668} \| {559}{577} | ~7×10⁻⁸ | — (7,14×10⁻¹⁰) | çözünürlük dışı: örneklerin %95'i tekil; her aile "eşsiz" |

**Okuma:** 1444 en sık görülen aile toplamıdır (medyan 1445 civarı). "Fraktal iç terazi" (C) ve "hücre kristali" (D), 12 üyeli rastgele bir ailenin *en sık* aldığı biçimlerdendir: gerçek kitabın C-yapılandırması rastgele ailelerin %87'sininkinden daha olasıdır. G düzeyinde ise her yapılandırma tekildir; 7×10⁻¹⁰ o düzeyin çözünürlüğüdür, anomali değil. Kristal zincirindeki her adım "daha nadir bir yapı" değil, "daha ince bir tarif"tir.

## 3. Saldırı 2 — Çatallanmış mercek turnuvası (kesif19)

**Sorun:** `kesif4_ozgulluk` / `kesif9` / `kesif17`'nin S1–S8 ölçütleri, gerçek kitabın m=19'daki gözlenen özelliklerinden yazılmıştır. Turnuva, "rastgele veri bu 8-bitlik parmak izini üretir mi?" sorusunu ölçer; cevabın hayır olması kaçınılmazdır. Doğru soru: ölçütleri üreten *prosedür* başka veriye uygulansaydı ne üretirdi? Bunun alt sınırı, yazarın depoda fiilen kutladığı alternatiflerdir.

**Çatallar (her biri depodan kanıtlı):**

| Ölçüt | Orijinal | Eklenen alternatif | Kanıt |
|---|---|---|---|
| S1 | k ≥ 2E | k = m | S6'da "birleşim = m" kutlanıyor |
| S2 | m² \| Σv | Σv tam kare / 114× / asal kuvveti (p ≤ 7, üs ≥ 3) | `kesif2_panorama.py:13` `pretty()`; KESIF.md:116 Σt(birleşim) = 3⁷ |
| S3 | v-paritesi eşit sayı ∧ eşit toplam | n-paritesiyle aynı; eşit sayı ∧ \|Δ\| ∈ {m², 2m²} | KESIF.md:72 "18 ve 20, 19'un etrafında"; 2c'nin ikinci ekseni n-paritesi |
| S4 | m \| Σc (iki yarı) | iki yarı tam kare; n-parite yarıları | KESIF.md:47 "722 = 2×19²", "38 = 2×19" |
| S5 | ardışık bant | adım-2 aritmetik dizi | KESIF.md:85 "5-7-9 çıkıyor" |
| S6 | \|F∪U\| = m | = 2m; k_u = k; k_u = m; k_u ≥ 2E | — |
| S7 | m \| Σa(birleşim) | Σn/Σv/Σw(birleşim) m'ye bölünür veya güzel; Σa(U), Σn(U); Σw(U) = m·k_u | KESIF.md:102 "Σu = 7×19, ortalama katsayı 1"; :116 3⁷; :207 "\|Σu\| = 21×19" |
| S8 | ayrık | kesişim tam 1 | — |
| El | t birincil | u birincil | u-eli kristal taşısaydı raporlanırdı |

Gerçek kitap her iki puanlamada 8/8 (yalnız (1,1,19,t); en yakın rakip çatallı 6/8).

**Sabit mercek (1,1,19) — dış çapa (74:30, 89:3) olduğu gibi kabul edilerek; 50.000.000 sentetik:**

| Puanlama | 7/8 | 8/8 |
|---|---|---|
| Katı (yazarın ölçütleri, t-eli) | 12 (2,4×10⁻⁷) | 0 (< 6×10⁻⁸, %95 üst) |
| Çatallı, t-eli | 226 (4,5×10⁻⁶) | 6 (1,2×10⁻⁷) |
| Çatallı, iki el | 2.653 (5,3×10⁻⁵) | 7 (1,4×10⁻⁷) |

**Serbest mercek (304 mercek × 2 el) — 200.000 sentetik:**

| Puanlama | ≥7/8 | 8/8 |
|---|---|---|
| Katı (kesif17 tekrarı) | 1 (5×10⁻⁶) | 0 |
| Çatallı | **6.596 (%3,3)** | **10 (5×10⁻⁵)** |

**Okuma:**
- kesif9/kesif17'deki "500.000 sentetikte hiçbiri 7/8'e bile ulaşamadı" ifadesi bir bulgu değil, beklentidir: katı profilde 7/8 sabit mercekte 2,4×10⁻⁷, mercek serbestken ~5×10⁻⁶'dır. 500k'da sıfır görmek 10⁻⁶'lık bir olayın olağan görünümüdür; "dört basamak üstünde" diye bir mesafe yoktur, tavan örneklem büyüklüğünün eseridir.
- Belgeli çatallar tek başına ≥7/8 oranını sabit mercekte ×220, mercek serbestken ×6.600 büyütüyor; tam profil ~5×10⁻⁵'e (mercek serbest) / ~1,4×10⁻⁷'ye (mercek sabit) çıkıyor.
- Bu çatal kümesi bir **alt sınırdır**: yalnız depoda belgesi olan alternatifler alındı; hücre düzeyi (2c) ve torbalar (2d) profile hiç girmediği için turnuva zaten yazar lehine kaba.

## 4. Fiyatlanmamış çatallar — doğrudan ölçüm (20M permütasyon)

| Katman | Raporlanan olay | Eşdeğer alternatif (yazarın kendi diliyle "güzel") | Çarpan |
|---|---|---|---|
| Katman 1 | 57/57 ∧ Σt(çift) = Σa | 57/57 ∧ Σa(çift) = Σa(tek) [4,1×10⁻⁴] | **×2,3** |
| Katman 2 | 12 ∧ Σt = 38² | 12 ∧ Σt "güzel" (`pretty()`: 19²×, kare, 114×) [2,2×10⁻³] | **×6,9** |
| İki El | 7 ∧ 19 ∧ 19 \| Σa(birleşim) | Σa/Σn/Σt/Σu'dan herhangi biri [2,1×10⁻⁴] | **×3,8** |
| El seçimi | t birincil | u birincil | ×~2 |

Not: "Σt(tek) = Σn" alternatifi (Σa(çift) = Σn(tek)) parite gereği imkânsızdır (Σt(çift) daima çift, 6555 tek); bu, Katman 1'in iki eşitliğinin *tek* eşitlik olduğunu da gösterir. "Σt = tam kare" ile "Σc = 76" 20M'de birebir aynı sayımı verdi (6.476 = 6.476): 38² retoriği ayrı bir olay değildir.

## 5. Cebirsel olarak zorunlu "bulgular" (anlatıda bulgu gibi sunulanlar)

- **+160/−160 anti-simetri** (KESIF.md:52): Σt yarıları eşitse Σn farkı = −Σa farkı zorunludur. Süs değil, özdeşlik.
- **Σt(tek) = Σn** (Katman 1 tablosu): Σt(çift) = Σa verildiğinde otomatik (Σt = Σa + Σn).
- **Katsayı paritesi = t paritesi**; "çift kefede yalnız çift katsayı" (KESIF.md:85) otomatik (19 tek).
- **Hücre içerikleri** kefe-multiset'i + hücre toplamı verildiğinde zorunlu (yazar bunu not etmiş; ama G'nin 2c'den ayrı bir "katman" gibi ×%0,6 çarpanı taşıması, aynı bilginin ikinci kez fiyatlanmasıdır: kesif20'de D→G adımı yalnız çözünürlük artışıdır).

## 6. Yapısal itirazlar

1. **"Ön-kayıtlı bağımsız tekrar" bu veri için tanımsız.** Metin sabittir; yeni örneklem yoktur; aynı tanım aynı sayıları verir. Depo'nun "kesin epistemik adım" dediği adım mevcut değildir. Geriye tarif-uzunluğu sorusu kalır; o da not 12'de kapatılamaz kabul edilmiştir.
2. **5σ kıyası** (README) yanlış ölçektir: parçacık fiziğinin 5σ'sı önceden sabitlenmiş istatistik + look-elsewhere düzeltmesi varsayar; burada istatistik veriye uydurulmuştur.
3. **Dış çapa** (74:30, 89:3) modül serbestliğini sıfırlamaz; metin başka sayıları da anar. Ama bu rapor çapayı olduğu gibi kabul eden sabit-mercek koşusunu da yaptı: sonuç değişmedi (çatallar tek başına ×220).
4. **Bonferroni "~75 aile"** yanlış birimdir: seçim, 75 önceden-tanımlı test arasından değil, verinin sayılamayan tarifleri arasından yapılmıştır. kesif19'un ölçtüğü şey tam olarak bu fark.

## 7. Ne kaldı?

Belgeli serbestlik altında, mercek sabitken, S-profili ~1,4×10⁻⁷; Katman 1 (çatallı 7,3×10⁻⁴) eklenince ~10⁻¹⁰; mercek serbestken ~4×10⁻⁸. Bu bir "düzeltilmiş p" değil, belgeli serbestliğin tek başına 5–7 basamak yediğinin ölçüsüdür. Kristalin ince yapısı (D, G) kesif20'ye göre anomali eklemez. Belgelenemeyen serbestlik fiyatlanamadığından kalibre bir sayı yoktur; olabilecek en iyimser okuma "sonradan bulunmuş, 10⁻⁴–10⁻⁵ mertebesinde bir desen"dir.

## 8. Dosyalar (commit edilmedi — yazarın kararı)

`kesif19_catallanma.py`, `kesif19_catallanma_sonuc.txt`, `kesif20_tipiklik.py`, `kesif20_tipiklik_sonuc.txt`, `kesif16_tekrar_2026-09-02.txt`, bu rapor.
