# Kur'an Matris Örüntüleri — Bağımsız Doğrulama Raporu

**Tarih:** 10 Haziran 2026
**İncelenen doküman:** `dokuman.md` (P/R matrisleri, mod 19 çevrimler, 360 örüntüsü)
**Test araçları:** `test_oruntular.py` (tüm hesaplar yeniden, bağımsız olarak yapıldı)
**Veri kaynakları:** `api.alquran.cloud/v1/meta` ve `api.quran.com/api/v4/chapters` (yerel kopyalar: `quran_meta.json`, `quran_chapters_qcom.json`)

---

## Yönetici Özeti

1. **Dokümandaki aritmetik ve cebirsel hesapların tamamı doğru** — ancak yalnızca **Tevbe sûresine 127 ayet kabul eden** (9:128–129'u Kur'an'dan sayan **standart metinden ayrılan**) veri setiyle.
2. **Standart Kûfe sayımıyla (Tevbe = 129, toplam 6236 ayet) 360 mimarisinin tamamı çöküyor:** ord(R)=18, ord(P)=30, etkileşim örüntülerinin hiçbiri tutmuyor.
3. Dokümanın en çarpıcı sunulan iki bulgusu — **"ortak 19 aynası" (DMD = M¹⁹)** ve **⟨P,R⟩ = GL₂(F₁₉)** — özel keşifler değil; ilki **evrensel bir cebir özdeşliği** (Cayley–Hamilton sonucu), ikincisi rastgele matris çiftleri için **%98 olasılıkla** zaten gerçekleşen bir durum.
4. Tek tek "360 çıkması" olayları da düşük olasılıklı değil: GL₂(F₁₉) içinde rastgele tersinir bir matrisin tam periyodunun 360 olma taban oranı **%13,3**.
5. **Sonuç:** Doküman hesap olarak tutarlı, ancak (a) standart olmayan bir metin kabulüne dayanıyor, (b) "dikkat çekici" diye sunulan yapıların çoğu matematiksel olarak otomatik veya istatistiksel olarak sıradan, (c) çok sayıda kural tarandıktan sonra tutan kurallar seçilmiş (seçim etkisi / "look-elsewhere" sorunu).

---

## 1. Veri ve Yöntem

İki bağımsız web kaynağından sûre numaraları ve ayet sayıları çekildi; **114 sûrenin tamamında birebir aynı** çıktı (standart Kûfe/Hafs sayımı, toplam 6236 ayet, Tevbe = 129).

Dokümandaki sayılar ise toplamı **6234** veren bir veri seti gerektiriyor (2931+3303 = 854+5380 = 6234). Bu, **Reşad Halife ekolünün** 9:128–129'u metinden çıkaran "127 kabulü"dür. Doküman bu kabulü P matrisi bağlamında açıkça tartışıyor, ancak **R matrisinin ve tüm etkileşim sonuçlarının da aynı kabule bağımlı olduğunu hiçbir yerde belirtmiyor.**

Bütün iddialar her iki kabul altında ayrı ayrı test edildi.

## 2. İddia Bazında Doğrulama Tablosu

| # | İddia | RK-127 kabulü | Standart (129) |
|---|---|---|---|
| 1 | t≡0 mod 19 olan 12 sûre: {6,15,21,39,41,42,50,55,56,70,88,107} | ✅ | ✅ (küme aynı) |
| 2 | Seçilen grup toplamları 590 / 854 | ✅ | ✅ |
| 3 | Kalan grup toplamları 5965 / **5380** | ✅ | ❌ (5382 çıkar) |
| 4 | R mod 19 = [[1,18],[18,3]], ord(R)=360, R¹⁸⁰=−I, R²⁰=2I | ✅ | ❌ ([[1,18],[18,5]], **ord=18**) |
| 5 | Ham P = [[3303,2931],[3252,3303]], P mod 19=[[16,5],[3,16]] | ✅ | ❌ (2933 çıkar → [[16,7],[3,16]]) |
| 6 | ord(P)=360, P¹⁸⁰=−I, P²⁰=13I | ✅ | ❌ (**ord=30**) |
| 7 | r-taraması: yalnız r=0 ve r=4 sınıfları 360 verir | ✅ | ❌ (360 verenler: r=3, 7, 9) |
| 8 | r=4 sûreleri {7,35,58,71,78,113}, toplamlar 362/346, ord=360 | ✅ | ❌ (ord=9) |
| 9 | Ayet çift/tek → ord 72; t-tekrar → ord 72; ayet≡0 mod 19 → ord 18; sûre no çift/tek → tersinir değil | ✅ | kısmen ❌ (72'ler 9 ve 342 olur) |
| 10 | ord(PR)=ord(RP)=18 | ✅ | ❌ (9, 9) |
| 11 | S=PR+RP=[[5,7],[18,12]], ord(S)=360 | ✅ | ❌ (ord=6) |
| 12 | D=PR−RP=[[17,10],[13,2]], D²=I | ✅ | ❌ |
| 13 | Komütatör K=PRP⁻¹R⁻¹, ord(K)=20, K¹⁰=−I | ✅ | ❌ (ord=10) |
| 14 | DPD=P¹⁹, DRD=R¹⁹, DSD=S¹⁹ ("ortak 19 aynası") | ✅ | ❌ |
| 15 | ⟨P,R⟩ = GL₂(F₁₉), \|GL₂(F₁₉)\|=123120 | ✅ | ❌ (61560 = indeks-2 altgrup) |
| 16 | Saf cebir: K²=2I, R¹⁹=2I−K, R²⁰=2I, ord₁₉(2)=18, (19²−1)(19²−19)=123120 | ✅ (veriden bağımsız, doğru) | ✅ |
| 17 | s-taraması: R_s ailesinde 360 verenler s=1,16 (2/19) | ✅ | ✅ |
| 18 | c-taraması: P_c ailesinde 360 verenler c=5,6,11,17 (4/19); P₁₂₉ ord=30 | ✅ | ✅ |
| 19 | Küçük değişiklik örnekleri (855 senaryosu→18, hücre+1→18, s=2→18) | ✅ | — |

**Özet:** Dokümanın yazarı hesaplarını doğru yapmış. Sorun hesapta değil, **veri kabulünde ve yorumda**.

## 3. Kritik Bulgu: Her Şey 9:128–129'un Çıkarılmasına Bağlı

Dünya genelinde basılan tüm mushaflarda ve bütün klasik sayım geleneklerinde (Kûfe, Basra, Medine, Şam, Mekke) 9:128–129 Kur'an metninin parçasıdır; Kûfe sayımında Tevbe 129 ayettir. İki bağımsız çevrim-içi kaynak da bunu doğruluyor.

Standart metinle:

- R = [[1,18],[18,5]] → **ord 18** (360 değil)
- P = [[16,7],[3,16]] → **ord 30** (360 değil)
- 360 veren kalıntı sınıfları r=0 ve r=4 değil, **r=3, 7, 9** olur — yani "doğal anlamı olan r=0 sınıfı" özelliğini kaybeder
- S, D, K, ayna ve GL₂ üretim sonuçlarının tamamı bozulur

Yani örüntü, "Kur'an'ın yapısından" değil, **iki ayetin metinden çıkarılması kararından** doğuyor. Üstelik bu dairesel bir gerekçelendirme riski taşıyor: 127 kabulünün kendisi de 19 mucizesi iddiasını kurtarmak için yapılmış bir müdahaledir; sonra bu kabul üzerinden bulunan yeni 19 örüntüleri kabulün kanıtı gibi sunulamaz.

### 3b. "127" Tek Seçenek mi? — Perturbasyon Taraması

"Acaba 127 değeri eşsiz mi?" sorusu için iki tarama yapıldı:

- **Tevbe taraması (109–149):** P ve R'yi aynı anda 360 yapan iki değer var (127 ve 138); tam mimariyi (S=360, D²=I dahil) veren tek değer 127. Tevbe'ye dar bakışla 127 "özel" görünür.
- **Genel tarama (114 sûre × ±1..±5 = 1123 deneme):** Tam mimariyi veren **47 kombinasyon** bulundu ve **hepsi delta = −2**. Yani 114 sûrenin 47'sinde, o sûreden 2 ayet silmek **birebir aynı mimariyi** üretiyor (aynı P=[16,5;3,16], aynı 360'lar, aynı ayna). Fâtiha'dan, Bakara'dan veya Nisâ'dan 2 ayet silmek de "Tevbe'den 2 ayet silmek" kadar işe yarıyor.

Mekanik sebep: matris yalnızca sütun toplamlarına (mod 19, mod 2) ve grup üyeliklerine duyarlıdır. −2 değişiklik t paritesini korur; sûre çift-t grubundaysa ve mod 19 sınıf üyeliği kaymıyorsa sonuç her durumda aynıdır.

**Çıkarım:** Örüntü "Tevbe = 127"yi değil, yalnızca "toplamdan 2 ayet eksilt" işlemini seçiyor; bunun 47 eşdeğer yolu var. Dolayısıyla bu örüntü, 9:128–129'un metinden çıkarılması gerektiğine dair bir kanıt olarak kullanılamaz — Tevbe'nin seçilmesi matematiksel değil, tarihsel/teolojik bir tercihtir.

## 4. "Dikkat Çekici" Bulguların Cebirsel Anatomisi

Bağımsız Monte Carlo simülasyonu (GL₂(F₁₉) içindeki 16.416 adet 360-periyotlu elemandan rastgele çiftler) ve teorik analizle:

| Dokümandaki bulgu | Gerçek durum | Taban oranı |
|---|---|---|
| ord(M)=360 olması | GL₂(F₁₉)'da maksimum periyot; sıradan | **%13,3** (16416/123120) |
| D=PR−RP için D²=I | tr(PR)=tr(RP) olduğundan **her zaman** D²=−det(D)·I (Cayley–Hamilton). Tek koşul det(D)=−1 | %5 |
| **DPD=P¹⁹, DRD=R¹⁹, DSD=S¹⁹** | **Evrensel özdeşlik.** tr(DM)=0 her M∈{P,R,S} için cebirsel zorunluluk; karakteristik polinomu indirgenemez her M için M¹⁹=tr(M)I−M (Frobenius). D²=I sağlanan **her** çift için otomatik | **%100** (150/150 simülasyon) |
| ⟨P,R⟩=GL₂(F₁₉) | Rastgele iki 360-elemanı zaten neredeyse her zaman tüm grubu üretir | **%98** |
| ord(PR+RP)=360 | Tek matris taban oranıyla aynı | %12,2 |
| ord(PR)=18 | Sık görülen kısa periyot | %22,2 |
| 360 = 18×20, M²⁰=det(M)·I | M²⁰ skaler olması, 360-periyotlu tüm elemanların ortak yapısıdır (F₃₆₁* içindeki gömme); "iki saatin paralelliği" değil, 360-periyotlu **olmanın tanım gereği** | otomatik |
| "P ve R aynı iskeleti paylaşıyor (20/180/360)" | 360-periyotlu **her** matris bu iskeleti paylaşır | otomatik |

En çarpıcı sunulan "ortak 19 aynası" bölümü, 2×2 matris cebirinin ders kitabı düzeyinde bir sonucudur: dokümanda "rastgele bir fark matrisi için beklenmez" denmesine karşın, simülasyonda D²=I sağlanan **istisnasız her çiftte** ayna özelliği gerçekleşti.

## 5. İstatistiksel Değerlendirme (Seçim Etkisi)

- Dokümanın kendisi en az **8 farklı bölme kuralı** taradığını yazıyor (t çift/tek, t mod 19'un 19 sınıfı, ayet paritesi, sûre paritesi, ayet-19, t-tekrar, …). Taban oranı %13 iken çok sayıda kural taranırsa birkaçının 360 vermesi **beklenen** sonuçtur.
- Nitekim **standart veriyle de 19 kalıntı sınıfından 3'ü (r=3,7,9) 360 veriyor** — 127 verisindeki 2 sınıftan (r=0,4) daha fazla. Yani "360 çıkması" verinin özel olduğunu göstermiyor; hangi sınıfların çıktığı kabule göre kayıyor ve anlatı, tutan sınıfın etrafına sonradan kuruluyor (r=0 tutunca "doğal anlamı var", r=4 tutunca "keşif adayı").
- Ayrıca iki serbest metin kabulü (127/129) arasından örüntüyü vereni seçme imkânı, etkin deneme sayısını ikiye katlıyor.
- Dokümanın kendi kaba tahmini (%2,4) bile, taranan kural sayısı (≥8) ve kabul seçimi (×2) hesaba katıldığında anlamlılığını yitirir: bağımsız varsayımla dahi beklenen "çifte 360" sayısı ~1'in üzerindedir.

### 5b. "6234'ü Kabul Etsek Bile Etkileyici mi?" — Sentetik Veri Testi

Soruya en dürüst cevabı vermek için yazarın keşif süreci aynen sentetik verilere uygulandı: gerçek ayet sayıları (6234 toplamlı set) sûreler arasında 500 kez rastgele karıştırıldı; her sentetik veri setinde aynı 23 kural tarandı (t-paritesi, ayet-paritesi, ayet-19, t-tekrar, t≡r için 19 sınıf), 360 veren kurallar bulundu ve her 360-çifti için tam etkileşim mimarisi (S=PR+RP→360 ve D²=I) kontrol edildi.

| Ölçüm | Gerçek 6234 verisi | Sentetik (500 deneme) |
|---|---|---|
| 360 veren kural sayısı | 3 (t-parite, t≡0, t≡4) | ortalama 2,8 |
| En az iki 360-kuralı çıkması | evet | %82 |
| En az bir çiftte tam mimari | evet (1 çift) | **%3,2** |

Ayrıca rastgele 360-çiftleri üzerinde ölçülen taban oranlar: ord(S)=360 → %12,3; D²=I → %5,4; ikisi birlikte → %0,64; tüm tablo (PR=18, RP=18, S=360, D²=I, K=20) → %0,14. Bu sayılar tek başına etkileyici görünse de, keşif sürecinin çok sayıda kural ve işlem taraması nedeniyle düzeltilmiş gerçek şaşırtıcılık **~%1–3** bandındadır.

**Yorum:** 6234 kabul edilse bile, dokümandaki mimarinin "herhangi bir rastgele veri setinde aynı keşif süreciyle bulunma" olasılığı yaklaşık %3'tür; gerçek çiftin "doğal anlamlı" t≡0 sınıfını içermesi bunu en fazla ~%1'e indirir. Bu, keşifsel (pre-registered olmayan) bir analizde p≈0,01–0,03 düzeyine karşılık gelir: **hafif ilginç, ama olağanüstü değil.** Üstelik bu hesap, 6234'ün kendisinin (47 eşdeğer yol arasından) ve mod 19 / 2×2 toplam-matris yönteminin kendisinin de seçilmiş olduğunu hesaba katmaz; bunlar dahil edildiğinde kalan şaşırtıcılık pratikte kaybolur.

## 6. Doğru Çıkanlar — Hakkını Teslim Etmek

- Tüm matris aritmetiği, kuvvet hesapları, periyotlar, taramalar ve grup üretim sayısı (RK-127 verisi üzerinde) **hatasız**.
- Saf cebirsel önermeler (K²=2I, R²⁰=2I, ord₁₉(2)=18, |GL₂(F₁₉)|=123120, s/c-taramaları) **doğru**.
- Dokümanın "bu otomatik mi?" bölümlerindeki öz-eleştirel ton (tek başına kanıt değil, P ve R bağımsız değil, kaba tahmin gerçek p-değeri değil) **yerinde ve dürüst**; ancak vardığı "ciddi biçimde dikkat çekici" hükmü, yukarıdaki otomatiklik ve seçim-etkisi analiziyle desteklenmiyor.

## 7. Sonuç

| Soru | Cevap |
|---|---|
| Hesaplar doğru mu? | Evet (RK-127 verisiyle birebir). |
| Standart Kur'an metniyle geçerli mi? | **Hayır** — Tevbe=129 ile tüm 360 yapısı çöküyor. |
| "360 çevrimi" nadir mi? | Hayır — taban oranı %13,3; çok kural taranınca beklenen sonuç. |
| "19 aynası" özel mi? | Hayır — Cayley–Hamilton'dan kaynaklanan evrensel özdeşlik (%100). |
| GL₂(F₁₉) üretimi özel mi? | Hayır — rastgele çiftlerin %98'i üretir. |
| Bilimsel kanıt değeri | Mevcut haliyle yok: standart-dışı metin kabulü + seçim etkisi + otomatik cebirsel yapılar. |

**Genel hüküm:** Doküman, mod 19 matris cebirinin gerçek ve doğru hesaplanmış özelliklerini, standart olmayan bir metin kabulü üzerine kurup, cebirsel olarak zorunlu veya istatistiksel olarak sıradan yapıları "dikkat çekici uyum" olarak yorumluyor. İddiaların hiçbiri, standart Kur'an metni üzerinde ve çoklu-test düzeltmesi yapılmış bir istatistiksel çerçevede ayakta kalmıyor.

---

*Tüm hesaplar `test_oruntular.py` ile yeniden üretilebilir: `python3 test_oruntular.py`*
