// hedef_dogrulama — dondurma öncesi kod doğrulaması için HEDEF OLMAYAN rastgele kitaplar (ONKAYIT §0, §7.3): hedef.bin biçiminde
// K kayıt; kayıt h, tohum(sıfır, 4, 100 + h) ile kurulan örnekleyicinin kitabıdır (M20'de hedef.c gibi 6400 emisyon atılır).
// Tür 4 = kod doğrulama tohumları (plasebo12 ile aynı tür; sıra ≥ 100). Kur'an bu dosyada yoktur.
// Kullanım: ./hedef_dogrulama <sifir U|M|L> <K> <cikti.bin>
#include "../src/kutuphane.h"
int main(int argc,char**argv){ char s=argv[1][0]; int K=atoi(argv[2]); kutuphane_init(); FILE *f=fopen(argv[3],"wb"); orn_t *Z=malloc(sizeof(orn_t));
  for(int h=0;h<K;h++){ orn_init(Z,s,tohum(s,4,100+h)); if(s=='M') for(int e=0;e<6400;e++) orn_sonraki(Z); orn_sonraki(Z);
    int32_t id=h; int16_t a16[114]; for(int i=0;i<114;i++) a16[i]=(int16_t)Z->a[i]; fwrite(&id,4,1,f); fwrite(a16,2,114,f); }
  fclose(f); return 0; }
