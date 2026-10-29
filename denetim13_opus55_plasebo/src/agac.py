# agac.py — kütüphane L*'ın üretim ağacı (düzey × tür × nicelik × ilişki) ve dil bedeli (ONKAYIT §2.2, §4).
# Tek kaynak: C başlığı src/agac_uretim.h bu dosyadan üretilir (python3 -I src/agac.py --c > src/agac_uretim.h).
# Yüklem bedeli c(π) = log2(#düzey) + log2(#tür | düzey) + log2(#nicelik | düzey, tür) + log2(#ilişki | nicelik), her düğümde
# L*'da kalan seçenekler üzerinde tekdüze (Kraft eşitliği: Σ 2^−c(π) = 1). Dil bedeli c(Λ) = log2(21) + Σ_{π∈Λ} c(π) − log2(|Λ|!).
import math, sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import kutuphane as K

DUZEY = ['kitap', 'aile', 'hücre', 'birleşim']
TUR = {'a': 'denge', 'b': 'modülün geri dönüşü', 'c': 'iki el', 'd': 'ardışıklık'}
# (düzey, tür, nicelik, [(yüklem, ilişki), ...]); None = üretilip her mercekte imkânsız olduğu için L* dışı bırakılan aday
AGAC = [
    ('kitap', 'a', 'E bölmesi (t-paritesi)', [('B1', 'eşit sayı'), ('B2', 'eşit toplam (12. den. B2)'), ('Ba', 'eşit Σa'),
                                              (None, 'eşit Σt [Bt; imkânsız]'), (None, 'eşit Σn [Bn; imkânsız]')]),
    ('kitap', 'b', 'E yarılarının n-toplamları', [('BMn', 'p | ikisi'), (None, 'p² | ikisi [imkânsız]'), (None, '= p, = 2p, merkez = p [imkânsız]')]),
    ('kitap', 'b', 'E yarılarının t-toplamları', [('BMt', 'p | ikisi'), ('BMt2', 'p² | ikisi'), (None, '= p, = 2p, merkez = p [imkânsız]')]),
    ('kitap', 'b', 'E yarılarının a-toplamları', [(None, 'p | ikisi, p² | ikisi, = p, = 2p, merkez = p [hepsi imkânsız]')]),
    ('aile', 'a', 'F satırları (t-paritesi yarıları)', [('F1', 'eşit sayı'), ('F2', 'eşit toplam')]),
    ('aile', 'a', 'F sütunları (n-paritesi yarıları)', [('FC1', 'eşit sayı'), ('FC2', 'eşit toplam')]),
    ('aile', 'a', 'F köşegenleri (00∪11 / 01∪10)', [('FD1', 'eşit sayı'), ('FD2', 'eşit toplam')]),
    ('aile', 'a', 'H satırları (t-paritesi yarıları)', [('HH1', 'eşit sayı'), ('HH2', 'eşit toplam')]),
    ('aile', 'b', 'k_F', [('QkF', '= p'), ('Zk2p', '= 2p'), ('Zkd', 'p |'), ('Zkd2', 'p² |')]),
    ('aile', 'b', 'Σq', [('FSp', '= p'), ('FS2p', '= 2p'), ('QS', 'p |'), ('FSd2', 'p² |')]),
    ('aile', 'b', 'F yarı toplamları', [(None, 'her biri = p [FHp; imkânsız]'), ('FH2p', 'her biri = 2p'), ('FHm', 'merkez = p'),
                                        ('Qh', 'p | her biri'), ('FHd2', 'p² | her biri')]),
    ('aile', 'b', 'F sütun toplamları', [('FCp', 'her biri = p'), ('FC2p', 'her biri = 2p'), ('FCm', 'merkez = p'), ('FCd', 'p | her biri'), ('FCd2', 'p² | her biri')]),
    ('aile', 'b', 'F yarı boyları', [('FHSp', 'her biri = p'), ('FHS2p', 'her biri = 2p'), ('FHSm', 'merkez = p'), ('FHSd', 'p | her biri'), ('FHSd2', 'p² | her biri')]),
    ('aile', 'b', 'Σ_F a', [('FAp', '= p'), ('FA2p', '= 2p'), ('FAd', 'p |'), ('FAd2', 'p² |')]),
    ('aile', 'b', 'Σ_F n', [('FNp', '= p'), ('FN2p', '= 2p'), ('FNd', 'p |'), ('FNd2', 'p² |')]),
    ('aile', 'd', 'F satır toplamları', [('FRK', 'ardışık (fark 1)')]),
    ('aile', 'd', 'F sütun toplamları', [('FCK', 'ardışık (fark 1)')]),
    ('aile', 'd', 'F katsayıları', [('K2', 'kesintisiz bant')]),
    ('aile', 'd', 'H katsayıları', [('HK2', 'kesintisiz bant')]),
    ('hücre', 'a', 'F satır-0 hücreleri', [('R0c', 'eşit sayı'), ('R0e', 'eşit toplam')]),
    ('hücre', 'a', 'F satır-1 hücreleri', [('R1c', 'eşit sayı'), ('R1e', 'eşit toplam')]),
    ('hücre', 'a', 'H satır-0 hücreleri', [('HR0c', 'eşit sayı'), ('HR0e', 'eşit toplam')]),
    ('hücre', 'a', 'H satır-1 hücreleri', [('HR1c', 'eşit sayı'), ('HR1e', 'eşit toplam')]),
    ('hücre', 'b', 'F hücre toplamları', [(None, 'her biri = p [FKp; imkânsız]'), ('FK2p', 'her biri = 2p'), ('Qc', 'merkez = p'),
                                         ('FKd', 'p | her biri'), ('FKd2', 'p² | her biri')]),
    ('hücre', 'b', 'F hücre boyları', [('FKSp', 'her biri = p'), ('FKS2p', 'her biri = 2p'), ('FKSm', 'merkez = p'), ('FKSd', 'p | her biri'), ('FKSd2', 'p² | her biri')]),
    ('hücre', 'd', 'F hücre toplamları', [('K1', 'farklı değerler ardışık'), ('FAP', 'aritmetik dizi (fark ≥ 1)')]),
    ('hücre', 'd', 'F hücre boyları', [('FKS', 'farklı değerler ardışık')]),
    ('hücre', 'd', 'F hücre katsayıları', [('FSC', 'sabit hücre (≥ 2 eşit q)')]),
    ('birleşim', 'b', '|U|', [('QU', '= p'), ('WU2p', '= 2p'), ('WUd', 'p |'), ('WUd2', 'p² |')]),
    ('birleşim', 'b', 'k_H', [('QkH', '= p'), ('WH2p', '= 2p'), ('WHd', 'p |'), ('WHd2', 'p² |')]),
    ('birleşim', 'b', '|F ∩ H|', [('WIp', '= p'), ('WI2p', '= 2p'), ('WId', 'p |'), ('WId2', 'p² |')]),
    ('birleşim', 'b', 'Σ_U a', [('WUAp', '= p'), ('WUA2p', '= 2p'), ('Qa', 'p |'), ('WUAd2', 'p² |')]),
    ('birleşim', 'b', 'Σ_U n', [('WUNp', '= p'), ('WUN2p', '= 2p'), ('Qn', 'p |'), ('WUNd2', 'p² |')]),
    ('birleşim', 'b', 'Σ_U t', [('WUTp', '= p'), ('WUT2p', '= 2p'), ('WUTd', 'p |'), ('WUTd2', 'p² |')]),
    ('birleşim', 'b', 'Σ_H a', [('WHAp', '= p'), ('WHA2p', '= 2p'), ('WHAd', 'p |'), ('WHAd2', 'p² |')]),
    ('birleşim', 'b', 'Σ_H n', [('WHNp', '= p'), ('WHN2p', '= 2p'), ('WHNd', 'p |'), ('WHNd2', 'p² |')]),
    ('birleşim', 'b', 'Σ_H u/p', [('WHRp', '= p'), ('WHR2p', '= 2p'), ('QH', 'p |'), ('WHRd2', 'p² |')]),
    ('birleşim', 'c', 'F ∩ H', [('W1', '= ∅'), ('WI1', '= 1 eleman')]),
    ('birleşim', 'c', '(k_F, k_H)', [('WEq', 'k_F = k_H'), ('WSum', 'k_F + k_H = p')]),
]


def yaprak():
    """L* yüklemleri: ad → (düzey, tür, nicelik, ilişki, bedel)"""
    # yalnız L*'da kalan seçenekler sayılır
    duz = {}
    for d, t, n, L in AGAC:
        canli = [(y, r) for y, r in L if y is not None]
        if not canli:
            continue
        duz.setdefault(d, {}).setdefault(t, {})[n] = canli
    nd = len(duz)
    out = {}
    for d, tt in duz.items():
        nt = len(tt)
        for t, nn in tt.items():
            nq = len(nn)
            for n, L in nn.items():
                nr = len(L)
                c = math.log2(nd) + math.log2(nt) + math.log2(nq) + math.log2(nr)
                for y, r in L:
                    out[y] = (d, t, n, r, c)
    return out


YAP = yaprak()
LSTAR = [ad for ad in K.TUM if ad in YAP]          # global sıra (C ile aynı)


def dil_bedeli(dil):
    k = len(dil)
    return math.log2(21) + sum(YAP[x][4] for x in dil) - math.lgamma(k + 1) / math.log(2)


if __name__ == '__main__':
    assert set(YAP) <= set(K.TUM)
    disari = [x for x in K.TUM if x not in YAP]
    kraft = sum(2 ** -v[4] for v in YAP.values())
    if '--c' in sys.argv:
        print('// agac_uretim.h — src/agac.py tarafından üretildi; elle değiştirmeyin.')
        print('#ifndef AGAC_URETIM_H\n#define AGAC_URETIM_H')
        print(f'#define NLSTAR {len(LSTAR)}')
        print('// global yüklem sırasında (kutuphane.h PADI): L* üyeliği, bedel (bit), düzey (0 kitap,1 aile,2 hücre,3 birleşim), tür (0 a,1 b,2 c,3 d)')
        print('static const uint8_t LUYE[NPRED]={' + ','.join('1' if x in YAP else '0' for x in K.TUM) + '};')
        print('static const double PBEDEL[NPRED]={' + ','.join(f'{YAP[x][4]:.15f}' if x in YAP else '0.0' for x in K.TUM) + '};')
        print('static const uint8_t PDUZEY[NPRED]={' + ','.join(str(DUZEY.index(YAP[x][0])) if x in YAP else '9' for x in K.TUM) + '};')
        print('static const uint8_t PTUR[NPRED]={' + ','.join(str('abcd'.index(YAP[x][1])) if x in YAP else '9' for x in K.TUM) + '};')
        print('#endif')
    else:
        print(f'L*: {len(LSTAR)} yüklem; L* dışı (imkânsız): {disari}; Kraft toplamı {kraft:.12f}')
        for d, t, n, L in AGAC:
            for y, r in L:
                if y in YAP:
                    print(f'  {y:6s} {d:9s} {t} {n:38s} {r:28s} {YAP[y][4]:.3f} bit')
        el = K.HAND
        print(f'el dili (20): Σc(π) = {sum(YAP[x][4] for x in el):.3f} bit; c(Λ_el) = {dil_bedeli(el):.3f} bit')
