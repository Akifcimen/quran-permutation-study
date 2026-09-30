# KEŞİF AMAÇLI (ön-kayıt dışı) tanı: denetçinin M sıfırının (S ≤ S_obs, tam-azalandan başlayan transpozisyon zinciri,
# 30.000 adım yakma) dengeye ulaşıp ulaşmadığı. S dağılımı yakmadan sonra ve çok daha uzun koşudan sonra karşılaştırılır.
import numpy as np
A0 = np.array([7,286,200,176,120,165,206,75,127,109,123,111,43,52,99,128,111,110,98,135,112,78,118,64,77,227,93,88,69,60,
 34,30,73,54,45,83,182,88,75,85,54,53,89,59,37,35,38,29,18,45,60,49,62,55,78,96,29,22,24,13,14,11,11,18,12,12,30,52,52,44,
 28,28,20,56,40,31,50,40,46,42,29,19,36,25,22,17,19,26,30,20,15,21,11,8,8,19,5,8,8,11,11,8,3,9,5,4,7,3,6,3,5,4,5,6])
N = np.arange(1, 115); S_obs = int((N * A0).sum())
rng = np.random.default_rng(7); CH = 512
P = np.tile(np.sort(A0)[::-1], (CH, 1)); S = P @ N; rows = np.arange(CH)
def step(P, S, n):
    for _ in range(n):
        i = rng.integers(0, 114, CH); j = rng.integers(0, 114, CH)
        Pi = P[rows, i]; Pj = P[rows, j]; dS = (N[i] - N[j]) * (Pj - Pi); ok = (S + dS) <= S_obs
        P[rows[ok], i[ok]] = Pj[ok]; P[rows[ok], j[ok]] = Pi[ok]; S = S + dS * ok
    return P, S
def t_stats(P):
    T = N[None, :] + P
    return np.mean(np.std(T, axis=1)), np.mean(((T >= 95) & (T <= 171)).mean(axis=1)), np.mean((T % 19 == 0).sum(1))
print(f"S_obs={S_obs}  S_min(tam azalan)={int((N*np.sort(A0)[::-1]).sum())}")
done = 0
for target in (30000, 60000, 120000, 240000):
    P, S = step(P, S, target - done); done = target
    sd, band, k19 = t_stats(P)
    print(f"adım {target:>6}: S ortalaması {S.mean():.0f} (S_obs'un %{100*S.mean()/S_obs:.2f}'i), S std {S.std():.0f} | "
          f"t std {sd:.1f} | t∈[95,171] oranı {band:.3f} | ort. 19-aile boyu {k19:.2f}")
T = N + A0
print(f"gerçek kitap: t std {np.std(T):.1f} | t∈[95,171] oranı {np.mean((T>=95)&(T<=171)):.3f} | 19-aile boyu {(T%19==0).sum()}")
