# Kur'an permütasyon çalışması: nadirliği kırmaya yönelik ikinci inceleme

**Tarih:** 20 Ekim 2026. **İncelenen sürüm:** `a9bec42c432607350eeb4ceebbb664bca8a3aa84`.

## Sonuç

**Nadirliğin modele dayanıklılığına karşı güçlü sayısal karşı örnekler buldum. Tam çekirdeği olağan bir olaya dönüştürmüş değilim.** Önceki yaklaşık `1,1×10⁻¹⁵` tam çekirdek tahmini, ayet sayılarının bütün konumlara eşit olasılıkla dağıtıldığı modele aittir. Yerel sırayı koruyan altılı blok modelinde tam çekirdek yaklaşık `1,1×10⁻¹²` çıkıyor. Aynı modele gözlenenden en az aynı derecede uzunluk sıralılığı koşulu eklendiğinde yaklaşık `9×10⁻¹³` kalıyor. Daha dar komşu çift modelinde ise tam çekirdeğin kesin olasılığı yaklaşık `6,2×10⁻¹¹`.

Bunlar önceki nokta tahminine göre yaklaşık **800–1000 kat** ve **56 bin kat** artışlardır. Bu karşılaştırma, serbest modelin kendi olasılığının yanlış hesaplandığı anlamına gelmez. Asıl bulgu, bu sayının farklı sıra varsayımlarına taşınamadığıdır.

Blokların koruduğu ayrıntıları kaldıran, yalnız uzunluk sırasından ayarlanmış daha geniş bir Mallows modelinde de **kristal G yaklaşık 126 kat sıklaştı**. Bu modelde tam çekirdeğin sıklığını güvenilir biçimde ölçmeye yetecek başarı çıkmadı. Kristal sonucunu tam çekirdek sonucu olarak sunmuyorum.

Bu rapordaki yeni sayılar ekli kod ve ham sonuçlardan hesaplanmıştır. Monte Carlo aralıkları belirtilen model içindeki örnekleme belirsizliğini kapsar; model seçimi, keşif sonrası seçim veya önceki `1,1×10⁻¹⁵` tahmininin belirsizliğini kapsamaz.

## 1. Sınanan olay ve sayımlar

Konum `n=1,…,114`, o konumdaki ayet sayısı `a`, `t=n+a`, `u=n−a` olsun.

- **A:** Çift t değerlerinin sayısı 57, toplamı ise bütün ayet sayılarının toplamıdır.
- **G:** t'nin 19'a bölündüğü aile tam 12 elemanlıdır. t ve n paritesine göre dört hücredeki `t/19` katsayıları çift-t hücrelerinde `(6,6,6)` ile `(6,6,8)`, tek-t hücrelerinde `(5,5,9)` ile `(5,7,7)` olur. Her iki hücre çifti kendi içinde yer değiştirebilir. Bu, çalışmadaki tam kristal koşuludur.
- **W:** u'nun 19'a bölündüğü aile 7 elemanlıdır; t ve u ailelerinin birleşimi 19 elemanlıdır; birleşimdeki ayet toplamı 19'a bölünür.
- **Tam çekirdek:** `A∩G∩W`. G tek başına bu ortak olaya eşit değildir.

İki girdi çeşidi ayrı incelendi. Depodaki `quran_meta.json` dokuzuncu sûre için 129 içeriyor ve toplam 6236. Nadirlik üretimlerinin çoğunda kullanılan 127 çeşidinin toplamı 6234. Bu rapordaki “127” ve “129” yalnız bu veri seçimini gösterir. Her çeşitte A'nın toplamı ve sıralılık eşiği kendi verisinden yeniden hesaplandı.

Karşılaştırma için önceki serbest-permütasyon kristal frekansı `7,142738482×10⁻¹⁰`, tam çekirdek nokta tahmini yaklaşık `1,1×10⁻¹⁵` alındı. İkinci sayı kesin bir kombinatorik sabit değildir. Çarpanları yaklaşık büyüklük olarak okumak gerekir.

## 2. Altılı bloklarda tam çekirdek: yaklaşık bin kat artış

Önce konumları `1–6, 7–12, …, 109–114` şeklinde bloklara ayırdım. Her bloktaki ayet sayıları kendi içinde eşit olasılıkla permüte edilir; bloklar arasında taşınmaz. Bu kural G, A veya W'ye göre başarı seçmez.

Bu başlangıç için G'nin olasılığı iki sayımda da tam olarak:

\[
P(G)=\frac{11077756692737}{24488801280000000000}
=4,523601039543002\times10^{-7}.
\]

Asıl yeni adım, G'nin gerçekleştiği diziler arasından **tam düzgün dağılımla** örnek çekip A ve W'yi birlikte saymaktır. İki sabit tohumda toplam 100 milyon G-koşullu çekiliş yapıldı; 127 için 262, 129 için 270 tam çekirdek bulundu.

| Sayım | Tam çekirdek tahmini | %95 Monte Carlo aralığı |
|---|---:|---:|
| 127 | `1,18518×10⁻¹²` | `[1,04600×10⁻¹²; 1,33773×10⁻¹²]` |
| 129 | `1,22137×10⁻¹²` | `[1,08002×10⁻¹²; 1,37609×10⁻¹²]` |

Hesap `P(G)×P(A∩W|G)` özdeşliğini kullanır. **A ve W'nin marjinal sıklıkları çarpılmadı ve bağımsızlık varsayılmadı.** G koşulunu örnekleme amacıyla kullanmak sonucu yapay olarak şişirmez: G'nin tam olasılığı nihai sonuca geri çarpılır.

### Blok başlangıcı seçimi sonucu yaratıyor mu?

Altı olası başlangıcın tamamını denedim. Faz 0 ilk altılı blokla başlar. Faz r>0 ilk r konumu kısa kenar bloğu yapar, sonra altışarlı bloklarla devam eder. Her faz ve sayım için 100 milyon G-koşullu örnek değerlendirildi.

| Faz | 127: tam çekirdek | 129: tam çekirdek |
|---:|---:|---:|
| 0 | `1,18518×10⁻¹²` | `1,22137×10⁻¹²` |
| 1 | `2,01071×10⁻¹²` | `1,81503×10⁻¹²` |
| 2 | `1,39826×10⁻¹³` | `1,44236×10⁻¹³` |
| 3 | `3,40836×10⁻¹³` | `2,60931×10⁻¹³` |
| 4 | `2,52444×10⁻¹³` | `2,16552×10⁻¹³` |
| 5 | `2,92447×10⁻¹²` | `2,87959×10⁻¹²` |

İlk faz en elverişli faz değildir. Etkinin büyüklüğü sınıra bağlı olsa da bütün fazlarda önceki tam çekirdek nokta tahmininden iki–üç mertebe daha yüksek sonuç var.

Faz önce eşit olasılıkla seçilirse doğru karışım `ΣP_r(G)P_r(AW|G)/6` olur:

| Sayım | Eşit altı-faz karışımında tam çekirdek | Muhafazakâr %95 aralık | Önceki nokta tahminine oran |
|---|---:|---:|---:|
| 127 | `1,14224×10⁻¹²` | `[9,81205×10⁻¹³; 1,32138×10⁻¹²]` | yaklaşık 1038 |
| 129 | `1,08962×10⁻¹²` | `[9,33459×10⁻¹³; 1,26369×10⁻¹²]` | yaklaşık 991 |

Toplam 1,1 milyar **ayrı** G-koşullu çekiliş yapıldı. Faz 0'ın iki veri çeşidinde örnek paylaşımı matematiksel olarak doğrulandığından aynı 100 milyon dizi iki çeşitte değerlendirildi; diğer beş fazda sayımlar ayrı çekildi. Altı faz boyunca ortak başarı sayıları 1781 ve 1721'dir. Pilotlar sonuçlara katılmadı.

## 3. Çalışmanın kendi uzunluk ölçüsünü ayrıca zorladım

Çalışmadaki ölçü:

\[
S=\sum_{n=1}^{114}n\,a_n.
\]

Daha küçük S, uzun sûrelerin daha erken konumlarda bulunmasını destekler. Gözlenen değer 127'de 209011, 129'da 209029'dur. Blok modeline ayrıca `S≤S_gözlenen` koşulunu uyguladım.

Bu hesapta hem `P(S≤S_gözlenen)` hem de P(G) tam tamsayılı hesaplardan gelir; örnekle ölçülen tek parça `P(A∩W∩{S≤S_gözlenen}|G)` oranıdır. Faz 0'da bu ortak olayı sağlayan 45 ve 40 başarı vardır. Faz 0 koşullu sonuçları yaklaşık `2,14×10⁻¹²` ve `1,91×10⁻¹²` olur.

Ancak faz 0'ı tek sonuç olarak seçmedim. Fazı önce eşit olasılıkla seçip sonra sıralılık koşulunu uygularsak:

\[
\widehat P(AGW\mid S\le S_{obs})=
\frac{\sum_r P_r(G)k_{r,ordered}/N_r}
     {\sum_r P_r(S\le S_{obs})}.
\]

| Sayım | Altı-faz karışımı ve S koşulu | Muhafazakâr %95 aralık |
|---|---:|---:|
| 127 | `9,33465×10⁻¹³` | `[7,06808×10⁻¹³; 1,22027×10⁻¹²]` |
| 129 | `8,88700×10⁻¹³` | `[6,74660×10⁻¹³; 1,16245×10⁻¹²]` |

Sıralılık koşulu eşit-faz karışımında olasılığı bir miktar düşürüyor. Buna rağmen serbest modelle arasındaki yaklaşık 800 kat fark kalıyor. Bu sonuç **yerel blok modeli üzerine eklenen S koşuluna** aittir; bütün 114! permütasyonun yalnız S eşiğiyle sınırlandırıldığı düzgün dağılımın sonucu değildir.

## 4. Komşu çiftlerde tam çekirdek: kesin sayım

Daha güçlü yerel sıra kontrolü için `(1,2), (3,4), …, (113,114)` çiftlerinin her birini bağımsız adil yazı-tura ile çevirdim. Model tanımında modüler olaylar kullanılmaz. Toplam 57 yazı-tura biti vardır. İki çift eşit değerler taşıdığı için `2⁵⁷` etiketli sonuç, `2⁵⁵=36.028.797.018.963.968` ayrı sayısal diziyi eşit olasılıkla temsil eder.

**Tam çekirdeğin bütün olasılığı kesin sayılabiliyor:**

| Sayım | Kesin olasılık | Ondalık karşılık | Yaklaşık sıklık |
|---|---|---:|---:|
| 127 | `2211747 / 36028797018963968` | `6,138831110×10⁻¹¹` | 16,29 milyarda bir |
| 129 | `70169 / 1125899906842624` | `6,232259153×10⁻¹¹` | 16,05 milyarda bir |

Yalnız 15 çift biti t-ailesini etkileyebilir. Bunların 2¹⁵ durumundan dört tanesi G'yi sağlar. W'yi etkileyebilen yedi bit daha eklendiğinde, toplam 22 etkin bitin **yalnız tek yönelimi** G∩W'yi sağlar. Kalan 35 bit için A'nın sayı ve toplam dengesi tamsayılı dinamik programlama ile sayılır: 127 için 8.846.988, 129 için 8.981.632 etiketli başarı bulunur.

Bu sonuç ilk başta yeterli bir alt olayın sınırı olarak elde edildi. Etkin bitlerin tamamı ayrıca sayılınca başka G∩W yönelimi olmadığı gösterildi; dolayısıyla burada verilen sayı bir alt sınırdan öte, **bütün tam çekirdek olasılığıdır**.

Doğrulama iki ayrı yolla yapıldı: değiştirilmemiş üretim C değerlendiricisiyle 2²² etkin-bit durumu tamamen tarandı; bağımsız bir ikiye bölüp eşleştirme algoritması 35 nötr bitin denge sayısını yeniden üretti.

127 modelinde **2.211.747 ayrı sayısal diziliş tam çekirdeği korur**. Pakette 54 konumdaki değeri değiştiren, A, G ve W'nin tümünü koruyan bir örnek var. S değeri 209071'dir. Bu örnek gözlenen S'den biraz büyüktür; `S≤S_gözlenen` koşulunu sağladığı iddia edilmez.

Modelin tamamında S, 127 için 208267–209579 aralığında kalır. Bu, uzunluk sırasının çok sıkı korunduğunu gösterir. Aynı zamanda modelin sınırıdır: her komşu çiftin mevcut değerlerini bilmek, yalnız “uzun sûreler başta” bilgisinden çok daha güçlü bir koşuldur. Yaklaşık 56 bin kat artışı bütünüyle tek bir uzunluk ölçüsüne mal etmek doğru olmaz.

## 5. Yalnız uzunluk sırasından ayarlanan daha geniş model

Bloklar mevcut sıranın çok ayrıntısını koruduğu için ayrıca **bütün dizilişlere pozitif olasılık veren** bir Mallows modeli kullandım. Merkez sırası ayet sayılarının büyükten küçüğe sıralanmasıdır. I(π), bu sıraya aykırı kesin çift sayısıdır; eşit uzunluklar terslik sayılmaz. Olasılıklar `q^I(π)` ile orantılıdır. Bu standart sıralama modelinin tanımı ve bağımsız örnekleme yapısı kaynak [3]'te bulunur.

127 verisinde gözlenen terslik sayısı 994, düzgün permütasyon beklentisi 3192'dir. `q=0,9146671727168991` yalnız beklenen terslik sayısını 994 yapmak için ayarlandı. Parametre uyumunda 19, A, G, W veya kristal katsayıları yoktur; modüler sonucu en çok artıran q araması yapılmadı. Eşit değerlerin çoklukları hem uyumda hem örneklemede hesaba katıldı.

200 milyon bağımsız üretim örneğinde:

- G **18 kez** gerçekleşti: `P(G)≈9,0×10⁻⁸`.
- Tam binom %95 aralığı `[5,33397×10⁻⁸; 1,42239×10⁻⁷]`.
- Serbest-permütasyon G frekansına göre artış yaklaşık **126 kat**; yalnız yeni Monte Carlo belirsizliğini kapsayan çarpan aralığı yaklaşık **75–199**.
- Ortalama terslik sayısı 993,99934, hedef 994 ile uyumlu.
- Ortalama S 208264,17; gözlenen 209011 modelin merkezine yakın.
- Tam çekirdek **sıfır kez** görüldü. Bu deneyden kullanılabilir bir tam çekirdek nokta olasılığı çıkmıyor.

Ek olarak bu modelde S≤209011 sağlayan 118.761.175 örneğin 13'ünde G vardı: yaklaşık `1,09463×10⁻⁷`, yani serbest G frekansının 153 katı. Bu da Mallows üzerine eklenen S koşuludur; S bölgesi üzerinde düzgün dağılım değildir.

Tam çekirdekte sıfır başarı, olasılığın sıfır olduğunu göstermez. 200 milyon deneme için tek taraflı %95 üst sınır yaklaşık `1,5×10⁻⁸` olur ve asıl `10⁻¹²`–`10⁻¹⁵` ölçeğinde ayırıcı değildir. G'nin yeni frekansını A ve W'nin marjinal frekanslarıyla çarpmak gerekçesiz bir bağımsızlık varsayımı getireceği için yapılmadı.

## 6. Genişlikler, tek takaslar ve başarısız saldırılar

Blok genişliği 2–8 için bütün başlangıçlar ve iki sayım incelendi: **70 ayrı, yozlaşmamış model**. G frekansı bunların tamamında serbest kıyas değerinden yüksek; en küçük artış yaklaşık 27 kat. Bir elemanlı bloklar diziyi hiç değiştirmediği için yalnız açıkça işaretlenmiş referans olarak saklandı, karışımlara katılmadı.

Bu tarama tek iyi genişliği seçme sorununu görünür kılar; hangi genişliğin tarihsel bakımdan doğru olduğunu belirlemez. Altılı modellerde yaklaşık 90 konumun değeri ortalama olarak değişir ve yaklaşık 10⁴⁹⋅⁵–10⁵¹⋅¹ ayrı dizi mümkündür. Geniş destek, tarihsel uygunluk için tek başına yeterli kanıt değildir.

Korunan bilginin somut bir örneği: bütün genişlik 2–8 modellerinde G'nin q=9 katsayısını üretebilen tek atama `n=6, a=165` olur. Yerel koşullama bazı modüler fırsatları da saklar. Bu bulgu, blok sonuçlarının dikkatli yorumlanmasını gerektirir; geniş Mallows modelini bu nedenle ayrıca kullandım.

Tüm tek transpozisyonlar da sayıldı. Eşit değerleri dışlayan 6384 konum çiftinin 845'inde tam çekirdek korunur: yaklaşık %13,24. Buna karşılık eşit olmayan tek bir komşu takas A'yı bozar. Komşu çiftlerde birden çok takas birlikte yapıldığında dengeler birbirini telafi edebilir. Tek adımlık kırılganlık ile bütün örnek uzayındaki nadirlik aynı soru değildir. Bu yerel oranlar küresel p-değeri değildir.

Başarısız yaklaşımı da kaydediyorum: her altılı bloğun mevcut G ve W katkısını sabitleyip yalnız A dengesini sayan yeterli-alt-olay hesapları yaklaşık `10⁻²¹` gibi çok küçük alt sınırlar verdi. Bunlar altılı modelde artışı kanıtlayacak güçte değildi. Tam G'ye koşullu örnekleme bu daraltmanın attığı büyük olasılık kütlesini geri kazandı. Olumlu sonuç diye sunulan sayılar bu zayıf sınırlar değildir.

## 7. Depodaki uzunluk savunmasının açık kalan kısmı

İncelenen `kesif16_boy_sirasi.py` yalnız A, B, C ve W'yi değerlendiriyor. G'yi ve A∩G∩W'yi ölçmüyor. Koşullu MCMC kısmında da bu iki hedefin sayacı yok. İlgili ön-kayıt metni, ulaşılamayan bileşik için marjinal katmanlar ve bağımsızlık argümanını kullanacağını belirtiyor [1–2].

Bu nedenle bazı marjinallerde 1–2 kat değişim gözlenmesi, çok daha seyrek kristalde de aynı değişim olacağını kanıtlamaz. Yeni Mallows deneyinde A yaklaşık 1,44 kat artarken G yaklaşık 126 kat artıyor. Bu, farklı katmanların uzunluk düzenine farklı cevap verebildiğine doğrudan bir örnektir.

Tam büyükten küçüğe sıralanmış tek dizinin deseni taşımaması da kısmen sıralı bir dağılımın deseni sıklaştıramayacağını göstermez. Parite ve kalanlara bağlı bir olay, sıfır karışıklıkta yokken ara derecede karışıklıkta daha sık olabilir.

İstatistiksel geçerlilik için rastgeleleştirmenin hangi değişmezlik/yer değiştirebilirlik varsayımına dayandığı önemlidir [4]. Ayrıca keşif sırasında seçilmiş sayı, eşik, katsayı deseni ve olay birleşimlerinin toplam arama alanı bilinmeden tek bir desenin olasılığı genel bir keşif p-değerine çevrilemez [5]. Burada bu bilinmeyen arama geçmişi için uydurma bir düzeltme katsayısı üretmedim.

## 8. Güven düzeyi ve savunulabilir yeni ifade

G'nin blok olasılıkları ve komşu çiftlerde tam çekirdek tamsayılı kesin hesaplardır. Altılı tam çekirdek sonuçlarında belirsizlik yalnız koşullu A∩W oranındadır. Tam G geçiş tablolarının 4404 düğümü bağımsız doğrulandı; küçük örnekler tamamen sayıldı; 3401 ayrı başarı kaydı, iki sayım değerlendirmeleri nedeniyle 3832 kez bağımsız olay hesabından geçirildi. Ayrıca 12 modelin P(W|G) değerleri başka bir kesin DP ile kontrol edildi. Kaydedilirken yarım kalan bir başarı dosyası aynı tohumla tekrar oynatılıp tamamlandı; süre dışındaki bütün sonuçlar aynı çıktı ve bu tekrar ek örnek sayılmadı.

Altı-faz karışımı için verilen muhafazakâr aralıklar, her fazın Bonferroni ile ayarlanmış Clopper–Pearson aralıklarının pozitif ağırlıklı toplamıdır. Bunlar yalnız tek tek faz aralıklarının iyimser biçimde birleştirilmesi değildir. Kaynak sayımları, başarısız denemeler, parametreler, tohumlar ve tam tanık dizileri pakettedir.

**Bugün savunabileceğim ifade:** “Belirlenmiş kristal ve tam çekirdek, serbest permütasyon altında çok seyrektir. Ancak sıklık yerel sıra koşullamasına birkaç mertebe, yalnız uzunluk sırasından ayarlanmış geniş bir modele de kristal bakımından yaklaşık iki mertebe duyarlıdır. Nadirliğin uzunluk ve sıra itirazından etkilenmediği sonucu eldeki deneylerle desteklenmiyor.”

**Henüz gösteremediğim:** Tarihsel olarak bağımsız gerekçelendirilmiş geniş bir model altında tam çekirdeğin sıradan bir sıklıkta olması; yalnız S eşiğine koşullu küresel düzgün modelde tam çekirdeğin kesin olasılığı; bütün keşif sürecine göre düzeltilmiş p-değeri. Bu eksikler aritmetik eşitlikleri geçersiz kılmaz, fakat tek başına bir olasılık sayısından köken veya nedensellik sonucu çıkarmayı sınırlar.

## Kaynaklar ve hesap izi

1. [İncelenen depo ve sabit sürüm](https://github.com/Akifcimen/quran-permutation-study/tree/a9bec42c432607350eeb4ceebbb664bca8a3aa84).
2. [Uzunluk deneyi](https://github.com/Akifcimen/quran-permutation-study/blob/a9bec42c432607350eeb4ceebbb664bca8a3aa84/kesif16_boy_sirasi.py) ve [ön-kayıt metni](https://github.com/Akifcimen/quran-permutation-study/blob/a9bec42c432607350eeb4ceebbb664bca8a3aa84/kesif16_boy_sirasi_prereg.md).
3. He, Müller ve Verstraaten (2023), [Cycles in Mallows random permutations](https://doi.org/10.1002/rsa.21169). Genel model ve örnekleme için; buradaki Kur'an verisi sonuçlarının kaynağı değildir.
4. Hemerik ve Goeman (2018), [Exact testing with random permutations](https://doi.org/10.1007/s11749-017-0571-1).
5. Gelman ve Loken (2013), [The garden of forking paths](https://sites.stat.columbia.edu/gelman/research/published/ForkingPaths.pdf).

Yeni sayısal sonuçlar: `audit_round2/conditional_core/full_core_summary.json`, `audit_round2/ordering_attack/exact_pair_core_results.json`, `audit_round2/ordering_attack/ordering_summary.json`; kesin G/W/S kontrolleri `audit_round2/exact_joint/`; bütün genişlikler `audit_round2/block_sensitivity/`.
