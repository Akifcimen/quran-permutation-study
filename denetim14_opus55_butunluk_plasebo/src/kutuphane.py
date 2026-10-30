# kutuphane.py — plasebo12 yüklem kütüphanesi L*'ın başvuru (tanım) uygulaması (ONKAYIT §2). Yavaş ama açık.
# C uygulaması (src/kutuphane.h) bununla bit-bit sınanır. Özgün 20 yüklem 12. denetimin src/dil.py tanımlarıyla aynıdır.
import math

AYAT129 = [7, 286, 200, 176, 120, 165, 206, 75, 129, 109, 123, 111, 43, 52, 99, 128, 111, 110, 98, 135,
           112, 78, 118, 64, 77, 227, 93, 88, 69, 60, 34, 30, 73, 54, 45, 83, 182, 88, 75, 85,
           54, 53, 89, 59, 37, 35, 38, 29, 18, 45, 60, 49, 62, 55, 78, 96, 29, 22, 24, 13,
           14, 11, 11, 18, 12, 12, 30, 52, 52, 44, 28, 28, 20, 56, 40, 31, 50, 40, 46, 42,
           29, 19, 36, 25, 22, 17, 19, 26, 30, 20, 15, 21, 11, 8, 8, 19, 5, 8, 8, 11,
           11, 8, 3, 9, 5, 4, 7, 3, 6, 3, 5, 4, 5, 6]
AL = [1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3]
BE = [1, -1, 2, -2, 3, -3, 1, -1, 3, -3, 1, -1, 2, -2]
PRIMES = [3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103,
          107, 109, 113]
GB = ['B1', 'B2', 'Ba', 'Bt', 'BMn', 'BMt', 'BMt2']
GZ = ['QkF', 'Zk2p', 'Zkd', 'Zkd2']
GF = ['F1', 'F2', 'R0c', 'R0e', 'R1c', 'R1e', 'K1', 'K2', 'Qc', 'QS', 'Qh',
      'FC1', 'FC2', 'FD1', 'FD2', 'FSp', 'FS2p', 'FSd2', 'FHp', 'FH2p', 'FHm', 'FHd2',
      'FCp', 'FC2p', 'FCm', 'FCd', 'FCd2', 'FHSp', 'FHS2p', 'FHSm', 'FHSd', 'FHSd2',
      'FAp', 'FA2p', 'FAd', 'FAd2', 'FNp', 'FN2p', 'FNd', 'FNd2', 'FRK', 'FCK',
      'FKp', 'FK2p', 'FKd', 'FKd2', 'FKSp', 'FKS2p', 'FKSm', 'FKSd', 'FKSd2', 'FAP', 'FKS', 'FSC']
GW = ['W1', 'QU', 'QkH', 'Qa', 'Qn', 'QH',
      'WI1', 'WEq', 'WSum', 'WU2p', 'WUd', 'WUd2', 'WH2p', 'WHd', 'WHd2', 'WIp', 'WI2p', 'WId', 'WId2',
      'WUAp', 'WUA2p', 'WUAd2', 'WUNp', 'WUN2p', 'WUNd2', 'WUTp', 'WUT2p', 'WUTd', 'WUTd2',
      'WHAp', 'WHA2p', 'WHAd', 'WHAd2', 'WHNp', 'WHN2p', 'WHNd', 'WHNd2', 'WHRp', 'WHR2p', 'WHRd2']
GH = ['HH1', 'HH2', 'HR0c', 'HR0e', 'HR1c', 'HR1e', 'HK2']
GRUPLAR = {'B': GB, 'Z': GZ, 'F': GF, 'W': GW, 'H': GH}
TUM = GB + GZ + GF + GW + GH
HAND = ['B1', 'B2', 'QkF', 'F1', 'F2', 'R0c', 'R0e', 'R1c', 'R1e', 'K1', 'K2', 'Qc', 'QS', 'Qh',
        'W1', 'QU', 'QkH', 'Qa', 'Qn', 'QH']
SN = 6555


def kosu(vals):
    """farklı değerler kesintisiz ardışık ve en az 2 farklı değer"""
    s = sorted(set(vals))
    return len(s) >= 2 and s[-1] - s[0] + 1 == len(s)


def aritmetik(vals):
    v = sorted(vals)
    d = v[1] - v[0]
    return d >= 1 and all(v[i + 1] - v[i] == d for i in range(len(v) - 1))


def bol(p, x):
    return x % p == 0


def dislanan(f, p, a_toplam=6236):
    """Analitik dışlama (ONKAYIT §2.5) — sabit yüklem–mercek çiftleri."""
    al, be = AL[f], BE[f]
    ST = al * SN + be * a_toplam
    x = set()
    if al % 2 == 0 or be % 2 == 0: x.add('B1')
    if al % 2 == 0: x.add('Ba')
    if ST % 2: x.add('Bt')
    if be % 2 == 0 or SN % p: x.add('BMn')
    if ST % p: x.add('BMt')
    if ST % (p * p): x.add('BMt2')
    x |= {'Bt', 'FHp', 'FKp'}                       # her mercekte imkânsız (L* dışı)
    if abs(be) % p == 0:
        x |= {'FC1'}                                 # F = {3 | n}: sütun sayıları 19/19
        x |= {'QU', 'QkH', 'Qn', 'W1', 'QkF', 'Zk2p', 'Zkd', 'Zkd2',
              'WI1', 'WEq', 'WSum', 'WU2p', 'WUd', 'WUd2', 'WH2p', 'WHd', 'WHd2', 'WIp', 'WI2p', 'WId', 'WId2',
              'WUNp', 'WUN2p', 'WUNd2', 'WHNp', 'WHN2p', 'WHNd', 'WHNd2', 'WUTd',
              'FNp', 'FN2p', 'FNd', 'FNd2', 'FHSp', 'FHS2p', 'FHSm', 'FHSd', 'FHSd2',
              'FKSp', 'FKS2p', 'FKSm', 'FKSd', 'FKSd2'}
    if al % p == 0:
        x |= {'QU', 'QkH', 'Qa', 'W1', 'QkF', 'Zk2p', 'Zkd', 'Zkd2',
              'WI1', 'WEq', 'WSum', 'WU2p', 'WUd', 'WUd2', 'WH2p', 'WHd', 'WHd2', 'WIp', 'WI2p', 'WId', 'WId2',
              'WUAp', 'WUA2p', 'WUAd2', 'WHAp', 'WHA2p', 'WHAd', 'WHAd2', 'WUTd',
              'FAp', 'FA2p', 'FAd', 'FAd2', 'FHSp', 'FHS2p', 'FHSm', 'FHSd2',
              'FKSp', 'FKS2p', 'FKSm', 'FKSd2'}
    if be % 2 == 0:
        x |= {'FD1', 'FD2', 'FKp', 'FK2p', 'FKd', 'FKd2', 'FKSp', 'FKS2p', 'FKSm', 'FKSd', 'FKSd2', 'FAP', 'FKS',
              'HR0c', 'HR0e', 'HR1c', 'HR1e'}
    return x


def profil(a, f, p):
    """Mercek (biçim f, asal p) için 112 yüklemin değeri ve yapı bilgisi. Bölme: D1 = t-paritesi, D2 = n-paritesi."""
    al, be = AL[f], BE[f]
    pp = p * p
    rec = []
    for n in range(1, 115):
        x = a[n - 1]
        t = al * n + be * x
        u = al * n - be * x
        rec.append(dict(n=n, a=x, t=t, u=u, d1=t % 2, d2=n % 2))
    v = {}
    # --- kitap ---
    E0 = [r for r in rec if r['d1'] == 0]
    E1 = [r for r in rec if r['d1'] == 1]
    v['B1'] = len(E0) == len(E1)
    ne = bool(E0) and bool(E1)
    S = lambda R, key: sum(r[key] for r in R)
    if ne:
        v['B2'] = (al * S(E0, 'n') == abs(be) * S(E1, 'a')) or (al * S(E1, 'n') == abs(be) * S(E0, 'a')) or (S(E0, 't') == S(E1, 't'))
    else:
        v['B2'] = False
    v['Ba'] = ne and S(E0, 'a') == S(E1, 'a')
    v['Bt'] = ne and S(E0, 't') == S(E1, 't')
    v['BMn'] = ne and bol(p, S(E0, 'n')) and bol(p, S(E1, 'n'))
    v['BMt'] = ne and bol(p, S(E0, 't')) and bol(p, S(E1, 't'))
    v['BMt2'] = ne and bol(pp, S(E0, 't')) and bol(pp, S(E1, 't'))
    # --- aileler ---
    F = [r for r in rec if r['t'] % p == 0]
    H = [r for r in rec if r['u'] % p == 0]
    q = {r['n']: r['t'] // p for r in F}
    rr = {r['n']: r['u'] // p for r in H}
    k = len(F)
    kH = len(H)
    hucre = {(i, j): [r for r in F if r['d1'] == i and r['d2'] == j] for i in (0, 1) for j in (0, 1)}
    c = {key: len(L) for key, L in hucre.items()}
    s = {key: sum(q[r['n']] for r in L) for key, L in hucre.items()}
    dolu = all(c[key] >= 1 for key in c)
    yari_n = [c[(0, 0)] + c[(0, 1)], c[(1, 0)] + c[(1, 1)]]
    yari_s = [s[(0, 0)] + s[(0, 1)], s[(1, 0)] + s[(1, 1)]]
    sut_n = [c[(0, 0)] + c[(1, 0)], c[(0, 1)] + c[(1, 1)]]
    sut_s = [s[(0, 0)] + s[(1, 0)], s[(0, 1)] + s[(1, 1)]]
    kos_n = [c[(0, 0)] + c[(1, 1)], c[(0, 1)] + c[(1, 0)]]
    kos_s = [s[(0, 0)] + s[(1, 1)], s[(0, 1)] + s[(1, 0)]]
    sq = sum(q.values())
    hs = [s[(0, 0)], s[(0, 1)], s[(1, 0)], s[(1, 1)]]
    hn = [c[(0, 0)], c[(0, 1)], c[(1, 0)], c[(1, 1)]]

    def denge(nn, ss):
        return (nn[0] == nn[1] and nn[0] >= 1), (nn[0] >= 1 and nn[1] >= 1 and ss[0] == ss[1])
    v['F1'], v['F2'] = denge(yari_n, yari_s)
    v['FC1'], v['FC2'] = denge(sut_n, sut_s)
    v['FD1'], v['FD2'] = denge(kos_n, kos_s)
    v['R0c'], v['R0e'] = denge([c[(0, 0)], c[(0, 1)]], [s[(0, 0)], s[(0, 1)]])
    v['R1c'], v['R1e'] = denge([c[(1, 0)], c[(1, 1)]], [s[(1, 0)], s[(1, 1)]])
    v['K1'] = dolu and kosu(hs)
    v['K2'] = k >= 2 and kosu(list(q.values()))
    v['Qc'] = dolu and sq == 4 * p
    v['QS'] = k >= 1 and bol(p, sq)
    # skaler nicelik ilişkileri: = p, = 2p, p |, p² |
    def skaler(onek, x, ok, adlar=('p', '2p', 'd', 'd2')):
        rel = {'p': x == p, '2p': x == 2 * p, 'd': bol(p, x), 'd2': bol(pp, x)}
        for r in adlar:
            v[onek + r] = ok and rel[r]
    skaler('FS', sq, k >= 1, ('p', '2p', 'd2'))
    skaler('FA', sum(r['a'] for r in F), k >= 1)
    skaler('FN', sum(r['n'] for r in F), k >= 1)
    # demet ilişkileri: her biri = p, her biri = 2p, merkez = p, p | her biri, p² | her biri (bütün parçalar dolu)
    def demet(onek, xs, ok, adlar=('p', '2p', 'm', 'd', 'd2')):
        rel = {'p': all(x == p for x in xs), '2p': all(x == 2 * p for x in xs), 'm': sum(xs) == len(xs) * p,
               'd': all(bol(p, x) for x in xs), 'd2': all(bol(pp, x) for x in xs)}
        for r in adlar:
            v[onek + r] = ok and rel[r]
    yari_dolu = yari_n[0] >= 1 and yari_n[1] >= 1
    sut_dolu = sut_n[0] >= 1 and sut_n[1] >= 1
    demet('FH', yari_s, yari_dolu, ('p', '2p', 'm', 'd2'))
    v['Qh'] = yari_dolu and bol(p, yari_s[0]) and bol(p, yari_s[1])
    demet('FC', sut_s, sut_dolu)
    demet('FHS', yari_n, yari_dolu)
    v['FRK'] = yari_dolu and abs(yari_s[0] - yari_s[1]) == 1
    v['FCK'] = sut_dolu and abs(sut_s[0] - sut_s[1]) == 1
    demet('FK', hs, dolu, ('p', '2p', 'd', 'd2'))
    demet('FKS', hn, dolu)
    v['FAP'] = dolu and aritmetik(hs)
    v['FKS'] = dolu and kosu(hn)
    v['FSC'] = any(len(L) >= 2 and len({q[r['n']] for r in L}) == 1 for L in hucre.values())
    # --- aile boyu (Z) ---
    v['QkF'] = k == p
    v['Zk2p'] = k == 2 * p
    v['Zkd'] = k >= 1 and bol(p, k)
    v['Zkd2'] = k >= 1 and bol(pp, k)
    # --- iki el, birleşim (W) ---
    Fn = {r['n'] for r in F}
    Hn = {r['n'] for r in H}
    U = [r for r in rec if r['n'] in Fn or r['n'] in Hn]
    ni = len(Fn & Hn)
    kU = len(U)
    v['W1'] = k >= 1 and kH >= 1 and ni == 0
    v['QU'] = kU == p
    v['QkH'] = kH == p
    v['Qa'] = kU >= 1 and bol(p, sum(r['a'] for r in U))
    v['Qn'] = kU >= 1 and bol(p, sum(r['n'] for r in U))
    v['QH'] = kH >= 1 and bol(p, sum(rr.values()))
    v['WI1'] = ni == 1
    v['WEq'] = k >= 1 and kH >= 1 and k == kH
    v['WSum'] = k >= 1 and kH >= 1 and k + kH == p
    v['WU2p'] = kU == 2 * p
    v['WUd'] = kU >= 1 and bol(p, kU)
    v['WUd2'] = kU >= 1 and bol(pp, kU)
    v['WH2p'] = kH == 2 * p
    v['WHd'] = kH >= 1 and bol(p, kH)
    v['WHd2'] = kH >= 1 and bol(pp, kH)
    v['WIp'] = ni == p
    v['WI2p'] = ni == 2 * p
    v['WId'] = ni >= 1 and bol(p, ni)
    v['WId2'] = ni >= 1 and bol(pp, ni)
    skaler('WUA', sum(r['a'] for r in U), kU >= 1, ('p', '2p', 'd2'))
    skaler('WUN', sum(r['n'] for r in U), kU >= 1, ('p', '2p', 'd2'))
    skaler('WUT', sum(r['t'] for r in U), kU >= 1)
    skaler('WHA', sum(r['a'] for r in H), kH >= 1)
    skaler('WHN', sum(r['n'] for r in H), kH >= 1)
    skaler('WHR', sum(rr.values()), kH >= 1, ('p', '2p', 'd2'))
    # --- karşı aile içi (H) ---
    hh = {(i, j): [r for r in H if r['d1'] == i and r['d2'] == j] for i in (0, 1) for j in (0, 1)}
    hc = {key: len(L) for key, L in hh.items()}
    hr = {key: sum(rr[r['n']] for r in L) for key, L in hh.items()}
    v['HH1'], v['HH2'] = denge([hc[(0, 0)] + hc[(0, 1)], hc[(1, 0)] + hc[(1, 1)]], [hr[(0, 0)] + hr[(0, 1)], hr[(1, 0)] + hr[(1, 1)]])
    v['HR0c'], v['HR0e'] = denge([hc[(0, 0)], hc[(0, 1)]], [hr[(0, 0)], hr[(0, 1)]])
    v['HR1c'], v['HR1e'] = denge([hc[(1, 0)], hc[(1, 1)]], [hr[(1, 0)], hr[(1, 1)]])
    v['HK2'] = kH >= 2 and kosu(list(rr.values()))
    for x in dislanan(f, p, sum(a)):
        v[x] = False
    assert set(v) == set(TUM), set(TUM) ^ set(v)
    bilgi = dict(kF=k, kH=kH, mc=min(hn))
    return v, bilgi


def maskeler(v):
    """grup maskeleri (C ile aynı bit sırası)"""
    return {g: sum(1 << i for i, ad in enumerate(L) if v[ad]) for g, L in GRUPLAR.items()}
