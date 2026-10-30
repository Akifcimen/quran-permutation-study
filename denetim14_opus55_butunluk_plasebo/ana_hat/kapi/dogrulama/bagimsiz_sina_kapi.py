# bagimsiz_sina.py — ONKAYIT §6.1 ve §6.3: kayıtlı kitapların yapısını C kodundan bağımsız olarak tanımlardan yeniden sınar.
# Mercek (α, β, p): t = αn + βa, u = αn − βa; F = {n : p | t}, H = {n : p | u}; hücre = (t mod 2, n mod 2); q = t/p.
# Yüklemler: B2 (terazi, kitap), K1 (hücre toplamları ardışık), Qc (Σq = 4p), Qh (iki satır toplamı p'ye bölünür), WSum (|F| + |H| = p).
# Kapı G: |F|·p ≥ 228 ve her hücre ≥ 2. Sınıf: sağlanan yüklemler ≥ tmin türden ve ≥ dmin düzeyden.
# Kullanım: python3 -I kos/bagimsiz_sina.py <sıfır> <kayıt.s> <seçim: hepsi|isabet|rastgele20> <eşik_dosyası>
import os, random, struct, sys
KE, KH = int(os.environ.get("KAPI_ESIK", 228)), int(os.environ.get("KAPI_HUCRE", 2))   # kapi çalışması: kapı ortam değişkeninden
FORMS = [(1, 1), (1, -1), (1, 2), (1, -2), (1, 3), (1, -3), (2, 1), (2, -1), (2, 3), (2, -3), (3, 1), (3, -1), (3, 2), (3, -2)]
PR = [3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113]
TUR = {'B2': 'a', 'K1': 'd', 'Qc': 'b', 'Qh': 'b', 'WSum': 'c'}
DUZ = {'B2': 'kitap', 'K1': 'hücre', 'Qc': 'hücre', 'Qh': 'aile', 'WSum': 'birleşim'}
DIL = {'M': [('A000', {'B2', 'K1', 'Qc', 'WSum'}, 3, 3), ('Am000', {'B2', 'K1', 'Qc'}, 2, 2), ('Ap000', {'B2', 'K1', 'Qh', 'WSum'}, 4, 4)],
       'U': [('A000', {'B2', 'K1', 'Qc', 'WSum'}, 3, 3)], 'L': [('A000', {'B2', 'K1', 'Qc', 'WSum'}, 3, 3)]}

def yuklemler(a, f, p):
    al, be = FORMS[f]; n_ = range(1, 115)
    t = [al * n + be * a[n - 1] for n in n_]; u = [al * n - be * a[n - 1] for n in n_]
    E0 = [n for n in n_ if t[n - 1] % 2 == 0]; E1 = [n for n in n_ if t[n - 1] % 2 == 1]
    sn0, sn1 = sum(E0), sum(E1); sa0, sa1 = sum(a[n - 1] for n in E0), sum(a[n - 1] for n in E1)
    st0, st1 = sum(t[n - 1] for n in E0), sum(t[n - 1] for n in E1)
    b2 = bool(E0) and bool(E1) and (al * sn0 == abs(be) * sa1 or al * sn1 == abs(be) * sa0 or st0 == st1)
    F = [n for n in n_ if t[n - 1] % p == 0]; H = [n for n in n_ if u[n - 1] % p == 0]
    hucre = {c: [] for c in range(4)}
    for n in F: hucre[2 * (t[n - 1] % 2) + (n % 2)].append(t[n - 1] // p)
    c = [len(hucre[i]) for i in range(4)]; s = [sum(hucre[i]) for i in range(4)]; dolu = all(c)
    ds = sorted(set(s)); k1 = dolu and len(ds) >= 2 and ds[-1] - ds[0] + 1 == len(ds)
    qc = dolu and sum(s) == 4 * p
    qh = (c[0] + c[1] >= 1) and (c[2] + c[3] >= 1) and (s[0] + s[1]) % p == 0 and (s[2] + s[3]) % p == 0
    wsum = len(F) >= 1 and len(H) >= 1 and len(F) + len(H) == p and not (abs(be) % p == 0 or al % p == 0)
    kapi = len(F) * p >= KE and min(c) >= KH
    sag = {x for x, v in (('B2', b2), ('K1', k1), ('Qc', qc), ('Qh', qh), ('WSum', wsum)) if v}
    return kapi, sag, len(F), len(H), c, s

def sinif(sag, dil, tmin, dmin):
    x = sag & dil; return len({TUR[y] for y in x}) >= tmin and len({DUZ[y] for y in x}) >= dmin

sifir, dosya, secim, esikf = sys.argv[1:5]
esik = {ln.split()[1]: float(ln.split()[3]) for ln in open(esikf) if ln.startswith('D ')}
K = struct.Struct('<IfhBB114H'); data = open(dosya, 'rb').read()
kay = [K.unpack_from(data, o) for o in range(0, len(data), K.size)]
if secim == 'isabet': kay = [r for r in kay if r[1] >= esik[DIL[sifir][r[4]][0]] - 1e-5]
elif secim == 'rastgele20': random.Random(20261029).shuffle(kay); kay = kay[:20]
bad = 0
for r in kay:
    sira, puan, L, sn, d = r[:5]; a = list(r[5:]); ad, dil, tmin, dmin = DIL[sifir][d]; f, i = divmod(L, 29); p = PR[i]
    kapi, sag, kF, kH, c, s = yuklemler(a, f, p); ok = kapi and sinif(sag, dil, tmin, dmin)
    # bütün mercekler: bu dilde bütünlüklü mercekler kümesi kaydın merceğini içermeli
    blist = [ff * 29 + j for ff in range(14) for j, pp in enumerate(PR) if (lambda z: z[0] and sinif(z[1], dil, tmin, dmin))(yuklemler(a, ff, pp))] if secim != 'hepsi' else None
    if not ok or (blist is not None and L not in blist): bad += 1
    if secim != 'hepsi' or not ok:
        print(f'{ad} sira {sira} puan {puan:.4f} mercek {FORMS[f]},{p} kapı {kapi} kF {kF} kH {kH} hücre {c} toplam {s} sağlanan {sorted(sag & dil)} sınıf {ok}'
              + (f' bütünlüklü mercekler {blist}' if blist is not None else ''))
print(f'{sifir} {secim}: {len(kay)} kayıt, uyuşmazlık {bad}')
