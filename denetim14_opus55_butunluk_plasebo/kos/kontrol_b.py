# kontrol_b.py — kontrol geçişlerinin düzeyli yürütücüsü (ONKAYIT §5.3, §8). Kullanım: python3 -I kos/kontrol_b.py <sifir> <küme A|B> <düzey 1|2|3>
# Kümenin dilleri tab/<sifir>/k<küme>/diller.txt'tedir (donmuş kol dosyalarının birleşimi; dilsiz hedefler hariç).
# Düzey 1: bütün diller, bölüt 1–4. Düzey 2: hedef 0'ın (Kur'an) dilleri ∪ {k(1–4) < 10 olan plasebo dilleri}, bölüt 5–40.
# Düzey 3: hedef 0'ın dilleri ∪ {düzey 2'ye girip k(1–40) < 10 kalan plasebo dilleri}, bölüt 41–400. (plasebo12 ile aynı kural.)
# Bellek: dil tabloları 3,5 GB'lık partilere sırayla (hedef 0'ın dilleri önce) doldurulur; her parti aynı bölütleri aynı tohumla yeniden
# üretir (aynı kitaplar). Bütçe: ikincil küme (B) için düzey, kayıtlı hesap + düzey kestirimi ≤ 40 çekirdek-saat ise koşar (ONKAYIT §8.2).
# Sürdürülebilir: var olan bölüt dosyaları atlanır.
import os, sys, subprocess, glob, datetime
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(KOK, 'src'))
import hesap_saati_b as HS
S, KUME, DZ = sys.argv[1], sys.argv[2], int(sys.argv[3])
TD = os.path.join(KOK, 'tab', S); KD = os.path.join(TD, 'k' + KUME)
P12 = os.path.expanduser('~/opus5.5/plasebo12')
REF = [os.path.join(P12, 'tab', S, f'ref_{g}.ref') for g in ('1111', '2222', '0444', '0888')]
ARALIK = {1: (1, 4), 2: (5, 40), 3: (41, 400)}
LIM = 3500 * 1024 * 1024
T0_US, TA_US, TB_US = 76.0, 0.9, 5.4          # dondurma öncesi ölçüm (sonuc/dogrulama/hiz.txt): taban, Kol A/A−/A+ dili, Kol B dili (µs/kontrol)
TAVAN = 40.0


def gunluk(msg):
    t = datetime.datetime.now(datetime.timezone.utc).strftime('%Y-%m-%dT%H:%M:%SZ')
    with open(os.path.join(KOK, 'sonuc', 'kosu_gunlugu.txt'), 'a') as f:
        f.write(f'[{t}] {msg}\n')


def say(dizinler, b1, b2):
    k = {}
    for dz in dizinler:
        for b in range(b1, b2 + 1):
            fn = os.path.join(dz, f'b{b:04d}.txt')
            if not os.path.exists(fn): continue
            for s in open(fn):
                w = s.split()
                if w and w[0] == 'D': k[w[1]] = k.get(w[1], 0) + int(w[2])
    return k


def bellek(satirlar, dz):
    fn = os.path.join(dz, 'bellek_girdi.txt')
    with open(fn, 'w') as f: f.writelines(satirlar)
    r = subprocess.run([os.path.join(KOK, 'src', 'kontrol_b'), S, os.path.join(TD, 'hedef.bin'), fn, '1', '1', dz] + REF,
                       env=dict(os.environ, KONTROL_BELLEK='1'), capture_output=True, text=True)
    if r.returncode != 0: gunluk(f'HATA bellek kestirimi {S} {KUME} düzey {DZ}: {r.stderr.strip()}'); sys.exit(2)
    return {w[1]: int(w[2]) for w in (s.split() for s in r.stdout.splitlines()) if w and w[0] == 'bayt'}


def kos(satirlar, b1, b2, dizin):
    os.makedirs(dizin, exist_ok=True)
    fn = os.path.join(dizin, 'diller.txt')
    if not os.path.exists(fn):
        with open(fn, 'w') as f: f.writelines(satirlar)
    gunluk(f'kontrol {S} küme {KUME} düzey {DZ} {os.path.basename(dizin)}: {len(satirlar)} dil, bölüt {b1}–{b2} başladı')
    r = subprocess.run([os.path.join(KOK, 'src', 'kontrol_b'), S, os.path.join(TD, 'hedef.bin'), fn, str(b1), str(b2), dizin] + REF,
                       stderr=open(os.path.join(dizin, 'hata.txt'), 'a'), env=dict(os.environ, ESZAMANLI='4'))
    gunluk(f'kontrol {S} küme {KUME} düzey {DZ} {os.path.basename(dizin)}: bitti rc={r.returncode}')
    return r.returncode


def main():
    tum = [s for s in open(os.path.join(KD, 'diller.txt')) if s.strip()]
    ozel = [s for s in tum if int(s.split()[1]) == 0]
    plas = [s for s in tum if int(s.split()[1]) != 0]
    b1, b2 = ARALIK[DZ]
    for j in range(1, DZ):
        if os.path.exists(os.path.join(KD, f'butce_durdu_d{j}.txt')):
            gunluk(f'kontrol {S} küme {KUME} düzey {DZ}: düzey {j} bütçe nedeniyle koşulmadığı için koşulmadı'); return
    if DZ == 1: secim = ozel + plas
    else:
        onceki = sorted(glob.glob(os.path.join(KD, 'd1_*'))) if DZ == 2 else sorted(glob.glob(os.path.join(KD, 'd1_*')) + glob.glob(os.path.join(KD, 'd2_*')))
        son = 4 if DZ == 2 else 40
        k = say(onceki, 1, son)
        if DZ == 2: aday = [s for s in plas if k.get(s.split()[0], 0) < 10]
        else:
            g2 = set(s.split()[0] for d in glob.glob(os.path.join(KD, 'd2_*')) for s in open(os.path.join(d, 'diller.txt')))
            aday = [s for s in plas if s.split()[0] in g2 and k.get(s.split()[0], 0) < 10]
        secim = ozel + aday
        gunluk(f'kontrol {S} küme {KUME} düzey {DZ}: {len(aday)} plasebo dili derinleşiyor: {" ".join(s.split()[0] for s in aday)}')
    # bütçe kestirimi (ONKAYIT §8.2)
    N = (b2 - b1 + 1) * 250000
    nA = sum(1 for s in secim if int(s.split()[2]) // 10 in (1, 2, 3)); nB = sum(1 for s in secim if int(s.split()[2]) // 10 == 4)
    kes = N * (T0_US + TA_US * nA + TB_US * nB) * 1e-6 / 3600
    kayit = HS.toplam()
    gunluk(f'bütçe {S} küme {KUME} düzey {DZ}: kayıtlı {kayit:.2f} ç-saat, kestirim {kes:.2f} ç-saat (N {N}, kol A dili {nA}, kol B dili {nB})')
    if KUME == 'B' and kayit + kes > TAVAN:
        gunluk(f'bütçe {S} küme {KUME} düzey {DZ}: {kayit:.2f} + {kes:.2f} > {TAVAN} — düzey koşulmadı (ONKAYIT §8.2)')
        open(os.path.join(KD, f'butce_durdu_d{DZ}.txt'), 'w').write(f'{kayit:.3f} {kes:.3f}\n'); return
    # bellek partileri
    tmpd = os.path.join(KD, f'bellek_d{DZ}'); os.makedirs(tmpd, exist_ok=True)
    by = bellek(secim, tmpd)
    partiler, cur, top = [], [], 0
    for s in secim:
        b = by[s.split()[0]]
        if b > LIM: gunluk(f'HATA {s.split()[0]} tek başına bellek sınırını aşıyor ({b >> 20} MB)'); sys.exit(3)
        if cur and top + b > LIM: partiler.append(cur); cur, top = [], 0
        cur.append(s); top += b
    if cur: partiler.append(cur)
    for i, p in enumerate(partiler):
        rc = kos(p, b1, b2, os.path.join(KD, f'd{DZ}_{i}'))
        if rc != 0: gunluk(f'HATA kontrol {S} küme {KUME} düzey {DZ} parti {i} rc={rc}'); sys.exit(rc)


if __name__ == '__main__':
    main()
