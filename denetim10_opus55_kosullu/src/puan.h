// puan.h — sabit mercek (1,1,19) puanlaması: robotun T2'si ve tekrarsız T2* (kurallar.h'deki kesin gerektirmeler).
// Tablo: olgu × bağlam isabet sayıları (robot4_m table_t'nin tek-mercek hâli); olasılık p = (isabet+0,5)/(deneme+1),
// bağlam denemesi < MINTR (1000) ise havuzlanmış olasılık (robotla aynı). LOO: bir kitabın kendi katkısı çıkarılır.
#ifndef PUAN_H
#define PUAN_H
#include "ortak.h"
typedef struct { uint64_t N; uint64_t hit[NF][NCTX]; uint64_t trF[NCTX], trG[NCTX]; } tablo_t;
static inline int cb(int k){ return k<NCTX-1?k:NCTX-1; }
static void tablo_ekle(tablo_t *T, const fres_t *R){ int cF=cb(R->kF), cG=cb(R->kG); T->N++; T->trF[cF]++; T->trG[cG]++;
  u128 b=R->bits; while(b){ int f=ctz128(b); b&=b-1; int c=FCTX[f]==0?0:(FCTX[f]==1?cF:cG); T->hit[f][c]++; } }
#define MINTR 1000
// loo=1: R'nin kitabı tabloda; kendi katkısı çıkarılır (R'deki doğru olgular için)
static inline double pfact_t(const tablo_t *T, int f, const fres_t *R, int loo){
  int c; double tr, h; double N=(double)T->N-(loo?1:0);
  if(FCTX[f]==0){ c=0; tr=N; } else if(FCTX[f]==1){ c=cb(R->kF); tr=(double)T->trF[c]-(loo?1:0); } else { c=cb(R->kG); tr=(double)T->trG[c]-(loo?1:0); }
  h=(double)T->hit[f][c]-(loo?1:0);
  if(FCTX[f]!=0 && tr<MINTR){ double hh=0; for(int k=0;k<NCTX;k++) hh+=T->hit[f][k]; hh-=(loo?1:0); return (hh+0.5)/(N+1.0); }
  return (h+0.5)/(tr+1.0); }

// ---------------- kurallar ----------------
typedef struct { u128 prem; int concl; const char *ad; } kural_t;
#define KMAXK 64
typedef struct { kural_t k[KMAXK]; int n; u128 bagli; u128 kapali; u128 kapali_kosul; } kurallar_t;
static void kural_ekle(kurallar_t *K, u128 prem, int concl, const char *ad){ K->k[K->n].prem=prem; K->k[K->n].concl=concl; K->k[K->n].ad=ad; K->n++; K->bagli|=prem|BIT(concl); }
static inline int gereksiz_mi(const kurallar_t *K, u128 S){ for(int r=0;r<K->n;r++){ const kural_t *k=&K->k[r]; if(((S>>k->concl)&1) && (S&k->prem)==k->prem) return 1; } return 0; }

typedef struct { double t2r, t2t; u128 hik_r, hik_t; double nodev[NNODE]; } puan_t;
// Robot T2 (en iyi 8 düğüm; düğümde en nadir olgu) ve tekrarsız T2*.
static void puanla(const tablo_t *T, const fres_t *R, int loo, const kurallar_t *K, puan_t *P){
  u128 b=R->bits & ~K->kapali; double x[NF]; int dogru[NF]; int nd=0;
  if(R->same){ for(int f=0;f<NFACT;f++){ int n=FNODE[f]; if((n>=6&&n<=11)||n==14) b&=~BIT(f); } }
  double nodev[NNODE]; int nodef[NNODE]; for(int i=0;i<NNODE;i++){ nodev[i]=0; nodef[i]=-1; }
  u128 bb=b; while(bb){ int f=ctz128(bb); bb&=bb-1; double p=pfact_t(T,f,R,loo); double v=-log10(p); if(v<0) v=0; x[f]=v; dogru[nd++]=f;
    if(v>nodev[FNODE[f]]||nodef[FNODE[f]]<0){ if(v>nodev[FNODE[f]]||nodef[FNODE[f]]<0){ nodev[FNODE[f]]=v; nodef[FNODE[f]]=f; } } }
  memcpy(P->nodev,nodev,sizeof nodev);
  // robot T2
  { int ord[NNODE]; for(int i=0;i<NNODE;i++) ord[i]=i; for(int i=1;i<NNODE;i++){ int y=ord[i]; int j=i-1; while(j>=0&&nodev[ord[j]]<nodev[y]){ ord[j+1]=ord[j]; j--; } ord[j+1]=y; }
    double s=0; u128 h=0; for(int i=0;i<8;i++){ s+=nodev[ord[i]]; if(nodef[ord[i]]>=0&&nodev[ord[i]]>0) h|=BIT(nodef[ord[i]]); } P->t2r=s; P->hik_r=h; }
  // tekrarsız T2*: bağlı düğümlerde seçenekleri sırala
  u128 bagli_dogru=b & K->bagli;
  if(!bagli_dogru){ P->t2t=P->t2r; P->hik_t=P->hik_r; return; }
  double ub[NNODE]; int uf[NNODE]; for(int i=0;i<NNODE;i++){ ub[i]=0; uf[i]=-1; }
  int lf[NNODE][8], nl[NNODE]; memset(nl,0,sizeof nl);
  for(int t=0;t<nd;t++){ int f=dogru[t], n=FNODE[f]; if((K->bagli>>f)&1){ if(nl[n]<8) lf[n][nl[n]++]=f; } else if(uf[n]<0||x[f]>ub[n]){ ub[n]=x[f]; uf[n]=f; } }
  int ln[NNODE], nln=0; for(int n=0;n<NNODE;n++) if(nl[n]) ln[nln++]=n;
  int sec[NNODE]; memset(sec,0,sizeof sec); // sec[i] ∈ [0, nl+1): 0 = bağsız en iyi (ya da hiç), k = lf[k-1]
  double best=-1; u128 besth=0;
  for(;;){
    u128 S=0; for(int i=0;i<nln;i++){ int n=ln[i]; if(sec[i]>0) S|=BIT(lf[n][sec[i]-1]); }
    if(!gereksiz_mi(K,S)){
      double v[NNODE]; int vf[NNODE]; for(int n=0;n<NNODE;n++){ v[n]=ub[n]; vf[n]=uf[n]; }
      for(int i=0;i<nln;i++){ int n=ln[i]; if(sec[i]>0){ int f=lf[n][sec[i]-1]; v[n]=x[f]; vf[n]=f; } }
      int ord[NNODE]; for(int i=0;i<NNODE;i++) ord[i]=i; for(int i=1;i<NNODE;i++){ int y=ord[i]; int j=i-1; while(j>=0&&v[ord[j]]<v[y]){ ord[j+1]=ord[j]; j--; } ord[j+1]=y; }
      double s=0; u128 h=0; for(int i=0;i<8;i++){ s+=v[ord[i]]; if(vf[ord[i]]>=0&&v[ord[i]]>0) h|=BIT(vf[ord[i]]); }
      if(s>best+1e-12){ best=s; besth=h; } }
    int i=0; while(i<nln){ sec[i]++; if(sec[i]<=nl[ln[i]]) break; sec[i]=0; i++; } if(i==nln) break; }
  P->t2t=best; P->hik_t=besth; }
#endif
