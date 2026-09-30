# Kur’an permütasyon çalışması: bağımsız kırma ve güçlendirme incelemesi

**Tarih:** 20 Ekim 2026  
**İncelenen sürüm:** a9bec42c432607350eeb4ceebbb664bca8a3aa84  
**Kaynak:** [Akifcimen/quran-permutation-study](https://github.com/Akifcimen/quran-permutation-study/tree/a9bec42c432607350eeb4ceebbb664bca8a3aa84)  
**Kapsam:** Ana aritmetik yapı, tekdüze permütasyon altındaki olay frekansları, düzeltilmiş nadir olay örneklemesi, alternatif diziliş modelleri ve çoklu seçim yorumları. Depodaki bütün keşif betiklerinin veya tarihsel kaynakların eksiksiz denetimi değildir. GitHub deposunun içeriği değiştirilmedi.

## 1. Sonuç ve kanıtın sınırı

Gözlenen aritmetik yapı doğrulandı. Tekdüze permütasyon modeli altında, önceden tarif edilmiş tam çekirdeğin yaklaşık \(10^{-15}\) frekans mertebesi de bağımsız bir hesaplama yöntemiyle yeniden elde edildi. Çalışmanın hesaplama temeli bu incelemede güçlendi.

Bununla birlikte, bu frekansın diziliş modeline dayanıklı olduğu veya keşif sürecinin tamamının şaşırtıcılığını ölçtüğü gösterilmiş değil. Komşu konumların ayet sayılarını koruyan alternatif bir karıştırma modelinde tam kristalin frekansı yaklaşık 740 kat, birleşik çekirdeğin frekansı yaklaşık 1160 kat artıyor. Bu ikinci sonuç doğrudan ortak önem örneklemesiyle ölçüldü. Mevcut uzunluk kontrolü yalnızca bazı marjinal olayları ölçtüğünden, bu farkı dışlamıyordu.

Üç soru birbirinden ayrılmalıdır:

1. **Aritmetik:** Gerçek veride belirtilen eşitlikler var mı? Evet.
2. **Sabit olayın frekansı:** Belirtilen rastgelelik modeli aynı tarifi ne sıklıkta üretir? Hesaplanabilir; ana modelde bağımsız ölçümler uyumlu.
3. **Keşfin genel anlamlılığı:** Araştırmacının seçebileceği tüm tarifler ve makul diziliş modelleri hesaba katılınca sonuç ne kadar şaşırtıcı? Depodaki tek sayı veya fiyat merdiveni bunu kalibre etmiyor.

Bu ayrım, bulgunun aritmetiğine ilişkin bir kuşku ile çıkarımının sınırına ilişkin kuşkuyu karıştırmayı önler. ASA’nın p-değerlerine ilişkin açıklaması da belirli bir model altındaki olasılığın, modelin doğru olma veya verinin yalnızca tesadüften doğma olasılığı olmadığını vurgular. [ASA açıklaması](https://www.amstat.org/asa/files/pdfs/p-valuestatement.pdf)

## 2. Veri ve olay tanımları

Depoda saklanan iki API çıktısı bağımsız olarak karşılaştırıldı. Sûre kimlikleri 1–114 aralığını eksiksiz kapsıyor; 114 ayet sayısının tamamı iki dosyada aynı. Standart listedeki toplam 6236; 54 sayı tek, 60 sayı çift. Üretim motorları ise dokuzuncu değeri 129 yerine 127 kullanarak toplamı 6234 yapıyor.

Buradaki “127” ve “129”, hesapta kullanılan veri varyantlarını ifade eder. Gerçek yapı her ikisinde de kontrol edildi. Bunların tarihsel/metinsel statülerinin eşdeğer olduğu varsayılmadı. Quran.com, Tevbe’yi 129 ayet olarak listeler. [Quran.com](https://quran.com/at-tawbah)

Tanımlar:

\[
t_n=n+a_n,\qquad u_n=n-a_n,
\]

\[
E=\{n:t_n\equiv0\pmod2\},\quad
T=\{n:t_n\equiv0\pmod{19}\},\quad
U=\{n:u_n\equiv0\pmod{19}\}.
\]

- **A — terazi:** \(|E|=57\) ve \(\sum_E t_n=\sum_n a_n\).
- **B — aile:** \(|T|=12\) ve \(\sum_T t_n=1444\).
- **C — iç terazi:** B’ye ek olarak, T’nin çift ve tek t yarıları altışar üye ve 722’şer toplam taşır.
- **G — tam kristal:** Dört parite hücresinin sayıları, toplamları ve katsayı içerikleri aşağıdaki tablodaki tarifi sağlar.
- **W — İki El:** \(|U|=7\), \(|T\cup U|=19\) ve \(19\mid\sum_{T\cup U}a_n\).
- **Tam çekirdek:** \(A\cap G\cap W\).

| Hücre | Sûre konumları | t/19 katsayıları | Katsayı toplamı |
|---|---|---|---:|
| t çift, n çift | 56, 70, 88 | 8, 6, 6 | 20 |
| t çift, n tek | 15, 39, 107 | 6, 6, 6 | 18 |
| t tek, n çift | 6, 42, 50 | 9, 5, 5 | 19 |
| t tek, n tek | 21, 41, 55 | 7, 5, 7 | 19 |

İki El için:

\[
U=\{14,23,48,78,80,91,104\}.
\]

T ve U ayrıktır; birleşimleri 19 konumdur ve birleşimdeki ayet toplamı \(1159=19\times61\) çıkar. Bunlar iki veri varyantında da doğrudur.

Bazı güzel görünen özellikler ayrı bilgi değildir. Örneğin A’nın toplam koşulu sağlanınca \(\sum_{E^c}t_n=\sum_n n=6555\) otomatik gelir. Ayrıca 19 tek olduğundan katsayının paritesi t’nin paritesidir. Depo, kristalin hücre içeriklerindeki bazı zorunlulukları zaten kabul ediyor; bunlar bu incelemede yeni hata diye sunulmadı.

## 3. Güçlendirme: terazi olasılığının tam hesabı

Bu incelemede A’nın olasılığı simülasyondan çıkarılıp tam kombinatorik hesaba dönüştürüldü.

| Veri varyantı | Tam \(P(A)\) |
|---|---:|
| 127; toplam 6234 | 0,00031371149836905516 |
| Standart 129; toplam 6236 | 0,0003140653823213589 |

Dolayısıyla deponun 127 için verdiği \(3{,}137\times10^{-4}\) değeri doğrulanıyor. Tam sonuçların pay ve paydaları deney paketindeki JSON dosyasında bulunuyor; rastgele sayı üreteci veya Monte Carlo belirsizliği yok.

### Türetme

Tek ayet sayılarından x tanesi tek sûre konumlarına yerleşsin. Çift konumlarda çift ayet sayısı taşıyanların sayısı \(60-(57-x)=3+x\) olur. Böylece:

\[
|E|=x+(3+x)=3+2x.
\]

57 üyeli terazi için x=27 gerekir:

\[
P(|E|=57)=
\frac{\binom{57}{27}^{2}}{\binom{114}{54}}
=0{,}14867994292027226.
\]

Toplam koşulu sadeleşir:

\[
\sum_{n\in E}(n+a_n)=\sum_n a_n
\quad\Longleftrightarrow\quad
\sum_{n\in E}n=\sum_{n\notin E}a_n.
\]

57/57 koşulu altında dört altküme seçilir: E’deki 27 tek konum ve 30 çift konum; E dışında kalan 27 tek ayet sayısı ve 30 çift ayet sayısı. Tekrarlanan değerler etiketli örnekler olarak sayılır. Eşit toplam veren altküme dörtlülerinin sayısı F olsun. O zaman:

\[
P(A)=\frac{F}{
\binom{114}{54}\binom{54}{27}\binom{60}{30}}.
\]

F, \(\prod_v(1+yx^v)\) türü üreteç fonksiyonlarının katsayıları alınarak tam tamsayı aritmetiğiyle hesaplandı. Büyük tamsayı paketlemesinde taban, bütün katsayı kütlesinden büyük seçildiği için taşıma hatası oluşmaz.

Ayrı bir kontrol olarak, dört küçük veri listesi üzerindeki bütün etiketli permütasyonlar tek tek sayıldı. Sonuçlar formülle birebir uyuştu: 6/24, 88/720, 1600/40320 ve 3136/40320. Tekrarlanan sayılar içeren bir örnek de bu kontrole dahildir.

### 127/129 ayrımının önemi

Gerçek desenin korunması, sıfır dağılımlarının birebir aynı olması anlamına gelmez. Tam hesapta:

\[
P_{129}(A)/P_{127}(A)=1{,}0011280554080535.
\]

Fark yalnızca yaklaşık **%0,113**; mertebeyi değiştirmiyor. Yine de frekansın kelimesi kelimesine sayım varyantından bağımsız olmadığı kanıtlanmış oluyor.

127 değerinin rastgele permütasyonda düştüğü konum J olsun. 127’yi 129 yapınca terazi artığı, J tekse değişmez; J çiftse iki azalır. Gerçek konum 9 tek olduğu için gerçek yapı korunur. Deney paketinde verilen bir karşı örnekte J=64’tür: 127 listesinde A doğrudur, 129 listesinde A yanlıştır. Bu örnek sadece A ile ilgilidir; tam çekirdek farkına ilişkin bir karşı örnek diye kullanılmadı.

## 4. Güçlendirme: nadir olayı bağımsız önem örneklemesiyle ölçmek

Depodaki eski çokluk ağırlığı hatası zaten düzeltilmiş. Düzeltilmiş formülde aynı ayet sayısından birden fazla kopya bulunmasının etkisi vardır:

\[
L(S)=\prod_v\frac{m_v!}{(m_v-k_v)!}.
\]

Bu inceleme, eski hatayı güncel kodda varmış gibi tekrarlamadı. Bunun yerine bağımsız bir örnekleyici kuruldu.

### 4.1. Kalan konumların tam normalizasyonu

Önce 12 kristal üyesi seçilir. Kalan 102 konuma yerleşecek sayıların yeni bir \(t\equiv0\pmod{19}\) üyesi oluşturmaması gerekir. Bu kaçınma olasılığı, mod 19 artık sınıflarının oluşturduğu yasak eşleştirme tahtası üzerinden tam hesaplanabilir.

Her artık sınıfındaki yasak dikdörtgen için kale polinomu:

\[
R_r(z)=\sum_k\binom{r_r}{k}\binom{c_r}{k}k!z^k.
\]

Bütün sınıflar için polinomlar çarpılır. \(R_k\) katsayılarıyla, yasak eşleşmeden kaçınan etiketli permütasyon sayısı:

\[
\sum_k(-1)^kR_k(102-k)!.
\]

Bu toplam tam tamsayılarla alındı; büyük zıt işaretli kayan nokta sayıları çıkarılmadı. 1260 farklı permütasyonu olan küçük bir örnekte, tam sayım ile polinom sonucu birebir uyuştu.

Böylece kalan 102 konumu çok kez karıştırarak tahmin edilen bir ara olasılık tam olarak entegre edildi. Dıştaki kristal ataması hâlâ rastgeledir; bu yöntem bütün G olasılığını tam sayım yaptığını iddia etmez.

### 4.2. Doğrudan ortak olay kestirimi

İkinci bağımsız yöntemde, kalan konumlara yalnızca izin verilen ayet kopyaları arasından seçim yapıldı. Bu seçimin yanlılığı doğru olasılık ağırlığıyla giderildi. Çıkmaza giren her girişim paydada sıfır katkıyla tutuldu. Ortak A ve W olayı aynı tamamlanmış dizilişte değerlendirildi; marjinal olasılıklar veya dışarıdan alınmış bir κ çarpılmadı.

| Olay ve veri | Önem örnekleme girişimi | Pozitif ortak olay sayısı | Bağımsız frekans kestirimi | Yaklaşık %95 Monte Carlo aralığı |
|---|---:|---:|---:|---:|
| Tam çekirdek; 127 | 20 milyon | 23 | \(9{,}05\times10^{-16}\) | \([5{,}08;\,13{,}03]\times10^{-16}\) |
| Tam çekirdek; standart 129 | 20 milyon | 24 | \(8{,}29\times10^{-16}\) | \([4{,}78;\,11{,}79]\times10^{-16}\) |
| 31 yapılık geniş kristal sınıfı ile çekirdek; 127 | 40 milyon | 54 | \(3{,}24\times10^{-14}\) | \([2{,}08;\,4{,}41]\times10^{-14}\) |

**Aralıkların anlamı:** Bunlar bağımsız önem ağırlıklı gözlemlerin varyansından türetilen yaklaşık normal aralıklardır. Model seçimi ve tarif seçimi belirsizliğini kapsamaz. Pozitif ortak olayların etkin örnek büyüklükleri sırasıyla yaklaşık 20, 21 ve 30’dur. Milyonlarca kristal örneği bulunması, en nadir ortak olayın aynı hassasiyetle ölçüldüğü anlamına gelmez.

127 için yalnız kristal olasılığı \(7{,}0821\times10^{-10}\), 129 için \(7{,}0730\times10^{-10}\) bulundu. Bunlar doğrudan GPU sayımındaki yaklaşık \(7{,}14\times10^{-10}\) ile GPU sayım hatası hesaba katıldığında uyumludur.

Sonuç olarak, bu inceleme **127 veya 129 altında ana \(10^{-15}\) mertebesini çürütemedi; bağımsız olarak destekledi**. İki tam çekirdek kestirimi arasındaki fark, Monte Carlo belirsizliği içinde kalıyor.

### 4.3. Sınıf düzeyindeki eksik varsayımın kaldırılması

Depodaki kesif21b kodunda κ=1,6 sabit kalmış; README ve güncellenmiş notlar κ=1,14 diyor. Ayrıca tek kristal üzerinde ölçülen etkileşim katsayısı geniş 31 yapılık sınıfa taşınıyor. Bu aktarım, kendi başına doğrulanmış bir ortak olay ölçümü değildir.

Yeni bağımsız 40 milyonluk deneyde ortak olay ağırlıkları doğrudan toplandı. Yaklaşık \(3{,}24\times10^{-14}\) sonucu, depodaki yaklaşık \(4\times10^{-14}\) mertebesini destekliyor. Böylece geniş sınıf için κ taşıma gereksinimi kaldırılabilecek somut bir yeniden üretim elde edildi.

31 yapılık sınıfın G’ye göre kristal frekans çarpanı yaklaşık 33,6’dır. Sınıf yalnızca bandın konumunu/genişliğini serbest bırakır; 12 üye, toplam 76 ve hücre toplamları gibi diğer sabitleri serbest bırakmaz. Dolayısıyla bu oran bütün keşif serbestliğinin fiyatı değildir.

## 5. Kırılan sağlamlık savunması: farklı diziliş modelleri

Deponun ana sıfır modeli, 114 ayet sayısını bütün konumlara eşit olasılıkla dağıtır. Alternatif incelemede sekiz model kullanıldı: tam karıştırma; ilk konum sabit; 5, 10, 20, 30 konumluk bloklar; beş ayet uzunluğu sınıfı; yirmilik uzunluk sırası grupları.

İlk taramada her model için 10 milyon doğrudan, bağımsız karıştırma yapıldı. Sonra dört belirlenmiş takip koşusunda 100’er milyon karıştırma yapıldı. Toplam **480 milyon doğrudan deneme** vardır. Farklı modellerin paydaları tek bir olasılık hesabı için birleştirilmedi. Takip modellerinin ilk taramadan sonra seçildiği açıkça kaydedildi.

### 5.1. Beşli blok modeli

Konumlar 1–5, 6–10, …, 111–114 şeklinde bölündü. Her blok kendi ayet sayısı çokluğunu korudu; o blok içindeki yerleşimler tekdüze karıştırıldı. Bu model, kitap genelindeki sıralamayı ve yerel uzunluk profilini tam karıştırmaya göre daha çok korur.

Bu bir tarihsel derleme modeli olarak kanıtlanmış değildir. Araştırmacının yayımlanmış sonucu bilerek seçtiği bir duyarlılık denemesidir. Bununla birlikte yalnızca birkaç yakın komşudan oluşmaz: yaklaşık \(1{,}035\times10^{45}\) farklı ayet sayısı dizilişini kapsar.

### 5.2. Aynı girdide doğrudan karşılaştırma

Aşağıdaki karşılaştırmalar 127 girdisini kullanır. İlk sütun, iki GPU dosyasının son kümülatif satırlarının toplamından yeniden hesaplandı.

| Olay | Tam karıştırma | Beşli blok; 100 milyon doğrudan deneme | Çarpan |
|---|---:|---:|---:|
| A — genel terazi | \(3{,}13713\times10^{-4}\) | \(5{,}4513\times10^{-4}\) | 1,738 |
| B — 12 üyeli, toplamı 1444 olan aile | \(3{,}15884\times10^{-4}\) | \(2{,}8923\times10^{-4}\) | 0,916 |
| C — iç terazi | \(1{,}85214\times10^{-5}\) | \(1{,}9090\times10^{-5}\) | 1,031 |
| W — İki El | \(5{,}50639\times10^{-5}\) | \(1{,}0901\times10^{-4}\) | 1,980 |
| **G — tam kristal** | **\(7{,}14208\times10^{-10}\)** | **\(5{,}30\times10^{-7}\)** | **742** |

Kristal 100 milyon denemede 53 kez oluştu. İki taraflı tam binom Monte Carlo %95 aralığı \([3{,}970;\,6{,}933]\times10^{-7}\). Standart 129 girdisinde bağımsız takip koşusu 57/100 milyon verdi.

Diğer takip koşuları:

| Model; 127 girdi | G isabeti / deneme | G frekansı | Tam binom %95 aralığı |
|---|---:|---:|---:|
| Onlu konum blokları | 5 / 100 milyon | \(5{,}00\times10^{-8}\) | \([1{,}623;\,11{,}668]\times10^{-8}\) |
| Ayet uzunluğu sınıfları | 2 / 100 milyon | \(2{,}00\times10^{-8}\) | \([0{,}242;\,7{,}225]\times10^{-8}\) |

İki isabetli satırın belirsizliği geniştir. İlk taramanın daha az çarpıcı sonuçları da deney paketinde tutuldu.

### 5.3. Neden mevcut uzunluk kontrolü yeterli değildi?

kesif16, \(S=\sum_n n a_n\) sıralılık istatistiğine koşullu bir model kullanıyor. Bu ciddi bir başlangıçtır. Fakat kod yalnızca A, B, C ve W’yi değerlendiriyor; G’yi veya \(A\cap G\cap W\)’yi ölçmüyor. Ön kayıt metni de birleşik olayı marjinaller ve bağımsızlık yaklaşımıyla ele aldığını belirtiyor.

Yeni deneyde C hemen hemen sabitken G yaklaşık 742 kat artıyor. Bu, “erken katmanların sıklığı çok değişmedi, dolayısıyla tam kristalin nadirliği de değişmez” çıkarımının gerçek veri üzerinde geçersiz olduğunu gösterir.

Olası açıklama şudur: G yalnızca artık sınıflarına ilişkin değildir; \(n+a_n\) büyüklüklerinin belirli dar bir katsayı bandına düşmesini de ister. Yerel uzunluk-konum ilişkisini korumak, bu büyüklükleri etkiler. Bu mekanizma bir çıkarımdır; tarihsel diziliş sürecini açıklayan, bağımsız veriye uydurulmuş bir model değildir.

Permütasyonun hangi konumlar arasında yapılabileceği, çıkarımın bir varsayımıdır. Değiştirilebilirlik koşulu hakkındaki metodolojik tartışma için: [Hardin ve arkadaşları](https://arxiv.org/abs/2406.07756).

### 5.4. Birleşik çekirdek için takip

Beşli blok modeli için aynı tam çekirdek, bağımsız bir önem örnekleyicisiyle doğrudan ölçüldü. 127 verisinde 500 milyon bağımsız öneri girişimi yapıldı. Çıkmaza giren 448.634.028 girişim paydada sıfır katkıyla tutuldu. Pozitif ortak olay sayısı 116; olayın ağırlıklarına göre etkin örnek büyüklüğü 51,87 oldu.

\[
\widehat P_{\mathrm{blok5}}(A\cap G\cap W)
=1{,}051517334\times10^{-12}.
\]

Önem örnekleme standart hatası \(1{,}4600\times10^{-13}\); normal yaklaşıma dayalı %95 Monte Carlo aralığı:

\[
[7{,}654\times10^{-13};\;1{,}338\times10^{-12}].
\]

Bu nokta kestirimi, aynı 127 girdisindeki bağımsız tam karıştırma kestiriminin yaklaşık **1162 katıdır**. Oranın payı ve paydası kestirimdir; 1162 kesin bir sabit değildir. Depodaki yuvarlanmış \(1{,}1\times10^{-15}\) değeriyle karşılaştırılırsa yaklaşık 956 kat çıkar. Her iki karşılaştırma da üç büyüklük mertebesine yakın bir model duyarlılığı gösterir. Olay, bu alternatif modelde de çok seyrektir.

**Örnekleyiciden bağımsız tam kontrol:** Her beşli blokta en çok 120 farklı ayet sayısı yerleşimi tek tek sayıldı. Kristalin katsayı/parite sayıları bir tamsayı dinamik programıyla bloklar arasında birleştirildi. Hem 127 hem 129 verisi için:

\[
P_{\mathrm{blok5}}(G)
=\frac{532234302397}{1012500000000000000}
=5{,}256635085402469\times10^{-7}.
\]

Bu G değeri tamdır; Monte Carlo belirsizliği yoktur. İki GPU dosyasının G frekansının yaklaşık 736 katı; daha hassas bağımsız tam karıştırma G kestiriminin yaklaşık 742 katıdır. Önceki tabloda verilen 53/100 milyon doğrudan sonuçla da uyumludur.

500 milyonluk önem örnekleyicisinin G kestirimi, bu tam sonucun yalnızca 0,297 standart hata uzağına düştü. Ayrıca iki farklı küçük blok örneğinde bütün 144 ve 288 diziliş üzerinde öneri yolları ve önem ağırlıkları tam sayılar/kesirlerle kontrol edildi; marjinal ve ortak olaylar birebir uyuştu.

Böylece model duyarlılığı artık yalnız G marjinaline ilişkin değildir; **tam A∩G∩W olayı da doğrudan ölçülmüştür**. Bu, beşli blok modelinin tek doğru tarihsel model olduğunu veya arama seçimine göre düzeltilmiş genel bir p-değeri elde edildiğini göstermez. Yaklaşık aralık ağırlıklı Monte Carlo belirsizliğidir; model seçimi belirsizliğini içermez.

## 6. Özgüllük turnuvasındaki matematiksel kısıtlar

### 6.1. Sekiz koşulun biri tam eşleşmede gereksiz

Katı profilde:

- S2, aile toplamının \(m^2\)’ye bölünmesini;
- S3, iki parite yarısının üye ve toplam eşitliğini;
- S4, her iki yarı toplamının ayrı ayrı \(m^2\)’ye bölünmesini ister.

S4 zaten S2’yi gerektirir. m tekken S2 ve S3 de S4’ü gerektirir: \(m^2\mid2s\) ise \(m^2\mid s\). m çiftken S3 zaten olanaksızdır; m’nin bütün katları çift olacağı için boş olmayan aile iki t-parite yarısına bölünemez.

Bu nedenle tam olayda:

\[
S2\cap S3\cap S4=S2\cap S3.
\]

“8/8”, sekiz indirgenemez bilgi parçası demek değildir. Doğru simüle edilmiş tam eşleşme frekansı bundan dolayı otomatik olarak yanlış olmaz; sayısal bir düzeltme çarpanı icat edilmemelidir. Ancak puan farkının retorik yorumunda bağımlılık açıklanmalıdır.

### 6.2. 38 modülün 26’sı baştan eleniyor

S1, \(k\ge228/m\) gerektirir. S6, birleşim büyüklüğünü m’ye eşitlediğinden \(k\le m\) gerektirir. Dolayısıyla:

\[
m^2\ge228.
\]

S3 ayrıca m’nin tek olmasını gerektirir. 3–40 aralığındaki yalnızca şu 12 değer gerekli koşulları geçer:

17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39.

“Yalnız 19 tam puan alıyor” gerçek veri için doğrudur. Bunun 37 eşit derecede uygun rakibe karşı zafer gibi sunulması doğru değildir. Bütün taramayı gerçekten yapan bir simülasyon, olanaksız modülleri zaten hesaba katmış olur.

### 6.3. KJV’nin 1189 bölümlük kontrolünde tam puan olanaksız

Genel N için S1 ve S6 birlikte \(N\le m^2/2\) gerektirir. KJV bölüm listesinde N=1189, taramada m≤40’tır:

\[
1189>40^2/2=800.
\]

Dolayısıyla bu kontrol, aynı tarama aralığında hiçbir olası diziliş veya pozitif bölüm sayısı listesiyle katı 8/8 alamaz. Başarısızlığı tam eşleşmenin özgüllüğünü destekleyen bir negatif kontrol değildir. Bu sonuç Mezmurlar gibi daha uygun boyuttaki bütün kontrolleri geçersiz kılmaz; belirli bir kontrolün mantıksal sınırını gösterir.

Bir nüzul sırası veya birkaç KJV özeti, tarifin bu dizilişlerde çıkmadığını gösterebilir. Nadir kuyrukların gerçek yapılandırılmış metinlerle tekdüze permütasyonlarda aynı olduğunu göstermez. Ayrıca KJV’nin kitap, bölüm ve Mezmurlar listeleri aynı kaynaktan gelir; üç bağımsız tarihsel metin gibi yorumlanmamalıdır.

## 7. Fiyat merdiveni niçin genel p-değeri olmuyor?

Depo bu konuda önceki sürümlere göre daha dikkatli: artık hiçbir basamağın kalibre edilmiş küresel p-değeri olmadığını söylüyor. Bu kabul korunmalıdır.

### Sabit tarif ile arama prosedürü farklıdır

Bir tarif gerçek veri görüldükten sonra seçilmişse, sonradan yapılacak milyarlarca karıştırma o tarifin olasılığını çok hassas ölçebilir. Tarifin seçilmiş olmasının maliyetini kendiliğinden ölçmez.

Sorun yalnızca kaç betik çalıştırıldığı değildir. Aynı veride farklı bir sayı, toplam, fark, bölme, hücre, bant, güzel sayı veya bileşik yapı görülse hangi tarifin seçileceği de önemlidir. Gelman ve Loken, veri sonucuna bağlı analiz seçimlerinin yalnızca tek analiz çalıştırılmış olsa bile çoklu karşılaştırma sorunu oluşturabileceğini açıklar. [Birincil metin](https://stat.columbia.edu/~gelman/research/unpublished/p_hacking.pdf)

Bu nedenle yaklaşık 75 “istatistik ailesi” için Bonferroni çarpanı, ancak o aileler ve bütün seçim prosedürü önceden eksiksiz tanımlı ve ilgili testler geçerliyse anlamlı bir sınır verir. 75 sayısı, veriyle birlikte şekillenmiş bütün tariflerin sayısı değildir.

### Bir olaydaki çarpan başka olaya taşınamaz

Gevşetilmiş S1–S8 profili için ölçülen çatal çarpanını, tam G kristali ve A ile oluşturulan çekirdeğe çarpmak, ortak davranışın aynı kaldığını varsayar. Benzer biçimde tek kristaldeki κ geniş kristal sınıfına kendiliğinden taşınamaz. Bu inceleme ikinci sorunu doğrudan örnekleyerek kısmen çözdü; bütün merdivenin transfer varsayımlarını çözmedi.

Doğrusal olmayan biçimler için verilen yaklaşık 2,6 çarpanı da tam genişletilmiş aramanın sentetik veriler üzerinde ortak dağılımını ölçmenin yerine geçmez. Aday sayısı ile etkin bağımsız arama maliyeti aynı şey değildir.

### Yeniden üretim materyali eksik kalan iddialar

README ve KESIF’in ikinci kırmızı takım turu anlatısında geçen 33 sınıf yüklemli “kirmizi21b” tipiklik testi, 300 milyonluk koşullu katı profil ölçümü ve bunlara dayanan bazı çatal fiyatlarının tam betik ve ham çıktıları incelenen sürümde bulunamadı. Bunlar bağımsız olarak yeniden üretilebilen sonuçlardan ayrı işaretlenmelidir.

Bu durum söz konusu sayıların yanlış olduğunun kanıtı değildir; bu inceleme için doğrulanabilirlik eksiğidir. Sayım, araç kodu, tohumlar ve tam çıktıların eklenmesi somut çözüm olur.

### Sayım okulu kapsamı

Aritmetik yapı Kûfe sayımına özgü olarak sunulmalı. Depodaki Medine/Warş kontrolünde aynı yapı tutmuyor; bu zaten açıkça raporlanmış. Klasik kaynaklardaki yedi ayet numaralandırma sistemi, tek bir değişmez sayım geleneği bulunduğu varsayımının uygun olmadığını gösterir. [Farrin, JIQSA](https://lockwoodonlinejournals.com/index.php/jiqsa/article/view/456)

Yedi okul sayısını bir frekansla çarpmak, bütün keşif için otomatik geçerli bir p-değeri üretmez. Her okula uygulanacak aynı, kalibre edilmiş test ve seçim kuralı ayrıca tanımlanmalıdır.

## 8. Araştırmayı en çok güçlendirecek somut değişiklikler

1. **Standart 129 listesini birincil analiz yapın.** Bu inceleme, asıl çekirdek mertebesinin standart listede de korunduğunu destekliyor. 127 ve diğer sayım varyantlarını açık duyarlılık kolları olarak raporlayın. Girdiyi ve her olayın hedef toplamını aynı yapılandırmadan üretin.

2. **Terazi basamağında tam hesabı yayımlayın.** Olay tanımı, üreteç fonksiyonu, tamsayı pay/payda ve küçük tam sayım kontrolleri verilsin. Simülasyon, bu basamağın esas kanıtı olmaktan çıkarılabilir.

3. **Nadir ortak olayları doğrudan ağırlıklı ölçün.** Yeni tamamlama örnekleyicisi ve tam kaçınma normalizasyonu, pahalı kör ret denemelerini azaltıyor. Genel ESS’in yanında her nadir olayın pozitif isabeti, etkin örnek büyüklüğü ve varyansı ayrı raporlansın.

4. **Diziliş modellerini önceden ilan edilmiş bir panelde karşılaştırın.** Tam karıştırma, geniş konum blokları, uzunluk sınıfları ve mevcut S-koşullu model aynı G ve tam çekirdek tanımıyla ölçülsün. Bütün modellerde yalnız marjinallerin değil ortak olayın sonucu verilsin. Model seçimi için tarihsel gerekçeler hesaplardan ayrı yazılsın.

5. **Arama dilini açıkça tanımlayın.** Kullanılabilecek işlemler, modüller, güzel sayı aileleri, bölme eksenleri ve birleşim kuralları makinece okunabilir bir listede tanımlansın. Her sentetik diziye aynı “en iyi hikâyeyi bulma” hakkı verilsin. Seçilmiş gerçek tarifi arayan bir turnuva, tarif seçme prosedürünü yeniden çalıştıran turnuvadan ayrılmalı.

6. **İsteğe bağlı tarif uzunluğu yaklaşımını gerçekten uygulayın.** Olaylara önceden sabitlenmiş bir önek kodu uzunluğu L veya ağırlık w verilecekse, kurallar gözlenen sayıları ucuzlatacak biçimde sonradan ayarlanmamalı. Geçerli sabit testler ve toplamı en çok 1 olan önceden belirlenmiş ağırlıklar altında ağırlıklı çoklu test sınırları kurulabilir. Bunun hangi arama sınıfını kapsadığı açık olmalıdır.

7. **Negatif kontrolleri baştan matematiksel uygunluk açısından denetleyin.** N’ye göre tam puanın ulaşılabilir olduğu korpuslar seçilsin; aynı kaynağın farklı özetleri bağımsız örnekler gibi sayılmasın. Korpus seçiminden önce ölçeklendirme kuralları dondurulsun.

8. **Tek kaynaklı bir sonuç tablosu üretin.** Betikler, girdiler, commit, tohumlar, sayaçlar, tahmin türü ve güven aralığı çıktıları tek manifestte tutulsun. README elle yazılmış κ veya eski oranlardan türetilmesin. “Ölçüldü”, “yaklaşıklıkla aktarıldı” ve “henüz yeniden üretilemiyor” etiketleri ayrı olsun.

Bu program, geçmişteki belgelenmemiş bütün seçimleri geriye dönük yok etmez. Açıkça tanımlanmış modeller ve arama sınıfları için daha denetlenebilir yeni sonuçlar üretir.

## 9. Hesaplama kapsamı ve güvence

Arşiv dosyalarının son kümülatif satırları yeniden toplandı:

- CPU + GPU toplamı: 24.086.000.129.040 deneme.
- Tam kristal G: 17.204 isabet.
- A ve G birlikte: 5 isabet.
- Tam A, G, W birlikte: arşivde 0 doğrudan isabet.

Yalnız iki GPU dosyasında toplam 24.000.000.129.024 deneme, 17.141 G ve 5 A∩G vardır. 5 isabetin oranı \(2{,}0833\times10^{-13}\); Poisson yaklaşımına dayalı %95 aralık \([6{,}765\times10^{-14};\,4{,}862\times10^{-13}]\). Bu dosyalar, 24 trilyonluk koşuyu bu oturumda yeniden yaptığımız anlamına gelmez.

Sıfır tam çekirdek isabeti, sıfır olasılık demek değildir. Bu GPU büyüklüğünde tek taraflı %95 üst sınır yaklaşık \(1{,}25\times10^{-13}\) olur; \(10^{-15}\) frekansını tek başına hassas ölçemez. Çok daha küçük frekansın yeniden üretimi ayrı önem örnekleme deneylerine dayanır. Monte Carlo testi p-değerleri ile olay frekansı kestirimi de ayrılmalıdır; standart Monte Carlo testlerinde sıfır p-değeri sorununa ilişkin kaynak: [Phipson ve Smyth](https://pubmed.ncbi.nlm.nih.gov/21044043/).

Bağımsız doğrudan simülatörün olay tanımları, gerçek veri, 5000 tam karıştırma, 5000 beşli blok karıştırması ve 6441 tek değiş tokuş dahil 16.442 dizide depo motoruyla karşılaştırıldı. İlgili 12 sayacın tamamı uyuştu. Depo C motorunun gerçek veri öz sınaması da bütün bayraklarda 1 verdi.

Kaynak dosya SHA-256 değerleri, arşiv sayaçlarının dosya bazında dökümü, bütün deney tohumları ve kodlar deney paketindedir. Bunlar inceleme izidir; tarihsel hipotez veya keşif genel anlamlılığı için bağımsız kanıt yerine geçmez.

## 10. Kapsamlı inceleme özeti

**Değerlendirme: Yorumları düzeltilerek paylaşılabilir; genel anlamlılık ve model sağlamlığı iddiaları revizyon gerektirir.**  
**Kapsam: Kısmi.** Ana olaylar ve belirtilen yöntemler incelendi; bütün yan keşifler veya tarihsel veri gelenekleri eksiksiz incelenmedi. Asıl depo değiştirilmedi; ayrı doğrulama kodları ve öneriler hazırlandı.

Aşağıdaki paydalar yalnız bu raporda açıkça tanımlanan bileşenleri sayar; bütün deponun başarı yüzdesi değildir. 0/N, o grupta gösterilmiş bir hata bulunmadığını ifade eder.

### Sunum ve yeniden üretilebilirlik

| Kategori | Gözlenen sorun / kapsam | Değerlendirme |
|---|---:|---|
| Ana olay tanımlarının açıklığı | 1 / 3 | A, G, W tanımları somut; sayım varyantı ile frekans bağımsızlığı aynı anlamda kullanılmamalı. |
| Ana sonuçların yeniden üretilebilirliği | 1 / 4 | A, G ve ana koşullu çekirdek yeniden üretildi; bazı ikinci tur arama fiyatlamalarının tam materyali eksik. |
| Görsel ve arayüz tutarlılığı | Uygulanamaz | Etkileşimli pano veya kullanıcı arayüzü incelenmedi. |

### Matematik ve çıkarım

| Kategori | Gözlenen sorun / kapsam | Değerlendirme |
|---|---:|---|
| Aritmetik A, G, W | 0 / 3 | Gerçek veride iki varyantta da doğrulandı. |
| A, G, tam çekirdek, geniş sınıf frekansları | 1 / 4 | Deponun geniş sınıf kodu eski κ’yı taşıyor; yeni bağımsız ortak ölçüm aynı mertebeyi destekliyor. |
| Özgüllük ve negatif kontrol yorumları | 2 / 3 | Modül ulaşılabilirliği/tekrarlı koşul ve 1189 bölümlük katı kontrolün olanaksızlığı gösterildi. |
| Diziliş modeli ve arama genellemesi | 2 / 2 | Marjinal kontrol tam çekirdeği kapsamaz; fiyat merdiveni küresel anlamlılığı kalibre etmez. |

## Dosyalar ve başlıca kaynaklar

Deney paketi; tam parite hesabını, aritmetik sertifikayı, tam kaçınma polinomunu, tamamlama önem örnekleyicilerini, doğrudan alternatif model simülatörünü, bütün önemli JSON sonuçlarını ve arşiv hesap özetini içerir. Tekrar çalıştırma adımları paketin README dosyasındadır.

Depoda incelenen başlıca dosyalar: README.md, KESIF.md, kesif4_motor.c, kesif_cuda.cu, kesif16_boy_sirasi.py ve ön kaydı, kesif18c_kosullu_cekirdek_cokluk.py, kesif21b_sinif_kristal_cokluk.py ve ilgili ön kayıt, kesif12_karsi_korpus.py, kesif19_catallanma.py, kesif20_tipiklik.py, kesif22–24 dosyaları ve kaydedilmiş sonuçlar.

Dış yöntem kaynakları:

- [ASA: p-değerlerinin yorumu](https://www.amstat.org/asa/files/pdfs/p-valuestatement.pdf).
- [Gelman ve Loken: veri sonucuna bağlı analiz yolları](https://stat.columbia.edu/~gelman/research/unpublished/p_hacking.pdf).
- [Hardin ve arkadaşları: permütasyon çıkarımında değiştirilebilirlik](https://arxiv.org/abs/2406.07756).
- [Phipson ve Smyth: Monte Carlo permütasyon p-değerleri](https://pubmed.ncbi.nlm.nih.gov/21044043/).
- [Farrin: yedi ayet numaralandırma sistemi](https://lockwoodonlinejournals.com/index.php/jiqsa/article/view/456).

