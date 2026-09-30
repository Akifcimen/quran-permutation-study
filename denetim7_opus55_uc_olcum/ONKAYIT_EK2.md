# ÖN-KAYIT EKİ 2 — tekdüze kestirici, örneklem hedefleri, köken bulgusu (keşif amaçlı ek basamak)

**Tarih:** 10 Ekim 2026 · `ONKAYIT.md` (5081bacd…) ve `ONKAYIT_EK1.md` (b64215f1…) SONRASI.
**Statü:** Kur'an'ın ya da hiçbir rakibin tam birlikte-olasılığı (S) henüz hesaplanmadı; aşağıdaki keşif basamağı henüz
koşulmadı. Özet: `sonuc/onkayit_ek2_ozet.txt`.

## 1. Tekdüze sıfır: "12-kale" önerisi yerine birleşik-tahta yerleştirme örneklemesi

- EK1 §2(b)'deki "12-kale" önerisi yansız çıktı (10⁹ öneri: E[w] = 1,7395×10⁻² ± 5×10⁻⁶ ⟷ kesin P(K≥12) = 1,7402×10⁻²;
  B, C, D 1,6σ içinde; `sonuc/is_dogrulama_F_1e9.txt`) ama **verimsiz**: önerilen kitapların üye sayısı çoğunlukla 15–20,
  K = 12 yalnız %0,39 (sıfırda %1,0). K = 12'li hikâyeler için düz sayımdan bile kötü. Kullanılmayacak.
- Yerine (`src/bi.c`): (1,1,19)'da kendi-blok eşleşmeleri 10 dikdörtgen bloka ayrışır (n ≡ ±j, j = 1..9, ve n ≡ 0);
  E1 bataryasının bütün olguları yalnız eşleşme kümesi M = F ∪ G'ye bağlıdır. Verili (kF, kG, kI) için M tekdüze çekilir
  (blok DP'si); ağırlık w = D(kalan)·N(kF,kG,kI)/114!, D = kalan kitabın hiç kendi-blok eşleşmesi olmayan dağıtılış
  sayısı (kale polinomu, kesin). E[w·1{H}] = P(H ∧ sayılar). MCMC yok; yansız.
- **Doğrulama (yapıldı):** yalnız-F kipi P(K=12) = 1,015341×10⁻² ± 4×10⁻⁷ ⟷ kesin 1,015342×10⁻²; birleşik kipte kF = 12'li
  130 kombonun toplamı 1,015246×10⁻² ± 2,4×10⁻⁶ (−0,4σ). (Hızlandırılmış sürümdeki bir iplik-yarışı hatası bu sağlamayla
  yakalanıp düzeltildi.)
- **Kullanım:** Kur'an'ın tekdüze E1 hikâyesi (k≥2E, Σq kare, hücreler dengeli, katsayı bandı, |U| = 19, 19 | Σa(U))
  için kombolar kF ∈ {12, 16} (hücreler dengeli k ≡ 0 mod 4 ister), kU = 19, kI = 0..4: (12,7,0) 10⁹ öneri, diğer 9 kombo
  2×10⁸'er. kF ≥ 20 katkısı ihmal edilir (kesin: P(kF ≥ 20)/P(kF = 12) = 2,2×10⁻⁴; ek olarak hücre ve 19-birleşim koşullarıyla daha da küçülür; raporda yazılır).

## 2. Örneklem hedefleri (makine yükü)

Makine başka iş yükleriyle paylaşılıyor (yük ortalaması 200+; süreçlerim çekirdek başına 0,3–0,7 pay alıyor). EK1'in
hedefleri ~40 çekirdek-saat tutuyor. Yeni hedefler:
- J10 ve M20 doğrudan sayım: **10¹⁰**'ar örnek (EK1: 3×10¹⁰). Koşular kontrol noktalı; rapor zamanında ulaşılan N ne ise
  o yazılır (tohum dizisi sabit, kalanı sonradan sürdürülebilir).
- Tekdüze rakipler: tek tek tam S hesabı bu bütçeyle mümkün değil (Kur'an düzeyinde ~10⁻¹⁰,⁵). Tekdüzede yalnız (b) yolu:
  E1 hikâyelerinin doğrudan sayımı 2×10⁹ örnekle (alt bantlarda D dağılımı) + Kur'an'ın birleşik-tahta S'si. Tekdüze sıra
  için tam yeniden hesap YAPILMAYACAK; Kur'an'ın D'si ile rakiplerin D dağılımı karşılaştırılıp sıra kayması yalnız
  tahmin olarak (keşif) verilecek.

## 3. Köken bulgusu ve keşif amaçlı ek basamak (ön-kayıt dışı; manşete girmez)

Literatür taramasında (1. ölçüm) iki eski yayın doğrulandı:
- **Submitters Perspective, Ocak 1990, s. 3, "David Dixon's Discovery":** n+a'sı 19'un katı olan 12 sûre (6, 15, 21, 39, 41,
  42, 50, 55, 56, 70, 88, 107; listeyi daha önce Ihsan Ramadan bulmuş), katsayılar toplamı 76 = 19×4, Σ(n+a) = 1444 =
  19×19×4. (722/722, hücreler, bant ve İki El yok.)
- **Submitters Perspective, Mayıs 1994, s. 2, Abdullah Arik, "By the Even and the Odd: New Overwhelming Proof":** n+a
  toplamlarının 57'si çift, 57'si tek; çiftlerin toplamı 6234 (ayet sayısı), teklerin 6555 (sûre numaraları toplamı);
  89:3 alıntısıyla. Yani terazi 2009'dan 15 yıl önce, 19 topluluğunun içinde yayımlanmış.

Bu, merdivendeki "çalışmanın kendi bulgusu" tanımını daraltır: aile büyüklüğü (12 ≥ 2E) ve Σq = 1444 de önceki
literatürdür. **Keşif basamağı "E1/E3 − aile":** E1 ve E3'ün aynı kitapları, aynı tablolar; terazi düğümüne ek olarak
F-boy (k≥2E, k=m) ve F-toplam (m²|Σq, Σq kare, 114|Σq) düğümleri de herkes için kapalı (simetrik). Yalnız raporlanır;
merdiven önerisinde "ön-kayıt dışı, köken bulgusundan sonra tanımlandı" diye etiketlenir.
