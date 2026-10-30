# denetim14 — on dördüncü denetim: bütünlük zorunlu plasebo, kapı duyarlılığı ve Kur'an'ın derin sayımı (29 Ekim 2026)

**Önceki durum.** On üçüncü denetimde dil yazmak otomatik bir yordama (D) çevrilmişti. D, Kur'an'a ve 400 rastgele kitaba aynen uygulandı. Kur'an sıralı sıfırlarda bütün plaseboları aştı. Ama D, Kur'an için yalnız üç yüklem seçti ve hiçbir hedefe bütünlüklü bir dil kurmadı. Sırayı üç yüklemlik bir çekirdek taşıyordu; bütün kalibre edilmemişti (not 26).

**Bu denetim üç parçadır:**
1. **plasebo14** (bu klasörün kökü). Her hedef kendi en iyi **bütünlüklü** dilini almak zorundadır: kendi en iyi merceğinde en az üç düzeye ve üç motif türüne yayılan bir dil (Kol A). Görevi araştırmacı ana hat aracılığıyla verdi (`GOREV.md`). Ayrı bir klasörde ve ayrı hafızayla çalışan bir Claude Opus 5.5 oturumu (en yüksek çaba) uyguladı. Depoyu yalnız okudu.
2. **Kapı çalışması** (`ana_hat/kapi/`). plasebo14'ün bütünlük kuralının içindeki kapı G (aile boyu k_F·p ≥ 228 ve her hücre ≥ 2) Kur'an'ın tam sınırındaydı: 12·19 = 228. On ikinci denetim bu kapıyı tam bu yüzden birincil puandan çıkarmıştı. Ana hat plasebo14'ün görevini yazarken onu kuralın içinde bıraktı. Bu ana hattın tasarım hatasıydı. Kol A, Kur'an'dan bağımsız kapılarla tekrarlandı: Gc (yalnız her hücre ≥ 2; birincil, sonuçlardan önce sabitlendi), GE (k_F ≥ E) ve G0 (kapı yok).
3. **Kademeli ölçüm** (`ana_hat/kademe/`). plasebo14'te Kur'an'ın kendi sırası 0 / 10⁸ çıkmıştı, yani yalnız bir üst sınırdı. Aynı dil ve eşikle kontrol sayısı 10¹⁰'a çıkarıldı.

İkinci ve üçüncü parçayı ana hat yaptı, araştırmacının izniyle ofis workstation'ında. Kod farkını sonuçları bilmeyen ayrı bir ajan denetledi. Bulduğu hata (Mallows örnekleyicisinde iş parçacığı yarışı) düzeltildi ve bütün Mallows sayımları yeniden yapıldı. Aşağıdaki Mallows sayıları düzeltilmiş sayımdandır.

## Sonuç

**Plasebo kalibrasyonu (Kol A).** Kalibre p = (1 + R)/(K + 1). R, Kur'an'a ulaşan ya da aralığı Kur'an'ınkiyle çakışan plasebo sayısıdır. K = 200 (M20), 100 (Mallows, tekdüze).

| Kapı | M20 | Mallows | Tekdüze | dilli plasebo (M20 / Mallows / tekdüze) |
|---|---|---|---|---|
| G2E (plasebo14) | **1/201** aşar | **1/101** aşar | **1/101** aşar | 35 / 15 / 18 |
| **Gc** (birincil) | **1/201** aşar | **1/101** aşar | 2/101 arada | 199 / 99 / 99 |
| GE | 1/201 aşar | 1/101 aşar | 2/101 arada | 195 / 99 / 97 |
| G0 (kapı yok) | 1/201 aşar | 1/101 aşar | 3/101 arada | 200 / 100 / 100 |

- **Kur'an'ın Kol A dili** her sıfırda ve her kapıda aynıdır: {B2, K1, Qc, WSum} @ (1,+1,19). Bunlar terazi, ardışık hücre toplamları, hücre merkezinin 19 olması ve iki elin birlikte tam 19 sûre tutmasıdır. Dört yüklem dört türe (denge, modülün dönüşü, iki el, ardışıklık) ve üç düzeye (kitap, hücre, birleşim) yayılır.
- **Kapının etkisi büyük ama tek yönlü.** G2E'de plaseboların %82–85'i bütünlüklü dil kuramıyordu. Gc'de neredeyse hepsi kuruyor ve Kur'an'a yaklaşıyor: M20'de en yakın plasebo 1,1×10⁻⁶'dan 2,5×10⁻⁷'ye geliyor. Yine de sıralı iki sıfırda hiçbiri yetişmiyor; kapı tamamen kalkınca da yetişmiyor.
- **Tekdüzede "arada".** Her kapıda aynı plasebo yetişiyor: A088, mercek (1,+3,3). Bu mercekte aile {3 | n}'dir ve dizilişe hiç bağlı değildir. On ikinci denetimin p = 3 dışlama listesi bu plasebonun üç yüklemini kapsamıyordu. G0'da buna iki hücresi boş, 5 üyeli bir aile (p = 43) eklenir.
- **Kol B** (bedelsiz en geniş dil, bütünlük kuralı yok): 3/201, 3/101, 3/101, "arada". Kol A'daki "aşar" bütünlük şartıyla gelir.

**Kur'an'ın kendi sırası, kendi Kol A dilinde.** Her rastgele kitap 406 mercek arasından kendi en iyi merceğini seçer. %95 aralık köşeli ayraçta.

| Kapı, N | M20 | Mallows | Tekdüze |
|---|---|---|---|
| **Gc, 10¹⁰** (k ≥ 10, ölçüldü) | **47 → 4,7×10⁻⁹** [3,5; 6,3] | **22 → 2,2×10⁻⁹** [1,4; 3,3] | **12 → 1,2×10⁻⁹** [0,6; 2,1] |
| G2E, 10¹⁰ (kademe S3) | 4 → 4×10⁻¹⁰ | 1 → 1×10⁻¹⁰ | 0 → < 3,7×10⁻¹⁰ |
| Merceğin asalı 19'a sabit, Gc (sonradan; ön kayıtta yok) | 3 → 3×10⁻¹⁰ | 2 → 2×10⁻¹⁰ | 0 → < 3,7×10⁻¹⁰ |

- Gc'de isabetlerin çoğu p = 13'tedir: M20'deki 47'nin 34'ü.
- p = 19'da Kur'an düzeyine ulaşan beş kitap Kur'an'ın kopyası değildir: Kur'an'la ortak konum sayısı 4–9. Hiçbiri Kur'an'ın olgularının hepsini taşımaz. En yakını yalnız Qa'yı, yani iki elin tuttuğu 19 sûrenin ayet toplamının 19'a bölünmesini taşımaz.
- Metnin andığı asalların hepsi {3, 5, 7, 11, 19} serbest bırakılınca sayılar 6 / 7 / 2 olur.

**Ön-kayıtlar.**
- plasebo14: `ONKAYIT.md`, 2026-10-29T04:38:14Z, SHA-256 `6080c6b17c4bf38184c11482b3931d4804633ee23d190d69d252e03926e1f748`.
- Kademe: `ana_hat/kademe/ONKAYIT.md`, 09:11:16Z, SHA-256 `db06c6f9f664262811ad6270c9de3188b4218fea3ecc8237b8d54076ab238bd6`. Bu bir ilk değerdir; bu kopya ondan bir genellemeyle ayrılır, aşağıya bakın.
- Kapı: `ana_hat/kapi/ONKAYIT.md`, 09:41:23Z, SHA-256 `6d8db06b45532c28568b71bbd92906999b60f06f6840ab32509480ca9c8b2e09`.

Tahminler sonuçlardan önce yazıldı. Ana hat Gc'de en olası okuma olarak "arada"yı bekliyordu, sonuç "aşar" çıktı. Sayılar tahmin aralıklarının içinde kaldı (kapı raporu §0.5). Sapmalar `ek/` altında zaman damgalıdır. Hiçbiri bir tanımı ya da okuma kuralını değiştirmez.

## Ana hattın kontrolleri

| Kontrol | Sonuç |
|---|---|
| plasebo14'ün ham bölüt dosyalarından yeniden sayımı (`ana_hat/dogrulama_p14/yeniden_say14.py`; oturumun `analiz_b.py`'si kullanılmadı) | sekiz kol-sıfır satırının hepsi birebir aynı |
| Kapı çalışmasının yeniden sayımı (`ana_hat/kapi/dogrulama/yeniden_say_kapi.py`) | §0.2 tablosu bu sayımdır |
| Dillerin bağımsız Python yazımıyla sınanması (`plasebo_dil_sina.py`) | her kapı ve sıfırda Kur'an ile en yakın üç plasebo: 36 / 36 |
| Kur'an tarafı isabetleri | Gc 10¹⁰'un bütün aday kayıtları donmuş tam yolla yeniden puanlandı: mercekler aynı, puan farkları ≤ 10⁻⁶. İsabetler tanımlardan sınandı: 0 uyuşmazlık |
| Kod denetimi (sonuçları bilmeyen ayrı bir ajan) | kod farkı yalnız üç amaçlanan değişiklik; `kontrol_h` kısayolu tam yolla kitap kitap aynı. **Bulgu:** Mallows iş parçacığı yarışı, aşağıda |
| Mallows yarışı | Kurulum küresel bir diziyi kilitsiz yeniden yazıyordu. Kilitle düzeltildi; düzeltilmiş sayaç tek iş parçacıklı sayımla birebir aynı. Bütün Mallows sayımları yeniden yapıldı. Özgün koşularda bozulan bölütler: Kur'an tarafı 13/40.000 (Gc), 14/4.000 (GE), 3/4.000 (G0); kademe 30/40.000; plasebo kolları 1–5/400; plasebo14 1/800; **13. denetim 0/400**. Değişen tek sayı Kur'an tarafı GE 10⁹ betimsel sayımı (3 → 2). Hiçbir kalibre p ve okuma değişmedi |
| Bu kopyadan yeniden üretim | kapı yeniden sayımı (üç kapı), plasebo14 yeniden sayımı ve Kur'an tarafı toplamları bu klasördeki dosyalardan birebir çıkıyor |

## Ana hattın okuması

- **Bütün kalibre edildi.** Not 26'nın açık bıraktığı soru şuydu: dilin bütünlüklü olması şart koşulunca ne olur? Cevap: sıralı sıfırlarda Kur'an, kendi bütünlüklü dilini kuran bütün plasebo kitapları aşar. Bu, Kur'an'ın sınırına denk gelen kapıya bağlı değildir. Tekdüzede sonuç "arada"dır; yetişen yapı dizilişe bağlı olmayan, yozlaşmış bir mercektir.
- **Kur'an'ın kendi sırası** kendi bütünlüklü dilinde kabaca iki yüz milyonda birdir (M20) ile sekiz yüz milyonda bir arasındadır (tekdüze). On milyar kontrolle ölçüldü. Bu sıra, dilin Kur'an'a göre yazılmasının bedelini içermez. O bedelin ölçüsü plasebo kalibrasyonudur ve 200 / 100 plasebo onu ancak 1/201 ve 1/101'e kadar ölçebilir.
- **Ölçülemeyenler.** Bütünlük kuralının kendisi (≥ 3 tür, ≥ 3 düzey) Kur'an'ın biçimi bilinerek yazıldı ve bedellendirilmedi. Malzemenin seçimi (sûre numarası, ayet sayısı, toplama) ve bu malzemede ilk gözlemleri bulan literatür araması da bedellendirilmedi. 19'a sabit sayım sonradan tanımlandı.

## Bu kopya

- **Kök.** plasebo14 oturumunun nihai paketinin açılmış hâlidir: `plasebo14_paket.tar.gz`, SHA-256 `0b271098748d264c5f4ce812e4e71b4e31a5bbfb83b9c0b61aee8aee80ca005a` (`paket/plasebo14_paket.sha256`). İçerik listesi `paket/plasebo14_icerik_sha256.txt`'tedir.
  - **Çıkarılanlar:** ikili aday dosyaları (`*.q`; çoğu boş) ve hedef/tarama dosyaları (`*.bin`). Liste, boyut, SHA-256 ve gerekçe `atlananlar.tsv`'dedir. Hepsi aynı tohumlardan yeniden üretilir.
  - **Yol genellemesi:** iki çıktı dosyasındaki (`sonuc/M/analiz_A_cikti.txt` ve `sonuc/M/duzeltme_oncesi/analiz_A_cikti.txt`) yerel kullanıcı yolu `~/` olarak genelleştirildi. Bu iki dosyanın özeti içerik listesindekinden farklıdır.
  - Kökteki Mallows bölütlerinden biri (`tab/L/kA/d3_0/b0041.txt`) yarıştan etkilenmiştir. Düzeltilmiş hâli ve onunla yapılan yeniden sayım (özgünle birebir aynı) `ana_hat/kapi/sonuc/yaris/plasebo14_duzeltilmis/`'tedir.
- **`ana_hat/kapi/`.** Ön-kayıt, rapor (`KAPI_RAPORU.md`), ekler, kod, akışlar ve doğrulama betikleri. Kapı başına diller ve ayrıntılar metin olarak, kontrol bölüt sayımları `bolutler_<kapı>.tar.gz` içinde. Mallows bölütleri düzeltilmiş yeniden sayımdandır. Kur'an tarafının bölüt sayımları `sonuc/kuran/<kapı>/bolutler_kuran_<kapı>.tar.gz` içindedir. Gc 10¹⁰ isabetleri metin olarak `sonuc/kuran/Gc10/<S>/isabet.txt`'tedir: sıra, puan, mercek ve 114 ayet sayısı.
  - Yarış kaydı `sonuc/yaris/`'tedir: düzeltilmiş sayaç kodu, yeniden koşu betikleri, günlük, bölüt bölüt farklar ve özgün bozuk bölütler (`yaris_oncesi_bozuk_bolutler.tar.gz`).
- **`ana_hat/kademe/`.** Ön-kayıt, rapor (`KADEME_RAPORU.md`), ekler, kod ve akışlar. Bölüt sayımları `bolutler_kademe.tar.gz` (özgün) ve `bolutler_kademe_L_yaris_duzeltilmis.tar.gz` içindedir. Yeniden puanlama çıktıları ve isabetlerin metin dökümleri `sonuc/dogrulama/`'dadır.
  - **Genelleme:** ön-kaydın ilk paragrafındaki workstation hesap adı "araştırmacının hesabı" olarak genelleştirildi. Bu kopyanın SHA-256'sı `da9c801a…b100`'dür; dondurulan özgününki yukarıdaki `db06c6f9…`'dur.
- **`ana_hat/dogrulama_p14/`.** plasebo14'ün bağımsız yeniden sayımı.
- **`arsivler.sha256`.** Bütün arşivlerin özetleri.

**Yeniden üretim.**
- Kapı yeniden sayımı için `sonuc/<kapı>/tab/` ile `bolutler_<kapı>.tar.gz` aynı dizine açılır, ardından `python3 -I ana_hat/kapi/dogrulama/yeniden_say_kapi.py <dizin>` çalıştırılır.
- plasebo14 için `python3 -I ana_hat/dogrulama_p14/yeniden_say14.py <bu klasör>`.
- Sayaçlar `kos/` ve `ana_hat/*/kos/` altındaki betiklerle yeniden derlenir. Mallows için `ana_hat/kapi/sonuc/yaris/src/`'teki düzeltilmiş sayaçlar kullanılmalıdır. Tek iş parçacığıyla özgün sayaçlar da doğru sonuç verir.
- Referans tabloları (`ref_*.ref`, sıfır başına ~165 MB) ve hedef dosyaları depoda yoktur; özetleri `sonuc/ref_sha256*.txt` ve `ana_hat/kademe/girdi/`'dedir.
