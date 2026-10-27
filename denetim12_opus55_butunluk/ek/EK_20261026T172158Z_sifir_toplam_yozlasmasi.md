# EK — sayım puanında "sıfır toplam" yozlaşması ve sonradan duyarlılık tanımı

**Zaman:** 2026-10-26T17:21:58Z. Ön-kayıt 2026-10-26T17:13:24Z'de dondurulmuştu.

**Gözlem (sonuçlara bakarak):** tekdüze referans bölütü 1'de (5×10⁶ kitap) sayım puanı en yüksek kitaplar β < 0 ve büyük p
merceklerinde ((1,−1,29…61), (1,−3,19/43)) toplanıyor. Nedeni: t = 0 olan konumlar (biçim (1,−1)'de a_n = n) 0 her asala
bölündüğü için **her** p'nin ailesine girer (q = 0); büyük p'de aile çoğunlukla q ∈ {−1, 0, 1} üyelerden oluşur ve hücre/yarı
toplamları 0 olur. Bu durumda F2, R0e, R1e ("0 = 0"), QS, Qh ("p | 0") kendiliğinden tutar. Kapı G (k_F ≥ 2E) büyük p'de 2E
küçük olduğu için bunu yakalamaz. Bu bir kod hatası değil, ön-kayıtlı tanımın öngörülmemiş bir yozlaşmasıdır.

**Ne yapılacak:**
1. Ön-kayıtlı sayım puanı (kapı G ve öteki kapılar) **olduğu gibi** raporlanır; sırası, en güçlü rakipleri ve ön-kayıtlı sınıflandırması
   değiştirilmez.
2. **Sonradan duyarlılık (S0), bu ekte, sonuçlarına bakılmadan önce tanımlanır:** sayım puanında toplamı sıfır olan karşılaştırmalar
   kredi almaz — F2 (Σq(F⁰) = Σq(F¹) = 0 ise), R0e (s00 = s01 = 0 ise), R1e (s10 = s11 = 0 ise), QS (Σq(F) = 0 ise), Qh (yarılardan
   biri 0 ise), QH (Σq(H) = 0 ise). Başka hiçbir şey değişmez (kapı G, kurallar, öteki yüklemler aynı). S0 ≤ C olduğundan S0'ın sırası
   ve en güçlü rakipleri, ön-kayıtlı aday dosyalarından (C ≥ C_Q − 2) tam olarak hesaplanır. Kur'an'ın toplamlarında sıfır yoktur
   (S0 = 11). Aynı duyarlılık menü analizine (§8.4) de uygulanır.
3. S0 bir **sonradan** analizdir; hiçbir karar kuralında birincil sayının yerine geçmez. Birincil puan ön-kayıttaki gibi bilgi-ağırlıklı
   puandır; onun bu yapıları aile boyuna koşullu olasılıkla nasıl fiyatladığı rakip incelemesinde gösterilir.
