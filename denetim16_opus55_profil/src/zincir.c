// zincir.c — profil olaylarının sıklığının çok düzeyli koşullu kestirimi (alt küme benzetimi; ONKAYIT_EK).
// Bir olay E = kapı Gc ∧ y_1 ∧ … ∧ y_m (olayın merceğinde). P(E) = Π_j P(K_j | K_1 … K_{j−1}); K'ler olayın koşullarıdır (kapı ve yüklemler).
// Düzey 0: sıfırın kendi örnekleyicisinden (orn_sonraki) N bağımsız kitap. Düzey j: önceki düzeyde K_1…K_j'yi taşıyan kitaplardan başlayan
// Metropolis zincirleri; hedef, sıfırın dağılımının {K_1 … K_j} ile sınırlanmışıdır. Öneri: iki konumun yer değiştirmesi (simetrik).
//   U (tekdüze): bütün dizilimler eşit olasılıklı.            Kabul: koşullar.
//   M (M20):     S = Σ n·a_n ≤ S_Kur'an olan dizilimler eşit.  Kabul: S' ≤ S_Kur'an ve koşullar (M20 zincirinin kendi kuralı).
//   L (Mallows): π(a) ∝ q^I(a), I = #{i<j: a_i < a_j}, q örnekleyicininki.  Kabul: min(1, q^ΔI) ve koşullar.
// Profil, özgün kutuphane.h yoluyla (kdegerlendir, kapiG; yalnız olayın biçimi) hesaplanır.
// Kullanım: ./zincir <sifir> <olaylar.txt> <secim.txt> <pilot|olcum> <N> <tekrar> <iş_parçacığı> <çıktı> [pilot_çıktısı]
//   pilot: koşulların sırası (her düzeyde, örneklerde en sık tutan kalan koşul) ve her düzeyin zincir adım sayısı T_j belirlenir
//          (kabul oranından: T_j = ⌈4/kabul⌉, 5…ZTMAX). Pilotun kestirimi raporlanır ama okumada kullanılmaz.
//   olcum: pilotun sırası ve T_j'leriyle, birbirinden bağımsız tohumlarla <tekrar> kestirim.
// Tohum: tohum(s, 9, 16·olay_no + r); r = 0 pilot, 1… ölçüm; olay_no olaylar.txt'deki satır sırası (0'dan).
#include "kutuphane.h"
#include <pthread.h>
#include <math.h>
#include <stdatomic.h>
#ifndef ZTMAX
#define ZTMAX 3000
#endif
#define KMAX 32
typedef struct { char ad[24]; int no, L, nc; int g[KMAX]; } olay_t;   // g[0] = kapı (−1); g[c] = 64·grup + bit
typedef struct { int e, r; } is_t;
typedef struct { int ord[KMAX], T[KMAX]; int ok; } plan_t;
static char SIF; static double QQ[512]; static olay_t *EV; static int NE; static is_t *IS; static int NIS; static atomic_int SIRA;
static plan_t *PLAN; static int N_, MOD_OLCUM; static FILE *CIK; static pthread_mutex_t KILIT=PTHREAD_MUTEX_INITIALIZER;
static uint64_t kosul(const olay_t *E, const int *a, prof_t *P){
  kdegerlendir(a, P, 1u << (E->L / NPR)); const int L=E->L; uint64_t r=0;
  if(kapiG(P,L)) r|=1;
  const uint64_t pm[5]={P->B[L],P->Z[L],P->F[L],P->W[L],P->H[L]};
  for(int c=1;c<E->nc;c++){ const int k=E->g[c]>>6, b=E->g[c]&63; if(pm[k]>>b&1) r|=1ull<<c; }
  return r; }
static long Sof(const int *a){ long S=0; for(int i=0;i<114;i++) S+=(long)(i+1)*a[i]; return S; }
// Kalan sınıfı içi çift: a_i ≡ a_j (mod m) olan bütün çiftler arasından tekdüze. Bu sınıflar böyle bir yer değiştirmeyle değişmez,
// dolayısıyla öneri simetriktir (a → a' ile a' → a aynı olasılıkta).
static int cift_sec(rng_t *r, const int *a, int m, int *pi, int *pj){
  int cnt[256]={0}; for(int i=0;i<114;i++) cnt[a[i]%m]++;
  long P=0; for(int c=0;c<m;c++) P+=(long)cnt[c]*(cnt[c]-1)/2;
  if(P==0) return 0;
  long k=(long)bounded(r,(uint32_t)P); int c=0; for(;c<m;c++){ const long pc=(long)cnt[c]*(cnt[c]-1)/2; if(k<pc) break; k-=pc; }
  int pos[114], n=0; for(int i=0;i<114;i++) if(a[i]%m==c) pos[n++]=i;
  int x=0; while(k>=n-1-x){ k-=n-1-x; x++; } *pi=pos[x]; *pj=pos[x+1+(int)k]; return 1; }
// Bir öneri; durum değiştiyse 1. Öneri, sabit ağırlıklı üç simetrik çekirdeğin karışımıdır: (0) tekdüze iki konum; (1) değerleri p'ye göre
// aynı kalanlı iki konum (aileler F ve H korunur, bölümler kayar); (2) 2p'ye göre aynı kalanlı iki konum (ayrıca parite sınıfları korunur).
static int adim(rng_t *r, int *a, long *S, uint64_t *cb, const olay_t *E, uint64_t R, prof_t *P){
  int i, j; const int tur=(int)bounded(r,3), p=PR[E->L%NPR];
  if(tur==0){ i=(int)bounded(r,114); j=(int)bounded(r,113); if(j>=i) j++; }
  else if(!cift_sec(r,a,tur==1?p:2*p,&i,&j)) return 0;
  if(a[i]==a[j]) return 0;
  if(i>j){ int t=i; i=j; j=t; }
  const int x=a[i], y=a[j]; long dS=0;
  if(SIF=='M'){ dS=(long)((i+1)-(j+1))*(y-x); if(*S+dS>S_OBS) return 0; }
  else if(SIF=='L'){ int dI=(y<x)-(x<y); for(int k=i+1;k<j;k++){ const int z=a[k]; dI+=(y<z)-(x<z)+(z<x)-(z<y); }
    if(dI>0 && unif(r)>=QQ[dI]) return 0; }
  a[i]=y; a[j]=x; const uint64_t c=kosul(E,a,P);
  if((c&R)!=R){ a[i]=x; a[j]=y; return 0; }
  *S+=dS; *cb=c; return 1; }
static void *isci(void *arg){
  (void)arg; static __thread prof_t P; const int N=N_;
  int *A=malloc(sizeof(int)*114*(size_t)N), *B=malloc(sizeof(int)*114*(size_t)N); uint64_t *CA=malloc(8*(size_t)N), *CB=malloc(8*(size_t)N);
  long *SA=malloc(sizeof(long)*N), *SB=malloc(sizeof(long)*N); orn_t *Z=malloc(sizeof(orn_t)); int *sec=malloc(sizeof(int)*N);
  for(;;){ const int q=atomic_fetch_add(&SIRA,1); if(q>=NIS) break; const olay_t *E=&EV[IS[q].e]; const int rep=IS[q].r; plan_t *PL=&PLAN[IS[q].e];
    const uint64_t sd=tohum(SIF,9,16L*E->no+rep); orn_init(Z,SIF,sd); rng_t rg; rseed(&rg,sd^0x5DEECE66DULL);
    for(int s=0;s<N;s++){ orn_sonraki(Z); memcpy(A+114*(size_t)s,Z->a,sizeof(int)*114); CA[s]=kosul(E,Z->a,&P); SA[s]=Sof(Z->a); }
    uint64_t R=0; double f[KMAX], kab[KMAX]; int ord[KMAX], T[KMAX], nd=0; double est=1.0; long adimlar=0;
    for(int j=0;j<E->nc;j++){ int c=-1, best=-1;
      if(MOD_OLCUM) c=PL->ord[j];
      else for(int cc=0;cc<E->nc;cc++){ if(R>>cc&1) continue; int n=0; const uint64_t RR=R|1ull<<cc; for(int s=0;s<N;s++) n+=((CA[s]&RR)==RR); if(n>best){ best=n; c=cc; } }
      const uint64_t RR=R|1ull<<c; int ns=0; for(int s=0;s<N;s++) if((CA[s]&RR)==RR) sec[ns++]=s;
      ord[j]=c; f[j]=(double)ns/N; est*=f[j]; nd=j+1; R=RR; kab[j]=-1; T[j]=0;
      if(ns==0 || j==E->nc-1) break;
      // zincirler: sec[]'teki kitaplardan başlar; her örnek, sıradaki zincirde T adım sonraki durumdur
      for(int k=0;k<ns;k++){ memcpy(B+114*(size_t)k,A+114*(size_t)sec[k],sizeof(int)*114); CB[k]=CA[sec[k]]; SB[k]=SA[sec[k]]; }
      int Tj;
      if(MOD_OLCUM) Tj=PL->T[j];
      else { long ac=0; const int deneme=2000; for(int d=0;d<deneme;d++){ const int k=d%ns; ac+=adim(&rg,B+114*(size_t)k,SB+k,CB+k,E,R,&P); }
        const double ka=(double)ac/deneme; Tj=ka>0?(int)ceil(4.0/ka):ZTMAX; if(Tj<5) Tj=5; if(Tj>ZTMAX) Tj=ZTMAX; }
      long ac=0;
      for(int s=0;s<N;s++){ const int k=s%ns; int *a=B+114*(size_t)k;
        for(int t=0;t<Tj;t++) ac+=adim(&rg,a,SB+k,CB+k,E,R,&P);
        memcpy(A+114*(size_t)s,a,sizeof(int)*114); CA[s]=CB[k]; SA[s]=SB[k]; }
      T[j]=Tj; kab[j]=(double)ac/((double)N*Tj); adimlar+=(long)N*Tj; }
    if(!MOD_OLCUM){ for(int j=0;j<nd;j++){ PL->ord[j]=ord[j]; PL->T[j]=T[j]; } PL->ok=(nd==E->nc); }
    pthread_mutex_lock(&KILIT);
    fprintf(CIK,"%s r %d p %.6e duzey %d/%d adim %ld | sira",E->ad,rep,est,nd,E->nc,adimlar); for(int j=0;j<nd;j++) fprintf(CIK," %d",ord[j]);
    fprintf(CIK," | f"); for(int j=0;j<nd;j++) fprintf(CIK," %.5f",f[j]); fprintf(CIK," | T"); for(int j=0;j<nd;j++) fprintf(CIK," %d",T[j]);
    fprintf(CIK," | kabul"); for(int j=0;j<nd;j++) fprintf(CIK," %.4f",kab[j]); fprintf(CIK,"\n"); fflush(CIK);
    pthread_mutex_unlock(&KILIT); }
  return NULL; }
int main(int argc,char**argv){
  if(argc<9){ fprintf(stderr,"kullanım: zincir <sifir> <olaylar> <secim> <pilot|olcum> <N> <tekrar> <iş> <çıktı> [pilot]\n"); return 1; }
  SIF=argv[1][0]; MOD_OLCUM=!strcmp(argv[4],"olcum"); N_=atoi(argv[5]); const int TEK=atoi(argv[6]), NT=atoi(argv[7]); kutuphane_init();
  if(SIF=='L'){ orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,'L',1); for(int d=0;d<512;d++) QQ[d]=pow(Z->q,d); free(Z); }
  else if(SIF!='M' && SIF!='U'){ fprintf(stderr,"sıfır?\n"); return 2; }
  // olaylar
  int cap=1<<16; EV=calloc(cap,sizeof(olay_t)); char (*TUM)[24]=calloc(cap,24); int *TL=calloc(cap,sizeof(int)); uint64_t (*TM)[5]=calloc(cap,40); int nt=0;
  { FILE *f=fopen(argv[2],"r"); if(!f){ perror(argv[2]); return 2; } char s[4096];
    while(fgets(s,sizeof s,f)){ char a[64]; int h,L; unsigned long long m[5]; if(sscanf(s,"%63s %d %d %llx %llx %llx %llx %llx",a,&h,&L,&m[0],&m[1],&m[2],&m[3],&m[4])!=8) return 3;
      if(nt==cap) return 4; snprintf(TUM[nt],24,"%s",a); TL[nt]=L; for(int k=0;k<5;k++) TM[nt][k]=m[k]; nt++; } fclose(f); }
  { FILE *f=fopen(argv[3],"r"); if(!f){ perror(argv[3]); return 2; } char a[64];
    while(fscanf(f,"%63s",a)==1){ int i=0; while(i<nt && strcmp(TUM[i],a)) i++; if(i==nt){ fprintf(stderr,"olay yok: %s\n",a); return 5; }
      olay_t *E=&EV[NE++]; snprintf(E->ad,24,"%s",a); E->no=i; E->L=TL[i]; E->nc=1; E->g[0]=-1;
      for(int k=0;k<5;k++) for(int b=0;b<64;b++) if(TM[i][k]>>b&1){ if(E->nc>=KMAX) return 6; E->g[E->nc++]=64*k+b; } } fclose(f); }
  PLAN=calloc(NE,sizeof(plan_t));
  if(MOD_OLCUM){ if(argc<10){ fprintf(stderr,"pilot dosyası?\n"); return 2; } FILE *f=fopen(argv[9],"r"); if(!f){ perror(argv[9]); return 2; } char s[16384];
    while(fgets(s,sizeof s,f)){ char a[64]; int r; if(sscanf(s,"%63s r %d",a,&r)!=2 || r!=0) continue; int e=0; while(e<NE && strcmp(EV[e].ad,a)) e++; if(e==NE) continue;
      char *p=strstr(s,"| sira"), *pt=strstr(s,"| T"); if(!p||!pt) return 7; p+=6; pt+=3; int nd=0;
      for(;;){ char *q; const long x=strtol(p,&q,10); if(q==p || nd>=KMAX) break; PLAN[e].ord[nd++]=(int)x; p=q; }
      for(int j=0;j<nd;j++){ char *q; PLAN[e].T[j]=(int)strtol(pt,&q,10); if(q==pt) return 7; pt=q; } PLAN[e].ok=(nd==EV[e].nc); } fclose(f);
    for(int e=0;e<NE;e++) if(!PLAN[e].ok){ fprintf(stderr,"pilot eksik ya da sıfıra düştü: %s (ölçülmez)\n",EV[e].ad); } }
  IS=malloc(sizeof(is_t)*(size_t)NE*(TEK+1));
  for(int r=(MOD_OLCUM?1:0);r<=(MOD_OLCUM?TEK:0);r++) for(int e=0;e<NE;e++) if(!MOD_OLCUM || PLAN[e].ok){ IS[NIS].e=e; IS[NIS].r=r; NIS++; }
  CIK=fopen(argv[8],"a"); if(!CIK){ perror(argv[8]); return 2; }
  fprintf(CIK,"# zincir sıfır %c mod %s N %d tekrar %d olay %d iş %d ZTMAX %d\n",SIF,argv[4],N_,TEK,NE,NIS,ZTMAX); fflush(CIK);
  pthread_t th[64]; for(int t=0;t<NT;t++) pthread_create(&th[t],NULL,isci,NULL); for(int t=0;t<NT;t++) pthread_join(th[t],NULL);
  fclose(CIK); return 0; }
