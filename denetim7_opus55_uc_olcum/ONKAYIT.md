# ÖN-KAYIT — merdivendeki üç olası aşağı-yönlü hatanın ölçümü

**Tarih:** 10 Ekim 2026 · **Yazan:** Claude (Opus 5.5), kullanıcının isteğiyle
**Statü:** Aşağıdaki ölçümlerin hiçbiri yapılmadan ÖNCE yazıldı ve donduruldu. SHA-256 özeti `sonuc/onkayit_ozet.txt`
dosyasına yazılır, dosya salt-okunur yapılır. Sonradan yapılacak her değişiklik ayrı, zaman damgalı bir ek dosyada olur.
**Kural (kullanıcının):** ölçmek için hatalı davranmamak; sonuç hangi yöne çıkarsa çıksın aynen yazılır.
**Salt okunur:** `<hrf2>` (depo), `<oturumlar>/kirma`,
`<oturumlar>/guclendirme`. Bütün çıktılar `<oturumlar>/uc_olcum` altında.

---

## 0. Dondurmadan önce gördüklerim (şeffaflık)

- README "honest ladder", KESIF.md (Katman 1 köken notu; notlar 13–19), `kesif26_denetim5_denetimi/KESIF26_SONUC.md`,
  `denetim5_opus55/KIRMA_RAPORU.md`, `denetim6_opus55_guclendirme/` (ONKAYIT.md, ONKAYIT_EK.md, GUCLENDIRME_RAPORU.md,
  `sonuc/ONKAYITLI_SONUCLAR.md`). Robot kaynaklarını okudum (`robot.c`, `robot2.c`, `robot4.c`, `robot_m.c`, `robot4_m.c`,
  `robot4_kisit.c`, `robot4_tutarli.c`, `motor.c`). Kopyaları `src/`; SHA-256'ları `sonuc/sha256_kaynaklar.txt`; tablolar
  `sonuc/sha256_tablolar.txt` (kirma/ ve guclendirme/ kopyaları birbirinin ve denetim6 kaydının aynısı).
- Kur'an'ın (1,1,19) olgu dökümü, E1 bataryası, tekdüze tablo (`U_2M.bin`): F: k≥2E 1,76 · F: Σq kare 1,48 ·
  F: hücreler dengeli 3,91 · F: katsayı bandı 1,21 · U: |U|=m 0,90 · U: m|Σa(U) 1,28 → T2 = 10,54 (6 düğüm). Terazi
  düğümü açık olsa 3,52 ekler (14,06). Bu döküm daha önce `kesif26/kuran_dokum.txt`'de yayımlıdır.
- Planlama için 6. oturumun `puan_ozet/E1_*.npz`, `E3_*.npz` kuyruk özetlerinde Kur'an eşiği altındaki bant sayıları
  (örn. E1 J10: T2 ≥ Q: 1.250; ≥ Q−1: 4.127; ≥ Q−1,5: 7.243).
- **Görmediklerim:** literatür kaynaklarının hiçbiri (1. ölçüm); hiçbir tam-olasılık (SuS) hesabı (2. ölçüm); hiçbir
  yeni robot koşusu (1–3. ölçüm).

---

## 1. Ölçüm 1 — Terazi literatürünün büyüklüğü

### 1.1 Sayım birimi ve sınıflar

- **İddia (olgu):** Kur'an hakkında yayımlanmış, belirli bir sayısal ilişkiyi (eşitlik, bölünebilirlik, toplam, sayım,
  rakam dizisi/birleştirme, simetri, parite dengesi vb.) dikkat çekici/mucize diye sunan, kontrol edilebilir tek önerme.
- **SA (sûre-ayet):** girdisi yalnız sûre numaraları, sûre başına ayet sayıları, ayet numaraları, sûre sırası/konumu,
  sûre/ayet sayımları (numarasız besmele sayısı dahil) ve bunlardan aritmetikle türetilen nicelikler olan iddialar.
- **HK (harf/kelime):** harf sayımı, kelime sayımı, belirli kelimelerin sıklığı veya geçtiği yerler, ebced, mukattaa
  harfleri gerektiren her iddia. Sûre-ayet niceliklerini metin özelliğiyle birleştiren karma iddialar (ör. "mukattaalı
  sûrelerin numaraları toplamı", "Allah'ın geçtiği ayet numaraları toplamı") HK'da sayılır, "karma" etiketiyle.
- **Tekilleştirme:** aynı nicelikler üzerinde aynı ilişki = 1 iddia (kaynak, dil, tarih ve yazım farkı önemsiz).
  Farklı ilişkiler ayrıdır (ör. "6346 = 19×334" ile "6+3+4+6 = 19" iki iddia).
- **Alt sınır N_alt:** taranan kaynaklarda bulunan, birebir alıntıyla belgelenmiş, tekilleştirilmiş, veriden içerik taşıyan
  iddialar; veride cebirsel olarak denk iki iddia (aynı olgunun yeniden yazımı; ör. Σt(çift) = Σa ⟺ Σn(çift) = Σa(tek))
  1 sayılır.
- **Üst sınır N_üst:** alt sınır + cebirsel denkler ayrı + kodeks varyantları (6346/6236 sürümleri) ayrı + yanlış ya da
  doğrulanamayan iddialar + taranmamış kısımlar için §1.2'deki dışdeğerleme.
- SA ve HK için iki sınır ayrı ayrı verilir.

### 1.2 Kaynaklar ve tarama

- K1: Reşad Halife, *Quran: The Final Testament* — Ek 1 ("One of the Great Miracles") ve Ek 24 (masjidtucson.org /
  submission.org); sayısal madde içeren diğer ekler.
- K2: Edip Yüksel (19.org ve çevrimiçi erişilebilen kitap/yazıları: *Nineteen: God's Signature in Nature and Scripture*,
  Türkçe 19 yazıları).
- K3: quranmiracles.com · K4: mucizeler.com · K5: 7ve19.com · K6: i3gaz.com (Wasim Masri dahil) · K7: kuranmucizeler.com.
- K8: aramayla bulunan diğerleri (ör. Bassam Jarrar, Abdüddaim el-Kâhil, iqra-quran.com, derleme/eleştiri sayfaları).
- Her kaynak için kaç sayfanın tarandığı kaydedilir. Yüzlerce makaleli kaynakta tam tarama mümkün değilse makale dizini
  çıkarılır, bir alt küme taranır; taranmamış makalelerin katkısı = (taranan makale başına YENİ SA iddiası) × (taranmamış
  makale sayısı); bu katkı yalnız üst sınıra eklenir.
- Araştırma alt-ajanlarla paralel yapılabilir. Her iddia için kaynak URL'si ve kaynaktan birebir alıntı tutulur;
  alıntısız iddia sayılmaz. Tekilleştirmeyi tek listede ben yaparım.

### 1.3 Kalıntı şaşırtıcılık (kullanıcının formülü; birincil)

- p_A = 3,137×10⁻⁴ (tekdüze); sıralı p_A' = 1,45 × p_A = 4,549×10⁻⁴ (kesif16).
- N bakışta: beklenen sayı E = N·p, kalıntı p_res = 1 − (1 − p)^N.
- **Birincil N = N_SA** (alt ve üst sınır ayrı ayrı).
- Duyarlılıklar (önceden tanımlı): (a) N = N_SA + N_HK; (b) kuyruk-oranı bakış tahmini L = 19 × H19, H19 =
  tekilleştirilmiş SA iddiaları içinde "bir nicelik 19'un katıdır" türündekilerin sayısı (bu türün nominal olasılığı ~1/19;
  literatür bu düzeydeki isabeti yayımladığından bakış ≈ 19 × isabet). Yayımlanan iddia sayısı bakış sayısının ALT
  sınırıdır; bu yüzden birincil p_res Kur'an lehine bir alt sınırdır. (b)'nin kendi zaafı (bölünebilirlik bakışı tek
  başına 1/3.200'lük olay üretemez) raporda yazılır.

### 1.4 Merdivene çeviri — doğrudan robot ölçümü (birincil)

- **Terazi kısmen sayılır:** sabit mercek (1,1,19) (mercek devralınmış). Her kitapta (Kur'an ve rastgele) "kitap-terazi"
  düğümünün değeri s = −log10 p (düğümde isabet eden en nadir olgunun tablo olasılığı) yerine
  s(N) = −log10[1 − (1 − p)^N] konur; diğer düğümler aynen; T2(N) = en iyi 8 düğüm. Aynı N bütün kitaplara (simetrik).
- **Kitaplar/tablolar:** (i) E1'in kitapları: `robot_m`, iddia bataryası (`Σw=m·kG|Σv(U) kare/asal-kuvvet` kapalı,
  `ROBOT_MINTR=1000`), tohumlar U 19901, J10 19902, M20 19903; tablolar `U_2M.bin`, `J10_2M.bin`, `M20_2M.bin`.
  (ii) E3'ün kitapları: `robot4_m`, defter bataryası (+`MX:` kapalı), tohumlar 19911/19912/19913; tablolar `T_U_2M.bin`,
  `T_J10_2M.bin`, `T_M20_metin14_2M.bin`. Sağlamalar: N = ∞ E1/E3'ün kayıtlı sayılarını birebir verir (115/1.250/1.125;
  191/8.394/7.794); N = 1 terazinin tam sayıldığı sabit-mercek değerini verir.
- N ızgarası: 1, 3, 10, 30, 100, 300, 1.000, 3.000, 10.000, ∞ ve N_SA,alt, N_SA,üst, (a), (b).
- **Önerilen basamak:** N = N_SA (alt–üst) için iki batarya × üç sıfır; bugünkü "terazi sayılmaz" basamağının biçimiyle
  [iddia; defter] aralığı. A1c'nin (17 biçim, terazi tam sayılı) bu eğride hangi N'ye denk düştüğü de raporlanır.

---

## 2. Ölçüm 2 — Robotun Kur'an'ı eksik puanlaması (düğüm bağımsızlığı)

### 2.1 Tanım

- Bir kitabın **hikâyesi H:** robotun o kitap için kullandığı mercekte (birincil analizde sabit (1,1,19)) en iyi 8
  düğüm; her düğümde düğüm değerini veren olgu (en nadir isabet; eşitlikte en küçük olgu numarası). Hikâye olayı:
  H'deki olguların hepsinin, aynı mercekte, robotun `facts()` fonksiyonuyla aynı anda tutması.
- **Tam puan** S(H) = −log10 P_sıfır(H olayı); aile boyuna koşullama yok — hikâyenin söylediği olayın kendisi.
- **Sapma** D = S(H) − T2 (robotun yaklaşık puanı).

### 2.2 Kestirici: alt-küme benzetimi (subset simulation, SuS)

- Seviyeler: H'nin olguları robot tablosundaki olasılıklarına (bağlamlı) göre büyükten küçüğe dizilir (sabit sıra,
  örneklerden seçilmez). Hikâyede "hücreler dengeli" varsa önüne "v-yarılar eşit", bir kitap eşitliği varsa önüne
  "|E|=|O|" ara seviye olarak eklenir (gerektirilen olgular). P = Π_j P(E_{j+1} | E_j).
- Seviye 0: sıfırdan bağımsız örnek (U: tekdüze karıştırma; J10: robotun titreşim örnekleyicisi; M20: robotun 64-zincirli
  MCMC'si, aynı 30.000 yakma, 20 inceltme).
- Koşullu seviyeler: hedef = sıfırın E_j'ye kısıtı. Hamleler (her adımda yarı yarıya): U ve M20'de (a) rastgele
  transpozisyon, (b) mercek modülüne göre aynı kalıntılı iki değerin yer değiştirmesi (üyelikleri korur); M20'de ayrıca
  S ≤ S_obs. J10'da gizil z üzerinde (a) bir koordinatı N(0,1)'den yeniden çekme, (b) aynı kalıntılı iki öğenin
  anahtar takası (Metropolis–Hastings, önsel oranıyla). Kabul ⟺ E_j tutar (ve MH oranı). Hepsi hedefi korur.
- Örneklem: rakip kitaplarda seviye başına N = 20.000 adım (bir seviyede isabet < 100 ise o seviye 10× büyütülür; tavan
  2×10⁶); Kur'an'da seviye başına N = 200.000, R = 8 bağımsız tekrar (tekrarların log-ortalaması ve yayılımı).
- **Doğrulama (analizden önce; tutmazsa analiz "güvenilmez" diye işaretlenir, yalnız doğrulanan kısım raporlanır):**
  - V1 (tekdüze, yazarın olayları, motor.c tanımı): G 7,14×10⁻¹⁰, D 1,394×10⁻⁷, C 1,849×10⁻⁵, E 2,11×10⁻⁸.
    Ölçüt: C, D, E için ×1,25; G için ×1,5 içinde (R = 8 tekrarın log-ortalaması).
  - V2 (J10 ve M20): aynı olaylar için kendi doğrudan sayımım (her sıfırda ≥ 2×10⁹ örnek; G J10'da ~10², M20'de ~10²
    isabet beklenir) — SuS ×1,3 içinde (G için ×1,5; isabet < 30 ise Poisson aralığıyla uyum).
  - V3 (robot hikâyeleri): her sıfırda E1 kitaplarından rastgele (sabit tohum) seçilmiş 20 kitabın (T2 ∈ [4; 5,5])
    hikâyesi için SuS ile doğrudan sayım (≥ 2×10⁹ örnek) karşılaştırılır; yalnız ≥ 30 doğrudan isabetli hikâyeler
    kullanılır. Ölçüt: oranların geometrik ortalaması [0,85; 1,18], %90'ı [0,7; 1,4] içinde.

### 2.3 Sıra hesabı (birincil: E1)

- Kitaplar: E1'in 2×10⁶ kitabı (yukarıdaki tohumlar); önce E1 sayıları birebir yeniden üretilir.
- T2 ≥ T2_Q − 1 olan bütün kitaplar için S(H) hesaplanır; [T2_Q − 2, T2_Q − 1) bandından sabit tohumla rastgele 1.500
  kitap; [T2_Q − 3, T2_Q − 2) bandından 500 kitap (yalnız denetim).
  k' = Σ_bant (bant sayısı / örneklenen) × #{örneklenen: S(H) ≥ S(H_Q)}; yeni p = (k' + 1)/(N + 1).
- Belirsizlik: Kur'an'ın R = 8 tekrarı; rakiplerde |S(H) − S(H_Q)| ≤ 0,5 olanlar seviye başına 200.000 ve R = 4 ile
  yeniden hesaplanır; bant örneklemesi için bootstrap; Poisson.
- En alt denetim bandında S(H_Q)'yu geçen kitap çıkarsa o bant tam taramaya genişletilir.
- (b) yolu: rakiplerin D dağılımı (T2 bantlarına göre medyan ve çeyrekler) ve D ≥ D_Q oranı raporlanır.

### 2.4 İkincil

- **E1 + terazi** (iddia bataryası, sabit mercek, terazi açık; KESIF26 karşılaştırmasının kendisi: 14,06/12,03/12,08):
  Kur'an'ın S(H)'si (R = 8) ve A1a 1. parçanın kitaplarında (tohumlar 19001/19101/19201, `robot_m`, `ROBOT_MODS=19`,
  sabit sütun) aynı sıra hesabı.
- **E3** (defter bataryası, sabit mercek, terazisiz): Kur'an'ın S(H)'si; T2 ≥ T2_Q olan bütün kitaplar,
  [T2_Q − 1, T2_Q)'dan 2.000 ve [T2_Q − 2, T2_Q − 1)'den 1.000 rastgele kitap.

### 2.5 Yorum

- Yeni p robotun p'sinin ×1,5 içindeyse: "düğüm bağımsızlığı yaklaşıklığı sıra üzerinde maddi etki yapmıyor". Değilse
  yön ve büyüklük aynen yazılır ve merdivene önerilir. Kur'an'ın D'si rakiplerin D dağılımıyla ayrıca karşılaştırılır.
- Not (önceden): KESIF26'nın 14,97/12,54/12,82 değerleri yazarın tam olayının (terazi ∧ kristal ∧ İki El) frekansıdır;
  robotun hikâyesi daha kaba (sınıf düzeyi) olgulardan oluşur. Bu yüzden S(H)'nin bu değerlere ulaşması beklenmez;
  karşılaştırmanın doğru nesnesi robotun kendi hikâyesinin birlikte-olasılığıdır.

---

## 3. Ölçüm 3 — "Yeterince yakın" cömertliği / modülle ifade

### 3.1 Olgu tanımı

- X_m = {c·m, c·m², (c·m)² : c = 1, 2, 3, 4}.
- **"|Σq| m-ifadeli"** (F ve G; düğüm X-toplam): ailenin doğal nicelik toplamının mutlak değeri X_m'de.
- **"v-yarılar m-ifadeli"** (F ve G; düğüm X-yarı/hücre): iki q-paritesi yarısı da boş değil ve iki yarının |Σq|'su da
  X_m'de.
- Yalnız S2 için: "Σn m-ifadeli", "Σa m-ifadeli" (F, G; X-çapraz), "Σa(U) m-ifadeli" (U-güzel).
- Olasılıklar diğer olgular gibi aile boyuna koşullu tablolardan. Yeni olgular mevcut 99 olgunun ARKASINA eklenir;
  tablolar orijinal tohumlarla yeniden kurulur; eski olguların sayaçları orijinal tablolarla birebir aynı olmalıdır
  (sağlama; tutmazsa analiz durur).

### 3.2 Değişkeler (hepsi bütün kitaplara, her kitabın kendi merceğinin m'sinde)

- **P (birincil, "tanım"):** "Σq kare" yerine "|Σq| m-ifadeli" (F, G); "v-yarılar m-ifadeli" çatal olarak eklenir.
- **S1 ("ekle"):** "Σq kare" korunur; iki yeni olgu çatal olarak eklenir.
- **S2 ("bütün kareler"):** Σq, Σn, Σa (F, G) ve Σa(U) "kare" olguları m-ifadeli sürümleriyle değiştirilir; yarı çatalı
  eklenir.

### 3.3 Basamaklar (eşit ağırlıkta; tek manşet yok)

- R-E1: iddia bataryası (defter düğümleri kapalı), sabit (1,1,19), terazisiz — `robot4` tabanlı yeni robot, T tabloları,
  E1 tohumları (19901/19902/19903).
- R-E3: defter bataryası, sabit (1,1,19), terazisiz — E3 tohumları (19911/19912/19913).
- R-A1c: defter, m = 19, 17 biçim, terazi sayılı — A1c 1. parça tohumları (19001/19101/19201).
- R-646: defter, 646 mercek, terazi sayılı ("rakipler kendi m'lerinde") — tohumlar U 44, J10 42 (denetim5), M20 1951
  (denetim6 B4); tablolar T_U (43), T_J10 (41), T_M20_2M (1950) yeniden kurulmuş.
- Her basamakta önce değişke kapalı **temel** değer (aynı robot, aynı tablo, aynı kitaplar), sonra P, S1, S2. Sıfırlar U,
  J10, M20. **×2 sonradan-ekleme bedeli** Kur'an'ın p'sine uygulanmış ve uygulanmamış hâliyle raporlanır.

### 3.4 Döküm

- Kur'an'ın (1,1,19)'da X_19'a düşen bütün sabitleri (aile ve yarı toplamları, birleşim toplamı, hücreler; katsayı
  toplamlarının karşılıkları).
- R-E1 ve R-646'da (J10 ve U), P altında en yüksek T2'li 12 rakibin mercekleri, T2'leri ve X_m'deki sabitleri.

### 3.5 Önceden tahmin (sonuç görülmeden)

m = 19 asal: her üyede q ≡ 0 (mod 19) olduğundan Σq'nun tam kare olması (19j)² olmasını gerektirir; k ≈ 12'nin tipik
aralığında (Σq ≈ 1.000–2.500) "Σq kare" zaten yalnız 1444'ü ödüllendirir. X_19 aynı aralıkta {1083, 1444} içerir, yani
**daha cömerttir**. Tahminim: R-E1/R-E3'te Kur'an'ın puanı P'de 0–0,2 düşer, S1'de değişmez; rakipler 2m², 3m²
isabetlerinden az kazanır; sabit-mercek sırası ×1–1,5 kötüleşir. R-646'da bileşik m'lerde "Σq kare"nin cömertliği
gerçektir; P ve S2 rakipleri biraz düşürebilir — yön belirsiz, ×0,5–2.

---

## 4. Genel kurallar

- Ön-kayıt dışındaki her analiz "keşif amaçlı" diye etiketlenir ve merdiven önerisine temel olmaz.
- Sapmalar (doğrulama başarısızlığı, bitmeyen koşu, hata düzeltmesi) zaman damgalı bir ek dosyada yazılır.
- Kodeks: robotlar önceki bütün robot koşuları gibi Tevbe = 127 kullanır; çekirdek katmanlar 127/129'dan bağımsızdır.
- Koşular 8 iş parçacığıyla, tohumlu ve yeniden üretilebilir; bütün kod ve ham çıktılar bu klasörde.
- Disk sınırlı (~14 GB boş): puan dizileri gerekirse kuyruk özetlerine sıkıştırılır; bu da raporda yazılır.
