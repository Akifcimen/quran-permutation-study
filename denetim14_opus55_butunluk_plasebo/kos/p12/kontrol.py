# kontrol.py — kontrol geçişlerinin düzeyli yürütücüsü (ONKAYIT §5.3). Kullanım: python3 -I kos/kontrol.py <sifir> <duzey 1|2|3>
# Düzey 1: bütün diller, bölüt 1–4. Düzey 2: {Q, QP2, EL} ∪ {k(1–4) < 10 olan plasebolar}, bölüt 5–40.
# Düzey 3: {Q, QP2, EL} ∪ {düzey 2'ye girip k(1–40) < 10 kalan plasebolar}, bölüt 41–400.
# Dil tabloları belleğe sığmazsa (kontrol programı çıkış kodu 3) diller ikiye bölünür (Q, QP2, EL ilk grupta) ve aynı bölütler her grup
# için aynı tohumla yeniden üretilir. Sürdürülebilir: var olan bölüt dosyaları atlanır.
import os, sys, subprocess, glob, datetime
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
S, DZ = sys.argv[1], int(sys.argv[2])
TD = os.path.join(KOK, 'tab', S)
REF = [os.path.join(TD, f'ref_{g}.ref') for g in ('1111', '2222', '0444', '0888')]
ARALIK = {1: (1, 4), 2: (5, 40), 3: (41, 400)}


def gunluk(msg):
    t = datetime.datetime.now(datetime.timezone.utc).strftime('%Y-%m-%dT%H:%M:%SZ')
    with open(os.path.join(KOK, 'sonuc', 'kosu_gunlugu.txt'), 'a') as f:
        f.write(f'[{t}] {msg}\n')


def diller():
    return [s for s in open(os.path.join(TD, 'diller.txt')) if s.strip()]


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


def kos(satirlar, b1, b2, dizin):
    os.makedirs(dizin, exist_ok=True)
    fn = os.path.join(dizin, 'diller.txt')
    if not os.path.exists(fn):
        with open(fn, 'w') as f: f.writelines(satirlar)
    gunluk(f'kontrol {S} düzey {DZ} {os.path.basename(dizin)}: {len(satirlar)} dil, bölüt {b1}–{b2} başladı')
    r = subprocess.run([os.path.join(KOK, 'src', 'kontrol'), S, os.path.join(TD, 'hedef.bin'), fn, str(b1), str(b2), dizin] + REF,
                       stderr=open(os.path.join(dizin, 'hata.txt'), 'a'))
    gunluk(f'kontrol {S} düzey {DZ} {os.path.basename(dizin)}: bitti rc={r.returncode}')
    return r.returncode


def main():
    tum = diller()
    ozel = [s for s in tum if s.split()[0] in ('Q', 'QP2', 'EL')]
    plas = [s for s in tum if s.split()[0] not in ('Q', 'QP2', 'EL')]
    b1, b2 = ARALIK[DZ]
    if DZ == 1: secim = tum
    else:
        onceki = sorted(glob.glob(os.path.join(TD, 'k1*'))) if DZ == 2 else sorted(glob.glob(os.path.join(TD, 'k1*')) + glob.glob(os.path.join(TD, 'k2*')))
        son = 4 if DZ == 2 else 40
        k = say(onceki, 1, son)
        if DZ == 2: aday = [s for s in plas if k.get(s.split()[0], 0) < 10]
        else:
            g2 = set(s.split()[0] for d in glob.glob(os.path.join(TD, 'k2*')) for s in open(os.path.join(d, 'diller.txt')))
            aday = [s for s in plas if s.split()[0] in g2 and k.get(s.split()[0], 0) < 10]
        secim = ozel + aday
        gunluk(f'kontrol {S} düzey {DZ}: {len(aday)} plasebo derinleşiyor: {" ".join(s.split()[0] for s in aday)}')
    gruplar = [(f'k{DZ}', secim)]; sayac = 0; i = 0
    while i < len(gruplar):
        ad, g = gruplar[i]; dz = os.path.join(TD, ad)
        rc = kos(g, b1, b2, dz)
        if rc == 3 and len(g) > 1:     # bellek: yeni adlarla ikiye böl (Q, QP2, EL ilk yarıda)
            ilk = [x for x in g if x.split()[0] in ('Q', 'QP2', 'EL')]; diger = [x for x in g if x not in ilk]; h = len(diger) // 2
            for x in glob.glob(os.path.join(dz, '*')): os.remove(x)
            os.rmdir(dz)
            gruplar.append((f'k{DZ}_{sayac}', ilk + diger[:h])); gruplar.append((f'k{DZ}_{sayac + 1}', diger[h:])); sayac += 2
            gunluk(f'kontrol {S} düzey {DZ}: bellek sınırı — {ad} ikiye bölündü')
        elif rc != 0: gunluk(f'HATA kontrol {S} düzey {DZ} rc={rc}'); sys.exit(rc)
        i += 1


if __name__ == '__main__':
    main()
