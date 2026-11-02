// tek_sina.c — kdeg_tek (tek_mercek.h) ile özgün kdegerlendir'in bayt bayt eşdeğerlik sınaması. Her sıfırdan N dizilim (tohum türü 4)
// ve Kur'an; her dizilimde rastgele bir mercek, her 50. dizilimde ve Kur'an'da bütün 406 mercek. Karşılaştırılan: B, Z, F, W, H, kF, kH, mc.
// Kullanım: ./tek_sina <N>
#include "kutuphane.h"
#include "tek_mercek.h"
static long fark=0, sinama=0;
static void kars(const int *a, int L){ static prof_t P1, P2; kdegerlendir(a,&P1,1u<<(L/NPR)); memset(&P2,0xA5,sizeof P2); kdeg_tek(a,&P2,L); sinama++;
  if(P1.B[L]!=P2.B[L]||P1.Z[L]!=P2.Z[L]||P1.F[L]!=P2.F[L]||P1.W[L]!=P2.W[L]||P1.H[L]!=P2.H[L]||P1.kF[L]!=P2.kF[L]||P1.kH[L]!=P2.kH[L]||P1.mc[L]!=P2.mc[L]){
    if(fark<5) fprintf(stderr,"FARK mercek %d\n",L); fark++; } }
int main(int argc,char**argv){ const long N=atol(argv[1]); kutuphane_init();
  for(int L=0;L<NLENS;L++) kars(BASE,L);
  const char S[3]={'U','M','L'};
  for(int s=0;s<3;s++){ orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,S[s],tohum(S[s],4,777)); rng_t r; rseed(&r,tohum(S[s],4,778));
    for(long i=0;i<N;i++){ orn_sonraki(Z); kars(Z->a,(int)bounded(&r,NLENS)); if(i%50==0) for(int L=0;L<NLENS;L++) kars(Z->a,L); } free(Z); }
  printf("sınama %ld, fark %ld\n",sinama,fark); return fark!=0; }
