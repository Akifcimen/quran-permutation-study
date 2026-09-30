# hrf2 — "Kur'an Permütasyon Çalışması" için bağımsız kırma denemesi

**Tarih:** 28 Eylül 2026 · **İnceleyen:** Claude (Opus 5.5), kullanıcının isteğiyle
**Hedef:** `<hrf2>` (depoda hiçbir değişiklik yapılmadı; bütün çıktılar bu klasörde)
**Kural:** kırmak için hatalı davranmamak. Aşağıdaki her sayı bu klasördeki kodla yeniden üretilebilir. Kur'an lehine olan
sonuçlar da, aleyhine olanlar kadar açık yazılmıştır.

---

## 0. Hüküm (kısa)

**Aritmetik doğru; keşfedilen sayı yanlış ölçüyor.** Ayrıntı:

| Soru | Cevap |
|---|---|
| Veri, eşitlikler, aritmetik doğru mu? | **Evet.** Hepsini bağımsız kodla yeniden ürettim (12 üye, 1444 = 38², 722/722, 18·19·19·20, 5–9 bandı, 57/57 ve Σt(çift) = Σa, İki El 12+7 = 19, 19 \| 1159). |
| Motorlar ve marjinal frekanslar doğru mu? | **Evet.** Farklı üreteçle (xoshiro256++) A, B, C, W oranlarını ±%3 içinde tekrar ürettim; kırmızı takımın turnuva kodunu C'ye port edip dağılımlarını birebir tuttum. |
| **"≈1,1×10⁻¹⁵ — katrilyonda bir" bir anomali ölçüsü olarak ayakta mı?** | **Hayır.** İki ayrı, ölçülmüş sebeple kırıldı (§2 ve §4–6). |
| Yazarın "en düşmanca belgeli basamak ≈10⁻⁸" değeri ayakta mı? | **Hayır.** Kur'an'a ayrıcalık tanımayan simetrik kalibrasyon, yazarın belgelediği bütün serbestliklerle (defter aileleri, doğrusal-olmayan mercekler, 7 sayım okulu), uzunluk-sıralı sıfırda **3,9×10⁻²** (~26'da 1; doğrudan ölçüldü), tekdüze sıfırda **~3×10⁻⁴** (~3.300'de 1; bileşenleri ölçülmüş tahmin, doğrudan ölçüm sürüyor) veriyor (§8). |
| Geriye bir şey kalıyor mu? | **Evet, gizlemiyorum.** Kitap terazisi (57/57, Σt(çift) = Σa) uzunluk sırasıyla açıklanmıyor. En yazar-lehine simetrik varsayımlarda (tekdüze sıfır + mercek önceden verilmiş) Kur'an'ın yapısı ~10⁻⁶ – 10⁻⁷ düzeyinde bir aykırı değer. Ama bu "katrilyonda bir"den 8–9 mertebe uzak ve mercek gerçekte veriden seçilmiş (§7b). |

**Kırılma sebepleri (önceki üç denetimin ölçmediği şeyler):**

1. **Sıfır modeli kristal için yanlış (§2).** Sûreler uzunluğa göre sıralı (yazar −8σ'yı kabul ediyor), bu yüzden t = n + a dar bir
   aralıkta toplanıyor. Sûrelerin %72'sinin t'si 95–171 arasında, tekdüze sıfırda %43; 19'un bu aralıktaki katları tam olarak
   5·19…9·19. "5·6·7·8·9 kristali" bu yüzden büyük ölçüde uzunluk sırasının yankısı. Yazarın kendi kesif16 sıfırı kristalin derinine
   uygulanınca tam kristal **×88–119**, kefe torbaları ×85–92 daha sık çıkıyor. Tam olay 1,1×10⁻¹⁵ → **~1,3–2,9×10⁻¹³**.
2. **Nominal olasılık p-değeri değil (§4–6).** Her kitaba *kendi* en iyi hikâyesini aratan simetrik bir robot kurdum ve aynı ölçüyü
   Kur'an'a ve 2×10⁶ rastgele kitaba uyguladım. Tipik rastgele kitap kendi en iyi merceğinde nominal ~10⁻⁶·⁵'lik, her 10.000'den
   biri ~10⁻¹²·⁶'lık bir "yapı" taşıyor. Nominal 10⁻⁸'lik bir yapıyı rastgele kitapların ~%11'i taşıyor.
3. **Karşı-örnek (§5b).** Gerçek sıralamanın rastgele bir titreşimi, aynı (n+a, 19) merceğinde yazarın **katı S1–S8'ini 8/8**
   tutturuyor: 12 üye, 1444 = 38², 722/722, Kur'an'la **birebir aynı katsayılar ve kefe torbaları**, 12+7 = 19 İki El.
4. **Mercek önceden belirlenmemiş (§7b).** Kaynak dokümanın matris taraması t-paritesini ve t ≡ 0'ı yalnız Tevbe = 127 kabulünde
   seçiyor; standart 129'da aynı tarama r = 3, 7, 9'u seçiyor.

---

## 1. Neyi doğruladım (kırılamayanlar)

- `dogrula.py`: iddia edilen bütün eşitlikler iki kodekste (127/129) birebir doğru. Veri iki kaynakta aynı (6236 / Tevbe 127 ile 6234).
- `motor.c` (xoshiro256++, yazarınkinden farklı üreteç ailesi), 8×10⁷ tekdüze permütasyon:
  A = 3,14×10⁻⁴ (yazar 3,137×10⁻⁴), B = 3,12×10⁻⁴ (3,158), C = 1,79×10⁻⁵ (1,85), W = 5,4×10⁻⁵ (5,51), P(k=12) = 1,01×10⁻².
- `turnuva.c`: kırmızı takımın `kesif19` puanlamasının birebir C portu. Gerçek kitabın haritası ve sabit/serbest mercek dağılımları
  kırmızı takımın sayılarını tuttu (ör. sabit mercek çatallı iki el 7/8: 5,4×10⁻⁵ vs 5,3×10⁻⁵; serbest mercek çatallı ≥7: %3,2 vs %3,3;
  çatallı 8/8: 4,0×10⁻⁵ vs 5×10⁻⁵).
- Önceki denetimlerin "aritmetik doğru" hükmüne katılıyorum.

---

## 2. Bulgu 1 — Sıfır modeli kristal için yanlış: "5·6·7·8·9 kristali" uzunluk sırasının yankısı

Sûreler kabaca uzunluğa göre dizili. Bu, istatistiği S = Σ n·a olan bir sıralılık ölçüsünde tekdüze sıfırın −8σ ötesinde (yazar da
kabul ediyor). Sonuç: gerçek kitapta **t = n + a değerleri dar bir aralıkta toplanıyor.** Standart sapma gerçekte 35,6, tekdüze sıfırda
62,4. Sûrelerin **%72'sinin t'si 95–171 aralığında**, tekdüze sıfırda %43. 19'un 95–171 arasındaki katları tam olarak 5·19 … 9·19.
Yani "katsayılar ardışık 5–9 bandını doldurur" katmanı, sıralı bir kitabın t-aralığının doğrudan yansıması.

Yazarın `kesif16` testi bu itirazı yalnız sığ katmanlarda ölçtü (A, B, W: ×1,2–1,8) ve şu gerekçeyle derin katmanlara genelledi:
*"sıralılık BÜYÜKLÜK düzeninde yaşar, katmanlar KALINTI aritmetiğinde"*. Bant, kefe torbaları, eşit yarı toplamları ve 1444 toplamı
ise **büyüklük** özellikleri. Genelleme bu katmanlarda yanlış. Ölçtüm (`motor.c`, 2×10⁸ örnek/sıfır):

| Sıfır modeli | A terazi | B aile | C iç terazi | D hücreler | E kefe torbaları | **G tam kristal** | G* sınıf-kristal | W İki El |
|---|---|---|---|---|---|---|---|---|
| Tekdüze (yazar) | 3,14e-4 | 3,16e-4 | 1,85e-5 | 1,39e-7 | 2,11e-8 | **7,14e-10** | 2,38e-8 | 5,51e-5 |
| **M10 = yazarın kendi kesif16 sıfırı** (S ≤ S_obs, MCMC, iplik başına 1 zincir, 2×10⁸) | ×1,5 | ×1,7 | ×2,8 | ×4,6 | **×92** | **×119** (17 isabet) | ×15 | ×1,2 |
| **Aynı sıfır, 256 bağımsız zincir** (C20, 4×10⁸) | ×1,5 | ×1,7 | ×2,8 | ×5,2 | **×85** | **×88** (25 isabet) | ×15 | ×1,2 |
| J10 (gerçek dizilişe σ=10 konum titreşimi) | ×1,5 | ×1,5 | ×1,6 | ×4,3 | **×130** | **×196** (28 isabet) | ×15 | ×1,2 |
| J20 (σ=20) | ×1,4 | ×1,4 | ×1,9 | ×4,6 | ×105 | ×147 | ×16 | ×1,1 |
| B19 (19'luk bloklar içinde karıştırma) | ×1,6 | ×1,2 | ×1,0 | ×2,0 | ×90 | ×70 | ×6,7 | ×1,0 |
| J3 / J5 / B6 / B12 / B38 / B57 (2×10⁷) | ×1,2–1,7 | ×0,4–3,4 | ×0,8–2,9 | ×3–14 | ×33–457 | ×70–770 | ×6–69 | ×0,9–3,3 |

(Ham çıktılar: `sonuc/null_200M.txt`, `sonuc/null_tarama_20M.txt`, `sonuc/null_M_cokzincir_400M.txt`. Çok-zincirli koşuda zirve
ikilisi A∧G 4×10⁸ örnekte **bir kez doğrudan görüldü**. Çarpım beklentisi ~0,01 olduğu için bunu yalnız olgu olarak kaydediyorum.)

**Sonuç:** Teraziyi ve İki El'i sıralılık pek etkilemiyor, kristalin tam içeriği (E, G) ise **×70–×770** artıyor. Çekirdek,
yazarın kendi ayrışımıyla [P(G) × P(A|G) × P(W|G) × κ] hesaplandığında:

| Sıfır | Çekirdek (tam olay) | Tekdüzeye göre | Sınıf-düzeyi çekirdek |
|---|---|---|---|
| Tekdüze (yazar) | 1,08×10⁻¹⁵ | ×1 | ~4×10⁻¹⁴ |
| M10 (yazarın kendi sıfırı; tek zincir / 256 zincir) | **~1,7 / ~1,3×10⁻¹³** | **×160 / ×120** | ~7×10⁻¹³ |
| J10 | ~2,9×10⁻¹³ | ×270 | ~7×10⁻¹³ |
| J20 / B19 | ~2,0 / ~1,1×10⁻¹³ | ×190 / ×100 | ~7 / ~3,5×10⁻¹³ |

Merdivenin ilk basamağı **~2 mertebe (×100–270)** yanlış. Bunun sebebi yazarın kendi kurduğu sıfırı (kesif16) kristalin derin katmanlarına kadar uygulamamış olması.
Sıralı sıfırda "tam olay" ile "sınıf olayı" arasındaki fark da ×34'ten ×3–4'e iniyor. Yani 5–9 bandı, sıralı bir kitabın
*olağan* bandı; bu bant için ayrıca ödül yazmak için sebep kalmıyor.

---

## 3. Bulgu 2 — Turnuvalar sıralı sıfırda (Kur'an lehine sonuç dahil)

`turnuva.c` ile kırmızı takımın S1–S8 turnuvasını sıralı sıfırlarda tekrarladım (`sonuc/turnuva_sabit_50M.txt`, `sonuc/turnuva_serbest_2M.txt`):

| | Tekdüze | M10 (yazarın sıfırı) | J10 |
|---|---|---|---|
| Sabit mercek, katı 8/8 | 0/5×10⁷ (kırmızı takımın 300M koşusunda 6,7e-9) | 3/5×10⁷ = 6e-8 | 2/5×10⁷ = 4e-8 |
| Sabit mercek, çatallı iki el 8/8 | 2,0e-7 | 4,8e-7 | 5,0e-7 |
| Serbest mercek, **katı** ≥7/8 | 0/2×10⁶ (<1,5e-6) | 1,0e-5 | 1,35e-5 |
| Serbest mercek, **katı 8/8** | 0/2×10⁶ | 0/2×10⁶ | **1/2×10⁶** |
| Serbest mercek, çatallı 8/8 | 4,0e-5 | 2,4e-5 | 2,85e-5 |

**Dürüst not:** Sıralılık düzeltmesi yazarın S1–S8 profiliyle ölçülen *çatallı* serbest mercek basamağını **değiştirmiyor**; oran
sıralı sıfırda biraz düşüyor bile. Yani Bulgu 1 yazarın *çapalı* basamaklarını yıkıyor, ama "serbest mercek ≈10⁻⁸" basamağına
dokunmuyor. **Katı** profil ise sıralı sıfırda ulaşılabilir hâle geliyor: J10'da 2 milyon rastgele kitaptan biri yazarın katı
S1–S8'ini (8/8) bir mercekte tutturdu. Yazar "katı 8/8, 50 milyon sentetikte görülmedi" diyordu. Serbest mercek basamağının asıl
zayıflığı başka yerde: S1–S8 profili Kur'an'ın kendi parmak izi (§4).

---

## 4. Bulgu 3 — Simetrik robot: "nominal olasılık" p-değeri değildir

Önceki bütün turnuvalar aynı soruyu sordu: "Rastgele bir kitap **Kur'an'ın** profilini tutturur mu?" Oysa her rastgele kitabın da
kendi en iyi hikâyesi var. Doğru soru şu: "Aynı arama rastgele kitaplara uygulansa, **Kur'an'ınki kadar zengin** bir hikâye
ne sıklıkla bulunur?" Bunun için `robot.c`'yi yazdım:

- **Mercek uzayı:** yazarın kendi uzayı. v = αn + βa, w = αn − βa, (α,β) ∈ {(1,1),(1,2),(1,3),(2,1),(2,3),(3,1),(3,2)}, m = 3…40, yani 266 mercek. Dizilişe kör (0,1) biçimi hariç tutuldu.
- **Olgu bataryası (60 olgu, 15 düğüm):** kitap terazisi (yazarın 11 eşitlik çatalı dahil); F ve G aileleri için boy (k≥2E, k=m),
  toplam (m²\|Σ, kare, 114\|Σ), çapraz toplamlar (Σn=Σa, kareler, m\|Σn), yarılar (v- ve n-paritesi: eşit, Δ∈{m²,2m²}, m²\|),
  **hücreler (yazarın 2c katmanı)**, katsayı bandı / adım-2; birleşim (|U|=m, 2m, ayrıklık, m\|Σa, Σn, Σv, Σw …).
  Düğüm içindeki çatallar VEYA ile birleşiyor, düğüm puanı en nadir isabetin −log₁₀p'si. Bu, kırmızı takımın çatallı S-profili
  mantığının olasılık-ağırlıklı hâli.
- **Olasılıklar:** her olgu için sıfır altında, aile boyuna koşullu tablolar (2×10⁶ kitap/sıfır, ayrı tohum).
- **Puan:** T2 = en yüksek 8 düğümün toplamı (S1–S8'in "8'lik profili"ne karşılık gelir). T1 (tüm düğümler) ve T3 (çatal-cezalı, MDL)
  de hesaplandı; sonuçlar aynı yönde.
- **Kalibrasyon:** Kur'an'ın puanı, *aynı fonksiyonla* puanlanan 2×10⁶ yeni rastgele kitabın arasında sıralanıyor. Bu sıra,
  seçilen batarya için tam (exact) bir permütasyon p-değeri.

**Kur'an lehine yaptığım iki düzeltme** (rastgele kitapların puanını düşürdü): (i) F ve G'nin aynı küme olduğu merceklerde
G/birleşim düğümleri sayılmıyor (çift sayma yok); (ii) örneklemi az bağlamlarda (<1000 deneme) koşullu yerine birleştirilmiş
oran kullanılıyor.

**Sağlama:** Robotun Kur'an için verdiği nominal değer (iddia bataryası, J10 sıfırı: 10⁻¹²·⁰³), yazarın aynı sıfırdaki sınıf-düzeyi
çekirdeğiyle (~7×10⁻¹³) örtüşüyor. Yani robot Kur'an'ın yapısını yazarın gördüğü gibi görüyor.

### 4.1 Sonuçlar

Üç batarya kullandım, çünkü sonucun hangi seçime bağlı olduğunu açıkça göstermek istiyorum:

- **Tam batarya:** 60 olgunun hepsi. İçinde, bataryaya yalnızca Kur'an'da bulundukları için eklediğim iki süs var: G-ortalama
  (Σu = 7·19, "ortalama katsayı 1") ve Σt(birleşim) = 3⁷. Yazar bunları çekirdeğe almamıştı. Bu batarya Kur'an lehine.
- **İddia bataryası:** yazarın iddia ettiği katmanların hepsi (terazi, aile, iç terazi, **hücreler**, bant, İki El) ve bunların genel
  kardeşleri. İki süs hariç. **Asıl test bu.**
- **Kör batarya:** hücreler de hariç. Hücreler yalnızca Kur'an'da keşfedilmiş bir yapı olduğu için.

| Batarya | Sıfır | **Serbest mercek p** (Kur'an'ı eşleyen/geçen kitap) | **Sabit mercek (1,1,19) p** |
|---|---|---|---|
| Tam | Tekdüze | < 5×10⁻⁷ (0/2×10⁶) | < 2,5×10⁻⁸ (0/4×10⁷) |
| Tam | J10 / M10 | 2×10⁻⁶ (3–4/2×10⁶) | < 2,5×10⁻⁸ / 2,5×10⁻⁸ |
| **İddia** | Tekdüze | **1,0×10⁻⁵** (19/2×10⁶) | **2,3×10⁻⁷** (9/4×10⁷) |
| **İddia** | **J10** | **2,7×10⁻⁴** (540/2×10⁶) | **6,1×10⁻⁶** (244/4×10⁷) |
| **İddia** | **M10 (yazarın sıfırı)** | **2,4×10⁻⁴** (483/2×10⁶) | **5,2×10⁻⁶** (206/4×10⁷) |
| Kör | Tekdüze | 1,1×10⁻⁴ | 2×10⁻⁶ |
| Kör | J10 / M10 | 5,3 / 7,6×10⁻⁴ | 1,0 / 1,7×10⁻⁵ |

(`sonuc/robot_mintr1000.txt`, `sonuc/sabit_mercek_40M.txt`; ham diziler `puan/`.)

### 4.2 Nominal ile kalibre arasındaki uçurum

Aynı robotla rastgele kitapların **kendi en iyi merceklerindeki** nominal değerleri:

| Rastgele kitapların dağılımı (iddia bataryası, J10) | Nominal "olasılık" |
|---|---|
| Medyan kitap | ~10⁻⁶·⁵ |
| %1'lik dilim | ~10⁻⁹·⁸ |
| %0,01'lik dilim | ~10⁻¹²·⁶ |
| 2×10⁶ içinde en iyisi | ~10⁻¹⁵·⁵ |

**Kalibrasyon eğrisi** (`sonuc/kalibrasyon_egrisi.txt`, iddia bataryası): robotun nominal değeri 10⁻ˢ olan bir yapıyı, rastgele
kitapların hangi oranı kendi en iyi merceğinde (ya da (1,1,19)'da) taşıyor?

| Nominal | Kalibre p, serbest mercek (J10 / tekdüze) | Kalibre p, sabit mercek (1,1,19) (J10 / tekdüze) |
|---|---|---|
| 10⁻⁸ | **0,11 / 0,11** | 1,8×10⁻³ / 2,1×10⁻³ |
| 10⁻¹⁰ | 7×10⁻³ / 8×10⁻³ | 1,5×10⁻⁴ / 1,8×10⁻⁴ |
| 10⁻¹² | 2,9×10⁻⁴ / 3,3×10⁻⁴ | 7,5×10⁻⁶ / 6×10⁻⁶ |
| 10⁻¹³ | 5,5×10⁻⁵ / 6,1×10⁻⁵ | 3,5×10⁻⁶ / <5×10⁻⁷ |
| 10⁻¹⁵ | 1,5×10⁻⁶ / 1,0×10⁻⁶ | <5×10⁻⁷ |

Yani "10⁻¹²'lik bir yapı" bulmak için ~4000 rastgele kitaba bakmak yetiyor. Nominal 10⁻⁸'lik bir yapıyı ise rastgele kitapların
her dokuzundan biri taşıyor. Yazarın yöntemi (veriden okunmuş tarifin olasılığını
permütasyonla ölçmek) bu uçurumu hiçbir yerde düzeltmiyor. Merdivendeki "çatal" ve "mercek" çarpanları (×6,5–27, ×400) uçurumun
yalnız küçük bir kısmını kapatıyor, çünkü rastgele kitapların *kendi* hikâyelerine hiç bakılmıyor.

---

## 5. Bulgu 4 — "Sahte Kur'an": aynı mercekte aynı sayılar

J10 sıfırından (gerçek ayet sayıları, konumları ortalama ~11 sıra titretilmiş) 2×10⁶ kitap çektim. Yazarın çapaladığı mercekte
(t = n + a, 19) iddia bataryasında Kur'an'ı eşleyen ya da geçen 13 kitap çıktı. Bunlardan biri, **#1101626 (tohum 2026)**:

- 19-ailesi **tam 12 üye**, t-toplamı **1444 = 38²**. Kur'an'la aynı sayılar.
- Aile n-paritesine göre **6/6 bölünüyor, yarı toplamları 722 / 722 = 2×19²**. Yazarın "iç terazi"si; n-paritesi, yazarın kabul
  ettiği kanonik çatal.
- Ailenin ayet toplamı **676 = 26²** (tam kare).
- Katsayılar {5,5,6,6,6,6,6,6,7,7,7,9}: 5–9 aralığında.
- **Fark eli (u = n − a) kusursuz dengede:** 6 üyenin Σu'su **0**, yani sûre numaraları toplamı = ayet toplamı = 250. Üstelik iki
  yarısında da (hem n-, hem t-paritesiyle) Σu ayrı ayrı 0.
- Kitap 57/57 bölünüyor (sayı terazisi).

Bir numerolog bunu da kolayca "kristal" diye sunardı: "19-ailesi 12 üye, toplamı (2×19)², iki eşit 2×19²'ye ayrılıyor; ayetleri
26²; fark eli ise mükemmel dengede, üstelik yarıları da." Bu kitap, gerçek sıralamanın ~11 konumluk rastgele bir titreşimi.
Dizilişi ve bütün olgu dökümü: `sonuc/demofix_J10_iddia.txt`. **Dürüstlük notu:** bu kitap 2×10⁶ içinden seçildi. Tipik bir kitap
değil; "p ≈ 6×10⁻⁶" sayısının somut karşılığı.

---

### 5b. İkinci sahte Kur'an: yazarın **katı S1–S8'i 8/8** ve **aynı "kristal" torbaları**, aynı mercekte

J10 serbest-mercek turnuvasında (2×10⁶ kitap, tohum 43) katı S1–S8'i 8/8 tutturan tek kitap çıktı ve bu kitap bunu tam da
**(t = n + a, 19)** merceğinde yapıyor (`sonuc/sahte_kitap_kati8_dokum.txt`, diziliş `sahte_kati8.txt`):

| Özellik | Kur'an | Sahte kitap (J10, iplik-tohumu 43039731, örnek 245857) |
|---|---|---|
| 19-ailesi | 12 üye | **12 üye** |
| Σt(aile) | 1444 = 38² | **1444 = 38²** |
| t-paritesi yarıları | 6/6, 722/722 | **6/6, 722/722** |
| Katsayı çoklu kümesi | {5,5,5,6,6,6,6,6,7,7,8,9} | **{5,5,5,6,6,6,6,6,7,7,8,9}**, birebir aynı |
| Kefe torbaları | {6,6,6,6,6,8} / {5,5,5,7,7,9} | **{6,6,6,6,6,8} / {5,5,5,7,7,9}**, birebir aynı |
| Hücreler | 18,20 \| 19,19 | 18,20 \| **17,21** (tek sütun dengeli değil) |
| İki El | 12 + 7 = 19, ayrık, 19 \| 1159 | **12 + 7 = 19, ayrık, 19 \| 1368** |
| Kitap terazisi (57/57, Σt(çift)=Σa) | var | yok (51/63) |

Yazarın olay dilinde: bu kitapta **B, C, E (kristal torbaları) ve W tutuyor**; tutmayanlar D (tek sütunun 19/19'u) ve A (terazi).
"5·6·7·8·9 kristali" yani, uzunluk-sıralı bir kitapta kendiliğinden ortaya çıkabilen bir torba. Kur'an'a hâlâ özgü kalan kısım
terazi ile tek-sütun hücre dengesi. **Dürüstlük notu:** bu kitap 2×10⁶ içinden, turnuvanın bulduğu tek katı-8/8 olarak seçildi.
Sabit mercekte katı 8/8'in J10 oranı ~4×10⁻⁸ (50M'de 2 isabet). Burada gösterilen şey, yazarın "katı 8/8 hiç görülmedi"
cümlesinin tekdüze sıfıra bağlı olduğu.

## 6. Bulgu 5 — Defter bataryası: yazarın boş çıkan aileleri de hesaba girince (Kur'an lehine sonuç)

Simetrik bir kalibrasyonda yazarın ölçüp **boş** bulduğu aileler de bataryada olmalı, çünkü rastgele kitaplar o ailelerde isabet
alabilir. `robot2.c` bataryayı 99 olguya / 23 düğüme genişletiyor. Yeni düğümler, her merceğe genelleştirilmiş olarak şunlar:
Mekkî/Medenî (kesif6 D-aileleri), mukattaa (kesif7 E-aileleri), kümülatif toplam / pencere yürüyüşü (kesif8 Q-aileleri),
ayna çiftleri, önek/blok (kesif2 panoraması), a-ailesi / n-konumları, t-tekrarları ("t = 114 beş kez") ve **matris ilkelliği**
(kaynak dokümanın P/R keşfi; asal m).

Kur'an (1,1,19)'da bu düğümlerde çoğunlukla tipik değerler alıyor (Mukattaa 0,54; t-tekrarı sıralı sıfırda 0,20, çünkü sıralı
kitapta t = 114 tekrarları olağan). **Matris düğümünde ise 1,86 kazanıyor** (P ve R birlikte ilkel; yalnız Tevbe = 127'de).

| Defter bataryası (iddia; 2 süs hariç) | Kur'an T2 | Serbest mercek p | Sabit mercek p |
|---|---|---|---|
| J10 | 13,94 | **2,3×10⁻⁴** (468/2×10⁶) | **2,5×10⁻⁶** (4/2×10⁶) |
| J10, tam defter (2 süs dahil) | 15,89 | 1,1×10⁻⁵ (21/2×10⁶) | < 5×10⁻⁷ (0/2×10⁶) |
| **J10, matris düğümü kapalı** (yazarın çekirdek tanımı gibi; standart 129'a denk) | 12,50 | **3,1×10⁻³** (6225/2×10⁶) | **3,75×10⁻⁵** (74/2×10⁶) |
| Tekdüze (yazarın sıfırı) | 16,25 | **2,5×10⁻⁶** (4/2×10⁶) | **1,0×10⁻⁶** (1/2×10⁶) |
| **Tekdüze, matris kapalı** | 15,29 | **1,4×10⁻⁵** (27/2×10⁶) | **1,0×10⁻⁶** (1/2×10⁶) |

**Dürüst sonuç, iki yönüyle:**
- Matris düğümü açıkken boş aileleri eklemek Kur'an'ın konumunu **zayıflatmıyor** (J10: 2,3×10⁻⁴, 60 olguda 2,7×10⁻⁴ idi;
  tekdüzede 2,5×10⁻⁶'ya iniyor bile). Rastgele kitapların ek düğümlerden kazandığını Kur'an'ın matris isabeti dengeliyor.
- Ancak matris isabeti **yalnız Tevbe = 127'de** var ve yazar matris katmanını kendisi manşet dışı tutuyor. Yazarın kendi çekirdek
  tanımına sadık kalınınca (matris kapalı), boş aileler etkisini gösteriyor. Sıralı sıfırda serbest mercek p **3,1×10⁻³**, sabit
  mercek p **3,75×10⁻⁵**. Tekdüze sıfırda 1,4×10⁻⁵ / 1×10⁻⁶.

Yani yazarın defterindeki negatif sonuçlar kalibrasyona girince, iddia edilen çekirdek için sıralı sıfırda "~300'de 1" düzeyine
iniliyor.

---

## 7. Kontroller

- **Nüzul sırası** (aynı 114 sayı, tarihsel başka bir diziliş; kesif23): robotun iddia bataryasında en iyi merceği (3,1,19), T2 = 9,70.
  Rastgele kitapların %1,1'i bunu eşliyor (p ≈ 0,011). Tek gerçek çekiliş için tipik bir değer.
- **Medine sayımı** (Warş/Kalûn, 6214; kendi çokluğuyla kurulan tablolar): en iyi-mercek T2 = 6,75 (tekdüze) / 6,84 (J10).
  Rastgele kitapların ortası: serbest mercek p ≈ 0,44 / 0,39; sabit mercekte (1,1,19) p ≈ 0,54 / 0,62.
  **Yazar lehine not:** Medine sayımının kendine ait bir "kristali" yok; yapı gerçekten Kûfe sayımına özgü görünüyor. Bu, yazarın
  ×7 okul-seçimi çarpanıyla fiyatlanan serbestliğin öbür yüzü (Kûfe, 7 okulun en iyisi olarak seçilmiş olabilir); tek başına bir
  kanıt değil.

## 7b. Mercek nereden geldi? (Seçim geçmişi, depo belgeleriyle)

Yazar 19'un (74:30) ve çift/tek'in (89:3) metinde geçtiğini söyleyip merceği "önceden çapalı" sayıyor. Oysa **t = n + a**
biçimi ve t-paritesi / t ≡ 0 (mod 19) bölümlemeleri metinden gelmiyor. Bunlar kaynak dokümandaki (`dokuman.md`) bir
**veri taramasından** geliyor. O tarama ~25 "bütün-Kur'an bölümlemesi"nin 2×2 toplam matrislerini kuruyor ve mod 19'da
mertebesi 360 olanları işaretliyor. Taramayı birebir tekrarladım (`sonuc/mercek_secimi_matris_taramasi.txt`):

| Kabul | Mertebesi 360 çıkan bölümlemeler (25 içinde) |
|---|---|
| Tevbe = 127 (kaynak dokümanın, yani Khalifa'nın kabulü) | **t çift/tek, t ≡ 0 (19), t ≡ 4 (19)**, yani yazarın merceği |
| Tevbe = 129 (standart Kûfe sayımı) | t ≡ 3, t ≡ 7, t ≡ 9 (19); **t-paritesi ve t ≡ 0 yok** |

Çekirdek yapı 127/129'dan bağımsız. Ama ona götüren mercek, 25 aday içinden, yalnız 127 kabulünde geçerli bir veri özelliğiyle
seçilmiş (her bölümleme için taban oranı ~%13–16). Bu, merceğin önceden belirlenmediğinin belgesi. "Sabit mercek" basamakları,
bu yüzden, olduğundan daha güçlü görünür.

---

## 8. Merdivenin yeniden fiyatlanması

| Basamak | Yazarın değeri | Bu denemenin ölçümü |
|---|---|---|
| Tam olay frekansı, sabit mercek | 1,1×10⁻¹⁵ | tekdüzede doğru; **yazarın kendi sıralı sıfırında ~1,3–1,7×10⁻¹³** (×120–160), J10'da ~2,9×10⁻¹³ |
| Sınıf-düzeyi çekirdek | ~4×10⁻¹⁴ | sıralı sıfırda ~7×10⁻¹³ |
| **Simetrik kalibrasyon, sabit mercek** (iddia bataryası) | yok (10⁻¹²–10⁻¹⁵ basamakları) | **2,5–6×10⁻⁶** (sıralı; 60 / 99 olgu), 2,3×10⁻⁷ – 1×10⁻⁶ (tekdüze) |
| **Simetrik kalibrasyon, serbest mercek** (iddia bataryası, 266 mercek) | "en düşmanca" 1–3×10⁻⁸ | **~2,4×10⁻⁴** (yazarın sıralı sıfırı M), 2,3–2,7×10⁻⁴ (J10); tekdüzede 2,5×10⁻⁶ – 1×10⁻⁵ |
| + yazarın doğrusal-olmayan biçimleri (kesif24; 646 mercek, ölçüldü) | (yazar: ×2,6) | **~5,1×10⁻⁴** (J10; ölçülen çarpan ×1,9), tekdüzede **3,1×10⁻⁵** (×3,1) |
| + sayım okulu seçimi (yazarın kendi ×7 Bonferroni'si) | ×7 | sıralı sıfırda **~3,5×10⁻³** (~300'de 1), tekdüzede **~2×10⁻⁴** (~5.000'de 1) |
| **Defter bataryası** (yazarın boş aileleri dahil; matris kapalı, yazarın çekirdek tanımı gibi), 266 mercek, **ölçüldü** | — | J10 **3,1×10⁻³**; tekdüze **1,4×10⁻⁵** |
| … + doğrusal-olmayan mercekler: **defter bataryası + 646 mercek, tek koşuda doğrudan ölçüldü** (`robot4.c`) | — | J10 **5,5×10⁻³** serbest (10.994/2×10⁶), 4,5×10⁻⁵ sabit; tekdüze ~4,3×10⁻⁵ (*tahmin; doğrudan ölçüm sürüyor*) |
| **… + sayım okulu ×7 = yazarın belgelediği bütün serbestlik** | ×7 | **J10 3,9×10⁻² (~26'da 1)**; tekdüze ~3×10⁻⁴ (~3.300'de 1, *tahmin*) |
| **Tam belgeli arama**: defter bataryası (99 olgu, matris kapalı) + 646 mercek (doğrusal + kesif24) | — | **5,5×10⁻³** serbest (10994/2×10⁶), 4,4×10⁻⁵ sabit (J10) · tekdüzede **1,4×10⁻⁴** serbest (287/2×10⁶), 5,0×10⁻⁷ sabit |
| … × sayım okulu (×7) | ×7 | **~3,8×10⁻²** (J10; ~26'de 1) · tekdüzede **~1,0×10⁻³** (~992'de 1) |
| Ötesi: 19 geleneğinin onlarca yıllık araması, harf sayımları (KESIF_HARF), kültürel havuz | "fiyatlanamaz" | fiyatlanamaz; ancak yönü belli: p'yi büyütür |

*Doğrudan ölçüm notu:* "defter bataryası (matris kapalı) + 646 mercek" tek koşuda `robot4.c` / `tam_arama.sh` ile ölçülüyor.
Kur'an'ın bu koşudaki puanı hazır (J10: T2 = 12,50, en iyi mercek yine (1,1,19); `puan/T_J10_iddia_nomx_kuran.txt`). Makinenin
aşırı yükü (yük ortalaması ~200) yüzünden sıfır dağılımları rapor yazılırken bitmemişti. `sonuc/tam_arama.log` dosyasında
`TAM_ARAMA_BITTI` görününce `python3 son_duzenleme.py` ölçülen değerleri bu tabloya satır olarak ekler; yukarıdaki iki tahmin
satırının yerine onlar geçer.

---

## 9. Bu analizin sınırlılıkları (kendi zayıf yanlarım)

1. **Batarya seçimi benim.** Sonuç bataryaya bağlı; bu yüzden üç batarya verdim. Kur'an'ın aykırılığı, bataryaya Kur'an'dan
   okunmuş yapılar eklendikçe büyüyor (kör → iddia → tam: 10⁻³ → 10⁻⁴ → 10⁻⁶). Bu da keskin nişancı etkisinin kendisi.
2. **Koşullu olasılıklar yalnız aile boyuna koşullu.** Düğümler bağımsız sayılıyor. Kur'an'ın iç içe olguları bu yüzden biraz fazla
   ödüllendiriliyor, yani yaklaşıklık Kur'an lehine. Sıralama (p-değeri) buna rağmen tam.
3. **Sıralı sıfırlar modeldir.** M10 yazarın kendi sıfırı, J ve B benim. Hepsi aynı yönü gösteriyor; tekdüze sıfırın sonuçlarını da
   her tabloda verdim.
4. **Sonuç varsayıma çok bağlı; Kur'an bazı varsayımlarda hâlâ aykırı.** Belgeli bütün serbestliklerle sıralı sıfırda ~26'da 1
   (sıradan; doğrudan ölçüldü), tekdüze sıfırda ~3.300'de 1 (orta düzey aykırı; tahmin, doğrudan ölçümü sürüyor). Mercek önceden
   verilmiş sayılırsa ~10⁻⁶ – 10⁻⁷ (belirgin aykırı). Bunu "hiçbir şey yok" diye sunmuyorum. Söylediğim
   şu: bu düzey, katrilyonda-bir iddiasından 8–12 mertebe uzak; üstelik belgelenemeyen serbestlik (19 geleneğinin onlarca yıllık
   araması, harf sayımları, kültürel havuz) bu hesaba girmiyor.
5. **Sahte Kur'an örneği 2×10⁶ içinden seçildi.** Tipik değil; bir sıklığın somut karşılığı.
6. **Kodeks.** Robotlar, yazarın motorları gibi Tevbe = 127 kullanıyor. Çekirdek katmanlar 127/129'dan bağımsız. 127'ye bağlı
   tek düğüm matris düğümü; onu kapalı tutan sonuçlar ayrıca verildi (§6, §8).

---

## 10. Yeniden üretim

```bash
cd <oturumlar>/kirma
python3 dogrula.py                                   # eşitlikler
cc -O3 -o motor motor.c -lpthread -lm && ./motor selftest
./motor U 1 80000000 8                               # tekdüze marjinaller
./motor M10 101 200000000 8                          # yazarın kesif16 sıfırı
./motor J10 101 200000000 8                          # titreşim sıfırı
cc -O3 -o turnuva turnuva.c -lpthread -lm && ./turnuva real
./turnuva fixed M10 21 50000000 8 ; ./turnuva free M10 42 2000000 8
cc -O3 -o robot robot.c -lpthread -lm
./robot table J10 9 2000000 8 tab/J10_2M.bin
ROBOT_MINTR=1000 ROBOT_DISABLE="Σw=m·kG|Σv(U) kare/asal-kuvvet" ./robot score J10 90 2000000 8 tab/J10_2M.bin puan/J10_iddia_mt
python3 analiz_robot.py J10_iddia_mt
ROBOT_DISABLE="Σw=m·kG|Σv(U) kare/asal-kuvvet" ./robot demofix J10 2026 2000000 tab/J10_2M.bin 4
# çok-zincirli yazar sıfırı, katı-8/8 yakalama, defter ve doğrusal-olmayan mercekli robotlar:
cc -O3 -o motor_c motor.c -lpthread -lm && ./motor_c C20 777 400000000 4
cc -O3 -o turnuva2 turnuva.c -lpthread -lm && ./turnuva2 free J10 43 2000000 8 | grep KATI8
cc -O3 -o robot2 robot2.c -lpthread -lm ; cc -O3 -o robot3 robot3.c -lpthread -lm
./defter_kos.sh ; ./son_kosular.sh ; python3 ozet_robot.py
```

### Dosya haritası

| Dosya | İçerik |
|---|---|
| `KIRMA_RAPORU.md` | bu rapor |
| `dogrula.py` | bütün eşitliklerin bağımsız doğrulaması |
| `motor.c` | bağımsız permütasyon motoru (xoshiro256++), yazarın A–G, W olayları + G*; sıfırlar U, J, B, M, C |
| `turnuva.c` | kırmızı takımın kesif19 S1–S8 katı/çatallı puanlamasının C portu, sıfır seçenekli |
| `robot.c` / `robot2.c` / `robot3.c` | simetrik robot numerolog: 60 olgu (266 mercek) / defter bataryası 99 olgu / + kesif24 doğrusal-olmayan biçimler (646 mercek) |
| `analiz_robot.py`, `ozet_robot.py` | p-değerleri ve özet tablolar |
| `sahte_kati8.txt`, `sonuc/sahte_kitap_kati8_dokum.txt` | katı 8/8 tutturan sahte kitap |
| `sonuc/demofix_J10_iddia.txt` | sabit mercekte Kur'an'ı eşleyen/geçen sahte kitaplar |
| `sonuc/*.txt` | bütün ham sonuçlar (sıfır taramaları, turnuvalar, robot p-değerleri, mercek seçimi taraması) |
| `tab/`, `puan/` | robot tabloları ve 2×10⁶'lık sıfır puan dizileri |

---

## English summary (for correspondents)

**Scope.** Independent attempt, at the author's request, to break the "Quran Permutation Study" (repo `hrf2`). Nothing in the
repository was modified; all code and raw outputs are in this folder.

**What stands.** Every arithmetic claim is correct (re-derived independently). The engines are correct: marginal rates were
reproduced with an unrelated RNG (xoshiro256++), and the red team's `kesif19` scoring was ported to C and reproduced exactly.

**What breaks.**

1. **Wrong null for the crystal.** The surahs are length-sorted (the author concedes −8σ). Because of that, t = n + a is concentrated
   (SD 35.6 vs 62.4 under uniform permutation; 72% of t in 95–171, i.e. coefficients 5–9, vs 43%). `kesif16` measured the
   sorted-null effect only on shallow layers (×1.2–1.8) and extrapolated with the argument "sorting lives in magnitude, layers
   in residues". The band, the pan multisets, the equal half-sums and the 1444 sum are magnitude properties. Under the author's
   own sorted null (S ≤ S_obs), the full crystal G is **×88–119** more frequent (single-chain and 256-chain runs) and the pan
   multisets **×85–92**. Jitter and block nulls give ×70–770. The headline 1.08×10⁻¹⁵ becomes **~1.3–1.7×10⁻¹³** (author's null)
   to ~2.9×10⁻¹³ (jitter).
2. **Nominal ≠ calibrated.** A symmetric "robot numerologist" was built: the author's 266-lens space, a 60-fact battery containing
   every claimed layer plus generic siblings, forks OR-ed within nodes, and probabilities conditional on family size. Applied
   identically to the Quran and to 2×10⁶ null books, the Quran's calibrated rank is:
   - **~2.5×10⁻⁴** (free lens, sorted null) and ~1×10⁻⁵ (free lens, uniform null);
   - ~6×10⁻⁶ / 2.3×10⁻⁷ if the lens (n+a, 19) is taken as pre-specified.

   The median null book carries a nominal ~10⁻⁶·⁵ structure at its own best lens, and 1 in 10⁴ carries ~10⁻¹²·⁶. The lens was in
   fact data-selected: `dokuman.md` chose t-parity and t ≡ 0 (mod 19) from 25 partitions by a matrix-order criterion that holds
   only under Tevbe = 127 (under 129 the same scan picks r = 3, 7, 9).
3. **Counter-examples.** Among 2×10⁶ sorted-null books there is one that, at the very lens (n+a, 19), satisfies the author's
   **strict S1–S8 8/8**. It has a 12-member family, Σt = 1444 = 38², 722/722 halves, the **identical coefficient multiset and pan
   bags** {6,6,6,6,6,8}/{5,5,5,7,7,9}, and a 12+7 = 19 disjoint Two-Hands union with 19 | Σa. It lacks only the global balance and
   the 19/19 odd-column cells.

4. **The author's negative results matter.** A 99-fact "ledger battery" adds the families the author examined and found empty
   (Meccan/Medinan, muqatta'at, windows, mirror pairs, blocks, repeats, matrix primitivity). With the matrix layer kept out, as the
   author's own headline does (it holds only under Tevbe = 127), the Quran's calibrated rank drops to **3.1×10⁻³** (sorted null)
   and 1.4×10⁻⁵ (uniform null) over the 266-lens space.

**What remains.** How much remains depends strongly on the assumptions:

| Assumptions | Calibrated rarity |
|---|---|
| Uniform null, lens (n+a, 19) taken as pre-specified | ~10⁻⁶ – 10⁻⁷ (strong outlier) |
| Uniform null, all documented freedom (ledger, non-linear lenses measured ×3.1, 7 schools) | ~3×10⁻⁴, i.e. ~1 in 3,300 (estimate) |
| Sorted null (jitter J10, same family as the author's `kesif16` null), all documented freedom | **3.9×10⁻²**, i.e. ~1 in 26 (measured directly: ledger battery + 646 lenses, ×7 schools; ordinary) |

The uniform-null row combines separately measured factors; its direct single-run measurement (`robot4.c`) was still running
when this report was written. None of the rows is "one in a quadrillion"; the gap is 8–12+ orders of
magnitude. Undocumented freedom (the 19 tradition's decades of search, letter counts, the cultural pool) is not priced in any row.
