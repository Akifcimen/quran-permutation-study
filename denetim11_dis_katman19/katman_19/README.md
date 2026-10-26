# Ortak katman kuralları ve 19 — yeniden üretim paketi

Bu paket 25 Ekim 2026 tarihli tamamlanmış çalışmanın verilerini, dondurulmuş
tanımlarını, kodunu, 3.000.000 sınama puanını ve doğrulama kayıtlarını içerir.

## Önce okuyun

- katman_19_raporu.md: tam Türkçe rapor.
- katman_19_analizi.html: çalıştırılmış defterin tabloları ve gömülü grafikleri.
- katman_19_analizi.ipynb: aynı analizin düzenlenebilir ve çalıştırılabilir defteri.

Birincil sonuç: üç karşılaştırma modelinin her birinde 1.000.000 yeni kontrolde
gözlenen puana eşit/aşan örnek sayısı sıfırdır. Model başına Monte Carlo sıra
p-değeri yaklaşık 10^-6'dır. Bu, önceden tanımlanan sonlu arama ve belirtilen
karşılaştırma modellerinin sonucudur; bütün tarihsel seçimleri düzelten evrensel
bir tesadüf olasılığı değildir.

## Kurulum

Linux, g++ ve OpenMP gerekir. Python 3.12 ile doğrulanmıştır. İsterseniz temiz
bir sanal ortam içinde:

~~~bash
python -m pip install -r requirements.txt
~~~

Paket kökünde mevcut sonuçları yeniden analiz etmek için:

~~~bash
python analyze.py --no-compress
python semantic/analyze_results_independent.py
python review/observed_decomposition.py
python build_notebook.py
~~~

Defter, saklanan sonuçları yeniden hesaplar; yeni milyonluk deneme başlatmaz.
Bütün eğitim ve sınamayı kayıtlı bütçe/tohumlarla baştan üretmek için:

~~~bash
bash run_study.sh
~~~

Aynı akışlar için sekiz iş parçacığı korunmalıdır. Çalışma kaydı gerçek takım
büyüklüğünü, işçi tohumlarını, işlenen örnekleri ve öneri sayısını içerir.
İlk çalışmanın toplam kabul edilen örnek sayısı 3.600.000'dir: 600.000 eğitim,
3.000.000 sınama. Terslik koşullu modelde reddedilen öneriler bu sayıya dahil
değildir.

## Dosya biçimleri

- data/counts.txt: 114 standart sûre uzunluğu.
- data/verse_counts.json: aynı verinin 129 ve 127 seçenekleri.
- results/train_MODEL.bin: uint64 eğitim sayımları. İlk iki sayı kabul edilen
  örnek ve öneri sayısıdır; ardından G,C,W,M tam maske histogramları gelir.
  Düzen src/hist.hpp içinde tanımlıdır.
- results/test_MODEL_scores_THREAD.bin: başlıksız, küçük uçlu float64; her
  satırda altı politika puanı. Her üretim parçası 125.000 × 6 değer taşır.
- results/test_MODEL_scores.npz: aynı puanların kayıpsız sıkıştırılmış hali.
- Altı sütunun sırası: uniform, mixture, literal, discussion, anchored19,
  frequency. Ayrıntılı adlar ve ağırlıklar protocol.json ile scorer.hpp içinde.
- results/test_MODEL_top.json: en yüksek 50 birincil kontrolün tam kimlik
  permütasyonu ve bütün puan izleri. Kimlikler 0 tabanlıdır.
- results/test_MODEL_diagnostics.json: protokolün 20 açıklayıcı sayacı.
- results/summary.json: ana sonuçlar, güven aralıkları ve koşullu tanılar.

Ham puan parçaları ve NPZ birlikte saklanmıştır. ZIP küçüktür çünkü puanların
çoğu taban değeri sıfırdır; açılmış veri daha büyük yer kaplar. Puanların sıfır
olması bir olayın sıfır olasılıklı olduğu anlamına gelmez.

## Bağımsız doğrulamalar

~~~bash
python src/test_matrix_kernel.py
python src/test_scorer_kernel.py
python src/test_features.py
python tests/test_sampler.py
python tests/test_worker_seeding.py
~~~

Kontrollerin kapsamı review/ ve tests/ altındaki kayıtlarla açıklanır.
Notebook yeniden çalıştırması için yerel Jupyter çekirdeğinin bağlantı açabilmesi
gerekir; internet bağlantısı analiz hesapları için gerekli değildir.

## Kaynaklar ve kullanım

Dondurulmuş sayısal kaynak:
https://github.com/Akifcimen/quran-permutation-study/tree/51fe1c0c36f2d481369480800aa84582e0a099e8

Tanzil Quran Text Version 1.1, Copyright © 2007–2026 Tanzil Project.
Özgün Arapça dosyalar değiştirilmeden ve atıf/koşul bloklarıyla korunmuştur.
semantic/SOURCE_NOTICE.txt dosyasına bakın.

Paket içeriğinin SHA-256 özetleri MANIFEST.json içinde yer alır.
Rapor ve kod, eski sohbetlerde bildirilmiş fakat ham verisi bulunamayan
simülasyon sonuçlarını doğrulanmış sonuç olarak devralmaz.

