# ÖN-KAYIT — plasebo14: bütünlük plasebosu — her hedef kendi bütünlüklü dilini kursa, Kur'an'ın bütünlüğü ne kadar sıra dışı?

**Oturum:** Claude (Opus 5.5), `~/opus5.5/plasebo14`, görev `GOREV.md` (29 Ekim 2026; SHA-256 `f4224b6c…5b13`, `gorev_sha256.txt` ile
eşleşti). `~/Desktop/hrf2`, `~/opus5.5/butunluk`, `~/opus5.5/kosullu`, `~/opus5.5/plasebo` ve `~/opus5.5/plasebo12` salt okunurdur;
`~/opus5.5/butunluk`, `~/opus5.5/kosullu` ve `~/opus5.5/plasebo` bu oturumda hiç açılmadı. plasebo12'nin kodu kopyalandı; referans
tabloları ve hedef dosyaları plasebo12'deki yerlerinde SHA-256 ile doğrulandı.
**Bu belge hiçbir hedefin (Kur'an dahil) bu görevin dilleriyle kurulan sonucuna ya da kontrol puanına bakılmadan yazıldı ve donduruldu.**
Dondurma anı ve SHA-256: `sonuc/onkayit_ozet.txt`; dondurma anındaki kod özetleri: `sonuc/onkayit_kod_sha256.txt`. Sapmalar yalnız
`ek/` altındaki zaman damgalı eklerde yazılır; bu dosya değiştirilmez.

Kısaltmalar: **plasebo12** = on üçüncü denetim (`~/opus5.5/plasebo12`, ön-kayıt SHA-256 `3837c9d8…71a5a`); **12. den.** = on ikinci denetim;
**L\*** = plasebo12'nin 109 yüklemli kütüphanesi; **D** = plasebo12'nin kısıtsız dil kurma yordamı; **D_B** = bu görevin bütünlük kısıtlı
yordamı; **p_x** = hedef x'in kendi dilinde kendi kontrollerine karşı sırası; **p_kal** = kalibre p.

**Bu testin yeri.** Bu test, ilk plasebonun (plasebo12) sonucunu gördükten sonra tasarlandı. Önceden kaydedilir; iki sonuç raporda yan
yana yazılır ve ikincisi birincinin yerine geçmez.

---

## 0. Dondurmadan önce yapılanlar (ve yapılmayanlar)

Görevin izin verdikleri yapıldı: kod doğrulamaları (yalnız uyuşmazlık sayıları), plasebo12 ile eşitlik sınamaları, hız ölçümü.

**(a) Girdi bütünlüğü.** plasebo12'nin 30 kaynak dosyası kopyalandı (`sonuc/p12_kopya_sha256.txt`; kopyalar özgünlerle aynı; 15'i plasebo12'nin
dondurma anı özetleriyle de aynı). Referans tabloları (`tab/{M,U,L}/ref_{1111,2222,0444,0888}.ref`, 12 dosya) ve hedef dosyaları
(`tab/{M,U,L}/hedef.bin`) `sonuc/ref_sha256.txt` ve `hedef_sha256.txt` ile doğrulandı: 12/12 ve 3/3 OK. Hedef dosyaları bu klasörün
`tab/<sıfır>/`'ına kopyalandı (kopyalar 3/3 OK); referans tabloları yerinde okunur.

**(b) Ö0 çapaları (plasebo12 ile eşitlik; yalnız karşılaştırma sonucu basıldı; `sonuc/o0/`).**
- Kısıtsız D (`src/dil_b`, P kipi = plasebo12'nin `dil_cikar.c` yolu): üç sıfırda `diller.txt` (P0 dilleri + Kur'an'ın P2 dili + el dili
  satırı) ve `ayrinti.txt` **bit-bit aynı** (M 203, U 103, L 103 satır; 0 farklı satır; SHA-256 `a55236f9…`, `61c82759…`, `ec36dd60…`).
- Kontrol programı (`src/kontrol_b`, kol 0 = plasebo12 puanı), plasebo12'nin düzey-1 dilleriyle bölüt 1'de: `esikler.txt` (Kur'an'ın Q,
  QP2 ve el dili eşikleri dahil bütün eşikler), `b0001.txt` sayımları (M 403, U 203, L 203 satır) ve aday dosyası `b0001.q` üç sıfırda
  **bit-bit aynı**. Sınama, Kol B'nin partili değerlendirmesi eklendikten sonra yeniden koşuldu; yine bit-bit aynı.
- Böylece kütüphane L\*, yüklem bedelleri, bilgi puanı, kısıtsız D ve kontrol akışı (aynı tohumlar, aynı kitaplar) plasebo12'dekiyle aynıdır.

**(c) Kod doğrulamaları (yalnız HEDEF OLMAYAN kitaplar: tür 4, sıra 100–2099; mekanik kontroller: tür 8; `sonuc/dogrulama/`).**
- Kaba-kuvvet D_B ve en geniş dil (`kontrol/dilb_kaba`), üç sıfırda 2.000'er kitap, üç kol (3/3, 2/2, 4/4): kapı G'nin tuttuğu ve sağlanan
  L\* yüklemi ≤ 16 olan her mercekte bütün alt kümeler bağımsız yoldan (F/W sayımları her girdinin alt maskeleri sayılarak; üst-küme
  dönüşümü kullanılmadan) puanlandı. Uyuşmazlık: (i) D_B'den iyi alt küme 0; (ii) D_B merceğinde eşitlik 0; (iii) tam taramada amaç ve
  dilsizlik kararı 0; (iv) dilin doğrudan puanı, B ve sağlanma 0; (v) en geniş dil (mercek, puan, liste) 0; (vi) tam taramada eşitlik
  kurallı seçilen dil (liste ve mercek) 0. Kapsam bayrakları (önceden yazılmış alt sınırlar): (ii) ≥ 300 evet, (iii) ≥ 300 evet,
  "taranan mercek ≥ 1.000" hayır.
- Kontrol programının bağımsız yeniden sayımı (`kontrol/kontrolb_sinama`; doğrudan double puan, Kol B'de budamasız): 16.000 tür-8 kontrol,
  hedef olmayan kitapların A, A−, A+ ve Kol B dilleri, üç sıfır: k, kB, eşik (kontrol_b) ve eşik (dil_b ayrıntısı) uyuşmazlığı 0
  (kapsam: toplam isabet ≥ 50 evet). Kol B'nin partili değerlendirmesinden sonra yeniden koşuldu; yine 0.
- `src/hedef_ayrinti_b` hedef olmayan kitaplarda çalışıyor (girdi ve çıktı satır sayıları eşit). Analiz betiği `src/analiz_b.py`,
  hiçbir hesaptan gelmeyen sentetik bölüt dosyalarıyla uçtan uca sınandı (R'nin bağımsız yeniden sayımı aynı).

**(d) Hız ve bellek (`sonuc/dogrulama/hiz.txt`; tür 6 kontrolleri, hedef olmayan kitapların dilleri).** Kontrol başına taban (üretim +
profil + kapı listesi) 72–76 µs (çekirdek); Kol A/A−/A+ dili başına ek ~0,9 µs/kontrol; Kol B dili başına ek ~5,4 µs/kontrol (partili
değerlendirmeyle; partisiz 17,5). Kol B tabloları: hedef olmayan 201 kitabın en geniş dilinde dil başına en çok 55 MB, toplam 864 MB.

**(e) Açıklama — bilgi ve istenmeden oluşan sızıntılar.**
1. plasebo12'nin yayımlanmış sonuçlarını biliyorum (rapor ve BENIOKU). Rapor, Kur'an'ın (1,+1,19)'daki L\* profilini yayımlar: 21 yüklem
   (B1 B2 F1 F2 R0c R1c R1e K1 K2 Qc QS Qh W1 QU Qa FC1 FD1 FH2p FSC WSum WUd), k_F = 12, en küçük hücre 3; 12·19 = 228 olduğundan kapı G
   orada tutar ve profil dört düzeye ve dört türe yayılır. Kur'an'ın plasebo12 dili {B2, K1, Qc} iki düzeyden (kitap, hücre) ve üç türden
   gelir. Okurken şunları çıkardım: Kur'an'ın üç kolda da (1,+1,19)'da bütünlüklü dili vardır; A−'de plasebo12 dili kabul edilebilir
   olduğundan D_{A−}(Kur'an) = {B2, K1, Qc} @ (1,+1,19) ve eşiği plasebo12'ninkine eşittir (kontrollerin puanlanması ise değişir).
   Bu çıkarımlar hiçbir tanımı etkilemedi: bütünlük kuralı, D_B, kollar, kütüphane ve bedeller görevde sabittir.
2. Dondurma öncesi doğrulama çıktılarından istenmeden şunlar okunabilir: mekanik sınamanın basılan dil sayısı (hedef olmayan 30 kitabın
   üç kolunda dilli kitap-kol sayısı M 12, U 19, L 17 / 90) hedef olmayan rastgele kitapların ne sıklıkta bütünlüklü dile sahip olduğunu
   kabaca gösterir; "taranan mercek ≥ 1.000: hayır" bayrağı (2.000 kitap) sağlanan yüklemi ≤ 16 olan kapı-G merceklerinin seyrekliğini,
   Kol A dili başına ölçülen ek süre kapı-G merceklerinin kontrollerde sık olduğunu, Kol B bellek ölçümü en geniş dillerin boyunu kabaca
   gösterir. Bunlar hedef ya da kontrol puanı değildir; hiçbir tanım, kural ya da eşik bunlardan sonra değiştirilmedi. Bütçe kuralları
   aşağıda en kötü duruma göre yazıldı.
3. plasebo12'nin hedef başına ayrıntı dosyaları (`ayrinti.txt`, `hedef_ayrinti.txt`, `analiz.json`'un hedef satırları) ve diller dosyaları
   dondurmadan önce açılmadı (diller yalnız `cmp` ile karşılaştırıldı; `analiz.json`'un yalnız anahtar yapısına bakıldı).

**Yapılmadı:** hiçbir hedef (Kur'an dahil) bu görevin D_B'siyle, Kol B yordamıyla ya da bütünlüklü puanla değerlendirilmedi; hiçbir hedefin
bu görevdeki dili ya da eşiği hesaplanmadı; hiçbir kontrol bu görevin dilleriyle puanlanmadı. Üretim kontrol tohumları (tür 2) yalnız Ö0
eşitlik sınamasında, plasebo12'nin dilleriyle, bölüt 1'de kullanıldı.

---

## 1. Okuma kuralı ve ana hattın önceden kaydettiği tahmin (aynen)

**Okuma kuralı** (görevden aynen; "not 25'teki kuralın bu teste uygulanışı; ön-kayda aynen yaz, değiştirme"):
> Kur'an'ın bütünlüklü ayrışması bütün plasebo kitaplarınkini aşarsa, nesnenin bütünlüğünün sırası dilin Kur'an'a göre yazılmasıyla açıklanamaz; kalibre p en küçük ~1/(K + 1) olabilir (K, plasebo kitap sayısı) ve aradaki fark ayrıca, model-tabanlı olarak verilir. Plasebo kitaplar Kur'an'ın bütünlüklü ayrışmasına sık ulaşırsa (R/K ≥ 0,05), bütünlüğün nadirliği büyük ölçüde dilin Kur'an'a göre yazılmasından gelir ve öyle yazılır. Arada kalan sonuçlar kalibre p'leriyle aynen yazılır.

**Ana hattın önceden kaydettiği tahmin** (görevden aynen; "ön-kayda aynen yaz, sonda değerlendir"):

Birincil kol A, birincil sıfır M20'de:
- Kur'an'ın bütünlüklü dilindeki bütünlük şartlı sırası p_Q için merkez **10⁻⁷**, aralık **10⁻⁹–10⁻⁶**.
- 200 plasebonun en az yarısında bütünlüklü bir dil bulunur.
- Plasebo p_x medyanı **10⁻³–10⁻¹**, en küçük p_x **10⁻⁶–10⁻⁴**.
- Kur'an'ın ayrışması 200 plasebonun **hepsini aşar** (kalibre p = 1/201).

Okuma kuralının işletilmesi §5.5'te, tahminin karar kuralları §10'dadır.

---

## 2. plasebo12'den aynen kalanlar

Kütüphane L\* (109 yüklem; tanımlar `src/kutuphane.h`, `src/kutuphane.py`), üretim ağacı ve yüklem bedelleri c(π) (`src/agac.py`,
`src/agac_uretim.h`), dil bedeli c(Λ) = log₂ 21 + Σ_{π∈Λ} c(π) − log₂(|Λ|!), 406 mercek λ = (α, β, p) (14 doğrusal biçim × 29 asal 3–113),
bölme çifti (t-paritesi, n-paritesi), dışlanan (sabit) yüklem–mercek çiftleri, mercek bedeli c(λ) (P0; (1,±1) için 7,317 bit), bilgi
puanı I_Λ(λ) (beş grup B, Z, F, W, H; grup içi tam birlikte "en-az" sıklığı, gruplar arası çarpım; bağlamlar k_F ve k_H kutularından),
bit işlevi ve çözülmemiş aramaların kuralı (§5.3), referans örneklemi (sıfır başına 10⁸ kitap, plasebo12'nin tabloları), kısıtsız D ve
grup başına 24 aday sınırı, hedefler, kontrol akışı ve derinlik kuralı. **Değişen yalnız bütünlük kısıtıdır** (§3–§4). Kodeks 129
(Σa = 6236); 127 ölçülmez. El dili ve P2 önseli bu görevde ölçülmez (görevde yok; P2 yalnız Ö0 eşitlik sınamasında kullanıldı).

---

## 3. Bütünlük kuralı B ve kollar

### 3.1 Bütünlük kuralı B (plasebo12 ön-kaydının §9 c kuralı)
Bir kitap y, mercek λ'da, dil Λ altında şu üç koşul tutarsa **bütünlüklüdür**, B(y, λ, Λ):
- (i) **kapı G:** k_F · p ≥ 228 (yani k_F ≥ 2E = 228/p) ve dört F hücresinin her biri ≥ 2 üyeli (yozlaşmamış aile; `kapiG`);
- (ii) Λ'nın λ'da y için **sağlanan** yüklemleri ≥ t_min motif türünden gelir;
- (iii) aynı yüklemler ≥ d_min düzeyden gelir.

Tür ve düzey etiketleri plasebo12 ön-kaydının §2.2–2.3'ündeki gibidir (`src/agac_uretim.h`: PTUR, PDUZEY): düzeyler kitap (B grubu), aile
(Z grubu; F'nin aile düzeyli yüklemleri: F1, F2, FC1, FC2, FD1, FD2, QS, Qh, FS\*, FH\*, FC\*, FHS\*, FA\*, FN\*, FRK, FCK, K2; H'nin HH1,
HH2, HK2), hücre (R0c, R0e, R1c, R1e, K1, Qc, FK\*, FKS\*, FAP, FKS, FSC; H'nin HR0c, HR0e, HR1c, HR1e), birleşim (W grubu); türler (a)
denge, (b) modülün geri dönüşü, (c) iki el, (d) ardışıklık. "Sağlanan" = kitabın profilinde o mercekte 1 olan ve dışlanmamış L\* yüklemi.

### 3.2 Kollar
| Kol | Rol | Bütünlük | Dil bedeli | Sıfırlar |
|---|---|---|---|---|
| **A** | birincil (Ö1) | B, t_min = 3, d_min = 3 | c(Λ) (plasebo12) | M20 (birincil), tekdüze, Mallows |
| **A−** | duyarlılık (Ö3) | B, t_min = 2, d_min = 2 | c(Λ) | yalnız M20 (bütçe kuralı §8.2) |
| **A+** | duyarlılık (Ö3) | B, t_min = 4, d_min = 4 | c(Λ) | yalnız M20 (bütçe kuralı §8.2) |
| **B** | ikincil (Ö2) | yok | 0 | önce M20; bütçe kalırsa tekdüze ve Mallows (§8.2) |

### 3.3 Bütünlüklü puan (Kol A, A−, A+)
Bir kitabın dil Λ altındaki puanı, yalnız B'nin tuttuğu mercekler üzerinden alınan en büyük değerdir:
S^B_y(Λ) = max { I_Λ(λ) − c(λ) : B(y, λ, Λ) }. Hiçbir mercekte B tutmuyorsa kitap o dilde **puansızdır**: hedef için p = 1, kontrol için
"aşmaz". I_Λ(λ) ve c(λ) plasebo12'dekiyle aynıdır (aynı tablolar, aynı işlev; hedef ve kontrol için).

---

## 4. D_B(x) — bütünlük kısıtlı dil kurma yordamı

### 4.1 Tanım
D_B(x) = argmax { S^B_x(Λ) − c(Λ) : Λ ⊆ L\*, |Λ| ≤ 20, en az bir mercekte B(x, λ, Λ) }. Eşdeğer olarak, (Λ, λ) çiftleri üzerinde
I_Λ(λ) − c(λ) − c(Λ)'nın B(x, λ, Λ) altındaki en büyüğü; seçilen çiftin merceği x'in o dildeki kendi en iyi (bütünlüklü) merceğidir.
- **Amaç negatif olsa bile seçilir**: bütünlük zorunludur, her hedefe kendi en iyi bütünlüklü dili verilir (boş dil B'yi sağlamaz).
- Böyle bir dil yoksa (hiçbir mercekte kapı G ile birlikte t_min tür ve d_min düzeye yayılan bir sağlanan yüklem kümesi yoksa) hedefin dili
  **boştur**: p_x = 1, aralık [1, 1]; bunun sıklığı her kolda ve sıfırda raporlanır.
- D_B'ye ne Kur'an'ın metni ne herhangi bir sonuç girer; girdileri hedefin kendi dizilişi, sıfırın referans tabloları, L\*, c(π), c(λ) ve
  (t_min, d_min)'dir. Her hedefe aynen uygulanır.

### 4.2 Neden alt kümeler yeterlidir
I_Λ(λ) yalnız Λ'nın λ'da sağlanan kısmına bağlıdır ve B de yalnız o kısma bağlıdır; sağlanmayan bir yüklemi eklemek bilgiyi ve B'yi
değiştirmez, c(Λ)'yı artırır. Bu yüzden en iyi dil, en iyi merceğinde hedefin sağladığı yüklemlerin bir alt kümesidir (plasebo12'nin
gerekçesi; tek istisna, |Λ| ≥ 16'da c(π) = 4 bitlik BMn'nin küme kodunda ≤ 0,32 bit "kazandırmasıdır" — bu bilgi değil kodun bir
yan etkisidir; plasebo12'deki gibi sağlanmayan yüklem hiçbir zaman eklenmez). Seçilen (Λ, λ)'nın değeri S^B_x(Λ) − c(Λ)'ya eşittir:
Λ başka bir bütünlüklü mercek λ′'de daha yüksek puan alsaydı, (Λ ∩ sağlanan(x, λ′), λ′) çifti daha büyük amaç verirdi.

### 4.3 Arama yöntemi (tam)
Kapı G'nin tuttuğu her mercek λ için (öteki merceklerde B tutamaz):
1. Her grup g (B, Z, F, W, H) için hedefin λ'da sağladığı L\* yüklemleri adaydır. Bir grupta aday sayısı 24'ü aşarsa adaylar, o bağlamdaki
   tekil bitleri en yüksek 24 yüklemdir (eşitlikte küçük indeks) — plasebo12'nin aynı kuralı; **tek yaklaşık adım** budur ve devreye
   girdiği (hedef, mercek, grup) üçlüleri raporlanır.
2. Adayların bütün alt kümeleri S taranır (en-az sayıları plasebo12'nin üst-küme toplamıyla); her (boy j ≤ 20, kapsam imzası σ) kovası için
   en iyi b_g(S) − Σ_{π∈S} c(π) tutulur. σ = 4 bit düzey ∪ 4 bit tür (S'nin yüklemlerinin etiketlerinin birleşimi).
3. Gruplar B, Z, F, W, H sırasıyla, (toplam boy J ≤ 20, birleşik kapsam σ) durumları üzerinde dinamik programlamayla tam olarak birleştirilir:
   durum değeri = grup değerlerinin toplamı. Amacın kalan terimi log₂(J!) − log₂ 21 − c(λ) yalnız J'ye, B ise yalnız σ'ya ve kapı G'ye
   bağlı olduğundan, kova ve durum başına en iyiyi tutmak kesindir (düzey ve tür kapsamı birleştirmede taşınır).
4. Son durumlardan σ'sı t_min türü ve d_min düzeyi karşılayanlar arasında amaç = değer + log₂(J!) − log₂ 21 − c(λ) en büyük olan seçilir.
5. **Eşitlik bozma** (plasebo12 ile aynı; amaçlar 10⁻⁹ bit içinde eşitse): küçük |Λ|; sonra küçük mercek indeksi (biçim × 29 + asal
   sırası); sonra sıralı global yüklem indeks listesinin sözlükte küçüğü. Kova ve durum içi eşitliklerde de sözlükte küçük liste tutulur;
   aynı durumdaki listeler aynı boydadır ve sonraki grupların indeksleri daha büyüktür, bu yüzden yerel seçim küresel kuralla tutarlıdır.
Çıktı: Λ_x, λ*_x, amaç, bilgi bileşenleri, dil bedeli, kapsam, kapı-G mercek sayısı, bütünlüklü dil kurulabilen mercek sayısı, aday sınırı
bayrağı (`src/dil_sec_b.h`, `src/dil_b.c`). Doğruluk dondurmadan önce kaba kuvvetle sınandı (§0 c).

### 4.4 Kol B — bedelsiz en geniş dil
c(Λ) = 0; bütünlük kuralı uygulanmaz. Her mercek λ'da hedefin sağladığı bütün L\* yüklemlerinin kümesi A_λ ve puanı I_{A_λ}(λ) − c(λ)
hesaplanır (grup içi tam birlikte en-az sayısı; plasebo12'nin bilgi puanı). **Dil, bu puanı en büyük yapan mercekte sağlanan L\*
yüklemlerinin hepsidir** (en geniş dil); mercek eşitliğinde (10⁻⁹ bit) küçük indeks. Boy sınırı yoktur (|Λ| 20'yi aşabilir; yoğun tablolar
için grup başına ≤ 20 sınırı bir korumadır, aşılırsa ek yazılır). Eşik S_x(Λ_x) = max_λ [I_{Λ_x}(λ) − c(λ)], plasebo12 puanıyla (bütün
mercekler) ve bu, seçilen mercekteki değere eşittir. Kontroller plasebo12 puanıyla (bütün mercekler) puanlanır.

### 4.5 Dondurma
Ön-kayıt dondurulduktan sonra ve **hiçbir kontrol geçişinden önce** bütün kolların ve bütün sıfırların dilleri kurulur
(`kos/diller.sh`): `tab/<sıfır>/diller_<kol>.txt` ve `ayrinti_<kol>.txt` salt okunur yapılır, SHA-256'ları `sonuc/diller_sha256.txt`'e
yazılır. Hedefin eşiği kontrol programında aynı tablolarla hesaplanır ve D_B'nin değeriyle karşılaştırılır (|fark| raporlanır).

---

## 5. Hedefler, kontroller, ayrışma

### 5.1 Hedefler
plasebo12'nin hedefleri, `tab/<sıfır>/hedef.bin` (SHA-256 doğrulandı): kayıt 0 = Kur'an (129). **M20 (birincil):** K = 200 plasebo;
**tekdüze** ve **Mallows:** K = 100. Hedefler süzülmez; çekilen her plasebo raporlanır. Aynı hedefler iki kalibrasyonun kitap kitap
karşılaştırılmasını sağlar.

### 5.2 Kontroller
Her sıfırda plasebo12'nin kontrol akışı: bölütler 2,5×10⁵ kitap, tohum(sıfır, 2, b), b = 1..400 (M20'de her bölüt yeni 64 zincirle
başlar). Kontrol kitapları plasebo12'ninkilerle aynıdır (aynı tohum ve kod; Ö0). Her kontrol, hedefin donmuş dilinde kendi en iyi
merceğini seçer; Kol A, A−, A+'da yalnız B'nin tuttuğu mercekler arasından (§3.3). Bütün hedefler ortak akışı paylaşır. Dil tabloları bir
süreçte 3,5 GB'ı aşarsa diller sırayla (Kur'an'ın dilleri önce) partilere bölünür ve aynı bölütler her parti için aynı tohumla yeniden
üretilir (aynı kitaplar).

### 5.3 Çözülmemiş aramalar ve doyma (plasebo12 ile aynı tek kural)
Bir grup araması için c = bağlamdaki referans kitaplarından maskesi istenen kümenin üst kümesi olanların sayısı, n = bağlam büyüklüğü:
c ≥ 10 ise b = log₂ n − log₂ c; 0 ≤ c < 10 ise b = log₂(n + 1) − log₂(c + 0,5); n = 0 ise 0. Bu değer **bütün** aramalarda — D_B'de,
Kol B'de, hedefin kendi puanında, her kontrolde, her sıfırda — aynen kullanılır; aralık taşınmaz, adanmış kestirim yoktur. Hedefin en iyi
merceğindeki çözülmemiş aramalar (c < 10) bayrak olarak raporlanır.

### 5.4 Ayrışma istatistiği ve derinlik kuralı
k_x = #{kontrol c : S_c(Λ_x) ≥ S_x(Λ_x) − 10⁻⁶ bit} (doğrudan sayım; eşitlik hedefin aleyhine; Kol A kollarında puansız kontrol
"aşmaz"); **p_x = (k_x + 1)/(N_x + 1)**, N_x hedefin son derinliği; iki yanlı %95 Clopper–Pearson [lo_x, hi_x] (k = 0'da lo = 0).
Dilsiz hedef: p_x = 1, [lo_x, hi_x] = [1, 1]. Kuyruk dış değerlemesi yapılmaz.
**Derinlik kuralı (plasebo12):** düzey 1: N₁ = 10⁶ (bölüt 1–4), bütün dilli plasebolar; k < 10 kalanlar düzey 2: N₂ = 10⁷ (bölüt 1–40);
orada da k < 10 kalanlar düzey 3: N₃ = 10⁸ (bölüt 1–400). Kur'an'ın dilleri (her kolda) doğrudan 10⁸. En çok 10⁸. Her kol ve her plasebo
dili ayrı bir hedeftir. Betimsel olarak Kol A kollarında kB_x = Λ_x altında en az bir mercekte bütünlüklü kontrol sayısı da yazılır.
M20'de Kur'an'ın isabetlerinin zincir kümelenmesi (bölüt içi sıra mod 64) raporlanır.

### 5.5 Kalibre p ve okuma kuralının işletilmesi
R = #{plasebo x : p_x ≤ p_Q ya da lo_x ≤ hi_Q} (eşitlikler ve aralık çakışmaları Kur'an'ın aleyhine); **p_kal = (1 + R)/(K + 1)**.
Her kol ve her sıfır ayrı; birincil = Kol A, M20, K = 200.
- R = 0 → "Kur'an'ın bütünlüklü ayrışması bütün plasebo kitaplarınkini aşar"; okuma kuralının ilk cümlesi uygulanır; p_kal = 1/(K + 1);
  fark ayrıca model-tabanlı verilir (§5.6).
- R/K ≥ 0,05 (K = 200'de R ≥ 10; K = 100'de R ≥ 5) → "plasebo kitaplar Kur'an'ın bütünlüklü ayrışmasına sık ulaşır"; ikinci cümle uygulanır.
- Aradaki her sonuç (K = 200'de 1 ≤ R ≤ 9; K = 100'de 1 ≤ R ≤ 4) p_kal ile aynen yazılır.
Okuma kuralı Kol A için (her sıfırda) işletilir. A−, A+ ve Kol B'de aynı R, p_kal ve aynı üç sınıf betimsel olarak, kol adıyla yazılır.
Kur'an bir kolda dilsizse (p_Q = 1) o kolda R = K ve p_kal = 1 olur ve öyle yazılır.

### 5.6 Model-tabanlı fark (önceden yazılmış; kararlarda kullanılmaz)
y_x = −log₁₀ p_x (bütün K plasebo; dilsiz → y = 0), y_Q = −log₁₀ p_Q; 10⁸'de k = 0 kalan plasebolar sağdan sansürlüdür
(y = log₁₀(10⁸ + 1)).
- **(a) plasebo12'nin modeli, aynen** (`src/istat.py`, plasebo12'nin `analiz.py`'sinden değiştirilmeden kopyalandı): birincil, en büyük
  %20'ye (K = 200'de 40, K = 100'de 20; eşik u = bu kümenin en küçüğünün hemen altındaki sıra istatistiği) genelleştirilmiş Pareto, en çok
  olabilirlik, P = 0,2·[1 + ξ(y_Q − u)/σ]^{−1/ξ} (ξ < 0 ve y_Q uç noktanın ötesindeyse 0; y_Q ≤ u ise tanımsız), parametrik önyükleme
  (2.000) %95 aralığı; ikincil, bütün y'lere sansürlü normal, P = 1 − Φ((y_Q − μ)/σ); betimsel fark Δ = y_Q − max_x y_x.
  **Dilsizlik kuralı:** üst %20 kümesinde p_x = 1 olan bir plasebo varsa GPD tanımsızdır ve öyle yazılır (normal ve Δ yine verilir).
- **(b) Koşullu karışım** (dilsiz plasebolar bir nokta kütlesi olduğundan): K_d = dili olan plasebo sayısı; P = (K_d/K) · P_koşullu.
  P_koşullu, (a)'nın aynı kodu yalnız K_d dilli plaseboya uygulanarak bulunur: GPD, round(0,2·K_d) ≥ 5 ve üst kümede p = 1 yoksa; normal,
  K_d ≥ 10 ise; K_d < 10 ise uyum yapılmaz.
Hepsi "model-tabanlı" etiketiyle verilir.

---

## 6. Kol B'nin ayrıntıları
Kol B'nin dili §4.4'tedir. Kontrol puanı plasebo12'ninkiyle aynıdır; değerlendirme, boy sınırıyla kesin budamayla hızlandırılır: her
(mercek, bağlam, grup) için T_g'nin s boylu alt kümeler üzerindeki en büyüğü UB_g[s]; bir mercekte Σ_g UB_g[|S_g|] − c(λ) eşiğin (ya da
aday tabanının) 10⁻⁹ bit payla altındaysa o mercek atlanır (değer bu sınırı aşamaz; sonuç değişmez; kontroller 1.024'lük partilerde
değerlendirilir). Sınıf sayısı kc (aşan kontrollerden plasebo12 sınıf kuralına uyanlar) betimseldir. Derinlik kuralı, kalibre p ve model
§5'teki gibidir.

---

## 7. Ö0 ve dondurma sonrası denetimler

**7.1 Ö0** (görev: kısıtsız D plasebo12'nin üç `diller.txt`'sini bit-bit vermeli; Kur'an'ın plasebo12 eşikleri aynı çıkmalı; bir kontrol
bölütünün sayımları aynı çıkmalı): §0 (b)'de yapıldı ve tuttu (`kos/o0_d.sh`, `kos/o0_k.sh`). Oturum sonunda bu iki betik, `kos/derle.sh`
ile yeniden derlenmiş ikililerle yinelenir.
**7.2 Tutarlılık denetimleri** (dondurmadan sonra, raporlanır, sonucu değiştirmez): (i) D_B'nin/Kol B'nin eşiği (double) ile kontrol
programının eşiği (float tablolar) arasındaki en büyük fark ve mercek eşitliği; (ii) aday sınırının (24) devreye girdiği hedefler;
(iii) hedeflerin en iyi merceğinde çözülmemiş aramalar; (iv) kontrol aday tamponu taşması; (v) dondurma anındaki kod dosyalarının özetleri.
**7.3 Doğrulama kodu** (§0 c) pakette ve `sonuc/dogrulama/`'dadır; `kos/dogrulama.sh` ve `kos/hiz.sh` ile yinelenir.

---

## 8. Sıra, bütçe, hesap ve kayıt

### 8.1 Sıra (`kos/akis_b.sh`)
1. Bütün diller (M20: A, A−, A+, B; tekdüze ve Mallows: A, B) kurulur ve dondurulur; hedef ayrıntıları yazılır (§4.5).
2. Kol A kümesi (M20'de A, A−, A+ birlikte; tekdüze ve Mallows'ta A), sıfır sırası M20 → tekdüze → Mallows; her sıfırda düzey 1, 2, 3 ve analiz.
3. Kol B M20 (düzey 1, 2, 3, bütçe kuralıyla) ve analiz.
4. Kol B tekdüze ve Mallows: yalnız Kol B M20 tamamlandıysa, bütçe kuralıyla.

### 8.2 Bütçe kuralları
Toplam hesap tavanı **40 çekirdek-saat**; aynı anda en çok **4 iş parçacığı ya da süreç** (kontrol programı 4 iş parçacığıyla koşarken
başka hesap başlatılmaz; analiz ve betimsel betikler geçişler arasında koşar; `ls`, `tail`, `cat` gibi izleme komutları hesap sayılmaz);
bellek ≤ 6 GB (dil tabloları süreç başına ≤ 3,5 GB); disk ≤ 8 GB. Kayıtlı hesap = kontrol bölütlerinin süre alanları + `sonuc/hesap_ek.txt`
kalemleri (`src/hesap_saati_b.py`; dondurma öncesi ≈ 0,9 çekirdek-saat).
- **Kol A kümesi koşulsuz koşar.** En kötü durum kestirimi (bütün plasebo dilleri dilli ve 10⁸'e iniyor; taban 76 µs, dil başına 0,9 µs):
  M20 (A, A−, A+: 603 dil) ≤ 17,2, tekdüze ve Mallows (101 dil) ≤ 4,6'şar → ≤ 26,5 çekirdek-saat; dondurma öncesiyle ≤ 27,4 < 40.
  Ö3'ün (A−, A+) bu en kötü durum içinde kalması, "bütçe kalırsa" koşulunun karşılandığı anlamına gelir; Ö3 bu yüzden M20 Kol A geçişine
  katılır (ortak kontroller).
- **Kol B (ikincil):** her düzey, kayıtlı hesap + düzeyin kestirimi ≤ 40 çekirdek-saat ise koşar; kestirim = N_düzey × (76 + 5,4 · n_B) µs
  (n_B = düzeye giren Kol B dili). Koşmayan bir düzeyden sonraki düzeyler de koşmaz ve kol "eksik" yazılır: tamamlanmamış plasebolar ve
  p_kal "ölçülmedi" olur (eksik düzeylerden okunabilecek sınırlar sıklık gibi okunmaz). Kol B tekdüze ve Mallows yalnız Kol B M20 tamamlandıysa koşar.
- Bütçe aşılacaksa yeni koşu başlatılmaz; eksik kalan yazılır (ek).

### 8.3 Sürdürülebilirlik ve kayıt
Kontrol bölüt çıktıları atomiktir; var olan bölüt atlanır (`kos/kontrol_b.py` yeniden çağrılabilir). Günlük `sonuc/kosu_gunlugu.txt`,
ara notlar `sonuc/ara_notlar.md`. Derleme `kos/derle.sh` (Apple clang, arm64; -O3 -std=gnu11 -w). Sonda `paket/plasebo14_paket.tar.gz`
(≤ 200 MB; referans tabloları hariç) ve SHA-256'sı `paket/plasebo14_paket.sha256`.

### 8.4 Tohumlar
tohum = 29102600000 + sıfır·10⁸ + tür·10⁷ + sıra (plasebo12'nin işlevi, değiştirilmedi); sıfır U = 1, M = 2, L = 3. Kullanılanlar:
tür 1 = referans (plasebo12'nin tabloları; yeniden üretilmez), tür 2 = kontrol bölütleri b = 1..400 (plasebo12 ile aynı akış), tür 3 =
hedefler (plasebo12'nin `hedef.bin`'i), tür 4 = kod doğrulama (sıra 100–2099; hedef olmayan kitaplar), tür 6 = hız ölçümü (bölüt 1–4),
tür 8 = mekanik sınama (bölüt 101–104). Model önyüklemesi tohumu 29102600 (plasebo12'nin `analiz.py` sabiti).

---

## 9. Betimseller (Ö4)
- Her kolda Kur'an'ın dili (yüklemler, mercek, türler, düzeyler, eşik, aramaların sayımları) ve plasebo12 diliyle ({B2, K1, Qc} @ (1,+1,19))
  farkı (ortak, eklenen, çıkan yüklemler); Kur'an'ın eşiğine 2 bitten yakın kontroller (aday dosyaları): en yüksekleri, mercekleri,
  dilin orada sağlanan yüklemleri, "hepsini taşıyan" kontroller, zincir kümelenmesi.
- Plasebo dillerinin boy, tür sayısı, düzey sayısı, tür ve düzey kullanımı, en sık yüklemler; dilsiz plasebo sayısı (K − K_d).
- En iyi beş plasebonun yapısı: p_x, k/N, aralık, mercek, k_F, k_H, |F ∩ H|, hücre boyları ve q-toplamları, dil ve sağlanan yüklemler,
  L\* profili (`src/hedef_ayrinti_b`).
- Kol A'da plasebo12'ye göre kitap kitap değişim: her plasebo için p^12_x (plasebo12'nin `analiz.json`'u) ve p^A_x; dilli plasebolarda
  log₁₀(p^A_x / p^12_x)'in medyanı ve çeyrekleri, azalan/artan sayısı, Spearman sıra korelasyonu, dili plasebo12'dekiyle aynı kalanlar; dilsiz
  plasebolarda p^12 dağılımı; Kur'an'ın iki sırası.
- Hesap, bellek, partiler, aday sınırı, çözülmemiş aramalar, eşik tutarlılığı (§7.2).

---

## 10. Tahminin değerlendirilmesi (karar kuralları; Kol A, M20)
- **p_Q:** k_Q ≥ 1 ise p̂ = (k_Q + 1)/(N + 1) ve %95 CP aralığı; "tuttu" = p̂ ∈ [10⁻⁹, 10⁻⁶]; "tutmadı" = %95 aralığı tümüyle [10⁻⁹, 10⁻⁶]
  dışında; öteki durumlar "karar verilemedi". k_Q = 0 ise yalnız tek yanlı %95 üst sınır U = 1 − 0,05^{1/N} vardır: U < 10⁻⁹ ise "tutmadı
  (daha nadir)", aksi halde "karar verilemedi". Kur'an dilsizse p_Q = 1 → "tutmadı". |log₁₀ p̂ + 7| ayrıca yazılır.
- **"200 plasebonun en az yarısında bütünlüklü bir dil bulunur":** K_d ≥ 100 → "tuttu"; K_d ≤ 99 → "tutmadı" (kesin sayım).
- **Plasebo p_x medyanı:** nokta = 200 p_x'in (dilsizler p = 1 ile) medyanı (100. ve 101. sıra istatistiklerinin geometrik ortalaması); %95
  aralık = [p_(86), p_(115)]; "tuttu" = nokta ∈ [10⁻³, 10⁻¹]; "tutmadı" = aralık tümüyle dışında; öteki "karar verilemedi".
- **En küçük p_x:** nokta = min_x p_x; aralık = [min_x lo_x, min_x hi_x]; aynı üç hüküm, [10⁻⁶, 10⁻⁴] ile.
- **"Hepsini aşar":** R = 0 → "tuttu"; R ≥ 1 → "tutmadı".
- **Bütün tahmin:** beş parça da tuttuysa "tuttu"; hiçbiri tutmayıp en az biri tutmadıysa "tutmadı"; öteki durumlar "kısmen tuttu" (ayrıntısıyla).

---

## 11. Rapor kuralları
`PLASEBO14_RAPORU.md` (Türkçe; sonunda İngilizce özet). Her sayının yanında sıfır, örneklem, isabet ve belirsizlik; senaryolar bir aralığın
uçları gibi sunulmaz; üst sınırlar sıklık gibi okunmaz. plasebo12'nin sonucu ile bu testin sonucu yan yana yazılır; ikincisi birincinin yerine
geçmez. Cevaplanacak sorular görevdeki altı sorudur. **Ölçülmeyenler** "ölçülmedi" diye yazılır, en azından: kütüphanenin ve bütünlük
kuralının sınırları (kural Kur'an'ın biçimi bilinerek yazıldı; L\* el dilinden genelleştirildi; 406 mercek; bölme seçimi; başka tür/düzey
etiketlemeleri ve eşikleri); literatürün araması; nesnenin (n, a) seçimi; sayım okulu; gruplar arası bağımsızlık yaklaşımı; kültürel havuz;
referans tablosu gürültüsünün p_x'e etkisi; dil bedelinin biçimi. Rapor, ana hattın onu depoya ekleyeceği ve bağımsız olarak doğrulayacağı
bilinerek yazılır: bütün dil dosyaları, eşikler, bölüt sayımları ve yeniden üretim komutları pakettedir.

## 12. Sapma politikası
Her sapma (bütçe, kod düzeltmesi, tanım netleştirmesi) `ek/EK_<UTC zaman>_<konu>.md` dosyasına gerekçesi ve sonuca etkisiyle yazılır.
Herhangi bir hedefin diline ya da kontrol puanına bakıldıktan sonra tanım, ağırlık, eşik, bedel ya da kural değiştirilmez; değiştirilmesi
zorunlu bir hata bulunursa düzeltme ve düzeltme öncesi sonuç birlikte raporlanır.
