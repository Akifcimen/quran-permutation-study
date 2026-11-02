# Profil çalışması, ek ön-kayıt — ölçüm 2: düz histogram kestirimi (1 Kasım 2026)

## 1. Neden

Profil çalışmasında (`ONKAYIT.md`, `RAPOR.md`) O2 ve O3'te Kur'an da en yakın plasebolar da 10¹⁰ kontrolde 0 isabet aldı. Yazılan R yalnız bir üst sınırdı.

Araştırmacı şöyle dedi: "öyle ölçelim ki sınır değil farkı görelim." Kur'an'ın bütün profilini doğrudan sayımla çözmek 10¹²–10¹⁴ kontrol ister. Bu ek, aynı olayların sıklığını bir nadir-olay kestiricisiyle ölçer. Kestirici, okumadan önce, sayımı bilinen olaylarda doğrudan sayımla sınanır.

Olaylar, hedefler, kontrollerin sıfırları ve okuma kuralının yapısı değişmez. Değişen tek şey, 10¹⁰'da çözülemeyen olayların sıklığının sayılmak yerine kestirilmesidir.

## 2. Kestirici (`src/duz.c`)

**Olay ve sıfırlar**
- Olay aynıdır: olayın merceğinde kapı Gc ve olayın bütün yüklemleri. D(a), a dizilimindeki tutmayan koşul sayısıdır. Olay {D = 0}'dır.
- Hedef dağılım π, sıfırın kendi tanımıdır:
  - **tekdüze:** bütün dizilimler eşit olasılıklı;
  - **M20:** S = Σ n·aₙ ≤ S_Kur'an olan dizilimler eşit olasılıklı (M20 zincirinin kabul kuralı);
  - **Mallows:** π(a) ∝ q^I(a), I = #{i < j : aᵢ < aⱼ}, q örnekleyicininki.

**Zincir**
- Metropolis zinciri, D üzerinde 1/g(D) ile ağırlıklanır. Kabul: min(1, π(a')/π(a) · g(D)/g(D')).
- Öneri, sabit ağırlıklı üç simetrik çekirdeğin karışımıdır (her biri 1/3):
  1. tekdüze iki konum;
  2. değerleri p'ye göre aynı kalanlı iki konum;
  3. değerleri 2p'ye göre aynı kalanlı iki konum.
- Kalan sınıfları bu yer değiştirmelerle değişmez; öneri simetriktir.
- Şartlar zinciri sınırlamaz. Zincir bütün D düzeyleri arasında dolaşır.

**Aşamalar**
- **Aşama 1:** Wang–Landau, 1/t çizelgesi (Belardinelli–Pereyra), W adım.
- **Aşama 2:** ağırlık sabit, M adım. π(D = 0), ziyaret histogramından yeniden ağırlıklamayla okunur.

**Bütçe ve tekrarlar**
- Bütçe, olayın düzey sayısı d = koşul sayısı + 1 ile ölçeklenir: W = max(5×10⁷, 4×10⁵ · d²), M = W/2. Kur'an'ın bütün profili için d = 23: W = 2,1×10⁸, M = 1,06×10⁸.
- Her olay için 4 bağımsız tekrar. Tohum tohum(s, 9, 16·olay_no + r), r = 1…4. Yeniden koşuda r = 5…8.

**Kalite şartı.** Bir tekrar şu üç koşulda "iyi" sayılır:
- aşama 2'ye 1/t evresinde girmiştir;
- aşama 2 histogramında en az ziyaret edilen düzey, ortalamanın en az 0,3'üdür;
- kestirim sıfırdan büyüktür.

İyi olmayan tekrar bir kez, iki kat bütçeyle ve yeni tohumla (r + 4) yeniden koşulur. Dört iyi tekrar toplanamayan olay "kestirilemedi" sayılır.

**Birleştirme.** Nokta değer dört tekrarın ortalamasıdır. Aralık: ortalama ×/÷ exp(3,182 · sd(ln p) / 2), yani t(0,975; 3) ile.

## 3. Doğrulama (okumadan önce; geçmezse okuma yapılmaz)

**D1, nadir uç; seçim sayımdan bağımsız**
- Seçim: her sıfırda, Kur'an dışı, ilk 10⁸ kontrolde (bölüt 1–400) hiç isabet almamış olaylar arasından 40 olay. Havuz: M20 936, Mallows 439, tekdüze 423.
- Rastgele seçimin tohumu bu ekin SHA-256'sıdır: `random.Random(int(sha, 16))`. Havuz ad sırasına dizilir; önce M20, sonra Mallows, sonra tekdüze için D1 (`sample(havuz, 40)`), ardından aynı sırayla D2 (`sample(havuz, 20)`).
- Karşılaştırma: kestirim × N₄ ile bölüt 401–40.000'deki sayım k₄ (N₄ = 9,9×10⁹). Seçim bölüt 1–400'e baktığı için k₄ seçimden bağımsızdır.

**D2, sık uç**
- Seçim: her sıfırda, ana hattın derinlik kuralıyla 10⁸'e kadar çözülmüş (k ≥ 10) Kur'an dışı olaylardan rastgele 20 olay.
- Karşılaştırma: doğrudan p = k/N.

**Şartlar** (her sıfırda, D1 ve D2 için ayrı ayrı):
- (a) log₂((k + 0,5)/(p̂N + 0,5)) medyanı ∈ [−0,5; +0,5] bit;
- (b) Σk / Σp̂N ∈ [0,8; 1,25];
- (c) olayların en az %85'inde k, [Pois₀,₀₀₅(p̂_alt·N), Pois₀,₉₉₅(p̂_üst·N)] aralığında;
- (d) yalnız D1 için: kestirime göre en nadir üçte birin medyanı ∈ [−1; +1] bit;
- (e) kestirilemeyen olay ≤ %10.

Bir sıfırda şartlardan biri sağlanmazsa, o sıfırın okuması yapılmaz ve O2/O3 için yalnız ana hattın üst sınırı geçerli kalır.

Kur'an'ın O1'i (M20'de 3, Mallows'ta 2 isabet) betimsel bir tutarlılık denetimidir.

**Uyarı.** Kur'an'ın bütün profili, beklenen sıklığıyla, D1'in en nadir olaylarının 1–3 basamak altındadır. Bu bölgede kestirici doğrudan sayımla sınanamaz; okuma bir dışdeğerlemeye dayanır ve bu, sonuçla birlikte yazılır.

## 4. Kestirilecek olaylar

- **Kur'an:** O1 ve O2 her sıfırda. Kur'an'da O3 = O2'dir; aynı kestirim iki kurala da girer.
- **Plasebolar:** her kuralda, ana hattın derinlik kuralıyla 10¹⁰'da k ≤ 9 alan bütün olaylar. İstisna: doğrudan sayımın %95 alt sınırı Kur'an'ın kestirim aralığının üst ucunun üstündeyse olay kestirilmez; doğrudan sayım onu Kur'an'dan sık gösterir.
- **Sıra:** Kur'an'ın kestirimi plasebo seçiminden önce yapılır, çünkü istisna kuralı ona dayanır. Doğrulama koşuları aynı anda yürüyebilir; ama okuma, doğrulama tamamlanıp değerlendirilmeden yapılmaz.

## 5. Okuma

- **Sıklık:** p_e, k ≥ 10 ise doğrudan sayımdır, değilse kestirimdir. Kur'an da aynı kurala uyar: üç olayında da k < 10, yani kestirim kullanılır.
- **Sıra:**
  - R_t = #{plasebo : p_e ≤ p̂_Kur'an}.
  - Aralık [R_alt, R_üst]. R_alt: üst ucu p̂_Kur'an'ın alt ucunun altında kalanlar. R_üst: alt ucu p̂_Kur'an'ın üst ucunun altında kalanlar ve kestirilemeyenler.
- **Kalibre değer:** p_kal,t = (1 + R_t)/(K + 1). Okuma kuralları ana hattaki gibidir: aşar / arada / sık.
- **Menü:** 3 × en küçük p_kal.
- **Fark (betimsel):** Kur'an'ın kestirimi ile en yakın beş plasebonunki arasındaki bit farkı.

## 6. Geliştirme beyanı

- **Alt küme benzetimi (`src/zincir.c`):** Şartları tek tek sabitleyen zincirler eşitlik şartları altında donuyor. Kur'an'ın O1'inde, iki öneri çekirdeğiyle de, son basamak sıfıra düştü. Yöntem bırakıldı.
- **Düz histogram:** Kur'an'ın olaylarında denendi.

  | Olay | Kestirim (4 tekrar) | Doğrudan sayım (10¹⁰) |
  |---|---|---|
  | O1, M20 (iki koşu) | 2,8–3,4 × 10⁻¹⁰ ve 2,8–3,5 × 10⁻¹⁰ | 3 isabet |
  | O1, tekdüze | 0,9–1,0 × 10⁻¹⁰ | 0 isabet |
  | O2, M20 (yakınsamış koşu) | 4,7–5,3 × 10⁻¹³ (ortalama 5,0 × 10⁻¹³) | 0 isabet |

  - O2, M20'de daha kısa ve yakınsamamış bir koşu da yapıldı: 4,2–11,5 × 10⁻¹³.
  - Mallows'ta iki hata bulundu ve düzeltildi:
    1. **Ağırlıklı kabul.** π oranı q^ΔI yalnız ΔI > 0'da hesaba katılıyordu; kestirim doğrudan sayımın 35 katı çıktı.
    2. **Yarış.** Çok iş parçacığında `orn_init` ortak REF_DESC tablosunu yerinde sıralıyordu, bu da başlangıç dizilimlerini bozabiliyordu. Artık kilitli.

    Bu yüzden Mallows'ta geliştirme sonucu yok sayılır.
- **Kabul (`kabul_duz/`):**
  - Tek merceklik profil (`tek_mercek.h`, `tek_uret.py` ile kdegerlendir'den mekanik olarak üretildi) Mac'te 5,47 milyon, iş istasyonunda 2,74 milyon karşılaştırmada özgünle bayt bayt aynı.
  - ΔI formülü 200.000 yer değiştirmede kaba kuvvetle aynı.
  - Üç sıfırda `duz.c` (özgün yol, çok iş parçacığı), `duz_emul` (kart çekirdeği, işlemci) ve `duz_kart` (RTX 4090 ve 3090) aynı tohumla aynı çıktıyı veriyor.

## 7. Kaynak

**Motorlar.** Üç uygulama aynı tohumla aynı sonucu verdiği için her yürüyüş herhangi birinde koşulabilir:
- iş istasyonu işlemcisi: en çok 12 iş parçacığı, kart süreçleri dahil; `nice -n 10`;
- iki kart;
- Mac: en çok 4 iş parçacığı.

**Dağılım.**
- Toplam adımı 1,5 × 10⁸ ve altındaki yürüyüşler kartlarda koşar; kartlar yürüyüş başına yavaş, ama çok yürüyüşü aynı anda yürütür.
- Ötekiler işlemcilerde koşar.

**Tahmini süre:** 4–6 saat.

## 8. Tahmin (ana koşudan önce)

Geliştirmede M20'de Kur'an'ın O2'si görüldü (~5 × 10⁻¹³). Bu, model bilgisinden (34,2 bit) ~6,7 bit nadir demektir. Aynı farkı öteki sıfırlara uygularsam:

| | M20 | Mallows | tekdüze |
|---|---|---|---|
| Kur'an O1 | 2–5 × 10⁻¹⁰ | 1–4 × 10⁻¹⁰ | 0,7–1,5 × 10⁻¹⁰ |
| Kur'an O2 = O3 | 3–8 × 10⁻¹³ | 0,5–5 × 10⁻¹³ | 0,05–1 × 10⁻¹³ |
| R_O2 | 10–40 | 2–15 | 0–8 |
| R_O3 | 0–3 | 0–2 | 0–1 |
| Menü (3 × en küçük p_kal) | ≈ 1/6.700 – 1/1.700 | ≈ 1/3.300 – 1/1.100 | ≈ 1/3.300 – 1/1.700 |

- **Doğrulama:** D1 ve D2'nin üç sıfırda da geçeceğini bekliyorum (olasılık ≈ 0,7).
- **En olası başarısızlık:** D1'in en nadir üçte birinde sistematik sapma.
