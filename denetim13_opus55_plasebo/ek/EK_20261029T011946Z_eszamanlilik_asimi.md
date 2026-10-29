# EK — eşzamanlılık sınırının kısa aşımı (20261029T011946Z)

**Ne oldu.** Mallows akışı (`kos/akis.sh L`) 2026-10-29T01:15:4xZ'de başladı; referans geçişi 4 süreçle koşarken, betimsel özet
betiği `src/el_sinif.py` (tek iş parçacıklı Python; yalnız `sonuc/{M,U}/analiz.json`'u okur) ön planda çalıştırıldı. Böylece yaklaşık
3–4 dakika boyunca **5 hesap süreci** aynı anda koştu; görevdeki "aynı anda en çok 4 iş parçacığı" sınırı aşıldı. Fark edilince
betik durduruldu (U kısmı yarıda kaldı); o andan sonra yalnız 4 referans süreci koşuyor.

**Etkisi.** Sonuçlara etkisi yoktur: referans geçişi deterministik tohumludur (tür 1), süreler dışında hiçbir çıktı eşzamanlılığa bağlı
değildir; `el_sinif.py` hiçbir tabloyu yazmaz. Kayıtlı süreler (`sure_s`) bu birkaç dakikada çekirdek paylaşımı yüzünden biraz uzamış
olabilir; hesap toplamı bu yüzden en çok birkaç çekirdek-dakika şişer.

**Önlem.** Bundan sonra arka planda 4 süreçli bir iş koşarken hiçbir hesap (analiz betikleri dahil) başlatılmaz; betimsel özetler akış
bitince koşulur.
