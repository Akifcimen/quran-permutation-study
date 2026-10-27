# rapor_kur.py — BUTUNLUK_RAPORU.md'yi rapor_taslak/ bölümlerinden kurar; [YER_TUTUCU] alanlarını sonuç dosyalarından üretilen
# tablolarla doldurur (elle aktarım hatası olmasın diye). Kullanım: python3 -I src/rapor_kur.py
import os, sys, io, contextlib, subprocess
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); os.chdir(KOK)
def kos(*arg, env=None):
    e = dict(os.environ, **(env or {}))
    return subprocess.run([sys.executable, '-I'] + list(arg), capture_output=True, text=True, check=True, env=e).stdout.rstrip('\n')
BOLUM = ['giris.md', 'kisa.md', 'yontem.md', 'kuran.md', 'siralar.md', 'belirsizlik.md', 'rakipler.md', 'ikincil127.md', 'kosullu.md',
         'duyarlilik.md', 'menu.md', 'tahmin.md', 'sinirlar.md', 'yeniden.md', 'ingilizce.md']
YER = {
    '[ANA_TABLO]': lambda: kos('src/rapor_tablo.py', 'ana', 'U129', 'M129', 'L129', 'J129'),
    '[TABLO_127]': lambda: kos('src/rapor_tablo.py', 'ana', 'U127', 'M127', 'L127'),
    '[DUYARLILIK_TABLOSU]': lambda: kos('src/rapor_tablo.py', 'duyarlilik', 'U129', 'M129', 'L129', 'J129'),
    '[KARAR_TABLOSU]': lambda: kos('src/karar.py', 'U129', 'M129', 'L129'),
    '[MENU_TABLOSU]': lambda: kos('src/menu_ozet.py', 'U129', 'M129', 'L129', env={'MENU_TABLO': '1'}),
}
sys.path.insert(0, os.path.join(KOK, 'src'))
from rapor_tablo import g, cp, oku_ozet, oku_bilgi, duzelt
def sayilar(sk):   # [<sk>_BILGI], [<sk>_BILGI_ARALIK], [<sk>_SAYIM] yer tutucuları için
    N, b = oku_bilgi(sk); SQ, ka, kn, ku = b[0]; dz = duzelt(sk)
    if dz: ka, kn, ku = dz
    k, n = oku_ozet(sk)['G']
    return {f'[{sk}_BILGI]': g((kn + 1) / (N + 1)), f'[{sk}_BILGI_ARALIK]': f'[{g((ka + 1) / (N + 1))}; {g((ku + 1) / (N + 1))}]',
            f'[{sk}_BILGI_K]': f'{ka} / {kn} / {ku}', f'[{sk}_SAYIM]': g((k + 1) / (n + 1)), f'[{sk}_SAYIM_K]': f'{k} / {g(n)}',
            f'[{sk}_SAYIM_CI]': '[' + '; '.join(g(x) for x in cp(k, n)) + ']'}
for sk in ('U129', 'M129', 'L129', 'J129', 'U127', 'M127', 'L127'):
    if os.path.exists(f'sonuc/sira/{sk}_bilgi.txt'):
        for k, v in sayilar(sk).items(): YER[k] = (lambda v=v: v)
YER['[U_BILGI]'] = YER.get('[U129_BILGI]', lambda: '?')
import re, glob
def belirsiz_tablo(sk):   # sonuc/belirsiz/<sk>_coz.txt (src/belirsiz_coz.py çıktısı) → markdown tablo
    fn = f'sonuc/belirsiz/{sk}_coz.txt'
    if not os.path.exists(fn): return f'({sk}: adanmış yeniden puanlama yok)'
    kim = {}
    for kf in glob.glob('sonuc/belirsiz/*.bin.kimlik'):
        for x in open(kf).read().split():
            seg, sira = x.split(':'); kim[(os.path.basename(kf)[:-7], sira)] = seg
    rows = ['| kitap (bölüt:sıra) | önce: nokta [alt; üst] | sonra: nokta [alt; üst] | karar | en iyi mercek; kullanılan kestirimler |', '|---|---|---|---|---|']
    for l in open(fn):
        m = re.match(r'^(\S+) (\d+): önce nokta (\S+) \[(\S+), ([^\]]+)\] → sonra nokta (\S+) \[(\S+), ([^\]]+)\] → (\S+)  \((.*)\)$', l.rstrip('\n'))
        if not m: continue
        f2 = lambda x: x.replace('.', ',')
        rows.append(f"| {kim.get((m.group(1), m.group(2)), '?')}:{m.group(2)} | {f2(m.group(3))} [{f2(m.group(4))}; {f2(m.group(5))}] | "
                    f"{f2(m.group(6))} [{f2(m.group(7))}; {f2(m.group(8))}] | {m.group(9)} | {m.group(10)} |")
    return '\n'.join(rows)
for sk in ('U129', 'M129', 'L129'): YER[f'[BELIRSIZ_{sk}]'] = (lambda sk=sk: belirsiz_tablo(sk))
def belirsiz_ozet():
    sat = ['**Düzeltilmiş sayılar** (`src/sira_duzelt.py`; bilgi V0 P0, N = 10⁸):']
    for sk, ad in (('U129', 'tekdüze'), ('M129', 'M20'), ('L129', 'Mallows')):
        N, b = oku_bilgi(sk); _, ka0, kn0, ku0 = b[0]; dz = duzelt(sk)
        if not dz: sat.append(f'- {ad}: düzeltme yok ({ka0} / {kn0} / {ku0})'); continue
        ka, kn, ku = dz
        sat.append(f'- {ad}: k_alt / k_nokta / k_üst {ka0} / {kn0} / {ku0} → **{ka} / {kn} / {ku}**; p_nokta {g((kn0 + 1) / (N + 1))} → '
                   f'**{g((kn + 1) / (N + 1))}**, belirsizlik aralığı [{g((ka + 1) / (N + 1))}; {g((ku + 1) / (N + 1))}]')
    return '\n'.join(sat)
YER['[BELIRSIZ_OZET]'] = belirsiz_ozet
metin = []
for b in BOLUM:
    fn = os.path.join('rapor_taslak', b)
    if not os.path.exists(fn): print('eksik bölüm:', b, file=sys.stderr); continue
    metin.append(open(fn).read().rstrip('\n'))
s = '\n\n'.join(metin) + '\n'
for k, f in YER.items():
    if k in s: s = s.replace(k, f())
kalan = [w for w in ('[' + x for x in s.split('[')[1:]) if w.split(']')[0].isupper() and '_' in w.split(']')[0]]
if kalan: print('doldurulmamış yer tutucular:', sorted(set(w.split(']')[0] + ']' for w in kalan)), file=sys.stderr)
import re as _re
if '## English summary' in s:   # İngilizce özette ondalık nokta ve aralık virgülü
    tr, en = s.split('## English summary', 1)
    en = _re.sub(r'(\d),(\d)', r'\1.\2', en); en = _re.sub(r'\[([^\]\[;]+); ([^\]\[;]+)\]', r'[\1, \2]', en)
    s = tr + '## English summary' + en
open('BUTUNLUK_RAPORU.md', 'w').write(s); print('BUTUNLUK_RAPORU.md yazıldı:', len(s.split('\n')), 'satır')
