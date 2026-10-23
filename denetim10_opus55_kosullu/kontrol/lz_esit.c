// Optimize lz_kitap'ın eski skaler sürümle aynı tohumda birebir aynı kitapları verdiğinin sınaması.
#include "../src/hizli.h"
static int eski(lz_t *Z, int *a){ int rem[114]; for(int i=0;i<114;i++) rem[i]=Z->ref[113-i]; int kF=0,cs=0; rng_t r=Z->r;
  for(int p=0;p<114;p++){ int R=114-p; uint64_t x=xnext(&r); int j=(int)(((x>>32)*(uint64_t)R)>>32); uint32_t u=(uint32_t)x; uint32_t th=Z->thr[R][j]; int V=(u<th||th==0xFFFFFFFFu)?j:Z->ali[R][j]; int ix=R-1-V; int v=rem[ix]; a[p]=v; for(int k=ix;k<R-1;k++) rem[k]=rem[k+1]; int c=COMPC[p+1][v]; if(c){kF++;cs+=c;} }
  Z->r=r; return (kF==12&&cs==76); }
int main(){ init_ortak(129); comp_hazirla(); ref_hazirla(); double q=mallows_q_ayarla(ters_kesin(BASE)); lz_t *A=malloc(sizeof(lz_t)), *B=malloc(sizeof(lz_t)); lz_init(A,q,11); lz_init(B,q,11);
  int a[114], b[114]; long fark=0; for(long i=0;i<2000000;i++){ int x=lz_kitap(A,a), y=eski(B,b); if(x!=y||memcmp(a,b,sizeof a)) fark++; } printf("2e6 kitapta fark: %ld\n",fark); return fark?1:0; }
