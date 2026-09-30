# Kaldığımız yer (28 Eylül 2026, 11:40 civarı durduruldu)

Rapor: `KIRMA_RAPORU.md`. Tek bir tahmin satırı dışında tamam.

## Bitenler
- Bütün doğrulamalar, sıfır-modeli taramaları, turnuvalar, robot (60 / 99 olgu), kontroller, karşı-örnekler.
- **Tam belgeli arama, uzunluk-sıralı sıfır (J10), doğrudan ölçüldü:** serbest mercek p = 5,5×10⁻³, ×7 okul → **3,9×10⁻²
  (~26'da 1)**. Rapora işlendi.

## Kalan tek adım
Tekdüze sıfırda aynı doğrudan ölçüm. Rapordaki "tekdüze ~3×10⁻⁴ (~3.300'de 1)" satırı şu an **tahmin**.
Tablo (`tab/T_U_2M.bin`) hazır; yalnız puanlama yeniden koşulacak (~53 dk işlemci zamanı; makine boşsa ~10 dk):

```bash
cd <oturumlar>/kirma
./devam.sh
```

Çıkan p-değeriyle raporda şu yerler güncellenecek:
- §0'daki tekdüze değer,
- §8'deki iki tekdüze "*tahmin*" satırı,
- §9 madde 4,
- İngilizce özetteki tekdüze satırı.
