// kosul_u.h — tekdüze sıfırda KESİN koşullu örnekleyici (MCMC yok, ağırlık yok, çıktılar bağımsız).
// Etiketli permütasyon tekdüze ⇔ (yapı γ: aile konumları + onlara giden etiketli değerler) ve (tamamlama τ: kalan
// değerlerin kalan konumlara YENİ aile üyesi oluşturmadan dağıtımı). P(γ) ∝ Φ(k(γ)), Φ = geçerli tamamlama sayısı;
// Φ yalnız sınıf-sayısı vektörü k'ye bağlıdır (yasak tahta 19 ayrık tam iki parçalı bloktur → kale polinomu çarpımı).
// Öneri: k ~ Π_r w_r(k_r) (Σk = K), w_r(k) = C(6,k)·C(c_r,k)·k!, sınıf içinde düzgün seçim → K sabitken her γ eşit olası.
// Koşul (B/C/O) γ'de sınanır; geçerse γ, Φ(k)/(M!·B) olasılıkla kabul edilir (B = max_k Φ/M!, Σk=K bütün k'lerin TAM
// taramasıyla; kosul_u_hazirla). Sonra τ, geçerli tamamlamalar arasından düzgün seçilir (ret; erken kesme).
// Sonuç: her çıktı P(γ,τ | koşul, tekdüze)'den bağımsız ve tam dağılımlı bir çekiliştir.
#ifndef KOSUL_U_H
#define KOSUL_U_H
#include "ortak.h"
#define KMAX_SINIF 7
typedef struct {
  int K;
  int cnt[19];                // değer sınıfı sayıları c_r (r = a mod 19)
  int vals[19][40];           // sınıftaki etiketli değerler (BASE indeksleri)
  int pos[19][6];             // sınıftaki konumlar (0 tabanlı i; n=i+1 ≡ −r mod 19)
  double w[19][KMAX_SINIF];
  double D[20][130];
  long double B;
} usamp_t;
static long double binom_ld(int n,int k){ if(k<0||k>n) return 0; long double r=1; for(int i=1;i<=k;i++) r=r*(n-k+i)/i; return r; }
static long double phi_oran(const usamp_t *U, const int *k){   // Φ(k)/M!, M = 114 − K
  long double P[130]; int deg=0; P[0]=1;
  for(int r=0;r<19;r++){ int p=6-k[r], v=U->cnt[r]-k[r]; int mn=p<v?p:v; if(mn<=0) continue;
    long double Rr[8]; for(int i=0;i<=mn;i++){ Rr[i]=binom_ld(p,i)*binom_ld(v,i); for(int t=2;t<=i;t++) Rr[i]*=t; }
    long double Q[130]; for(int j=0;j<=deg+mn;j++) Q[j]=0; for(int a=0;a<=deg;a++) for(int b=0;b<=mn;b++) Q[a+b]+=P[a]*Rr[b];
    deg+=mn; for(int j=0;j<=deg;j++) P[j]=Q[j]; }
  int M=114-U->K; long double s=0, ff=1;
  for(int j=0;j<=deg;j++){ if(j>0) ff*=(M-j+1); long double term=P[j]/ff; s+=(j&1)?-term:term; }
  return s; }
static void usamp_init(usamp_t *U, int K){
  memset(U,0,sizeof *U); U->K=K;
  for(int i=0;i<114;i++){ int r=BASE[i]%19; U->vals[r][U->cnt[r]++]=i; }
  int pc[19]={0}; for(int i=0;i<114;i++){ int n=i+1; int r=(19-n%19)%19; U->pos[r][pc[r]++]=i; }
  for(int r=0;r<19;r++) if(pc[r]!=6){ fprintf(stderr,"konum sınıfı hatası\n"); exit(1); }
  for(int r=0;r<19;r++) for(int k=0;k<KMAX_SINIF;k++){ if(k>6||k>U->cnt[r]){ U->w[r][k]=0; continue; } double x=(double)(binom_ld(6,k)*binom_ld(U->cnt[r],k)); for(int t=2;t<=k;t++) x*=t; U->w[r][k]=x; }
  for(int s=0;s<130;s++) U->D[19][s]=(s==0); for(int r=18;r>=0;r--) for(int s=0;s<130;s++){ double a=0; for(int k=0;k<KMAX_SINIF&&k<=s;k++) a+=U->w[r][k]*U->D[r+1][s-k]; U->D[r][s]=a; }
}
static void usamp_B_oku(usamp_t *U, const char *dosya){ FILE *f=fopen(dosya,"r"); if(!f){ fprintf(stderr,"B dosyası yok: %s\n",dosya); exit(1); }
  int kod,K; long double b; if(fscanf(f,"%d %d %Lg",&kod,&K,&b)!=3||kod!=KODEKS||K!=U->K){ fprintf(stderr,"B dosyası uyumsuz\n"); exit(1); } fclose(f); U->B=b*(1.0L+1e-12L); }
static void usamp_k(const usamp_t *U, rng_t *rg, int *k){ int s=U->K;
  for(int r=0;r<19;r++){ double tot=U->D[r][s]; double u=unif(rg)*tot, acc=0; int x=-1, last=-1;
    for(int y=0;y<KMAX_SINIF&&y<=s;y++){ double wy=U->w[r][y]*U->D[r+1][s-y]; if(wy>0) last=y; acc+=wy; if(x<0&&u<acc){ x=y; } }
    if(x<0) x=last; k[r]=x; s-=x; } }
static void usamp_gamma(const usamp_t *U, rng_t *rg, const int *k, int *fi, int *fl){
  int m=0; for(int r=0;r<19;r++){ int kr=k[r]; if(!kr) continue; int pp[6]; memcpy(pp,U->pos[r],sizeof pp); int vv[40]; int c=U->cnt[r]; memcpy(vv,U->vals[r],sizeof(int)*c);
    for(int j=0;j<kr;j++){ int a=j+(int)bounded(rg,(uint32_t)(6-j)); int t=pp[j]; pp[j]=pp[a]; pp[a]=t; int b=j+(int)bounded(rg,(uint32_t)(c-j)); t=vv[j]; vv[j]=vv[b]; vv[b]=t; fi[m]=pp[j]; fl[m]=vv[j]; m++; } } }
// γ koşulu: 'B' = O1, 'C' = O1∧O2, 'O' = O1∧O2∧O3
static inline int gamma_kosul(const int *fi, const int *fl, int K, char kos){
  int cs=0, ce=0, ne=0, c2=0, c3=0; for(int j=0;j<K;j++){ int n=fi[j]+1; int t=n+BASE[fl[j]]; int c=t/19; cs+=c; if(!(t&1)){ ne++; ce+=c; } else { if(n&1) c3+=c; else c2+=c; } }
  if(cs!=76) return 0; if(kos=='B') return 1; if(ne!=6||ce!=38) return 0; if(kos=='C') return 1; return (c2==19&&c3==19); }
static long usamp_tau(const usamp_t *U, rng_t *rg, const int *fi, const int *fl, int *a){
  int up[114]={0}, uv[114]={0}; for(int j=0;j<U->K;j++){ up[fi[j]]=1; uv[fl[j]]=1; a[fi[j]]=BASE[fl[j]]; }
  int P[114], V[114], np=0, nv=0; for(int i=0;i<114;i++){ if(!up[i]) P[np++]=i; if(!uv[i]) V[nv++]=BASE[i]; }
  long deneme=0;
  for(;;){ deneme++; int ok=1;
    for(int i=0;i<nv;i++){ int j=i+(int)bounded(rg,(uint32_t)(nv-i)); int t=V[i]; V[i]=V[j]; V[j]=t; if((P[i]+1+V[i])%19==0){ ok=0; break; } }
    if(ok) break; }
  for(int i=0;i<np;i++) a[P[i]]=V[i];
  return deneme; }
typedef struct { long oneri, kosul_gecen, phi_kabul, tau_deneme, bihlal; long double phi_maks_gorulen; } ustat_t;
// Yalnız γ (aile) çek: koşul + Φ kabulü. fi/fl döner.
static void usamp_gamma_kosullu(const usamp_t *U, rng_t *rg, char kos, int *fi, int *fl, ustat_t *st){
  int k[19];
  for(;;){ usamp_k(U,rg,k); usamp_gamma(U,rg,k,fi,fl); st->oneri++;
    if(!gamma_kosul(fi,fl,U->K,kos)) continue; st->kosul_gecen++;
    long double f=phi_oran(U,k); if(f>st->phi_maks_gorulen) st->phi_maks_gorulen=f; if(f>U->B) st->bihlal++;
    if(unif(rg)*U->B < f){ st->phi_kabul++; return; } } }
static void usamp_kitap(const usamp_t *U, rng_t *rg, char kos, int *a, int *fi, int *fl, ustat_t *st){
  usamp_gamma_kosullu(U,rg,kos,fi,fl,st); st->tau_deneme+=usamp_tau(U,rg,fi,fl,a); }
#endif
