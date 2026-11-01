# K×10 plasebo kalibrasyonu — ön-kayıt (ana hat, 31 Ekim 2026)

## 1. Soru

Kapı çalışmasında (birincil kapı Gc) Kur'an, kendi bütünlüklü dilini kuran bütün plaseboları sıralı iki sıfırda aştı: R = 0. Kalibre p bu yüzden tasarımın verebileceği en küçük değerde kaldı: 1/201 (M20) ve 1/101 (Mallows). Kur'an'ın kendi sırası (Gc, 10¹⁰ kontrol) M20'de 4,7×10⁻⁹, en yakın plaseboyla M20'de 2,5×10⁻⁷. Ama bu uzaklık kalibre p'ye girmez.

Bu çalışma aynı tasarımı on kat plaseboyla tekrarlar. Sorular:
- Kur'an hepsini yine aşıyor mu?
- Aşmıyorsa kaç plasebo yetişiyor?

## 2. Yöntem

Kapı çalışmasının Gc kolunun aynısıdır (`~/opus5.5/kapi/ONKAYIT.md` §2–§3). Değişenler:

| | kapı çalışması | bu çalışma |
|---|---|---|
| Plasebo sayısı K | M20 200, Mallows 100, tekdüze 100 | **M20 2.000, Mallows 1.000, tekdüze 1.000** |
| Hedefler | `hedef.bin` (plasebo12) | aynı üretici (`hedef.c`) ve tohum kuralı (`tohum(s, 3, h)`), h = 1..K. İlk 200 / 100 hedef kapı çalışmasınınkilerle bayt bayt aynı olmalıdır; koşudan önce sınanır |
| Mallows sayacı | özgün | yarış düzeltmeli (`orn_init` kilit altında; kapı çalışmasının 29 Ekim 19:02Z eki). Kilit yalnız L için uygulanır: M20 ve tekdüze kurulumu ortak durum yazmaz; tam kilit sonuçları etkilemez, yalnız yavaşlatır (ısınma tanısının eki) |
| Yeniden sayım | `yeniden_say_kapi.py` | `dogrulama/yeniden_say_k10.py`: bütün bellek partilerini okur, eksik bölütte durur. Kapı çalışmasının üç kapısında aynı sonucu verdiği sınandı |

Aynı kalanlar şunlardır:
- kapı Gc (her hücre ≥ 2);
- 109 yüklemlik kütüphane;
- bütünlük kuralı (≥ 3 tür, ≥ 3 düzey) ve dil bedeli;
- kontrol tohumları (tür 2);
- derinlik kuralı: k(1–4) ≥ 10 ise dur; k(1–40) ≥ 10 ise dur; değilse 1–400. Kur'an'ın dili her zaman 1–400.

Kontrol bölütleri kapı çalışmasınınkilerle aynı tohumlardan gelir. Kapı çalışmasında da bulunan 200 / 100 plasebonun sayımları kapı çalışmasının düzeltilmiş sayımlarıyla aynı çıkmalıdır; bu bir yeniden üretim sınamasıdır.

**M20 ısınması.** `~/opus5.5/isinma` tanısının kararına göre belirlenir. Karar "ihmal edilebilir" ise 30.000 (özgün) kullanılır. "Yanlılık var" ise tanının belirlediği ısınma kullanılır ve kapı çalışmasının M20 satırları da onunla yeniden sayılır. Bu madde tanının sonucu görüldükten sonra, dondurmadan önce dolduruldu: tanı 31 Ekim 16:21Z'de bitti, karar **ihmal edilebilir** (`~/opus5.5/isinma/RAPOR.md`; `b2` oranı 0,9997 [0,9971; 1,0022], bütünlüklü kontrol oranı 0,982 [0,965; 0,999], bölüt içi kayma yok). **Isınma 30.000 (özgün) kullanılır.**

## 3. Okuma kuralı (kapı çalışmasıyla aynı)

R, p'si Kur'an'ınkinden büyük olmayan ya da %95 aralığı Kur'an'ınkiyle çakışan plasebo sayısıdır. Kur'an'ın p'si ve aralığı, plasebolarla aynı derinlikten (10⁸ kontrol) alınır. p_kal = (1 + R)/(K + 1).
- R = 0: **aşar**.
- R/K ≥ 0,05: **sık**.
- Arası: **arada**, kalibre p'siyle yazılır.

Birincil karar sıralı sıfırlarda (M20 ve Mallows) verilir. Tekdüze ikincildir.

**İkincil derinleştirme (yalnız R > 0 ise).** Birincil kuralda R'ye giren plasebolar düzey 4'e (bölüt 401–4.000, 10⁹ kontrol) indirilir. Kur'an'ın değeri olarak Gc 10¹⁰ sayımı kullanılır (M20 4,7×10⁻⁹ [3,5; 6,3], Mallows 2,2×10⁻⁹ [1,4; 3,3], tekdüze 1,2×10⁻⁹ [0,6; 2,1]). Sonuç **R_derin** olarak ayrıca yazılır. Gerekçe: 10⁸ kontrol Kur'an'ı ancak 3,7×10⁻⁸'e kadar çözer. Plasebo sayısı arttıkça bu derinlikte Kur'an'la çakışan plaseboların sayısı da artar. Birincil kural bu durumda temkinlidir (Kur'an'ın aleyhine). Birincil karar R'dir; R_derin onun yerine geçmez, yanında verilir.

## 4. Yorum için önceden yazılan çerçeve

Bir sonuç, malzemenin seçimini yapan bilinmeyen aramaya karşı ancak kalibre p'si kadar sağlamdır. Kabaca, S bağımsız malzemeye bakılmış olsa bileşik sınır S · p_kal olur. R = 0 çıkarsa M20'de bu sınır S ≤ 100 için 0,05'in altında kalır, Mallows'ta S ≤ 50 için. Bu bir karar değil, okurun kendi arama varsayımını sayıya çevirmesi için bir ölçektir.

**Betimsel:** her sıfırda en küçük p'li on plasebo, mercekleri ve dilleri; plasebo p dağılımının alt kuyruğu. Model tabanlı kuyruk kestirimleri yapılırsa etiketlenir ve kararlarda kullanılmaz.

## 5. Ana hattın tahmini (sonuçlardan önce)

- **M20 (K = 2.000):** dilli 1.980–2.000. En küçük plasebo p 2×10⁻⁸ – 2×10⁻⁷. Birincil R 0–4; R = 0 için P ≈ 0,45. Okuma "aşar" 0,45, "arada" 0,55, "sık" < 0,01. R > 0 ise R_derin 0–1.
- **Mallows (K = 1.000):** dilli 985–1.000. En küçük plasebo p 3×10⁻⁸ – 3×10⁻⁷. R 0–3; R = 0 için P ≈ 0,5.
- **Tekdüze (K = 1.000):** R 5–25, yani A088 türünden yozlaşmış merceklerde yetişen plasebolar. "arada" 0,9, "sık" 0,05.

Gerekçe: kapı çalışmasında 200 M20 plasebosunun en küçüğü 2,5×10⁻⁷'dir. On kat plasebo kabaca on kat küçük bir alt uç verir. Bu, Kur'an'ın 10⁸'deki aralığının üst ucuna (3,7×10⁻⁸) yaklaşır. Tahmin hükmü: okuma sınıfı tutarsa "tuttu", sayılar aralıktaysa "aralıkta".

## 6. Kaynak ve doğrulama

Workstation (`~/hrf_olcum/k10/`), en çok 12 iş parçacığı, `nice -n 10`. Tahmini süre birincil için 4–6 saat. Derinleştirme gerekirse sıfır başına 8–10 saat.

Doğrulama:
1. `yeniden_say_k10.py` ile bağımsız yeniden sayım.
2. Her sıfırda en küçük p'li beş plasebonun ve Kur'an'ın dilinin `plasebo_dil_sina.py` ile sınanması.
3. İlk 200 / 100 plasebonun kapı çalışmasıyla aynı çıkması.
4. Mallows kollarında düzeltilmiş sayacın tek iş parçacıklı sayımla birkaç bölütte aynı çıkması.

Sapmalar `ek/` altında zaman damgalı yazılır.
