// hizli.h — sıralı sıfırlarda O-koşullu kitap avı için hızlı örnekleyiciler.
// M20: robotun 'M' sıfırının BİREBİR aynısı (64 zincir, tam azalandan 30000 adım yakma, 20 önerilik seyreltme, aynı RNG
//      tüketimi) ama aile durumu (kF, Σc) her kabul edilen takasta artımlı güncellenir; yalnız kF=12 ∧ Σc=76 iken tam olay
//      hesabı yapılır. Çıktı akışı robotun sampler_next('M') akışıyla aynı kitap dizisidir (kontrol/hizli_sina.c).
// Mallows: Lehmer kodu, kesik geometrik alias (tek 64-bit çekiliş: üst 32 bit indeks, alt 32 bit eşik; nicemleme 2^-32),
//      elle kaydırma, döngü içinde aile sayımı ve ön-eleme.
#ifndef HIZLI_H
#define HIZLI_H
#include "ortak.h"
static signed char COMPC[115][300];   // COMPC[n][a] = (n+a)/19 eğer 19|(n+a), yoksa 0
static void comp_hazirla(void){ for(int n=0;n<=114;n++) for(int v=0;v<300;v++) COMPC[n][v]=(n>0&&(n+v)%19==0)?(signed char)((n+v)/19):0; }

// ---------------- M20 ----------------
typedef struct { int b[114]; long S; int kF, cs; } mz_t;
typedef struct { rng_t r; mz_t ch[NCH]; int cur; int thin; long emis; } m20_t;
static void m20_init(m20_t *Z, uint64_t seed, int thin){
  memset(Z,0,sizeof *Z); rseed(&Z->r,seed); Z->thin=thin; Z->cur=0;
  int srt[114]; memcpy(srt,BASE,sizeof srt); for(int i=1;i<114;i++){ int x=srt[i]; int j=i-1; while(j>=0&&srt[j]<x){srt[j+1]=srt[j];j--;} srt[j+1]=x; }
  if(getenv("M20_BASLANGIC") && !strcmp(getenv("M20_BASLANGIC"),"gercek")) memcpy(srt,BASE,sizeof srt);   // Ç3 başlangıç denetimi
  for(int c=0;c<NCH;c++){ int *a=Z->ch[c].b; memcpy(a,srt,sizeof srt); long S=0; for(int i=0;i<114;i++) S+=(long)(i+1)*a[i];
    for(int bb=0;bb<30000;bb++){ int i=(int)bounded(&Z->r,114), j=(int)bounded(&Z->r,114); long dS=(long)((i+1)-(j+1))*(a[j]-a[i]); if(S+dS<=S_OBS){ int t=a[i]; a[i]=a[j]; a[j]=t; S+=dS; } }
    Z->ch[c].S=S; int kF=0,cs=0; for(int i=0;i<114;i++){ int c2=COMPC[i+1][a[i]]; if(c2){ kF++; cs+=c2; } } Z->ch[c].kF=kF; Z->ch[c].cs=cs; }
}
// bir emisyon: sıradaki zinciri thin öneri ilerlet; zincir göstergesini döndür (kitap Z->ch[c].b)
static inline int m20_emit(m20_t *Z){
  int c=Z->cur; Z->cur=(Z->cur+1)%NCH; mz_t *m=&Z->ch[c]; int *b=m->b; long S=m->S; int kF=m->kF, cs=m->cs; rng_t r=Z->r;
  for(int st=0;st<Z->thin;st++){ int i=(int)bounded(&r,114), j=(int)bounded(&r,114); long dS=(long)((i+1)-(j+1))*(b[j]-b[i]);
    if(S+dS<=S_OBS){ int ai=b[i], aj=b[j]; int o1=COMPC[i+1][ai], o2=COMPC[j+1][aj], n1=COMPC[i+1][aj], n2=COMPC[j+1][ai];
      kF+=(n1!=0)+(n2!=0)-(o1!=0)-(o2!=0); cs+=n1+n2-o1-o2; b[i]=aj; b[j]=ai; S+=dS; } }
  Z->r=r; m->S=S; m->kF=kF; m->cs=cs; Z->emis++; return c; }

// ---------------- Mallows ----------------
typedef struct { rng_t r; double q; int ref[114]; uint32_t thr[115][114]; uint8_t ali[115][114]; long n; } lz_t;
static void lz_init(lz_t *Z, double q, uint64_t seed){
  memset(Z,0,sizeof *Z); rseed(&Z->r,seed); Z->q=q; ref_hazirla(); memcpy(Z->ref,REF_DESC,sizeof Z->ref);
  for(int R=1;R<=114;R++){ double w[114], s=0, p=1; for(int j=0;j<R;j++){ w[j]=p; s+=p; p*=q; }
    double sc[114]; int small[114], large[114], ns=0, nl=0; double P[114]; int A[114];
    for(int j=0;j<R;j++){ sc[j]=w[j]/s*R; if(sc[j]<1.0) small[ns++]=j; else large[nl++]=j; }
    while(ns>0 && nl>0){ int l=small[--ns], g=large[--nl]; P[l]=sc[l]; A[l]=g; sc[g]=(sc[g]+sc[l])-1.0; if(sc[g]<1.0) small[ns++]=g; else large[nl++]=g; }
    while(nl>0){ int g=large[--nl]; P[g]=1.0; A[g]=g; } while(ns>0){ int l=small[--ns]; P[l]=1.0; A[l]=l; }
    for(int j=0;j<R;j++){ double t=P[j]*4294967296.0; Z->thr[R][j]=(t>=4294967295.0)?0xFFFFFFFFu:(uint32_t)t; Z->ali[R][j]=(uint8_t)A[j]; } }
}
// bir kitap: a[] doldur; dönüş: ön-eleme geçti mi (kF==12 && Σc==76)
static inline int lz_kitap(lz_t *Z, int *restrict a){
  int rem[114+48]; for(int i=0;i<114;i++) rem[i]=Z->ref[113-i]; for(int i=114;i<114+48;i++) rem[i]=0;
  int kF=0, cs=0; rng_t r=Z->r;   // yerel kopya: durum yazmaçlarda kalsın
  for(int p=0;p<114;p++){ int R=114-p; uint64_t x=xnext(&r); int j=(int)(((x>>32)*(uint64_t)R)>>32); uint32_t u=(uint32_t)x;
    uint32_t th=Z->thr[R][j]; int V=(u<th||th==0xFFFFFFFFu)?j:Z->ali[R][j]; int ix=R-1-V; int v=rem[ix]; a[p]=v;
    // kaydırma: rem[ix..R-2] = rem[ix+1..R-1] (V eleman). Sabit 32'lik vektör kopyası (dolgu sayesinde taşmaz; R-1'in ötesi kullanılmaz),
    // V > 32 ise kalan kısım döngüyle — sonuç skaler döngüyle birebir aynı.
    __builtin_memmove(rem+ix, rem+ix+1, 32*sizeof(int));
    for(int k=ix+32;k<R-1;k++) rem[k]=rem[k+1];
    int c=COMPC[p+1][v]; if(c){ kF++; cs+=c; } }
  Z->r=r; Z->n++; return (kF==12 && cs==76); }

// ---------------- J10 (robotun J sıfırının dağılımca aynısı; Box–Muller'in iki çıktısı da kullanılır) ----------------
// anahtar_i = i + σ·Z_i (Z_i bağımsız N(0,1)), konumlar anahtar sırasına göre; a[k] = BASE[idx[k]]. Robotun gauss()'u yalnız cos çıktısını
// kullanır; burada (cos, sin) çifti iki bağımsız normal verir → aynı dağılım, yarı log/sqrt maliyeti. (Robot akışıyla bit-birebir DEĞİL.)
typedef struct { rng_t r; double sig; long n; } jz_t;
// kesin kova sıralaması: kova = ⌊anahtar⌋ (uçlarda kırpılır; kırpılan kova da kendi içinde anahtarla sıralanır) → sonuç
// anahtarlara göre artan sıra (eşitlik olasılığı 0); ekleme sıralamasıyla aynı çıktı (kontrol/jz_sina.c).
#define JZ_KMIN (-120)
#define JZ_KN 360
static inline void jz_sirala(const double *key, int *idx){
  int head[JZ_KN], nxt[114]; for(int b=0;b<JZ_KN;b++) head[b]=-1;
  for(int i=113;i>=0;i--){ int b=(int)floor(key[i])-JZ_KMIN; if(b<0) b=0; if(b>=JZ_KN) b=JZ_KN-1; nxt[i]=head[b]; head[b]=i; }
  int m=0; for(int b=0;b<JZ_KN;b++){ int st=m; for(int e=head[b]; e>=0; e=nxt[e]){ int j=m-1; double ke=key[e]; while(j>=st && key[idx[j]]>ke){ idx[j+1]=idx[j]; j--; } idx[j+1]=e; m++; } } }
static void jz_init(jz_t *Z, double sig, uint64_t seed){ memset(Z,0,sizeof *Z); rseed(&Z->r,seed); Z->sig=sig; }
static inline int jz_kitap(jz_t *Z, int *restrict a){
  double key[114]; int idx[114]; rng_t r=Z->r;
  for(int i=0;i<114;i+=2){ double u1=(xnext(&r)>>11)*(1.0/9007199254740992.0), u2=(xnext(&r)>>11)*(1.0/9007199254740992.0); if(u1<1e-300) u1=1e-300;
    double rad=sqrt(-2.0*log(u1)), th=6.283185307179586*u2; key[i]=(i+1)+Z->sig*rad*cos(th); key[i+1]=(i+2)+Z->sig*rad*sin(th); idx[i]=i; idx[i+1]=i+1; }
  jz_sirala(key,idx);
  int kF=0, cs=0; for(int k=0;k<114;k++){ int v=BASE[idx[k]]; a[k]=v; int c=COMPC[k+1][v]; if(c){ kF++; cs+=c; } }
  Z->r=r; Z->n++; return (kF==12 && cs==76); }
#endif
