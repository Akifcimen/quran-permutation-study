// bilgi.h — bilgi-ağırlıklı bütünlük puanı (ONKAYIT §3.3). Mercek L için:
//   I(L) = I_B(biçim) + I_boy(L) + I_F(L | kF) + I_W(L | kF) − c(L)
//   I_B  = −log2 P(kitap maskesindeki yüklemlerin hepsi | biçim)              (en-az olasılık)
//   I_boy= −log2 P(kF = p)  yalnız QkF doğruysa (koşulsuz)
//   I_F  = −log2 P(aile-içi maskedeki yüklemlerin hepsi | kF bağlamı)
//   I_W  = −log2 P(iki-el maskesindeki yüklemlerin hepsi | kF bağlamı)
//   c(L) = biçim bedeli + asal bedeli (önsele göre). Kitabın puanı: max_L I(L).
// Değişkeler v = 4·kapı + önsel: kapı 0 = yok (birincil; kF'ye koşullama), 1 = G kapısı (kapı tutmazsa I_F = I_boy = 0);
// önsel 0 = geo2 biçim + eşit asal (birincil), 1 = eşit biçim, 2 = geo2 + metin ağırlıklı asal, 3 = geo4 biçim.
#ifndef BILGI_H
#define BILGI_H
#include "tablo.h"
#define NVAR 8
static double COST[4][NLENS];
static double bicim_bedeli(int f, double taban){ double s=0; for(int g=0;g<NFORM;g++) s+=pow(taban,-(AL[g]+abs(BE[g])-2)); return -log2(pow(taban,-(AL[f]+abs(BE[f])-2))/s); }
static double asal_metin(int p){ double u=1.0/87, l=1.0/15, d=1.0/12; if(p==3||p==5||p==7||p==19) return u+l+d; if(p==11) return u+l; return u; }
static void bedel_hazirla(void){
  for(int f=0;f<NFORM;f++) for(int i=0;i<NPR;i++){ int L=f*NPR+i;
    COST[0][L]=bicim_bedeli(f,2.0)+log2(29.0);
    COST[1][L]=log2(14.0)+log2(29.0);
    COST[2][L]=bicim_bedeli(f,2.0)-log2(asal_metin(PR[i]));
    COST[3][L]=bicim_bedeli(f,4.0)+log2(29.0); } }
typedef struct { yuzey_t B, S, F, W; int ctx; uint64_t n; int kapi; } bilesen_t;
static inline yuzey_t sifir_y(void){ yuzey_t y; y.pt=y.lo=y.hi=0; y.coz=1; return y; }
static inline void mercek_bilesen(const tablo_t *T, const profil_t *P, int L, bilesen_t *b){
  const uint32_t m=P->m[L]; const int f=L/NPR, p=PR[L%NPR]; const int k=kkutu(P->kF[L]); const int c=T->ctx[L*KC+k]; const uint64_t n=T->cn[c];
  b->ctx=c; b->n=n; b->kapi=kapi_tut(P,L,0);
  const uint32_t bm=bmask2(m); b->B = bm? yuzey(T->BA[f][bm],T->N) : sifir_y();
  b->S = (m&(1u<<P_QkF))? yuzey(p<KC? T->NKU[(size_t)L*KC+p]:0, T->N) : sifir_y();
  const uint32_t fm=fmask11(m); b->F = fm? yuzey(T->FA[(size_t)c*NFM+fm], n) : sifir_y();
  const uint32_t wm=wmask6(m); b->W = wm? yuzey(T->WA[(size_t)c*NWM+wm], n) : sifir_y(); }
typedef struct { double pt[NVAR], lo[NVAR], hi[NVAR]; int L[NVAR]; int coz[NVAR]; } bilgi_t;   // coz: en iyi (pt) mercekte bütün aramalar çözülmüş mü
static void bilgi_puan(const tablo_t *T, const profil_t *P, bilgi_t *o){
  for(int v=0;v<NVAR;v++){ o->pt[v]=o->lo[v]=o->hi[v]=-1e300; o->L[v]=-1; o->coz[v]=1; }
  for(int L=0;L<NLENS;L++){ bilesen_t b; mercek_bilesen(T,P,L,&b);
    double r0p=b.B.pt+b.S.pt+b.F.pt+b.W.pt, r0l=b.B.lo+b.S.lo+b.F.lo+b.W.lo, r0h=b.B.hi+b.S.hi+b.F.hi+b.W.hi;
    int c0=b.B.coz&&b.S.coz&&b.F.coz&&b.W.coz;
    double r1p, r1l, r1h; int c1;
    if(b.kapi){ r1p=r0p; r1l=r0l; r1h=r0h; c1=c0; } else { r1p=b.B.pt+b.W.pt; r1l=b.B.lo+b.W.lo; r1h=b.B.hi+b.W.hi; c1=b.B.coz&&b.W.coz; }
    for(int g=0;g<2;g++) for(int pr=0;pr<4;pr++){ int v=g*4+pr; double cst=COST[pr][L];
      double sp=(g?r1p:r0p)-cst, sl=(g?r1l:r0l)-cst, sh=(g?r1h:r0h)-cst;
      if(sp>o->pt[v]){ o->pt[v]=sp; o->L[v]=L; o->coz[v]=g?c1:c0; }
      if(sl>o->lo[v]) o->lo[v]=sl; if(sh>o->hi[v]) o->hi[v]=sh; } } }
#endif
