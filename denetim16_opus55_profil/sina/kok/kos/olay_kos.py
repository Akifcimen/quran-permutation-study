# olay_kos.py — profil çalışmasının düzeyli yürütücüsü (ONKAYIT §4). Kullanım: python3 -I kos/olay_kos.py <sifir> <düzey 1..4>
# Düzey 1: bütün olaylar, bölüt 1–4 (10⁶ kontrol). Düzey 2: Kur'an'ın olayları ∪ {k(1–4) < 10}, bölüt 5–40.
# Düzey 3: Kur'an'ın olayları ∪ {düzey 2'de olup k(1–40) < 10}, bölüt 41–400. Düzey 4: Kur'an'ın olayları ∪ {düzey 3'te olup k(1–400) < 10},
# bölüt 401–40.000 (toplam 10¹⁰; Kur'an'la eşit derinlik). İki kart aynı anda, aynı dizine: RTX 4090 b mod 5 ∈ {0,1,2}, RTX 3090 {3,4}.
import os, sys, subprocess, glob, datetime
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
S, DZ = sys.argv[1], int(sys.argv[2])
TD = os.path.join(KOK, 'tab', S)
ARALIK = {1: (1, 4), 2: (5, 40), 3: (41, 400), 4: (401, 40000)}
if os.environ.get('OLAY_ARALIK'): ARALIK = {j + 1: tuple(int(x) for x in a.split('-')) for j, a in enumerate(os.environ['OLAY_ARALIK'].split(','))}   # yalnız sınama
KARTLAR = (('1', '0', '2'), ('0', '3', '4'))
def gunluk(msg):
    t = datetime.datetime.now(datetime.timezone.utc).strftime('%Y-%m-%dT%H:%M:%SZ')
    with open(os.path.join(KOK, 'sonuc', 'kosu_gunlugu.txt'), 'a') as f: f.write(f'[{t}] {msg}\n')
def say(dz, b1, b2):
    k = {}
    for b in range(b1, b2 + 1):
        fn = os.path.join(dz, f'b{b:04d}.txt')
        if not os.path.exists(fn): gunluk(f'HATA {S}: eksik bölüt {fn}'); sys.exit(2)
        for s in open(fn):
            w = s.split()
            if w and w[0] == 'D': k[w[1]] = k.get(w[1], 0) + int(w[2])
    return k
def main():
    tum = [s for s in open(os.path.join(TD, 'olaylar.txt')) if s.strip()]
    b1, b2 = ARALIK[DZ]
    if DZ == 1: secim = tum
    else:
        k = {}
        for j in range(1, DZ):
            for a, x in say(os.path.join(TD, f'd{j}'), *ARALIK[j]).items(): k[a] = k.get(a, 0) + x
        onceki = set(s.split()[0] for s in open(os.path.join(TD, f'd{DZ - 1}', 'olaylar.txt')))
        secim = [s for s in tum if int(s.split()[1]) == 0 or (s.split()[0] in onceki and k.get(s.split()[0], 0) < 10)]
    dz = os.path.join(TD, f'd{DZ}'); os.makedirs(dz, exist_ok=True)
    fn = os.path.join(dz, 'olaylar.txt')
    if not os.path.exists(fn):
        with open(fn, 'w') as f: f.writelines(secim)
    gunluk(f'olay {S} düzey {DZ}: {len(secim)} olay, bölüt {b1}–{b2} başladı (kart)')
    pr = []
    for dev, alt, ust in KARTLAR:
        env = dict(os.environ, CUDA_DEVICE_ORDER='PCI_BUS_ID', CUDA_VISIBLE_DEVICES=dev, KART_MOD='5', KART_ALT=alt, KART_UST=ust, KART_PARTI=os.environ.get('KART_PARTI', '2000'))
        pr.append(subprocess.Popen(['nice', '-n', '10', os.path.join(KOK, 'bin', os.environ.get('KART_OLAY_IKILI', 'kart_olay')), S, os.path.join(TD, 'hedef.bin'), fn, str(b1), str(b2), dz],
                                   stderr=open(os.path.join(dz, f'hata_kart{dev}.txt'), 'a'), env=env))
    rc = [p.wait() for p in pr]
    gunluk(f'olay {S} düzey {DZ}: bitti rc={rc}')
    if max(abs(x) for x in rc): sys.exit(2)
if __name__ == '__main__':
    main()
