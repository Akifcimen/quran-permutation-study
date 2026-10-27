# aday_cek.py — sınama aday dosyalarından (362 B kayıt) seçilen kitapları tek bir .bin dosyasına çeker (src/ayrinti girdisi).
# Kullanım: python3 -I src/aday_cek.py <cikti.bin> <aday_dizini> <bNNN:sıra> [...]
import sys, struct
R = 362; out = bytearray()
for arg in sys.argv[3:]:
    b, s = arg.split(':'); s = int(s); d = open(f'{sys.argv[2]}/{b}.aday', 'rb').read(); bul = False
    for o in range(0, len(d) - R + 1, R):
        if struct.unpack_from('<Q', d, o + 354)[0] == s: out += d[o:o + R]; bul = True; break
    if not bul: print('bulunamadı:', arg, file=sys.stderr)
open(sys.argv[1], 'wb').write(out); print(len(out) // R, 'kitap')
