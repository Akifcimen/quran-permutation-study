# Kapı çalışması — ön-kayıt (ana hat, 29 Ekim 2026)

## 1. Neden

plasebo14'ün birincil kolunda (Kol A) bütünlük şartı üç parçadır: kapı G, ≥ 3 motif türü ve ≥ 3 düzey. Kapı G iki şart koyar: aile boyu beklenenin iki katı ya da fazlası (k_F ≥ 2E, yani k_F·p ≥ 228) ve dört F hücresinin her biri ≥ 2 üye. Kur'an bu kapıyı tam sınırda sağlar: 12·19 = 228. Kapıyı 406 mercekten yalnız birinde sağlar.

On ikinci denetimin ön-kaydı (§2.8) 2E kapısını tam da bu yüzden birincil bilgi puanından çıkarmıştı: "Kur'an'ın aile boyuna tam denk düştüğü için". Ana hat plasebo14'ün görevini yazarken bu kapıyı bütünlük kuralının içinde bıraktı. Bu bir tasarım hatasıdır. Kapının etkisi büyüktür:
- plasebo12'de Kur'an'ın çekirdek dilinde Kur'an'a ulaşan 33 kontrolün 33'ü de kapı G'yi sağlamayan merceklerdedir. Ana hat bunu bölüt aday dosyalarından saydı.
- plasebo14'te bütünlüklü dil kuramayan plaseboların çoğunda hiç kapı-G merceği yoktur: 138/165, 73/82, 72/85.
- Araştırmacı düzeltmeyi 29 Ekim'de onayladı ("önerdiğin gibi yapalım"). İş ana hatta, workstation'da yapılır.

Bu çalışma plasebo14 Kol A'yı Kur'an'ın değerine bağlı olmayan kapılarla tekrarlar. Kapılar on ikinci denetimin ön-kaydındaki duyarlılık kapılarıdır (§2.8: GE, Gc, G0); yeni tanım değildir.

## 2. Kapılar

| Ad | Tanım | `kutuphane.h` parametresi | Durum |
|---|---|---|---|
| **Gc** | dört hücrenin her biri ≥ 2 üye; aile boyu şartı yok | KAPI_ESIK 0, KAPI_HUCRE 2 | **birincil** |
| GE | k_F ≥ E (k_F·p ≥ 114) ve her hücre ≥ 2 | 114, 2 | duyarlılık |
| G0 | kapı yok | 0, 0 | duyarlılık |
| G2E | plasebo14'ün kapısı (k_F·p ≥ 228, her hücre ≥ 2) | 228, 2 | başvuru (plasebo14 sonucu) |

Gc'nin birincil seçilme gerekçesi: yalnız yozlaşmayı (boş ya da tek üyeli hücre) dışlar ve Kur'an'ın aile boyuna bağlı değildir. Kur'an'ın hücreleri 3/3/3/3'tür, kapıyı payla geçer. Gc'nin birincil olduğu, sonuçlar görülmeden önce burada sabitlenir. Üç kapı arasından sonradan "en iyisini" seçmek yapılmaz; üçü de raporlanır.

## 3. Değişmeyenler

Kapı dışında her şey plasebo14 Kol A'nın aynısıdır:
- Kütüphane L* (109 yüklem) ve bütünlük kısıtlı D_B: ≥ 3 tür, ≥ 3 düzey; dil bedeli ve mercek bedeli P0 plasebo12'deki gibi.
- Zorunlu bütünlük: her hedef sınıfı sağlayan en iyi dilini alır. Hiç yoksa dilsizdir ve p = 1 alır.
- Hedefler: aynı `hedef.bin` dosyaları; Kur'an ve 200 M20, 100 tekdüze, 100 Mallows plasebo.
- Kontroller: aynı tohumlar (tür 2). Derinlik kuralı aynı: k(1–4) ≥ 10 ise dur, k(1–40) ≥ 10 ise dur, değilse bölüt 1–400; Kur'an dilleri her zaman 1–400.
- Kontrol puanı: kontrolün kendi en iyi merceğinde, aynı kapıyla bütünlüklü puan.
- p_x = (k+1)/(N+1); %95 Clopper–Pearson aralığı. R, p'si Kur'an'ınkinden büyük olmayan ya da aralığı Kur'an'ınkiyle çakışan plasebo sayısıdır. p_kal = (1+R)/(K+1).

Kod: plasebo14'ün donmuş kaynakları üç değişiklikle kullanılır (özetler `sonuc/kod_sha256.txt`):
1. `kutuphane.h`'de `kapiG` iki derleme parametresine bağlandı. Varsayılan (228, 2) plasebo14'tür.
2. `kontrol_b.c`'de iş parçacığı tavanı 4'ten 24'e çıktı.
3. `kos/kontrol_b.py`'de referans yolu ve iş parçacığı sayısı ortam değişkenlerine bağlandı.

Akış `kos/kapi_akis.sh`'dir: derleme, dil kurma, kontrol kümesi ve düzey 1–3. Derleyici gcc 10.5 (x86-64), `-O3 -std=gnu11 -w`.

**Ö0 (dondurmadan önce):** G2E ile Linux'ta derlenen hat plasebo14'ü birebir verdi:
- `diller_A.txt`, `ayrinti_A.txt` ve `hedef_ayrinti_A.txt`, üç sıfırda.
- M20 Kol A'nın 36 dilinin bölüt 1–4 sayımları (FARK 0) ve eşikler.

## 4. Sıra ve kaynak

Gc: M20 → tekdüze → Mallows. Sonra GE aynı sırayla, sonra G0 aynı sırayla. Workstation (`~/hrf_olcum/kapi/<kapı>/`), `nice -n 10`, iş parçacığı en çok 12. Kademeli ölçümün 10¹⁰ kademesi sürerken ilk saatte 6 iş parçacığı kullanılır. Tahmini süre kapı başına 1,5–3 saat. Süre biterse eksik kalan kapı-sıfır çiftleri raporda "koşulmadı" diye yazılır; sıra değişmez.

## 5. Okuma kuralı (kapı ve sıfır başına; plasebo14 ile aynı)

- **R = 0: "aşar".** Kur'an'ın bütünlüklü ayrışması bu kapıyla bütün plaseboları aşar.
- **R/K ≥ 0,05: "sık".**
- **Arası: "arada".** Kalibre p'siyle yazılır.

Birincil karar Gc'de, sıralı sıfırlarda (M20 ve Mallows) verilir:
- **Gc iki sıralı sıfırda da "aşar":** plasebo14'ün sonucu kapı G'nin yerleşimine bağlı değildir. Bütünlük okuması README'nin ana okuması olur. Çekince "kapı Kur'an'ın sınırında, ama sonuç kapıdan bağımsız" diye yazılır; GE ve G0 yanında verilir.
- **Gc'de en az bir sıralı sıfırda "arada":** bütünlük ayrışması kalibre p'siyle yazılır. Ana okuma, plasebo12'nin çekirdek okumasıdır ("sırayı üç yüklemlik çekirdek taşır"). plasebo14'ün "aşar"ı kapı G'ye bağlı diye yazılır.
- **Gc'de "sık":** plasebo14'ün sonucu büyük ölçüde kapı G'nin yerleşiminden gelir. README bunu açıkça yazar.

## 6. Kur'an'ın kendi sırası

- **Kısayolla derin sayım:** Gc'de Kur'an'ın Kol A dili yine B2'yi gerektiriyorsa, `kontrol_h` Gc ile derlenir. Kur'an'ın sırası `~/opus5.5/kademe` ön-kaydının kademe ve durma kuralıyla 10⁹ ve 10¹⁰'a, gerekirse 10¹¹'e inilerek ölçülür. B2'yi gerektirmiyorsa kısayol kullanılmaz; Kur'an'ın sırası 10⁸ ile kalır.
- **Betimsel sayım:** plasebo14'ün donmuş Kur'an dili ({B2, K1, Qc, WSum} @ (1,+1,19)) Gc, GE ve G0 kapılarıyla 10⁹ kontrolde sayılır. Bu, aynı tarifin nadirliğinin kapıya ne kadar bağlı olduğunu gösterir. Eşik her kapıda yeniden hesaplanır.

## 7. Doğrulama

1. Ana hattın bağımsız yeniden sayımı (`dogrulama/yeniden_say_kapi.py`, plasebo14'te birebir doğrulanmış betiğin aynısı): bölüt dosyalarından k, p, R ve p_kal.
2. Kur'an'ın dili ve en iyi üç plasebonun dilleri, bağımsız Python koduyla kendi merceklerinde sınanır: kapı, yüklemler ve sınıf.
3. Kod farkının (plasebo14 → kapı çalışması) ve yeniden sayımın, sonuçları bilmeyen ayrı bir ajanla denetimi.

## 8. Ana hattın tahmini (dondurmadan önce)

| Kapı, sıfır | dilli plasebo | en küçük plasebo p | R | okuma |
|---|---|---|---|---|
| Gc, M20 | 120–200 / 200 | 10⁻⁸ – 10⁻⁶ | 0–5 | en olası "arada" (P ≈ 0,4); "aşar" 0,35; "sık" 0,25 |
| Gc, Mallows | 60–100 / 100 | 10⁻⁸ – 10⁻⁶ | 0–5 | aynı |
| GE, M20 | 60–150 / 200 | 10⁻⁷ – 10⁻⁵ | 0–2 | "aşar" ya da "arada" |
| G0, M20 | Gc'ye yakın ya da fazla | Gc'ye yakın | Gc'ye yakın | Gc'ye yakın |

Gerekçe: Gc'de neredeyse her kitabın küçük p'li merceklerinde bütün hücreleri ≥ 2 olan aileleri vardır. plasebo12'de Kur'an'a ulaşan 33 kitap bu merceklerdeydi. Kur'an'ın Kol A dilindeki birleşim yüklemi WSum (k_F + k_H = p) büyük ailelerde imkânsızdır, bu yüzden Kur'an'ın kendi sırası Gc'de G2E'ye göre 3–30 kat artar. Plasebolar kendi dillerini seçtikleri için Kur'an'a daha çok yaklaşır.

Tahmin hükmü: okuma sınıfı tutarsa "tuttu", en küçük plasebo p ve R aralıktaysa "aralıkta".

## 9. Sapmalar

Dondurmadan sonraki her değişiklik `ek/` altında zaman damgalı yazılır.
