## 1. Kısa cevaplar

**(1) Kur'an'ın bütünlük puanı ve sırası.** L dilinde Kur'an'ın en iyi merceği her sıfırda, her puanda ve her duyarlılık değişkesinde
**(α, β, p) = (1, +1, 19)**'dur (t = n + a, modül 19). 20 yüklemin 15'i sağlanır; kanıtlı gerektirmeler ayıklanınca **sayım puanı 11**.
**Bilgi puanı** (birincil) tekdüzede 31,06, M20'de 27,41, Mallows'ta 27,27 bit (J10: 25,98). Rastgele kitaplar arasında sıra
p = (k + 1)/(N + 1):

| Sıfır | bilgi puanı (birincil): k_alt / k_nokta / k_üst → p_nokta [belirsizlik aralığı] | sayım puanı (kapı G): k / N → p |
|---|---|---|
| **M20** (birincil sıralı; N = 10⁸) | [M129_BILGI_K] → **[M129_BILGI]** [M129_BILGI_ARALIK] | [M129_SAYIM_K] → **[M129_SAYIM]** [M129_SAYIM_CI] |
| **Mallows** (N = 10⁸) | [L129_BILGI_K] → **[L129_BILGI]** [L129_BILGI_ARALIK] | [L129_SAYIM_K] → **[L129_SAYIM]** [L129_SAYIM_CI] |
| **Tekdüze** (N = 10⁸) | [U129_BILGI_K] → **[U129_BILGI]** [U129_BILGI_ARALIK] | [U129_SAYIM_K] → **[U129_SAYIM]** [U129_SAYIM_CI] |
| J10 (ikincil; N = 10⁷) | [J129_BILGI_K] → [J129_BILGI] [J129_BILGI_ARALIK] | [J129_SAYIM_K] → [J129_SAYIM] [J129_SAYIM_CI] |

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
