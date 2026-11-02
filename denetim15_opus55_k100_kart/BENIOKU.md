# denetim15 — on beşinci denetim: on ve yüz kat plasebo, eşit derinlik (31 Ekim – 1 Kasım 2026)

**Önceki durum.** On dördüncü denetimde (`denetim14`) her hedef kendi en iyi bütünlüklü dilini kurdu: en az üç düzeye ve üç motif türüne yayılan dil. Kur'an sıralı sıfırlarda 200 (M20) ve 100 (Mallows) plasebonun hepsini aştı: kalibre p 1/201 ve 1/101. Bunlar o plasebo sayısıyla olabilecek en küçük değerlerdi, yani bir ölçüm değil, bir tabandı. Kur'an'ın kendi sırası ise 10¹⁰ kontrolle ölçülmüştü: 4,7×10⁻⁹ (M20), 2,2×10⁻⁹ (Mallows), 1,2×10⁻⁹ (tekdüze).

Bu denetim tabanın altına bakar. Dört parçadır. Hepsini ana hat yaptı, araştırmacının izniyle ofis workstation'ında. Her parça kendi ön-kaydıyla, sonuçlardan önce donduruldu.

| Parça | Klasör | Soru | Sonuç |
|---|---|---|---|
| Isınma tanısı | `isinma/` | M20 örnekleyicisinin 30.000 takaslık ısınması yetiyor mu? | **İhmal edilebilir (incelenen göstergelerde).** Yüz kat uzun ısınma b2 oranını 0,9997 [0,9971; 1,0022] değiştirir; bütünlüklü kontrolleri ±%2 içinde. Nadir kuyruk (9 / 6 / 5 isabet) için kesin sınır yok |
| K×10 | `k10/` | 2.000 / 1.000 / 1.000 plaseboyla Kur'an yine hepsini aşar mı? | **Hayır; üç sıfırda "arada".** 4/2001, 5/1001, 8/1001. Derinleştirmede (plasebolar 10⁹) 1, 1, 2 plasebo Kur'an'dan ayrılamaz |
| Kart sayacı | `kart/` | Sayaç ekran kartına bayt bayt eşdeğer taşınabilir mi? | **Evet.** Kabul sınaması sonuçlardan önce yazıldı; iki kartta 7/7 karşılaştırma, fark 0 (üç ikili sürümde). İşlemcinin 12 iş parçacığından ~45–60 kat hızlı |
| K×100, eşit derinlik | `k100/` | 20.000 / 10.000 / 10.000 plaseboyla, yakın olanlar Kur'an'la aynı derinlikte (10¹⁰) sayılınca, aynı örüntü arama yordamı rastgele kitaplarda Kur'an'da bulduğu kadar düşük olasılıklı bir örüntüyü ne sıklıkla yakalar? | **M20'de ~4.000, Mallows ve tekdüzede ~10.000 kitapta bir.** Bu, Kur'an'ın ham sırasının arama bedeli ödenince taşıdığı kanıttır; kitapların bir sıralaması değildir |

## Sonuç (K×100)

| Sıfır | K (dilli) | Kur'an, 10¹⁰ | R_eşit (Kur'an'dan ayrılamayan) | p_kal | R_nokta (yordamın Kur'an'daki kadar ya da daha düşük olasılıklı örüntü yakaladığı) | π̂ = R_nokta/K [%95] |
|---|---|---|---|---|---|---|
| M20 | 20.000 (19.856) | 47 → 4,7×10⁻⁹ | 8 | 9/20.001 ≈ 1/2.200 | 5 | ≈ **1/4.000** [1/12.000; 1/1.700] |
| Mallows | 10.000 (9.939) | 22 → 2,2×10⁻⁹ | 3 | 4/10.001 ≈ 1/2.500 | 1 | ≈ **1/10.000** [1/400.000; 1/1.800] |
| tekdüze | 10.000 (9.921) | 12 → 1,2×10⁻⁹ | 2 | 3/10.001 ≈ 1/3.300 | 1 | ≈ **1/10.000** [1/400.000; 1/1.800] |

Okuma, ön-kayıtlı kurala göre üç sıfırda da **"arada"**.

Altı rastgele kitapta yordamın yakaladığı örüntü, Kur'an'da bulduğundan açıkça daha düşük olasılıklıdır; güven aralıkları çakışmaz:

| Sıfır | Kitap | Mercek | İsabet / 10¹⁰ |
|---|---|---|---|
| M20 | A19926 | 2n − a, 7 | 3 |
| M20 | A14175 | 2n − a, 13 | 7 |
| M20 | A14080 | n − a, 7 | 11 |
| M20 | A7676 | n + a, 23 | 18 |
| Mallows | A7192 | 2n + a, 17 | 0 |
| tekdüze | A5225 | 2n + a, 17 | 1 |

A535 (M20; n − a, 13) 44 isabetle Kur'an'a eşittir. Hepsi gerçekten rastgele dizilişlerdir: 114 sûreden 3–9'u Kur'an'daki yerindedir. Dilleri bağımsız bir programla kendi merceklerinde sınandı.

**Okunuşu.** Plasebolar içine hiçbir yapı konmamış, rastgele karıştırılmış kitaplardır; onlara Kur'an'a uygulanan aynı otomatik yordam uygulandı ve yordam her birinde bulabildiği en iyi örüntüyü seçti. Ölçülen şey kitaplar değil, yordamdır: tesadüfi bir kitapta ne kadar düşük olasılıklı bir örüntü "bulabildiği". Yakalanan örüntüler Kur'an'ınkinden başka türdendir (başka mercekler, küçük asallar, dar yapılar); ölçü onları yalnız olasılıkla karşılaştırır. Bu yüzden "Kur'an'dan nadir kitaplar" demek yanlış olur.

Bu, 14. denetimle çelişmez. Gerçek değer 1/4.000 ise, 200 plasebo arasında yordamın Kur'an'daki kadar düşük olasılıklı bir örüntü yakaladığı kitap çıkmaması beklenen şeydir. 1/201 "en az" demekti; burada ilk kez ölçüldü.

Kur'an'ın ham değeri (4,7×10⁻⁹) ile kalibre değeri (~2,5×10⁻⁴) arasında ~20.000 kat fark var. Bu, merceğin ve dilin kitaba bakılarak seçilmesinin bedelidir. Rastgele kitapların her biri kendi tarifiyle incelendiğinde kendi gözünde "yüz milyonda bir" görünür.

**Kesinlik.** Ön-kayıttaki ±%30 kesinlik hedefi sağlanmadı: R_nokta 5 ve 1 için bağıl hata ~%45 ve ~%100'dür. Kur'an'ın sınırındaki sınıflandırma (M20'de 47'ye karşı 44, 48, 50 isabet) bu hassasiyette kesin değildir ve π̂'nin aralığına katılmadı.

**Puan ile yapı.** Kur'an'ın iç sayımında puanına ulaşan 81 kontrolün yalnız 12'si dilin dört yüklemini birden taşır, hiçbiri Kur'an'ın merceğindeki olguların hepsini taşımaz (`dis_inceleme/`). Puan, dilin o mercekte tutan alt kümesinin bilgisidir. Bu çalışmanın kalibre ettiği şey bu puandır, nesnenin tam profili değil.

## Ne ölçülmedi (araştırmacının itirazları; kural menüsüne aday)

- **Asal.** 19'u metin kendisi anıyor; burada 29 asal eşit bedelli. Rastgele kitaplarda yakalanan en düşük olasılıklı örüntüler 7, 13, 17 ve 23'te.
- **Kitap düzeyi.** Kural ≥ 3 düzey istiyor ama kitap düzeyini şart koşmuyor. Terazi türünden bir denklik (B2: 3303 = 3303) Kur'an'ın dilinde var; bu yedi örüntünün hiçbirinin dilinde yok. B2, rastgele kitapların binde 1,2'sinde bir biçimde tutar; plasebo dillerinin binde 1,1'inde bulunur.
- **Genişlik.** Puana yalnız dile seçilen 3–6 yüklem girer. Kur'an'ın merceğinde kütüphanenin 21 yüklemi aynı anda doğrudur; bu yedi örüntünün merceklerinde 8–18. *(Bu, 16. denetimde kısmi puansız ölçüldü: `denetim16_opus55_profil/`.)*

Bu farklar sonuçlardan sonra fark edildi. Bunlara bakıp ölçüyü değiştirmek, kaçınılan şeyin kendisi olur. Doğru yol: kuralları önceden yazmak, aynı plasebolarla ölçmek, hepsinin sonucunu raporlamak ve seçimin bedelini ödemek.

## Doğrulama

- **Kart sayacı:** `kart/ek/EK_20261031T2010Z_kart_tasarim.md`. Önce Mac'te öykünmeyle sınandı, sonra kartta sınandı; bkz. `kart/kabul/`.
- **K×100:** `k100/RAPOR.md` §Doğrulama.
  - Diller TAMAM.
  - K×10 ile 93.408 ortak (dil, bölüt) çifti aynı: K×10'u işlemci, K×100'ü kart saydı.
  - Kur'an'ın 3×40.000 bölütü kapı çalışmasının `kontrol_h` sayımıyla bölüt bölüt aynı.
  - Kartın yazdığı 10 bölüt işlemcide tek iş parçacığıyla yeniden hesaplandı; hepsi aynı.
- **K×10'un derinleştirmesi:** kısmen kartta hesaplandı. Kalan 981 M20 bölütü ile Mallows ve tekdüzenin tamamı; `k10/ek/EK_20261031T2040Z_kart_gecisi.md`.

## Ön-kayıtlı tahminler

- **Isınma:** "ihmal edilebilir" tahmini tuttu.
- **K×10:** Ana hattın tahminleri Kur'an'ın ayrışmasını sürekli büyük gördü. "M20'de R_derin = 0, olasılık 0,85" tutmadı. Bu, sonuçtan önce bir ekle düzeltildi.
- **K×100:** M20 sayıları tahmine çok yakın çıktı (π̂ ~1/4.000, R_nokta 5). Mallows ve tekdüzede Kur'an tahminden iyi çıktı; ana hattın Kur'an'ın aleyhine yanıldığı ilk durum. Okuma sınıfı üçünde de tuttu.

## Dış inceleme (1 Kasım 2026)

Araştırmacının ilettiği bir dış inceleme ve ana hattın değerlendirmesi `dis_inceleme/` altındadır. Bulguları:
- puana ulaşmanın yapıyı taşımak olmaması (bağımsız olarak doğrulandı);
- WSum'un yalnız boylar toplamına bakması;
- "herhangi üç düzey" kuralının kitap düzeyini şart koşmaması ve aynı toplamın iki düzeyde sayılabilmesi;
- paketteki eski `kontrol_h.c` kopyası (değiştirildi);
- ısınma ve kesinlik ifadelerinin fazla kesin olması (düzeltildi).

## Dosyalar

- `isinma/`, `k10/`, `kart/`, `k100/`: her biri ön-kayıt, ekler, kaynak, koşu betikleri, rapor ve yeniden sayım çıktılarıyla.
- **Bölüt dosyaları** arşivdedir:
  - `k10/sonuc/bolutler_k10_<S>.tar.gz`;
  - `k100/sonuc/tab/<S>/bolutler_k100_<S>.tar.gz` (açılınca `kA/d<düzey>_<parti>/b<bölüt>.txt`).
- **Hedef kitaplar ve dilleri:** `k100/sonuc/tab/<S>/hedef.bin` ve `diller_A.txt`. İlk 2.000 / 1.000'i K×10'unkilerle aynıdır.
- `dogrulama/`:
  - `plasebo_dil_sina.py`: dil sınaması (bu denetimde FC1, FNd2, FCK, FH2p, WHNp, FCd ve QH eklendi);
  - `kitap_incele.py`, `mercek_profil.c`: kitapların somut yapısı;
  - `k10_k100_ayni.py`: K×10 ile aynılık.
- **Referans tabloları** (`ref_*.ref`, sıfır başına ~160 MB) depoda yoktur. 13. denetimdeki plasebo12 tablolarının aynısıdır; oradaki üretici ve tohumlarla yeniden üretilebilir. SHA-256 özetleri `referans_tablolari_sha256.txt` dosyasındadır.
