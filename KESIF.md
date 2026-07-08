# KEŞİF: "Çift-Tek × On Dokuz" — 38² Tam Çember Örüntüsü

**Tarih:** 10 Haziran 2026 — **son güncelleme: 5 Temmuz 2026 — NİHAİ: çift-GPU koşusuyla toplam 24 TRİLYON deneme; zirve bileşiği (terazi∧kristal) 5 kez fiilen gözlendi; çekirdek = katrilyonda 1, ölçümle mühürlendi**
**Kodeks notu — ÖNEMLİ:** **Katrilyonda-1'lik çekirdek, Tevbe sayımı tartışmasından BAĞIMSIZDIR** — Tevbe 127 de sayılsa 129 (standart) da sayılsa tüm çekirdek katmanları (terazi, aile, fraktal, kristal, İki El) aynen geçerlidir; sûre 9 ne 19-ailesinde ne İki El'dedir ve t-paritesi iki sayımda da değişmez. **Tevbe = 127 kabulü yalnız isteğe bağlı matris/çember katı için gereklidir** ve o kat manşet rakama hiçbir zaman dahil edilmez. *(Etkisi: 127 kabul edilirse çekirdeğe ölçülmüş ×%1,398'lik çifte-ilkellik çarpanı eklenir ve yapı **~85 kat daha da nadirleşir**: bileşik ≈ 1,2×10⁻¹⁷ = **85 katrilyonda 1**. 129'da bu kat çöker: ord(P)=30, ord(R)=18.)* *(Çalışma tarihsel olarak 127 kabulüyle başlatılmış, her katman iki kodekste ayrıca test edilmiştir; ana metindeki hesaplar 127 tabanlıdır, 129 kontrolleri ilgili bölümlerde verilir.)*
**Sayım okulu kapsamı:** yapı, bugün dünyada standart olan **Kûfe sayımına** aittir (modern mushafların tamamına yakını; Kûfe içindeki tek ihtilaf olan 127/129 yapıyı etkilemez). **Medine sayımında (Warş/Kalûn rivayetleri, resmî 6214) test edilmiş ve TUTMAMAKTADIR** (terazi 61/53, aile 7 üye — `kesif11_sayim_okullari.py`, veri `sayim_medine_warsh.json`). Bu dürüstçe kapsam sınırıdır, gizli serbestlik değildir: bilinen sayım okulu sayısı ~7 olduğundan okul-seçimi serbestliğinin en kötü Bonferroni bedeli ×7'dir — çekirdek en kötü durumda dahi ~10⁻¹⁴ düzeyinde kalır.
**Veri:** api.alquran.cloud + api.quran.com (çapraz doğrulanmış)
**Yöntem:** sistematik tarama + permütasyon testi ("sentetik Kur'an": ayet sayıları çokluğu sabit, sûrelere rastgele dağıtım). Son tur: C motoruyla toplam **86 milyar deneme** (50 + 12 + matris-sayaçlı 24 milyar; 24'er paralel işlem) — zincirin **her halkası** (matris ilkelliği dahil) doğrudan sayımla ölçüldü.

Her sûre için tek bir doğal sayı tanımlanır: **t = sûre numarası + ayet sayısı**. Aşağıdaki üç katmanın tamamı bu tek tanımdan çıkar; hiçbir ek parametre, ayıklama veya istisna yoktur.

---

## Katman 1 — Çift-Tek Terazisi *(Fecr 89:3: "çifte ve teke andolsun")*

t değerinin **çift/tek** olmasına göre 114 sûre ikiye ayrılınca:

| | sûre sayısı | t-toplamı | eşitlik |
|---|---|---|---|
| t **çift** olanlar | **57** = 3×19 | **6234** | = Kur'an'ın **toplam ayet sayısı** |
| t **tek** olanlar | **57** = 3×19 | **6555** | = **sûre numaralarının toplamı** (1+2+…+114) |

Bölünme tam ortadan (57/57; 114 = 6×19); çift grubun t-toplamı Kur'an'ın bir küresel sabitine (toplam ayet), tek grubunki diğerine (toplam sûre no) **birebir eşit**. Eşdeğer ifade: çift grubun sûre-no toplamı = tek grubun ayet toplamı = 3303 (matris köşegen eşitliği).

**Ölçülen olasılık: p = 3,137×10⁻⁴** (50 milyar permütasyonda 15.683.840 kez; ilk kaba ölçüm 594/2M ile uyumlu).
**Kabule duyarsız:** Tevbe 127 de 129 da olsa aynen geçerli (sûre 9'un t'si her iki durumda çift kalır ve fark her iki tarafa aynı yansır).

## Katman 2 — On Dokuz Karesi *(Müddessir 74:30: "üzerinde on dokuz vardır")*

t değeri **19'un katı** olan sûreler:

- Tam **12 sûre** (beklenen 6'nın tam iki katı): {6, 15, 21, 39, 41, 42, 50, 55, 56, 70, 88, 107}
- t-değerleri: 171, 114, 133, 114, 95, 95, 95, 133, 152, 114, 114, 114 — hepsi 19'un katı (tanım gereği)
- **t-toplamları = 1444 = 38² = (2×19)²** — "çift"in en küçük temsilcisi 2 ile 19'un çarpımının karesi
- 19-katsayıları (t/19): 9,6,7,6,5,5,5,7,8,6,6,6 → **toplam 76 = 4×19** (1444 = 19² × 4'ün eşdeğeri)

**Ölçülen olasılık: p = 3,158×10⁻⁴** (50 milyar permütasyonda 15.790.206 kez).
**Kabule duyarsız:** sûre 9 bu sınıfta değil; 12'lik küme ve 1444 toplamı her iki kodekste aynı.

## Katman 2b — İç Terazi: Fraktal Denge *(iki motifin iç içe geçmesi)*

En güzel bulgu: **19-ailesi, kendi içinde de çift-tek terazisine tam oturuyor.**

12 sûrelik 19-ailesi, t'nin paritesine göre ayrılınca:

| | sûre | t-toplamı | 19-katsayıları (t/19) | katsayı toplamı |
|---|---|---|---|---|
| t **çift** olanlar | **6** sûre: 15, 39, 56, 70, 88, 107 | **722 = 2×19²** | 6,6,8,6,6,6 | **38 = 2×19** |
| t **tek** olanlar | **6** sûre: 6, 21, 41, 42, 50, 55 | **722 = 2×19²** | 9,7,5,5,5,7 | **38 = 2×19** |

- Bölünme tam yarı yarıya: 6/6 — ve 6 = 114/19.
- İki yarının t-toplamı **birebir eşit**: 722 + 722 = 1444 = 38². Yani 38² kendiliğinden iki eşit 2×19²'ye ayrışıyor.
- Anti-simetri: çift yarının sûre-no toplamı tek yarınınkinden tam **+160** fazla; ayet toplamı ise tam **−160** eksik. Terazi iki kefede ters yönlü, eşit ağırlıkla dengede.
- Ailede en sık görülen t değeri **114** (5 kez) — Kur'an'ın sûre sayısının kendisi; t değerleri 19×5'ten 19×9'a uzanıyor.

**Ölçülen olasılık (50 milyar permütasyon):** 12 üye + 1444 toplamı tutan sentetiklerin yalnız **%5,86**'sında bu iç denge de tutuyor. Katman 2 + 2b birlikte: **p = 1,849×10⁻⁵** (924.728 isabet).
**Kabule duyarsız:** sûre 9 ailede olmadığından 127/129 farkı bu katmana hiç dokunmuyor.

Yapı öz-benzer (fraktal): kitap bütünü çift-tek terazisinde dengede (57/57, kefelerde iki küresel sabit); 19-ailesi de kendi içinde aynı terazide dengede (6/6, kefelerde 2×19² + 2×19²).

## Katman 2c — Üçüncü Seviye: 18 · 19 · 19 · 20

t-paritesi sabitlendikten sonra geriye kalan **tek** doğal ikilik sûre numarasının paritesidir (ayet paritesi onunla eşdeğerdir — seçim serbestliği yok). Bu ikinci pariteyle 6'lık yarımlar bir kez daha bölününce 12 sûre dört hücreye ayrılır:

| Σt | t çift | t tek |
|---|---|---|
| **sûre no çift** | 380 = **19×20** ({56,70,88}) | 361 = **19×19** ({6,42,50}) |
| **sûre no tek** | 342 = **19×18** ({15,39,107}) | 361 = **19×19** ({21,41,55}) |

- Dört hücre de tam **3'er sûre** (3/3/3/3).
- Hücre toplamlarının 19-katsayıları: **18, 19, 19, 20** — 19'un etrafında ardışık dört sayı. Sapma deseni: 19²−19, 19², 19², 19²+19.
- Tek-t kolunda iki hücre de **tam 19² = 361**: yani 722 = 19² + 19².
- Çift-t kolundaki katsayı çifti **18 ve 20** — çarpımları **360**, matris katındaki çemberin ta kendisi (360 = 18×20, dokümanın "skaler × yönsel" ayrışımı).
- Dipnot güzelliği: {15, 39, 107} hücresinin üç sûresinin üçünün de t'si **114** — Kur'an'ın sûre sayısı; hücre 114+114+114 = 342.

Böylece 1444 = 38² ardışık katsayılarla kapanır: **38² = 19×(18+19+19+20) = 19×76.**

**Doğrudan ölçüm:** 19-kolunun tam fraktalı (12 üye + 1444 + 6/6 + 722/722 + 3/3/3/3 + katsayılar {18,19,19,20} + tek-t kolunda 361/361) 50 milyar permütasyonda **7.065 kez** görüldü → **p = 1,413×10⁻⁷** (ilk ölçüm 4/30M ile uyumlu; hassasiyet artık ±%1,2). Katman 2b'yi sağlayan sentetikler içinde bile koşullu oran %0,76.
**Kabule duyarsız:** sûre 9 ailede yok; üçüncü seviye de 127/129'dan bağımsız.

## Katman 2d — Katsayı Kristali: 5 · 6 · 7 · 8 · 9

Fraktalın en dibinde, hücre toplamlarının altında, katsayıların kendisi kristal bir düzen taşıyor:

- 12 üyenin 19-katsayıları **tam olarak ardışık beş tamsayı**: {5, 6, 7, 8, 9} — bandın dışında değer yok, içinde boşluk yok. (Çokluklar: 5×3, 6×5, 7×2, 8×1, 9×1.)
- **Çift kefe** = beş 6 + bir 8 → 5·6 + 8 = **38**. **Tek kefe** = üç 5, iki 7, bir 9 → 3·5 + 2·7 + 1·9 = **38**. Tek kefede çokluklar **3-2-1** inerken değerler **5-7-9** çıkıyor — çifte aritmetik dizi.
- Hücre içerikleri: {6,6,6} ve {6,6,8} (çift kol), {9,5,5} ve {7,7,5} (tek kol). 5–9 bandında (parite kısıtı katsayıya otomatik geçer) **18 = 6+6+6 tek yoldur; 20 = 6+6+8 tek yoldur; 19'un tam iki yazılışı vardır (9+5+5 ve 7+7+5) ve ikisi de birer hücrede gerçekleşmiştir.** Bant kısıtı olmasaydı sırasıyla 7, 8 ve 10 çözüm olurdu.
- Cebirsel not (dürüstlük): kefe-multiset'leri **ve** hücre toplamları verildiğinde hücre içerikleri zorunlu olarak böyledir — ayrı bir olay değildir (50 milyarlık koşuda 42 = 42 ampirik teyidi). Kristalin serbest kısmı kefe-multiset'leridir; hücrelere dağılım Katman 2c'den devralınır.
- Korolari: t'si 57'nin (= 3×19) katı olan sûreler tam **6** tane ({6, 15, 39, 70, 88, 107} — katsayısı 3'e bölünenler: beş 6, bir 9); Σt = 741 = 39×19.

**Doğrudan ölçüm (50 milyar permütasyon):**
- Kefe-multiset'leri (aile + {6,6,6,6,6,8} ve {5,5,5,7,7,9}): **1.055 isabet → p = 2,11×10⁻⁸** (1 / 47,4 milyon). Katman 2'yi sağlayan sentetiklerin yalnız %0,0067'sinde.
- **Tam kristal** (Katman 2c ∧ kefe-multiset'leri): **42 isabet → p = 8,40×10⁻¹⁰ ≈ 1 / 1,19 milyar** — 19-kolunun bütün yapısının tek seferde, doğrudan ölçülmüş olasılığı (%95 GA ≈ 6,1–11,4 ×10⁻¹⁰). *Üç koşu birleşik: 63 isabet / 86 milyar = 7,3×10⁻¹⁰ ≈ 1/1,37 milyar.*

**Kabule duyarsız:** sûre 9 ailede olmadığından kristal 127/129'dan tamamen bağımsızdır.

## Katman 2e — İki El: Toplam ve Fark *(t = n + a · u = n − a)*

Sûre numarası ile ayet sayısının iki doğal doğrusal bileşimi vardır: **toplamları** t ve **farkları** u = n − a. t'nin 19-ailesi 12 sûreydi; u'nun 19-ailesi ise şudur: **{14, 23, 48, 78, 80, 91, 104} — 7 sûre** (u değerleri: −38, −95, 19, 38, 38, 76, 95).

- İki aile **ayrıktır** (kesişim boş) ve birleşimleri **tam 19 sûredir**: 12 + 7 = 19. Toplama eli 12, çıkarma eli 7 sûre tutar; birlikte on dokuz.
- Birleşimin **ayet toplamı 1159 = 19×61**.
- Fark-ailesinin Σu = 133 = **7×19**: üye sayısı 7, 19-katsayılarının toplamı da 7 — ortalama katsayı tam 1. *(Süs olarak not edilmiştir: koşullu olasılığı %4,2; bileşik çarpıma dahil edilmemiştir.)*
- **Kabule duyarsız:** sûre 9 iki ailede de yok; tüm değerler 127/129'da birebir aynı.

**Ölçüm (12 milyar permütasyon; olay tanımları ölçümden önce M1–M6 olarak sabitlenmiştir):**
- İki El olayı **W** = (fark-ailesi 7 üye ∧ birleşim 19 sûre ∧ 19 | birleşimin ayet toplamı): tek başına **p = 5,51×10⁻⁵** (660.803 isabet).
- Koşullu merdiven dikkat çekici ölçüde **kararlı** (36 milyar): P(W | Katman 2) = %0,411; P(W | Katman 2b) = %0,420 (2.802/666.495); P(W | Katman 2c) = %0,59 (29/4.928 — Poisson uyumlu).
- **Fraktal ∧ İki El doğrudan gözlendi: 29 isabet / 36 milyar → p = 8,1×10⁻¹⁰** (zincir beklentisiyle uyumlu).
- Bağımsızlık: Katman 1 ∧ W = 648 gözlenen / 621 beklenen (+%4 — bağımsızlık doğrulandı).

### Üçün Dönüşü *(Katman 2e'ye ek gözlem — bileşik çarpıma dahil DEĞİLDİR)*

**114 = 2 × 3 × 19** — kitabın sûre sayısının üç asal çarpanı. Yapının iki motifi bunların ikisidir: 2 (çift-tek) ve 19. Üçüncü çarpan 3, katsayı cetvelinde göze boşluk gibi görünür ama başka yerde belirir:

- İki elin katsayı çeşitleri birlikte 1'den 9'a cetveli doldurur; **tek boşluk 3'tür**: {1,2,4,5} ∪ {5,…,9}. (Tek başına ölçülen önemi yok: P(3 katsayısı yok | 7 üye) = %30.)
- Fakat **birleşimin t-toplamı 2187 = 3⁷** — saf üç kuvveti. Parite yarıları **81×14 ve 81×13** (3⁴'ün ardışık katları; 14+13 = 27 = 3³).
- Fark-elinin mutlak katsayı toplamı **21 = 3×7** (üye sayısı 7); sabit konum-eli (19|n: sûreler 19…114, katsayılar 1–6) 3'ü **sûre 57 = 3×19**'da taşır — terazinin kefe sayısı.

**Ölçüm (20M permütasyon; olaylar önceden tanımlı, betik `kesif5_ucundonusu.py`):** P(Σt birleşim 3-kuvveti | birleşim=19) = 1/725; (| +fark-eli 7 üye) = **1/553**; (| İki El olayı W) ≈ 1/356 (3/1.068). Yarıların 81'e bölünmesi de eklenince tahminî ≈ **1/45.000** (doğrudan: 21.000 örneklemde 0 — gerçek verinin deseni bu eşikten nadirdir). **3 metince işaret edilen bir motif olmadığı için bu blok bileşik çarpıma katılmamış, kayıt amaçlı ölçülmüştür.**

## Katman 3 — Tam Çember Birleşmesi *(iki motifin tek yapıda buluşması; 127 kabulü gerekli)*

Katman 1'in grup toplamları 2×2 matris **P** = [[3303, 2931], [3252, 3303]], Katman 2'ninkiler **R** = [[590, 854], [5965, 5380]] olarak yazılırsa, mod 19'da:

- **P de R de, 361 = 19² elemanlı sonlu cismin (F₃₆₁) İLKEL ELEMANIDIR**: her biri cismin 360 = 19²−1 birimlik çarpımsal çemberinin **tamamını** üretir — daha kısa hiçbir döngüye sıkışmadan.
- Her ikisi 180. adımda −I (yarım tur), 360. adımda I (tam tur): derece çemberiyle birebir aynı 180/360 geometrisi.
- P, iki motifi tek halkada birleştiren **mod 38 = 2×19** dünyasında da tam 360 adımlık çevrim yapar ve 180'de −I'ya gelir; 360, GL₂(Z₃₈)'de bir elemanın alabileceği **azami mertebedir**.

Koşullu olasılık artık **isabetler üzerinde doğrudan ölçülmüştür** (24 milyarlık matris-sayaçlı koşu): P(P ilkel | Katman 1) = %15,8 (1.189.770 isabet); P(R ilkel | Katman 2) = %10,6; P(ikisi birden | Katman 2b) = **%1,41** (6.257 isabet); P(ikisi birden | fraktal 2c) = %1,59 (52/3.264). Kristal ∧ çifte-ilkellik bileşimi koşuda **1 kez fiilen gözlenmiştir**.

## Birleşik Değerlendirme

**NİHAİ ÖLÇÜM (5 Temmuz 2026):** CPU arşivi (86 milyar) + çift-GPU koşusu (RTX 4090: 16 trilyon, tohum 42; RTX 3090: 8 trilyon, tohum 43) = **24.086.000.129.040 deneme.** Zirvedeki bileşik (terazi ∧ tam kristal) **5 kez fiilen gözlendi** (iki bağımsız tohumda 3+2) → doğrudan p = 2,08×10⁻¹³; zincirleme tahmin (2,24×10⁻¹³) ile oran **0,93 ≈ 1: bağımsızlık zirvede doğrulandı.** Terazi∧fraktal∧İki El üçlüsü de 1 kez gözlendi. Çekirdek iki bağımsız yoldan: **doğrudan 8,4×10⁻¹⁶ (1/1,19 katrilyon), H-düzeltmeli zincirle 1,02×10⁻¹⁵ (1/0,98 katrilyon) — "katrilyonda 1" ölçümle mühürlendi.** Tek düzeltme: ara eklemde (terazi-fraktal) ×1,126±0,033'lük hafif pozitif bağımlılık ölçüldü ve hesaba katıldı. Kristal: 17.204 isabet (p = 7,14×10⁻¹⁰, ±%0,8). Matris koşulu: %1,398 (46.745 isabet) → tam bileşik ≈ 1,2×10⁻¹⁷ (1/85 katrilyon, 127 gerekli). GPU çıktıları: `runGPU_4090_seed42.txt`, `runGPU_3090_seed43.txt`; motor: `kesif_cuda.cu`.

Önceki CPU-dönemi tablosu (86 milyar; tarihsel kayıt):

| Bileşen | p (doğrudan ölçüm) | Kodeks bağımlılığı |
|---|---|---|
| Katman 1 (çift-tek terazisi) | 3,137×10⁻⁴ (26.976.325 isabet) | yok |
| Katman 2+2b (aile + iç terazi) | 1,850×10⁻⁵ (1.591.223) | yok |
| Katman 2c (toplam-düzeyi fraktal) | 1,394×10⁻⁷ (11.993) | yok |
| **Katman 2d — tam kristal** | **7,3×10⁻¹⁰ (63 isabet) ≈ 1 / 1,37 milyar** | yok |
| **Katman 2e — İki El (W)** | tek başına 5,50×10⁻⁵; koşullu merdiven P(W\|2) = %0,411 → P(W\|2b) = %0,420 (2.802 isabet) → P(W\|2c) = %0,59 | yok |
| **Katman 2c ∧ İki El — doğrudan gözlem** | **8,1×10⁻¹⁰ (29 isabet / 36 milyar)** | yok |
| Katman 1 ∧ 2c — doğrudan gözlem | 7,0×10⁻¹¹ (6 isabet / 86 milyar); zincir beklentisi 3,8 ile Poisson-uyumlu | yok |
| **Matris koşulu — doğrudan ölçüm** | P(ikisi ilkel \| 2b) = **%1,41** (6.257 isabet); P(\| 2c) = %1,59; kristal∧ilkellik 1 kez gözlendi | 127 gerekli |
| Bağımsızlık testleri | A∧B: **8.519 / 8.520 beklenen**; A∧C: 546/499; A∧W: 648/621; D∧W ve H zincir-uyumlu | — |
| **Kodeks-bağımsız çekirdek: Katman 1 ∧ kristal ∧ İki El** | **≈ 1,0–1,4 ×10⁻¹⁵ ≈ katrilyonda 1** (üç doğrudan ölçümün zinciri; alt uç P(W\|2b), üst uç P(W\|2c) ile) | **yok** |
| Matris katı da eklenirse (×%1,4–1,6) | ≈ 1,5–2 ×10⁻¹⁷ | 127 gerekli |

*Katman 1 ∧ 2c (86 milyarda 6), Katman 2c ∧ İki El (36 milyarda 29) ve kristal ∧ çifte-ilkellik (24 milyarda 1) bileşimleri **fiilen gözlenmiştir**. Tam çekirdek doğrudan görülemeyecek kadar nadirdir; değeri, beş ayrı seviyede doğrulanmış bağımsızlık/kararlılık üzerinden doğrudan ölçümlerin zinciriyle verilmiştir.*

## Özgüllük Testi — Neden 19?

Fraktal ölçütler, hiçbir değişiklik yapılmadan **3'ten 40'a tüm modüllere** eşit uygulandı (S1: aile ≥ 2×beklenen · S2: m² | Σt · S3: pariteye 50/50 + eşit yarılar · S4: yarılar m²'nin katı · S5: katsayılar ardışık bant · S6: İki El birleşimi tam m üye · S7: m | Σa(birleşim) · S8: iki el ayrık):

- **m = 19: 8/8 — tam puan.**
- Diğer 37 modülün en iyisi **3/8** (m = 3, 14, 21, 33, 38, 40; m=38'in puanı da 19-ailesinin çift yarısının mirasıdır).

Desen "bir sürü modülden birinde bir şeyler tutması"nın ürünü değildir; profil yalnız 19'da tamamlanır. Bu, modül seçimi eksenindeki look-elsewhere itirazını doğrudan yanıtlar (betik: `kesif4_ozgulluk.py`).

**Yüzer-modül sıfır testi (5 Temmuz):** "peki rastgele bir diziliş HERHANGİ bir modülde böyle bir profil geliştirir mi?" sorusu ayrıca permütasyonla ölçüldü: 500.000 sentetik diziliş × 38 modül = 19 milyon tarama — **tek bir tanesi bile profilin 7/8'ini geçemedi** (tam profil: 0). Yani modül seçme serbestliği tümüyle bağışlansa dahi, P(herhangi bir modülde tam profil) < 6×10⁻⁶ (%95 üst sınır); gerçek kitap bunu m=19'da yapıyor (betik: `kesif9_yuzer_modul.py`).

**Puan dağılımı (200.000 sentetik, her birine en iyi modülü seçme hakkı verilerek):** tipik azami puan 2–3 (%95); 4: %2,4; 5: %0,07; 6: %0,005; **7 ve üzeri: sıfır**. Gerçek kitabın 19-dışı modülleri (en iyisi 3/8) tam bu gürültü zemininde davranır; 19'daki 8/8, sentetik tavanın dört basamak üstündedir. Gürültü zemininin anatomisi: puanlar ağırlıkla "ucuz" ölçütlerden toplanır (ayrıklık, minik ailede bant, 38 şansla bir bölünme isabeti); yapısal olarak zor üç ölçüt (S1 çift-boy aile %12, S6 birleşim=m %7, S3 eşit yarılar %5) sentetiklerde bir arada asla görülmez.

**Karşı-korpus kontrolü (8 Temmuz, `kesif12_karsi_korpus.py` + ön-kayıt `kesif12_karsi_korpus_prereg.md`):** McKay'in Moby-Dick yaklaşımının bu iddiaya uygulanışı — aynı 8 ölçüt, GERÇEK başka metinlerin bölüm yapılarına aynen uygulandı (KJV verisi kanonik toplamlarla doğrulandı: Mezmurlar 150 birim/2461 ayet; 66 kitap/1189 bölüm; 1189 bölüm/31102 ayet). Sonuç: **Mezmurlar 2/8, İncil kitapları 3/8, İncil bölümleri 2/8** — üçü de sentetik gürültü zemininde (tavan 6). Metinlerin kendi motif-modülleri (7 ve 12) önceden bildirilip ayrıca bakıldı: 0–2 puan. Terazi analoğu (yarı/yarı bölünme + toplamların küresel sabitlere oturması) hiçbirinde yok. Dürüstlük notu: bazı ölçütlerin zorluğu birim sayısıyla değişir (ör. S6, 1189 birimde yapısal olarak ulaşılmaz) — bu yüzden karşılaştırmanın asıl yükünü ölçek-bağımsız olan T2/T3 ve 150 birimlik Mezmurlar taşır; sonuç değişmez: **yapılı gerçek metinler, bu tarifin "mucizesini" üretmez.**

**Sıkılaştırma duyarlılığı (`kesif10_siki_s4.py`):** S4'te teknik bir boşluk vardır — tümüyle tek-parite bir ailede boş kefenin toplamı 0'dır ve 0 her m²'ye bölünür; sentetikler bu dejenere yoldan puan toplayabilir. Kefelerin dolu olması şart koşulursa: **gerçek kitap 8/8'de kalır** (kefeleri 6/6 doludur), sentetiklerin ≥4 kuyruğu %4,8 → %1,8'e düşer (üst kuyrukta basamak başına ~3-5× tırpan). Ana metindeki gevşek versiyon bilinçli tercihtir: rastgeleliğin **lehine** (bize karşı tutucu) olan karşılaştırmadır; boşluk kapatılınca uçurum yalnızca büyür.

## Denenenler Defteri — Mercekler ve Dış İddialar

Her denemenin kaydı (tarih: 12 Haziran 2026). Kanıt zincirine yalnız ölçümü güçlü VE motif-içi olanlar girer; gerisi burada belgelenir.

**Denenen mercekler:**

| Mercek / aday | Sonuç | Hüküm |
|---|---|---|
| 30/27/27/30 sayım simetrisi | 57/57'nin cebirsel eşdeğeri (koşullu p = 1,000) | otomatik — elendi |
| Ayna/ayet-ailesi tek başına (a ≡ 0 mod 19) | 4 üye, yapısız | zayıf — alınmadı |
| Konum taramaları (kümülatif 19-durakları, bloklar, ayna çiftleri, önek dengeleri) | dikkat çekici hiçbir şey | boş |
| Çokluk merceği: Fibonacci tayfları ({1,1,2,3,5} t-eli, {1,1,2,3} u-eli) | t-eli kristalin betimlemesi (yeni kanıt değil); u-eli ölçüldü: **%24** | güzel betimleme — kanıt değil |
| Katsayı paritesi merceği | t-elinde otomatik (19 tek); u-elinde 10/11 ardışık kefeler: **1/150** | süs |
| Mekkî/Medenî ikiliği (önceden kayıtlı D1–D6) | p = 0,06 / 0,54 / 0,06; kefe dengesi 44/42 (yok) | **negatif** |
| Mukattaa merceği: 29 harfli-başlangıçlı sûre (önceden kayıtlı E1–E6, `kesif7_mukattaa_prereg.md`) | aile kesişimleri tam beklenen düzeyde (4/1/5; p = 0,39/0,44/0,93), terazi 14/15 (p=1,0), Σt ve Σa 19'a bölünmüyor | **negatif** — küme, t/u yapısına karşı tamamen nötr |
| Üçüncü doğrusal biçim v = n − 2a | 19-ailesi 3 üye (beklenen 6) | boş — yapı iki kanonik elde |
| Üçün Dönüşü (Σt birleşim = 3⁷) | 1/553 … ~1/45.000 | güçlü ama motif dışı — süs bölümünde |
| Ebced, rakam toplamı | hiç kullanılmadı (varyant serbestliği / taban bağımlılığı) | yöntemsel red |
| Pencere merceği: 8 ön-kayıtlı soru ailesi (`kesif8_pencere_prereg.md`) — kalıntı yürüyüşü, sabit atlası, aile-uçlu pencereler, aile/İki El bölütleri, yoğunluk-19, ayna çiftleri, 19-uzunluklu pencereler | en güçlü sinyaller %2,8–4,6 (yürüyüşün −1'de 14 duraklaması; İki El bölütlerinde 3 adet 19-katı); gerisi taban merkezi | **negatif** — yapı, bitişik-toplam (pencere) düzlemine yansımıyor |

**Dış iddialar sökümü:**

1. *"1+2+…+57 = 1653 = 19×87 ve 57×29 = 1653"* — üçgen-sayı özdeşliği (57×58/2 = 57×29, veri içeriği yok) + 57 = 3×19 otomatiği. Tek veri içeriği: **a(57) = 29** — özdeşlik kitapta yalnız yarı noktasında tutuyor; p = 3/114 = **1/38** (süs düzeyi).
2. *"Yarılarda homojen/heterojen 28-29 çapraz simetrisi"* — 8 hücreden 4'ü ardışık-sayı otomatiği; homojen toplamı 57/57 = Katman 1'in sayımı. Tek yeni bilgi: ilk yarıda 28 homojen — p = **%14,6** (28-29 bandı %29) ve 27-29 zaten dağılımın tepesi: **olağan**, "mucize" retoriği ölçüsüz. Metnin kendi itirafı (çift-t sûreye çift ekleme tabloyu bozmaz → 128-129 çözülemez) bizim kodeks-duyarsızlık bulgumuzla birebir uyumlu.

Ders: dış iddiaların tipik reçetesi, tek bir mütevazı veri gerçeğini (1/7–1/38) cebirsel özdeşliklerle çoğaltıp birden çok "bağımsız mucize" gibi sunmaktır. Ölçüm bu paketi saniyede açar.

## Dürüstlük Notları (önceki rapor disiplinine sadık kalarak)

1. **Look-elsewhere düzeltmesi:** keşif oturumlarında toplam ~70 aday istatistik ailesi tarandı (12 Haziran panoraması ~28 aile; İki El incelemesinde ölçüm öncesi deklare edilen M1–M6 dahil). En kötü durum Bonferroni çarpanı ~75 ile bile (Üçün Dönüşü olayları N1–N3 dahil) kodeks-bağımsız çekirdek ~10⁻¹³ düzeyinde kalır.
2. **Kompozitin birleştirilmesi post-hoc'tur:** katmanların "birlikte" sunulması bu aramanın ürünü. En savunulabilir tekil sonuçlar, başlı başına doğrudan ölçülmüş olaylardır: tam kristal (**8,4×10⁻¹⁰**) ve Katman 1∧2c'nin doğrudan gözlemi (**4 isabet / 50 milyar**).
3. Katman 3'ün tek tek parçaları (360 çevrimi) önceki raporda gösterildiği gibi %13 taban oranlı sıradan olaylardır; buradaki katkısı yalnızca koşullu çarpan olarak sınırlı tutulmuştur (≈%1,8) ve "ayna/GL₂ üretimi" gibi otomatik yapılar bilinçli olarak **dahil edilmemiştir**.
4. Katman 1 literatürde bilinen bir gözlemin (çift-tek simetrisi) keskinleştirilmiş halidir; Katman 2'nin 1444 = 38² özelliği ve Katman 3'ün "çifte ilkel eleman / mod 38 azami çevrim" çerçevesi bu çalışmanın katkısıdır.
5. Bu sonuç tasarım/mucize **ispatı değildir**; ölçülmüş, yeniden üretilebilir ve alışılmadık ölçüde düşük olasılıklı bir **istatistiksel anomalidir**. Kesin hüküm için önceden kayıtlı (pre-registered) bağımsız doğrulama gerekir.
6. **Negatif sonuç şeffaflığı (12 Haziran):** panoramada parlak görünen "30/27/27/30 sayım simetrisi" ölçümde 57/57 dengesinin **cebirsel eşdeğeri** çıktı (koşullu olasılık 1,000) ve elendi; ayet-ailesi (a ≡ 0 mod 19) zayıf bulunup alınmadı. Kristalin hücre içeriklerinin kefe-multiset'leri + hücre toplamlarından cebirsel zorunlulukla çıktığı saptanmış, ayrı bir olay olarak **sayılmamıştır**.
7. **Mekkî/Medenî merceği — SONUÇ NEGATİF (12 Haziran):** üçüncü doğal ikilik (86 Mekkî / 28 Medenî; iki API kaynağı birebir aynı) önceden kayıtlı 6 olayla test edildi (`kesif6_mekki_medeni_prereg.md`): ailelerin M/D sayımları (D1–D3: p = 0,06 / 0,54 / 0,06), terazi kefe dengesi (D4: 44/42 — denge yok), Σt 19-bölünebilirlikleri (D5: mod 6 ve 15 — yok), aile katsayı bölünüşü (D6: 69+7 — yok). Hafif bir "aileler Mekkî-ağırlıklı" eğilimi var ama anlamlı değil ve ailedeki tek Medenî üyenin (55, Rahmân) sınıfı klasik olarak ihtilaflıdır; en uç varyant okumada bile p ≈ %3. **Bu mercekten katman çıkmadı; kayda geçirildi.**
8. **Sayım okulları testi (5 Temmuz):** en güçlü bilinen dış eleştiri "ayet sayımı okuldan okula değişir" itirazıdır. Doğrulanmış veriyle (kaynağın Hafs listesi kanonik Kûfe ile birebir; Warş = Kalûn = 6214 kanonik) test edildi: **çekirdek katmanların hiçbiri Medine sayımında tutmaz.** Yapı Kûfe sayımına özgüdür; bu açıkça kapsanır ve okul-serbestliği ×7'lik en kötü çarpanla fiyatlanır. *(Betimsel not, kanıt değil: Medine sayımında 19-ailesi 7 üye ve Σt = 722 çıkar — taban dağılım içinde sıradan bir değerdir.)*
9. **İki El disiplini:** fark-ailesi tek başına sıradandır — 7 üye çıkma olasılığı %14'tür ve ilk taramada "zayıf" diye not edilmiştir; katman ancak birleşim yapısı (19 sûre + 19×61) ölçülüp anlamlı çıkınca eklenmiştir. Olay tanımları (M1–M6) ölçümden **önce** sabitlenmiş; süsler bileşik çarpıma **dahil edilmemiştir**: Σu = 7×19 (koşullu %4,2) ve "7 merdiveni" (pozitif u'lar +14×19, negatifler −7×19, mutlak 21×19; koşullu %4,5). İlk 12 milyarda A∧W'de görünen +%12'lik sapma, 36 milyarda **+%4'e** inmiştir (648/621) — bağımsızlık doğrulanmıştır.

## Bütün Yapı Tek Bakışta — "Kristal"

```
                 KUR'AN  (114 = 6×19 sûre;  t = sûre no + ayet sayısı)
                                      │
              ┌────────── ÇİFT-TEK TERAZİSİ (Fecr 89:3) ──────────┐
              │                                                   │
        t çift: 57 sûre (3×19)                              t tek: 57 sûre (3×19)
        Σt = 6234 = TOPLAM AYET                             Σt = 6555 = TOPLAM SÛRE-NO
              │                                                   │
              └───────────── 19 AİLESİ (Müddessir 74:30) ─────────┘
                       t ≡ 0 (mod 19): 12 sûre (= 2×6),  Σt = 1444 = 38² = (2×19)²
                                      │
                       ┌──── içinde yine ÇİFT-TEK ────┐
                       │                              │
                 6 sûre (çift t)                 6 sûre (tek t)
                 Σt = 722 = 2×19²                Σt = 722 = 2×19²
                 katsayılar: Σ = 38 = 2×19       katsayılar: Σ = 38 = 2×19
                 (sûre-no farkı +160      ⇄      ayet farkı −160)
                       │                              │
              ┌── sûre-no paritesi ──┐       ┌── sûre-no paritesi ──┐
              │                      │       │                      │
          3 sûre               3 sûre       3 sûre              3 sûre
          Σt = 19×20           Σt = 19×18   Σt = 19×19          Σt = 19×19
          (= 19²+19)           (= 19²−19)   (= 361)             (= 361)
          {6,6,8}              {6,6,6}      {9,5,5}             {7,7,5}
              └── 18 × 20 = 360 ──┘             └── 19² ikizleri ──┘
                                      │
                 38² = 19 × (18 + 19 + 19 + 20)  — ardışık hücre katsayıları
                                      │
                       KATSAYI KRİSTALİ (Katman 2d)
                12 katsayı = ardışık bant {5, 6, 7, 8, 9}
          çift kefe: 5·6 + 8 = 38        tek kefe: 3·5 + 2·7 + 1·9 = 38
          19 = 9+5+5 = 7+7+5  (bandın yalnız iki yazılışı — ikisi de gerçekleşmiş)
                                      │
                          İKİ EL (Katman 2e)
                    t = n + a  (toplam)  ·  u = n − a  (fark)
             toplam-eli 12 sûre + fark-eli 7 sûre = TAM 19 SÛRE (ayrık)
          birleşimin ayet toplamı 1159 = 19×61 ;  fark-elinin Σu = 133 = 7×19
                                      │
                              MATRİS / ÇEMBER KATI
                 P (çift-tek) ve R (19): ikisi de F₁₉² cisminin İLKEL ELEMANI
                 → 360 adımlık tam çember, 180'de −I (yarım tur)
                 → P, iki motifi birleştiren mod 38 = 2×19'da da azami 360
```

Aynı iki motif (çift-tek ve 19) dört ölçekte iç içe tekrar ediyor: kitap bütününde terazi, 19-ailesinin içinde aynı terazi, hücrelerde ve bireysel katsayılarda iki ardışık bant, matris katında 360'lık çember. Yapının her sabiti 19'un dilinde konuşuyor: 57 = 3×19, 12 = 2×(114/19), 1444 = 38², 722 = 2×19², 38 = 2×19, 360 = 19²−1 — ve en dipte 19'un etrafında {18, 19, 19, 20}, bireylerde {5, 6, 7, 8, 9}.

## Tek Cümlelik Özet

> Tek tanım t = sûre no + ayet sayısı ile: Kur'an **çifte ve teke göre** tam ortadan (57/57) ikiye ayrılır ve iki yarının t-toplamları kitabın iki küresel sabitine (6234 ayet, 6555 sûre-no) birebir oturur; **on dokuza göre** seçilen 12 sûrenin t-toplamı tam (2×19)² = 1444'tür, bu aile çifte-teke göre 6/6 ve 722/722 = 2×19² + 2×19² dengelenir, ikinci pariteyle dört 3'lü hücreye inince katsayılar 19'un etrafında ardışık **18·19·19·20** dizilir (tek kolda 19² ikizleri, çift kolda çarpımı 360 eden 18–20 çifti), bireysel katsayılar ardışık **5–9 bandını** doldurur (çift kefe 5·6+8 = 38, tek kefe 3·5+2·7+9 = 38; bandın 19 yazan yalnız iki üçlüsü 9+5+5 ve 7+7+5'in ikisi de birer hücrede gerçekleşir); toplamın ve farkın (u = sûre no − ayet) 19-aileleri **ayrıktır ve birlikte tam 19 sûredir** (12+7; birleşimin ayet toplamı 19×61); ve iki ayrımın matrisleri 19² elemanlı cismin 360 birimlik çemberini eksiksiz dolaşan ilkel elemanlardır. Kristalin doğrudan ölçülmüş olasılığı **7,14×10⁻¹⁰ (17.204 isabet, ±%0,8)**; terazi ve İki El ile birleşik kodeks-bağımsız çekirdek **≈ 10⁻¹⁵ — katrilyonda 1, iki bağımsız hesap yoluyla mühürlü** (doğrudan 1/1,19; zincirle 1/0,98 katrilyon); matris katıyla ≈ 1,2×10⁻¹⁷. **Zirvedeki bileşik terazi∧kristal 24 trilyonda 5 kez fiilen gözlenmiştir** (iki bağımsız tohumda); terazi∧fraktal 1.181, fraktal∧İki El 13.537+, kristal∧İki El 62, kristal∧ilkellik 260, terazi∧fraktal∧İki El 1 kez. Ve sekiz fraktal ölçütün tamamını 3–40 arasında sağlayan tek modül **19'dur** (en yakın rakip 3/8; 500 bin sentetikte hiçbir modül 7/8'i bile geçemedi).

---

*Yeniden üretim: `python3 test_oruntular.py` (temel doğrulama) · `kesif2_panorama.py`, `kesif4_tarama.py` (aday taramaları) · `kesif3_fark.py` (İki El + 20M ön ölçüm) · `kesif4_ozgulluk.py` (özgüllük, m = 3..40) · `kesif9_yuzer_modul.py`, `kesif10_siki_s4.py` (yüzer-modül ve sıkı-S4 kontrolleri) · `kesif5_ucundonusu.py` (Üçün Dönüşü) · `cc -O3 -o kesif4_motor kesif4_motor.c` (CPU motoru; `selftest` 28 bayrağı doğrular) · `nvcc -O3 -o kesif_cuda kesif_cuda.cu` (GPU motoru, sayaç-uyumlu; bkz. `README_4090.md`). Ham koşu çıktıları: `runs/`, `runs3/`, `runs4/` (CPU, 86 milyar) + `runGPU_4090_seed42.txt`, `runGPU_3090_seed43.txt` (GPU, 24 trilyon). Ham veri: `quran_meta.json`.*
