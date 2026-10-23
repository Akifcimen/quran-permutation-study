// Tekdüze kesin koşullu örnekleyicinin GPU çapalarına karşı sınanması (C = O1∧O2 koşullu kitaplar).
// GPU (127, 2,4e13): P(D|C)=3331136/444513933, P(E|C)=500965/…, P(G|C)=17141/…, P(W|C)=1827628/…, P(A|C)=155597/…,
// P(DW|C)=13537/…, P(GW|C)=62/… ; P(W|G)=62/17141. G-yapıları için TAU yeni tamamlama ile P(W|G) ayrıca.
// Kullanım: ./u_capa <kodeks> <seed> <N_C_kitap> <G_basina_tau> <cikti_prefix>
#include "../src/kosul_u.h"
#include <time.h>
static double now(){ struct timespec t; clock_gettime(CLOCK_MONOTONIC,&t); return t.tv_sec+1e-9*t.tv_nsec; }
int main(int argc, char **argv){
  int kod=atoi(argv[1]); init_ortak(kod); uint64_t seed=strtoull(argv[2],NULL,10); long NC=atol(argv[3]); int TAU=atoi(argv[4]);
  usamp_t *U=malloc(sizeof(usamp_t)); usamp_init(U,12); char bf[256]; snprintf(bf,256,"sonuc/u_B_%d_K12.txt",kod); usamp_B_oku(U,bf);
  rng_t rg; rseed(&rg,seed); ustat_t st; memset(&st,0,sizeof st);
  long nD=0,nE=0,nG=0,nW=0,nA=0,nDW=0,nGW=0,nO3=0,nAW=0; long gW=0,gT=0,gA=0,gAW=0; int a[114], fi[12], fl[12];
  double t2=now(); char fn[512]; snprintf(fn,512,"%s_ara.txt",argv[5]);
  for(long it=0; it<NC; it++){
    usamp_kitap(U,&rg,'C',a,fi,fl,&st); int f=olaylar(a);
    if(!(f&EV_C)){ printf("HATA: C tutmuyor\n"); return 1; }
    if(f&EV_D) nD++; if(f&EV_E) nE++; if(f&EV_A) nA++; if(f&EV_W) nW++; if((f&EV_D)&&(f&EV_W)) nDW++; if(f&EV_O3) nO3++; if((f&EV_A)&&(f&EV_W)) nAW++;
    if(f&EV_G){ nG++; if(f&EV_W) nGW++;
      for(int t=0;t<TAU;t++){ usamp_tau(U,&rg,fi,fl,a); int g=olaylar(a); if(!(g&EV_G)){ printf("HATA: G tutmuyor\n"); return 1; } gT++; if(g&EV_W) gW++; if(g&EV_A) gA++; if((g&EV_A)&&(g&EV_W)) gAW++; } }
    if((it+1)%1000000==0){ FILE *fp=fopen(fn,"w"); fprintf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld | %ld %ld %ld %ld\n",it+1,nO3,nD,nE,nG,nW,nA,nDW,nGW,nAW,gT,gW,gA,gAW); fclose(fp); }
  }
  double t3=now();
  snprintf(fn,512,"%s.txt",argv[5]); FILE *fo=fopen(fn,"w");
  fprintf(fo,"kodeks %d seed %llu: C-kitap %ld (%.0f s, %.1f µs/kitap) | öneri %ld, koşul geçen %ld, Φ kabul %ld (oran %.4f), τ deneme ort %.1f, Φ/M! maks %.8Lg ≤ B %.8Lg, ihlal %ld\n",
    kod,(unsigned long long)seed,NC,t3-t2,(t3-t2)/NC*1e6,st.oneri,st.kosul_gecen,st.phi_kabul,(double)st.phi_kabul/st.kosul_gecen,(double)st.tau_deneme/NC,st.phi_maks_gorulen,U->B,st.bihlal);
  fprintf(fo,"SAY N=%ld O3=%ld D=%ld E=%ld G=%ld W=%ld A=%ld DW=%ld GW=%ld AW=%ld | tauG=%ld tauGW=%ld tauGA=%ld tauGAW=%ld\n",NC,nO3,nD,nE,nG,nW,nA,nDW,nGW,nAW,gT,gW,gA,gAW);
  fclose(fo); return 0; }
