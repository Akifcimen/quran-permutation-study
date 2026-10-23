// jz_kitap (iki çıktılı Box–Muller) ile robot J10'unun dağılım karşılaştırması: P(B), P(C), ortalama ters sayım, ortalama |kayma|; hız.
#include "../src/hizli.h"
#include <time.h>
static double now(){ struct timespec t; clock_gettime(CLOCK_MONOTONIC,&t); return t.tv_sec+1e-9*t.tv_nsec; }
int main(int argc,char **argv){ init_ortak(129); comp_hazirla(); long N=atol(argv[1]); jz_t *J=malloc(sizeof(jz_t)); jz_init(J,10,3); sampler_t *Z=malloc(sizeof(sampler_t)); sampler_init(Z,'J',10,3);
  int a[114]; long b1=0,b2=0,c1=0,c2=0; double i1=0,i2=0; double t0=now(); for(long k=0;k<N;k++){ if(jz_kitap(J,a)){ b1++; if(olaylar(a)&EV_C) c1++; } if(k<20000) i1+=ters_kesin(a); } double t1=now();
  for(long k=0;k<N;k++){ sampler_next(Z); int kF=0,cs=0; for(int i=0;i<114;i++){ int c=COMPC[i+1][Z->a[i]]; if(c){kF++;cs+=c;} } if(kF==12&&cs==76){ b2++; if(olaylar(Z->a)&EV_C) c2++; } if(k<20000) i2+=ters_kesin(Z->a); } double t2=now();
  printf("jz: %.0f ns, P(B)=%.4e (%ld) P(C)=%.3e (%ld) ort I=%.2f | robot J10: %.0f ns, P(B)=%.4e (%ld) P(C)=%.3e (%ld) ort I=%.2f\n",(t1-t0)/N*1e9,b1/(double)N,b1,c1/(double)N,c1,i1/20000,(t2-t1)/N*1e9,b2/(double)N,b2,c2/(double)N,c2,i2/20000); return 0; }
