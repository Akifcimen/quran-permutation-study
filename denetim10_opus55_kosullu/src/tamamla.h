// tamamla.h — iskelet (t ≡ 0 ailesi: konumlar + değerleri) SABİT tutularak tamamlamanın koşullu dağılımından örnekleme.
// Hedefler: U (tekdüze: geçerli tamamlamalar üzerinde düzgün), M (M20: aynı + S = Σn·a ≤ S_obs), L (Mallows: ∝ q^I).
// "Geçerli" = aile dışındaki hiçbir konum t ≡ 0 almaz (aile tam olarak iskelettir; O/G koşulu korunur).
// Zincir: aile dışı iki konumun değerleri takas edilir (simetrik öneri), Metropolis kabulü (U, M: kısıt; L: min(1, q^ΔI)).
// L'de öneri karışımı: yarı olasılıkla yerel (|i−j| ≤ DL), yarı olasılıkla küresel (iki aile dışı konum) — ikisi de simetrik.
// Başlangıç: verilen kitabın kendi tamamlaması (U ve L'de koşullu dağılımdan kesin bir çekiliş, M'de durağan yaklaşık).
// A (terazi) ve W (İki El) durumları her kabulde O(1) güncellenir.
#ifndef TAMAMLA_H
#define TAMAMLA_H
#include "ortak.h"
#define DL 12
typedef struct {
  char null; int a[114]; uint8_t fam[114]; int fr[114], nfr; long S; double q, lq; rng_t r;
  int ecnt; long sumTE; int nu, uniCnt; long uniSa; long kabul, oneri;
} tz_t;
static inline int tz_te(int i, int v){ return (((i+1)+v)&1)==0; }
static inline int tz_fu(int i, int v){ return (((i+1)-v)%19)==0; }
static void tz_yeniden_say(tz_t *Z){ Z->ecnt=0; Z->sumTE=0; Z->nu=0; Z->uniCnt=0; Z->uniSa=0; Z->S=0;
  for(int i=0;i<114;i++){ int v=Z->a[i]; Z->S+=(long)(i+1)*v; if(tz_te(i,v)){ Z->ecnt++; Z->sumTE+=(i+1)+v; } int fu=tz_fu(i,v); if(fu) Z->nu++; if(fu||Z->fam[i]){ Z->uniCnt++; Z->uniSa+=v; } } }
static void tz_init(tz_t *Z, char null, const int *a, uint64_t seed, double q){
  memset(Z,0,sizeof *Z); Z->null=null; memcpy(Z->a,a,sizeof Z->a); rseed(&Z->r,seed); Z->q=q; Z->lq=(q>0)?log(q):0;
  for(int i=0;i<114;i++){ Z->fam[i]=(((i+1)+a[i])%19==0); if(!Z->fam[i]) Z->fr[Z->nfr++]=i; }
  tz_yeniden_say(Z); }
static inline int tz_A(const tz_t *Z){ return Z->ecnt==57 && Z->sumTE==SA_TOT; }
static inline int tz_W(const tz_t *Z){ return Z->nu==7 && Z->uniCnt==19 && (Z->uniSa%19)==0; }
static inline void tz_kaldir(tz_t *Z, int i){ int v=Z->a[i]; if(tz_te(i,v)){ Z->ecnt--; Z->sumTE-=(i+1)+v; } if(tz_fu(i,v)){ Z->nu--; Z->uniCnt--; Z->uniSa-=v; } }
static inline void tz_ekle(tz_t *Z, int i){ int v=Z->a[i]; if(tz_te(i,v)){ Z->ecnt++; Z->sumTE+=(i+1)+v; } if(tz_fu(i,v)){ Z->nu++; Z->uniCnt++; Z->uniSa+=v; } }
static inline int tz_adim(tz_t *Z){
  int i,j; Z->oneri++;
  if(Z->null=='L' && (xnext(&Z->r)&1)){ // yerel
    i=Z->fr[bounded(&Z->r,(uint32_t)Z->nfr)]; int d=1+(int)bounded(&Z->r,DL); if(xnext(&Z->r)&1) d=-d; j=i+d; if(j<0||j>113||Z->fam[j]) return 0; }
  else { i=Z->fr[bounded(&Z->r,(uint32_t)Z->nfr)]; j=Z->fr[bounded(&Z->r,(uint32_t)Z->nfr)]; if(i==j) return 0; }
  int x=Z->a[i], y=Z->a[j]; if(x==y) return 0;
  if((((i+1)+y)%19)==0 || (((j+1)+x)%19)==0) return 0;            // yeni aile üyesi yasak
  long dS=(long)((i+1)-(j+1))*(y-x);
  if(Z->null=='M'){ if(Z->S+dS>S_OBS) return 0; }
  else if(Z->null=='L'){ int lo=i<j?i:j, hi=i<j?j:i; int u=Z->a[lo], w=Z->a[hi]; // lo konumunda u, hi konumunda w; takas sonrası lo:w, hi:u
    long dI=(long)(w<u)-(long)(u<w); for(int k=lo+1;k<hi;k++){ int z=Z->a[k]; dI+=(long)(w<z)-(long)(u<z)+(long)(z<u)-(long)(z<w); }
    if(dI>0){ double ac=exp(Z->lq*dI); if(unif(&Z->r)>=ac) return 0; } }
  tz_kaldir(Z,i); tz_kaldir(Z,j); Z->a[i]=y; Z->a[j]=x; tz_ekle(Z,i); tz_ekle(Z,j); Z->S+=dS; Z->kabul++; return 1; }
#endif
