// mercek_profil <hedef.bin> <hedef_no> <L> — kütüphanenin o mercekte doğru olan bütün yüklemleri (dışlamalardan sonra), düzey/tür etiketiyle.
#include "kutuphane.h"
#include "agac_uretim.h"
int main(int argc,char**argv){ kutuphane_init(); FILE *f=fopen(argv[1],"rb"); int h=atoi(argv[2]), L=atoi(argv[3]);
  fseek(f,(long)h*232+4,SEEK_SET); int16_t a16[114]; if(fread(a16,2,114,f)!=114) return 2; fclose(f); int a[114]; for(int i=0;i<114;i++) a[i]=a16[i];
  static prof_t P; kdegerlendir(a,&P,0x3FFF); const uint64_t m[5]={P.B[L],P.Z[L],P.F[L],P.W[L],P.H[L]}; int n=0; const char *DZ="KAHB", *TR="abcd";
  for(int g=0;g<NGRUP;g++) for(int b=0;b<GBOY[g];b++) if(m[g]>>b&1){ int gi=GOFS[g]+b; printf("%s(%c%c) ",PADI[gi],PDUZEY[gi]<4?DZ[PDUZEY[gi]]:'?',PTUR[gi]<4?TR[PTUR[gi]]:'?'); n++; }
  printf("\n  toplam %d doğru yüklem; kF %d kH %d en küçük hücre %d\n",n,P.kF[L],P.kH[L],P.mc[L]); return 0; }
