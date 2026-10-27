# sira_duzelt.py — bilgi puanı sırasının adanmış kestirimlerle düzeltilmesi (ONKAYIT §4.3). bilgi_sira.py'nin k_alt, k_nokta, k_üst
# sayıları, belirsiz_coz.py çıktısındaki yeniden puanlanmış kitapların önce/sonra sınıfına göre düzeltilir (her kitap bir kez).
# Kullanım: python3 -I src/sira_duzelt.py <N> <k_alt> <k_nokta> <k_üst> <S_Q> <belirsiz_coz çıktısı>
import sys, re, math
N, ka, kn, ku = (int(float(x)) for x in sys.argv[1:5]); SQ = float(sys.argv[5]) - 1e-6
sayi = lambda s: math.inf if s.strip() == '∞' else float(s)
d = [0, 0, 0]; gor = set()
for l in open(sys.argv[6]):
    m = re.search(r'^(\S+) (\d+): önce nokta ([\d.]+) \[([\d.]+), ([^\]]+)\] → sonra nokta ([\d.]+) \[([\d.]+), ([^\]]+)\]', l)
    if not m or (m.group(1), m.group(2)) in gor: continue
    gor.add((m.group(1), m.group(2)))
    once = (sayi(m.group(4)) >= SQ, float(m.group(3)) >= SQ, sayi(m.group(5)) >= SQ)
    # sonranın alt/üst sınıfı tam duyarlıkla verilmiş karardan (alt ≥ S_Q ⟺ ÜSTTE, üst ≥ S_Q ⟺ ALTTA değil)
    karar = re.search(r'→ (ÜSTTE|ALTTA|BELİRSİZ)', l[m.end():]).group(1)
    sonra = (karar == 'ÜSTTE', float(m.group(6)) >= SQ, karar != 'ALTTA')
    # 2 ondalıklı yazılmış değerler S_Q'ya yuvarlama payından yakınsa uyar
    for g in (3, 4, 5, 6):
        if abs(sayi(m.group(g)) - SQ) < 0.006: print(f'UYARI: {m.group(1)} {m.group(2)}: {m.group(g)} S_Q\'ya yuvarlama payından yakın')
    for i in range(3): d[i] += int(sonra[i]) - int(once[i])
ka += d[0]; kn += d[1]; ku += d[2]
p = lambda k: (k + 1) / (N + 1)
print(f'yeniden puanlanan kitap {len(gor)}; düzeltme Δk_alt {d[0]:+d}, Δk_nokta {d[1]:+d}, Δk_üst {d[2]:+d}')
print(f'k_alt {ka}, k_nokta {kn}, k_üst {ku} → p_nokta = {p(kn):.3g}; aralık [{p(ka):.3g}, {p(ku):.3g}] (N = {N:.3g})')
