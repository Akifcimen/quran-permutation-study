# M20 ısınma tanısı — ön-kayıt (ana hat, 31 Ekim 2026)

## 1. Soru

M20 örnekleyicisi her bölütte 64 zinciri tam sıralı dizilişten (ayet sayıları büyükten küçüğe) başlatır. Her zincire 30.000 takas denemesiyle ısınma uygular (S = Σ (i+1)·a_i ≤ S_obs şartıyla). Sonra zincirleri sırayla, örnek başına 20 takasla ilerletir. Bir bölütün 250.000 örneğinde her zincir ~3.906 örnek verir, yani ısınmadan sonra ~78.000 takas daha ilerler.

Isınma yetersizse bölütün ilk örnekleri hedef dağılımdan (S ≤ S_obs olan dizilişler üzerinde tekdüze) daha sıralı kalır. Bu, M20 sayılarını Kur'an'a benzer yöne çekebilir. Başka bir ekip M20'nin başlangıç yanlılığının sayıyla sınırlanmadığını belirtti (31 Ekim). Bu tanı o sınırı koyar.

## 2. Yöntem

İki ölçüm yapılır. Kod, plasebo14/kapı çalışmasının donmuş kaynaklarıdır. Tek değişiklik, `kutuphane.h`'nin bir kopyasında ısınma uzunluğunun bir derleme parametresine (`ISINMA`, varsayılan 30.000) bağlanmasıdır. Ölçümler bu tanı klasöründe yapılır; çalışmaların kodu değişmez. Koşulardan önce yapılacak eşdeğerlik sınaması: `ISINMA` = 30.000 ile derlenen `kontrol_h_Gc`, M20 bölüt 1–24'te kapı çalışmasının özgün çıktısıyla süre alanı dışında birebir aynı olmalıdır.

**(A) Bölüt içi kayma.** `isinma_S`, sayaç olmadan yalnız örnekleyiciyi çalıştırır. Bölütün 250.000 örneğini sıraya göre on dilime ayırır. Her dilimde ortalama S'yi ve B2 sayısını yazar; B2, en az bir biçimde terazinin tuttuğu kitaplardır ve `kontrol_h`'nin `b2` alanının aynısıdır. Isınma 30.000 ve 3.000.000 ile bölüt 1–2.000 sayılır.

**(B) Isınma uzunluğunun sayımlara etkisi.** `kontrol_h_Gc` (Kur'an'ın donmuş dilleri A000, Am000, Ap000) M20 bölüt 1–4.000'de (10⁹ kontrol) üç ısınmayla sayılır:
- 30.000: özgün; kapı çalışmasının Kur'an tarafı Gc 10⁹ sayımı;
- 300.000;
- 3.000.000.

Ölçülen nicelikler: `b2` toplamı, A000 ve Am000'ın kB'si (bütünlüklü kontroller) ve k'si.

Bölütler bağımsızdır, çünkü her bölüt kendi tohumuyla yeniden başlar. Standart hatalar bölütler arası değişimden hesaplanır (bölüt başına değerlerin ortalamasının standart hatası). Oranların %95 aralığı delta yöntemiyle verilir.

## 3. Karar kuralı (sonuçlardan önce)

- **İhmal edilebilir:** (B)'de ISINMA 300.000 ve 3.000.000 ile 30.000 arasındaki oranın %95 aralığı iki ölçüte de uyar. `b2` oranı için aralık [0,97; 1,03] içinde kalmalıdır. Am000 kB oranı için [0,90; 1,10] içinde kalmalıdır. (A)'da 30.000 ısınmayla ilk dilimin S ortalaması ve B2 oranı, son dilimden 3 standart hatadan fazla sapmamalıdır.
- **Yanlılık var:** aralıklardan biri dışındaysa ya da ilk dilim 3 standart hatadan fazla saparsa M20 sayıları (kapı çalışması Kur'an tarafı Gc 10¹⁰ ve plasebo kolları), farkı ortadan kaldıran en kısa ısınmayla yeniden sayılır. Bu bir ekle raporlanır.
- **Belirsiz:** aralık eşiği kesiyor ama 1'i içeriyorsa (B) bölüt 4.001–8.000 ile genişletilir. Karar o zaman verilir.

A000 k'si (10⁹'da ~9) bu karar için fazla seyrektir. Yalnız betimsel yazılır.

## 4. Ana hattın tahmini (sonuçlardan önce)

114 konumda rastgele takasların karışma süresi kabaca n·ln n ≈ 540 kabul edilmiş takastır. S şartı altında kabul oranının %10–30 olduğunu kestiriyorum. Buna göre 30.000 deneme ~3.000–9.000 kabul eder; bu karışma süresinin birkaç katıdır.

Tahmin: yanlılık ihmal edilebilir. `b2` oranları %1'den, Am000 kB oranları %5'ten az farklıdır. Bölüt içinde kayma görülmez. Olasılık ~0,8. Yanlılık çıkarsa yönü, 30.000 ısınmanın daha sıralı, dolayısıyla daha Kur'an'a benzer kitaplar vermesi olur.

## 5. Kaynak

Workstation (`~/hrf_olcum/isinma/`), en çok 12 iş parçacığı, `nice -n 10`. Tahmini süre birkaç saat. Sapmalar `ek/` altında zaman damgalı yazılır.
