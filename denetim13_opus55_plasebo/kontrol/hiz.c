// hiz — yalın değerlendirme hızı (tür 6 tohumları; yalnız süre basılır)
#include "../src/kutuphane.h"
#include <time.h>
int main(void){ kutuphane_init(); orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,'M',tohum('M',6,99)); static prof_t P; struct timespec t0,t1; clock_gettime(CLOCK_MONOTONIC,&t0);
  long N=200000; unsigned s=0; for(long b=0;b<N;b++){ orn_sonraki(Z); kdegerlendir(Z->a,&P,0x3FFF); s+=P.kF[b%NLENS]; } clock_gettime(CLOCK_MONOTONIC,&t1);
  printf("kitap başına değerlendirme (14 biçim, 406 mercek, M20 örnekleme dahil): %.1f µs (%u)\n",1e6*((t1.tv_sec-t0.tv_sec)+1e-9*(t1.tv_nsec-t0.tv_nsec))/N,s%2); return 0; }
