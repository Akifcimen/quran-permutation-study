# EK — raporlama ve yeniden üretim için dondurmadan sonra eklenen betikler

Hiçbiri bir tanımı, ağırlığı, eşiği, bedeli ya da kuralı değiştirmez; hiçbir sonuç dosyasını değiştirmez. Hepsi ya ön-kaydın
raporlanmasını istediği bir şeyi sayar ya da yeniden üretimi kolaylaştırır.

- `src/el_sinif.py` — el dilinde sınıf kuralının betimsel özeti (ön-kayıt §9 a ile §9 c'nin kesişimi). Kur'an'ın ve plaseboların el
  dilindeki p ve p^c değerleri, sınıftaki plasebo sayısı. **Ön-kayıtta kalibrasyon olarak tanımlı değildir**; raporda "betimsel ek"
  etiketiyle verilir. Girdi yalnız `sonuc/<sıfır>/analiz.json`.
- `kontrol/aday_siniri.c` — ön-kayıt §4.3'ün "uygulandığı hedef-mercekler raporlanır" yükümlülüğü: D'nin grup başına 24 aday sınırının
  hangi (hedef, mercek, grup) üçlülerinde devreye girdiğini sayar. Girdi `tab/<sıfır>/hedef.bin` ve `diller.txt`.
- `kos/derle.sh` — bütün C programlarının derleme komutları. Oturumda kullanılan ikililerin derleme bayrakları kaydedilmemişti; bu betik
  onları yeniden kurar. Yeniden derlenmiş ikililerin çıktıları oturumun sonuç dosyalarıyla `kos/yeniden_sina.sh` ile karşılaştırıldı
  (rapor §11; sonuç `sonuc/yeniden_derleme.txt`).
- `kos/yeniden_sina.sh` — yukarıdaki karşılaştırma (Ö0'ın Kur'an kısmı, hedefler, D, hedef ayrıntısı, bir kontrol bölütü, küçük bir
  referans geçişi, bilgi çapası).

**Sonuca etkisi.** Yok.
