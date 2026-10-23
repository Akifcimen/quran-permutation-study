# EK — Mallows 129 O havuzu bütçe tavanında kaldı (yazılış: 2026-10-23T07:07:12Z)

**Tür:** bütçe kısıtı (ONKAYIT §4.3: "Tavana ulaşılırsa yeni bölüt başlatılmaz; tamamlanmış bölütlerle ulaşılan N yazılır").

- Ön-kayıtlı tavan: 16 çekirdek-saat (tamamlanan bölütlerin `.say` dosyalarındaki süre toplamı). Hedef: 100 bölüt × 1.000 = 10⁵ O-kitap.
- İki sürücü (ileri: 02:48Z'den, eşzamanlı 3 bölüt; ters: 05:00Z'den, eşzamanlı 2 bölüt; ortak tavan) tavana 2026-10-23T07:01:32Z'de
  ulaştı; o andan sonra yeni bölüt başlatılmadı, koşmakta olan bölütler tamamlandı (son bölüt 07:06:23Z).
- **Ulaşılan: 83 bölüt, N = 83.000 O-kitap** (tohumlar 3291001–3291063 ve 3291081–3291100). Başlatılmayan: 3291064–3291080 (17 bölüt).
  Toplam süre 16,86 çekirdek-saat (koşmakta olan bölütlerin tamamlanmasıyla tavanın %5 üzerinde).
- Havuz sayaçları: 38.122.652.686 Mallows örneği; O 83.000 (P(O | Mallows) = 2,18×10⁻⁶); B 20.618.960; C 1.602.594; G 2.948.
- Süre bölüt başına duvar saatiyle ölçüldü; makine paylaşımlıydı (yük ortalaması zaman zaman 100–157), bu yüzden tavan işlemci süresi olarak
  daha az işe karşılık geldi. Bölüt süreleri 582–1.142 s.
- **Etki:** yalnız örneklem büyüklüğü (Mallows sırasının k/N aralığı N = 83.000'e göre). Hangi bölütlerin tamamlandığı yalnız süreye göre
  belirlendi; puanlama havuz tamamlandıktan sonra bir kez yapıldı (ara puanlama yok); bölütler bağımsız ve tohumları önceden sabitti.

**Ek not (yazılış: 2026-10-23T07:24:09Z) — O' tohumları.** `kos/o_ussu.sh` O'-bölüt tohumunu 3292001 + b ile verir, b = tamamlanmış
O-bölütlerinin sıralı listesindeki yer (0'dan). Bütün bölütleri tamamlanan M20'de b = bölüt numarasıdır; Mallows'ta 3291064–3291080 eksik
olduğundan 3291081–3291100 bölütlerinin O' tohumları 3292064–3292083'tür (her O'-bölütünün `.say` dosyasında yazılı). Tohumlar puanlara
bakılmadan, betiğin kuralıyla belirlendi; sonuçlara etkisi yoktur.
