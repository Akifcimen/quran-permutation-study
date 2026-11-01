# isabet_yeniden.py — dış incelemenin (1 Kasım 2026) iç sayım iddiasının bağımsız yeniden hesabı. Kur'an'ın Gc 10¹⁰ sayımında puanına
# ulaşan her kontrolün (denetim14 .../kuran/Gc10/<S>/isabet.txt; 114 ayet sayısı ve mercek kayıtlı) kendi merceğinde Kur'an'ın dört dil
# yüklemini (B2, K1, Qc, WSum) ve Kur'an'ın merceğindeki başka yedi olgusunu (B1 57/57, hücreler 3/3/3/3, katsayı bandı K2, ayrık eller W1,
# 19 üyeli birleşim QU, birleşim ayet toplamının 19'a bölünmesi Qa, yarılar 38/38 FH2p) taşıyıp taşımadığını tanımlardan hesaplar.
# Kullanım: python3 -I isabet_yeniden.py <.../kuran/Gc10 dizini>
import sys, os
D = sys.argv[1]
def prof(a, al, be, p):
    N = range(1, 115); t = {n: al*n + be*a[n-1] for n in N}; u = {n: al*n - be*a[n-1] for n in N}
    E0 = [n for n in N if t[n] % 2 == 0]; E1 = [n for n in N if t[n] % 2]
    sn0, sa1, sn1, sa0 = sum(E0), sum(a[n-1] for n in E1), sum(E1), sum(a[n-1] for n in E0)
    st0, st1 = sum(t[n] for n in E0), sum(t[n] for n in E1); abe = abs(be)
    F = [n for n in N if t[n] % p == 0]; H = [n for n in N if u[n] % p == 0]
    cell = lambda n: 2*(t[n] % 2) + n % 2
    c = [sum(1 for n in F if cell(n) == i) for i in range(4)]; s = [sum(t[n]//p for n in F if cell(n) == i) for i in range(4)]
    q = [t[n]//p for n in F]; U = set(F) | set(H); I = set(F) & set(H)
    d = sorted(set(s)); kosu = len(d) >= 2 and d[-1]-d[0]+1 == len(d)
    qd = sorted(set(q)); band = len(q) >= 2 and 2 <= qd[-1]-qd[0]+1 <= len(q) and len(qd) == qd[-1]-qd[0]+1
    return dict(B2=bool(E0 and E1) and (al*sn0 == abe*sa1 or al*sn1 == abe*sa0 or st0 == st1), K1=all(c) and kosu, Qc=all(c) and sum(s) == 4*p,
                WSum=bool(F and H) and len(F)+len(H) == p, B1=len(E0) == len(E1), H3333=c == [3, 3, 3, 3], K2=band, W1=not I and bool(F and H),
                QU=len(U) == p, Qa=bool(U) and sum(a[n-1] for n in U) % p == 0, FH2p=s[0]+s[1] == 2*p and s[2]+s[3] == 2*p)
for S in 'MLU':
    rows = [l.split() for l in open(os.path.join(D, S, 'isabet.txt')) if not l.startswith('#')]
    v = [prof([int(x) for x in w[6:120]], int(w[3]), int(w[4]), int(w[5])) for w in rows]
    print(f"{S}: {len(rows)} isabet; dört dil yüklemi birden {sum(all(x[k] for k in ('B2','K1','Qc','WSum')) for x in v)}; on bir olgunun hepsi {sum(all(x.values()) for x in v)}")
