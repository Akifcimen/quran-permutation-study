// isinma_S — M20 ısınma tanısı (ONKAYIT §2A): örnekleyiciyi sayaçsız çalıştırır. Her bölütün 250.000 örneğini sıraya göre on dilime ayırır;
// dilim başına S = Σ (i+1)·a_i toplamını ve B2 sayısını (en az bir biçimde terazi; kontrol_h'nin b2 alanının aynısı) yazar.
// Isınma uzunluğu kutuphane.h kopyasındaki ISINMA derleme parametresidir. Tohumlar kontrol_h ile aynı (tür 2).
// Kullanım: ./isinma_S <ilk_bolut> <son_bolut>   (iş parçacığı ESZAMANLI, ≤ 24) → stdout: "b <bolut> S <10 toplam> B2 <10 sayı>"
#include "kutuphane.h"
#include <pthread.h>
#define BN 250000
#define DILIM 10
static int B1_, B2_, SONRAKI; static pthread_mutex_t KILIT=PTHREAD_MUTEX_INITIALIZER;
// kontrol_h.c b2_bicimler'in aynısı (B2'nin tuttuğu biçim maskesi)
static inline uint32_t b2_bicimler(const int *a){ uint32_t m=0;
  for(int f=0;f<NFORM;f++){ const int al=AL[f], be=BE[f], abe=abs(be); int e0=0; long sn0=0, sa0=0, st0=0, st=0;
    for(int n=1;n<=114;n++){ const int x=a[n-1]; const int t=al*n+be*x; const int d1=t&1; st+=t; if(!d1){ e0++; sn0+=n; sa0+=x; st0+=t; } }
    const int e1=114-e0; const long sn1=SN_TOT-sn0, sa1=SA_TOT-sa0, st1=st-st0;
    if(e0>0 && e1>0 && ((long)al*sn0==(long)abe*sa1 || (long)al*sn1==(long)abe*sa0 || st0==st1)) m|=1u<<f; }
  return m; }
static void *isci(void *arg){ (void)arg; orn_t *Z=malloc(sizeof(orn_t));
  for(;;){ pthread_mutex_lock(&KILIT); int b=SONRAKI++; pthread_mutex_unlock(&KILIT); if(b>B2_) break;
    orn_init(Z,'M',tohum('M',2,b));                 // M yolu ortak durum yazmaz (REF_DESC yalnız L'de)
    long sS[DILIM]={0}, nB[DILIM]={0};
    for(int i=0;i<BN;i++){ orn_sonraki(Z); const int d=i/(BN/DILIM); long S=0;
      for(int q=0;q<114;q++) S+=(long)(q+1)*Z->a[q];
      sS[d]+=S; if(b2_bicimler(Z->a)) nB[d]++; }
    pthread_mutex_lock(&KILIT);
    printf("b %d S",b); for(int d=0;d<DILIM;d++) printf(" %ld",sS[d]); printf(" B2"); for(int d=0;d<DILIM;d++) printf(" %ld",nB[d]); printf("\n"); fflush(stdout);
    pthread_mutex_unlock(&KILIT); }
  free(Z); return NULL; }
int main(int argc, char **argv){
  if(argc<3){ fprintf(stderr,"kullanım: isinma_S <ilk_bolut> <son_bolut>\n"); return 1; }
  B1_=atoi(argv[1]); B2_=atoi(argv[2]); SONRAKI=B1_; kutuphane_init();
  int nt=atoi(getenv("ESZAMANLI")?getenv("ESZAMANLI"):"12"); if(nt<1) nt=1; if(nt>24) nt=24;
  fprintf(stderr,"isinma_S: ISINMA %d, bölüt %d–%d, %d iş parçacığı, S_obs %ld\n",ISINMA,B1_,B2_,nt,S_OBS);
  pthread_t th[24]; for(int t=0;t<nt;t++) pthread_create(&th[t],NULL,isci,NULL); for(int t=0;t<nt;t++) pthread_join(th[t],NULL);
  return 0; }
