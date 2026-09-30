# Bağımsız doğrulama: iddia edilen tüm eşitlikler (kodeks 127 ve 129)
import json
from collections import Counter
d = json.load(open('quran_meta.json'))
A129 = [s['numberOfAyahs'] for s in d['data']['surahs']['references']]
A127 = list(A129); A127[8] = 127
for tag, A in (("127", A127), ("129", A129)):
    N = list(range(1, 115))
    t = [n + a for n, a in zip(N, A)]; u = [n - a for n, a in zip(N, A)]
    E = [i for i in range(114) if t[i] % 2 == 0]; O = [i for i in range(114) if t[i] % 2 == 1]
    print(f"--- kodeks {tag}: Σa={sum(A)} Σn={sum(N)}")
    print(f" terazi: |E|={len(E)} |O|={len(O)} Σt(E)={sum(t[i] for i in E)} Σt(O)={sum(t[i] for i in O)}")
    print(f"   Σn(E)={sum(N[i] for i in E)} Σa(O)={sum(A[i] for i in O)} Σa(E)={sum(A[i] for i in E)} Σn(O)={sum(N[i] for i in O)}")
    F = [i for i in range(114) if t[i] % 19 == 0]
    c = [t[i] // 19 for i in F]
    print(f" aile: {[i+1 for i in F]} k={len(F)} Σt={sum(t[i] for i in F)} katsayılar={c}")
    Fe = [i for i in F if t[i] % 2 == 0]; Fo = [i for i in F if t[i] % 2 == 1]
    print(f"   çift yarı {[i+1 for i in Fe]} Σt={sum(t[i] for i in Fe)} | tek yarı {[i+1 for i in Fo]} Σt={sum(t[i] for i in Fo)}")
    cells = {}
    for i in F:
        key = ('te' if t[i] % 2 == 0 else 'to', 'ne' if (i+1) % 2 == 0 else 'no')
        cells.setdefault(key, []).append(t[i] // 19)
    print("   hücreler:", {k: (v, sum(v)) for k, v in sorted(cells.items())})
    G = [i for i in range(114) if u[i] % 19 == 0]
    U = sorted(set(F) | set(G))
    print(f" u-aile: {[i+1 for i in G]} k_u={len(G)} u={[u[i] for i in G]} kesişim={set(F)&set(G)} |birleşim|={len(U)} Σa(birl)={sum(A[i] for i in U)} mod19={sum(A[i] for i in U)%19} Σt(birl)={sum(t[i] for i in U)}")
