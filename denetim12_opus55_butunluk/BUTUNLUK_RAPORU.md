# BÜTÜNLÜK RAPORU — aynı unsurların katmanlar arasında yeniden belirmesi ne kadar nadir?

**Tarih:** 26–27 Ekim 2026 · **Yazan:** Claude (Opus 5.5), `GOREV.md` gereği, `~/opus5.5/butunluk` klasöründe; `~/Desktop/hrf2` yalnız
okundu, hiçbir dosyası değiştirilmedi.
**Ön-kayıt:** `ONKAYIT.md`, hiçbir rastgele kitabın bütünlük puanına bakılmadan yazıldı ve **2026-10-26T17:13:24Z**'de donduruldu
(SHA-256 `d213dc89332884f3973b4b2e5b33afaa2070748b95dda0d344f553eeefcfd513`; `sonuc/onkayit_ozet.txt`; dondurma anındaki kod özetleri
`sonuc/onkayit_kod_sha256.txt`). Dondurmadan önce yalnız Kur'an'ın profili ve sayım puanı ile kod/örnekleyici doğrulamaları yapıldı.
Sapmalar ve sonradan analizler zaman damgalı eklerde (`ek/`, §11).
**Birincil sayım:** Tevbe = 129 (Σa = 6236); 127 ikincil. **Birincil puan:** bilgi-ağırlıklı puan (değişke V0, önsel P0); sayım puanı
(kapı G) her yerde yanında.

> Bu rapordaki her sayının yanında sıfır modeli, örneklem, isabet sayısı ve belirsizlik vardır. Farklı sıfırların ya da senaryoların
> sayıları bir aralığın uçları değildir; üst sınırlar sıklık değildir. Sıra p = (k + 1)/(N + 1); köşeli parantezler %95 aralığıdır
> (k/N için Clopper–Pearson) ya da — açıkça yazıldığında — çözülmemiş aramalardan doğan [k_alt, k_üst] belirsizlik aralığıdır.

## 1. Kısa cevaplar

**(1) Kur'an'ın bütünlük puanı ve sırası.** L dilinde Kur'an'ın en iyi merceği her sıfırda, her puanda ve her duyarlılık değişkesinde
**(α, β, p) = (1, +1, 19)**'dur (t = n + a, modül 19). 20 yüklemin 15'i sağlanır; kanıtlı gerektirmeler ayıklanınca **sayım puanı 11**.
**Bilgi puanı** (birincil) tekdüzede 31,06, M20'de 27,41, Mallows'ta 27,27 bit (J10: 25,98). Rastgele kitaplar arasında sıra
p = (k + 1)/(N + 1):

| Sıfır | bilgi puanı (birincil): k_alt / k_nokta / k_üst → p_nokta [belirsizlik aralığı] | sayım puanı (kapı G): k / N → p |
|---|---|---|
| **M20** (birincil sıralı; N = 10⁸) | 16 / 22 / 18165 → **2,3×10⁻⁷** [1,7×10⁻⁷; 1,8×10⁻⁴] | 59 / 2,0×10⁸ → **3,0×10⁻⁷** [2,2×10⁻⁷; 3,8×10⁻⁷] |
| **Mallows** (N = 10⁸) | 37 / 46 / 16150 → **4,7×10⁻⁷** [3,8×10⁻⁷; 1,6×10⁻⁴] | 51 / 2,0×10⁸ → **2,6×10⁻⁷** [1,9×10⁻⁷; 3,4×10⁻⁷] |
| **Tekdüze** (N = 10⁸) | 2 / 3 / 16592 → **4,0×10⁻⁸** [3,0×10⁻⁸; 1,7×10⁻⁴] | 89 / 2,0×10⁸ → **4,5×10⁻⁷** [3,6×10⁻⁷; 5,5×10⁻⁷] |
| J10 (ikincil; N = 10⁷) | 1 / 4 / 7473 → 5,0×10⁻⁷ [2,0×10⁻⁷; 7,5×10⁻⁴] | 11 / 3,0×10⁷ → 4,0×10⁻⁷ [1,8×10⁻⁷; 6,6×10⁻⁷] |

Uzunluk-sıralı sıfırlarda Kur'an'ın bütünlüğü **~3×10⁻⁷ mertebesindedir** (kabaca 2–4 milyon rastgele kitapta bir); tanımları çok
farklı iki puan aynı mertebeyi verir. Tekdüzede bilgi puanında ~4×10⁻⁸. Sayım puanının tekdüzedeki tepesi yozlaşmış ailelerle
dolduğu için (§6.2) sonradan sıfır-toplam dışlamalı S0 ayrıca verildi: tekdüze 3×10⁻⁸, M20 9×10⁻⁸, Mallows 6×10⁻⁸. Bilgi puanının
belirsizlik aralığının üst ucu (~1,6–1,8×10⁻⁴), referans diliminde hiç görülmemiş maskeler taşıyan 16–18 bin kitabın üst sınırının ∞
olmasından gelir; bu bir sıklık kestirimi değildir — bu kitapların neredeyse hepsi eşiğin çok altındadır (§5). 127 sayımında sıralar
aynı mertebededir (§7.1). Bölme menüsünde (§7.4; yalnız sayım puanı)
her rastgele kitaba 20 bölme çiftinin en iyisini seçme hakkı verilince Kur'an'ın sırası yalnız ~2 kat yükselir (5×10⁻⁷ – 1×10⁻⁶).
Buna karşılık t-paritesi bölmesi herkes için menüden çıkarılınca Kur'an'ın sırası ~2×10⁻⁴'e çıkar. Yani üstünlük Kur'an'ın kendi bölme
türüne bağlıdır, ama bölme seçiminin bir yan ürünü değildir.

**(2) En güçlü rakipler aynı sınıftan mı?** **Bilgi puanında hayır:** her sıfırda ilk 20'nin yalnız 0–2'si ön-kayıtlı Kur'an
sınıfındadır; tepeyi tek nadir olaylar tutar (aile ya da karşı aile boyunun p'ye eşit olması, kitap düzeyinde çapraz denge) —
dağınık denk gelmeler. **Sayım puanında** ilk 20'nin hepsi kural gereği sınıftadır, ama: **tekdüzede hepsi yozlaşmıştır** (büyük p'de 8
üyeli, toplamları sıfır aileler; ön-kayıtlı sınıf kuralı katsayı büyüklüğü yozlaşmasını yakalamıyor); **sıralı sıfırlarda** yozlaşma
azdır ve rakiplerin bir kısmı gerçekten Kur'an türündendir: **Kur'an'ın kendi merceği (1,+1,19)'da 12 üyeli 3/3/3/3 aileler**,
ardışık hücre toplamları, kesintisiz katsayı bandı, çoğunda k_H = 7, \|U\| = 19 ve 57/57 — ama **hiçbirinde "merkez 19"
(Σq = 4·19) ve terazi (B2) yoktur**. S0 ≥ 11 olan kitapların M20'de 17'sinin 9'u, Mallows'ta 11'inin 5'i bu türdendir.

**(3) Eski literatür verildiğinde kalan katmanlar ne kadar ayırt edici?** O (12 üyeli aile, 1444, 6/6, 722/722, 361/361) verildiğinde
kalan katmanlar (terazi dahil) bilgi puanında 5,0×10⁻⁶ (tekdüze; k = 0, tek yanlı %95 üst sınır 1,5×10⁻⁵) ile 4,8×10⁻⁵ (Mallows)
arasındadır; bunun çoğu 1994 terazisinden gelir. **O' = O ∧ terazi** verildiğinde — yalnız bu çalışmanın eklediği katmanlar: çift-t
hücreleri 18/20, 5–9 bandı, 3/3/3/3, iki el — sıra bilgi puanında **1,0×10⁻³ (tekdüze) – 3,4×10⁻³ (sıralı)**, sayım puanında
4,1×10⁻⁴ – 1,6×10⁻³; mercek serbest bırakılınca (duyarlılık, yalnız sayım) ~3×10⁻². Yani bu çalışmanın eklediği katmanlar kendi
başlarına binde bir mertebesinde ayırt edicidir (§7.2).

**(4) Ana hattın tahmini tuttu mu? Kısmen.** Uzunluk-sıralı sıfırlar **tuttu** (M20 ve Mallows, iki puanda da aralıkta; ama merkez
10⁻⁸'in 1,4–1,7 mertebe üstünde, aralığın "az nadir" ucunda). Tekdüze **tutmadı** (tahminden 2,6–3,7 mertebe daha az nadir). Eski
literatür: görevin birincil koşulu O ile çoğunlukla **tutmadı (daha nadir)**; tahminin ifadesine karşılık gelen O' ile **tuttu** (§8).

**(5) Ölçülmeyenler** (§9): dilin Kur'an görüldükten sonra yazılmasının etkisi — **ölçülmedi**; mercek ailesi ve modül aralığı seçimi —
**ölçülmedi**; gruplar arası bağımsızlık yaklaşımının Kur'an dışındaki etkisi — **ölçülmedi**; literatür araması, sayım okulu,
kültürel havuz — **ölçülmedi**; bilgi puanıyla menü analizi — **ölçülmedi**; bölmeli örnekleme / nadir-olay kuyruğu — **ölçülmedi**
(gerekmedi); belirsiz kitapların tamamının adanmış çözümü — **ölçülmedi** (bütçe; aralık olarak verildi).

## 2. Ne ölçüldü, nasıl (özet; ayrıntı `ONKAYIT.md`)

### 2.1 Bütünlük dili L
Bir kitap = 114 konuma atanmış ayet sayıları a_n. **Mercek** λ = (α, β, p): t = αn + βa, u = αn − βa; 14 doğrusal biçim (α, |β| ∈ {1,2,3},
aralarında asal, β'nin iki işareti; dış çalışmanın 14 biçimi) × 29 asal (3…113) = 406 mercek. **Bölme:** birincil (D1, D2) = (t-paritesi,
n-paritesi). **Düzeyler:** kitap (E⁰/E¹), aile F = {p | t} (yarılar F⁰/F¹), hücreler (yarıların n-paritesiyle bölünmesi), katsayılar q = t/p,
karşı aile H = {p | u} ve birleşim U = F ∪ H. Dilde Kur'an'a özgü sabit yoktur.

20 yüklem, dört motif türünde:

| Tür | Yüklemler |
|---|---|
| (a) bölme dengesi | B1 kitap 57/57; B2 kitap toplamı (eşit toplam ya da n-/a-bileşenine çapraz denge: α·Σ_{E⁰}n = \|β\|·Σ_{E¹}a); F1/F2 aile yarıları eşit sayı/eşit Σq; R0c/R0e, R1c/R1e her satırda hücrelerin eşit sayısı/eşit toplamı |
| (b) modülün geri dönüşü | Qc hücre merkezi = p (Σq = 4p); QS p \| Σq; Qh p \| iki yarı toplamı; QkF k_F = p; QU \|U\| = p; QkH k_H = p; Qa p \| Σa(U); Qn p \| Σn(U); QH p \| Σ(u/p)(H) |
| (c) iki el | W1 F ∩ H = ∅ |
| (d) ardışıklık | K1 dört hücre toplamının farklı değerleri kesintisiz; K2 farklı katsayılar kesintisiz bant |

Permütasyonla değişmeyen (sabit) yüklemler analitik olarak dışlanır. **Kanıtlı gerektirmeler** (sayım puanında ayıklanır): Qc→QS,
Qh→QS, F2∧QS→Qh, QkF∧QkH∧QU→Qn,Qa, **K1∧Qc→F2,R1e** (t-çift satırda q çift olduğundan ardışık hücre toplamları ve toplam 4p yalnız
(p−1, p+1 / p, p) desenine izin verir), p ≥ 5'te K1∧Qh→F2, pozitif biçimlerde Qc∧Qh→F2 ve p = 3'e özgü sonlu-uzay kuralları; kanıtlanamamış
aday kalmadı (yöntem: 3,9×10⁶ kitapta birlikte-görülme taraması, soyut yapılandırma uzaylarının kapsamlı sayımı, kesin bölüntü sayımı,
karşı-örnek araması; çürütülen adaylar ön-kayıtta listelidir).

### 2.2 İki puan
- **(i) Sayım puanı:** bir mercekte kapıdan geçen sağlanmış yüklemlerden kanıtlı gerektirmelerle türetilemeyenlerin en küçük sayısı
  (tekrarsız kredi); kitabın puanı en iyi merceğindeki değer. **Yozlaşma kapısı G** (görevin önerisi): aile ve hücre yüklemleri yalnız
  k_F ≥ 2E (E = 114/p) ve her hücre ≥ 2 üyeliyse sayılır.
- **(ii) Bilgi-ağırlıklı puan (birincil):** I(λ) = I_B + I_boy + I_F + I_W − c(λ). I_F ve I_W, aile-içi (11 yüklem) ve iki-el (6 yüklem)
  maskelerinin **aile boyuna koşullu birlikte** sıklığının −log₂'si; I_B kitap maskesinin (biçim düzeyi) birlikte sıklığı; I_boy yalnız
  k_F = p ise −log₂ P(k_F = p). Gruplar arası çarpım bir yaklaşımdır; grup içindeki bütün cebirsel bağlar tam birlikte sıklıkla hesaplanır.
  Küçük aileler kapıyla değil aile boyuna koşullamayla bedellendirilir (2E kapısı Kur'an'ın aile boyuna tam denk düştüğü için birincil
  bilgi puanında kullanılmadı; duyarlılık değişkesi V1). c(λ) = sadelik önseli (biçim ∝ 2^{−(α+\|β\|−2)}; (1,±1) 2,46 bit … (2,±3) 5,46 bit)
  + asal eşit ağırlık (log₂ 29 = 4,86 bit). Sıklıklar her sıfırın 10⁸ kitaplık bağımsız **referans örnekleminden** (sınama kitaplarından
  ayrı tohum), aile boyu bağlamında; referans diliminde ≥ 10 isabeti olmayan arama "çözülmemiş" sayılır ve puan bir taban değere
  kırpılmaz, [alt, üst] sınırla taşınır (tek yanlı %95 Poisson–CP); S_alt'ı en yüksek belirsiz kitaplar adanmış kestirimle yeniden
  puanlanır (§5).

### 2.3 Sıfırlar, örneklemler, sıra
Tekdüze (U), M20 (birincil sıralı: robotun 64 zincirli S ≤ S_gözlenen zinciri), Mallows (q yalnız terslik sayısına ayarlı), J10 (ikincil,
veriye merkezli). Örnekleyiciler 10. denetiminkilerle bit-bit aynıdır. Sıfır başına referans 10⁸ + sınama 10⁸ kitap (J10: 2×10⁷ + 10⁷;
127 sayımı: 3×10⁷ + 10⁷). Sıra p = (k + 1)/(N + 1), k = #{puan ≥ Kur'an'ınki}; sayım puanında N = referans + sınama, bilgi puanında
N = sınama.

## 3. Kur'an'ın L altındaki profili, puanları ve merceği

**En iyi mercek (her iki puanda, her sıfırda, sekiz bilgi değişkesinin hepsinde): (α, β, p) = (1, +1, 19)** — yani t = n + a, modül 19.
İkinci en iyi mercek (1, −1, 19) (aynı aile çiftinin rolleri yer değiştirmiş): sayım 6.

| Nicelik | Değer |
|---|---|
| Aile F = {19 \| n + a} | 12 üye (= 2E; E = 114/19 = 6) |
| Karşı aile H = {19 \| n − a} | 7 üye; F ∩ H = ∅; \|U\| = 19 |
| Hücreler (t-paritesi × n-paritesi) | sayılar 3/3/3/3; q-toplamları (t çift: n çift 20, n tek 18), (t tek: 19, 19) |
| Katsayılar q = t/19 | 5,5,5,6,6,6,6,6,7,7,8,9 → farklı değerler 5–9 kesintisiz; Σq = 76 = 4·19; yarılar 38/38 |
| Kitap | 57/57; Σ_{t çift} t = 6236 = Σa (çapraz denge Σ_{E⁰}n = Σ_{E¹}a) |
| Birleşim | Σa(U) = 1159 = 19·61; Σn(U) = 1028 (19'a bölünmez); Σ(u/19)(H) = 7 |

**Sağlanan 15 yüklem** (20'den): B1, B2, F1, F2, R0c, R1c, R1e, K1, K2, Qc, QS, Qh, W1, QU, Qa. Sağlanmayan: R0e (20 ≠ 18), QkF (12 ≠ 19),
QkH (7 ≠ 19), Qn, QH. Sonuç **Kur'an sınıfında**dır (kapı G; dört motif türü; dört düzey).

**Sayım puanı = 11** (bütün kapılarda). Ayıklanan dört yüklem: F2 ve R1e, **K1 ∧ Qc**'den zorunlu çıkar (ardışık hücre toplamları
ve merkez 19 ⇒ satırlar 18/20 ve 19/19 ⇒ yarılar eşit ve tek-t satırı eşit); QS Qc'den; Qh F2 ∧ QS'den. Ana hattın betimsel
tablosundaki 13/13 ile fark: orada "sabit hücre" de bir özellikti ve "dört hücre eşit" ile "b,b / b−1,b+1 deseni" tek özellikti; burada eşit
hücreler üç ayrı denge (F1, R0c, R1c) olarak, desen ise K1 ∧ Qc'nin zorunlu sonucu olarak yer alır. **"Hücre merkezi 19" ile "ardışık hücre
toplamları" birlikte, iç terazinin (722/722) ve 361/361 satırının cebirsel olarak zorunlu sonucudur** — bu ayrı bir yeniden-belirme
sayılmaz.

**Bilgi puanı (birincil değişke V0, önsel P0)**, bileşenler bit olarak (I_B kitap + I_F aile-içi | k_F = 12 + I_W iki el | k_F = 12 − bedel 7,317):

| Sıfır | I_B | I_F | I_W | **Puan S_Q** | not |
|---|---|---|---|---|---|
| Tekdüze | 11,63 | 18,73 | 8,02 | **31,06** | I_F referansta çözülmedi (k = 12 diliminde 1.014.579 kitapta 2 isabet); adanmış kesin koşullu kestirim 2,31×10⁻⁶ (46 isabet / 2×10⁷ öneri) — tablonun 11 tekil sıklığıyla çapalandı (\|z\| ≤ 1,6) |
| M20 | 11,06 | 15,61 | 8,05 | **27,41** | bütün aramalar referansta çözülmüş (k = 12 diliminde ~1,2×10⁶ kitap) |
| Mallows | 11,11 | 15,46 | 8,01 | **27,27** | bütün aramalar referansta çözülmüş (k = 12 diliminde 1,31×10⁶ kitap) |
| J10 (ikincil) | 11,02 | 14,20 | 8,08 | **25,98** | bütün aramalar çözülmüş (referans 2×10⁷; k = 12 diliminde 244.019 kitap) |

Okuma: Kur'an'ın bilgi puanı üç bileşenin toplamıdır — kitap düzeyi (57/57 ve terazi birlikte, ~11–12 bit), aile-içi yapı
(12 üyeli ailede eşit yarılar, 3/3/3/3, ardışık hücre toplamları, merkez 19, katsayı bandı birlikte; ~15–19 bit) ve iki el (ayrık
aileler, birleşim 19, birleşimin ayet toplamı 19'a bölünür; ~8 bit); mercek bedeli 7,3 bit düşülür. Sıralı sıfırlarda aile-içi yapı
~3 bit daha olasıdır (sıralı sıfırlarda aynı mercekte Kur'an'a benzer aileler daha sık; §6.2). Değişkelerde (önseller P1–P3, kapı V1) S_Q yalnız
mercek bedeli kadar kayar; en iyi mercek her değişkede (1,+1,19)'dur.

## 4. Kur'an'ın sırası (her sıfır, her puan)

Sıra p = (k + 1)/(N + 1). **Bilgi puanı:** k_alt / k_nokta / k_üst = S_alt / S_nokta / S_üst değeri Kur'an'ın puanına eşit ya da
büyük olan sınama kitabı sayısı (N = 10⁸ sınama kitabı; tablolar ayrı tohumlu 10⁸ kitaplık referans örnekleminden); p_nokta k_nokta'dan,
köşeli parantezde k_nokta için %95 Clopper–Pearson ve çözülmemiş aramalardan doğan belirsizlik aralığı [(k_alt+1)/(N+1),
(k_üst+1)/(N+1)] (§5). **Sayım puanı (kapı G):** k / N, N = referans + sınama = 2×10⁸. J10 ikincildir (N_T = 10⁷, sayımda 3×10⁷).

| Sıfır (kodeks) | S_Q (bit) | **bilgi (V0, P0)**: k_alt / k_nokta / k_üst → p_nokta | sayım (kapı G): k / N → p [%95] |
|---|---|---|---|
| Tekdüze (129) | 31,06 | 2 / 3 / 16592 → **4,0×10⁻⁸** [k_nokta %95: 6,2×10⁻⁹; 8,8×10⁻⁸]; belirsizlik aralığı [3,0×10⁻⁸; 1,7×10⁻⁴] (adanmış kestirimle düzeltilmiş) | 89 / 2,0×10⁸ → **4,5×10⁻⁷** [3,6×10⁻⁷; 5,5×10⁻⁷] |
| M20 (129) | 27,41 | 16 / 22 / 18165 → **2,3×10⁻⁷** [k_nokta %95: 1,4×10⁻⁷; 3,3×10⁻⁷]; belirsizlik aralığı [1,7×10⁻⁷; 1,8×10⁻⁴] (adanmış kestirimle düzeltilmiş) | 59 / 2,0×10⁸ → **3,0×10⁻⁷** [2,2×10⁻⁷; 3,8×10⁻⁷] |
| Mallows (129) | 27,27 | 37 / 46 / 16150 → **4,7×10⁻⁷** [k_nokta %95: 3,4×10⁻⁷; 6,1×10⁻⁷]; belirsizlik aralığı [3,8×10⁻⁷; 1,6×10⁻⁴] (adanmış kestirimle düzeltilmiş) | 51 / 2,0×10⁸ → **2,6×10⁻⁷** [1,9×10⁻⁷; 3,4×10⁻⁷] |
| J10 (129) | 25,98 | 1 / 4 / 7473 → **5,0×10⁻⁷** [k_nokta %95: 1,1×10⁻⁷; 1,0×10⁻⁶]; belirsizlik aralığı [2,0×10⁻⁷; 7,5×10⁻⁴] | 11 / 3,0×10⁷ → **4,0×10⁻⁷** [1,8×10⁻⁷; 6,6×10⁻⁷] |

- **Uzunluk-sıralı sıfırlarda (M20, Mallows)** Kur'an'ın bütünlüğü her iki puanda **~3×10⁻⁷ mertebesindedir** (2,3–4,7×10⁻⁷);
  tanımları çok farklı iki puan (yüklem sayısı; birlikte sıklık) aynı mertebeyi verir. Veriye merkezli J10 (ikincil) de aynı mertebededir.
- **Tekdüzede** bilgi puanında 4,0×10⁻⁸, sayım puanında 4,5×10⁻⁷. Tekdüze sıfır Kur'an'ın uzunluk sıralı dizilişini de rastgele
  saydığı için bilgi puanında daha nadirdir; sayım puanında ise tekdüzenin tepesi yozlaşmış (sıfır toplamlı) ailelerle dolar (§6.2) —
  sonradan S0 bunları temizleyince tekdüze 3×10⁻⁸, M20 9×10⁻⁸, Mallows 6×10⁻⁸ olur (§7.3).
- **Bilgi puanının belirsizlik aralığı geniştir** (üst uç ~1,6–1,8×10⁻⁴): referans diliminde hiç görülmemiş maske taşıyan ~16.000–18.000
  kitabın üst sınırı ∞'dur. Bunların neredeyse hepsi eşiğin çok altındadır (S_alt medyanı ~8,5 bit; eşiği geçmek için çözülmemiş
  aramanın %95 üst sınırından medyan 2¹⁹–2²² kat daha nadir olması gerekir); eşiğe 3 bitten yakın olanlar tekdüzede 4, M20'de 25,
  Mallows'ta 31 kitaptır ve S_alt sırasıyla en üsttekiler adanmış kestirimle yeniden puanlandı (tekdüzede bir sıra sapmasıyla; §5). Aralığın üst ucu bir sıklık kestirimi değildir.
- **M20 zincir kümelenmesi** (ön-kayıt §4.2; `sonuc/sira/M129_zincir.txt`): bilgi isabetleri — adanmış düzeltmeden önceki 25 k_nokta
  kitabı; düzeltilmiş 22 bunların alt kümesidir — 25 farklı zincirde (zincir başına en çok 1); sayım isabetleri 59 → 57 zincir (iki
  zincirde ikişer: aynı zincirin ardışık emisyonları). Kümelenme ihmal edilebilir.

## 5. Bilgi puanında çözülmemiş aramalar, adanmış kestirim ve R2

**Neden aralık var.** Bilgi puanı, kitabın sağladığı maskelerin (aile boyuna koşullu) sıklığını 10⁸ kitaplık referans örnekleminden
okur. Nadir bir maske referans diliminde hiç ya da 10'dan az görülmüşse arama **çözülmemiş** sayılır ve ön-kayıt gereği bir tabana
kırpılmaz: puanın alt sınırına olasılığın tek yanlı %95 üst sınırı, üst sınırına alt sınırı girer (sayı 0 ise üst sınır ∞). Böylece
"belirsiz" kitaplar (S_alt < S_Q ≤ S_üst) çıkar: tekdüzede 16.595, M20'de ~18.150, Mallows'ta ~16.120 (10⁸ sınama kitabında; ~%0,02).
**Bunların neredeyse hepsi eşiğin çok altındadır:** Δ = S_Q − S_alt (Kur'an'ı geçmek için çözülmemiş aramanın %95 üst sınırından 2^Δ kat
daha nadir olması gerekir) dağılımı — tekdüze: en küçük 0,31, medyan 22,5, %90'lık 27,8 bit (Δ ≤ 3: 4 kitap); M20: 0,12 / 19,1 /
24,2 (Δ ≤ 3: 25); Mallows: 0,11 / 18,7 / 24,0 (Δ ≤ 3: 31). Belirsizlik aralığının üst ucu bu yüzden bir sıklık kestirimi değildir.

**Adanmış kestirim (ön-kayıt §4.3).** Önce Kur'an'ın çözülmemiş aramaları (yalnız tekdüzede vardı: 129'da aile maskesi, 2/1.014.579 →
kesin koşullu kestirim 46 isabet / 2×10⁷ öneri, 2,31×10⁻⁶; 127'de tekdüze ve Mallows, §7.1), sonra belirsiz kitaplar S_alt'a göre azalan
sırayla. Yöntem: tekdüzede kesin koşullu önem örneklemesi (aile yapısı sınıf-sayı vektörüyle önerilir, Φ(k)/M! ağırlığıyla düzeltilir;
p ≥ 11) ya da tek mercekli doğrudan örnekleme; sıralı sıfırlarda sıfırın kendi örnekleyicisiyle tek mercekli doğrudan örnekleme.
Doğrulama: kesin kestirici Kur'an'ın merceğinde 11 maskenin tekil sıklığında referans tablosuyla uyumlu (\|z\| ≤ 1,62); tek-mercek
değerlendirmesi tam değerlendirmeyle dört sıfırda 20.000'er kitapta bit-bit aynı. Bütçe (ön-kayıt): arama başına ≤ 2×10⁹ kitap
(tekdüzede ≤ 2×10⁸ öneri), sıfır başına ≤ 4 çekirdek-saat; hedef ≥ 30 isabet. Arama başına kitap sayısı, referanstaki nokta kestirimine
göre beklenen ≥ 30 isabete yetecek kadar seçildi (tavan 2×10⁹). Kullanılan: tekdüze 10 koşu / 12 arama (Kur'an'ınki dahil; ~1,8 çekirdek-saat),
10 kitap yeniden puanlandı. Bunların ikisi (S_alt 26,88) adanmış kestirim almadı: beş araması (~1,7 çekirdek-saat) süre önceliğiyle
koşulmadı, iki kitap aralıkta kalır. S_alt'ı 24,93 olan bir kitap ise tek ve ucuz bir aramayla S_alt sırasının dışında çözüldü. Bu sapma
`ek/EK_20261026T224527Z_tekduze_adanmis_sirasi.md`'de yazılıdır ve yalnız k_üst'ü 1 azaltır. M20 12 koşu / 12 arama, 14 kitap
(1,9×10¹⁰ kitap, ~2,9 çekirdek-saat); Mallows 11 koşu / 11 arama, 14 kitap (8,3×10⁹ kitap, ~3,4 çekirdek-saat; Mallows örnekleyicisi
hız ölçümünde M20'ninkinden 2,2 kat yavaş). Adanmış kestirimin kendi
belirsizliği (≥ 10 isabette ±1,96/√h/ln 2 bit; < 10 isabette Poisson–CP) aralığa taşınır: eşiğe ~0,4 bitten yakın kitaplar
adanmış kestirimden sonra da belirsiz kalabilir. Bir kitabın başka bir merceğinde çözülmemiş bir arama kalmışsa üst sınırı yine ∞'dur.

**Yeniden puanlanan kitaplar** (önce: referans tablosuyla; sonra: adanmış kestirimlerle; karar: alt ≥ S_Q → ÜSTTE, üst < S_Q → ALTTA):

*Tekdüze (S_Q 31,06):*

| kitap (bölüt:sıra) | önce: nokta [alt; üst] | sonra: nokta [alt; üst] | karar | en iyi mercek; kullanılan kestirimler |
|---|---|---|---|---|
| b015:3611874 | 34,99 [30,75; ∞] | 37,10 [36,67; 37,53] | ÜSTTE | (3,-1,17) kF=17; B:tablo 14870152/100000000, F:adanmış 73/1.99e+07, W:adanmış 794/9.93e+06, boy:tablo 24277 |
| b016:1250616 | 31,57 [30,55; 33,29] | 30,78 [30,42; 31,13] | BELİRSİZ | (1,-1,11) kF=11; B:tablo 31637/100000000, F:adanmış 63/9.82e+07, W:tablo 1693468/12263408, boy:tablo 12263408 |
| b010:1292372 | 30,69 [28,11; ∞] | 31,56 [30,62; 33,04] | BELİRSİZ | (2,+1,19) kF=6; F:tablo 979/16462346, W:adanmış 5/3.29e+08 |
| b018:2901379 | 29,67 [28,01; 34,54] | 28,82 [28,81; 28,83] | ALTTA | (1,-1,19) kF=19; B:tablo 112908/100000000, F:adanmış 88639/1.99e+07, W:tablo 263/606, boy:tablo 605 |
| b002:4795913 | 30,86 [29,92; 32,34] | 30,81 [30,54; 31,07] | BELİRSİZ | (1,+3,11) kF=11; B:adanmış 114/2e+09, F:tablo 178449/12292072, W:tablo 100896/12292072, boy:tablo 12292072 |
| b006:2181479 | 28,48 [27,14; ∞] | 28,63 [28,21; 29,05] | ALTTA | (1,+1,19) kF=6; F:tablo 1820/16464106, W:adanmış 45/3.29e+08 |
| b002:622353 | 29,46 [26,88; ∞] | 29,46 [26,88; ∞] | BELİRSİZ | (2,+1,5) kF=26; F:tablo 382/6709734, W:tablo 0/6709734 |
| b003:3971988 | 29,46 [26,88; ∞] | 29,46 [26,88; ∞] | BELİRSİZ | (1,-1,3) kF=36; B:tablo 112908/100000000, F:tablo 0/7383061, W:tablo 820903/7383061 |
| b005:2034545 | 29,16 [26,57; ∞] | 27,35 [26,87; 27,83] | ALTTA | (1,+1,19) kF=7; B:tablo 14870152/100000000, F:tablo 28043/14111542, W:adanmış 35/2.82e+08 |
| b004:2427157 | 26,60 [24,93; 31,47] | 27,59 [27,48; 27,71] | ALTTA | (2,+1,17) kF=17; F:adanmış 580/1.98e+07, W:tablo 53/25715, boy:tablo 25715 |

*M20 (S_Q 27,41):*

| kitap (bölüt:sıra) | önce: nokta [alt; üst] | sonra: nokta [alt; üst] | karar | en iyi mercek; kullanılan kestirimler |
|---|---|---|---|---|
| b002:3449001 | 29,87 [27,29; ∞] | 27,50 [27,10; ∞] | BELİRSİZ | (1,-1,5) kF=20; F:tablo 747/7657163, W:adanmış 52/1.53e+08 |
| b014:1713494 | 28,01 [26,86; 30,11] | 27,82 [27,37; 28,27] | BELİRSİZ | (1,-1,7) kF=23; B:tablo 14852634/100000000, F:adanmış 40/2.07e+07, W:tablo 191/2070059 |
| b005:11350 | 28,35 [26,69; 33,22] | 27,83 [27,40; 28,26] | BELİRSİZ | (1,-1,11) kF=11; B:tablo 14852634/100000000, F:adanmış 43/2.43e+08, W:tablo 100183/12163840, boy:tablo 12163840 |
| b008:2421252 | 27,95 [26,62; 30,77] | 26,66 [26,41; 26,92] | ALTTA | (2,-1,17) kF=7; F:tablo 1143/15381019, W:adanmış 122/3.08e+08 |
| b020:4411008 | 27,60 [26,45; 29,7] | 27,43 [26,98; 27,88] | BELİRSİZ | (1,+1,17) kF=17; F:tablo 28/31834, W:adanmış 40/3.23e+05, boy:tablo 31834 |
| b017:3690548 | 28,90 [26,32; ∞] | 29,32 [28,51; 30,51] | ÜSTTE | (3,-1,7) kF=14; B:tablo 14852634/100000000, F:adanmış 7/1.83e+08, W:tablo 3537/9169425 |
| b018:180465 | 30,50 [25,33; ∞] | 30,50 [25,33; ∞] | BELİRSİZ | (1,-1,19) kF=19; F:tablo 0/600, W:tablo 0/600, boy:tablo 597 |
| b007:4085278 | 30,22 [28,56; 35,09] | 30,22 [28,56; 35,09] | ÜSTTE | (1,-1,11) kF=9; B:tablo 46833/100000000, F:tablo 1/12721635, W:tablo 1156621/12721635 |
| b013:3447723 | 27,18 [26,25; 28,66] | 27,29 [26,89; 27,69] | BELİRSİZ | (1,-3,13) kF=9; B:tablo 14852634/100000000, F:adanmış 51/1.37e+08, W:tablo 2351/13653499 |
| b016:1210550 | 28,57 [25,99; ∞] | 26,81 [26,32; 27,29] | ALTTA | (1,-3,29) kF=7; B:adanmış 34/2e+09, F:tablo 127579/5608613, W:tablo 193286/5608613 |
| b004:1315858 | 28,40 [25,82; ∞] | 28,27 [27,41; 29,12] | ÜSTTE | (1,-1,13) kF=8; F:adanmış 11/2.81e+08, W:tablo 7000/14075785 |
| b007:3025535 | 26,92 [25,77; 29,01] | 25,92 [25,58; 26,25] | ALTTA | (1,-3,19) kF=6; B:tablo 14852634/100000000, F:tablo 6468/16478711, W:adanmış 70/1.65e+08 |
| b008:2217288 | 26,55 [25,62; 28,04] | 26,27 [25,92; 26,61] | ALTTA | (2,+1,13) kF=13; F:adanmış 67/4.46e+07, W:tablo 2581/4462950, boy:tablo 4462950 |
| b012:2173162 | 28,11 [25,52; ∞] | 26,52 [26,01; 27,04] | ALTTA | (2,-1,7) kF=17; F:adanmış 30/2.05e+08, W:tablo 2271/10236154 |

*Mallows (S_Q 27,27):*

| kitap (bölüt:sıra) | önce: nokta [alt; üst] | sonra: nokta [alt; üst] | karar | en iyi mercek; kullanılan kestirimler |
|---|---|---|---|---|
| b002:4901976 | 27,93 [27,16; 29,02] | 28,62 [28,00; 29,24] | ÜSTTE | (3,-1,13) kF=13; B:tablo 14854928/100000000, F:adanmış 21/1.78e+07, W:tablo 2164/4447502, boy:tablo 4447502 |
| b002:1106647 | 28,15 [27,12; 29,86] | 28,19 [27,71; 28,66] | ÜSTTE | (1,+1,29) kF=7; B:tablo 122711/100000000, F:adanmış 35/5.91e+07, W:tablo 208877/7390543 |
| b009:4425513 | 29,42 [26,84; ∞] | 27,34 [26,90; 27,78] | BELİRSİZ | (2,+1,17) kF=17; F:tablo 18/20688, W:adanmış 42/4.1e+05, boy:tablo 20688 |
| b017:1446297 | 27,85 [26,82; 29,57] | 27,89 [27,41; 28,37] | ÜSTTE | (1,-1,13) kF=13; B:tablo 14854928/100000000, F:adanmış 35/3.4e+07, W:tablo 16474/4246968, boy:tablo 4246968 |
| b005:4275857 | 27,57 [26,71; 28,89] | 27,59 [27,09; 28,09] | BELİRSİZ | (1,-1,7) kF=7; B:tablo 122711/100000000, F:adanmış 32/1.88e+06, W:tablo 148355/376918, boy:tablo 376918 |
| b005:3952558 | 27,12 [25,97; 29,22] | 26,64 [26,23; 27,04] | ALTTA | (1,+1,11) kF=11; B:tablo 14854928/100000000, F:adanmış 49/1.22e+08, W:tablo 100334/12152536, boy:tablo 12152536 |
| b012:4080581 | 26,78 [25,97; 27,97] | 27,06 [26,55; 27,57] | BELİRSİZ | (1,+3,19) kF=6; B:tablo 14854928/100000000, F:tablo 3305/16475836, W:adanmış 31/8.24e+07 |
| b020:942065 | 26,69 [25,97; 27,71] | 27,42 [26,83; 28,0] | BELİRSİZ | (3,-1,11) kF=11; B:tablo 14854928/100000000, F:adanmış 23/4.88e+07, W:tablo 12504/12198421, boy:tablo 12198421 |
| b019:1756562 | 26,83 [25,96; 28,14] | 27,20 [26,64; 27,77] | BELİRSİZ | (2,-3,11) kF=11; F:adanmış 25/6.16e+07, W:tablo 1249/12317864, boy:tablo 12317864 |
| b014:63204 | 26,81 [25,87; 28,29] | 27,61 [26,97; 28,26] | BELİRSİZ | (1,+3,17) kF=17; B:tablo 14854928/100000000, F:adanmış 19/1.37e+05, W:tablo 37/22615, boy:tablo 22615 |
| b015:2715991 | 26,84 [25,82; 28,56] | 26,59 [26,16; 27,02] | ALTTA | (1,-1,19) kF=5; B:tablo 14854928/100000000, F:tablo 22187/16886117, W:adanmış 43/1.35e+08 |
| b005:793155 | 27,34 [25,68; 32,21] | 27,34 [25,68; 32,21] | BELİRSİZ | (2,+1,11) kF=15; F:tablo 1/4061873, W:tablo 203/4061873 |
| b006:364162 | 27,32 [25,66; 32,19] | 27,32 [25,66; 32,19] | BELİRSİZ | (1,+3,7) kF=15; F:tablo 1/10314657, W:tablo 662/10314657 |
| b012:2452286 | 28,17 [25,59; ∞] | 28,17 [25,59; ∞] | BELİRSİZ | (1,-1,7) kF=14; B:tablo 14854928/100000000, F:tablo 0/9504327, W:tablo 25230/9504327 |

**Düzeltilmiş sayılar** (`src/sira_duzelt.py`; bilgi V0 P0, N = 10⁸):
- tekdüze: k_alt / k_nokta / k_üst 1 / 3 / 16596 → **2 / 3 / 16592**; p_nokta 4,0×10⁻⁸ → **4,0×10⁻⁸**, belirsizlik aralığı [3,0×10⁻⁸; 1,7×10⁻⁴]
- M20: k_alt / k_nokta / k_üst 14 / 25 / 18170 → **16 / 22 / 18165**; p_nokta 2,6×10⁻⁷ → **2,3×10⁻⁷**, belirsizlik aralığı [1,7×10⁻⁷; 1,8×10⁻⁴]
- Mallows: k_alt / k_nokta / k_üst 34 / 44 / 16152 → **37 / 46 / 16150**; p_nokta 4,5×10⁻⁷ → **4,7×10⁻⁷**, belirsizlik aralığı [3,8×10⁻⁷; 1,6×10⁻⁴]

**R2 (model-tabanlı kuyruk).** Ön-kayıtlı doğrulama ölçütü: R2'nin P(S ≥ s) eğrisi doğrudan sıklıkla, doğrudan isabetin ≥ 30 olduğu en
yüksek dört eşikte oran [0,5; 2,0] içinde olmalı. Tekdüzede 3×10⁷ kitaplık ara bakışta geçti (0,55–0,65), tam örneklemde (10⁸) kıl payı
**geçemedi** (0,49–0,67): oran eşik yükseldikçe düşüyor, yani R2 kuyruğu giderek eksik kestiriyor (gruplar arası bağımlılık ve hiç
görülmemiş maskeler modelde yok). Bu yüzden R2 hiçbir kararda kullanılmadı; her birincil sıfırda doğrudan isabet var (k ≥ 1). Sayım
puanında R2 doğrudan sıklıkla neredeyse birebir (c = 6–10'da oran 1,00). M20 ve Mallows'ta R2 gerekmedi (ölçülmedi).

## 6. En güçlü rakipler — Kur'an'ın türünden mi, dağınık denk gelme mi?

Tam tablolar (her sıfır × puan, ilk 20; mercek, profil, k_F, k_H, \|U\|, hücre sayıları ve toplamları, en büyük \|q\|, sağlanan yüklemler,
ön-kayıtlı sınıf): `sonuc/rakipler/<sıfır>_{bilgi,sayim}.txt`. **Kur'an sınıfı** (ön-kayıt §7, önceden yazılmış): en iyi mercekte kapı G
tutar ve kredi alan yüklemler 4 motif türünün ≥ 3'ünden, 4 düzeyin ≥ 3'ünden gelir. Kur'an bu sınıftadır (4 tür, 4 düzey).

| Sıfır, puan | ilk 20'de ön-kayıtlı sınıfta | baskın yapı |
|---|---|---|
| Tekdüze, bilgi | 2 / 20 | tek nadir olay: aile boyu = p (10/20 QkF), kitap düzeyinde çapraz denge (B2, (1,±3)), nadir k bağlamında zengin maske |
| Tekdüze, sayım | 20 / 20 | **yozlaşmış**: büyük p (29–79), 8 üyeli 2/2/2/2 aileler, hücre toplamlarının hepsi 0 (16/20) ya da küçük ve eşit |
| M20, bilgi | 1 / 20 | aynı tür tek olaylar; biçim (1,−1) baskın (11/20); 6/20 tamamen çözülmüş |
| M20, sayım | 20 / 20 | (1,+1,p) büyük p'de 2/2/2/2, toplamlar eşit ve sıfırdan farklı (8,8,8,8 / 6,6,6,6); ve **Kur'an'ın merceğinde** 3/3/3/3 aileler |
| Mallows, bilgi | 2 / 20 | aynı tür tek olaylar; 15/20'de bir boy olayı (k_F, k_H ya da \|U\| = p); biçim (1,−1) baskın (13/20) |
| Mallows, sayım | 20 / 20 | M20 ile aynı: (1,+1,p) eşit toplamlı 2/2/2/2, (1,+1,19)'da 3/3/3/3 (3/20), birkaç sıfır-toplamlı |
| J10 (ikincil), bilgi | 1 / 20 | aynı tür tek olaylar |
| J10 (ikincil), sayım | 20 / 20 | aynı desen (sıfır-toplamlı, eşit toplamlı 2/2/2/2, bir (1,+1,19) 3/3/3/3) |

### 6.1 Bilgi puanı (birincil)
**Tekdüze.** Tepedeki kitaplar puanlarını çoğunlukla bir ya da iki nadir olaydan alır, Kur'an'daki gibi dört düzeye yayılmış bir
yeniden-belirme ağından değil:
- **37,1 bit** [36,7; 37,5] (adanmış kestirimle; ilk aşamada 34,99 [30,75; ∞]) — (3,−1,17): **k_F = 17 = p** (QkF; P(k_F = 17) ≈ 2,4×10⁻⁴),
  k = 17 bağlamında {R1c, K2} (3,7×10⁻⁶, adanmış kesin kestirim, 73 isabet) ve {QU, QH} (8,0×10⁻⁵, adanmış, 794 isabet), B1. Bu mercek
  ön-kayıtlı sınıf koşulunu geçer (kapı G, 3 tür, 4 düzey) ama içeriği Kur'an'ınkinden farklıdır: \|U\| = 17 = k_F (H ⊆ F), hücre sayıları
  2/5/5/5, hücre toplamları (10, 32, 23, 41) — eşit sayı yalnız bir satırda, hücre toplamlarında denge, ardışıklık ve merkez yok.
- **32,7 bit** [31,8; 34,2] — (1,+3,7): puanının ~24 biti tek bir kitap-düzeyi olaydan, (1,±3) biçiminde çapraz dengeden gelir
  (Σ_{E⁰} n = 3·Σ_{E¹} a; adanmış kestirim 5,7×10⁻⁸ — 114 isabet / 2×10⁹), üstüne k_F = p = 7, QS, Qh, W1, Qn. Kapı G tutmaz (sınıf dışı).
- Sınıftaki ikinci kitap (1,−1,19), 29,67 → adanmışla 28,82 [28,81; 28,83] (eşiğin altında): k_F = 19 = p, hücre toplamları (0, 10, 0, 9).
- Öteki belirsiz-üst kitaplar benzer: büyük-sapma tipi tek denge (B2'nin α ≠ \|β\| biçimleri), k_F ya da k_H'nin p'ye eşit olması,
  ya da nadir k bağlamında zengin bir maske.

**M20.** İlk 20'nin yalnız biri sınıfta: (1,+1,23), 27,99 [22,81; ∞] — k_F = 23 = p, hücreler 8/5/5/5, R1c K2 QkF W1 (dört yüklem;
sınıf kuralı genişliği ölçer, derinliği değil). Tepedekiler: (1,−1,19) k_F = 19 = p, 30,50 [25,33; ∞] (k_F = 19 bağlamı referansta yalnız
600 kez görülmüş; aramaları çözülemedi); (1,−1,11) 30,22 [28,56; 35,09] (B1 ∧ B2 + dört hücre toplamı eşit 8,8,8,8 — eşiğin kesin
üstünde); (1,−1,5) 29,87 [27,29; ∞] (adanmışla 27,50 [27,10; ∞], belirsiz); (1,−1,7) 29,66 (çözülmüş; k_F = 7 = p, B1 ∧ B2, K1). 20'nin 8'inde QkF (aile boyu = p), 4'ünde
B1 ∧ B2. Hiçbiri Kur'an'ın merceğinde değildir.

**Mallows.** İlk 20'nin 2'si sınıfta: (1,−1,17) k_F = 17 = p, 29,98 (çözülmüş; B1 B2 F2 QS Qh QkF W1 Qa) ve (2,+1,17) k_F = 17 = p,
29,42 [26,84; ∞] (adanmışla 27,34 [26,90; 27,78], belirsiz). Tepede (1,+1,11) 31,78 [30,63; 33,88] (k_F = \|U\| = 11 = p: QkF, QU ve Qa/Qn/QH birlikte) ve (1,−1,17) 31,59
(çözülmüş; k_H = 17 = p, K1, K2). İlk 20'nin 15'inde bir boy olayı (k_F, k_H ya da \|U\| = p) vardır. Hiçbiri Kur'an'ın merceğinde
değildir. **J10** (ikincil): ilk 20'nin biri sınıfta; tepede (2,−1,5) 27,36 [25,70; 32,23] ve (1,−1,19) 27,08 (çözülmüş; dört hücre
toplamı eşit 4,4,4,4, \|U\| = 19).

### 6.2 Sayım puanı (kapı G)
**Tekdüze: yozlaşma.** İlk 20'nin hepsi aynı türdür: büyük p (29–79), 19'u β < 0 biçimleri ((1,−1), (1,−3)), 8 üyeli 2/2/2/2 hücreli
aileler, hücre toplamlarının hepsi 0 (16/20) ya da \|s\| ≤ 6 küçük ve eşit sayılar, en büyük \|q\| 1–4. Nedeni: t = 0 konumları (ör. (1,−1)'de a_n = n) her
asalın ailesine girer (q = 0) ve büyük p'de aile q ∈ {−2…2} üyelerden oluşur; "0 = 0" ve "p \| 0" dengeleri kendiliğinden tutar. İkisi 12
ile Kur'an'ı geçer. Ön-kayıtlı sınıf kuralı (kapı, tür, düzey) bunların hepsine "Kur'an sınıfı" der — **kural katsayı büyüklüğü
yozlaşmasını yakalamıyor**; bu, ön-kayıtlı sınıflandırmanın bir zayıflığıdır. Sonradan S0 (sıfır toplamlı karşılaştırmalar kredisiz;
ek, sonuçlara bakılmadan önce tanımlandı) C ≥ 11 olan 89 kitabın 66'sını 7'ye indirir; **S0 ≥ 11: 5 kitap / 2×10⁸** (→ 3×10⁻⁸): ikisi
(1,−1,19)'da 12 üyeli 3/3/3/3 aileler (toplamlar 4,4,5,3 ve 4,4,3,3; katsayılar −2…4 — yine küçük katsayılı), üçü 8 üyeli 2/2/2/2.

**M20: Kur'an'ın merceğinde Kur'an'a benzer aileler.** Sıralı sıfırda yozlaşma zayıftır (C ≥ 11 olan 59 kitabın 9'u S0'da 7'ye
düşer) ve rakipler başka bir türdendir: (1,+1,p), p = 29–43, toplamları sıfırdan farklı ve eşit (8,8,8,8; 6,6,6,6) 2/2/2/2 aileler ve
**Kur'an'ın kendi merceği (1,+1,19)'da 12 üyeli 3/3/3/3 aileler**. S0 ≥ 11 olan 17 kitabın (2×10⁸'de; → 9×10⁻⁸) **9'u (1,+1,19)'dadır**;
bunların 7'sinde k_H = 7 ve \|U\| = 19 (QU), hücre toplamları (18,18,19,17), (14,16,15,15), (16,18,17,17), (20,20,21,19) …; 8'inde ardışık hücre
toplamları (K1), hepsinde dar ve kesintisiz katsayı bandı (ör. 5–7, 4–6, 4–7; K2) — Kur'an'ın aile yapısına çok yakın (`sonuc/s0/M129_S0_11.txt`). **Hiçbirinde Σq = 4p (Qc, "merkez 19") yoktur**
(Σq = 60–80; Kur'an 76): Kur'an'ı bunlardan ayıran yüklemler merkez (Qc) ve terazidir (B2; hiçbirinde yok — 57/57 (B1) ise çoğunda var). Bununla uyumlu olarak Kur'an'ın
aile-içi maskesinin k = 12'deki olasılığı M20'de 2,0×10⁻⁵, tekdüzede 2,3×10⁻⁶'dır (~9 kat; §3). Olası açıklama (ayrıca ölçülmedi):
uzunluk-sıralı bir dizilişte t = n + a düzgün değişir, 19 ailesinin katsayıları dar bir banda düşer ve hücrelerin eşit sayısı ile
ardışık toplamları kolaylaşır; Kur'an'ın (1,+1,19) ailesinin yapısının bir kısmı dizilişin sıralı olmasıyla açıklanabilir.
**Mallows'ta aynı desen:** C ≥ 11 olan 51 kitap; S0 ≥ 11: 11 / 2×10⁸ (→ 6×10⁻⁸), bunların 5'i (1,+1,19)'da 12 üyeli 3/3/3/3 aile
(k_H = 7, \|U\| = 19; Σq = 60–70), ötekiler (1,+1,p) eşit toplamlı 2/2/2/2; yine hiçbirinde merkez yok. **J10'da** S0 ≥ 11: 2 / 3×10⁷
(biri (1,+1,19) 3/3/3/3).

**Ana hattın bilinen rakipleri** (`tab/ana_hat_rakipleri.txt`): J10 sahte katı 8/8 (Kur'an'ın titreşimli kopyası) sayımda 10 (aynı
mercek, 3/3/3/3); dört J10 robot rakibi 2–4; dış çalışmanın "7'de Kur'an'ı geçen" üç rakibi sayımda 3–4 (kapı G'de; kapısız G0'da 6–10 —
yozlaşma kuralı amaçlandığı gibi eliyor), bilgi puanında 9,2–22,6 bit (üç birincil sıfırda; Kur'an 27,3–31,1; `sonuc/ana_hat_rakipleri_129.txt`).

### 6.3 Keşif: yalnız Kur'an-sınıfı mercekleri (sonradan; ek)
Sınıf-kısıtlı puan (yalnız ön-kayıtlı sınıf koşulunu sağlayan mercekler; sonuçlar görüldükten sonra tanımlandığı için keşiftir):
tekdüzede bilgi ≥ S_Q 1 kitap / 10⁸ (yukarıdaki 37,1 bitlik (3,−1,17) kitabı), sayım ≥ 11: 89 / 2×10⁸ (hepsi sınıfta), S0 ≥ 11: 5;
M20'de bilgi 1 / 10⁸ (27,99 [22,81; ∞], belirsiz), sayım 59, S0 17; Mallows'ta bilgi 3 / 10⁸, sayım 51, S0 11; J10'da bilgi
0 / 10⁷, sayım 11 / 3×10⁷, S0 2.

## 7. İkincil analizler

### 7.1 (a) 127 sayımı (Tevbe 127)
Kur'an'ın 127 profili 129'dakiyle aynıdır (en iyi mercek (1,+1,19); 15 yüklem; sayım 11 — sûre 9 ailede değildir, terazi 6234 ile tutar).
Her sıfırda kendi 3×10⁷ kitaplık referans tablosu ve 10⁷ sınama kitabı (sayımda N = 4×10⁷). Kur'an'ın aile araması referansta tekdüze ve
Mallows'ta çözülmedi (k = 12 bağlamında 2/304.101 ve 7/390.136); adanmış kestirimle çözüldü: tekdüze kesin koşullu 40 isabet / 2×10⁷
öneri, P = 2,01×10⁻⁶ (18,92 bit; geçici noktadan +2,03 bit); Mallows 39 isabet / 2,61×10⁶ bağlam kitabı (2×10⁸ kitap), P = 1,49×10⁻⁵
(+0,36 bit). M20'de arama referansta çözülmüştü (c ≥ 10; 14,47 bit); adanmış denetim koşusu 71 isabetle 15,05 bit verdi (~1,3σ
fark; ön-kayıt gereği tablo değeri kullanıldı). S_Q: tekdüze 31,20, M20 26,27, Mallows 27,83 bit.

| Sıfır (kodeks) | S_Q (bit) | **bilgi (V0, P0)**: k_alt / k_nokta / k_üst → p_nokta | sayım (kapı G): k / N → p [%95] |
|---|---|---|---|
| Tekdüze (127) | 31,20 | 0 / 0 / 5301 → **1,0×10⁻⁷** (k_nokta = 0: tek yanlı %95 üst sınır 3,0×10⁻⁷); belirsizlik aralığı [1,0×10⁻⁷; 5,3×10⁻⁴] | 23 / 4,0×10⁷ → **6,0×10⁻⁷** [3,6×10⁻⁷; 8,6×10⁻⁷] |
| M20 (127) | 26,27 | 4 / 8 / 6021 → **9,0×10⁻⁷** [k_nokta %95: 3,5×10⁻⁷; 1,6×10⁻⁶]; belirsizlik aralığı [5,0×10⁻⁷; 6,0×10⁻⁴] | 15 / 4,0×10⁷ → **4,0×10⁻⁷** [2,1×10⁻⁷; 6,2×10⁻⁷] |
| Mallows (127) | 27,83 | 0 / 1 / 5230 → **2,0×10⁻⁷** [k_nokta %95: 2,5×10⁻⁹; 5,6×10⁻⁷]; belirsizlik aralığı [1,0×10⁻⁷; 5,2×10⁻⁴] | 7 / 4,0×10⁷ → **2,0×10⁻⁷** [7,0×10⁻⁸; 3,6×10⁻⁷] |

127'de sıralar 129'dakilerle aynı mertebededir (sayım 2–6×10⁻⁷; bilgi 1–9×10⁻⁷, tekdüzede k = 0: artı-bir sıra 1×10⁻⁷, tek yanlı %95
üst sınır 3×10⁻⁷). Belirsiz kitaplar için 127'de adanmış kestirim yapılmadı (yalnız Kur'an'ın aramaları); aralıklar olduğu gibi
verilmiştir. Rakipler: bilgi puanında ilk 20'nin hiçbiri sınıfta değil (üç sıfırda da 0/20); sayımda 20/20 (129'daki desen). Sonradan
S0 ≥ 11: tekdüze 0, M20 4, Mallows 2 (N = 4×10⁷).

**Ana hattın bilinen rakipleri 127 tablolarıyla** (`sonuc/ana_hat_rakipleri_127.txt`): J10 sahte katı 8/8 (Kur'an'ın titreşimli kopyası)
sayımda 10 (S0 10) ve bilgi puanında 21,15 (tekdüze) / 18,47 (M20) / 19,45 (Mallows) bit — Kur'an'ın 31,20 / 26,27 / 27,83 bitinin
~8–10 bit altında; dört J10 robot rakibi sayımda 2–4, bilgide 2,6–13,5 bit.

### 7.2 (b) Eski literatüre koşullu sürüm — kalan katmanların bütünlüğü

**Koşul.** O = (n+a, 19) merceğinde 12 üyeli aile ve Σt = 1444 [1990], 6/6 ve 722/722 [en geç 2015], tek-t hücreleri 361/361 [en geç
2015]; ikincil O' = O ∧ terazi [1994]. Koşullu kitaplar 10. denetimin havuzlarıdır (örnekleyicileri ve tohumlarıyla; iki bölüt depodaki
kodla bit-bit yeniden üretildi). **Mercek devralınmıştır** ((1, +1, 19) sabit). O'nun kapattığı yüklemler herkes için kapalıdır: F1, F2,
R1e, Qc, QS, Qh (sayımda kapı G ile zorunlu olan R1c de); O' altında ayrıca B1, B2. Kalan yüklemler: B1, B2 (yalnız O), R0c, R1c (yalnız
bilgi), K1, K2, W1, QU, QkH, Qa, Qn, QH. Bilgi puanı: kalan maskelerin üç grupta (kitap, aile-içi kalan, iki el) koşullu sıklığı,
havuzdan **biri-dışarıda**.

**Kur'an'ın kalan yapısı:** B1, B2 (terazi), R0c, R1c (3/3/3/3), K1 (çift-t hücreleri 18/20), K2 (5–9 bandı), W1, QU, Qa (İki El).
Sayım: O'da **8**, O''de **6**. Bilgi (bit): O'da tekdüze 25,11 / M20 22,37 / Mallows 22,12 / J10 22,78 (J10'da bir arama çözülmemiş:
[21,75; 24,50]); O''de 13,56 / 11,50 / 11,28.

| Koşul, sıfır | N (havuz) | **bilgi: k_alt / k_nokta / k_üst → p_nokta** | sayım (sabit mercek): k → p | sayım (serbest mercek, duyarlılık): k → p |
|---|---|---|---|---|
| O, tekdüze | 200.000 | 0 / 0 / 2 → **5,0×10⁻⁶** (k = 0; tek yanlı %95 üst sınır 1,5×10⁻⁵; belirsizlikle ≤ 1,5×10⁻⁵) | 0 → 5,0×10⁻⁶ | 49 → 2,5×10⁻⁴ |
| O, M20 | 100.000 | 1 / 1 / 1 → **2,0×10⁻⁵** | 1 → 2,0×10⁻⁵ | 64 → 6,5×10⁻⁴ |
| O, Mallows | 83.000 | 3 / 3 / 5 → **4,8×10⁻⁵** [4,8×10⁻⁵; 7,2×10⁻⁵] | 0 → 1,2×10⁻⁵ | 39 → 4,8×10⁻⁴ |
| O, J10 (ikincil) | 20.000 | 0 / 0 / 2 → 5,0×10⁻⁵ [5,0×10⁻⁵; 1,5×10⁻⁴] | 1 → 1,0×10⁻⁴ | 12 → 6,5×10⁻⁴ |
| **O', tekdüze** | 104.514 | 104 / 106 / 106 → **1,02×10⁻³** | 42 → 4,1×10⁻⁴ | 3.322 → 3,2×10⁻² |
| **O', M20** | 150.660 | 495 / 495 / 495 → **3,29×10⁻³** | 233 → 1,6×10⁻³ | 5.015 → 3,3×10⁻² |
| **O', Mallows** | 126.963 | 433 / 433 / 433 → **3,42×10⁻³** | 204 → 1,6×10⁻³ | 4.470 → 3,5×10⁻² |

Okuma. (i) **O verildiğinde** (terazi hâlâ "yeni" sayılırken) kalan katmanların bütünlüğü 10⁻⁵ mertebesinde ayırt edicidir; bunun
çoğu 1994 terazisinden gelir (O-kitaplarında P(terazi) ≈ 3–6×10⁻⁴, ~11–12 bit). (ii) **O' verildiğinde** (yalnız bu çalışmanın
eklediği: çift-t hücreleri 18/20 = K1, 5–9 bandı = K2, 3/3/3/3 = R0c/R1c, İki El = W1/QU/Qa) sıra tekdüzede ~1/1.000, sıralı
sıfırlarda ~1/300'dür. (iii) Mercek serbest bırakılınca (duyarlılık, yalnız sayım) O''de ~1/30'a çıkar: rakipler başka merceklerde
benzer sayıda kalan olgu bulur. 10. denetimin aynı soruya (terazi kapalıyken, yani buradaki O''nün karşılığı) robotun 82 olguluk
bataryasıyla verdiği 3,95×10⁻² (tekdüze), 0,164 (M20) ve 0,191 (Mallows) sıraları, bu dilin daha dar (yeniden-belirme motiflerine
odaklı) yüklem kümesiyle ~40–55 kat daha ayırt edici çıkmaktadır (1,02×10⁻³, 3,29×10⁻³, 3,42×10⁻³); fark büyük
ölçüde dilin Kur'an'ın kalan katmanlarını adlandıran yüklemlerden kurulmasındandır (dil sonradan yazılmıştır).
Sınırlılık: O' havuzları 10. denetimde O-iskeletlerinin teraziyle yeniden tamamlanmasıyla üretildi; aynı iskeletten gelen kitapların
aile-içi maskesi ortaktır (iskelet başına ~1 terazi isabeti; ilişki zayıf ama sıfır değil).

### 7.3 (c) Yozlaşma kuralına ve sadelik önseline duyarlılık

Aynı sınama (ve sayımda referans) kitaplarında, her değişke kendi Kur'an eşiğiyle; hücreler p = (k + 1)/(N + 1) (bilgi: k_nokta,
N = 10⁸; sayım: N = 2×10⁸). Kur'an'ın puanı bütün kapılarda 11'dir; bilgi değişkelerinde S_Q yalnız mercek bedeli kadar kayar (V1'de
Kur'an'ın kapısı tuttuğu için V1 = V0). V0 P0 dışındaki bilgi değişkeleri adanmış düzeltme olmadan, nokta değerleriyle verilmiştir;
\* = k_nokta = 0 (artı-bir sıra; tek yanlı %95 üst sınır 3,0×10⁻⁸, J10'da 3,0×10⁻⁷).

| Sıfır (kodeks) | sayım G | GE | Gc | G0 | V0 P0 | V0 P1 | V0 P2 | V0 P3 | V1 P0 | V1 P1 | V1 P2 | V1 P3 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| Tekdüze (129) | 4,5×10⁻⁷ | 7,5×10⁻⁷ | 8,0×10⁻⁷ | 1,3×10⁻⁴ | 4,0×10⁻⁸ | 6,0×10⁻⁸ | 3,0×10⁻⁸ | 3,0×10⁻⁸ | 2,0×10⁻⁸ | 3,0×10⁻⁸ | 2,0×10⁻⁸ | 2,0×10⁻⁸ |
| M20 (129) | 3,0×10⁻⁷ | 1,3×10⁻⁶ | 1,8×10⁻⁶ | 1,6×10⁻⁴ | 2,3×10⁻⁷ | 5,3×10⁻⁷ | 1,2×10⁻⁷ | 2,0×10⁻⁷ | 3,0×10⁻⁸ | 6,0×10⁻⁸ | 1,0×10⁻⁸* | 3,0×10⁻⁸ |
| Mallows (129) | 2,6×10⁻⁷ | 1,7×10⁻⁶ | 2,4×10⁻⁶ | 1,5×10⁻⁴ | 4,7×10⁻⁷ | 7,4×10⁻⁷ | 2,4×10⁻⁷ | 3,8×10⁻⁷ | 4,0×10⁻⁸ | 6,0×10⁻⁸ | 2,0×10⁻⁸ | 4,0×10⁻⁸ |
| J10 (129) | 4,0×10⁻⁷ | 1,4×10⁻⁶ | 2,0×10⁻⁶ | 1,5×10⁻⁴ | 5,0×10⁻⁷ | 1,2×10⁻⁶ | 4,0×10⁻⁷ | 3,0×10⁻⁷ | 1,0×10⁻⁷* | 2,0×10⁻⁷ | 1,0×10⁻⁷* | 1,0×10⁻⁷* |

Okuma:
- **Sayım puanının kapıya duyarlılığı büyüktür.** Kapısız (G0) sayımda Kur'an ~1/7.000'dir (k ≈ 27.000–32.000 / 2×10⁸): küçük ailelerde
  dengeler kendiliğinden tutar. Görevin önerdiği kapı G bunu ~3 mertebe değiştirir; daha gevşek GE (k_F ≥ E) ve Gc (yalnız hücre ≥ 2)
  sırayı sıralı sıfırlarda 4–9 kat, tekdüzede ~1,7 kat yükseltir.
- **Bilgi puanı önsele az duyarlıdır** (P0–P3 arasında en çok ~4 kat — tekdüzede 2, M20'de 4,4, Mallows'ta 3,1: P1 eşit biçim ağırlığı Kur'an'ın (1,±1) avantajını azaltır,
  P2 metin ağırlıklı asal önseli 19'u ucuzlatır). **V1 (2E kapısı bilgi puanında)** sırayı sıralı sıfırlarda ~1 mertebe düşürür
  (M20 3×10⁻⁸, Mallows 4×10⁻⁸): bilgi puanının tepesindeki rakiplerin çoğu küçük ya da boyu p'ye eşit ailelerden gelir ve kapıyı geçmez.
  V1 birincil değildir (ön-kayıt §2.8: 2E kapısı Kur'an'ın aile boyuna tam denk düştüğü için).
- **Sonradan S0** (sıfır toplamlı karşılaştırmalar kredisiz): tekdüzede 5 / 2×10⁸ (3×10⁻⁸), M20'de 17 (9×10⁻⁸), Mallows'ta 11 (6×10⁻⁸);
  sayım puanının yozlaşmış tepesini temizler; sıralı sıfırlarda kalan rakiplerin yarısı Kur'an'ın merceğinde Kur'an'a benzer
  ailelerdir (§6.2).

### 7.4 (d) Dil seçiminin kalibrasyonu — bölme menüsü

**Soru:** Kur'an'ın üstünlüğü yalnız kendi bölme türüne (t-paritesi × n-paritesi) mi bağlı? **Menü** (ön-kayıt §8.4): yoğunluğu 1/2
olan beş ikili bölme — t2 (t mod 2), n2 (n mod 2), a2 (a mod 2), h (n ≤ 57 / > 57), t4 (t mod 4 ∈ {0,1} / {2,3}); her kitap sıralı
bir çift (D1, D2), D1 ≠ D2 (20 çift) ve mercek seçer. Yüklemler ön-kayıt §2.4'teki gibi (D1, D2) ile; kanıtlı kurallar çiftin parite yapısına
göre; yalnız sayım puanı (kapı G); N = 10⁷ / sıfır.

| Sıfır (N) | (d2) her kitap 20 çiftten seçer (Kur'an 11) | (d3) t2 menü dışı (Kur'an 9) | (d4) yalnız (t2, n2) (Kur'an 11) | S0 (d2) | S0 (d3) | S0 (d4) |
|---|---|---|---|---|---|---|
| Tekdüze (1,0×10⁷) | 8 / 1,0×10⁷ → 9,0×10⁻⁷ [3,5×10⁻⁷; 1,6×10⁻⁶] | 1634 / 1,0×10⁷ → 1,6×10⁻⁴ [1,6×10⁻⁴; 1,7×10⁻⁴] | 4 / 1,0×10⁷ → 5,0×10⁻⁷ [1,1×10⁻⁷; 1,0×10⁻⁶] | 0 / 1,0×10⁷ → 1,0×10⁻⁷ (tek yanlı %95 üst sınır 3,0×10⁻⁷) | 490 / 1,0×10⁷ → 4,9×10⁻⁵ [4,5×10⁻⁵; 5,4×10⁻⁵] | 0 / 1,0×10⁷ → 1,0×10⁻⁷ (tek yanlı %95 üst sınır 3,0×10⁻⁷) |
| M20 (1,0×10⁷) | 4 / 1,0×10⁷ → 5,0×10⁻⁷ [1,1×10⁻⁷; 1,0×10⁻⁶] | 2661 / 1,0×10⁷ → 2,7×10⁻⁴ [2,6×10⁻⁴; 2,8×10⁻⁴] | 3 / 1,0×10⁷ → 4,0×10⁻⁷ [6,2×10⁻⁸; 8,8×10⁻⁷] | 1 / 1,0×10⁷ → 2,0×10⁻⁷ [2,5×10⁻⁹; 5,6×10⁻⁷] | 1180 / 1,0×10⁷ → 1,2×10⁻⁴ [1,1×10⁻⁴; 1,2×10⁻⁴] | 1 / 1,0×10⁷ → 2,0×10⁻⁷ [2,5×10⁻⁹; 5,6×10⁻⁷] |
| Mallows (1,0×10⁷) | 9 / 1,0×10⁷ → 1,0×10⁻⁶ [4,1×10⁻⁷; 1,7×10⁻⁶] | 2680 / 1,0×10⁷ → 2,7×10⁻⁴ [2,6×10⁻⁴; 2,8×10⁻⁴] | 3 / 1,0×10⁷ → 4,0×10⁻⁷ [6,2×10⁻⁸; 8,8×10⁻⁷] | 4 / 1,0×10⁷ → 5,0×10⁻⁷ [1,1×10⁻⁷; 1,0×10⁻⁶] | 1215 / 1,0×10⁷ → 1,2×10⁻⁴ [1,1×10⁻⁴; 1,3×10⁻⁴] | 2 / 1,0×10⁷ → 3,0×10⁻⁷ [2,4×10⁻⁸; 7,2×10⁻⁷] |

**(d1)** Kur'an'ın en iyi çifti (t2, n2), merceği (1,+1,19), puanı 11 (birincil ile aynı); t2'siz en iyisi (n2, a2) @ (1,+1,19): 9.

Okuma:
- **Menü bedeli küçüktür.** Her kitap 20 sıralı çiftin en iyisini seçebildiğinde (d2), Kur'an'ın sırası aynı örneklemde yalnız
  (t2, n2) ile bulunana (d4) göre 1,3–2,5 kat yükselir: tekdüze 5,0×10⁻⁷ → 9,0×10⁻⁷, M20 4,0×10⁻⁷ → 5,0×10⁻⁷, Mallows
  4,0×10⁻⁷ → 1,0×10⁻⁶. İsabetler sırasıyla 4 → 8, 3 → 4, 3 → 9'dur; sayılar küçük olduğundan oranın belirsizliği büyüktür. Kur'an'ın
  en iyi çifti zaten (t2, n2)'dir; rastgele kitaplar başka çiftlerde 11'e nadiren ulaşır. Yani dilin bölme türünü Kur'an'a bakarak
  seçmenin bu menü içindeki bedeli ~2 kattır. Kur'an'ın üstünlüğü bölme seçiminin yan ürünü değildir.
- **Ama üstünlük Kur'an'ın kendi bölme türüne bağlıdır.** t2 herkes için menüden çıkarılınca (d3) Kur'an'ın en iyisi
  (n2, a2) @ (1,+1,19)'da 9'dur. Sırası 1,6×10⁻⁴ (tekdüze) ile 2,7×10⁻⁴ (sıralı sıfırlar) arasına çıkar: her ~4–6 bin rastgele kitaptan
  biri en az onun kadar puan alır. Bu, t-paritesi bölmesindeki sıradan 2,5–3 mertebe daha az nadirdir. Kur'an'ın yeniden-belirme yapısı
  t = n + a'nın paritesi üzerine kuruludur; başka bölmelerde aynı derecede öne çıkmaz.
- (d4)'ün bağımsız 10⁷'lik örneklemdeki değerleri, birincil sayımla (2×10⁸; §4) uyumludur. Sonradan S0 sürümleri aynı deseni verir:
  (d2) 1,0×10⁻⁷ (tekdüze, k = 0; tek yanlı %95 üst sınır 3,0×10⁻⁷) ile 5,0×10⁻⁷ (Mallows) arası; (d3) 4,9×10⁻⁵ ile 1,2×10⁻⁴ arası.
- Menü yalnız bölme türü seçimini kalibre eder. Yüklem seçimi ile mercek ailesi seçimi kalibre edilmedi (§9.1).

## 8. Ana hattın önceden kaydettiği tahmin tuttu mu?

**Tahmin (ön-kayıt §1, aynen):** "nesnenin bütününün bütünlük sırası uzunluk-sıralı sıfırlarda (M20, Mallows) merkez **10⁻⁸**,
aralık **10⁻⁶–10⁻¹⁰**; tekdüze sıfırda **10⁻¹⁰–10⁻¹²**; eski literatür koşullu (yalnız bu çalışmanın eklediği) bütünlük **10⁻²–10⁻⁴**."

**Kurallar (ön-kayıt §9, puanlara bakılmadan yazıldı):** "tuttu" = nokta kestirimi tahmin aralığında; "tutmadı" = %95 aralığı
(belirsizlik aralığıyla birleşik) tümüyle tahmin aralığının dışında, ya da yalnız üst sınır varken o üst sınır aralığın alt ucunun
altında; öteki durumlar "karar verilemedi". Birincil puan bilgi puanıdır, sayım puanı aynı kurallarla ayrıca değerlendirilir. Eski
literatür parçası O ve O' ile ayrı ayrı değerlendirilir (ek `EK_20261026T171720Z_O_ve_Ousu_tahmin.md`; bu karar O/O' sonuçları
görüldükten sonra, seçim yanlılığı olmasın diye "ikisi birden" olarak yazıldı). Hükümler `src/karar.py` ile doğrudan sonuç
dosyalarından üretilmiştir.

| Parça | Sıfır | Puan | Ölçülen | Tahmin aralığı | Hüküm |
|---|---|---|---|---|---|
| nesnenin bütünü | Tekdüze | bilgi (V0, P0) | p̂ = 4,0×10⁻⁸, birleşik aralık [2,4×10⁻⁹; 1,7×10⁻⁴] | [1,0×10⁻¹²; 1,0×10⁻¹⁰] | **tutmadı (daha az nadir)** |
| nesnenin bütünü | Tekdüze | sayım (G) | p̂ = 4,5×10⁻⁷, birleşik aralık [3,6×10⁻⁷; 5,5×10⁻⁷] | [1,0×10⁻¹²; 1,0×10⁻¹⁰] | **tutmadı (daha az nadir)** |
| nesnenin bütünü | M20 | bilgi (V0, P0) | p̂ = 2,3×10⁻⁷, birleşik aralık [9,1×10⁻⁸; 1,8×10⁻⁴]; \|log₁₀ p̂ + 8\| = 1,36 | [1,0×10⁻¹⁰; 1,0×10⁻⁶] | **tuttu** |
| nesnenin bütünü | M20 | sayım (G) | p̂ = 3,0×10⁻⁷, birleşik aralık [2,2×10⁻⁷; 3,8×10⁻⁷]; \|log₁₀ p̂ + 8\| = 1,48 | [1,0×10⁻¹⁰; 1,0×10⁻⁶] | **tuttu** |
| nesnenin bütünü | Mallows | bilgi (V0, P0) | p̂ = 4,7×10⁻⁷, birleşik aralık [2,6×10⁻⁷; 1,6×10⁻⁴]; \|log₁₀ p̂ + 8\| = 1,67 | [1,0×10⁻¹⁰; 1,0×10⁻⁶] | **tuttu** |
| nesnenin bütünü | Mallows | sayım (G) | p̂ = 2,6×10⁻⁷, birleşik aralık [1,9×10⁻⁷; 3,4×10⁻⁷]; \|log₁₀ p̂ + 8\| = 1,41 | [1,0×10⁻¹⁰; 1,0×10⁻⁶] | **tuttu** |
| eski literatür koşullu: O (görevin birincil koşulu) | Tekdüze | bilgi | k = 0 / 2,0×10⁵: artı-bir sıra 5,0×10⁻⁶, üst sınır 1,5×10⁻⁵ (N = 200.000) | [1,0×10⁻⁴; 1,0×10⁻²] | **tutmadı (daha nadir)** |
| eski literatür koşullu: O (görevin birincil koşulu) | Tekdüze | sayım | k = 0 / 2,0×10⁵: artı-bir sıra 5,0×10⁻⁶, üst sınır 1,5×10⁻⁵ (N = 200.000) | [1,0×10⁻⁴; 1,0×10⁻²] | **tutmadı (daha nadir)** |
| eski literatür koşullu: O (görevin birincil koşulu) | M20 | bilgi | p̂ = 2,0×10⁻⁵, birleşik aralık [2,5×10⁻⁷; 5,6×10⁻⁵] (N = 100.000) | [1,0×10⁻⁴; 1,0×10⁻²] | **tutmadı (daha nadir)** |
| eski literatür koşullu: O (görevin birincil koşulu) | M20 | sayım | p̂ = 2,0×10⁻⁵, birleşik aralık [2,5×10⁻⁷; 5,6×10⁻⁵] (N = 100.000) | [1,0×10⁻⁴; 1,0×10⁻²] | **tutmadı (daha nadir)** |
| eski literatür koşullu: O (görevin birincil koşulu) | Mallows | bilgi | p̂ = 4,8×10⁻⁵, birleşik aralık [7,5×10⁻⁶; 1,1×10⁻⁴] (N = 83.000) | [1,0×10⁻⁴; 1,0×10⁻²] | **karar verilemedi** |
| eski literatür koşullu: O (görevin birincil koşulu) | Mallows | sayım | k = 0 / 8,3×10⁴: artı-bir sıra 1,2×10⁻⁵, üst sınır 3,6×10⁻⁵ (N = 83.000) | [1,0×10⁻⁴; 1,0×10⁻²] | **tutmadı (daha nadir)** |
| eski literatür koşullu: O' = O ∧ terazi (tahminin ifadesi) | Tekdüze | bilgi | p̂ = 1,0×10⁻³, birleşik aralık [8,1×10⁻⁴; 1,2×10⁻³] (N = 104.514) | [1,0×10⁻⁴; 1,0×10⁻²] | **tuttu** |
| eski literatür koşullu: O' = O ∧ terazi (tahminin ifadesi) | Tekdüze | sayım | p̂ = 4,1×10⁻⁴, birleşik aralık [2,9×10⁻⁴; 5,4×10⁻⁴] (N = 104.514) | [1,0×10⁻⁴; 1,0×10⁻²] | **tuttu** |
| eski literatür koşullu: O' = O ∧ terazi (tahminin ifadesi) | M20 | bilgi | p̂ = 3,3×10⁻³, birleşik aralık [3,0×10⁻³; 3,6×10⁻³] (N = 150.660) | [1,0×10⁻⁴; 1,0×10⁻²] | **tuttu** |
| eski literatür koşullu: O' = O ∧ terazi (tahminin ifadesi) | M20 | sayım | p̂ = 1,6×10⁻³, birleşik aralık [1,4×10⁻³; 1,8×10⁻³] (N = 150.660) | [1,0×10⁻⁴; 1,0×10⁻²] | **tuttu** |
| eski literatür koşullu: O' = O ∧ terazi (tahminin ifadesi) | Mallows | bilgi | p̂ = 3,4×10⁻³, birleşik aralık [3,1×10⁻³; 3,7×10⁻³] (N = 126.963) | [1,0×10⁻⁴; 1,0×10⁻²] | **tuttu** |
| eski literatür koşullu: O' = O ∧ terazi (tahminin ifadesi) | Mallows | sayım | p̂ = 1,6×10⁻³, birleşik aralık [1,4×10⁻³; 1,8×10⁻³] (N = 126.963) | [1,0×10⁻⁴; 1,0×10⁻²] | **tuttu** |

**Hüküm.**
- **Uzunluk-sıralı sıfırlar — tuttu** (M20 ve Mallows, her iki puan): ölçülen sıralar 2,3–4,7×10⁻⁷, tahmin aralığının [10⁻¹⁰, 10⁻⁶]
  içinde; ancak tahminin merkezi 10⁻⁸'in **1,4–1,7 mertebe üstünde** (\|log₁₀ p̂ + 8\| = 1,36–1,67), aralığın "az nadir" ucunda.
- **Tekdüze — tutmadı** (her iki puan): ölçülen 4,0×10⁻⁸ (bilgi) ve 4,5×10⁻⁷ (sayım) tahmin aralığının [10⁻¹², 10⁻¹⁰] **üstünde** —
  Kur'an'ın bütünlüğü tekdüzede tahmin edilenden 2,6–3,7 mertebe **daha az nadirdir**.
- **Eski literatür koşullu — O ile çoğunlukla tutmadı (daha nadir), O' ile tuttu.** Görevin birincil koşulu O altında (12 üyeli aile,
  1444, 6/6, 722/722, 361/361 verildiğinde) kalan katmanlar tahminden daha nadirdir (bilgi 5×10⁻⁶–4,8×10⁻⁵; Mallows bilgi puanında
  %95 aralığı 10⁻⁴'e değdiği için karar verilemedi); bunun çoğu 1994 terazisinden gelir. Tahminin kendi ifadesine ("yalnız bu çalışmanın
  eklediği") karşılık gelen O' = O ∧ terazi altında kalan katmanlar 1,0×10⁻³–3,4×10⁻³ (bilgi) ve 4,1×10⁻⁴–1,6×10⁻³ (sayım): aralıkta.
- **Bütün tahmin: kısmen tuttu.** Sıralı sıfırlar ve O' parçası tuttu; tekdüze parçası tutmadı (daha az nadir); O parçası çoğunlukla
  tutmadı (daha nadir).

J10 ve 127 tahmine dahil değildir (ikincil): J10'da bilgi 5×10⁻⁷, sayım 4×10⁻⁷; 127 için §7.1.

## 9. Ölçülmeyenler ve sınırlılıklar

### 9.1 Ölçülmedi
- **Dilin Kur'an görüldükten sonra yazılmasının etkisi — ölçülmedi.** L'nin yüklemleri (B1, B2, F1, R0c/R1c, K1, K2, Qc, W1, QU, Qa …)
  Kur'an'ın bilinen katmanlarını adlandırmak için seçildi; dilde Kur'an'a özgü sabit yok, ama hangi niceliklerin "doğal" sayıldığı
  Kur'an'a bakılarak belirlendi. Mercek bedeli ve aile boyuna koşullama mercek seçimini ve küçük aileleri fiyatlar, **yüklem seçimini
  fiyatlamaz.** Bütün sıralar bu dile koşulludur; başka bir makul dilin (Kur'an görülmeden yazılmış) vereceği sıra ölçülmedi. Menü
  analizi (§7.4) yalnız bölme türü seçimini kalibre eder.
- **Mercek ailesi ve modül aralığı seçimi** (14 doğrusal biçim, 29 asal 3–113) — değiştirilmedi, ölçülmedi.
- **Gruplar arası bağımsızlık yaklaşımının etkisi** (bilgi puanında kitap ⊥ aile-içi ⊥ iki-el \| k_F) **Kur'an dışındaki kitaplar için —
  ölçülmedi.** Kur'an'ın merceğinde önceki ölçüm (10. denetim, κ ≈ 1) uyumludur. R2'nin doğrulamayı kıl payı geçememesi (§5) bu
  yaklaşımın kuyrukta kusursuz olmadığını gösterir; sıraları doğrudan sayım verdiği için kararlar bundan etkilenmez.
- **Literatür araması** (yayımlanmış iddiaların ve denenmiş örüntülerin sayısı), **sayım okulu** (Kûfe dışındaki ayet sayımları; yalnız
  Tevbe 129/127 ikiliği ölçüldü), **kültürel havuz** (başka metinler/iddialar) — ölçülmedi.
- **Bilgi puanıyla menü analizi** — ön-kayıt gereği ölçülmedi (menü yalnız sayım puanıyla).
- **Bölmeli örnekleme / alt küme benzetimi (ön-kayıt §6.3)** — yapılmadı, ölçülmedi. Bütün birincil sıfırlarda doğrudan isabet olduğu için
  (k ≥ 1) nadir-olay kuyruğu gerekmedi. R2 yalnız tekdüzede hesaplandı (doğrulanmadı); M20 ve Mallows'ta gerekmedi, ölçülmedi.
- **Belirsiz kitapların tamamının çözümü** — bütçe yüzünden yalnız S_alt sırasında en üsttekiler (tekdüzede bir sıra sapmasıyla;
  ek `EK_20261026T224527Z_tekduze_adanmis_sirasi.md`); kalanlar aralık olarak (§5).

### 9.2 Sınırlılıklar
- **Sınıf kuralı genişliği ölçer, derinliği değil.** Ön-kayıtlı "Kur'an sınıfı" koşulu (kapı, ≥ 3 tür, ≥ 3 düzey) sıfır toplamlı ya da
  küçük katsayılı yozlaşmış aileleri ve dört yüklemli profilleri de içeri alır (§6). Sonradan S0 ve sınıf-kısıtlı keşif bunu ayrıca
  gösterir; ikisi de karar kurallarında birincil sayının yerine geçmez.
- **Kapı G yozlaşmayı tam yakalamaz** (sayım puanı): büyük p'de 2E küçüktür; t = 0 konumları her aileye girer. Ön-kayıtlı sayım puanı
  olduğu gibi raporlandı; S0 sonradan tanımlıdır.
- **Bilgi puanının üst kuyruğu tek nadir olaylara duyarlıdır** (çapraz kitap dengesi, aile boyu = p). Bu, dilin olayları eşit
  ağırlıklandırmasının değil, birlikte sıklığın doğal sonucudur; ama "çok motifli bütünlük" ile "tek büyük sapma" aynı ölçekte yarışır.
- **O' havuzları** 10. denetimde O-iskeletlerinin teraziyle yeniden tamamlanmasıyla üretildi; aynı iskeletten gelen kitaplar
  ilişkilidir (§7.2).
- **Sıralar sıfır modeline koşulludur.** M20 robotun zinciridir (S ≤ S_gözlenen); Mallows yalnız terslik sayısına ayarlıdır; ikisi de
  "uzunluk-sıralı" dizilişin bir modelidir, tarihsel diziliş sürecinin değil.

## 10. Sapmalar ve ekler

Ön-kayıt dondurulduktan sonra yazılan bütün ekler (`ek/`), zamanlarıyla:

| Ek | Konu | Sonuca etkisi |
|---|---|---|
| `EK_20261026T171720Z_O_ve_Ousu_tahmin.md` | Eski literatür koşullu tahmin parçası O ve O' ile **ayrı ayrı** değerlendirilir (sonuç görüldükten sonra yazıldı) | yalnız hükmün yazılışı |
| `EK_20261026T172158Z_sifir_toplam_yozlasmasi.md` | Sayım puanında sıfır-toplam yozlaşması gözlendi; **sonradan** S0 duyarlılığı tanımlandı (sonuçlarına bakılmadan önce) | birincil sayılar değişmez; S0 ayrıca |
| `EK_20261026T174901Z_aday_payi.md` | Aday dosyası payı 6 → 2 bit (disk) | k sayıları değişmez |
| `EK_20261026T175853Z_R2_bant_yonu.md` | R2 kalibrasyon bandının yönü (R2/oran) | R2 kararda kullanılmadı |
| `EK_20261026T180313Z_aday_G0.md` | Sayım aday ölçütünden G0 çıkarıldı (disk) | k sayıları değişmez |
| `EK_20261026T181111Z_sinif_kisitli_kesif.md` | Keşif: yalnız Kur'an-sınıfı mercekleriyle sıra | keşif; karar dışı |
| `EK_20261026T194928Z_adanmis_tohumlari.md` | Adanmış kestirim tohumlarında sıfır numarası bir kaydırılmış | yok |
| `EK_20261026T200253Z_dondurma_sonrasi_kod.md` | Dondurmadan sonra değişen/eklenen kod; `dil.py` ↔ C yeniden sınandı (0 uyuşmazlık) | yok |
| `EK_20261026T224527Z_tekduze_adanmis_sirasi.md` | Tekdüze adanmış kestirimde S_alt sırasından sapma: bir kitap (S_alt 24,93) sırası gelmeden çözüldü; S_alt 26,88 olan iki kitabın beş araması koşulmadı | k_üst −1; k_alt, k_nokta değişmez |
| `EK_20261026T234102Z_son_kod_degisiklikleri.md` | Önceki ekten sonra değişen/eklenen koşu ve rapor kodu; `src/rapor_tablo.py` aralık hesabında büyük k taşma hatası (rapora girmeden) düzeltildi | yok |

Puanlara bakıldıktan sonra hiçbir tanım, ağırlık, eşik ya da bütçe değiştirilmedi. Tek yordam sapması tekdüze adanmış kestirimin
sırasıdır (`EK_20261026T224527Z_tekduze_adanmis_sirasi.md`).

## 11. Yeniden üretim

- **Ön-kayıt:** `ONKAYIT.md` (salt okunur), `sonuc/onkayit_ozet.txt` (UTC + SHA-256), `sonuc/onkayit_kod_sha256.txt`.
- **Tanım:** `src/dil.py` (Python başvuru), `src/butunluk.h` (C puanlayıcı; bit-bit eşit), `src/tablo.h`, `src/bilgi.h` (bilgi puanı).
- **Üretim:** `kos/orkestra.py kos/plan_ana.txt` — R (referans bölütü, `src/referans`) → B (`src/birlestir`, `src/tablo_kur`) → Q
  (`src/kuran_puan`, Kur'an eşiği) → T (sınama bölütü, `src/test`); sürdürülebilir (`.say` = bölüt tamam). Tohum =
  26102600000 + sıfır·10⁸ + tür·10⁷ + kodeks·10⁶ + bölüt (ön-kayıt §5.3; adanmış koşular için ek). Derleme: `kos/derle.sh`.
- **Adanmış kestirim:** `src/adanmis_u` (tekdüze, kesin koşullu önem örneklemesi), `src/adanmis_s` (tek mercekli doğrudan örnekleme);
  diziler `kos/adanmis_U3.sh`, `kos/adanmis_M.sh` ve iş kuyruğu `kos/is_listesi_2.txt` (Mallows, 127 Kur'an eşikleri; küresel
  4-süreç sınırıyla `kos/is_kuyrugu.py`); sonuçlar `sonuc/adanmis/`; yeniden puanlama
  `src/ayrinti` + `src/belirsiz_coz.py` + `src/sira_duzelt.py` (`sonuc/belirsiz/`).
- **Analiz:** `kos/analiz_sifir.sh <sıfırkodeks>` (sıra özetleri `src/ozet.py`, `src/bilgi_sira.py`; rakipler `src/rakipler.py`;
  S0 `src/s0`; keşif `src/sinif`); rapor tabloları `src/rapor_tablo.py`.
- **İkincil:** koşullu (b) `src/kosullu` (`sonuc/kosullu/`); menü (d) `src/menu` (`tab/D_*`; ilk dört tekdüze bölütü `kos/menu_kos.sh`,
  kalanı iş kuyruğuyla, aynı tohum yasasıyla), S0 `src/menu_s0`,
  özet `src/menu_ozet.py`; ana hat rakipleri `src/puanla_liste`.
- **Denetimler:** `kontrol/` (C ↔ Python eşitliği, örnekleyici bit-eşitliği, gerektirme taramaları, kural sayımları).
- **Kısa analizler** `kos/duraklat.sh` altında koşar (4 ağır süreç çalışıyorsa birini geçici durdurur).
- **Günlükler:** `sonuc/kosu_gunlugu.txt` (her koşunun bitişi), `sonuc/ara_notlar.md` (ara notlar).
- **Rapor:** `python3 -I src/rapor_kur.py` (bu dosyayı `rapor_taslak/` bölümlerinden kurar; tablolar ve hükümler sonuç dosyalarından).
- **Hesap ve disk:** günlüğe kayıtlı koşular 24,3 çekirdek-saattir: üretim 12,6, adanmış kestirim 7,8, menü 3,9 (`src/hesap_saati.py`,
  `sonuc/kosu_gunlugu.txt`; sürenin yazılmadığı sıralı dizilerde duvar saati, yani üst sınır). Kayıt dışı kısa analizler, dondurma öncesi
  doğrulamalar ve ilk adanmış denemeler ölçülmedi (kestirim: birkaç çekirdek-saat; toplam ~40 çekirdek-saatlik bütçenin altında).
  Aynı anda en çok 4 ağır süreç koştu (`kos/is_kuyrugu.py`, `kos/duraklat.sh`). Çalışma klasörü ~0,8 GB, paket ~60 MB.
- **Paket:** `paket/butunluk_paket.tar.gz` ve SHA-256'sı `paket/butunluk_paket.sha256`. Pakette derlenmiş ikililer, büyük sıklık
  tabloları (`tab/*.tab`; R bölütlerinden aynı tohumlarla yeniden kurulur), dondurma öncesi çıkarım taraması örneklemleri (`tab/cikarim`)
  ve dış girdiler (`girdi/`; yalnız SHA-256 listesi `sonuc/girdi_sha256.txt`) yoktur. Pakete giren her dosyanın SHA-256'sı
  `paket/PAKET_ICERIGI.sha256`'dadır.

---

## English summary

**Question.** How rare is the Quran structure's *cross-layer recurrence* — the same balanced binary split at every level, the same
modulus returning outside its definition, two disjoint "hands", consecutiveness at two levels — measured fairly against random
arrangements? (Task `GOREV.md`; study folder `~/opus5.5/butunluk`; `~/Desktop/hrf2` was only read.)

**Pre-registration.** `ONKAYIT.md` was written and frozen (2026-10-26T17:13:24Z, SHA-256 `d213dc89…fd513`) **before any random book's
integrity score was computed or seen**; only the Quran's own profile/count score and code validations were done before freezing.
Deviations are in timestamped addenda (`ek/`); no definition, weight, threshold or budget was changed after scores were seen.
One procedural deviation is documented: in the uniform null, one ambiguous book was resolved out of S_alt order, and the five
lookups of two higher-ranked books were not run (effect: upper ambiguity count −1).

**Language L.** Lenses (α, β, p): t = αn + βa, u = αn − βa, the external study's 14 forms × 29 primes 3–113 (406 lenses). Primary
split: t-parity × n-parity. 20 predicates in four motif types — (a) split balance (book 57/57 and book-level cross balance; family
halves equal count/sum; cells equal count/sum per row), (b) modulus return (centre Σq = 4p, p | Σq, p | half sums, family/H/union
size = p, p | Σa(U), p | Σn(U), p | Σ(u/p)), (c) two hands (F ∩ H = ∅), (d) consecutiveness (cell sums, coefficient band). No
Quran-specific constant appears. Proven logical implications are pruned (e.g. K1 ∧ Qc ⇒ F2, R1e); no unproven candidate rule remained.
**Count score:** number of non-implied predicates at the best lens, family/cell predicates only if k_F ≥ 2E and every cell ≥ 2 (gate G).
**Information score (primary):** −log₂ of the joint frequency of the satisfied predicate masks (book; within-family | k_F; two hands |
k_F; size event k_F = p), from an independent 10⁸-book reference sample per null, minus a pre-registered lens description cost
(simplicity prior). Unresolved lookups (< 10 reference hits) are carried as [lower, upper] bounds — no floor saturation — and the
highest-ranked ambiguous books were re-scored by dedicated estimation (exact conditional importance sampling for the uniform null, direct
single-lens sampling for the ordered nulls).

**Quran.** Best lens in every null, score and variant: **(1, +1, 19)** (t = n + a); 15 of 20 predicates; **count score 11**;
information score 31.06 bits (uniform), 27.41 (M20), 27.27 (Mallows), 25.98 (J10).

**(1) Rank** p = (k+1)/(N+1), 10⁸ test books per primary null (count score: 2×10⁸ incl. reference books):

| Null | information score: p_point [ambiguity interval] | count score (gate G) |
|---|---|---|
| M20 (primary ordered) | 2.3×10⁻⁷ [1.7×10⁻⁷, 1.8×10⁻⁴] | 3.0×10⁻⁷ [2.2×10⁻⁷, 3.8×10⁻⁷] |
| Mallows | 4.7×10⁻⁷ [3.8×10⁻⁷, 1.6×10⁻⁴] | 2.6×10⁻⁷ [1.9×10⁻⁷, 3.4×10⁻⁷] |
| Uniform | 4.0×10⁻⁸ [3.0×10⁻⁸, 1.7×10⁻⁴] | 4.5×10⁻⁷ [3.6×10⁻⁷, 5.5×10⁻⁷] |
| J10 (secondary, 10⁷) | 5.0×10⁻⁷ [2.0×10⁻⁷, 7.5×10⁻⁴] | 4.0×10⁻⁷ [1.8×10⁻⁷, 6.6×10⁻⁷] |

In the length-ordered nulls the Quran's integrity is at the **~3×10⁻⁷ level** under both scores; uniform ~4×10⁻⁸ (information).
The upper end of the ambiguity interval (~1.6–1.8×10⁻⁴) comes from 16–18 thousand books carrying masks never seen in the reference
sample (infinite upper bounds); almost all of them lie far below the threshold (median 19–22 bits short) — it is not a frequency
estimate. Codex 127: same order of magnitude.

**(2) Rivals.** Information score: only 0–2 of the top 20 per null pass the pre-registered "Quran class" rule; the top is held by
single rare events (family size = p, book-level cross balance) — scattered coincidences. Count score: all top-20 pass the class rule,
but in the uniform null they are degenerate (8-member zero-sum families at large p — the rule does not catch coefficient degeneracy;
a post-hoc zero-sum-excluded score S0 is reported: uniform 3×10⁻⁸, M20 9×10⁻⁸, Mallows 6×10⁻⁸). In the ordered nulls part of the
rivals are genuinely of the Quran's kind: **12-member 3/3/3/3 families at the Quran's own lens (1, +1, 19)**, with consecutive cell
sums, a coefficient band, mostly k_H = 7 and |U| = 19 — but **none has the centre Σq = 4·19 or the book-level cross balance (terazi)**.

**(3) Given the old literature.** Given O (12-member family, 1444, 6/6, 722/722, 361/361) the remaining layers rank 5.0×10⁻⁶ (uniform,
k = 0) to 4.8×10⁻⁵ (Mallows), mostly due to the 1994 balance (terazi). Given O' = O ∧ terazi — only this study's additions remain
(even-t cells 18/20, the 5–9 band, 3/3/3/3, the two hands) — 1.0×10⁻³ (uniform) to 3.4×10⁻³ (ordered) by information score,
4.1×10⁻⁴ – 1.6×10⁻³ by count; ~3×10⁻² with a free lens (sensitivity).

**(4) The main line's pre-registered prediction — partly held.** Ordered nulls (M20, Mallows): **held** under both scores (inside
[10⁻¹⁰, 10⁻⁶], but 1.4–1.7 orders of magnitude above the predicted centre 10⁻⁸). Uniform: **did not hold** (2.6–3.7 orders less rare than
the predicted 10⁻¹⁰–10⁻¹²). Old-literature conditional: with the task's primary condition O mostly **did not hold (rarer)**; with O',
which matches the prediction's wording ("only this study's additions"), **held**.

**(5) Not measured:** the effect of having written the language after seeing the Quran (the predicates name its known layers; lens
cost and size-conditioning do not price predicate choice); the choice of lens family and modulus range; the effect of the
between-group independence approximation for books other than the Quran; literature search, counting school, cultural pool; the
partition menu with the information score; rare-event (subset-simulation) tails (not needed: k ≥ 1 everywhere); dedicated resolution
of all ambiguous books (budget; reported as intervals).

**(d) Partition menu** (count score only; N = 10⁷ per null). Letting every random book choose the best of 20 ordered partition pairs
raises the Quran's rank by only a factor of 1.3–2.5, compared with the fixed (t-parity, n-parity) pair on the same sample: uniform
5.0×10⁻⁷ → 9.0×10⁻⁷, M20 4.0×10⁻⁷ → 5.0×10⁻⁷, Mallows 4.0×10⁻⁷ → 1.0×10⁻⁶ (3–9 hits, so the ratio is uncertain). The Quran's best pair
is already the primary one. If t-parity is removed from the menu for everyone, the Quran's best scores 9 and its rank rises to
1.6–2.7×10⁻⁴. So the Quran's advantage is tied to its own partition type, but it is not an artefact of choosing that type from the menu.
Predicate choice remains uncalibrated.
