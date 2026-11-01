// kart_ortak.h — kontrol_b (kol 1–3) ve kontrol_h sayaçlarının ekran kartı için yeniden düzenlenmiş çekirdeği.
// Aynı kod hem CUDA aygıtında (KD = __host__ __device__) hem düz C'de (kart_emul.c: Mac'te eşdeğerlik sınaması) derlenir.
// Başvuru: kapi/kaynak/src/{kutuphane.h, dtab.h, dtab_b.h, kontrol_b.c, kontrol_h.c} (değiştirilmeden). Buradaki her işlev onlardaki
// karşılığının aynı sonucu veren yeniden yazımıdır:
//   - RNG, tohum, örnekleyiciler (M, L, U): kutuphane.h ile aynı tamsayı işlemleri, aynı sıra.
//   - profil: kdegerlendir'in mercek (biçim f, asal i) başına ayrılmış hâli. Aile istatistikleri int32 (en büyük |değer| 136.800);
//     bant (K2, HK2) için q/r değerleri dizide tutulmaz, gerekirse ikinci geçişte 128 bitlik maskeye yazılır (aynı tanım).
//   - puan: puan_B'nin budamasız hâli (budama kesindir: değer üst sınırı aşamaz; en büyük değer ve en küçük mercek aynı).
#ifndef KART_ORTAK_H
#define KART_ORTAK_H
#include <stdint.h>
#ifdef __CUDACC__
#define KD __host__ __device__
#else
#define KD
#endif
#define K_NFORM 14
#define K_NPR 29
#define K_NLENS 406
#define K_KC 80
#define K_DMAX 64
#define K_NPRED 112
#define K_NCH 64
#define K_TMAX 1200
#ifndef K_DIVM
#define K_DIVM(C,x) ((C)->divm[x])
#endif

typedef struct {
  int ng[5]; int pos[5][K_DMAX]; uint64_t gm[5];
  int tmin, dmin, onsel, hed;
  double esik_k, esik_q;                 // DESIK − 1e-6 ve DESIK − 2.0 (ana programda kontrol_b ile aynı ifadeyle)
  uint64_t oTB, oTZ, oTF, oTW, oTH;      // havuz içi ofsetler (float)
} kdil_t;

typedef struct {                          // sabit tablolar (ana programda özgün başlıklardan doldurulur)
  char sif; int kip;                      // kip 0: kontrol_b kol 1–3; 1: kontrol_h (B2 kısayolu)
  int KE, KH;                             // kapı G
  int16_t base[114]; int16_t srt[114];   // BASE, büyükten küçüğe sıralı BASE (M ısınması)
  int64_t s_obs; int32_t sn_tot, sa_tot;
  uint32_t divm[K_TMAX+1]; uint32_t inv32[K_NPR];
  int al[K_NFORM], be[K_NFORM], pr[K_NPR];
  uint8_t exb[K_NLENS], exz[K_NLENS], exh[K_NLENS]; uint64_t exf[K_NLENS], exw[K_NLENS];
  int16_t cF[K_NLENS][K_KC], cH[K_NLENS][K_KC];
  int32_t ofs[K_NLENS+1], ofsH[K_NLENS+1];
  double lbedel[2][K_NLENS];
  uint8_t imza[K_NPRED];
  int gofs[5];
  uint32_t thr[115][114]; uint8_t ali[115][114]; int16_t ref[114];   // Mallows
} ksabit_t;

typedef struct { uint64_t s[4]; } krng_t;
typedef struct { krng_t r; int cur; int64_t chS[K_NCH]; int16_t ch[K_NCH][114]; int16_t a[114]; } korn_t;

typedef struct { uint64_t F[K_NLENS], W[K_NLENS]; uint8_t B[K_NLENS], Z[K_NLENS], H[K_NLENS], kF[K_NLENS], kH[K_NLENS], mc[K_NLENS]; } kprof_t;
typedef struct { int e0, sn0, sa0, st0, st; } kbk_t;

// kayıt (ana programda aday_t'ye çevrilir): tur 0 = .q (eşiğin 2 bit altına kadar), 1 = .s (kontrol_h: bütünlüklü bütün Kur'an kontrolleri)
typedef struct { int32_t bolut_ix; uint32_t sira; double puan; int16_t L; uint16_t dil; uint8_t tur; uint16_t a[114]; } kkayit_t;

// ---------------- RNG (kutuphane.h ile birebir) ----------------
KD static inline uint64_t k_rotl(uint64_t x, int k){ return (x << k) | (x >> (64 - k)); }
KD static inline uint64_t k_xnext(krng_t *r){ uint64_t *s=r->s; const uint64_t res=k_rotl(s[0]+s[3],23)+s[0]; const uint64_t t=s[1]<<17;
  s[2]^=s[0]; s[3]^=s[1]; s[1]^=s[2]; s[0]^=s[3]; s[2]^=t; s[3]=k_rotl(s[3],45); return res; }
KD static inline void k_rseed(krng_t *r, uint64_t seed){ uint64_t z=seed; for(int i=0;i<4;i++){ z+=0x9E3779B97F4A7C15ULL; uint64_t x=z;
  x=(x^(x>>30))*0xBF58476D1CE4E5B9ULL; x=(x^(x>>27))*0x94D049BB133111EBULL; r->s[i]=x^(x>>31);} }
KD static inline uint32_t k_bounded(krng_t *r, uint32_t n){
#ifdef __CUDA_ARCH__
  return (uint32_t)__umul64hi(k_xnext(r),(uint64_t)n);
#else
  return (uint32_t)(((unsigned __int128)k_xnext(r)*n)>>64);
#endif
}
KD static inline uint64_t k_tohum(char s, int tur, long sira){ const int no = s=='U'?1:(s=='M'?2:(s=='L'?3:9));
  return 29102600000ULL + (uint64_t)no*100000000ULL + (uint64_t)tur*10000000ULL + (uint64_t)sira; }

// ---------------- örnekleyiciler (kutuphane.h orn_init / orn_sonraki ile birebir) ----------------
KD static void k_orn_init(korn_t *Z, const ksabit_t *C, uint64_t seed){
  k_rseed(&Z->r,seed); Z->cur=0; for(int q=0;q<114;q++) Z->a[q]=C->base[q];
  if(C->sif=='M'){
    for(int c=0;c<K_NCH;c++){ int16_t *a=Z->ch[c]; int64_t S=0; for(int i=0;i<114;i++){ a[i]=C->srt[i]; S+=(int64_t)(i+1)*a[i]; }
      for(int b=0;b<30000;b++){ const int i=(int)k_bounded(&Z->r,114), j=(int)k_bounded(&Z->r,114); const int64_t dS=(int64_t)((i+1)-(j+1))*(a[j]-a[i]);
        if(S+dS<=C->s_obs){ const int16_t t=a[i]; a[i]=a[j]; a[j]=t; S+=dS; } }
      Z->chS[c]=S; } } }
KD static inline void k_orn_sonraki(korn_t *Z, const ksabit_t *C, int16_t *out){
  if(C->sif=='U'){ int16_t *a=Z->a; krng_t r=Z->r; for(int i=113;i>0;i--){ const int j=(int)k_bounded(&r,(uint32_t)(i+1)); const int16_t t=a[i]; a[i]=a[j]; a[j]=t; }
    Z->r=r; for(int q=0;q<114;q++) out[q]=a[q]; }
  else if(C->sif=='M'){ const int c=Z->cur; Z->cur=(Z->cur+1)%K_NCH; int16_t *b=Z->ch[c]; int64_t S=Z->chS[c]; krng_t r=Z->r;
    for(int st=0;st<20;st++){ const int i=(int)k_bounded(&r,114), j=(int)k_bounded(&r,114); const int64_t dS=(int64_t)((i+1)-(j+1))*(b[j]-b[i]);
      if(S+dS<=C->s_obs){ const int16_t t=b[i]; b[i]=b[j]; b[j]=t; S+=dS; } }
    Z->r=r; Z->chS[c]=S; for(int q=0;q<114;q++) out[q]=b[q]; }
  else { int16_t rem[114]; for(int i=0;i<114;i++) rem[i]=C->ref[113-i]; krng_t r=Z->r;           // 'L'
    for(int p=0;p<114;p++){ const int R=114-p; const uint64_t x=k_xnext(&r); const int j=(int)(((x>>32)*(uint64_t)R)>>32); const uint32_t u=(uint32_t)x;
      const uint32_t th=C->thr[R][j]; const int V=(u<th||th==0xFFFFFFFFu)?j:C->ali[R][j]; const int ix=R-1-V; out[p]=rem[ix];
      for(int k=ix;k<R-1;k++) rem[k]=rem[k+1]; }                    // özgündeki memmove + döngü: rem[ix]'i çıkarır (R−1 ötesi bir daha okunmaz)
    Z->r=r; } }

// ---------------- kitap düzeyi (biçim başına) ----------------
KD static inline void k_kitap(const int16_t *a, int al, int be, kbk_t *bk){ int e0=0, sn0=0, sa0=0, st0=0, st=0;
  for(int n=1;n<=114;n++){ const int x=a[n-1]; const int t=al*n+be*x; st+=t; if(!(t&1)){ e0++; sn0+=n; sa0+=x; st0+=t; } }
  bk->e0=e0; bk->sn0=sn0; bk->sa0=sa0; bk->st0=st0; bk->st=st; }
KD static inline uint8_t k_mb0(const kbk_t *bk, int al, int be, const ksabit_t *C){ const int abe=be<0?-be:be;
  const int e0=bk->e0, e1=114-bk->e0, sn0=bk->sn0, sn1=C->sn_tot-bk->sn0, sa0=bk->sa0, sa1=C->sa_tot-bk->sa0, st0=bk->st0, st1=bk->st-bk->st0;
  uint8_t m=0; if(e0==e1) m|=1u<<0;                                                                   // B1
  if(e0>0 && e1>0 && (al*sn0==abe*sa1 || al*sn1==abe*sa0 || st0==st1)) m|=1u<<1;                      // B2
  if(e0>0 && e1>0 && sa0==sa1) m|=1u<<2;                                                             // Ba
  if(e0>0 && e1>0 && st0==st1) m|=1u<<3;                                                             // Bt
  return m; }

KD static inline int k_pc64(uint64_t x){
#ifdef __CUDA_ARCH__
  return __popcll(x);
#else
  return __builtin_popcountll(x);
#endif
}
KD static inline int k_ctz64(uint64_t x){
#ifdef __CUDA_ARCH__
  return __ffsll((long long)x)-1;
#else
  return __builtin_ctzll(x);
#endif
}
KD static inline int k_kosu4(int a, int b, int c, int d){ int v[4]={a,b,c,d};
  for(int i=1;i<4;i++){ int x=v[i]; int j=i-1; while(j>=0&&v[j]>x){ v[j+1]=v[j]; j--; } v[j+1]=x; }
  int nd=1; for(int i=1;i<4;i++) if(v[i]!=v[i-1]) nd++; return nd>=2 && (v[3]-v[0]+1)==nd; }
KD static inline int k_ardisik4(int a, int b, int c, int d){ int v[4]={a,b,c,d};
  for(int i=1;i<4;i++){ int x=v[i]; int j=i-1; while(j>=0&&v[j]>x){ v[j+1]=v[j]; j--; } v[j+1]=x; }
  int dd=v[1]-v[0]; return dd>=1 && v[2]-v[1]==dd && v[3]-v[2]==dd; }

// ---------------- bir mercek (biçim f, asal i): aile, karşı aile, iki el, maskeler ----------------
// Bit numaraları kutuphane.h'deki enum sırasıdır (B 0–6, Z 0–3, F 0–53, W 0–39, H 0–6).
KD static void k_mercek(const int16_t *a, int f, int i, const kbk_t *bk, uint8_t mb0, const ksabit_t *C, kprof_t *P){
  const int al=C->al[f], be=C->be[f]; const int p=C->pr[i], pp=p*p; const int L=f*K_NPR+i; const uint32_t bit=1u<<i, inv=C->inv32[i];
  int c0=0,c1=0,c2=0,c3=0, s0=0,s1=0,s2=0,s3=0, f0=0,f1=0,f2=0,f3=0, q0e=1,q1e=1,q2e=1,q3e=1;
  int k=0, qmin=1<<30, qmax=-(1<<30), sa=0, sn=0; uint64_t b0=0, b1=0;
  int hc0=0,hc1=0,hc2=0,hc3=0, r0=0,r1=0,r2=0,r3=0; int kH=0, rmin=1<<30, rmax=-(1<<30), hsa=0, hsn=0, hst=0, hsr=0; uint64_t hb0=0, hb1=0;
  for(int n=1;n<=114;n++){ const int x=a[n-1]; const int t=al*n+be*x, u=al*n-be*x; const int ci=(t&1)*2+(n&1);
    if(K_DIVM(C,t<0?-t:t)&bit){ const int q=(int32_t)((uint32_t)t*inv);
      if(ci==0){ if(c0==0) f0=q; else if(q!=f0) q0e=0; c0++; s0+=q; }
      else if(ci==1){ if(c1==0) f1=q; else if(q!=f1) q1e=0; c1++; s1+=q; }
      else if(ci==2){ if(c2==0) f2=q; else if(q!=f2) q2e=0; c2++; s2+=q; }
      else { if(c3==0) f3=q; else if(q!=f3) q3e=0; c3++; s3+=q; }
      k++; if(q<qmin) qmin=q; if(q>qmax) qmax=q; sa+=x; sn+=n; if(n<=64) b0|=1ULL<<(n-1); else b1|=1ULL<<(n-65); }
    if(K_DIVM(C,u<0?-u:u)&bit){ const int r=(int32_t)((uint32_t)u*inv);
      if(ci==0){ hc0++; r0+=r; } else if(ci==1){ hc1++; r1+=r; } else if(ci==2){ hc2++; r2+=r; } else { hc3++; r3+=r; }
      kH++; if(r<rmin) rmin=r; if(r>rmax) rmax=r; hsa+=x; hsn+=n; hst+=t; hsr+=r; if(n<=64) hb0|=1ULL<<(n-1); else hb1|=1ULL<<(n-65); } }
  // bant (butunluk.h kosu_q): k ≥ 2, 2 ≤ açıklık ≤ k ve farklı değerlerin sayısı = açıklık
  int K2=0, HK2=0;
  { const int spF=qmax-qmin+1, spH=rmax-rmin+1; const int nF=(k>=2 && spF>=2 && spF<=k), nH=(kH>=2 && spH>=2 && spH<=kH);
    if(nF||nH){ uint64_t w0=0,w1=0,v0=0,v1=0;
      for(int n=1;n<=114;n++){ const int x=a[n-1]; const int t=al*n+be*x, u=al*n-be*x;
        if(nF && (K_DIVM(C,t<0?-t:t)&bit)){ const int d=(int32_t)((uint32_t)t*inv)-qmin; if(d<64) w0|=1ULL<<d; else w1|=1ULL<<(d-64); }
        if(nH && (K_DIVM(C,u<0?-u:u)&bit)){ const int d=(int32_t)((uint32_t)u*inv)-rmin; if(d<64) v0|=1ULL<<d; else v1|=1ULL<<(d-64); } }
      if(nF) K2=(k_pc64(w0)+k_pc64(w1))==spF; if(nH) HK2=(k_pc64(v0)+k_pc64(v1))==spH; } }
  // kitap (mercek)
  const int e0=bk->e0, e1=114-bk->e0, sn0=bk->sn0, sn1=C->sn_tot-bk->sn0, st0=bk->st0, st1=bk->st-bk->st0;
  uint8_t mb=mb0;
  if(e0>0 && e1>0){ if(sn0%p==0 && sn1%p==0) mb|=1u<<4; if(st0%p==0 && st1%p==0) mb|=1u<<5; if(st0%pp==0 && st1%pp==0) mb|=1u<<6; }
  // aile içi
  const int h0=c0+c1, h1=c2+c3; const int q0=s0+s1, q1=s2+s3, sq=q0+q1; const int dolu=(c0&&c1&&c2&&c3);
  const int k0=c0+c2, k1=c1+c3; const int cs0=s0+s2, cs1=s1+s3; const int g0=c0+c3, g1=c1+c2; const int ds0=s0+s3, ds1=s1+s2;
  uint64_t m=0;
  if(h0==h1 && h0>=1) m|=1ull<<0;                         // F1
  if(h0>=1 && h1>=1 && q0==q1) m|=1ull<<1;                // F2
  if(c0==c1 && c0>=1) m|=1ull<<2;                         // R0c
  if(c0>=1 && c1>=1 && s0==s1) m|=1ull<<3;                // R0e
  if(c2==c3 && c2>=1) m|=1ull<<4;                         // R1c
  if(c2>=1 && c3>=1 && s2==s3) m|=1ull<<5;                // R1e
  if(dolu && k_kosu4(s0,s1,s2,s3)) m|=1ull<<6;            // K1
  if(K2) m|=1ull<<7;                                      // K2
  if(dolu && sq==4*p) m|=1ull<<8;                         // Qc
  if(k>=1 && sq%p==0) m|=1ull<<9;                         // QS
  if(h0>=1 && h1>=1 && q0%p==0 && q1%p==0) m|=1ull<<10;   // Qh
  if(k0==k1 && k0>=1) m|=1ull<<11;                        // FC1
  if(k0>=1 && k1>=1 && cs0==cs1) m|=1ull<<12;             // FC2
  if(g0==g1 && g0>=1) m|=1ull<<13;                        // FD1
  if(g0>=1 && g1>=1 && ds0==ds1) m|=1ull<<14;             // FD2
  if(k>=1){ if(sq==p) m|=1ull<<15; if(sq==2*p) m|=1ull<<16; if(sq%pp==0) m|=1ull<<17;
    if(sa==p) m|=1ull<<32; if(sa==2*p) m|=1ull<<33; if(sa%p==0) m|=1ull<<34; if(sa%pp==0) m|=1ull<<35;
    if(sn==p) m|=1ull<<36; if(sn==2*p) m|=1ull<<37; if(sn%p==0) m|=1ull<<38; if(sn%pp==0) m|=1ull<<39; }
  if(h0>=1 && h1>=1){
    if(q0==p && q1==p) m|=1ull<<18; if(q0==2*p && q1==2*p) m|=1ull<<19; if(sq==2*p) m|=1ull<<20; if(q0%pp==0 && q1%pp==0) m|=1ull<<21;
    if(h0==p && h1==p) m|=1ull<<27; if(h0==2*p && h1==2*p) m|=1ull<<28; if(h0+h1==2*p) m|=1ull<<29;
    if(h0%p==0 && h1%p==0) m|=1ull<<30; if(h0%pp==0 && h1%pp==0) m|=1ull<<31;
    if(q0-q1==1 || q1-q0==1) m|=1ull<<40; }                // FRK
  if(k0>=1 && k1>=1){
    if(cs0==p && cs1==p) m|=1ull<<22; if(cs0==2*p && cs1==2*p) m|=1ull<<23; if(cs0+cs1==2*p) m|=1ull<<24;
    if(cs0%p==0 && cs1%p==0) m|=1ull<<25; if(cs0%pp==0 && cs1%pp==0) m|=1ull<<26;
    if(cs0-cs1==1 || cs1-cs0==1) m|=1ull<<41; }            // FCK
  if(dolu){
    if(s0==p && s1==p && s2==p && s3==p) m|=1ull<<42; if(s0==2*p && s1==2*p && s2==2*p && s3==2*p) m|=1ull<<43;
    if(s0%p==0 && s1%p==0 && s2%p==0 && s3%p==0) m|=1ull<<44; if(s0%pp==0 && s1%pp==0 && s2%pp==0 && s3%pp==0) m|=1ull<<45;
    if(c0==p && c1==p && c2==p && c3==p) m|=1ull<<46; if(c0==2*p && c1==2*p && c2==2*p && c3==2*p) m|=1ull<<47;
    if(c0+c1+c2+c3==4*p) m|=1ull<<48;
    if(c0%p==0 && c1%p==0 && c2%p==0 && c3%p==0) m|=1ull<<49; if(c0%pp==0 && c1%pp==0 && c2%pp==0 && c3%pp==0) m|=1ull<<50;
    if(k_ardisik4(s0,s1,s2,s3)) m|=1ull<<51;               // FAP
    if(k_kosu4(c0,c1,c2,c3)) m|=1ull<<52; }                // FKS
  if((c0>=2&&q0e) || (c1>=2&&q1e) || (c2>=2&&q2e) || (c3>=2&&q3e)) m|=1ull<<53;   // FSC
  uint8_t mz=0; if(k==p) mz|=1u<<0; if(k==2*p) mz|=1u<<1; if(k>=1 && k%p==0) mz|=1u<<2; if(k>=1 && k%pp==0) mz|=1u<<3;
  // iki el, birleşim
  const uint64_t i0=b0&hb0, i1=b1&hb1; const int ni=k_pc64(i0)+k_pc64(i1); const int kU=k+kH-ni;
  int saU=sa+hsa, snU=sn+hsn, stU=p*sq+hst;
  if(ni){ uint64_t w=i0; while(w){ const int b=k_ctz64(w); w&=w-1; const int nn=b+1, x=a[b]; saU-=x; snU-=nn; stU-=al*nn+be*x; }
          w=i1; while(w){ const int b=k_ctz64(w); w&=w-1; const int nn=b+65, x=a[b+64]; saU-=x; snU-=nn; stU-=al*nn+be*x; } }
  uint64_t mw=0;
  if(k>=1 && kH>=1 && ni==0) mw|=1ull<<0;                 // W1
  if(kU==p) mw|=1ull<<1;                                  // QU
  if(kH==p) mw|=1ull<<2;                                  // QkH
  if(kU>=1 && saU%p==0) mw|=1ull<<3;                      // Qa
  if(kU>=1 && snU%p==0) mw|=1ull<<4;                      // Qn
  if(kH>=1 && hsr%p==0) mw|=1ull<<5;                      // QH
  if(ni==1) mw|=1ull<<6;                                  // WI1
  if(k>=1 && kH>=1 && k==kH) mw|=1ull<<7;                 // WEq
  if(k>=1 && kH>=1 && k+kH==p) mw|=1ull<<8;               // WSum
  if(kU==2*p) mw|=1ull<<9; if(kU>=1 && kU%p==0) mw|=1ull<<10; if(kU>=1 && kU%pp==0) mw|=1ull<<11;
  if(kH==2*p) mw|=1ull<<12; if(kH>=1 && kH%p==0) mw|=1ull<<13; if(kH>=1 && kH%pp==0) mw|=1ull<<14;
  if(ni==p) mw|=1ull<<15; if(ni==2*p) mw|=1ull<<16; if(ni>=1 && ni%p==0) mw|=1ull<<17; if(ni>=1 && ni%pp==0) mw|=1ull<<18;
  if(kU>=1){ if(saU==p) mw|=1ull<<19; if(saU==2*p) mw|=1ull<<20; if(saU%pp==0) mw|=1ull<<21;
    if(snU==p) mw|=1ull<<22; if(snU==2*p) mw|=1ull<<23; if(snU%pp==0) mw|=1ull<<24;
    if(stU==p) mw|=1ull<<25; if(stU==2*p) mw|=1ull<<26; if(stU%p==0) mw|=1ull<<27; if(stU%pp==0) mw|=1ull<<28; }
  if(kH>=1){ if(hsa==p) mw|=1ull<<29; if(hsa==2*p) mw|=1ull<<30; if(hsa%p==0) mw|=1ull<<31; if(hsa%pp==0) mw|=1ull<<32;
    if(hsn==p) mw|=1ull<<33; if(hsn==2*p) mw|=1ull<<34; if(hsn%p==0) mw|=1ull<<35; if(hsn%pp==0) mw|=1ull<<36;
    if(hsr==p) mw|=1ull<<37; if(hsr==2*p) mw|=1ull<<38; if(hsr%pp==0) mw|=1ull<<39; }
  // karşı aile içi
  const int hh0=hc0+hc1, hh1=hc2+hc3; uint8_t mh=0;
  if(hh0==hh1 && hh0>=1) mh|=1u<<0;                       // HH1
  if(hh0>=1 && hh1>=1 && r0+r1==r2+r3) mh|=1u<<1;         // HH2
  if(hc0==hc1 && hc0>=1) mh|=1u<<2;                       // HR0c
  if(hc0>=1 && hc1>=1 && r0==r1) mh|=1u<<3;               // HR0e
  if(hc2==hc3 && hc2>=1) mh|=1u<<4;                       // HR1c
  if(hc2>=1 && hc3>=1 && r2==r3) mh|=1u<<5;               // HR1e
  if(HK2) mh|=1u<<6;                                      // HK2
  P->B[L]=mb&~C->exb[L]; P->Z[L]=mz&~C->exz[L]; P->F[L]=m&~C->exf[L]; P->W[L]=mw&~C->exw[L]; P->H[L]=mh&~C->exh[L];
  P->kF[L]=(uint8_t)k; P->kH[L]=(uint8_t)kH;
  int mc=c0; if(c1<mc) mc=c1; if(c2<mc) mc=c2; if(c3<mc) mc=c3; P->mc[L]=(uint8_t)mc; }

// ---------------- puan (dtab.h dtab_mercek, dtab_b.h kapsam / puan_B) ----------------
KD static inline int k_kapi(const ksabit_t *C, const kprof_t *P, int L){ return (int)P->kF[L]*C->pr[L%K_NPR]>=C->KE && P->mc[L]>=C->KH; }
KD static inline uint32_t k_dsik(uint64_t mask, const int *pos, int m){ uint32_t x=0; for(int j=0;j<m;j++) x|=(uint32_t)((mask>>pos[j])&1ull)<<j; return x; }
KD static inline int k_kapsam_tamam(const ksabit_t *C, const kdil_t *D, const kprof_t *P, int L){
  const uint64_t x[5]={P->B[L],P->Z[L],P->F[L],P->W[L],P->H[L]}; unsigned s=0;
  for(int g=0;g<5;g++){ uint64_t y=x[g]&D->gm[g]; while(y){ const int b=k_ctz64(y); y&=y-1; s|=C->imza[C->gofs[g]+b]; } }
  int nd=0, nt=0; for(int b=0;b<4;b++){ nd+=(s>>b)&1u; nt+=(s>>(4+b))&1u; } return nd>=D->dmin && nt>=D->tmin; }
KD static inline double k_mercek_puan(const ksabit_t *C, const kdil_t *D, const float *H, const kprof_t *P, int L){
  const int kb=P->kF[L]<K_KC-1?P->kF[L]:K_KC-1, hb=P->kH[L]<K_KC-1?P->kH[L]:K_KC-1; const int cf=C->cF[L][kb], ch=C->cH[L][hb]; double b=0;
  if(D->ng[0]){ uint64_t x=P->B[L]&D->gm[0]; if(x) b+=H[D->oTB+(((uint64_t)L<<D->ng[0])|k_dsik(x,D->pos[0],D->ng[0]))]; }
  if(D->ng[1]){ uint64_t x=P->Z[L]&D->gm[1]; if(x) b+=H[D->oTZ+(((uint64_t)L<<D->ng[1])|k_dsik(x,D->pos[1],D->ng[1]))]; }
  if(D->ng[2]){ uint64_t x=P->F[L]&D->gm[2]; if(x) b+=H[D->oTF+(((uint64_t)(C->ofs[L]+cf)<<D->ng[2])|k_dsik(x,D->pos[2],D->ng[2]))]; }
  if(D->ng[3]){ uint64_t x=P->W[L]&D->gm[3]; if(x) b+=H[D->oTW+(((uint64_t)(C->ofs[L]+cf)<<D->ng[3])|k_dsik(x,D->pos[3],D->ng[3]))]; }
  if(D->ng[4]){ uint64_t x=P->H[L]&D->gm[4]; if(x) b+=H[D->oTH+(((uint64_t)(C->ofsH[L]+ch)<<D->ng[4])|k_dsik(x,D->pos[4],D->ng[4]))]; }
  return b-C->lbedel[D->onsel][L]; }
#endif
