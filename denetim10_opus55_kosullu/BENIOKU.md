# denetim10 — onuncu denetim: koşullu ölçüm (23 Ekim 2026)

Dokuzuncu denetimin "silmek koşullamak değildir" tespiti üzerine, araştırmacının isteğiyle ayrı bir klasörde ve ayrı hafızayla çalışan bir Claude Opus 5.5 oturumu (en yüksek çaba) şu soruyu ölçtü: önceki literatürü (12 üyeli aile ve 1444, 722/722, iki 361 hücresi; ikincil olarak terazi) zaten taşıyan rastgele kitaplar arasında, bu çalışmanın eklediği yapı ne kadar sıra dışı? Görev tanımı `GOREV.md`. Oturum depoyu yalnız okudu. Ana hattın değerlendirmesi `KESIF.md` dürüstlük notu 23'tedir.

**Sonuç (ayrıntı `KOSULLU_RAPORU.md`, İngilizce özet sonunda):**
- Tam çekirdek sıralı sıfırlarda ilk kez doğrudan ölçüldü: M20 1,69×10⁻¹³, Mallows 1,79×10⁻¹³ (129).
- P(yeni | eski), kalibre sıra: tekdüze 3,95×10⁻² (~1/25), M20 0,164 (~1/6), Mallows 0,191; teraziye de koşullamak değiştirmez. Terazi kanıt sayılırsa (iyimser) 9,6×10⁻⁴ / 7,8×10⁻³ / 1,0×10⁻².
- Robotun kesin gerektirmeleri (27 + 2, tanımdan kanıtlı) ve tekrarsız puan; dokuzuncu denetimin deneyi yalnız iddia bataryasıyla yeniden üretildi.

**Ön-kayıt:** `ONKAYIT.md`, hiçbir koşullu puana bakılmadan 2026-10-23T02:48:18Z'de donduruldu (SHA-256 `4608859bd46f5951c8eac47a0a3901f4fac6747295fe7360d0b8ecc35ec71159`, `sonuc/onkayit_ozet.txt`). Sapmalar `ek/` altında zaman damgalı.

## Bu kopya

Oturumun ürettiği paketin (`kosullu_paket.tar.gz`, 54,5 MB, SHA-256 `3fcb920a0379d3ba2f0578470cb173f808c954a2fefed2055c3dfd440f829171`; 718 dosya) açılmış hâlidir. **Çıkarılanlar:** rastgele-kitap puan dizileri (`sonuc/**/*.bin`) ve kitap başına tam birlikte-olasılık ayrıntıları (`sonuc/**/*.tam.txt`, `*.tam.parca*.txt`); toplam ~190 MB. `paket/MANIFEST.sha256` çıkarılanlar dahil bütün dosyaların özetlerini taşır. Çıkarılan dosyalar `kos/hepsi.sh` ile aynı tohumlardan yeniden üretilir (rapor §11). Paketin kendisi araştırmacının makinesindedir.

**Yol genellemesi:** `sonuc/sha256_girdiler.txt`'teki mutlak yollar `<hrf2>/`, `GOREV.md`'deki hafıza yolu `<hrf2-proje>` olarak genelleştirildi; bu iki dosyanın özeti bu yüzden MANIFEST'tekinden farklıdır.

**Yeniden üretim:** `bash kos/hepsi.sh <adım>` (adımlar: derle, sinir, c1, o0b, o0a, havuz, u, m20, mallows, j10, kesif). Betikler oturum klasörü düzenine göre yazılmıştır; depo dosyalarına `ref/` altındaki kopyalardan erişir.
