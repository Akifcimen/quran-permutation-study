// referans — referans örneklemi bölütü (ONKAYIT §3.2, §5): kitap üret, 406 mercekte değerlendir, sıklık tablolarını biriktir;
// ayrıca her kitabın SAYIM puanı (4 kapı) — referans kitapları sayım puanının sıra örnekleminin parçasıdır.
// Kullanım: ./referans <kodeks> <sifir U|M|L|J> <tohum> <N> <C_Q> <cikti_onek>
// Çıktılar (atomik): <onek>.ref (seyrek tablo deltası), <onek>.aday (sayım puanı ≥ C_Q−2 ya da bölüt içi en yüksek 30), <onek>.say (özet)
#include "tablo.h"
#include <time.h>
typedef struct __attribute__((packed)) { uint16_t a[114]; uint8_t C[4]; int16_t L; uint64_t sira; } aday_t;
int main(int argc,char**argv){
  if(argc<7){ fprintf(stderr,"kullanım: %s <kodeks> <sifir> <tohum> <N> <C_Q> <onek>\n",argv[0]); return 1; }
  int kod=atoi(argv[1]); char s=argv[2][0]; uint64_t seed=strtoull(argv[3],0,10); long N=atol(argv[4]); int CQ=atoi(argv[5]); const char *on=argv[6];
  butunluk_init(kod); orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,s,seed); ref_t R; ref_ayir(&R); static profil_t P;
  long hist[4][NPRED+1]; memset(hist,0,sizeof hist); long kq[4]={0,0,0,0};
  char fa[1024]; snprintf(fa,1024,"%s.aday.yaziliyor",on); FILE *fo=fopen(fa,"wb");
  aday_t top[30]; int ntop=0; long nyaz=0;
  struct timespec t0,t1; clock_gettime(CLOCK_MONOTONIC,&t0);
  const int puansiz = getenv("PUANSIZ")!=NULL;   // ön-kayıt öncesi sınamalar: rastgele kitabın puanı HESAPLANMAZ
  for(long b=0;b<N;b++){ orn_sonraki(Z); degerlendir(Z->a,&P); ref_ekle(&R,&P);
    if(puansiz) continue;
    int C[4], L0=-1; for(int g=0;g<4;g++){ int L; C[g]=sayim_puani(&P,g,&L); if(g==0) L0=L; hist[g][C[g]]++; if(C[g]>=CQ) kq[g]++; }
    aday_t A; for(int i=0;i<114;i++) A.a[i]=(uint16_t)Z->a[i]; for(int g=0;g<4;g++) A.C[g]=(uint8_t)C[g]; A.L=(int16_t)L0; A.sira=(uint64_t)b;
    if(C[0]>=CQ-2 || C[1]>=CQ-2 || C[2]>=CQ-2){ fwrite(&A,sizeof A,1,fo); nyaz++; }
    // bölüt içi en yüksek 30 (kapı G sayımı)
    if(ntop<30){ top[ntop++]=A; } else { int mi=0; for(int j=1;j<30;j++) if(top[j].C[0]<top[mi].C[0]) mi=j; if(A.C[0]>top[mi].C[0]) top[mi]=A; } }
  for(int j=0;j<ntop;j++) fwrite(&top[j],sizeof(aday_t),1,fo);
  fclose(fo); clock_gettime(CLOCK_MONOTONIC,&t1); double sn=(t1.tv_sec-t0.tv_sec)+1e-9*(t1.tv_nsec-t0.tv_nsec);
  char fr[1024]; snprintf(fr,1024,"%s.ref",on); ref_yaz(&R,fr);
  char fa2[1024]; snprintf(fa2,1024,"%s.aday",on); rename(fa,fa2);
  char fs[1024], ft[1024]; snprintf(fs,1024,"%s.say",on); snprintf(ft,1024,"%s.say.yaziliyor",on); FILE *f=fopen(ft,"w");
  fprintf(f,"kodeks %d sifir %c tohum %llu N %ld sure_s %.1f C_Q %d aday %ld ust30 %d\n",kod,s,(unsigned long long)seed,N,sn,CQ,nyaz,ntop);
  for(int g=0;g<4;g++){ fprintf(f,"kapi%d k(C>=C_Q) %ld hist",g,kq[g]); for(int c=0;c<=NPRED;c++) fprintf(f," %ld",hist[g][c]); fprintf(f,"\n"); }
  fclose(f); rename(ft,fs); return 0; }   // .say en son: bölüt tamam işareti
