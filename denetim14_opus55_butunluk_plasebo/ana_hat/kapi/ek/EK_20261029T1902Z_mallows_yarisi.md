# Ek — Mallows örnekleyicisinde iş parçacığı yarışı (29 Ekim 2026, 19:02Z)

**Bulan.** Ön-kayıt §7.3'ün kod denetimi: sonuçları görmemiş, ayrı bir ajan (19:00Z civarı). Bu kapı çalışmasının, kademeli ölçümün, plasebo14'ün ve 13. denetimin (plasebo12) ortak kodundadır.

**Hata.** `kutuphane.h`'de Mallows örnekleyicisinin kurulumu, `orn_init(…, 'L', …)`, küresel bir diziyi (`REF_DESC`) her çağrıda önce yeniden yazar, sonra yerinde sıralar. Ardından bölütün q'sunu bu diziden hesaplar ve diziyi bölütün referansına kopyalar. `kontrol_b.c`, `kontrol_h.c` ve plasebo12'nin `kontrol.c`'si `orn_init`'i iş parçacıklarından kilitsiz çağırır. İki iş parçacığı aynı anda çağırırsa bir bölütün referansı ya da q'su bozulabilir. O bölütün 250.000 kitabı yanlış dağılımdan gelir; kitaplar 114 ayet sayısının permütasyonu bile olmayabilir. M20 ve tekdüze etkilenmez. 12. denetimin kodu tek iş parçacıklıdır, etkilenmez.

**Gerçek koşulardaki iz (ana hat, 18:45–19:00Z).**
- Kur'an tarafı Mallows kayıtlarında permütasyon olmayan kitaplar: Gc 10¹⁰'da 6.279 kayıtta 4; GE 10⁹'da 2; G0 10⁹'da 1. GE'dekilerden birinin puanı (31,6 bit) Kur'an'ın eşiğinin üstündedir.
- Gc 10¹⁰'un 22 Mallows isabetinin hepsi geçerli permütasyondur.
- Aynı Mallows bölütlerini iki ayrı koşu saydı: Kur'an tarafı Gc ve kademeli ölçüm. Kapıdan bağımsız `b2` alanı 40.000 bölütün 43'ünde uyuşmuyor. Bunların bir kısmı çağrıların ilk turundadır (401–407, 4001–4008). 22 isabetin bölütlerinin hiçbiri bu 43 bölütte değildir.
- plasebo14 ve plasebo12'nin Mallows aday dosyalarında permütasyon olmayan kayıt yok (plasebo14'ün aday dosyaları boş; plasebo12'de 185 kayıt). Bu bozulmanın olmadığını göstermez.

**Düzeltme.** `orn_init` çağrısı, sayaçların zaten var olan `KILIT` kilidiyle sarıldı. Donmuş `kutuphane.h`'ye dokunulmadı. Değişen yalnız bu üç satırdır (`kontrol_b.c:40`, `kontrol_h.c:44`, plasebo12 `kontrol.c:29`). Workstation `~/hrf_olcum/yaris/src/`, özetler `yaris/sonuc/kilit_kod_sha256.txt`. Tek iş parçacıklı çıktı değişmez.

**Eşdeğerlik.** Düzeltilmiş `kontrol_h_Gc` ile bölüt 1–24 sayıldı:
- M20, 12 iş parçacığı: özgün çıktıyla birebir aynı.
- Mallows: 1 ve 12 iş parçacığıyla birbirinin ve özgün çıktının aynısı.

**Yeniden koşu (19:00Z'de başladı; `yaris/kos/yeniden_L.sh`, `yeniden_p12.sh`).** Bütün Mallows sayımları düzeltilmiş sayaçlarla yeniden sayılır ve özgün bölüt dosyalarıyla bölüt bölüt karşılaştırılır (süre alanı hariç):
1. Kur'an tarafı Gc, 1–40.000.
2. Kademe, G2E, 1–40.000.
3. Kur'an tarafı GE ve G0, 1–4.000.
4. Üç kapının plasebo Mallows kolları (Kol A, düzey 1–3, aynı dil dosyaları).
5. plasebo14 Mallows Kol A ve Kol B.
6. plasebo12 Mallows.

Raporlar yeniden sayımla güncellenecek. Bozuk bölütler yeniden sayımın değerleriyle değiştirilir. Okuma kuralları değişmez. Bu ek yazılırken yeniden koşunun hiçbir sonucu görülmemişti.
