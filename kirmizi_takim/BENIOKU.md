# kirmizi_takim — kırmızı takımın ikinci tur ölçümleri (2 Eylül 2026)

`KESIF.md` dürüstlük notu 14'te anılan iki ölçümün betikleri ve ham çıktıları. Kırmızı takım ayrı bir klasörde çalıştı; bu dosyalar oradan kopyalandı (20 Ekim 2026). Sekizinci denetim, bu sonuçların betiklerinin depoda olmadığını haklı olarak işaretlemişti (not 21).

| Dosya | Ne ölçer |
|---|---|
| `kirmizi21b_saf_sinif.py`, `kirmizi21b_sonuc.txt` | Sınıf-düzeyi tipiklik: 33 "güzel yüklem" menüsü, her 12'lik aileye kendi en iyi hikâyesi (≤ 4 yüklem). 1,5 milyon rastgele 12'lik aile: gerçek ailenin hikâyesi kadar iyi hikâyesi olan aile oranı %0,005–%0,19 (L'ye göre). |
| `kirmizi22_asgari_catal.py`, `kirmizi22_sonuc.txt` | Sabit mercek (1,1,19), 3×10⁸ permütasyon (k_t ≥ 12 ∨ k_u ≥ 12 koşullu örnekleme): katı 8/8 6,7×10⁻⁹; asgarî çatallarla 4,3×10⁻⁸ (t) / 7,7×10⁻⁸ (iki el); geniş çatallarla 8,3×10⁻⁸ / 1,8×10⁻⁷. |

Değişiklik: yalnız depo yolu mutlak yoldan göreli yola çevrildi (`REPO = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')`). Çalıştırma: `python3 kirmizi_takim/kirmizi22_asgari_catal.py [permütasyon sayısı]`, `python3 kirmizi_takim/kirmizi21b_saf_sinif.py [aile sayısı]`. Her iki betik de Tevbe = 127 kullanır (çekirdek katmanlar 127/129'dan bağımsızdır).

Not: sekizinci denetimin gösterdiği gibi tek m'de S4, S2 ile S3'ten çıkar; "katı 8/8" yedi bağımsız koşuldur (not 21).
