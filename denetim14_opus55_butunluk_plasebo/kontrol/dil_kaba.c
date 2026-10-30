// dil_kaba — D'nin en iyiliğinin kaba-kuvvet sınaması (ONKAYIT §0): kod-doğrulama kitaplarında (tür 4), sağlanan L* yüklemlerinin
// birleşimi ≤ 14 olan her mercekte bütün alt kümeler (|S| ≤ 20) doğrudan sayımla puanlanır; D'nin amacı bu mercekler üzerindeki
// en büyük değerden küçük olmamalı ve D'nin merceği taranan mercekler arasındaysa eşit olmalı. Yalnız uyuşmazlık sayıları basılır.
#include "../src/dil_sec.h"
static double amac_dogrudan(const atablo_t *T, const prof_t *P, int L, const int *g, int ng){
  uint64_t lm[NGRUP]={0}; for(int i=0;i<ng;i++){ int k=pgrup(g[i]); lm[k]|=1ull<<(g[i]-GOFS[k]); }
  const int cf=T->cF[L][kkutu(P->kF[L])], ch=T->cH[L][kkutu(P->kH[L])]; double b=0;
  uint64_t sB=P->B[L]&lm[0], sZ=P->Z[L]&lm[1], sF=P->F[L]&lm[2], sW=P->W[L]&lm[3], sH=P->H[L]&lm[4];
  if(sB) b+=bitf(T->BA[L][sB],T->N); if(sZ) b+=bitf(T->ZA[L][sZ],T->N); if(sH) b+=bitf(T->HA[((size_t)L*NCX+ch)*128+sH],T->nnH[L][ch]);
  for(int gg=0;gg<2;gg++){ uint64_t s=gg?sW:sF; if(!s) continue; uint64_t c=0; for(int k=0;k<KC;k++){ if(T->cF[L][k]!=cf) continue; uint32_t lk=LKG(L,k,gg);
      for(uint64_t e=T->bas[lk];e<T->bas[lk+1];e++) if((T->E[e].mask&s)==s) c+=T->E[e].c; } b+=bitf(c,T->nnF[L][cf]); }
  double bed=0; for(int i=0;i<ng;i++) bed+=PBEDEL[g[i]]; bed+=log2(21.0)-lgamma((double)ng+1.0)/log(2.0);
  return b-bed-LBEDEL[0][L]; }
int main(int argc,char**argv){ int nk=atoi(argv[1]); kutuphane_init(); mercek_bedeli_hazirla(); lmask_hazirla();
  static atablo_t T; atablo_kur(&T,argv+2,argc-2); genum_t *G=genum_ayir(); orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,'U',tohum('U',4,9)); static prof_t P;
  long h_ust=0, h_esit=0, n_mer=0, n_esit=0;
  for(int b=0;b<nk;b++){ orn_sonraki(Z); kdegerlendir(Z->a,&P,0x3FFF); dil_t d; dil_bul(&T,&P,0,G,&d);
    double enb=-INFINITY; int enbL=-1;
    for(int L=0;L<NLENS;L++){ int u[64], nu=0; const uint64_t s[NGRUP]={P.B[L]&LMASK[0],P.Z[L]&LMASK[1],P.F[L]&LMASK[2],P.W[L]&LMASK[3],P.H[L]&LMASK[4]};
      for(int k=0;k<NGRUP;k++){ uint64_t x=s[k]; while(x){ int bb=__builtin_ctzll(x); x&=x-1; u[nu++]=GOFS[k]+bb; } }
      if(nu>14) continue; n_mer++; double lb=-INFINITY;
      for(uint32_t S=0;S<(1u<<nu);S++){ int g[20], ng=0; for(int j=0;j<nu;j++) if(S>>j&1) g[ng++]=u[j]; if(ng>DMAX) continue;
        double v=amac_dogrudan(&T,&P,L,g,ng); if(v>lb) lb=v; }
      if(lb>enb){ enb=lb; enbL=L; }
      if(L==d.L){ n_esit++; if(fabs(lb-d.amac)>1e-6) h_esit++; } }
    if(enb>d.amac+1e-6) h_ust++; }
  printf("kaba-kuvvet: taranan mercek %ld; D'den iyi alt küme bulunan kitap %ld / %d; D merceğinde eşitlik uyuşmazlığı %ld / %ld\n",n_mer,h_ust,nk,h_esit,n_esit);
  return (h_ust||h_esit)?1:0; }
