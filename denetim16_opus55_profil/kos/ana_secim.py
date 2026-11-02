# ana_secim.py — ONKAYIT_EK §4: Kur'an'ın kestiriminden sonra kestirilecek plasebo olayları. Her kuralda, derinlik kuralıyla 10¹⁰'da k ≤ 9
# alan Kur'an dışı olaylar; doğrudan sayımın %95 alt sınırı Kur'an'ın kestirim aralığının üst ucunun üstündeyse olay kestirilmez.
# Motor: toplam adım ≤ 1,5e8 → kart, değilse işlemci (ONKAYIT_EK §7).
# Kullanım: python3 -I ana_secim.py <tab> <S> <Kur'an_çıktısı[,yeniden_koşu_çıktısı…]> <çıktı_dizini>
import sys, os
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'dogrulama'))
import duz_oku as d
T, S, QF, O = sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]
TD = os.path.join(T, S); k = d.sayimlar(TD); o = d.olaylar(TD); r = d.duz_oku(QF.split(','))
nl = {}
for s in open(os.path.join(TD, 'olaylar.txt')):
    w = s.split(); nl[w[0]] = 2 + sum(bin(int(x, 16)).count('1') for x in w[3:8])
def adim(n): w = max(5e7, 4e5 * n * n); return int(w) + int(w) // 2
sec = []
for t in (1, 2, 3):
    bQ = d.birlestir(r.get(f'A000.{t}', {})) or (d.birlestir(r.get('A000.2', {})) if t == 3 else None)
    if bQ is None: sys.exit(f'Kur\'an O{t} kestirilemedi')
    mQ, loQ, hiQ = bQ; n_k9 = n_dis = 0
    for a in o:
        if not a.endswith(f'.{t}') or a.startswith('A000'): continue
        kk, N = d.derin(k.get(a, [0, 0, 0, 0]), 1)
        if kk >= 10: continue
        n_k9 += 1
        if d.cp(kk, N)[0] > hiQ: n_dis += 1; continue
        sec.append(a)
    print(f'{S} O{t}: Kur\'an {mQ:.2e} [{loQ:.2e}, {hiQ:.2e}]; k ≤ 9: {n_k9}; doğrudan sayımla dışarıda: {n_dis}; kestirilecek: {n_k9 - n_dis}')
os.makedirs(O, exist_ok=True)
kart = [a for a in sec if adim(nl[a]) <= 1.5e8]; isl = [a for a in sec if adim(nl[a]) > 1.5e8]
open(os.path.join(O, f'sec_A_{S}_kart.txt'), 'w').write(''.join(a + '\n' for a in kart))
open(os.path.join(O, f'sec_A_{S}_isl.txt'), 'w').write(''.join(a + '\n' for a in isl))
print(f'{S}: toplam {len(sec)} olay; kart {len(kart)}, işlemci {len(isl)}; adım (4 tekrar) kart {4 * sum(adim(nl[a]) for a in kart):.2e}, işlemci {4 * sum(adim(nl[a]) for a in isl):.2e}')
