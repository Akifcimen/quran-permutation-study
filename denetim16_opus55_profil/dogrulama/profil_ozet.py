# profil_ozet.py — profil çalışmasının betimsel özeti (RAPOR için): olay maskelerinin yüklem adları, Kur'an'a yetişen plasebo olaylarının
# mercek, yüklem ve isabetleri; model bilgisinin Kur'an'ın çevresindeki dağılımı. Okuma kurallarını değiştirmez (onlar olay_say.py'de).
# Kullanım: python3 -I profil_ozet.py <src/kutuphane.h> <sonuc/tab>
import sys, os, re, glob
H, D = sys.argv[1], sys.argv[2]
src = open(H).read()
ad = re.findall(r'"([^"]+)"', re.search(r'PADI\[NPRED\]=\{(.*?)\};', src, re.S).group(1))
boy = [len(re.search(r'enum \{ (%s_[^}]*?), N%s_ \};' % (g, g), src, re.S).group(1).split(',')) for g in 'BZFWH']
ofs = [sum(boy[:k]) for k in range(5)]; assert sum(boy) == len(ad)
AL = [1,1,1,1,1,1,2,2,2,2,3,3,3,3]; BE = [1,-1,2,-2,3,-3,1,-1,3,-3,1,-1,2,-2]
PR = [3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113]
def mercek(L): f, i = divmod(L, 29); return f'({AL[f]}n{"+" if BE[f] > 0 else "−"}{abs(BE[f])}a, {PR[i]})'
def adlar(m): return [ad[ofs[k] + b] for k in range(5) for b in range(64) if m[k] >> b & 1]
A = {1: (1, 4), 2: (5, 40), 3: (41, 400), 4: (401, 40000)}
for S in 'MLU':
    olay = {}
    for s in open(os.path.join(D, S, 'olaylar.txt')):
        w = s.split(); olay[w[0]] = dict(hed=int(w[1]), L=int(w[2]), m=[int(x, 16) for x in w[3:8]], S=float(w[8]), bilgi=float(w[9]), n=int(w[10]))
    say = {}; nb = 0
    for j in (1, 2, 3, 4):
        for fn in glob.glob(os.path.join(D, S, f'd{j}', 'b*.txt')):
            for s in open(fn):
                f = s.split()
                if f and f[0] == 'D': say.setdefault(f[1], [0, 0, 0, 0])[j - 1] += int(f[2])
    def kn(a):
        k = say.get(a, [0, 0, 0, 0]); c = 0
        for j, n in ((1, 1e6), (2, 1e7), (3, 1e8)):
            c += k[j - 1]
            if c >= 10 and olay[a]['hed']: return c, n
        return c + k[3], 1e10
    print(f'=== {S}')
    for t in (1, 2, 3):
        Q = olay[f'A000.{t}']; kQ = kn(f'A000.{t}')[0]
        print(f'  O{t}: Kur\'an {mercek(Q["L"])} {Q["n"]} yüklem, isabet {kQ}/1e10, bilgi {Q["bilgi"]:.2f}, S {Q["S"]:.2f}: {" ".join(adlar(Q["m"]))}')
        pl = sorted((a for a in olay if a.endswith(f'.{t}') and a != f'A000.{t}'), key=lambda a: ((kn(a)[0] + 1) / (kn(a)[1] + 1), -olay[a]['bilgi']))
        for a in pl[:6 if t == 1 else 4]:
            o = olay[a]; k, n = kn(a)
            print(f'     {a} {mercek(o["L"])} isabet {k}/{n:.0e}, {o["n"]} yüklem, bilgi {o["bilgi"]:.2f}, S {o["S"]:.2f}: {" ".join(adlar(o["m"]))}')
        sk = sorted((olay[a]['S'] for a in olay if a.endswith(f'.{t}') and a != f'A000.{t}'), reverse=True)
        sifir = [a for a in pl if kn(a)[0] == 0]
        print(f'     model: en yüksek plasebo S {sk[0]:.2f}, {sk[1]:.2f}, {sk[2]:.2f} (Kur\'an {Q["S"]:.2f}); 10¹⁰\'da 0 isabetli {len(sifir)} plasebo olayının bilgisi: '
              + (f'en çok {max(olay[a]["bilgi"] for a in sifir):.2f}, Kur\'an\'ınkinden yüksek {sum(olay[a]["bilgi"] >= Q["bilgi"] for a in sifir)}' if sifir else '—'))
