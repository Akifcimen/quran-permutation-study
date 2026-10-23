#include "../src/hizli.h"
#include <time.h>
static double now(){ struct timespec t; clock_gettime(CLOCK_MONOTONIC,&t); return t.tv_sec+1e-9*t.tv_nsec; }
int main(){ init_ortak(129); comp_hazirla(); ref_hazirla(); double q=mallows_q_ayarla(ters_kesin(BASE)); lz_t *L=malloc(sizeof(lz_t)); lz_init(L,q,5); int a[114];
  long n=0,b=0; double t0=now(); while(now()-t0<8){ for(int i=0;i<100000;i++){ b+=lz_kitap(L,a); n++; } } double t1=now(); printf("Mallows: %.0f ns/örnek (%ld örnek, B-ön-eleme %ld)\n",(t1-t0)/n*1e9,n,b);
  sampler_t *Z=malloc(sizeof(sampler_t)); sampler_init(Z,'J',10,5); n=0; t0=now(); while(now()-t0<5){ for(int i=0;i<10000;i++){ sampler_next(Z); n++; } } t1=now(); printf("J10: %.0f ns/örnek\n",(t1-t0)/n*1e9);
  m20_t *M=malloc(sizeof(m20_t)); m20_init(M,5,20); n=0; t0=now(); while(now()-t0<5){ for(int i=0;i<100000;i++){ m20_emit(M); n++; } } t1=now(); printf("M20: %.0f ns/emisyon\n",(t1-t0)/n*1e9); return 0; }
