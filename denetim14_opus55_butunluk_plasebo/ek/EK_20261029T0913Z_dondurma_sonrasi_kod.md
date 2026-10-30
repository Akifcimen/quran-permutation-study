# EK — dondurmadan sonra eklenen kod ve süreç notları

Yazıldı: 2026-10-29T09:13Z. Tür: ekleme (yalnız yeniden üretim sınaması ve raporlama). Hesaplanan hiçbir sonucu, tanımı, eşiği, bedeli
ya da kuralı değiştirmez. Betimsel düzeltme (`src/analiz_b.py`) ayrı ektedir: `EK_20261029T0905Z_betimsel_duzeltme.md`.

## 1. Eklenen dosyalar

| Dosya | Yazıldı (UTC) | Ne yapar |
|---|---|---|
| `kos/yeniden_sina.sh` | 04:40:42Z (dondurmadan 2,5 dk sonra; ilk kontrol geçişinden önce) | ONKAYIT §7.1'in oturum sonu maddesi: Ö0'ın karşılaştırmalarını (`o0_d.sh`, `o0_k.sh` ile aynı karşılaştırmalar) `kos/derle.sh` ile geçici dizine yeniden derlenmiş ikililerle yineler. İki betik `sonuc/o0/`'a yazdığı için doğrudan çağrılmadı; aynı komutlar geçici dizinde koşar. Ayrıca bu oturumun üretim çıktılarını yeniden üretir: üç sıfırın bütün kol dilleri ve ayrıntıları, M20 Kol A ve Kol B kümelerinin düzey-1 bölüt 1'i (başlık satırı hariç sayımlar, `.q`, `esikler.txt`). |
| `src/rapor_tablo_b.py` | 04:44:15Z; sayı biçimi 09:11:56Z'de değişti | Analiz JSON'larından `sonuc/rapor_tablolar.md`'yi yazar. Yalnız biçimlendirme: üst simgeli üsler, virgüllü ondalık, en iyi beş listesinde dilsiz plasebolar gösterilmez. |

İkisi de `sonuc/onkayit_kod_sha256.txt`'te yoktur (dondurma anında yoktular).

## 2. Oturum sonu sınamasının sonucu

`./kos/yeniden_sina.sh <geçici dizin>`, 2026-10-29T09:02–09:09Z (duvar 401 s), hiçbir başka hesap koşmazken; en çok 4 iş parçacığı; user + sys 510 s,
en büyük süreç 1,87 GB. Sonuç `sonuc/yeniden_sina_rapor.txt`: **16 / 16 AYNI**: Ö0 kısıtsız D (M, U, L), Ö0 kontrol bölüt 1 (M, U, L),
diller (M: A, A−, A+, B; U: A, B; L: A, B), M20 Kol A düzey 1 bölüt 1, M20 Kol B düzey 1 bölüt 1.

Dondurma anı özetleri (`shasum -a 256 -c sonuc/onkayit_kod_sha256.txt`): 50 satırın 49'u OK; farklı olan yalnız `src/analiz_b.py`
(betimsel düzeltme). `ONKAYIT.md`'nin SHA-256'sı değişmedi (`6080c6b1…f748`); dil dosyaları 16 / 16 OK (`sonuc/diller_sha256.txt`).
plasebo12'nin referans tabloları ve hedef dosyaları sonda yeniden doğrulandı (20 / 20 ve 3 / 3 OK). Salt okunur beş dizinde
(`~/Desktop/hrf2`, `~/opus5.5/butunluk`, `kosullu`, `plasebo`, `plasebo12`) oturum başından (03:45Z) sonra değişmiş dosya yok (`find -newermt`).

## 3. Süreç notları (sapma değildir)

- **Hedef ayrıntı dosyaları** (`tab/<sıfır>/hedef_ayrinti_<kol>.txt`; betimsel L\* profilleri) salt okunur yapılmadı. ONKAYIT §4.5 yalnız
  `diller_<kol>.txt` ve `ayrinti_<kol>.txt`'yi donduruyordu. Hepsinin özetleri paketin içerik listesindedir (`paket/plasebo14_icerik_sha256.txt`).
- **İzleme döngüleri.** Akışı izlemek için arka planda `until grep …; do sleep 30; done` döngüleri kullanıldı; bir ikisi izlenen olay
  geçtikten sonra da bir süre açık kaldı. Yalnız `grep` ve `sleep` çalıştırırlar; ONKAYIT §8.2'ye göre izleme komutları hesap sayılmaz.
  09:05Z'de açık döngü yoktu.
- **Ek adı.** Bu ek önce yanlışlıkla ileri bir saatle (`…T0920Z…`) adlandırıldı ve 09:13Z'de yazıldığı saate göre yeniden adlandırıldı.
- **Sonradan kaydedilen hesap kalemleri.** `sonuc/hesap_ek.txt`'e akış bittikten sonra üç kalem eklendi: dillerin kuruluşu (40 ç-sn),
  analizler (300 ç-sn) ve oturum sonu sınaması (510 ç-sn). Kayıtlı toplam 18,10 çekirdek-saattir (tavan 40).
