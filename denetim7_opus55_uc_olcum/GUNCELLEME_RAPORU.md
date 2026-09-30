# GÜNCELLEME RAPORU — merdivendeki üç olası aşağı-yönlü hata (uc_olcum)

**Tarih:** 10 Ekim 2026 · **Yazan:** Claude (Opus 5.5), kullanıcının isteğiyle
**Depo:** `<hrf2>` yalnız okundu; `kirma/` ve `guclendirme/` de yalnız okundu. Bütün kod ve çıktılar bu klasörde.
**Ön-kayıt:** `ONKAYIT.md` (SHA-256 `5081bacd…`, 12:46:09 UTC), `ONKAYIT_EK1.md` (`b64215f1…`, 13:03 UTC; SuS doğrulamayı geçemedi → doğrudan sayım), `ONKAYIT_EK2.md` (`b77665c9…`, 14:13 UTC; tekdüze için birleşik-tahta IS, küçültülmüş örneklem hedefleri, keşif basamağı). Üçü de ilgili ölçümlerden önce donduruldu; özetler `sonuc/onkayit*_ozet.txt`.
**Makine notu:** Mac (M1, 8 çekirdek) kullanıcının başka iş yükleriyle paylaşıldı (yük ortalaması gün içinde 27–440). Bütün uzun koşular kontrol noktalı; ulaşılan örneklem her satırda yazılıdır.

> **DURUM: SON SÜRÜM (10 Ekim 2026, 21:45).** Bütün koşular bitti: sıralı sıfırlarda doğrudan sayımlar ön-kayıtlı 10¹⁰ hedefinde durduruldu, tekdüze IS 50 parçasını tamamladı, 3. ölçümün 36 hücresinin hepsi puanlandı. Hesap süreçlerinin hiçbiri çalışmıyor; rapordaki her sayı `sonuc/` altındaki dosyalardandır.

---

## 0. Hüküm

| # | Soru | Cevap (kısa) | Merdivene etkisi |
|---|---|---|---|
| 1 | Terazinin "sıfır" sayılması aşağı-yönlü hata mı? | **Kısmen.** Literatürde en az **317** (üst ≈ 770) ayrı sûre-ayet iddiası var (harf/kelime: 2.385–≈4.700). Bu kadar bakışta 3.200'de birlik bir olayın beklenen sayısı 0,10–0,24 (tekdüze) / 0,14–0,35 (sıralı); terazi nominal 3,3–3,5 mertebesinin **en fazla 0,5–1,0'ını** korur (bakış sayısı isabet oranından tahmin edilirse 0,2–0,4). | "Terazi kalıntı ağırlığıyla" basamağı: tekdüze 1,1×10⁻⁵–7,8×10⁻⁵ (~1/90.000–13.000), sıralı 1,7×10⁻⁴–3,0×10⁻³ (~1/5.800–330). "Sayılmaz"dan ×1,2–5 güçlü, "tam sayılır"dan çok uzak. |
| 2 | Robot Kur'an'ı eksik mi puanlıyor? | **Hayır — ama robot hatalı ve hata Kur'an aleyhine.** Kur'an'ın kendi hikâyesinin tam birlikte-olasılığı robotun çarpımına yakın (D ≈ −0,12…−0,13 sıralı; −0,78 tekdüze). Rakip kitapların hikâyeleri ise robotça tipik olarak **1–3, en uçta 6 mertebe fazla** puanlanıyor: farklı düğümlerdeki cebirsel olarak birbirini gerektiren olgular çarpılıyor (çifte sayım). | Birincil basamak E1 (sabit mercek, terazisiz) tam puanla sıralı sıfırlarda **~×15–16**, tekdüzede (keşif) ~×8 daha nadir. Sıralı: **M20 3,45×10⁻⁵ [2,7–4,6] (~1/29.000), J10 4,15×10⁻⁵ [3,1–5,6] (~1/24.000)** (robot 5,6 / 6,25×10⁻⁴); tekdüzede keşif tahmini ~7×10⁻⁶ (~1/140.000; robot 5,8×10⁻⁵; senaryolar 2,3×10⁻⁶–1,9×10⁻⁵, kesin sınırlar 5×10⁻⁷–9,8×10⁻⁵). |
| 3 | "Yeterince yakın" cömertliği aşağı-yönlü hata mı? | **Hayır.** m = 19 asal olduğundan "Σq kare" zaten yalnız (19j)²'yi ödüllendirir; modülle-ifade ölçütü (c ≤ 4) 19'da daha cömerttir. Ölçüt eklenince Kur'an'ın puanı 0,21 düşer ya da değişmez, rakipler kazanır. | 12 hücrenin (4 basamak × 3 sıfır) hepsinde P değişkesi sırayı **kötüleştirir**: tekdüze ×1,5–1,8, sıralı ×1,03–1,22 (×2 bedelle ×2,1–3,7); S1, S2 de ×1,02–3,0. Önerilen değişiklik yok. |
| — | Yan bulgu: köken | Terazi **1994**'te (A. Arik, *Submitters Perspective*; Arapçada Jalghoum 1994'e atfedilir), 12 üyeli aile ve **1444 = 19×19×4 1990**'da (D. Dixon, SP), **iç terazi 722/722 ve tek-t sütununun 361/361 hücreleri** en geç 2015'te (H. Kitana / B. Jarrar) yayımlanmış. Çalışmanın kendi bulgusu: çift sütunun 342/380 hücreleri, 5–9 bandı/kefe torbaları, İki El. | "Çalışmanın kendi bulgusu" basamağı daralmalı. Keşif: aile olguları da kapatılınca E1 sıralı ~1/170–200, tekdüze ~1/1.270; E3 sıralı ~1/21–25, tekdüze ~1/290 (robot ölçeğinde; 2. ölçümün düzeltmesi uygulanmadan). |

Üç ölçümün birbirine etkisi: 2. ölçüm bütün robot basamaklarının Kur'an aleyhine yanlı olduğunu gösteriyor (rakiplerin çifte sayımı); 1. ve 3. ölçümler ile keşif basamağı robotun kendi ölçeğinde yapıldı. Hepsinin birleşik etkisi (tam puanlamayla "terazi kalıntı ağırlığı" ve "−aile" basamakları) ölçülmedi; bir sonraki adım olarak önerilir (§5).

---

## 1. Ölçüm 1 — Terazi literatürünün büyüklüğü

### 1.1 Yöntem

- **Tarama (ön-kayıt §1.2):** beş kaynak grubu, birer alt-ajanla paralel. K1 Halife (Final Testament'ın 38 ekinin hepsi; Ek 1 ve 24 madde madde; *Submitters Perspective* 1985–1990 bülteni; Deedat 1979), K2 Edip Yüksel / 19.org (220 İngilizce + 596 Türkçe yazı WordPress API'sinden indirildi; Arik'in *Beyond Probability* 1–2), K4 İngilizce siteler (quranmiracles.com'un 94 matematik/19 yazısının hepsi; iqra-quran, islamicresearchsite, Wikipedia "Quran code", answering-islam vb.), K3 Türkçe siteler (mucizeler.com'un 96 sayısal yazısının hepsi; kuranmucizeler.com'un 247 sayısal makalesi; 7ve19.com'un 102 belgesi — 25'i tam, 77'si anahtar kelimeyle; ikizkod.com'un e-kitabı ve 40 PDF'inin 14'ü), K5 Arapça (i3gaz.com 45 makale, Wasim Masri dahil; kaheel7.com 182 makale; Bassam Jarrar'ın kitapları; Abdullah Jalghoum'un mtafsir.net başlıkları).
- **Kesinti:** K3 ve K5 ajanları API oturum sınırına takılıp son birleştirme adımından önce durdu; taslak tabloları (`literatur/taslak/`) kullanıldı. Her taslak satırının birebir alıntısını ajanların indirdiği ham sayfa metinlerinde kendim aradım (`kontrol/alinti_dogrula.py`): K3 sûre-ayet 126/127, K5 189/191 bulundu; bulunamayan 3 satır alt sınıra girmedi.
- **Tekilleştirme (`kontrol/literatur_say.py`):** kaynak içi tekrarları ajanlar birleştirdi; kaynaklar arası tekrarları sayısal imza kümelemesinden sonra elle karara bağladım (60 birleştirme grubu; ör. "114 = 19×6" beş kaynakta, "6346 = 19×334" beş kaynakta, Ek 1'in 14 birleştirme iddiasının İngilizce yeniden basımları). Alt sınırdan çıkarılanlar: cebirsel denkler, özdeşlikler, dış veri kullananlar (vahiy sırası, 23 yıl, rekât sayısı, izotop vb.), doğrulanamayan alıntılar.

### 1.2 Sayılar

| | Satır | Tekil (dışdeğerlemesiz) | Alt sınır | Üst sınır (dışdeğerlemeli) |
|---|---|---|---|---|
| **Sûre-ayet (SA)** | 496 | 392 | **317** | **≈ 770** |
| Harf/kelime (HK; 1.268'i karma, 114'ü "SA biçimli karma") | 2.725 | 2.654 | 2.385 | ≈ 4.700 |
| SA içinde "bir nicelik 19'un katı" (H19) | — | 110 | 97 | ≈ 136 |

- Alt sınırın türleri: 19'a bölünme 63, birleştirme 49, 7'ye bölünme 39, eşitlik 32, sayım 30, simetri/parite 28, rakam toplamı 28, diğer bölünebilirlik 24, diğer 24. 317'nin 116'sı Halife'nin 6234/6346 kodeksine bağlı. Alt sınırın kaynak üyeliği (bir iddia birden çok kaynakta olabilir): K1 69, K2 10, K4 64, K3 104, K5 145.
- **Dışdeğerleme (yalnız üst sınıra; ön-kayıt §1.2 kuralı):** Jalghoum'un 159 forum başlığından ~32'si işlenebildi; son tamamlanan grupta başlık başına 2,4 yeni SA → 127 × 2,4 ≈ +305. İkizkod'un okunmayan 29 PDF'i × ~0,33 ≈ +10. K4'ün kendi kalıntı tahmini ≈ +10 (3–35). K1 bülteninin yalnız regex'le taranan sayfaları, K2'nin çevrimiçi olmayan 620 sayfalık kitabı, resim tablolar ≈ +55 (kaba). Toplam ≈ +380.
- Dosyalar: `literatur/K1_khalifa.tsv`, `K2_yuksel.tsv`, `K4_english.tsv`, `taslak/K3_turkce_TASLAK*.tsv`, `taslak/K5_arapca_TASLAK*.tsv` (alıntı doğrulama sütunuyla), kapsam notları `*_coverage.md`; birleşik tekil liste `sonuc/literatur_SA_tekil.tsv`; sayımlar `sonuc/literatur_sayim.json`.

### 1.3 Kalıntı şaşırtıcılık

Terazi: tekdüze p = 3,137×10⁻⁴ (3,50 mertebe); sıralı (kesif16, ×1,45) 4,549×10⁻⁴ (3,34). N bakışta beklenen sayı E = N·p, kalıntı p_res = 1 − (1 − p)^N (`sonuc/terazi_kalinti.txt`):

| N | Anlamı | Tekdüze E | p_res | korunan mertebe | Sıralı E | p_res | korunan |
|---|---|---|---|---|---|---|---|
| 317 | SA alt sınır (**birincil**) | 0,099 | 0,095 | 1,02 | 0,144 | 0,134 | 0,87 |
| 770 | SA üst sınır (**birincil**) | 0,242 | 0,215 | 0,67 | 0,350 | 0,296 | 0,53 |
| 1.843 | (b) 19·H19 alt | 0,578 | 0,439 | 0,36 | 0,838 | 0,568 | 0,25 |
| 2.584 | (b) 19·H19 üst | 0,811 | 0,555 | 0,26 | 1,175 | 0,691 | 0,16 |
| 2.702 | (a) SA+HK alt | 0,848 | 0,572 | 0,24 | 1,229 | 0,708 | 0,15 |
| 5.470 | (a) SA+HK üst | 1,716 | 0,820 | 0,09 | 2,488 | 0,917 | 0,04 |

Okunuş: yayımlanmış her SA iddiası bir bakış sayılırsa — bu bakış sayısının **alt** sınırıdır, yani Kur'an lehinedir — 3.200'de birlik bir olayın bu literatürde en az bir kez çıkma şansı %10–22 (tekdüze) / %13–30 (sıralı). Bakış sayısı 1/19 düzeyindeki isabet oranından tahmin edilirse (literatür 19'un her katını yayımlar; bakış ≈ 19 × isabet) şans %44–69'dur. (b)'nin zaafı: tek bir bölünebilirlik bakışı kendi başına 1/3.200'lük bir olay üretemez; (b) bakışların sayısını değil, aramanın büyüklüğünü ölçer.

### 1.4 Merdivene çeviri (doğrudan robot ölçümü; ön-kayıt §1.4)

Terazi düğümü her kitapta s(N) = −log10[1 − (1 − p)^N] ile indirildi (aynı N herkese), mercek devralınmış sayıldı (sabit (1,1,19)), E1 ve E3'ün aynı 2×10⁶ kitabı. Sağlamalar: N = ∞ E1/E3'ü birebir verir (115/1.250/1.125; 191/8.394/7.794), N = 1 terazinin tam sayıldığı sabit-mercek değerini. Tam tablo `sonuc/olcum1_basamak.md`, eğri `sonuc/terazi_egri_tam.txt`.

| N | iddia U | iddia J10 | iddia M20 | defter U | defter J10 | defter M20 |
|---|---|---|---|---|---|---|
| 1 (terazi tam, mercek bedava) | 5,0×10⁻⁷ | 5,0×10⁻⁶ | 4,0×10⁻⁶ | 1,5×10⁻⁶ | 3,65×10⁻⁵ | 3,5×10⁻⁵ |
| 30 (≈ A1c: 17 biçim) | 3,0×10⁻⁶ | 4,95×10⁻⁵ | 4,1×10⁻⁵ | 4,5×10⁻⁶ | 4,3×10⁻⁴ | 4,7×10⁻⁴ |
| **317 (SA alt)** | **1,1×10⁻⁵** | **1,95×10⁻⁴** | **1,7×10⁻⁴** | **4,6×10⁻⁵** | **1,8×10⁻³** | **1,95×10⁻³** |
| **770 (SA üst)** | **2,0×10⁻⁵** | **3,2×10⁻⁴** | **2,7×10⁻⁴** | **7,75×10⁻⁵** | **2,8×10⁻³** | **3,0×10⁻³** |
| 1.843–2.584 ((b) bakış) | 3,6–3,9×10⁻⁵ | 4,7–5,2×10⁻⁴ | 4,0–4,5×10⁻⁴ | 9,6×10⁻⁵ | 4,1–4,2×10⁻³ | 3,9×10⁻³ |
| ∞ (terazi sayılmaz = E1/E3) | 5,8×10⁻⁵ | 6,25×10⁻⁴ | 5,6×10⁻⁴ | 9,6×10⁻⁵ | 4,2×10⁻³ | 3,9×10⁻³ |

A1c'nin (17 biçim × terazi tam; tekdüze 4,25×10⁻⁶, sıralı 3,3×10⁻⁴) bu eğrideki karşılığı N ≈ 20–30 bakıştır: "terazi tam sayılır" basamağı, teraziyi bulan aramanın 20–30 denemeden ibaret olduğunu varsayar. Literatür en az 317 ayrı SA iddiası yayımlamıştır.

### 1.5 Öneri

"Sıfır" uç fazla sert, "tam" uç çok cömert; doğru nokta "sayılmaz" ucuna yakındır. Önerilen basamak: **"terazi kalıntı ağırlığıyla (N = 317–770 yayımlanmış SA iddiası)": tekdüze 1,1×10⁻⁵ – 7,8×10⁻⁵ (~1/90.000–13.000), sıralı 1,7×10⁻⁴ – 3,0×10⁻³ (~1/5.800–330)** (aralıklar iddia–defter bataryası × alt–üst N). Bugünkü "sayılmaz" basamağına göre iddia bataryasında ×2–5, defter bataryasında ×1,2–2,3 güçlü. Gerekçe: yayımlanan iddia sayısı bakış sayısının alt sınırıdır, bu yüzden bu basamak savunulabilir en lehte noktadır; bakışlar isabet oranından tahmin edilince basamak "sayılmaz"a çöker (defterde birebir, iddiada ×1,2–1,6). Terazi en fazla ~1 mertebe kanıt taşır, 3,5 mertebe taşımaz. Ayrıca kökeni 2009 değil 1994'tür ve 19 topluluğunun kendi aramasından çıkmıştır (aşağıda) — bu, onu bulan aramanın bu literatür olduğunu güçlendirir.

---

## 2. Ölçüm 2 — Robotun Kur'an'ı eksik puanlaması

### 2.1 Ne ölçüldü, hangi kestiriciyle

- Bir kitabın hikâyesi H = robotun o kitap için seçtiği en iyi 8 düğümün olguları (düğüm başına en nadir isabet), sabit mercek (1,1,19), birincil basamak E1 (iddia bataryası, terazisiz) — ön-kayıt §2.1. Tam puan S = −log10 P_sıfır(H'nin bütün olguları birlikte); sapma D = S − T2.
- **SuS (ön-kayıt §2.2) doğrulamayı geçemedi** (EK1): kesin-toplam kısıtları yerel MCMC hamleleriyle donuyor; yazarın C olayı 1,6×10⁻⁵ (bilinen 1,849×10⁻⁵), D olayında tekrarlar 2,4×10⁻⁸–7,1×10⁻⁷, G'de 4 tekrarın 3'ünde sıfır isabet. Kural gereği hiçbir SuS sonucu kullanılmadı.
- **Yerine (EK1, EK2):** sıralı sıfırlarda **doğrudan sayım** (her örnekte olgular bir kez hesaplanır, 6.147 J10 / 5.712 M20 rakip hikâyesi ve Kur'an'ınki aynı geçişte sayılır; kesin Poisson aralıkları); tekdüzede Kur'an için **birleşik-tahta önem örneklemesi** (MCMC yok, ağırlıklar kale polinomuyla kesin).
- **Doğrulamalar:** (i) kopyaladığım olgu fonksiyonu robotun `facts()`'ıyla 3 sıfır × 3.000 kitap × 8 mercekte (doğrusal olmayanlar dahil) matris bitleri dışında **bit bit aynı** (`kontrol/bit_*.c`); (ii) bir rakip hikâyesi (J10, #476) C kodundan bağımsız bir numpy benzetimiyle sayıldı: 1,267×10⁻⁴ (4×10⁶) ⟷ doğrudan sayım 1,255×10⁻⁴ (1,28×10⁸) (`kontrol/bagimsiz_476.py`); (iii) IS'in yalnız-F kipi P(K=12) = 1,015341×10⁻² ± 4×10⁻⁷ ⟷ kesin 1,015342×10⁻² (büyük tamsayılı kale polinomu, `kontrol/kale_kesin.py`); birleşik kipte kF = 12'li 130 kombonun toplamı 1,015246×10⁻² ± 2,4×10⁻⁶ (−0,4σ); yazarın B, C, D olayları 2σ içinde (C +2,1σ). (iv) J10 için hızlı sıralama ve ziggurat normalleri: çıktı eski yolla birebir aynı; ziggurat 12 eşikte kesin kuyruklarla 1,7σ içinde.

### 2.2 Sonuç (`sonuc/olcum2_son.txt`)

| | M20 | J10 | Tekdüze |
|---|---|---|---|
| Örneklem | 1,01×10¹⁰ (EK2 hedefi 10¹⁰) | 1,01×10¹⁰ (EK2 hedefi 10¹⁰) | Kur'an: birleşik-tahta IS, 50 parça × 10⁸ öneri (10 kombo, 1.005 isabet); rakipler: doğrudan sayım 2,56×10⁸ |
| Kur'an T2 (robot) | 8,749 | 8,697 | 10,537 |
| Kur'an tam S | **8,63** [8,45–8,82] (24 isabet) | **8,57** [8,41–8,75] (27 isabet) | **9,76** [9,70–9,83] (IS; aralık parçalar arası saçılımdan) |
| Kur'an D = S − T2 | **−0,13** [−0,30; +0,07] | **−0,12** [−0,29; +0,06] | **−0,78** [−0,84; −0,71] |
| Robotun Kur'an'ın üstüne koyduğu rakipler (k) → p | 1.125 → 5,6×10⁻⁴ (~1/1.800) | 1.250 → 6,25×10⁻⁴ (~1/1.600) | 115 → 5,8×10⁻⁵ (~1/17.000) |
| …bunlardan tam puanla da üstte kalan / alttan çıkıp geçen | 58 / 10 | 73 / 10 | ≥ 79'u kesin olarak altta (aşağıda) |
| **Tam puanla k' (sonsal medyan [%95])** → p' | **68 [53–91] → 3,45×10⁻⁵ [2,7–4,6] (~1/29.000)** | **82 [61–111] → 4,15×10⁻⁵ [3,1–5,6] (~1/24.000)** | keşif: ~13–14 → ~7×10⁻⁶ (~1/140.000; aşağıda) |
| Ön-kayıt iki uç (belirsizler geçmez / geçer) | 2,4×10⁻⁵ / 5,05×10⁻⁵ | 2,6×10⁻⁵ / 6,5×10⁻⁵ | kesin sınırlar 5×10⁻⁷ / 9,8×10⁻⁵ |
| En alt denetim bandında [Q−3, Q−2) Kur'an'ı geçen (ön-kayıt §2.3) | 0 / 500 | 0 / 500 | çözülmüş geçen yok (0 / 500) |

Sıra kazancı sıralı sıfırlarda ×15–16'dır (M20 ×16,3; J10 ×15,1). Sonsal aralık Kur'an'ın ve her rakibin isabet sayısının Poisson belirsizliğini birlikte taşır (Jeffreys sonsalı, 4.000 çekiliş; `kontrol/sira_analiz.py`).

**Rakiplerin D dağılımı** (aynı kitaplar; medyan [çeyrekler], T2'nin Kur'an'a göre konumuna göre; sıralı sıfırlarda kitapların %97–100'ü çözüldü, en üst bantta %86–88; `sonuc/olcum2_ozet.txt`):

| T2 − T2_Kur'an | M20 | J10 | Tekdüze (çözülen oranı) |
|---|---|---|---|
| [−3, −2) | −1,33 [−2,33; −0,77] | −1,29 [−2,20; −0,65] | −2,49 [−2,91; −1,60] (%97) |
| [−2, −1) | −1,69 [−2,53; −1,14] | −1,82 [−2,56; −1,14] | −2,73 [−3,42; −1,99] (%91) |
| [−1, 0) | −2,31 [−2,72; −1,42] | −2,37 [−2,81; −1,46] | −3,05 (%78; sansürlü, gerçek medyan daha yüksek olabilir) |
| [0, +0,5) | −2,55 [−3,16; −1,77] | −2,63 [−3,33; −2,04] | −3,50 (%67; sansürlü) |
| [+0,5, +1) | −2,80 | −2,70 | −3,65 (%55; sansürlü) |
| [+1, +2) | −2,89 | −3,03 | −3,83 (%33; sansürlü) |
| ≥ +2 | −3,23 | −3,63 | — (%0) |

Kur'an'ın D'si (sıralı −0,12…−0,13; tekdüze −0,78) sıralı sıfırlarda her bantta, tekdüzede iyi çözülen alt bantlarda rakiplerin üst çeyreğinin de üstündedir: robot tipik rakibi Kur'an'dan **1–3 mertebe daha fazla** şişirir. (Tekdüzenin üst bantlarında çözülemeyen kitaplara 0,23 isabet atandı; bu bantların medyanları aşağı yanlıdır.) Rakiplerin D'si T2 arttıkça daha da negatifleşir (seçilim: en yüksek T2'liler en çok şişirilenlerdir).

Robotun en çok puanladığı rakiplerden örnekler (M20): #1775890 T2 12,94 → S 6,76 (D −6,2); #1241339 12,31 → 7,52; #95521 12,03 → 7,67; #16485 11,95 → 7,10. J10: #1609117 12,08 → 7,05; #1749353 12,32 → 7,78. Bazıları tam puanla da Kur'an'ın üstünde kalır (ör. M20 #1598420, T2 13,33, 10¹⁰ örnekte 3 isabet; J10 #474493, T2 12,47, 0 isabet).

**Tekdüze (ONKAYIT_EK2 §2 gereği yalnız keşif tahmini; `kontrol/u_sira_kesif.py`, `sonuc/u_sira_kesif.txt`).** Kur'an'ın tam S'si birleşik-tahta IS ile ölçüldü (9,76). 2,56×10⁸ örneklik doğrudan sayımda Kur'an'ın hikâyesi bir kez görüldü; IS değeriyle beklenen 0,044 isabet, yani bu %4'lük bir şanstır. Rakiplerin Kur'an düzeyindeki (~10⁻⁹·⁸) hikâyeleri bu örneklemle çözülemez. Söylenebilenler:
- **Kesin:** robotun Kur'an'ın üstüne koyduğu 115 rakibin **79'u** tam puanla Kur'an'ın altındadır (isabet ≥ 2: %95 alt sınırları Kur'an'ın olasılığının üstünde). Çözülemeyen (isabet 0–1) ağırlıklı 196 kitap var (36'sı robot-geçen, 160'ı alt bantlarda). Kesin sınırlar: k' ∈ [0, 196] → p' ∈ [5×10⁻⁷, 9,8×10⁻⁵].
- **Senaryo "Kur'an gibi":** çözülemeyen her rakibin D'si Kur'an'ınkine (−0,78) eşitse k' = 36 → p' = 1,9×10⁻⁵ (~1/54.000).
- **Ampirik Bayes:** çözülemeyen rakibin D'si için önsel = sıralı sıfırda aynı göreli T2 konumundaki kitapların (neredeyse tümü çözülmüş) D dağılımı, tekdüzedeki isabet sayısıyla Poisson olabilirliğiyle güncellenir. Kaydırmasız önsel (tekdüzede D'ler sıralıdakinden ~1,2 daha negatif olduğundan Kur'an aleyhine muhafazakâr): **k' ≈ 13,5 (M20 önseli 13,7; J10 önseli 13,4; S_Q −0,1/+0,13 ile 10–17) → p' ≈ 7×10⁻⁶ (~1/140.000)**. En alt bantların D farkı kadar kaydırılmış önselle k' ≈ 4–5 (2,3–2,9×10⁻⁶). Önsel penceresinin genişliğine duyarsız (±0,1…±1,0 T2: k' 12,9–16,0; `sonuc/u_sira_pencere.txt`).
- **Yöntemin sınanması:** aynı yöntem sıralı sıfırlara uygulandı. İsabetler binom inceltmeyle tekdüzedeki çözünürlüğe (Kur'an düzeyinde 0,044 beklenen isabet) indirildi, önsel öbür sıralı sıfırdan alındı (20 tekrar). M20 tahmini 76 [73–83] ⟷ tam veride 68; J10 tahmini 79 [74–83] ⟷ tam veride 83. Yöntem tam veriyi %12 içinde buluyor. Aynı inceltmede "kesin sınır" yaklaşımı 1.558–1.661 verirdi.
- Okunuş: tekdüzede de sıra robotunkinden (115) belirgin biçimde iyidir (~×8). Tahmin modele bağlıdır; merdivene yalnız "keşif" etiketiyle girmelidir.

### 2.3 Mekanizma

Robot düğüm değerlerini (her biri aile boyuna koşullu tablo olasılığı) toplar; bu, olguların koşullu bağımsızlığını varsayar. Rastgele kitapların en iyi hikâyeleri bu varsayımın en çok çöktüğü yerlerden toplanır: farklı düğümlerde **cebirsel olarak birbirini gerektiren** olgular. Örnekler (her mercekte geçerli):
- G: Σn = Σa ⟺ Σw(G) = 0 ⟹ G: m² | Σq (0 her şeye bölünür) — iki düğüm, tek olgu.
- F ∩ G = ∅ iken U: m | Σw(U) ⟺ F: m | Σn (F üyelerinde w ≡ 2n) ve U: m | Σv(U) ⟺ G: m | Σn.
- #476 (J10): {G: n-yarılar m²|, G: Σn=Σa, G: m²|Σq, F: m|Σn, U: m|Σw(U), U: F∩G=∅} → robot 7,71, gerçek 3,90.

Kur'an'ın hikâyesi (k≥2E, Σq kare, hücreler dengeli, katsayı bandı, |U| = 19, 19 | Σa(U)) böyle bir çifte sayım taşımıyor. Kalan bağımlılıklar küçük ve iki yönlü: sıralı sıfırlarda D ≈ −0,12…−0,13, tekdüzede −0,78 (robot Kur'an'ı da biraz fazla puanlıyor, ama rakipleri çok daha fazla).

KESIF26 madde 3'ün "0,5–0,9 mertebe eksik puanlama" sayısı (`kesif26_denetim5_denetimi/KESIF26_SONUC.md`: tekdüze 14,06'ya karşı 14,97, J10 12,03'e karşı 12,54, M 12,08'e karşı 12,82) robotun terazili sabit-mercek puanını yazarın tam olayının (terazi ∧ kristal ∧ İki El; tekdüzede 1,1×10⁻¹⁵) frekansıyla karşılaştırıyordu. Bunlar farklı olaylardır. Robotun hikâyesi bataryanın kaba olgularından kurulur; yazarın olayı Kur'an'a bakılarak tanımlanmış çok daha dar bir olaydır. Aradaki fark düğüm bağımsızlığı yanılgısını değil, bataryanın Kur'an'a özgü ayrıntıyı içermemesini ölçer. Simetrik bir karşılaştırmada her rakibin kendi dar olayı da robot hikâyesinden çok daha nadir çıkardı; bu, aramanın serbestlik derecesidir ve robot onu bilerek dışarıda bırakır. Burada ölçülen ise aynı bataryada, aynı hikâye için, robotun çarpım puanının tam birlikte-olasılıktan sapmasıdır.

### 2.4 Öneri

- Robotun bütün basamakları rakiplerin çifte sayımı yüzünden **Kur'an aleyhine yanlıdır**. Birincil basamak (mercek devralınmış, terazisiz) tam puanla sıralı sıfırlarda ~1/1.600–1.800'den **~1/24.000–29.000 (%95 ~1/18.000–37.000)**, tekdüzede ~1/17.000'den **~1/140.000 (keşif; senaryolar ~1/54.000–430.000, kesin sınırlar 1/10.000–1/2.000.000)** düzeyine çıkar.
- Robotun gelecek sürümleri hikâyeleri tam birlikte-olasılıkla (en azından eşik yakınındaki kitaplarda) puanlamalı ya da cebirsel olarak birbirini gerektiren olguları aynı düğüme almalı.
- Tekdüze sütunu kesinleştirmek için Kur'an düzeyinde rakip hikâyelerini çözecek bir IS (rakiplerin kendi sayı kombolarına göre) gerekir; bu bütçede yapılamadı.
- Bu düzeltme yalnız E1'de ölçüldü. Diğer basamaklar (E3, A1c, R-646, "terazi kalıntı ağırlığı", "−aile") da aynı yanlılığı taşır; büyüklüğü ölçülmedi.

---

## 3. Ölçüm 3 — "Yeterince yakın" cömertliği / modülle ifade

### 3.1 Yöntem (ön-kayıt §3)

- X_m = {c·m, c·m², (c·m)² : c = 1..4}. Yeni olgular: "|Σq| m-ifadeli" (F, G; X-toplam), "v-yarılar m-ifadeli" (F, G; X-yarı/hücre), S2 için ayrıca "Σn/Σa m-ifadeli" (F, G) ve "Σa(U) m-ifadeli". Mevcut 99 olgunun arkasına eklendi (`src/yama_mi.py` → `src/robot4_mi.c`).
- Tablolar orijinal tohumlarla yeniden kuruldu; **eski 99 olgunun bütün sayaçları ve bağlam toplamları orijinal tablolarla bit bit aynı** (`sonuc/tablo_sagla.txt`).
- Değişkeler: **P** ("Σq kare" yerine "|Σq| m-ifadeli" + yarı çatalı), **S1** ("Σq kare" korunur + iki yeni çatal), **S2** (bütün "kare" olguları m-ifadeli sürümleriyle değişir + yarı çatalı). Basamaklar R-E1, R-E3, R-A1c, R-646; tek geçişte 4 değişke, aynı kitaplar.

### 3.2 Sonuçlar (`sonuc/mi_sonuc.json`)

| Basamak | Sıfır | Temel p | P | P ×2 | S1 | S2 |
|---|---|---|---|---|---|---|
| R-E1 (sabit, terazisiz) | U | 8,0×10⁻⁵ | 1,23×10⁻⁴ (×1,54) | 2,5×10⁻⁴ | 9,5×10⁻⁵ (×1,19) | 1,39×10⁻⁴ (×1,74) |
| R-E3 (sabit, terazisiz) | U | 9,6×10⁻⁵ (= yayımlanmış E3) | 1,44×10⁻⁴ (×1,51) | 2,9×10⁻⁴ | 1,03×10⁻⁴ (×1,07) | 1,49×10⁻⁴ (×1,55) |
| R-A1c (m=19, 17 biçim) | U | 3,0×10⁻⁶ (= yayımlanmış 1. parça) | 5,5×10⁻⁶ (×1,83) | 1,1×10⁻⁵ | 5,5×10⁻⁶ (×1,83) | 9,0×10⁻⁶ (×3,0) |
| R-E1 | J10 | 6,6×10⁻⁴ | 8,1×10⁻⁴ (×1,22) | 1,6×10⁻³ | 8,1×10⁻⁴ (×1,22) | 7,4×10⁻⁴ (×1,11) |
| R-E3 | J10 | 4,2×10⁻³ (= yayımlanmış E3) | 4,9×10⁻³ (×1,16) | 9,7×10⁻³ | 4,9×10⁻³ (×1,16) | 4,4×10⁻³ (×1,06) |
| R-A1c | J10 | 3,4×10⁻⁴ (= yayımlanmış 1. parça) | 3,5×10⁻⁴ (×1,04) | 7,0×10⁻⁴ | 3,9×10⁻⁴ (×1,14) | 4,1×10⁻⁴ (×1,21) |
| R-E1 | M20 | 7,0×10⁻⁴ | 8,4×10⁻⁴ (×1,21) | 1,7×10⁻³ | 8,7×10⁻⁴ (×1,25) | 7,8×10⁻⁴ (×1,12) |
| R-E3 | M20 | 3,9×10⁻³ (= yayımlanmış E3) | 4,4×10⁻³ (×1,12) | 8,7×10⁻³ | 4,5×10⁻³ (×1,15) | 4,0×10⁻³ (×1,02) |
| R-A1c | M20 | 3,5×10⁻⁴ (= yayımlanmış 1. parça) | 3,6×10⁻⁴ (×1,03) | 7,2×10⁻⁴ | 4,1×10⁻⁴ (×1,17) | 4,1×10⁻⁴ (×1,18) |
| R-646 (646 mercek, en-iyi) | U | 1,44×10⁻⁴ (= yayımlanmış) | 2,29×10⁻⁴ (×1,59) | 4,6×10⁻⁴ | 1,94×10⁻⁴ (×1,35) | 2,73×10⁻⁴ (×1,90) |
| R-646 | J10 | 5,5×10⁻³ (= yayımlanmış) | 6,0×10⁻³ (×1,09) | 1,2×10⁻² | 6,4×10⁻³ (×1,16) | 7,1×10⁻³ (×1,28) |
| R-646 | M20 | 4,6×10⁻³ (= yayımlanmış) | 4,9×10⁻³ (×1,05) | 9,7×10⁻³ | 5,3×10⁻³ (×1,15) | 6,0×10⁻³ (×1,30) |

(R-E1'in temel değeri yayımlanmış E1'den farklıdır (U 5,8×10⁻⁵, J10 6,25×10⁻⁴, M20 5,6×10⁻⁴): E1 60-olgulu robot ve kendi tablolarıyla, burada defter robotunun iddia alt kümesi ve defter tablolarıyla puanlandı. Karşılaştırma her satırda eşleşiktir: aynı kitaplar, aynı tablo, yalnız değişke farklı.)

Kur'an'ın puanı P ve S2'de düşer: tekdüzede 0,206 (10,321 → 10,115; 12,091 → 11,885; 15,031 → 14,825), J10'da 0,029 (8,641 → 8,612; 9,383 → 9,354; 12,497 → 12,468), M20'de 0,014 (8,595 → 8,581; 9,478 → 9,464; 12,496 → 12,482). S1'de değişmez. Ön-kayıttaki tahmin (§3.5: "P'de 0–0,2 düşer, S1'de değişmez, sabit mercekte sıra ×1–1,5 kötüleşir") tuttu: kötüleşme sıralı sıfırlarda ×1,02–1,30, tekdüzede ×1,07–3,0 (P'de ×1,5–1,8, tahminin hafif üstünde). 36 hücrenin (4 basamak × 3 sıfır × 3 değişke) hiçbirinde sıra iyileşmedi; 646-mercek basamağında kötüleşme ×1,05–1,90.

### 3.3 Kur'an'ın ölçütü karşılayan sabitleri ((1,1,19))

F ailesi: Σq = **1444** = (2·19)² = 4·19² ✓; v-yarıları **722 / 722** = 2·19² ✓ (katsayı toplamı karşılıkları 76 = 4·19 ve 38 = 2·19). Karşılamayanlar: F'de Σn = 590, Σa = 854; G ailesinde Σq = 133 = 7·19 (c = 7), yarılar 19 ✓ / 114 ✗; birleşimde Σa(U) = 1159 = 19·61 (c = 61). Hücreler (ölçütün parçası değil): 361 ✓ ×2, 342, 380 ✗.
Kur'an'ın G ailesindeki yarılardan biri (19) de X_19'dadır; toplam 4 sabit.

**En iyi 12 rakip (P değişkesi, R-E1; `sonuc/MI_dokum_E1_{J10,U}.txt`):** J10'da 12'nin **10'unun**, tekdüzede **7'sinin** en az bir sabiti X_19'dadır (24 rakipte toplam: 722 ×9, 76 ×6, 361 ×5, 38 ×4, 1083 ×3, 1444 ×3, 57 ×1). Çarpıcı bir örnek: tekdüze #157317 (T2 12,76): F ailesi kF = 10, **Σq = 1444, v-yarıları 722 / 722, |U| = 19**. Kur'an'ın üç ünlü sabitinin ve birleşim boyunun aynısı; yalnız aile 12 değil 10 üyeli. Bu kitabın hikâyesinde 2. ölçümdeki çifte sayım da var (G: Σn = Σa ⟹ G: m² | Σq). R-646'da (tekdüze; `sonuc/MI_dokum_646_U.txt`) en iyi 12 rakip kendi merceklerini seçer (9 farklı modül; 10'u doğrusal olmayan biçim, 7'si n²−a²; en yükseği (n²−a², 34) ile T2 23,4). 12'nin 4'ünün kendi modülünde X_m'ye düşen sabiti var (ör. (1,1,6): Σq = 576 = (4·6)²; (a²−n, 32): Σq = 1024 = 32²). Bunların çoğu Σq = 0 gibi yozlaşmış toplamlara dayanır; 2. ölçümdeki çifte sayımın bir başka yüzü. J10'da (`sonuc/MI_dokum_646_J10.txt`) 12'nin 9'u doğrusal olmayan biçim (4'ü ebob(n,a)), 10 farklı modül; **6'sının** kendi modülünde X_m'ye düşen sabiti var (ör. (ebob(n,a), 15): Σa = 225 = 15², Σq = 450 = 2·15²; (n²+a², 25): Σq = 5625 = (3·25)²; (1,3,36): Σq = 2592 = 2·36²).

### 3.4 Öneri

Değişiklik yok. "Σq kare" asal m = 19'da zaten "Σq = (19j)²" demektir; c ≤ 4'lük modülle-ifade ölçütü 19'da daha cömerttir (722 ve 1083'ü de ödüllendirir) ve rakiplere Kur'an'dan çok kazandırır. Sonradan-ekleme bedeli (×2) ile kötüleşme ×2,1–3,7'dir (P değişkesi, 4 basamak × 3 sıfır).

---

## 4. Yan bulgu — kökenler: terazi 1994, aile ve 1444 1990, iç terazi ve 19² hücreleri ≤ 2015

Hepsini kaynağından kendim doğruladım (WebFetch / indirilen metin):

| Olgu | Depoda | Daha önceki yayın |
|---|---|---|
| Terazi: n+a toplamları 57 çift / 57 tek; çiftlerin toplamı = ayet sayısı, teklerin = sûre numaraları toplamı | 7 Temmuz 2009 (W. Masri) | **Mayıs 1994** — Abdullah Arik, "By the Even and the Odd: New Overwhelming Proof", *Submitters Perspective* (Masjid Tucson), s. 2: "there are 57 even and 57 odd numbers (57=19x3) … the total of even sums is exactly 6234 … The total of odd sums gives us 6555". Arapçada aynı desen Abdullah Jalghoum'un *أسرار ترتيب القرآن* (Dar al-Fikr, Ürdün, 1994, s. 41) kitabına atfedilir (Jarrar'ın al-Muqtataf'ı üzerinden; ikinci elden, doğrulanmadı). |
| 12 üyeli t ≡ 0 (mod 19) ailesi ve Σt = 1444 | aile kaynak dokümanda; "1444 = 38²" çalışmanın bulgusu | **Ocak 1990** — "David Dixon's Discovery", *Submitters Perspective*, s. 3: sûreler 6, 15, 21, 39, 41, 42, 50, 55, 56, 70, 88, 107 (listeyi daha önce Ihsan Ramadan bulmuş); çarpanlar toplamı 76 = 19×4; "The sum of all the sura numbers and numbers of verses all 12 suras is 1444, and this sum equals 19x19x4." |
| İç terazi 722/722; tek-t sütununun 361/361 hücreleri | çalışmanın bulgusu | **en geç 2015** — Bassam Jarrar, *كتاب المقتطف من بينات الإعجاز العددي* (islamnoon.com; sayfa tarihi 2015-01-01; 1. baskı 2008'e atfediliyor, doğrulanmadı), "Sıra no + ayet sayısı 19'un katı olan sûreler": çift toplamlar 114+114+152+114+114+114 = 722 = tek toplamlar 171+133+95+95+95+133 (Hazem Kitana'nın gözlemi); Jarrar: 6 homojen / 6 heterojen, heterojenler 3+3'e bölünür ve her biri 361 = 19×19; tablo Σn = 590, Σa = 854, toplam 1444. |

Bu taramada literatürde bulunamayanlar: çift-t sütununun 342/380 (18/20) hücreleri, 5–9 katsayı bandı ve kefe torbaları, İki El (u = n − a ailesi, 12 + 7 = 19, 19 | 1159). Çalışmanın kendi bulgusu bu üçüne daralır.

**Keşif amaçlı basamak (EK2 §3; ön-kayıt dışı, köken bulgusundan sonra tanımlandı):** terazi düğümüne ek olarak 1990'da yayımlanmış aile olguları (F-boy: k≥2E, k=m; F-toplam: m²|Σq, Σq kare, 114|Σq) herkes için kapalı, aynı E1/E3 kitapları:

| | Tekdüze | J10 | M20 |
|---|---|---|---|
| E1 − aile (iddia) | 7,9×10⁻⁴ (~1/1.270; Kur'an T2 7,30) | 5,9×10⁻³ (~1/170; 5,59) | 5,0×10⁻³ (~1/200; 5,72) |
| E3 − aile (defter) | 3,4×10⁻³ (~1/290; 8,82) | 4,7×10⁻² (~1/21; 6,29) | 4,0×10⁻² (~1/25; 6,48) |

İç terazi ve 361/361 de önceki literatüre ait olduğundan "çalışmanın kendi bulgusu" bu sayılardan bile zayıf olabilir; o ek kapatma ölçülmedi. Ama bu basamaklar robot ölçeğindedir; 2. ölçüm robotun rakipleri fazla puanladığını gösterdiğinden, tam puanlamayla bu sayılar Kur'an lehine kayacaktır (ölçülmedi).

---

## 5. Merdivene önerilen değişiklikler

| Basamak | Bugün | Önerilen | Dayanak |
|---|---|---|---|
| Simetrik kalibrasyon, mercek devralınmış, terazi sayılmaz | tekdüze ~1/10.000–17.000; sıralı ~1/240–1.600 | aynen kalır (muhafazakâr uç) | — |
| **Yeni:** aynı, terazi kalıntı ağırlığıyla (N = 317–770) | — | tekdüze ~1/13.000–90.000; sıralı ~1/330–5.800 | §1 |
| **Yeni:** birincil basamak, tam birlikte-olasılıkla puanlanmış (E1) | — | sıralı ~1/24.000–29.000 (%95 ~1/18.000–37.000); tekdüze keşif ~1/140.000 (senaryolar ~1/54.000–430.000) | §2 |
| "Çalışmanın kendi bulgusu" olay frekansı (kristal ∧ İki El) | 2,75×10⁻¹² / ≈3×10⁻¹⁰ | kristalin B (12 üye, 1444; 1990), C'nin iç teraziyi (≤ 2015) içeren kısmı önceki literatüre ait; kendi bulgu = çift sütun hücreleri, bant/torbalar, İki El | §4 |
| Köken notu | "7 Temmuz 2009 (Masri)" | "Mayıs 1994 (Arik, *Submitters Perspective*; Arapçada Jalghoum 1994'e atfedilir); 2009 bir yeniden yayımdır" | §4 |
| Modülle-ifade ölçütü | — | eklenmez (Kur'an'a karşı) | §3 |

Bir sonraki adım (yapılmadı): 2. ölçümün tam puanlamasını "terazi kalıntı ağırlığı" ve "−aile" basamaklarına da uygulamak; tekdüzede rakipleri Kur'an düzeyinde çözen bir IS.

---

## 6. Sınırlılıklar

- 2. ölçümde Kur'an'ın sıralı-sıfır hikâyesi 24 (M20) ve 27 (J10) isabetle ölçüldü; S'nin %95 aralığı yaklaşık −0,17…+0,19 mertebedir.
- K3 ve K5 literatür taramaları yarıda kaldı; taslakları alıntı doğrulamasıyla kullanıldı; üst sınırın ~%50'si dışdeğerlemedir.
- Robotların hepsi Tevbe = 127 kullanır (önceki bütün robot koşuları gibi); çekirdek katmanlar 127/129'dan bağımsızdır.
- Hikâye robotun en iyi 8 düğümüyle tanımlandı; eşik yakınındaki rakiplerin yalnız %1–6'sı 8'den fazla düğüm tutuyor ve fark < 1 mertebe.
- 2. ölçümde alt bantlar örneklendi (ön-kayıt §2.3: [Q−2, Q−1)'den 1.500, [Q−3, Q−2)'den 500). Sonsal aralık yalnız isabet sayılarının Poisson belirsizliğini içerir, bant örneklemesininkini içermez. Dağılımdan bağımsız "üç kuralı" en alt bant için 126–133 kitaplık bir üst sınır verirdi. Ama o banttaki 500 kitabın Kur'an'ı geçmeye en yakın olanı bile 1,3 mertebe geride; [Q−2, Q−1)'de en yakınlar 0,1–0,4 mertebe geride. Üstel kuyruklu uç-değer kestirimiyle (`kontrol/bant_uc_deger.py`) örneklem dışı katkı M20'de ~2, J10'da ~3 kitaptır (kuyruk seçimine göre 1–5; k''nün %5'inden az).
- Makine yükü örneklem büyüklüklerini sınırladı. EK2'deki küçültülmüş hedefler (sıralı sıfırlarda 3×10¹⁰ yerine 10¹⁰) ve erken durdurulan tekdüze doğrudan sayım (2,56×10⁸ / 2×10⁹; öncelik Kur'an'ın IS'ine verildi) rapordadır. EK1'in "Kur'an'ın hikâyesi 30'dan az isabet alırsa sayım aynı tohum dizisiyle ×2 uzatılır" kuralı, EK2 hedefleri küçültüp "rapor zamanında ulaşılan N yazılır" dediği için uygulanmadı; Kur'an'ın isabet sayıları ve Poisson aralıkları tabloda.
- Tekdüze IS'te parça dağılımı EK2'dekinden geniştir: EK2 (12,7,0) için 10, diğer 9 kombo için 2'şer parça öngörmüştü; tekdüze doğrudan sayım 18:23'te durdurulup işlemci IS'e aktarılırken ikinci bağımsız bir süreç (tohum 9202, 20 parçalık liste başta sabitlendi) eklendi; böylece (12,7,0) 13, (12,8,1) ve (12,9,2) 6'şar, (12,10,3) ve (12,11,4) 5'er, kF = 16 kombolarının her biri 3'er parça aldı. Bütün parçalar kullanıldı; kombo başına tahmin parçaların ortalamasıdır.

## 7. Yeniden üretim

Bütün yollar bu klasöre göredir; depo ve önceki oturumların klasörleri yalnız okunur.

| Adım | Betik / program | Çıktı |
|---|---|---|
| Kaynaklar | `src/`: denetçinin robotları birebir (`robot.c`, `robot4*.c`, `motor.c`; SHA-256 `sonuc/sha256_kaynaklar.txt`); yamalar `yama_uc.py` (düğüm vektörü, hikâye dökümü), `yama_mi.py` (3. ölçümün 9 olgusu, `scorev`, `dumpconst`); yeni programlar `say.c` (doğrudan sayım), `bi.c` (birleşik-tahta IS), `sus.c` (başarısız SuS, kayıt için), ortak olgu kodu `olgu_ortak.h` | `bin/` |
| E1/E3 yeniden üretimi, düğüm vektörleri | `kos_tekrar.sh`, `kos_nodes.sh`; `kontrol/tekrar_dogrula.py` | `puan/E1_*`, `puan/E3_*` |
| 1. ölçüm: literatür | alt-ajan tabloları `literatur/`; `kontrol/alinti_dogrula.py`, `kontrol/literatur_say.py` | `sonuc/literatur_sayim.json`, `sonuc/literatur_SA_tekil.tsv` |
| 1. ölçüm: kalıntı ve basamak | `kontrol/terazi_egri.py` | `sonuc/terazi_kalinti.txt`, `sonuc/terazi_egri_tam.txt`, `sonuc/olcum1_basamak.md` |
| 2. ölçüm: hikâyeler | `kos_dump_e1.sh` (robot dökümü), `kontrol/sec_bant.py` (bant seçimi, tohumlar 2301–2303) | `hikaye/E1_*` |
| 2. ölçüm: doğrulamalar | `kontrol/bit_*.c`, `kontrol/bagimsiz_476.py`, `kontrol/kale_kesin.py`, `./bin/bi F …`, `kontrol/zigtest2.c`, `kontrol/j10_hiz.c` | `sonuc/is_dogrulama_F_1e9.txt`, `sonuc/bi_dogrulama_kF12.txt`, `sonuc/kale_kesin.txt` |
| 2. ölçüm: doğrudan sayım | `kos_say_sirali.sh` (M20, tohum 5102), `kos_say_j10.sh` (J10, tohum 5101), `kos_say_u.sh` (U, tohum 5103); kontrol noktalı, aynı komutla sürdürülür | `sonuc/say_E1_{J10,M20}_3e10.txt`, `sonuc/say_E1_U_2e9.txt` |
| 2. ölçüm: Kur'an'ın tekdüze IS'i | `kos_bi_kuran.sh` (tohum 9101), `kos_bi_kuran2.sh` (9202); `kontrol/bi_ozet.py` | `sonuc/bi_kuran_E1_U*.txt` |
| 2. ölçüm: sıra | `kontrol/sira_analiz.py <null> <sayım>`; tekdüze keşif `kontrol/u_sira_kesif.py`; bant kuyruğu `kontrol/bant_uc_deger.py` | `sonuc/olcum2_son.txt`, `sonuc/u_sira_kesif.json` |
| 3. ölçüm | `kos_tablo_mi.sh`, `kos_tablo_mi_m20.sh` (tablolar, orijinal tohumlar), `kontrol/tablo_sagla.py`, `kos_mi_skor2.sh` (puanlama), `kos_mi_dokum2.sh` (12 rakip); `kontrol/mi_analiz.py` | `tab/*_mi.bin` (depoda değil, ~27 MB'lık), `puan/MI_*`, `sonuc/mi_sonuc.json`, `sonuc/MI_dokum_*.txt`, `sonuc/tablo_sagla.txt` |
| Keşif: −aile | `kos_aile.sh` | `puan/E1aile_*`, `puan/E3aile_*` |

---

## English summary

At the user's request I measured three possible *downward* errors in the "honest ladder" of the Quran permutation study (`hrf2`), under frozen pre-registrations (SHA-256 recorded; two dated amendments). The repository was only read.

1. **Size of the literature behind the parity balance.** Five language groups were scanned (Khalifa, Yüksel/19.org, English, Turkish, Arabic sites). After cross-source deduplication there are at least **317** (upper ≈ 770) distinct published claims about sura numbers and verse counts, and 2,385–≈4,700 letter/word claims. With N = 317–770 looks, a 1-in-3,200 event is expected 0.10–0.24 times (uniform) / 0.14–0.35 (sorted); the balance keeps at most **0.5–1.0** of its nominal 3.3–3.5 decades. Re-scoring the same 2×10⁶ books with the balance node discounted for N looks gives a rung of ~1/13,000–90,000 (uniform) and ~1/330–5,800 (sorted): somewhat stronger than "not counted", far from "fully counted" (which corresponds to only ~20–30 looks).
2. **Does the robot under-score the Quran?** No. The exact joint frequency of the Quran's own robot story is close to the robot's product (D ≈ −0.12 to −0.13 decades under the sorted nulls, −0.78 uniform). But the robot **over-scores typical top random books by 1–3 decades (up to 6)**, because their best stories collect algebraically linked facts placed in different nodes (e.g. Σn = Σa within family G forces its Σq = 0, which the robot counts again as "m² | Σq"). Scoring stories by exact joint frequency (direct counting, 1.01×10¹⁰ samples per sorted null; the Quran's story was hit 24 and 27 times) moves the primary rung (lens inherited, balance not counted) from ~1/1,600–1,800 to **~1/24,000–29,000 under the sorted nulls (95% ~1/18,000–37,000)**, a ×15–16 gain. Under the uniform null rivals at the Quran's level cannot be resolved with this compute; a pre-registered exploratory estimate (empirical-Bayes, validated by thinning the sorted-null counts) gives ~1/140,000 (scenarios ~1/54,000–430,000; hard bounds 1/10,000–1/2,000,000) instead of the robot's ~1/17,000.
3. **"Close enough" generosity.** Not a downward error. At the prime modulus 19 "Σq is a square" already means Σq = (19j)²; the modulus-expression criterion (c ≤ 4) is more generous there. Adding it lowers the Quran's score by 0.21 or leaves it unchanged, and rivals gain: the calibrated p worsens ×1.5–1.8 under the uniform null and ×1.03–1.22 under the sorted nulls (×2.1–3.7 with the ×2 post-hoc cost); none of the 36 rung × null × variant cells (4 rungs incl. the 646-lens search, 3 nulls, 3 variants) improved. Among the 12 top-scoring random books, 10 (J10) and 7 (uniform) also have constants in X_19; one uniform book has exactly the Quran's 1444, 722/722 and |U| = 19. 

**Provenance (verified at source):** the parity balance was published in **May 1994** (A. Arik, *Submitters Perspective*; in Arabic attributed to A. Jalghoum's 1994 book), not first in 2009; the 12-member family and **1444 = 19×19×4** in **January 1990** (D. Dixon, *Submitters Perspective*); the inner balance **722/722** and the two **361 = 19²** cells by **2015 at the latest** (H. Kitana / B. Jarrar). Not found in the literature: the 342/380 cells, the 5–9 band/pan bags, and the Two Hands. In the robot's own scale, removing the 1990 family facts as well gives ~1/170–200 (sorted) and ~1/1,270 (uniform) for the claim battery; this does not yet include the exact-scoring correction of item 2, which would move it in the Quran's favour.
