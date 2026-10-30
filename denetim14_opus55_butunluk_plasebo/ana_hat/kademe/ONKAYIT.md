# Kademeli ölçüm — ön-kayıt (ana hat, 29 Ekim 2026)

Bu belge, plasebo14'ün ana kolunda (bütünlük zorunlu, Kol A) Kur'an'ın kendi sırasını derin kontrol sayımıyla ölçen işin ön-kaydıdır. plasebo14'te Kur'an'ın sayımı üç sıfırda da 0 / 10⁸ çıktı; p_Q yalnız üst sınırla biliniyor (%95 üst 3,7×10⁻⁸). Bu iş aynı donmuş dili, aynı eşiği, aynı tohum akışını ve aynı sayım tanımını kullanarak kontrol sayısını kademe kademe artırır. Yeni bir tanım, yeni bir dil ya da yeni bir eşik yoktur. Araştırmacı (Akif Çimen) işi 29 Ekim'de ana hatta verdi ("iş sende"); hesap ofis workstation'ında (SSH, araştırmacının hesabı) yapılır.

Bu ön-kayıt, bölüt 401 ve sonrası (henüz hiçbir yerde sayılmamış kontroller) çalıştırılmadan önce dondurulur. Dondurmadan önce yalnız plasebo14'ün zaten saydığı bölütler (1–400) eşdeğerlik sınaması için yeniden sayıldı (§3).

## 1. Ölçülen nicelik

Kur'an'ın plasebo14 Kol A dilinde, sıfır kitapları (kontroller) arasındaki kendi sırası:

p_Q = P_sıfır( S^B_y(Λ_Q) ≥ eşik_Q ),

burada S^B_y, kontrolün Λ_Q altında, kapı G'nin tuttuğu ve sınıf şartının (≥ tmin tür, ≥ dmin düzey) sağlandığı mercekler üzerindeki en iyi bütünlüklü puanıdır (bilgi − mercek bedeli; plasebo14 ONKAYIT §4–§6, `dtab_b.h` `puan_B`). Bir kontrolün hiçbir mercekte bütünlüklü olmaması "aşmaz" demektir. Sayım tanımı plasebo14 `kontrol_b.c` kol 1–3 yolunun aynısıdır.

| Sıfır | Dil | Λ_Q @ mercek | Eşik (bit) | Durum |
|---|---|---|---|---|
| M20 | A (birincil) | {B2, K1, Qc, WSum} @ (1,+1,19) | 18,801347495 | birincil |
| Tekdüze | A | aynı | 20,830033303 | ikincil |
| Mallows | A | aynı | 19,997884751 | ikincil |
| M20 | A− (≥ 2 tür, ≥ 2 düzey) | {B2, K1, Qc} | 15,946826697 | duyarlılık |
| M20 | A+ (4 tür, 4 düzey) | {B2, K1, Qh, WSum} | 18,801347495 | duyarlılık |

A− ve A+ yalnız M20'de ölçülür, çünkü plasebo14 bu iki kolu yalnız M20'de dondurdu. Kol B (bedelsiz en geniş dil) bu işe girmez; kısayol (§3) yalnız kol 1–3 için geçerlidir.

**Okuma sınırı.** p_Q, Kur'an'ın kendi bütünlüklü dilindeki sırasıdır. Dilin Kur'an'a göre yazılmasının bedeli plasebo14'ün kalibrasyonudur (Kol A: 1/201, 1/101, 1/101) ve bu iş onu değiştirmez. p_Q'nun derinleşmesi kalibre p'yi küçültmez. p_Q "Kur'an'ın rastlantı olma olasılığı" değildir. Sıfır modelleri plasebo14'tekilerdir (M20: 20 takaslı zincir, S ≤ S_obs; tekdüze; Mallows).

**Bağımlılık.** Dil ve eşik plasebo14'ün donmuş çıktısıdır. plasebo14 raporu ve ana hattın doğrulaması henüz bitmedi. Doğrulamada dil kurma ya da eşikte bir hata çıkarsa, bu işin sonuçları o düzeltmeyle yeniden okunur; bu ön-kayıt o durumda ek ile güncellenir.

## 2. Donmuş girdiler

- plasebo14 `ONKAYIT.md` SHA-256 `6080c6b17c4bf38184c11482b3931d4804633ee23d190d69d252e03926e1f748` (donma 2026-10-29T04:38:14Z).
- Diller: `girdi/<sıfır>/diller_Q.txt`, plasebo14 `tab/<sıfır>/diller_A.txt`, `diller_Am.txt`, `diller_Ap.txt` dosyalarındaki A000, Am000 ve Ap000 satırlarının aynısı. Bu dosyaların özetleri plasebo14 `sonuc/diller_sha256.txt` ile tutar. `diller_Q.txt` özetleri: M `72302edcaf79ae4f2d3b393dbe61bd6295ddf4ae75129912c01d63ff71e11365`; U ve L (aynı satır) `9b820275261ec0e18b46175389393399a9f9b650499cf8a405a1d6c76278a878`.
- Hedef dosyaları (kayıt 0 = Kur'an, kodeks 129): M `a30b1b35…7e46`, U `b6a3a79b…b3ce`, L `9a6fb27d…2bb` (plasebo14 `sonuc/hedef_sha256_p12.txt` ile aynı).
- Referans tabloları: plasebo12 `tab/<sıfır>/ref_{1111,2222,0444,0888}.ref`, 12 dosya, özetler `girdi/ref_sha256.txt` (plasebo14 `sonuc/ref_sha256_p12.txt` ile aynı; workstation kopyası `sha256sum -c` ile 12/12 OK).
- Kütüphane ve puan kodu: plasebo14'ün donmuş `kutuphane.h`, `sayim.h`, `agac_uretim.h`, `tablo.h`, `dtab.h`, `dtab_b.h` (özetler `girdi/p14_kod_sha256.txt`, plasebo14 `sonuc/onkayit_kod_sha256.txt` ile 7/7 OK; değiştirilmeden içerilir).
- Hızlı sayaç `src/kontrol_h.c` SHA-256 `33856c3bfb39fc44b87c7e8b268e73b81f29789bae093fa6bd304ee24cebba1e`; derleme `gcc -O3 -std=gnu11 -w -DDMAX=64 … -lm -lpthread` (gcc 10.5, x86-64; `-march` yok, kaynaşmış çarpma-toplama üretilmez).
- Tohumlar: plasebo14 ile aynı, `tohum(sıfır, 2, bölüt)` = 29102600000 + sıfır·10⁸ + 2·10⁷ + bölüt. Bölüt büyüklüğü 250.000; bölüt numarası 10⁷'ye kadar başka tohum türleriyle çakışmaz.

## 3. Hızlı sayaç ve eşdeğerlik

**Kısayol.** Beş dilin hepsinde B grubunun tek yüklemi B2'dir (terazi). B2 olmadan sınıf şartı sağlanamaz: A'da kitap düzeyi, A−'de kitap düzeyi, A+'da dört düzeyin biri eksik kalır. Bu şart başlangıçta her dil için programla sınanır (`kontrol_h.c`, "kısayol sınaması"). B2 biçim düzeyindedir: `kdegerlendir`'deki mb0'dan gelir ve dışlama EXB[L] onu ancak düşürür. Bu yüzden B2'nin tutmadığı biçimlerin mercekleri bu dillerde bütünlüklü olamaz. `kontrol_h` her kontrol için önce 14 biçimde B2'yi `kdegerlendir`'deki satırların aynısıyla hesaplar. Donmuş `kdegerlendir`'i yalnız B2'nin tuttuğu biçimlerde çağırır, kapı G listesini o biçimlerin merceklerinden artan L sırasıyla kurar ve donmuş `puan_B`'yi çağırır. Kontrollerin ~%0,2'sinde en az bir biçimde B2 tutar.

**Dondurmadan önce yapılan sınamalar** (yalnız plasebo14'ün saydığı bölüt 1–400):
1. plasebo14'ün özgün `kontrol_b.c`'si Linux'ta derlendi. Bölüt 1–4'te üç sıfırda Kur'an dillerinin D satırları Mac'teki plasebo14 bölütleriyle aynı (Am000 bölüt 4: kB = 2 dahil). Eşikler 9 ondalığa kadar aynı.
2. Örnekleyici akışları: `src/akis_ozet.c` (SHA-256 `74162ef1…938b`) ile bölüt 1, 400 ve 4000'in ilk 20.000 kitabının FNV özeti, B2 biçim sayısı ve Mallows q'su Mac (Apple clang, arm64) ve Linux (gcc, x86-64) arasında üç sıfırda aynı (`sonuc/akis_ozet_{mac,linux}.txt`).
3. `kontrol_h`, bölüt 1–400 × 3 sıfır = 1.200 bölütte plasebo14'ün Mac bölütleriyle karşılaştırıldı (`kos/karsilastir.sh`): her bölütte her Kur'an dilinin k'si ve kB'si aynı, **FARK 0**. Toplamlar: M20 A k 0, kB 8; A− k 0, kB 132; A+ k 0, kB 0; tekdüze A k 0, kB 6; Mallows A k 0, kB 10. Süre: 10⁸ kontrol sıfır başına 18–28 s (12 iş parçacığı).

**Betimsel (bölüt 1–400, `.s` kayıtları):** Kol A'da bütünlüklü kontrollerin en iyi puanı M20 13,43, tekdüze 11,71, Mallows 14,49 bittir; eşiğin 5,4, 9,1 ve 5,5 bit altında. Eşiğin 2 bit yakınında aday yok.

## 4. Kademeler ve durma kuralı

Kademe s'de bölüt 1 … 4·10^(s−1) sayılmış olur (N_s = 10^(s+7)):

| Kademe | Bölütler | N | Durum |
|---|---|---|---|
| S1 | 1–400 | 10⁸ | plasebo14 (k = 0, beş dilin hepsinde) |
| S2 | 401–4.000 | 10⁹ | işlemci |
| S3 | 4.001–40.000 | 10¹⁰ | işlemci |
| S4 | 40.001–400.000 | 10¹¹ | işlemci (~4–5 saat/sıfır) |
| S5 | 400.001–4.000.000 | 10¹² | yalnız S4'ten sonra k < 10 kalan diller için; işlemci (~2 gün/sıfır) ya da bit-bit doğrulanmış GPU sürümü |

- Bir kademe içinde her sıfırın bölütleri bir kez ve tam sayılır. Bir dil-sıfır çifti için kademe sonunda **birikimli k ≥ 10** ise o çift durur. Sırası ölçülmüş olur ve göreli hassasiyet kabaca ±%60'tır. k < 10 ise bir sonraki kademeye geçilir. Bir sıfırda tek bir sayım bütün dillerini birlikte yürütür; durmuş bir dil sayıma kalsa da sonucu durduğu kademedeki değeriyle raporlanır.
- S5'e geçiş yalnız bu kurala bağlıdır. Sonucun "iyi" görünüp görünmemesi kararı etkilemez. S5 zaman ya da kaynak yüzünden yapılamazsa bu açıkça yazılır ve sonuç S4'teki değerle raporlanır.
- Sıra: her kademede M20 → tekdüze → Mallows.

## 5. Kestirim ve raporlama

- Her kademe ve her dil-sıfır çifti için k, N, p̂ = k/N, plasebo14 ile uyumlu (k+1)/(N+1) ve %95 Clopper–Pearson aralığı. k = 0 ise %95 üst sınır 3,69/N.
- **M20 zincir düzeltmesi.** M20 kontrolleri 64 zincirden sırayla alınır: kontrol i, zincir i mod 64'ün 20 takas sonraki hâlidir. Aynı bölütte, aynı zincirde (sira mod 64 aynı) ve |Δsira| ≤ 96.000 olan isabetler (≤ 1.500 zincir adımı, 30.000 takas; başlangıçtaki ısınma uzunluğu) tek küme sayılır. M20 için k ve k_küme birlikte raporlanır. Aralık k_küme üzerinden hesaplanır; N değişmez, bu temkinlidir. Tekdüze ve Mallows kontrolleri bağımsızdır.
- Betimsel: bütünlüklü kontrol oranı kB/N, bütünlüklü kontrollerin puan dağılımı (`.s`) ve eşik yakınındaki adaylar (`.q`, eşik − 2 bit).

## 6. Doğrulama

1. **Her isabet ve her `.q` adayı:** kitap kayıttan okunur. Donmuş kodla tam yol (`kdegerlendir` 0x3FFF, `kapi_listesi`, `puan_B`) yeniden puanlanır ve aynı puan ile aynı mercek beklenir. Ayrıca ana hattın bağımsız Python koduyla kapı G (k_F·p ≥ 228, her hücre ≥ 2), B2, K1, Qc, Qh, WSum ve sınıf şartı (tür/düzey) yeniden sınanır.
2. **Kademe başına özgün sayaçla örneklem sınaması:** her yeni kademede dört bölüt (ilk, son, %25'teki ve %75'teki bölüt) özgün `kontrol_b` ile de sayılır; k ve kB aynı olmalı.
3. **Bütünlüklü kayıtlar:** her kademede `.s` kayıtlarından rastgele 20'si (tohum 20261029) bağımsız Python koduyla yeniden sınanır.

Herhangi bir uyuşmazlık sonuçların raporlanmasını durdurur. Neden bulunur, zaman damgalı bir ekte yazılır.

## 7. Ana hattın tahmini (dondurmadan önce)

Gerekçe: S1'de bütünlüklü kontrollerin oranı M20'de 8×10⁻⁸, en iyileri eşiğin 5,4 bit altında. Kur'an'ın puanına ulaşmak için bütünlüklü bir kontrolün K1 ile Qc'yi birlikte ve dar bir bağlamda sağlaması gerekir. Bunun bütünlüklüler içinde ~10⁻²–10⁻³ olduğunu kestiriyorum.

| Dil-sıfır | p_Q merkez | %90 aralık | S2'de (10⁹) k |
|---|---|---|---|
| M20 A | 3×10⁻¹⁰ | 10⁻¹² – 3×10⁻⁹ | 0 (P ≈ 0,7) |
| Tekdüze A | 10⁻¹¹ | 10⁻¹³ – 10⁻⁹ | 0 |
| Mallows A | 3×10⁻¹⁰ | 10⁻¹² – 3×10⁻⁹ | 0 |
| M20 A− | 10⁻⁹ | 10⁻¹¹ – 10⁻⁸ | 0–2 |
| M20 A+ | 10⁻¹¹ | 10⁻¹⁴ – 10⁻⁹ | 0 |

kB oranlarının S2'de S1'deki gibi kalacağını bekliyorum: M20 A ~80 / 10⁹ (50–120), A− ~1.300 / 10⁹. Tahmin hükmü: merkez ±1 mertebe "tuttu", aralık içinde "aralıkta", dışında "tutmadı".

## 8. Kaynak ve sapmalar

Workstation (i9-10920X, 12 çekirdek / 24 iş parçacığı, RTX 4090 + RTX 3090), `~/hrf_olcum`; işlemci en çok 12 iş parçacığı, `nice -n 10`; disk ≤ 5 GB. GPU'lar araştırmacının izniyle (29 Ekim) kullanılabilir. Yalnız S5 gerekirse, bit-bit eşdeğerliği bölüt 1–400 ve rastgele yeni bölütlerde sınanmış ayrı bir sürümle kullanılır. Dondurmadan sonra yapılan her değişiklik `ek/` altında zaman damgalı yazılır.
