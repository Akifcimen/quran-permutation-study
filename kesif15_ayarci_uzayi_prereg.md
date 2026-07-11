# KESIF-15 ÖN KAYIT: Ayarcı Uzayı (İnsan-Eli Hipotezinin Ölçümü)

**Tarih:** 11 Temmuz 2026 · **Statü:** Analizden ÖNCE yazıldı. Çekirdek kanıt zincirinden ayrı bir soruyu ölçer.

## 1. Soru

İnsan-eli hipotezinin en güçlü sürümü: "Kûfe sayım okulu, duraklama (ayet sonu) kararlarını
kullanarak yapıyı bilerek ayarladı." Ayarcının **meşru serbestlik uzayı**, okulların fiilen
ayrıştığı kararlardır. Soru: **çekirdek yapı bu meşru karar uzayında ne kadar yoğun / ne kadar
yalıtık?** Ayarcı kaç eşgüdümlü kararı tam isabetle vermek zorundaydı?

## 2. Uzay tanımı (analizden önce sabitlendi)

- Sûre dizilişi sabit (tarihsel olarak ayarcının elinde değil: Mushaf tertibi ayrı zincir).
- Her sûre için ayet sayısı, **belgeli iki okul değerinden biri** seçilebilir:
  Kûfe (kanonik, toplam 6236) veya Medine Warş/Kalûn (toplam 6214; `sayim_medine_warsh.json`).
- Ayrışan sûre sayısı **k = 50** → uzay **2⁵⁰ ≈ 1,13×10¹⁵** yapılandırma.
- Gerekçe: bu, duraklama serbestliğinin *belgelenmiş* kısmıdır. Gerçek tarihsel serbestlik daha
  büyük ve daha incedir (Basra/Mekke/Şam okulları; sûre-içi tekil sınır kararları); yani burada
  ölçülen "ayar hassasiyeti biti" bir **alt sınırdır**, yoğunluk ifadesi bu alt-uzaya kapsamlıdır.

## 3. Puanlayıcı (kesif11'den BİREBİR; yeni serbestlik yok)

Her yapılandırma A (114 ayet sayısı) için, t=n+a, u=n−a, kendi toplamıyla:
- **A1 Terazi:** |{t çift}| = 57 VE Σt(çift) = Σa (yapılandırmanın kendi toplamı — öz-gönderim korunur)
- **B Aile:** |{t≡0 (19)}| = 12 VE Σt(aile) = 1444
- **C İç terazi:** B VE |aile∧çift| = 6 VE Σt(aile∧çift) = 722
- **W İki El:** |{u≡0 (19)}| = 7 VE |aile∪u-aile| = 19 VE Σa(birleşim) ≡ 0 (mod 19)

Skor = geçen katman sayısı (0–4). "Tam yapı" = 4/4.

## 4. Analizler

- **A1 (birincil):** 2⁵⁰ uzayında tam-yapı yapılandırmalarının **kesin sayısı** (istatistiklerin
  sûre-yerel ayrışması + tam sayım/DP; örnekleme değil). Ayar hassasiyeti = log₂(2⁵⁰ / çözüm sayısı) bit.
- **A2:** Kûfe köşesi çevresinde Hamming halkaları r = 1, 2, 3 (20.875 yapılandırma, doğrudan
  değerlendirme): skor kaç adımda, hangi katmanlardan düşüyor? (Elle-iyileştirme gradyanı var mı?)
- **A3:** Skor merdiveni: tekdüze rastgele 10⁷ örnek (tohum 42) ile skor dağılımı.
- **A4:** Katman-başına kesin marjinal sayımlar (her katman tek başına uzayı ne kadar kısıtlıyor).
- **Sağlamalar:** saf-Kûfe köşesi 4/4 vermeli; saf-Medine kesif11 sonucunu (✗✗✗✗ deseni) vermeli;
  DP toplamı 2⁵⁰'ye eşit olmalı.

## 5. Yorum kuralları (önceden taahhüt)

- **Çözüm sayısı = 1 (yalnız Kûfe köşesi):** yapı meşru karar uzayında tam yalıtık; ayarcı ~50
  ikili kararın HEPSİNİ eşgüdümlü ve isabetli vermek zorundaydı (≈50 bit ≈ 10¹⁵'te 1 hedef).
  Ayrıca hiçbir karma-okul sayımının yapıyı üretmediği gösterilmiş olur (okul-özgüllüğü güçlenir).
- **Az sayıda çözüm, Kûfe köşesine Hamming-yakın:** yapı bazı kararlara duyarsız; ayar
  hassasiyeti bit sayısı kadar azalır, okul-bağımlılık cezası da yumuşar. Dürüstçe iki yönde rapor edilir.
- **Çok sayıda dağınık çözüm:** yapı bu uzayda jeneriktir → hem ayar kolaydı hem de Kûfe
  özgüllüğü zayıflar (keşif aleyhine). Bu da aynı açıklıkla yazılır.
- Bu test ŞANSI ölçmez (o, permütasyon sıfırıyla ölçüldü); yalnız insan-ayarcının gereken
  hassasiyetini ve yapının varyant-uzayı jenerikliğini ölçer.
- A2'de r=1'de skorun hemen ≤2'ye düşmesi, "gradyansız arazi" demektir: tam değerlendirme
  yapamayan (günde ~1 el hesabı) bir ayarcı için açgözlü iyileştirme yolu yoktur.

## 6. Dürüstlük notları

- Uzay bir vekildir: yalnız iki okulun sûre-bazlı verisi elimizde. Daha çok okul → daha büyük uzay
  → daha yüksek gerçek bit sayısı; bu yüzden A1 sonucu alt sınır olarak raporlanır.
- "Okul paketi" taneciği kabadır: gerçek ayarcı tekil ayet sınırlarını oynatır; bu, uzayı daha da
  büyütür (aynı yönlü muhafazakârlık).
- Sonuç ne çıkarsa çıksın rapora girer; dosya yereldir, yayımlama kararı ayrı verilir.

---

# SONUÇ (11 Temmuz 2026 — analiz sonrası eklendi)

**A1 (kesin sayım):** 2⁵⁰ = 1,126×10¹⁵ yapılandırma içinde tam-yapı (4/4) çözümü **23.863.920**
→ yoğunluk **2,12×10⁻⁸** → ayar hassasiyeti **25,5 bit** (47,2 milyonda 1 hedef; alt sınır).
Sağlamalar geçti (saf Kûfe 4/4; saf Medine 0/4 = kesif11; DP toplamı 2⁵⁰; A2 doğrudan batarya
değerlendirmesi DP çözüm varlığını bağımsız yoldan teyit etti).

**Beklenmedik bulgu — değişmezlik grubu:** Dört karar bataryaya tamamen görünmez: sûre **11, 22,
27, 38** (hepsi çift-t, çift-delta, mod-19 üyeliklerine dokunmuyor). İnce yapı dahil HER ŞEY korunuyor
(aile t-listesi aynen, Σa(İki El)=1159 aynen, öz-gönderimli kefe eşitliği yeni toplamı takip ederek —
örn. sûre 27 çevrilince toplam 6238 olur ve çift kefe 6238'i verir). Yani yapı tek nokta değil,
**16 tam-eşdeğer yapılandırmalık bir yörünge**; kalan ~1,49 milyon "görünür-farklı" çözüm ise
toplamları koruyan ayarsız karışımlardır. Belgeli okullardan çözüm kümesinde olan TEK sayım Kûfe'dir
(kesif11 hükmü değişmedi; karışımlar tarihsel olarak tanıksız kompozitlerdir).

**A2 (zirve çevresi):** r=1'de 50 tekil kararın **11'i yapıyı 0/4'e düşürür**, 29'u 3/4'te bırakır,
4'ü görünmezdir. **A3 (arazi):** meşru uzayda rastgele yapılandırma %99,4 skor 0; skor≥3 ancak
2,8×10⁻⁵ (36 binde 1); 10⁷ örnekte 4/4 hiç görülmedi. **A4 (marjinaller):** katman başına 10⁻³–10⁻⁴;
bileşik 2,1×10⁻⁸.

**Yorum (ön-kayıt kurallarına göre):** Orta durum — çözüm tek değil ama yoğunluk 10⁻⁸ mertebesinde:
yapı bu uzayda jenerik DEĞİL. İnsan-ayarcı, yalnız belgelenmiş serbestlik içinde bile **en az ~25
ikili duraklama kararını tam eşgüdümle** vermek zorundaydı; arazi %99,4 sinyalsiz olduğundan açgözlü
el-iyileştirme yolu yok; kör aramanın beklenen maliyeti günde bir tam el hesabıyla **~129.000 yıl**.
Cebirle ileriye-doğru kurma bu aramayı atlar — ama o da kavram envanteri + konumsal hesap ister
(tarihsel boşluk aynen durur). Ayrıca dürüst kayıt: "yapı Kûfe köşesine özgü" ifadesi inceltilmelidir —
yapı, ölçüsü 2×10⁻⁸ olan bir dilimde yaşar ve 4 kararlık bir değişmezlik yörüngesi taşır.
