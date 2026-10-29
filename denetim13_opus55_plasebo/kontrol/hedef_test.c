// hedef_test — mekanik sınama için yedek "hedefler": hepsi kod-doğrulama tohumlarından (tür 4) rastgele kitaplar; Kur'an YOK.
#include "../src/kutuphane.h"
int main(int argc,char**argv){ char s=argv[1][0]; int K=atoi(argv[2]); kutuphane_init(); FILE *f=fopen(argv[3],"wb"); orn_t *Z=malloc(sizeof(orn_t));
  for(int h=0;h<=K;h++){ orn_init(Z,s,tohum(s,4,1000+h)); orn_sonraki(Z); int32_t id=h; int16_t a16[114]; for(int i=0;i<114;i++) a16[i]=(int16_t)Z->a[i]; fwrite(&id,4,1,f); fwrite(a16,2,114,f); }
  fclose(f); return 0; }
