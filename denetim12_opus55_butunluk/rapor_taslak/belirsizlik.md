## 5. Bilgi puanında çözülmemiş aramalar, adanmış kestirim ve R2

**Neden aralık var.** Bilgi puanı, kitabın sağladığı maskelerin (aile boyuna koşullu) sıklığını 10⁸ kitaplık referans örnekleminden
okur. Nadir bir maske referans diliminde hiç ya da 10'dan az görülmüşse arama **çözülmemiş** sayılır ve ön-kayıt gereği bir tabana
kırpılmaz: puanın alt sınırına olasılığın tek yanlı %95 üst sınırı, üst sınırına alt sınırı girer (sayı 0 ise üst sınır ∞). Böylece
"belirsiz" kitaplar (S_alt < S_Q ≤ S_üst) çıkar: tekdüzede 16.595, M20'de ~18.150, Mallows'ta ~16.120 (10⁸ sınama kitabında; ~%0,02).
**Bunların neredeyse hepsi eşiğin çok altındadır:** Δ = S_Q − S_alt (Kur'an'ı geçmek için çözülmemiş aramanın %95 üst sınırından 2^Δ kat
daha nadir olması gerekir) dağılımı — tekdüze: en küçük 0,31, medyan 22,5, %90'lık 27,8 bit (Δ ≤ 3: 4 kitap); M20: 0,12 / 19,1 /
24,2 (Δ ≤ 3: 25); Mallows: 0,11 / 18,7 / 24,0 (Δ ≤ 3: 31). Belirsizlik aralığının üst ucu bu yüzden bir sıklık kestirimi değildir.

**Adanmış kestirim (ön-kayıt §4.3).** Önce Kur'an'ın çözülmemiş aramaları (yalnız tekdüzede vardı: 129'da aile maskesi, 2/1.014.579 →
kesin koşullu kestirim 46 isabet / 2×10⁷ öneri, 2,31×10⁻⁶; 127'de tekdüze ve Mallows, §7.1), sonra belirsiz kitaplar S_alt'a göre azalan
sırayla. Yöntem: tekdüzede kesin koşullu önem örneklemesi (aile yapısı sınıf-sayı vektörüyle önerilir, Φ(k)/M! ağırlığıyla düzeltilir;
p ≥ 11) ya da tek mercekli doğrudan örnekleme; sıralı sıfırlarda sıfırın kendi örnekleyicisiyle tek mercekli doğrudan örnekleme.
Doğrulama: kesin kestirici Kur'an'ın merceğinde 11 maskenin tekil sıklığında referans tablosuyla uyumlu (\|z\| ≤ 1,62); tek-mercek
değerlendirmesi tam değerlendirmeyle dört sıfırda 20.000'er kitapta bit-bit aynı. Bütçe (ön-kayıt): arama başına ≤ 2×10⁹ kitap
(tekdüzede ≤ 2×10⁸ öneri), sıfır başına ≤ 4 çekirdek-saat; hedef ≥ 30 isabet. Arama başına kitap sayısı, referanstaki nokta kestirimine
göre beklenen ≥ 30 isabete yetecek kadar seçildi (tavan 2×10⁹). Kullanılan: tekdüze 10 koşu / 12 arama (Kur'an'ınki dahil; ~1,8 çekirdek-saat),
10 kitap yeniden puanlandı. Bunların ikisi (S_alt 26,88) adanmış kestirim almadı: beş araması (~1,7 çekirdek-saat) süre önceliğiyle
koşulmadı, iki kitap aralıkta kalır. S_alt'ı 24,93 olan bir kitap ise tek ve ucuz bir aramayla S_alt sırasının dışında çözüldü. Bu sapma
`ek/EK_20261026T224527Z_tekduze_adanmis_sirasi.md`'de yazılıdır ve yalnız k_üst'ü 1 azaltır. M20 12 koşu / 12 arama, 14 kitap
(1,9×10¹⁰ kitap, ~2,9 çekirdek-saat); Mallows 11 koşu / 11 arama, 14 kitap (8,3×10⁹ kitap, ~3,4 çekirdek-saat; Mallows örnekleyicisi
hız ölçümünde M20'ninkinden 2,2 kat yavaş). Adanmış kestirimin kendi
belirsizliği (≥ 10 isabette ±1,96/√h/ln 2 bit; < 10 isabette Poisson–CP) aralığa taşınır: eşiğe ~0,4 bitten yakın kitaplar
adanmış kestirimden sonra da belirsiz kalabilir. Bir kitabın başka bir merceğinde çözülmemiş bir arama kalmışsa üst sınırı yine ∞'dur.

**Yeniden puanlanan kitaplar** (önce: referans tablosuyla; sonra: adanmış kestirimlerle; karar: alt ≥ S_Q → ÜSTTE, üst < S_Q → ALTTA):

*Tekdüze (S_Q 31,06):*

[BELIRSIZ_U129]

*M20 (S_Q 27,41):*

[BELIRSIZ_M129]

*Mallows (S_Q 27,27):*

[BELIRSIZ_L129]

[BELIRSIZ_OZET]

**R2 (model-tabanlı kuyruk).** Ön-kayıtlı doğrulama ölçütü: R2'nin P(S ≥ s) eğrisi doğrudan sıklıkla, doğrudan isabetin ≥ 30 olduğu en
yüksek dört eşikte oran [0,5; 2,0] içinde olmalı. Tekdüzede 3×10⁷ kitaplık ara bakışta geçti (0,55–0,65), tam örneklemde (10⁸) kıl payı
**geçemedi** (0,49–0,67): oran eşik yükseldikçe düşüyor, yani R2 kuyruğu giderek eksik kestiriyor (gruplar arası bağımlılık ve hiç
görülmemiş maskeler modelde yok). Bu yüzden R2 hiçbir kararda kullanılmadı; her birincil sıfırda doğrudan isabet var (k ≥ 1). Sayım
puanında R2 doğrudan sıklıkla neredeyse birebir (c = 6–10'da oran 1,00). M20 ve Mallows'ta R2 gerekmedi (ölçülmedi).
