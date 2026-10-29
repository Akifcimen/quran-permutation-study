// c_dok — kod doğrulaması için rastgele kitapları (tür 4 tohumları) ve 406 mercekteki kütüphane maskelerini ikili dosyaya döker.
// Çıktı yalnız c_py_esit.py tarafından okunur; hiçbir değer ekrana basılmaz. Kullanım: ./c_dok <sifir> <sira> <N> <cikti>
#include "../src/kutuphane.h"
int main(int argc,char**argv){ char s=argv[1][0]; long sira=atol(argv[2]); long N=atol(argv[3]); kutuphane_init();
  orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,s,tohum(s,4,sira)); static prof_t P; FILE *f=fopen(argv[4],"wb");
  for(long b=0;b<N;b++){ orn_sonraki(Z); kdegerlendir(Z->a,&P,0x3FFF); int16_t a16[114]; for(int i=0;i<114;i++) a16[i]=(int16_t)Z->a[i];
    fwrite(a16,2,114,f); fwrite(P.B,1,NLENS,f); fwrite(P.Z,1,NLENS,f); fwrite(P.F,8,NLENS,f); fwrite(P.W,8,NLENS,f); fwrite(P.H,1,NLENS,f);
    fwrite(P.kF,1,NLENS,f); fwrite(P.kH,1,NLENS,f); fwrite(P.mc,1,NLENS,f); }
  fclose(f); return 0; }
