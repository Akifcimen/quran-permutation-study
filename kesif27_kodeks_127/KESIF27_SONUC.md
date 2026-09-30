# kesif27 — 127 kodeksi: matris katı ve Halife'nin gerekçeleri

**Tarih:** 29 Eylül 2026 · **Sonuç:** dürüstlük notu 18 (`KESIF.md`).

## 1. Matris katı Halife koşulunun kendisidir (`yol127.py`)

Soru: Halife ekolünün "Tevbe 127" tercihi ile bizim matris katımız (ord P = ord R = ord S = 360, D² = I; yalnız 127'de tutar) farklı yollardan aynı yere mi çıkıyor?

Ölçüm: 114 sûre × delta −100…+100, 16.592 tek-sûre düzeltmesi. Çekirdeği koruyan (57/57 ve Σt(çift) = Σa, 12'li t-ailesi, 7'li u-ailesi, üyelere dokunulmamış) 3.007 düzeltmede tam matris mimarisi ile Halife'nin "(Σa + 112 besmele) ≡ 0 (mod 19)" koşulu birebir örtüşür:

| | Halife koşulu var | yok |
|---|---|---|
| Mimari var | 145 | 0 |
| Mimari yok | 0 | 2.862 |

Yalnız delta ≡ −2 (mod 19) işler. Tevbe 13, 51, 89, 127, 165, … (adım 38) aynı mimariyi verir. Tevbe dahil 39 sûreden 2 ayet silmek aynı sonucu verir (`RAPOR.md`'deki 47, u-ailesi şartı olmadan). Çekirdeği bozan 29 mimari isabetinin hepsi de koşulu sağlar.

Mekanizma: çekirdeği koruyan bir düzeltme P'de yalnız Σa(çift-t), R'de yalnız Σa(aile dışı) hücresini aynı miktarda kaydırır; matris yalnız Σa mod 19'u görür. Matris zaten 127 verisinde bulunmuştu (`dokuman.md` R'yi 5380 ile kurar). Halife'nin tercihi tam bu koşula dayandığından matrisin 127'de çıkması kesindi. 127 bağımsız bir yol değildir; `kesif26`'daki ×11–12 kazanç bu koşulun yeniden sayımıdır.

## 2. Ek 24 ligi (`hasat_*.py`)

Halife'nin çevirisindeki Ek 24, 9:128–129'un çıkarılması için 65 sayısal madde, bir rivayet ve teolojik iddialar sıralar (kontrol edilen maddelerin aritmetiği doğrudur). Sayısal maddeler için lig: her sûrenin son iki ayeti tek tek silinir; aynı şablon ailesinde kaç yeni 19-katı oluşur (düzeltme), kaçı bozulur (bozma); Tevbe 114 yarışmacıdan biridir.

| Lig | Tevbe düzeltme sırası | Tevbe net sırası |
|---|---|---|
| Sayısal, Halife'nin madde türleriyle (1.762 şablon; 129 metninde zaten 82 isabet) | 1–2 (Hûd ile) | 7–12 |
| Aynı aile, 9'a çakılı alt kümeler (1–9, 9–27, 9–114, rakam toplamı 19…) çıkınca | 8–10 | 18–26 |
| Çapalar yerine her sûre için simetrik aralıklar (sonradan eklenen duyarlılık) | 10–16 | 50–53 |
| Kelimeler, yazıldığı biçimiyle | 15–35 | 63–97 |
| Kelimeler, önekler soyulunca | 9–25 | 77–101 |

Şablon ailesi (`hasat_sayisal.py`) sonuç görülmeden yazıldı: 65 alt küme × 28 nicelik (Σn, Σa, besmeleli Σa, ayet-no toplamı, kümülatif, rakam toplamları, basamak sayıları, rakam sayımları, bitişik yazımlar). Şansa göre beklenen düzeltme: dokunulan şablon × 1/18 (`hasat_sayisal_b.py`): Tevbe 388 şablona dokunur (lig medyanı 302), 31 düzeltir, 20,4 beklenir; "fazla"da 2./114. Kelime ligi (`hasat_kelime.py`) api.alquran.cloud `quran-simple-clean` metniyle, numarasız besmeleler çıkarılarak.

Amiral maddeler tutar: "Allah" 2699 → 2698 (önekli biçimler dahil, Allahümme hariç); "Allah" geçen ayetlerin numara toplamı 118.252 → 118.123 = 19×6217; "lâ ilâhe illâ hû" 30 → 29; العرش 20 → 19. Ama tekil değildir: son iki ayeti silinince "Allah"ı 2698'e indiren 17 sûre vardır, ikinci madde yalnız 9'u bırakır (şans beklentisi ≈ 0,9). Aynı iki ayet 129 lehine de "kanıt" üretir: أنفسكم 38 ve وهو 171 yalnız bu iki ayetle 19'un katıdır. 6346 = 19×334 maddesi §1'deki denklemin kendisidir.

Rivayet de ayırt edici değildir: Buhârî 4986 (Tevbe'nin sonu yalnız Ebû Huzeyme'de bulundu) ile 4988 (Ahzâb 23 yalnız Huzeyme b. Sâbit'te bulundu) aynı türdendir; 33:23 metindedir. Bütün klasik sayım gelenekleri 9:128–129'u sayar.

**Sonuç:** kendi hesabımız 9:128–129'un çıkarılmasına dayanak vermez; savunulabilir satır 129 satırıdır.

Kaynaklar: Halife, Ek 24 (masjidtucson.org/quran/appendices/appendix24.html); Buhârî 4986, 4988 (sunnah.com).
