# plasebo_dil_sina.py — kapı çalışması ön-kaydı §7.2: hedeflerin Kol A dillerini kendi merceklerinde C kodundan bağımsız olarak sınar.
# Yüklemler tanımlarından yazıldı (kütüphane L*: kitap B2, BMn; aile F1… ; iki el ve birleşim W…; karşı aile H…). Tür/düzey sınıflaması
# agac_uretim.h'deki PTUR/PDUZEY tablosundan, adlar kutuphane.h'deki PADI'den okunur (sınıflama tablosu; hesap değil).
# Kullanım: python3 -I plasebo_dil_sina.py <kaynak_src> <hedef.bin> <diller_A.txt> <KAPI_ESIK> <KAPI_HUCRE> <ad>...
import re, struct, sys
src, hedef, diller, KE, KH = sys.argv[1], sys.argv[2], sys.argv[3], int(sys.argv[4]), int(sys.argv[5]); adlar = sys.argv[6:]
h = open(f'{src}/kutuphane.h').read(); padi = re.findall(r'"([A-Za-z0-9]+)"', h[h.index('static const char *PADI'):h.index('static inline int pgrup')])
ag = open(f'{src}/agac_uretim.h').read()
PDUZ = [int(x) for x in re.search(r'PDUZEY\[NPRED\]=\{([^}]*)\}', ag).group(1).split(',')]
PTUR = [int(x) for x in re.search(r'PTUR\[NPRED\]=\{([^}]*)\}', ag).group(1).split(',')]
FORMS = [(1, 1), (1, -1), (1, 2), (1, -2), (1, 3), (1, -3), (2, 1), (2, -1), (2, 3), (2, -3), (3, 1), (3, -1), (3, 2), (3, -2)]
PR = [3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113]
SN = 6555

def kosu(v):  # farklı değerler ardışık tamsayılar, en az iki farklı değer
    d = sorted(set(v)); return len(d) >= 2 and d[-1] - d[0] + 1 == len(d)

def yuklemler(a, f, p):
    al, be = FORMS[f]; N = range(1, 115); pp = p * p
    t = {n: al * n + be * a[n - 1] for n in N}; u = {n: al * n - be * a[n - 1] for n in N}
    E0 = [n for n in N if t[n] % 2 == 0]; E1 = [n for n in N if t[n] % 2 == 1]
    sn0, sn1 = sum(E0), sum(E1); sa0, sa1 = sum(a[n - 1] for n in E0), sum(a[n - 1] for n in E1); st0, st1 = sum(t[n] for n in E0), sum(t[n] for n in E1)
    v = {}
    v['B2'] = bool(E0 and E1) and (al * sn0 == abs(be) * sa1 or al * sn1 == abs(be) * sa0 or st0 == st1)
    v['BMn'] = bool(E0 and E1) and sn0 % p == 0 and sn1 % p == 0 and not (be % 2 == 0 or SN % p != 0)
    hucre = lambda n: 2 * (t[n] % 2) + (n % 2)
    F = [n for n in N if t[n] % p == 0]; H = [n for n in N if u[n] % p == 0]
    c = [0] * 4; s = [0] * 4; qs = {i: [] for i in range(4)}
    for n in F: i = hucre(n); c[i] += 1; s[i] += t[n] // p; qs[i].append(t[n] // p)
    hc = [0] * 4; hr = [0] * 4
    for n in H: i = hucre(n); hc[i] += 1; hr[i] += u[n] // p
    k, kH = len(F), len(H); dolu = all(c); h0, h1 = c[0] + c[1], c[2] + c[3]; q0, q1 = s[0] + s[1], s[2] + s[3]
    v['K1'] = dolu and kosu(s); v['Qc'] = dolu and sum(s) == 4 * p
    v['FRK'] = h0 >= 1 and h1 >= 1 and abs(q0 - q1) == 1
    v['FSp'] = k >= 1 and sum(s) == p
    v['FSC'] = any(c[i] >= 2 and len(set(qs[i])) == 1 for i in range(4))
    v['FKS'] = dolu and kosu(c) and be % 2 != 0
    I = set(F) & set(H); ni = len(I); U = set(F) | set(H); kU = len(U)
    saU = sum(a[n - 1] for n in U); snU = sum(U)
    p3 = (abs(be) % p == 0) or (al % p == 0)      # p = 3 dışlamaları (bu dosyadaki W yüklemleri için)
    v['W1'] = k >= 1 and kH >= 1 and ni == 0 and not p3
    v['Qn'] = kU >= 1 and snU % p == 0 and not (abs(be) % p == 0)
    v['WUAd2'] = kU >= 1 and saU % pp == 0
    v['WUNd2'] = kU >= 1 and snU % pp == 0 and not (abs(be) % p == 0)
    v['WSum'] = k >= 1 and kH >= 1 and k + kH == p and not p3
    hh0, hh1 = hc[0] + hc[1], hc[2] + hc[3]
    rv = [u[n] // p for n in H]
    def bant(vals):
        if len(vals) < 2: return False
        span = max(vals) - min(vals) + 1; return 2 <= span <= len(vals) and len(set(vals)) == span
    v['B1'] = len(E0) == len(E1) and al % 2 != 0 and be % 2 != 0
    v['F1'] = h0 == h1 and h0 >= 1
    v['F2'] = h0 >= 1 and h1 >= 1 and q0 == q1
    v['FKd'] = dolu and all(x % p == 0 for x in s)
    v['WI1'] = ni == 1 and not p3
    v['WHRd2'] = kH >= 1 and sum(rv) % pp == 0
    v['WHA2p'] = kH >= 1 and sum(a[n - 1] for n in H) == 2 * p
    v['HK2'] = bant(rv)
    v['Zk2p'] = k == 2 * p and not p3
    v['R0c'] = c[0] == c[1] and c[0] >= 1
    stU = sum(t[n] for n in U)
    v['WUTd2'] = kU >= 1 and stU % pp == 0
    v['FSd2'] = k >= 1 and sum(s) % pp == 0
    v['HR1c'] = hc[2] == hc[3] and hc[2] >= 1 and be % 2 != 0
    v['HH2'] = hh0 >= 1 and hh1 >= 1 and hr[0] + hr[1] == hr[2] + hr[3]
    # GE ve G0 hedeflerinin dilleri için eklenenler (29 Eki akşam)
    v['BMt'] = bool(E0 and E1) and st0 % p == 0 and st1 % p == 0
    qF = [t[n] // p for n in F]
    v['K2'] = len(qF) >= 2 and kosu(qF)
    k0, k1 = c[0] + c[2], c[1] + c[3]                       # sütunlar: n çift / n tek
    v['FC2'] = k0 >= 1 and k1 >= 1 and s[0] + s[2] == s[1] + s[3]
    v['WEq'] = k >= 1 and kH >= 1 and k == kH and not p3
    v['WHNd2'] = kH >= 1 and sum(H) % pp == 0 and not (abs(be) % p == 0)
    v['WUN2p'] = kU >= 1 and snU == 2 * p and not (abs(be) % p == 0)
    v['QS'] = k >= 1 and sum(s) % p == 0
    v['FD2'] = c[0] + c[3] >= 1 and c[1] + c[2] >= 1 and s[0] + s[3] == s[1] + s[2] and be % 2 != 0   # köşegenler 00∪11 / 01∪10
    v['HR0c'] = hc[0] == hc[1] and hc[0] >= 1 and be % 2 != 0
    v['QkH'] = kH == p and not p3
    v['FA2p'] = k >= 1 and sum(a[n - 1] for n in F) == 2 * p and not (al % p == 0)
    ss = sorted(s); v['FAP'] = dolu and ss[1] - ss[0] >= 1 and ss[2] - ss[1] == ss[1] - ss[0] and ss[3] - ss[2] == ss[1] - ss[0] and be % 2 != 0
    kapi = k * p >= KE and min(c) >= KH
    return kapi, v, k, kH, c, s

data = open(hedef, 'rb').read(); rec = lambda i: list(struct.unpack_from('<114h', data, i * 232 + 4))
satir = {l.split()[0]: l.split() for l in open(diller)}
bad = 0
for ad in adlar:
    w = satir[ad]; h_ = int(w[1]); L = int(w[3]); n = int(w[4]); g = [int(x) for x in w[5:5 + n]]; ad_y = [padi[x] for x in g]
    f, i = divmod(L, 29); p = PR[i]; kapi, v, k, kH, c, s = yuklemler(rec(h_), f, p)
    eksik = [y for y in ad_y if y not in v]
    if eksik: print(ad, 'tanımlanmamış yüklem:', eksik); bad += 1; continue
    sag = [y for y in ad_y if v[y]]; tur = {PTUR[x] for x in g if v[padi[x]]}; duz = {PDUZ[x] for x in g if v[padi[x]]}
    ok = kapi and len(sag) == n and len(tur) >= 3 and len(duz) >= 3
    bad += not ok
    print(f'{ad}: mercek {FORMS[f]},{p} kapı {kapi} kF {k} kH {kH} hücre {c} toplam {s} | dil {ad_y} sağlanan {sag} tür {len(tur)} düzey {len(duz)} → {"TAMAM" if ok else "UYUŞMAZLIK"}')
print('uyuşmazlık', bad)
