// B = max_k Φ(k)/M! (Σk = K, bütün k'lerin tam taraması) ve kesin P(|F| = K) = Σ_k Nconf(k)·Φ(k)/114!.
// Kullanım: ./kosul_u_hazirla <kodeks> <K> <cikti>
#include "kosul_u.h"
static long double BEST=0, PKT=0; static long NL=0;
static void rec(const usamp_t *U, int r, int rem, int *k){
  if(r==19){ if(rem) return; NL++; long double f=phi_oran(U,k); if(f>BEST) BEST=f; long double nc=1; for(int s=0;s<19;s++) nc*=U->w[s][k[s]]; PKT+=nc*f; return; }
  int cap=0; for(int s=r;s<19;s++) cap+=(U->cnt[s]<6?U->cnt[s]:6); if(cap<rem) return;
  int mx=U->cnt[r]<6?U->cnt[r]:6; if(mx>rem) mx=rem; for(int x=0;x<=mx;x++){ k[r]=x; rec(U,r+1,rem-x,k); } k[r]=0; }
int main(int argc, char **argv){ int kod=atoi(argv[1]), K=atoi(argv[2]); init_ortak(kod); usamp_t *U=malloc(sizeof(usamp_t)); usamp_init(U,K);
  int k[19]={0}; rec(U,0,K,k); long double lr=0; for(int x=114-K+1;x<=114;x++) lr+=logl((long double)x);
  long double pk=PKT*expl(-lr);
  FILE *f=fopen(argv[3],"w"); fprintf(f,"%d %d %.18Lg\n# taranan k vektörü %ld; kesin P(|F|=%d) = %.12Lg\n",kod,K,BEST,NL,K,pk); fclose(f);
  printf("kodeks %d K=%d: %ld vektör, B=%.15Lg, P(|F|=K)=%.12Lg\n",kod,K,NL,BEST,pk); return 0; }
