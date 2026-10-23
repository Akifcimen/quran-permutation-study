# GÖREV — koşullu ölçüm: önceki literatürü taşıyan kitaplar arasında yeni yapı ne kadar sıra dışı?

**Veren:** araştırmacı (Akif Çimen), ana hat oturumu aracılığıyla · **Tarih:** 23 Ekim 2026

## Bağlam

`~/Desktop/hrf2` (github.com/Akifcimen/quran-permutation-study) Kur'an'ın sûre numaraları n ve ayet sayıları a üzerindeki bir aritmetik yapıyı inceler: t = n + a, u = n − a; terazi (57/57 parite, Σt(çift) = Σa), 12 üyeli t ≡ 0 (mod 19) ailesi (Σt = 1444), iç terazi 722/722, dört hücre (katsayı toplamları 18·19·19·20), 5–9 katsayı bandı, İki El (u ≡ 0 ailesi 7 üye, birleşim 19, 19 | Σa = 1159). Dokuz denetimden sonra durum README'deki merdivende ve `KESIF.md` dürüstlük notları 13–22'dedir.

Önce şunları oku: `README.md` (merdiven), `KESIF.md` (notlar 13–22), `denetim9_inceleme_ss/inceleme_ss.md` (dokuzuncu inceleme — bu görevin çıkış noktası), `denetim8_dis_inceleme/`, `denetim7_opus55_uc_olcum/GUNCELLEME_RAPORU.md`, `denetim6_opus55_guclendirme/` (robot kaynakları `src/`, tablo betikleri), `denetim5_opus55/KIRMA_RAPORU.md`, `kesif4_motor.c`, `kesif18c_kosullu_cekirdek_cokluk.py`, `kesif25_*`.

Dokuzuncu incelemenin üç tespiti bu görevi doğurdu:
1. **Silmek koşullamak değildir.** Önceki literatürdeki olguların puan düğümlerini herkes için kapatmak, "eski olgulara puan vermeyen bir ölçütte sıra"yı ölçer; "eski olguları zaten taşıyan kitaplar arasında yeni yapı ne kadar seyrek" sorusunu ölçmez. Doğru biçim P(yeni | eski).
2. **Robot cebirsel tekrarları ödüllendiriyor.** Örnek: G (u ≡ 0) ailesinde Σn = Σa ise Σq = 0 ve "m² | Σq" zorunlu tutar; robot birini G-çapraz, ötekini G-toplam düğümünde iki kez sayar. Denetçi yalnız bu tekrarı kaldırınca Kur'an'ı geçen/eşleyen rastgele kitaplar (129, 2×10⁶ tablo + ayrı tohumla 2×10⁶ değerlendirme) U 116 → 53, J10 1.135 → 573, M20 827 → 393 oldu. Yedinci oturum tam birlikte-olasılıkla puanlamada ×15–16 bulmuştu ama kodu kayboldu.
3. **Sabit olay çapası:** iki GPU kaydında (`runGPU_4090_seed42.txt`, `runGPU_3090_seed43.txt`) C (12 üye + 1444 + 6/6 + 722/722) = 296.340.752 + 148.173.181 = 444.513.933, G∩W (tam kristal ∧ İki El) = 41 + 21 = 62 → P(G∩W | C) = 1,39×10⁻⁷ (tekdüze, 127, arama bedeli yok).

Ayrıca: **J10 sıfırı gerçek dizilişin σ = 10 konumluk titreşimidir** (robot4_m.c: anahtar = konum + 10·Z, a = BASE[idx]); veriye merkezlidir. M20 yalnız uzunluk ölçüsünü kullanır (tam azalan diziden başlar, S = Σ n·a ≤ S_obs kısıtlı takaslar); birincil sıralı sıfır M20'dir.

## Kurallar

- **`~/Desktop/hrf2` salt okunurdur; hiçbir dosyasını değiştirme.** Bütün çalışma `~/opus5.5/kosullu` altında. Ana hattın hafızasına (`~/.claude/projects/<hrf2-proje>/memory/`) yazma; kendi hafızanı kendi proje dizininde tut.
- **Ölçmek için hatalı davranma.** Sonuç hangi yöne çıkarsa çıksın aynen yaz. Amaç nesneyi güçlendirmek ya da zayıflatmak değil, doğru soruyu doğru ölçmek.
- **Ön-kayıt:** hiçbir rastgele kitabın koşullu puanına bakmadan `ONKAYIT.md` yaz; SHA-256'sını ve UTC zamanını `sonuc/onkayit_ozet.txt`'e yaz, dosyayı salt okunur yap. Sapmalar (doğrulama başarısızlığı, bütçe kısıtı, hata düzeltmesi) ayrı, zaman damgalı ek dosyalarda.
- **Disk:** makinede yalnız ~6 GB boş. Toplam kullanımın 3 GB'ı geçmesin; büyük ham dizileri tutma, kuyruk özetleri ve sayaçlar tut.
- **Kayıt güvenliği:** yedinci oturumun klasörü kayboldu ve kodu kurtarılamadı. Bütün kodu, betikleri, ön-kayıtları ve küçük çıktıları bu klasörde tut. İş bitince `paket/kosullu_paket.tar.gz` (≤ 200 MB: kod + koşu betikleri + sonuçlar + rapor; büyük tablolar hariç, tohumlarla yeniden üretilebilir olmalı) üret ve SHA-256'sını yaz.
- Makine başka işlerle paylaşılıyor; uzun koşular kontrol noktalı ve sürdürülebilir olsun. Her sayının yanında sıfırı, örneklemi, isabet sayısını ve belirsizliği yaz.
- Birincil sayım **Tevbe = 129** (toplam 6236). GPU çapaları 127 ile koşulduğu için çapa karşılaştırmalarında 127 de koşulur.

## Ölçümler

**Ö0 — Tekrarların temizlenmesi.**
(a) Dokuzuncu incelemenin deneyini yeniden üret (robot4_m, defter bataryası, matris kapalı, sabit mercek (1,1,19); U, J10, M20; 129 ve 127).
(b) Bataryada **mantıksal olarak birbirini gerektiren** bütün olgu çiftlerini/gruplarını sistematik bul (yalnız kesin gerektirme; örneklemden değil, tanımdan ya da bütün kitaplarda geçerli bir kanıtla). Listeyi yaz.
(c) Bunları birleştiren "tekrarsız puan"ı ön-kayıtta tanımla (gerektirilen olgu, gerektirenin bulunduğu hikâyede ek puan almaz). Ö1 bu puanı kullanır.

**Ö1 — Koşullu kalibrasyon (asıl ölçüm).**
- Mercek sabit (n+a ve n−a, modül 19): devralınmış sayılır, bedeli bu ölçümün konusu değildir.
- Önceki literatür (kaynaklar `KESIF.md` not 20): O1 = 12 üyeli t ≡ 0 ailesi ve Σt = 1444 (1990); O2 = 6/6 ve 722/722 (en geç 2015); O3 = tek-t kolunun iki hücresinin her biri 361 (en geç 2015); O4 = terazi (1994). **Birincil koşul O = O1 ∧ O2 ∧ O3**; ikincil O' = O ∧ O4.
- Sıfırlar: **U** (tekdüze), **M20** (birincil sıralı), **Mallows** (merkez: ayet sayılarının büyükten küçüğe sırası; I(π) bu sıraya aykırı kesin çift sayısı, eşit değerler terslik sayılmaz; q yalnız beklenen terslik sayısı gözlenene eşit olacak şekilde ayarlanır, başka hiçbir şeye bakılmaz). J10 yalnız ikincil, "veriye merkezli" etiketiyle.
- Her sıfırda O'yu sağlayan rastgele kitaplar üret: ret örneklemesi ya da yansız önem örneklemesi. MCMC kullanılacaksa karışması bağımsız çapalarla doğrulanmalı (yedinci oturumda alt-küme benzetimi kesin-toplam kısıtlarında dondu). Hedef: sıfır başına ≥ 10⁵ koşullu kitap; ulaşılamazsa ulaşılan sayı yazılır.
- **Puan:** her koşullu kitap yalnız O'nun (ikincilde O''nün) gerektirmediği olgulardan kendi en iyi hikâyesini kurar (aynı defter bataryası, matris kapalı; O'nun gerektirdiği olgular herkes için kapalı; olgu olasılıkları koşullu sıfırdan tahmin edilir; Ö0(c)'nin tekrarsız puanı). Kur'an'ın sırası p = (k+1)/(N+1). Kur'an'ı geçen ya da ona yakın kitaplar için ayrıca **tam birlikte-olasılık** (koşullu sıfırda doğrudan sayım) ile sıra.
- **Çapa:** sabit olay P(G∩W | O1∧O2) tekdüzede 1,39×10⁻⁷ (yukarıdaki GPU sayaçları) ile tutmalı; tutmazsa sebebini bul ve yaz.

**Ö2 — Tam çekirdeğin sıralı sıfırlarda doğrudan ölçümü.** A∩G∩W (tanımlar `kesif4_motor.c`, `kesif18c`) M20 ve Mallows altında, aktarım yapmadan. Çapalar: tekdüzede 1,1×10⁻¹⁵ [0,5–1,7] (bağımsız 9,05×10⁻¹⁶); kristal G'nin sıralı sıfırdaki şişmesi ×88–160 (M20/J10, 5. denetim), Mallows ×126 (8. denetim).

## Rapor

`KOSULLU_RAPORU.md` (Türkçe; sonunda İngilizce özet). Üç soruyu ayrı cevapla: (1) beyan edilen tam olay ne kadar seyrek; (2) bu çalışma önceki literatüre ne ekledi (Ö1); (3) seçim ve arama bedelleri ödenince ne kadar kanıt kalır — bunu ölçemediğin yerde "ölçülmedi" yaz. Farklı senaryoları bir aralığın uçları gibi sunma; ×7 okul çarpanı gibi üst sınırları sıklık gibi okuma. Ana hattın bu raporu depoya ekleyeceğini bilerek yaz.
