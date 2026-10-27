## 6. En güçlü rakipler — Kur'an'ın türünden mi, dağınık denk gelme mi?

Tam tablolar (her sıfır × puan, ilk 20; mercek, profil, k_F, k_H, \|U\|, hücre sayıları ve toplamları, en büyük \|q\|, sağlanan yüklemler,
ön-kayıtlı sınıf): `sonuc/rakipler/<sıfır>_{bilgi,sayim}.txt`. **Kur'an sınıfı** (ön-kayıt §7, önceden yazılmış): en iyi mercekte kapı G
tutar ve kredi alan yüklemler 4 motif türünün ≥ 3'ünden, 4 düzeyin ≥ 3'ünden gelir. Kur'an bu sınıftadır (4 tür, 4 düzey).

| Sıfır, puan | ilk 20'de ön-kayıtlı sınıfta | baskın yapı |
|---|---|---|
| Tekdüze, bilgi | 2 / 20 | tek nadir olay: aile boyu = p (10/20 QkF), kitap düzeyinde çapraz denge (B2, (1,±3)), nadir k bağlamında zengin maske |
| Tekdüze, sayım | 20 / 20 | **yozlaşmış**: büyük p (29–79), 8 üyeli 2/2/2/2 aileler, hücre toplamlarının hepsi 0 (16/20) ya da küçük ve eşit |
| M20, bilgi | 1 / 20 | aynı tür tek olaylar; biçim (1,−1) baskın (11/20); 6/20 tamamen çözülmüş |
| M20, sayım | 20 / 20 | (1,+1,p) büyük p'de 2/2/2/2, toplamlar eşit ve sıfırdan farklı (8,8,8,8 / 6,6,6,6); ve **Kur'an'ın merceğinde** 3/3/3/3 aileler |
| Mallows, bilgi | 2 / 20 | aynı tür tek olaylar; 15/20'de bir boy olayı (k_F, k_H ya da \|U\| = p); biçim (1,−1) baskın (13/20) |
| Mallows, sayım | 20 / 20 | M20 ile aynı: (1,+1,p) eşit toplamlı 2/2/2/2, (1,+1,19)'da 3/3/3/3 (3/20), birkaç sıfır-toplamlı |
| J10 (ikincil), bilgi | 1 / 20 | aynı tür tek olaylar |
| J10 (ikincil), sayım | 20 / 20 | aynı desen (sıfır-toplamlı, eşit toplamlı 2/2/2/2, bir (1,+1,19) 3/3/3/3) |

### 6.1 Bilgi puanı (birincil)
**Tekdüze.** Tepedeki kitaplar puanlarını çoğunlukla bir ya da iki nadir olaydan alır, Kur'an'daki gibi dört düzeye yayılmış bir
yeniden-belirme ağından değil:
- **37,1 bit** [36,7; 37,5] (adanmış kestirimle; ilk aşamada 34,99 [30,75; ∞]) — (3,−1,17): **k_F = 17 = p** (QkF; P(k_F = 17) ≈ 2,4×10⁻⁴),
  k = 17 bağlamında {R1c, K2} (3,7×10⁻⁶, adanmış kesin kestirim, 73 isabet) ve {QU, QH} (8,0×10⁻⁵, adanmış, 794 isabet), B1. Bu mercek
  ön-kayıtlı sınıf koşulunu geçer (kapı G, 3 tür, 4 düzey) ama içeriği Kur'an'ınkinden farklıdır: \|U\| = 17 = k_F (H ⊆ F), hücre sayıları
  2/5/5/5, hücre toplamları (10, 32, 23, 41) — eşit sayı yalnız bir satırda, hücre toplamlarında denge, ardışıklık ve merkez yok.
- **32,7 bit** [31,8; 34,2] — (1,+3,7): puanının ~24 biti tek bir kitap-düzeyi olaydan, (1,±3) biçiminde çapraz dengeden gelir
  (Σ_{E⁰} n = 3·Σ_{E¹} a; adanmış kestirim 5,7×10⁻⁸ — 114 isabet / 2×10⁹), üstüne k_F = p = 7, QS, Qh, W1, Qn. Kapı G tutmaz (sınıf dışı).
- Sınıftaki ikinci kitap (1,−1,19), 29,67 → adanmışla 28,82 [28,81; 28,83] (eşiğin altında): k_F = 19 = p, hücre toplamları (0, 10, 0, 9).
- Öteki belirsiz-üst kitaplar benzer: büyük-sapma tipi tek denge (B2'nin α ≠ \|β\| biçimleri), k_F ya da k_H'nin p'ye eşit olması,
  ya da nadir k bağlamında zengin bir maske.

**M20.** İlk 20'nin yalnız biri sınıfta: (1,+1,23), 27,99 [22,81; ∞] — k_F = 23 = p, hücreler 8/5/5/5, R1c K2 QkF W1 (dört yüklem;
sınıf kuralı genişliği ölçer, derinliği değil). Tepedekiler: (1,−1,19) k_F = 19 = p, 30,50 [25,33; ∞] (k_F = 19 bağlamı referansta yalnız
600 kez görülmüş; aramaları çözülemedi); (1,−1,11) 30,22 [28,56; 35,09] (B1 ∧ B2 + dört hücre toplamı eşit 8,8,8,8 — eşiğin kesin
üstünde); (1,−1,5) 29,87 [27,29; ∞] (adanmışla 27,50 [27,10; ∞], belirsiz); (1,−1,7) 29,66 (çözülmüş; k_F = 7 = p, B1 ∧ B2, K1). 20'nin 8'inde QkF (aile boyu = p), 4'ünde
B1 ∧ B2. Hiçbiri Kur'an'ın merceğinde değildir.

**Mallows.** İlk 20'nin 2'si sınıfta: (1,−1,17) k_F = 17 = p, 29,98 (çözülmüş; B1 B2 F2 QS Qh QkF W1 Qa) ve (2,+1,17) k_F = 17 = p,
29,42 [26,84; ∞] (adanmışla 27,34 [26,90; 27,78], belirsiz). Tepede (1,+1,11) 31,78 [30,63; 33,88] (k_F = \|U\| = 11 = p: QkF, QU ve Qa/Qn/QH birlikte) ve (1,−1,17) 31,59
(çözülmüş; k_H = 17 = p, K1, K2). İlk 20'nin 15'inde bir boy olayı (k_F, k_H ya da \|U\| = p) vardır. Hiçbiri Kur'an'ın merceğinde
değildir. **J10** (ikincil): ilk 20'nin biri sınıfta; tepede (2,−1,5) 27,36 [25,70; 32,23] ve (1,−1,19) 27,08 (çözülmüş; dört hücre
toplamı eşit 4,4,4,4, \|U\| = 19).

### 6.2 Sayım puanı (kapı G)
**Tekdüze: yozlaşma.** İlk 20'nin hepsi aynı türdür: büyük p (29–79), 19'u β < 0 biçimleri ((1,−1), (1,−3)), 8 üyeli 2/2/2/2 hücreli
aileler, hücre toplamlarının hepsi 0 (16/20) ya da \|s\| ≤ 6 küçük ve eşit sayılar, en büyük \|q\| 1–4. Nedeni: t = 0 konumları (ör. (1,−1)'de a_n = n) her
asalın ailesine girer (q = 0) ve büyük p'de aile q ∈ {−2…2} üyelerden oluşur; "0 = 0" ve "p \| 0" dengeleri kendiliğinden tutar. İkisi 12
ile Kur'an'ı geçer. Ön-kayıtlı sınıf kuralı (kapı, tür, düzey) bunların hepsine "Kur'an sınıfı" der — **kural katsayı büyüklüğü
yozlaşmasını yakalamıyor**; bu, ön-kayıtlı sınıflandırmanın bir zayıflığıdır. Sonradan S0 (sıfır toplamlı karşılaştırmalar kredisiz;
ek, sonuçlara bakılmadan önce tanımlandı) C ≥ 11 olan 89 kitabın 66'sını 7'ye indirir; **S0 ≥ 11: 5 kitap / 2×10⁸** (→ 3×10⁻⁸): ikisi
(1,−1,19)'da 12 üyeli 3/3/3/3 aileler (toplamlar 4,4,5,3 ve 4,4,3,3; katsayılar −2…4 — yine küçük katsayılı), üçü 8 üyeli 2/2/2/2.

**M20: Kur'an'ın merceğinde Kur'an'a benzer aileler.** Sıralı sıfırda yozlaşma zayıftır (C ≥ 11 olan 59 kitabın 9'u S0'da 7'ye
düşer) ve rakipler başka bir türdendir: (1,+1,p), p = 29–43, toplamları sıfırdan farklı ve eşit (8,8,8,8; 6,6,6,6) 2/2/2/2 aileler ve
**Kur'an'ın kendi merceği (1,+1,19)'da 12 üyeli 3/3/3/3 aileler**. S0 ≥ 11 olan 17 kitabın (2×10⁸'de; → 9×10⁻⁸) **9'u (1,+1,19)'dadır**;
bunların 7'sinde k_H = 7 ve \|U\| = 19 (QU), hücre toplamları (18,18,19,17), (14,16,15,15), (16,18,17,17), (20,20,21,19) …; 8'inde ardışık hücre
toplamları (K1), hepsinde dar ve kesintisiz katsayı bandı (ör. 5–7, 4–6, 4–7; K2) — Kur'an'ın aile yapısına çok yakın (`sonuc/s0/M129_S0_11.txt`). **Hiçbirinde Σq = 4p (Qc, "merkez 19") yoktur**
(Σq = 60–80; Kur'an 76): Kur'an'ı bunlardan ayıran yüklemler merkez (Qc) ve terazidir (B2; hiçbirinde yok — 57/57 (B1) ise çoğunda var). Bununla uyumlu olarak Kur'an'ın
aile-içi maskesinin k = 12'deki olasılığı M20'de 2,0×10⁻⁵, tekdüzede 2,3×10⁻⁶'dır (~9 kat; §3). Olası açıklama (ayrıca ölçülmedi):
uzunluk-sıralı bir dizilişte t = n + a düzgün değişir, 19 ailesinin katsayıları dar bir banda düşer ve hücrelerin eşit sayısı ile
ardışık toplamları kolaylaşır; Kur'an'ın (1,+1,19) ailesinin yapısının bir kısmı dizilişin sıralı olmasıyla açıklanabilir.
**Mallows'ta aynı desen:** C ≥ 11 olan 51 kitap; S0 ≥ 11: 11 / 2×10⁸ (→ 6×10⁻⁸), bunların 5'i (1,+1,19)'da 12 üyeli 3/3/3/3 aile
(k_H = 7, \|U\| = 19; Σq = 60–70), ötekiler (1,+1,p) eşit toplamlı 2/2/2/2; yine hiçbirinde merkez yok. **J10'da** S0 ≥ 11: 2 / 3×10⁷
(biri (1,+1,19) 3/3/3/3).

**Ana hattın bilinen rakipleri** (`tab/ana_hat_rakipleri.txt`): J10 sahte katı 8/8 (Kur'an'ın titreşimli kopyası) sayımda 10 (aynı
mercek, 3/3/3/3); dört J10 robot rakibi 2–4; dış çalışmanın "7'de Kur'an'ı geçen" üç rakibi sayımda 3–4 (kapı G'de; kapısız G0'da 6–10 —
yozlaşma kuralı amaçlandığı gibi eliyor), bilgi puanında 9,2–22,6 bit (üç birincil sıfırda; Kur'an 27,3–31,1; `sonuc/ana_hat_rakipleri_129.txt`).

### 6.3 Keşif: yalnız Kur'an-sınıfı mercekleri (sonradan; ek)
Sınıf-kısıtlı puan (yalnız ön-kayıtlı sınıf koşulunu sağlayan mercekler; sonuçlar görüldükten sonra tanımlandığı için keşiftir):
tekdüzede bilgi ≥ S_Q 1 kitap / 10⁸ (yukarıdaki 37,1 bitlik (3,−1,17) kitabı), sayım ≥ 11: 89 / 2×10⁸ (hepsi sınıfta), S0 ≥ 11: 5;
M20'de bilgi 1 / 10⁸ (27,99 [22,81; ∞], belirsiz), sayım 59, S0 17; Mallows'ta bilgi 3 / 10⁸, sayım 51, S0 11; J10'da bilgi
0 / 10⁷, sayım 11 / 3×10⁷, S0 2.
