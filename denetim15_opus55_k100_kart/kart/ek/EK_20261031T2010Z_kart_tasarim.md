# Ek — ekran kartı sayacı: tasarım ve kabul ölçütü (31 Ekim 2026, 20:10Z; kart üzerinde hiçbir sınama koşulmadan önce yazıldı)

**Onay:** Araştırmacı ~20:00Z'de onay verdi: "onay veriyorum yazalım; kod biterse ve taşıma uygunsa taşıyalım". Kartlar: workstation üzerindeki RTX 4090 ve RTX 3090, CUDA 12.4.

**Kısıt:** 12 iş parçacığı sınırı sürüyor. Kartı süren işlemci iş parçacığı da bu sınıra sayılır. İşlemci derinleştirmesi 12 iş parçacığını kullanırken kartta derleme ya da sınama yapılmaz.

## 1. Ne taşındı

Ölçüm tanımlarının hiçbiri değişmedi. Kart sayacı (`src/kart_*`), iki özgün sayacın birebir eşdeğeri olarak yazıldı:

- **Kip 0:** `kontrol_b`'nin kol 1–3 yolu (plasebo dilleri).
- **Kip 1:** `kontrol_h` (Kur'an dilleri; B2 kısayolu).

Özgün başlıklar (`kutuphane.h`, `sayim.h`, `tablo.h`, `dtab.h`, `dtab_b.h`, `agac_uretim.h`) değiştirilmeden ana programa içerilir. Dosya özetleri, k10 ve kapı çalışmasındakilerle aynıdır.

Ana program (`kart_ana.c`) şunları özgün `main()` gövdesinin adımlarıyla kurar: referans tabloları, dil tabloları, eşikler ve `esikler.txt`. Yeniden yazılan yalnız kontrol başına yapılan hesaptır (`kart_ortak.h`):

- **RNG ve örnekleyiciler (M20, Mallows, tekdüze):** Aynı tamsayı işlemleri, aynı sıra, aynı tohumlar (tür 2). Mallows'un takma tabloları işlemcide bir kez kurulur, böylece özgündeki küresel `REF_DESC` yarışı burada yoktur. Kartta bir bölütün örneklemesi tek iş parçacığında sırayla yürür.
- **Profil:** `kdegerlendir`, mercek (biçim, asal) başına ayrıldı.
  - Aile istatistikleri int32 tutulur; en büyük mutlak değer 136.800'dür.
  - Bant yüklemleri (K2, HK2) için q/r değerleri dizide saklanmaz. Açıklık 2 ile k arasındaysa ikinci geçişte 128 bitlik maskeye yazılır; tanım aynıdır.
- **Puan:** `puan_B` budamasız hesaplanır. Budama kesindir: bir merceğin değeri o merceğin üst sınırını aşamaz. Bu yüzden en büyük değer de, eşitlikte seçilen en küçük mercek de aynı çıkar. Toplama sırası (B, Z, F, W, H; float → double) ve eşik ifadeleri (`DESIK − 1e-6`, `DESIK − 2.0`) aynıdır. Kart kodu `--fmad=false` ile derlenir.
- **Kayıtlar (`.q`, `.s`):** Ortak tampona yazılır. Ana programda (bölüt, sıra, dil) sırasına dizilir ve `ADAYMAX` sınırı özgündeki gibi uygulanır.
- **Çıktı biçimi:** Özgünle aynıdır. Tek fark, `sure_s` alanının partinin duvar süresinin bölüt başına payı olmasıdır. Kartın kimliği `kart_gunlugu.txt`'e yazılır.

## 2. Mac'te yapılan ön sınama (kartsız; öykünme arka ucu `kart_emul.c`)

Aynı çekirdek, işlemcide sırayla koşturuldu ve özgün sayaçlarla aynı girdilerde karşılaştırıldı. Bütün dosyalar `sure_s` dışında bayt bayt aynı çıktı:

| Sıfır | Kip | Dil | Bölüt | Kontrol | Sonuç |
|---|---|---|---|---|---|
| M20 | 0 | 3 (A535, A1645, A690) | 1–2 | 5.000'er | aynı |
| M20 | 0 | 1.988 (k10 düzey 1 listesi) | 3 | 5.000 | aynı (toplam k 75.077, kB 361.915) |
| Mallows | 0 | 53 | 7–8 | 5.000'er | aynı |
| tekdüze | 0 | 50 | 7–8 | 5.000'er | aynı |
| M20, Mallows, tekdüze | 1 | Kur'an dilleri | 1–3 | 5.000'er | aynı |
| M20 | 1 | Kur'an dilleri | 1–3 ve 5 | 250.000'er | aynı (`.s` 3/4/11 kayıt, `.q` 1 kayıt) |

Mac çıktısı, M20 bölüt 5'te iş istasyonunun aynı bölüt için yazdığı özgün dosyalarla da aynı çıktı (`.txt`, `.q`, `.s`).

## 3. Kart üzerinde kabul ölçütü (önceden yazıldı)

Kart sayacı, iş istasyonunda işlemci sayaçlarının zaten yazdığı bölüt dosyalarıyla karşılaştırılır (`kos/is_sina.sh`):

| Ad | Kip | Sıfır | Diller | Bölüt | Karşılaştırılan |
|---|---|---|---|---|---|
| M_b_d1 | 0 | M20 | k10 düzey 1 (1.988 dil) | 1–4 | `k10/tab/M/kA/d1_0` |
| M_b_d4 | 0 | M20 | derinleştirme (3 dil) | 401–424 | `k10/tab/M/kA/d4_0` |
| L_b_d2 | 0 | Mallows | düzey 2 | 5–28 | `k10/tab/L/kA/d2_0` |
| U_b_d2 | 0 | tekdüze | düzey 2 | 5–28 | `k10/tab/U/kA/d2_0` |
| M_h_Gc | 1 | M20 | Kur'an dilleri | 1–24 | `kapi/kuran/Gc/M` |
| U_h_Gc | 1 | tekdüze | Kur'an dilleri | 1–24 | `kapi/kuran/Gc/U` |
| L_h_Gc | 1 | Mallows | Kur'an dilleri | 1–24 | `yaris/kuran/Gc/L` (yarış düzeltmeli) |

**Kabul:** Bütün `.txt` dosyaları `sure_s` dışında, bütün `.q` ve `.s` dosyaları tam olarak bayt bayt aynı olmalı. Tek bir fark varsa kart sayacı kullanılmaz, fark bulunup giderilir ve sınama baştan yapılır.

Karşılaştırma yalnız isabet sayılarına (k ≈ 0) dayanmaz. kB sayaçları bölüt başına binlerce olaydır; `b2` sayısı ve `.s` kayıtları da örnekleyici akışının parmak izidir. Bu, Mallows yarışından alınan derstir.

Her iki kart (sm_86, sm_89) ayrı ayrı sınanır.

## 4. Sonra

Kabul edilirse, kartla yapılacak her yeni sayım ayrı bir ön-kayıtla başlar. Bu ek yalnız aracı kabul eder; hiçbir ölçümün kuralını değiştirmez.
