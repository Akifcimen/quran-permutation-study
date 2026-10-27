# belirsiz_kimlik.py — sonuc/belirsiz/*.bin içindeki (src/aday_cek ile çekilmiş) kitapların hangi sınama bölütünden geldiğini, kaydı
# bayt bayt eşleyerek bulur; <bin>.kimlik dosyasına "bölüt:sıra" satırları yazar (rapor tablosu için).
# Kullanım: python3 -I src/belirsiz_kimlik.py <sıfırkodeks> <bin> ...
import sys, os, glob, struct
KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); sk = sys.argv[1]; R = 362
seg = {os.path.basename(f)[:-5]: open(f, 'rb').read() for f in sorted(glob.glob(f'{KOK}/tab/T_{sk}/b*.aday'))}
for fn in sys.argv[2:]:
    d = open(fn, 'rb').read(); out = []
    for o in range(0, len(d) - R + 1, R):
        rec = d[o:o + R]; s, = struct.unpack_from('<Q', rec, 354)
        bul = [b for b, x in seg.items() if x.find(rec) >= 0 and x.find(rec) % R == 0]
        out.append(f'{bul[0] if len(bul) == 1 else "?"}:{s}')
    open(fn + '.kimlik', 'w').write('\n'.join(out) + '\n'); print(os.path.basename(fn), ' '.join(out))
