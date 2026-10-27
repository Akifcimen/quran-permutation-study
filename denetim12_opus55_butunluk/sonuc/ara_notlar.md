# Ara notlar — bütünlük ölçümü (oturum günlüğü)

Bu dosya kayıt güvenliği içindir: oturum kesilirse kaldığı yer buradan okunur. Saatler UTC.

## 2026-10-26 ~15:35Z — başlangıç
- GOREV.md okundu. Zorunlu okumalar: hrf2 README (merdiven), KESIF.md notları 19–24, denetim10 KOSULLU_RAPORU.md,
  denetim11 BENIOKU.md; girdi/ana_hat_motif_tablosu.py (okundu: yalnız JSON/metin okur, tablo basar; ağ/yazma yok),
  girdi/ana_hat_motif_tablosu_cikti.md, girdi/RAPOR_yapisal_ayrisma.md (= depodaki rapor_yapisal_ayrisma_25ekim.md, SHA eşit).
- girdi/*.zip arşivleri açılmadan listelendi; SHA-256'ları depodaki denetim11/BENIOKU.md kaydıyla eşleşiyor
  (59d7d02d… yeniden_uretim, b4756158… nadirlik_ek). Henüz açılmadı; içlerindeki kod çalıştırılmadı.
- Makine: Apple M1, 8 çekirdek, 16 GB; diskte ~15 GB boş (görev sınırı 8 GB; hedef ≤ 2–3 GB).
- 10. denetimin oturum klasörü ~/opus5.5/kosullu duruyor; O-koşullu havuzları (tab/havuz: U129O, M129O, L129O, J129O,
  U129P, M129P, L129P) ikincil (b) analizi için yalnız okunarak yeniden kullanılabilir (bir bölüt tohumdan yeniden
  üretilip bit-eşitliği sınanacak). ~/opus5.5/plasebo ayrı bir oturumdur; kullanılmayacak.
- Klasörler: src kos kontrol sonuc ek tab paket.

## 2026-10-26 ~16:30–17:05Z — dil, puanlayıcı, doğrulamalar (ön-kayıt öncesi; hiçbir rastgele kitabın bütünlük puanına bakılmadı)
- Dil L: 20 yüklem (src/dil.py tanım, src/butunluk.h hızlı C). Kitap: B1, B2; aile-içi: F1 F2 R0c R0e R1c R1e K1 K2 Qc QS Qh;
  boy: QkF; iki el/birleşim: W1 QU QkH Qa Qn QH. Sabit yüklemler analitik olarak dışlanır (t2'de α ya da β çiftse B1; p=3 ve
  |β|=3 → QU QkF QkH Qn W1; p=3 ve α=3 → QU QkF QkH Qa W1). Tarama: dışlanmamış hep-doğru yüklem 0.
- C ↔ Python: Kur'an + 4 sıfır × 150–300 rastgele kitap + 300 O-havuzu kitabı × 406 mercek: maske/boy/kredi uyuşmazlığı 0.
- Örnekleyiciler (U, M20, Mallows, J10) denetim10'unkilerle aynı tohumda 129 ve 127'de 200.000 kitapta bit-bit aynı.
- 10. denetimin O-havuzları: U_O_129_1291001 ve M_O_129_2291001 depo koduyla yeniden üretildi, bit-bit aynı.
- Gerektirmeler: rastgele (3,5M kitap) + O-havuzu (403k) taraması + soyut kapsamlı sayım (hücre ≤ 5 üye) + K2 için açık
  katsayılı sayım + p=3 Qc uzayının kesin sayımı + K1∧Qc∧K2 altında bölüntülerin kesin sayımı (p ≤ 23) + rastgele karşı-örnek
  araması. Kanıtlı kurallar: Qc→QS, Qh→QS, F2∧QS→Qh, QkF∧QkH∧QU→Qn,Qa, K1∧Qc→F2,R1e; p≥5: K1∧Qh→F2; p=3: R1c∧K1∧Qh→F2,
  R1e∧K1∧Qh→F2; β>0: Qc∧Qh→F2; p=3∧β>0: Qc uzayının 7 ek kuralı. Çürütülen adaylar: K2 içerenlerin hepsi (sınırlı pencere
  yapıtları), F1∧R1c∧K1→R0c, F2∧R0e∧R1e→R1c vb. (hücre sınırı yapıtları).
- KUR'AN (129, kapı G): en iyi mercek (1,+1,19); sağlanan 15 yüklem: B1 B2 F1 F2 R0c R1c R1e K1 K2 Qc QS Qh W1 QU Qa;
  ayıklanan 4 (F2, R1e ← K1∧Qc; QS ← Qc; Qh ← F2∧QS) → SAYIM PUANI 11. İkinci en iyi mercek (1,−1,19): 6.
- Hız: rastgele kitap başına ~42 µs (değerlendirme + sayım).

## 2026-10-26 ~17:05–17:13Z — bilgi puanı makinesi ve ön-kayıt
- src/tablo.h (referans tabloları, bağlam, en-az, Poisson–CP sınırları), src/bilgi.h (8 değişke), src/referans.c, birlestir.c,
  tablo_kur.c, kuran_puan.c, test.c; kos/orkestra.py (tek kuyruk, ≤ 4 süreç, sürdürülebilir). Mekanik sınamalar 'K' kipiyle;
  referans tablosu puansız kipte yapısal olarak (Python ile bağımsız en-az hesabı: uyuşmazlık 0).
- Menü yapıları için kurallar soyut sayımla (eoeo, eooe, xxxx desenleri) çıkarıldı.
- ONKAYIT.md yazıldı; dondurma öncesi kod özetleri sonuc/onkayit_kod_sha256.txt.

## 2026-10-26 17:13:24Z — ÖN-KAYIT DONDURULDU
- SHA-256(ONKAYIT.md) = d213dc89332884f3973b4b2e5b33afaa2070748b95dda0d344f553eeefcfd513 (sonuc/onkayit_ozet.txt).
- Üretim: `ESZAMANLI=3 python3 -I kos/orkestra.py kos/plan_ana.txt` (3 süreç; 4. yuva geliştirme için). Günlük: sonuc/kosu_gunlugu.txt.

## 2026-10-26 ~17:20Z — (b) eski literatüre koşullu (src/kosullu.c; 10. denetimin havuzları; mercek (1,+1,19) sabit)
- Kur'an kalan sayım: O'da 8, O''de 6. Bilgi: O'da 25,1 bit (U), 22,4 (M20), 22,1 (Mallows); O''de 13,6 / 11,5 / 11,3.
- Sıra (bilgi, nokta): O — U 5,0e-6 (k=0/2e5; belirsizlikle ≤ 1,5e-5), M20 2,0e-5 (1/1e5), Mallows 4,8e-5 (3/83000), J10 5e-5 (0/2e4).
  O' — U 1,02e-3 (106/104514), M20 3,29e-3 (495/150660), Mallows 3,42e-3 (433/126963).
  Sayım (sabit mercek): O — U 0/2e5, M20 1/1e5, Mallows 0/83000; O' — U 4,1e-4, M20 1,6e-3, Mallows 1,6e-3. Serbest mercek (sayım) O' ~3e-2.
- EK: O/O' belirsizliği sonuç görüldükten sonra kayda geçti → tahmin iki okumayla ayrı değerlendirilecek.

## 2026-10-26 ~17:25Z — menü programı; sayım puanında yozlaşma gözlemi
- src/menu.h + menu.c (bölme menüsü, sayım): (t2,n2) kredileri ana puanlayıcıyla 20.000 M20 kitabında eşit; 20 çift × 406 mercek
  Python tanımıyla (Kur'an + 24 rastgele kitap) eşit. Kur'an: menüde 11 (t2,n2); t2'siz en iyi 9 ((n2,a2) köşegen, (1,+1,19)).
- GÖZLEM (sonradan, U129 referans bölüt 1): sayım puanında en yüksek tekdüze rakipler β<0, büyük p mercekleri ((1,−1,29..61),
  (1,−3,19/43)): t = 0 (a_n = n) konumları her ailede (q = 0); q ∈ {−1,0,1} simetrik ⇒ bütün toplamlar 0 ⇒ F2, R0e, R1e, QS, Qh
  kendiliğinden; kapı G (2E küçük) yakalamıyor. 5×10⁶ kitapta C ≥ 10: 46, C = 11: 1 (Kur'an'la eşit). Ön-kayıtlı sayım puanı
  olduğu gibi raporlanacak; ek olarak açıkça SONRADAN etiketli duyarlılık (sıfır toplamlı denge/bölünebilirlik yüklemleri dışlanmış)
  adaylardan hesaplanacak (C' ≤ C olduğundan adaylar yeterli). Bilgi puanının bunu nasıl fiyatladığı ayrıca gösterilecek.

## 2026-10-26 ~17:35Z — adanmış kestiriciler
- src/adanmis_u.c (tekdüze, kesin koşullu önem örneklemesi; p ≥ 11): Kur'an'ın aile maskesi (1,+1,19), K = 12:
  pilot 2e6 öneri 6 isabet (2,98e-6); üretim 2e7 öneri (tohum 26142600002) 46 isabet → P̂ = 2,31e-6 (SE 3,4e-7) → 18,73 bit.
  Tekil sıklıklar (K = 12): F1 0,2258, F2 0,0180, R0c 0,1679, R1c 0,1680, R1e 0,0257, K1 0,00139, K2 0,0604, Qc 0,0277,
  QS 0,0523, Qh 0,00324 — referans tablosuyla çapa karşılaştırması yapılacak. sonuc/adanmis/U129_kuran_F_1_1_19_K12.txt
- src/adanmis_s.c (tek mercekli doğrudan örnekleme; U/M/L/J): degerlendir() ile 4 sıfır × 2 mercek × 20000 kitapta eşit.
  Pilot M20 (tohum 26242600001, 2e7 kitap): P(kF=12) = 1,2%; P(Kur'an F-maskesi | K=12) = 3,3e-5 (8/239886).
- src/s0.c (sonradan S0 duyarlılığı): U129 bölüt 1'de C=11 rakibi S0'da 7'ye iner; S0 = 10 kalan 4 kitaptan biri gerçekten yapılı
  ((1,+3,19): 12 üye, 3/3/3/3, 76/76, 38/38, 19|152), ötekiler küçük katsayılı (q ∈ {−2..2}) yarı-yozlaşmış.

## 2026-10-26 17:43Z — tekdüze tablo; Kur'an'ın bilgi puanı
- U129 tablosu (N_R = 1e8). Kur'an (V0,P0) geçici 30,962 bit @ (1,+1,19): B 11,626 + F [çözülmemiş: 2/1.014.579] + W 8,023 − bedel 7,317.
- Çapa: adanmış kesin kestirici ↔ referans tablosu, (1,+1,19) k=12'de 11 maske: |z| ≤ 1,62 (Kur'an maskesi z = −0,23). TUTTU.
- Nihai eşik (adanmış 2,307e-6 → 18,725 bit): V0P0 = 31,057 bit (sonuc/esik_nihai_U129.txt; bütün değişkeler +0,095).
- Menü koşusu 4. yuvada başladı (kos/menu_kos.sh; U, M, L × 10 × 1e6).

## 2026-10-26 ~18:00Z — tekdüze sınama ilk bölütleri; belirsizlik; R2
- T_U129 bölüt 1 (5e6): bilgi v0 k_alt 0, k_nokta 0, k_üst 797 (799/801 belirsizin üst sınırı ∞: referans diliminde hiç görülmemiş
  aile/iki-el maskeleri; S_alt medyanı 8,5 bit, en büyük 24,6; S_nokta en büyük 26,3; eşik 31,06). 815 farklı tehlikeli arama,
  neredeyse hiç paylaşım yok; çoğu küçük p. Hepsini adanmış örneklemeyle çözmek bütçeyle olanaksız → ön-kayıt gereği aralık +
  S_alt sırasıyla en yakınların adanmış çözümü.
- Aday dosyası bölüt başına 110 MB çıktı → pay 6→2 bit (EK_..._aday_payi.md); ilk 4 bölütün adayları sonradan süzülecek.
- R2 (sonuc/kuyruk_U129.txt): bilgi v0, doğrulama (en yüksek 4 eşik, 25,75–26,5 bit, 30–45 doğrudan isabet): oran 0,55–0,65 →
  DOĞRULANMIŞ. R2 @ S_Q = 31,057: ham 1,68e-8; kalibre 2,58e-8 – 3,09e-8 (EK_..._R2_bant_yonu.md). Sayım: R2 ↔ doğrudan c=6..10 oran 1,00.
- Sayım (kapı G, tekdüze, R 1e8 + T 3e7): C ≥ 11: 59 (4,5e-7); bir kitap 12 (Kur'an'ı geçti).

## 2026-10-26 ~18:12Z — tekdüze rakipler; ekler
- Aday dosyası sorunu asıl olarak G0 sayım ölçütünden (EK_..._aday_G0.md); ikili 18:03Z'de değişti, eski adaylar süzüldü (~1500/bölüt).
- Sayım (kapı G) tekdüze ilk 20: hepsi büyük p, β<0, 8 üyeli 2/2/2/2 aileler, hücre toplamları 0 ya da eşit küçük; maks|q| 1–4;
  ikisi 12 (Kur'an'ı geçer). Ön-kayıtlı sınıf kuralı hepsine "evet" diyor (kural katsayı büyüklüğü yozlaşmasını yakalamıyor).
- Bilgi (v0) tekdüze ilk 20: hiçbiri Kur'an sınıfında değil. 1.: 32,72 [31,79; 34,20] bit @ (1,+3,7) — Kur'an'ı GEÇER (alt sınır bile);
  puanı çoğunlukla tek nadir kitap-düzeyi olayından (B2 @ (1,±3): ~5e-8 ≈ 24 bit) + kF = p (7) + QS, Qh, W1, Qn.
- Belirsizlerin en üstündekiler için adanmış dizi (kos/adanmis_U.sh, tohum 26142600003…11) 4. yuvada; menü duraklatıldı (U 4/10 bölüt).
- KEŞİF eki (EK_..._sinif_kisitli_kesif.md): yalnız Kur'an-sınıfı merceklerinde puanla sıra; src/sinif.c.

## 2026-10-26 ~18:45Z — tekdüze tamam (R 1e8 + T 1e8)
- Sayım (G): k = 89 / 2e8 → 4,5e-7 [3,6–5,5e-7]; GE 7,6e-7; Gc 8,1e-7; G0 1,34e-4. S0 (sonradan, sınıf-kısıtlı) 5/2e8.
- Bilgi v0 (nihai S_Q 31,057): k_alt 1, k_nokta 3, k_üst 16.596 → p_nokta 4e-8 [2e-8; 1,66e-4]. R2 tam örneklemle DOĞRULANMADI (oran
  0,49–0,67; 0,49 < 0,5): kararda kullanılmaz (ham 1,68e-8).
- Adanmış: B2 @ (1,±3) 114/2e9 = 5,7e-8; W @ (1,+1,19): K=6 maske 37: 45/3,29e8 = 1,37e-7; K=7 maske 13: 35/2,82e8 = 1,24e-7.
  İki adanmis_u koşusu argüman denetimi hatasıyla düştü (argc < 10 → < 9 düzeltildi); günlükteki "bitti" satırları yanıltıcıydı;
  öncelik sıralı tek dizi kos/adanmis_U3.sh ile yeniden koşuluyor (aynı tohumlar).
- Keşif (sınıf-kısıtlı) tekdüze: bilgi ≥ S_Q 1 (nokta 34,99; çözülmemiş aramalar adanmışta), sayım 89, S0 5.

## 2026-10-26 ~19:55Z — M20 tamam (R 1e8 + T 1e8; 3,55 çekirdek-saat sınama)
- Sayım (G): k = 59 / 2e8 → 3,0e-7 [2,2–3,8e-7]; GE 1,3e-6; Gc 1,75e-6; G0 1,61e-4. S0 (sonradan; bütün mercekler) ≥ 11: 17 / 2e8
  (9e-8; biri 12). M20'de sayım rakipleri tekdüzedekinden farklı: (1,+1,p) büyük p'de 8 üyeli 2/2/2/2, hücre toplamları eşit ve
  sıfır değil (8,8,8,8 / 6,6,6,6); üçü (1,+1,19)'da 12 üyeli 3/3/3/3 (ör. 18,18,19,17; |U| = 19, QU) — Kur'an'ın yapısına çok yakın.
  İlk 20'nin hepsi ön-kayıtlı sınıfta (20/20).
- Bilgi v0 (S_Q 27,408; Kur'an'ın M20 aramalarının hepsi çözülmüş): k_alt 14, k_nokta 25, k_üst 18.170 → p_nokta 2,6e-7
  [1,5e-7; 1,82e-4]. Değişkeler: v1 5,3e-7; v2 1,2e-7; v3 2e-7; v4 3e-8; v5 6e-8; v6 k = 0 (U95 3e-8); v7 3e-8.
- Bilgi ilk 20: 1'i sınıfta ((1,+1,23): k_F = 23 = p, 8/5/5/5; R1c K2 QkF W1), 19'u dağınık. Keşif (sınıf-kısıtlı): bilgi 1 (nokta
  27,99, belirsiz), sayım 59, S0 17.
- M20 adanmış dizisi (kos/adanmis_M.sh, 12 arama, 1,9e10 kitap ≈ 3,4 çekirdek-saat; tohum 26242600002…13) 4. yuvada; tekdüze dizisinin
  kalanı (U3) SIGSTOP ile donduruldu, M dizisi bitince sürecek (kos/sira_M_once.sh). Tohum numaralanması eki: EK_..._adanmis_tohumlari.md.
- Tekdüze belirsizlerin yeniden puanlanması (sonuc/belirsiz/U129_coz.txt; src/sira_duzelt.py): k_alt 2, k_nokta 3, k_üst 16.593 →
  p_nokta 4e-8 [3e-8; 1,66e-4]. (3,−1,17) kitabı adanmışla 37,10 [36,67; 37,53] ÜSTTE; (1,−1,11) 30,78 [30,42; 31,13] BELİRSİZ (nokta
  altta); (2,+1,19) 31,56 [30,62; 33,04] BELİRSİZ (nokta üstte; adanmış W yalnız 5 isabet / 3,3e8 bağlam, 2e9 tavanında).

- src/menu_s0.c (sonradan S0'ın menüye uygulanması; ek/EK_..._sifir_toplam_yozlasmasi.md'nin sözü): menu_degerlendir'in kopyası + çift
  başına sıfır bayrakları. Denetim (U menü bölüt 1, 1.411 aday): SIFIRSIZ=1 iken menu_degerlendir ile eşitsiz 0; birincil çift S0'ı
  src/s0 ile eşitsiz 0. src/s0_sinif_ozet.py, src/sira_duzelt.py, src/aday_cek.py, src/ayrinti_yaz.py, kos/duraklat.sh (kısa analizlerde
  çalışan adanmış süreci SIGSTOP'la durdurup 4-süreç sınırını korur) eklendi.

## 2026-10-26 ~20:10Z — öncelik kararları
- Tekdüze adanmış dizisi (U3) durduruldu: kalan beş arama (F_1m1_3_K36 ve dört W p=5) yalnız iki kitabı ilgilendiriyor (622353 ve
  3971988: nokta 29,46, S_alt 26,88 — eşiğin 4,2 bit altında; çözülse bile çoğunlukla yine aralıkta kalırlar); ~1,7 çekirdek-saat
  M20/Mallows adanmış kestirimine ve menüye ayrıldı. Ön-kayıt bütçeyi tavan olarak koyar (≤ 4 çekirdek-saat/sıfır); çözülmeyen kitaplar
  aralıkta raporlanır (§4.3). F_2p1_17_K17 (2427157; 2e7 öneri) dondurulmuş çocuk olarak M dizisinden sonra tamamlanacak.
- Sonuç dosyaları: sonuc/sira/ (sıra özetleri), sonuc/rakipler/, sonuc/s0/, sonuc/sinif/, sonuc/belirsiz/, sonuc/menu/,
  sonuc/ana_hat_rakipleri_129.txt. Rapor taslakları rapor_taslak/ (rakipler.md, sinirlar.md, yeniden.md eklendi; rakipler_U.md kaldırıldı:
  "tekdüze bilgi ilk 20'de sınıfta 0" cümlesi son tabloyla yanlıştı — doğrusu 2/20).

## 2026-10-26 20:08Z — Mallows tablosu; Kur'an'ın Mallows puanı
- L129 tablosu (N_R = 1e8). Kur'an (V0,P0) 27,268 bit @ (1,+1,19): B 11,110 + F 15,461 (k = 12 bağlamında n = 1.308.370) + W 8,014 − bedel
  7,317; bütün aramalar çözülmüş (nihai = geçici). Sayım 11 (her kapı). Sınama bölütleri (TL129) başladı.

## 2026-10-26 ~20:50Z — Mallows tamam (R 1e8 + T 1e8; 3,38 çekirdek-saat sınama)
- Sayım (G): k = 51 / 2e8 → 2,6e-7 [1,9–3,4e-7]; GE 1,73e-6; Gc 2,42e-6; G0 1,47e-4. S0 ≥ 11: 11 / 2e8 (6e-8); 5'i (1,+1,19)'da 12 üyeli
  3/3/3/3 (k_H = 7, |U| = 19; Σq 60–70, hiçbiri 76 değil), ötekiler (1,+1,p) 2/2/2/2 eşit toplamlı.
- Bilgi v0 (S_Q 27,268): k_alt 34, k_nokta 44, k_üst 16.152 → p_nokta 4,5e-7 [3,5e-7; 1,62e-4]. Değişkeler: v1 7,4e-7; v2 2,4e-7;
  v3 3,8e-7; v4 4e-8; v5 6e-8; v6 2e-8; v7 4e-8. Belirsizlerde Δ medyanı 18,7 bit; Δ ≤ 1: 5, Δ ≤ 3: 31.
- Rakipler: bilgi ilk 20'de sınıfta 2/20; sayım 20/20 (tekdüze ve M20 ile aynı desen). Keşif (sınıf-kısıtlı): bilgi 3, sayım 51, S0 11.
- Mallows tek-mercek örneklemesi 1,47 µs/kitap (M20'nin 2,2 katı) → 4 çekirdek-saat tavanı için arama başına kitap sayısı beklenen ≥ 30
  isabete göre (4e8–2e9); S_alt sırasında ilk 11 arama, 8,3e9 kitap ≈ 3,4 çekirdek-saat (tohum 26342600000 + sıra).
- kos/is_kuyrugu.py (küresel 4-süreç sınırı: bütün ağır süreçleri çalıştırılabilir adına göre sayar, durdurulmuşlar dahil; orkestra
  yaşarken ona 3 yuva ayırır) + kos/is_listesi_1.txt: 11 Mallows adanmış + tekdüze F_2p1_17_K17 (dondurulmuş çocuk sonlandırıldı, aynı
  tohumla baştan) + 26 menü bölütü. kos/duraklat.sh genelleştirildi (4 ağır süreç çalışıyorsa birini durdurur).

- M20 zincir kümelenmesi (§4.2; src/zincir.py, sonuc/sira/M129_zincir.txt): bilgi isabetleri 25 → 25 farklı zincir (zincir başına
  en çok 1); sayım isabetleri 59 → 57 zincir (2 zincirde 2'şer: aynı zincirin ardışık emisyonları). Kümelenme ihmal edilebilir.
- J10 Kur'an: 25,98 bit @ (1,+1,19) (B 11,02 + F 14,20 + W 8,08 − 7,32), bütün aramalar çözülmüş; sayım 11.
- M20 adanmış ilk üç: W20 @ (1,−1,5) K20: 52/1,53e8 = 3,40e-7; F580 @ (1,−1,7) K23: 40/2,07e7 = 1,93e-6; F204 @ (1,−1,11) K11:
  43/2,43e8 = 1,77e-7.

## 2026-10-26 ~21:00Z — J10 (ikincil) tamam (R 2e7 + T 1e7)
- Sayım (G): 11 / 3e7 → 4e-7 [1,8–6,6e-7]; GE 1,43e-6; Gc 2,03e-6; G0 1,46e-4. S0 ≥ 11: 2 / 3e7 (1e-7; biri (1,+1,19) 3/3/3/3).
- Bilgi v0 (S_Q 25,982): 1 / 4 / 7.473 → p_nokta 5e-7 [2e-7; 7,47e-4]. Rakipler: bilgi 1/20 sınıfta, sayım 20/20 (aynı desen).
- U127 Kur'an: (1,+1,19) F araması çözülmedi (k = 12 bağlamı 304.101 kitap); geçici nokta 29,174 [alt 27,842]; adanmış kestirim
  kuyruğun ilk işi (adanmis_u 127, 2e7 öneri).

## 2026-10-26 ~21:40Z — orkestra bitti (hata yok); 127 (ikincil)
- 127 Kur'an eşikleri: U127 F araması 2/304.101 (çözülmemiş) → adanmış adanmis_u 40 isabet / 2e7 öneri, P = 2,01e-6 → 18,92 bit
  (geçici 16,89; fark +2,03) → S_Q 31,203; L127 7/390.136 → adanmış 39/2,61e6 bağlam (2e8 kitap), P = 1,49e-5 → 16,03 bit (fark +0,36)
  → S_Q 27,830; M127 çözülmüş (c ≥ 10): 26,270 (adanmış denetim 71/2,40e6 = 2,96e-5 = 15,05 bit; tablo 14,47 bit; ~1,3σ).
- Sayım (G; N = 4e7): U127 23 → 6e-7; M127 15 → 4e-7; L127 7 → 2e-7. Bilgi v0 (N_T = 1e7): U127 0/0/5.301 → k = 0 (artı-bir 1e-7,
  üst sınır 3e-7); M127 4/8/6.021 → 9e-7; L127 0/1/5.230 → 2e-7. S0 ≥ 11 (N = 4e7): U127 0 (artı-bir 2,5e-8), M127 4
  (1,25e-7), L127 2 (7,5e-8).
- Rakipler: bilgi ilk 20'de sınıfta 0/20 (üç sıfırda); sayım 20/20 (aynı desen).
- Ana hat rakipleri 127 tablolarıyla (sonuc/ana_hat_rakipleri_127.txt): J10 sahte 8/8 sayım 10 (S0 10), bilgi 21,15 (U127) / 18,47 (M127)
  / 19,45 (L127) — Kur'an 31,20 / 26,27 / 27,83; robot rakipleri bilgi 2,6–13,5, sayım 2–4.

## 2026-10-26 ~22:45Z — adanmış kestirimler bitti; düzeltilmiş bilgi sıraları
- M20 adanmış dizisi (12 arama, 1,9e10 kitap; 19:49–22:41Z duvar saati ≈ 2,85 çekirdek-saat) → sonuc/belirsiz/M129_coz.txt: 14 kitap
  yeniden puanlandı; 3 ÜSTTE (biri zaten üstteydi), 5 ALTTA, 6 BELİRSİZ. Düzeltilmiş k_alt / k_nokta / k_üst: 14 / 25 / 18.170 →
  **16 / 22 / 18.165** → p_nokta **2,3e-7** [1,7e-7; 1,82e-4].
- Mallows adanmış (11 arama, 8,3e9 kitap; kuyruk süreleri toplamı 205,9 dk ≈ 3,4 çekirdek-saat) → L129_coz.txt: 14 kitap; 3 ÜSTTE,
  2 ALTTA, 9 BELİRSİZ. 34 / 44 / 16.152 → **37 / 46 / 16.150** → p_nokta **4,7e-7** [3,8e-7; 1,62e-4].
- Tekdüze: F_2p1_17_K17 (2427157) kuyruktan baştan koşuldu (aynı tohum 26142600006; 581 isabet / ESS 1,98e7, P̂ 2,92e-5) →
  27,59 [27,48; 27,71] ALTTA. 1 / 3 / 16.596 → **2 / 3 / 16.592** → p_nokta 4e-8 [3e-8; 1,66e-4]. Bu kitap S_alt sırası dışında
  çözüldü (betiğin yorumunda S_alt yerine nokta yazılmıştı); S_alt 26,88 olan iki kitabın beş araması koşulmadı →
  ek/EK_20261026T224527Z_tekduze_adanmis_sirasi.md.
- src/sira_duzelt.py: "sonra" alt/üst sınıfı artık belirsiz_coz'un tam duyarlıklı kararından alınıyor; 2 ondalıklı değerler S_Q'ya
  0,006'dan yakınsa uyarı basılıyor (U, M, L'de uyarı yok; U sonucu önceki sürümle aynı).
- src/rapor_tablo.py duyarlılık tablosunda V0 P0 artık düzeltilmiş k_nokta'yla (ana tabloyla aynı); öteki değişkeler düzeltmesiz.
- Tahmin hükümleri (src/karar.py) değişmedi: sıralı sıfırlar iki puanda "tuttu" (|log10 p̂ + 8| = 1,36–1,67), tekdüze "tutmadı (daha az
  nadir)", O çoğunlukla "tutmadı (daha nadir)", O' "tuttu" → bütünü "kısmen tuttu".
- Menü kuyruğu: U129 b005–b008 koşuyor (26 bölütün ilk dördü); ardından M129, L129.

## 2026-10-26 ~23:45Z — menü (d) tamam; aralık hesabı hatası düzeltildi
- 26 menü bölütü kuyrukta bitti (bölüt başına 7,4–7,8 dk); her sıfırda 10 bölüt, N = 1e7. src/menu_s0 U/M/L için 10 bölütle yeniden
  koşuldu (sonuc/menu/<sk>_s0.txt); özet sonuc/menu/menu_ozet.txt.
- Sayım (kapı G), Kur'an d2 11 / d3 9 / d4 11 (en iyi çift (t2, n2) @ (1,+1,19); t2'siz en iyisi (n2, a2) @ (1,+1,19)):
  tekdüze d2 8 → 9,0e-7, d3 1634 → 1,6e-4, d4 4 → 5,0e-7; M20 4 → 5,0e-7, 2661 → 2,7e-4, 3 → 4,0e-7; Mallows 9 → 1,0e-6,
  2680 → 2,7e-4, 3 → 4,0e-7. S0: d2 0 / 1 / 4; d3 490 / 1180 / 1215; d4 0 / 1 / 2.
- Okuma: menü bedeli (d2/d4) 1,3–2,5 kat (küçük sayılar); t2 olmadan Kur'an ~2e-4 (2,5–3 mertebe daha az nadir) → üstünlük kendi
  bölme türüne bağlı, bölme seçiminin yan ürünü değil.
- HATA bulundu ve düzeltildi: src/rapor_tablo.py cp() Poisson toplamını exp(−λ)'dan başlatıyordu; λ > ~745'te taşma → k ≳ 745 için
  aralık [7,45e-5; 7,45e-5] gibi anlamsız çıkıyordu (yalnız menü d3 sütunlarında görüldü; raporda bu değerler hiç yer almadı). Terimler
  log uzayında hesaplanıyor; küçük k'de eski sonuçla göreli fark 2e-13, büyük k'de kesin binom CP (src/ozet.py) ile aynı. Raporun
  başka hiçbir sayısı değişmedi (yeniden kurulan raporla fark yalnız menü satırları).
- Hesap: kayıtlı koşular 24,3 çekirdek-saat (üretim 12,6; adanmış 7,8; menü 3,9; src/hesap_saati.py). Klasör ~0,8 GB.

## 2026-10-26 ~23:45Z — rapor son hali; paket
- BUTUNLUK_RAPORU.md son kez kuruldu (yer tutucu kalmadı; 654 satır). Son okumada ön-kayda gönderme yapan üç "§" referansı
  açıklaştırıldı (§2.4, §5.3, §6.3 → "ön-kayıt §…"); İngilizce özette "resolved" → "re-scored".
- Denetimler: ONKAYIT.md SHA-256 d213dc89…fd513 (salt okunur); ~/Desktop/hrf2'de GOREV.md'den yeni dosya yok; dondurma kod listesinde
  yalnız src/test.c ve src/dil.py farklı (ek). Son kod değişiklikleri: ek/EK_20261026T234102Z_son_kod_degisiklikleri.md.
- Paket: kos/paketle.sh → paket/butunluk_paket.tar.gz (SHA-256 paket/butunluk_paket.sha256; dosya özetleri paket/PAKET_ICERIGI.sha256).

## Durum
- [x] Dil L, puanlayıcı (C + Python başvuru), örnekleyici doğrulamaları, bilgi puanı makinesi
- [x] ONKAYIT.md yazıldı ve donduruldu (2026-10-26T17:13:24Z)
- [x] Üretim koşuları (U / M20 / Mallows: R 1e8 + T 1e8; J10: R 2e7 + T 1e7; 127: R 3e7 + T 1e7)
- [x] Adanmış kestirim (Kur'an aramaları; belirsiz kitaplar U / M20 / Mallows) ve düzeltilmiş sıralar
- [x] İkincil (a) 127, (b) koşullu O / O', (c) duyarlılık
- [x] (d) menü: U129, M129, L129 (10'ar bölüt, N = 1e7)
- [x] BUTUNLUK_RAPORU.md son hali, paket + SHA-256
