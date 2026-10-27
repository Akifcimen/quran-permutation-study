# orkestra.py — üretim koşularının tek kuyruklu yürütücüsü (ONKAYIT §5.3). Toplamda EN ÇOK 4 eşzamanlı süreç.
# İşler ve bağımlılıklar: R (referans bölütü) → B (deltaları birleştir + tablo kur) → Q (Kur'an eşiği, kuran_puan) → T (sınama bölütü).
# Sürdürülebilir: tamamlanmış iş çıktısından tanınır (.say, .tab, esik). Kullanım: python3 -I kos/orkestra.py <plan>
# Plan satırı: <sıfır> <kodeks> <R_bölüt> <R_N> <T_bölüt> <T_N>
# Tohum = 26102600000 + sıfır·10^8 + tür·10^7 + kodeks·10^6 + bölüt  (sıfır U1 M2 L3 J4; tür R1 T2; kodeks 129→0, 127→1)
import os, sys, subprocess, time, glob, datetime
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SIFIR = {'U': 1, 'M': 2, 'L': 3, 'J': 4, 'K': 9}; TUR = {'R': 1, 'T': 2}   # K = yalnız Kur'an (mekanik sınama)
ESZ = min(4, int(os.environ.get('ESZAMANLI', '4')))
SRC = os.path.join(KOK, 'src')

def tohum(s, tur, kod, b):
    return 26102600000 + SIFIR[s] * 10**8 + TUR[tur] * 10**7 + (0 if kod == 129 else 1) * 10**6 + b

def gunluk(msg):
    t = datetime.datetime.now(datetime.timezone.utc).strftime('%Y-%m-%dT%H:%M:%SZ')
    with open(os.environ.get('GUNLUK', os.path.join(KOK, 'sonuc', 'kosu_gunlugu.txt')), 'a') as f:
        f.write(f'[{t}] {msg}\n')

class Is:
    def __init__(self, ad, cmd, bitti, bag, hazirla=None, son=None):
        self.ad, self.cmd, self.bitti, self.bag, self.hazirla, self.son = ad, cmd, bitti, bag, hazirla, son

def plan_oku(fn):
    isler = []
    for satir in open(fn):
        satir = satir.split('#')[0].split()
        if not satir: continue
        s, kod, nR, RN, nT, TN = satir[0], int(satir[1]), int(satir[2]), int(satir[3]), int(satir[4]), int(satir[5])
        dR = os.path.join(KOK, 'tab', f'R_{s}{kod}'); dT = os.path.join(KOK, 'tab', f'T_{s}{kod}')
        os.makedirs(dR, exist_ok=True); os.makedirs(dT, exist_ok=True)
        tab = os.path.join(KOK, 'tab', f'{s}{kod}.tab'); ana = os.path.join(KOK, 'tab', f'{s}{kod}.ref')
        esik = os.path.join(KOK, 'sonuc', f'esik_{s}{kod}.txt')
        Rad = []
        for b in range(1, nR + 1):
            on = os.path.join(dR, f'b{b:03d}')
            def temizle(on=on):
                for x in glob.glob(on + '.*'): os.remove(x)
            isler.append(Is(f'R{s}{kod}.{b}', [os.path.join(SRC, 'referans'), str(kod), s, str(tohum(s, 'R', kod, b)), str(RN), '11', on],
                            lambda on=on, tab=tab: os.path.exists(on + '.say'), [], temizle))
            Rad.append(f'R{s}{kod}.{b}')
        def B_cmd(dR=dR, ana=ana, tab=tab, kod=kod):
            d = sorted(glob.glob(os.path.join(dR, 'b*.ref')))
            if os.path.exists(ana): os.remove(ana)
            for i in range(0, len(d), 8):
                subprocess.run([os.path.join(SRC, 'birlestir'), str(kod), ana] + d[i:i + 8], check=True, stderr=subprocess.DEVNULL)
            subprocess.run([os.path.join(SRC, 'tablo_kur'), str(kod), ana, tab], check=True, stderr=subprocess.DEVNULL)
            for x in d: os.remove(x)
            return 0
        isler.append(Is(f'B{s}{kod}', B_cmd, lambda tab=tab: os.path.exists(tab), Rad))
        kout = os.path.join(KOK, 'sonuc', f'kuran_{s}{kod}.txt')
        isler.append(Is(f'Q{s}{kod}', ['/bin/sh', '-c', f'"{os.path.join(SRC, "kuran_puan")}" {kod} "{tab}" "{esik}" > "{kout}"'],
                        lambda esik=esik: os.path.exists(esik), [f'B{s}{kod}']))
        for b in range(1, nT + 1):
            on = os.path.join(dT, f'b{b:03d}')
            def temizle(on=on):
                for x in glob.glob(on + '.*'): os.remove(x)
            isler.append(Is(f'T{s}{kod}.{b}', [os.path.join(SRC, 'test'), str(kod), s, str(tohum(s, 'T', kod, b)), str(TN), tab, esik, on],
                            lambda on=on: os.path.exists(on + '.say'), [f'Q{s}{kod}'], temizle))
    return isler

def main(plan):
    isler = plan_oku(plan)
    durum = {i.ad: ('bitti' if i.bitti() else 'bekliyor') for i in isler}
    gunluk(f'orkestra başladı: {sum(1 for v in durum.values() if v=="bekliyor")} iş bekliyor, eşzamanlı {ESZ}')
    calisan = {}
    import threading
    while True:
        # bitenleri topla
        for ad, (p, i, t0) in list(calisan.items()):
            if isinstance(p, threading.Thread):
                if p.is_alive(): continue
                rc = 0
            else:
                rc = p.poll()
                if rc is None: continue
            del calisan[ad]
            ok = (rc == 0) and i.bitti()
            durum[ad] = 'bitti' if ok else 'hata'
            gunluk(f'{ad} {"bitti" if ok else "HATA rc=" + str(rc)} ({(time.time()-t0)/60:.1f} dk)')
        bekleyen = [i for i in isler if durum[i.ad] == 'bekliyor' and all(durum[b] == 'bitti' for b in i.bag)]
        if not bekleyen and not calisan:
            break
        while bekleyen and len(calisan) < ESZ:
            i = bekleyen.pop(0)
            if i.hazirla: i.hazirla()
            durum[i.ad] = 'calisiyor'
            if callable(i.cmd):
                th = threading.Thread(target=i.cmd); th.start(); calisan[i.ad] = (th, i, time.time())
            else:
                err = open(os.path.join(KOK, 'tab', 'son_hata.txt'), 'a')
                calisan[i.ad] = (subprocess.Popen(i.cmd, stdout=subprocess.DEVNULL, stderr=err), i, time.time())
        time.sleep(3)
    hata = [a for a, v in durum.items() if v == 'hata']
    gunluk(f'orkestra bitti; hata: {hata if hata else "yok"}')

if __name__ == '__main__':
    main(sys.argv[1])
