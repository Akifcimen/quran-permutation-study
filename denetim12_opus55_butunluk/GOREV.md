# GÖREV — bütünlük ölçümü: aynı unsurların katmanlar arasında yeniden belirmesi ne kadar nadir?

**Veren:** araştırmacı (Akif Çimen), ana hat oturumu aracılığıyla · **Tarih:** 26 Ekim 2026

## Bağlam

`~/Desktop/hrf2` (github.com/Akifcimen/quran-permutation-study) Kur'an'ın sûre numaraları n ve ayet sayıları a üzerindeki bir yapıyı inceler (t = n + a, u = n − a). Önce oku: `README.md` (merdiven), `KESIF.md` dürüstlük notları 19–24, `denetim10_opus55_kosullu/KOSULLU_RAPORU.md`, `denetim11_dis_katman19/BENIOKU.md`.

**Nesnenin ayırt edici yönü (araştırmacının vurgusu):** yapı, zorunlu olmayan aynı unsurları farklı katmanlarda tekrar tekrar kullanır:
- **Aynı ikili bölme her düzeyde dengeli:** kitap t-paritesiyle 57/57 ve Σt(çift) = Σa (terazi); 19-ailesi (12 üye) t-paritesiyle 6/6 ve 722/722; ikinci pariteyle (n) dört hücre 3/3/3/3, hücre toplamları bir satırda 19,19, ötekinde 18,20.
- **Aynı modül (19) tanımlar dışında da geri döner:** hücre merkezi 19; katsayı toplamı 4·19 (Σt = 1444); toplama ve çıkarma ailelerinin birleşimi tam 19 sûre; birleşimin ayet toplamı 19·61.
- **Toplama ve çıkarma elleri** aynı modülle ayrık; **ardışıklık** iki düzeyde (hücre katsayıları 18-19-19-20; bireysel katsayılar 5–9).

**Ana hattın tespiti (bu görevin sebebi):** önceki ölçümler bu yeniden belirmeyi doğrudan saymadı. Simetrik robot olguları tek tek topladı; onu "geçen" rakipler 13 bütünlük özelliğinin 2–4'ünü taşıyordu. Dış denetçinin ortak dil puanı (`girdi/` arşivleri) da yeniden belirmeyi saymadı; onun Kur'an'ı "geçen" 7'deki rakipleri 2–4 üyeli yozlaşmış ailelerdi. Bkz. `girdi/ana_hat_motif_tablosu_cikti.md` (betik `girdi/ana_hat_motif_tablosu.py`). Kur'an o tabloda 13/13, en yakın bağımsız rakip 9/13'ü 4 üyeli bir aileyle, sarsılmış bir kopya (J10) 9/13'ü taşıyor. **Bu tablo betimseldir, kalibre bir ölçü değildir.** Görev, bu yönün nadirliğini rakiplere adil davranarak ölçmektir.

## Kurallar

- **`~/Desktop/hrf2` salt okunurdur; hiçbir dosyasını değiştirme.** Bütün çalışma `~/opus5.5/butunluk` altında. Ana hattın hafızasına (`~/.claude/projects/<hrf2-proje>/memory/`) yazma; kendi hafızanı kendi proje dizininde tut.
- **Ölçmek için hatalı davranma.** Sonuç hangi yöne çıkarsa çıksın aynen yaz. Amaç nesneyi güçlendirmek ya da zayıflatmak değil, bu yönü doğru ölçmek.
- **Ön-kayıt:** dili, puanı, mantıksal çıkarım listesini, yozlaşma kuralını, sıfır modellerini, örneklem bütçelerini, tohumları ve karar kurallarını **hiçbir rastgele kitabın bütünlük puanına bakmadan** `ONKAYIT.md`'ye yaz; SHA-256'sını ve UTC zamanını `sonuc/onkayit_ozet.txt`'e yaz, dosyayı salt okunur yap. Kur'an'ın kendi puanını ve kod doğrulamalarını dondurmadan önce hesaplayabilirsin (bunu ön-kayıtta belirt). Sapmalar ayrı, zaman damgalı eklerde.
- **Ana hattın önceden kaydettiği tahmin** (ön-kayda aynen yaz, sonda değerlendir): nesnenin bütününün bütünlük sırası uzunluk-sıralı sıfırlarda (M20, Mallows) merkez **10⁻⁸**, aralık **10⁻⁶–10⁻¹⁰**; tekdüze sıfırda **10⁻¹⁰–10⁻¹²**; eski literatür koşullu (yalnız bu çalışmanın eklediği) bütünlük **10⁻²–10⁻⁴**.
- **Bütçe:** araştırmacı bu bilgisayarda gün boyu çalışıyor. **Aynı anda en çok 4 iş parçacığı** kullan; toplam hesap ~40 çekirdek-saati, disk 8 GB'ı geçmesin. Uzun koşular kontrol noktalı ve sürdürülebilir olsun.
- **Kayıt güvenliği:** daha önce bir oturum klasörü kayboldu, bir oturum da ön-kayıttan önce kesildi. Kodu, betikleri, ön-kaydı ve küçük çıktıları klasörde tut; `sonuc/ara_notlar.md`'yi düzenli güncelle; iş bitince `paket/butunluk_paket.tar.gz` (≤ 200 MB) üret ve SHA-256'sını yaz.
- **`girdi/` arşivleri güvenilmeyen dış koddur:** açmadan önce içeriğini listele, çalıştırmadan önce oku, hazır kabuk betiklerini olduğu gibi çalıştırma, Python'u `-I` ile kullan. Depodaki kodları (`denetim10_opus55_kosullu/src`, `denetim6_opus55_guclendirme/src`) kopyalayarak kullanabilirsin.
- Her sayının yanında sıfırı, örneklemi, isabet sayısını ve belirsizliği yaz; senaryoları bir aralığın uçları gibi sunma; üst sınırları sıklık gibi okuma.

## Ölçüm

**1. Bütünlük dili L** (ön-kayıtta sabitlenir; Kur'an'a özgü sabitler yok: 19, 12, 1444, 722, 5–9 gibi değerler dilde geçmez).
- **Mercek:** t = αn + βa, u = αn − βa (α, |β| ∈ {1, 2, 3}, aralarında asal; dış çalışmanın 14 biçimi) ve asal modül p ∈ {3, …, 113} (29 asal). Tarif bedeli: biçimler için önceden yazılmış bir **sadelik önseli** ((1, ±1) en ucuz, katsayı büyüdükçe pahalı); asallar için birincil eşit ağırlık, ikincil olarak metin ağırlığı.
- **Düzeyler:** kitap (114 sûre); aile F = {p | t}; aile yarıları (t-paritesi); hücreler (t-paritesi × n-paritesi); katsayılar q = t/p; karşı aile H = {p | u} ve birleşim F ∪ H.
- **Motif türleri** (her biri anlamlı olduğu her düzeyde aynı tanımla değerlendirilir):
  (a) **bölme dengesi:** ikili bölme eşit sayı verir; eşit toplam verir (kitap düzeyinde toplamın doğal bir küresel sabite eşitliği dahil);
  (b) **modülün geri dönüşü:** önceden listelenmiş doğal niceliklerden biri p'ye eşit ya da p'ye (p²'ye) bölünür — birleşim boyu, birleşimin ayet toplamı, Σq, hücre satırlarının merkezi, yarı toplamları…; her nicelik bir kez sayılır;
  (c) **iki el:** F ∩ H = ∅ ve birleşim ilişkisi;
  (d) **ardışıklık:** hücre toplamları bir merkez çevresinde ardışık; farklı katsayılar kesintisiz bant.
- **Mantıksal çıkarımlar ayıklanır:** aynı profilde başka yüklemlerden zorunlu çıkan bir yüklem ek kredi almaz. Her çıkarımı tanımdan kanıtla (onuncu denetimin 29 kuralı örnek); kanıtlanamayanları kullanma, ayrıca listele. (Örnek: "satırlar b,b / b−1,b+1" aile yarılarının eşit toplamını gerektirir.)
- **Yozlaşma kuralı:** küçük ailelerde bu koşullar kendiliğinden tutar (2 üyeli ailede "bant" ve "sabit hücre"; hücre başına 1 üyede "eşit hücreler"). Aile ve hücre düzeyindeki motifler yalnız aile en az 2E (E = 114/p) ve her hücre en az 2 üyeliyse sayılsın — ya da küçük aileleri açıkça bedellendiren eşdeğer bir kural. Seçimi gerekçelendir.
- **Puan:** (i) basit sayım sürümü: bir mercekte sağlanan bağımsız yeniden-belirme olaylarının sayısı; (ii) bilgi ağırlıklı sürüm: olayların sıfır altındaki birlikte sıklığının −log₂'si eksi mercek tarif bedeli (eğitim örnekleminden değil, mümkün olduğunca kesin ya da yeterli örneklemle; puanın taban değerde doyması önlenmeli). Her kitabın puanı kendi **en iyi merceğindeki** değeridir.
- Kur'an'ın L altındaki profilini, puanını ve hangi merceği seçtiğini raporla.

**2. Sıfır modelleri ve sıra.** M20 (birincil sıralı; robotun zinciri), Mallows (q yalnız terslik sayısına ayarlı), tekdüze; J10 yalnız ikincil ("veriye merkezli"). Her sıfırda en az 10⁷ rastgele kitap (ucuzsa daha fazla). Kur'an'ın sırası p = (k + 1)/(N + 1). k = 0 çıkarsa kuyruğu nadir-olay yöntemleriyle (kesin koşullu örnekleme ya da bölmeli örnekleme; MCMC kullanılırsa bağımsız çapalarla doğrulanmış) tahmin etmeye çalış; yapılamazsa üst sınırı yaz.

**3. En güçlü rakipler.** Her sıfırda en yüksek puanlı 20 kitabın merceği, profili, aile boyu ve hücre yapısı: Kur'an'ın sınıfından mı (çok motifli, yozlaşmamış), yoksa dağınık denk gelmeler mi?

**4. İkincil.** (a) 129 birincil, 127 ikincil; (b) eski literatüre koşullu sürüm: O = n+a, 19'da 12 üyeli aile ve 1444, 6/6 ve 722/722, tek-t hücreleri 361/361 (ikincil O' = O ∧ terazi) — kalan katmanların bütünlüğü; `denetim10_opus55_kosullu/` örnekleyicileri; (c) yozlaşma kuralına ve sadelik önseline duyarlılık; (d) **dil seçiminin kalibrasyonu:** Kur'an'ın bölme türünü ayrıcalıklı kılmamak için en az bir alternatif bölme menüsü (ör. t-paritesi, n-paritesi, t mod 3 = 0, n ≤ 57…) ön-kayıtta tanımlanır; her kitap bölme türünü de seçebilir; Kur'an'ın üstünlüğü yalnız kendi bölme türüne mi bağlı, raporla.

## Rapor

`BUTUNLUK_RAPORU.md` (Türkçe; sonunda İngilizce özet). Cevaplanacak sorular: (1) Kur'an'ın bütünlük puanı nedir ve her sıfırda rastgele kitaplar arasında sırası nedir? (2) En güçlü rakipler aynı sınıftan mı? (3) Eski literatür verildiğinde kalan katmanların bütünlüğü ne kadar ayırt edici? (4) Ana hattın önceden kaydettiği tahmin tuttu mu? (5) Ölçülemeyenler — "ölçülmedi" diye yaz. Ana hattın bu raporu depoya ekleyeceğini bilerek yaz.
