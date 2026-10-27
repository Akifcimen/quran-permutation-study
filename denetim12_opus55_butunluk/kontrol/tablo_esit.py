# tablo_esit — seyrek ana referans tablosundan (ana.ref) en-az sayılarını ve bağlamları bağımsız hesaplar; C'nin t.tab'ı ile
# karşılaştırır. YALNIZ uyuşmazlık sayısı basılır (sayı değerleri basılmaz).
import sys, numpy as np, random
NL, KC, NFM, NWM, NF = 406, 80, 2048, 64, 14
def oku_ref(fn):
    d = open(fn, 'rb').read(); o = 0
    im, kod = np.frombuffer(d, '<u4', 2, o); o += 8
    N = int(np.frombuffer(d, '<u8', 1, o)[0]); o += 8
    BC = np.frombuffer(d, '<u8', NF*4, o).reshape(NF, 4).copy(); o += 8*NF*4
    out = []
    for n in (NL*KC*NFM, NL*KC*NWM, NL*KC):
        nz = int(np.frombuffer(d, '<u8', 1, o)[0]); o += 8
        pairs = np.frombuffer(d, '<u4', 2*nz, o).reshape(nz, 2); o += 8*nz
        A = np.zeros(n, dtype=np.uint64); np.add.at(A, pairs[:, 0].astype(np.int64), pairs[:, 1].astype(np.uint64)); out.append(A)
    return N, BC, out[0].reshape(NL, KC, NFM), out[1].reshape(NL, KC, NWM), out[2].reshape(NL, KC)
def oku_tab(fn):
    d = open(fn, 'rb').read(); o = 8
    N = int(np.frombuffer(d, '<u8', 1, o)[0]); o += 8
    nctx = int(np.frombuffer(d, '<i4', 1, o)[0]); o += 4
    ctx = np.frombuffer(d, '<i4', NL*KC, o).reshape(NL, KC); o += 4*NL*KC
    cn = np.frombuffer(d, '<u8', nctx, o); o += 8*nctx
    FA = np.frombuffer(d, '<u4', nctx*NFM, o).reshape(nctx, NFM); o += 4*nctx*NFM
    WA = np.frombuffer(d, '<u4', nctx*NWM, o).reshape(nctx, NWM); o += 4*nctx*NWM
    BA = np.frombuffer(d, '<u8', NF*4, o).reshape(NF, 4); o += 8*NF*4
    NKU = np.frombuffer(d, '<u8', NL*KC, o).reshape(NL, KC)
    return N, ctx, cn, FA, WA, BA, NKU
N, BC, FC, WC, NK = oku_ref(sys.argv[1]); N2, ctx, cn, FA, WA, BA, NKU = oku_tab(sys.argv[2])
hata = 0; kontrol = 0
assert N == N2
# toplamlar
if int(NK.sum()) != N * NL: hata += 1
if int(FC.sum()) != N * NL or int(WC.sum()) != N * NL: hata += 1
# bağlam sayıları ve en-az sayıları (rastgele örneklem)
rnd = random.Random(7)
for _ in range(4000):
    L = rnd.randrange(NL); k = rnd.randrange(KC); c = ctx[L, k]
    ks = [kk for kk in range(KC) if ctx[L, kk] == c]
    n = int(NK[L, ks].sum()); kontrol += 1
    if n != int(cn[c]): hata += 1
    m = rnd.randrange(NFM); sup = [mm for mm in range(NFM) if (mm & m) == m]
    if int(FC[L, ks][:, sup].sum()) != int(FA[c, m]): hata += 1
    w = rnd.randrange(NWM); supw = [mm for mm in range(NWM) if (mm & w) == w]
    if int(WC[L, ks][:, supw].sum()) != int(WA[c, w]): hata += 1
for f in range(NF):
    b = BC[f]; at = [b.sum(), b[1]+b[3], b[2]+b[3], b[3]]
    if any(int(at[i]) != int(BA[f, i]) for i in range(4)): hata += 1
if (NKU != NK).any(): hata += 1
print(f'tablo eşitliği: {kontrol} rastgele (mercek, kF) denetimi; uyuşmazlık {hata}')
