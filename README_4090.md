# 4090'da Trilyon Koşusu — Ofis Talimatı

**Hedef:** J olayının (terazi ∧ tam kristal, ~2,3×10⁻¹³) ilk doğrudan gözlemi.
10 trilyon denemede beklenen isabet ~2,3 → görme olasılığı ~%90.

## 1. Kurulum (bir kez)

- Windows'ta: [CUDA Toolkit 12.x](https://developer.nvidia.com/cuda-downloads) kur; "x64 Native Tools Command Prompt" aç (veya WSL2 Ubuntu + `sudo apt install nvidia-cuda-toolkit`).
- Bu klasörden iki dosyayı taşı: `kesif_cuda.cu` ve (çapraz kontrol için) `kesif4_motor.c`.

## 2. Derle

```
nvcc -O3 -arch=sm_89 -o kesif_cuda kesif_cuda.cu
```
(`sm_89` = 4090/Ada. Hata verirse `-arch=native` dene.)

## 3. Doğrulama (ZORUNLU — koşudan önce)

**a) Öz-test** — gerçek veride 28 sayacın TAMAMI 1 olmalı:
```
./kesif_cuda selftest
```
Beklenen: `selftest T=1 A=1 B=1 ... X6=1` (hepsi 1).

**b) Kalibrasyon (1 milyar, ~birkaç saniye-dakika):**
```
./kesif_cuda 7 1000000000
```
Son satırdaki oranlar şu bantlarda olmalı (86 milyarlık CPU arşivinin değerleri):

| Sayaç | Beklenen oran | 1 milyarda kabul bandı |
|---|---|---|
| A | 3,137×10⁻⁴ | 310.000 – 317.500 |
| B | 3,158×10⁻⁴ | 312.500 – 319.500 |
| C | 1,850×10⁻⁵ | 17.700 – 19.300 |
| D | 1,39×10⁻⁷ | 100 – 180 |
| W | 5,50×10⁻⁵ | 53.500 – 56.500 |
| X1/A oranı | ~%15,8 | %15,5 – %16,1 |

Bant dışıysa DURDUR ve sonucu getir — koşuya geçme.

**c) (İsteğe bağlı) CPU çaprazı aynı makinede:**
```
cc -O3 -o kesif4_motor kesif4_motor.c && ./kesif4_motor selftest
```

## 4. Üretim koşusu

```
./kesif_cuda 42 10000000000000 > runGPU_seed42.txt 2> runGPU_seed42.log &
```
- 10 trilyon deneme; tahminî süre 4090'da **~3–9 saat** (kalibrasyondaki hızdan kesin süreyi hesapla: 10¹³ ÷ hız).
- Çıktı her ~yarım milyar denemede bir kümülatif satır yazar → **kesilse bile son satır geçerli sonuçtur.**
- İlerlemeyi izle: `tail -f runGPU_seed42.txt` (T sütunu deneme sayısı).
- **Bakılacak sütun: `J=`** — sıfırdan büyükse tarihî gözlem gerçekleşti demektir. `AGW`, `X6`, `H`, `GW` da değerli.

## 5. Dönüş

`runGPU_seed42.txt` dosyasını (tamamını, sadece son satır da yeter) buraya getir; birleşik arşive işleyip raporu güncelleyeceğim.

## Notlar

- Farklı tohumla ikinci koşu paralel başka GPU'da koşabilir (ör. `./kesif_cuda 43 ...`) — sonuçlar toplanabilir.
- Motor, CPU sürümüyle sayaç-uyumlu: aynı olay tanımları, aynı çıktı formatı; RNG farklı ama tarafsız (istatistiksel eşdeğerlik kalibrasyonla doğrulanıyor).
- GPU'da ECC yok; kozmik-ışın tek-bit riski ihmal edilebilir ama J>0 çıkarsa tohumuyla tekrar koşup teyit etmek raporluk standarttır.
