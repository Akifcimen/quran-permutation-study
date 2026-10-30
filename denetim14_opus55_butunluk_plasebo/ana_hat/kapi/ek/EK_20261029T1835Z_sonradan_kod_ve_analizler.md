# Ek — dondurmadan sonra yazılan kod ve sonradan analizler (29 Ekim 2026, 18:35Z)

Ön-kayıt 09:41:23Z'de donduruldu (SHA-256 `6d8db06b…2e09`). Donmuş kod listesi (`sonuc/kod_sha256.txt`) koşuların sonunda yeniden sınandı: yerelde ve workstation'da bütün dosyalar aynı (workstation'da yalnız yerelde duran `dogrulama/yeniden_say_kapi.py` yok).

## 1. Dondurmadan sonra yazılan kod

Hesap kodu (C kaynakları, `kapi_akis.sh`, `kontrol_b.py`) değişmedi. Sonradan yazılanlar çalıştırma ve sınama içindir:

| Dosya | Yazıldı | Ne yapar |
|---|---|---|
| `kos/kuran_kapi.sh` | 09:53Z | §6 sayımlarını çalıştırır: Kur'an'ın donmuş dilleri `kontrol_h` ile, verilen kapıyla, bölüt 1..son. `kontrol_h.c` donmuş listededir (`33856c3b…`). |
| `dogrulama/bagimsiz_sina_kapi.py` | 11:46Z | `~/opus5.5/kademe/kos/bagimsiz_sina.py`'nin aynısı; tek fark, kapı eşikleri ortam değişkeninden okunur. Kur'an tarafı isabetlerini tanımlardan sınar. |
| `dogrulama/plasebo_dil_sina.py` | gün içinde; son ek 18:30Z | §7.2: hedeflerin Kol A dillerini kendi merceklerinde sınar. Yüklemler `kutuphane.h`'deki tanımlardan ve dışlama kurallarından (`dislama_hazirla`) Python'a yazıldı, sınanan dillerde geçtikçe eklendi. 18:15–18:30Z arasında eklenenler: BMt, K2, FC2, WEq, WHNd2, WUN2p, QS, FD2, HR0c, QkH, FA2p, FAP. |

## 2. §6 derin sayımı ön kayda uygun

Kur'an'ın Kol A dili üç kapıda da plasebo14'ünkiyle aynıdır: `A000 0 10 6 4 1 17 19 73`, yani {B2, K1, Qc, WSum} @ (1,+1,19). Dil B2'yi gerektirdiği için §6'nın ilk maddesi uygulandı: `kontrol_h` Gc ile derlendi ve Kur'an'ın sırası kademe ön-kaydının durma kuralıyla (k ≥ 10) 10¹⁰'da ölçüldü. Sapma değildir.

## 3. Sonradan analizler (ön kayıtta yok)

Aşağıdakiler aynı kayıtlardan sonuçlar görüldükten sonra yapıldı. Betimseldir. Hiçbir okuma kuralına girmez.

1. **19-öncel sayımı (18:00Z civarı).** Gc 10¹⁰ isabet kayıtlarında (`sonuc/kuran/Gc10/<S>/isabet.s`) en iyi merceği p = 19 olanlar ve 19'da bütünlüklü merceği olanlar sayıldı. İki sayı her sıfırda aynı çıktı: M20 3, Mallows 2, tekdüze 0. Metnin andığı asallar {3, 5, 7, 11, 19} birlikte: 6, 7, 2. Gerekçe: on dokuzu metin anıyor (Müddessir 30). Yine de bu sayım sonuçlar görüldükten sonra tanımlandı.
2. **19'daki beş isabetin yapısı.** Her biri Kur'an'ın olgularıyla karşılaştırıldı (57/57, terazi, 6236/6555, k_F/k_H, ayrıklık, 1444, 722/722, hücre sayıları ve toplamları, katsayı bandı, |U|, birleşimin ayet toplamı mod 19). Hiçbiri hepsini taşımıyor. En yakını (M20 sıra 14644) yalnız Qa'yı taşımıyor. Kur'an'la ortak konum sayıları 4–9. Betik: oturum karalama dizininde `yapi_sina.py`; rapora alınacak.
3. **Plasebo dil sınamasının genişletilmesi.** §7.2 "en iyi üç plasebo" diyordu. Her kapı ve sıfırda Kur'an ve en küçük p'li üç plasebo sınandı: 36 / 36 TAMAM (`sonuc/plasebo_dil_sinamasi.txt`).
