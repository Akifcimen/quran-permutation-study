// dtab_b.h — plasebo14: sabit bir dil Λ için kol başına puanlama (ONKAYIT §4–§6). plasebo12'nin dtab.h'si değiştirilmeden içerilir
// (tablolar, dtab_mercek, dtab_puan, dtab_asar, dtab_sinif aynen); bu dosya üstüne üç şey ekler:
//  (1) bütünlüklü puan (Kol A, A−, A+): S^B_y(Λ) = max { I_Λ(λ) − c(λ) : λ'da kapı G ve Λ'nın orada sağlanan yüklemleri ≥ tmin türden ve
//      ≥ dmin düzeyden }; hiçbir mercekte B tutmuyorsa puansız (−∞, "aşmaz"). Kapı G yalnız kitabın kendi profilinden okunur; kontrol
//      başına kapı G'nin tuttuğu mercekler bir kez listelenir.
//  (2) Kol B (bedelsiz en geniş dil): puan plasebo12'deki gibidir (bütün mercekler, B yok); dil 20'den büyük olabilir (DMAX = 64 ile
//      derlenir). Boy sınırıyla kesin budama: her (mercek, bağlam, grup) için T_g'nin s boylu alt kümeler üzerindeki en büyüğü UB_g[s];
//      bir mercekte Σ_g UB_g[|S_g|] − c(λ) eşiğin altındaysa o mercek atlanır (değer bu sınırı aşamaz; sonuç değişmez).
//  (3) kol kodu: diller dosyasının 3. sütunu = önsel + 10·kol (kol 0 = plasebo12 puanı, 1 = A, 2 = A−, 3 = A+, 4 = B).
#ifndef DTAB_B_H
#define DTAB_B_H
#include "dtab.h"
static inline uint8_t imza_b(int g){ return (uint8_t)((1u<<PDUZEY[g])|(1u<<(4+PTUR[g]))); }
static inline int kapsam_tamam_b(uint8_t s, int tmin, int dmin){ return __builtin_popcount(s&15u)>=dmin && __builtin_popcount((unsigned)s>>4)>=tmin; }
static const int KOL_TMIN[5]={0,3,2,4,0}, KOL_DMIN[5]={0,3,2,4,0};
typedef struct { int kol, tmin, dmin; float *UB[NGRUP]; int ubs[NGRUP]; size_t bayt; } dek_t;
// kontrol başına: kapı G'nin tuttuğu mercekler
static inline int kapi_listesi(const prof_t *P, int *GL){ int n=0; for(int L=0;L<NLENS;L++) if(kapiG(P,L)) GL[n++]=L; return n; }
// Λ'nın λ'da sağlanan yüklemlerinin kapsam imzası
static inline uint8_t kapsam(const dtab_t *D, const prof_t *P, int L){ uint8_t s=0; const uint64_t x[NGRUP]={P->B[L],P->Z[L],P->F[L],P->W[L],P->H[L]};
  for(int k=0;k<NGRUP;k++){ uint64_t y=x[k]&D->gm[k]; while(y){ int b=__builtin_ctzll(y); y&=y-1; s|=imza_b(GOFS[k]+b); } } return s; }
// (1) bütünlüklü puan; GL = kapı G mercekleri. *bvar: en az bir mercekte B tutuyor mu. Dönüş: en iyi değer (yoksa −∞), *enL.
static double puan_B(const dtab_t *D, const dek_t *E, const atablo_t *T, const prof_t *P, const int *GL, int nGL, int *enL, int *bvar){
  double best=-INFINITY; int bl=-1, bv=0;
  for(int i=0;i<nGL;i++){ const int L=GL[i]; if(!kapsam_tamam_b(kapsam(D,P,L),E->tmin,E->dmin)) continue; bv=1;
    if(D->mx[L]-D->lc[L]<=best) continue; const double v=dtab_mercek(D,T,P,L)-D->lc[L]; if(v>best){ best=v; bl=L; } }
  if(enL) *enL=bl; if(bvar) *bvar=bv; return best; }
// (2) boy sınırları: UB_g[(ctx)*(ng+1) + s]
static void ub_kur(const dtab_t *D, const atablo_t *T, dek_t *E){
  const int nx[NGRUP]={NLENS,NLENS,D->ofs[NLENS],D->ofs[NLENS],D->ofsH[NLENS]}; const float *TT[NGRUP]={D->TB,D->TZ,D->TF,D->TW,D->TH}; E->bayt=0;
  for(int k=0;k<NGRUP;k++){ const int m=D->ng[k]; E->ubs[k]=m+1; E->UB[k]=calloc((size_t)nx[k]*(m+1),sizeof(float)); E->bayt+=(size_t)nx[k]*(m+1)*sizeof(float);
    if(!E->UB[k]){ fprintf(stderr,"bellek (UB)\n"); exit(2); }
    for(int c=0;c<nx[k];c++){ float *u=E->UB[k]+(size_t)c*(m+1); const float *t=TT[k]+((size_t)c<<m);
      for(uint32_t s=1;s<(1u<<m);s++){ const int j=__builtin_popcount(s); if(t[s]>u[j]) u[j]=t[s]; } } }
  (void)T; }
static inline double ub_mercek(const dtab_t *D, const dek_t *E, const atablo_t *T, const prof_t *P, int L){
  const int cf=T->cF[L][kkutu(P->kF[L])], ch=T->cH[L][kkutu(P->kH[L])]; double b=0;
  if(D->ng[0]) b+=E->UB[0][(size_t)L*E->ubs[0]+__builtin_popcountll((uint64_t)P->B[L]&D->gm[0])];
  if(D->ng[1]) b+=E->UB[1][(size_t)L*E->ubs[1]+__builtin_popcountll((uint64_t)P->Z[L]&D->gm[1])];
  if(D->ng[2]) b+=E->UB[2][(size_t)(D->ofs[L]+cf)*E->ubs[2]+__builtin_popcountll(P->F[L]&D->gm[2])];
  if(D->ng[3]) b+=E->UB[3][(size_t)(D->ofs[L]+cf)*E->ubs[3]+__builtin_popcountll(P->W[L]&D->gm[3])];
  if(D->ng[4]) b+=E->UB[4][(size_t)(D->ofsH[L]+ch)*E->ubs[4]+__builtin_popcountll((uint64_t)P->H[L]&D->gm[4])];
  return b; }
// bağlamları önceden hesaplanmış (kontrol başına bir kez) sürümler: değerler dtab_mercek/ub_mercek ile birebir aynıdır
static inline double ub_mercek2(const dtab_t *D, const dek_t *E, const prof_t *P, const int16_t *cfL, const int16_t *chL, int L){
  const int cf=cfL[L], ch=chL[L]; double b=0;
  if(D->ng[0]) b+=E->UB[0][(size_t)L*E->ubs[0]+__builtin_popcountll((uint64_t)P->B[L]&D->gm[0])];
  if(D->ng[1]) b+=E->UB[1][(size_t)L*E->ubs[1]+__builtin_popcountll((uint64_t)P->Z[L]&D->gm[1])];
  if(D->ng[2]) b+=E->UB[2][(size_t)(D->ofs[L]+cf)*E->ubs[2]+__builtin_popcountll(P->F[L]&D->gm[2])];
  if(D->ng[3]) b+=E->UB[3][(size_t)(D->ofs[L]+cf)*E->ubs[3]+__builtin_popcountll(P->W[L]&D->gm[3])];
  if(D->ng[4]) b+=E->UB[4][(size_t)(D->ofsH[L]+ch)*E->ubs[4]+__builtin_popcountll((uint64_t)P->H[L]&D->gm[4])];
  return b; }
static inline double dtab_mercek2(const dtab_t *D, const prof_t *P, const int16_t *cfL, const int16_t *chL, int L){
  const int cf=cfL[L], ch=chL[L]; double b=0;
  if(D->ng[0]){ uint64_t x=P->B[L]&D->gm[0]; if(x) b+=D->TB[((size_t)L<<D->ng[0])|dsik(x,D->pos[0],D->ng[0])]; }
  if(D->ng[1]){ uint64_t x=P->Z[L]&D->gm[1]; if(x) b+=D->TZ[((size_t)L<<D->ng[1])|dsik(x,D->pos[1],D->ng[1])]; }
  if(D->ng[2]){ uint64_t x=P->F[L]&D->gm[2]; if(x) b+=D->TF[((size_t)(D->ofs[L]+cf)<<D->ng[2])|dsik(x,D->pos[2],D->ng[2])]; }
  if(D->ng[3]){ uint64_t x=P->W[L]&D->gm[3]; if(x) b+=D->TW[((size_t)(D->ofs[L]+cf)<<D->ng[3])|dsik(x,D->pos[3],D->ng[3])]; }
  if(D->ng[4]){ uint64_t x=P->H[L]&D->gm[4]; if(x) b+=D->TH[((size_t)(D->ofsH[L]+ch)<<D->ng[4])|dsik(x,D->pos[4],D->ng[4])]; }
  return b; }
static inline void baglam_listesi(const atablo_t *T, const prof_t *P, int16_t *cfL, int16_t *chL){
  for(int L=0;L<NLENS;L++){ cfL[L]=T->cF[L][kkutu(P->kF[L])]; chL[L]=T->cH[L][kkutu(P->kH[L])]; } }
#define UB_PAY 1e-9
// Kol B: taban'dan (ör. eşik − 2) küçük olmayan en iyi değer; tabana ulaşan mercek yoksa −∞. Budama kesindir.
static double puan_G(const dtab_t *D, const dek_t *E, const atablo_t *T, const prof_t *P, double taban, int *enL){
  double best=-INFINITY; int bl=-1;
  for(int L=0;L<NLENS;L++){ const double lim=(best>taban?best:taban);
    if(D->mx[L]-D->lc[L]<lim-UB_PAY) continue; if(ub_mercek(D,E,T,P,L)-D->lc[L]<lim-UB_PAY) continue;
    const double v=dtab_mercek(D,T,P,L)-D->lc[L]; if(v>best){ best=v; bl=L; } }
  if(enL) *enL=bl; return best>=taban?best:-INFINITY; }
static inline int asar_G(const dtab_t *D, const dek_t *E, const atablo_t *T, const prof_t *P, double esik){
  for(int L=0;L<NLENS;L++){ if(D->mx[L]-D->lc[L]<esik-1e-6) continue; if(ub_mercek(D,E,T,P,L)-D->lc[L]<esik-1e-6-UB_PAY) continue;
    if(dtab_mercek(D,T,P,L)-D->lc[L]>=esik-1e-6) return 1; } return 0; }
static double puan_G2(const dtab_t *D, const dek_t *E, const prof_t *P, const int16_t *cfL, const int16_t *chL, double taban, int *enL){
  double best=-INFINITY; int bl=-1;
  for(int L=0;L<NLENS;L++){ const double lim=(best>taban?best:taban);
    if(D->mx[L]-D->lc[L]<lim-UB_PAY) continue; if(ub_mercek2(D,E,P,cfL,chL,L)-D->lc[L]<lim-UB_PAY) continue;
    const double v=dtab_mercek2(D,P,cfL,chL,L)-D->lc[L]; if(v>best){ best=v; bl=L; } }
  if(enL) *enL=bl; return best>=taban?best:-INFINITY; }
static inline int asar_G2(const dtab_t *D, const dek_t *E, const prof_t *P, const int16_t *cfL, const int16_t *chL, double esik){
  for(int L=0;L<NLENS;L++){ if(D->mx[L]-D->lc[L]<esik-1e-6) continue; if(ub_mercek2(D,E,P,cfL,chL,L)-D->lc[L]<esik-1e-6-UB_PAY) continue;
    if(dtab_mercek2(D,P,cfL,chL,L)-D->lc[L]>=esik-1e-6) return 1; } return 0; }
#endif
