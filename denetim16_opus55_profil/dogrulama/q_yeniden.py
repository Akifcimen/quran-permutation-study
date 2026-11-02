# q_yeniden.py — ONKAYIT §6.2: Kur'an'ın olaylarının bütün isabetlerinin (.q kayıtları) bağımsız yeniden hesabı. Her kayıt için 114 ayet
# sayısı ve mercek okunur; kaydın gerçekten bir dizilim olduğu (Kur'an'ın 114 sayısının permütasyonu), merceğin Kur'an'ın merceği olduğu
# ve kapı Gc ile Kur'an'ın dört dil yükleminin (B2, K1, Qc, WSum) o mercekte tuttuğu tanımlardan (isabet_yeniden.py'nin yüklemleri) hesaplanır.
# Kullanım: python3 -I q_yeniden.py <sonuc/tab>
import sys, os, glob, struct
D = sys.argv[1]
AL = [1,1,1,1,1,1,2,2,2,2,3,3,3,3]; BE = [1,-1,2,-2,3,-3,1,-1,3,-3,1,-1,2,-2]
PR = [3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113]
def prof(a, al, be, p):
    N = range(1, 115); t = {n: al*n + be*a[n-1] for n in N}; u = {n: al*n - be*a[n-1] for n in N}
    E0 = [n for n in N if t[n] % 2 == 0]; E1 = [n for n in N if t[n] % 2]
    sn0, sa1, sn1, sa0 = sum(E0), sum(a[n-1] for n in E1), sum(E1), sum(a[n-1] for n in E0)
    st0, st1 = sum(t[n] for n in E0), sum(t[n] for n in E1); abe = abs(be)
    F = [n for n in N if t[n] % p == 0]; H = [n for n in N if u[n] % p == 0]
    cell = lambda n: 2*(t[n] % 2) + n % 2
    c = [sum(1 for n in F if cell(n) == i) for i in range(4)]; s = [sum(t[n]//p for n in F if cell(n) == i) for i in range(4)]
    d = sorted(set(s)); kosu = len(d) >= 2 and d[-1]-d[0]+1 == len(d)
    return dict(Gc=min(c) >= 2, B2=bool(E0 and E1) and (al*sn0 == abe*sa1 or al*sn1 == abe*sa0 or st0 == st1), K1=all(c) and kosu,
                Qc=all(c) and sum(s) == 4*p, WSum=bool(F and H) and len(F)+len(H) == p, hucre=c, kF=len(F), kH=len(H))
def kuran(fn):
    b = open(fn, 'rb').read(232); assert struct.unpack('<i', b[:4])[0] == 0; return list(struct.unpack('<114h', b[4:]))
Q = kuran(os.path.join(D, 'M', 'hedef.bin')); assert all(kuran(os.path.join(D, S, 'hedef.bin')) == Q for S in 'LU')
print('Kur\'an kendi merceğinde (1, 1, 19):', {k: v for k, v in prof(Q, 1, 1, 19).items()})
toplam = 0
for S in 'MLU':
    ev = {w[0]: int(w[2]) for w in (l.split() for l in open(os.path.join(D, S, 'olaylar.txt'))) if w[0].startswith('A000.')}
    for fn in sorted(glob.glob(os.path.join(D, S, 'd*', 'b*.q'))):
        raw = open(fn, 'rb').read(); assert len(raw) % 240 == 0
        for j in range(len(raw) // 240):
            sira, puan, L, sinif, dil = struct.unpack('<IfhBB', raw[240*j:240*j+12]); a = list(struct.unpack('<114H', raw[240*j+12:240*j+240]))
            f, i = divmod(L, len(PR)); v = prof(a, AL[f], BE[f], PR[i]); toplam += 1
            ok = sorted(a) == sorted(Q) and a != Q and L == ev['A000.1'] and all(v[k] for k in ('Gc', 'B2', 'K1', 'Qc', 'WSum'))
            print(f'{S} {os.path.relpath(fn, D)} sıra {sira} olay #{dil} mercek {L} (α={AL[f]}, β={BE[f]}, p={PR[i]}): dizilim {sorted(a) == sorted(Q)}; '
                  + ' '.join(f'{k} {v[k]}' for k in ('Gc', 'B2', 'K1', 'Qc', 'WSum')) + f'; hücre {v["hucre"]}, k_F {v["kF"]}, k_H {v["kH"]} → {"TAMAM" if ok else "UYUMSUZ"}')
print(f'toplam {toplam} kayıt')
