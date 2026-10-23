// Yoklama: Kur'an'ın sabit-mercek olguları, Mallows q, sıfırlarda olay oranları ve hız.
#include "../src/ortak.h"
#include <time.h>
static double now(){ struct timespec t; clock_gettime(CLOCK_MONOTONIC,&t); return t.tv_sec+1e-9*t.tv_nsec; }
int main(int argc, char **argv){
  int kod=atoi(argv[1]); init_ortak(kod); ref_hazirla();
  long Iobs=ters_kesin(BASE); double q=mallows_q_ayarla(Iobs);
  printf("kodeks %d: Σa=%d S_obs=%ld I_kesin=%ld → Mallows q=%.16f (E[I]=%.6f; tekdüze E=%.3f)\n",kod,SA_TOT,S_OBS,Iobs,q,mallows_beklenen(q),mallows_beklenen(0.99999999));
  fres_t R; facts19(BASE,&R,1); printf("Kur'an olguları (kF=%d kG=%d same=%d):\n",R.kF,R.kG,R.same);
  u128 b=R.bits; while(b){ int f=ctz128(b); b&=b-1; printf("   [%-17s] %s\n",NODENAME[FNODE[f]],FNAME[f]); }
  int fq=olaylar(BASE); printf("Kur'an olayları: A%d B%d C%d O3%d D%d E%d G%d W%d\n",!!(fq&EV_A),!!(fq&EV_B),!!(fq&EV_C),!!(fq&EV_O3),!!(fq&EV_D),!!(fq&EV_E),!!(fq&EV_G),!!(fq&EV_W));
  if(argc<4) return 0;
  char null=argv[2][0]; double par=strlen(argv[2])>1?atof(argv[2]+1):0; if(null=='L') par=q; uint64_t N=strtoull(argv[3],NULL,10);
  sampler_t *Z=malloc(sizeof(sampler_t)); double t0=now(); sampler_init(Z,null,par,12345); double t1=now();
  long cA=0,cB=0,cC=0,cO=0,cOA=0,cD=0,cG=0,cW=0,cK12=0; double sI=0;
  for(uint64_t it=0; it<N; it++){ sampler_next(Z); int f=olaylar(Z->a);
    if(f&EV_A) cA++; if(f&EV_B) cB++; if(f&EV_C) cC++; if(OK_O(f)){ cO++; if(f&EV_A) cOA++; } if(f&EV_D) cD++; if(f&EV_G) cG++; if(f&EV_W) cW++;
    if(null=='L' && it<20000) sI+=ters_kesin(Z->a); }
  double t2=now();
  printf("null %s N=%llu (%.2f s; %.0f ns/kitap, init %.2f s): A %.3e(%ld) B %.3e(%ld) C %.3e(%ld) O %.3e(%ld) O∧A %ld D %.2e(%ld) G %ld W %.3e(%ld)",argv[2],(unsigned long long)N,t2-t1,(t2-t1)/N*1e9,t1-t0,
    cA/(double)N,cA,cB/(double)N,cB,cC/(double)N,cC,cO/(double)N,cO,cOA,cD/(double)N,cD,cG,cW/(double)N,cW);
  if(null=='L') printf(" | ort. I (ilk 2e4) %.2f",sI/20000.0); printf("\n"); return 0; }
