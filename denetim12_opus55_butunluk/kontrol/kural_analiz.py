# kural_analiz — soyut sayımdan bütün gerektirmeler: öncül kümesi (≤ 3 yüklem) → sonuç, karşı-örneği SIFIR, öncülü gerçekleşebilir.
# Yalnız en küçük öncüllü kurallar basılır (alt kümesi zaten aynı sonucu veren öncüller atlanır).
import sys, itertools, numpy as np
AD = ['F1','F2','R0c','R0e','R1c','R1e','K1','Qc','QS','Qh']
def kurallar(H):
    M = 1024; S = H.astype(np.float64).copy()
    for b in range(10):
        for m in range(M):
            if not (m >> b) & 1: S[m] += S[m | (1 << b)]
    out = []
    for r in (1, 2, 3):
        for onc in itertools.combinations(range(10), r):
            om = sum(1 << i for i in onc)
            if S[om] == 0: continue                      # öncül gerçekleşmiyor
            for y in range(10):
                if y in onc: continue
                if S[om] - S[om | (1 << y)] == 0:
                    # daha küçük öncül aynı sonucu veriyor mu?
                    kucuk = any(S[sum(1 << i for i in alt)] > 0 and S[sum(1 << i for i in alt)] - S[sum(1 << i for i in alt) | (1 << y)] == 0
                                for rr in range(1, r) for alt in itertools.combinations(onc, rr))
                    if not kucuk: out.append((tuple(AD[i] for i in onc), AD[y]))
    return out
import os
onek = sys.argv[1] if len(sys.argv) > 1 else ''
sonuc = {}
for p in (3, 5, 7, 11, 19):
    for k in (0, 1):
        fn = f'tab/kural/{onek}p{p}_k{k}.bin'
        if not os.path.exists(fn): continue
        H = np.fromfile(fn, dtype=np.uint64)
        sonuc[(p, k)] = set(kurallar(H))
ortak = set.intersection(*sonuc.values())
print('Bütün p ve iki işaret kipinde geçerli kurallar:')
for o, y in sorted(ortak): print('   ', ' ∧ '.join(o), '→', y)
for key, v in sorted(sonuc.items()):
    ek = v - ortak
    if ek: print(f'Yalnız p={key[0]}, kip={"pozitif q" if key[1]==0 else "serbest işaret"}:', sorted(' ∧ '.join(o)+' → '+y for o, y in ek))
