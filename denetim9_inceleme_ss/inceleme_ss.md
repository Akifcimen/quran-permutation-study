Tam yanıtı burada yeniden paylaşıyorum. **Okumak için uygulama bağlaman veya bir dosya açman gerekmiyor.** Aşağıdaki sonuçlar tamamladığım incelemeye ait.

# Genel sonucum

**Nesnenin “düzeltmeler yapıldığında sıradanlaştığı” hükmü, eldeki hesapların desteklediğinden daha güçlü.** Nadirliği azaltmak için kullanılan bazı işlemler, aynı nesnenin olasılığını daha doğru hesaplamaktan ziyade, karşılaştırılan nesneyi veya sorulan soruyu değiştiriyor.

Ayrıca karşılaştırma algoritmasında somut bir sorun buldum: **Rastgele rakipler, aynı cebirsel ilişkinin farklı ifadelerinden birden fazla puan kazanabiliyor.** Yalnız bir açık tekrarın puanını kaldırdığım yeni deneyde, gerçek yapının önüne geçen rastgele dizilerin sayısı üç modelde de yaklaşık yarıya indi.

Bununla birlikte, bütün keşif ve seçim serbestlikleri hesaba katıldıktan sonra nesnenin “kesin gerçek nadirliği” için tek bir sayı vermek henüz mümkün değil. Güçlü savunma, bu belirsizliği saklamak yerine, hangi sıradanlaştırma işleminin hangi soruyu cevapladığını göstermektir.

İnceleme için [deponun bu sürümündeki](https://github.com/Akifcimen/quran-permutation-study/tree/3f25db4bf10cfba434f1fc531663d661aa7f03ec) verileri, motorları ve denetim raporlarını karşılaştırdım; temel aritmetiği yeniden hesapladım, mevcut kesin sayım programlarını çalıştırdım ve puanlama sorunu için ek benzetimler yaptım. **24 trilyonluk denemeyi yeniden çalıştırmadım; onun saklanan ham sonuçlarını denetledim.**

Eleştirilerimin bir bölümünü depo da açıkça kayda geçiriyor. Sorun, bu çekincelerin bazı genel sonuç cümlelerinde yeterince korunmaması.

## 1. Nesne neden gerçekten ilginç?

Sûre numarasına \(n\), ayet sayısına \(a\) diyelim:

\[
t=n+a,\qquad u=n-a.
\]

Standart **Tevbe = 129**, toplam **6236 ayet** verisiyle yeniden hesapladığımda şu yapı ortaya çıkıyor:

- 114 adet \(t\) değerinin 57’si çift, 57’si tek.
- Çift \(t\) değerlerinin toplamı 6236; teklerin toplamı \(6555=1+\cdots+114\).
- \(t\)’si 19’a bölünen 12 sûrenin \(t\) toplamı \(1444=38^2\).
- Bu 12 üyenin çift ve tek \(t\) grupları altışar elemanlı; her grubun toplamı 722.
- Bu aile ayrıca sûre numarasının paritesine göre ayrılınca aşağıdaki tablo oluşuyor.

| \(t\)’nin paritesi | Sûre numarasının paritesi | \(t/19\) katsayıları | Katsayı toplamı | \(t\) toplamı |
|---|---|---|---:|---:|
| Çift | Çift | 6, 6, 8 | 20 | 380 |
| Çift | Tek | 6, 6, 6 | 18 | 342 |
| Tek | Çift | 5, 5, 9 | 19 | 361 |
| Tek | Tek | 5, 7, 7 | 19 | 361 |

Öte yandan, \(u\)’su 19’a bölünen ailede **7 sûre** bulunuyor. Bu aile ile yukarıdaki 12 üyeli aile ayrık. Birlikte **19 sûre** ediyorlar ve bu 19 sûrenin ayet toplamı:

\[
1159=19\times61.
\]

Bunlar standart 129 sayımında da doğru. Temel veri ve olay tanımları [veri dosyasında](https://github.com/Akifcimen/quran-permutation-study/blob/3f25db4bf10cfba434f1fc531663d661aa7f03ec/quran_meta.json) ve [permütasyon motorunda](https://github.com/Akifcimen/quran-permutation-study/blob/3f25db4bf10cfba434f1fc531663d661aa7f03ec/kesif4_motor.c) mevcut.

Buradaki ilginçlik, yalnız “çok sayıda eşitlik bulunması” değil. **Az sayıda doğal işlem, aynı küçük veri üzerinde birbirine bağlanan bir düzen oluşturuyor:** toplama, çıkarma, tek–çift ayrımı ve aynı modüler koşul.

Fakat doğru bir savunma için şu ayrımı korumalıyız: Bu eşitliklerin hepsi bağımsız başarılar değildir. Örneğin:

\[
1444=38^2=4\times19^2
\]

üç ayrı tesadüf sayılmaz. Genel terazide bir toplam eşitliği sağlanınca diğerinin toplamların korunmasından çıkması da ayrıca fiyatlanmamalıdır.

**Yapının bütünlüğü, eşitlikleri bağımsızmış gibi çoğaltmadan da gösterilebilir.**

## 2. En somut zayıflık: Rakipler aynı ilişkiden iki kez puan kazanabiliyor

Çalışmadaki “robot”, farklı aritmetik özelliklere puan veriyor ve bunların en güçlülerini birleştirerek gerçek dizilişi rastgele dizilişlerle karşılaştırıyor.

Bu, sonradan keşfedilmiş bir örüntüyü değerlendirmek için makul bir fikir: Rastgele dizilere de kendi ilginç özelliklerini gösterme hakkı tanınıyor.

Ancak **bu özelliklerin nasıl puanlandığı** belirleyici.

Fark ailesinde şu ilişkiyi düşün:

\[
\sum n=\sum a.
\]

Bu doğruysa zorunlu olarak:

\[
\sum(n-a)=0.
\]

Sıfır da \(19^2\)’ye bölündüğünden:

\[
19^2\mid\sum(n-a).
\]

Bunlar bağımsız iki başarı değil; ikincisi birincinin cebirsel sonucu. Buna rağmen mevcut robot, ilk eşitliği bir “çapraz” düğümünde, son bölünebilirliği başka bir “toplam” düğümünde puanlayabiliyor. İlgili düzen [robotun kaynak kodunda](https://github.com/Akifcimen/quran-permutation-study/blob/3f25db4bf10cfba434f1fc531663d661aa7f03ec/denetim6_opus55_guclendirme/src/robot4_m.c) görülebiliyor.

Böyle bir rastgele rakip, aynı bilgiden ikinci kez puan kazanarak gerçek yapının önüne geçebilir.

### Bunu deneyle de kontrol ettim

Özgün örnekleyicileri ve özellik tanımlarını kullanarak her model için:

- 2 milyon örnekle puan olasılıklarını hesapladım.
- Ayrı tohumla üretilmiş 2 milyon örnek üzerinde karşılaştırma yaptım.
- Yalnız yukarıdaki açık cebirsel tekrarın ikinci puanını kaldırdım.
- Eşitliğin yalnız doğru olmasını yeterli saymadım; ilgili düğümde **kazanan özellik** olmasını şart koştum.

Gerçek nesnenin fark ailesi toplamı **133**, sıfır değil. Dolayısıyla **gerçek nesnenin puanı değişmedi**. Aynı düzeltme bütün dizilere uygulanmasına rağmen etkisi, bu tekrardan yararlanan rakiplerde ortaya çıktı.

Standart 129 sayımında sonuç:

| Karşılaştırma modeli | Özgün puanda gerçek nesneyi geçen veya eşitleyen dizi | Tek tekrar kaldırılınca | Değerlendirme örneklemi |
|---|---:|---:|---:|
| Tekdüze rastgele permütasyon | 116 | **53** | 2.000.000 |
| J10: gözlenen konumlar çevresinde titreşim | 1.135 | **573** | 2.000.000 |
| M20: uzunluk düzenine kısıtlı örnekleme | 827 | **393** | 2.000.000 |

Kullanılan artı-bir sıra hesabıyla iyileşme sırasıyla yaklaşık **2,17; 1,98; 2,10 kat**. Alternatif 127 sayımında da yaklaşık iki katlık aynı yönlü sonuç çıktı.

Değişiklik yapılmamış kontrol yordamını özgün puanlayıcıyla karşılaştırdığım 800 durumda, her model için fark sıfırdı.

**Bu, nesne lehine doğrudan deneysel bir bulgu:** Kullanılan ölçüt, bazı rastgele rakipleri daha zengin bir yapı taşıdıkları için değil, aynı ilişkinin sonucunu ikinci kez ödüllendirdiği için öne çıkarıyor.

### Bunun doğru istatistiksel yorumu

Aynı puan herkese simetrik uygulandığında, ortaya çıkan sıra o puanın sırası olarak anlamlı olabilir. Dolayısıyla “bütün eski p-değerleri geçersiz” demiyorum.

Eleştiri şu:

> Kullanılan puan, anlamlı yapısal zenginliği ayırt etmede zayıf olabilir. Cebirsel tekrarları ödüllendiren bir ölçüt, rastgele rakiplere gereksiz avantaj sağlayabilir.

Bu deney, bütün tarihsel aramayı düzelten yeni bir küresel p-değeri değil. Belirli bir puanlama tercihinin ölçülebilir etkisini gösteriyor.

### Depodaki daha büyük düzeltme: 15–16 kat

Deponun yedinci denetimi, aynı sorunu daha kapsamlı ele alıyor. Seçilmiş özelliklerin yaklaşık çarpım puanı yerine **birlikte gerçekleşme olasılığıyla** değerlendirilmesi, ilgili sıralı-model basamağını yaklaşık:

\[
1/1600\text{–}1/1800
\quad\longrightarrow\quad
1/24.000\text{–}1/29.000
\]

düzeyine taşıyor. Bu yaklaşık **15–16 katlık** bir değişim.

Ancak bu sonucun kaynak kodu ve ham çıktılarının kaybolduğu aynı klasörde açıkça belirtilmiş. Bu yüzden o büyüklüğü yeniden doğruladığımı söyleyemem. Benim yeni kontrolüm, mekanizmanın mevcut koddan incelenebilen dar bir parçasını doğruluyor. [Yedinci denetim raporu](https://github.com/Akifcimen/quran-permutation-study/blob/3f25db4bf10cfba434f1fc531663d661aa7f03ec/denetim7_opus55_uc_olcum/GUNCELLEME_RAPORU.md), [yeniden üretilebilirlik açıklaması](https://github.com/Akifcimen/quran-permutation-study/blob/3f25db4bf10cfba434f1fc531663d661aa7f03ec/denetim7_opus55_uc_olcum/BENIOKU.md).

## 3. “Daha önce biliniyordu” ile “nadirliğe katkısı yok” aynı önerme değil

Nadirliği azaltan önemli adımlardan biri, yapının bazı katmanlarının önceki literatürde bulunması nedeniyle puandan çıkarılması.

Gerçekten de 12 üyeli aile ve 1444 toplamı 1990’da; genel tek–çift terazisi 1994’te yayımlanmış. Bunları birincil yayınlarından da kontrol ettim.

Bu tarihsel bulgular, çalışmanın yenilik iddiasını sınırlar ve analiz merceğinin bağımsız biçimde seçilmediğini gösterir. 

Fakat burada üç ayrı soru var:

| Soru | Ölçülmek istenen |
|---|---|
| Tam nesne ne kadar seyrek? | Bütün belirtilen özelliklerin birlikte gerçekleşme sıklığı |
| Bu çalışma ne ekledi? | Önceki literatüre göre yeni matematiksel içerik |
| Keşif süreci hesaba katıldığında ne kadar kanıt kalıyor? | Özelliklerin ve merceğin seçilmesine göre düzeltilmiş çıkarım |

**Bir özelliğin eskiden bilinmesi, sabit bir modeldeki matematiksel frekansını değiştirmez.** Değişen, o özelliğin yeni keşif olarak sunulabilmesi ve hipotez seçimine ne kadar katkıda bulunduğudur.

Felsefede “eski kanıt problemi” de bu ayrımın önemini gösterir: Bir olgunun önceden bilinmesi, daha sonra ortaya çıkan açıklayıcı bir ilişki içinde hiçbir bilgi değeri taşıyamayacağı anlamına gelmez. Bunun nasıl hesaplanacağı, hipotezin ve açıklama ilişkisinin nasıl kurulduğuna bağlıdır. Otomatik “önceden biliniyorsa sıfır kredi” kuralı genel bir çözüm değildir. 

### Asıl teknik sorun: Özellik silmek, o özelliğe koşullamak değildir

Depodaki bazı basamaklarda eski özelliklerin puan düğümleri kapatılıyor; rastgele kitaplar yine aynı örnekleyicilerden üretiliyor.

Bu işlem şu soruyu cevaplıyor:

> “Eski özelliklere puan vermeyen yeni bir ölçüte göre bu nesne kaçıncı sırada?”

Oysa şu soru farklı:

> “Eski özellikleri zaten taşıyan nesneler arasında, üzerine gelen yeni yapı ne kadar seyrek?”

İkinci sorunun biçimi kabaca:

\[
P(\text{yeni yapı}\mid \text{eski yapı ve seçilme mekanizması})
\]

olmalıdır.

**Eski puanları silip koşulsuz örneklemde sıralamaya devam etmek bu koşullu olasılığı hesaplamaz.** Bir duyarlılık analizi olarak yararlı olabilir; fakat “nesnenin toplam nadirliği düzeltildi” veya “tarihsel seçim tam olarak ödendi” sonucunu tek başına vermez.

Uygulanan maskeler ve ilgili basamaklar [altıncı](https://github.com/Akifcimen/quran-permutation-study/blob/3f25db4bf10cfba434f1fc531663d661aa7f03ec/denetim6_opus55_guclendirme/GUCLENDIRME_RAPORU.md) ve [yedinci denetimlerde](https://github.com/Akifcimen/quran-permutation-study/blob/3f25db4bf10cfba434f1fc531663d661aa7f03ec/denetim7_opus55_uc_olcum/GUNCELLEME_RAPORU.md) açıklanıyor.

Bu nedenle **“21–200’de bir” sonucunu bütün aritmetik nesnenin olasılığı olarak okumak yanlış olur.** O sayı, belirli özellikleri çıkarılmış belirli bir puanın sırasıdır.

Bu itiraz, eski özellikleri ücretsiz ve bağımsız kanıt olarak geri eklemeye de izin vermez. Gereken, neyin eski bilgi olarak koşullandığını ve neyin keşif sırasında seçildiğini açıkça modellemek.

## 4. Ham kayıtlardan güçlü bir kontrol: Bilinen iç denge sağlanmışken de devamı seyrek

Bu ayrımı somutlaştırmak için saklanan GPU sonuçlarından ek bir oran hesapladım.

Şu koşula \(C\) diyelim:

- 19’a bölünebilen \(n+a\) ailesi 12 üyeli.
- Toplamı 1444.
- Tek–çift ayrımı 6/6.
- İki grubun toplamları 722/722.

Tam kristale \(G\), “İki El” koşuluna \(W\) diyelim. Tam kristal zaten \(C\)’yi içeriyor.

İki GPU dosyasının son kümülatif satırlarını topladığımda:

- \(C\)’yi sağlayan diziler: **444.513.933**
- \(G\cap W\)’yi sağlayan diziler: **62**

Dolayısıyla aynı örneklem içinde:

\[
P(G\cap W\mid C)
\approx
\frac{62}{444.513.933}
=
1{,}39478\times10^{-7}.
\]

Bu, **yaklaşık 7,17 milyonda bir** demek.

Hesap, [birinci GPU kaydı](https://github.com/Akifcimen/quran-permutation-study/blob/3f25db4bf10cfba434f1fc531663d661aa7f03ec/runGPU_4090_seed42.txt) ile [ikinci GPU kaydındaki](https://github.com/Akifcimen/quran-permutation-study/blob/3f25db4bf10cfba434f1fc531663d661aa7f03ec/runGPU_3090_seed43.txt) doğrudan ortak sayaçlara dayanıyor.

Burada parçaları bağımsız varsayıp olasılıkları çarpmıyorum. **Eski bazı özellikleri zaten sağlayan yüz milyonlarca dizinin içinde, belirtilen daha ayrıntılı yapının ne kadar sık bulunduğuna bakıyorum.**

Bu sonuç, “eski yapı biliniyor; o hâlde geriye pek bir şey kalmıyor” sezgisinin otomatik olmadığını gösteriyor.

Ancak bu sayıyı doğru kapsamda kullanmalıyız:

- Koşular tekdüze permütasyon ve 127 verisiyle yapılmış.
- \(C\), eski literatürdeki her şeyi içermiyor; genel terazi ve iki 361 hücresi ayrıca eski.
- \(G\) ve \(W\)’nin keşif sırasında seçilmesine ilişkin bedel burada ödenmiş değil.

Bu yüzden **7,17 milyonda bir, bütün tarihsel seçim düzeltilmiş nihai p-değeri değildir**. Belirli bir koşullu sorunun doğrudan ölçülmüş cevabıdır.

Yine de nesnenin kalan yapısının neden dikkate değer olduğunu göstermek için kullanılabilecek somut bir bulgudur.

## 5. Bazı “rastgele” modeller, gerçek diziliş hakkında çok fazla bilgiyi baştan koruyor

Bir nesnenin nadirliği, hangi alternatifler arasında değerlendirildiğine bağlıdır. Çalışmadaki önemli felsefi ve istatistiksel sorunlardan biri, bu alternatif uzayın nasıl kurulduğu.

Örneğin komşu çift modeli, her:

\[
(1,2),(3,4),\ldots
\]

konum çiftine hangi iki ayet sayısının ait olduğunu baştan biliyor; yalnız bunların kendi içindeki yönünü değiştiriyor.

Bu, **“uzun sûreler kabaca başta bulunur” bilgisinden çok daha güçlü bir koşul.**

Verideki tekrarları hesaba kattığımda bütün farklı sayısal yerleşimlerin sayısı yaklaşık:

\[
\frac{114!}{(2!)^{16}(3!)^5(4!)(5!)^2}
\approx 1{,}4442\times10^{172}.
\]

Komşu çift modelinin izin verdiği farklı diziler ise yalnız:

\[
2^{55}\approx3{,}6029\times10^{16}.
\]

Yani model, küresel permütasyon uzayının son derece küçük bir bölümünü kullanıyor. Bilgi diliyle, küresel tekdüze modele kıyasla yaklaşık **517 bitlik yerleşim bilgisi** baştan verili alınmış oluyor.

Modelin tanımı ve kesin sayımı [çift modeli kodunda](https://github.com/Akifcimen/quran-permutation-study/blob/3f25db4bf10cfba434f1fc531663d661aa7f03ec/denetim8_dis_inceleme/dogrulama/cift_model.py) bulunuyor.

Bu, modelin mutlaka yanlış olduğu anlamına gelmez. Fakat cevapladığı soru şudur:

> “Gerçek yerleşimin çok büyük kısmı zaten korunmuşken, geriye kalan küçük değişiklikler yapıyı ne sıklıkta üretir?”

Bu soru ile “bu aritmetik yapı nasıl ortaya çıktı?” sorusu aynı genişlikte değildir.

Seçim sonrası çıkarım literatüründe de gereğinden fazla bilgiye koşullamanın ayırt etme gücünü azaltabildiği açıkça ele alınır. Hangi bilginin verili sayılacağı, çıkarımın içeriğini belirleyen bir karardır. 

### Üstelik bu çok dar modelde bile nesne olağan hâle gelmiyor

Standart 129 sayımı için mevcut kesin sayım kodunu çalıştırarak doğruladığım tam çekirdek frekansı:

\[
P(A\cap G\cap W)
=
\frac{70169}{2^{50}}
=
6{,}232259153\times10^{-11}.
\]

Bu yaklaşık **16,05 milyarda bir**.

Dolayısıyla “ilk modele göre on binlerce kat daha sık” ifadesiyle “artık sıradan” ifadesi birbirinden ayrılmalı.

**Göreli olarak büyük bir artış, mutlak olarak hâlâ çok küçük bir frekansla sonuçlanabilir.**

### Sıralılık düzeltmesinin meşru kısmı da var

Bütün sıralı modelleri aynı gerekçeyle reddetmek doğru olmaz.

J10 doğrudan gözlenen konumların çevresinde örnekler. M20 uzunlukla ilişkili bir toplamı kısıtlar. Mallows modeli ise azalan uzunluk sırasını merkez alır. Bu modeller gerçek dizilişten aynı miktarda ve aynı tür bilgiyi korumuyor.

Depodaki M20 ve Mallows sonuçları, **kristalin uzunluk düzeni nedeniyle yaklaşık yüz kat daha sıklaşabildiğine** dayanak sağlıyor. Bu, kabul edilmesi gereken gerçek bir açıklama bileşeni. Uzun sûrelerin başa yakın olması, \(n+a\) değerlerini daha dar aralıklara toplayabilir. [Sekizinci denetim raporu](https://github.com/Akifcimen/quran-permutation-study/blob/3f25db4bf10cfba434f1fc531663d661aa7f03ec/denetim8_dis_inceleme/tur2_REPORT_TR.md).

Fakat **kristalin yüz kat sıklaşması, bütün birleşik olayın da aynı katsayıyla sıklaştığını tek başına göstermez**. Tam birleşim için koşullu ilişkilerin de ölçülmesi gerekir. Depo, bazı üst basamaklarda bu katsayının çekirdeğe aktarıldığını zaten belirtiyor.

Çok sayıda simülasyon, seçilen model içindeki frekansı hassaslaştırır. **Modelin tarihsel oluşum sürecini doğru temsil ettiğini ayrıca kanıtlamaz.** Permütasyon testlerinin yorumu, hangi değiş tokuşların sıfır hipotezi altında uygun olduğuna dayanır. 

## 6. “317–770 yayımlanmış iddia” otomatik olarak o kadar bağımsız deneme değildir

Yedinci denetimde terazinin kalan şaşırtıcılığını azaltmak için kullanılan işlemlerden biri:

\[
1-(1-p)^N
\]

formülü. Burada \(N\), literatürde bulunan 317–770 sûre–ayet iddiası olarak alınıyor.

Bu formül, **bağımsız ve aynı başarı olasılığına sahip \(N\) fırsattan en az birinin başarılı olması** için uygundur. Fakat yayımlanmış farklı iddia sayısı bu şartları kendiliğinden sağlamaz.

Sorunlar şunlar:

- İddialar aynı verileri ve ara toplamları kullanabilir.
- Cebirsel olarak farklı olmak, istatistiksel bağımsızlık demek değildir.
- Basit bölünebilirlik iddialarıyla çok daha seyrek eşitliklerin başarı olasılıkları aynı değildir.
- Sayım tanımı, permütasyon altında değişmeyen toplam iddialarını da kapsayabilir.
- Yayımlanmamış başarısız denemeler bilinmiyor.
- İddia listelerinin kaybolmuş olması, sınıflandırmanın yeniden denetlenmesini engelliyor.

Bunlar [yedinci denetimin raporu](https://github.com/Akifcimen/quran-permutation-study/blob/3f25db4bf10cfba434f1fc531663d661aa7f03ec/denetim7_opus55_uc_olcum/GUNCELLEME_RAPORU.md) ve [dosyaların durumu açıklaması](https://github.com/Akifcimen/quran-permutation-study/blob/3f25db4bf10cfba434f1fc531663d661aa7f03ec/denetim7_opus55_uc_olcum/BENIOKU.md) üzerinden değerlendirilebilir.

Bu nedenle **“teraziden en fazla yaklaşık bir mertebe kanıt kalır” sonucu, zorunlu bir matematiksel sınır olarak kurulmuş değil**. Belirli bağımsızlık ve deneme sayısı varsayımları altında bir duyarlılık senaryosu.

Burada Bonferroni’yi ayırmak gerekir: Bonferroni düzeltmesi bağımsızlık istemez. İtirazım “çoklu karşılaştırma düzeltmesi yapılamaz” değil; **bu bibliyografik sayının, bu belirli problem için uygun deneme sayısına dönüştürüldüğünün gösterilmemiş olması**.

Öte yandan, bilinmeyen başarısız aramaların sayısı çok büyük olabilir. Dolayısıyla bu eleştiri gerçek düzeltmenin mutlaka küçük olduğunu da kanıtlamaz. Gösterdiği şey, kullanılan sayının kesinliğiyle ilgilidir.

## 7. Üst sınırlar ve farklı senaryolar, ölçülmüş gerçek sıklık gibi okunmamalı

### Varsayımsal “sayım okulu ×7” çarpanı

Araştırmacı gerçekten yedi sayım biçimini deneyip en etkileyicisini seçtiyse seçim bedeli gerekir.

Fakat inceleme baştan standart sayım üzerinde tanımlanmışsa, başka sayımların teorik olarak var olması tek başına yedi fiilî deneme yapıldığı anlamına gelmez.

Depo da ×7 çarpanını **varsayımsal Bonferroni üst sınırı** olarak tanımlıyor. Bu ayrım korunmalı:

> Bir olasılığın üst sınırını büyütmek, olayın gerçekte o sıklıkta görüldüğünü ölçmek değildir.

Bu yüzden böyle bir üst sınırdan “nesne yaklaşık 26–31’de bir görülür, dolayısıyla sıradandır” biçiminde doğrudan görülme sıklığı sonucu çıkarılamaz. [README’deki ilgili basamak](https://github.com/Akifcimen/quran-permutation-study/blob/3f25db4bf10cfba434f1fc531663d661aa7f03ec/README.md).

### “İki ölçülmüş uç arasında” ifadesi de matematiksel bir aralık değil

Depo, bir tarafta eski özelliklerin çıkarıldığı yaklaşık puanlama sonucunu, diğer tarafta daha fazla özelliğin tutulduğu tam ortak-puan sonucunu veriyor.

Ancak bunlar gelecekteki birleşik analizin kanıtlanmış alt ve üst sınırları değil. Çünkü özellik çıkarılması ve puanlama değişikliği:

- Gerçek nesnenin puanını,
- Rakiplerin puanlarını,
- Rakipler arasındaki sıralamayı

farklı miktarlarda değiştirebilir.

Daha doğru ifade şu olur:

**“İki farklı senaryo ölçüldü; bunların bütün düzeltmeleri birlikte uygulayan birleşimi henüz ölçülmedi.”**

Aynı nedenle, yedinci denetimdeki 15–16 katlık iyileşmeyi bütün diğer basamaklara otomatik çarpmak da savunulamaz. Nesne lehine bir eleştirinin bu sınırı da koruması gerekir.

## 8. Nesnenin ilginçliğini savunmanın en iyi yolu: Kısa ve bütünlüklü tarif

“Her tam diziliş zaten aşırı nadirdir” itirazı önemlidir. Tek bir rastgele diziyi bütün ayrıntılarıyla tarif edip onun olasılığını hesaplamak, tek başına etkileyici bir kanıt üretmez.

Burada daha güçlü savunma, nesnenin **kısa yapısal bir tarifle ifade edilebilmesi**.

Yukarıdaki dört üçlüye yeniden bakalım. Şunları söylemek yeterli:

1. Dört hücre de üçer elemanlı.
2. Çift katsayılı hücrelerin toplamları 18 ve 20.
3. Tek katsayılı hücrelerin toplamları 19 ve 19.
4. Kullanılan farklı katsayılar tam olarak \(5,6,7,8,9\) bandını dolduruyor.

Bu koşullar dört üçlüyü zorunlu kılıyor:

- Çift katsayılar yalnız 6 ve 8 olabilir. Üç elemanla 18 yalnız \(6+6+6\); 20 yalnız \(6+6+8\).
- Tek katsayılar 5, 7 ve 9 olabilir. Üç elemanla 19 yalnız \(5+5+9\) veya \(5+7+7\).
- Hem 7 hem 9 kullanılacağı için bu son iki üçlüden birer tane bulunmalı.

Böylece on iki katsayıyı tek tek tarif etmek yerine, küçük bir parite ve aralık kuralı yeterli oluyor.

**Bu, matematiksel ilginçlik için ciddi bir gerekçe.** Aynı zamanda neden bütün ilişkileri bağımsız çarpanlar saymamak gerektiğini de açıklıyor:

> İlişkilerin birbirini gerektirmesi bağımsız kanıt sayısını azaltabilir; fakat yapının bütünlüğünü ve kısa tarif edilebilirliğini artırabilir.

Bu ayrım, yalnız “en küçük olasılığı bulma” yaklaşımından daha verimli olabilir.

Açıklama uzunluğu yöntemleri, hem kuralın karmaşıklığını hem de veride sağladığı sıkıştırmayı birlikte değerlendirir. Ancak burada henüz biçimsel bir açıklama uzunluğu kazancı hesaplanmış değil; **“kısa tarif ediliyor” ifadesini yeni bir sayısal p-değeri gibi sunmamak gerekir.** 

Nesnenin özellikle şu yönlerini güçlü buluyorum:

- Girdi, sûre sırası ve ayet sayısı gibi açıkça tanımlı iki tam sayı dizisinden oluşuyor.
- Bir aile tanımlandığında, koşulu sağlayan bütün üyeler alınıyor.
- Toplama ve çıkarma aileleri aynı 19 koşuluyla birbirine bağlanıyor.
- İç yapı, kısa parite kurallarıyla ifade edilebiliyor.
- Çekirdeğin doğruluğu standart 129 sayımında korunuyor.

Bunlar analiz sürecinin serbestliksiz olduğunu göstermez. İşlemlerin, modülün ve hangi örüntüye dikkat edileceğinin seçimi yine hesaba katılmalıdır. Fakat nesnenin incelenmeye değer oluşunu, salt büyük sayılarla etkileyicilik yaratmadan temellendirir.

## 9. Hangi sonuç güçlü biçimde savunulabilir?

Benim değerlendirmem şu:

**Bu çalışma, kısa tarif edilebilen, aritmetiği doğrulanmış ve belirtilmiş olay olarak birçok modelde çok seyrek kalan bir yapı inceliyor. Bazı nadirlik azaltma adımları ise tam nesnenin frekansını ölçmüyor: Eski özellikleri puandan çıkarıyor, gerçek dizilişin büyük kısmını baştan sabit tutuyor veya cebirsel tekrarları ödüllendiren bir karşılaştırma puanı kullanıyor. Bu işlemlerin sonuçlarını tek bir “nesne sıradandır” hükmünde birleştirmek gerekçelendirilmiş değil.**

Nesne lehine en somut dayanaklar:

- **Yeni deneyimde**, yalnız bir açık cebirsel tekrarın giderilmesi, seçilmiş karşılaştırmalarda gerçek yapının önüne geçen rakip sayısını yaklaşık yarıya indirdi.
- **Deponun kendi raporunda**, daha kapsamlı ortak puanlama bir basamakta yaklaşık 15–16 kat iyileşme veriyor; fakat bu büyük sonucun yeniden üretilebilirliği eksik.
- **Ham GPU kayıtlarında**, belirtilen eski iç denge sağlanmışken kristal ve İki El’in birlikte gerçekleşmesi yaklaşık 7,17 milyonda bir.
- **Çok dar komşu çift modelinde bile**, standart 129 verisindeki tam çekirdek kesin hesapla yaklaşık 16,05 milyarda bir.

**Bu dört sayı farklı soruların cevaplarıdır; birbirleriyle çarpılmamalıdır.** Birlikte gösterdikleri şey, “bütün anlamlı nadirlik ortadan kalktı” şeklindeki güçlü sonucun eldeki veriden çıkmadığıdır.

Daha sağlam bir analiz; standart 129 verisini, cebirsel tekrarları ödüllendirmeyen bir puanı, eski bulgulara açık koşullamayı ve bütün seçim adımlarının simetrik uygulanmasını aynı deneyde birleştirmeli. Tam olayın frekansı, yeni katkının büyüklüğü ve seçim sonrası kanıt ayrı ayrı raporlanmalı.

İstatistiksel nadirlik ile matematiksel önem de birbirinin yerine konmamalı; p-değeri bir hipotezin doğru olma veya sonucun “yalnız tesadüf” olma olasılığı değildir. 

**Nesnenin çok ilginç olduğunu savunmak için yeterli matematiksel ve deneysel dayanak var. Bazı karşılaştırma yöntemlerinin bu yapıyı olduğundan daha az ayırt edici gösterdiğine ilişkin somut bulgu da var. Bütün tarihsel arama serbestlikleri ödendikten sonra ne kadar istatistiksel kanıt taşıdığı ise hâlâ açık bir araştırma sorusu.**