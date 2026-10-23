// usamp_k önerisinin sınanması: k_r marjinalleri kesin DP değerleriyle (∝ Π w) ve Φ-kabullü dağılımın kesin
// marjinalleriyle (∝ Π w · Φ; tam tarama) karşılaştırılır. Ayrıca sınıf içi konum paritesi düzgünlüğü.
#include "../src/kosul_u.h"
static long double EX0[19][7], EX1[19][7], Z0=0, Z1=0;
static void rec(const usamp_t *U, int r, int rem, int *k){
  if(r==19){ if(rem) return; long double nc=1; for(int s=0;s<19;s++) nc*=U->w[s][k[s]]; long double f=phi_oran(U,k); Z0+=nc; Z1+=nc*f; for(int s=0;s<19;s++){ EX0[s][k[s]]+=nc; EX1[s][k[s]]+=nc*f; } return; }
  int cap=0; for(int s=r;s<19;s++) cap+=(U->cnt[s]<6?U->cnt[s]:6); if(cap<rem) return;
  int mx=U->cnt[r]<6?U->cnt[r]:6; if(mx>rem) mx=rem; for(int x=0;x<=mx;x++){ k[r]=x; rec(U,r+1,rem-x,k); } k[r]=0; }
int main(int argc,char **argv){ int kod=atoi(argv[1]); long N=atol(argv[2]); init_ortak(kod); usamp_t *U=malloc(sizeof(usamp_t)); usamp_init(U,12); char bf[256]; snprintf(bf,256,"sonuc/u_B_%d_K12.txt",kod); usamp_B_oku(U,bf);
  int k[19]={0}; rec(U,0,12,k);
  rng_t rg; rseed(&rg,77); static long H0[19][7], H1[19][7]; long n1=0; long par[2]={0,0};
  for(long it=0; it<N; it++){ usamp_k(U,&rg,k); for(int s=0;s<19;s++) H0[s][k[s]]++;
    long double f=phi_oran(U,k); if(unif(&rg)*U->B<f){ n1++; for(int s=0;s<19;s++) H1[s][k[s]]++; }
    int fi[12],fl[12]; usamp_gamma(U,&rg,k,fi,fl); for(int j=0;j<12;j++) par[(fi[j]+1)&1]++; }
  double chi0=0, chi1=0; int df=0;
  for(int s=0;s<19;s++) for(int x=0;x<7;x++){ double e0=N*(double)(EX0[s][x]/Z0), e1=n1*(double)(EX1[s][x]/Z1); if(e0>20){ chi0+=(H0[s][x]-e0)*(H0[s][x]-e0)/e0; df++; } if(e1>20) chi1+=(H1[s][x]-e1)*(H1[s][x]-e1)/e1; }
  printf("öneri k-marjinalleri: χ²=%.1f (≈%d hücre) | Φ-kabullü: χ²=%.1f (kabul %ld/%ld)\n",chi0,df,chi1,n1,N);
  printf("örnek: sınıf 0: kesin öneri "); for(int x=0;x<5;x++) printf("%.5f ",(double)(EX0[0][x]/Z0)); printf("| görülen "); for(int x=0;x<5;x++) printf("%.5f ",H0[0][x]/(double)N); printf("\n");
  printf("aile konumu paritesi: tek n %ld, çift n %ld (beklenen ~eşit: her sınıfta 3/3)\n",par[1],par[0]); return 0; }
