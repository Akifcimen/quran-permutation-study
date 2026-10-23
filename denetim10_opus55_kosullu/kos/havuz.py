#!/usr/bin/env python3
"""Bölüt sürücüsü: eksik bölütleri paralel koşturur; tamamlanmış bölüt (atomik yazılmış .bin) atlanır → sürdürülebilir.
Bütçe tavanı: tamamlanan bölütlerin toplam süresi (çekirdek-saniye, .say'daki sure_s) tavanı aşınca yeni bölüt başlatılmaz.
Kullanım: python3 -I kos/havuz.py <kodeks> <sifir> <kosul> <seed_bas> <nbolut> <bolut_N> <dizin> <paralel> <tavan_cekirdek_saat>
"""
import sys, os, subprocess, time, glob
from concurrent.futures import ThreadPoolExecutor
kod, sif, kos, s0, nb, bn, d, par, tavan = sys.argv[1:10]
s0, nb, bn, par, tavan = int(s0), int(nb), int(bn), int(par), float(tavan) * 3600
os.makedirs(d, exist_ok=True)
kok = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
uret = os.path.join(kok, 'src', 'kosullu_uret')
def harcanan():
    t = 0.0
    for f in glob.glob(os.path.join(d, '*.say')):
        try: t += float(open(f).read().split('sure_s ')[1].split()[0])
        except Exception: pass
    return t
def kos_bolut(i):
    seed = s0 + i
    out = os.path.join(d, f'{sif}_{kos}_{kod}_{seed}.bin')
    if os.path.exists(out): return f'{seed} mevcut'
    if os.path.exists(out + '.yazılıyor') and time.time() - os.path.getmtime(out + '.yazılıyor') < 1800: return f'{seed} başka sürücüde sürüyor'
    if harcanan() >= tavan: return f'{seed} TAVAN'
    t0 = time.time()
    r = subprocess.run(['nice', '-n', '10', uret, kod, sif, kos, str(seed), str(bn), out], capture_output=True, text=True)
    return f'{seed} bitti rc={r.returncode} {time.time()-t0:.0f}s {r.stderr.strip()[:200]}'
with open(os.path.join(d, 'surucu.log'), 'a') as log:
    log.write(f'[{time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime())}] BAŞLA {sys.argv[1:]}\n'); log.flush()
    with ThreadPoolExecutor(max_workers=par) as ex:
        sira = list(range(nb))[::-1] if os.environ.get('TERS') else range(nb)   # TERS=1: ikinci sürücü sondan başlar
        for msg in ex.map(kos_bolut, sira):
            log.write(f'[{time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime())}] {msg} | harcanan {harcanan()/3600:.2f} çekirdek-saat\n'); log.flush()
    log.write(f'[{time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime())}] BİTTİ\n')
