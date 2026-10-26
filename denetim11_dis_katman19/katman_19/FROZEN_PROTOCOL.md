# Ortak işlem dili ve metinsel sayı dayanağı: çalışma protokolü

Bu protokol yeni eğitim ve sınama simülasyonlarının sonuçları görülmeden
`protocol.json` ile birlikte kaydedilmiştir. Gözlenen 19 örüntüsü daha önce
bilinmektedir. Bu kayıt, eski keşfin geriye dönük ön kaydı değildir.

## Soru ve kapsam

Kitabın sûre uzunlukları ve tanımlanan sıralılık özellikleri korunurken,
metinsel sayı kullanımlarıyla uyumlu, ortak parametreleri yeniden kullanan
kısa bir yapısal tarif ne kadar ayırt edicidir?

Ölçüm, aşağıda tam tanımlanan sonlu aileyi kapsar. Her mümkün matematiksel
ifade, bütün doğal dil yorumları veya geçmişteki bütün araştırmacı seçimleri
bu ailenin içinde değildir. Bu sınır sonuçlarda korunacaktır.

## Veri

- Depo: Akifcimen/quran-permutation-study; commit
  `51fe1c0c36f2d481369480800aa84582e0a099e8`.
- Depodaki standart 114 ayet sayısı Tanzil Version 1.1 tam Arapça metninden
  bağımsız olarak çıkarılan 6236 ayet sayımıyla birebir doğrulanmıştır.
- 127 sayısal seçeneği yalnız özgün sûre kimliği 9'un 129 değerini 127 yapar.
  Her örnekte aynı sûre kimliği permütasyonu iki seçeneğe de uygulanır.
- Özgün Arapça kaynak dosyaları değiştirilmez. 9:128–129 çıkarımı metinsel
  aday sayı kümelerini değiştirmemektedir.

## Sonlu tarif dili

Modüller 3–113 arasındaki 29 tek asaldır. Doğrusal ifadeler
`t=alpha*n+beta*a` ve karşıt ifade `u=alpha*n-beta*a` biçimindedir.
`alpha` 1–3, `abs(beta)` 1–3, gcd(alpha,abs(beta))=1 ve beta'nın iki işareti
seçilebilir: toplam 14 ifade. Bu alan ilk incelemedeki n±a ailesini genişletir;
bileşik modüller ve daha yüksek katsayılar bu protokolde yer almaz.

E={t çift}, F={p|t}, H={p|u}; F'nin hücreleri t paritesi ile n paritesinin
kesişimidir. q=t/p katsayıları F içinde tanımlıdır. L(K), K ve tümleyenindeki
n ve a toplamlarını aynı 2x2 tabloda toplar. P=L(E), R=L(F).

Dört koşul grubu vardır:

| Grup | Bitler |
|---|---|
| G | E üye sayısı 57; sum_E n=sum_notE a |
| C | Dört hücre eşit ve pozitif üye sayılı; hücre toplamlarının bir satırı b,b ve öteki b−1,b+1; q desteği kesintisiz ve en az iki üyeli bir hücrenin bütün q'ları eşit; sum_F q=4p |
| W | F,H ayrık ve birleşim büyüklüğü p; birleşim a toplamı p'ye bölünüyor |
| M | P, R ve PR+RP matrislerinin her biri p²−1 mertebeli; (PR−RP)²=I |

C grubunun bitleri ayrı yüklemlerdir; bunların birbirini içerdiği varsayılmaz.
Özellikle son bit, önceki genel merkez örüntüsünü aynı modüle bağlar.
Matrislerde 16 satır/sütun sunumu, her iki matrise aynı anda yapılan
eşleniklik nedeniyle aynı dört özelliği veren 8 kanonik sunuma indirgenir.
Bu eşdeğerlik bağımsız tam matris hesabıyla doğrulanır.

Bir program G,C,W,M gruplarının boş olmayan herhangi bir altkümesini seçer.
Seçilen grupların BÜTÜN küme bölüntüleri taranır. Bir blok içindeki gruplar
aynı p,alpha,beta'yı paylaşır; farklı bloklar bunları ayrı seçebilir.
Yalnız G'den oluşan blok p kullanmaz. Veri seçeneği programın tamamında
ortaktır. Böylece ortak parametreli ve ayrı parametreli açıklamalar aynı
yarışmaya, açıkça farklı tarif maliyetleriyle girer.

## Tarif maliyetleri

- Grup seçimi: 4 bit.
- Seçilen grup içindeki yüklem maskesi: G=2, C=4, W=2, M=4 bit.
- Veri seçeneği: 1 bit.
- k etkin grubun bölüntüsü: log2(Bell(k)) bit.
- Her parametre bloğunun ifade seçimi: log2(14) bit.
- p gerektiren her bloğun modülü: -log2(q_policy(p)) bit.
- M grubu kullanıldığında matris sunumu: 3 bit.

Bu sonlu kod, grup seçimini, hangi başarıların raporlandığını, paylaşılan
parametreleri, sayım seçeneğini ve sunumu açıkça belirtir. Ortak bağlama
parametre seçim maliyetini bir kez öder; ayrı bağlamalar tekrar öder.
Maliyet sözlüğünün bu veriyi gördükten sonra seçilmiş olması, tarihsel
seçimlerin tamamını düzelttiğimiz iddiasına izin vermez.

## Metinsel sayı kuralları

Tam Arapça kaynak, açık sözlü tam sayı ifadeleri için taranmıştır. Birleşik
ifadeler tek sayı değerine çözülür; ayet adresleri, ebced, çıkarımla bulunan
toplamlar ve dilbilgisel ikiller birincil kümeye eklenmez.

- Açık asal değerler: {3,5,7,11,19}; eşit ağırlık 1/5.
- Sayının kendisinin ayrıca tartışıldığı genel bağlamlar: {3,5,7,19};
  eşit ağırlık 1/4.
- Birincil ağırlık: bütün 29 asal üzerinde eşit ağırlık, açık sayı kümesi ve
  genel sayı tartışması kümesinin eşit ağırlıklı karışımı.
- Ayrı duyarlılık sonuçları: bütün asallar; yalnız açık sayılar; yalnız genel
  sayı tartışması; 19'a koşullu hedef; açık sayıların geçiş sıklıkları.

19'a koşullu hedef, 19'un seçiminin bütün geçmiş bedellerini ödemiş global
sonuç olarak sunulmaz. Anlamsal sınıflandırma hipotezden haberdar model
destekli incelemedir; bağımsız, kör bir insan paneli yapılmış değildir.
Metinsel özet, sûrelerin yeniden sıralanmasında değişmeyen yan bilgidir.
Metinde 19'un bulunması ayrıca bağımsız bir olasılık çarpanı değildir.

## Öğrenilen karşılaştırma puanı

Her karşılaştırma modelinde bağımsız 200000 eğitim örneği oluşturulur.
Her sabit parametre ve grup için gerçek bitmaskesi sayılır. Bir pozitif
yüklem birleşiminin sıklığı bütün üst maskelerin sayımlarından elde edilir.
Sıfır maskenin olasılığı 1'dir; diğerleri (count+0.5)/(Ntrain+1) ile yumuşatılır.

Program puanı, grup içi birleşimlerin eksi log2 sıklıklarının toplamından
tarif maliyetlerinin çıkarılmasıdır. Bütün programlar arasında maksimum
alınır; sıfır puan yapısız karşılaştırma tabanıdır.

ÖNEMLİ: gruplar arasındaki sıklıklar çarpıldığı için bu, gerçek ortak olayın
eksi log olasılığı değildir. Bağımsızlık iddiası veya 2^(-score) p-değeri
üretilmez. Öğrenilmiş bir sıralama puanıdır. Çıkarımsal sonuç, aynı tam aramayı
uygulayan bağımsız sınama örneklerindeki konumundan gelir.

## Karşılaştırma modelleri ve sınama

1. Bütün etiketli sûre permütasyonları arasında eşit dağılım.
2. Sabit 19 adet altılı blok içinde bağımsız eşit permütasyon.
3. Standart sayımda 993, 127 seçeneğinde 994 kesin terslik koşuluna birlikte
   koşullanan eşit permütasyon. Mallows önerisi ve ret örneklemesi kullanılır;
   q sabit terslik diliminde iptal olur. Eşit uzunluklu sûrelerin kimlikleri
   eşit olasılıkla dağıtılır.

Her model için 1000000 yeni bağımsız sınama örneği, önceden kaydedilen
tohumlar ve 8 iş parçacığı ile üretilir. Her örneğe aynı tam arama uygulanır.
P=(1+#(kontrol_puanı>=gözlenen_puan))/(Ntest+1). Sabit örneklem bütçesinde
binom kuyruk güven aralığı ve sıfır isabet üst sınırı ayrıca raporlanır.
Üç modele karşı ortak dayanıklılık, en büyük model p-değeriyle özetlenir.

## Katkı tanıları

Aynı aday uzayında E1: bütün grupların tam başarısı ayrı adaylarda mevcut;
E2: bunlar aynı p ve ifadede mevcut (C'nin ilk üç biti);
E3: aynı koşullara C'nin merkez=modül biti de eklenmiş;
E4: ortak p genel sayı tartışması kümesinde.
E4⊆E3⊆E2⊆E1. Sıfır paydalı koşullu oranlar ölçülmüş sayılmaz.
Gözlenen ve en güçlü kontrollerin kazanan programları da yayımlanır.

## Doğrulama ve tamamlanma

- Ham veri kaynaklar arası eşleşmesi.
- Matris mertebe tablosunun bağımsız kesin aritmetik kontrolü.
- Küçük çoklu kümelerde örnekleme ve koşullu eşitlik kontrolü.
- C++ özellik çıkarımının doğrudan Python kümeleriyle karşılaştırılması.
- Program bölüntüsü optimizasyonunun tam tarama ile karşılaştırılması.
- Tam çalıştırılan notebook, rapor, kaynak/protokol özetleri ve yeniden
  üretme komutları.

Yalnız hız veya doğruluk sorunları için uygulama optimizasyonu yapılabilir;
hedefin kontrol grubundan ayrışmasına bakarak özellik, ağırlık veya örneklem
bütçesi değiştirilemez. Düzeltmeler kayıt altına alınır.
