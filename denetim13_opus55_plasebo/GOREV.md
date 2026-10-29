# GÖREV — plasebo kalibrasyonu: bütünlük dilini her hedef kendisi kursa, Kur'an'ın ayrışması ne kadar sıra dışı?

**Veren:** araştırmacı (Akif Çimen), ana hat oturumu aracılığıyla · **Tarih:** 29 Ekim 2026 · araştırmacı onayladı ve başlattı

## Bağlam

`~/Desktop/hrf2` (github.com/Akifcimen/quran-permutation-study) Kur'an'ın sûre numaraları n ve ayet sayıları a üzerindeki bir yapıyı inceler. Önce oku: `README.md` (merdiven), `KESIF.md` notları 22–25, `denetim12_opus55_butunluk/BENIOKU.md`, `denetim12_opus55_butunluk/BUTUNLUK_RAPORU.md` ve `ONKAYIT.md`.

On ikinci denetim (`~/opus5.5/butunluk`, ön-kayıt SHA-256 `d213dc89…`) katmanlar arası yeniden belirmeyi 20 yüklemli bir dille ölçtü: her kitap 406 mercekten kendi en iyisini seçer, mercek sadelik önseliyle bedellendirilir, birincil puan bilgi-ağırlıklıdır. Kur'an (n + a, 19)'da 15 yüklem taşır. Bilgi puanındaki sırası M20'de 2,3×10⁻⁷, Mallows'ta 4,7×10⁻⁷, tekdüzede 4,0×10⁻⁸'dir (10⁸ kontrol).

**Bu görevin konusu (not 25, çekince 1):** dil, Kur'an'ın katmanlarını adlandırmak için Kur'an görüldükten sonra yazıldı. Mercek seçimi bedellendi, **yüklem seçimi bedellenmedi.** Ölçülen sıra "rastgele kitaplar Kur'an'a göre yazılmış dilde ne kadar puan alıyor?" sorusunun cevabıdır. Cevaplanacak soru şudur: "her hedef kendi dilini aynı yordamla kursa, Kur'an'ın ayrışması ne kadar sıra dışı kalır?" Doğru kalibrasyon bir **plasebo testi**dir. Dil yazmak otomatik bir yordama çevrilir ve yordam Kur'an'a ve rastgele kitaplara aynen uygulanır. Her hedef kendi diliyle kendi kontrollerine karşı sınanır. Kur'an'ın ayrışması plasebo kitapların ayrışmaları arasında sıralanır.

**Depoda 27 Ekim'de önceden ilan edilen okuma kuralı (not 25; ön-kayda aynen yaz, değiştirme):**
> Kur'an'ın ayrışması bütün plasebo kitaplarınkini aşarsa, bu basamağın sırası dilin Kur'an'a göre yazılmasıyla açıklanamaz; kalibre p en küçük ~1/(K + 1) olabilir (K, plasebo kitap sayısı) ve aradaki fark ayrıca, model-tabanlı olarak verilir. Plasebo kitaplar Kur'an'ın ayrışmasına sık ulaşırsa, bu basamağın nadirliği büyük ölçüde dilin Kur'an'a göre yazılmasından gelir ve öyle yazılır. Arada kalan sonuçlar kalibre p'leriyle aynen yazılır.

## Kurallar

- **`~/Desktop/hrf2` salt okunurdur; hiçbir dosyasını değiştirme.** `~/opus5.5/butunluk`, `~/opus5.5/kosullu` ve `~/opus5.5/plasebo` de salt okunurdur. `~/opus5.5/plasebo`, 26 Ekim'de ön-kayıttan önce durmuş eski bir oturumdur ve bu görevin girdisi değildir. Kod kopyalanarak kullanılabilir. Bütün çalışma `~/opus5.5/plasebo12` altında yapılır. Ana hattın hafızasına (`~/.claude/projects/<hrf2-proje>/memory/`) yazma; kendi hafızanı kendi proje dizininde tut.
- **Ölçmek için hatalı davranma.** Sonuç hangi yöne çıkarsa çıksın aynen yaz. Amaç nesneyi güçlendirmek ya da zayıflatmak değil, yüklem seçiminin bedelini doğru ölçmek.
- **Ön-kayıt:** `ONKAYIT.md`'ye şunları yaz: kütüphaneyi ve üretim kuralını, dil bedelini, D yordamını, hedef sayılarını, örneklem bütçelerini ve derinlik kuralını, tohumları, ayrışma istatistiğini, çözülmemiş aramaların kuralını, model-tabanlı fark modelini ve karar kurallarını. Bunların hiçbiri hiçbir hedefin (Kur'an dahil) kütüphane profiline, D sonucuna ya da kontrol puanına bakılarak yazılmaz. Kütüphane tek tek seçilmez; önceden yazılmış bir üretim kuralıyla (düzey × tür × nicelik × ilişki) üretilir. **Kur'an'ın kütüphane kardeşlerinden hangilerini sağladığı dondurmadan önce hesaplanmaz.** Dondurmadan önce izin verilenler: kod doğrulamaları (yalnız uyuşmazlık sayıları), örnekleyici eşitlikleri, sabit ya da hep-doğru yüklem taramaları (yalnız bayraklar), hız ölçümü. Ön-kaydın SHA-256'sını ve UTC zamanını `sonuc/onkayit_ozet.txt`'e yaz, dosyayı salt okunur yap. Sapmalar `ek/` altında, ayrı ve zaman damgalı eklerde.
- **Bütçe:** araştırmacı bu bilgisayarda gün boyu çalışıyor (Apple M1, 8 çekirdek, 16 GB). **Aynı anda en çok 4 iş parçacığı** kullan. Toplam hesap tavanı 50 çekirdek-saatidir (hedef ~30). Bellek 6 GB'ı, disk 8 GB'ı geçmesin. Uzun koşular kontrol noktalı ve sürdürülebilir olsun. Bütçe aşılırsa yeni koşu başlatılmaz, eksik kalan yazılır.
- **Kayıt güvenliği:** daha önce bir oturum klasörü kayboldu, bir oturum da ön-kayıttan önce durdu. Kodu, betikleri, ön-kaydı ve küçük çıktıları klasörde tut. `sonuc/ara_notlar.md`'yi düzenli güncelle. İş bitince `paket/plasebo12_paket.tar.gz` (≤ 200 MB) üret ve SHA-256'sını yaz.
- Her sayının yanında sıfırı, örneklemi, isabet sayısını ve belirsizliği yaz. Senaryoları bir aralığın uçları gibi sunma; üst sınırları sıklık gibi okuma.

## Ana hattın önceden kaydettiği tahmin (ön-kayda aynen yaz, sonda değerlendir)

Birincil sıfır M20'de:
- D(Kur'an) diliyle Kur'an'ın kendi kontrollerine karşı sırası p_Q için merkez **10⁻⁷**, aralık **10⁻⁹–10⁻⁶**.
- 200 plasebo kitabın p_x medyanı **10⁻³–10⁻¹**, en küçük p_x **10⁻⁶–10⁻⁴**.
- Kur'an'ın ayrışması 200 plasebonun **hepsini aşar** (kalibre p = 1/201).

## Ölçümler

**Ö0 — Çapa.** Kütüphane özgün 20 yükleme sınırlandırılınca maskeler on ikinci denetimin `src/butunluk.h`'si ile bit-bit aynı olmalı: Kur'an'da ve her sıfırdan ≥ 1.000 rastgele kitapta, 406 mercekte. Kur'an'ın el dili profili de aynı çıkmalı: (1,+1,19)'da 15 yüklem. Örnekleyiciler (U, M20, Mallows) on ikinci denetiminkilerle aynı tohumda bit-bit aynı olmalı. Yeni referans tablosuyla Kur'an'ın el dilindeki bilgi puanı, on ikinci denetimin değerine tablo gürültüsü içinde eşit olmalı (M20 27,41; U 31,06 bit). Tutmazsa sebebini bul ve yaz; Ö1'e geçmeden önce çöz.

**Ö1 — Kütüphane L\*.** Özgün 20 yüklemi aynen içerir. Kardeşler aynı dört motif türünde ve aynı düzeylerde, el dilindeki yüklemlerle benzer tarif uzunluğunda olur:
- (a) **Denge:** el dilindeki bölmeler, sütun yarıları (n-paritesine göre), köşegenler ve karşı ailenin (H) yarıları ve hücreleri; her biri için eşit sayı ve eşit toplam. Kitap düzeyinde eşit toplamın kardeşleri de eklenir (ör. Σ_{E⁰} a = Σ_{E¹} a, Σ_{E⁰} n = Σ_{E¹} n).
- (b) **Modülün geri dönüşü:** önceden listelenmiş doğal nicelikler için "= p", "= 2p", "merkez = p", "p | ·" ve "p² | ·" ilişkileri. Boylar: k_F, k_H, |U|, |F ∩ H|, yarı ve hücre boyları. Toplamlar: Σq, yarı, satır, sütun ve hücre toplamları, Σ_U a, Σ_U n, Σ_U t, Σ_F a, Σ_F n, Σ_H a, Σ_H n, Σ_H u/p ve kitap yarılarının toplamları. Her nicelik–ilişki çifti bir yüklemdir.
- (c) **İki el:** F ∩ H = ∅'nin yanında |F ∩ H| = 1, k_F = k_H ve k_F + k_H = p.
- (d) **Ardışıklık:** hücre toplamları ve katsayıların yanında karşı ailenin katsayı bandı, satır ve sütun toplamlarının ardışıklığı, hücre toplamlarının aritmetik dizi oluşu, hücre boylarının ardışıklığı ve sabit hücre (≥ 2 eşit q).

Hedef büyüklük 60–150 yüklemdir. Her özgün yüklemin nasıl genelleştirildiğini ve kaç kardeşi olduğunu tabloyla yaz. Sabit (permütasyonla değişmeyen) yüklem–mercek çiftleri on ikinci denetimin §2.6'sındaki gibi analitik olarak ve taramayla dışlanır. Gruplar (kitap; aile-içi | k_F; iki-el | k_F; gerekirse H-içi | k_H) ve grup içi tam birlikte sıklık on ikinci denetimdeki gibidir. Bölme çifti (t-paritesi, n-paritesi) birincilde sabittir. Gerekçe: on ikinci denetimin menüsünde her kitaba 20 bölme çiftinden seçme hakkı vermek sırayı yalnız ~2 kat değiştirdi. Bölme seçimi yalnız ikincildir (Ö5 f).

**Ö2 — Dil bedeli ve keşif yordamı D(x).** Dil bedeli c(Λ), önceden yazılmış bir tarif uzunluğudur (ör. düzey × tür × nicelik × ilişki kodunun bit uzunluğu). El dilinin 20 yükleminin aynı kuralla bedelini raporla. D(x) otomatik ve deterministiktir; **her hedefe aynen** uygulanır. Λ ⊆ L\* ve |Λ| ≤ 20 olmak üzere, S_x(Λ) − c(Λ)'yi en büyük yapan Λ'yı seçer. Burada S_x(Λ) = max_λ [I_Λ(λ) − c(λ)], yani on ikinci denetimin bilgi puanıdır (V0, mercek önseli P0, asallar eşit ağırlıklı). Arama yöntemini (tam, açgözlü ya da ışın) ve eşitlik bozmayı ön-kayıtta sabitle. D'ye ne Kur'an'ın metni, ne el dili, ne de herhangi bir sonuç girer.

**Ö3 — Hedefler.** Kur'an ile birincil sıfır M20'den bağımsız çekilmiş **K = 200** plasebo kitap; M20 hedefleri ayrı zincirlerden gelir. İkincil sıfır tekdüzeden K = 100. Bütçe kalırsa Mallows'tan K = 100; kalmazsa "ölçülmedi" yaz. Kodeks 129 birinciltir; 127 ölçülmez. Hedefler zenginliklerine göre süzülmez.

**Ö4 — Kontroller ve ayrışma.** Her hedefin dili Λ_x = D(x) donar. Kontroller aynı sıfırdan, referans örnekleminden ve hedeflerden bağımsız tohumla çekilir. Her kontrol kendi en iyi merceğini seçer; dil Λ_x sabittir. Ayrışma p_x = (k + 1)/(N_x + 1) ile ölçülür; k, Λ_x altında puanı hedefinkine eşit ya da büyük olan kontrol sayısıdır. Bu doğrudan sayımdır, kuyruk dış değerlemesi yapılmaz. Bütün hedefler ortak bir kontrol örneklemini paylaşabilir. Derinlik uyarlamalı olabilir: önce bütün hedeflerde N₁ (ör. 10⁶); k < 10 kalanlarda 10⁷, sonra 10⁸. Kur'an'da doğrudan ≥ 10⁸ kullanılır. Derinlik kuralı önceden yazılır. Çözülmemiş aramalar ve puan doyması için tek bir kural yazılır ve bütün hedeflerde aynen uygulanır.

**Ö5 — Kalibre cevap ve ikinciller.**
1. p_kal = (1 + #{plasebo : p_x ≤ p_Q})/(K + 1). Eşitlikler ve aralık çakışmaları Kur'an'ın aleyhine sayılır. Okuma kuralı (yukarıda) aynen uygulanır. Aradaki fark, önceden yazılmış bir modelle (ör. plasebo −log₁₀ p_x dağılımına kuyruk uyumu) ayrıca ve "model-tabanlı" etiketiyle verilir.
2. İkinciller:
   - (a) El dili plasebo kitaplara uygulanınca p_x: dil yazımının kazancı, kendi dili ile el dili arasındaki farktır.
   - (b) D(Kur'an) ile el dili arasındaki ortak yüklemler; Kur'an'ın el dilindeki ve otomatik dilindeki p'si.
   - (c) **Bütünlük şartlı ayrışma:** on ikinci denetimin §7'sindeki önceden yazılmış Kur'an sınıfı kuralı (kapı G; ≥ 3 motif türü; ≥ 3 düzey) kardeşlere tür ve düzey etiketleriyle genelleştirilir. Hedefte ve kontrolde, en iyi mercekte sınıf şartı aranır. Kur'an'ın ve plasebo kitapların bu ayrışmasını ve plasebo dillerinin kaçının sınıfa girdiğini raporla. Amaç, yüksek puanlı ama parçalı rakipleri bütünlüklü olanlardan ayırmaktır.
   - (d) Tekdüze (ve bütçe kalırsa Mallows) plasebo kalibrasyonu.
   - (e) Kur'an için metin ağırlıklı asal önseli (on ikinci denetimin P2'si) yalnız ek satır olarak verilir; plasebo kitapların metni yoktur.
   - (f) Bütçe kalırsa bölme seçimi kütüphaneye eklenir (menünün 5 bölmesi).

**Uygulama önerisi (bağlayıcı değil).** Referans geçişinde (sıfır başına 10⁸) her (mercek, bağlam, grup) için tam kütüphane maskelerinin seyrek sayımlarını tut. Herhangi bir Λ'nın tablosu bu sayımların izdüşümü ve üst-küme toplamıyla elde edilir. Kontrol geçişlerinde dilleri, bellek sınırına sığan gruplar halinde işle.

## Rapor

Rapor `PLASEBO12_RAPORU.md` adını taşır (Türkçe; sonunda İngilizce özet). Cevaplanacak sorular:
1. Kendi dilini kuran rastgele kitaplar kendi kontrollerinden ne kadar ayrışıyor? Kur'an bu ayrışmalar arasında nerede, kalibre p nedir?
2. D(Kur'an) el dilini buluyor mu? Dil yazımının kazancı ne kadar?
3. Bütünlük şartlı ayrışmada tablo değişiyor mu?
4. Ana hattın tahmini tuttu mu?
5. Ölçülemeyenler "ölçülmedi" diye yazılır, en azından şunlar: kütüphanenin sınırları; literatürün on yıllar süren araması; nesnenin (n, a) seçimi; sayım okulu; kültürel havuz; gruplar arası bağımsızlık yaklaşımının etkisi.

Raporu, ana hattın onu depoya ekleyeceğini ve bağımsız olarak doğrulayacağını bilerek yaz.
