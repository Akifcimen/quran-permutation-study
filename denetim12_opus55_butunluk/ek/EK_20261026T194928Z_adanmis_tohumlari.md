# EK — adanmış kestirim tohumlarının numaralanması (§5.3'ten belgelenmiş sapma)

**Gözlem:** §5.3'ün formülü adanmış kestirim (tür 4) için tohum = 26102600000 + sıfır·10⁸ + 4·10⁷ + kodeks·10⁶ + sıra verir
(U = 1, M = 2, L = 3). Uygulamada adanmış koşularda sıfır numarası **bir eksik** kullanıldı: tekdüze adanmış koşuları
26142600000 + i (`kos/adanmis_U3.sh`; i = 2 Kur'an aramasının üretim koşusu, 3–17 belirsiz kitaplar), M20 26242600000 + i (i = 1
pilot, 99 hız ölçümü; üretim i = 2'den), Mallows 26342600000 + i. Tutarlılık için bu kaydırılmış numaralama sonuna kadar korunur.

**Etkisi:** yok. Tohumlar sabit ve önceden yazılmış bir yasaya göre seçilir; kaydırılmış uzay (26142…, 26242…, 26342…) başka hiçbir
akışla çakışmaz (referans tür 1, sınama tür 2, menü tür 3 akışları ve formülün asıl adanmış uzayının kullanılmayan kısımları ayrıktır;
her tohum bir kez kullanılır). Hiçbir sonuç bu kayma yüzünden değişmez; yalnız yeniden üretim için gerçek tohumlar koşu
betiklerinde ve sonuç dosyalarının yanında yazılıdır.
