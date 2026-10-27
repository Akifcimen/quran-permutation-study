// butunluk.h — bütünlük dili L'nin hızlı C uygulaması ve sıfır örnekleyicileri.
// Tanımların başvurusu src/dil.py'dir; bu dosya onunla bit-bit sınanır (kontrol/c_py_esit.*).
// Örnekleyiciler ~/Desktop/hrf2/denetim10_opus55_kosullu/src/{ortak.h,hizli.h}'den kopyalanmıştır (aynı xoshiro256++ ve
// tohumlama; robotun U/M20 sıfırları; Mallows Lehmer + alias; J10 titreşimi).
#ifndef BUTUNLUK_H
#define BUTUNLUK_H
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
static int BASE[114];
static int KODEKS = 129;
static long SA_TOT, S_OBS;
#define SN_TOT 6555L

// ---------------- RNG (denetim10 ortak.h ile birebir) ----------------
typedef struct { uint64_t s[4]; } rng_t;
static inline uint64_t rotl(const uint64_t x, int k){ return (x << k) | (x >> (64 - k)); }
static inline uint64_t xnext(rng_t *r){ uint64_t *s=r->s; const uint64_t res=rotl(s[0]+s[3],23)+s[0]; const uint64_t t=s[1]<<17;
  s[2]^=s[0]; s[3]^=s[1]; s[1]^=s[2]; s[0]^=s[3]; s[2]^=t; s[3]=rotl(s[3],45); return res; }
static void rseed(rng_t *r, uint64_t seed){ uint64_t z=seed; for(int i=0;i<4;i++){ z+=0x9E3779B97F4A7C15ULL; uint64_t x=z;
  x=(x^(x>>30))*0xBF58476D1CE4E5B9ULL; x=(x^(x>>27))*0x94D049BB133111EBULL; r->s[i]=x^(x>>31);} }
static inline uint32_t bounded(rng_t *r, uint32_t n){ return (uint32_t)(((unsigned __int128)xnext(r)*n)>>64); }
static inline double unif(rng_t *r){ return (xnext(r)>>11)*(1.0/9007199254740992.0); }

static void kodeks_sec(int k){
  KODEKS=k; memcpy(BASE,AYAT129,sizeof BASE); if(k==127) BASE[8]=127; else if(k!=129){ fprintf(stderr,"kodeks 127|129\n"); exit(1); }
  SA_TOT=0; S_OBS=0; for(int i=0;i<114;i++){ SA_TOT+=BASE[i]; S_OBS+=(long)(i+1)*BASE[i]; }
}

// ---------------- mercekler ----------------
#define NFORM 14
#define NPR 29
#define NLENS (NFORM*NPR)
static const int AL[NFORM]={1,1,1,1,1,1,2,2,2,2,3,3,3,3};
static const int BE[NFORM]={1,-1,2,-2,3,-3,1,-1,3,-3,1,-1,2,-2};
static const int PR[NPR]={3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113};
#define TMAX 1200
static uint32_t DIVM[TMAX+1];          // DIVM[|t|] = t'yi bölen asalların (indeks) maskesi; t = 0 → hepsi
static uint32_t INV32[29];             // p'nin 2^32 modülüne göre tersi: tam bölünen t için t/p = t·INV32 (mod 2^32)

// yüklem bitleri (src/dil.py PRED sırası)
enum { P_B1, P_B2, P_F1, P_F2, P_R0c, P_R0e, P_R1c, P_R1e, P_K1, P_K2, P_Qc, P_QS, P_Qh, P_QkF,
       P_W1, P_QU, P_QkH, P_Qa, P_Qn, P_QH, NPRED };
static const char *PNAME[NPRED]={"B1","B2","F1","F2","R0c","R0e","R1c","R1e","K1","K2","Qc","QS","Qh","QkF",
  "W1","QU","QkH","Qa","Qn","QH"};
#define BITF_MASK  ((1u<<P_F1)|(1u<<P_F2)|(1u<<P_R0c)|(1u<<P_R0e)|(1u<<P_R1c)|(1u<<P_R1e)|(1u<<P_K1)|(1u<<P_K2)|(1u<<P_Qc)|(1u<<P_QS)|(1u<<P_Qh))
#define BITW_MASK  ((1u<<P_W1)|(1u<<P_QU)|(1u<<P_QkH)|(1u<<P_Qa)|(1u<<P_Qn)|(1u<<P_QH))
#define BITB_MASK  ((1u<<P_B1)|(1u<<P_B2))
#define GATED_MASK (BITF_MASK|(1u<<P_QkF))
static uint32_t EXCL[NLENS];          // sabit (dışlanan) yüklemler
static inline uint32_t fmask11(uint32_t m){ return (m>>P_F1)&0x7FFu; }      // F1..Qh → 11 bit
static inline uint32_t wmask6(uint32_t m){ return (m>>P_W1)&0x3Fu; }        // W1..QH → 6 bit
static inline uint32_t bmask2(uint32_t m){ return m&3u; }

// ---------------- kanıtlı gerektirmeler (ONKAYIT §2.7; src/dil.py KURALLAR ile aynı) ----------------
// Birincil bölme (D1 = t2). Kural kümesi merceğe bağlıdır: değişke v = (p == 3 ? 2 : 0) + (β > 0 ? 1 : 0).
//  Genel: Qc→QS, Qh→QS, F2∧QS→Qh, QkF∧QkH∧QU→Qn, QkF∧QkH∧QU→Qa, K1∧Qc→F2, K1∧Qc→R1e
//  p ≥ 5: K1∧Qh→F2          p = 3: R1c∧K1∧Qh→F2, R1e∧K1∧Qh→F2
//  β > 0: Qc∧Qh→F2          p = 3 ∧ β > 0: Qc'nin sonlu uzayının kesin kuralları (kontrol/p3_qc_kesin.py)
#define B_(x) (1u<<(x))
typedef struct { uint32_t onc, son; } kural_t;
static kural_t KUR[4][40]; static int NKUR[4];
static void kural_ekle(int v, uint32_t onc, uint32_t son){ KUR[v][NKUR[v]].onc=onc; KUR[v][NKUR[v]].son=son; NKUR[v]++; }
static void kurallari_kur(void){
  for(int v=0;v<4;v++){ NKUR[v]=0; int p3=(v>=2), poz=(v&1);
    kural_ekle(v,B_(P_Qc),B_(P_QS)); kural_ekle(v,B_(P_Qh),B_(P_QS)); kural_ekle(v,B_(P_F2)|B_(P_QS),B_(P_Qh));
    kural_ekle(v,B_(P_QkF)|B_(P_QkH)|B_(P_QU),B_(P_Qn)|B_(P_Qa));
    kural_ekle(v,B_(P_K1)|B_(P_Qc),B_(P_F2)|B_(P_R1e));
    if(!p3) kural_ekle(v,B_(P_K1)|B_(P_Qh),B_(P_F2));
    else { kural_ekle(v,B_(P_R1c)|B_(P_K1)|B_(P_Qh),B_(P_F2)); kural_ekle(v,B_(P_R1e)|B_(P_K1)|B_(P_Qh),B_(P_F2)); }
    if(poz) kural_ekle(v,B_(P_Qc)|B_(P_Qh),B_(P_F2));
    if(p3 && poz){ uint32_t Qc=B_(P_Qc);
      kural_ekle(v,Qc|B_(P_F1)|B_(P_F2),B_(P_R0c)|B_(P_R1c)); kural_ekle(v,Qc|B_(P_F1)|B_(P_R0e),B_(P_R0c));
      kural_ekle(v,Qc|B_(P_F1)|B_(P_R1e),B_(P_R1c)); kural_ekle(v,Qc|B_(P_F1)|B_(P_K1),B_(P_R0c)|B_(P_R1c)|B_(P_K2));
      kural_ekle(v,Qc|B_(P_F1)|B_(P_Qh),B_(P_R0c)|B_(P_R1c)); kural_ekle(v,Qc|B_(P_F2)|B_(P_R1e),B_(P_K1));
      kural_ekle(v,Qc|B_(P_R1e)|B_(P_Qh),B_(P_K1)); } } }
static inline uint32_t kapanis_v(uint32_t s, int v){ int ch=1; while(ch){ ch=0; for(int i=0;i<NKUR[v];i++){ const kural_t *k=&KUR[v][i];
      if((s&k->onc)==k->onc && (s&k->son)!=k->son){ s|=k->son; ch=1; } } } return s; }
// kredi(S) = S'yi kapanışla üreten en küçük alt kümenin boyu. Kural bitleri 2..18 (F1..Qn) içindedir.
static uint8_t RED17[4][1u<<17];
static int kredi_kesin(uint32_t S, int v){ // S'nin türetilebilir öğeleri D; D'nin alt kümelerinden en çok çıkarılabilen
  uint32_t D=0; for(int j=0;j<NPRED;j++) if(S&B_(j)){ if(kapanis_v(S&~B_(j),v)&B_(j)) D|=B_(j); }
  int best=0; uint32_t sub=D; for(;;){ int c=__builtin_popcount(sub); if(c>best && (kapanis_v(S&~sub,v)&S)==S) best=c; if(sub==0) break; sub=(sub-1)&D; }
  return __builtin_popcount(S)-best; }
static void kredi_tablolari(void){ kurallari_kur();
  for(int v=0;v<4;v++) for(uint32_t x=0;x<(1u<<17);x++){ uint32_t S=x<<2; RED17[v][x]=(uint8_t)(__builtin_popcount(S)-kredi_kesin(S,v)); } }
static int MVAR[NLENS];
static inline int kredi(uint32_t m, int L){ return __builtin_popcount(m)-RED17[MVAR[L]][(m>>2)&0x1FFFFu]; }

static void mercek_hazirla(void){
  for(int x=0;x<=TMAX;x++){ uint32_t m=0; for(int i=0;i<NPR;i++) if(x%PR[i]==0) m|=1u<<i; DIVM[x]=m; }
  for(int i=0;i<NPR;i++){ uint32_t p=(uint32_t)PR[i], y=1; for(int k=0;k<5;k++) y*=2u-p*y; INV32[i]=y;
    for(int t=-TMAX;t<=TMAX;t++) if(t%PR[i]==0 && (int32_t)((uint32_t)t*y)!=t/PR[i]){ fprintf(stderr,"ters hatası\n"); exit(3); } }
  for(int f=0;f<NFORM;f++) for(int i=0;i<NPR;i++){ int p=PR[i]; uint32_t x=0;
    if(AL[f]%2==0 || BE[f]%2==0) x|=1u<<P_B1;          // birincil bölme t2: sınıf boyu sabit
    if(abs(BE[f])%p==0) x|=(1u<<P_QU)|(1u<<P_QkF)|(1u<<P_QkH)|(1u<<P_Qn)|(1u<<P_W1);
    if(AL[f]%p==0)      x|=(1u<<P_QU)|(1u<<P_QkF)|(1u<<P_QkH)|(1u<<P_Qa)|(1u<<P_W1);
    EXCL[f*NPR+i]=x; }
  for(int f=0;f<NFORM;f++) for(int i=0;i<NPR;i++) MVAR[f*NPR+i]=(PR[i]==3?2:0)+(BE[f]>0?1:0);
  kredi_tablolari();
}

// ---------------- bir kitabın 406 mercekteki profili ----------------
typedef struct { uint32_t m[NLENS]; uint8_t kF[NLENS], kH[NLENS], mc[NLENS]; } profil_t;  // mc = en küçük hücre sayısı
typedef struct { int k, c[4]; long s[4]; int qmin,qmax; uint64_t b0,b1; long sa, sn; int qv[114]; } fam_t;
typedef struct { int k; long sq, sa, sn; uint64_t b0,b1; } hfam_t;

static inline int kosu4(long a, long b, long c, long d){ long v[4]={a,b,c,d};
  for(int i=1;i<4;i++){ long x=v[i]; int j=i-1; while(j>=0&&v[j]>x){ v[j+1]=v[j]; j--; } v[j+1]=x; }
  int nd=1; for(int i=1;i<4;i++) if(v[i]!=v[i-1]) nd++; return nd>=2 && (v[3]-v[0]+1)==nd; }
static inline int kosu_q(const fam_t *F){ // farklı q'lar kesintisiz, ≥2 farklı
  if(F->k<2) return 0; long span=(long)F->qmax-F->qmin+1; if(span<2 || span>F->k) return 0;
  uint64_t w[2]={0,0}; for(int i=0;i<F->k;i++){ int d=F->qv[i]-F->qmin; w[d>>6]|=1ULL<<(d&63); }
  return (__builtin_popcountll(w[0])+__builtin_popcountll(w[1]))==span; }

static void degerlendir(const int *a, profil_t *P){
  static __thread fam_t FF[NPR]; static __thread hfam_t HH[NPR];
  for(int f=0;f<NFORM;f++){
    const int al=AL[f], be=BE[f], abe=abs(be);
    for(int i=0;i<NPR;i++){ fam_t *F=&FF[i]; F->k=0; F->c[0]=F->c[1]=F->c[2]=F->c[3]=0; F->s[0]=F->s[1]=F->s[2]=F->s[3]=0;
      F->qmin=1<<30; F->qmax=-(1<<30); F->b0=F->b1=0; F->sa=F->sn=0; HH[i].k=0; HH[i].sq=HH[i].sa=HH[i].sn=0; HH[i].b0=HH[i].b1=0; }
    int e0=0; long sn0=0, sa0=0, st0=0, st=0;
    for(int n=1;n<=114;n++){ const int x=a[n-1]; const int t=al*n+be*x, u=al*n-be*x;
      const int d1=t&1, d2=n&1; st+=t; if(!d1){ e0++; sn0+=n; sa0+=x; st0+=t; }
      uint32_t mt=DIVM[t<0?-t:t]; const int ci=d1*2+d2;
      while(mt){ int i=__builtin_ctz(mt); mt&=mt-1; fam_t *F=&FF[i]; const int q=(int32_t)((uint32_t)t*INV32[i]);
        F->c[ci]++; F->s[ci]+=q; F->qv[F->k++]=q; if(q<F->qmin) F->qmin=q; if(q>F->qmax) F->qmax=q; F->sa+=x; F->sn+=n;
        if(n<=64) F->b0|=1ULL<<(n-1); else F->b1|=1ULL<<(n-65); }
      uint32_t mu=DIVM[u<0?-u:u];
      while(mu){ int i=__builtin_ctz(mu); mu&=mu-1; hfam_t *H=&HH[i]; H->k++; H->sq+=(int32_t)((uint32_t)u*INV32[i]); H->sa+=x; H->sn+=n;
        if(n<=64) H->b0|=1ULL<<(n-1); else H->b1|=1ULL<<(n-65); } }
    // kitap düzeyi (biçim düzeyi; p'den bağımsız)
    uint32_t mb=0; const int e1=114-e0; const long sn1=SN_TOT-sn0, sa1=SA_TOT-sa0, st1=st-st0;
    if(e0==e1) mb|=1u<<P_B1;
    if(e0>0 && e1>0 && ((long)al*sn0==(long)abe*sa1 || (long)al*sn1==(long)abe*sa0 || st0==st1)) mb|=1u<<P_B2;
    for(int i=0;i<NPR;i++){ const int p=PR[i]; const int L=f*NPR+i; fam_t *F=&FF[i]; hfam_t *H=&HH[i]; uint32_t m=mb;
      const int c0=F->c[0],c1=F->c[1],c2=F->c[2],c3=F->c[3]; const long s0=F->s[0],s1=F->s[1],s2=F->s[2],s3=F->s[3];
      const int h0=c0+c1, h1=c2+c3; const long q0=s0+s1, q1=s2+s3, sq=q0+q1; const int dolu=(c0&&c1&&c2&&c3);
      if(h0==h1 && h0>=1) m|=1u<<P_F1;
      if(h0>=1 && h1>=1 && q0==q1) m|=1u<<P_F2;
      if(c0==c1 && c0>=1) m|=1u<<P_R0c;
      if(c0>=1 && c1>=1 && s0==s1) m|=1u<<P_R0e;
      if(c2==c3 && c2>=1) m|=1u<<P_R1c;
      if(c2>=1 && c3>=1 && s2==s3) m|=1u<<P_R1e;
      if(dolu && kosu4(s0,s1,s2,s3)) m|=1u<<P_K1;
      if(kosu_q(F)) m|=1u<<P_K2;
      if(dolu && sq==4L*p) m|=1u<<P_Qc;
      if(F->k>=1 && sq%p==0) m|=1u<<P_QS;
      if(h0>=1 && h1>=1 && q0%p==0 && q1%p==0) m|=1u<<P_Qh;
      if(F->k==p) m|=1u<<P_QkF;
      const uint64_t i0=F->b0&H->b0, i1=F->b1&H->b1; const int ni=__builtin_popcountll(i0)+__builtin_popcountll(i1);
      const int kU=F->k+H->k-ni;
      long saU=F->sa+H->sa, snU=F->sn+H->sn;
      if(ni){ uint64_t w=i0; while(w){ int b=__builtin_ctzll(w); w&=w-1; saU-=a[b]; snU-=b+1; }
              w=i1; while(w){ int b=__builtin_ctzll(w); w&=w-1; saU-=a[b+64]; snU-=b+65; } }
      if(F->k>=1 && H->k>=1 && ni==0) m|=1u<<P_W1;
      if(kU==p) m|=1u<<P_QU;
      if(H->k==p) m|=1u<<P_QkH;
      if(kU>=1 && saU%p==0) m|=1u<<P_Qa;
      if(kU>=1 && snU%p==0) m|=1u<<P_Qn;
      if(H->k>=1 && H->sq%p==0) m|=1u<<P_QH;
      m&=~EXCL[L];
      P->m[L]=m; P->kF[L]=(uint8_t)F->k; P->kH[L]=(uint8_t)H->k;
      int mc=c0; if(c1<mc) mc=c1; if(c2<mc) mc=c2; if(c3<mc) mc=c3; P->mc[L]=(uint8_t)mc; }
  }
}

// kapılar: 0 = G (kF ≥ 2E ∧ hücre ≥ 2), 1 = GE (kF ≥ E ∧ hücre ≥ 2), 2 = Gc (hücre ≥ 2), 3 = G0 (yok)
static inline int kapi_tut(const profil_t *P, int L, int tur){ const int p=PR[L%NPR]; const int k=P->kF[L]; const int mc=P->mc[L];
  switch(tur){ case 0: return k*p>=228 && mc>=2; case 1: return k*p>=114 && mc>=2; case 2: return mc>=2; default: return 1; } }
static inline int sayim_mercek(const profil_t *P, int L, int tur){ uint32_t m=P->m[L]; if(!kapi_tut(P,L,tur)) m&=~GATED_MASK; return kredi(m,L); }
static inline int sayim_puani(const profil_t *P, int tur, int *enL){ int best=-1, bl=-1;
  for(int L=0;L<NLENS;L++){ int c=sayim_mercek(P,L,tur); if(c>best){ best=c; bl=L; } } if(enL) *enL=bl; return best; }

// ---------------- sıfır örnekleyicileri (denetim10'dan) ----------------
// U: tekdüze (Fisher–Yates, robotun 'U' akışı). M: robotun M20 zinciri (64 zincir, tam azalandan 30000 adım yakma,
// emisyon başına 20 öneri, S = Σ n·a ≤ S_obs). L: Mallows (q, E[I] = I_gözlenen; Lehmer + alias). J: J10 titreşimi (σ = 10).
#define NCH 64
typedef struct { char null; rng_t r; int a[114];
  int ch[NCH][114]; long chS[NCH]; int cur;                         // M20
  double q; int ref[114]; uint32_t thr[115][114]; uint8_t ali[115][114];   // Mallows
  double sig; } orn_t;
static int REF_DESC[114];
static void ref_hazirla(void){ memcpy(REF_DESC,BASE,sizeof REF_DESC); for(int i=1;i<114;i++){ int x=REF_DESC[i]; int j=i-1; while(j>=0&&REF_DESC[j]<x){REF_DESC[j+1]=REF_DESC[j];j--;} REF_DESC[j+1]=x; } }
static long ters_kesin(const int *a){ long I=0; for(int i=0;i<114;i++) for(int j=i+1;j<114;j++) if(a[i]<a[j]) I++; return I; }
static double kg_ort(double q, int R){ double s=0,w=0,p=1; for(int j=0;j<R;j++){ s+=j*p; w+=p; p*=q; } return s/w; }
static double mallows_beklenen(double q){ double E=0; for(int R=1;R<=114;R++) E+=kg_ort(q,R);
  int v[114]; memcpy(v,REF_DESC,sizeof v); int i=0; while(i<114){ int j=i; while(j<114&&v[j]==v[i]) j++; int mg=j-i; for(int R=1;R<=mg;R++) E-=kg_ort(q,R); i=j; }
  return E; }
static double mallows_q_ayarla(long Iobs){ double lo=0.5, hi=0.999999; for(int it=0;it<200;it++){ double mid=0.5*(lo+hi); if(mallows_beklenen(mid)<Iobs) lo=mid; else hi=mid; } return 0.5*(lo+hi); }
static void orn_init(orn_t *Z, char null, uint64_t seed){
  memset(Z,0,sizeof *Z); Z->null=null; rseed(&Z->r,seed); memcpy(Z->a,BASE,sizeof Z->a);
  if(null=='M'){ int srt[114]; memcpy(srt,BASE,sizeof srt); for(int i=1;i<114;i++){ int x=srt[i]; int j=i-1; while(j>=0&&srt[j]<x){srt[j+1]=srt[j];j--;} srt[j+1]=x; }
    for(int c=0;c<NCH;c++){ int *a=Z->ch[c]; memcpy(a,srt,sizeof srt); long S=0; for(int i=0;i<114;i++) S+=(long)(i+1)*a[i];
      for(int b=0;b<30000;b++){ int i=(int)bounded(&Z->r,114), j=(int)bounded(&Z->r,114); long dS=(long)((i+1)-(j+1))*(a[j]-a[i]); if(S+dS<=S_OBS){ int t=a[i]; a[i]=a[j]; a[j]=t; S+=dS; } }
      Z->chS[c]=S; } }
  if(null=='L'){ ref_hazirla(); Z->q=mallows_q_ayarla(ters_kesin(BASE)); memcpy(Z->ref,REF_DESC,sizeof Z->ref);
    for(int R=1;R<=114;R++){ double w[114], s=0, p=1; for(int j=0;j<R;j++){ w[j]=p; s+=p; p*=Z->q; }
      double sc[114]; int small[114], large[114], ns=0, nl=0; double P[114]; int A[114];
      for(int j=0;j<R;j++){ sc[j]=w[j]/s*R; if(sc[j]<1.0) small[ns++]=j; else large[nl++]=j; }
      while(ns>0 && nl>0){ int l=small[--ns], g=large[--nl]; P[l]=sc[l]; A[l]=g; sc[g]=(sc[g]+sc[l])-1.0; if(sc[g]<1.0) small[ns++]=g; else large[nl++]=g; }
      while(nl>0){ int g=large[--nl]; P[g]=1.0; A[g]=g; } while(ns>0){ int l=small[--ns]; P[l]=1.0; A[l]=l; }
      for(int j=0;j<R;j++){ double t=P[j]*4294967296.0; Z->thr[R][j]=(t>=4294967295.0)?0xFFFFFFFFu:(uint32_t)t; Z->ali[R][j]=(uint8_t)A[j]; } } }
  if(null=='J') Z->sig=10.0;
}
#define JZ_KMIN (-120)
#define JZ_KN 360
static inline void jz_sirala(const double *key, int *idx){
  int head[JZ_KN], nxt[114]; for(int b=0;b<JZ_KN;b++) head[b]=-1;
  for(int i=113;i>=0;i--){ int b=(int)floor(key[i])-JZ_KMIN; if(b<0) b=0; if(b>=JZ_KN) b=JZ_KN-1; nxt[i]=head[b]; head[b]=i; }
  int m=0; for(int b=0;b<JZ_KN;b++){ int st=m; for(int e=head[b]; e>=0; e=nxt[e]){ int j=m-1; double ke=key[e]; while(j>=st && key[idx[j]]>ke){ idx[j+1]=idx[j]; j--; } idx[j+1]=e; m++; } } }
// sıradaki kitabı Z->a'ya yaz ('K' = yalnız Kur'an'ın kendisi; kod sınaması için)
static inline void orn_sonraki(orn_t *Z){
  int *a=Z->a;
  if(Z->null=='K'){ memcpy(a,BASE,sizeof(int)*114); }
  else if(Z->null=='U'){ rng_t *r=&Z->r; for(int i=113;i>0;i--){ int j=(int)bounded(r,(uint32_t)(i+1)); int t=a[i]; a[i]=a[j]; a[j]=t; } }
  else if(Z->null=='M'){ int c=Z->cur; Z->cur=(Z->cur+1)%NCH; int *b=Z->ch[c]; long S=Z->chS[c]; rng_t r=Z->r;
    for(int st=0;st<20;st++){ int i=(int)bounded(&r,114), j=(int)bounded(&r,114); long dS=(long)((i+1)-(j+1))*(b[j]-b[i]); if(S+dS<=S_OBS){ int t=b[i]; b[i]=b[j]; b[j]=t; S+=dS; } }
    Z->r=r; Z->chS[c]=S; memcpy(a,b,114*sizeof(int)); }
  else if(Z->null=='L'){ int rem[114+48]; for(int i=0;i<114;i++) rem[i]=Z->ref[113-i]; for(int i=114;i<114+48;i++) rem[i]=0; rng_t r=Z->r;
    for(int p=0;p<114;p++){ int R=114-p; uint64_t x=xnext(&r); int j=(int)(((x>>32)*(uint64_t)R)>>32); uint32_t u=(uint32_t)x;
      uint32_t th=Z->thr[R][j]; int V=(u<th||th==0xFFFFFFFFu)?j:Z->ali[R][j]; int ix=R-1-V; a[p]=rem[ix];
      __builtin_memmove(rem+ix, rem+ix+1, 32*sizeof(int)); for(int k=ix+32;k<R-1;k++) rem[k]=rem[k+1]; }
    Z->r=r; }
  else if(Z->null=='J'){ double key[114]; int idx[114]; rng_t r=Z->r;
    for(int i=0;i<114;i+=2){ double u1=(xnext(&r)>>11)*(1.0/9007199254740992.0), u2=(xnext(&r)>>11)*(1.0/9007199254740992.0); if(u1<1e-300) u1=1e-300;
      double rad=sqrt(-2.0*log(u1)), th=6.283185307179586*u2; key[i]=(i+1)+Z->sig*rad*cos(th); key[i+1]=(i+2)+Z->sig*rad*sin(th); idx[i]=i; idx[i+1]=i+1; }
    jz_sirala(key,idx); for(int k=0;k<114;k++) a[k]=BASE[idx[k]]; Z->r=r; }
}

static void butunluk_init(int kodeks){ kodeks_sec(kodeks); mercek_hazirla(); }
#endif
