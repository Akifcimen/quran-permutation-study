# dil.py — bütünlük dili L'nin başvuru (tanım) uygulaması. Yavaş ama açık; C puanlayıcısı bununla sınanır.
# Kullanım: python3 -I src/dil.py  (Kur'an'ın 406 mercekteki profilini ve sayım puanını basar)
import json, math, os, sys

REPO = os.path.expanduser('~/Desktop/hrf2')  # depo kopyasında yol genelleştirildi (BENIOKU.md)
AL = [1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3]            # dış çalışmanın 14 biçimi (aynı sıra)
BE = [1, -1, 2, -2, 3, -3, 1, -1, 3, -3, 1, -1, 2, -2]
PRIMES = [3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103,
          107, 109, 113]
PRED = ['B1', 'B2', 'F1', 'F2', 'R0c', 'R0e', 'R1c', 'R1e', 'K1', 'K2', 'Qc', 'QS', 'Qh', 'QkF',
        'W1', 'QU', 'QkH', 'Qa', 'Qn', 'QH']
IDX = {k: i for i, k in enumerate(PRED)}
GRP_B = ['B1', 'B2']
GRP_F = ['F1', 'F2', 'R0c', 'R0e', 'R1c', 'R1e', 'K1', 'K2', 'Qc', 'QS', 'Qh']      # aile-içi (11 bit)
GRP_W = ['W1', 'QU', 'QkH', 'Qa', 'Qn', 'QH']                                      # iki el ve birleşim (6 bit)
GATED = set(GRP_F) | {'QkF'}                                                       # kapıya tabi (sayım puanı)
PARTS = ['t2', 'n2', 'a2', 'h', 't4']
SN = 6555


def veri(kodeks=129):
    d = json.load(open(f'{REPO}/quran_meta.json'))
    a = [r['numberOfAyahs'] for r in d['data']['surahs']['references']]
    if kodeks == 127:
        a[8] = 127
    return a


def dsinif(part, n, a, t):
    if part == 't2': return t % 2
    if part == 'n2': return n % 2
    if part == 'a2': return a % 2
    if part == 'h':  return 1 if n > 57 else 0
    if part == 't4': return 1 if (t % 4) >= 2 else 0
    raise ValueError(part)


def kosu(vals):
    """farklı değerler kesintisiz ardışık bir dizi ve en az 2 farklı değer"""
    s = sorted(set(vals))
    return len(s) >= 2 and s[-1] - s[0] + 1 == len(s)


def sabit_dislanan(f, p, D1='t2', D2='n2'):
    """Permütasyonla değişmeyen (sabit) yüklemler — hiçbir kitapta puan almaz. Analitik liste (ONKAYIT §2.6)."""
    al, be = AL[f], BE[f]
    x = set()
    if D1 in ('n2', 'a2', 'h') or (D1 == 't2' and (al % 2 == 0 or be % 2 == 0)):
        x.add('B1')                                  # sınıf boyu yalnız konumdan ya da yalnız değerden belirlenir
    if abs(be) % p == 0:                             # p | β (yalnız p = 3, |β| = 3): F = H = {p | n} sabit
        x |= {'QU', 'QkF', 'QkH', 'Qn', 'W1'}
    if al % p == 0:                                  # p | α (yalnız p = 3, α = 3): F = H = {p | a}; sabit boy, sabit Σa
        x |= {'QU', 'QkF', 'QkH', 'Qa', 'W1'}
    return x


def profil(a, f, p, D1='t2', D2='n2'):
    """Bir mercekte (biçim f, asal p) ve bölme çiftinde (D1, D2) 20 yüklemin değeri ve yapı bilgisi."""
    al, be = AL[f], BE[f]
    A = sum(a)
    rec = []
    for n in range(1, 115):
        x = a[n - 1]
        t = al * n + be * x
        u = al * n - be * x
        rec.append((n, x, t, u, dsinif(D1, n, x, t), dsinif(D2, n, x, t)))
    v = {}
    # --- kitap düzeyi (D1 bölmesi) ---
    E0 = [r for r in rec if r[4] == 0]; E1 = [r for r in rec if r[4] == 1]
    v['B1'] = len(E0) == len(E1)
    if E0 and E1:
        sn0 = sum(r[0] for r in E0); sn1 = sum(r[0] for r in E1)
        sa0 = sum(r[1] for r in E0); sa1 = sum(r[1] for r in E1)
        st0 = sum(r[2] for r in E0); st1 = sum(r[2] for r in E1)
        v['B2'] = (al * sn0 == abs(be) * sa1) or (al * sn1 == abs(be) * sa0) or (st0 == st1)
    else:
        v['B2'] = False
    # --- aile ---
    F = [r for r in rec if r[2] % p == 0]
    H = [r for r in rec if r[3] % p == 0]
    Fn = {r[0] for r in F}; Hn = {r[0] for r in H}
    U = [r for r in rec if r[0] in Fn or r[0] in Hn]
    q = {r[0]: r[2] // p for r in F}               # tam bölünür; negatif olabilir
    F0 = [r for r in F if r[4] == 0]; F1_ = [r for r in F if r[4] == 1]
    cell = {(i, j): [r for r in F if r[4] == i and r[5] == j] for i in (0, 1) for j in (0, 1)}
    cs = {k: sum(q[r[0]] for r in c) for k, c in cell.items()}
    cn = {k: len(c) for k, c in cell.items()}
    sq0 = sum(q[r[0]] for r in F0); sq1 = sum(q[r[0]] for r in F1_); sq = sq0 + sq1
    hepsi_dolu = all(cn[k] >= 1 for k in cn)
    v['F1'] = len(F0) == len(F1_) and len(F0) >= 1
    v['F2'] = len(F0) >= 1 and len(F1_) >= 1 and sq0 == sq1
    v['R0c'] = cn[(0, 0)] == cn[(0, 1)] and cn[(0, 0)] >= 1
    v['R0e'] = cn[(0, 0)] >= 1 and cn[(0, 1)] >= 1 and cs[(0, 0)] == cs[(0, 1)]
    v['R1c'] = cn[(1, 0)] == cn[(1, 1)] and cn[(1, 0)] >= 1
    v['R1e'] = cn[(1, 0)] >= 1 and cn[(1, 1)] >= 1 and cs[(1, 0)] == cs[(1, 1)]
    v['K1'] = hepsi_dolu and kosu([cs[(0, 0)], cs[(0, 1)], cs[(1, 0)], cs[(1, 1)]])
    v['K2'] = len(F) >= 2 and kosu(list(q.values()))
    v['Qc'] = hepsi_dolu and sq == 4 * p
    v['QS'] = len(F) >= 1 and sq % p == 0
    v['Qh'] = len(F0) >= 1 and len(F1_) >= 1 and sq0 % p == 0 and sq1 % p == 0
    v['QkF'] = len(F) == p
    # --- iki el, birleşim ---
    v['W1'] = len(F) >= 1 and len(H) >= 1 and not (Fn & Hn)
    v['QU'] = len(U) == p
    v['QkH'] = len(H) == p
    v['Qa'] = len(U) >= 1 and sum(r[1] for r in U) % p == 0
    v['Qn'] = len(U) >= 1 and sum(r[0] for r in U) % p == 0
    v['QH'] = len(H) >= 1 and sum(r[3] // p for r in H) % p == 0
    for k in sabit_dislanan(f, p, D1, D2):
        v[k] = False
    bilgi = dict(kF=len(F), kH=len(H), kU=len(U), kesisim=len(Fn & Hn),
                 hucre_sayi=[cn[(0, 0)], cn[(0, 1)], cn[(1, 0)], cn[(1, 1)]],
                 hucre_toplam=[cs[(0, 0)], cs[(0, 1)], cs[(1, 0)], cs[(1, 1)]],
                 katsayilar=sorted(q.values()), sq=sq, sq_yari=[sq0, sq1])
    return v, bilgi


# ---------------- kapı, gerektirmeler, sayım puanı ----------------
def kapi(bilgi, p, tur='G'):
    """G (birincil): kF ≥ 2E ve her hücre ≥ 2; GE: kF ≥ E ve her hücre ≥ 2; Gc: her hücre ≥ 2; G0: kapı yok."""
    mc = min(bilgi['hucre_sayi'])
    if tur == 'G':  return bilgi['kF'] * p >= 228 and mc >= 2
    if tur == 'GE': return bilgi['kF'] * p >= 114 and mc >= 2
    if tur == 'Gc': return mc >= 2
    if tur == 'G0': return True
    raise ValueError(tur)


# Kanıtlı gerektirmeler (ONKAYIT §2.7): (öncüller) -> sonuçlar. Birincil bölme D1 = t2; kural kümesi p ve β işaretine bağlı.
def kurallar(p, be):
    K = [(('Qc',), ('QS',)), (('Qh',), ('QS',)), (('F2', 'QS'), ('Qh',)),
         (('QkF', 'QkH', 'QU'), ('Qn', 'Qa')), (('K1', 'Qc'), ('F2', 'R1e'))]
    if p >= 5:
        K.append((('K1', 'Qh'), ('F2',)))
    else:
        K += [(('R1c', 'K1', 'Qh'), ('F2',)), (('R1e', 'K1', 'Qh'), ('F2',))]
    if be > 0:
        K.append((('Qc', 'Qh'), ('F2',)))
    if p == 3 and be > 0:
        K += [(('Qc', 'F1', 'F2'), ('R0c', 'R1c')), (('Qc', 'F1', 'R0e'), ('R0c',)), (('Qc', 'F1', 'R1e'), ('R1c',)),
              (('Qc', 'F1', 'K1'), ('R0c', 'R1c', 'K2')), (('Qc', 'F1', 'Qh'), ('R0c', 'R1c')),
              (('Qc', 'F2', 'R1e'), ('K1',)), (('Qc', 'R1e', 'Qh'), ('K1',))]
    return K


def kapanis(S, K):
    S = set(S); degisti = True
    while degisti:
        degisti = False
        for onc, son in K:
            if all(x in S for x in onc) and not all(y in S for y in son):
                S |= set(son); degisti = True
    return S


def kredi(S, p=19, be=1):
    """S'yi kuralların kapanışıyla üreten en küçük alt kümenin boyu (tekrarsız sayım)."""
    K = kurallar(p, be); S = set(S)
    D = [x for x in S if x in kapanis(S - {x}, K)]        # başkalarından türetilebilenler
    best = 0
    for r in range(len(D), 0, -1):
        import itertools
        for alt in itertools.combinations(D, r):
            if S <= kapanis(S - set(alt), K):
                best = r; break
        if best: break
    return len(S) - best


def sayim(v, bilgi, p, tur='G', be=1):
    g = kapi(bilgi, p, tur)
    S = [k for k in PRED if v[k] and (g or k not in GATED)]
    return kredi(S, p, be), S


# ---------------- mercek bedeli ----------------
def bicim_bedeli(f, onsel='geo2'):
    al, be = AL[f], abs(BE[f])
    if onsel == 'tek':
        return math.log2(14)
    taban = {'geo2': 2.0, 'geo4': 4.0}[onsel]
    w = [taban ** -(AL[g] + abs(BE[g]) - 2) for g in range(14)]
    return -math.log2(taban ** -(al + be - 2) / sum(w))


ASAL_METIN = {3: 1 / 87 + 1 / 15 + 1 / 12, 5: 1 / 87 + 1 / 15 + 1 / 12, 7: 1 / 87 + 1 / 15 + 1 / 12,
              11: 1 / 87 + 1 / 15, 19: 1 / 87 + 1 / 15 + 1 / 12}


def asal_bedeli(p, onsel='esit'):
    if onsel == 'esit':
        return math.log2(29)
    return -math.log2(ASAL_METIN.get(p, 1 / 87))


# ---------------- bölme menüsü (ONKAYIT §8.4) ----------------
MENU = ['t2', 'n2', 'a2', 'h', 't4']
CIFTLER = [(x, y) for x in MENU for y in MENU if x != y]


def t_esit(d, f):
    return d == 't2' or (d == 'n2' and BE[f] % 2 == 0) or (d == 'a2' and AL[f] % 2 == 0)


def yapi(d1, d2, f):
    if t_esit(d1, f): return 'R'
    if t_esit(d2, f): return 'C'
    if {d1, d2} == {'n2', 'a2'} and AL[f] % 2 and BE[f] % 2: return 'D'
    return 'X'


def kurallar_menu(p, be, y):
    if y == 'R':
        return kurallar(p, be)
    K = [(('Qc',), ('QS',)), (('Qh',), ('QS',)), (('F2', 'QS'), ('Qh',)), (('QkF', 'QkH', 'QU'), ('Qn', 'Qa'))]
    if p >= 5:
        K.append((('K1', 'Qh'), ('F2',)))
    if y == 'X':
        K += [(('R0e', 'K1', 'Qc'), ('F2',)), (('R1e', 'K1', 'Qc'), ('F2',))]
        if be > 0:
            K += [(('R0e', 'Qc', 'Qh'), ('F2',)), (('R1e', 'Qc', 'Qh'), ('F2',))]
        if p == 3:
            K.append((('K1', 'Qc', 'Qh'), ('F2',)))
    return K


def kredi_K(S, K):
    import itertools
    S = set(S)
    D = [x for x in S if x in kapanis(S - {x}, K)]
    for r in range(len(D), 0, -1):
        for alt in itertools.combinations(D, r):
            if S <= kapanis(S - set(alt), K):
                return len(S) - r
    return len(S)


def menu_kredi(a, f, p, d1, d2):
    v, b = profil(a, f, p, d1, d2)
    g = kapi(b, p, 'G')
    S = [k for k in PRED if v[k] and (g or k not in GATED)]
    return kredi_K(S, kurallar_menu(p, BE[f], yapi(d1, d2, f)))


if __name__ == '__main__':
    kod = int(sys.argv[1]) if len(sys.argv) > 1 else 129
    a = veri(kod)
    enk = None
    satirlar = []
    for f in range(14):
        for p in PRIMES:
            v, b = profil(a, f, p)
            c, S = sayim(v, b, p, 'G', BE[f])
            satirlar.append((c, f, p, S, b))
    satirlar.sort(key=lambda x: -x[0])
    print(f'Kur\'an ({kod}) — sayım puanı (kapı G) en yüksek 12 mercek:')
    for c, f, p, S, b in satirlar[:12]:
        print(f'  ({AL[f]},{BE[f]:+d},{p}) puan {c}  kF={b["kF"]} kH={b["kH"]} hücre={b["hucre_sayi"]} Σ={b["hucre_toplam"]}  {S}')
    c, f, p, S, b = satirlar[0]
    v, b = profil(a, f, p)
    print('En iyi mercekte bütün yüklemler:', {k: int(v[k]) for k in PRED})
    print('bilgi:', b)