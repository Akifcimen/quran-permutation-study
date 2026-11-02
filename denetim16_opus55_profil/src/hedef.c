// hedef — hedef kitaplar (ONKAYIT §5.1): kayıt 0 = Kur'an (129), kayıt i = i. plasebo kitap (i = 1..K), tohum = tohum(sıfır, 3, i).
// M20: her plasebo kendi örnekleyicisinden (ayrı 64 zincir); ilk 6400 emisyon atılır, sonraki emisyon alınır (zincir 0'ın 101. emisyonu).
// U ve Mallows: tohumlu örnekleyicinin ilk kitabı (kesin örnekleme). Hedefler zenginliklerine göre süzülmez.
// Kullanım: ./hedef <sifir U|M|L> <K> <cikti.bin>   (kayıt: int32 hedef_no | int16 a[114])
#include "kutuphane.h"
int main(int argc,char**argv){ char s=argv[1][0]; int K=atoi(argv[2]); kutuphane_init(); FILE *f=fopen(argv[3],"wb");
  int32_t id=0; int16_t a16[114]; for(int i=0;i<114;i++) a16[i]=(int16_t)BASE[i]; fwrite(&id,4,1,f); fwrite(a16,2,114,f);
  orn_t *Z=malloc(sizeof(orn_t));
  for(int h=1;h<=K;h++){ orn_init(Z,s,tohum(s,3,h)); if(s=='M') for(int e=0;e<6400;e++) orn_sonraki(Z); orn_sonraki(Z);
    id=h; for(int i=0;i<114;i++) a16[i]=(int16_t)Z->a[i]; fwrite(&id,4,1,f); fwrite(a16,2,114,f); }
  fclose(f); return 0; }
