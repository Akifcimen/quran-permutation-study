# GÖREV — bütünlük plasebosu: her hedef kendi bütünlüklü dilini kursa, Kur'an'ın bütünlüğü ne kadar sıra dışı?

**Veren:** araştırmacı (Akif Çimen), ana hat oturumu aracılığıyla · **Tarih:** 29 Ekim 2026 · araştırmacı onayladı ve başlattı

## Bağlam

`~/Desktop/hrf2` (github.com/Akifcimen/quran-permutation-study) Kur'an'ın sûre numaraları n ve ayet sayıları a üzerindeki bir yapıyı inceler. Önce oku:
- `README.md` (merdiven);
- `KESIF.md` notları 24–26;
- `denetim12_opus55_butunluk/BENIOKU.md`;
- `denetim13_opus55_plasebo/BENIOKU.md`, `PLASEBO12_RAPORU.md` ve `ONKAYIT.md`.

**On üçüncü denetim (plasebo12, `~/opus5.5/plasebo12`)** dil yazmayı otomatik bir yordama çevirdi. Kütüphane L\* 109 yüklemdir. Yordam D her hedef için bilgi − dil bedeli − mercek bedelini en büyük yapan dili seçer, Kur'an'a ve rastgele kitaplara aynen uygulanır. Her hedef kendi diliyle kendi kontrollerine karşı sıralanır.
- **Sonuç:** sıralı sıfırlarda Kur'an bütün plasebolardan daha çok ayrıştı (kalibre p M20'de 1/201, Mallows'ta 1/101; tekdüzede 2/101).
- **Ama:** dil bedeli altında D, Kur'an için yalnız 3 yüklem seçti (terazi, ardışık hücre toplamları, merkez 19; iki düzeyden). Plasebolara ortalama 1 yüklem seçti.
- **Bu yüzden bütünlük sınanamadı.** Araştırmacının ana ölçütü, zorunlu olmayan aynı unsurların katmanlar arasında yeniden belirmesi, kalibre edilmedi. Ön-kayıtlı bütünlük şartlı satır tanımsız kaldı.

**Bu görevin sorusu:** her hedef kendi en iyi **bütünlüklü** dilini kursa, yani kendi en iyi merceğinde en az üç düzeye ve üç motif türüne yayılan bir yapı seçmek zorunda olsa, Kur'an'ın bütünlüklü ayrışması plasebo kitapların bütünlüklü ayrışmaları arasında nerede kalır?

L\*'ın bütün yüklemleri bir mercekte aynı modülü (p) ve aynı ikili bölmeyi (t-paritesi) kullanır. Bu yüzden tek bir mercekte üç düzeye yayılan bir yapı, aynı unsurların üç katmanda yeniden belirmesidir.

**Bu test, ilk plasebonun sonucunu gördükten sonra tasarlandı.** Önceden kaydedilir, iki sonuç yan yana yazılır ve ikincisi birincinin yerine geçmez.

**Okuma kuralı** (not 25'teki kuralın bu teste uygulanışı; ön-kayda aynen yaz, değiştirme):
> Kur'an'ın bütünlüklü ayrışması bütün plasebo kitaplarınkini aşarsa, nesnenin bütünlüğünün sırası dilin Kur'an'a göre yazılmasıyla açıklanamaz; kalibre p en küçük ~1/(K + 1) olabilir (K, plasebo kitap sayısı) ve aradaki fark ayrıca, model-tabanlı olarak verilir. Plasebo kitaplar Kur'an'ın bütünlüklü ayrışmasına sık ulaşırsa (R/K ≥ 0,05), bütünlüğün nadirliği büyük ölçüde dilin Kur'an'a göre yazılmasından gelir ve öyle yazılır. Arada kalan sonuçlar kalibre p'leriyle aynen yazılır.

## Kurallar

- **Salt okunur:** `~/Desktop/hrf2`, `~/opus5.5/butunluk`, `~/opus5.5/kosullu`, `~/opus5.5/plasebo` ve `~/opus5.5/plasebo12`. Hiçbir dosyalarını değiştirme.
  - Kod kopyalanarak kullanılabilir.
  - plasebo12'nin referans tabloları (`tab/<sıfır>/ref_*.ref`) ve hedef dosyaları (`tab/<sıfır>/hedef.bin`) yerinde okunarak yeniden kullanılabilir. Önce SHA-256'larını `~/opus5.5/plasebo12/sonuc/ref_sha256.txt` ve `hedef_sha256.txt` ile doğrula.
  - Bütün çalışma `~/opus5.5/plasebo14` altında yapılır.
  - Ana hattın hafızasına (`~/.claude/projects/<hrf2-proje>/memory/`) yazma; kendi hafızanı kendi proje dizininde tut.
- **Ölçmek için hatalı davranma.** Sonuç hangi yöne çıkarsa çıksın aynen yaz. Amaç nesneyi güçlendirmek ya da zayıflatmak değil, bütünlüğün nadirliğini doğru ölçmek.
- **Ön-kayıt:** şunları **hiçbir hedefin bu görevin dilleriyle kurulan sonucuna ya da kontrol puanına bakmadan** `ONKAYIT.md`'ye yaz:
  - bütünlük kuralı, D'nin bütünlük kısıtlı arama yöntemi, kollar;
  - hedefler, bütçeler, derinlik kuralı, tohumlar;
  - ayrışma istatistiği, çözülmemiş aramaların kuralı, model-tabanlı fark modeli, karar kuralları.

  plasebo12'nin yayımlanmış sonuçlarını biliyorsun. Bu bilgi hiçbir tanımı Kur'an'a ya da belirli bir plaseboya göre ayarlamak için kullanılmaz. Kural, kütüphane ve bedeller plasebo12'deki gibi kalır; değişen yalnız bütünlük kısıtıdır.

  Dondurmadan önce izin verilenler: kod doğrulamaları (yalnız uyuşmazlık sayıları), plasebo12 ile eşitlik sınamaları, hız ölçümü. Ön-kaydın SHA-256'sını ve UTC zamanını `sonuc/onkayit_ozet.txt`'e yaz, dosyayı salt okunur yap. Sapmalar `ek/` altında, ayrı ve zaman damgalı eklerde.
- **Bütçe:** araştırmacı bu bilgisayarda çalışıyor (Apple M1, 8 çekirdek, 16 GB).
  - **Aynı anda en çok 4 iş parçacığı ya da süreç.** Analiz betikleri dahil, 4 süreçli bir iş koşarken başka hesap başlatma.
  - Toplam hesap tavanı 40 çekirdek-saat. Bellek en çok 6 GB, disk en çok 8 GB.
  - Uzun koşular kontrol noktalı ve sürdürülebilir olsun. Bütçe aşılacaksa yeni koşu başlatılmaz, eksik kalan yazılır.
- **Kayıt güvenliği:**
  - Kodu, betikleri, ön-kaydı ve küçük çıktıları klasörde tut; `sonuc/ara_notlar.md`'yi düzenli güncelle.
  - Derleme komutlarını baştan bir betikte tut.
  - İş bitince `paket/plasebo14_paket.tar.gz` (≤ 200 MB) üret ve SHA-256'sını yaz.
- **Raporlama disiplini:** her sayının yanında sıfırı, örneklemi, isabet sayısını ve belirsizliği yaz. Senaryoları bir aralığın uçları gibi sunma; üst sınırları sıklık gibi okuma.

## Ana hattın önceden kaydettiği tahmin (ön-kayda aynen yaz, sonda değerlendir)

Birincil kol A, birincil sıfır M20'de:
- Kur'an'ın bütünlüklü dilindeki bütünlük şartlı sırası p_Q için merkez **10⁻⁷**, aralık **10⁻⁹–10⁻⁶**.
- 200 plasebonun en az yarısında bütünlüklü bir dil bulunur.
- Plasebo p_x medyanı **10⁻³–10⁻¹**, en küçük p_x **10⁻⁶–10⁻⁴**.
- Kur'an'ın ayrışması 200 plasebonun **hepsini aşar** (kalibre p = 1/201).

## Ölçümler

**Ö0 — Çapa.** Kütüphane L\*, yüklem bedelleri, bilgi puanı ve kısıtsız D plasebo12'dekiyle aynı olmalı. plasebo12'nin kodunu kopyalayarak kullan.
- Kısıtsız D, plasebo12'nin üç sıfırdaki `diller.txt` dosyalarını bit-bit vermeli.
- Kur'an'ın plasebo12 eşikleri aynı çıkmalı.
- Bir kontrol bölütünün sayımları aynı çıkmalı.

Tutmazsa sebebini bul ve yaz; Ö1'e geçmeden önce çöz.

**Ö1 — Kol A (birincil): bütünlük kısıtlı dil.**
1. **Bütünlük kuralı B.** Bir kitap bir mercekte, dil Λ altında şu üç koşul tutarsa bütünlüklüdür:
   - (i) kapı G tutar: k_F ≥ 2E = 228/p ve her F hücresi ≥ 2 üyeli (yozlaşmamış aile);
   - (ii) Λ'nın orada sağlanan yüklemleri ≥ 3 motif türünden gelir;
   - (iii) aynı yüklemler ≥ 3 düzeyden gelir.

   Tür ve düzey etiketleri plasebo12 ön-kaydının §2.2–2.3'ündeki gibidir. Bu, plasebo12 ön-kaydının §9 c kuralıdır.
2. **Bütünlüklü puan.** Bir kitabın Λ altındaki puanı, yalnız B'nin tuttuğu mercekler üzerinden alınan en büyük [I_Λ(λ) − c(λ)] değeridir. Hiçbir mercekte B tutmuyorsa kitap o dilde puansızdır; bu durum hedef için p = 1, kontrol için "aşmaz" demektir.
3. **D_B(x):** |Λ| ≤ 20 olmak üzere, x'in kendi en iyi merceğinde B'yi sağlayan diller arasından S^B_x(Λ) − c(Λ)'yi en büyük yapan Λ seçilir.
   - Amaç negatif olsa bile seçilir: bütünlük zorunludur ve her hedefe kendi en iyi bütünlüklü dili verilir.
   - Böyle bir dil yoksa hedefin dili boştur (p_x = 1); bunun sıklığını raporla.
   - Arama tam olmalı: düzey ve tür kapsamını grup birleştirmesinde taşı. Yaklaşık bir adım gerekirse önceden yaz ve devreye girdiği yerleri raporla.
4. **Hedefler:** plasebo12'nin hedefleri: M20'den Kur'an + 200 (birincil), tekdüze ve Mallows'tan Kur'an + 100. Aynı hedefleri kullanmak iki kalibrasyonu kitap kitap karşılaştırmayı sağlar.
5. **Kontroller ve ayrışma** plasebo12'deki gibidir: derinlik kuralı 10⁶ / 10⁷ / 10⁸, Kur'an'da doğrudan 10⁸. p_x = (k + 1)/(N + 1), k bütünlüklü puanı hedefinkine eşit ya da büyük olan kontrollerdir.
   - Kalibre p = (1 + R)/(K + 1). R, ya p_x ≤ p_Q olan ya da aralığı Kur'an'ınkiyle çakışan plasebolardır; eşitlikler ve çakışmalar Kur'an'ın aleyhine sayılır.
   - Model-tabanlı fark ayrıca ve etiketiyle verilir.

**Ö2 — Kol B (ikincil): bedelsiz en geniş dil.** Dil bedeli c(Λ) = 0 alınır; D, hedefin bilgi puanını en büyük yapan merceğinde sağladığı L\* yüklemlerinin hepsini seçer (en geniş dil). Bütünlük kuralı uygulanmaz. Soru şudur: her kitap sahip olduğu her şeyle tarif edilse, Kur'an'ın tarifi rastgele kitapların tariflerinden ne kadar seyrek? Önce M20; bütçe kalırsa tekdüze ve Mallows.

**Ö3 — Duyarlılık (yalnız M20, bütçe kalırsa):**
- Kol A−: (ii) ve (iii)'te ≥ 2 tür ve ≥ 2 düzey.
- Kol A+: 4 tür ve 4 düzey.

**Ö4 — Betimseller:**
- Her kolda Kur'an'ın dili ve plasebo12 diliyle farkı.
- Plasebo dillerinin boy, düzey ve tür dağılımı.
- En iyi beş plasebonun yapısı (k_F, hücreler, mercek, yüklemler).
- Kol A'da plasebo12'ye göre kitap kitap değişim.

## Ana hattın planı (bilgi için; oturumun kararı değildir)

Kol A sıralı sıfırlarda "aşar" çıkarsa README'de nesnenin bütünlüğü, kalibre p'siyle birlikte, deponun ana okumasına alınacak. Plasebolar sık ulaşırsa bütünlüğün nadirliği dil yazımına bağlanacak ve öyle yazılacak.

## Rapor

Rapor `PLASEBO14_RAPORU.md` adını taşır (Türkçe; sonunda İngilizce özet). Cevaplanacak sorular:
1. Kendi bütünlüklü dilini kuran rastgele kitaplar ne kadar ayrışıyor? Kur'an nerede, kalibre p nedir (her sıfır)?
2. Kaç plasebonun bütünlüklü bir dili var, bu diller neye benziyor?
3. Bedelsiz en geniş dilde tablo nedir?
4. Duyarlılık kolları sonucu değiştiriyor mu?
5. Ana hattın tahmini tuttu mu?
6. Ölçülemeyenler "ölçülmedi" diye yazılır, en azından şunlar: kütüphanenin ve bütünlük kuralının sınırları (kural Kur'an'ın biçimi bilinerek yazıldı); literatürün araması; nesnenin (n, a) seçimi; sayım okulu; gruplar arası bağımsızlık yaklaşımı.

Raporu, ana hattın onu depoya ekleyeceğini ve bağımsız olarak doğrulayacağını bilerek yaz.
