# dil_ozet.py — D'nin kurduğu dillerin betimsel özeti (rapor için): dil boyu, grup/tür/düzey kullanımı, mercek dağılımı, en sık yüklemler,
# el dili yüklemlerinin payı, çözülmemiş bayrakları, sınıf durumu. Kullanım: python3 -I src/dil_ozet.py <sifir> → sonuc/<sifir>/dil_ozet.txt
import sys, os, collections, re
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(KOK, 'src'))
import kutuphane as KU
import agac as AG
S = sys.argv[1]; TD = os.path.join(KOK, 'tab', S); OUT = os.path.join(KOK, 'sonuc', S); os.makedirs(OUT, exist_ok=True)
D = {}
for s in open(os.path.join(TD, 'diller.txt')):
    w = s.split(); D[w[0]] = dict(hedef=int(w[1]), onsel=int(w[2]), L=int(w[3]), g=[KU.TUM[int(x)] for x in w[5:5 + int(w[4])]])
A = {}
for s in open(os.path.join(TD, 'ayrinti.txt')):
    w = s.split(); m = re.search(r'amac (\S+) bilgi (\S+) dil_bedeli (\S+) mercek_bedeli (\S+)', s)
    A[w[0]] = dict(amac=float(m.group(1)), bilgi=float(m.group(2)), dil_bedeli=float(m.group(3)), mercek=re.search(r'mercek (\(\S+\))', s).group(1))
H = {}
for s in open(os.path.join(TD, 'hedef_ayrinti.txt')):
    w = s.split(); m = re.search(r'esik (\S+) mercek (\(\S+\)) Ldil (\S+) sinif (\d) cozulmus (\d) kF (\d+) kH (\d+) mc (\d+)', s)
    H[w[0]] = dict(esik=float(m.group(1)), mercek=m.group(2), sinif=int(m.group(4)), coz=int(m.group(5)), kF=int(m.group(6)), satir=s.strip())
plas = [a for a in D if a not in ('Q', 'QP2', 'EL')]
with open(os.path.join(OUT, 'dil_ozet.txt'), 'w') as f:
    def yaz(x=''): f.write(x + '\n')
    boy = collections.Counter(len(D[a]['g']) for a in plas)
    yaz(f'sıfır {S}: {len(plas)} plasebo dili')
    yaz('dil boyu dağılımı: ' + ', '.join(f'{k}: {v}' for k, v in sorted(boy.items())))
    yaz(f"ortalama |Λ| {sum(len(D[a]['g']) for a in plas) / len(plas):.2f}")
    grp = collections.Counter(); tur = collections.Counter(); duz = collections.Counter(); yk = collections.Counter(); el = 0; elv = 0
    for a in plas:
        g = D[a]['g']; yk.update(g)
        for p in g:
            grp[next(k for k in 'BZFWH' if p in KU.GRUPLAR[k])] += 1
            tur[AG.YAP[p][1]] += 1; duz[AG.YAP[p][0]] += 1
        e = sum(1 for p in g if p in KU.HAND); el += e; elv += 1 if e else 0
    yaz('grup kullanımı (yüklem sayısı): ' + ', '.join(f'{k} {v}' for k, v in sorted(grp.items())))
    yaz('tür kullanımı: ' + ', '.join(f'{k} {v}' for k, v in sorted(tur.items())))
    yaz('düzey kullanımı: ' + ', '.join(f'{k} {v}' for k, v in sorted(duz.items())))
    yaz(f'el dili yüklemi içeren plasebo dili: {elv} / {len(plas)}; toplam el dili yüklemi {el} / {sum(len(D[a]["g"]) for a in plas)}')
    yaz('en sık 25 yüklem: ' + ', '.join(f'{p} {v}' for p, v in yk.most_common(25)))
    mer = collections.Counter(A[a]['mercek'].rsplit(',', 1)[0] + ')' for a in plas)
    yaz('en iyi mercek biçimleri: ' + ', '.join(f'{k} {v}' for k, v in mer.most_common()))
    asal = collections.Counter(int(A[a]['mercek'].rstrip(')').rsplit(',', 1)[1]) for a in plas)
    yaz('en iyi mercek asalları: ' + ', '.join(f'{k} {v}' for k, v in sorted(asal.items())))
    yaz(f"çözülmemiş arama içeren plasebo (en iyi mercekte c < 10): {sum(1 for a in plas if not H[a]['coz'])} / {len(plas)}")
    yaz(f"kendi dilinde sınıfta plasebo: {sum(1 for a in plas if H[a]['sinif'])} / {len(plas)}")
    import statistics as st
    am = [A[a]['amac'] for a in plas]; bi = [A[a]['bilgi'] for a in plas]; db = [A[a]['dil_bedeli'] for a in plas]; es = [H[a]['esik'] for a in plas]
    yaz(f'amaç (bilgi − dil bedeli − mercek bedeli): medyan {st.median(am):.2f}, min {min(am):.2f}, maks {max(am):.2f}')
    yaz(f'bilgi: medyan {st.median(bi):.2f}; dil bedeli: medyan {st.median(db):.2f}; eşik S_x(Λ_x): medyan {st.median(es):.2f}, maks {max(es):.2f}')
    for q in ('Q', 'QP2'):
        if q in D:
            yaz(f"{q}: dil {' '.join(D[q]['g'])} (|Λ| = {len(D[q]['g'])}); {A[q]}; {H[q]['satir']}")
            yaz(f"   el dili ile ortak: {[p for p in D[q]['g'] if p in KU.HAND]}; el dilinde olup D(Q)'da olmayan: {[p for p in KU.HAND if p not in D[q]['g']]}")
    yaz(); yaz('plasebo dilleri (ad: amaç | mercek | dil):')
    for a in plas: yaz(f"  {a}: {A[a]['amac']:.2f} | {A[a]['mercek']} | {' '.join(D[a]['g'])}")
print(open(os.path.join(OUT, 'dil_ozet.txt')).read()[:4000])
