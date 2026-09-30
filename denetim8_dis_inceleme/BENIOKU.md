# denetim8 — sekizinci denetim: dış inceleme, iki tur (20 Ekim 2026)

Araştırmacıya ulaştırılan iki rapor. İkisi de yayımdaki `a9bec42` sürümünü inceler; yerel olarak hazırlanmış not 16–19 paketini görmemiştir. Raporlar denetçinin kim olduğunu ya da hangi araçla çalıştığını yazmaz. İkinci rapor kendini "ikinci inceleme" diye tanıtır ve dosya yolları `audit_round2/` diye geçer; bu yüzden aynı denetçinin iki turu sayılmıştır. Raporlarda anılan deney paketleri (kod, JSON sonuçları) bize ulaşmadı. Ana hattın değerlendirmesi `KESIF.md` dürüstlük notu 21'dedir.

| Dosya | İçerik |
|---|---|
| `tur1_quran_permutasyon_incelemesi.md` | 1. tur: terazinin tam olasılığı; çekirdeğin bağımsız önem örneklemesi (127 ve 129); beşli blok modeli (kristal ×742, çekirdek ×~1.160); S1–S8 profilinin mantıksal kısıtları (S4 gereksizliği, 12 ulaşılabilir modül, KJV 1.189 bölüm kontrolünün olanaksızlığı); `kesif21b`'deki κ; eksik yeniden üretim materyali |
| `tur2_REPORT_TR.md` | 2. tur: altılı blok (altı faz karışımı, S ≤ S_obs koşullu), komşu çift modelinde kesin çekirdek, Mallows modeli, 2–8 blok genişlikleri, tek takaslar |
| `dogrulama/tam_terazi.py`, `dogrulama/tam_terazi_sonuc.txt` | Terazinin tam olasılığının bizim bağımsız hesabımız (alt-küme toplamı üreteç fonksiyonları, büyük tamsayı, ~1 sn): 127 için 3,1371149836905516×10⁻⁴, 129 için 3,140653823213589×10⁻⁴; rapordakiyle her basamakta aynı |
| `dogrulama/cift_model.py`, `dogrulama/cift_model_sonuc.txt` | Komşu çift modelinde tam çekirdeğin bizim kesin sayımımız: 15 T-biti, 7 U-biti, 35 nötr bit; G∧W'yi sağlayan tek yönelim; 127 için 2.211.747/2⁵⁵ = 6,14×10⁻¹¹, 129 için 70.169/2⁵⁰ = 6,23×10⁻¹¹; rapordakiyle aynı |

Çalıştırma (depo kökünden): `python3 denetim8_dis_inceleme/dogrulama/tam_terazi.py quran_meta.json` ve `python3 denetim8_dis_inceleme/dogrulama/cift_model.py quran_meta.json`.

## Ana hattın kısa değerlendirmesi

- **Kabul:** çekirdeğin bağımsız yeniden ölçümü (4. bağımsız türetme); terazinin tam hesabı; sıralı sıfırdaki çekirdek değerinin (≈1,5×10⁻¹³) doğrudan ölçüm değil aktarım olduğu (merdivende artık ≈10⁻¹³–10⁻¹²); S4'ün gereksizliği, 12 ulaşılabilir modül, KJV bölüm kontrolünün bilgisizliği; `kesif21b`'deki κ; eksik kırmızı takım betikleri (`kirmizi_takim/`'a eklendi); 129'un birincil sayım olması.
- **Sınırı çizilen:** blok ve çift modelleri gözlenen verinin bir kısmını elde tutar (genişlik 1'de olasılık 1'dir). Çift modeli nadirlikten çok kırılganlık ölçer. Yalnız uzunluğa ayarlı temiz test Mallows'tur; oradaki kristal ×126, `kesif25` ve 5. denetimin ×88–160'ıyla örtüşür. Kalibre basamaklar zaten sıralı sıfırlardadır; blok sıfırı rastgele kitapları Kur'an'ın kopyası yapacağından kalibrasyonda kullanılamaz.
- **Açık:** tam çekirdeğin yalnız-uzunluk modelinde (Mallows ya da M20) doğrudan ölçümü.
