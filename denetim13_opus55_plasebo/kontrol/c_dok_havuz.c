// c_dok_havuz — kod doğrulaması: 10. denetimin O-havuzu kitapları (244 baytlık kayıtlar; ~/opus5.5/kosullu/tab/havuz, salt okunur)
// üzerinde kütüphane maskelerini c_py_esit.py biçiminde döker. Bu kitaplar (n+a,19)'da eski literatür yapısını taşıyan rastgele
// kitaplardır; nadir yüklemleri çalıştırmak için kullanılır. Hiçbir değer ekrana basılmaz. Kullanım: ./c_dok_havuz <havuz.bin> <N> <cikti>
#include "../src/kutuphane.h"
int main(int argc,char**argv){ long N=atol(argv[2]); kutuphane_init(); FILE *fi=fopen(argv[1],"rb"); FILE *f=fopen(argv[3],"wb");
  static prof_t P; unsigned char rec[244]; long b=0;
  while(b<N && fread(rec,244,1,fi)==1){ int a[114]; int16_t a16[114]; for(int i=0;i<114;i++){ uint16_t v; memcpy(&v,rec+2*i,2); a[i]=v; a16[i]=(int16_t)v; }
    kdegerlendir(a,&P,0x3FFF);
    fwrite(a16,2,114,f); fwrite(P.B,1,NLENS,f); fwrite(P.Z,1,NLENS,f); fwrite(P.F,8,NLENS,f); fwrite(P.W,8,NLENS,f); fwrite(P.H,1,NLENS,f);
    fwrite(P.kF,1,NLENS,f); fwrite(P.kH,1,NLENS,f); fwrite(P.mc,1,NLENS,f); b++; }
  fclose(f); fclose(fi); return 0; }
