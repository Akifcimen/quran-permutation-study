# tek_uret.py — kutuphane.h'deki kdegerlendir'den tek merceklik kdeg_tek'i mekanik olarak üretir (tek_mercek.h). Yalnız döngü sınırları
# ve asal maskesi değişir; yüklem hesabı satırları aynen kalır. Her değişiklik tam bir kez eşleşmelidir.
# Kullanım: python3 -I tek_uret.py <kutuphane.h> <tek_mercek.h>
import sys
s = open(sys.argv[1]).read()
b = s.index('static void kdegerlendir(const int *a, prof_t *P, uint32_t fbit){'); e = s.index('\n}\n', b) + 3; f = s[b:e]
D = [('static void kdegerlendir(const int *a, prof_t *P, uint32_t fbit){', 'static void kdeg_tek(const int *a, prof_t *P, int L0){'),
     ('static __thread kfam_t FF[NPR]; static __thread khfam_t HH[NPR];', 'static __thread kfam_t FF[NPR]; static __thread khfam_t HH[NPR]; const int i0=L0%NPR;'),
     ('for(int f=0;f<NFORM;f++){ if(!(fbit>>f&1)) continue;', 'for(int f=L0/NPR;f<=L0/NPR;f++){'),
     ('for(int i=0;i<NPR;i++){ kfam_t *F=&FF[i]; F->k=0;', 'for(int i=i0;i<=i0;i++){ kfam_t *F=&FF[i]; F->k=0;'),
     ('uint32_t mt=DIVM[t<0?-t:t];', 'uint32_t mt=DIVM[t<0?-t:t]&(1u<<i0);'),
     ('uint32_t mu=DIVM[u<0?-u:u];', 'uint32_t mu=DIVM[u<0?-u:u]&(1u<<i0);'),
     ('for(int i=0;i<NPR;i++){ const int p=PR[i]; const long pp=(long)p*p; const int L=f*NPR+i;', 'for(int i=i0;i<=i0;i++){ const int p=PR[i]; const long pp=(long)p*p; const int L=f*NPR+i;')]
for x, y in D:
    assert f.count(x) == 1, x
    f = f.replace(x, y)
open(sys.argv[2], 'w').write('// tek_mercek.h — tek_uret.py ile kutuphane.h kdegerlendir\'den üretildi; elle düzenlenmez.\n#pragma once\n' + f)
print('üretildi:', len(f.splitlines()), 'satır')
