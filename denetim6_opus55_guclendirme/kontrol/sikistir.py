# Disk doldu (%100). Tam puan dizileri (9 sütun, float32) tohumlu koşularla birebir yeniden üretilebilir.
# Her dizi için: Kur'an eşiklerinin 1,5 altına kadar olan kuyruk satırları (tam hassasiyet, satır indeksiyle) + N + nicelikler.
# analiz.py'nin kullandığı her sayım (k = #{sütun ≥ Q − 1e-6}) bu kuyruktan birebir yeniden üretilir; doğrulama aşağıda.
import glob, numpy as np, os, sys
EPS = 1e-6
tot_before = tot_after = 0
for f in sorted(glob.glob('puan/*_sifir.f32')):
    base = os.path.basename(f)[:-len('_sifir.f32')]
    X = np.fromfile(f, dtype=np.float32).reshape(-1, 9)
    Q = np.loadtxt(f.replace('_sifir.f32', '_kuran.txt'))
    keep = np.zeros(len(X), bool)
    for c in range(6):
        keep |= X[:, c] >= Q[c] - 1.5
    idx = np.nonzero(keep)[0].astype(np.int64)
    qs = np.quantile(X[:, :6], [0.5, 0.9, 0.99, 0.999, 0.9999], axis=0).astype(np.float64)
    k = np.array([int(np.sum(X[:, c] >= Q[c] - EPS)) for c in range(6)])
    np.savez_compressed(f'puan_ozet/{base}.npz', N=len(X), Q=Q, idx=idx, tail=X[idx], quantiles=qs, k=k)
    # doğrulama: kuyruktan yeniden sayım
    T = np.load(f'puan_ozet/{base}.npz')
    k2 = np.array([int(np.sum(T['tail'][:, c] >= T['Q'][c] - EPS)) for c in range(6)])
    assert np.array_equal(k, k2) and int(T['N']) == len(X), base
    tot_before += os.path.getsize(f); tot_after += os.path.getsize(f'puan_ozet/{base}.npz')
print(f'sıkıştırıldı: {tot_before/1e9:.2f} GB → {tot_after/1e6:.1f} MB; bütün sayımlar kuyruktan birebir yeniden üretildi')
