// hiz — puanlayıcı ve örnekleyici hız ölçümü. Puan değerleri BASILMAZ (yalnız süre).
#include "../src/butunluk.h"
#include <time.h>
static double simdi(void){ struct timespec t; clock_gettime(CLOCK_MONOTONIC,&t); return t.tv_sec+1e-9*t.tv_nsec; }
int main(int argc,char**argv){
  butunluk_init(129); long N=atol(argv[1]); static profil_t P; volatile long sink=0;
  double t0=simdi(); for(long b=0;b<N;b++){ degerlendir(BASE,&P); sink+=sayim_puani(&P,0,0); } double t1=simdi();
  printf("Kur'an tekrarı: %.2f µs/kitap (değerlendirme + sayım)\n",1e6*(t1-t0)/N);
  const char *S="UMLJ"; for(int k=0;k<4;k++){ orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,S[k],12345);
    double a0=simdi(); for(long b=0;b<N;b++){ orn_sonraki(Z); sink+=Z->a[0]; } double a1=simdi();
    for(long b=0;b<N;b++){ orn_sonraki(Z); degerlendir(Z->a,&P); sink+=sayim_puani(&P,0,0); } double a2=simdi();
    printf("sıfır %c: örnekleyici %.3f µs/kitap; örnekleyici+değerlendirme+sayım %.2f µs/kitap\n",S[k],1e6*(a1-a0)/N,1e6*(a2-a1)/N); free(Z); }
  return 0; }
