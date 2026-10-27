// menu.h — bölme menüsü (ONKAYIT §8.4): bölmeler t2, n2, a2, h, t4; sıralı çiftler (D1, D2), D1 ≠ D2 (20 çift).
// Yalnız SAYIM puanı (kapı G; kapı hücreleri seçilen çiftin hücreleri). Kural kümesi çiftin parite yapısına göre:
//  'R' satır paritesi (D1 ≡ t-paritesi): birincil kurallar (butunluk.h, 4 değişke)
//  'C' sütun paritesi (D2 ≡ t-paritesi) ya da 'D' köşegen ({D1,D2} = {n2,a2}, α ve β tek): P1–P4; p ≥ 5: K1∧Qh→F2
//  'X' paritesiz: P1–P4; p ≥ 5: K1∧Qh→F2; R0e∧K1∧Qc→F2, R1e∧K1∧Qc→F2; β > 0: R0e∧Qc∧Qh→F2, R1e∧Qc∧Qh→F2; p = 3: K1∧Qc∧Qh→F2
#ifndef MENU_H
#define MENU_H
#include "butunluk.h"
enum { D_t2, D_n2, D_a2, D_h, D_t4, ND };
static const char *DAD[ND]={"t2","n2","a2","h","t4"};
static int CIFT[20][2]; static int NCIFT=0;
// menü kural tabloları: 'R' → RED17[v] (butunluk.h); 'C'/'D' → 2 değişke (p ≥ 5, p = 3); 'X' → 4 değişke
static kural_t MKUR[6][16]; static int NMKUR[6]; static uint8_t MRED[6][1u<<17];
static void mkural_ekle(int v, uint32_t onc, uint32_t son){ MKUR[v][NMKUR[v]].onc=onc; MKUR[v][NMKUR[v]].son=son; NMKUR[v]++; }
static inline uint32_t mkapanis(uint32_t s, int v){ int ch=1; while(ch){ ch=0; for(int i=0;i<NMKUR[v];i++){ const kural_t *k=&MKUR[v][i];
      if((s&k->onc)==k->onc && (s&k->son)!=k->son){ s|=k->son; ch=1; } } } return s; }
static int mkredi_kesin(uint32_t S, int v){ uint32_t D=0; for(int j=0;j<NPRED;j++) if(S&B_(j)){ if(mkapanis(S&~B_(j),v)&B_(j)) D|=B_(j); }
  int best=0; uint32_t sub=D; for(;;){ int c=__builtin_popcount(sub); if(c>best && (mkapanis(S&~sub,v)&S)==S) best=c; if(sub==0) break; sub=(sub-1)&D; }
  return __builtin_popcount(S)-best; }
static void menu_hazirla(void){
  NCIFT=0; for(int a=0;a<ND;a++) for(int b=0;b<ND;b++) if(a!=b){ CIFT[NCIFT][0]=a; CIFT[NCIFT][1]=b; NCIFT++; }
  // v: 0 = C/D p≥5, 1 = C/D p=3, 2 = X p≥5 β>0, 3 = X p≥5 β<0, 4 = X p=3 β>0, 5 = X p=3 β<0
  for(int v=0;v<6;v++){ NMKUR[v]=0; int p3=(v==1||v==4||v==5), poz=(v==2||v==4), X=(v>=2);
    mkural_ekle(v,B_(P_Qc),B_(P_QS)); mkural_ekle(v,B_(P_Qh),B_(P_QS)); mkural_ekle(v,B_(P_F2)|B_(P_QS),B_(P_Qh));
    mkural_ekle(v,B_(P_QkF)|B_(P_QkH)|B_(P_QU),B_(P_Qn)|B_(P_Qa));
    if(!p3) mkural_ekle(v,B_(P_K1)|B_(P_Qh),B_(P_F2));
    if(X){ mkural_ekle(v,B_(P_R0e)|B_(P_K1)|B_(P_Qc),B_(P_F2)); mkural_ekle(v,B_(P_R1e)|B_(P_K1)|B_(P_Qc),B_(P_F2));
      if(poz){ mkural_ekle(v,B_(P_R0e)|B_(P_Qc)|B_(P_Qh),B_(P_F2)); mkural_ekle(v,B_(P_R1e)|B_(P_Qc)|B_(P_Qh),B_(P_F2)); }
      if(p3) mkural_ekle(v,B_(P_K1)|B_(P_Qc)|B_(P_Qh),B_(P_F2)); }
    for(uint32_t x=0;x<(1u<<17);x++){ uint32_t S=x<<2; MRED[v][x]=(uint8_t)(__builtin_popcount(S)-mkredi_kesin(S,v)); } } }
// çiftin parite yapısı (biçime bağlı): 'R','C','D','X'
static inline int t_esit(int d, int f){ // bölme d, bu biçimde t-paritesiyle aynı işlev mi?
  if(d==D_t2) return 1; if(d==D_n2 && BE[f]%2==0) return 1; if(d==D_a2 && AL[f]%2==0) return 1; return 0; }
static inline char yapi(int d1, int d2, int f){
  if(t_esit(d1,f)) return 'R'; if(t_esit(d2,f)) return 'C';
  if(((d1==D_n2&&d2==D_a2)||(d1==D_a2&&d2==D_n2)) && AL[f]%2 && BE[f]%2) return 'D'; return 'X'; }
static inline int mkredi(uint32_t m, int f, int i, char y){ const int p=PR[i];
  if(y=='R') return kredi(m,f*NPR+i);
  int v; if(y=='C'||y=='D') v=(p==3)?1:0; else v=(p==3)?((BE[f]>0)?4:5):((BE[f]>0)?2:3);
  return __builtin_popcount(m)-MRED[v][(m>>2)&0x1FFFFu]; }
static inline int dsin(int d, int n, int a, int t){ switch(d){ case D_t2: return t&1; case D_n2: return n&1; case D_a2: return a&1;
  case D_h: return n>57; default: return (((t%4)+4)%4)>=2; } }
// B1 sabit mi (çift ve biçim için): D1 yalnız konuma/değere bağlıysa
static inline int b1_sabit(int d1, int f){ if(d1==D_n2||d1==D_a2||d1==D_h) return 1; if(d1==D_t2 && (AL[f]%2==0||BE[f]%2==0)) return 1; return 0; }

typedef struct { int k; int sc[32]; long ss[32]; int qmin,qmax; uint64_t b0,b1; long sa,sn; int qv[114]; } mfam_t;
// Bir kitabın menü puanları: tum = 20 çift üzerinde en iyi; t2siz = t2 içermeyen 12 çift; bir = yalnız (t2,n2) (birincil eşitlik denetimi)
typedef struct { int tum, t2siz, bir; int tumC, tumL, t2sizC, t2sizL; } menu_t;
static uint8_t *MENU_DOK=NULL;   // isteğe bağlı: [çift][mercek] kredileri (denetim dökümü)
static void menu_degerlendir(const int *a, menu_t *o, int ayrinti_L /* kullanılmıyor */, int *birL){
  static __thread mfam_t FF[NPR]; static __thread hfam_t HH[NPR];
  o->tum=o->t2siz=o->bir=-1; o->tumC=o->tumL=o->t2sizC=o->t2sizL=-1;
  for(int f=0;f<NFORM;f++){ const int al=AL[f], be=BE[f], abe=abs(be);
    for(int i=0;i<NPR;i++){ mfam_t *F=&FF[i]; F->k=0; memset(F->sc,0,sizeof F->sc); memset(F->ss,0,sizeof F->ss); F->qmin=1<<30; F->qmax=-(1<<30); F->b0=F->b1=0; F->sa=F->sn=0;
      HH[i].k=0; HH[i].sq=HH[i].sa=HH[i].sn=0; HH[i].b0=HH[i].b1=0; }
    int e0[ND]={0}; long sn0[ND]={0}, sa0[ND]={0}, st0[ND]={0}; long st=0;
    for(int n=1;n<=114;n++){ const int x=a[n-1]; const int t=al*n+be*x, u=al*n-be*x; st+=t;
      int sig=0; for(int d=0;d<ND;d++){ int c=dsin(d,n,x,t); sig|=c<<d; if(!c){ e0[d]++; sn0[d]+=n; sa0[d]+=x; st0[d]+=t; } }
      uint32_t mt=DIVM[t<0?-t:t];
      while(mt){ int i=__builtin_ctz(mt); mt&=mt-1; mfam_t *F=&FF[i]; const int q=(int32_t)((uint32_t)t*INV32[i]);
        F->sc[sig]++; F->ss[sig]+=q; F->qv[F->k++]=q; if(q<F->qmin) F->qmin=q; if(q>F->qmax) F->qmax=q; F->sa+=x; F->sn+=n;
        if(n<=64) F->b0|=1ULL<<(n-1); else F->b1|=1ULL<<(n-65); }
      uint32_t mu=DIVM[u<0?-u:u];
      while(mu){ int i=__builtin_ctz(mu); mu&=mu-1; hfam_t *H=&HH[i]; H->k++; H->sq+=(int32_t)((uint32_t)u*INV32[i]); H->sa+=x; H->sn+=n;
        if(n<=64) H->b0|=1ULL<<(n-1); else H->b1|=1ULL<<(n-65); } }
    uint32_t mbD[ND];
    for(int d=0;d<ND;d++){ uint32_t mb=0; const int e1=114-e0[d]; const long sn1=SN_TOT-sn0[d], sa1=SA_TOT-sa0[d], st1=st-st0[d];
      if(e0[d]==e1 && !b1_sabit(d,f)) mb|=1u<<P_B1;
      if(e0[d]>0 && e1>0 && ((long)al*sn0[d]==(long)abe*sa1 || (long)al*sn1==(long)abe*sa0[d] || st0[d]==st1)) mb|=1u<<P_B2;
      mbD[d]=mb; }
    for(int i=0;i<NPR;i++){ const int p=PR[i]; const int L=f*NPR+i; mfam_t *F=&FF[i]; hfam_t *H=&HH[i];
      // çiftten bağımsız kısım
      uint32_t m0=0; long sq=0; for(int s=0;s<32;s++) sq+=F->ss[s];
      { fam_t tmp; tmp.k=F->k; tmp.qmin=F->qmin; tmp.qmax=F->qmax; memcpy(tmp.qv,F->qv,sizeof(int)*F->k); if(kosu_q(&tmp)) m0|=1u<<P_K2; }
      if(F->k>=1 && sq%p==0) m0|=1u<<P_QS;
      if(F->k==p) m0|=1u<<P_QkF;
      const uint64_t i0=F->b0&H->b0, i1=F->b1&H->b1; const int ni=__builtin_popcountll(i0)+__builtin_popcountll(i1); const int kU=F->k+H->k-ni;
      long saU=F->sa+H->sa, snU=F->sn+H->sn;
      if(ni){ uint64_t w=i0; while(w){ int b=__builtin_ctzll(w); w&=w-1; saU-=a[b]; snU-=b+1; } w=i1; while(w){ int b=__builtin_ctzll(w); w&=w-1; saU-=a[b+64]; snU-=b+65; } }
      if(F->k>=1 && H->k>=1 && ni==0) m0|=1u<<P_W1;
      if(kU==p) m0|=1u<<P_QU; if(H->k==p) m0|=1u<<P_QkH;
      if(kU>=1 && saU%p==0) m0|=1u<<P_Qa; if(kU>=1 && snU%p==0) m0|=1u<<P_Qn; if(H->k>=1 && H->sq%p==0) m0|=1u<<P_QH;
      for(int c=0;c<NCIFT;c++){ const int d1=CIFT[c][0], d2=CIFT[c][1];
        int cn[4]={0,0,0,0}; long cs[4]={0,0,0,0};
        for(int s=0;s<32;s++){ if(!F->sc[s]) continue; int ci=((s>>d1)&1)*2+((s>>d2)&1); cn[ci]+=F->sc[s]; cs[ci]+=F->ss[s]; }
        const int c0=cn[0],c1=cn[1],c2=cn[2],c3=cn[3]; const long s0=cs[0],s1=cs[1],s2=cs[2],s3=cs[3];
        const int h0=c0+c1, h1=c2+c3; const long q0=s0+s1, q1=s2+s3; const int dolu=(c0&&c1&&c2&&c3);
        uint32_t m=m0|mbD[d1];
        if(h0==h1 && h0>=1) m|=1u<<P_F1; if(h0>=1 && h1>=1 && q0==q1) m|=1u<<P_F2;
        if(c0==c1 && c0>=1) m|=1u<<P_R0c; if(c0>=1 && c1>=1 && s0==s1) m|=1u<<P_R0e;
        if(c2==c3 && c2>=1) m|=1u<<P_R1c; if(c2>=1 && c3>=1 && s2==s3) m|=1u<<P_R1e;
        if(dolu && kosu4(s0,s1,s2,s3)) m|=1u<<P_K1; if(dolu && sq==4L*p) m|=1u<<P_Qc;
        if(h0>=1 && h1>=1 && q0%p==0 && q1%p==0) m|=1u<<P_Qh;
        uint32_t ex=EXCL[L]&~(1u<<P_B1); m&=~ex;                 // p | αβ dışlamaları (B1 çifte göre yukarıda)
        int mc=c0; if(c1<mc) mc=c1; if(c2<mc) mc=c2; if(c3<mc) mc=c3;
        if(!(F->k*p>=228 && mc>=2)) m&=~GATED_MASK;
        const int kr=mkredi(m,f,i,yapi(d1,d2,f)); if(MENU_DOK) MENU_DOK[c*NLENS+L]=(uint8_t)kr;
        if(kr>o->tum){ o->tum=kr; o->tumC=c; o->tumL=L; }
        if(d1!=D_t2 && d2!=D_t2 && kr>o->t2siz){ o->t2siz=kr; o->t2sizC=c; o->t2sizL=L; }
        if(d1==D_t2 && d2==D_n2){ if(kr>o->bir) o->bir=kr; if(birL) birL[L]=kr; } } } } }
#endif
