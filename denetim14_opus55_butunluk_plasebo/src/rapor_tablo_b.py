# rapor_tablo_b.py — rapor tabloları (dondurmadan sonra eklendi; yalnız raporlama): sonuc/<sıfır>/analiz_<kol>.json → sonuc/rapor_tablolar.md
# Kullanım: python3 -I src/rapor_tablo_b.py
import os, json, math
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SAD = {'M': 'M20', 'U': 'Tekdüze', 'L': 'Mallows'}
KAD = {'A': 'Kol A', 'Am': 'Kol A−', 'Ap': 'Kol A+', 'B': 'Kol B'}
UST = str.maketrans('0123456789-', '⁰¹²³⁴⁵⁶⁷⁸⁹⁻')


def e(x, d=2):
    if x is None: return '—'
    if x == 0: return '0'
    if x >= 0.1: return f'{x:.{d + 1}g}'.replace('.', ',')
    m, k = f'{x:.{d}e}'.split('e'); return f"{m.replace('.', ',')}×10{str(int(k)).translate(UST)}"


def v(x, n=2): return '—' if x is None else f'{x:.{n}f}'.replace('.', ',').replace('-', '−')


def n10(N):
    if N is None: return '—'
    k = round(math.log10(N)); return f'10{str(k).translate(UST)}' if 10 ** k == N else f'{N:,}'.replace(',', '.')


def ci(c): return f'[{e(c[0])}; {e(c[1])}]' if c else '—'


def oku(S, k):
    fn = os.path.join(KOK, 'sonuc', S, f'analiz_{k}.json')
    return json.load(open(fn)) if os.path.exists(fn) else None


def satir_ana(S, k, d):
    H = d['hedefler']; Q = H[f'{k}000']
    if not d['tamam']: return f"| {SAD[S]} | {KAD[k]} | {d['K']} | {d['K_d']} | {' '.join(Q['dil'])} | eksik | | | | | | |"
    kq = d['kalibre']; pl = d['plasebo']; en = d['en_iyi_bes'][0]
    return (f"| {SAD[S]} | {KAD[k]} | {d['K']} | {d['K_d']} | {{{', '.join(Q['dil'])}}} {Q['hedef_ayrinti'].split('mercek ')[1].split(' ')[0] if 'mercek ' in Q['hedef_ayrinti'] else ''} | "
            f"{v(Q.get('esik'), 3)} | {Q['k']} / {n10(Q['N'])} | **{e(kq['pQ'])}** {ci(kq['ciQ'])} | {e(pl['medyan'])} {ci(pl['medyan_95'])} | "
            f"{e(pl['min'])} ({en['ad']}; {en['k']} / {n10(en['N'])}) {ci(en['ci'])} | {kq['R']} | **{v(kq['p_kal'], 4)}** | {kq['okuma']} |")


def main():
    out = []
    out.append('## Ana tablo (her kol, her sıfır)\n')
    out.append("| Sıfır | Kol | K | dilli K_d | Kur'an'ın dili (mercek) | eşik (bit) | k_Q / N | p_Q [%95] | plasebo p medyanı [%95] | en küçük plasebo p (ad; k / N) [%95] | R | p_kal | okuma |")
    out.append('|---|---|---|---|---|---|---|---|---|---|---|---|---|')
    D = {}
    for S in ('M', 'U', 'L'):
        for k in ('A', 'Am', 'Ap', 'B'):
            d = oku(S, k)
            if d: D[(S, k)] = d; out.append(satir_ana(S, k, d))
    out.append('\n## Model-tabanlı fark (kararlarda kullanılmaz)\n')
    out.append('| Sıfır | Kol | GPD P [önyükleme %95] (u; σ; ξ) | normal P (μ; σ) | karışım: GPD P | karışım: normal P | Δ (mertebe) |')
    out.append('|---|---|---|---|---|---|---|')
    for (S, k), d in D.items():
        if not d['tamam']: continue
        m = d['kalibre']['model']; mk = d['kalibre']['model_karisim']
        g = 'tanımsız' if m.get('P_gpd') is None else f"{e(m['P_gpd'])} [{e(m['P_gpd_95'][0])}; {e(m['P_gpd_95'][1])}] ({v(m['esik_u'])}; {v(m['gpd_sigma'])}; {v(m['gpd_xi'])})"
        gk = mk.get('not') or ('tanımsız' if mk.get('P_gpd') is None else e(mk['P_gpd']))
        nk = mk.get('not') or e(mk.get('P_normal'))
        out.append(f"| {SAD[S]} | {KAD[k]} | {g} | {e(m['P_normal'])} ({v(m['normal_mu'])}; {v(m['normal_sd'])}) | {gk} | {nk} | {v(d['kalibre']['delta_mertebe'])} |")
    out.append('\n## Plasebo dağılımı\n')
    out.append('| Sıfır | Kol | dilsiz | p < 10⁻² / 10⁻³ / 10⁻⁴ / 10⁻⁵ / 10⁻⁶ / 10⁻⁷ | dilli plasebo p medyanı | düzey 1 / 2 / 3 | sansürlü (k = 0 @ 10⁸) |')
    out.append('|---|---|---|---|---|---|---|')
    for (S, k), d in D.items():
        if not d['tamam']: continue
        p = d['plasebo']; ea = p['esik_alti']
        out.append(f"| {SAD[S]} | {KAD[k]} | {p['dilsiz']} | {' / '.join(str(ea[t]) for t in ('0.01', '0.001', '0.0001', '1e-05', '1e-06', '1e-07'))} | {e(p['dilli_medyan'])} | "
                   f"{p['duzeyler'].get('1', 0)} / {p['duzeyler'].get('2', 0)} / {p['duzeyler'].get('3', 0)} | {p['sansurlu']} |")
    out.append('\n## En iyi beş plasebo\n')
    for (S, k), d in D.items():
        if not d['tamam']: continue
        dl = [x for x in d['en_iyi_bes'] if x['k'] is not None]
        out.append(f"**{SAD[S]}, {KAD[k]}**" + ('' if len(dl) == len(d['en_iyi_bes']) else f" (yalnız {len(dl)} plasebo dilli)") + '\n')
        out.append('| ad | p_x [%95] | k / N | dil | türler / düzeyler | yapı (mercek, k_F, k_H, hücreler) |'); out.append('|---|---|---|---|---|---|')
        for x in dl:
            ha = x['hedef_ayrinti']; yap = ''
            if 'mercek ' in ha:
                w = ha.split(); g = lambda a: w[w.index(a) + 1] if a in w else '?'
                yap = f"{g('mercek')}, k_F {g('kF')}, k_H {g('kH')}, \\|F∩H\\| {g('FnH')}, hücre boyları {g('hucre_boy')}, q-toplamları {g('hucre_q')}, kapı G {g('kapiG')}"
            out.append(f"| {x['ad']} | {e(x['p'])} {ci(x['ci'])} | {x['k']} / {n10(x['N'])} | {' '.join(x['dil'])} | {''.join(x['turler'])} / {', '.join(x['duzeyler'])} | {yap} |")
        out.append('')
    out.append("\n## Plasebo12'ye göre kitap kitap\n")
    out.append("| Sıfır | Kol | dilli | dili plasebo12'dekiyle aynı | log₁₀(p14/p12) medyanı [çeyrekler] | azalan / artan / eşit | Spearman (dilliler) | dilsizlerin p12 medyanı | Kur'an p12 → p14 |")
    out.append('|---|---|---|---|---|---|---|---|---|')
    for (S, k), d in D.items():
        kk = d.get('kitap_kitap')
        if not kk: continue
        q = kk['log10_oran_ceyrek'] or [None, None]
        med = '—' if kk['log10_oran_medyan'] is None else f"{v(kk['log10_oran_medyan'])} [{v(q[0])}; {v(q[1])}]"
        out.append(f"| {SAD[S]} | {KAD[k]} | {kk['dilli']} | {kk['ayni_dil']} | {med} | {kk['azalan']} / {kk['artan']} / {kk['esit']} | {v(kk['spearman_dilli'])} | "
                   f"{e(kk['dilsizlerin_p12_medyani'])} | {e(kk['Q_p12'])} → {e(kk['Q_p14'])} |")
    out.append("\n## Kur'an'ın rakipleri (eşiğe 2 bitten yakın kontroller)\n")
    for (S, k), d in D.items():
        rk = d.get('rakipler')
        if not rk or not d['tamam']: continue
        out.append(f"**{SAD[S]}, {KAD[k]}:** eşiği aşan/eşleyen {rk['asan']}; aday {rk['aday']}; Λ'nın hepsini taşıyan aşan {rk['hepsini_tasiyan_asan']}; "
                   f"farklı (bölüt, zincir) {rk['farkli_zincir']}, en çok aynı zincir {rk['en_cok_ayni_zincir']}\n")
        for x in rk['en_yuksek'][:8]:
            out.append(f"- {v(x['puan'], 3)} bit, bölüt {x['bolut']} sıra {x['sira']}, {x['mercek']}, k_F = {x['kF']}, k_H = {x['kH']}; Λ'dan sağlanan: {' '.join(x['saglanan'])}")
    out.append('\n## Tahmin (M20, Kol A)\n')
    d = D.get(('M', 'A'))
    if d and 'tahmin' in d: out.append('```\n' + json.dumps(d['tahmin'], ensure_ascii=False, indent=1) + '\n```')
    out.append('\n## Denetimler\n')
    for (S, k), d in D.items(): out.append(f"- {SAD[S]}, {KAD[k]}: {json.dumps(d['denetim'], ensure_ascii=False)}")
    fn = os.path.join(KOK, 'sonuc', 'rapor_tablolar.md'); open(fn, 'w').write('\n'.join(out) + '\n'); print(open(fn).read())


if __name__ == '__main__':
    main()
