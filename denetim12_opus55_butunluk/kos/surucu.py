# surucu.py — bölüt koşturucu (ONKAYIT §5.3): en çok 4 eşzamanlı süreç; kaldığı yerden sürer (.say varsa bölüt tamam).
# Kullanım:
#   python3 -I kos/surucu.py R <sifir> <kodeks> <bolut_sayisi> <bolut_N>            referans bölütleri
#   python3 -I kos/surucu.py T <sifir> <kodeks> <bolut_sayisi> <bolut_N>            sınama bölütleri (tablo + eşik gerekir)
#   python3 -I kos/surucu.py B <sifir> <kodeks>                                     referans deltalarını birleştir, tablo kur
# Tohum = 26102600000 + sıfır·10^8 + tür·10^7 + kodeks·10^6 + bölüt  (sıfır U1 M2 L3 J4; tür R1 T2; kodeks 129→0, 127→1)
import os, sys, subprocess, time, glob, datetime
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SIFIR = {'U': 1, 'M': 2, 'L': 3, 'J': 4}
TUR = {'R': 1, 'T': 2}
ESZ = int(os.environ.get('ESZAMANLI', '4'))
assert 1 <= ESZ <= 4

def tohum(s, tur, kod, b):
    return 26102600000 + SIFIR[s] * 10**8 + TUR[tur] * 10**7 + (0 if kod == 129 else 1) * 10**6 + b

def gunluk(msg):
    t = datetime.datetime.now(datetime.timezone.utc).strftime('%Y-%m-%dT%H:%M:%SZ')
    with open(os.path.join(KOK, 'sonuc', 'kosu_gunlugu.txt'), 'a') as f:
        f.write(f'[{t}] {msg}\n')

def kos(tur, s, kod, nb, bN):
    d = os.path.join(KOK, 'tab', f'{tur}_{s}{kod}'); os.makedirs(d, exist_ok=True)
    isler = []
    for b in range(1, nb + 1):
        on = os.path.join(d, f'b{b:03d}')
        if os.path.exists(on + '.say'):
            continue
        for x in glob.glob(on + '.*'):
            os.remove(x)                                   # yarım kalan bölüt baştan
        if tur == 'R':
            cmd = [os.path.join(KOK, 'src', 'referans'), str(kod), s, str(tohum(s, 'R', kod, b)), str(bN), '11', on]
        else:
            cmd = [os.path.join(KOK, 'src', 'test'), str(kod), s, str(tohum(s, 'T', kod, b)), str(bN),
                   os.path.join(KOK, 'tab', f'{s}{kod}.tab'), os.path.join(KOK, 'sonuc', f'esik_{s}{kod}.txt'), on]
        isler.append((b, cmd))
    gunluk(f'{tur} {s}{kod}: {len(isler)} bölüt kuyrukta (toplam {nb}, bölüt N = {bN})')
    calisan = []
    while isler or calisan:
        while isler and len(calisan) < ESZ:
            b, cmd = isler.pop(0)
            calisan.append((b, subprocess.Popen(cmd, stdout=subprocess.DEVNULL, stderr=open(os.path.join(d, f'b{b:03d}.err'), 'w'))))
        time.sleep(2)
        kalan = []
        for b, p in calisan:
            rc = p.poll()
            if rc is None:
                kalan.append((b, p))
            else:
                gunluk(f'{tur} {s}{kod} bölüt {b} bitti rc={rc}')
        calisan = kalan
    sn = 0.0
    for f in glob.glob(os.path.join(d, '*.say')):
        for tok in open(f).read().split('\n')[0].split():
            pass
        w = open(f).read().split()
        if 'sure_s' in w:
            sn += float(w[w.index('sure_s') + 1])
    gunluk(f'{tur} {s}{kod}: BİTTİ; toplam süre {sn/3600:.2f} çekirdek-saat')

def birlestir(s, kod):
    d = os.path.join(KOK, 'tab', f'R_{s}{kod}')
    ana = os.path.join(KOK, 'tab', f'{s}{kod}.ref')
    deltalar = sorted(glob.glob(os.path.join(d, 'b*.ref')))
    say = sorted(glob.glob(os.path.join(d, 'b*.say')))
    assert len(deltalar) == len(say) and deltalar, 'eksik bölüt'
    if os.path.exists(ana):
        os.remove(ana)
    for i in range(0, len(deltalar), 8):
        subprocess.run([os.path.join(KOK, 'src', 'birlestir'), str(kod), ana] + deltalar[i:i + 8], check=True)
    subprocess.run([os.path.join(KOK, 'src', 'tablo_kur'), str(kod), ana, os.path.join(KOK, 'tab', f'{s}{kod}.tab')], check=True)
    for x in deltalar:
        os.remove(x)
    gunluk(f'B {s}{kod}: {len(deltalar)} delta birleştirildi (deltalar silindi), tablo kuruldu')

if __name__ == '__main__':
    tur, s, kod = sys.argv[1], sys.argv[2], int(sys.argv[3])
    if tur in ('R', 'T'):
        kos(tur, s, kod, int(sys.argv[4]), int(sys.argv[5]))
    elif tur == 'B':
        birlestir(s, kod)
