# secim_ek.py — ONKAYIT_EK §3–§4: D1, D2 doğrulama olaylarının seçimi (tohum: ekin SHA-256'sı) ve Kur'an'ın olayları; olay başına bütçe
# ve motor (≤ 1,5e8 adım: kart, değilse işlemci). Kullanım: python3 -I secim_ek.py <ONKAYIT_EK.md> <tab> <çıktı_dizini>
import sys, os, glob, hashlib, random
T, O = sys.argv[2], sys.argv[3]; os.makedirs(O, exist_ok=True)
sha = hashlib.sha256(open(sys.argv[1], 'rb').read()).hexdigest(); r = random.Random(int(sha, 16))
def adim(nl): w = max(5e7, 4e5 * nl * nl); return int(w) + int(w) // 2
bilgi = {}
for S in 'MLU':
    k = {}; n = {}
    for j in (1, 2, 3, 4):
        for fn in glob.glob(os.path.join(T, S, f'd{j}', 'b*.txt')):
            for s in open(fn):
                if s.startswith('D '): w = s.split(); k.setdefault(w[1], [0, 0, 0, 0])[j - 1] += int(w[2])
    for s in open(os.path.join(T, S, 'olaylar.txt')):
        w = s.split(); n[w[0]] = 2 + sum(bin(int(x, 16)).count('1') for x in w[3:8])
    d3 = set(s.split()[0] for s in open(os.path.join(T, S, 'd3', 'olaylar.txt')))
    h1 = sorted(a for a in d3 if not a.startswith('A000') and sum(k.get(a, [0] * 4)[:3]) == 0)
    def coz(a):
        c = 0
        for j in (1, 2, 3):
            c += k.get(a, [0] * 4)[j - 1]
            if c >= 10: return True
        return False
    h2 = sorted(a for a in n if not a.startswith('A000') and coz(a))
    bilgi[S] = (h1, h2, n)
sec = {}
for S in 'MLU': sec[(S, 'D1')] = r.sample(bilgi[S][0], 40)
for S in 'MLU': sec[(S, 'D2')] = r.sample(bilgi[S][1], 20)
for S in 'MLU': sec[(S, 'Q')] = ['A000.1', 'A000.2']
for (S, ad), L in sec.items():
    n = bilgi[S][2]
    open(os.path.join(O, f'sec_{ad}_{S}.txt'), 'w').write('\n'.join(L) + '\n')
    kart = [a for a in L if adim(n[a]) <= 1.5e8]; isl = [a for a in L if adim(n[a]) > 1.5e8]
    if ad != 'Q':
        open(os.path.join(O, f'sec_{ad}_{S}_kart.txt'), 'w').write(''.join(a + '\n' for a in kart))
        open(os.path.join(O, f'sec_{ad}_{S}_isl.txt'), 'w').write(''.join(a + '\n' for a in isl))
    print(f'{S} {ad}: {len(L)} olay (havuz {len(bilgi[S][0]) if ad == "D1" else len(bilgi[S][1]) if ad == "D2" else 2}); kart {len(kart)}, işlemci {len(isl)}; '
          f'toplam adım (4 tekrar) {4 * sum(adim(n[a]) for a in L):.2e}')
print('ek SHA-256', sha[:16])
