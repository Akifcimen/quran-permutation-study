// duz.c — profil olaylarının sıklığının düz histogram (Wang–Landau, 1/t çizelgesi) ve ardından sabit ağırlıklı çok kanonik ölçümle kestirimi.
// Bir olay E = kapı Gc ∧ y_1 ∧ … ∧ y_m (olayın merceğinde). D(a) = olayın tutmayan koşul sayısı (0 … m+1). E = {D = 0}.
// Zincir sıfırın dağılımı π'yi hedefler ama D üzerinde 1/g(D) ile ağırlıklanır: kabul min(1, π(a')/π(a) · g(D)/g(D')). g yakınsayınca
// g(D) ∝ π(D = D); P(E) = g(0) / Σ_D g(D). Şartlar zinciri sınırlamaz; zincir bütün düzeyler arasında serbestçe dolaşır.
//   U: π tekdüze. M: π, S = Σ n·a_n ≤ S_Kur'an üzerinde tekdüze (M20 zincirinin kuralı). L: π ∝ q^I(a) (Mallows örnekleyicisinin q'su).
// Öneri: üç simetrik çekirdeğin sabit ağırlıklı karışımı (tekdüze iki konum; p'ye ve 2p'ye göre aynı kalanlı iki konum).
// Aşama 1 (WL): ln g güncellenir; 1/t çizelgesi (Belardinelli–Pereyra: görülmüş her düzey son sıfırlamadan beri en az bir kez ziyaret
// edilince ln f yarılanır; ln f < düzey/t olunca ln f = düzey/t). Aşama 2 (ölçüm): ln g sabit, ziyaret histogramı H toplanır;
// ln π(D) = ln g(D) + ln H(D) (yeniden ağırlıklama). Kestirim aşama 2'den gelir.
// Kullanım: ./duz <sifir> <olaylar.txt> <secim.txt> <wl_adım> <ölçüm_adım> <tekrar> <iş_parçacığı> <çıktı> [ilk_tekrar]
//   wl_adım "k:<c>:<en_az>" biçimindeyse olay başına W = max(en_az, c·düzey²), ölçüm = W/2 (ölçüm_adım yok sayılır).
// Tohum: tohum(s, 9, 16·olay_no + r), r = 1 … tekrar; olay_no olaylar.txt'deki satır sırası.
#include "kutuphane.h"
#include "tek_mercek.h"
#include <pthread.h>
#include <math.h>
#include <stdatomic.h>
#define KMAX 32
typedef struct { char ad[24]; int no, L, nc; int g[KMAX]; } olay_t;
typedef struct { int e, r; } is_t;
static char SIF; static double QQ[512], LQ; static olay_t *EV; static int NE; static is_t *IS; static int NIS; static atomic_int SIRA;
static long WL_, OL_; static double BC_=0; static long BMIN_=0; static FILE *CIK; static pthread_mutex_t KILIT=PTHREAD_MUTEX_INITIALIZER, KILIT_ORN=PTHREAD_MUTEX_INITIALIZER;   // KILIT_ORN: orn_init (Mallows) ortak REF_DESC'i yerinde sıralar; yarışı önler
static int bozuk(const olay_t *E, const int *a, prof_t *P){
  kdeg_tek(a, P, E->L); const int L=E->L; int d=0;
  if(!kapiG(P,L)) d++;
  const uint64_t pm[5]={P->B[L],P->Z[L],P->F[L],P->W[L],P->H[L]};
  for(int c=1;c<E->nc;c++){ const int k=E->g[c]>>6, b=E->g[c]&63; if(!(pm[k]>>b&1)) d++; }
  return d; }
static long Sof(const int *a){ long S=0; for(int i=0;i<114;i++) S+=(long)(i+1)*a[i]; return S; }
static int cift_sec(rng_t *r, const int *a, int m, int *pi, int *pj){
  int cnt[256]={0}; for(int i=0;i<114;i++) cnt[a[i]%m]++;
  long P=0; for(int c=0;c<m;c++) P+=(long)cnt[c]*(cnt[c]-1)/2;
  if(P==0) return 0;
  long k=(long)bounded(r,(uint32_t)P); int c=0; for(;c<m;c++){ const long pc=(long)cnt[c]*(cnt[c]-1)/2; if(k<pc) break; k-=pc; }
  int pos[114], n=0; for(int i=0;i<114;i++) if(a[i]%m==c) pos[n++]=i;
  int x=0; while(k>=n-1-x){ k-=n-1-x; x++; } *pi=pos[x]; *pj=pos[x+1+(int)k]; return 1; }
// Bir öneri, ln g ile ağırlıklı kabul. Döner: yeni D.
static int adim(rng_t *r, int *a, long *S, int D, const double *lg, const olay_t *E, prof_t *P){
  int i, j; const int tur=(int)bounded(r,3), p=PR[E->L%NPR];
  if(tur==0){ i=(int)bounded(r,114); j=(int)bounded(r,113); if(j>=i) j++; }
  else if(!cift_sec(r,a,tur==1?p:2*p,&i,&j)) return D;
  if(a[i]==a[j]) return D;
  if(i>j){ int t=i; i=j; j=t; }
  const int x=a[i], y=a[j]; long dS=0; double lr=0;
  if(SIF=='M'){ dS=(long)((i+1)-(j+1))*(y-x); if(*S+dS>S_OBS) return D; }
  else if(SIF=='L'){ int dI=(y<x)-(x<y); for(int k=i+1;k<j;k++){ const int z=a[k]; dI+=(y<z)-(x<z)+(z<x)-(z<y); } lr=dI*LQ; }   // π(a')/π(a) = q^ΔI, her iki işarette (ağırlıklı kabulde oranın tamamı gerekir)
  a[i]=y; a[j]=x; const int D2=bozuk(E,a,P);
  const double l=lr+lg[D]-lg[D2];
  if(l<0 && log(unif(r))>=l){ a[i]=x; a[j]=y; return D; }
  *S+=dS; return D2; }
static double logsum(const double *v, int n){ double m=-1e300; for(int i=0;i<n;i++) if(v[i]>m) m=v[i]; double s=0; for(int i=0;i<n;i++) s+=exp(v[i]-m); return m+log(s); }
static void *isci(void *arg){
  (void)arg; static __thread prof_t P; orn_t *Z=malloc(sizeof(orn_t));
  for(;;){ const int q=atomic_fetch_add(&SIRA,1); if(q>=NIS) break; const olay_t *E=&EV[IS[q].e]; const int rep=IS[q].r, nl=E->nc+1;
    long WL=WL_, OL=OL_; if(BC_>0){ WL=(long)(BC_*nl*nl); if(WL<BMIN_) WL=BMIN_; OL=WL/2; }
    const uint64_t sd=tohum(SIF,9,16L*E->no+rep); pthread_mutex_lock(&KILIT_ORN); orn_init(Z,SIF,sd); pthread_mutex_unlock(&KILIT_ORN); orn_sonraki(Z); rng_t rg; rseed(&rg,sd^0x5DEECE66DULL);
    int a[114]; memcpy(a,Z->a,sizeof a); long S=Sof(a); int D=bozuk(E,a,&P);
    double lg[KMAX+2]={0}; long H[KMAX+2]={0}, gor[KMAX+2]={0}; double lf=1.0; long t=0, son=0; int asama=0; int min_d=D;
    // Aşama 1: WL (1/t). Düzeyler: şimdiye kadar görülmüş olanlar; D = 0 görülmeden yarılama yapılmaz.
    for(t=1;t<=WL;t++){ D=adim(&rg,a,&S,D,lg,E,&P); lg[D]+=lf; H[D]++; if(!gor[D]){ gor[D]=1; son=t; } if(D<min_d) min_d=D;
      if(asama==0 && (t&1023)==0 && gor[0]){ int tamam=1; for(int d=0;d<nl;d++) if(gor[d] && !H[d]){ tamam=0; break; }
        if(tamam){ lf/=2; memset(H,0,sizeof H); if(lf<(double)nl/t) asama=1; } }
      if(asama==1) lf=(double)nl/t; }
    // Aşama 2: sabit ağırlık, ziyaret histogramı
    long V[KMAX+2]={0}; for(long u=0;u<OL;u++){ D=adim(&rg,a,&S,D,lg,E,&P); V[D]++; }
    double lp[KMAX+2]; int nv=0; for(int d=0;d<nl;d++){ lp[d]=V[d]?lg[d]+log((double)V[d]):-1e300; if(V[d]) nv++; }
    const double ltop=logsum(lp,nl); const double p0=V[0]?exp(lp[0]-ltop):0.0;
    pthread_mutex_lock(&KILIT);
    fprintf(CIK,"%s r %d p %.6e lnf %.2e asama %d en_az_D %d adim %ld | V",E->ad,rep,p0,lf,asama,min_d,WL+OL); for(int d=0;d<nl;d++) fprintf(CIK," %ld",V[d]);
    fprintf(CIK," | lnpi"); for(int d=0;d<nl;d++) fprintf(CIK," %.3f",V[d]?lp[d]-ltop:-999.0); fprintf(CIK,"\n"); fflush(CIK);
    pthread_mutex_unlock(&KILIT); }
  return NULL; }
int main(int argc,char**argv){
  if(argc<9){ fprintf(stderr,"kullanım: duz <sifir> <olaylar> <secim> <wl_adım> <ölçüm_adım> <tekrar> <iş> <çıktı>\n"); return 1; }
  SIF=argv[1][0]; if(strchr(argv[4],':')){ if(sscanf(argv[4],"k:%lf:%ld",&BC_,&BMIN_)!=2) return 1; } else { WL_=atol(argv[4]); OL_=atol(argv[5]); }
  const int TEK=atoi(argv[6]), NT=atoi(argv[7]), R0=argc>9?atoi(argv[9]):1; kutuphane_init();
  if(SIF=='L'){ orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,'L',1); for(int d=0;d<512;d++) QQ[d]=pow(Z->q,d); LQ=log(Z->q); free(Z); }
  else if(SIF!='M' && SIF!='U'){ fprintf(stderr,"sıfır?\n"); return 2; }
  int cap=1<<16; char (*TUM)[24]=calloc(cap,24); int *TL=calloc(cap,sizeof(int)); uint64_t (*TM)[5]=calloc(cap,40); int nt=0;
  { FILE *f=fopen(argv[2],"r"); if(!f){ perror(argv[2]); return 2; } char s[4096];
    while(fgets(s,sizeof s,f)){ char a[64]; int h,L; unsigned long long m[5]; if(sscanf(s,"%63s %d %d %llx %llx %llx %llx %llx",a,&h,&L,&m[0],&m[1],&m[2],&m[3],&m[4])!=8) return 3;
      if(nt==cap) return 4; snprintf(TUM[nt],24,"%s",a); TL[nt]=L; for(int k=0;k<5;k++) TM[nt][k]=m[k]; nt++; } fclose(f); }
  EV=calloc(cap,sizeof(olay_t));
  { FILE *f=fopen(argv[3],"r"); if(!f){ perror(argv[3]); return 2; } char a[64];
    while(fscanf(f,"%63s",a)==1){ int i=0; while(i<nt && strcmp(TUM[i],a)) i++; if(i==nt){ fprintf(stderr,"olay yok: %s\n",a); return 5; }
      olay_t *E=&EV[NE++]; snprintf(E->ad,24,"%s",a); E->no=i; E->L=TL[i]; E->nc=1; E->g[0]=-1;
      for(int k=0;k<5;k++) for(int b=0;b<64;b++) if(TM[i][k]>>b&1){ if(E->nc>=KMAX) return 6; E->g[E->nc++]=64*k+b; } } fclose(f); }
  IS=malloc(sizeof(is_t)*(size_t)NE*TEK); for(int r=R0;r<R0+TEK;r++) for(int e=0;e<NE;e++){ IS[NIS].e=e; IS[NIS].r=r; NIS++; }
  CIK=fopen(argv[8],"a"); if(!CIK){ perror(argv[8]); return 2; }
  fprintf(CIK,"# duz sıfır %c wl %s ölçüm %ld tekrar %d (ilk %d) olay %d\n",SIF,argv[4],OL_,TEK,R0,NE); fflush(CIK);
  pthread_t th[64]; for(int t=0;t<NT;t++) pthread_create(&th[t],NULL,isci,NULL); for(int t=0;t<NT;t++) pthread_join(th[t],NULL);
  fclose(CIK); return 0; }
