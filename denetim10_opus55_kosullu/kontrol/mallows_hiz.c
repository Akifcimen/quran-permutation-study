// Hızlı kesin Mallows örnekleyici + aile ön-elemesi; P(kF=12), P(B), P(C), P(O), P(G) ve hız.
#include "../src/ortak.h"
#include <time.h>
static double now(){ struct timespec t; clock_gettime(CLOCK_MONOTONIC,&t); return t.tv_sec+1e-9*t.tv_nsec; }
int main(int argc, char **argv){
  int kod=atoi(argv[1]); init_ortak(kod); ref_hazirla(); long Iobs=ters_kesin(BASE); double q=mallows_q_ayarla(Iobs);
  uint64_t seed=strtoull(argv[2],NULL,10), N=strtoull(argv[3],NULL,10);
  sampler_t *Z=malloc(sizeof(sampler_t)); sampler_init(Z,'L',q,seed);
  static signed char comp[115][300]; for(int n=1;n<=114;n++) for(int v=0;v<300;v++) comp[n][v]=((n+v)%19==0)?(signed char)((n+v)/19):0;
  long hk[40]={0}, cB=0,cC=0,cO=0,cG=0,cD=0,cOA=0,cOW=0; int a[114];
  double t0=now();
  for(uint64_t it=0; it<N; it++){
    int rem[114]; for(int i=0;i<114;i++) rem[i]=Z->ref[113-i];
    int kF=0, cs=0;
    for(int p=0;p<114;p++){ int R=114-p; int V=alias_cek(Z,R); int ix=R-1-V; int v=rem[ix]; a[p]=v; memmove(rem+ix,rem+ix+1,sizeof(int)*(R-1-ix)); int c=comp[p+1][v]; if(c){ kF++; cs+=c; } }
    hk[kF<39?kF:39]++;
    if(kF==12 && cs==76){ cB++; int f=olaylar(a); if(f&EV_C) cC++; if(OK_O(f)){ cO++; if(f&EV_A) cOA++; if(f&EV_W) cOW++; } if(f&EV_D) cD++; if(f&EV_G) cG++; }
  }
  double t1=now();
  printf("kodeks %d seed %llu: N=%llu, %.0f ns/kitap | P(kF=12)=%.4e P(B)=%.4e(%ld) P(C)=%.4e(%ld) P(O)=%.4e(%ld) P(D)=%.3e(%ld) P(G)=%.3e(%ld) | O∧A %ld O∧W %ld\n",kod,(unsigned long long)seed,(unsigned long long)N,(t1-t0)/N*1e9,
    hk[12]/(double)N,cB/(double)N,cB,cC/(double)N,cC,cO/(double)N,cO,cD/(double)N,cD,cG/(double)N,cG,cOA,cOW);
  printf("  kF dağılımı:"); for(int k=0;k<25;k++) printf(" %d:%.4f",k,hk[k]/(double)N); printf("\n"); return 0; }
