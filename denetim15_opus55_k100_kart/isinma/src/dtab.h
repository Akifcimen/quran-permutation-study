// dtab.h — sabit bir dil Λ için puan tabloları ve puanlama (ONKAYIT §5): her (mercek, bağlam, grup) için Λ'nın o gruptaki
// yüklemleri üzerinde en-az sayımlarının bit değeri (float), kontrolün puanı S_y(Λ) = max_L [Σ_g T_g − c(L)], en iyi mercek ve
// sınıf kuralı (ONKAYIT §8.3: kapı G; ≥ 3 motif türü; ≥ 3 düzey).
#ifndef DTAB_H
#define DTAB_H
#include "tablo.h"
#ifndef DMAX
#define DMAX 20
#endif
typedef struct {
  int n, g[DMAX]; int onsel;                          // dil: global yüklem indeksleri; mercek önseli (0 = P0, 1 = P2)
  int ng[NGRUP], pos[NGRUP][DMAX]; uint64_t gm[NGRUP]; // grup başına konumlar (artan) ve maske
  int ofs[NLENS+1], ofsH[NLENS+1];                     // bağlam ofsetleri (F/W için cF, H için cH)
  float *TB, *TZ, *TF, *TW, *TH;                       // TB[L<<nB | s], TZ[L<<nZ | s], TF[(ofs[L]+c)<<nF | s] ...
  double mx[NLENS];                                    // mercekte ulaşılabilir en büyük bit toplamı
  double lc[NLENS];                                    // mercek bedeli
  size_t bayt;
} dtab_t;
static inline uint32_t dsik(uint64_t mask, const int *pos, int m){ uint32_t x=0; for(int j=0;j<m;j++) x|=(uint32_t)((mask>>pos[j])&1ull)<<j; return x; }
static size_t dtab_bayt(const atablo_t *T, const int *g, int n){
  int ng[NGRUP]={0}; for(int i=0;i<n;i++) ng[pgrup(g[i])]++; size_t cf=0, ch=0; for(int L=0;L<NLENS;L++){ cf+=T->nF[L]; ch+=T->nH[L]; }
  return 4*((size_t)NLENS*((1ull<<ng[0])+(1ull<<ng[1])) + cf*((1ull<<ng[2])+(1ull<<ng[3])) + ch*(1ull<<ng[4])); }
static void dtab_kur(dtab_t *D, const atablo_t *T, const int *g, int n, int onsel){
  memset(D,0,sizeof *D); D->n=n; memcpy(D->g,g,sizeof(int)*n); D->onsel=onsel;
  for(int i=0;i<n;i++){ int k=pgrup(g[i]); D->pos[k][D->ng[k]++]=g[i]-GOFS[k]; D->gm[k]|=1ull<<(g[i]-GOFS[k]); }
  D->ofs[0]=0; D->ofsH[0]=0; for(int L=0;L<NLENS;L++){ D->ofs[L+1]=D->ofs[L]+T->nF[L]; D->ofsH[L+1]=D->ofsH[L]+T->nH[L]; D->lc[L]=LBEDEL[onsel][L]; }
  const int nB=D->ng[0], nZ=D->ng[1], nF=D->ng[2], nW=D->ng[3], nH=D->ng[4];
  D->TB=calloc((size_t)NLENS<<nB,4); D->TZ=calloc((size_t)NLENS<<nZ,4); D->TF=calloc((size_t)D->ofs[NLENS]<<nF,4); D->TW=calloc((size_t)D->ofs[NLENS]<<nW,4);
  D->TH=calloc((size_t)D->ofsH[NLENS]<<nH,4); D->bayt=dtab_bayt(T,g,n);
  if(!D->TB||!D->TZ||!D->TF||!D->TW||!D->TH){ fprintf(stderr,"bellek (dtab)\n"); exit(2); }
  uint64_t *A=malloc(8ull<<(nF>nW?nF:nW));
  for(int L=0;L<NLENS;L++){ double mb=0, mz=0, mf=0, mw=0, mh=0;
    for(uint32_t s=1;s<(1u<<nB);s++){ uint32_t M=0; for(int j=0;j<nB;j++) if(s>>j&1) M|=1u<<D->pos[0][j]; float v=(float)bitf(T->BA[L][M],T->N); D->TB[((size_t)L<<nB)|s]=v; if(v>mb) mb=v; }
    for(uint32_t s=1;s<(1u<<nZ);s++){ uint32_t M=0; for(int j=0;j<nZ;j++) if(s>>j&1) M|=1u<<D->pos[1][j]; float v=(float)bitf(T->ZA[L][M],T->N); D->TZ[((size_t)L<<nZ)|s]=v; if(v>mz) mz=v; }
    for(int c=0;c<T->nH[L];c++){ const uint64_t *h=T->HA+((size_t)L*NCX+c)*128; const uint64_t nn=T->nnH[L][c];
      for(uint32_t s=1;s<(1u<<nH);s++){ uint32_t M=0; for(int j=0;j<nH;j++) if(s>>j&1) M|=1u<<D->pos[4][j]; float v=(float)bitf(h[M],nn); D->TH[((size_t)(D->ofsH[L]+c)<<nH)|s]=v; if(v>mh) mh=v; } }
    for(int gg=0;gg<2;gg++){ const int m=gg?nW:nF; if(m==0) continue; const int *pos=gg?D->pos[3]:D->pos[2]; float *TT=gg?D->TW:D->TF; double *mm=gg?&mw:&mf;
      for(int c=0;c<T->nF[L];c++){ memset(A,0,8ull<<m); const uint64_t nn=T->nnF[L][c];
        for(int kb=0;kb<KC;kb++){ if(T->cF[L][kb]!=c) continue; const uint32_t lk=LKG(L,kb,gg); for(uint64_t e=T->bas[lk];e<T->bas[lk+1];e++) A[dsik(T->E[e].mask,pos,m)]+=T->E[e].c; }
        for(int b=0;b<m;b++) for(uint32_t S=0;S<(1u<<m);S++) if(!(S>>b&1)) A[S]+=A[S|(1u<<b)];
        for(uint32_t s=1;s<(1u<<m);s++){ float v=(float)bitf(A[s],nn); TT[((size_t)(D->ofs[L]+c)<<m)|s]=v; if(v>*mm) *mm=v; } } }
    D->mx[L]=mb+mz+mf+mw+mh; }
  free(A); }
static void dtab_birak(dtab_t *D){ free(D->TB); free(D->TZ); free(D->TF); free(D->TW); free(D->TH); }
// bir mercekteki bit toplamı
static inline double dtab_mercek(const dtab_t *D, const atablo_t *T, const prof_t *P, int L){
  const int kb=kkutu(P->kF[L]), hb=kkutu(P->kH[L]); const int cf=T->cF[L][kb], ch=T->cH[L][hb]; double b=0;
  if(D->ng[0]){ uint64_t x=P->B[L]&D->gm[0]; if(x) b+=D->TB[((size_t)L<<D->ng[0])|dsik(x,D->pos[0],D->ng[0])]; }
  if(D->ng[1]){ uint64_t x=P->Z[L]&D->gm[1]; if(x) b+=D->TZ[((size_t)L<<D->ng[1])|dsik(x,D->pos[1],D->ng[1])]; }
  if(D->ng[2]){ uint64_t x=P->F[L]&D->gm[2]; if(x) b+=D->TF[((size_t)(D->ofs[L]+cf)<<D->ng[2])|dsik(x,D->pos[2],D->ng[2])]; }
  if(D->ng[3]){ uint64_t x=P->W[L]&D->gm[3]; if(x) b+=D->TW[((size_t)(D->ofs[L]+cf)<<D->ng[3])|dsik(x,D->pos[3],D->ng[3])]; }
  if(D->ng[4]){ uint64_t x=P->H[L]&D->gm[4]; if(x) b+=D->TH[((size_t)(D->ofsH[L]+ch)<<D->ng[4])|dsik(x,D->pos[4],D->ng[4])]; }
  return b; }
// tam puan ve en iyi mercek (eşitlikte küçük mercek indeksi)
static double dtab_puan(const dtab_t *D, const atablo_t *T, const prof_t *P, int *enL){
  double best=-INFINITY; int bl=-1;
  for(int L=0;L<NLENS;L++){ if(D->mx[L]-D->lc[L]<=best) continue; const double v=dtab_mercek(D,T,P,L)-D->lc[L]; if(v>best){ best=v; bl=L; } }
  if(enL) *enL=bl; return best; }
// eşik sınaması: S_y(Λ) ≥ esik − tol ise 1 (erken çıkış)
static inline int dtab_asar(const dtab_t *D, const atablo_t *T, const prof_t *P, double esik){
  for(int L=0;L<NLENS;L++){ if(D->mx[L]-D->lc[L]<esik-1e-6) continue; if(dtab_mercek(D,T,P,L)-D->lc[L]>=esik-1e-6) return 1; } return 0; }
// sınıf kuralı: en iyi mercekte kapı G ve Λ'nın orada sağlanan yüklemleri ≥ 3 türden ve ≥ 3 düzeyden
static int dtab_sinif(const dtab_t *D, const prof_t *P, int L){
  if(L<0 || !kapiG(P,L)) return 0; int tur=0, duz=0; const uint64_t s[NGRUP]={P->B[L],P->Z[L],P->F[L],P->W[L],P->H[L]};
  for(int k=0;k<NGRUP;k++){ uint64_t x=s[k]&D->gm[k]; while(x){ int b=__builtin_ctzll(x); x&=x-1; const int gi=GOFS[k]+b; tur|=1<<PTUR[gi]; duz|=1<<PDUZEY[gi]; } }
  return __builtin_popcount(tur)>=3 && __builtin_popcount(duz)>=3; }
#endif
