// yeniden_puan — ONKAYIT §6.1: .q/.s kayıtlarındaki kitapları donmuş tam yolla (kdegerlendir 0x3FFF, kapi_listesi, puan_B) yeniden puanlar.
// Kullanım: ./yeniden_puan <diller.txt> <kayıt_dosyası> <ref>...   Çıktı satırı: dil sira kayıt_puan yeniden_puan kayıt_L yeniden_L bv kF kH mc
#include "dtab_b.h"
typedef struct __attribute__((packed)) { uint32_t sira; float puan; int16_t L; uint8_t sinif, dil; uint16_t a[114]; } aday_t;
int main(int argc, char **argv){
  if(argc<4){ fprintf(stderr,"kullanım\n"); return 1; }
  kutuphane_init(); mercek_bedeli_hazirla(); static atablo_t T; atablo_kur(&T,argv+3,argc-3);
  static dtab_t DT[8]; static dek_t DE[8]; char ad[8][64]; int ND=0;
  FILE *f=fopen(argv[1],"r"); int hed, kod, L, n;
  while(ND<8 && fscanf(f,"%63s %d %d %d %d",ad[ND],&hed,&kod,&L,&n)==5){ int g[DMAX]; for(int i=0;i<n;i++) if(fscanf(f,"%d",&g[i])!=1) return 2;
    const int kol=kod/10, ons=kod%10; dtab_kur(&DT[ND],&T,g,n,ons); DE[ND].kol=kol; DE[ND].tmin=KOL_TMIN[kol]; DE[ND].dmin=KOL_DMIN[kol]; ND++; }
  fclose(f);
  FILE *q=fopen(argv[2],"rb"); aday_t A; static prof_t P; int GL[NLENS];
  while(fread(&A,sizeof A,1,q)==1){ int a[114]; for(int i=0;i<114;i++) a[i]=A.a[i]; kdegerlendir(a,&P,0x3FFF); const int nGL=kapi_listesi(&P,GL);
    const int d=A.dil; int Lx=-1, bv=0; const double v=nGL?puan_B(&DT[d],&DE[d],&T,&P,GL,nGL,&Lx,&bv):-INFINITY;
    printf("%s %u %.6f %.6f %d %d %d %d %d %d\n",ad[d],A.sira,(double)A.puan,v,A.L,Lx,bv,Lx>=0?P.kF[Lx]:-1,Lx>=0?P.kH[Lx]:-1,Lx>=0?P.mc[Lx]:-1); }
  fclose(q); return 0; }
