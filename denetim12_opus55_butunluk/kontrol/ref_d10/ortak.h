// ortak.h — koşullu ölçüm (kosullu) için ortak parçalar.
// Veri, RNG (robot4_m ile aynı xoshiro256++ ve tohumlama), robotun U/J/M sıfırları (birebir port),
// Mallows örnekleyicisi (Lehmer kodu, kesin), sabit mercek (1,1,19) olguları (robot4_m facts() portu,
// aynı olgu sırası ve aynı bitler), olaylar (kesif4_motor tanımları) ve yardımcılar.
// Kodeks: kodeks_sec(129) ya da kodeks_sec(127). Birincil sayım 129.
#ifndef ORTAK_H
#define ORTAK_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static const int AYAT129[114] = {
  7,286,200,176,120,165,206,75,129,109,123,111,43,52,99,128,111,110,98,135,
  112,78,118,64,77,227,93,88,69,60,34,30,73,54,45,83,182,88,75,85,
  54,53,89,59,37,35,38,29,18,45,60,49,62,55,78,96,29,22,24,13,
  14,11,11,18,12,12,30,52,52,44,28,28,20,56,40,31,50,40,46,42,
  29,19,36,25,22,17,19,26,30,20,15,21,11,8,8,19,5,8,8,11,
  11,8,3,9,5,4,7,3,6,3,5,4,5,6};
static int BASE[114];      // seçili kodeks (sûre sırasıyla)
static int KODEKS = 129;
static int SA_TOT;         // toplam ayet (129: 6236, 127: 6234)
static long S_OBS;         // Σ n·a(n) gözlenen
static const long SN_TOT = 6555;
static const int MEDINAN[28] = {2,3,4,5,8,9,13,22,24,33,47,48,49,55,57,58,59,60,61,62,63,64,65,66,76,98,99,110};
static const int MUQ[29] = {2,3,7,10,11,12,13,14,15,19,20,26,27,28,29,30,31,32,36,38,40,41,42,43,44,45,46,50,68};
static int ISMD[115], ISMQ[115];

// ---------------- RNG (robot4_m ile birebir) ----------------
typedef struct { uint64_t s[4]; } rng_t;
static inline uint64_t rotl(const uint64_t x, int k){ return (x << k) | (x >> (64 - k)); }
static inline uint64_t xnext(rng_t *r){ uint64_t *s=r->s; const uint64_t res=rotl(s[0]+s[3],23)+s[0]; const uint64_t t=s[1]<<17;
  s[2]^=s[0]; s[3]^=s[1]; s[1]^=s[2]; s[0]^=s[3]; s[2]^=t; s[3]=rotl(s[3],45); return res; }
static void rseed(rng_t *r, uint64_t seed){ uint64_t z=seed; for(int i=0;i<4;i++){ z+=0x9E3779B97F4A7C15ULL; uint64_t x=z;
  x=(x^(x>>30))*0xBF58476D1CE4E5B9ULL; x=(x^(x>>27))*0x94D049BB133111EBULL; r->s[i]=x^(x>>31);} }
static inline uint32_t bounded(rng_t *r, uint32_t n){ return (uint32_t)(((unsigned __int128)xnext(r)*n)>>64); }
static inline double unif(rng_t *r){ return (xnext(r)>>11)*(1.0/9007199254740992.0); }
static inline double gauss(rng_t *r){ double u1=unif(r),u2=unif(r); if(u1<1e-300)u1=1e-300; return sqrt(-2.0*log(u1))*cos(6.283185307179586*u2); }

// ---------------- kodeks ----------------
static void kodeks_sec(int k){
  KODEKS=k; memcpy(BASE,AYAT129,sizeof BASE); if(k==127) BASE[8]=127; else if(k!=129){ fprintf(stderr,"kodeks 127|129\n"); exit(1); }
  SA_TOT=0; S_OBS=0; for(int i=0;i<114;i++){ SA_TOT+=BASE[i]; S_OBS+=(long)(i+1)*BASE[i]; }
  memset(ISMD,0,sizeof ISMD); memset(ISMQ,0,sizeof ISMQ);
  for(int i=0;i<28;i++) ISMD[MEDINAN[i]]=1; for(int i=0;i<29;i++) ISMQ[MUQ[i]]=1;
}

// ---------------- olgular: robot4_m init_facts() ile aynı sıra ----------------
#define NF 128
#define NNODE 23
#define NCTX 41
#define MLENS 19
typedef unsigned __int128 u128;
#define BIT(f) (((u128)1)<<(f))
static inline int ctz128(u128 b){ uint64_t lo=(uint64_t)b; if(lo) return __builtin_ctzll(lo); return 64+__builtin_ctzll((uint64_t)(b>>64)); }
static inline int pop128(u128 b){ return __builtin_popcountll((uint64_t)b)+__builtin_popcountll((uint64_t)(b>>64)); }
static const char *NODENAME[NNODE]={"kitap-terazi","F-boy","F-toplam","F-çapraz","F-yarı/hücre","F-katsayı","G-boy","G-toplam","G-çapraz","G-yarı/hücre","G-katsayı","U-boy/ilişki","U-bölünebilirlik","U-güzel","G-ortalama",
  "Mekkî/Medenî","Mukattaa","Kümülatif/pencere","Ayna-çiftleri","Önek/blok","a-ailesi/n-konum","Tekrar","Matris"};
static const char *FNAME[NF]; static int FCTX[NF]; static int FNODE[NF]; static int NFACT=0; static int CUR_NODE=0; static int NODE_NF[NNODE];
static int fid(const char *nm, int ctx){ FNAME[NFACT]=nm; FCTX[NFACT]=ctx; FNODE[NFACT]=CUR_NODE; NODE_NF[CUR_NODE]++; return NFACT++; }
static int B_CNT, B_EQ[11];
static const char *BEQN[11] = {"vE=Σa","vE=Σn","vO=Σa","vO=Σn","vE=vO","aE=aO","nE=nO","nE=aE","nO=aO","nE=aO","nO=aE"};
static int K1[2],K2[2],S_M2[2],S_SQ[2],S_114[2],X_NA[2],X_N0[2],X_NSQ[2],X_ASQ[2],H_EQV[2],H_NEARV[2],H_M2V[2],H_EQN[2],H_NEARN[2],H_M2N[2],DCELL[2],BAND[2],AP2[2];
static int U_M,U_2M,U_DIS,U_I1,U_A,U_N,U_V,U_W,U_VPP,U_ASQ,R_EQ,G_AVG;
static int MD_F0,MD_FH,MD_G0,MD_GH,MD_U0,MD_UH,MD_BAL,MD_DIV;
static int MQ_F0,MQ_FH,MQ_G0,MQ_GH,MQ_U0,MQ_UH,MQ_BAL,MQ_DIV,MQ_DIVA;
static int W_STOP,W_FLAT,W_HI,W_LO,W_VSTOP;
static int MR_V,MR_A,MR_VEQ;
static int BL_HEQ,BL_HM2,BL_THIRD,BL_PREF;
static int AF_N,AF_NSQ,NP_A,NP_ASQ;
static int RP_F,RP_MODEN,RP_BOOK;
static int MX_P,MX_R,MX_PR;
static char NAMEBUF[16][40];
static char NB2[2][18][40];
static void init_facts(void){
  NFACT=0; memset(NODE_NF,0,sizeof NODE_NF);
  CUR_NODE=0; B_CNT=fid("kitap: |E|=|O|",0);
  for(int j=0;j<11;j++){ snprintf(NAMEBUF[j],40,"kitap: |E|=|O| & %s",BEQN[j]); B_EQ[j]=fid(NAMEBUF[j],0); }
  const char *X[2]={"F","G"};
  for(int x=0;x<2;x++){ int c=x==0?1:2; int i=0; int b0=x==0?1:6;
    #define MK(var,txt,ctx) snprintf(NB2[x][i],40,"%s: %s",X[x],txt); var[x]=fid(NB2[x][i],ctx); i++;
    CUR_NODE=b0;   MK(K1,"k≥2E",0) MK(K2,"k=m",0)
    CUR_NODE=b0+1; MK(S_M2,"m²|Σq",c) MK(S_SQ,"Σq kare",c) MK(S_114,"114|Σq",c)
    CUR_NODE=b0+2; MK(X_NA,"Σn=Σa",c) MK(X_N0,"m|Σn",c) MK(X_NSQ,"Σn kare",c) MK(X_ASQ,"Σa kare",c)
    CUR_NODE=b0+3; MK(H_EQV,"v-yarılar eşit",c) MK(H_NEARV,"v-yarılar Δ∈{m²,2m²}",c) MK(H_M2V,"v-yarılar m²|",c)
                   MK(H_EQN,"n-yarılar eşit",c) MK(H_NEARN,"n-yarılar Δ∈{m²,2m²}",c) MK(H_M2N,"n-yarılar m²|",c) MK(DCELL,"hücreler dengeli",c)
    CUR_NODE=b0+4; MK(BAND,"katsayı bandı",c) MK(AP2,"katsayı adım-2",c)
    #undef MK
  }
  CUR_NODE=11; U_M=fid("U: |U|=m",1); U_2M=fid("U: |U|=2m",1); U_DIS=fid("U: F∩G=∅",1); U_I1=fid("U: |F∩G|=1",1); R_EQ=fid("R: kF=kG",0);
  CUR_NODE=12; U_A=fid("U: m|Σa(U)",1); U_N=fid("U: m|Σn(U)",1); U_V=fid("U: m|Σv(U)",1); U_W=fid("U: m|Σw(U)",1);
  CUR_NODE=13; U_VPP=fid("U: Σv(U) kare/asal-kuvvet",1); U_ASQ=fid("U: Σa(U) kare",1);
  CUR_NODE=14; G_AVG=fid("G: Σw=m·kG",2);
  CUR_NODE=15; MD_F0=fid("MD: F∩Medenî=∅ (kF≥4)",1); MD_FH=fid("MD: |F∩Medenî|≥2×bekl.",1); MD_G0=fid("MD: G∩Medenî=∅ (kG≥4)",2); MD_GH=fid("MD: |G∩Medenî|≥2×bekl.",2);
               MD_U0=fid("MD: U∩Medenî=∅ (|U|≥4)",1); MD_UH=fid("MD: |U∩Medenî|≥2×bekl.",1); MD_BAL=fid("MD: Medenî v-yarılara 14/14",0); MD_DIV=fid("MD: m|Σv(Medenî)",0);
  CUR_NODE=16; MQ_F0=fid("MQ: F∩MQ=∅ (kF≥4)",1); MQ_FH=fid("MQ: |F∩MQ|≥2×bekl.",1); MQ_G0=fid("MQ: G∩MQ=∅ (kG≥4)",2); MQ_GH=fid("MQ: |G∩MQ|≥2×bekl.",2);
               MQ_U0=fid("MQ: U∩MQ=∅ (|U|≥4)",1); MQ_UH=fid("MQ: |U∩MQ|≥2×bekl.",1); MQ_BAL=fid("MQ: MQ v-yarılara 14/15",0); MQ_DIV=fid("MQ: m|Σv(MQ)",0); MQ_DIVA=fid("MQ: m|Σa(MQ)",0);
  CUR_NODE=17; W_STOP=fid("W: kümülatif-a durakları ≥2E",0); W_FLAT=fid("W: kalıntı yürüyüşü düz",0); W_HI=fid("W: m-bölünen pencere çok",0); W_LO=fid("W: m-bölünen pencere az",0); W_VSTOP=fid("W: kümülatif-v durakları ≥2E",0);
  CUR_NODE=18; MR_V=fid("AY: m|v(n)+v(115−n) çiftleri ≥2E",0); MR_A=fid("AY: a(n)=a(115−n) çifti ≥3",0); MR_VEQ=fid("AY: v(n)=v(115−n) çifti ≥2",0);
  CUR_NODE=19; BL_HEQ=fid("BL: Σv(1..57)=Σv(58..114)",0); BL_HM2=fid("BL: m²|Σv(1..57)",0); BL_THIRD=fid("BL: üç 38'lik blok m|Σv",0); BL_PREF=fid("BL: ilk 38 ve 76'da v-parite dengesi",0);
  CUR_NODE=20; AF_N=fid("AF: m|Σn{a≡0}",0); AF_NSQ=fid("AF: Σn{a≡0} kare",0); NP_A=fid("NP: m|Σa{n≡0}",0); NP_ASQ=fid("NP: Σa{n≡0} kare",0);
  CUR_NODE=21; RP_F=fid("RP: F'de aynı v ≥5 kez",1); RP_MODEN=fid("RP: F'nin tepe değeri v=114 (≥2)",1); RP_BOOK=fid("RP: kitapta aynı v ≥6 kez",0);
  CUR_NODE=22; MX_P=fid("MX: P matrisi ilkel",0); MX_R=fid("MX: R matrisi ilkel",0); MX_PR=fid("MX: P ve R ilkel",0);
}
static long PP[200]; static int nPP=0;
static inline long labs_(long x){ return x<0?-x:x; }
static inline int is_sq(long x){ x=labs_(x); if(x==0) return 0; long r=(long)floor(sqrt((double)x)); return (r*r==x)||((r+1)*(r+1)==x); }
static inline int is_pp(long x){ x=labs_(x); for(int i=0;i<nPP;i++) if(PP[i]==x) return 1; return 0; }
// mod-19 matris ilkelliği (robot ile aynı; olgular kapalı olsa da bit eşitliği için hesaplanabilir)
static int PF19[8], NPF19=0;
static inline void mmul(const int *A, const int *B, int *C, int m){ int c0=(A[0]*B[0]+A[1]*B[2])%m, c1=(A[0]*B[1]+A[1]*B[3])%m, c2=(A[2]*B[0]+A[3]*B[2])%m, c3=(A[2]*B[1]+A[3]*B[3])%m; C[0]=c0;C[1]=c1;C[2]=c2;C[3]=c3; }
static inline void mpow(const int *A, long e, int *R, int m){ int X[4]={A[0],A[1],A[2],A[3]}; R[0]=1;R[1]=0;R[2]=0;R[3]=1; while(e){ if(e&1) mmul(R,X,R,m); mmul(X,X,X,m); e>>=1; } }
static inline int primitive(long a00,long a01,long a10,long a11,int m){
  int A[4]={(int)(((a00%m)+m)%m),(int)(((a01%m)+m)%m),(int)(((a10%m)+m)%m),(int)(((a11%m)+m)%m)};
  if(((long)A[0]*A[3]-(long)A[1]*A[2])%m==0) return 0;
  { long tr=(A[0]+A[3])%m, de=(((long)A[0]*A[3]-(long)A[1]*A[2])%m+m)%m; long disc=((tr*tr-4*de)%m+m)%m;
    if(disc==0) return 0; long e=(m-1)/2, r=1, b=disc%m; while(e){ if(e&1) r=r*b%m; b=b*b%m; e>>=1; } if(r==1) return 0; }
  long ord=(long)m*m-1; int R[4]; mpow(A,ord,R,m); if(!(R[0]==1&&R[1]==0&&R[2]==0&&R[3]==1)) return 0;
  for(int i=0;i<NPF19;i++){ mpow(A,ord/PF19[i],R,m); if(R[0]==1&&R[1]==0&&R[2]==0&&R[3]==1) return 0; }
  return 1;
}
static void init_ortak(int kodeks){
  kodeks_sec(kodeks); init_facts();
  nPP=0; for(int p=2;p<=7;p++){ if(p==4||p==6) continue; long v=(long)p*p*p; while(v<1000000){ PP[nPP++]=v; v*=p; } }
  long o=19L*19-1; NPF19=0; for(long q=2;q<=o;q++){ if(o%q==0){ int isp=1; for(long d=2;d*d<=q;d++) if(q%d==0){isp=0;break;} if(isp) PF19[NPF19++]=(int)q; } }
}

typedef struct { u128 bits; int kF,kG; int same; } fres_t;
static inline void family_facts(int x, int m, double E, int k, const long *q, const int *nn, const int *aa, u128 *bits){
  long mm=(long)m*m;
  if(k>=2*E) *bits|=BIT(K1[x]);
  if(k==m) *bits|=BIT(K2[x]);
  if(k==0) return;
  long sq=0,sn=0,sa=0; for(int i=0;i<k;i++){ sq+=q[i]; sn+=nn[i]; sa+=aa[i]; }
  if(sq%mm==0) *bits|=BIT(S_M2[x]);
  if(is_sq(sq)) *bits|=BIT(S_SQ[x]);
  if(sq!=0 && sq%114==0) *bits|=BIT(S_114[x]);
  if(k>=2 && sn==sa) *bits|=BIT(X_NA[x]);
  if(sn%m==0) *bits|=BIT(X_N0[x]);
  if(is_sq(sn)) *bits|=BIT(X_NSQ[x]);
  if(is_sq(sa)) *bits|=BIT(X_ASQ[x]);
  int kv0=0,kn0=0; long qv0=0,qn0=0; int kc[4]={0,0,0,0}; long sc[4]={0,0,0,0};
  for(int i=0;i<k;i++){ int pv=(q[i]%2==0)?0:1; int pn=(nn[i]%2==0)?0:1;
    if(!pv){kv0++; qv0+=q[i];} if(!pn){kn0++; qn0+=q[i];} int ci=pv*2+pn; kc[ci]++; sc[ci]+=q[i]/m; }
  long qv1=sq-qv0, qn1=sq-qn0;
  if(2*kv0==k){ long d=labs_(qv0-qv1); if(d==0) *bits|=BIT(H_EQV[x]); if(d==mm||d==2*mm) *bits|=BIT(H_NEARV[x]); }
  if(kv0>0 && kv0<k && qv0%mm==0 && qv1%mm==0) *bits|=BIT(H_M2V[x]);
  if(2*kn0==k){ long d=labs_(qn0-qn1); if(d==0) *bits|=BIT(H_EQN[x]); if(d==mm||d==2*mm) *bits|=BIT(H_NEARN[x]); }
  if(kn0>0 && kn0<k && qn0%mm==0 && qn1%mm==0) *bits|=BIT(H_M2N[x]);
  if(2*kv0==k && qv0==qv1 && k%4==0 && kc[0]==k/4 && kc[1]==k/4 && kc[2]==k/4 && kc[3]==k/4 && labs_(sc[0]-sc[1])<=2 && labs_(sc[2]-sc[3])<=2) *bits|=BIT(DCELL[x]);
  if(k>=3){ long c[128]; for(int i=0;i<k;i++) c[i]=q[i]/m;
    for(int i=1;i<k;i++){ long y=c[i]; int j=i-1; while(j>=0&&c[j]>y){c[j+1]=c[j];j--;} c[j+1]=y; }
    int nd=1; for(int i=1;i<k;i++) if(c[i]!=c[i-1]) nd++; long span=c[k-1]-c[0];
    if(nd==span+1) *bits|=BIT(BAND[x]);
    int samep=1; long p0=((c[0]%2)+2)%2; for(int i=0;i<k;i++) if((((c[i]%2)+2)%2)!=p0){samep=0;break;}
    if(samep && nd>=2 && span%2==0 && nd==span/2+1) *bits|=BIT(AP2[x]); }
}
// Sabit mercek (1,1,19): robot4_m facts(a, LI_Q, R) ile aynı. mx=0 ise matris olguları hesaplanmaz (kapalı).
static void facts19(const int *a, fres_t *R, int mx){
  const int m=19; double E=114.0/m; u128 bits=0;
  long qF[114],qG[114]; int nF[114],aF[114],nG[114],aG[114]; int kF=0,kG=0;
  int e_cnt=0; long vE=0,nE=0,aE=0, SA=0,SV=0; const long SN=6555;
  int kU=0,kI=0; long saU=0,snU=0,svU=0,swU=0;
  long v_[114];
  int mdF=0,mdG=0,mdU=0,mqF=0,mqG=0,mqU=0,mdE=0,mqE=0; long svMD=0,svMQ=0,saMQ=0;
  for(int i=0;i<114;i++){
    int n=i+1; long v=n+a[i], w=n-a[i]; v_[i]=v;
    SA+=a[i]; SV+=v; int ev=(v%2==0);
    if(ev){ e_cnt++; vE+=v; nE+=n; aE+=a[i]; }
    int inF=(v%m==0), inG=(w%m==0);
    if(inF){ qF[kF]=v; nF[kF]=n; aF[kF]=a[i]; kF++; }
    if(inG){ qG[kG]=w; nG[kG]=n; aG[kG]=a[i]; kG++; }
    if(inF||inG){ kU++; saU+=a[i]; snU+=n; svU+=v; swU+=w; }
    if(inF&&inG) kI++;
    if(ISMD[n]){ if(inF) mdF++; if(inG) mdG++; if(inF||inG) mdU++; if(ev) mdE++; svMD+=v; }
    if(ISMQ[n]){ if(inF) mqF++; if(inG) mqG++; if(inF||inG) mqU++; if(ev) mqE++; svMQ+=v; saMQ+=a[i]; }
  }
  long vO=SV-vE, nO=SN-nE, aO=SA-aE;
  if(e_cnt==57){ bits|=BIT(B_CNT);
    int eq[11]={vE==SA, vE==SN, vO==SA, vO==SN, vE==vO, aE==aO, nE==nO, nE==aE, nO==aO, nE==aO, nO==aE};
    for(int j=0;j<11;j++) if(eq[j]) bits|=BIT(B_EQ[j]); }
  family_facts(0,m,E,kF,qF,nF,aF,&bits);
  family_facts(1,m,E,kG,qG,nG,aG,&bits);
  if(kU==m) bits|=BIT(U_M);
  if(kU==2*m) bits|=BIT(U_2M);
  if(kF>0 && kG>0 && kI==0) bits|=BIT(U_DIS);
  if(kI==1) bits|=BIT(U_I1);
  if(kU>0){ if(saU%m==0) bits|=BIT(U_A); if(snU%m==0) bits|=BIT(U_N); if(svU%m==0) bits|=BIT(U_V); if(swU%m==0) bits|=BIT(U_W);
            if(is_sq(svU)||is_pp(svU)) bits|=BIT(U_VPP); if(is_sq(saU)) bits|=BIT(U_ASQ); }
  if(kF>0 && kF==kG) bits|=BIT(R_EQ);
  if(kG>0){ long sw=0; for(int i=0;i<kG;i++) sw+=qG[i]; if(sw==(long)m*kG) bits|=BIT(G_AVG); }
  if(kF>=4 && mdF==0) bits|=BIT(MD_F0);  if(mdF>=2 && mdF>=2.0*kF*28/114.0) bits|=BIT(MD_FH);
  if(kG>=4 && mdG==0) bits|=BIT(MD_G0);  if(mdG>=2 && mdG>=2.0*kG*28/114.0) bits|=BIT(MD_GH);
  if(kU>=4 && mdU==0) bits|=BIT(MD_U0);  if(mdU>=2 && mdU>=2.0*kU*28/114.0) bits|=BIT(MD_UH);
  if(mdE==14) bits|=BIT(MD_BAL); if(svMD%m==0) bits|=BIT(MD_DIV);
  if(kF>=4 && mqF==0) bits|=BIT(MQ_F0);  if(mqF>=2 && mqF>=2.0*kF*29/114.0) bits|=BIT(MQ_FH);
  if(kG>=4 && mqG==0) bits|=BIT(MQ_G0);  if(mqG>=2 && mqG>=2.0*kG*29/114.0) bits|=BIT(MQ_GH);
  if(kU>=4 && mqU==0) bits|=BIT(MQ_U0);  if(mqU>=2 && mqU>=2.0*kU*29/114.0) bits|=BIT(MQ_UH);
  if(mqE==14||mqE==15) bits|=BIT(MQ_BAL); if(svMQ%m==0) bits|=BIT(MQ_DIV); if(saMQ%m==0) bits|=BIT(MQ_DIVA);
  { int cnt[64]; memset(cnt,0,sizeof cnt); long S=0; int stops=0; cnt[0]=1; long T=0; int vstops=0;
    for(int i=0;i<114;i++){ S+=a[i]; int r=(int)(S%m); cnt[r]++; if(r==0) stops++; T+=v_[i]; if(T%m==0) vstops++; }
    if(stops>=2*E) bits|=BIT(W_STOP); if(vstops>=2*E) bits|=BIT(W_VSTOP);
    int mx_=0,mn=1<<30; long Nm=0; for(int r=0;r<m;r++){ if(cnt[r]>mx_) mx_=cnt[r]; if(cnt[r]<mn) mn=cnt[r]; Nm+=(long)cnt[r]*(cnt[r]-1)/2; }
    if(mx_-mn<=1) bits|=BIT(W_FLAT); double EN=6555.0/m; if(Nm>=1.5*EN) bits|=BIT(W_HI); if(Nm<=0.6*EN) bits|=BIT(W_LO); }
  { int cv=0,ca=0,ce=0; for(int n=1;n<=57;n++){ int i=n-1,j=114-n; if((v_[i]+v_[j])%m==0) cv++; if(a[i]==a[j]) ca++; if(v_[i]==v_[j]) ce++; }
    if(cv>=3 && cv>=2*57.0/m) bits|=BIT(MR_V); if(ca>=3) bits|=BIT(MR_A); if(ce>=2) bits|=BIT(MR_VEQ); }
  { long h1=0,h2=0; for(int i=0;i<57;i++) h1+=v_[i]; for(int i=57;i<114;i++) h2+=v_[i];
    if(h1==h2) bits|=BIT(BL_HEQ); if(h1%((long)m*m)==0) bits|=BIT(BL_HM2);
    long t1=0,t2=0,t3=0; for(int i=0;i<38;i++) t1+=v_[i]; for(int i=38;i<76;i++) t2+=v_[i]; for(int i=76;i<114;i++) t3+=v_[i];
    if(t1%m==0 && t2%m==0 && t3%m==0) bits|=BIT(BL_THIRD);
    { int p38=0,p76=0; for(int i=0;i<76;i++){ if(v_[i]%2==0){ if(i<38) p38++; p76++; } } if(p38==19 && p76==38) bits|=BIT(BL_PREF); } }
  { long sn=0; int ka=0; long sa=0; for(int i=0;i<114;i++){ if(a[i]%m==0){ ka++; sn+=i+1; } if((i+1)%m==0) sa+=a[i]; }
    if(ka>0 && sn%m==0) bits|=BIT(AF_N); if(ka>0 && is_sq(sn)) bits|=BIT(AF_NSQ);
    if(sa%m==0) bits|=BIT(NP_A); if(is_sq(sa)) bits|=BIT(NP_ASQ); }
  { if(kF>=5){ long c[114]; for(int i=0;i<kF;i++) c[i]=qF[i]; for(int i=1;i<kF;i++){ long y=c[i]; int j=i-1; while(j>=0&&c[j]>y){c[j+1]=c[j];j--;} c[j+1]=y; }
      int best=1,run=1; long mode=c[0]; for(int i=1;i<kF;i++){ if(c[i]==c[i-1]) run++; else run=1; if(run>best){best=run; mode=c[i];} }
      if(best>=5) bits|=BIT(RP_F); if(best>=2 && mode==114) bits|=BIT(RP_MODEN); }
    else if(kF>=2){ int best=1; long mode=0; for(int i=0;i<kF;i++){ int c=0; for(int j=0;j<kF;j++) if(qF[j]==qF[i]) c++; if(c>best){best=c; mode=qF[i];} } if(best>=2 && mode==114) bits|=BIT(RP_MODEN); }
    { static __thread int cntv[8192], stv[8192], stamp=0; stamp++; int best=1;
      for(int i=0;i<114;i++){ int ix=(int)v_[i]+4096; if(ix<0||ix>=8192) continue; if(stv[ix]!=stamp){ stv[ix]=stamp; cntv[ix]=0; } if(++cntv[ix]>best) best=cntv[ix]; }
      if(best>=6) bits|=BIT(RP_BOOK); } }
  if(mx){ long snF=0,saF=0; for(int i=0;i<kF;i++){ snF+=nF[i]; saF+=aF[i]; }
    int p=primitive(nE,aE,nO,aO,m); int r=primitive(snF,saF,SN-snF,SA-saF,m);
    if(p) bits|=BIT(MX_P); if(r) bits|=BIT(MX_R); if(p&&r) bits|=BIT(MX_PR); }
  R->bits=bits; R->kF=kF; R->kG=kG; R->same=(kI==kF && kI==kG);
}

// ---------------- olaylar (kesif4_motor tanımları; Σa kodeksten) ----------------
// bayraklar
enum { EV_A=1, EV_B=2, EV_C=4, EV_D=8, EV_E=16, EV_G=32, EV_W=64, EV_O3=128 };
// O1=B (12 üye, Σt=1444), O2: C (6/6, 722/722), O3: tek-t kolunun iki hücresi 361 (katsayı 19+19), O4=A (terazi).
static inline int olaylar(const int *a){
  int evenCnt=0; long sumTEven=0; int famCnt=0, coeffSum=0;
  int cellCnt[4]={0,0,0,0}, cellSum[4]={0,0,0,0}; uint8_t histE[24], histO[24]; memset(histE,0,24); memset(histO,0,24);
  int cellVals[4][4]; int cellN[4]={0,0,0,0}; int nu=0, uniCnt=0; long uniSa=0;
  for(int i=0;i<114;i++){ int n=i+1, t=n+a[i], u=n-a[i]; int fu=(u%19==0), ft=(t%19==0);
    if(fu) nu++; if(ft||fu){ uniCnt++; uniSa+=a[i]; }
    int te=((t&1)==0); if(te){ evenCnt++; sumTEven+=t; }
    if(ft){ int c=t/19; famCnt++; coeffSum+=c; int idx=(te?0:2)+((n&1)?1:0); cellCnt[idx]++; cellSum[idx]+=c;
      if(cellN[idx]<4) cellVals[idx][cellN[idx]++]=c; if(c<24){ if(te) histE[c]++; else histO[c]++; } } }
  int f=0;
  if(evenCnt==57 && sumTEven==SA_TOT) f|=EV_A;
  int B_=(famCnt==12 && coeffSum==76); if(B_) f|=EV_B;
  int C_=(B_ && cellCnt[0]+cellCnt[1]==6 && cellSum[0]+cellSum[1]==38); if(C_) f|=EV_C;
  if(C_ && cellSum[2]==19 && cellSum[3]==19) f|=EV_O3;
  if(C_){ int cnts_ok=(cellCnt[0]==3&&cellCnt[1]==3&&cellCnt[2]==3&&cellCnt[3]==3);
    int odd_ok=(cellSum[2]==19&&cellSum[3]==19); int even_ok=((cellSum[0]==18&&cellSum[1]==20)||(cellSum[0]==20&&cellSum[1]==18));
    int D_=cnts_ok&&odd_ok&&even_ok; int E_=(histE[6]==5&&histE[8]==1&&histO[5]==3&&histO[7]==2&&histO[9]==1);
    if(D_) f|=EV_D; if(E_) f|=EV_E;
    if(D_&&E_){ int id[4]; for(int k=0;k<4;k++){ int s=0,q=0; for(int mm=0;mm<cellN[k];mm++){ s+=cellVals[k][mm]; q+=cellVals[k][mm]*cellVals[k][mm]; } id[k]=s*1000+q; }
      int evenPair=((id[0]==18108&&id[1]==20136)||(id[0]==20136&&id[1]==18108)); int oddPair=((id[2]==19131&&id[3]==19123)||(id[2]==19123&&id[3]==19131));
      if(evenPair&&oddPair) f|=EV_G; } }
  if(nu==7 && uniCnt==19 && (uniSa%19)==0) f|=EV_W;
  return f;
}
#define OK_O(f) (((f)&(EV_C|EV_O3))==(EV_C|EV_O3))

// ---------------- robotun sıfırları (birebir port) ----------------
#define NCH 64
typedef struct { char null; double par; rng_t r; int a[114]; int ch[NCH][114]; long chS[NCH]; int cur;
  // Mallows (Lehmer kodu)
  int ref[114]; double q; int nal[115]; double *alP; int *alA;  } sampler_t;
static void mallows_hazirla(sampler_t *Z);
static void sampler_init(sampler_t *Z, char null, double par, uint64_t seed){
  memset(Z,0,sizeof *Z); Z->null=null; Z->par=par; rseed(&Z->r,seed); memcpy(Z->a,BASE,sizeof Z->a); Z->cur=0;
  if(null=='M'){
    int srt[114]; memcpy(srt,BASE,sizeof srt); for(int i=1;i<114;i++){ int x=srt[i]; int j=i-1; while(j>=0&&srt[j]<x){srt[j+1]=srt[j];j--;} srt[j+1]=x; }
    for(int c=0;c<NCH;c++){ int *a=Z->ch[c]; memcpy(a,srt,sizeof srt); long S=0; for(int i=0;i<114;i++) S+=(long)(i+1)*a[i];
      for(int b=0;b<30000;b++){ int i=(int)bounded(&Z->r,114), j=(int)bounded(&Z->r,114); long dS=(long)((i+1)-(j+1))*(a[j]-a[i]); if(S+dS<=S_OBS){ int t=a[i]; a[i]=a[j]; a[j]=t; S+=dS; } }
      Z->chS[c]=S; } }
  if(null=='L'){ Z->q=par; mallows_hazirla(Z); }
}
static void mallows_ornek(sampler_t *Z, int *a);
static void sampler_next(sampler_t *Z){
  int *a=Z->a; rng_t *r=&Z->r;
  if(Z->null=='U'){ for(int i=113;i>0;i--){ int j=(int)bounded(r,(uint32_t)(i+1)); int t=a[i]; a[i]=a[j]; a[j]=t; } }
  else if(Z->null=='J'){ double key[114]; int idx[114]; for(int i=0;i<114;i++){ key[i]=(i+1)+Z->par*gauss(r); idx[i]=i; }
    for(int i=1;i<114;i++){ int x=idx[i]; double kx=key[x]; int j=i-1; while(j>=0&&key[idx[j]]>kx){ idx[j+1]=idx[j]; j--; } idx[j+1]=x; }
    for(int k=0;k<114;k++) a[k]=BASE[idx[k]]; }
  else if(Z->null=='M'){ int thin=(int)Z->par; if(thin<1)thin=1; int c=Z->cur; Z->cur=(Z->cur+1)%NCH; int *b=Z->ch[c]; long S=Z->chS[c];
    for(int st=0;st<thin;st++){ int i=(int)bounded(r,114), j=(int)bounded(r,114); long dS=(long)((i+1)-(j+1))*(b[j]-b[i]); if(S+dS<=S_OBS){ int t=b[i]; b[i]=b[j]; b[j]=t; S+=dS; } }
    Z->chS[c]=S; memcpy(a,b,114*sizeof(int)); }
  else if(Z->null=='L'){ mallows_ornek(Z,a); }
}

// ---------------- Mallows: P(diziliş) ∝ q^I, I = büyükten küçüğe merkeze aykırı KESİN çift sayısı ----------------
// Etiketli Mallows (Lehmer kodu: V_p ~ kesik geometrik(q, 115−p), bağımsız) değerlere izdüşürülünce
// eşit değerlerin iç terslikleri her dizilişe aynı Π[m_g]_q! çarpanını verir → izdüşüm tam olarak ∝ q^{I_kesin}.
static int REF_DESC[114];
static void ref_hazirla(void){ memcpy(REF_DESC,BASE,sizeof REF_DESC); for(int i=1;i<114;i++){ int x=REF_DESC[i]; int j=i-1; while(j>=0&&REF_DESC[j]<x){REF_DESC[j+1]=REF_DESC[j];j--;} REF_DESC[j+1]=x; } }
static long ters_kesin(const int *a){ long I=0; for(int i=0;i<114;i++) for(int j=i+1;j<114;j++) if(a[i]<a[j]) I++; return I; }
static double kg_ort(double q, int R){ double s=0,w=0,p=1; for(int j=0;j<R;j++){ s+=j*p; w+=p; p*=q; } return s/w; }
static double mallows_beklenen(double q){ // E[I_kesin]
  double E=0; for(int R=1;R<=114;R++) E+=kg_ort(q,R);
  // eşit değer grupları
  int v[114]; memcpy(v,REF_DESC,sizeof v); int i=0; while(i<114){ int j=i; while(j<114&&v[j]==v[i]) j++; int mg=j-i; for(int R=1;R<=mg;R++) E-=kg_ort(q,R); i=j; }
  return E; }
static double mallows_q_ayarla(long Iobs){ double lo=0.5, hi=0.999999; for(int it=0;it<200;it++){ double mid=0.5*(lo+hi); if(mallows_beklenen(mid)<Iobs) lo=mid; else hi=mid; } return 0.5*(lo+hi); }
// Walker alias tabloları, R = 1..114
static void mallows_hazirla(sampler_t *Z){
  ref_hazirla(); memcpy(Z->ref,REF_DESC,sizeof Z->ref);
  int tot=0; for(int R=1;R<=114;R++){ Z->nal[R]=tot; tot+=R; }
  Z->alP=malloc(sizeof(double)*tot); Z->alA=malloc(sizeof(int)*tot);
  for(int R=1;R<=114;R++){ double *P=Z->alP+Z->nal[R]; int *A=Z->alA+Z->nal[R]; double w[114], s=0, p=1; for(int j=0;j<R;j++){ w[j]=p; s+=p; p*=Z->q; }
    double sc[114]; int small[114], large[114], ns=0, nl=0; for(int j=0;j<R;j++){ sc[j]=w[j]/s*R; if(sc[j]<1.0) small[ns++]=j; else large[nl++]=j; }
    while(ns>0 && nl>0){ int l=small[--ns], g=large[--nl]; P[l]=sc[l]; A[l]=g; sc[g]=(sc[g]+sc[l])-1.0; if(sc[g]<1.0) small[ns++]=g; else large[nl++]=g; }
    while(nl>0){ int g=large[--nl]; P[g]=1.0; A[g]=g; } while(ns>0){ int l=small[--ns]; P[l]=1.0; A[l]=l; } }
}
static inline int alias_cek(sampler_t *Z, int R){ int j=(int)bounded(&Z->r,(uint32_t)R); double u=unif(&Z->r);
  const double *P=Z->alP+Z->nal[R]; const int *A=Z->alA+Z->nal[R]; return (u<P[j])?j:A[j]; }
static void mallows_ornek(sampler_t *Z, int *a){
  // kalan öğeler TERS sırada tutulur: rem[R-1] merkez sırasında ilk kalan (en büyük); V'inci (0 tabanlı) rem[R-1-V]
  int rem[114]; for(int i=0;i<114;i++) rem[i]=Z->ref[113-i];
  for(int p=0;p<114;p++){ int R=114-p; int V=alias_cek(Z,R); int ix=R-1-V; a[p]=rem[ix]; memmove(rem+ix,rem+ix+1,sizeof(int)*(R-1-ix)); }
}
#endif
