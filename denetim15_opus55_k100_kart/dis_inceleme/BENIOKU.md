# Dış inceleme — 15. denetim (`9a3268a`), 1 Kasım 2026

Araştırmacı, başka bir yapay zekâ ekibiyle yaptığı bir incelemenin özetini ana hatta iletti. İnceleme, yayımdaki `9a3268a` sürümünü ve ondan önceki `5ef087d` düzeltmesini okudu. Özet, `INCELEME_OZETI.md`'de aldığı biçimiyle duruyor. Tam rapor bize ulaşmadı. Özetteki özel kitaplık bağlantıları çıkarıldı, başka bir şeye dokunulmadı.

## Ana hattın değerlendirmesi

**Doğrulananlar.** Aşağıdakiler bağımsız olarak yeniden hesaplandı:

1. **Puana ulaşmak yapıyı taşımak değildir.** Kur'an'ın Gc 10¹⁰ sayımında puanına ulaşan 81 kontrol var (M20 47, Mallows 22, tekdüze 12). Bunların yalnız 12'si (9 + 2 + 1) dilin dört yüklemini birden taşır. Puan, dilin o mercekte tutan alt kümesinin bilgisidir: B2 + WSum + (K1 ya da Qc) eşiğe yetebilir.
   - Kur'an'ın merceğindeki on bir olgunun (dört dil yüklemi ve B1, 3/3/3/3, K2, W1, QU, Qa, FH2p) hepsini taşıyan kontrol yoktur.
   - Tam profili taşıyan bir kontrol dört yüklemi de taşıyacağından isabetler arasında görünürdü. Öyleyse 3×10¹⁰ kontrolün hiçbirinde, hiçbir mercekte bu profil yoktur. Ham üst sınır sıfır başına ~3,7×10⁻¹⁰'dur.
   - Kaynak: `isabet_yeniden.py`, `isabet_yeniden_cikti.txt`.
2. **M20 41379 karşıörneği.** Mercek (n + a, 19), puan Kur'an'ınkiyle aynı, aileler 12 ve 7, eller ayrık, hücre toplamları 20/18/19/19. Ama:
   - parite sayımı 55/59;
   - hücreler 4/2/3/3;
   - katsayılar 3, 4, 5, 6, 9, 11, 12, yani boşluklu;
   - birleşimin ayet toplamı 1215, 19'a bölünmez.
3. **Mallows 94893.** Aileler 12 ve 7, ama bir ortak üye var; birleşim 18 sûredir. WSum yalnız k_F + k_H = p'ye baktığı için yine doğrudur. Ellerin ayrık olması (W1) ve birleşimin p üyeli olması (QU) ayrı yüklemlerdir.
4. **Paketteki kusur.** `kart/src/kontrol_h.c` yarış düzeltmesiz eski kopyaydı (`orn_init` kilitsiz). Ana hat bu dosyayı Mac'teki eşdeğerlik sınamaları için kopyalamış, orada yalnız tek iş parçacığıyla çalıştırmıştı. Kartın kabul sınamaları ise iş istasyonundaki düzeltilmiş çıktılarla yapıldı; yayımlanan sayılar etkilenmez. Dosya, düzeltilmiş sürümle (`yaris`, SHA-256 `33f62601…`) değiştirildi.

**Kabul edilen ifade düzeltmeleri:**
- **Isınma raporu:** "yeterlidir" ifadesi yumuşatıldı. İncelenen göstergelerde etki görülmedi; nadir kuyruk (9 / 6 / 5 isabet) için kesin sınır elde edilmedi.
- **Kesinlik hedefi:** K×100'ün ön-kayıtlı ±%30 kesinlik hedefi sağlanmadı. Beş isabette bağıl hata ~%45, tek isabette ~%100'dür.
- **Sınıflandırma belirsizliği:** Kur'an'ın sınırındaki sınıflandırma kesin değildir; Kur'an 47, plasebolar 44, 48, 50 isabet. π̂'nin aralığı bu belirsizliği içermez.
- **WSum'un tarifi:** README'de, not 27'de ve 14. denetimin BENIOKU'sunda "iki elin birlikte tam 19 sûre tutması" olarak geçiyordu. Doğrusu "iki ailenin boyları toplamı 19"dur.
- **Değerlendirme metni:** "Kur'an'ın bütünlüğüne ulaşan kitaplar" yerine "Kur'an'ın dilinde onun puanına ulaşan kitaplar" yazılmalı. Bu kitapların çoğunun yapıyı taşımadığı da eklenmeli.

**Ölçünün sınırı (kabul).** Placebo kalibrasyonunun cevapladığı soru şudur: bu örüntü arama yordamı rastgele veride ne kadar uç bir puan üretir? Bu, araştırmacının sorusuyla aynı soru değildir: Kur'an'daki ilişkilerin birlikte bulunması, aynı türden ilişkileri koruyan bir karşılaştırmada ne kadar nadirdir? İkinci soru ölçülmedi.
- Bütünlük kuralının "herhangi üç düzey"i kitap düzeyini şart koşmaz.
- Aynı toplam iki düzey adı altında sayılabilir. Örneğin A7192'de F ∪ H = F'dir ve 578 hem aile hem birleşim düzeyinde sayılır.

**Sonraki çalışma için kabul edilen ilkeler:**
1. İlişkileri koruyan bir ölçü.
2. Zorunlu sonuçların ve aynı olgunun iki adla görünmesinin iki kez sayılmaması.
3. Önceden yazılmış ve bedeli ödenen bir kural menüsü.
4. Taze plasebolar. Bugünkü 40.000 plasebo artık tasarım sırasında görülmüş veridir.

Önerilen beşinci iş (tam olayın olasılığını genel motorda koşullu tamamlamayla ölçmek) ikincil görüldü. Tam olayın ham sıklığı zaten doğrudan ölçülmüştür (10⁻¹³ – 10⁻¹⁵). Eksik olan onun kalibrasyonudur.
