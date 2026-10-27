# EK — tekdüze adanmış kestirimde S_alt sırasından sapma ve koşulmayan aramalar (§4.3)

**Ön-kayıt (§4.3):** belirsiz kitaplar adanmış kestirimi S_alt'a göre azalan sırayla alır; bütçe sıfır başına ≤ 4 çekirdek-saat;
çözülemeyen kitaplar aralık olarak raporlanır.

**Ne yapıldı (tekdüze, kodeks 129):**
- `kos/adanmis_U3.sh`'ın 1–4. maddeleri S_alt sırasıyla koşuldu (S_alt 30,75; 30,55; 28,11; 28,01 / 27,14).
- 5. madde, **b004:2427157** kitabının (2,+1,17) k = 17 aile araması, 6. maddeden önce yazılmıştı. Betiğin yorum satırı bu kitap için
  S_alt yerine nokta değerini (26,6) yazıyordu; kitabın gerçek S_alt'ı **24,93**'tür. Sıra bu yanlışla kuruldu, yani S_alt sırası bu
  kitapta **çiğnendi**: 6. maddenin iki kitabından (S_alt 26,88) önce koşuldu.
- 2026-10-26T20:05Z'de dizi durduruldu (`sonuc/kosu_gunlugu.txt` 20:05:34Z; `sonuc/ara_notlar.md` ~20:10Z). 6. maddenin beş araması
  koşulmadı: F (1,−1,3) k = 36 ve dört p = 5 iki-el araması. Bu aramalar yalnız iki kitabı ilgilendiriyordu: b002:622353 ve
  b003:3971988 (nokta 29,46; S_alt 26,88, yani eşiğin 4,2 bit altı). Maliyetleri ~1,7 çekirdek-saatti; bu süre M20 ve Mallows adanmış
  kestirimlerine ve menüye ayrıldı. Tekdüzenin adanmış toplamı ~1,7 çekirdek-saatte kaldı (tavan 4).
- 2427157'nin araması dondurulmuş bir alt süreçti. Bu süreç sonlandırıldı ve aynı tohumla (26142600006) iş kuyruğundan baştan koşuldu:
  2×10⁷ öneri, 3,2 dk, 581 isabet, P̂ = 2,92×10⁻⁵. Sonuç: 27,59 [27,48; 27,71] → ALTTA.

**Etkisi:** yalnız k_üst değişir: 16.593 → 16.592. k_alt (2) ve k_nokta (3) değişmez. Koşulmayan iki kitap BELİRSİZ olarak kalır: üst
sınırları ∞ olduğundan k_üst'e girerler; noktaları S_Q'nun altında olduğundan k_nokta'ya girmezler. Sıra kuralına uyulup bu iki kitap
çözülseydi sayılar en çok k_alt ≤ 4, k_nokta ≤ 5, k_üst ≥ 16.590 kadar değişebilirdi. Bu, p_nokta'yı en çok 4×10⁻⁸'den 6×10⁻⁸'e
taşıyabilirdi; tahmin hükmünü değiştirmez (§9: tekdüze aralığı [10⁻¹², 10⁻¹⁰], ölçülen 4×10⁻⁸).
