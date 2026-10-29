# Ana hattın bağımsız yüklem kontrolü (oturumun kodunu içe aktarmaz): tekdüze hedeflerin hedef.bin'inden (pakette) Kur'an'ı ve x068'i
# okur, tanımları ön-kayıttan ve L* tanım dosyasından yeniden yazar. Kullanım: python3 -I yuklem_kontrol.py <paket>/tab/U/hedef.bin
import sys, struct
def oku(fn):
    b = open(fn, 'rb').read(); kay = {}
    for i in range(0, len(b), 4 + 228):
        hid = struct.unpack('<i', b[i:i + 4])[0]; kay[hid] = list(struct.unpack('<114h', b[i + 4:i + 232]))
    return kay
def profil(a, al, be, p):
    t = {n: al * n + be * a[n - 1] for n in range(1, 115)}
    u = {n: al * n - be * a[n - 1] for n in range(1, 115)}
    F = [n for n in t if t[n] % p == 0]; H = [n for n in u if u[n] % p == 0]
    q = {n: t[n] // p for n in F}
    hucre = {(i, j): [n for n in F if t[n] % 2 == i and n % 2 == j] for i in (0, 1) for j in (0, 1)}
    s = {k: sum(q[n] for n in L) for k, L in hucre.items()}; c = {k: len(L) for k, L in hucre.items()}
    E0 = [n for n in t if t[n] % 2 == 0]; E1 = [n for n in t if t[n] % 2 == 1]
    kosu = lambda v: len(set(v)) >= 2 and max(v) - min(v) + 1 == len(set(v))
    dolu = all(c[k] >= 1 for k in c)
    y = [c[0, 0] + c[0, 1], c[1, 0] + c[1, 1]]; ys = [s[0, 0] + s[0, 1], s[1, 0] + s[1, 1]]
    U = sorted(set(F) | set(H))
    v = dict(
        B2=bool(E0 and E1) and (al * sum(E0) == abs(be) * sum(a[n - 1] for n in E1) or al * sum(E1) == abs(be) * sum(a[n - 1] for n in E0)
                                or sum(t[n] for n in E0) == sum(t[n] for n in E1)),
        K1=dolu and kosu([s[k] for k in s]), Qc=dolu and sum(q.values()) == 4 * p,
        F2=y[0] >= 1 and y[1] >= 1 and ys[0] == ys[1], K2=len(F) >= 2 and kosu(list(q.values())),
        FSC=any(len(L) >= 2 and len({q[n] for n in L}) == 1 for L in hucre.values()),
        WUN2p=len(U) >= 1 and sum(U) == 2 * p, W1=len(F) >= 1 and len(H) >= 1 and not (set(F) & set(H)))
    return v, dict(kF=len(F), kH=len(H), hucre_sayi=c, hucre_top=s, U=len(U), sumUn=sum(U))
kay = oku(sys.argv[1])
Q = kay[0]
print("Kur'an kaydı: Σa =", sum(Q), "ilk 5:", Q[:5], "son 3:", Q[-3:])
print("Kur'an (1,+1,19):", profil(Q, 1, 1, 19))
print("x068 (3,+2,43):", profil(kay[68], 3, 2, 43))
print("x068 ayet sayıları (n = 1..114):", kay[68])
