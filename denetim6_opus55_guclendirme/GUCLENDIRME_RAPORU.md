# hrf2 — "Kur'an Permütasyon Çalışması" için güçlendirme denemesi

**Tarih:** 29 Eylül 2026 · **Yazan:** Claude (Opus 5.5), kullanıcının isteğiyle
**Hedef:** `<hrf2>`. Depo yalnız okundu, hiçbir dosyası değiştirilmedi. Bütün çıktılar bu klasörde.
**Kural (kullanıcının):** güçlendirmek için hatalı davranmamak.
**Ön-kayıt:** `ONKAYIT.md` (18:47 UTC'de donduruldu, SHA-256 `d57c2caf…`) ve ek `ONKAYIT_EK.md` (18:54 UTC, `a775c6ea…`). İkisi
de rastgele kitapların puanlarına bakılmadan önce yazıldı. Bütün sayılar bu klasördeki kodla yeniden üretilebilir.

---

## 0. Hüküm

**Keşfedilen sayıyı gerçekten güçlendiremedim.** Güçlendirir gibi görünen tek sonucun, yakından bakınca bir varsayıma
dayandığı ortaya çıktı. O varsayım da tarihsel kayıtla çelişiyor. Ayrıntısı:

| # | Bulgu | Yön |
|---|---|---|
| 1 | Beşinci denetimin hesapları doğru. Dört kayıtlı puan dizisini **bit bit** yeniden ürettim. M sıfırı dengeye ulaşıyor. Sabit-mercek basamaklarını 10 kat örneklemle tekrarladım. Kur'an aleyhine bir kod hatası yok. | nötr (denetimi doğrular) |
| 2 | Merdivende bir boşluk vardı. §7b itirazı biçim (n+a) hakkındadır, modül hakkında değil. 19'un metinde geçtiğini iki taraf da kabul ediyor. **"19 sabit, geri kalan bütün belgeli serbestlik serbest"** basamağını ön-kayıtla ölçtüm: sıralı sıfırda **2,4×10⁻³** (~420'de 1), tekdüzede **3,0×10⁻⁵** (~34.000'de 1), ×7 okul dahil. Denetçinin alt basamağından (1/26; 1/1.000) ×16–34 güçlü. | güçlendirir gibi |
| 3 | **Ama bu basamak teraziyi bu çalışmanın kanıtı sayıyor.** Terazi (57 homojen sûre, Σ(n+a) = 6236 = toplam ayet; 57 heterojen, Σ = 6555) **birebir aynı hâliyle 7 Temmuz 2009'da yayımlanmış** (Wasim Masri, i3gaz.com). t = n + a merceği oradan devralınmış, yani terazi merceği "satın almış". Tutarlı hesapta terazi kanıt sayılmaz, mercek devralınmış sayılır. O zaman çalışmanın kendi bulgusu (kristal + İki El) defter bataryası ve ×7 okulla sıralı sıfırda **~1/35**, tekdüzede **~1/1.500** çıkıyor. Bu, denetçinin alt basamağıyla aynı mertebe. | 2'yi geri alır |
| 4 | Yazarın en lehine kalibre basamağı ("mercek verili", 2,3×10⁻⁷ / 6×10⁻⁶) **teraziyi iki kez sayıyor**: merceği bedava sayıyor ve aynı zamanda merceği seçmiş olan teraziyi kanıt olarak sayıyor. Tutarlı sürümü (sabit mercek, terazisiz): **tekdüze 5,8×10⁻⁵, sıralı 5,6–6,3×10⁻⁴**. Bu ×100–300 daha zayıf. | zayıflatır |
| 5 | §7b'nin tarih okuması belgeyle uyuşmuyor (dokuman.md'deki tarama merceği seçmedi). Gerçek köken 2009 literatürü. Bu, denetçinin "mercek metinden gelmedi" hükmünü **daha da güçlendirir**. | denetimi güçlendirir |
| 6 | Denetçinin atladığı M20 (yazarın kendi sıralı sıfırı) tam belgeli aramada ölçüldü: **4,6×10⁻³**, ×7 okulla **3,25×10⁻² (~31'de 1)**. Denetçinin J10 değerini (3,9×10⁻²) tutuyor. | denetimi doğrular |

**Dürüstçe söylenebilecek, sağlam kalan şey:** Aritmetik gerçek. Çalışmanın **kendi** bulgusu olan 19-ailesi yapısı
(kristal + İki El) devralınan (n+a, 19) merceğinde, iddia edilen katmanlar üzerinden **orta düzey bir aykırı değer**:
sıralı sıfırda ~1.700'de 1, tekdüzede ~17.000'de 1. Belgeli bütün hikâye serbestliği (yazarın boş çıkan aileleri) ve okul
serbestliği eklenince sıradan bölgeye iniyor: ~35'te 1 ve ~1.500'de 1. "Katrilyonda bir" bir olay frekansı olarak kalıyor.
Çalışmanın kendi bulgusunun olay frekansı ise terazisiz **~3×10⁻¹²** (tekdüze) ve **~3×10⁻¹⁰** (sıralı).

---

## 1. Neyi doğruladım (denetçinin lehine)

- **Kayıtlı diziler bit bit yeniden üretildi** (`sonuc/saglama.txt`). Denetçinin `robot`, `robot2` ve `robot4` programları dört
  kayıtlı puan dizisini (`T_J10_iddia_nomx`, `T_U_iddia_nomx`, `D_J10_iddia_nomx`, `J10_iddia_mt`) her iş parçacığının ilk
  1000 kitabında birebir aynı üretti. Yani denetçinin raporundaki p-değerleri gerçekten o kodun o kitaplardaki çıktısı.
- **Zararsız bir belgeleme farkı:** `J10_iddia_mt` raporda yazıldığı gibi 8 değil, **7** iş parçacığıyla koşulmuş. Thread-1'in
  ilk kitabı 285.714. sırada, 7 × 285.714 = 1.999.998 = dosyadaki kitap sayısı. Tohum (90) doğru. Sonuca etkisi yok.
- **M sıfırı dengede** (`sonuc/kesif_m_yakinsama.txt`, keşif amaçlı). Tam-azalandan başlayan zincirin S ortalaması 30.000.
  adımda S_obs'un %99,74'ü ve 240.000. adıma kadar değişmiyor. t'nin standart sapması 35,4 (gerçek kitapta 35,6).
- **Sabit-mercek basamakları 2×10⁷ kitapla tekrarlandı** (denetçi 2–4×10⁷ ile ölçmüştü):

  | Sabit mercek (1,1,19), iddia bataryası | Bu deneme | Denetçi |
  |---|---|---|
  | Tekdüze | 2,0×10⁻⁷ (3/2×10⁷) | 2,3×10⁻⁷ (9/4×10⁷) |
  | J10 | 5,7×10⁻⁶ (113/2×10⁷) | 6,1×10⁻⁶ (244/4×10⁷) |
  | M20 | 4,4×10⁻⁶ (87/2×10⁷) | 5,2×10⁻⁶ (206/4×10⁷) |
  | Defter, J10 | 3,8–4,0×10⁻⁵ | 3,75–4,4×10⁻⁵ |

- Robotlarda Kur'an **aleyhine** işleyen bir hata aradım, bulamadım. Tablo gürültüsünün ("kazananın laneti") rastgele
  kitapları kayırması olası bir kusurdu. 10 kat büyük tablolarla ölçtüm: etkisi ×0,7–0,94 (B2, §2.3). Önemsiz.

## 2. Merdivendeki boşluk: "19 metinden, biçim serbest" (ön-kayıtlı birincil analiz)

### 2.1 Gerekçe

Denetçi §7b'de "t = n + a biçimi ve bölümlemeler veriden seçildi" dedi ve "mercek serbest" basamağına geçti. Bu basamak
7 (17) biçim × **38 modül** = 266 (646) merceği serbest bırakıyor. Oysa §7b'nin gerekçesi modülle ilgili değil. 19,
74:30'da adıyla geçiyor ve yazarın not 16d'si bunu denetçiyle birlikte kabul ediyor. Bu itiraza karşılık gelen basamak
**"modül 19 sabit, biçim ve olgular serbest"** basamağıdır. Denetçinin kodunda **tek bir değişiklikle** ölçtüm: mercek uzayını
m = 19'a kısıtlayan bir maske (`src/yama.py`, diff'ler `src/*_yama.diff`, 12 satır). Olgular, düğümler, puanlar, sıfırlar ve
tablolar aynen korundu.

### 2.2 Sonuçlar (T2, 2×10⁷ rastgele kitap / hücre)

| Analiz | Tekdüze | J10 | M20 |
|---|---|---|---|
| A1a: iddia bataryası, 7 doğrusal biçim | 8,0×10⁻⁷ (15) | 1,88×10⁻⁵ (376) | 1,72×10⁻⁵ (343) |
| A1b: defter bataryası (matris kapalı), 7 biçim | 1,2×10⁻⁶ (23) | 1,97×10⁻⁴ (3.942) | 1,61×10⁻⁴ (3.227) |
| **A1c: defter, 17 biçim (7 doğrusal + 10 doğrusal-olmayan)** | **4,25×10⁻⁶** (84) | **3,31×10⁻⁴** (6.627) | **3,39×10⁻⁴** (6.786) |
| A1c × 7 okul | **3,0×10⁻⁵** (~34.000'de 1) | 2,3×10⁻³ | **2,4×10⁻³** (~420'de 1) |

Karşılaştırma (denetçi): mercek verili 2,3×10⁻⁷ / 6,1×10⁻⁶; mercek serbest (266) 1,0×10⁻⁵ / 2,7×10⁻⁴; defter + 646 mercek
1,44×10⁻⁴ / 5,5×10⁻³; ×7 → **1,0×10⁻³ / 3,9×10⁻²**. Ön-kayıtlı tahminlerim tuttu: A1c tekdüze ~1×10⁻⁵ [0,3–3] beklenmişti,
4,25×10⁻⁶ çıktı; sıralı ~4×10⁻⁴ [1–10] beklenmişti, 3,3–3,4×10⁻⁴ çıktı.

### 2.3 Duyarlılık

- **T1 / T3 istatistikleri** (B3): A1c tekdüze 1,9×10⁻⁵ / 1,0×10⁻⁵, J10 6,0×10⁻⁴ / 4,3×10⁻⁴, M20 5,1×10⁻⁴ / 5,0×10⁻⁴.
  T2'ye göre ×1,3–4,4 daha zayıf. Birincil istatistik T2'ydi, aralık bu.
- **Metnin adlandırdığı 14 modül** (B1; Kur'an'da geçen 30 tam sayının 3–40 aralığındakiler: 3–12, 19, 20, 30, 40):
  defter + 17 biçim tekdüzede 3,05×10⁻⁵, J10 1,33×10⁻³, M20 1,37×10⁻³. ×7 okulla **2,1×10⁻⁴ / 9,6×10⁻³ (~1/100)**.
- **Kör batarya** (hücreler de kapalı; B5, m = 19, 7 biçim): tekdüze 9,5×10⁻⁶, J10 3,45×10⁻⁵.
- **19 ve 38 = 2×19** (keşif amaçlı, yalnız zayıflatabilir): tekdüze 2,55×10⁻⁵ (×6), J10 5,0×10⁻⁴ (×1,5),
  M20 4,4×10⁻⁴ (×1,3). Tekdüze sonuç bu seçime duyarlı, sıralı sonuç değil.
- **Tablo hassasiyeti** (B2; m = 19'da 2×10⁷ kitaplık yeni tablolar, 2×10⁷ yeni kitap): tekdüze 2,95×10⁻⁶ (×0,69),
  J10 3,13×10⁻⁴ (×0,94), M20 3,06×10⁻⁴ (×0,90). Birincil sonuç sağlam.

**Ön-kayıt kuralı 3'e göre** (A1c×7 sıralı, denetçinin 3,9×10⁻²'sinden ≥ ×3 küçük) sonuç şöyle raporlanır: *19 metinden
sabit sayılınca, belgeli serbestliğin geri kalanı altında Kur'an sıralı sıfırda ~420'de 1, tekdüzede ~34.000'de 1'lik bir
aykırı değerdir.* **Bu varsayıma bağlıdır.** Bir sonraki bölüm, varsayımın örtük bir parçasının tarihsel kayıtla
çeliştiğini gösteriyor.

## 3. Asıl bulgu: terazi iki kez sayılıyor ve kökeni 2009

### 3.1 Soru (ön-kayıt eki, sonuç görülmeden)

"Mercek verili" basamağı hem merceği bedava sayıyor hem de teraziyi kanıt olarak sayıyor. Merceği terazi seçtiyse bu
çift saymadır. Tutarlı iki seçenek var:

- **(i)** Teraziyi say, biçim serbestliğini öde. Bu §2'deki A1'dir.
- **(ii)** Merceği devralınmış say, teraziyi sayma. Terazi merceği "satın almıştır".

### 3.2 Ölçüm (keşif amaçlı; `ONKAYIT_EK.md`, N = 2×10⁶)

| Terazi kanıt sayılmazsa | Tekdüze | J10 | M20 |
|---|---|---|---|
| E1: sabit (1,1,19), iddia bataryası | **5,8×10⁻⁵** (115) | **6,25×10⁻⁴** (1.250) | **5,6×10⁻⁴** (1.125) |
| E2: m = 19, 7 biçim, iddia | 1,7×10⁻⁴ | 2,1×10⁻³ | 1,9×10⁻³ |
| E3: sabit (1,1,19), defter bataryası | 9,6×10⁻⁵ (191) | 4,2×10⁻³ (8.394) | 3,9×10⁻³ (7.794) |
| E3 sabit × 7 okul | **6,7×10⁻⁴** (~1.500'de 1) | **2,9×10⁻²** (~34'te 1) | 2,7×10⁻² |
| E3: m = 19, 17 biçim, defter (en muhafazakâr) | 6,4×10⁻⁴ | 2,2×10⁻² | 2,0×10⁻² |

Terazi, Kur'an'ın kalibre nadirliğinin büyük kısmını taşıyor. Onu çıkarmak sabit mercekte p'yi ×100–300 büyütüyor (bu denemenin sabit-mercek değerlerine göre ×110–290).

### 3.3 Terazinin kökeni (bu soruyu E sonucu görüldükten sonra araştırdım, bunu açıkça yazıyorum)

Yazarın not 4'ü zaten "Katman 1, literatürde bilinen bir gözlemin keskinleştirilmiş hâlidir" diyor. Literatürdeki gözlemi
aradım. **Katman 1'in tamamı 7 Temmuz 2009'da yayımlanmış.** Kaynak: Wasim Masri (وسيم مصري), "الإحكام في توزيع الآيات",
i3gaz.com:

> "هنالك سور تتطابق أرقامها وعدد آياتها زوجياً أو فردياً … هذه هي السور المتجانسه والتي عددها 57 سوره. الأمر المثير
> للاهتمام هو أن مجموع آيات وأرقام هذه السور المتجانسه هو 6236" · "… السور الغير متجانسه والتي عددها 57 سوره …
> مجموع آيات وأرقام هذه السور الغير متجانسه هو 6555"

Yani 57 homojen sûre ve Σ(n+a) = 6236; 57 heterojen sûre ve Σ(n+a) = 6555. Bu Katman 1'in kendisi. Aynı iddia başka
sitelerde de dolaşıyor. Örneğin tarihsiz iqra-quran.com sayfası, 50/50 bölünmeyle birlikte "The sum of the even numbers
is 6236 … The sum of the odd numbers is 6555" diyor. Kaynaklar (29 Eylül 2026'da erişildi):
<https://www.i3gaz.com/2009/07/07/al2i7kam-fi-tawzi3-al2ayat> ·
<https://iqra-quran.com/miracles-of-quran/mathematical-miracles-in-the-quran/>.

"Homojen" (sûre no ile ayet sayısı aynı paritede) tam olarak t = n + a'nın çift
olmasıdır. Dolayısıyla **t = n + a biçimi ve t-paritesi bölümlemesi, sayısal-mucize literatürünün (belgelenemeyen,
çok geniş) aramasıyla bulunmuş bir desenden devralınmıştır.** 19-ailesi yapısının (12 sûre, Σt = 1444 = 38²) daha önce
yayımlandığına dair bir iz bulamadım. Görünüşe göre kristal ve İki El bu çalışmanın kendi bulgusu.

### 3.4 Sonuç

Tarih (ii)'yi destekliyor. Terazi bu çalışmanın bulgusu değil. Onu bulan aramanın büyüklüğü bilinmiyor ve not 12'nin
"fiyatlanamaz" dediği türden. Bu yüzden:

- **A1 (§2) iyimser bir sınırdır.** Terazinin arkasındaki aramayı robotun 7–17 biçim × 12 eşitlik çatalına eşit sayıyor.
- **E3 muhafazakâr sınırdır.** Terazinin arkasındaki aramanın onu tamamen "harcadığını" varsayıyor.
- Gerçek değer ikisinin arasında, ama nerede olduğu belgelenemeyen bir aramaya bağlı. Kullanıcının kuralı gereği
  **manşete muhafazakâr ucu** koyuyorum: çalışmanın kendi bulgusu, belgeli bütün serbestlikle, sıralı sıfırda ~1/35,
  tekdüzede ~1/1.500. Bu, denetçinin alt basamağıyla (1/26; 1/1.000) aynı mertebe. §2'deki güçlenme bu hesapta kayboluyor.
- **"Mercek verili" basamağı her iki okumada da tutarsızdır.** (i)'ye göre A1a ile (tekdüze 8×10⁻⁷, sıralı ~2×10⁻⁵),
  (ii)'ye göre E1 ile (tekdüze 5,8×10⁻⁵, sıralı ~6×10⁻⁴) değiştirilmelidir. Tarih (ii)'yi gösterdiği için E1'i öneriyorum.
- **Olay frekansı:** deponun ölçtüğü bileşenlerle, terazi çıkarılınca çalışmanın kendi olayı P(G∧W) =
  7,14×10⁻¹⁰ × 3,85×10⁻³ = **2,75×10⁻¹²** (tekdüze). Sıralı sıfırda P(G) = 6,3–8,5×10⁻⁸ ile **~2,4–3,3×10⁻¹⁰**
  (P(W|G) tekdüzedekiyle aynı varsayıldı; `sonuc/turetilmis_olay_frekansi.txt`). "Katrilyonda bir"in ~×2.800'lük kısmı
  (1/[P(A)·κ]) 2009'da yayımlanmış terazinin çarpanıdır.

## 4. §7b'nin kronolojisi: denetçinin tarihi yanlış, hükmü doğru

Denetçi §7b'de şöyle yazdı: t-paritesi ve t ≡ 0 (mod 19) "kaynak dokümandaki (dokuman.md) bir veri taramasından
geliyor". Belge bunu göstermiyor. dokuman.md bir sohbetin ortasından başlıyor. P ve R₀ ilk satırlardan itibaren zaten
tanımlı ("Tek/çift parite matrisimiz P şuydu", "R matrisi, şu ayrımdan gelmişti"). Kalıntı taraması ("Evet, bu adımı
sistematik yapalım", satır 779 ve sonrası) **sonra** geliyor ve r = 0'a ek olarak r = 4'ü buluyor. Yani tarama merceği
seçmedi, sonradan yapılmış bir kontroldü. Denetçinin kaynağa dair tespiti yanlış.

Ancak gerçek köken, denetçinin hükmünü **zayıflatmıyor, güçlendiriyor**. Biçim ve parite bölümlemesi, 2009'da yayımlanmış
bir desenden devralındı (§3.3). Tevbe = 129'da matris kancasının olmaması (r = 3, 7, 9) ise yalnız araştırmanın
**motivasyonunu** etkiler. O motivasyon kristalin olgularından bağımsızdır ve kristalin nadirliğini "harcamaz".

## 5. Denetçinin atladığı basamak: M20 sıfırında tam belgeli arama (B4)

Denetçi "tam belgeli arama" basamağını (defter bataryası, matris kapalı, 646 mercek) yalnız J10 ve tekdüze sıfırda
ölçmüştü. M20'yi (yazarın kendi `kesif16` sıfırı) atlamıştı (`sonuc/gecis.log`: "M20 atlandı"). Denetçinin orijinal `robot4`'üyle, yeni
bir M20 tablosu kurarak (2×10⁶, tohum 1950) ve 2×10⁶ kitap puanlayarak (tohum 1951) ölçtüm:

| Tam belgeli arama (646 mercek) | serbest p | × 7 okul | sabit (1,1,19) |
|---|---|---|---|
| **M20 (bu deneme)** | **4,64×10⁻³** (9.276/2×10⁶) | **3,25×10⁻² (~31'de 1)** | 3,8×10⁻⁵ (75) |
| J10 (denetçi) | 5,5×10⁻³ (10.994/2×10⁶) | 3,9×10⁻² (~26'da 1) | 4,4×10⁻⁵ |
| Tekdüze (denetçi) | 1,44×10⁻⁴ (287/2×10⁶) | 1,0×10⁻³ | 5×10⁻⁷ |

Yazarın kendi sıralı sıfırı, denetçinin titreşim sıfırıyla aynı sonucu veriyor. Merdivenin alt basamağı iki farklı sıralı
sıfırda da ~1/30. M20'de sabit mercek değeri (3,8×10⁻⁵) başka bir tablo tohumuyla A1c'deki değeri (3,75×10⁻⁵) tekrarlıyor.

## 6. Önerilen merdiven (yazar benimserse; depoda hiçbir şey değiştirmedim)

| Basamak | Varsayım | Tekdüze | Sıralı (J10 / M20) | Durum |
|---|---|---|---|---|
| Olay frekansı, terazi ∧ kristal ∧ İki El | beyan edilen olay | 1,1×10⁻¹⁵ | ≈1,5×10⁻¹³ | olay frekansı; anomali ölçüsü değil |
| **Olay frekansı, çalışmanın kendi bulgusu** (kristal ∧ İki El) | terazi 2009 literatürüne ait | **2,75×10⁻¹²** | **~3×10⁻¹⁰** | yeni (türetilmiş) |
| ~~Simetrik, mercek verili~~ | teraziyi iki kez sayıyor | ~~2×10⁻⁷~~ | ~~6×10⁻⁶~~ | **kaldırılmalı / değiştirilmeli** |
| **Simetrik, mercek devralınmış, terazisiz** (E1) | n+a 2009'dan, 19 metinden; terazi merceği satın aldı | **5,8×10⁻⁵** | **6,3 / 5,6×10⁻⁴** | yeni (keşif amaçlı) |
| Simetrik, 19 metinden, biçim serbest, terazi sayılı (A1a) | terazinin arkasındaki arama = robotun çatalları (iyimser) | 8,0×10⁻⁷ | 1,9 / 1,7×10⁻⁵ | yeni (ön-kayıtlı) |
| Simetrik, mercek serbest (266) | denetçi | 1×10⁻⁵ | 2,7 / 2,4×10⁻⁴ | aynen |
| **Devralınmış mercek + defter + ×7 okul** (E3 × 7) | çalışmanın kendi bulgusu, belgeli bütün hikâye ve okul serbestliği | **6,7×10⁻⁴** | **2,9 / 2,7×10⁻²** | yeni (keşif amaçlı), **muhafazakâr manşet** |
| 19 metinden + defter + 17 biçim + ×7 (A1c × 7) | iyimser (terazi sayılı) | 3,0×10⁻⁵ | 2,3 / 2,4×10⁻³ | yeni (ön-kayıtlı) |
| Metindeki 14 sayı + defter + 17 biçim + ×7 (B1c × 7) | iyimser (terazi sayılı) | 2,1×10⁻⁴ | 9,3 / 9,6×10⁻³ | yeni (ön-kayıtlı) |
| Denetçinin tüm belgeli serbestliği (646 mercek, ×7) | denetçi | 1,0×10⁻³ | 3,9×10⁻² / **3,25×10⁻²** | aynen; M20 bu denemede |
| Ötesi | 19 geleneğinin ve sayısal-mucize literatürünün araması, kültürel havuz | fiyatlanamaz | fiyatlanamaz | aynen |

**Merdivenin okunuşu:** Aritmetik gerçek. Çalışmanın kendi bulgusu, devralınan mercekte ve iddia edilen katmanlar
üzerinden orta düzey bir aykırı değer (tekdüze ~6×10⁻⁵, sıralı ~6×10⁻⁴). Belgeli bütün serbestlik ödenince sıralı sıfırda
sıradan (~1/30), tekdüzede hafif aykırı (~1/1.500). Bu iki uç arası, bu verinin bugünkü dürüst fiyatıdır.

## 7. Güçlendirmeyi neden yapamadım, ve bu denemenin sınırları

- **Sıralı sıfır doğru sıfırdır.** Mushafın kabaca uzunluğa göre dizilmesi belgeli ve sayısal olmayan bir derleme
  ilkesi. "Sayısal tasarım var mı?" sorusu için uzunluk sırasına koşullanmak gerekir. Bu yüzden sıralı sütunlar birincil
  okumadır. Tekdüze sütunlar üst sınırdır.
- **Terazi bu çalışmanın bulgusu değil.** Onu kanıt olarak saymanın tek meşru yolu, 2009 literatürünün aramasını
  fiyatlamak. Bu mümkün değil.
- **Düğüm-bağımsızlığı yaklaşıklığı** (denetçi: Kur'an lehine) bütün basamaklarda aynen var.
- **×7 okul** bir Bonferroni üst sınırı. Gerçek çarpan daha küçük olabilir, ama bunu ölçmek için 7 okulun sûre-bazlı
  verisi gerekir. Depoda yalnız Kûfe ve Medine var.
- **Benim çatallarım:** A1 ön-kayıtlıydı. E, A1 sonuçlarına bakmadan eklendi. Terazinin kökenini ise E sonucunu gördükten
  sonra araştırdım. O araştırma yazarın lehine değil aleyhine sonuç verdi. Onu raporlamamak "güçlendirmek için hatalı
  davranmak" olurdu. 19+38 kontrolü de sonradan eklendi ve yalnız zayıflatabilecek bir kontroldü.
- **Veri:** Kûfe sayımı, Tevbe = 127 (denetçinin robotlarıyla aynı). Çekirdek katmanlar 127/129'dan bağımsız.

## 8. Yeniden üretim

```bash
cd denetim6_opus55_guclendirme
python3 src/yama.py                         # denetçinin robotlarına yalnız mercek maskesi ekler
for r in robot robot2 robot4; do cc -O3 -o $r src/$r.c -lpthread -lm; cc -O3 -o ${r}_m src/${r}_m.c -lpthread -lm; done
python3 kontrol/saglama.py                   # S1–S4: denetçinin kayıtlı dizileri bit bit
./kos_hepsi.sh                               # ön-kayıtlı bütün koşular (A1, B1, B2, B4, B5)
./kos_ek.sh ; ./kos_ek2.sh                   # keşif amaçlı: E (terazisiz) ve 19+38
python3 analiz.py                            # → sonuc/ONKAYITLI_SONUCLAR.md (puan/ yoksa puan_ozet/'ten, aynı p-değerleri)
python3 kontrol/m_yakinsama.py               # M sıfırı denge tanısı
```

| Dosya | İçerik |
|---|---|
| `ONKAYIT.md`, `ONKAYIT_EK.md` | ön-kayıtlar (özetleri `sonuc/onkayit_ozet.txt`, `sonuc/onkayit_ek_ozet.txt`) |
| `src/` | denetçinin kaynakları (birebir), yama betiği ve diff'ler |
| `tab/` | denetçinin tabloları (SHA-256: `sonuc/sha256_girdiler.txt`) ve bu denemenin yeni tabloları |
| `puan_ozet/` | rastgele-kitap puanlarının doğrulanmış kuyruk özetleri (45 MB). Tam diziler (9,9 GB) disk dolduğu için silindi; tohumlu koşularla bit bit yeniden üretilir (`puan_ozet/BENIOKU.md`) |
| `puan/` | Kur'an'ın her koşudaki puanı (`*_kuran.txt`) |
| `sonuc/ONKAYITLI_SONUCLAR.md` | otomatik sonuç tablosu (bütün p-değerleri, aralıklar, parça başına sayımlar) |
| `sonuc/saglama.txt`, `sonuc/kesif_m_yakinsama.txt`, `sonuc/turetilmis_olay_frekansi.txt` | sağlamalar ve türetilmiş değerler |

---

## English summary

**Task.** At the user's request, try to strengthen the Quran permutation study (`hrf2`) and its headline number, without
cutting corners. The repository was only read; all code and outputs are in this folder. The main analysis was
pre-registered (`ONKAYIT.md`) before any null scores were examined. The balance-excluded check is an exploratory addendum
(`ONKAYIT_EK.md`), also registered before results. The provenance search for the balance layer came after seeing that
check, and it cut against the favourable reading.

**Verdict: I could not genuinely strengthen it.**

1. **The fifth review's numerics hold.** Its code reproduces its saved score arrays bit for bit. The M (sorted) null
   equilibrates. The fixed-lens rungs replicate at 10× the sample size. I found no Quran-unfavourable bug.
2. **A real gap, measured (pre-registered).** §7b of the fifth review objects to the *form* (n+a), not the *modulus*. Both
   sides accept that 19 is named in the text (74:30). Fixing m = 19 and freeing everything else documented (the ledger
   battery with the author's null families, 17 linear and non-linear forms, ×7 counting schools) gives **2.4×10⁻³**
   (sorted null) and **3.0×10⁻⁵** (uniform null). The fifth review's corresponding values were 1/26 and 1/1,000.
   Robustness: T1/T3 ×1.3–4.4, table size ×0.7–0.94, text-named moduli ×4, adding m = 38 ×1.3–6.
3. **But that rung counts the "balance" layer as this study's evidence, and it is not.** Layer 1 (57 homogeneous surahs
   with Σ(n+a) = 6236 = total verses; 57 heterogeneous with Σ = 6555) was published verbatim on 7 July 2009 (Wasim Masri,
   i3gaz.com). The n+a lens was therefore inherited from a pattern found by the numerological literature's undocumented
   search. Consistent accounting grants the inherited lens and does *not* count the balance. Measured that way, the study's
   own finding (the mod-19 crystal plus the "two hands", which I could not find published earlier) is **5.8×10⁻⁵
   (uniform) / ~6×10⁻⁴ (sorted)** at the fixed lens over the claimed layers. With the full ledger and ×7 schools it is
   **~1/1,500 / ~1/35**, the same order as the fifth review's bottom rung.
4. **The "lens granted" rung (2×10⁻⁷ / 6×10⁻⁶) double-counts.** The balance both selected the lens and is counted as
   evidence. It should be replaced by the consistent value above, which is ×100–300 weaker.
5. **§7b's history is wrong, but its verdict is right.** In `dokuman.md` the P and R₀ partitions predate the residue scan,
   so the scan did not select the lens. The true origin, a 2009 publication, makes "the lens is not text-derived" stronger.
6. **The event frequency of the study's own finding** (crystal ∧ two hands, balance excluded) is **2.75×10⁻¹²** (uniform)
   and **~3×10⁻¹⁰** (sorted), from the repository's own measured components.
7. **The author's own sorted null (M20), which the fifth review skipped for the full documented search, gives the same
   bottom rung:** 4.6×10⁻³, ×7 schools → **3.25×10⁻² (~1 in 31)**, compared with the fifth review's J10 value of 3.9×10⁻².

**What honestly remains:** the arithmetic is real. The study's own mod-19 structure is a moderate outlier at the inherited
lens (~1 in 1,700 sorted, ~1 in 17,000 uniform, before story-extension and school freedoms). With every documented freedom
priced it is ordinary under the sorted null (~1 in 30) and a mild outlier under the uniform null (~1 in 1,500).
