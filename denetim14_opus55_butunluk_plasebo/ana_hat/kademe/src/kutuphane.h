// kutuphane.h — plasebo12: yüklem kütüphanesi L* (ONKAYIT §2) ve sıfır örnekleyicileri.
// Tanımların başvurusu src/kutuphane.py'dir; bu dosya onunla bit-bit sınanır (kontrol/c_py_esit.*).
// Örnekleyiciler, RNG, mercekler, DIVM/INV32 ve kosu4/kosu_q: ~/opus5.5/butunluk/src/butunluk.h'den AYNEN kopyalandı
// (12. denetim; orada denetim10_opus55_kosullu/src/{ortak.h,hizli.h}'den kopyalanıp çapalarla doğrulanmıştı).
// Kütüphane 20 özgün yükleme sınırlandırılınca maskeler butunluk.h degerlendir() ile bit-bit aynıdır (kontrol/o0_maske.c).
#ifndef KUTUPHANE_H
#define KUTUPHANE_H
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

// ---------------- RNG (butunluk.h ile birebir) ----------------
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

// ---------------- mercekler (butunluk.h ile birebir) ----------------
#define NFORM 14
#define NPR 29
#define NLENS (NFORM*NPR)
static const int AL[NFORM]={1,1,1,1,1,1,2,2,2,2,3,3,3,3};
static const int BE[NFORM]={1,-1,2,-2,3,-3,1,-1,3,-3,1,-1,2,-2};
static const int PR[NPR]={3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113};
#define TMAX 1200
static uint32_t DIVM[TMAX+1];
static uint32_t INV32[29];

// ---------------- kütüphane L*: gruplar ve bitler (ONKAYIT §2.3) ----------------
// Gruplar: B kitap (mercek başına), Z aile boyu (koşulsuz), F aile-içi | k_F, W iki el ve birleşim | k_F, H karşı aile içi | k_H.
// Özgün 20 yüklem: B 0–1, Z 0, F 0–10 (12. denetimin 11 bitlik sırası), W 0–5 (12. denetimin 6 bitlik sırası).
enum { B_B1, B_B2, B_Ba, B_Bt, B_BMn, B_BMt, B_BMt2, NB_ };
enum { Z_QkF, Z_k2p, Z_kd, Z_kd2, NZ_ };
enum { F_F1, F_F2, F_R0c, F_R0e, F_R1c, F_R1e, F_K1, F_K2, F_Qc, F_QS, F_Qh,
       F_FC1, F_FC2, F_FD1, F_FD2, F_FSp, F_FS2p, F_FSd2, F_FHp, F_FH2p, F_FHm, F_FHd2,
       F_FCp, F_FC2p, F_FCm, F_FCd, F_FCd2, F_FHSp, F_FHS2p, F_FHSm, F_FHSd, F_FHSd2,
       F_FAp, F_FA2p, F_FAd, F_FAd2, F_FNp, F_FN2p, F_FNd, F_FNd2, F_FRK, F_FCK,
       F_FKp, F_FK2p, F_FKd, F_FKd2, F_FKSp, F_FKS2p, F_FKSm, F_FKSd, F_FKSd2, F_FAP, F_FKS, F_FSC, NF_ };
enum { W_W1, W_QU, W_QkH, W_Qa, W_Qn, W_QH,
       W_WI1, W_WEq, W_WSum, W_WU2p, W_WUd, W_WUd2, W_WH2p, W_WHd, W_WHd2, W_WIp, W_WI2p, W_WId, W_WId2,
       W_WUAp, W_WUA2p, W_WUAd2, W_WUNp, W_WUN2p, W_WUNd2, W_WUTp, W_WUT2p, W_WUTd, W_WUTd2,
       W_WHAp, W_WHA2p, W_WHAd, W_WHAd2, W_WHNp, W_WHN2p, W_WHNd, W_WHNd2, W_WHRp, W_WHR2p, W_WHRd2, NW_ };
enum { H_HH1, H_HH2, H_HR0c, H_HR0e, H_HR1c, H_HR1e, H_HK2, NH_ };
#define NGRUP 5
enum { G_B, G_Z, G_F, G_W, G_H };
static const int GBOY[NGRUP]={NB_,NZ_,NF_,NW_,NH_};
static const int GOFS[NGRUP]={0,NB_,NB_+NZ_,NB_+NZ_+NF_,NB_+NZ_+NF_+NW_};
#define NPRED (NB_+NZ_+NF_+NW_+NH_)        // 112
static const char *PADI[NPRED]={
  "B1","B2","Ba","Bt","BMn","BMt","BMt2",
  "QkF","Zk2p","Zkd","Zkd2",
  "F1","F2","R0c","R0e","R1c","R1e","K1","K2","Qc","QS","Qh",
  "FC1","FC2","FD1","FD2","FSp","FS2p","FSd2","FHp","FH2p","FHm","FHd2",
  "FCp","FC2p","FCm","FCd","FCd2","FHSp","FHS2p","FHSm","FHSd","FHSd2",
  "FAp","FA2p","FAd","FAd2","FNp","FN2p","FNd","FNd2","FRK","FCK",
  "FKp","FK2p","FKd","FKd2","FKSp","FKS2p","FKSm","FKSd","FKSd2","FAP","FKS","FSC",
  "W1","QU","QkH","Qa","Qn","QH",
  "WI1","WEq","WSum","WU2p","WUd","WUd2","WH2p","WHd","WHd2","WIp","WI2p","WId","WId2",
  "WUAp","WUA2p","WUAd2","WUNp","WUN2p","WUNd2","WUTp","WUT2p","WUTd","WUTd2",
  "WHAp","WHA2p","WHAd","WHAd2","WHNp","WHN2p","WHNd","WHNd2","WHRp","WHR2p","WHRd2",
  "HH1","HH2","HR0c","HR0e","HR1c","HR1e","HK2"};
static inline int pgrup(int g){ for(int k=NGRUP-1;k>=0;k--) if(g>=GOFS[k]) return k; return 0; }

// ---------------- bir kitabın 406 mercekteki kütüphane profili ----------------
typedef struct { uint64_t F[NLENS], W[NLENS]; uint8_t B[NLENS], Z[NLENS], H[NLENS], kF[NLENS], kH[NLENS], mc[NLENS]; } prof_t;
static uint8_t  EXB[NLENS], EXZ[NLENS], EXH[NLENS];      // sabit (dışlanan) yüklem–mercek çiftleri (ONKAYIT §2.5)
static uint64_t EXF[NLENS], EXW[NLENS];

static void mercek_hazirla_k(void){
  for(int x=0;x<=TMAX;x++){ uint32_t m=0; for(int i=0;i<NPR;i++) if(x%PR[i]==0) m|=1u<<i; DIVM[x]=m; }
  for(int i=0;i<NPR;i++){ uint32_t p=(uint32_t)PR[i], y=1; for(int k=0;k<5;k++) y*=2u-p*y; INV32[i]=y;
    for(int t=-TMAX;t<=TMAX;t++) if(t%PR[i]==0 && (int32_t)((uint32_t)t*y)!=t/PR[i]){ fprintf(stderr,"ters hatası\n"); exit(3); } }
}

// Analitik dışlama listesi (ONKAYIT §2.5). Özgün yüklemler için 12. denetimin listesi aynen; kardeşler için sabit-doğru ve
// sabit-yanlış çiftler. Hepsi kodeks 129 içindir.
static void dislama_hazirla(void){
  const long SN=SN_TOT, SA=SA_TOT;
  for(int f=0;f<NFORM;f++) for(int i=0;i<NPR;i++){ const int L=f*NPR+i, p=PR[i], al=AL[f], be=BE[f], abe=abs(be);
    uint8_t xb=0, xz=0, xh=0; uint64_t xf=0, xw=0;
    const long ST=(long)al*SN+(long)be*SA;                    // Σt (her dizilişte aynı)
    // --- kitap ---
    if(al%2==0 || be%2==0) xb|=1u<<B_B1;                      // [12. den.] D1 yalnız değere ya da yalnız konuma bağlı
    if(al%2==0) xb|=1u<<B_Ba;                                  // α çift: E⁰ = {a çift} ⇒ Σ_{E⁰} a sabit
    if(ST%2!=0) xb|=1u<<B_Bt;                                  // Σt tek ⇒ eşit t-toplamı imkânsız
    if(be%2==0 || SN%p!=0) xb|=1u<<B_BMn;                      // β çift: E⁰ = {n çift} sabit; p ∤ Σn ⇒ imkânsız
    if(ST%p!=0) xb|=1u<<B_BMt;
    if(ST%((long)p*p)!=0) xb|=1u<<B_BMt2;
    // --- p = 3, |β| = 3: F = H = {3 | n} (konumla sabit; k = 38, Σn = 2223 = 3²·13·19) ---
    if(abe%p==0){
      xw|=(1ull<<W_QU)|(1ull<<W_QkH)|(1ull<<W_Qn)|(1ull<<W_W1); xz|=1u<<Z_QkF;      // [12. den.]
      xf|=1ull<<F_FC1;                                                            // sütun sayıları 19/19 (tarama bayrağı)
      xz|=(1u<<Z_k2p)|(1u<<Z_kd)|(1u<<Z_kd2);
      xw|=(1ull<<W_WI1)|(1ull<<W_WEq)|(1ull<<W_WSum)|(1ull<<W_WU2p)|(1ull<<W_WUd)|(1ull<<W_WUd2)|(1ull<<W_WH2p)|(1ull<<W_WHd)|(1ull<<W_WHd2)
         |(1ull<<W_WIp)|(1ull<<W_WI2p)|(1ull<<W_WId)|(1ull<<W_WId2)|(1ull<<W_WUNp)|(1ull<<W_WUN2p)|(1ull<<W_WUNd2)
         |(1ull<<W_WHNp)|(1ull<<W_WHN2p)|(1ull<<W_WHNd)|(1ull<<W_WHNd2)|(1ull<<W_WUTd);
      xf|=(1ull<<F_FNp)|(1ull<<F_FN2p)|(1ull<<F_FNd)|(1ull<<F_FNd2)
         |(1ull<<F_FHSp)|(1ull<<F_FHS2p)|(1ull<<F_FHSm)|(1ull<<F_FHSd)|(1ull<<F_FHSd2)
         |(1ull<<F_FKSp)|(1ull<<F_FKS2p)|(1ull<<F_FKSm)|(1ull<<F_FKSd)|(1ull<<F_FKSd2); }
    // --- p = 3, α = 3: F = H = {3 | a} (değerle sabit; k = 39 = 3·13, Σa = 2193 = 3·17·43) ---
    if(al%p==0){
      xw|=(1ull<<W_QU)|(1ull<<W_QkH)|(1ull<<W_Qa)|(1ull<<W_W1); xz|=1u<<Z_QkF;      // [12. den.]
      xz|=(1u<<Z_k2p)|(1u<<Z_kd)|(1u<<Z_kd2);
      xw|=(1ull<<W_WI1)|(1ull<<W_WEq)|(1ull<<W_WSum)|(1ull<<W_WU2p)|(1ull<<W_WUd)|(1ull<<W_WUd2)|(1ull<<W_WH2p)|(1ull<<W_WHd)|(1ull<<W_WHd2)
         |(1ull<<W_WIp)|(1ull<<W_WI2p)|(1ull<<W_WId)|(1ull<<W_WId2)|(1ull<<W_WUAp)|(1ull<<W_WUA2p)|(1ull<<W_WUAd2)
         |(1ull<<W_WHAp)|(1ull<<W_WHA2p)|(1ull<<W_WHAd)|(1ull<<W_WHAd2)|(1ull<<W_WUTd);
      xf|=(1ull<<F_FAp)|(1ull<<F_FA2p)|(1ull<<F_FAd)|(1ull<<F_FAd2)
         |(1ull<<F_FHSp)|(1ull<<F_FHS2p)|(1ull<<F_FHSm)|(1ull<<F_FHSd2)
         |(1ull<<F_FKSp)|(1ull<<F_FKS2p)|(1ull<<F_FKSm)|(1ull<<F_FKSd2); }
    // --- β çift (β = ±2, α tek): D1 = D2 = n-paritesi ⇒ F⁰¹ = F¹⁰ = ∅ (ve H'de aynısı) ---
    if(be%2==0){
      xf|=(1ull<<F_FD1)|(1ull<<F_FD2)|(1ull<<F_FKp)|(1ull<<F_FK2p)|(1ull<<F_FKd)|(1ull<<F_FKd2)|(1ull<<F_FKSp)|(1ull<<F_FKS2p)
         |(1ull<<F_FKSm)|(1ull<<F_FKSd)|(1ull<<F_FKSd2)|(1ull<<F_FAP)|(1ull<<F_FKS);
      xh|=(1u<<H_HR0c)|(1u<<H_HR0e)|(1u<<H_HR1c)|(1u<<H_HR1e); }
    // --- her mercekte imkânsız (L* dışı; ONKAYIT §2.2): Bt (α tek: Σt tek; α = 2: 4·Σ_{E⁰}n ≡ 2 mod 4), FHp ve FKp
    //     (t-çift satırda q = t/p çift, p tek ⇒ satır-0 yarı toplamı ve hücre toplamı p olamaz) ---
    xb|=1u<<B_Bt; xf|=(1ull<<F_FHp)|(1ull<<F_FKp);
    EXB[L]=xb; EXZ[L]=xz; EXF[L]=xf; EXW[L]=xw; EXH[L]=xh; } }

static inline int kosu4(long a, long b, long c, long d){ long v[4]={a,b,c,d};
  for(int i=1;i<4;i++){ long x=v[i]; int j=i-1; while(j>=0&&v[j]>x){ v[j+1]=v[j]; j--; } v[j+1]=x; }
  int nd=1; for(int i=1;i<4;i++) if(v[i]!=v[i-1]) nd++; return nd>=2 && (v[3]-v[0]+1)==nd; }
static inline int ardisik_dizi4(long a, long b, long c, long d){ long v[4]={a,b,c,d};   // sıralı toplamlar ortak farkı ≥ 1 olan aritmetik dizi
  for(int i=1;i<4;i++){ long x=v[i]; int j=i-1; while(j>=0&&v[j]>x){ v[j+1]=v[j]; j--; } v[j+1]=x; }
  long dd=v[1]-v[0]; return dd>=1 && v[2]-v[1]==dd && v[3]-v[2]==dd; }
static inline int bant(const int *qv, int k, int qmin, int qmax){ // farklı değerler kesintisiz, ≥ 2 farklı (butunluk.h kosu_q ile aynı)
  if(k<2) return 0; long span=(long)qmax-qmin+1; if(span<2 || span>k) return 0;
  uint64_t w[2]={0,0}; for(int i=0;i<k;i++){ int d=qv[i]-qmin; w[d>>6]|=1ULL<<(d&63); }
  return (__builtin_popcountll(w[0])+__builtin_popcountll(w[1]))==span; }

typedef struct { int k, c[4]; long s[4]; int qmin,qmax; uint64_t b0,b1; long sa, sn; int qf[4]; uint8_t eq[4]; int qv[114]; } kfam_t;
typedef struct { int k, c[4]; long r[4]; int rmin,rmax; uint64_t b0,b1; long sa, sn, st, sr; int rv[114]; } khfam_t;

// fbit: hangi biçimler değerlendirilecek (bit f); referans geçişi biçimleri iş parçacıklarına böler.
static void kdegerlendir(const int *a, prof_t *P, uint32_t fbit){
  static __thread kfam_t FF[NPR]; static __thread khfam_t HH[NPR];
  for(int f=0;f<NFORM;f++){ if(!(fbit>>f&1)) continue;
    const int al=AL[f], be=BE[f], abe=abs(be);
    for(int i=0;i<NPR;i++){ kfam_t *F=&FF[i]; F->k=0; for(int c=0;c<4;c++){ F->c[c]=0; F->s[c]=0; F->eq[c]=1; F->qf[c]=0; }
      F->qmin=1<<30; F->qmax=-(1<<30); F->b0=F->b1=0; F->sa=F->sn=0;
      khfam_t *H=&HH[i]; H->k=0; for(int c=0;c<4;c++){ H->c[c]=0; H->r[c]=0; } H->rmin=1<<30; H->rmax=-(1<<30); H->b0=H->b1=0; H->sa=H->sn=H->st=H->sr=0; }
    int e0=0; long sn0=0, sa0=0, st0=0, st=0;
    for(int n=1;n<=114;n++){ const int x=a[n-1]; const int t=al*n+be*x, u=al*n-be*x;
      const int d1=t&1, d2=n&1; st+=t; if(!d1){ e0++; sn0+=n; sa0+=x; st0+=t; }
      const int ci=d1*2+d2;
      uint32_t mt=DIVM[t<0?-t:t];
      while(mt){ int i=__builtin_ctz(mt); mt&=mt-1; kfam_t *F=&FF[i]; const int q=(int32_t)((uint32_t)t*INV32[i]);
        if(F->c[ci]==0) F->qf[ci]=q; else if(q!=F->qf[ci]) F->eq[ci]=0;
        F->c[ci]++; F->s[ci]+=q; F->qv[F->k++]=q; if(q<F->qmin) F->qmin=q; if(q>F->qmax) F->qmax=q; F->sa+=x; F->sn+=n;
        if(n<=64) F->b0|=1ULL<<(n-1); else F->b1|=1ULL<<(n-65); }
      uint32_t mu=DIVM[u<0?-u:u];
      while(mu){ int i=__builtin_ctz(mu); mu&=mu-1; khfam_t *H=&HH[i]; const int r=(int32_t)((uint32_t)u*INV32[i]);
        H->c[ci]++; H->r[ci]+=r; H->rv[H->k++]=r; if(r<H->rmin) H->rmin=r; if(r>H->rmax) H->rmax=r; H->sa+=x; H->sn+=n; H->st+=t; H->sr+=r;
        if(n<=64) H->b0|=1ULL<<(n-1); else H->b1|=1ULL<<(n-65); } }
    // kitap düzeyi
    const int e1=114-e0; const long sn1=SN_TOT-sn0, sa1=SA_TOT-sa0, st1=st-st0;
    uint8_t mb0=0;
    if(e0==e1) mb0|=1u<<B_B1;
    if(e0>0 && e1>0 && ((long)al*sn0==(long)abe*sa1 || (long)al*sn1==(long)abe*sa0 || st0==st1)) mb0|=1u<<B_B2;
    if(e0>0 && e1>0 && sa0==sa1) mb0|=1u<<B_Ba;
    if(e0>0 && e1>0 && st0==st1) mb0|=1u<<B_Bt;
    for(int i=0;i<NPR;i++){ const int p=PR[i]; const long pp=(long)p*p; const int L=f*NPR+i; kfam_t *F=&FF[i]; khfam_t *H=&HH[i];
      uint8_t mb=mb0;
      if(e0>0 && e1>0){ if(sn0%p==0 && sn1%p==0) mb|=1u<<B_BMn; if(st0%p==0 && st1%p==0) mb|=1u<<B_BMt; if(st0%pp==0 && st1%pp==0) mb|=1u<<B_BMt2; }
      const int c0=F->c[0],c1=F->c[1],c2=F->c[2],c3=F->c[3]; const long s0=F->s[0],s1=F->s[1],s2=F->s[2],s3=F->s[3];
      const int h0=c0+c1, h1=c2+c3, k=F->k; const long q0=s0+s1, q1=s2+s3, sq=q0+q1; const int dolu=(c0&&c1&&c2&&c3);
      const int k0=c0+c2, k1=c1+c3; const long cs0=s0+s2, cs1=s1+s3;          // sütunlar (n çift / tek)
      const int g0=c0+c3, g1=c1+c2; const long ds0=s0+s3, ds1=s1+s2;          // köşegenler (00∪11 / 01∪10)
      uint64_t m=0;
      if(h0==h1 && h0>=1) m|=1ull<<F_F1;
      if(h0>=1 && h1>=1 && q0==q1) m|=1ull<<F_F2;
      if(c0==c1 && c0>=1) m|=1ull<<F_R0c;
      if(c0>=1 && c1>=1 && s0==s1) m|=1ull<<F_R0e;
      if(c2==c3 && c2>=1) m|=1ull<<F_R1c;
      if(c2>=1 && c3>=1 && s2==s3) m|=1ull<<F_R1e;
      if(dolu && kosu4(s0,s1,s2,s3)) m|=1ull<<F_K1;
      if(bant(F->qv,k,F->qmin,F->qmax)) m|=1ull<<F_K2;
      if(dolu && sq==4L*p) m|=1ull<<F_Qc;
      if(k>=1 && sq%p==0) m|=1ull<<F_QS;
      if(h0>=1 && h1>=1 && q0%p==0 && q1%p==0) m|=1ull<<F_Qh;
      if(k0==k1 && k0>=1) m|=1ull<<F_FC1;
      if(k0>=1 && k1>=1 && cs0==cs1) m|=1ull<<F_FC2;
      if(g0==g1 && g0>=1) m|=1ull<<F_FD1;
      if(g0>=1 && g1>=1 && ds0==ds1) m|=1ull<<F_FD2;
      if(k>=1){ if(sq==p) m|=1ull<<F_FSp; if(sq==2L*p) m|=1ull<<F_FS2p; if(sq%pp==0) m|=1ull<<F_FSd2;
        if(F->sa==p) m|=1ull<<F_FAp; if(F->sa==2L*p) m|=1ull<<F_FA2p; if(F->sa%p==0) m|=1ull<<F_FAd; if(F->sa%pp==0) m|=1ull<<F_FAd2;
        if(F->sn==p) m|=1ull<<F_FNp; if(F->sn==2L*p) m|=1ull<<F_FN2p; if(F->sn%p==0) m|=1ull<<F_FNd; if(F->sn%pp==0) m|=1ull<<F_FNd2; }
      if(h0>=1 && h1>=1){
        if(q0==p && q1==p) m|=1ull<<F_FHp; if(q0==2L*p && q1==2L*p) m|=1ull<<F_FH2p; if(sq==2L*p) m|=1ull<<F_FHm;
        if(q0%pp==0 && q1%pp==0) m|=1ull<<F_FHd2;
        if(h0==p && h1==p) m|=1ull<<F_FHSp; if(h0==2*p && h1==2*p) m|=1ull<<F_FHS2p; if(h0+h1==2*p) m|=1ull<<F_FHSm;
        if(h0%p==0 && h1%p==0) m|=1ull<<F_FHSd; if(h0%pp==0 && h1%pp==0) m|=1ull<<F_FHSd2;
        if(labs(q0-q1)==1) m|=1ull<<F_FRK; }
      if(k0>=1 && k1>=1){
        if(cs0==p && cs1==p) m|=1ull<<F_FCp; if(cs0==2L*p && cs1==2L*p) m|=1ull<<F_FC2p; if(cs0+cs1==2L*p) m|=1ull<<F_FCm;
        if(cs0%p==0 && cs1%p==0) m|=1ull<<F_FCd; if(cs0%pp==0 && cs1%pp==0) m|=1ull<<F_FCd2;
        if(labs(cs0-cs1)==1) m|=1ull<<F_FCK; }
      if(dolu){
        if(s0==p && s1==p && s2==p && s3==p) m|=1ull<<F_FKp; if(s0==2L*p && s1==2L*p && s2==2L*p && s3==2L*p) m|=1ull<<F_FK2p;
        if(s0%p==0 && s1%p==0 && s2%p==0 && s3%p==0) m|=1ull<<F_FKd; if(s0%pp==0 && s1%pp==0 && s2%pp==0 && s3%pp==0) m|=1ull<<F_FKd2;
        if(c0==p && c1==p && c2==p && c3==p) m|=1ull<<F_FKSp; if(c0==2*p && c1==2*p && c2==2*p && c3==2*p) m|=1ull<<F_FKS2p;
        if(c0+c1+c2+c3==4*p) m|=1ull<<F_FKSm;
        if(c0%p==0 && c1%p==0 && c2%p==0 && c3%p==0) m|=1ull<<F_FKSd; if(c0%pp==0 && c1%pp==0 && c2%pp==0 && c3%pp==0) m|=1ull<<F_FKSd2;
        if(ardisik_dizi4(s0,s1,s2,s3)) m|=1ull<<F_FAP;
        if(kosu4(c0,c1,c2,c3)) m|=1ull<<F_FKS; }
      if((c0>=2&&F->eq[0]) || (c1>=2&&F->eq[1]) || (c2>=2&&F->eq[2]) || (c3>=2&&F->eq[3])) m|=1ull<<F_FSC;
      uint8_t mz=0;
      if(k==p) mz|=1u<<Z_QkF; if(k==2*p) mz|=1u<<Z_k2p; if(k>=1 && k%p==0) mz|=1u<<Z_kd; if(k>=1 && k%pp==0) mz|=1u<<Z_kd2;
      // iki el, birleşim
      const uint64_t i0=F->b0&H->b0, i1=F->b1&H->b1; const int ni=__builtin_popcountll(i0)+__builtin_popcountll(i1);
      const int kH=H->k, kU=k+kH-ni;
      long saU=F->sa+H->sa, snU=F->sn+H->sn, stU=(long)p*sq+H->st;
      if(ni){ uint64_t w=i0; while(w){ int b=__builtin_ctzll(w); w&=w-1; const int nn=b+1, x=a[b]; saU-=x; snU-=nn; stU-=al*nn+be*x; }
              w=i1; while(w){ int b=__builtin_ctzll(w); w&=w-1; const int nn=b+65, x=a[b+64]; saU-=x; snU-=nn; stU-=al*nn+be*x; } }
      uint64_t mw=0;
      if(k>=1 && kH>=1 && ni==0) mw|=1ull<<W_W1;
      if(kU==p) mw|=1ull<<W_QU;
      if(kH==p) mw|=1ull<<W_QkH;
      if(kU>=1 && saU%p==0) mw|=1ull<<W_Qa;
      if(kU>=1 && snU%p==0) mw|=1ull<<W_Qn;
      if(kH>=1 && H->sr%p==0) mw|=1ull<<W_QH;
      if(ni==1) mw|=1ull<<W_WI1;
      if(k>=1 && kH>=1 && k==kH) mw|=1ull<<W_WEq;
      if(k>=1 && kH>=1 && k+kH==p) mw|=1ull<<W_WSum;
      if(kU==2*p) mw|=1ull<<W_WU2p; if(kU>=1 && kU%p==0) mw|=1ull<<W_WUd; if(kU>=1 && kU%pp==0) mw|=1ull<<W_WUd2;
      if(kH==2*p) mw|=1ull<<W_WH2p; if(kH>=1 && kH%p==0) mw|=1ull<<W_WHd; if(kH>=1 && kH%pp==0) mw|=1ull<<W_WHd2;
      if(ni==p) mw|=1ull<<W_WIp; if(ni==2*p) mw|=1ull<<W_WI2p; if(ni>=1 && ni%p==0) mw|=1ull<<W_WId; if(ni>=1 && ni%pp==0) mw|=1ull<<W_WId2;
      if(kU>=1){ if(saU==p) mw|=1ull<<W_WUAp; if(saU==2L*p) mw|=1ull<<W_WUA2p; if(saU%pp==0) mw|=1ull<<W_WUAd2;
        if(snU==p) mw|=1ull<<W_WUNp; if(snU==2L*p) mw|=1ull<<W_WUN2p; if(snU%pp==0) mw|=1ull<<W_WUNd2;
        if(stU==p) mw|=1ull<<W_WUTp; if(stU==2L*p) mw|=1ull<<W_WUT2p; if(stU%p==0) mw|=1ull<<W_WUTd; if(stU%pp==0) mw|=1ull<<W_WUTd2; }
      if(kH>=1){ if(H->sa==p) mw|=1ull<<W_WHAp; if(H->sa==2L*p) mw|=1ull<<W_WHA2p; if(H->sa%p==0) mw|=1ull<<W_WHAd; if(H->sa%pp==0) mw|=1ull<<W_WHAd2;
        if(H->sn==p) mw|=1ull<<W_WHNp; if(H->sn==2L*p) mw|=1ull<<W_WHN2p; if(H->sn%p==0) mw|=1ull<<W_WHNd; if(H->sn%pp==0) mw|=1ull<<W_WHNd2;
        if(H->sr==p) mw|=1ull<<W_WHRp; if(H->sr==2L*p) mw|=1ull<<W_WHR2p; if(H->sr%pp==0) mw|=1ull<<W_WHRd2; }
      // karşı aile içi
      const int hc0=H->c[0],hc1=H->c[1],hc2=H->c[2],hc3=H->c[3]; const long r0=H->r[0],r1=H->r[1],r2=H->r[2],r3=H->r[3];
      const int hh0=hc0+hc1, hh1=hc2+hc3; uint8_t mh=0;
      if(hh0==hh1 && hh0>=1) mh|=1u<<H_HH1;
      if(hh0>=1 && hh1>=1 && r0+r1==r2+r3) mh|=1u<<H_HH2;
      if(hc0==hc1 && hc0>=1) mh|=1u<<H_HR0c;
      if(hc0>=1 && hc1>=1 && r0==r1) mh|=1u<<H_HR0e;
      if(hc2==hc3 && hc2>=1) mh|=1u<<H_HR1c;
      if(hc2>=1 && hc3>=1 && r2==r3) mh|=1u<<H_HR1e;
      if(bant(H->rv,kH,H->rmin,H->rmax)) mh|=1u<<H_HK2;
      P->B[L]=mb&~EXB[L]; P->Z[L]=mz&~EXZ[L]; P->F[L]=m&~EXF[L]; P->W[L]=mw&~EXW[L]; P->H[L]=mh&~EXH[L];
      P->kF[L]=(uint8_t)k; P->kH[L]=(uint8_t)kH;
      int mc=c0; if(c1<mc) mc=c1; if(c2<mc) mc=c2; if(c3<mc) mc=c3; P->mc[L]=(uint8_t)mc; }
  }
}
static inline int kapiG(const prof_t *P, int L){ return (int)P->kF[L]*PR[L%NPR]>=228 && P->mc[L]>=2; }

// ---------------- sıfır örnekleyicileri (butunluk.h ile birebir; J kullanılmaz) ----------------
#define NCH 64
typedef struct { char null; rng_t r; int a[114];
  int ch[NCH][114]; long chS[NCH]; int cur;
  double q; int ref[114]; uint32_t thr[115][114]; uint8_t ali[115][114];
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
  else { fprintf(stderr,"sıfır?\n"); exit(1); }
}

// ---------------- tohumlar (ONKAYIT §6) ----------------
// tohum = 29102600000 + sıfır·10^8 + tür·10^7 + sıra; sıfır U=1, M=2, L=3; tür: 1 referans bölütü, 2 kontrol bölütü, 3 hedef,
// 4 kod doğrulama, 5 sabit-yüklem taraması, 6 hız/bellek ölçümü, 7 örnekleyici eşitliği, 8 mekanik boru hattı sınaması.
static inline int sifir_no(char s){ return s=='U'?1:(s=='M'?2:(s=='L'?3:9)); }
static inline uint64_t tohum(char s, int tur, long sira){ return 29102600000ULL + (uint64_t)sifir_no(s)*100000000ULL + (uint64_t)tur*10000000ULL + (uint64_t)sira; }

static void kutuphane_init(void){ kodeks_sec(129); mercek_hazirla_k(); dislama_hazirla(); }
#endif
