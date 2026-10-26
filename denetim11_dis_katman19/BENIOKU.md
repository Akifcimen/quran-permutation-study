# denetim11 — on birinci denetim: katmanlar arası bütünlük, üç dış çalışma (25–26 Ekim 2026)

Araştırmacıya 26 Ekim'de ulaştırılan üç çalışma. Büyük ihtimalle 8. ve 9. denetimleri yapan aynı dış denetçiye aittir: raporlar, o incelemeleri kendi "Kütüphane"lerinden okuduklarını yazar. Veri kaynağı yayımdaki `51fe1c0` (RAPOR.md için arşivdeki `a9bec42` verisi; ayet sayıları aynı). Ana hattın değerlendirmesi `KESIF.md` dürüstlük notu 24'tedir.

| Dosya / klasör | İçerik |
|---|---|
| `rapor_yapisal_ayrisma_25ekim.md` | Yedi koşullu yapısal puan (A terazi; B–D hücreler, merkez deseni, bant; W İki El; M, J matris). Eş-terslik dilimi ve 6-blokta 200.000'er kontrolün en iyisi 4, Kur'an (127) 7. Arşivdeki 1.781 blok çekirdek tanığında tam matris 4 (doğal sunum) / 9 (16 sunum). **Kodu ve sonuç dosyaları bize ulaşmadı.** |
| `katman_19/` | joint-grammar-v1 (ön-kayıtlı): dört grup G, C, W, M; 29 asal × 14 doğrusal biçim × 2 sayım × 8 matris sunumu; parametre paylaşımını ödüllendiren MDL-benzeri puan; metinde adı geçen asallara ağırlık. Tekdüze, 6-blok ve tam terslik dilimi (I = 993/994) modellerinde 10⁶'şar kontrolün hiçbiri Kur'an'ın puanına ulaşmadı. Rapor `katman_19_raporu.md`, çalıştırılmış defter `katman_19_analizi.html`, dondurulmuş protokol `FROZEN_PROTOCOL.md`, `protocol.json`. |
| `nadirlik_ek/` | Ek çalışma: sabit doğal desenin kesin sayımları ve koşullu örneklemesi (6-blok C∩W∩G∩M 6,04×10⁻¹⁵; tekdüze 7,8×10⁻¹⁸), geniş testin kuyruğu için SMC (nokta tahminleri hassasiyet kapılarını geçmedi; 6-blokta sınırlar [1,9×10⁻¹²; 3,7×10⁻⁶]), p = 7 bileşeni. Rapor `nadirlik_ek_raporu.md`, defter `nadirlik_ek_analizi.html`. |
| `dogrulama/` | Ana hattın bağımsız kontrolleri (aşağıda). |
| `atlananlar.tsv` | Bu kopyaya alınmayan dosyalar, boyut ve gerekçeyle. |

## Bu kopyada olmayanlar

Özgün paketler toplam ~300 MB'tır. Alınmayanlar: 1 MB'tan büyük dosyalar (24 ham sınama puanı parçası `results/test_*_scores_*.bin` — aynı puanlar kayıpsız `results/test_*_scores.npz` dosyalarında vardır; örnekleyici tabloları `rare_event/exact/*.bin`, `rare_event/uniform_anchor/*.bin`), Tanzil Arapça metin dosyaları ve büyük sayı envanterleri (`semantic/`), pilot koşular, defterlerin `.ipynb` sürümleri (`.html` çıktıları duruyor), 60–200 KB'tan büyük durum-denetim ayrıntıları. Ayrıntılı liste `atlananlar.tsv`; her özgün dosyanın SHA-256'sı `katman_19/MANIFEST.json` ve `nadirlik_ek/package_manifest.json` içindedir.

Araştırmacının aldığı dosyaların SHA-256 özetleri:

```
0d4e6eff9ce0925101c2caa5d447f93c642bc789d6952190fe6de6963c11feee  RAPOR.md
59d7d02da9db2819afb623f5fe1df075d14fa7cee419ebc1eca8aeba7e9e708c  katman_19_yeniden_uretim.zip
b475615835500abab3ebed48e7a9e13def4c48ffea6d05d296ef0a4317525658  katman_19_nadirlik_ek_calismasi.zip
```

## Ana hattın kontrolleri (`dogrulama/`)

| İddia | Onların sonucu | Bizim kontrolümüz |
|---|---|---|
| Matris değerleri ve mertebeleri | 127: P, R, PR+RP mertebeleri 360, (PR−RP)² = I; 129: 30, 18, 6, (PR−RP)² = 3I | aynısı (`matris.py`) |
| (PR−RP)² = s²(x² + sy − y²)I özdeşliği | 19³ üçlüde doğrulandı | 19³ üçlüde 0 ihlal |
| s = 2'de yalnız (x, y) = (1, 16) bütün matris ilişkilerini verir | evet | evet; **s = 4'te (129) hiçbir (x, y) vermez** (yeni) |
| 6-blokta P(C) | 8,8457×10⁻⁷ (kesin sayım) | 8,803×10⁻⁷ — kendi C kodumuzla 10¹⁰ örnek, 8.803 isabet (−0,5σ) (`blok6_cw.c`) |
| 6-blokta P(C∩W) | 4,1384×10⁻⁹ (kesin sayım) | 3,4×10⁻⁹ — 34 isabet / 10¹⁰ (−1,2σ) |
| "Kısa kurallar katsayı torbasını belirler": yalnız 2 torba kalır | 2 | 2 (`torba_sayimi.py`; torba sayıları üst sınıra göre değişir, sonuç değişmez) |

**Matris katmanının indirgenmesi.** Terazi tuttuğunda P = [[y, s−y], [−y, y]] ve R = [[x, −x], [−x, s+x]] (mod 19; x = Σ_F n, y = Σ_E n, s = Σa). Dört matris koşulu bu durumda yalnız şuna eşdeğerdir: s ≡ 2 (127 sayımı), x ≡ 1 ve y ≡ 16. Standart 129 sayımında (s ≡ 4) teraziyi sağlayan hiçbir diziliş bu mimariyi veremez. Üç çalışmada ölçülen 1/349–1/415 seçicilik ve arşiv tanıklarındaki 4/1781 ile 9/1781 (beklenen 4,9 ve 9,9), iki düzgün kalanın 1/361'idir.

## Kısa değerlendirme

**Kabul:** katmanlar arası bütünlük olay olarak güçlü biçimde ayrışır. Rakiplere modül, biçim ve parametre paylaşımı serbestliği verildiğinde de uzunluk sırasını koruyan modellerde hiçbir kontrol bu kadar katmanı aynı parametreyle bir araya getiremedi; standart 129 sayımında da (Kur'an 19,9; kontrol en yükseği 5,2–11,7). Yapı uzunluk sıralamasının bir yan ürünü değildir.

**Çekinceler:** (1) Dilin dört grubu ve özellikle C'nin şekilleri Kur'an'da görülenden yazıldı; test "rastgele kitaplar Kur'an'a ne kadar benziyor?" sorusunu ölçer, "her kitap kendi en iyi bütünlüğünü kursa Kur'an'ınki ne kadar sıra dışı?" sorusunu ölçmez. (2) G (1994), Σq = 4p (1444, 1990) ve b,b satırı (361/361, 2015) kanıt sayılır, koşullanmaz. (3) Matris katmanı iki kalan eşleşmesidir ve standart sayımda imkânsızdır. Bu yüzden 26 Ekim'de ayrı bir oturumda **plasebo kalibrasyonu** başlatıldı: aynı otomatik dil-kurma yordamı Kur'an'a ve sıfırdan çekilmiş rastgele kitaplara uygulanır, her hedef kendi diliyle sınanır, Kur'an'ın ayrışması plasebo ayrışmaları arasında sıralanır.
