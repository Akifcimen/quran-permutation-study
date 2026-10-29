// dil_mekanik — D yordamının mekanik sınaması (ONKAYIT §0): ölçüm referansıyla (tür 6) ve kod-doğrulama kitaplarıyla (tür 4; hedef değil).
// (i) üst-küme sayımları: fw_hazirla/yogun_hazirla'nın A[S] değerleri doğrudan sayımla aynı mı (rastgele alt kümeler);
// (ii) D'nin döndürdüğü amaç, dilin doğrudan yeniden hesaplanan puanıyla (S_x(Λ) − c(Λ)) aynı mı; (iii) süre.
// Yalnız uyuşmazlık sayıları ve süre basılır; hiçbir dil ya da puan basılmaz.
#include "../src/dil_sec.h"
#include <time.h>
// bir dilin bir kitaptaki puanı: max_L [Σ_g bit(en-az sayısı) − c(L)], sayımlar doğrudan listelerden (yavaş, bağımsız yol)
static double puan_dogrudan(const atablo_t *T, const prof_t *P, const int *g, int ng, int onsel){
  uint64_t lm[NGRUP]={0}; for(int i=0;i<ng;i++){ int k=pgrup(g[i]); lm[k]|=1ull<<(g[i]-GOFS[k]); }
  double best=-INFINITY;
  for(int L=0;L<NLENS;L++){ const int kb=kkutu(P->kF[L]), hb=kkutu(P->kH[L]); const int cf=T->cF[L][kb], ch=T->cH[L][hb]; double b=0;
    uint64_t sB=P->B[L]&lm[0], sZ=P->Z[L]&lm[1], sF=P->F[L]&lm[2], sW=P->W[L]&lm[3], sH=P->H[L]&lm[4];
    if(sB) b+=bitf(T->BA[L][sB],T->N); if(sZ) b+=bitf(T->ZA[L][sZ],T->N); if(sH) b+=bitf(T->HA[((size_t)L*NCX+ch)*128+sH],T->nnH[L][ch]);
    for(int gg=0;gg<2;gg++){ uint64_t s=gg?sW:sF; if(!s) continue; uint64_t c=0; for(int k=0;k<KC;k++){ if(T->cF[L][k]!=cf) continue; uint32_t lk=LKG(L,k,gg);
        for(uint64_t e=T->bas[lk];e<T->bas[lk+1];e++) if((T->E[e].mask&s)==s) c+=T->E[e].c; } b+=bitf(c,T->nnF[L][cf]); }
    const double v=b-LBEDEL[onsel][L]; if(v>best) best=v; }
  return best; }
int main(int argc,char**argv){ int nk=atoi(argv[1]); kutuphane_init(); mercek_bedeli_hazirla(); lmask_hazirla();
  static atablo_t T; atablo_kur(&T,argv+2,argc-2);
  genum_t *G=genum_ayir(); orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,'M',tohum('M',4,9)); static prof_t P;
  long h1=0, n1=0, h2=0; double ts=0; rng_t r; rseed(&r,12345);
  for(int b=0;b<nk;b++){ orn_sonraki(Z); kdegerlendir(Z->a,&P,0x3FFF);
    // (i) rastgele (mercek, grup) için A[S] doğrulaması
    for(int t=0;t<20;t++){ int L=(int)bounded(&r,NLENS); int gg=(int)bounded(&r,2); const int cf=T.cF[L][kkutu(P.kF[L])];
      uint64_t s=(gg?P.W[L]&LMASK[G_W]:P.F[L]&LMASK[G_F]); fw_hazirla(&T,L,cf,gg,s,&G[2],gg?GOFS[G_W]:GOFS[G_F]);
      for(int q=0;q<8 && G[2].m>0;q++){ uint32_t S=(uint32_t)(xnext(&r)&((1ull<<G[2].m)-1)); uint64_t M=0; for(int j=0;j<G[2].m;j++) if(S>>j&1) M|=1ull<<G[2].pos[j];
        uint64_t c=0; for(int k=0;k<KC;k++){ if(T.cF[L][k]!=cf) continue; uint32_t lk=LKG(L,k,gg); for(uint64_t e=T.bas[lk];e<T.bas[lk+1];e++) if((T.E[e].mask&M)==M) c+=T.E[e].c; }
        n1++; if(c!=G[2].A[S]) h1++; } }
    // (ii) D amacı ↔ doğrudan yeniden hesap
    struct timespec t0,t1; clock_gettime(CLOCK_MONOTONIC,&t0); dil_t d; dil_bul(&T,&P,0,G,&d); clock_gettime(CLOCK_MONOTONIC,&t1);
    ts+=(t1.tv_sec-t0.tv_sec)+1e-9*(t1.tv_nsec-t0.tv_nsec);
    double s2=puan_dogrudan(&T,&P,d.g,d.n,0)-d.bedel; if(fabs(s2-d.amac)>1e-6) h2++; }
  printf("üst-küme sayımı uyuşmazlığı %ld / %ld; D amacı ↔ doğrudan puan uyuşmazlığı %ld / %d; D süresi kitap başına %.3f s\n",h1,n1,h2,nk,ts/nk);
  return (h1||h2)?1:0; }
