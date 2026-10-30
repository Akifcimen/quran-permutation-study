# on_dokuz_sayimi.py — ek 18:35Z §3.1 (sonradan, betimsel): Gc 10¹⁰ isabetlerinden p = 19'da ve metnin andığı asallarda Kur'an düzeyine ulaşanlar.
# Girdi: bagimsiz_sina_kapi.py'nin "isabet" çıktısı (her kayıt için en iyi mercek ve bütünlüklü mercekler listesi).
# Alt sınır: en iyi merceği kümede olan isabet; üst sınır: kümede bütünlüklü merceği olan isabet. İkisi eşitse sayı kesindir.
# Kullanım: python3 -I on_dokuz_sayimi.py <sina_M.txt> <sina_U.txt> <sina_L.txt>
import re, sys
PR = [3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113]
for dosya in sys.argv[1:]:
    for ad, S in (('19', {19}), ('metnin andığı {3,5,7,11,19}', {3, 5, 7, 11, 19})):
        alt = ust = top = 0; asal = {}
        for ln in open(dosya):
            m = re.search(r'mercek \((-?\d+), (-?\d+)\),(\d+).*bütünlüklü mercekler \[(.*)\]', ln)
            if not m: continue
            top += 1; p = int(m.group(3)); bl = [int(x) for x in m.group(4).split(',') if x.strip()]
            asal[p] = asal.get(p, 0) + 1; alt += p in S; ust += any(PR[L % 29] in S for L in bl)
        print(f'{dosya}: isabet {top}; {ad}: alt {alt} üst {ust}; en iyi merceğin asalı {dict(sorted(asal.items()))}')
