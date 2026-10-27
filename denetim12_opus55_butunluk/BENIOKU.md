# denetim12 — on ikinci denetim: bütünlük ölçümü (26–27 Ekim 2026)

On birinci denetimin çekincesi ("test 'rastgele kitaplar Kur'an'a ne kadar benziyor?' sorusunu ölçer, 'her kitap kendi en iyi bütünlüğünü kursa' sorusunu ölçmez") ve araştırmacının ana ölçütü (zorunlu olmayan aynı unsurların katmanlar arasında yeniden belirmesi) üzerine, ayrı bir klasörde ve ayrı hafızayla çalışan bir Claude Opus 5.5 oturumu (en yüksek çaba) şunu ölçtü: aynı ikili bölmenin her düzeyde dengeli olması, modülün tanımı dışında geri dönmesi, iki ayrık el ve iki düzeyde ardışıklık, her rastgele kitap kendi en iyi merceğini seçtiğinde ne kadar nadirdir? Dil, Kur'an'a özgü sabit içermeyen 20 yüklemden oluşur; mercek 14 doğrusal biçim × 29 asaldan seçilir. Görev tanımı `GOREV.md`. Oturum depoyu yalnız okudu. Ana hattın değerlendirmesi `KESIF.md` dürüstlük notu 25'tedir.

**Sonuç (ayrıntı `BUTUNLUK_RAPORU.md`, İngilizce özet sonunda):**
- Kur'an'ın en iyi merceği her sıfırda ve her değişkede (n + a, 19). 20 yüklemin 15'ini sağlar; kanıtlı gerektirmeler ayıklanınca sayım puanı 11.
- Sıra, bilgi puanında (birincil; N = 10⁸ sınama kitabı): M20 22 isabet → 2,3×10⁻⁷; Mallows 46 → 4,7×10⁻⁷; tekdüze 3 → 4,0×10⁻⁸. Çözülmemiş aramalardan doğan belirsizlik aralığının üst ucu ~1,6–1,8×10⁻⁴'tür; bu bir sıklık kestirimi değildir.
- Sıra, sayım puanında (N = 2×10⁸): M20 59 → 3,0×10⁻⁷; Mallows 51 → 2,6×10⁻⁷; tekdüze 89 → 4,5×10⁻⁷. Tekdüzenin tepesi sıfır toplamlı yozlaşmış ailelerdir; sonradan tanımlanan S0 bunları ayıklayınca 3×10⁻⁸.
- Eski literatür verildiğinde, yani O' = 12 üyeli aile, 1444, 722/722, iki 361 hücresi ve terazi verildiğinde, bu çalışmanın eklediği katmanların sırası tekdüzede 1,0×10⁻³, M20'de 3,3×10⁻³, Mallows'ta 3,4×10⁻³. Terazi de kalan katman sayılırsa (O) sıra 5×10⁻⁶ ile 5×10⁻⁵ arasındadır.
- Bölme menüsü: her kitaba 20 bölme çiftinden birini seçme hakkı verilince sıra yalnız ~2 kat değişir. t-paritesi herkesin menüsünden çıkarılınca Kur'an'ın sırası 1,6–2,7×10⁻⁴'e çıkar; yapı t = n + a'nın paritesi üzerine kuruludur.
- Ana hattın önceden kaydettiği tahmin **kısmen tuttu**. Sıralı sıfırlarda sonuç aralıktadır, ama 10⁻⁸ merkezinin 1,4–1,7 mertebe üstündedir. Tekdüzede tutmadı: sonuç tahminden daha az nadirdir. O' ile tuttu; O ile sonuç tahminden daha nadirdir.

**Ön-kayıt:** `ONKAYIT.md`, hiçbir rastgele kitabın bütünlük puanına bakılmadan 2026-10-26T17:13:24Z'de donduruldu (SHA-256 `d213dc89332884f3973b4b2e5b33afaa2070748b95dda0d344f553eeefcfd513`, `sonuc/onkayit_ozet.txt`). Sapmalar `ek/` altında zaman damgalıdır (10 ek).

## Ana hattın kontrolleri (`dogrulama/`)

Puanlayıcı ve menü puanlayıcısı yalnız `ONKAYIT.md` metninden yazıldı; oturumun `dil.py` ve C kodu okunmadı.

| Kontrol | Sonuç |
|---|---|
| Kur'an'ın profili ve puanı (`bagimsiz.py`, `menu_kontrol.py`) | 11 @ (1,+1,19), aynı 15 yüklem; ikinci en iyi mercek (1,−1,19) 6; dört kapıda 11. Menüde 11 ((t2, n2)) ve t2'siz 9 ((n2, a2)) (`sonuc_kuran_profili.txt`, `sonuc_menu.txt`) |
| Oturumun puanlarıyla karşılaştırma | Sayımda Kur'an'a ulaşan bütün kitaplar (yedi sıfırda 255) ve 1.300 rastgele aday, dört kapı ve S0: 0 uyuşmazlık. Menüde 110 kitap: 0 uyuşmazlık |
| Sıra sayılarının aday dosyalarından yeniden sayımı (`kontrol_k.py`, `menu_kontrol.py`) | Yedi sıfırın hepsinde k_alt / k_nokta / k_üst (adanmış düzeltmeden önce ve sonra), sayım k ve S0 ≥ 11 birebir aynı (`sonuc_sira_yeniden_sayim.txt`). Menü (d2)–(d4) birebir aynı |
| Kendi tekdüze örnekleyicimiz (`ib_iw.c`, iki tohum, 2×10⁸ kitap) | Kur'an'ın bilgi puanı bileşenleri: I_B 11,64 bit (oturum 11,63), I_W 7,97 ± 0,03 bit (oturum 8,02), P(k_F = 12) 0,01016 (oturum 0,01015) (`sonuc_tekduze_bilesenler.txt`). Aile-içi bileşen (~10¹⁰ kitap gerekir) ve sıralı sıfırların bileşenleri yeniden ölçülmedi |
| Kur'an'ın merceğinde en yakın rastgele kitaplar (`yakinlik.py`; yalnız aday dosyaları) | M20 ve Mallows'ta 15 yüklemin 13'ünü taşıyan ikişer kitap var; hiçbirinde 15'i birden yok (`sonuc_kuran_mercegi_yakinlik.txt`) |

**Ana hattın eklediği bulgu (raporda yok).** Raporun §6.2'sindeki "hiçbirinde merkez 19 (Qc) ve terazi (B2) yoktur" cümlesi yalnız sayım puanında Kur'an'a ulaşan (S0 ≥ 11) 17 M20 kitabı içindir. Bütün aday dosyaları Kur'an'ın merceğinde tarandı; bu dosyalar sayım puanı ≥ 9 olan ya da bilgi eşiğine 2 bitten yakın kitaplardır. Sonuç:
- **M20 R b017:1648552:** hücreler aynen 20/18/19/19, katsayılar aynen 5,5,5,6,6,6,6,6,7,7,8,9. Terazi, iki elin ayrıklığı ve 19 | Σa(U) var. Eksik: 57/57 ve |U| = 19 (13/15).
- **M20 T b008:3677272:** 57/57, terazi ve merkez 19 var. Eksik: |U| = 19 ve 19 | Σa(U) (13/15; bilgi puanı 20,09 bit).
- **Mallows T b004:4452815 ve T b007:913292:** 57/57, merkez 19 ve birleşim 19 var. Eksik: terazi ve 19 | Σa(U) (13/15; 14,67 bit).

Merkez 19 ve terazi dahil, Kur'an'ın parçalarının her biri rastgele sıralı kitaplarda da görülür; 2×10⁸ kitapta görülmeyen, hepsinin bir arada olmasıdır. İskelet de, yani aynı mercekte 12 üyeli 3/3/3/3 aile ve b,b / b±1 satır deseni, M20 aday dosyalarında en az 435 kitapta vardır (≥ 1/460.000). Aday dosyaları yalnız yüksek puanlı kitapları tuttuğu için gerçek sıklık bundan yüksektir. Dil Kur'an'ın katmanlarını adlandırmak için yazıldığından "15'i birden" kısmen tanım gereği Kur'an'a hastır. Adil ölçü bu yüzden yukarıdaki sıralardır: her kitap kendi en iyi merceğinde puanlanır. Bu sıralar da yüklem seçimini bedellendirmez; bunu yapacak plasebo kalibrasyonu ana hattın sıradaki adımıdır ve o yapılana kadar sıralar geçicidir (`KESIF.md` not 25).

## Bu kopya

Bu klasör, oturumun nihai paketinin açılmış hâlidir: `butunluk_paket.tar.gz`, 68,5 MB, SHA-256 `0b51ca2d924a9784344a1c8a50e1cabd020b32a8d68beb744363235c2a762f42`; 806 dosya, açılmış ~234 MB.

**Çıkarılanlar** (378 dosya, ~245 MB; yol, boyut, SHA-256 ve gerekçe `atlananlar.tsv`'de):
- ikili aday ve histogram dosyaları (`tab/*/b*.aday`, `*.hist`);
- referans olasılık tabloları (`tab/*.ref`, `tab/*.tab`);
- adanmış kestirimin kitap dosyaları (`sonuc/belirsiz/*.bin`);
- bütün adayların S0, sınıf ve menü dökümleri;
- iş kuyruğu işaretleri;
- paketin kendisi.

`paket/PAKET_ICERIGI.sha256` bütün dosyaların özetlerini taşır. Çıkarılan dosyalar aynı tohumlardan yeniden üretilir (rapor §11). Paketin kendisi araştırmacının makinesindedir. `dogrulama/` betikleri aday dosyalarını okuduğu için paketin tam ve açılmış hâlini ister (`BUTUNLUK` ortam değişkeni).

**Yol genellemesi:** `src/dil.py`'deki depo yolu `os.path.expanduser('~/Desktop/hrf2')` olarak genelleştirildi. `sonuc/belirsiz/*.tsv`'deki oturum scratchpad yolları da `<oturum-scratchpad>/` olarak genelleştirildi. Bu altı dosyanın özeti bu yüzden `PAKET_ICERIGI.sha256`'dakinden farklıdır.

**Pakette olmayanlar:** `girdi/` pakete alınmadı. İçeriği dış çalışmanın arşivleri ile ana hattın betimsel motif tablosudur; özetleri `sonuc/girdi_sha256.txt`'tedir, dış arşivler `denetim11_dis_katman19/BENIOKU.md`'de listelidir.

**Yeniden üretim:** rapor §11.
