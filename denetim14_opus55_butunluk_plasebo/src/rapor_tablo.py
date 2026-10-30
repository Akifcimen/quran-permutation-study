# rapor_tablo.py — rapor tablolarını sonuç dosyalarından üretir (sonuc/rapor_tablolar.md). Kullanım: python3 -I src/rapor_tablo.py
import os, json, math
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
AD = {'M': 'M20 (birincil)', 'U': 'Tekdüze', 'L': 'Mallows'}


def e(x, n=2):
    """bilimsel gösterim, Türkçe ondalık virgül"""
    if x is None: return '—'
    if x == 0: return '0'
    if x >= 1 and x < 1000: return f'{x:.{n + 1}g}'.replace('.', ',')
    m, s = f'{x:.{n}e}'.split('e'); s = int(s)
    sup = str.maketrans('-0123456789', '⁻⁰¹²³⁴⁵⁶⁷⁸⁹')
    if float(m) == 1 and n == 0: return f"10{str(s).translate(sup)}"
    return f"{m.replace('.', ',')}×10{str(s).translate(sup)}"


satir = []
J = {}
for S in ('M', 'U', 'L'):
    fn = os.path.join(KOK, 'sonuc', S, 'analiz.json')
    if os.path.exists(fn): J[S] = json.load(open(fn))
satir.append('| Sıfır | K | Kur\'an dili Λ_Q (mercek) | eşik (bit) | k_Q / N | p_Q [%95] | plasebo p medyanı [%95] | en küçük plasebo p [aralık] | R | p_kal | okuma |')
satir.append('|---|---|---|---|---|---|---|---|---|---|---|')
for S, d in J.items():
    if not d.get('tamam'): satir.append(f'| {AD[S]} | {d["K"]} | ölçüm eksik | | | | | | | | |'); continue
    q = d['hedefler']['Q']; k = d['kalibre']; pl = d['plasebo']
    mer = q['ayrinti'].split('mercek ')[1].split(' ')[0] if 'mercek ' in q['ayrinti'] else ''
    satir.append(f"| {AD[S]} | {d['K']} | {{{', '.join(q['dil'])}}} {mer} | {q['esik']:.3f}".replace('.', ',') +
                 f" | {q['k']} / {e(q['N'], 0)} | **{e(q['p'])}** [{e(q['ci'][0])}; {e(q['ci'][1])}] | {e(pl['medyan'])} [{e(pl['medyan_95'][0])}; {e(pl['medyan_95'][1])}]"
                 f" | {e(pl['min'])} [{e(pl['min_aralik'][0])}; {e(pl['min_aralik'][1])}] | {k['R']} | **{e(k['p_kal'], 3)}** | {k['okuma']} |")
satir.append('')
satir.append('| Sıfır | model-tabanlı: GPD P [önyükleme %95] | GPD (u, σ, ξ) | normal P (μ, σ) | betimsel fark Δ (mertebe) |')
satir.append('|---|---|---|---|---|')
for S, d in J.items():
    if not d.get('tamam'): continue
    m = d['kalibre']['model']
    satir.append(f"| {AD[S]} | {e(m['P_gpd'])} [{e(m['P_gpd_95'][0])}; {e(m['P_gpd_95'][1])}] | ({m['esik_u']:.2f}; {m['gpd_sigma']:.2f}; {m['gpd_xi']:.2f})".replace('.', ',') +
                 f" | {e(m['P_normal'])} ({m['normal_mu']:.2f}; {m['normal_sd']:.2f})".replace('.', ',') + f" | {m['delta_mertebe']:.2f} |".replace('.', ','))
satir.append('')
satir.append('| Sıfır | Kur\'an el dilinde: k / N → p [%95] | dil yazımının kazancı G (mertebe): plasebo medyanı [çeyrekler]; Kur\'an | P2 ek satırı: Λ, k / N → p |')
satir.append('|---|---|---|---|')
for S, d in J.items():
    if not d.get('tamam'): continue
    el = d['el_ozet']; qp = d['hedefler'].get('QP2')
    satir.append(f"| {AD[S]} | {el['kQ_el']} / 10⁸ → {e(el['pQ_el'])} [{e(el['ciQ_el'][0])}; {e(el['ciQ_el'][1])}] | {el['G_medyan']:.2f} [{el['G_ceyrek'][0]:.2f}; {el['G_ceyrek'][1]:.2f}]; {el['G_Q']:.2f}".replace('.', ',') +
                 (f" | {{{', '.join(qp['dil'])}}}, {qp['k']} / 10⁸ → {e(qp['p'])} [{e(qp['ci'][0])}; {e(qp['ci'][1])}] |" if qp else ' | — |'))
satir = [r.replace('; -', '; −').replace('(-', '(−') for r in satir]
open(os.path.join(KOK, 'sonuc', 'rapor_tablolar.md'), 'w').write('\n'.join(satir) + '\n')
print('\n'.join(satir))
