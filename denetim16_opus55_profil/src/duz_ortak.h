// duz_ortak.h — duz.c'nin yürüyüşünün (öneri, kabul, Wang–Landau 1/t, ölçüm) ekran kartı için yeniden yazımı. duz.c ile aynı RNG çağrı
// sırası ve aynı aritmetik; profil kart_ortak.h k_mercek ile (kdegerlendir'le bayt bayt eşdeğerliği kabul edilmiş çekirdek).
// Aynı tohumla duz.c ile aynı yörüngeyi üretmesi beklenir (tek fark: log() kütüphanesinin son basamağı; kabul karşılaştırmasını
// değiştirme olasılığı adım başına ~1e-16).
#ifndef DUZ_ORTAK_H
#define DUZ_ORTAK_H
#include "kart_ortak.h"
#define DKMAX 32
typedef struct { int L, nl; uint64_t om[5]; } dolay_t;                 // nl = koşul sayısı + 1 (D = 0 … nl−1)
typedef struct { krng_t r; int16_t a[114]; int64_t S; int D, e, asama, min_d; int64_t t, u, WL, OL; double lf;
  double lg[DKMAX+2]; int64_t H[DKMAX+2], V[DKMAX+2]; uint8_t gor[DKMAX+2]; } yur_t;
KD static inline double d_unif(krng_t *r){ return (k_xnext(r)>>11)*(1.0/9007199254740992.0); }
KD static inline int d_bozuk(const int16_t *a, const dolay_t *E, const ksabit_t *C, kprof_t *P){
  const int L=E->L, f=L/K_NPR, i=L%K_NPR; kbk_t bk; k_kitap(a,C->al[f],C->be[f],&bk); const uint8_t mb0=k_mb0(&bk,C->al[f],C->be[f],C);
  k_mercek(a,f,i,&bk,mb0,C,P);
  int d=k_kapi(C,P,L)?0:1;
  d+=k_pc64(E->om[0]&~(uint64_t)P->B[L])+k_pc64(E->om[1]&~(uint64_t)P->Z[L])+k_pc64(E->om[2]&~P->F[L])+k_pc64(E->om[3]&~P->W[L])+k_pc64(E->om[4]&~(uint64_t)P->H[L]);
  return d; }
KD static inline int d_cift(krng_t *r, const int16_t *a, int m, int *pi, int *pj){
  int cnt[256]; for(int c=0;c<m;c++) cnt[c]=0; for(int i=0;i<114;i++) cnt[a[i]%m]++;
  long long P=0; for(int c=0;c<m;c++) P+=(long long)cnt[c]*(cnt[c]-1)/2;
  if(P==0) return 0;
  long long k=(long long)k_bounded(r,(uint32_t)P); int c=0; for(;c<m;c++){ const long long pc=(long long)cnt[c]*(cnt[c]-1)/2; if(k<pc) break; k-=pc; }
  int pos[114], n=0; for(int i=0;i<114;i++) if(a[i]%m==c) pos[n++]=i;
  int x=0; while(k>=n-1-x){ k-=n-1-x; x++; } *pi=pos[x]; *pj=pos[x+1+(int)k]; return 1; }
KD static inline int d_adim(krng_t *r, int16_t *a, int64_t *S, int D, const double *lg, const dolay_t *E, const ksabit_t *C, kprof_t *P, double LQ){
  int i, j; const int tur=(int)k_bounded(r,3), p=C->pr[E->L%K_NPR];
  if(tur==0){ i=(int)k_bounded(r,114); j=(int)k_bounded(r,113); if(j>=i) j++; }
  else if(!d_cift(r,a,tur==1?p:2*p,&i,&j)) return D;
  if(a[i]==a[j]) return D;
  if(i>j){ const int t=i; i=j; j=t; }
  const int x=a[i], y=a[j]; int64_t dS=0; double lr=0;
  if(C->sif=='M'){ dS=(int64_t)((i+1)-(j+1))*(y-x); if(*S+dS>C->s_obs) return D; }
  else if(C->sif=='L'){ int dI=(y<x)-(x<y); for(int k=i+1;k<j;k++){ const int z=a[k]; dI+=(y<z)-(x<z)+(z<x)-(z<y); } lr=dI*LQ; }
  a[i]=(int16_t)y; a[j]=(int16_t)x; const int D2=d_bozuk(a,E,C,P);
  const double l=lr+lg[D]-lg[D2];
  if(l<0 && log(d_unif(r))>=l){ a[i]=(int16_t)x; a[j]=(int16_t)y; return D; }
  *S+=dS; return D2; }
// Bir yürüyüşün en çok `parca` adımı (aşama 1: WL 1/t, aşama 2: ölçüm). duz.c isci() döngüsüyle aynı.
KD static inline void d_yuru(yur_t *y, const dolay_t *E, const ksabit_t *C, kprof_t *P, double LQ, int64_t parca){
  const int nl=E->nl; const int64_t WL=y->WL, OL=y->OL;
  for(int64_t s=0;s<parca;s++){
    if(y->t<WL){ const int64_t t=++y->t; const int D=d_adim(&y->r,y->a,&y->S,y->D,y->lg,E,C,P,LQ); y->D=D;
      y->lg[D]+=y->lf; y->H[D]++; if(!y->gor[D]) y->gor[D]=1; if(D<y->min_d) y->min_d=D;
      if(y->asama==0 && (t&1023)==0 && y->gor[0]){ int tamam=1; for(int d=0;d<nl;d++) if(y->gor[d] && !y->H[d]){ tamam=0; break; }
        if(tamam){ y->lf/=2; for(int d=0;d<DKMAX+2;d++) y->H[d]=0; if(y->lf<(double)nl/t) y->asama=1; } }
      if(y->asama==1) y->lf=(double)nl/t; }
    else if(y->u<OL){ y->u++; const int D=d_adim(&y->r,y->a,&y->S,y->D,y->lg,E,C,P,LQ); y->D=D; y->V[D]++; }
    else break; } }
#endif
