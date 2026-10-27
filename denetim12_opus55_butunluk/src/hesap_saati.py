# hesap_saati.py — sonuc/kosu_gunlugu.txt'ten kayıtlı hesap süresinin dökümü (çekirdek-saat; her koşu tek iş parçacıklı).
# Süresi yazılı satırlar "(x dk)" doğrudan toplanır; süresi yazılı olmayan sıralı diziler (menü bölütleri 1–4, adanmış U/M dizileri)
# ardışık bitiş zamanlarının farkıyla, dizinin başlangıcı verilmişse ondan ölçülür (duvar saati; üst sınır).
# Kullanım: python3 -I src/hesap_saati.py [günlük]
import sys, re, datetime as dt
fn = sys.argv[1] if len(sys.argv) > 1 else 'sonuc/kosu_gunlugu.txt'
zaman = lambda s: dt.datetime.strptime(s, '%Y-%m-%dT%H:%M:%SZ')
satir = [(zaman(m.group(1)), m.group(2)) for m in (re.match(r'\[(\S+)\] (.*)', l) for l in open(fn, encoding='utf-8')) if m]
grup = {}
def ekle(ad, dk): grup[ad] = grup.get(ad, 0.0) + dk
for t, s in satir:
    m = re.search(r'^(\S+) bitti \(([\d.]+) dk\)', s) or re.search(r'^kuyruk (\S+) bitti \(([\d.]+) dk\)', s)
    if not m: continue
    ad = m.group(1)
    if ad.startswith(('R', 'T', 'B', 'Q')) and re.match(r'^[RTBQ][UMLJ]\d{3}', ad): ekle(f'üretim {ad[0]} ({ad[1]}{ad[2:5]})', float(m.group(2)))
    elif ad.startswith('menu_'): ekle('menü (kuyruk)', float(m.group(2)))
    else: ekle('adanmış (kuyruk)', float(m.group(2)))
# süresi yazılı olmayan diziler: ardışık bitişler
def dizi(desen, ad, bas=None):
    ts = [t for t, s in satir if re.search(desen, s)]
    if not ts: return
    t0 = bas if bas else ts[0]; top = (ts[-1] - t0).total_seconds() / 60
    if not bas and len(ts) > 1: top = top / (len(ts) - 1) * len(ts)   # ilk işin süresi ortalamayla
    ekle(ad, top)
dizi(r'^menü U129 bölüt \d bitti', 'menü (bölüt 1–4, sıralı)')
dizi(r'^adanmış M129_', 'adanmış M20 dizisi (duvar saati)', bas=[t for t, s in satir if s.startswith('U3 donduruldu')][0])
dizi(r'^adanmış U129_(F_3m1_17|W_3m1_17|F_1m1_11|W_2p1_19|W_2m1_19|F_1m1_19_K19b)', 'adanmış tekdüze U3 dizisi (duvar saati)')
toplam = 0.0
for ad in sorted(grup):
    print(f'{ad:45s} {grup[ad] / 60:7.2f} çekirdek-saat'); toplam += grup[ad]
print(f'{"KAYITLI TOPLAM":45s} {toplam / 60:7.2f} çekirdek-saat')
