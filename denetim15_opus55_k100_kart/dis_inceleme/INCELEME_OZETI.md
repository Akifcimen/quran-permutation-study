# Dış inceleme özeti (araştırmacının ilettiği biçimiyle; 1 Kasım 2026)

> Not (ana hat): Aşağıdaki metin, araştırmacının başka bir yapay zekâ ekibiyle yaptığı incelemenin, ana hatta iletilen özetidir. Metin aynen korunmuştur; yalnız iki özel kitaplık bağlantısı ("Ayrıntılı rapor" ve "Tam rapor") çıkarılmıştır. Tam rapor depoda yoktur. Ana hattın değerlendirmesi `BENIOKU.md`'dedir.

---

**İnceledim. Senin itirazının haklı bir tarafı var: yeni placebo çalışması, nesnenin bütün ilişkileriyle ne kadar nadir olduğunu ölçmüyor.** Bazı önemli ilişkilerin kaybolmasına izin veren bir puanlama yöntemini kalibre ediyor. Bunu yalnız yöntem açıklamasından değil, **Kur'an'a yetiştiği sayılan örneklerin gerçek dizilerini yeniden hesaplayarak** doğruladım.

Bununla birlikte, **ekibin ana sayımları yanlış yaptığına veya sonucu nesnenin aleyhine yapay olarak düşürdüğüne ilişkin kanıt bulmadım.** Sayımlar doğrulanıyor. Esas mesele, doğru sayılmış sonucun hangi soruyu cevapladığı. Ayrıca yeniden üretim paketinde gerçek bir kod kusuru ve bazı yorumlarda gereğinden fazla kesinlik buldum.

İncelediğim sürüm, **1 Kasım tarihli `9a3268a` commit'i**. Ondan önceki `5ef087d` düzeltmesini ve yeni çalışmanın dayandığı önceki tanımları da kontrol ettim. Ayrıntılı raporu hazırladım: [bağlantı çıkarıldı].

## 1. En önemli bulgu: "aynı puana ulaşmak", nesneyi oluşturmak anlamına gelmiyor

Yeni yöntemin Kur'an için seçtiği dil dört koşuldan oluşuyor:

- **B2:** Kitabın iki parite parçası arasında tanımlı toplam denkliği.
- **K1:** Dört hücre toplamındaki farklı değerlerin kesintisiz bir aralık oluşturması.
- **Qc:** Hücre katsayı toplamlarının merkezinin kullanılan modüle bağlanması.
- **WSum:** F ve H ailelerinin üye sayıları toplamının kullanılan modüle eşit olması.

Ancak kritik ayrıntı şu: **kontrolün bu dört koşulu birden sağlaması gerekmiyor.** Bunların bir alt kümesi üç tür ve üç düzey şartını sağlıyorsa, o alt kümenin bilgi puanı hesaplanıyor. Yeterince yüksek puan verirse kontrol Kur'an'a yetişmiş sayılıyor. Bu davranış kodda açıkça var. [`puan_B` ve kapsam hesabı](https://github.com/Akifcimen/quran-permutation-study/blob/9a3268a/denetim15_opus55_k100_kart/kart/src/dtab_b.h).

Bunu denetlemek için saklanan **47 M20, 22 Mallows ve 12 tekdüze isabetin**, her birinin 114 elemanlı dizisini bağımsız aritmetikle yeniden değerlendirdim:

| Model | Kur'an'ın puanına ulaşan kayıtlı kontrol | Seçilmiş dört koşulun tamamını taşıyan | 15 yüklemlik ortak profilin tamamını taşıyan |
|---|---:|---:|---:|
| M20 | 47 | **9** | **0** |
| Mallows | 22 | **2** | **0** |
| Tekdüze | 12 | **1** | **0** |
| **Toplam** | **81** | **12** | **0** |

**81 örneğin 69'u, seçilmiş dört koşulu bile birlikte taşımıyor.** Tam 15 yüklemlik profil ise bu kayıtların hiçbirinde, ne kanonik `(n+a,19)` merceğinde ne de taranan 406 merceğin herhangi birinde bulunuyor. Bu sonuçlar, repodaki isabet dizilerinin bağımsız yeniden hesabından geliyor. [M20 kayıtları](https://github.com/Akifcimen/quran-permutation-study/blob/9a3268a/denetim14_opus55_butunluk_plasebo/ana_hat/kapi/sonuc/kuran/Gc10/M/isabet.txt) · [Mallows kayıtları](https://github.com/Akifcimen/quran-permutation-study/blob/9a3268a/denetim14_opus55_butunluk_plasebo/ana_hat/kapi/sonuc/kuran/Gc10/L/isabet.txt) · [Tekdüze kayıtları](https://github.com/Akifcimen/quran-permutation-study/blob/9a3268a/denetim14_opus55_butunluk_plasebo/ana_hat/kapi/sonuc/kuran/Gc10/U/isabet.txt).

Bu tablo, Kur'an'ın kendi dilindeki **iç kontrol sayımına** ait. Aşağıda ele alacağım 40.000 placeboluk dış kalibrasyon ayrı bir katman.

Buradan çıkan hüküm kesin:

> **"Bu puana ulaşan örnekler bulundu" sonucundan, "nesnenin bütünü o sıklıkta oluşuyor" sonucuna geçilemez.**

Bu ayrım senin uzun zamandır üzerinde durduğun meseleyle doğrudan ilgili.

## 2. Aynı mercekte, aynı puanı alan somut bir karşıörnek buldum

Yöntemin neyi kaybettiğini en açık gösteren örnek, M20 kayıtlarındaki **41379 numaralı isabet**. Başka bir asal veya karmaşık bir form kullanmıyor: **Kur'an'la aynı `(n+a,19)` merceğinde** ve aynı dil puanında. Fakat yapının önemli parçaları eksik:

| Özellik | Kur'an | Aynı puanı alan kontrol |
|---|---|---|
| Mercek | **n+a, 19** | **n+a, 19** |
| Dil puanı | ≈18,801348 | ≈18,801348 |
| F ve H boyları | 12 ve 7 | 12 ve 7 |
| Aileler ayrık mı? | Evet | Evet |
| Hücre toplamları | 20 / 18 / 19 / 19 | 20 / 18 / 19 / 19 |
| Kitabın parite sayımı | **57 / 57** | **55 / 59** |
| Hücrelerdeki üye sayıları | **3 / 3 / 3 / 3** | **4 / 2 / 3 / 3** |
| Farklı katsayılar | **5, 6, 7, 8, 9 — boşluksuz** | **3, 4, 5, 6, 9, 11, 12 — boşluklu** |
| Birleşimin ayet toplamı | **1159 = 61×19** | **1215; 19'a bölünmüyor** |

Kaynak, [M20 isabet dosyasındaki 41379 kaydı](https://github.com/Akifcimen/quran-permutation-study/blob/9a3268a/denetim14_opus55_butunluk_plasebo/ana_hat/kapi/sonuc/kuran/Gc10/M/isabet.txt). Tablodaki yapısal değerleri diziden yeniden hesapladım.

**Bu kontrol, nesnenin dört önemli bağlantısını kaybetmesine rağmen aynı puanı alıyor.**

Dolayısıyla burada nesnenin bütünlüğünün ölçüde eksik temsil edildiğini söylemek için somut dayanak var. "Bana rakip daha zayıf görünüyor" şeklinde estetik bir itirazdan söz etmiyoruz; hangi özelliklerin kaybolduğu tek tek gösterilebiliyor.

Bir başka örnek de ayrımı daha belirgin kılıyor: Mallows **94893** kaydında F=12 ve H=7, fakat **bir ortak üye var**. Böylece birleşim 19 değil, **18 farklı üyeden** oluşuyor. Buna rağmen `WSum` doğru sayılıyor; çünkü bu yüklem yalnızca **12+7=19** hesabına bakıyor. Ayrıklık ve birleşimin 19 üyeli olması kodda ayrı koşullar. [Mallows kaydı](https://github.com/Akifcimen/quran-permutation-study/blob/9a3268a/denetim14_opus55_butunluk_plasebo/ana_hat/kapi/sonuc/kuran/Gc10/L/isabet.txt) · [W1, QU ve WSum tanımları](https://github.com/Akifcimen/quran-permutation-study/blob/9a3268a/denetim15_opus55_k100_kart/kart/src/kutuphane.h).

Bu nedenle, seçilen dört koşullu dile dayanarak "iki ayrık elin birlikte 19 üyeyi tutması da ölçüldü" demek doğru olmaz.

## 3. "Üç düzeyde bütünlük" şartı da senin kastettiğin bütünlüğü garanti etmiyor

Ana kural, dört düzeyden herhangi üçünün bulunmasını istiyor:

**Kitap, aile, hücre, birleşim.**

Bu yüzden **aile + hücre + birleşim** ile geçmek mümkün; kitabın bütününe uzanan bir bağlantı bulunması zorunlu değil. Ayrıca farklı düzey adları altında görünen iki koşul, bazı örneklerde aynı sayısal olguyu ifade edebiliyor. [Bütünlük kuralının tanımı](https://github.com/Akifcimen/quran-permutation-study/blob/9a3268a/denetim14_opus55_butunluk_plasebo/ONKAYIT.md).

### En güçlü Mallows rakibinde olan şey

Yeni K×100 çalışmasının en güçlü Mallows placebosu **A7192**:

- Mercek: **2n+a, 17**.
- F ailesi **9 üyeli**.
- H ailesi **tek üyeli**.
- H'nin bu tek üyesi zaten F'nin içinde.
- Dolayısıyla **F∪H = F**.

Bu yapıda F'nin ayet toplamı da birleşimin ayet toplamı da aynı sayı: **578 = 2×17²**. Seçilen dil, bu toplamın 17²'ye bölünmesini hem aile grubunda hem birleşim grubunda ifade ediyor. Böylece iki ayrı düzey etiketi altında, bu örnek bakımından aynı toplam karşımıza çıkıyor. [A7192'nin dili ve sonucu](https://github.com/Akifcimen/quran-permutation-study/blob/9a3268a/denetim15_opus55_k100_kart/k100/RAPOR.md).

Bu, senin "aynı sayının zorunlu olmadan farklı ilişkilerde tekrar etmesi" fikrin bakımından önemli bir sınırlılık.

**Düzeylerin adlarının farklı olması, taşıdıkları bilginin de farklı olduğunu göstermiyor.**

Ama burada da sınırı koruyalım: aynı puanlama kontrollere uygulandığı ve kuyruk doğrudan sayıldığı için, bu gözlem placebo sonucunu kendiliğinden geçersiz kılmaz. Ölçünün hangi tür yapıları ödüllendirdiğini gösterir.

Ayrıca yeni kalibrasyonda nokta kestirimine göre yetişen **yedi placebonun hiçbirinde**, Kur'an'ın B2 türündeki kitap toplam denkliği bulunmuyor. Bunu yalnız seçilmiş merceklerinde değil, 14 doğrusal biçimin tamamında kontrol ettim. Bazılarında başka kitap düzeyi bölünebilirlikleri var; bu yüzden "hiçbir kitap düzeyi özelliği yok" demek de yanlış olur.

**Sizin nesnenizin bütün kitapla kurduğu belirli bağlantının bu rakiplerde bulunmadığı ise doğrulanmış bir fark.**

## 4. Buna rağmen placebo kalibrasyonunun kendisi neden meşru?

Ekibin yaptığı şu soru için anlamlı:

> "Her rastgele kitapta kendi en iyi örüntüsünü arayan bu yordam, Kur'an'da bulduğu kadar uç bir puan sonucu ne sıklıkta üretir?"

Bu soruda rakip örüntülerin Kur'an'ınkine benzemesi zorunlu değildir. Her hedefin kendi özelliklerinden bir dil seçmesi, araştırmacının örüntü seçme serbestliğini hesaba katmayı amaçlar.

Senin asıl sorunsa daha dar ve daha yapısal:

> "Nesnede gördüğümüz bu bağlantıların birlikte bulunması, aynı türden ilişkileri koruyan bir karşılaştırmada ne kadar nadir?"

**Mevcut test ilk soruya ilişkin bilgi veriyor. İkinci soruyu tamamlamış değil.**

Ekibin bunu hiç fark etmediğini söyleyemem. Son raporlarında, rakiplerin başka türden olduğunu, kitap düzeyi zorunluluğunun ve tam profilin henüz ölçülmediğini açıkça yazmışlar. Bu, değerlendirmelerinde korunması gereken dürüst bir ayrım. [K×100 raporu: anlamı ve sınırı](https://github.com/Akifcimen/quran-permutation-study/blob/9a3268a/denetim15_opus55_k100_kart/k100/RAPOR.md).

Benim itirazım, **"bütün nesnenin kalibrasyonu" gibi geniş başlıkların, okuyucuda bu ikinci sorunun da çözüldüğü izlenimi oluşturması**. Teknik rapordaki sınırlar, ana sonuç cümlelerine daha açık taşınmalı.

## 5. Nadirlik gerçekten önceki placebo sonucundan daha mı kötü?

Burada iki karşılaştırma karışıyor.

Ham sonuçları bağımsız topladığımda yeni tabloyu doğruladım:

| Model | Placebo sayısı | Nokta kestirimine göre yetişen | Oran kestirimi | Belirsizlik çakışması da rakip sayılınca |
|---|---:|---:|---:|---:|
| M20 | 20.000 | 5 | **1/4.000** | **9/20.001 ≈ 1/2.222** |
| Mallows | 10.000 | 1 | **1/10.000** | **4/10.001 ≈ 1/2.500** |
| Tekdüze | 10.000 | 1 | **1/10.000** | **3/10.001 ≈ 1/3.334** |

Kaynak: [yeni ana sonuç tablosu](https://github.com/Akifcimen/quran-permutation-study/blob/9a3268a/denetim15_opus55_k100_kart/k100/RAPOR.md).

**Önceki 1/201 ile karşılaştırırsak, yeni sonuç sayısal olarak daha küçük bir oran veriyor.** Önceki sayı, 200 placebonun izin verdiği çözünürlük tabanıydı. Gerçek oran 1/4.000 olsaydı, 200 placeboda hiç rakip görülmeme olasılığı yaklaşık **%95** olurdu. Dolayısıyla önceki "hiç yetişen olmadı" ile yeni "20.000'de beş yetişti" arasında çelişki yok.

Değişen şey, daha büyük örneklemde rakiplerin görünür hâle gelmesi.

Buna karşılık **4,7×10⁻⁹ gibi ham puan kuyruğunu 1/4.000 gibi dil seçimi sonrası bir oranla karşılaştırırsak**, kanıtın daha zayıf göründüğü doğru. Fakat bunlar aynı sorunun iki ölçümü değil. İkinci aşama, dilin hedefe bakılarak seçilmesinin etkisini ölçmeye çalışıyor.

Bir de rapordaki **"arada"** sınıfı yanıltıcı anlaşılabilir. Protokolde bu, kabaca "en az bir rakip var, ama oran %5'in altında" anlamına geliyor. "Sıradan çıktı" anlamına gelmiyor. **Bu yordam ve modeller kapsamında binlerde bir oran, hâlâ seyrek bir sonuç.** [Okuma kuralları](https://github.com/Akifcimen/quran-permutation-study/blob/9a3268a/denetim15_opus55_k100_kart/k100/ONKAYIT.md).

## 6. Bulduğum gerçek teknik ve istatistiksel sorunlar

### A. Yeniden üretim paketinde eski Mallows yarışı kalmış

Yeni paketteki `kart/src/kontrol_h.c`, Mallows başlangıcında ortak bir diziyi kilitsiz değiştiriyor. Aynı başlangıcı birden fazla iş parçacığı eşzamanlı çağırınca referans dizisi ve model parametresi bozulabiliyor.

Bunu bağımsız stres testiyle yeniden ürettim: **320 başlangıçta 39 yanlış referans dizisi ve 52 yanlış parametre değeri** oluştu. Düzeltilmiş kaynak kopyası repoda başka bir yerde bulunuyor; dosyalar tutarlı hâle getirilmeli. [Sorunlu kaynak kopyası](https://github.com/Akifcimen/quran-permutation-study/blob/9a3268a/denetim15_opus55_k100_kart/kart/src/kontrol_h.c).

**Bu, yayımlanan K×100 sonuçlarının bozuk olduğunu göstermiyor.** GPU ana programı ilgili tabloyu seri hazırlıyor; CPU çapraz kontrolleri de tek iş parçacıklı yapılmış. Sorun, yeniden üretim paketinde yanlışlıkla kullanılabilecek eski CPU yolunun kalmış olması.

### B. Isınma tanısından çıkarılan bazı ifadeler fazla kesin

M20'de ısınmayı 30.000'den 3.000.000 takasa çıkarmışlar. Daha sık gözlenen göstergelerde sonuçlar çok yakın. Bu gerçek bir ilerleme; önceki "başlangıç etkisi hiç sınanmadı" eleştirisini aynen sürdürmek artık doğru olmaz.

Fakat asıl nadir olay, üç ısınma süresinde 10⁹ kontrolde yalnız **9, 6 ve 5** kez görülmüş. Bu sayılar, nadir kuyruğun kalan yanlılığını sıkı biçimde sınırlandırmaya yetmiyor.

Bu nedenle "30.000 yeterlidir" yerine şu ifade daha doğru:

> İncelenen göstergelerde ısınmayı yüz kat artırmanın belirgin etkisi görülmedi; nadir kuyruktaki kalan yanlılık için kesin sınır elde edilmedi.

Raporun son paragrafı zaten bu sınıra değiniyor; baştaki kesinlik onunla uyumlu hâle getirilmeli. [Isınma raporu](https://github.com/Akifcimen/quran-permutation-study/blob/9a3268a/denetim15_opus55_k100_kart/isinma/RAPOR.md).

### C. Ön-kayıttaki hassasiyet hedefi gerçekleşmemiş

Çalışma **±%30 kesinlik** hedeflemiş. Fakat dış sayımda:

- Beş isabetin yaklaşık bağıl standart hatası **%45**.
- Tek isabetin yaklaşık bağıl standart hatası **%100**.

Bu yüzden **1/4.000 ve 1/10.000 hassas biçimde belirlenmiş sabitler değil**. Rapordaki aralıklar da geniş: M20 yaklaşık **1/12.300–1/1.700**; diğer ikisi yaklaşık **1/395.000–1/1.800**. Aralıkların verilmesi doğru; hedefin sağlanmadığı ayrıca açıkça söylenmeli. [Ön-kayıt](https://github.com/Akifcimen/quran-permutation-study/blob/9a3268a/denetim15_opus55_k100_kart/k100/ONKAYIT.md) · [Sonuç raporu](https://github.com/Akifcimen/quran-permutation-study/blob/9a3268a/denetim15_opus55_k100_kart/k100/RAPOR.md).

### D. İç ölçüm belirsizliğiyle dış oran belirsizliği tam birleştirilmemiş

M20'de Kur'an **47**, yakın placebo **44**, diğerleri **48 ve 50** isabet vermiş. Nokta hesabında 44 dahil, 48 ve 50 dışarıda. Ama gerçek sıralamaları bu hassasiyette kesinleşmiş değil.

Beş kazanan üzerinden hesaplanan dış Poisson aralığı, bu iç sınıflandırma belirsizliğini kapsamıyor. `R_eşit` bunu temkinli biçimde ele almaya çalışıyor; fakat aralıkların çakışmasına dayanan sayıya "+1" eklemek, bütün adaptif yöntemi kendiliğinden kesin bir permütasyon testi yapmıyor.

Bu sorunlar sonucu çöpe atmayı gerektirmiyor. **Sonucun hassasiyetini ve matematiksel güvencesini doğru adlandırmayı gerektiriyor.**

## 7. Ekibe yöneltmememiz gereken iki eleştiri de var

İncelemede bunları özellikle kontrol ettim:

**"n+a ile daha karmaşık biçimleri eşit değerlendiriyorlar" demek doğru değil.** Kodda basit biçim önceliği var: n±a daha düşük, 2n±a daha yüksek bedelli. Birincil modelde eşit bedelli olanlar asallar. 19'a ayrıca öncelik verilmesi ayrı bir senaryo olarak ölçülebilir. [Mercek bedelleri](https://github.com/Akifcimen/quran-permutation-study/blob/9a3268a/denetim15_opus55_k100_kart/kart/src/tablo.h).

**"Tam yapı referans örnekleminde hiç görülmediği için mecburen dört koşula düşmüşler" açıklaması da ana M20 sonucu için desteklenmiyor.** İlgili aile içi tam profil referansta 42 kez, kısa çekirdek 97 kez görülmüş. Dolayısıyla burada yalnız sıfır sayımlı bir tavan sorunu yok. Seçilen bilgi puanı ve dil maliyeti, daha kısa açıklamayı tercih ediyor. Bunun anlamı, ek koşulların gerçek ortak olay nadirliğine hiçbir katkı yapmadığı değildir. [Referans sayımları ve dil seçimi](https://github.com/Akifcimen/quran-permutation-study/blob/9a3268a/denetim13_opus55_plasebo/PLASEBO12_RAPORU.md).

Eleştiriyi bu ayrımlar üzerine kurmak, savunmayı çok daha güçlü yapar.

## 8. Bundan sonra hangi çalışma istenmeli?

Bence ekibe verilecek teknik yön şu olmalı:

**Birinci iş, nesnenin tam ortak olayını ve onu temsil etmek için kullanılan puanı kesin biçimde ayırmak.** Her sonuç satırında hangisinin ölçüldüğü yazılmalı. Son commit, önceki genel P15 hesabımızın hassasiyet sorununu çözdüğünü göstermiyor; başka bir istatistik üzerinde ilerliyor.

Ardından şu çalışma yapılmalı:

1. **İlişkileri koruyan bir karşılaştırma tanımı kurulmalı.** Kitap toplam denkliği, sayım dengesi, aynı bölmenin aile ve hücrelere taşınması, katsayı bandı, iki ailenin ayrıklığı ve birleşimin toplam bağlantısı birlikte ele alınmalı. Bir koşulun kaybını başka bir nadir koşulla telafi edebilen puan, bu sorunun tek ölçüsü olmamalı.

2. **Mantıksal sonuçlar tekrar sayılmamalı.** F=12 gibi mevcut koşullardan zorunlu çıkan özellikler bağımsız nadirlik çarpanı olarak eklenmemeli. Aynı toplamın iki farklı adla görünmesi de iki bağımsız tekrar sayılmamalı.

3. **Yapı tanımının seçimi de kalibre edilmeli.** Şimdi gördüğümüz yedi rakibi eleyecek kurallar ekleyip "artık rakip kalmadı" demek yeterli olmaz. Kitap düzeyi zorunluluğu, 19 önceliği ve tam profil gibi seçenekler önceden açık bir menüye konmalı; aynı seçim ve değerlendirme yordamı bütün hedeflere uygulanmalı.

4. **Yeni doğrulama hedefleri kullanılmalı.** Mevcut placebolar artık tasarım sırasında görülmüş veri. Yeni koşulların ve seçim menüsünün sonucu, ayrı hedeflerle sınanmalı. Yalnız kontrol tohumlarını değiştirmek, kural seçiminin geçmişini tek başına ortadan kaldırmaz.

5. **Tam olayın olasılığını ölçme işi sürdürülmeli.** Önceki konuşmamızdaki C31/koşullu tamamlama yaklaşımının genel motora aktarılması, doğru ağırlıkların ve çoklukların denetlenmesi, ardından katkı dengesi ve iki bütçede hassasiyet doğrulaması hâlâ yerinde bir teknik hedef.

**Benim kanaatim, nesnenin ilginçliğini küçümsemek için bu yeni çalışmanın yeterli olmadığı.** Aynı puanı alan örneklerin, nesnenin dikkat çekici bağlantılarını taşımadığını doğrudan gördük. Bu, senin bütünlük itirazının sağlam tarafı.

Ama bu bulgu kendi başına "gerçek kalibre nadirlik yeniden katrilyonda bire çıktı" sonucunu da vermiyor. Şu anda savunabileceğimiz daha güçlü ve daha kesin cümle şu:

> **Yeni çalışma, belirli bir örüntü arama yönteminin rastgele veride ne yapabildiğini ölçtü. Nesnenin bütün ilişkilerinin o sıklıkta tekrarlandığını göstermedi. Bu iki sorunun ayrılması ve ikinci sorunun tamamlanması gerekiyor.**

Tam raporda sayısal yeniden hesapları, somut karşıörnekleri, kod kusurunu ve incelemenin kapsam sınırlarını birlikte verdim: [bağlantı çıkarıldı].
