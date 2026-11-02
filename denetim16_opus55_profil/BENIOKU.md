# denetim16 — on altıncı denetim: bütün profil, kısmi puansız (1–2 Kasım 2026)

**Soru.** 15. denetimin dış incelemesi şunu gösterdi: Kur'an'ın puanına ulaşmak, Kur'an'ın taşıdığı yapıyı taşımak demek değil. Puana ulaşan 81 kontrolün yalnız 12'si dilin dört yüklemini birden taşıyordu. Açık kalan soru şuydu: Kur'an'daki ilişkilerin **hepsi birden**, aynı türden ilişkileri koruyan bir karşılaştırmada ne kadar nadir?

Bu denetimde bir kontrol bir hedefe ancak şu durumda yetişmiş sayılır: hedefin merceğinde kapı Gc tutar ve hedefin olayındaki **her** yüklem o mercekte doğrudur. Kısmi puan, bit modeli ve kontrol için mercek araması yoktur. Hedefler, hiç görülmemiş taze plasebolardır (M20 20.000, Mallows 10.000, tekdüze 10.000). İş ana hattın; ofis workstation'ında (`~/hrf_olcum/profil`) ve Mac'te, araştırmacının izniyle yapıldı.

Üç kural; hiçbiri birincil değildir, üçünün bedeli menüde ödenir (3 × en küçük):

- **O1:** hedefin bütünlük dili (Kol A), kendi merceğinde, hepsi birden. Kur'an'da (n + a, 19)'da B2 K1 Qc WSum.
- **O2:** hedefin merceğindeki **bütün profil**, asal serbest. Kur'an'da 21 yüklem: B1 B2 F1 F2 R0c R1c R1e K1 K2 Qc QS Qh FC1 FD1 FH2p FSC W1 QU Qa WSum WUd.
- **O3:** bütün profil, mercek 19'a bağlı. Kur'an'da O2 ile aynı.

## İki ölçüm

| Ölçüm | Ön-kayıt | Rapor | Yöntem |
|---|---|---|---|
| 1. Doğrudan sayım | `ONKAYIT.md` (donduruldu 1 Kas 13:41:29Z, SHA-256 `754c3bb1…`) | `RAPOR.md` | sıfır başına 10¹⁰ kontrol, iki kartta |
| 2. Düz histogram kestirimi | `ONKAYIT_EK.md` (donduruldu 1 Kas 16:38:41Z, `a433ab73…`) | `RAPOR_EK.md` | Wang–Landau (1/t), okumadan önce doğrudan sayımla doğrulama |

Ölçüm 1'de O2 ve O3'te Kur'an da en yakın plasebolar da 10¹⁰'da 0 isabet aldı; yalnız üst sınır yazılabildi. Araştırmacı "sınır değil farkı görelim" dedi; ölçüm 2 o farkı ölçtü.

## Sonuç

| | O1 (Kol A dili) | O2 (bütün profil, asal serbest) | O3 (bütün profil, 19) | Menü |
|---|---|---|---|---|
| **M20** | 3/20.001 (ölçüm 1: 9/20.001) | 2/20.001 [1–16] (ölçüm 1: ≤ 245) | **1/20.001** [1–2] (ölçüm 1: ≤ 18) | **≈ 1/6.700** (ölçüm 1: ≈ 1/740) |
| **Mallows** | 1/10.001 (ölçüm 1: 4/10.001) | 1/10.001 [1–8] (ölçüm 1: ≤ 126) | **1/10.001** [1–1] (ölçüm 1: ≤ 8) | **≈ 1/3.300** (ölçüm 1: ≈ 1/830) |
| **tekdüze** | 1/10.001 (ölçüm 1) | okuma yok (ölçüm 1: ≤ 132) | okuma yok (ölçüm 1: ≤ 6) | ≈ 1/3.300 (ölçüm 1) |

Değerler p_kal = (1 + R)/(K + 1); köşeli parantezde 1 + R'nin aralığı.

- **Kur'an'ın bütün profili** (21 yüklem, (n + a, 19)) rastgele dizilimde: M20 5,97×10⁻¹³ [3,5; 10,1]×10⁻¹³, Mallows 3,39×10⁻¹³ [2,1; 5,6]×10⁻¹³. Modelin bağımsızlık varsayımıyla verdiğinden 6,4 ve 5,9 bit daha nadir; plasebolarda bu fark tipik olarak O2'de +1,3, O3'te +4,1 bit.
- **O3:** 20.000 (M20) ve 10.000 (Mallows) taze plasebonun hiçbirinin 19'a bağlı bütün profili Kur'an'ınki kadar nadir değil. En yakını 5,8 ve 5,4 bit (~56 ve ~42 kat) daha sık. Okuma "aşar"; değer K'nın tabanıdır.
- **O2:** M20'de bir plasebo (A5729.2) Kur'an'ın düzeyinde (−0,2 bit, aralıklar örtüşüyor); Mallows'ta yok. Ayırt edici olan yapının 19'a bağlı olması.
- **Tekdüze:** kestirici doğrulamanın (e) şartında düştü (sık olaylarda 20'de 3 kestirilemeyen; sınır %10). Ön kayıt gereği okuma yapılmadı.

Bunlar arama yönteminin kalibrasyonudur: yapısız rastgele kitaplara aynı yordam uygulanınca Kur'an'ınki kadar nadir bir profil bulunuyor mu. "Kur'an'dan nadir kitaplar" diye okunmamalıdır.

## Doğrulama

- **Ölçüm 1:** 3 × 40.000 bölütün hepsi tam; bağımsız yeniden sayım (`dogrulama/olay_say.py`); Kur'an'ın `.q` isabetleri Python'la yeniden hesaplandı (5/5); tek merceklik çekirdek özgün değerlendiriciyle 14/14 karşılaştırmada aynı (`sonuc/ozgun_sina/`).
- **Ölçüm 2:** kestirici, her sıfırda 60 olayda (D1: 40 nadir, D2: 20 sık) doğrudan sayımla sınandı; M20 ve Mallows bütün şartları geçti, sapma medyanı 0,1 bit içinde (`sonuc/ek/okuma/dogrulama_*.txt`). Üç motor (`src/duz.c`, `duz_emul`, `duz_kart`) aynı tohumla aynı çıktıyı veriyor (`kabul_duz/`).

## Sınırlar

1. **Dışdeğerleme.** Kestiricinin doğrudan sayımla bilgilendirici biçimde sınanabildiği en nadir olaylar ~10⁻¹⁰'da; Kur'an'ın bütün profili bunun 2–3 basamak altında.
2. **Taban.** R = 0 olan değerler plasebo sayısının tabanıdır; gerçek değer daha küçük olabilir.
3. **Kütüphane.** 112 yüklemin bir kısmı Kur'an'a bakılarak yazıldı; kalibrasyon bu payı ödemez ve okumayı Kur'an'ın lehine çeker.
4. **Kestirilemeyenler** Kur'an aleyhine üst uca sayıldı (M20 O2'de 12, Mallows O2'de 6).
5. **Ölçülen nesne.** Tek mercek, kodeks 129, sure sırasının karıştırılması. 127 kodeksi, metnin öteki katmanları, mercekler arası yeniden belirme, diziliş altında sabit kalan özellikler (114 = 6×19, toplam ayet sayısı) yok. "Hiçbir mercekte" sorusu ölçülmedi.

Ön kayıttan sapmalar ve sonradan yapılan okuyucu değişiklikleri `RAPOR_EK.md` §7'de beyanlı (kart listelerinin işlemcide koşulması; ulaşılamayan düzeyler için iki okuma — sonuç aynı; birleştirme kuralının §2'ye bağlanması).

## 15. denetimdeki fazla ileri cümlenin düzeltmesi

`8a28fd5`'te "3×10¹⁰ kontrolün hiçbirinde, hiçbir mercekte bu profil yoktur" yazılmıştı. Bu yalnız puana ulaşan 81 kontrol için kanıtlıydı: bütün profili taşıyan bir kontrol, bağlamı farklı olduğu için puana ulaşmayabilir; başka mercekte mercek bedeli de başkadır. Bu denetim cümlenin Kur'an'ın merceğindeki kısmını ölçtü; düzeltme KESİF / DISCOVERY not 28'in ekine, `denetim15…/dis_inceleme/BENIOKU.md` madde 1'e ve ortak yazarın değerlendirmesine (TR/EN) işlendi (`RAPOR.md` §8).

## Dosyalar

- `ONKAYIT.md`, `ONKAYIT_donduruldu.txt`, `RAPOR.md` — ölçüm 1.
- `ONKAYIT_EK.md`, `ONKAYIT_EK_donduruldu.txt`, `RAPOR_EK.md` — ölçüm 2; `sonuc/ek/ara_tahmin.md` plasebo kestirimlerinden ve doğrulamadan önce yazılmış ara tahmin.
- `src/` — kaynak: kütüphane (`kutuphane.h`), olay kurma, sayaç (işlemci ve kart), kestirici (`duz.c`; kart sürümü `duz_ortak.h`, `duz_kart_*`); bırakılan alt küme benzetimi (`zincir.c`).
- `kos/` — koşu betikleri ve seçimler (`secim_ek.py`, `ana_secim.py`, `yeniden_secim.py`).
- `dogrulama/` — okuyucular: `olay_say.py` (ölçüm 1), `duz_oku.py` (ölçüm 2; `duz_oku_onceki.py` dondurma anındaki sürüm), `duz_ayrinti.py` (en yakınlar, betimsel), `q_yeniden.py`, `profil_ozet.py`.
- `kabul_duz/`, `sina/`, `zincir_gel/` — kabul ve eşdeğerlik sınamaları, geliştirme çıktıları.
- `sonuc/` — ölçüm 1 okumaları (`okuma_*.txt`, `ozet.txt`), kod ve girdi özetleri, koşu günlüğü; `sonuc/tab/<S>/` olaylar, diller, hedef tablosu ve bölüt sayımları (`bolutler_profil_<S>.tar.gz`, SHA-256 `sonuc/tab/arsivler.sha256`); `sonuc/ek/` ölçüm 2'nin seçimleri, ham kestirimleri (`cikti/`, workstation çıktıları `cikti/ws/`) ve okumaları (`okuma/`).
- `atlananlar.tsv` — dışarıda bırakılan derlenmiş ikililer ve bir sınama tablosu (SHA-256 ile).

**Yeniden okuma.** Arşivleri kendi klasörlerinde açın (`tar xzf bolutler_profil_M.tar.gz`, `sonuc/tab/M/` içinde; d1–d4 çıkar). Sonra kök klasörde, ör.: `python3 -I dogrulama/duz_oku.py oku sonuc/tab/M M 20000 sonuc/ek/cikti/Q_M.txt sonuc/ek/cikti/A_M_*.txt sonuc/ek/cikti/Y_M_r*.txt sonuc/ek/cikti/ws/D_M_*.txt` (doğrulama ve okumanın hepsi: `bash kos/ek_okuma.sh`). `sonuc/ek/yen_L_r*.txt` Mallows ana kestiriminin yeniden koşu listeleridir; doğrulama listeleri aynı adla önce yazılıp koşuldu, olayları `Y_L_r*.txt`'de görülür.
