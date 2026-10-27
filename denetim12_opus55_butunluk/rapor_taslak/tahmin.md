## 8. Ana hattın önceden kaydettiği tahmin tuttu mu?

**Tahmin (ön-kayıt §1, aynen):** "nesnenin bütününün bütünlük sırası uzunluk-sıralı sıfırlarda (M20, Mallows) merkez **10⁻⁸**,
aralık **10⁻⁶–10⁻¹⁰**; tekdüze sıfırda **10⁻¹⁰–10⁻¹²**; eski literatür koşullu (yalnız bu çalışmanın eklediği) bütünlük **10⁻²–10⁻⁴**."

**Kurallar (ön-kayıt §9, puanlara bakılmadan yazıldı):** "tuttu" = nokta kestirimi tahmin aralığında; "tutmadı" = %95 aralığı
(belirsizlik aralığıyla birleşik) tümüyle tahmin aralığının dışında, ya da yalnız üst sınır varken o üst sınır aralığın alt ucunun
altında; öteki durumlar "karar verilemedi". Birincil puan bilgi puanıdır, sayım puanı aynı kurallarla ayrıca değerlendirilir. Eski
literatür parçası O ve O' ile ayrı ayrı değerlendirilir (ek `EK_20261026T171720Z_O_ve_Ousu_tahmin.md`; bu karar O/O' sonuçları
görüldükten sonra, seçim yanlılığı olmasın diye "ikisi birden" olarak yazıldı). Hükümler `src/karar.py` ile doğrudan sonuç
dosyalarından üretilmiştir.

[KARAR_TABLOSU]

**Hüküm.**
- **Uzunluk-sıralı sıfırlar — tuttu** (M20 ve Mallows, her iki puan): ölçülen sıralar 2,3–4,7×10⁻⁷, tahmin aralığının [10⁻¹⁰, 10⁻⁶]
  içinde; ancak tahminin merkezi 10⁻⁸'in **1,4–1,7 mertebe üstünde** (\|log₁₀ p̂ + 8\| = 1,36–1,67), aralığın "az nadir" ucunda.
- **Tekdüze — tutmadı** (her iki puan): ölçülen [U_BILGI] (bilgi) ve 4,5×10⁻⁷ (sayım) tahmin aralığının [10⁻¹², 10⁻¹⁰] **üstünde** —
  Kur'an'ın bütünlüğü tekdüzede tahmin edilenden 2,6–3,7 mertebe **daha az nadirdir**.
- **Eski literatür koşullu — O ile çoğunlukla tutmadı (daha nadir), O' ile tuttu.** Görevin birincil koşulu O altında (12 üyeli aile,
  1444, 6/6, 722/722, 361/361 verildiğinde) kalan katmanlar tahminden daha nadirdir (bilgi 5×10⁻⁶–4,8×10⁻⁵; Mallows bilgi puanında
  %95 aralığı 10⁻⁴'e değdiği için karar verilemedi); bunun çoğu 1994 terazisinden gelir. Tahminin kendi ifadesine ("yalnız bu çalışmanın
  eklediği") karşılık gelen O' = O ∧ terazi altında kalan katmanlar 1,0×10⁻³–3,4×10⁻³ (bilgi) ve 4,1×10⁻⁴–1,6×10⁻³ (sayım): aralıkta.
- **Bütün tahmin: kısmen tuttu.** Sıralı sıfırlar ve O' parçası tuttu; tekdüze parçası tutmadı (daha az nadir); O parçası çoğunlukla
  tutmadı (daha nadir).

J10 ve 127 tahmine dahil değildir (ikincil): J10'da bilgi 5×10⁻⁷, sayım 4×10⁻⁷; 127 için §7.1.
