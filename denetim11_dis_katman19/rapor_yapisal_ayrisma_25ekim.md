# Uzunluk düzeninin ötesinde yapısal ayrışma
## 127 nesnesinin bağlantılı aritmetik ve matris yapısı için yeni karşılaştırma

**Tarih:** 25 Ekim 2026  
**İncelenen girdi:** 114 sûre numarası ve ayet sayısı; eşlenik 129 ve 127 seçenekleri.  
**Yeni çalışma:** İki modelde ayrı ayrı 200.000 yeni kontrol; arşivdeki 1.781 tam çekirdek örneğinin yeni matris denetimi.  
**Sonuçların niteliği:** Belirtilen sıfır modelleri ve yapısal tarif dili içinde keşif amaçlı, yeniden üretilebilir karşılaştırma. Bütün tarihsel araştırmacı tercihleri için nihai anlamlılık hesabı değildir.

## 1. Sonuç

**Nesne, yalnız belirli bir uzun diziliş olduğu için seyrek görünmüyor. Uzunluk düzeni korunmuş kontrollere farklı modüller, iki aritmetik yön, iki sayım seçeneği ve farklı matris sunumları deneme hakkı verildiğinde de, bağlantılı yapısı açık biçimde ayrışıyor.**

Yedi genel yapı koşulu tanımladım. Gözlenen 127 nesnesi yedisini birden sağlıyor. Aynı terslik sayısını tam koruyan 200.000 kontrolün de, yerel altılı blokları koruyan 200.000 kontrolün de en yüksek skoru **4** oldu. Hiçbir kontrol 5, 6 veya 7 koşulu birlikte sağlayamadı.

Bu deney, hedefte görülen 19, 12, 3, 1444, 361, 1159 veya 5–9 katsayı bandını kontrol olayına tek tek yazmıyor. Rakipler kendi modüllerini, aile yönlerini ve matris yönlerini seçiyorlar. Buna rağmen gözlenen nesnenin bütünlüğüne ulaşamıyorlar.

İkinci bir bulgu, önceki araştırmada açık bırakılan matris sorusuna kısmi ama doğrudan bir cevap veriyor: Arşivdeki **terazi–tam kristal–İki El** çekirdeğini zaten sağlayan 1.781 örneğin yalnız **4** tanesi doğal sunumda tam matris mimarisini koruyor. Matrislerde 16 satır/sütun sunumundan en iyisini seçme hakkı tanınınca sayı **9** oluyor. Böylece matris katmanı yalnız sözel bir ilave veya çekirdeğin cebirsel tekrarı olarak kalmıyor; bu kontrollü örneklerde ölçülebilir ek seçicilik gösteriyor.

**Bu sonuçlar hangi kapsamda güçlü?** Tanımlı yapı sınıfında ve kullanılan uzunluk-korumalı modellerde. Yedi koşulu seçmek de araştırmacı tercihidir; dil hedef görüldükten sonra kurulmuştur. Yeni kontroller görülmeden dondurulması, geçmişteki bütün alternatif dilleri ve keşif yollarını fiyatlandırmaz. Bu sınırlama deneyin gösterdiği ayrışmayı ortadan kaldırmaz, fakat onu evrensel veya nedensel bir hükme dönüştürmeyi engeller.

## 2. Veri ve hesapların kaynağı

Bu oturumda canlı GitHub içeriğine erişilemedi. Bu yüzden deponun güncel başına ilişkin bir inceleme yaptığımı söylemiyorum. Kullanıcının Kütüphanesinde bulunan önceki rapor ve deney arşivlerinden özgün veri, örnekleyici kaynakları ve ham tanıklar geri alındı.

Veri, 20 Ekim arşivindeki `quran_meta.json` dosyasıdır. Arşivin `SOURCE.md` belgesi kaynak sürümünü `a9bec42c432607350eeb4ceebbb664bca8a3aa84` olarak kaydediyor. Verinin SHA-256 özeti:

```
3755ee97f87e9f162c3dc5bb88f740c6d3a0eb3171cb1ba5d26608abbb669d35
```

Bu özet, `51fe1c0c36f2d481369480800aa84582e0a099e8` sürümüne ilişkin 23 Ekim raporunda kullanılan veriyle eşleşiyor. Toplam, uzunluk, ayet sayıları ve önceki yapı sertifikaları yeniden doğrulandı.

Bu raporun 400.000 kontrolü **yeni üretildi**. Arşivdeki 600 milyon tam-kristal-koşullu çekiliş **yeniden çalıştırılmadı**; onların kaydedilmiş bütün 127 çekirdek isabetleri yeniden okundu ve matris özellikleri ilk kez bu karşılaştırmada hesaplandı. Önceki raporlardaki yaklaşık 10⁻¹³ olay frekansları bu yeni deneyin sonucu olarak sunulmuyor ve yeni sonuçlarla çarpılmıyor.

## 3. Nesnenin hangi kısmı gerçekten bağlantılı?

Sûre konumu n, ayet sayısı a(n) olsun. İncelenen 127 seçeneğinde toplam ayet sayısı 6234. Şunları tanımlayalım:

\[
t=n+a(n),\qquad u=n-a(n).
\]

### 3.1. Bütün dizi ile alt aile aynı aritmetik dili kullanıyor

Bütün 114 konumun t değerleri 57 çift ve 57 tek değere ayrılıyor:

\[
\sum_{t\ \mathrm{çift}}t=6234=\sum a,
\qquad
\sum_{t\ \mathrm{tek}}t=6555=\sum_{n=1}^{114}n.
\]

İkinci toplam eşitliği birinciden ve toplamların korunmasından çıkar; iki bağımsız başarı sayılmadı.

Toplama ve çıkarma aileleri aynı 19 modülüyle, hiçbir üye elle seçilmeden kuruluyor:

\[
F=\{n:19\mid n+a(n)\},\qquad H=\{n:19\mid n-a(n)\}.
\]

F'nin 12 üyesi `[6,15,21,39,41,42,50,55,56,70,88,107]`; H'nin 7 üyesi `[14,23,48,78,80,91,104]`. Aileler ayrık; birleşimin büyüklüğü 19 ve ayet toplamı 1159=19×61.

İlginç olan yalnız iki bölünebilirlik bulmak değildir: **Aileleri kuran modül, iki ailenin birleşiminin büyüklüğünde ve birleşimin ayet toplamında yeniden yer alıyor.** Bunun neden seçilmiş bir özellik olduğu seçim tartışmasının konusudur; fakat tanımlandıktan sonra gerçek, açık bir ortak kısıttır.

### 3.2. Kristal, on iki sayının uzun bir ezberi değil

F içinde q=t/19 katsayılarını, t ve n paritelerine göre dört hücreye ayıralım:

| t paritesi | n paritesi | q katsayıları | Üye sayısı | q toplamı | t toplamı |
|---|---|---|---:|---:|---:|
| Çift | Çift | 6, 6, 8 | 3 | 20 | 380 |
| Çift | Tek | 6, 6, 6 | 3 | 18 | 342 |
| Tek | Çift | 5, 5, 9 | 3 | 19 | 361 |
| Tek | Tek | 5, 7, 7 | 3 | 19 | 361 |

Dört hücre aynı büyüklükte. İki satırın toplamı aynı: 38. Bir satır merkez etrafında 20/18, öteki 19/19. Farklı katsayılar kesintisiz 5,6,7,8,9 bandını dolduruyor. Hücrelerden biri üç eşit katsayıdan oluşuyor.

Bu yapı şu kısa tarifte önemli ölçüde yakalanabilir: **Dört eşit hücre; eşit satır toplamları; bir satırda eşitlik, diğerinde merkezden birer birim sapma; kesintisiz katsayı bandı; tekrarlı bir hücre.**

Önceki rapordaki daha dar cebirsel sonucu da tekrar hesapladım. Eski O koşulu altında eşit hücreler, kesintisiz bant ve aynı katsayıdan en az beş tane bulunması istenirse, 44 olası çift satır torbası ile 55 olası tek satır torbasının 2420 eşleşmesinden yalnız iki tanesi kalıyor:

| | Çift katsayılar | Tek katsayılar | Farklı katsayı bandı |
|---|---|---|---|
| Gözlenen | 6,6,6,6,6,8 | 5,5,5,7,7,9 | 5–9 |
| Diğer seçenek | 6,6,6,6,6,8 | 5,5,7,7,7,7 | 5–8 |

Beş ayrı katsayı içeren kesintisiz bant gözlenen torbayı seçiyor. **2420 torba eşit olasılıklı değildir; bu bir olasılık hesabı değil, kısa kuralların yapıyı ne kadar belirlediğinin tam sayımıdır.** Yeni ana deney bundan daha geniş bir sınıf kullanıyor: üçlü hücre, beş tekrar veya bant genişliği beş şartını dayatmıyor.

### 3.3. İki bölümleme aynı iki matris tarifine dönüşüyor

Herhangi bir alt küme K için aynı yapım kuralını kullanalım:

\[
L(K)=\begin{pmatrix}
\sum_{n\in K}n&\sum_{n\in K}a(n)\\
\sum_{n\notin K}n&\sum_{n\notin K}a(n)
\end{pmatrix}.
\]

Birinci alt küme çift t konumları, ikinci alt küme F ailesi olsun. Ham toplamlar:

\[
P=\begin{pmatrix}3303&2931\\3252&3303\end{pmatrix},
\qquad
R=\begin{pmatrix}590&854\\5965&5380\end{pmatrix}.
\]

Mod 19'da bunlar:

\[
P=\begin{pmatrix}16&5\\3&16\end{pmatrix},
\qquad
R=\begin{pmatrix}1&18\\18&3\end{pmatrix}.
\]

Aynı iki matristen iki farklı birleşim elde ediliyor:

\[
PR+RP=\begin{pmatrix}5&7\\18&12\end{pmatrix},
\qquad
PR-RP=\begin{pmatrix}17&10\\13&2\end{pmatrix}.
\]

Doğrudan sonlu alan hesabı:

\[
\operatorname{ord}(P)=\operatorname{ord}(R)=\operatorname{ord}(PR+RP)=360,
\qquad(PR-RP)^2=I.
\]

Burada mertebe, matrisin tekrar tekrar çarpıldığında ilk kez birim matrise döndüğü kuvvettir. 360=19²−1, 19 modülündeki tersinir 2×2 matrislerin mümkün olan en büyük mertebesidir. İndirgenemez karakteristik polinomda matris sonlu alanın çarpımsal grubunda davranır ve mertebe 360'ı böler; ayrışan farklı özdeğerlerde mertebe 18'i, tek Jordan bloğunda 342'yi böler. Böylece 360 gerçekten azami değerdir.

**Üç ayrı matris azami uzunlukta çevrim verirken aynı iki temel matrisin komütatörü iki adımda birime dönüyor.** Matrisin 360 mertebeli olmasının zorunlu kuvvet sonuçları ayrı başarılar olarak eklenmedi.

Önemli bir ayıklama: Her 2×2 komütatörün izi sıfırdır; Cayley–Hamilton nedeniyle karesi zaten bir skaler matristir. İlginç koşul “karesi skaler” değil, o skalerin tam 1 olması ve bunun azami mertebelerle birlikte sağlanmasıdır.

### 3.4. Bu matris ilişkileri toplamların yeniden yazımı mı?

Çekirdek altında, mod 19'da s=Σa=2, x=Σ_F n ve y=Σ_{t çift}n olsun. İki matris şu biçime iner:

\[
P=\begin{pmatrix}y&s-y\\-y&y\end{pmatrix},\qquad
R=\begin{pmatrix}x&-x\\-x&s+x\end{pmatrix}.
\]

Yeni bir açık özdeşlik de bunu gösterir:

\[
(PR-RP)^2=s^2(x^2+s y-y^2)I.
\]

Özdeşlik 19³=6859 kalıntı üçlüsünde bağımsız olarak denetlendi. Gözlenen çift (x,y)=(1,16). 361 olası çiftin yalnız 6'sı P ve R için azami mertebe veriyor; bütün doğal matris ilişkileri birlikte istenince yalnız (1,16) kalıyor. Matrislerin 16 sunumuna izin verildiğinde (1,16) ve (3,18) olmak üzere iki çift kalıyor.

**Bu çiftlerin permütasyonlar altında eş olasılıklı olduğunu varsaymadım.** Koşullu matris nadirliği aşağıda gerçek çekirdek örneklerinden ölçülüyor.

## 4. Özel sayıları ezberlemeyen karşılaştırma

Gözlenen nesneyi bilerek oluşturulmuş bir tanım dilinin seçim bedelini yok saymak doğru olmaz. Bununla birlikte, “bu örüntüler sıralı uzun dizilerde de benzer biçimde bulunur” iddiasını doğrudan kontrol etmek için aynı dili bütün örneklere uygulayabiliriz.

Yeni kontrol üretiminden önce `PROTOCOL.md` dosyasına şu yedi koşulu sabitledim:

| Kod | Genel koşul |
|---|---|
| A | Bütün dizide t paritesi 57/57 ve çift t toplamı toplam ayet sayısına eşit. |
| B | Seçilen modüler ailenin dört parite hücresi aynı, pozitif üye sayısına sahip. |
| C | B ile birlikte hücre toplamları bir satırda b,b; diğerinde b−1,b+1. b ve yönler sabit değil. |
| D | B ile birlikte farklı katsayılar kesintisiz tam sayı bandı ve en az iki elemanlı bir hücre sabit. |
| W | Karşıt iki aile ayrık; birleşimin üye sayısı p ve ayet toplamı p'ye bölünüyor. |
| M | İki bölümleme matrisi de p²−1 mertebeli. |
| J | PR+RP matrisi p²−1 mertebeli ve (PR−RP)²=I. J ayrıca M'yi şart koşmuyor. |

Her diziye, 3–113 arasındaki **29 tek asal modül**, **iki aile yönü** (n+a veya n−a), **iki sayım seçeneği** ve P,R'nin ayrı ayrı satır/sütunlarını değiştiren **16 matris sunumu** serbest bırakıldı. Bu, dizi başına 1856 aday sunum demek. Puan, bütün sunumlar içindeki en yüksek sağlanan koşul sayısıdır.

Bütün adaylar bağımsız olmak zorunda değildir. C ve D zaten B'yi gerektirir. **Bu puan yedi bağımsız tesadüfün çarpımı değil; tanımlı bir yapısal sınıfa uygunluk derecesidir.** Olasılıkları çarpmak yerine aynı puanın kontrollere doğrudan uygulanması kullanıldı.

Bu arama bazı önemli serbestlikleri karşılar; bütün olası matematiksel dilleri karşılamaz. Bileşik modüller, farklı matris yapım kuralları, farklı hücre sınıfları, sayısız alternatif aritmetik özellik ve tarihsel keşif yolları bu 1856 adayda yoktur.

## 5. Uzunluk düzenini koruyan yeni deneyler

### 5.1. Birincil model: tam aynı terslik sayısı

\[
I(a)=\#\{(i,j):i<j,\ a_i<a_j\}
\]

azalan uzunluk sırasından sapmayı sayar. Gerçek 129 listesinde I=993, 127 listesinde I=994.

Kontroller, 129 çoklu kümesinin I=993 olan ve tek 129 değerinin tek 128 değerinden önce bulunduğu sayısal permütasyonları arasından tekdüze üretildi. Böylece hareket eden 129 etiketi 127 yapılınca her eşlenik kontrolde I tam 994 oluyor. Kontrollerin her iki sayımda da nesneyle **aynı** sıra bozukluğu sayısına sahip olması sağlandı.

Mallows dağılımında ağırlık q^I olduğundan, aynı I dilimine koşullanınca q iptal olur. Ret örnekleyicide q=0.914573128462399 yalnız verimli öneri üretmek içindir. Yinelenen sayılar doğru çoklu-küme ağırlığıyla ele alındı; küçük bir çoklu kümenin 60 farklı permütasyonu üzerinde tam rasyonel olasılık hesabıyla kontrol edildi. Örnekler bağımsız ret çekilişleridir; MCMC zinciri kullanılmadı.

200.000 kabul edilen kontrol için 94.791.885 öneri işlendi. Tohum: 2026102501. Birincil model, önceki raporun q ayarlı koşulsuz Mallows modeliyle aynı model değildir; burada açık bir eş-sıralılık dilimi sınanıyor.

### 5.2. İkincil model: yerel uzunluk profilini koruma

Konumlar (1–6), (7–12), … biçiminde 19 bloğa ayrıldı. Her bloktaki altı ayet sayısı kendi içinde bağımsız ve tekdüze karıştırıldı. Böylece sadece “uzunlar başta” genel eğilimi değil, hangi altı uzunluğun hangi altı konumda olduğu bilgisi bile korunuyor. Tohum: 2026102502; N=200.000.

Bu modelin ortalama terslik sayısı 1009,986; gerçek 129 değerinde 993. Global sıra istatistiği tam eşit değil, yerel ayet-sayısı profili tam eşit. İki model ayrı açıklama varsayımlarını sınar.

### 5.3. Yeni sonuçlar

| En yüksek yapı skoru | Tam aynı terslik sayısı | Altılı yerel bloklar |
|---:|---:|---:|
| 0 | 4 | 9 |
| 1 | 166.011 | 168.222 |
| 2 | 31.497 | 29.856 |
| 3 | 2.461 | 1.902 |
| 4 | 27 | 11 |
| 5 | 0 | 0 |
| 6 | 0 | 0 |
| 7 | 0 | 0 |
| **Toplam** | **200.000** | **200.000** |

Gözlenen 127 nesnesinin skoru **7**. Bu iki yeni örneklemin hiçbirinde 5'e ulaşan kontrol bile yok.

![Yapısal skor karşılaştırması](figures/structural_scores.png)

Her modelde 0/200.000 isabet için olay frekansının tek yanlı %95 binom üst sınırı:

\[
1-0.05^{1/200000}=1.4978549\times10^{-5}.
\]

Artı-bir Monte Carlo sırası `(0+1)/(200000+1)=4.999975×10⁻⁶` olur. Bu sayı **tanımlı dilin null sırasıdır; dilin hedef görüldükten sonra seçilmiş olmasını düzeltmez**. İki farklı modelin örneklerini tek bir 400.000 denemelik olasılık hesabında birleştirmiyorum.

### 5.4. Ayrım tek bir özelliğe bağımlı değil

Genel terazi A'yı çıkarırsak nesne 6 koşulu sağlar. Her kontrolün azaltılmış puanı tam puanından büyük olamaz; dolayısıyla en fazla 4 kalır.

İki matris koşulu M,J'yi çıkarırsak nesne 5 koşulu sağlar. Yine bütün kontrollerin azaltılmış puanı en fazla 4'tür.

Bunlar iki ayrı duyarlılık kontrolüdür. A,M,J üçünü birden çıkarmak için aynı üstünlük iddiası yapılmıyor. Sonuç, ayrışmanın yalnız eski genel teraziden veya yalnız yeni matris eklerinden kaynaklanmadığını gösterir.

## 6. Rastgele dizilerde de ilginç özellikler var; fark birleşimde

Birincil 200.000 örnekte 127 seçeneğinin doğal 19/+ sunumunda:

| Özellik | Sağlayan kontrol sayısı |
|---|---:|
| Genel terazi A | 93 |
| Dört eşit hücre B | 3435 |
| Merkez etrafında toplam düzeni C | 611 |
| Kesintisiz bant ve sabit hücre D | 440 |
| Karşıt aile birleşimi W | 59 |
| İki matrisin azami mertebesi M | 2767 |
| Karışım matrislerinin ilişkileri J | 1182 |

Tek tek bakıldığında birçok “ilginç” özellik bulunuyor. Aynı doğal sunumda M ve J'yi birlikte sağlayan 26 kontrol dahi var. B,C,D'yi birlikte sağlayan 7 kontrol var; bunların hiçbirinde W de gerçekleşmiyor. Bu sayılar bağımsız olasılıklar gibi çarpılmadı.

**Nesnenin ayrıldığı nokta, sıradan kontrollerde hiçbir yapı bulunmaması değil; aynı az sayıda bölümleme ve işlem üzerinde bu yapıların birbirine bağlanmasıdır.**

Somut bir güçlü rakip de kaydedildi. Birincil deneyin 11.934'üncü kabul edilen kontrolü en yüksek 4 puanı alıyor. 59 modülünü, n−a ailesini, 127 seçeneğini ve R matrisinin satır değişimini seçiyor. Hücreleri:

\[
[0],\quad[-2],\quad[-1],\quad[-1].
\]

Dört eşit tek-elemanlı hücre, merkez etrafında toplam düzeni ve iki matris koşulu bulunuyor. Ama global terazi yok; en az iki elemanlı sabit hücre yok; karşıt ailelerin birleşimi 59 değil 5 üyeli ve ayet toplamı 365. Aynı I=994 düzeyinde düzenli bir rakip, fakat gözlenen bağlantıların tamamını taşımıyor.

Bu örnek, ilk 20.000 kontrolün aynı tohumla yeniden üretilmesiyle geri kazanıldı; yeni bağımsız kontrol olarak sayılmadı. Sadece bir örnektir; bütün yüksek puanlı kontrollerin aynı hücre biçimine sahip olduğu iddia edilmez.

## 7. Çekirdek zaten verilmişken matrisin ek kısıtı

Önceki rapor yalnız bir karşıörnekle matrisin çekirdekten zorunlu çıkmadığını göstermişti. Şimdi saklanmış koşullu örneklerden ne kadar ek seçicilik getirdiğini ölçebiliyoruz.

20 Ekim arşivinde, altılı blokların altı farklı başlangıç hizalaması için ikişer bağımsız tohumla 50'şer milyon tam-kristal-koşullu çekiliş yapılmış. Bu örneklerde terazi ve İki El de gerçekleştiğinde tam çekirdek tanıkları saklanmış. 127 için bütün tanıklar geri alındı; her birinin aynı ayet-sayısı çoklu kümesine sahip olduğu, A,G,W koşulları ve matrisler bağımsız kodla yeniden hesaplandı. Tanık sayıları özgün sayaçlarla tamamen eşleşti.

| Blok hizalaması | Tam çekirdek örneği | Doğal sunumda tam matris | 16 sunumdan herhangi birinde |
|---:|---:|---:|---:|
| 0 | 262 | 0 | 1 |
| 1 | 298 | 0 | 0 |
| 2 | 317 | 2 | 3 |
| 3 | 302 | 0 | 1 |
| 4 | 211 | 1 | 1 |
| 5 | 391 | 1 | 3 |
| **Toplam** | **1781** | **4** | **9** |

Altı model birbirinin aynısı değildir; 9/1781 betimleyici bir toplamdır, evrensel koşullu olasılık değildir. Her faz için tek yanlı Clopper–Pearson üst sınırı hesaplayıp altı karşılaştırmaya Bonferroni uyguladım. Eşzamanlı %95 üst sınırların en büyüğü **0,0320981** oldu.

Bu nedenle, arşivdeki bağımsız koşullu örnekleme varsayımı altında, **bu altı yerel modelin her birinde** tam çekirdek verilmiş olsa bile tam matris yapısının frekansı ortak %95 güven düzeyinde %3,21'in altında kalır. Eşdeğer olarak matris eklemek, bu modellerin her birinde en az yaklaşık **31,15 kat** ek seçicilik getirir.

Bu 31 katlık sınır birincil eş-terslik modeline, eski Mallows sonuçlarına veya başka bir sıfır modele taşınamaz. Doğal sunumda yalnız 4 isabet bulunması da çok hassas bir nokta tahmini vermeyi haklı kılmaz.

![Koşullu matris seçiciliği](figures/conditional_matrix.png)

## 8. İlginçlik ile kanıt gücü arasındaki doğru bağlantı

Bir tam dizilişi 114 sayıyı tek tek yazarak tanımlamak kolaydır; onun çok küçük olasılığından yapısal bir sonuç çıkmaz. Burada ise sınanan olay, tam dizilişin kopyası değildir. Farklı modüllere, farklı hücre büyüklüklerine, farklı merkezlere ve farklı matris sunumlarına açık kısa bir kurallar sınıfıdır.

İlginçlik üç ayrı gözlemle somutlaştı:

**Birincisi, kısa kurallar ayrıntıları belirliyor.** Katsayı çoklukları ve toplamlar birbirinden kopuk bir tesadüf listesi değil; küçük hücre kuralları büyük kısmını birlikte belirliyor. Mantıksal bağımlılık, bağımsız kanıt sayısını azaltırken tarifin sadeliğini artırabilir.

**İkincisi, aynı sade kurallar uzunluk-korumalı kontrollerde aynı ölçüde bir araya gelmiyor.** Genel tarama her diziye uygulanmasına rağmen gözlenen 7 ile kontrol maksimumu 4 arasında boşluk kalıyor. Bu, “uzunluk sırası zaten her şeyi açıklıyor” iddiasına doğrudan bir deneysel cevaptır; bütün olası tarihsel oluşum modellerine cevap değildir.

**Üçüncüsü, matris katmanı çekirdeğin mekanik bir sonucu değil.** Çekirdek örnekleri arasında da nadir kalıyor. Aynı ayet-sayısı verisinden kurulan iki doğal bölümleme, daha sonra ayrı sonlu alan koşullarını birlikte sağlıyor.

Buna rağmen bu çalışma, nesne için tamamlanmış bir minimum açıklama uzunluğu veya Bayes faktörü hesabı değildir. Kullanılan yedi koşulun, izin verilen diller arasından neden bu biçimde seçildiği hâlâ seçim problemidir. Yeni kontrol örnekleri bu seçim geçmişini geri alamaz. Dolayısıyla en doğru hüküm şudur:

> **İncelenen nesne, tanımlı genel yapısal dil ve iki uzunluk-korumalı sıfır modelinde güçlü biçimde ayrışan, aritmetik çekirdeğin üzerinde ayrıca seçici bir matris katmanı taşıyan bir nesnedir. Bu kapsamda sıradan bir tekil dizilişin seyrekliğine indirgenemez. Tüm tarihsel arama özgürlükleri düzeltilmiş evrensel bir anomali p-değeri, metnin doğruluğu veya belirli bir köken açıklaması bu deneyden çıkmaz.**

## 9. Doğrulama ve yeniden üretilebilirlik

Bağımsız Python ve C++ uygulamaları, üç koşudan örneklenen 10'ar dizinin bütün tarama sonuçlarında eşleşti. Her ana koşunun saklanan ilk 100 dizisinde çoklu-küme ve sıralılık koşulları doğrudan kontrol edildi. Küçük çoklu-küme üzerinde 60 permütasyonun rasyonel olasılıkları, eş-terslik koşullu örneklemenin tekdüzeliğini doğruladı. Gözlenen yapı, önceki matris karşıörneği, 361 sonlu-alan çifti, 2420 katsayı çifti ve bütün 1781 arşiv tanığı yeniden denetlendi.

Tam yeni simülasyonlar C++17 ile çalıştırılır. Python 3, NumPy, SciPy, pandas, Matplotlib, nbformat ve nbclient, denetim ve defter için kullanılır. Çalıştırılmış `yapisal_ayrisma.ipynb`, saklanan çıktıları okuyup önemli hesapları tekrar üretir. Tüm 400.000 dizi diskte tutulmaz; kod, sabit tohumlar, ilk 100'er örnek, toplu sayaçlar ve güçlü kontrol tanığı saklanmıştır. Arşivdeki 1781 çekirdek örneğinin tamamı bulunur.

```bash
g++ -O3 -std=c++17 src/experiment.cpp -o experiment
./experiment exact 200000 2026102501 results/exact < data/counts129.txt
./experiment block6 200000 2026102502 results/block6 < data/counts129.txt
python src/validate.py
python src/core_witness_audit.py
python src/orientation_sensitivity.py
python src/summarize.py
python src/build_notebook.py
```

Ana kanıt dosyaları: `results/exact_summary.json`, `results/block6_summary.json`, `results/validation.json`, `results/orientation_sensitivity.json`, `results/strong_control.json`; tam veri ve kod paket içinde.

## Kaynaklar ve yöntem notları

- Akifcimen/quran-permutation-study, arşiv veri ve kodları: https://github.com/Akifcimen/quran-permutation-study . Bu oturumda canlı içerik doğrulanamadı; arşiv kaynağı yukarıda açıklandı.
- 23 Ekim 2026 önceki inceleme: `quran_nadirlik_yeni_inceleme.md`, Kütüphaneden okundu. Bu rapordaki yeni sonuçların yerine kullanılmadı; tanımlar ve doğrulama kodu için kaynak oldu.
- He, Müller ve Verstraaten, *Cycles in Mallows random permutations*, https://arxiv.org/abs/2201.11610 . Mallows ağırlığının q^inv biçimi için birincil kaynak.
- Phipson ve Smyth, *Permutation p-values should never be zero: calculating exact p-values when permutations are randomly drawn*, 2010; https://arxiv.org/abs/1603.05766 . Sıfır isabeti sıfır p-değeri olarak yorumlamama ve artı-bir sıralama ilkesi.

Bu raporun sayısal bulguları, anılan yöntem makalelerinden değil paketteki kendi çalıştırılmış hesaplarından gelir. 127, incelenen sayısal seçenektir; metinsel doğruluk iddiası değildir.
