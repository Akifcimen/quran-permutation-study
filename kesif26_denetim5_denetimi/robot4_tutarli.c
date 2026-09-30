// ROBOT NUMEROLOG — DEFTER BATARYASI (robot.c'nin genişletilmiş sürümü)
// robot.c'deki 60 olgu (15 düğüm) + yazarın defterinde incelediği, çoğu Kur'an için BOŞ çıkan aileler:
//   15 Mekkî/Medenî (kesif6 D-aileleri), 16 Mukattaa (kesif7 E-aileleri), 17 Kümülatif/pencere (kesif8 Q-aileleri),
//   18 Ayna çiftleri, 19 Önek/blok (kesif2 panoraması), 20 a-ailesi / n-konumları, 21 Tekrar (t=114 beşlisi vb.),
//   22 Matris ilkelliği (kaynak dokümanın P/R keşfi; asal m).
// Tüm olgular her merceğe genelleştirilmiştir; olasılıklar bağlama koşullu sıfır tablolarından gelir.
//   ./robot2 table <null> <seed> <N> <threads> <tabfile>
//   ./robot2 score <null> <seed> <N> <threads> <tabfile> <outprefix>
//   ./robot2 facts <tabfile> [liste] [ad]
//   ./robot2 demofix <null> <seed> <N> <tabfile> <K>
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
static int BASE[114];
// Medenî sûreler (quran.com revelation_place, 28 sûre) ve mukattaalı 29 sûre (kesif7 ön-kaydı)
static const int MEDINAN[28] = {2,3,4,5,8,9,13,22,24,33,47,48,49,55,57,58,59,60,61,62,63,64,65,66,76,98,99,110};
static const int MUQ[29] = {2,3,7,10,11,12,13,14,15,19,20,26,27,28,29,30,31,32,36,38,40,41,42,43,44,45,46,50,68};
static int ISMD[115], ISMQ[115];

#define NLIN 7
#define NFORM 17
static const int FORMS[NLIN][2] = {{1,1},{1,2},{1,3},{2,1},{2,3},{3,1},{3,2}};
static const char *NLNAME[10]={"n·a","n²+a","n+a²","n²+a²","n²−a²","ebob(n,a)","n·a+n+a","a²−n","n²−a","n·a−(n+a)"};
static inline long gcdl(long x,long y){ if(x<0)x=-x; if(y<0)y=-y; while(y){ long t=x%y; x=y; y=t; } return x; }
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
#define NF 128
typedef unsigned __int128 u128;
#define BIT(f) (((u128)1)<<(f))
static inline int ctz128(u128 b){ uint64_t lo=(uint64_t)b; if(lo) return __builtin_ctzll(lo); return 64+__builtin_ctzll((uint64_t)(b>>64)); }

#define NNODE 23
static const char *NODENAME[NNODE]={"kitap-terazi","F-boy","F-toplam","F-çapraz","F-yarı/hücre","F-katsayı","G-boy","G-toplam","G-çapraz","G-yarı/hücre","G-katsayı","U-boy/ilişki","U-bölünebilirlik","U-güzel","G-ortalama",
  "Mekkî/Medenî","Mukattaa","Kümülatif/pencere","Ayna-çiftleri","Önek/blok","a-ailesi/n-konum","Tekrar","Matris"};
static const char *FNAME[NF]; static int FCTX[NF]; static int FNODE[NF]; static int NFACT=0; static int CUR_NODE=0; static int NODE_NF[NNODE];
static int fid(const char *nm, int ctx){ FNAME[NFACT]=nm; FCTX[NFACT]=ctx; FNODE[NFACT]=CUR_NODE; NODE_NF[CUR_NODE]++; return NFACT++; }

static int B_CNT, B_EQ[11];
static const char *BEQN[11] = {"vE=Σa","vE=Σn","vO=Σa","vO=Σn","vE=vO","aE=aO","nE=nO","nE=aE","nO=aO","nE=aO","nO=aE"};
static int K1[2],K2[2],S_M2[2],S_SQ[2],S_114[2],X_NA[2],X_N0[2],X_NSQ[2],X_ASQ[2],H_EQV[2],H_NEARV[2],H_M2V[2],H_EQN[2],H_NEARN[2],H_M2N[2],DCELL[2],BAND[2],AP2[2];
static int U_M,U_2M,U_DIS,U_I1,U_A,U_N,U_V,U_W,U_VPP,U_ASQ,R_EQ,G_AVG;
// yeni
static int MD_F0,MD_FH,MD_G0,MD_GH,MD_U0,MD_UH,MD_BAL,MD_DIV;
static int MQ_F0,MQ_FH,MQ_G0,MQ_GH,MQ_U0,MQ_UH,MQ_BAL,MQ_DIV,MQ_DIVA;
static int W_STOP,W_FLAT,W_HI,W_LO,W_VSTOP;
static int MR_V,MR_A,MR_VEQ;
static int BL_HEQ,BL_HM2,BL_THIRD,BL_PREF;
static int AF_N,AF_NSQ,NP_A,NP_ASQ;
static int RP_F,RP_MODEN,RP_BOOK;
static int MX_P,MX_R,MX_PR;
static char NAMEBUF[16][40];

static void init_facts(void){
  CUR_NODE=0; B_CNT=fid("kitap: |E|=|O|",0);
  for(int j=0;j<11;j++){ snprintf(NAMEBUF[j],40,"kitap: |E|=|O| & %s",BEQN[j]); B_EQ[j]=fid(NAMEBUF[j],0); }
  const char *X[2]={"F","G"}; static char nb[2][18][40];
  for(int x=0;x<2;x++){ int c=x==0?1:2; int i=0; int b0=x==0?1:6;
    #define MK(var,txt,ctx) snprintf(nb[x][i],40,"%s: %s",X[x],txt); var[x]=fid(nb[x][i],ctx); i++;
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

// ---- mod-m 2x2 matris ilkelliği (m asal): mertebe = m²−1 ----
static int ISPRIME[64]; static int PF[64][8]; static int NPF[64];
static inline void mmul(const int *A, const int *B, int *C, int m){ int c0=(A[0]*B[0]+A[1]*B[2])%m, c1=(A[0]*B[1]+A[1]*B[3])%m, c2=(A[2]*B[0]+A[3]*B[2])%m, c3=(A[2]*B[1]+A[3]*B[3])%m; C[0]=c0;C[1]=c1;C[2]=c2;C[3]=c3; }
static inline void mpow(const int *A, long e, int *R, int m){ int X[4]={A[0],A[1],A[2],A[3]}; R[0]=1;R[1]=0;R[2]=0;R[3]=1; while(e){ if(e&1) mmul(R,X,R,m); mmul(X,X,X,m); e>>=1; } }
static inline int primitive(long a00,long a01,long a10,long a11,int m){
  int A[4]={(int)(((a00%m)+m)%m),(int)(((a01%m)+m)%m),(int)(((a10%m)+m)%m),(int)(((a11%m)+m)%m)};
  if(((long)A[0]*A[3]-(long)A[1]*A[2])%m==0) return 0;
  { // ilkel ⇒ karakteristik polinom F_m üzerinde indirgenemez ⇒ diskriminant karesel kalıntı DEĞİL (ucuz ön-test)
    long tr=(A[0]+A[3])%m, de=(((long)A[0]*A[3]-(long)A[1]*A[2])%m+m)%m; long disc=((tr*tr-4*de)%m+m)%m;
    if(disc==0) return 0; long e=(m-1)/2, r=1, b=disc%m; while(e){ if(e&1) r=r*b%m; b=b*b%m; e>>=1; } if(r==1) return 0; }
  long ord=(long)m*m-1; int R[4]; mpow(A,ord,R,m); if(!(R[0]==1&&R[1]==0&&R[2]==0&&R[3]==1)) return 0;
  for(int i=0;i<NPF[m];i++){ mpow(A,ord/PF[m][i],R,m); if(R[0]==1&&R[1]==0&&R[2]==0&&R[3]==1) return 0; }
  return 1;
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

static void facts(const int *a, int li, fres_t *R){
  int f=li/NMOD, m=MMIN+li%NMOD; int al=f<NLIN?FORMS[f][0]:1, be=f<NLIN?FORMS[f][1]:1;
  double E=114.0/m; u128 bits=0;
  long qF[114],qG[114]; int nF[114],aF[114],nG[114],aG[114]; int kF=0,kG=0;
  int e_cnt=0; long vE=0,nE=0,aE=0, SA=0,SV=0; const long SN=6555;
  int kU=0,kI=0; long saU=0,snU=0,svU=0,swU=0;
  int blind=(f<NLIN)?!((al&1)&&(be&1)):0;
  long v_[114]; char inFm[114], inGm[114];
  int mdF=0,mdG=0,mdU=0,mqF=0,mqG=0,mqU=0,mdE=0,mqE=0; long svMD=0,svMQ=0,saMQ=0;
  for(int i=0;i<114;i++){
    int n=i+1; long v,w; vw(f,n,a[i],&v,&w); v_[i]=v;
    SA+=a[i]; SV+=v; int ev=(v%2==0);
    if(ev){ e_cnt++; vE+=v; nE+=n; aE+=a[i]; }
    int inF=(v%m==0), inG=(w%m==0); inFm[i]=inF; inGm[i]=inG;
    if(inF){ qF[kF]=v; nF[kF]=n; aF[kF]=a[i]; kF++; }
    if(inG){ qG[kG]=w; nG[kG]=n; aG[kG]=a[i]; kG++; }
    if(inF||inG){ kU++; saU+=a[i]; snU+=n; svU+=v; swU+=w; }
    if(inF&&inG) kI++;
    if(ISMD[n]){ if(inF) mdF++; if(inG) mdG++; if(inF||inG) mdU++; if(ev) mdE++; svMD+=v; }
    if(ISMQ[n]){ if(inF) mqF++; if(inG) mqG++; if(inF||inG) mqU++; if(ev) mqE++; svMQ+=v; saMQ+=a[i]; }
  }
  long vO=SV-vE, nO=SN-nE, aO=SA-aE;
  int cnt_ok = blind ? 1 : (e_cnt==57);
  if(!blind && e_cnt==57) bits|=BIT(B_CNT);
  if(cnt_ok){ int eq[11]={vE==SA, vE==SN, vO==SA, vO==SN, vE==vO, aE==aO, nE==nO, nE==aE, nO==aO, nE==aO, nO==aE};
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
  // 15 Mekkî/Medenî (28/114), 16 Mukattaa (29/114)
  if(kF>=4 && mdF==0) bits|=BIT(MD_F0);  if(mdF>=2 && mdF>=2.0*kF*28/114.0) bits|=BIT(MD_FH);
  if(kG>=4 && mdG==0) bits|=BIT(MD_G0);  if(mdG>=2 && mdG>=2.0*kG*28/114.0) bits|=BIT(MD_GH);
  if(kU>=4 && mdU==0) bits|=BIT(MD_U0);  if(mdU>=2 && mdU>=2.0*kU*28/114.0) bits|=BIT(MD_UH);
  if(!blind && mdE==14) bits|=BIT(MD_BAL); if(svMD%m==0) bits|=BIT(MD_DIV);
  if(kF>=4 && mqF==0) bits|=BIT(MQ_F0);  if(mqF>=2 && mqF>=2.0*kF*29/114.0) bits|=BIT(MQ_FH);
  if(kG>=4 && mqG==0) bits|=BIT(MQ_G0);  if(mqG>=2 && mqG>=2.0*kG*29/114.0) bits|=BIT(MQ_GH);
  if(kU>=4 && mqU==0) bits|=BIT(MQ_U0);  if(mqU>=2 && mqU>=2.0*kU*29/114.0) bits|=BIT(MQ_UH);
  if(!blind && (mqE==14||mqE==15)) bits|=BIT(MQ_BAL); if(svMQ%m==0) bits|=BIT(MQ_DIV); if(saMQ%m==0) bits|=BIT(MQ_DIVA);
  // 17 kümülatif / pencere (a-toplamları; kesif8) + kümülatif v
  { int cnt[64]; memset(cnt,0,sizeof cnt); long S=0; int stops=0; cnt[0]=1; long T=0; int vstops=0;
    for(int i=0;i<114;i++){ S+=a[i]; int r=(int)(S%m); cnt[r]++; if(r==0) stops++; T+=v_[i]; if(T%m==0) vstops++; }
    if(stops>=2*E) bits|=BIT(W_STOP); if(vstops>=2*E) bits|=BIT(W_VSTOP);
    int mx=0,mn=1<<30; long Nm=0; for(int r=0;r<m;r++){ if(cnt[r]>mx) mx=cnt[r]; if(cnt[r]<mn) mn=cnt[r]; Nm+=(long)cnt[r]*(cnt[r]-1)/2; }
    if(mx-mn<=1) bits|=BIT(W_FLAT); double EN=6555.0/m; if(Nm>=1.5*EN) bits|=BIT(W_HI); if(Nm<=0.6*EN) bits|=BIT(W_LO); }
  // 18 ayna çiftleri
  { int cv=0,ca=0,ce=0; for(int n=1;n<=57;n++){ int i=n-1,j=114-n; if((v_[i]+v_[j])%m==0) cv++; if(a[i]==a[j]) ca++; if(v_[i]==v_[j]) ce++; }
    if(cv>=3 && cv>=2*57.0/m) bits|=BIT(MR_V); if(ca>=3) bits|=BIT(MR_A); if(ce>=2) bits|=BIT(MR_VEQ); }
  // 19 önek / blok
  { long h1=0,h2=0; for(int i=0;i<57;i++) h1+=v_[i]; for(int i=57;i<114;i++) h2+=v_[i];
    if(h1==h2) bits|=BIT(BL_HEQ); if(h1%((long)m*m)==0) bits|=BIT(BL_HM2);
    long t1=0,t2=0,t3=0; for(int i=0;i<38;i++) t1+=v_[i]; for(int i=38;i<76;i++) t2+=v_[i]; for(int i=76;i<114;i++) t3+=v_[i];
    if(t1%m==0 && t2%m==0 && t3%m==0) bits|=BIT(BL_THIRD);
    if(!blind){ int p38=0,p76=0; for(int i=0;i<76;i++){ if(v_[i]%2==0){ if(i<38) p38++; p76++; } } if(p38==19 && p76==38) bits|=BIT(BL_PREF); } }
  // 20 a-ailesi / n-konumları
  { long sn=0; int ka=0; long sa=0; for(int i=0;i<114;i++){ if(a[i]%m==0){ ka++; sn+=i+1; } if((i+1)%m==0) sa+=a[i]; }
    if(ka>0 && sn%m==0) bits|=BIT(AF_N); if(ka>0 && is_sq(sn)) bits|=BIT(AF_NSQ);
    if(sa%m==0) bits|=BIT(NP_A); if(is_sq(sa)) bits|=BIT(NP_ASQ); }
  // 21 tekrar
  { if(kF>=5){ long c[114]; for(int i=0;i<kF;i++) c[i]=qF[i]; for(int i=1;i<kF;i++){ long y=c[i]; int j=i-1; while(j>=0&&c[j]>y){c[j+1]=c[j];j--;} c[j+1]=y; }
      int best=1,run=1; long mode=c[0]; for(int i=1;i<kF;i++){ if(c[i]==c[i-1]) run++; else run=1; if(run>best){best=run; mode=c[i];} }
      if(best>=5) bits|=BIT(RP_F); if(best>=2 && mode==114) bits|=BIT(RP_MODEN); }
    else if(kF>=2){ int best=1; long mode=0; for(int i=0;i<kF;i++){ int c=0; for(int j=0;j<kF;j++) if(qF[j]==qF[i]) c++; if(c>best){best=c; mode=qF[i];} } if(best>=2 && mode==114) bits|=BIT(RP_MODEN); }
    { static __thread int cntv[8192], stv[8192], stamp=0; stamp++; int best=1;
      for(int i=0;i<114;i++){ int ix=(int)v_[i]+4096; if(ix<0||ix>=8192) continue; if(stv[ix]!=stamp){ stv[ix]=stamp; cntv[ix]=0; } if(++cntv[ix]>best) best=cntv[ix]; }
      if(best>=6) bits|=BIT(RP_BOOK); } }
  // 22 matris ilkelliği (asal m): P = [[Σn(E),Σa(E)],[Σn(O),Σa(O)]], R = [[Σn(F),Σa(F)],[Σn(F^c),Σa(F^c)]]
  if(ISPRIME[m] && m>2){ long snF=0,saF=0; for(int i=0;i<kF;i++){ snF+=nF[i]; saF+=aF[i]; }
    int p=blind?0:primitive(nE,aE,nO,aO,m); int r=primitive(snF,saF,SN-snF,SA-saF,m);
    if(p) bits|=BIT(MX_P); if(r) bits|=BIT(MX_R); if(p&&r) bits|=BIT(MX_PR); }
  R->bits=bits; R->kF=kF; R->kG=kG; R->same=(kI==kF && kI==kG);
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
  tjob_t *J=(tjob_t*)arg; sampler_t Z; sampler_init(&Z,J->null,J->par,J->seed); table_t *T=J->T;
  for(uint64_t it=0; it<J->n; it++){ sampler_next(&Z); T->N++;
    for(int li=0; li<NLENS; li++){ fres_t R; facts(Z.a,li,&R); int cF=cb(R.kF), cG=cb(R.kG); T->trF[li][cF]++; T->trG[li][cG]++;
      u128 b=R.bits; while(b){ int f=ctz128(b); b&=b-1; int c=FCTX[f]==0?0:(FCTX[f]==1?cF:cG); T->hit[li][f][c]++; } } }
  return NULL;
}
static table_t *TAB; static u128 DISABLED=0; static long MINTR=0;
static void set_disabled(void){ const char *e=getenv("ROBOT_DISABLE"); if(!e) return; char buf[2048]; strncpy(buf,e,2047); buf[2047]=0;
  char *tok=strtok(buf,"|"); while(tok){ for(int f=0;f<NFACT;f++) if(strstr(FNAME[f],tok)) DISABLED|=BIT(f); tok=strtok(NULL,"|"); }
  fprintf(stderr,"devre dışı olgular:"); for(int f=0;f<NFACT;f++) if((DISABLED>>f)&1) fprintf(stderr," [%s]",FNAME[f]); fprintf(stderr,"\n"); }
static inline double pfact(int li, int f, int kF, int kG){
  int c; uint64_t tr;
  if(FCTX[f]==0){ c=0; tr=TAB->N; } else if(FCTX[f]==1){ c=cb(kF); tr=TAB->trF[li][c]; } else { c=cb(kG); tr=TAB->trG[li][c]; }
  if(FCTX[f]!=0 && (long)tr<MINTR){ uint64_t h=0; for(int k=0;k<NCTX;k++) h+=TAB->hit[li][f][k]; return ((double)h+0.5)/((double)TAB->N+1.0); }
  return ((double)TAB->hit[li][f][c]+0.5)/((double)tr+1.0);
}
typedef struct { double t1,t2,t3; } sc_t;
static int COHERENT = 0;
static sc_t score_lens(const int *a, int li, double *nodev_out){
  fres_t R; facts(a,li,&R); u128 b=R.bits&~DISABLED;
  double nodev[NNODE], nvF[NNODE], nvG[NNODE]; for(int i=0;i<NNODE;i++){ nodev[i]=0; nvF[i]=0; nvG[i]=0; }
  while(b){ int f=ctz128(b); b&=b-1; double p=pfact(li,f,R.kF,R.kG); double x=-log10(p); if(x<0)x=0; int nd=FNODE[f];
    if(x>nodev[nd]) nodev[nd]=x;
    /* aile sınıfı: 1 = yalnız F, 2 = yalnız G, 0 = ikisi de (kitap bütünü ve birleşim) */
    int cls=0;
    if(nd>=1&&nd<=5) cls=1; else if((nd>=6&&nd<=10)||nd==14) cls=2;
    else if(f==MD_F0||f==MD_FH||f==MQ_F0||f==MQ_FH||f==RP_F||f==RP_MODEN) cls=1;
    else if(f==MD_G0||f==MD_GH||f==MQ_G0||f==MQ_GH) cls=2;
    if(cls!=2 && x>nvF[nd]) nvF[nd]=x; if(cls!=1 && x>nvG[nd]) nvG[nd]=x; }
  if(R.same){ for(int i=6;i<=11;i++){ nodev[i]=0; nvF[i]=0; nvG[i]=0; } nodev[14]=0; nvF[14]=0; nvG[14]=0; }
  if(nodev_out) memcpy(nodev_out,nodev,sizeof nodev);
  sc_t S={0,0,0};
  int NV = COHERENT? 2: 1;
  for(int h=0; h<NV; h++){ const double *v = !COHERENT? nodev: (h? nvG: nvF); double s[NNODE], t1=0,t2=0,t3=0;
    for(int i=0;i<NNODE;i++){ s[i]=v[i]; t1+=v[i]; double pen=log10((double)NODE_NF[i]+1.0); if(v[i]>pen) t3+=v[i]-pen; }
    for(int i=1;i<NNODE;i++){ double y=s[i]; int j=i-1; while(j>=0&&s[j]<y){s[j+1]=s[j];j--;} s[j+1]=y; }
    for(int i=0;i<8;i++) t2+=s[i];
    if(t1>S.t1) S.t1=t1; if(t2>S.t2) S.t2=t2; if(t3>S.t3) S.t3=t3; }
  return S;
}
static int LI_Q;
static int LENS_OK[NLENS];
static void set_lens_mask(void){
  int mok[64]; for(int i=0;i<64;i++) mok[i]=1;
  const char *e=getenv("ROBOT_MSET"); if(e){ for(int i=0;i<64;i++) mok[i]=0; char buf[256]; strncpy(buf,e,255); buf[255]=0;
    for(char *t=strtok(buf,","); t; t=strtok(NULL,",")){ int m=atoi(t); if(m>=0&&m<64) mok[m]=1; } }
  int n=0; for(int li=0;li<NLENS;li++){ int m=MMIN+li%NMOD; LENS_OK[li]=mok[m]; n+=LENS_OK[li]; }
  fprintf(stderr,"izinli mercek sayısı: %d / %d\n", n, NLENS);
}
typedef struct { sc_t best, fix; int arg1,arg2,arg3; } bs_t;
static bs_t score_book(const int *a){ bs_t B; memset(&B,0,sizeof B);
  for(int li=0;li<NLENS;li++){ if(!LENS_OK[li] && li!=LI_Q) continue; sc_t S=score_lens(a,li,NULL); if(li==LI_Q) B.fix=S; if(!LENS_OK[li]) continue;
    if(S.t1>B.best.t1){B.best.t1=S.t1;B.arg1=li;} if(S.t2>B.best.t2){B.best.t2=S.t2;B.arg2=li;} if(S.t3>B.best.t3){B.best.t3=S.t3;B.arg3=li;} }
  return B; }
typedef struct { char null; double par; uint64_t seed, n; float *out; } sjob_t;
static void *score_worker(void *arg){ sjob_t *J=(sjob_t*)arg; sampler_t Z; sampler_init(&Z,J->null,J->par,J->seed);
  for(uint64_t it=0; it<J->n; it++){ sampler_next(&Z); bs_t B=score_book(Z.a); float *o=J->out+it*9;
    o[0]=B.best.t1; o[1]=B.best.t2; o[2]=B.best.t3; o[3]=B.fix.t1; o[4]=B.fix.t2; o[5]=B.fix.t3; o[6]=B.arg1; o[7]=B.arg2; o[8]=B.arg3; }
  return NULL; }
static void lens_name(int li, char *buf){ int f=li/NMOD, m=MMIN+li%NMOD; if(f<NLIN) sprintf(buf,"(%d,%d,%d)",FORMS[f][0],FORMS[f][1],m); else sprintf(buf,"(%s,%d)",NLNAME[f-NLIN],m); }
static void dump_lens(const int *a, int li){ fres_t R; facts(a,li,&R); char nb[32]; lens_name(li,nb); double nv[NNODE]; sc_t S=score_lens(a,li,nv);
  printf("  döküm %s (kF=%d kG=%d%s): T1=%.2f T2=%.2f T3=%.2f\n",nb,R.kF,R.kG,R.same?", F=G":"",S.t1,S.t2,S.t3);
  u128 b=R.bits&~DISABLED; while(b){ int f=ctz128(b); b&=b-1; double p=pfact(li,f,R.kF,R.kG); printf("     [%-17s] %-40s p=%.2e (−log10=%.2f)\n",NODENAME[FNODE[f]],FNAME[f],p,-log10(p)); }
  printf("     düğüm maksimumları:"); for(int i=0;i<NNODE;i++) if(nv[i]>0) printf(" %s=%.2f",NODENAME[i],nv[i]); printf("\n"); }
static void dump_book(const int *a, const char *title){ bs_t B=score_book(a); char nb[32];
  printf("== %s ==\n",title); lens_name(B.arg2,nb); printf("  en iyi mercek (T2): %s  T1=%.2f T2=%.2f T3=%.2f | sabit (1,1,19): T1=%.2f T2=%.2f T3=%.2f\n",nb,B.best.t1,B.best.t2,B.best.t3,B.fix.t1,B.fix.t2,B.fix.t3);
  dump_lens(a,LI_Q); if(B.arg2!=LI_Q) dump_lens(a,B.arg2); }

int main(int argc, char **argv){
  init_facts();
  for(int p=2;p<=7;p++){ if(p==4||p==6) continue; long v=(long)p*p*p; while(v<1000000){ PP[nPP++]=v; v*=p; } }
  for(int m=2;m<64;m++){ int pr=1; for(int d=2;d*d<=m;d++) if(m%d==0) pr=0; ISPRIME[m]=pr; if(pr){ long o=(long)m*m-1; NPF[m]=0; for(long q=2;q<=o;q++){ if(o%q==0){ int isp=1; for(long d=2;d*d<=q;d++) if(q%d==0){isp=0;break;} if(isp) PF[m][NPF[m]++]=(int)q; } } } }
  for(int i=0;i<28;i++) ISMD[MEDINAN[i]]=1; for(int i=0;i<29;i++) ISMQ[MUQ[i]]=1;
  memcpy(BASE,AYAT127,sizeof BASE);
  if(getenv("ROBOT_BASE")){ const char *s=getenv("ROBOT_BASE"); char *e; for(int i=0;i<114;i++){ BASE[i]=(int)strtol(s,&e,10); s=e; if(*s==',') s++; } }
  S_OBS=0; for(int i=0;i<114;i++) S_OBS+=(long)(i+1)*BASE[i];
  LI_Q=0*NMOD+(19-MMIN); set_disabled(); if(getenv("ROBOT_MINTR")) MINTR=atol(getenv("ROBOT_MINTR")); set_lens_mask(); if(getenv("ROBOT_COHERENT")) COHERENT=atoi(getenv("ROBOT_COHERENT")); fprintf(stderr,"tutarlı hikâye kuralı: %d\n", COHERENT);
  if(argc<2){ fprintf(stderr,"kullanım: table|score|facts|demofix ...\n"); return 1; }
  if(strcmp(argv[1],"table")==0){
    char null=argv[2][0]; double par=strlen(argv[2])>1?atof(argv[2]+1):0; uint64_t seed=strtoull(argv[3],NULL,10), N=strtoull(argv[4],NULL,10); int th=atoi(argv[5]);
    tjob_t *J=calloc(th,sizeof(tjob_t)); pthread_t *P=calloc(th,sizeof(pthread_t));
    for(int i=0;i<th;i++){ J[i].null=null; J[i].par=par; J[i].seed=seed*1000003ULL+i*7919ULL+3; J[i].n=N/th; J[i].T=calloc(1,sizeof(table_t)); pthread_create(&P[i],NULL,table_worker,&J[i]); }
    table_t *T=calloc(1,sizeof(table_t));
    for(int i=0;i<th;i++){ pthread_join(P[i],NULL); table_t *X=J[i].T; T->N+=X->N;
      for(int l=0;l<NLENS;l++){ for(int c=0;c<NCTX;c++){ T->trF[l][c]+=X->trF[l][c]; T->trG[l][c]+=X->trG[l][c]; } for(int f=0;f<NF;f++) for(int c=0;c<NCTX;c++) T->hit[l][f][c]+=X->hit[l][f][c]; } free(X); }
    FILE *fp=fopen(argv[6],"wb"); fwrite(T,sizeof(table_t),1,fp); fclose(fp);
    printf("tablo yazıldı: N=%llu, olgu=%d, düğüm=%d, mercek=%d\n",(unsigned long long)T->N,NFACT,NNODE,NLENS); return 0; }
  TAB=calloc(1,sizeof(table_t));
  if(strcmp(argv[1],"facts")==0){ FILE *fp=fopen(argv[2],"rb"); fread(TAB,sizeof(table_t),1,fp); fclose(fp);
    if(argc>3){ int b[114]; char *s=argv[3]; for(int i=0;i<114;i++){ b[i]=(int)strtol(s,&s,10); if(*s==',') s++; } dump_book(b,argc>4?argv[4]:"harici"); }
    else dump_book(BASE, getenv("ROBOT_BASE")?"TEMEL KİTAP (ROBOT_BASE)":"KUR'AN (kodeks 127)");
    return 0; }
  if(strcmp(argv[1],"demofix")==0){
    char null=argv[2][0]; double par=strlen(argv[2])>1?atof(argv[2]+1):0; uint64_t seed=strtoull(argv[3],NULL,10), N=strtoull(argv[4],NULL,10);
    FILE *fp=fopen(argv[5],"rb"); fread(TAB,sizeof(table_t),1,fp); fclose(fp); int K=atoi(argv[6]);
    sc_t Qs=score_lens(BASE,LI_Q,NULL); printf("temel kitap sabit-mercek T1=%.2f T2=%.2f T3=%.2f\n",Qs.t1,Qs.t2,Qs.t3);
    sampler_t Z; sampler_init(&Z,null,par,seed); static int best[16][114]; double bv[16]; uint64_t bi[16]; for(int i=0;i<K;i++) bv[i]=-1; uint64_t nge=0;
    for(uint64_t it=0; it<N; it++){ sampler_next(&Z); sc_t S=score_lens(Z.a,LI_Q,NULL); if(S.t2>=Qs.t2-1e-9) nge++;
      if(S.t2>bv[K-1]){ int j=K-1; while(j>0 && bv[j-1]<S.t2){ bv[j]=bv[j-1]; bi[j]=bi[j-1]; memcpy(best[j],best[j-1],sizeof best[0]); j--; } bv[j]=S.t2; bi[j]=it; memcpy(best[j],Z.a,sizeof best[0]); } }
    printf("sabit mercekte T2 ≥ temel kitap olan sıfır kitabı: %llu / %llu\n",(unsigned long long)nge,(unsigned long long)N);
    for(int i=0;i<K;i++){ printf("--- SIFIR KİTABI #%llu (tohum %llu, %s)\n",(unsigned long long)bi[i],(unsigned long long)seed,argv[2]); dump_lens(best[i],LI_Q);
      printf("  diziliş: "); for(int k=0;k<114;k++) printf("%d%s",best[i][k],k<113?",":"\n"); }
    return 0; }
  if(strcmp(argv[1],"score")==0){
    char null=argv[2][0]; double par=strlen(argv[2])>1?atof(argv[2]+1):0; uint64_t seed=strtoull(argv[3],NULL,10), N=strtoull(argv[4],NULL,10); int th=atoi(argv[5]);
    FILE *fp=fopen(argv[6],"rb"); fread(TAB,sizeof(table_t),1,fp); fclose(fp);
    bs_t Q=score_book(BASE); char fn[512]; snprintf(fn,512,"%s_kuran.txt",argv[7]); FILE *fq=fopen(fn,"w");
    fprintf(fq,"%.4f %.4f %.4f %.4f %.4f %.4f %d %d %d\n",Q.best.t1,Q.best.t2,Q.best.t3,Q.fix.t1,Q.fix.t2,Q.fix.t3,Q.arg1,Q.arg2,Q.arg3); fclose(fq);
    uint64_t per=N/th; float *out=malloc(sizeof(float)*9*per*th);
    sjob_t *J=calloc(th,sizeof(sjob_t)); pthread_t *P=calloc(th,sizeof(pthread_t));
    for(int i=0;i<th;i++){ J[i].null=null; J[i].par=par; J[i].seed=seed*1000003ULL+i*7919ULL+5; J[i].n=per; J[i].out=out+i*per*9; pthread_create(&P[i],NULL,score_worker,&J[i]); }
    for(int i=0;i<th;i++) pthread_join(P[i],NULL);
    snprintf(fn,512,"%s_sifir.f32",argv[7]); FILE *fo=fopen(fn,"wb"); fwrite(out,sizeof(float),9*per*th,fo); fclose(fo);
    printf("puanlandı: %llu kitap. temel kitap: en-iyi T1=%.2f T2=%.2f T3=%.2f | sabit T1=%.2f T2=%.2f T3=%.2f\n",(unsigned long long)(per*th),Q.best.t1,Q.best.t2,Q.best.t3,Q.fix.t1,Q.fix.t2,Q.fix.t3);
    return 0; }
  return 1;
}
