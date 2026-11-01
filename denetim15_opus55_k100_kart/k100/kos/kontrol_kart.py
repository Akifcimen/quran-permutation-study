# kontrol_kart.py — K×100 kontrol geçişlerinin düzeyli yürütücüsü (ONKAYIT §2). kapı/k10'un kontrol_b.py'sinden türetildi; farklar:
#   sayaç kart sayacıdır (iki kart aynı anda, aynı dizine: RTX 4090 bölüt b mod 5 ∈ {0,1,2}, RTX 3090 {3,4}); bellek ölçümü işlemci
#   ikilisiyle (KONTROL_BELLEK); düzey 4 = hedef 0 ∪ R₁, düzey 5 = hedef 0 ∪ R_derin (yeniden_say_k100.py --liste ile).
# Düzey 1–3 seçim kuralı kontrol_b.py ile aynıdır. Kullanım: python3 -I kos/kontrol_kart.py <sifir> <düzey 1..5>
import os, sys, subprocess, glob, datetime
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
S, DZ = sys.argv[1], int(sys.argv[2])
TD = os.path.join(KOK, 'tab', S); KD = os.path.join(TD, 'kA')
REF = [os.path.join(os.environ['KAPI_REF'], S, f'ref_{g}.ref') for g in ('1111', '2222', '0444', '0888')]
ARALIK = {1: (1, 4), 2: (5, 40), 3: (41, 400), 4: (401, 4000), 5: (4001, 40000)}
QDERIN = {'M': ('4.7e-9', '3.5e-9', '6.3e-9'), 'L': ('2.2e-9', '1.4e-9', '3.3e-9'), 'U': ('1.2e-9', '0.6e-9', '2.1e-9')}
KSAY = {'M': '20000', 'L': '10000', 'U': '10000'}
LIM = 3500 * 1024 * 1024
KARTLAR = (('1', '0', '2'), ('0', '3', '4'))          # (CUDA_VISIBLE_DEVICES, KART_ALT, KART_UST), CUDA_DEVICE_ORDER=PCI_BUS_ID


def gunluk(msg):
    t = datetime.datetime.now(datetime.timezone.utc).strftime('%Y-%m-%dT%H:%M:%SZ')
    with open(os.path.join(KOK, 'sonuc', 'kosu_gunlugu.txt'), 'a') as f: f.write(f'[{t}] {msg}\n')


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
    if r.returncode != 0: gunluk(f'HATA bellek kestirimi {S} düzey {DZ}: {r.stderr.strip()}'); sys.exit(2)
    return {w[1]: int(w[2]) for w in (s.split() for s in r.stdout.splitlines()) if w and w[0] == 'bayt'}


def kos(satirlar, b1, b2, dizin):
    os.makedirs(dizin, exist_ok=True)
    fn = os.path.join(dizin, 'diller.txt')
    if not os.path.exists(fn):
        with open(fn, 'w') as f: f.writelines(satirlar)
    gunluk(f'kontrol {S} düzey {DZ} {os.path.basename(dizin)}: {len(satirlar)} dil, bölüt {b1}–{b2} başladı (kart)')
    pr = []
    for dev, alt, ust in KARTLAR:
        env = dict(os.environ, CUDA_DEVICE_ORDER='PCI_BUS_ID', CUDA_VISIBLE_DEVICES=dev, KART_MOD='5', KART_ALT=alt, KART_UST=ust,
                   KART_PARTI=os.environ.get('KART_PARTI', '2000'))
        pr.append(subprocess.Popen(['nice', '-n', '10', os.path.join(KOK, 'bin', 'kart_b'), S, os.path.join(TD, 'hedef.bin'), fn, str(b1), str(b2), dizin] + REF,
                                   stderr=open(os.path.join(dizin, f'hata_kart{dev}.txt'), 'a'), env=env))
    rc = [p.wait() for p in pr]
    gunluk(f'kontrol {S} düzey {DZ} {os.path.basename(dizin)}: bitti rc={rc}')
    return max(abs(x) for x in rc)


def main():
    tum = [s for s in open(os.path.join(KD, 'diller.txt')) if s.strip()]
    ozel = [s for s in tum if int(s.split()[1]) == 0]
    plas = [s for s in tum if int(s.split()[1]) != 0]
    b1, b2 = ARALIK[DZ]
    if DZ == 1: secim = ozel + plas
    elif DZ in (2, 3):
        onceki = sorted(glob.glob(os.path.join(KD, 'd1_*'))) if DZ == 2 else sorted(glob.glob(os.path.join(KD, 'd1_*')) + glob.glob(os.path.join(KD, 'd2_*')))
        k = say(onceki, 1, 4 if DZ == 2 else 40)
        if DZ == 2: aday = [s for s in plas if k.get(s.split()[0], 0) < 10]
        else:
            g2 = set(s.split()[0] for d in glob.glob(os.path.join(KD, 'd2_*')) for s in open(os.path.join(d, 'diller.txt')))
            aday = [s for s in plas if s.split()[0] in g2 and k.get(s.split()[0], 0) < 10]
        secim = ozel + aday
        gunluk(f'kontrol {S} düzey {DZ}: {len(aday)} plasebo dili derinleşiyor')
    else:
        lf = os.path.join(KD, f'secim_d{DZ}.txt')
        r = subprocess.run([sys.executable, '-I', os.path.join(KOK, 'dogrulama', 'yeniden_say_k100.py'), KOK, S, KSAY[S], *QDERIN[S],
                            '--liste', 'R1' if DZ == 4 else 'Rderin', lf], capture_output=True, text=True)
        if r.returncode != 0: gunluk(f'HATA düzey {DZ} seçimi {S}: {r.stderr.strip()}'); sys.exit(2)
        adlar = set(s.strip() for s in open(lf) if s.strip())
        aday = [s for s in plas if s.split()[0] in adlar]
        if len(aday) != len(adlar): gunluk(f'HATA düzey {DZ} seçimi {S}: liste {len(adlar)}, bulunan {len(aday)}'); sys.exit(2)
        secim = ozel + aday
        gunluk(f'kontrol {S} düzey {DZ}: {len(aday)} plasebo dili derinleşiyor ({"R₁" if DZ == 4 else "R_derin"}): {" ".join(s.split()[0] for s in aday)}')
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
        if rc != 0: gunluk(f'HATA kontrol {S} düzey {DZ} parti {i} rc={rc}'); sys.exit(rc)


if __name__ == '__main__':
    main()
