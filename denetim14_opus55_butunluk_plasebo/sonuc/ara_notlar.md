# Ara notlar — plasebo14

- 2026-10-29T03:51:54Z: GOREV.md okundu; SHA-256 gorev_sha256.txt ile eşleşti (f4224b6c…5b13). Bağlam okundu: README (merdiven), KESIF notları 24–26,
  denetim12 BENIOKU, denetim13 BENIOKU / PLASEBO12_RAPORU / ONKAYIT.
- plasebo12 kaynakları kopyalandı (30 dosya, sonuc/p12_kopya_sha256.txt; kopyalar özgünlerle aynı; 15'i plasebo12'nin dondurma anı
  özetleriyle de aynı). Referans tabloları (12 dosya) ve hedef dosyaları (3) plasebo12'deki yerlerinde SHA-256 ile doğrulandı (12/12, 3/3 OK);
  hedef.bin'ler tab/<sıfır>/'a kopyalandı ve kopyalar doğrulandı (3/3 OK). Referans tabloları yerinde okunacak.
- 2026-10-29T04:01:50Z: Ö0 (dondurmadan önce; plasebo12 ile eşitlik sınamaları, yalnız uyuşmazlık sonuçları): kısıtsız D (src/dil_b P kipi) üç sıfırda
  plasebo12'nin diller.txt ve ayrinti.txt'sini bit-bit verdi (M 203, U 103, L 103 satır; 0 farklı). kontrol_b (kol 0) plasebo12'nin
  düzey-1 dilleriyle bölüt 1'de esikler.txt (Kur'an'ın Q, QP2, el dili eşikleri dahil), b0001.txt sayımları (M 403, U 203, L 203 satır;
  0 farklı) ve b0001.q'yu bit-bit verdi (kos/o0_d.sh, kos/o0_k.sh → sonuc/o0/o0_d.txt, o0_k.txt).
- 2026-10-29T04:10:06Z: dondurma öncesi kod doğrulamaları (kos/dogrulama.sh; yalnız hedef olmayan kitaplar, tür 4 sıra ≥ 100; tür-8 kontroller):
  kaba-kuvvet D_B + en geniş dil 3 × 2.000 kitapta 0 uyuşmazlık (seçilen dil, liste ve mercek dahil); kontrol_b'nin bağımsız yeniden
  sayımı (16.000 kontrol, 3 sıfır) k, kB ve eşiklerde 0 uyuşmazlık; hedef_ayrinti_b çalışıyor. **Sızıntı (istenmeden):** mekanik sınama
  çıktısı sınamaya giren dil sayısını bastı (kol 1–3: M 12, U 19, L 17 / 90 kitap-kol); bu, hedef olmayan rastgele kitapların ne sıklıkta
  bütünlüklü dile sahip olduğunu kabaca gösterir. Ayrıca kaba-kuvvet kapsam bayrağı "taranan mercek ≥ 1000: hayır" (2.000 kitapta)
  kapı-G merceklerinin seyrekliğini gösterir. Hiçbir tanım bundan etkilenmez (tanımlar görevde sabit); ön-kayıtta açıklanacak.
- 2026-10-29T04:38:14Z: **ONKAYIT.md donduruldu** (SHA-256 6080c6b17c4bf38184c11482b3931d4804633ee23d190d69d252e03926e1f748; kod özetleri sonuc/onkayit_kod_sha256.txt, 50 dosya).
- 2026-10-29T04:43:22Z: diller kuruldu ve donduruldu (sonuc/diller_sha256.txt, 16 dosya). Kur'an: Kol A {B2, K1, Qc, WSum} @ (1,+1,19) üç sıfırda (4 tür,
  3 düzey; eşik M 18,80 / U 20,83 / L 20,00 bit); A− {B2, K1, Qc} (plasebo12 dili; eşik 15,947); A+ {B2, K1, Qh, WSum}; Kol B 15 yüklem
  @ (1,−1,19). Dilli plasebo: M20 A 35/200, A− 60/200, A+ 2/200; U A 18/100; L A 15/100. M20 Kol A'da dilsiz 165 plasebonun 138'inde hiç
  kapı-G merceği yok. Not: kapı G (k_F·p ≥ 228) Kur'an'da tam sınırda tutar (12·19 = 228).
- 2026-10-29T04:43:22Z: kos/akis_b.sh arka planda başladı (Kol A: M → U → L; sonra Kol B). M20 Kol A düzey 1: 100 dil, 21 s; düzey 2'ye 3 plasebo dili (A146,
  A159, A195). Dondurmadan sonra eklenen tek kod: kos/yeniden_sina.sh (oturum sonu sınaması; sonucu değiştirmez).
- 2026-10-29T05:13:30Z: **M20 Kol A bitti.** Kur'an {B2, K1, Qc, WSum} @ (1,+1,19), eşik 18,801 bit: k = 0 / 10⁸ → p_Q = 1,0×10⁻⁸ [0; 3,7×10⁻⁸]; eşiğin
  2 bit yakınında aday kontrol 0. Plasebo: K_d = 35/200, medyan 1, en küçük A195 1,1×10⁻⁶ (10/10⁷) [4,8×10⁻⁷; 1,8×10⁻⁶]; R = 0 → p_kal =
  1/201 (aşar). Model: (a) GPD tanımsız (üst %20'de dilsiz), normal 2,3×10⁻⁹ (nokta kütlesiyle bozuk); (b) karışım GPD 0 [0; 2,0×10⁻⁴],
  normal 9,2×10⁻⁸; Δ = 2,04 mertebe (p_Q çözünürlük sınırında, k = 0). Tahmin: kısmen tuttu (p_Q karar verilemedi; yarısı dilli tutmadı;
  medyan tutmadı; en küçük tuttu; hepsini aşar tuttu). A−: Kur'an {B2,K1,Qc} 0/10⁸ (plasebo12'de aynı dil 33/10⁸), K_d 60, en küçük
  4,7×10⁻⁵, R = 0. A+: Kur'an {B2,K1,Qh,WSum} 0/10⁸, K_d 2, R = 0. Kayıtlı hesap 3,0 ç-saat.
- 2026-10-29T05:13:30Z: betimsel düzeltme (sonucu değiştirmez): analiz_b.py'deki "dili plasebo12'dekiyle aynı" sayımı dilsiz-dilsiz çiftleri de sayıyordu;
  yalnız dilli plasebolar üzerinden sayılacak biçimde düzeltildi (ek yazılacak; M20 analizi sonda yeniden üretilecek).
- 2026-10-29T05:46:44Z: **Tekdüze Kol A bitti.** Kur'an aynı dil, eşik 20,830 bit: k = 0 / 10⁸ → p_Q ≤ 3,7×10⁻⁸ (aday 0). K_d = 18/100; en küçük A018 ve
  A045 4,1×10⁻⁶ (40/10⁷) [2,9; 5,4]×10⁻⁶; R = 0 → p_kal = 1/101 (aşar). plasebo12'nin tekdüzedeki çakışan plasebosu x068 burada dilsiz
  (1 kapı-G merceği, bütünlüklü alt küme yok). Model: (a) GPD tanımsız, normal 5,0×10⁻⁸; (b) GPD tanımsız (m = 4 < 5), normal 1,3×10⁻⁶;
  Δ = 2,61. Kitap kitap: 18 dilli; log₁₀(p14/p12) medyan −1,46; 15 azaldı, 3 arttı. Mallows Kol A sürüyor.
- 2026-10-29T06:19:08Z: **Mallows Kol A bitti.** Kur'an eşik 19,998 bit: k = 0 / 10⁸ (aday 0). K_d = 15/100; en küçük A047 {Zk2p, R0c, FKS, WUTd2} @ (1,+3,11)
  3,4×10⁻⁷ (33/10⁸) [2,27; 4,63]×10⁻⁷, sonra A068 6,8×10⁻⁷ (67/10⁸); aralıklar Kur'an'ınkiyle (üst 3,69×10⁻⁸) çakışmıyor → R = 0, p_kal =
  1/101 (aşar); Δ = 1,53. Model: (a) GPD tanımsız, normal 4,1×10⁻⁷; (b) GPD tanımsız (m = 3), normal 2,9×10⁻⁴. Kitap kitap: 15 dilli,
  medyan −1,62 mertebe, 14 azaldı, 1 arttı. Kayıtlı hesap 7,28 ç-saat. Kol B M20 düzey 1 başladı (kestirim 0,32 ç-saat).
- 2026-10-29T07:19:46Z: **Kol B M20 bitti.** Kur'an'ın en geniş dili 15 yüklem @ (1,−1,19) (B1 B2 FKS W1 QU Qa QH WSum WUd HH1 HH2 HR0c HR1c HR1e HK2), eşik
  31,784 bit: k = 0 / 10⁸ (aday 0). Plasebolar: 200/200 dilli (boy 5–21), medyan 8,4×10⁻⁵ [5,2×10⁻⁵; 1,15×10⁻⁴], p < 10⁻⁶: 8, < 10⁻⁷: 2.
  En yakın: B183 2,0×10⁻⁸ (1/10⁸) [2,5×10⁻¹⁰; 5,6×10⁻⁸] ve B087 8,0×10⁻⁸ (7/10⁸) [2,8×10⁻⁸; 1,4×10⁻⁷] — aralıklar çakışıyor → R = 2,
  p_kal = 3/201 = 0,0149 (arada). Model: GPD 7,4×10⁻⁴ [0; 5,3×10⁻³], normal 1,9×10⁻⁴; Δ = 0,30. Kitap kitap: 188/200 plasebonun p'si
  plasebo12'ye göre küçüldü (medyan −2,22 mertebe). Düzey 3'e 9 plasebo dili indi. Kayıtlı hesap 11,28 ç-saat; Kol B U ve L başladı.
- 2026-10-29T08:09:43Z: **Tekdüze Kol B bitti.** Kur'an aynı 15 yüklem @ (1,−1,19), eşik 35,814 bit: k = 0 / 10⁸ (aday 0). Plasebolar 100/100 dilli,
  medyan 9,9×10⁻⁵ [4,3×10⁻⁵; 2,3×10⁻⁴]; Kur'an'ın aralığına girenler B080 2,0×10⁻⁸ (1/10⁸; çözülmüş) ve B068 5,0×10⁻⁸ (4/10⁸; W araması
  çözülmemiş) → R = 2, p_kal = 3/101 = 0,0297 (arada). Model: GPD 8,8×10⁻⁴ [0; 8,9×10⁻³], normal 6,8×10⁻⁴; Δ = 0,30.
- 2026-10-29T08:58:36Z: **Mallows Kol B bitti; akış bitti.** Kur'an eşik 31,769 bit: k = 0 / 10⁸ (aday 0). Medyan 1,8×10⁻⁴ [9,5×10⁻⁵; 3,9×10⁻⁴];
  B090 0/10⁸ (Kur'an'la eşit, çözülmüş) ve B061 3,0×10⁻⁸ (2/10⁸; F araması çözülmemiş) → R = 2, p_kal = 3/101 (arada); Δ = 0,00.
  Kayıtlı hesap 17,86 ç-saat.
- 2026-10-29T08:59:56Z: M20 Kol A kümesinin analizi (A, A−, A+) betimsel düzeltmeyle yeniden koşturuldu: "dili plasebo12'dekiyle aynı"
  14 / 13 / 16 → 0 / 0 / 0; JSON'larda değişen tek alan bu. Düzeltme öncesi çıktılar sonuc/M/duzeltme_oncesi/'de.
- 2026-10-29T09:02–09:09Z: kos/yeniden_sina.sh (yeniden derlenmiş ikililer): 16/16 AYNI (sonuc/yeniden_sina_rapor.txt). Dondurma anı kod özetleri
  49/50 OK (farklı: src/analiz_b.py, düzeltme); ONKAYIT.md özeti aynı; diller 16/16 OK; plasebo12 tabloları 20/20, hedefler 3/3 OK; salt
  okunur beş dizinde oturum başından sonra değişen dosya yok.
- 2026-10-29T09:13Z: ekler yazıldı (ek/EK_20261029T0905Z_betimsel_duzeltme.md, ek/EK_20261029T0913Z_dondurma_sonrasi_kod.md); hesap_ek.txt'e
  dillerin kuruluşu, analizler ve oturum sonu sınaması eklendi → kayıtlı 18,10 ç-saat. src/rapor_tablo_b.py yalnız sayı biçimi için güncellendi.
- 2026-10-29: PLASEBO14_RAPORU.md yazıldı (Türkçe; İngilizce özet). Okuma: Kol A üç sıfırda R = 0 → ilk cümle (p_kal 1/201, 1/101, 1/101);
  A−/A+ aynı; Kol B üç sıfırda R = 2 → "arada" (betimsel). Tahmin: kısmen tuttu.
- 2026-10-29T09:23Z: paket kuruldu: paket/plasebo14_paket.tar.gz (5.047 dosya; referans tabloları, ikililer ve taslaklar hariç); SHA-256'sı
  paket/plasebo14_paket.sha256'da, içerik özetleri paket/plasebo14_icerik_sha256.txt'te (açılıp 5.046 / 5.046 doğrulandı).
