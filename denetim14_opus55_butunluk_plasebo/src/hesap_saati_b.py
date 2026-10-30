# hesap_saati_b.py — kayıtlı hesap (çekirdek-saat; ONKAYIT §8.2): kontrol bölütlerinin süre_s alanları (tab/<sıfır>/k<küme>/d*/b*.txt)
# ve sonuc/hesap_ek.txt'deki elle kaydedilmiş kalemler (satır: <çekirdek-saniye> <açıklama>; dondurma öncesi sınamalar, dil kurma, Ö0).
# Kullanım: python3 -I src/hesap_saati_b.py  (ya da içe aktarılıp toplam() çağrılır)
import os, glob
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def kalemler():
    out = {}
    for fn in glob.glob(os.path.join(KOK, 'tab', '*', 'k*', 'd*', 'b*.txt')):
        p = fn.split(os.sep); anah = f'{p[-4]}/{p[-3]}'
        w = open(fn).readline().split()
        if 'sure_s' in w: out[anah] = out.get(anah, 0.0) + float(w[w.index('sure_s') + 1])
    ek = 0.0
    fn = os.path.join(KOK, 'sonuc', 'hesap_ek.txt')
    if os.path.exists(fn):
        for s in open(fn):
            w = s.split()
            if w and not w[0].startswith('#'): ek += float(w[0])
    out['ek'] = ek
    return out


def toplam():
    return sum(kalemler().values()) / 3600


if __name__ == '__main__':
    k = kalemler()
    for a in sorted(k): print(f'{a}: {k[a] / 3600:.2f} çekirdek-saat')
    print(f'toplam (kayıtlı): {toplam():.2f} çekirdek-saat')
