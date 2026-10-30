# analiz_b.py — ONKAYIT §5.3–§11'in uygulaması: derinlik kuralı, p_x ve Clopper–Pearson, dilsiz hedefler (p = 1), kalibre p ve okuma
# kuralı, model-tabanlı fark (plasebo12'nin modeli aynen + dilsizlik kuralları + koşullu karışım), tahmin hükümleri (M20, Kol A),
# betimseller (diller, en iyi beş plasebo, plasebo12'ye göre kitap kitap değişim, Kur'an'ın rakipleri).
# Kullanım: python3 -I src/analiz_b.py <sifir U|M|L> <küme A|B>   → sonuc/<sifir>/analiz_<kol>.json ve .txt
import sys, os, math, json, glob, struct, collections
import numpy as np
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(KOK, 'src'))
import kutuphane as KU
import agac as AG
import istat as IS

S, KUME = sys.argv[1], sys.argv[2]
K = {'M': 200, 'U': 100, 'L': 100}[S]
TD = os.path.join(KOK, 'tab', S); KD = os.path.join(TD, 'k' + KUME)
OUT = os.path.join(KOK, 'sonuc', S); os.makedirs(OUT, exist_ok=True)
P12 = os.path.expanduser('~/opus5.5/plasebo12')
BN = 250000; NMAX = 400 * BN
KOLLAR = (['A', 'Am', 'Ap'] if S == 'M' else ['A']) if KUME == 'A' else ['B']
KOLAD = {'A': 'Kol A (≥ 3 tür, ≥ 3 düzey)', 'Am': 'Kol A− (≥ 2 tür, ≥ 2 düzey)', 'Ap': 'Kol A+ (4 tür, 4 düzey)', 'B': 'Kol B (bedelsiz en geniş dil)'}
TUM = KU.TUM
REC = struct.Struct('<IfhBB114H')


def diller_oku(fn):
    D = {}
    for s in open(fn):
        w = s.split()
        if not w: continue
        n = int(w[4]); D[w[0]] = dict(ad=w[0], hedef=int(w[1]), kod=int(w[2]), L=int(w[3]), g=[int(x) for x in w[5:5 + n]])
    return D


def satir_oku(fn):
    A = {}
    if os.path.exists(fn):
        for s in open(fn):
            w = s.split()
            if w: A[w[0]] = s.strip()
    return A


def bolutler():
    """{ad: {bölüt: (k, kB)}} bütün düzey ve parti dizinlerinden; aynı (ad, bölüt) iki kez varsa hata"""
    B = collections.defaultdict(dict); E = {}
    for dz in sorted(glob.glob(os.path.join(KD, 'd*_*'))):
        for fn in glob.glob(os.path.join(dz, 'b*.txt')):
            b = int(os.path.basename(fn)[1:5])
            for s in open(fn):
                w = s.split()
                if w and w[0] == 'D':
                    if b in B[w[1]]: raise RuntimeError(f'yinelenen bölüt {w[1]} {b}')
                    B[w[1]][b] = (int(w[2]), int(w[3]))
        fe = os.path.join(dz, 'esikler.txt')
        if os.path.exists(fe):
            for s in open(fe):
                w = s.split()
                if w and w[0] == 'D':
                    E.setdefault(w[1], []).append((float(w[3]), int(w[4]), int(w[w.index('mercek') + 1]) if 'mercek' in w else None))
    return B, E


def derinlik(seg, dogrudan):
    def say(son):
        eks = [b for b in range(1, son + 1) if b not in seg]
        return sum(seg[b][0] for b in range(1, son + 1) if b in seg), sum(seg[b][1] for b in range(1, son + 1) if b in seg), eks
    if dogrudan:
        k, kb, ek = say(400); return dict(k=k, kB=kb, N=None if ek else NMAX, duzey=3, eksik=len(ek))
    for i, son in enumerate((4, 40, 400)):
        k, kb, ek = say(son)
        if ek: return dict(k=k, kB=kb, N=None, duzey=i + 1, eksik=len(ek))
        if k >= 10 or son == 400: return dict(k=k, kB=kb, N=son * BN, duzey=i + 1, eksik=0)


def p12_oku():
    fn = os.path.join(P12, 'sonuc', S, 'analiz.json')
    return json.load(open(fn)) if os.path.exists(fn) else None


def model_karisim(yd, cd, yQ, Kd, K_):
    """dili olan plasebolara koşullu: P = (K_d/K)·P_koşullu; GPD (üst %20) ve normal (ONKAYIT §5.6 (c))"""
    out = dict(K_d=Kd)
    if Kd < 10: out['not'] = 'K_d < 10: uyum yapılmadı'; return out
    y = np.asarray(yd, float); o = np.sort(y); m = int(round(0.2 * Kd))
    if m >= 5 and o[Kd - m] > 0:
        r = IS.model_fark(yd, cd, yQ, oran=0.2)
        out.update(P_gpd=None if r['P_gpd'] is None else (Kd / K_) * r['P_gpd'], P_gpd_95=[(Kd / K_) * x for x in r['P_gpd_95']],
                   gpd=(r['esik_u'], r['gpd_sigma'], r['gpd_xi']), P_normal=(Kd / K_) * r['P_normal'], normal=(r['normal_mu'], r['normal_sd']))
    else:
        r = IS.model_fark(yd, cd, yQ, oran=0.2); out.update(P_gpd=None, gpd_not='üst kümede p = 1 var ya da m < 5: GPD tanımsız',
                                                           P_normal=(Kd / K_) * r['P_normal'], normal=(r['normal_mu'], r['normal_sd']))
    return out


def rakipler(ad, esik, g, tavan=15):
    """Kur'an dilinde eşiğe 2 bitten yakın kontroller (aday dosyaları): en yüksekler ve Λ'nın orada sağlanan yüklemleri"""
    A = []
    for dz in sorted(glob.glob(os.path.join(KD, 'd*_*'))):
        fd = os.path.join(dz, 'diller.txt')
        if not os.path.exists(fd): continue
        adlar = [s.split()[0] for s in open(fd) if s.strip()]
        if ad not in adlar: continue
        di = adlar.index(ad)
        for fn in glob.glob(os.path.join(dz, 'b*.q')):
            b = int(os.path.basename(fn)[1:5]); d = open(fn, 'rb').read()
            for o_ in range(0, len(d), REC.size):
                r = REC.unpack_from(d, o_)
                if r[4] == di: A.append(dict(bolut=b, sira=r[0], puan=r[1], L=r[2], sinif=r[3], a=list(r[5:])))
    A.sort(key=lambda x: (-x['puan'], x['bolut'], x['sira']))
    ust = [x for x in A if x['puan'] >= esik - 1e-6]
    adl = [TUM[i] for i in g]; sat = []
    for x in A[:tavan]:
        f_, pi = divmod(x['L'], 29); p = KU.PRIMES[pi]; v, bil = KU.profil(x['a'], f_, p)
        sat.append(dict(puan=x['puan'], bolut=x['bolut'], sira=x['sira'], mercek=f'({KU.AL[f_]},{KU.BE[f_]:+d},{p})', kF=bil['kF'], kH=bil['kH'],
                        sinif=x['sinif'], saglanan=[y for y in adl if v[y]], hepsi=all(v[y] for y in adl)))
    zin = collections.Counter((x['bolut'], x['sira'] % 64) for x in ust)
    return dict(asan=len(ust), aday=len(A), hepsini_tasiyan_asan=sum(1 for x in sat if x['hepsi'] and x['puan'] >= esik - 1e-6),
                farkli_zincir=len(zin), en_cok_ayni_zincir=max(zin.values()) if zin else 0, en_yuksek=sat)


def dil_betim(g):
    t = sorted(set(AG.YAP[TUM[i]][1] for i in g)); d = sorted(set(AG.YAP[TUM[i]][0] for i in g))
    return dict(dil=[TUM[i] for i in g], boy=len(g), turler=t, duzeyler=d, n_tur=len(t), n_duzey=len(d))


def kol_analiz(kol, B, E, p12):
    D = diller_oku(os.path.join(TD, f'diller_{kol}.txt'))
    AY = satir_oku(os.path.join(TD, f'ayrinti_{kol}.txt')); HA = satir_oku(os.path.join(TD, f'hedef_ayrinti_{kol}.txt'))
    H = {}
    for ad, d in sorted(D.items(), key=lambda x: x[1]['hedef']):
        r = dict(ad=ad, hedef=d['hedef'], L=d['L'], **dil_betim(d['g']), ayrinti=AY.get(ad, ''), hedef_ayrinti=HA.get(ad, ''))
        dilsiz = d['L'] < 0 or len(d['g']) == 0
        if dilsiz:                                                        # dilsiz hedef: p = 1, aralık [1, 1] (ONKAYIT §5.4)
            r.update(dilsiz=True, p=1.0, ci=[1.0, 1.0], k=None, N=None, duzey=0)
        else:
            r['dilsiz'] = False
            r.update(derinlik(B.get(ad, {}), d['hedef'] == 0))
            e = E.get(ad)
            if e:
                if len(set(round(x[0], 9) for x in e)) > 1: raise RuntimeError(f'eşik partiler arasında farklı: {ad}')
                r['esik'], r['esik_bayrak'], r['esik_mercek'] = e[0]
            if r['N']: r['p'] = (r['k'] + 1) / (r['N'] + 1); r['ci'] = list(IS.cp(r['k'], r['N']))
        H[ad] = r
    Q = H[f'{kol}000']; plas = [H[a] for a in sorted(H) if H[a]['hedef'] != 0]
    out = dict(sifir=S, kol=kol, kol_adi=KOLAD[kol], K=K, hedefler=H)
    # tutarlılık ve raporlama denetimleri (ONKAYIT §4.3, §5.5): D'nin eşiği ↔ kontrol programının eşiği; aday sınırı; çözülmemiş aramalar
    fark, mer = [], 0
    for r in H.values():
        if r['dilsiz'] or 'esik' not in r: continue
        w = r['ayrinti'].split(); anah = 'esik' if 'esik' in w else ('puan' if 'puan' in w else None)
        if anah: fark.append(abs(float(w[w.index(anah) + 1]) - r['esik']))
        if r.get('esik_mercek') is not None and r['esik_mercek'] != r['L']: mer += 1
    out['denetim'] = dict(esik_farki_en_cok=max(fark) if fark else None, esik_karsilastirma=len(fark), mercek_farkli=mer,
                          aday_asimi=sum(1 for r in H.values() if 'aday_asimi 1' in r['ayrinti']),
                          cozulmemis=[r['ad'] for r in H.values() if 'cozulmus 0' in r['hedef_ayrinti']])
    tamam = Q.get('p') is not None and len(plas) == K and all(x.get('p') is not None for x in plas)
    out['tamam'] = bool(tamam)
    Kd = sum(1 for x in plas if not x['dilsiz']); out['K_d'] = Kd
    if not tamam: return out
    pQ = Q['p']; loQ, hiQ = Q['ci']
    R = [x['ad'] for x in plas if x['p'] <= pQ or x['ci'][0] <= hiQ]
    pkal = (1 + len(R)) / (K + 1)
    oku = 'aşar' if len(R) == 0 else ('sık ulaşır' if len(R) / K >= 0.05 else 'arada')
    y = [-math.log10(x['p']) for x in plas]; cens = [bool(x.get('k') == 0 and x.get('N') == NMAX) for x in plas]; yQ = -math.log10(pQ)
    m = int(round(0.2 * K)); ys = np.sort(np.asarray(y))
    mf = IS.model_fark(y, cens, yQ, oran=0.2)
    if ys[K - m] <= 0: mf['P_gpd'] = None; mf['P_gpd_95'] = None; mf['gpd_not'] = 'üst %20 kümesinde p = 1 olan plasebo var: GPD tanımsız (ONKAYIT §5.6)'
    yd = [-math.log10(x['p']) for x in plas if not x['dilsiz']]; cd = [bool(x.get('k') == 0 and x.get('N') == NMAX) for x in plas if not x['dilsiz']]
    mk = model_karisim(yd, cd, yQ, Kd, K)
    out['kalibre'] = dict(pQ=pQ, ciQ=Q['ci'], kQ=Q['k'], NQ=Q['N'], R=len(R), ulasanlar=R, p_kal=pkal, okuma=oku, model=mf, model_karisim=mk,
                          delta_mertebe=float(yQ - max(y)))
    ps = np.array([x['p'] for x in plas]); o = np.sort(ps)
    med = math.sqrt(o[K // 2 - 1] * o[K // 2]); ranks = (86, 115) if K == 200 else (40, 61)
    pd_ = sorted(x['p'] for x in plas if not x['dilsiz'])
    out['plasebo'] = dict(medyan=med, medyan_95=[float(o[ranks[0] - 1]), float(o[ranks[1] - 1])], min=float(o[0]),
                          min_aralik=[min(x['ci'][0] for x in plas), min(x['ci'][1] for x in plas)],
                          ceyrekler=[float(np.quantile(ps, q)) for q in (0.1, 0.25, 0.5, 0.75, 0.9)],
                          dilli_medyan=float(np.median(pd_)) if pd_ else None, dilsiz=K - Kd,
                          esik_alti={str(t): int(sum(1 for x in plas if x['p'] < t)) for t in (1e-2, 1e-3, 1e-4, 1e-5, 1e-6, 1e-7)},
                          duzeyler={str(i): sum(1 for x in plas if x['duzey'] == i) for i in (0, 1, 2, 3)},
                          sansurlu=int(sum(cens)))
    # en iyi beş plasebo
    en = sorted(plas, key=lambda x: (x['p'], x['hedef']))[:5]
    out['en_iyi_bes'] = [dict(ad=x['ad'], p=x['p'], ci=x['ci'], k=x.get('k'), N=x.get('N'), dil=x['dil'], turler=x['turler'], duzeyler=x['duzeyler'],
                              hedef_ayrinti=x['hedef_ayrinti']) for x in en]
    # dil betimleri
    pl = [x for x in plas if not x['dilsiz']]
    out['diller'] = dict(boy=dict(collections.Counter(x['boy'] for x in pl)), n_tur=dict(collections.Counter(x['n_tur'] for x in pl)),
                         n_duzey=dict(collections.Counter(x['n_duzey'] for x in pl)),
                         tur_kullanimi=dict(collections.Counter(t for x in pl for t in x['turler'])),
                         duzey_kullanimi=dict(collections.Counter(t for x in pl for t in x['duzeyler'])),
                         sik_yuklemler=collections.Counter(p for x in pl for p in x['dil']).most_common(25),
                         Q=dict(dil=Q['dil'], turler=Q['turler'], duzeyler=Q['duzeyler'], esik=Q.get('esik'), ayrinti=Q['ayrinti']))
    # plasebo12 ile
    if p12:
        q12 = p12['hedefler']['Q']; out['p12_Q'] = dict(dil=q12['dil'], p=q12['p'], ci=q12['ci'], k=q12['k'])
        out['Q_p12_farki'] = dict(ortak=[x for x in Q['dil'] if x in q12['dil']], eklenen=[x for x in Q['dil'] if x not in q12['dil']],
                                  cikan=[x for x in q12['dil'] if x not in Q['dil']])
        deg = []
        for x in plas:
            a12 = f"x{x['hedef']:03d}"; h12 = p12['hedefler'].get(a12)
            if h12 is None or h12.get('p') is None: continue
            deg.append(dict(hedef=x['hedef'], p12=h12['p'], p14=x['p'], dilsiz=x['dilsiz'], dil12=h12['dil'], dil14=x['dil'], ayni_dil=sorted(h12['dil']) == sorted(x['dil'])))
        dl = [d for d in deg if not d['dilsiz']]; lr = [math.log10(d['p14']) - math.log10(d['p12']) for d in dl]
        def spearman(a, b):
            if len(a) < 3: return None
            ra = np.argsort(np.argsort(a)); rb = np.argsort(np.argsort(b)); return float(np.corrcoef(ra, rb)[0, 1])
        out['kitap_kitap'] = dict(n=len(deg), dilli=len(dl), ayni_dil=sum(1 for d in dl if d['ayni_dil']),          # yalnız dilli plasebolar (EK_..._betimsel_duzeltme)
                                  log10_oran_medyan=float(np.median(lr)) if lr else None,
                                  log10_oran_ceyrek=[float(np.quantile(lr, 0.25)), float(np.quantile(lr, 0.75))] if lr else None,
                                  azalan=sum(1 for v in lr if v < 0), artan=sum(1 for v in lr if v > 0), esit=sum(1 for v in lr if v == 0),
                                  spearman_dilli=spearman([math.log10(d['p12']) for d in dl], [math.log10(d['p14']) for d in dl]),
                                  dilsizlerin_p12_medyani=float(np.median([d['p12'] for d in deg if d['dilsiz']])) if any(d['dilsiz'] for d in deg) else None,
                                  Q_p12=p12['hedefler']['Q']['p'], Q_p14=pQ,
                                  p12de_Qa_ulasan=p12['kalibre']['R'], ayrinti=deg)
    out['rakipler'] = rakipler(Q['ad'], Q.get('esik', float('inf')), [TUM.index(p) for p in Q['dil']]) if not Q['dilsiz'] else None
    # tahmin (yalnız M20, Kol A; ONKAYIT §10)
    if S == 'M' and kol == 'A':
        def hukum(nokta, aralik, a, b):
            if a <= nokta <= b: return 'tuttu'
            if aralik[1] < a or aralik[0] > b: return 'tutmadı'
            return 'karar verilemedi'
        th = {}
        if Q['dilsiz']: th['p_Q'] = 'tutmadı'
        elif Q['k'] >= 1: th['p_Q'] = hukum(pQ, Q['ci'], 1e-9, 1e-6)
        else:
            U = IS.ust_tek(Q['N']); th['p_Q'] = 'tutmadı (daha nadir)' if U < 1e-9 else 'karar verilemedi'
        th['p_Q_merkez_uzaklik'] = abs(math.log10(pQ) + 7)
        th['dil_yarisi'] = 'tuttu' if Kd >= 100 else 'tutmadı'
        th['medyan'] = hukum(med, out['plasebo']['medyan_95'], 1e-3, 1e-1)
        th['en_kucuk'] = hukum(float(o[0]), out['plasebo']['min_aralik'], 1e-6, 1e-4)
        th['hepsini_asar'] = 'tuttu' if len(R) == 0 else 'tutmadı'
        parca = [th['p_Q'], th['dil_yarisi'], th['medyan'], th['en_kucuk'], th['hepsini_asar']]
        if all(p == 'tuttu' for p in parca): th['butun'] = 'tuttu'
        elif not any(p == 'tuttu' for p in parca) and any(p.startswith('tutmadı') for p in parca): th['butun'] = 'tutmadı'
        else: th['butun'] = 'kısmen tuttu'
        out['tahmin'] = th
    return out


def yaz(out):
    kol = out['kol']
    json.dump(out, open(os.path.join(OUT, f'analiz_{kol}.json'), 'w'), ensure_ascii=False, indent=1, default=float)
    with open(os.path.join(OUT, f'analiz_{kol}.txt'), 'w') as f:
        H = out['hedefler']; Q = H[f'{kol}000']
        f.write(f"sıfır {S}, {out['kol_adi']}, K = {out['K']}, dili olan plasebo K_d = {out['K_d']}, tamam = {out['tamam']}\n")
        f.write(f"denetim: {json.dumps(out['denetim'], ensure_ascii=False, default=float)}\n")
        f.write(f"Kur'an: dil {Q['dil']} (|Λ| = {Q['boy']}; türler {Q['turler']}; düzeyler {Q['duzeyler']}), eşik {Q.get('esik')}, k = {Q.get('k')} / N = {Q.get('N')} → p_Q = {Q.get('p')} {Q.get('ci')}\n")
        if out['tamam']:
            kq = out['kalibre']
            f.write(f"kalibre: R = {kq['R']} {kq['ulasanlar'][:20]} → p_kal = {kq['p_kal']:.5f}; okuma: {kq['okuma']}; Δ = {kq['delta_mertebe']:.2f} mertebe\n")
            f.write(f"model (plasebo12 modeli): {json.dumps(kq['model'], ensure_ascii=False, default=float)}\n")
            f.write(f"model (koşullu karışım): {json.dumps(kq['model_karisim'], ensure_ascii=False, default=float)}\n")
            f.write(f"plasebo: {json.dumps(out['plasebo'], ensure_ascii=False, default=float)}\n")
            f.write(f"diller: {json.dumps(out['diller'], ensure_ascii=False, default=float)[:3000]}\n")
            for x in out['en_iyi_bes']: f.write(f"  en iyi: {x['ad']} p = {x['p']:.3e} {x['ci']} k = {x['k']} / {x['N']} | {x['dil']} | {x['hedef_ayrinti'][:400]}\n")
            if 'kitap_kitap' in out: kk = dict(out['kitap_kitap']); kk.pop('ayrinti'); f.write(f"plasebo12'ye göre: {json.dumps(kk, ensure_ascii=False, default=float)}\n")
            if 'Q_p12_farki' in out: f.write(f"Kur'an'ın plasebo12 diliyle farkı: {out['Q_p12_farki']}\n")
            if out.get('rakipler'): rk = dict(out['rakipler']); f.write(f"rakipler: aşan {rk['asan']}, aday {rk['aday']}, zincir {rk['farkli_zincir']}/{rk['en_cok_ayni_zincir']}\n")
            if out.get('rakipler'):
                for x in out['rakipler']['en_yuksek']: f.write(f"   {x['puan']:.4f} bit b{x['bolut']} s{x['sira']} {x['mercek']} kF={x['kF']} kH={x['kH']} hepsi={x['hepsi']} sağlanan {x['saglanan']}\n")
            if 'tahmin' in out: f.write(f"tahmin: {json.dumps(out['tahmin'], ensure_ascii=False, default=float)}\n")
    print(open(os.path.join(OUT, f'analiz_{kol}.txt')).read()[:6000])


def main():
    B, E = bolutler(); p12 = p12_oku()
    for kol in KOLLAR: yaz(kol_analiz(kol, B, E, p12))


if __name__ == '__main__':
    main()
