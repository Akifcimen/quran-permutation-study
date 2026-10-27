// test — sınama örneklemi bölütü (ONKAYIT §5): kitap üret, 406 mercekte değerlendir; bilgi puanı (8 değişke, tablo araması)
// ve sayım puanı (4 kapı). Kur'an eşikleri dosyadan okunur; sıra sayıları ve adaylar yazılır.
// Kullanım: ./test <kodeks> <sifir U|M|L|J> <tohum> <N> <tablo.tab> <esik.txt> <cikti_onek>
// Aday: herhangi değişkede S_üst ≥ SQ − 2 bit (ilk sürümde − 6; disk için daraltıldı, ek/EK_..._aday_payi.md) ya da herhangi kapıda C ≥ CQ − 2;
// ayrıca bölüt içi en yüksek 30 (birincil nokta puanı).
#include "bilgi.h"
#include <time.h>
#define HB 1200
#define HMIN (-20.0)
#define HW 0.1
typedef struct __attribute__((packed)) { uint16_t a[114]; float pt[NVAR], lo[NVAR], hi[NVAR]; int16_t L[NVAR]; uint8_t coz[NVAR]; uint8_t C[4]; int16_t CL; uint64_t sira; } kayit_t;
static inline int hkutu(double x){ int b=(int)floor((x-HMIN)/HW); return b<0?0:(b>=HB?HB-1:b); }
int main(int argc,char**argv){
  if(argc<8){ fprintf(stderr,"kullanım\n"); return 1; }
  int kod=atoi(argv[1]); char s=argv[2][0]; uint64_t seed=strtoull(argv[3],0,10); long N=atol(argv[4]); const char *on=argv[7];
  butunluk_init(kod); bedel_hazirla(); tablo_t T; if(!tablo_oku(&T,argv[5])){ fprintf(stderr,"tablo\n"); return 2; }
  double SQ[NVAR]; int CQ[4]; { FILE *f=fopen(argv[6],"r"); char t[8]; int i; double x; int nq=0;
    while(fscanf(f,"%7s %d %lf",t,&i,&x)==3){ if(!strcmp(t,"SQ")) SQ[i]=x; else CQ[i]=(int)x; nq++; } fclose(f); if(nq!=12){ fprintf(stderr,"eşik\n"); return 2; } }
  orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,s,seed); static profil_t P;
  static long Hpt[NVAR][HB], Hlo[NVAR][HB]; long Hc[4][NPRED+1]; memset(Hc,0,sizeof Hc);
  long klo[NVAR]={0}, kpt[NVAR]={0}, khi[NVAR]={0}, kc[4]={0};
  char fa[1024]; snprintf(fa,1024,"%s.aday.yaziliyor",on); FILE *fo=fopen(fa,"wb"); long nyaz=0;
  kayit_t top[30]; int ntop=0;
  struct timespec t0,t1; clock_gettime(CLOCK_MONOTONIC,&t0);
  for(long b=0;b<N;b++){ orn_sonraki(Z); degerlendir(Z->a,&P); bilgi_t B; bilgi_puan(&T,&P,&B);
    kayit_t K; int aday=0;
    for(int v=0;v<NVAR;v++){ Hpt[v][hkutu(B.pt[v])]++; Hlo[v][hkutu(B.lo[v])]++;
      if(B.lo[v]>=SQ[v]-1e-6) klo[v]++; if(B.pt[v]>=SQ[v]-1e-6) kpt[v]++; if(B.hi[v]>=SQ[v]-1e-6) khi[v]++; if(B.hi[v]>=SQ[v]-2.0) aday=1;
      K.pt[v]=(float)B.pt[v]; K.lo[v]=(float)B.lo[v]; K.hi[v]=isinf(B.hi[v])?INFINITY:(float)B.hi[v]; K.L[v]=(int16_t)B.L[v]; K.coz[v]=(uint8_t)B.coz[v]; }
    for(int g=0;g<4;g++){ int L; int c=sayim_puani(&P,g,&L); K.C[g]=(uint8_t)c; if(g==0) K.CL=(int16_t)L; Hc[g][c]++; if(c>=CQ[g]) kc[g]++; if(g<3 && c>=CQ[g]-2) aday=1; }   // G0 aday ölçütünden çıkarıldı (ek: aday_G0)
    for(int i=0;i<114;i++) K.a[i]=(uint16_t)Z->a[i]; K.sira=(uint64_t)b;
    if(aday){ fwrite(&K,sizeof K,1,fo); nyaz++; }
    if(ntop<30) top[ntop++]=K; else { int mi=0; for(int j=1;j<30;j++) if(top[j].pt[0]<top[mi].pt[0]) mi=j; if(K.pt[0]>top[mi].pt[0]) top[mi]=K; } }
  for(int j=0;j<ntop;j++) fwrite(&top[j],sizeof(kayit_t),1,fo);
  fclose(fo); clock_gettime(CLOCK_MONOTONIC,&t1); double sn=(t1.tv_sec-t0.tv_sec)+1e-9*(t1.tv_nsec-t0.tv_nsec);
  char fh[1024], fht[1024]; snprintf(fh,1024,"%s.hist",on); snprintf(fht,1024,"%s.hist.yaziliyor",on); FILE *f=fopen(fht,"wb");
  fwrite(Hpt,8,NVAR*HB,f); fwrite(Hlo,8,NVAR*HB,f); fwrite(Hc,8,4*(NPRED+1),f); fclose(f); rename(fht,fh);
  char fa2[1024]; snprintf(fa2,1024,"%s.aday",on); rename(fa,fa2);
  char fs[1024], ft[1024]; snprintf(fs,1024,"%s.say",on); snprintf(ft,1024,"%s.say.yaziliyor",on); f=fopen(ft,"w");
  fprintf(f,"kodeks %d sifir %c tohum %llu N %ld sure_s %.1f aday %ld\n",kod,s,(unsigned long long)seed,N,sn,nyaz);
  for(int v=0;v<NVAR;v++) fprintf(f,"degiske %d SQ %.6f k_alt %ld k_nokta %ld k_ust %ld\n",v,SQ[v],klo[v],kpt[v],khi[v]);
  for(int g=0;g<4;g++) fprintf(f,"kapi %d CQ %d k %ld\n",g,CQ[g],kc[g]);
  fclose(f); rename(ft,fs); return 0; }
