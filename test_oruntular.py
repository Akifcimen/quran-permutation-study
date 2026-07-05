#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""dokuman.md'deki tüm matris/örüntü iddialarının bağımsız testi.
Veri: api.alquran.cloud + api.quran.com (çapraz doğrulanmış, standart Kûfe sayımı).
İki kabul test edilir: STANDART (Tevbe=129, toplam 6236) ve RK127 (Tevbe=127, toplam 6234).
"""
import json

MOD = 19

d = json.load(open('/Users/dejuremacminim1/Desktop/hrf2/quran_meta.json'))
AYAT_STD = {s['number']: s['numberOfAyahs'] for s in d['data']['surahs']['references']}
AYAT_127 = dict(AYAT_STD); AYAT_127[9] = 127

# ---------- mod-19 2x2 matris araçları ----------
def mmul(A, B, m=MOD):
    return [[(A[0][0]*B[0][0]+A[0][1]*B[1][0]) % m, (A[0][0]*B[0][1]+A[0][1]*B[1][1]) % m],
            [(A[1][0]*B[0][0]+A[1][1]*B[1][0]) % m, (A[1][0]*B[0][1]+A[1][1]*B[1][1]) % m]]

I = [[1,0],[0,1]]
NEG_I = [[MOD-1,0],[0,MOD-1]]

def mpow(A, n):
    R = I; B = [row[:] for row in A]
    while n:
        if n & 1: R = mmul(R, B)
        B = mmul(B, B); n >>= 1
    return R

def det(A): return (A[0][0]*A[1][1] - A[0][1]*A[1][0]) % MOD

def order(A, cap=123120):
    if det(A) == 0: return None
    X = [row[:] for row in A]
    for k in range(1, cap+1):
        if X == I: return k
        X = mmul(X, A)
    return None

def minv(A):
    dd = det(A)
    di = pow(dd, MOD-2, MOD)
    return [[(A[1][1]*di) % MOD, (-A[0][1]*di) % MOD],
            [(-A[1][0]*di) % MOD, (A[0][0]*di) % MOD]]

def red(A): return [[A[0][0] % MOD, A[0][1] % MOD], [A[1][0] % MOD, A[1][1] % MOD]]

def group_matrix(ayat, pred):
    sel = [n for n in range(1,115) if pred(n, ayat[n])]
    rest = [n for n in range(1,115) if n not in sel]
    M = [[sum(sel), sum(ayat[n] for n in sel)],
         [sum(rest), sum(ayat[n] for n in rest)]]
    return sel, M

def report_matrix(name, M):
    Mr = red(M)
    o = order(Mr)
    half = mpow(Mr, 180) == NEG_I if o else False
    p20 = mpow(Mr, 20)
    scalar20 = p20[0][1] == 0 and p20[1][0] == 0 and p20[0][0] == p20[1][1]
    print(f"  {name}: ham={M}  mod19={Mr}  det={det(Mr)}  ord={o}  M^180==-I:{half}  M^20={'%dI'%p20[0][0] if scalar20 else p20}")
    return Mr, o

print("="*100)
for label, AYAT in [("STANDART KABUL (Tevbe=129, toplam=%d)" % sum(AYAT_STD.values()), AYAT_STD),
                    ("RK-127 KABULÜ (Tevbe=127, toplam=%d)" % sum(AYAT_127.values()), AYAT_127)]:
    print("\n" + "#"*100)
    print("# VERİ SETİ:", label)
    print("#"*100)

    # --- 1) R matrisi: t = n + ayet ≡ 0 (mod 19) ---
    sel, Mr_raw = group_matrix(AYAT, lambda n,a: (n+a) % 19 == 0)
    print(f"\n[1] t≡0 mod19 sûreleri ({len(sel)} adet): {sel}")
    print(f"    seçilen: sûre toplamı={Mr_raw[0][0]}, ayet toplamı={Mr_raw[0][1]} (iddia: 590, 854)")
    print(f"    kalan  : sûre toplamı={Mr_raw[1][0]}, ayet toplamı={Mr_raw[1][1]} (iddia: 5965, 5380)")
    R, ordR = report_matrix("R", Mr_raw)

    # --- 2) P matrisi: t çift / tek ---
    selP, Mp_raw = group_matrix(AYAT, lambda n,a: (n+a) % 2 == 0)
    print(f"\n[2] t çift sûre sayısı: {len(selP)}")
    print(f"    iddia edilen ham P: [[3303,2931],[3252,3303]]")
    P, ordP = report_matrix("P", Mp_raw)

    # --- 3) tüm r sınıfları taraması ---
    print("\n[3] t ≡ r mod 19 taraması (tam periyot):")
    res = []
    for r in range(19):
        _, M = group_matrix(AYAT, lambda n,a,r=r: (n+a) % 19 == r)
        o = order(red(M))
        res.append((r, o))
    print("    " + ", ".join(f"r={r}:ord={o}" for r,o in res))
    print("    ord=360 verenler:", [r for r,o in res if o == 360], "(iddia: [0, 4])")

    # --- 4) r=4 detayı ---
    sel4, M4_raw = group_matrix(AYAT, lambda n,a: (n+a) % 19 == 4)
    print(f"\n[4] r=4 sûreleri: {sel4} (iddia: [7,35,58,71,78,113])")
    print(f"    toplamlar: {M4_raw[0]} (iddia: [362,346]); kalan {M4_raw[1]} (iddia: [6193,5888])")
    report_matrix("R4", M4_raw)

    # --- 5) diğer doğal kurallar ---
    print("\n[5] Diğer kurallar:")
    for nm, pred, iddia in [
        ("sûre no çift/tek", lambda n,a: n % 2 == 0, "tersinir değil"),
        ("ayet çift/tek", lambda n,a: a % 2 == 0, "ord=72"),
        ("ayet ≡0 mod19", lambda n,a: a % 19 == 0, "ord=18"),
    ]:
        _, M = group_matrix(AYAT, pred)
        o = order(red(M))
        print(f"    {nm}: ord={o} (iddia: {iddia})")
    # t tekrar edenler
    from collections import Counter
    tvals = Counter((n + AYAT[n]) for n in range(1,115))
    _, Mt = group_matrix(AYAT, lambda n,a: tvals[n+a] > 1)
    print(f"    t tekrar edenler: ord={order(red(Mt))} (iddia: ord=72)")
    _, Mh = group_matrix(AYAT, lambda n,a: tvals[n+a] > 1 and (n+a) % 19 == 0)
    print(f"    t hem 19-katı hem tekrar: ord={order(red(Mh))} (iddia: ord=18)")

    # --- 6) P-R etkileşimi ---
    print("\n[6] P-R etkileşimi:")
    PR, RPm = mmul(P, R), mmul(R, P)
    print(f"    PR={PR} ord={order(PR)} (iddia: [[11,18],[6,7]], ord=18)")
    print(f"    RP={RPm} ord={order(RPm)} (iddia: [[13,8],[12,5]], ord=18)")
    S = red([[PR[i][j]+RPm[i][j] for j in range(2)] for i in range(2)])
    print(f"    S=PR+RP={S} ord={order(S)} S^180==-I:{mpow(S,180)==NEG_I} (iddia: [[5,7],[18,12]], ord=360)")
    D = red([[PR[i][j]-RPm[i][j] for j in range(2)] for i in range(2)])
    print(f"    D=PR-RP={D} D^2==I:{mmul(D,D)==I} (iddia: [[17,10],[13,2]], D^2=I)")
    if det(P) and det(R):
        K = mmul(mmul(P,R), mmul(minv(P), minv(R)))
        print(f"    K=PRP⁻¹R⁻¹={K} ord={order(K)} K^10==-I:{mpow(K,10)==NEG_I} (iddia: ord=20)")
    # ayna iddiaları
    if det(D):
        for nm, M in [("P",P),("R",R),("S",S)]:
            lhs = mmul(mmul(D,M),D); rhs = mpow(M,19)
            print(f"    D{nm}D == {nm}^19 : {lhs==rhs}  ({nm}^19={rhs})")

    # --- 7) <P,R> grup üretimi (BFS) ---
    if det(P) and det(R):
        seen = {tuple(I[0]+I[1])}
        frontier = [I]
        gens = [P, R, minv(P), minv(R)]
        while frontier:
            nf = []
            for X in frontier:
                for g in gens:
                    Y = mmul(X, g)
                    t_ = tuple(Y[0]+Y[1])
                    if t_ not in seen:
                        seen.add(t_); nf.append(Y)
            frontier = nf
        print(f"\n[7] |<P,R>| = {len(seen)} (iddia: 123120 = |GL2(F19)|)")

# --- 8) kabule bağlı olmayan cebirsel iddialar ---
print("\n" + "#"*100)
print("# SAF CEBİRSEL İDDİALAR (veri kabulünden bağımsız)")
print("#"*100)
Rclaim = [[1, 18], [18, 3]]
K2 = [[18,18],[18,1]]  # K = R - 2I = [[-1,-1],[-1,1]]
print(f"[8a] K=R-2I için K²={mmul(K2,K2)} == 2I: {mmul(K2,K2)==[[2,0],[0,2]]}")
print(f"[8b] R^19 = {mpow(Rclaim,19)}  2I-K = {red([[2-(-1),2-(-1)],[2-(-1),2-1]])} ... iddia R^19=2I-K={[[(2-(-1))%19,(0-(-1))%19],[(0-(-1))%19,(2-1)%19]]}")
print(f"     R^20 = {mpow(Rclaim,20)} (iddia: 2I)")
print(f"[8c] mod 19'da 2'nin çarpımsal mertebesi: {next(k for k in range(1,19) if pow(2,k,19)==1)} (iddia: 18); 2^9 mod 19 = {pow(2,9,19)} (iddia: 18 ≡ -1)")
print(f"[8d] |GL2(F19)| = (19²-1)(19²-19) = {(361-1)*(361-19)} (iddia: 123120)")

# s-taraması: R_s = [[s,-s],[-s,s+2]]
print("[8e] R_s=[[s,-s],[-s,s+2]] taraması — ord=360 veren s değerleri:",
      [s for s in range(19) if order(red([[s,-s],[-s,s+2]])) == 360], "(iddia: [1,16])")
# c-taraması: P_c = [[16,c],[3,16]]
print("[8f] P_c=[[16,c],[3,16]] taraması — ord=360 veren c değerleri:",
      [c for c in range(19) if order([[16,c],[3,16]]) == 360], "(iddia: [5,6,11,17])")
print("     P_129=[[16,7],[3,16]] ord =", order([[16,7],[3,16]]), "(iddia: 30)")

# rastgelelik kıyası: GL2(F19) içinde ord=360 olan eleman oranı
tot = 0; o360 = 0
for a in range(19):
    for b in range(19):
        for c in range(19):
            for dd in range(19):
                M = [[a,b],[c,dd]]
                if det(M) == 0: continue
                tot += 1
                if order(M) == 360: o360 += 1
print(f"[8g] GL2(F19) içinde ord=360 eleman oranı: {o360}/{tot} = %{100*o360/tot:.1f}  (rastgele bir tersinir matrisin 360 verme taban olasılığı)")
