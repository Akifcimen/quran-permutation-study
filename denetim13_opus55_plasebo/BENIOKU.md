# denetim13 — on üçüncü denetim: bütünlük dilinin plasebo kalibrasyonu (28–29 Ekim 2026)

On ikinci denetimin dili Kur'an görüldükten sonra yazıldı. O denetimde mercek seçimi bedellendi, yüklem seçimi bedellenmedi (not 25, çekince 1). Ayrı bir klasörde ve ayrı hafızayla çalışan bir Claude Opus 5.5 oturumu (en yüksek çaba) bu çekinceyi plasebo kalibrasyonuyla sınadı. Dil yazmak otomatik bir yordama (D) çevrildi. Kütüphane 109 yüklemdir: on ikinci denetimin 20 yüklemi ve onlardan bir üretim kuralıyla (düzey × tür × nicelik × ilişki) türetilmiş kardeşler. D, her hedef için kendi puanını en büyük yapan dili seçer. Ölçüt bilgi − dil bedeli − mercek bedelidir (MDL). Yordam Kur'an'a ve rastgele kitaplara aynen uygulanır. Her hedef kendi diliyle kendi kontrollerine karşı sınanır ve Kur'an'ın ayrışması plasebo kitapların ayrışmaları arasında sıralanır. Görev tanımı `GOREV.md`'dir. Okuma kuralı 27 Ekim'de not 25'te önceden ilan edilmişti. Oturum depoyu yalnız okudu. Ana hattın değerlendirmesi `KESIF.md` dürüstlük notu 26'dadır.

**Sonuç (ayrıntı `PLASEBO12_RAPORU.md`, İngilizce özet sonunda):**
- D(Kur'an) üç sıfırda da aynıdır: **{B2, K1, Qc} @ (1,+1,19)**, yani terazi, ardışık hücre toplamları ve hücre merkezinin 19 olması. Kur'an bu mercekte 109 yüklemin 21'ini sağlar; öteki 18'i bedelini karşılayacak ek bilgi taşımaz. Plasebo dilleri 0–5 yüklemlidir (ortalama ~1).

| Sıfır | Plasebo | Kur'an p_Q (k / 10⁸) | En iyi plasebo | Plasebo medyanı | Kur'an'a ulaşan | Kalibre p | Okuma |
|---|---|---|---|---|---|---|---|
| M20 (birincil) | 200 | 3,4×10⁻⁷ (33) | 3,0×10⁻⁶ | 1,35×10⁻² | 0 | **1/201 = 0,005** | aşar |
| Mallows | 100 | 8,0×10⁻⁸ (7) | 5,8×10⁻⁶ | 1,40×10⁻² | 0 | **1/101 = 0,0099** | aşar |
| Tekdüze | 100 | 5,0×10⁻⁸ (4) | 1,0×10⁻⁷ | 1,06×10⁻² | 1 (aralık çakışması) | **2/101 = 0,020** | arada |

- **Kalibre p ve model.** Kalibre p, plasebo sayısı K ile en küçük 1/(K + 1) olabilir. Aradaki fark yalnız model-tabanlıdır ve kararlarda kullanılmadı. M20'de GPD kuyruk uyumu Kur'an'ı uyumun uç noktasının ötesine koyar (%95 üst ucu 3,1×10⁻⁴); normal model 1,1×10⁻⁵ verir.
- **Dil yazımının kazancı.** Plasebolar kendi dillerinde, el diline göre medyanda ~1,5 mertebe daha ayırt edicidir. Kur'an'da bu fark 0,1–0,3 mertebedir.
- **Bütünlük şartlı ayrışma** ön-kayıtlı biçimiyle tanımsız kaldı. D(Kur'an) iki düzeyden gelir (kitap, hücre), sınıf kuralı ise ≥ 3 düzey ister.
- **Ana hattın tahmini** (M20; önceden kaydedilmişti) dört parçasıyla da tuttu.

**Ön-kayıt:** `ONKAYIT.md` 2026-10-28T22:33:04Z'de donduruldu (SHA-256 `3837c9d819f63b027e89b5e21f413c94766c440cea20d981b068fba269771a5a`, `sonuc/onkayit_ozet.txt`). Dondurulduğu anda hiçbir hedef çekilmemişti, hiçbir dil kurulmamıştı ve hiçbir kontrol puanlanmamıştı. Oturumun dondurmadan önceki bir düşünce adımı ön-kayıtta ve raporun §1.1'inde açıklanmıştır. Sapmalar `ek/` altında zaman damgalıdır (4 ek); hiçbiri bir tanımı değiştirmez.

## Ana hattın kontrolleri (`dogrulama/`)

Bu kontroller oturumun `analiz.py`'si ve C kodu kullanılmadan yapıldı.

| Kontrol | Sonuç |
|---|---|
| Ön-kayıt ve kod bütünlüğü | Ön-kaydın özeti tutuyor. Dondurma anındaki 30 kaynak dosya değişmemiş (`shasum -c`, 30/30). Hedef ve kontrol tohumları dondurmadan sonra ilk kez kullanılmış (koşu günlüğü). `~/Desktop/hrf2` değişmemiş |
| Sıraların ham bölüt dosyalarından yeniden sayımı (`yeniden_say.py`) | Üç sıfırda Kur'an'ın k'si, N, p_Q ve %95 aralığı, en iyi plasebo, R ve kalibre p birebir aynı; derinlik kuralı ön-kayıttaki gibi işletildi (`sonuc_yeniden_sayim.txt`) |
| Yüklemlerin bağımsız yeniden yazımı (`yuklem_kontrol.py`; hedef dosyası paketten) | Kur'an (1,+1,19)'da B2, K1 ve Qc'yi sağlar (hücreler 3/3/3/3, toplamlar 20/18/19/19). Tekdüzede Kur'an'ın aralığına giren x068 (3,+2,43)'te F2, K2, FSC ve WUN2p'yi sağlar (`sonuc_yuklem.txt`; kitabın ayet sayıları çıktıdadır) |
| Kendi tekdüze örnekleyicimiz (`b2_tekduze.py`, iki tohum, 1,6×10⁷ kitap) | (1,+1) biçiminde B2'nin sıklığı 18.046 / 1,6×10⁷ = 1,128×10⁻³ ± 0,008×10⁻³. Oturumun tablosu 1,123×10⁻³ (`sonuc_b2_tekduze.txt`). Aile-içi arama (K1 ∧ Qc \| k_F = 12) ve kontrollerin puanlaması yeniden yapılmadı |

**Ana hattın okuması.**
- **Sıralı sıfırlarda okuma kuralının ilk cümlesi uygulanır.** On ikinci denetimin basamağının sırası, dilin Kur'an'a göre yazılmasıyla açıklanamaz. Tekdüzede sonuç aradadır ve kalibre p'siyle (0,020) yazılır.
- **Nadirliği taşıyan, bütün değil çekirdektir.** El dili (20 yüklem) Kur'an'ı rastgele kitaplardan çekirdekten daha fazla ayırmaz. M20'de el diliyle 6,4×10⁻⁷, çekirdekle 3,4×10⁻⁷; Mallows'ta 1,5×10⁻⁷ ve 8,0×10⁻⁸; tekdüzede 6,0×10⁻⁸ ve 5,0×10⁻⁸. On ikinci denetimin "en yakın rastgele kitaplar 15 yüklemin 13'ünü taşır, hiçbiri hepsini taşımaz" bulgusu doğrudur. Ama sıra ölçüsünde nadirliği üç yüklem taşır. "Nadirlik bütündedir" okuması bu kalibrasyonla sınanmış olmaz. MDL bedeli altında D hiçbir hedefe bütünlüklü bir dil kurmaz; bütünün kalibrasyonu ayrıca yapılmalıdır.
- **Kur'an'a en çok yaklaşan plasebo parçalı bir denk gelmedir.** Tekdüzede Kur'an'ın aralığına giren x068 5 üyeli bir ailedir (p = 43); dört hücresinin ikisi boştur, kapı G tutmaz. Puanını, hücrelerde sabit katsayılar ve birleşimin konum toplamının 2p = 86 olmasından alır.
- **Ölçülemeyen:** çekirdeğin üç yüklemi Kur'an'da büyük ölçüde 1990, 1994 ve en geç 2015'te yayımlanmış gözlemlerdir (raporun §4.1'i). Kalibrasyon D'nin seçimini bedellendirir; bu gözlemleri ilk bulan insan aramasının büyüklüğünü bedellendirmez.

## Bu kopya

Bu klasör, oturumun nihai paketinin açılmış hâlidir: `plasebo12_paket.tar.gz`, 2,0 MB, SHA-256 `5da4781746ce363b5e3bd351b034f62b8728913a5b94160c0c63c6d5243a7318` (`paket/plasebo12_paket.sha256`). Paket 2.603 dosya içerir.

**Çıkarılanlar** (1.228 dosya, ~1,6 MB; yol, boyut, SHA-256 ve gerekçe `atlananlar.tsv`'de):
- kontrol bölütlerinin ikili aday dosyaları (`tab/*/k*/b*.q`; çoğu boş);
- hedef ve tarama dosyaları (`tab/*/hedef.bin`, `tab/tarama/*.bin`);
- derlenmiş programlar (`kos/derle.sh` ile yeniden derlenir).

`paket/PAKET_ICERIGI.sha256` paketteki bütün dosyaların özetlerini taşır. Çıkarılan dosyalar aynı tohumlardan yeniden üretilir (rapor §11). Seyrek referans tabloları (`ref_*.ref`, sıfır başına ~165 MB) zaten pakette yoktur; özetleri `sonuc/ref_sha256.txt`'tedir. Paketin kendisi ve tablolar araştırmacının makinesindedir.

**Yol genellemesi:** `src/b12/dil.py`'deki depo yolu `os.path.expanduser('~/Desktop/hrf2')` olarak genelleştirildi ve `os` içe aktarımı eklendi. Dosya bu yüzden `denetim12_opus55_butunluk/src/dil.py` ile birebir aynıdır; özeti `PAKET_ICERIGI.sha256`'dakinden farklıdır.

**Yeniden üretim:** rapor §11. `dogrulama/yeniden_say.py` bu klasördeki metin dosyalarıyla çalışır. `yuklem_kontrol.py` paketteki `tab/U/hedef.bin`'i ister.
