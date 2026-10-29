# Bağımsız kontrol: tekdüze sıfırda (1,+1) biçiminde B2'nin (çapraz terazi) sıklığı; oturumun tablosu 112.329 / 10⁸.
# Kullanım: python3 -I b2_tekduze.py <tohum> <parça (×10⁵ kitap)>
import sys, os, json, numpy as np
d = json.load(open(os.path.expanduser('~/Desktop/hrf2/quran_meta.json')))
Q = np.array([r['numberOfAyahs'] for r in d['data']['surahs']['references']], dtype=np.int64)
assert Q.sum() == 6236
rng = np.random.default_rng(int(sys.argv[1])); n = np.arange(1, 115); isabet = 0; N = 0
for _ in range(int(sys.argv[2])):
    m = 100000
    a = Q[np.argsort(rng.random((m, 114)), axis=1)]
    t = n + a; e0 = (t % 2 == 0)
    sn0 = (n * e0).sum(1); sn1 = n.sum() - sn0
    sa0 = (a * e0).sum(1); sa1 = a.sum(1) - sa0
    st0 = (t * e0).sum(1); st1 = t.sum(1) - st0
    ne = e0.any(1) & (~e0).any(1)
    isabet += int((ne & ((sn0 == sa1) | (sn1 == sa0) | (st0 == st1))).sum()); N += m
p = isabet / N
print(f"B2 @ (1,+1), tekdüze: {isabet} / {N} = {p:.4e} ± {np.sqrt(isabet)/N:.1e}; oturum 1,1233e-03")
