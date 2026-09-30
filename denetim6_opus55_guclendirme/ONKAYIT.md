# ÖN-KAYIT — hrf2 güçlendirme denemesi

**Tarih:** 29 Eylül 2026 · **Yazan:** Claude (Opus 5.5), kullanıcının isteğiyle
**Statü:** Rastgele kitapların kısıtlı mercek uzayında puanlanmasından ÖNCE yazıldı ve donduruldu. SHA-256 özeti
`sonuc/onkayit_ozet.txt` dosyasına, koşulardan önce yazıldı.
**Kural (kullanıcının):** güçlendirmek için hatalı davranmamak. Mevcut merdivenin hiçbir basamağı değiştirilmez ya da
çıkarılmaz. Sonuç hangi yöne çıkarsa çıksın aynen yazılır.
**Depo:** `<hrf2>` yalnız okunur. Bütün çıktılar bu klasördedir.

---

## 1. Gerekçe (sonuç görülmeden yazıldı)

Beşinci denetim (`denetim5_opus55/KIRMA_RAPORU.md`, §7b), "mercek verili" basamağını zayıf saydı. Gerekçesi şuydu:
**t = n + a biçimi ve t-paritesi / t ≡ 0 (mod 19) bölümlemeleri metinden gelmiyor, veriden seçildi.** Buradan
**"mercek serbest"** basamağına geçti: 7 (ya da 17) biçim × **38 modül** (m = 3…40) = 266 (646) mercek.

§7b'nin itirazı biçim ve bölümleme hakkındadır, modül hakkında değildir. Modül 19 metinde adıyla geçer (74:30). Yazar da
denetçi de bunu kabul ediyor (KESIF.md not 16d: "Modül 19 ve parite metinde adlandırılıdır; t=n+a biçimi ve bölümlemeler
veriden … seçilmiştir"). §7b'ye mantıksal olarak karşılık gelen basamak şudur: **modül 19 sabit (metin), biçim ve olgular
serbest.** Bu basamak merdivende yok. Denetçi "biçim veriden seçildi" itirazından doğrudan "38 modülün hepsi serbest"
varsayımına geçti.

Ek tarihsel gözlem (ölçüm değil; §7'de ayrıntılı): `dokuman.md`de P (t-paritesi matrisi) ve R₀ (t ≡ 0 mod 19 matrisi)
**tarama başlamadan önce** tanımlıdır ("Tek/çift parite matrisimiz P şuydu"). Kalıntı taraması (satır 779 ve sonrası, "Evet,
bu adımı sistematik yapalım") sonradan yapılmış bir kontroldür. Yani 25 bölümlemelik tarama merceği **seçmedi**. Ancak
t = n + a biçiminin kendisi de metinden gelmez. Bu biçim, sûre no ile ayet sayısının aynı pariteyi taşıyıp taşımadığına
bakan homojen/heterojen çift-tek literatüründen gelir. Katman 1'in literatürdeki bir gözlemin keskinleştirilmiş hâli
olduğunu yazar zaten kabul ediyor (KESIF.md not 4). Bu yüzden biçim serbestliğinin fiyatlanması doğrudur. Dolayısıyla
doğru düzeltme yine **"19 sabit, biçim serbest"** basamağıdır.

## 2. Kod ve tek değişiklik

- Kaynaklar: beşinci denetimin `robot.c`, `robot2.c`, `robot4.c` dosyaları. Depodaki `denetim5_opus55/` kopyalarıyla birebir
  aynılar (`cmp` ile doğrulandı). SHA-256 özetleri `sonuc/sha256_girdiler.txt` dosyasında.
- **Tek değişiklik** (`src/yama.py`, diff'ler `src/*_yama.diff`): `ROBOT_MODS` ortam değişkeniyle mercek uzayı verilen
  modüllere kısıtlanır. Tablo kurma ve puanlama döngüleri yalnız izinli mercekleri dolaşır. Olgular, düğümler, puanlar, sıfır
  örnekleyicileri, tohum şeması, MINTR geri-düşüşü ve devre-dışı mantığı **aynen** korunur.
- Tablolar: denetçinin 2×10⁶'lık tabloları (`tab/` kopyaları; özetler kayıtlı). Denetçinin M20 sıfırında defter tablosu
  yoktu. Onları aynı boyutta (2×10⁶) yeni tohumlarla kuracağım (aşağıda).

## 3. Birincil analiz — A1: "19-analisti" basamağı (modül 19 sabit, biçim serbest)

Mercek kümesi = {m = 19} × robotun bütün biçimleri. Kur'an'ın puanı değişmez (en iyi merceği zaten (1,1,19)).

| Kod | Robot | Batarya (denetçinin tanımı) | Biçim | Sıfırlar ve tablolar |
|---|---|---|---|---|
| A1a | `robot_m` (60 olgu) | iddia: `ROBOT_DISABLE="Σw=m·kG\|Σv(U) kare/asal-kuvvet"`, `ROBOT_MINTR=1000` | 7 doğrusal | U: `U_2M.bin` · J10: `J10_2M.bin` · M20: `M20_2M.bin` |
| A1b | `robot2_m` (99 olgu, defter) | defter-iddia, matris kapalı: `…\|MX:` | 7 doğrusal | U: `D_U_2M.bin` · J10: `D_J10_2M.bin` · M20: **yeni** 2×10⁶ tablo, tohum 1920 |
| **A1c** | `robot4_m` (99 olgu, defter) | defter-iddia, matris kapalı | **17** (7 doğrusal + 10 doğrusal-olmayan) | U: `T_U_2M.bin` · J10: `T_J10_2M.bin` · M20: **yeni** 2×10⁶ tablo (14 metin modülünde kurulur, bkz. B1), tohum 1930 |

- **A1c birincil basamaktır.** Anlamı: 19 dışındaki bütün belgeli serbestlik (defter bataryası, 17 biçim), denetçinin "tam
  belgeli arama" satırının modülü sabitlenmiş hâli.
- Puanlanan rastgele kitap sayısı: her (analiz, sıfır) için 10 × 2×10⁶ = **2×10⁷**, 8 iş parçacığı. Tohumlar: U 19001–19010,
  J10 19101–19110, M20 19201–19210 (üç analizde aynı, yani kitaplar eşleşik). Herhangi bir hücrede aşan kitap sayısı k < 20
  çıkarsa aynı tohum dizisi sürdürülerek 10⁸'e çıkılır.
- İstatistik: **T2** (en iyi 8 düğüm), izinli mercekler üzerinden en iyisi. p = (k+1)/(N+1), k = #{T2 ≥ T2_Kur'an − 10⁻⁶}.
  %95 aralık Clopper–Pearson.
- Sayım okulu: denetçinin ×7 Bonferroni üst sınırı aynen uygulanır. ×7'siz değer de verilir.
- Sıralı sıfırın birincil değeri, J10 ile M20 arasında **daha muhafazakâr olanıdır** (daha büyük p).

## 4. İkincil analizler

- **B1, "metnin adlandırdığı modüller" basamağı:** Kur'an'da geçen 30 tam sayının 3–40 aralığındakiler, yani
  m ∈ {3,4,5,6,7,8,9,10,11,12,19,20,30,40} (14 modül). Bu basamak kültürel havuzu **kısmen** fiyatlar: metin-merkezli
  bir sayı avcısı metindeki herhangi bir sayıyı çapa alabilirdi. A1a ve A1c bataryalarıyla U, J10 ve M20 sıfırlarında,
  N = 2×10⁶ (tohum U 19401, J10 19402, M20 19403). A1c için M20 tablosu bu 14 modülde kurulan tablodur (tohum 1930).
- **B2, tablo hassasiyeti (kazananın laneti kontrolü):** A1c için m = 19 merceklerinde 2×10⁷ kitaplık yeni tablolar kurulur
  (U 1940, J10 1941, M20 1942) ve 2×10⁷ yeni kitapla yeniden puanlanır (tohumlar 19501–19510, 19601–19610, 19701–19710).
  Birincille ×3'ten büyük fark çıkarsa iki değer de verilir, daha muhafazakâr olanı birincil sayılır.
- **B3:** T1 ve T3 istatistikleri, birincil çıktılardan hesaplanır. Yalnız raporlanır, manşete girmez.
- **B4, denetçinin merdivenini tamamlama:** denetçinin atladığı **M20** sıfırında "tam belgeli arama" (orijinal `robot4`,
  defter-iddia, matris kapalı, 646 mercek). Tablo 2×10⁶ (tohum 1950), puanlama 2×10⁶ (tohum 1951).
- **B5, kör batarya** (hücreler de kapalı: `…|hücreler dengeli`): A1a biçiminde, U ve J10, N = 2×10⁶ (tohumlar 19801, 19802).

## 5. Sağlamalar (analizden önce koşulur; biri bile tutmazsa analiz durur)

- **S1:** Orijinal robotlar, denetçinin kayıtlı puan dizilerindeki kitapları (her iş parçacığının ilk 1000 kitabı) birebir
  yeniden üretir: `J10_iddia_mt` (robot, tohum 90, 6 iş parçacığı), `D_J10_iddia_nomx` (robot2, tohum 22, 8),
  `T_J10_iddia_nomx` (robot4, tohum 42, 8), `T_U_iddia_nomx` (robot4, tohum 44, 8).
- **S2:** Yamalı robot `ROBOT_MODS` olmadan orijinalle birebir aynı çıktıyı verir.
- **S3:** `ROBOT_MODS=19` ile sabit-mercek (1,1,19) sütunu birebir aynı kalır; en-iyi T2 hiçbir kitapta tam uzaydakinden
  büyük olamaz.
- **S4:** Kur'an'ın T2'si kısıtlı uzayda değişmez.

## 6. Önceden tahmin (sonuç görülmeden; tutmazsa öyle yazılacak)

Kaba akıl yürütme: denetçide sabit mercekten serbest merceğe geçişin çarpanı iddia bataryasında ×45 (J10), defterde ×83.
Tek modülde 7–17 biçim, bu çarpanın yalnız küçük bir kısmını getirmeli.

| | U | J10 / M20 |
|---|---|---|
| A1a | ~1,5×10⁻⁶ (0,5–5) | ~3×10⁻⁵ (1–10) |
| A1b | ~5×10⁻⁶ | ~2×10⁻⁴ |
| **A1c** | **~1×10⁻⁵ (0,3–3)** | **~4×10⁻⁴ (1–10)** |
| A1c × 7 okul | ~7×10⁻⁵ | ~3×10⁻³ |

Beklentim, A1c×7'nin denetçinin "tüm belgeli serbestlik" değerinden (J10 3,9×10⁻²; U 1,0×10⁻³) yaklaşık ×10 küçük
çıkması. Bu **büyük bir güçlendirme değildir**. Böyle çıkarsa sonuç "19-analisti varsayımı altında orta düzey bir aykırı
değer" diye okunur.

## 7. Tarihsel not (ölçüm değil, belge okuması)

`dokuman.md` bir sohbetin ortasından başlıyor. P ve R, önceki kısımda zaten tanımlı: "R matrisi, şu ayrımdan gelmişti:
sûre no + ayet sayısı 19'un katı olan sûreler…", "Tek/çift parite matrisimiz P şuydu". Sistematik tarama
(r = 0…18 kalıntıları ve beş başka ayrım) **sonra** geliyor ve r = 0 ile r = 4'ü buluyor. Denetçinin §7b'deki "bunlar
dokümandaki bir veri taramasından geliyor" cümlesi bu kronolojiyle uyuşmuyor. Aynı notun ikinci yarısı doğru ve raporda
ağırlığı bu yarıya vereceğim: Tevbe = 129'da matris kancası yok (r = 3, 7, 9), yani **araştırmanın motivasyonu** 127'ye
bağlıydı. Bu motivasyon kristalin olgularından bağımsızdır. Bu yüzden kristalin nadirliğini "harcamaz".

## 8. Yorum kuralları (önceden taahhüt)

1. Yeni basamaklar merdivene **eklenir**. Mevcut hiçbir basamak değişmez. Her basamak varsayımıyla yazılır.
2. Birincil okuma A1c'dir: sıralı sıfır (J10/M20'nin muhafazakâr olanı) ve tekdüze sıfır yan yana, ×7'li ve ×7'siz.
3. A1c×7 (sıralı), denetçinin 3,9×10⁻²'sinden **en az ×3 küçükse**, "19 metinden sabit sayılınca belgeli serbestliğin geri
   kalanı altında Kur'an 1/X düzeyinde aykırı" diye ve **bu varsayıma bağlı olarak** raporlanır. Fark ×3'ten küçükse
   "19'u sabitlemek tabloyu değiştirmiyor" diye raporlanır.
4. B2 birincili ×3'ten fazla değiştirirse daha muhafazakâr değer birincil olur.
5. Ön-kayıt dışında hiçbir batarya, istatistik ya da mercek kümesi manşete alınmaz. Sonradan akla gelen her analiz
   "keşif amaçlı" diye etiketlenir.
6. Bu analiz "19'u kim, neden seçti" sorusunu cevaplamaz. 19 geleneğinin onlarca yıllık araması ve kültürel havuz (not 12)
   fiyatlanamaz kalır. B1 bunu yalnız "metindeki sayılar" kadar fiyatlar.
7. Düğüm-bağımsızlığı yaklaşıklığı (denetçi: Kur'an lehine) bu analizde de aynen vardır. Raporda aynı uyarıyla yazılır.
