# is_kuyrugu.py — bağımsız işlerin (menü bölütleri, adanmış kestirimler) KÜRESEL 4-süreç sınırı altında yürütücüsü.
# Her 5 sn'de bilgisayardaki bütün ağır süreçler sayılır (src/referans, src/test, src/menu, src/adanmis_s, src/adanmis_u; durumu ne
# olursa olsun — kos/duraklat.sh ile geçici durdurulanlar da sayılır, böylece yarış olmaz). kos/orkestra.py yaşıyorsa ona 3 yuva
# ayrılır. Toplam < 4 ise sıradaki iş başlatılır. İş listesi satırı: <ad>\t<kabuk komutu>; başarılı iş tab/kuyruk/<ad>.bitti ile
# işaretlenir (sürdürülebilir). Kullanım: python3 -I kos/is_kuyrugu.py <iş_listesi>
import os, sys, subprocess, time, datetime
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); os.chdir(KOK)
AGIR = ('src/referans', 'src/test', 'src/menu', 'src/adanmis_s', 'src/adanmis_u')
def gunluk(m):
    with open('sonuc/kosu_gunlugu.txt', 'a') as f:
        f.write(f"[{datetime.datetime.now(datetime.timezone.utc).strftime('%Y-%m-%dT%H:%M:%SZ')}] {m}\n")
def say():   # çalıştırılabilir dosya adına göre (kabuk sarmalayıcıları sayılmaz)
    ex = subprocess.run(['ps', '-axo', 'comm='], capture_output=True, text=True).stdout.split('\n')
    agir = sum(1 for c in ex if any(c.strip().endswith(a) for a in AGIR))
    ork_cocuk = sum(1 for c in ex if any(c.strip().endswith(a) for a in ('src/referans', 'src/test')))   # orkestranın ağır çocukları
    ork = any('kos/orkestra.py' in c for c in subprocess.run(['ps', '-axo', 'command='], capture_output=True, text=True).stdout.split('\n'))
    return agir, ork, ork_cocuk
isler = [l.rstrip('\n').split('\t', 1) for l in open(sys.argv[1]) if l.strip() and not l.startswith('#')]
os.makedirs('tab/kuyruk', exist_ok=True)
bekleyen = [(a, k) for a, k in isler if not os.path.exists(f'tab/kuyruk/{a}.bitti')]
gunluk(f'iş kuyruğu başladı ({os.path.basename(sys.argv[1])}): {len(bekleyen)} iş bekliyor')
calisan = {}
while bekleyen or calisan:
    for a, (p, t0) in list(calisan.items()):
        rc = p.poll()
        if rc is None: continue
        del calisan[a]
        if rc == 0: open(f'tab/kuyruk/{a}.bitti', 'w').close(); gunluk(f'kuyruk {a} bitti ({(time.time() - t0) / 60:.1f} dk)')
        else: gunluk(f'kuyruk {a} HATA rc={rc}')
    agir, ork, ork_cocuk = say()
    dolu = agir + (max(0, 3 - ork_cocuk) if ork else 0)        # orkestra yaşıyorsa 3 yuvası (boşta olsa bile) ayrılı
    if bekleyen and dolu < 4:
        a, k = bekleyen.pop(0)
        calisan[a] = (subprocess.Popen(['/bin/bash', '-c', k], stdout=subprocess.DEVNULL, stderr=open('tab/kuyruk/hata.txt', 'a')), time.time())
        time.sleep(2); continue
    time.sleep(5)
gunluk(f'iş kuyruğu bitti ({os.path.basename(sys.argv[1])})')
