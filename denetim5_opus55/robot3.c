// ROBOT NUMEROLOG — simetrik, önceden sabitlenmiş genel olgu bataryası.
// Her mercek λ = (α,β,m): v = αn+βa, w = αn−βa; F = {v≡0 (m)}, G = {w≡0 (m)}, U = F∪G.
// Olgular (≤64): kitap terazisi (v-paritesi), F ve G aile olguları (boy, toplam, yarılar, hücreler,
// katsayı bandı...), birleşim olguları. Her olgunun sıfır altındaki olasılığı, bağlama (aile boyu)
// koşullu olarak ayrı bir sıfır örnekleminden tablolanır. Puan = isabet eden olguların −log10 p toplamı.
// Aynı fonksiyon Kur'an'a ve rastgele kitaplara uygulanır → sıra (rank) kalibre bir p-değeridir.
//
//   ./robot table <null> <seed> <N> <threads> <tabfile>
//   ./robot score <null> <seed> <N> <threads> <tabfile> <outprefix>
//   ./robot facts <tabfile>                 (Kur'an'ın (1,1,19) ve en iyi mercekteki döküm)
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

static const int AYAT127[114] = {
  7,286,200,176,120,165,206,75,127,109,123,111,43,52,99,128,111,110,98,135,
  112,78,118,64,77,227,93,88,69,60,34,30,73,54,45,83,182,88,75,85,
  54,53,89,59,37,35,38,29,18,45,60,49,62,55,78,96,29,22,24,13,
  14,11,11,18,12,12,30,52,52,44,28,28,20,56,40,31,50,40,46,42,
  29,19,36,25,22,17,19,26,30,20,15,21,11,8,8,19,5,8,8,11,
  11,8,3,9,5,4,7,3,6,3,5,4,5,6};
static int BASE[114];   // örneklenen çokluk (varsayılan AYAT127)

#define NLIN 7
#define NFORM 17
static const int FORMS[NLIN][2] = {{1,1},{1,2},{1,3},{2,1},{2,3},{3,1},{3,2}};
static const char *NLNAME[10]={"n·a","n²+a","n+a²","n²+a²","n²−a²","ebob(n,a)","n·a+n+a","a²−n","n²−a","n·a−(n+a)"};
static inline long gcdl(long x,long y){ if(x<0)x=-x; if(y<0)y=-y; while(y){ long t=x%y; x=y; y=t; } return x; }
// f<NLIN: doğrusal (α,β); f≥NLIN: kesif24 doğrusal-olmayan biçimleri (v,w) çiftleri
static inline void vw(int f, long n, long a, long *v, long *w){
  if(f<NLIN){ *v=FORMS[f][0]*n+FORMS[f][1]*a; *w=FORMS[f][0]*n-FORMS[f][1]*a; return; }
  switch(f-NLIN){
    case 0: *v=n*a; *w=n-a; break;
    case 1: *v=n*n+a; *w=n-a; break;
    case 2: *v=n+a*a; *w=n-a; break;
    case 3: *v=n*n+a*a; *w=n*n-a*a; break;
    case 4: *v=n*n-a*a; *w=n*n+a*a; break;
    case 5: *v=gcdl(n,a); *w=n-a; break;
    case 6: *v=n*a+n+a; *w=n*a-n-a; break;
    case 7: *v=a*a-n; *w=a*a+n; break;
    case 8: *v=n*n-a; *w=n*n+a; break;
    default: *v=n*a-n-a; *w=n*a+n+a; break; } }
#define MMIN 3
#define MMAX 40
#define NMOD (MMAX-MMIN+1)
#define NLENS (NFORM*NMOD)
#define NCTX 41
#define NF 64

// ---------------- olgu adları ve bağlam tipleri ----------------
// bağlam: 0 = koşulsuz, 1 = kF'ye koşullu, 2 = kG'ye koşullu
static const char *FNAME[NF]; static int FCTX[NF]; static int FNODE[NF]; static int NFACT = 0;
#define NNODE 15
static int CUR_NODE=0; static int NODE_NF[NNODE];
static const char *NODENAME[NNODE]={"kitap-terazi","F-boy","F-toplam","F-çapraz","F-yarı/hücre","F-katsayı","G-boy","G-toplam","G-çapraz","G-yarı/hücre","G-katsayı","U-boy/ilişki","U-bölünebilirlik","U-güzel","G-ortalama"};
static int fid(const char *nm, int ctx){ FNAME[NFACT]=nm; FCTX[NFACT]=ctx; FNODE[NFACT]=CUR_NODE; NODE_NF[CUR_NODE]++; return NFACT++; }
// kitap
static int B_CNT, B_EQ[11];
static const char *BEQN[11] = {"vE=Σa","vE=Σn","vO=Σa","vO=Σn","vE=vO","aE=aO","nE=nO","nE=aE","nO=aO","nE=aO","nO=aE"};
// aile (F: X=0, G: X=1)
static int K1[2],K2[2],S_M2[2],S_SQ[2],S_114[2],X_NA[2],X_N0[2],X_NSQ[2],X_ASQ[2],H_EQV[2],H_NEARV[2],H_M2V[2],H_EQN[2],H_NEARN[2],H_M2N[2],DCELL[2],BAND[2],AP2[2];
// birleşim
static int U_M,U_2M,U_DIS,U_I1,U_A,U_N,U_V,U_W,U_VPP,U_ASQ,R_EQ,G_AVG;
static char NAMEBUF[64][32];
static void init_facts(void){
  CUR_NODE=0;
  B_CNT = fid("kitap: |E|=|O|",0);
  for(int j=0;j<11;j++){ snprintf(NAMEBUF[j],32,"kitap: |E|=|O| & %s",BEQN[j]); B_EQ[j]=fid(NAMEBUF[j],0); }
  const char *X[2]={"F","G"};
  static char nb[2][18][40];
  for(int x=0;x<2;x++){
    int c = x==0?1:2; int i=0;
    #define MK(var,txt,ctx) snprintf(nb[x][i],40,"%s: %s",X[x],txt); var[x]=fid(nb[x][i],ctx); i++;
    int b0 = x==0?1:6;
    CUR_NODE=b0;   MK(K1,"k≥2E",0) MK(K2,"k=m",0)
    CUR_NODE=b0+1; MK(S_M2,"m²|Σq",c) MK(S_SQ,"Σq kare",c) MK(S_114,"114|Σq",c)
    CUR_NODE=b0+2; MK(X_NA,"Σn=Σa",c) MK(X_N0,"m|Σn",c) MK(X_NSQ,"Σn kare",c) MK(X_ASQ,"Σa kare",c)
    CUR_NODE=b0+3; MK(H_EQV,"v-yarılar eşit",c) MK(H_NEARV,"v-yarılar Δ∈{m²,2m²}",c) MK(H_M2V,"v-yarılar m²|",c)
                   MK(H_EQN,"n-yarılar eşit",c) MK(H_NEARN,"n-yarılar Δ∈{m²,2m²}",c) MK(H_M2N,"n-yarılar m²|",c)
                   MK(DCELL,"hücreler dengeli",c)
    CUR_NODE=b0+4; MK(BAND,"katsayı bandı",c) MK(AP2,"katsayı adım-2",c)
    #undef MK
  }
  CUR_NODE=11; U_M=fid("U: |U|=m",1); U_2M=fid("U: |U|=2m",1); U_DIS=fid("U: F∩G=∅",1); U_I1=fid("U: |F∩G|=1",1); R_EQ=fid("R: kF=kG",0);
  CUR_NODE=12; U_A=fid("U: m|Σa(U)",1); U_N=fid("U: m|Σn(U)",1); U_V=fid("U: m|Σv(U)",1); U_W=fid("U: m|Σw(U)",1);
  CUR_NODE=13; U_VPP=fid("U: Σv(U) kare/asal-kuvvet",1); U_ASQ=fid("U: Σa(U) kare",1);
  CUR_NODE=14; G_AVG=fid("G: Σw=m·kG",2);
}

static long PP[200]; static int nPP=0;
static inline long labs_(long x){ return x<0?-x:x; }
static inline int is_sq(long x){ x=labs_(x); if(x==0) return 0; long r=(long)floor(sqrt((double)x)); return (r*r==x)||((r+1)*(r+1)==x); }
static inline int is_pp(long x){ x=labs_(x); for(int i=0;i<nPP;i++) if(PP[i]==x) return 1; return 0; }

// ---------------- olgu hesaplayıcı ----------------
typedef struct { uint64_t bits; int kF, kG; int same; } fres_t;

static inline void family_facts(int x, int m, double E, int k, const long *q, const int *nn, const int *aa, uint64_t *bits){
  // q: doğal nicelik (F için v, G için w), nn: n, aa: a (üye listeleri)
  long mm=(long)m*m;
  if(k>=2*E) *bits|=1ULL<<K1[x];
  if(k==m) *bits|=1ULL<<K2[x];
  if(k==0) return;
  long sq=0,sn=0,sa=0; for(int i=0;i<k;i++){ sq+=q[i]; sn+=nn[i]; sa+=aa[i]; }
  if(sq%mm==0) *bits|=1ULL<<S_M2[x];
  if(is_sq(sq)) *bits|=1ULL<<S_SQ[x];
  if(sq!=0 && sq%114==0) *bits|=1ULL<<S_114[x];
  if(k>=2 && sn==sa) *bits|=1ULL<<X_NA[x];
  if(sn%m==0) *bits|=1ULL<<X_N0[x];
  if(is_sq(sn)) *bits|=1ULL<<X_NSQ[x];
  if(is_sq(sa)) *bits|=1ULL<<X_ASQ[x];
  // v-paritesi (q-paritesi) ve n-paritesi yarıları
  int kv0=0,kn0=0; long qv0=0,qn0=0; int kc[4]={0,0,0,0}; long sc[4]={0,0,0,0};
  for(int i=0;i<k;i++){ int pv=(q[i]%2==0)?0:1; int pn=(nn[i]%2==0)?0:1;
    if(!pv){kv0++; qv0+=q[i];} if(!pn){kn0++; qn0+=q[i];}
    int ci=pv*2+pn; kc[ci]++; sc[ci]+=q[i]/m; }
  long qv1=sq-qv0, qn1=sq-qn0;
  if(2*kv0==k){ long d=labs_(qv0-qv1); if(d==0) *bits|=1ULL<<H_EQV[x]; if(d==mm||d==2*mm) *bits|=1ULL<<H_NEARV[x]; }
  if(kv0>0 && kv0<k && qv0%mm==0 && qv1%mm==0) *bits|=1ULL<<H_M2V[x];
  if(2*kn0==k){ long d=labs_(qn0-qn1); if(d==0) *bits|=1ULL<<H_EQN[x]; if(d==mm||d==2*mm) *bits|=1ULL<<H_NEARN[x]; }
  if(kn0>0 && kn0<k && qn0%mm==0 && qn1%mm==0) *bits|=1ULL<<H_M2N[x];
  if(2*kv0==k && qv0==qv1 && k%4==0 && kc[0]==k/4 && kc[1]==k/4 && kc[2]==k/4 && kc[3]==k/4
     && labs_(sc[0]-sc[1])<=2 && labs_(sc[2]-sc[3])<=2) *bits|=1ULL<<DCELL[x];
  if(k>=3){
    long c[128]; for(int i=0;i<k;i++) c[i]=q[i]/m;
    for(int i=1;i<k;i++){ long y=c[i]; int j=i-1; while(j>=0&&c[j]>y){c[j+1]=c[j];j--;} c[j+1]=y; }
    int nd=1; for(int i=1;i<k;i++) if(c[i]!=c[i-1]) nd++;
    long span=c[k-1]-c[0];
    if(nd==span+1) *bits|=1ULL<<BAND[x];
    int samep=1; long p0=((c[0]%2)+2)%2; for(int i=0;i<k;i++) if((((c[i]%2)+2)%2)!=p0){samep=0;break;}
    if(samep && nd>=2 && span%2==0 && nd==span/2+1) *bits|=1ULL<<AP2[x];
  }
}

static void facts(const int *a, int li, fres_t *R){
  int f=li/NMOD, m=MMIN+li%NMOD; int al=f<NLIN?FORMS[f][0]:1, be=f<NLIN?FORMS[f][1]:1;
  double E=114.0/m; uint64_t bits=0;
  long qF[114],qG[114]; int nF[114],aF[114],nG[114],aG[114]; int kF=0,kG=0;
  int e_cnt=0; long vE=0,nE=0,aE=0, SA=0,SV=0; const long SN=6555;
  int kU=0,kI=0; long saU=0,snU=0,svU=0,swU=0;
  int blind_membership = (f<NLIN) ? !((al&1)&&(be&1)) : 0;
  for(int i=0;i<114;i++){
    int n=i+1; long v,w; vw(f,n,a[i],&v,&w);
    SA+=a[i]; SV+=v;
    if(v%2==0){ e_cnt++; vE+=v; nE+=n; aE+=a[i]; }
    int inF=(v%m==0), inG=(w%m==0);
    if(inF){ qF[kF]=v; nF[kF]=n; aF[kF]=a[i]; kF++; }
    if(inG){ qG[kG]=w; nG[kG]=n; aG[kG]=a[i]; kG++; }
    if(inF||inG){ kU++; saU+=a[i]; snU+=n; svU+=v; swU+=w; }
    if(inF&&inG) kI++;
  }
  // kitap terazisi
  long vO=SV-vE, nO=SN-nE, aO=SA-aE;
  int cnt_ok = blind_membership ? 1 : (e_cnt==57);
  if(!blind_membership && e_cnt==57) bits|=1ULL<<B_CNT;
  if(cnt_ok){
    int eq[11]={vE==SA, vE==SN, vO==SA, vO==SN, vE==vO, aE==aO, nE==nO, nE==aE, nO==aO, nE==aO, nO==aE};
    for(int j=0;j<11;j++) if(eq[j]) bits|=1ULL<<B_EQ[j];
  }
  family_facts(0,m,E,kF,qF,nF,aF,&bits);
  family_facts(1,m,E,kG,qG,nG,aG,&bits);
  if(kU==m) bits|=1ULL<<U_M;
  if(kU==2*m) bits|=1ULL<<U_2M;
  if(kF>0 && kG>0 && kI==0) bits|=1ULL<<U_DIS;
  if(kI==1) bits|=1ULL<<U_I1;
  if(kU>0){ if(saU%m==0) bits|=1ULL<<U_A; if(snU%m==0) bits|=1ULL<<U_N; if(svU%m==0) bits|=1ULL<<U_V; if(swU%m==0) bits|=1ULL<<U_W;
            if(is_sq(svU)||is_pp(svU)) bits|=1ULL<<U_VPP; if(is_sq(saU)) bits|=1ULL<<U_ASQ; }
  if(kF>0 && kF==kG) bits|=1ULL<<R_EQ;
  if(kG>0){ long sw=0; for(int i=0;i<kG;i++) sw+=qG[i]; if(sw==(long)m*kG) bits|=1ULL<<G_AVG; }
  R->bits=bits; R->kF=kF; R->kG=kG; R->same=(kI==kF && kI==kG);   // F ve G aynı küme mi?
}

// ---------------- RNG + sıfırlar ----------------
typedef struct { uint64_t s[4]; } rng_t;
static inline uint64_t rotl(const uint64_t x, int k){ return (x << k) | (x >> (64 - k)); }
static inline uint64_t xnext(rng_t *r){ uint64_t *s=r->s; const uint64_t res=rotl(s[0]+s[3],23)+s[0]; const uint64_t t=s[1]<<17;
  s[2]^=s[0]; s[3]^=s[1]; s[1]^=s[2]; s[0]^=s[3]; s[2]^=t; s[3]=rotl(s[3],45); return res; }
static void rseed(rng_t *r, uint64_t seed){ uint64_t z=seed; for(int i=0;i<4;i++){ z+=0x9E3779B97F4A7C15ULL; uint64_t x=z;
  x=(x^(x>>30))*0xBF58476D1CE4E5B9ULL; x=(x^(x>>27))*0x94D049BB133111EBULL; r->s[i]=x^(x>>31);} }
static inline uint32_t bounded(rng_t *r, uint32_t n){ return (uint32_t)(((unsigned __int128)xnext(r)*n)>>64); }
static inline double unif(rng_t *r){ return (xnext(r)>>11)*(1.0/9007199254740992.0); }
static inline double gauss(rng_t *r){ double u1=unif(r),u2=unif(r); if(u1<1e-300)u1=1e-300; return sqrt(-2.0*log(u1))*cos(6.283185307179586*u2); }
static long S_OBS;
#define NCH 64
typedef struct { char null; double par; rng_t r; int a[114]; long S; int ch[NCH][114]; long chS[NCH]; int cur; } sampler_t;
static void sampler_init(sampler_t *Z, char null, double par, uint64_t seed){
  Z->null=null; Z->par=par; rseed(&Z->r,seed); memcpy(Z->a,BASE,sizeof Z->a); Z->S=0; Z->cur=0;
  if(null=='M'){ // NCH bağımsız zincir; her biri tam-azalandan başlar, 30000 adım yakma
    int srt[114]; memcpy(srt,BASE,sizeof srt); for(int i=1;i<114;i++){ int x=srt[i]; int j=i-1; while(j>=0&&srt[j]<x){srt[j+1]=srt[j];j--;} srt[j+1]=x; }
    for(int c=0;c<NCH;c++){ int *a=Z->ch[c]; memcpy(a,srt,sizeof srt); long S=0; for(int i=0;i<114;i++) S+=(long)(i+1)*a[i];
      for(int b=0;b<30000;b++){ int i=(int)bounded(&Z->r,114), j=(int)bounded(&Z->r,114); long dS=(long)((i+1)-(j+1))*(a[j]-a[i]); if(S+dS<=S_OBS){ int t=a[i]; a[i]=a[j]; a[j]=t; S+=dS; } }
      Z->chS[c]=S; } }
}
static void sampler_next(sampler_t *Z){
  int *a=Z->a; rng_t *r=&Z->r;
  if(Z->null=='U'){ for(int i=113;i>0;i--){ int j=(int)bounded(r,(uint32_t)(i+1)); int t=a[i]; a[i]=a[j]; a[j]=t; } }
  else if(Z->null=='J'){ double key[114]; int idx[114]; for(int i=0;i<114;i++){ key[i]=(i+1)+Z->par*gauss(r); idx[i]=i; }
    for(int i=1;i<114;i++){ int x=idx[i]; double kx=key[x]; int j=i-1; while(j>=0&&key[idx[j]]>kx){ idx[j+1]=idx[j]; j--; } idx[j+1]=x; }
    for(int k=0;k<114;k++) a[k]=BASE[idx[k]]; }
  else if(Z->null=='M'){ int thin=(int)Z->par; if(thin<1)thin=1; int c=Z->cur; Z->cur=(Z->cur+1)%NCH; int *b=Z->ch[c]; long S=Z->chS[c];
    for(int st=0;st<thin;st++){ int i=(int)bounded(r,114), j=(int)bounded(r,114); long dS=(long)((i+1)-(j+1))*(b[j]-b[i]); if(S+dS<=S_OBS){ int t=b[i]; b[i]=b[j]; b[j]=t; S+=dS; } }
    Z->chS[c]=S; memcpy(a,b,114*sizeof(int)); }
}

// ---------------- tablo ----------------
typedef struct { uint64_t N; uint64_t hit[NLENS][NF][NCTX]; uint64_t trF[NLENS][NCTX], trG[NLENS][NCTX]; } table_t;
static inline int cb(int k){ return k<NCTX-1?k:NCTX-1; }

typedef struct { char null; double par; uint64_t seed, n; table_t *T; } tjob_t;
static void *table_worker(void *arg){
  tjob_t *J=(tjob_t*)arg; sampler_t Z; sampler_init(&Z,J->null,J->par,J->seed);
  table_t *T=J->T;
  for(uint64_t it=0; it<J->n; it++){
    sampler_next(&Z); T->N++;
    for(int li=0; li<NLENS; li++){ fres_t R; facts(Z.a,li,&R);
      int cF=cb(R.kF), cG=cb(R.kG); T->trF[li][cF]++; T->trG[li][cG]++;
      uint64_t b=R.bits; while(b){ int f=__builtin_ctzll(b); b&=b-1; int c = FCTX[f]==0?0:(FCTX[f]==1?cF:cG); T->hit[li][f][c]++; } }
  }
  return NULL;
}

static table_t *TAB;
static uint64_t DISABLED=0;
static void set_disabled(void){ const char *e=getenv("ROBOT_DISABLE"); if(!e) return; char buf[1024]; strncpy(buf,e,1023); buf[1023]=0;
  char *tok=strtok(buf,"|"); while(tok){ for(int f=0;f<NFACT;f++) if(strstr(FNAME[f],tok)) DISABLED|=1ULL<<f; tok=strtok(NULL,"|"); }
  fprintf(stderr,"devre dışı olgular:"); for(int f=0;f<NFACT;f++) if(DISABLED>>f&1) fprintf(stderr," [%s]",FNAME[f]); fprintf(stderr,"\n"); }
static long MINTR=0;   // bağlam deneme sayısı bunun altındaysa, olgunun o mercekteki bağlamdan-bağımsız (birleştirilmiş) oranına düş
static inline double pfact(int li, int f, int kF, int kG){
  int c; uint64_t tr;
  if(FCTX[f]==0){ c=0; tr=TAB->N; } else if(FCTX[f]==1){ c=cb(kF); tr=TAB->trF[li][c]; } else { c=cb(kG); tr=TAB->trG[li][c]; }
  if(FCTX[f]!=0 && (long)tr<MINTR){ uint64_t h=0; for(int k=0;k<NCTX;k++) h+=TAB->hit[li][f][k]; return ((double)h+0.5)/((double)TAB->N+1.0); }
  return ((double)TAB->hit[li][f][c]+0.5)/((double)tr+1.0);
}

// puanlar: T1 = Σ −log10 p (tüm isabetler), T2 = en büyük 10'un toplamı, T3 = Σ max(0, −log10 p − 1)
typedef struct { double t1,t2,t3; } sc_t;
// Düğüm puanı = düğümdeki isabetlerin en nadirinin −log10 p'si (çatallar düğüm içinde VEYA).
// T1 = Σ düğüm ; T2 = en iyi 8 düğüm (S1–S8 benzeri profil) ; T3 = Σ max(0, düğüm − log10(n_düğüm+1)) (çatal cezalı / MDL)
static sc_t score_lens(const int *a, int li, uint64_t *bits_out){
  fres_t R; facts(a,li,&R); uint64_t b=R.bits&~DISABLED; if(bits_out) *bits_out=b;
  double nodev[NNODE]; for(int i=0;i<NNODE;i++) nodev[i]=0;
  while(b){ int f=__builtin_ctzll(b); b&=b-1; double p=pfact(li,f,R.kF,R.kG); double x=-log10(p); if(x<0)x=0; if(x>nodev[FNODE[f]]) nodev[FNODE[f]]=x; }
  if(R.same){ for(int i=6;i<=11;i++) nodev[i]=0; nodev[14]=0; }   // F=G: G düğümleri ve birleşim-boy/ilişki F'nin kopyası — çift sayma yok
  sc_t S={0,0,0}; double s[NNODE];
  for(int i=0;i<NNODE;i++){ s[i]=nodev[i]; S.t1+=nodev[i]; double pen=log10((double)NODE_NF[i]+1.0); if(nodev[i]>pen) S.t3+=nodev[i]-pen; }
  for(int i=1;i<NNODE;i++){ double y=s[i]; int j=i-1; while(j>=0&&s[j]<y){s[j+1]=s[j];j--;} s[j+1]=y; }
  for(int i=0;i<8;i++) S.t2+=s[i];
  return S;
}
static int LI_Q;   // (1,1,19)
typedef struct { sc_t best, fix; int arg1,arg2,arg3; } bs_t;
static bs_t score_book(const int *a){
  bs_t B; memset(&B,0,sizeof B);
  for(int li=0;li<NLENS;li++){ sc_t S=score_lens(a,li,NULL);
    if(S.t1>B.best.t1){B.best.t1=S.t1;B.arg1=li;} if(S.t2>B.best.t2){B.best.t2=S.t2;B.arg2=li;} if(S.t3>B.best.t3){B.best.t3=S.t3;B.arg3=li;}
    if(li==LI_Q) B.fix=S; }
  return B;
}

typedef struct { char null; double par; uint64_t seed, n; float *out; } sjob_t;
static void *score_worker(void *arg){
  sjob_t *J=(sjob_t*)arg; sampler_t Z; sampler_init(&Z,J->null,J->par,J->seed);
  for(uint64_t it=0; it<J->n; it++){ sampler_next(&Z); bs_t B=score_book(Z.a);
    float *o=J->out+it*9; o[0]=B.best.t1; o[1]=B.best.t2; o[2]=B.best.t3; o[3]=B.fix.t1; o[4]=B.fix.t2; o[5]=B.fix.t3; o[6]=B.arg1; o[7]=B.arg2; o[8]=B.arg3; }
  return NULL;
}

static void lens_name(int li, char *buf){ int f=li/NMOD, m=MMIN+li%NMOD; if(f<NLIN) sprintf(buf,"(%d,%d,%d)",FORMS[f][0],FORMS[f][1],m); else sprintf(buf,"(%s,%d)",NLNAME[f-NLIN],m); }

static void dump_book(const int *a, const char *title){
  bs_t B=score_book(a); char nb[32];
  printf("== %s ==\n", title);
  lens_name(B.arg1,nb); printf("  en iyi mercek T1: %s  T1=%.2f\n",nb,B.best.t1);
  lens_name(B.arg2,nb); printf("  en iyi mercek T2: %s  T2=%.2f\n",nb,B.best.t2);
  lens_name(B.arg3,nb); printf("  en iyi mercek T3: %s  T3=%.2f\n",nb,B.best.t3);
  printf("  sabit mercek (1,1,19): T1=%.2f T2=%.2f T3=%.2f\n",B.fix.t1,B.fix.t2,B.fix.t3);
  int lis[2]={LI_Q,B.arg2};
  for(int z=0;z<2;z++){ int li=lis[z]; fres_t R; facts(a,li,&R); lens_name(li,nb);
    printf("  döküm %s (kF=%d kG=%d):\n",nb,R.kF,R.kG);
    uint64_t b=R.bits; while(b){ int f=__builtin_ctzll(b); b&=b-1; double p=pfact(li,f,R.kF,R.kG); printf("     [%-16s] %-34s p=%.2e  (−log10=%.2f)\n",NODENAME[FNODE[f]],FNAME[f],p,-log10(p)); } }
}

int main(int argc, char **argv){
  init_facts();
  for(int p=2;p<=7;p++){ if(p==4||p==6) continue; long v=(long)p*p*p; while(v<1000000){ PP[nPP++]=v; v*=p; } }
  memcpy(BASE,AYAT127,sizeof BASE);
  if(getenv("ROBOT_BASE")){ const char *s=getenv("ROBOT_BASE"); char *e; for(int i=0;i<114;i++){ BASE[i]=(int)strtol(s,&e,10); s=e; if(*s==',') s++; } }
  S_OBS=0; for(int i=0;i<114;i++) S_OBS+=(long)(i+1)*BASE[i];   // (BASE ayarlandıktan sonra)

  LI_Q = 0*NMOD + (19-MMIN);
  set_disabled(); if(getenv("ROBOT_MINTR")) MINTR=atol(getenv("ROBOT_MINTR"));
  if(argc<2){ fprintf(stderr,"kullanım: table|score|facts ...\n"); return 1; }
  if(strcmp(argv[1],"table")==0){
    char null=argv[2][0]; double par=strlen(argv[2])>1?atof(argv[2]+1):0; uint64_t seed=strtoull(argv[3],NULL,10), N=strtoull(argv[4],NULL,10); int th=atoi(argv[5]);
    tjob_t *J=calloc(th,sizeof(tjob_t)); pthread_t *P=calloc(th,sizeof(pthread_t));
    for(int i=0;i<th;i++){ J[i].null=null; J[i].par=par; J[i].seed=seed*1000003ULL+i*7919ULL+3; J[i].n=N/th; J[i].T=calloc(1,sizeof(table_t)); pthread_create(&P[i],NULL,table_worker,&J[i]); }
    table_t *T=calloc(1,sizeof(table_t));
    for(int i=0;i<th;i++){ pthread_join(P[i],NULL); table_t *X=J[i].T; T->N+=X->N;
      for(int l=0;l<NLENS;l++){ for(int c=0;c<NCTX;c++){ T->trF[l][c]+=X->trF[l][c]; T->trG[l][c]+=X->trG[l][c]; } for(int f=0;f<NF;f++) for(int c=0;c<NCTX;c++) T->hit[l][f][c]+=X->hit[l][f][c]; } }
    FILE *fp=fopen(argv[6],"wb"); fwrite(T,sizeof(table_t),1,fp); fclose(fp);
    printf("tablo yazıldı: N=%llu, olgu sayısı=%d, mercek=%d\n",(unsigned long long)T->N,NFACT,NLENS); return 0;
  }
  TAB=calloc(1,sizeof(table_t));
  if(strcmp(argv[1],"facts")==0){ FILE *fp=fopen(argv[2],"rb"); fread(TAB,sizeof(table_t),1,fp); fclose(fp);
    dump_book(BASE, getenv("ROBOT_BASE")?"TEMEL KİTAP (ROBOT_BASE)":"KUR'AN (kodeks 127)");
    if(argc>3){ // Medine sayımı vb. harici kitap: virgüllü liste
      int b[114]; char *s=argv[3]; for(int i=0;i<114;i++){ b[i]=(int)strtol(s,&s,10); if(*s==',') s++; } dump_book(b,argc>4?argv[4]:"harici"); }
    return 0; }
  if(strcmp(argv[1],"demo")==0){
    char null=argv[2][0]; double par=strlen(argv[2])>1?atof(argv[2]+1):0; uint64_t seed=strtoull(argv[3],NULL,10), N=strtoull(argv[4],NULL,10);
    FILE *fp=fopen(argv[5],"rb"); fread(TAB,sizeof(table_t),1,fp); fclose(fp); int K=atoi(argv[6]);
    sampler_t Z; sampler_init(&Z,null,par,seed); int best[16][114]; double bv[16]; uint64_t bi[16]; for(int i=0;i<K;i++) bv[i]=-1;
    for(uint64_t it=0; it<N; it++){ sampler_next(&Z); bs_t B=score_book(Z.a);
      if(B.best.t2>bv[K-1]){ int j=K-1; while(j>0 && bv[j-1]<B.best.t2){ bv[j]=bv[j-1]; bi[j]=bi[j-1]; memcpy(best[j],best[j-1],sizeof best[0]); j--; } bv[j]=B.best.t2; bi[j]=it; memcpy(best[j],Z.a,sizeof best[0]); } }
    for(int i=0;i<K;i++){ char t[128]; snprintf(t,128,"SIFIR KİTABI #%llu (tohum %llu, sıfır %s)",(unsigned long long)bi[i],(unsigned long long)seed,argv[2]); dump_book(best[i],t);
      printf("  diziliş: "); for(int k=0;k<114;k++) printf("%d%s",best[i][k],k<113?",":"\n"); }
    return 0; }
  if(strcmp(argv[1],"demofix")==0){   // yalnız sabit mercek (1,1,19): Kur'an'ı geçen sıfır kitaplarını bul ve dök
    char null=argv[2][0]; double par=strlen(argv[2])>1?atof(argv[2]+1):0; uint64_t seed=strtoull(argv[3],NULL,10), N=strtoull(argv[4],NULL,10);
    FILE *fp=fopen(argv[5],"rb"); fread(TAB,sizeof(table_t),1,fp); fclose(fp); int K=atoi(argv[6]);
    sc_t Qs=score_lens(AYAT127,LI_Q,NULL); printf("Kur'an sabit-mercek T1=%.2f T2=%.2f T3=%.2f\n",Qs.t1,Qs.t2,Qs.t3);
    sampler_t Z; sampler_init(&Z,null,par,seed); int best[16][114]; double bv[16]; uint64_t bi[16]; for(int i=0;i<K;i++) bv[i]=-1; uint64_t nge=0;
    for(uint64_t it=0; it<N; it++){ sampler_next(&Z); sc_t S=score_lens(Z.a,LI_Q,NULL); if(S.t2>=Qs.t2-1e-9) nge++;
      if(S.t2>bv[K-1]){ int j=K-1; while(j>0 && bv[j-1]<S.t2){ bv[j]=bv[j-1]; bi[j]=bi[j-1]; memcpy(best[j],best[j-1],sizeof best[0]); j--; } bv[j]=S.t2; bi[j]=it; memcpy(best[j],Z.a,sizeof best[0]); } }
    printf("sabit mercekte T2 ≥ Kur'an olan sıfır kitabı: %llu / %llu\n",(unsigned long long)nge,(unsigned long long)N);
    for(int i=0;i<K;i++){ fres_t R; facts(best[i],LI_Q,&R); printf("--- SIFIR KİTABI #%llu (tohum %llu, %s): sabit T2=%.2f  kF=%d kG=%d\n",(unsigned long long)bi[i],(unsigned long long)seed,argv[2],bv[i],R.kF,R.kG);
      uint64_t b=R.bits&~DISABLED; while(b){ int f=__builtin_ctzll(b); b&=b-1; double p=pfact(LI_Q,f,R.kF,R.kG); printf("     [%-16s] %-34s p=%.2e  (−log10=%.2f)\n",NODENAME[FNODE[f]],FNAME[f],p,-log10(p)); }
      printf("  diziliş: "); for(int k=0;k<114;k++) printf("%d%s",best[i][k],k<113?",":"\n"); }
    return 0; }
  if(strcmp(argv[1],"score")==0){
    char null=argv[2][0]; double par=strlen(argv[2])>1?atof(argv[2]+1):0; uint64_t seed=strtoull(argv[3],NULL,10), N=strtoull(argv[4],NULL,10); int th=atoi(argv[5]);
    FILE *fp=fopen(argv[6],"rb"); fread(TAB,sizeof(table_t),1,fp); fclose(fp);
    bs_t Q=score_book(BASE);
    char fn[512]; snprintf(fn,512,"%s_kuran.txt",argv[7]); FILE *fq=fopen(fn,"w");
    fprintf(fq,"%.4f %.4f %.4f %.4f %.4f %.4f %d %d %d\n",Q.best.t1,Q.best.t2,Q.best.t3,Q.fix.t1,Q.fix.t2,Q.fix.t3,Q.arg1,Q.arg2,Q.arg3); fclose(fq);
    uint64_t per=N/th; float *out=malloc(sizeof(float)*9*per*th);
    sjob_t *J=calloc(th,sizeof(sjob_t)); pthread_t *P=calloc(th,sizeof(pthread_t));
    for(int i=0;i<th;i++){ J[i].null=null; J[i].par=par; J[i].seed=seed*1000003ULL+i*7919ULL+5; J[i].n=per; J[i].out=out+i*per*9; pthread_create(&P[i],NULL,score_worker,&J[i]); }
    for(int i=0;i<th;i++) pthread_join(P[i],NULL);
    snprintf(fn,512,"%s_sifir.f32",argv[7]); FILE *fo=fopen(fn,"wb"); fwrite(out,sizeof(float),9*per*th,fo); fclose(fo);
    printf("puanlandı: %llu kitap. Kur'an: en-iyi T1=%.2f T2=%.2f T3=%.2f | sabit T1=%.2f T2=%.2f T3=%.2f\n",(unsigned long long)(per*th),Q.best.t1,Q.best.t2,Q.best.t3,Q.fix.t1,Q.fix.t2,Q.fix.t3);
    return 0;
  }
  return 1;
}
