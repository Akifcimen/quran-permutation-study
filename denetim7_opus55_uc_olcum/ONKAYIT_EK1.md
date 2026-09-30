# ÖN-KAYIT EKİ 1 — 2. ölçümde kestirici değişikliği (SuS doğrulamayı geçemedi)

**Tarih:** 10 Ekim 2026 · ana ön-kayıttan (`ONKAYIT.md`, SHA-256 `5081bacd…`, 12:46:09 UTC) SONRA yazıldı.
**Statü:** Kur'an'ın ya da herhangi bir rakip kitabın tam birlikte-olasılığı (S) HENÜZ hesaplanmadı. Bu ek, aşağıdaki
yeni kestiricilerle o hesaplar yapılmadan önce dondurulur (özet: `sonuc/onkayit_ek1_ozet.txt`).

## 1. Ne oldu (V1 doğrulaması)

Ön-kaydın §2.2 kestiricisi (alt-küme benzetimi, `src/sus.c`) yazarın tekdüze sıfırdaki bilinen olaylarında denendi
(seviye başına N = 20.000, R = 4–8):

| Olay | Bilinen | SuS (ilk hamle kümesi) | SuS (U-hedefli + çift kalıntı takaslı hamleler) |
|---|---|---|---|
| C | 1,849×10⁻⁵ | 0,98–1,48×10⁻⁵ | 1,60×10⁻⁵ (tekrarlar 0,77–2,6×10⁻⁵) |
| D | 1,394×10⁻⁷ | C→D seviyesinde 2×10⁶ adımda 0 isabet | 8,6×10⁻⁸ (tekrarlar 2,4×10⁻⁸–7,1×10⁻⁷) |
| G | 7,14×10⁻¹⁰ | 0 isabet | 4 tekrarın 3'ünde D→G 0 isabet |

Sebep: kesin-toplam kısıtları (katsayı toplamı = 76, yarı toplamı = 38, hücreler) yerel hamlelerle neredeyse donmuş
kümeler oluşturuyor; zincirler aile yapıları arasında karışamıyor, tahmin tohumların çeşitliliğine kalıyor. V1 ölçütü
(×1,25 / ×1,5) **tutmadı**. Ön-kayıt §2.2 gereği SuS bu sorunda **güvenilmez** sayılır ve hiçbir sonucu raporlanmaz
(yalnız bu başarısızlık raporlanır).

## 2. Yeni kestiriciler (hiçbiri MCMC karışmasına dayanmaz)

**(a) Doğrudan sayım (sıralı sıfırlar J10 ve M20; tekdüzede k≥2E içermeyen hikâyeler).** Sıfırdan bağımsız örnekler
çekilir; her örnekte sabit mercekte (1,1,19) olgular bir kez hesaplanır ve dosyadaki her hikâyenin olgu kümesinin tümüyle
tutup tutmadığı sayılır. S = −log10(isabet/deneme); belirsizlik kesin Poisson aralığı. Hız için her hikâye en düşük
koşulsuz sıklıklı olgusuna "çapa"lanır (yalnız çapa olgusu tutan örneklerde denetlenir; sonuç değişmez).
J10 örnekleyicisi robotunkiyle aynı dağılımdır (anahtar = konum + 10·Z); Z için kesin bir normal üreteci (ziggurat)
kullanılır. M20 örnekleyicisi robotun 64 zincirli, 30.000 yakmalı, 20 inceltmeli MCMC'sidir.

**(b) Tekdüze sıfırda önem örneklemesi ("12-kale" önerisi; F: k≥2E ya da G: k≥2E içeren hikâyeler).** Sabit mercekte
F üyeliği a ≡ −n (mod 19), G üyeliği a ≡ n (mod 19) koşuluna bağlıdır; konumlar 19 kalıntı sınıfına (6'şar), değerler
kalıntılarına göre ayrılır. Öneri: tahta üzerinde 12 saldırmayan kale (12 eşleşmiş konum–değer çifti) tekdüze çekilir
(sınıf doluluk vektörü ∏ C(6,s)·C(c_r,s)·s! ağırlığıyla, ardından sınıf içinde tekdüze), kalan 102 değer kalan
konumlara tekdüze dağıtılır. Her çıktı kitabında üye sayısı K ≥ 12'dir ve ağırlık kesin olarak
w = r₁₂ · 102! / (114! · C(K, 12)) olur (r₁₂: tahtanın 12-kale sayısı; değerler ayırt edilebilir sayılır). Böylece
P(H) = E_q[w · 1{H}] yansızdır. Sağlamalar (analizden önce): E_q[w] = P(K ≥ 12) (kesin değer, büyük tamsayı hesabı),
yazarın B, C, D, G olayları (3,158×10⁻⁴, 1,849×10⁻⁵, 1,394×10⁻⁷, 7,14×10⁻¹⁰ — ölçüt ×1,25, G için ×1,5) ve
k≥2E içeren V3 hikâyelerinde doğrudan sayımla uyum (ölçüt §2.2 V3'teki gibi).

**Örneklem büyüklükleri (önceden):** J10 ve M20 doğrudan sayım 3×10¹⁰ örnek; tekdüze doğrudan sayım 5×10¹⁰; tekdüze
önem örneklemesi F ve G tahtalarında 2×10¹⁰'ar öneri. Kur'an'ın hikâyesi için isabet < 30 çıkarsa ilgili koşu aynı
tohum dizisiyle ×2 uzatılır (bir kez).

## 3. Sıra hesabı (ön-kayıt §2.3 aynen; yalnız S'nin kestiricisi değişti)

- Kitaplar ve bantlar aynı (E1 kitapları; ≥ Q−1 tamamı, [Q−2, Q−1)'den 1.500, [Q−3, Q−2)'den 500; seçim dosyaları
  `hikaye/E1_*_{b0,b1,b2}.idx`, tohum 2301/2302/2303).
- Bir rakibin isabeti Kur'an'ınkinden az ya da eşitse (S_rakip ≥ S_Kur'an) "Kur'an'ı eşler/geçer" sayılır. Sayımlar
  küçükse karar kesin Poisson aralıklarıyla verilir; aralıkları çakışan kitaplar için "belirsiz" sayısı ayrıca yazılır
  ve p iki uçla (belirsizler geçer / geçmez) raporlanır.
- (b) yolu aynen: D = S − T2 dağılımı, bantlara göre.

## 4. Bu eki yazmadan önce gördüklerim

- SuS'un yalnız yukarıdaki doğrulama sonuçları (yazar olayları).
- E1 hikâye dökümlerinin bileşimi: hikâyelerin ~%27'sinde F ya da G için k≥2E var (tekdüze 618/2.297, J10 1.488/6.127,
  M20 1.411/5.692); en sık olgular G: m²|Σq, G: Σn=Σa, U: F∩G=∅, U: m|Σw(U), F: 114|Σq. Kur'an'ın J10 hikâyesi:
  hücreler dengeli, k≥2E, Σq kare, m|Σa(U), |U|=m, katsayı bandı.
- Hiçbir hikâyenin tam olasılığı.
