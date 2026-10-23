// hizli.h sınaması: (1) m20_emit akışı robotun sampler_next('M') akışıyla birebir mi; kF/Σc artımlı doğru mu.
// (2) lz_kitap dağılımı eski Mallows örnekleyicisiyle uyumlu mu (ortalama I, P(kF=12), P(B)); hız.
#include "../src/hizli.h"
#include <time.h>
static double now(){ struct timespec t; clock_gettime(CLOCK_MONOTONIC,&t); return t.tv_sec+1e-9*t.tv_nsec; }
int main(int argc, char **argv){
  int kod=atoi(argv[1]); init_ortak(kod); comp_hazirla(); long N=atol(argv[2]);
  // (1)
  sampler_t *Z=malloc(sizeof(sampler_t)); sampler_init(Z,'M',20,4242); m20_t *M=malloc(sizeof(m20_t)); m20_init(M,4242,20);
  long fark=0, kfark=0; for(long it=0; it<N; it++){ sampler_next(Z); int c=m20_emit(M); int *b=M->ch[c].b; if(memcmp(b,Z->a,sizeof(int)*114)) fark++;
    int kF=0,cs=0; for(int i=0;i<114;i++){ int cc=COMPC[i+1][b[i]]; if(cc){kF++;cs+=cc;} } if(kF!=M->ch[c].kF||cs!=M->ch[c].cs) kfark++; }
  printf("M20: %ld emisyon, robot akışından fark %ld, artımlı kF/Σc hatası %ld\n",N,fark,kfark);
  double t0=now(); long nO=0, nB=0; for(long it=0; it<N*20; it++){ int c=m20_emit(M); if(M->ch[c].kF==12&&M->ch[c].cs==76){ nB++; if(OK_O(olaylar(M->ch[c].b))) nO++; } } double t1=now();
  printf("M20 hız: %.1f ns/emisyon; B %ld, O %ld / %ld\n",(t1-t0)/(N*20)*1e9,nB,nO,N*20);
  // (2)
  ref_hazirla(); double q=mallows_q_ayarla(ters_kesin(BASE)); lz_t *L=malloc(sizeof(lz_t)); lz_init(L,q,99); int a[114];
  double sI=0; long hk12=0, hB=0, hO=0; long NL=N*10; t0=now();
  for(long it=0; it<NL; it++){ int pre=lz_kitap(L,a); if(it<200000) sI+=ters_kesin(a); int kF=0; for(int i=0;i<114;i++) if(COMPC[i+1][a[i]]) kF++; if(kF==12) hk12++; if(pre){ hB++; if(OK_O(olaylar(a))) hO++; } }
  t1=now(); printf("Mallows hızlı: %.1f ns/kitap (ters sayım ve kF yeniden sayımı dahil) | ort I (ilk 2e5) %.3f (hedef %ld) | P(kF=12) %.5f | P(B) %.4e (%ld) | O %ld / %ld\n",(t1-t0)/NL*1e9,sI/200000.0,ters_kesin(BASE),hk12/(double)NL,hB/(double)NL,hB,hO,NL);
  t0=now(); long hO2=0; for(long it=0; it<NL; it++){ if(lz_kitap(L,a)) if(OK_O(olaylar(a))) hO2++; } t1=now(); printf("Mallows salt hız: %.1f ns/kitap (O %ld)\n",(t1-t0)/NL*1e9,hO2);
  return (fark||kfark)?1:0; }
