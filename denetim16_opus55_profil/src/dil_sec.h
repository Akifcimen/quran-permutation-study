// dil_sec.h — keşif yordamı D(x) (ONKAYIT §4): Λ ⊆ L*, |Λ| ≤ 20, amaç S_x(Λ) − c(Λ) en büyük.
// S_x(Λ) = max_λ [I_Λ(λ) − c(λ)] ve c(Λ) azalmayan olduğundan en iyi Λ, en iyi mercekte x'in sağladığı yüklemlerin alt kümesidir.
// Her mercek ve her grup için sağlanan yüklemlerin bütün alt kümeleri tam olarak taranır (grup başına en çok 24 aday; fazlası varsa
// tekil bilgisi en yüksek 24'ü, eşitlikte küçük indeks); gruplar |Λ| ≤ 20 altında boyut başına en iyilerle birleştirilir.
// Eşitlik bozma (1e-9 bit içinde eşit amaç): küçük |Λ|, sonra küçük mercek indeksi, sonra sıralı global indeks listesi sözlükte küçük.
#ifndef DIL_SEC_H
#define DIL_SEC_H
#include "tablo.h"
#define DMAX 20
#define GADAY 24
#define ETOL 1e-9
typedef struct { int L; int n; int g[DMAX]; double amac, bilgi, bedel; double ig[NGRUP]; } dil_t;   // g: global yüklem indeksleri (sıralı)
static uint64_t LMASK[NGRUP];                    // grup başına L* bitleri
static void lmask_hazirla(void){ for(int k=0;k<NGRUP;k++){ uint64_t m=0; for(int b=0;b<GBOY[k];b++) if(LUYE[GOFS[k]+b]) m|=1ull<<b; LMASK[k]=m; } }
// sıralı listeler için sözlük karşılaştırması
static int liste_kucuk(const int *a, int na, const int *b, int nb){ int n=na<nb?na:nb; for(int i=0;i<n;i++){ if(a[i]!=b[i]) return a[i]<b[i]; } return na<nb; }
typedef struct { int m; int pos[GADAY]; double cc[GADAY]; uint32_t *A; double *C; double best[DMAX+1]; uint32_t arg[DMAX+1]; } genum_t;
// Bir grubun aday kümesi üzerinde tam alt-küme taraması. A[S] = en-az sayısı (önceden doldurulmuş), n = bağlam büyüklüğü.
static void genum_tara(genum_t *G, uint64_t n){
  const int m=G->m; const uint32_t top=1u<<m; G->C[0]=0.0; for(int j=0;j<=DMAX;j++){ G->best[j]=-INFINITY; G->arg[j]=0; } G->best[0]=0.0;
  for(uint32_t S=1;S<top;S++){ const int b=__builtin_ctz(S); G->C[S]=G->C[S&(S-1)]+G->cc[b]; const int j=__builtin_popcount(S); if(j>DMAX) continue;
    const double v=bitf(G->A[S],n)-G->C[S];
    if(v>G->best[j]+ETOL){ G->best[j]=v; G->arg[j]=S; }
    else if(v>=G->best[j]-ETOL){ const uint32_t d=S^G->arg[j]; if(S&(d&(~d+1))){ G->best[j]=v; G->arg[j]=S; } } } }
// tekil bilgiye göre en iyi GADAY adayı seç (eşitlikte küçük bit), konumları artan sırada bırak
static int aday_sec(uint64_t s, const double *tek, int *pos){
  int c[64], nc=0; for(int b=0;b<64;b++) if(s>>b&1) c[nc++]=b;
  if(nc>GADAY){ for(int i=0;i<nc;i++) for(int j=i+1;j<nc;j++){ const int x=c[i], y=c[j]; if(tek[y]>tek[x]+ETOL || (fabs(tek[y]-tek[x])<=ETOL && y<x)){ c[i]=y; c[j]=x; } }
    nc=GADAY; for(int i=0;i<nc;i++) for(int j=i+1;j<nc;j++) if(c[j]<c[i]){ int t=c[i]; c[i]=c[j]; c[j]=t; } }
  for(int i=0;i<nc;i++) pos[i]=c[i]; return nc; }
static inline uint32_t sikistir(uint64_t mask, const int *pos, int m){ uint32_t x=0; for(int j=0;j<m;j++) x|=(uint32_t)((mask>>pos[j])&1ull)<<j; return x; }
// F/W listelerinden bağlam için tekil ve alt-küme sayımları
static void fw_hazirla(const atablo_t *T, int L, int ctx, int g, uint64_t s, genum_t *G, int grup_ofs){
  const int16_t *cm=(g==0)?T->cF[L]:T->cF[L]; double tek[64]; uint64_t cnt[64]; memset(cnt,0,sizeof cnt);
  const uint64_t n=T->nnF[L][ctx];
  for(int kb=0;kb<KC;kb++){ if(cm[kb]!=ctx) continue; const uint32_t lk=LKG(L,kb,g); for(uint64_t e=T->bas[lk];e<T->bas[lk+1];e++){ uint64_t x=T->E[e].mask&s; while(x){ int b=__builtin_ctzll(x); x&=x-1; cnt[b]+=T->E[e].c; } } }
  for(int b=0;b<64;b++) tek[b]=(s>>b&1)?bitf(cnt[b],n):0.0;
  G->m=aday_sec(s,tek,G->pos); for(int j=0;j<G->m;j++) G->cc[j]=PBEDEL[grup_ofs+G->pos[j]];
  const uint32_t top=1u<<G->m; memset(G->A,0,sizeof(uint32_t)*top);
  for(int kb=0;kb<KC;kb++){ if(cm[kb]!=ctx) continue; const uint32_t lk=LKG(L,kb,g); for(uint64_t e=T->bas[lk];e<T->bas[lk+1];e++){ G->A[sikistir(T->E[e].mask,G->pos,G->m)]+=T->E[e].c; } }
  for(int b=0;b<G->m;b++) for(uint32_t S=0;S<top;S++) if(!(S>>b&1)) G->A[S]+=G->A[S|(1u<<b)];
  genum_tara(G,n); }
// yoğun tablolu gruplar (B, Z, H): A[S] = tablo[genişlet(S)]
static void yogun_hazirla(const uint64_t *tab, uint64_t n, uint64_t s, genum_t *G, int grup_ofs){
  double tek[64]; for(int b=0;b<64;b++) tek[b]=(s>>b&1)?bitf(tab[1u<<b],n):0.0;
  G->m=aday_sec(s,tek,G->pos); for(int j=0;j<G->m;j++) G->cc[j]=PBEDEL[grup_ofs+G->pos[j]];
  const uint32_t top=1u<<G->m; for(uint32_t S=0;S<top;S++){ uint32_t M=0; for(int j=0;j<G->m;j++) if(S>>j&1) M|=1u<<G->pos[j]; G->A[S]=(uint32_t)tab[M]; }
  genum_tara(G,n); }
// A ve C tampunları yalnız tarama sırasında gerekir: bütün gruplar aynı tamponu paylaşır (iş parçacığı başına ~192 MB)
static genum_t *genum_ayir(void){ genum_t *G=calloc(NGRUP,sizeof(genum_t)); uint32_t *A=malloc(sizeof(uint32_t)<<GADAY); double *C=malloc(sizeof(double)<<GADAY);
  if(!A||!C){ fprintf(stderr,"bellek (genum)\n"); exit(2); } for(int k=0;k<NGRUP;k++){ G[k].A=A; G[k].C=C; } return G; }
// D(x): profil P, önsel (0 = P0, 1 = P2). Sonuç d.
static void dil_bul(const atablo_t *T, const prof_t *P, int onsel, genum_t *G, dil_t *d){
  d->amac=-INFINITY; d->n=0; d->L=-1; const double lg21=log2(21.0);
  for(int L=0;L<NLENS;L++){
    const int kb=kkutu(P->kF[L]), hb=kkutu(P->kH[L]); const int cf=T->cF[L][kb], ch=T->cH[L][hb];
    yogun_hazirla(T->BA[L],T->N,(uint64_t)P->B[L]&LMASK[G_B],&G[G_B],GOFS[G_B]);
    yogun_hazirla(T->ZA[L],T->N,(uint64_t)P->Z[L]&LMASK[G_Z],&G[G_Z],GOFS[G_Z]);
    fw_hazirla(T,L,cf,0,P->F[L]&LMASK[G_F],&G[G_F],GOFS[G_F]);
    fw_hazirla(T,L,cf,1,P->W[L]&LMASK[G_W],&G[G_W],GOFS[G_W]);
    yogun_hazirla(T->HA+((size_t)L*NCX+ch)*128,T->nnH[L][ch],(uint64_t)P->H[L]&LMASK[G_H],&G[G_H],GOFS[G_H]);
    const double cl=LBEDEL[onsel][L];
    int mx[NGRUP]; for(int k=0;k<NGRUP;k++){ mx[k]=G[k].m<DMAX?G[k].m:DMAX; }
    for(int jb=0;jb<=mx[0];jb++){ if(G[0].best[jb]==-INFINITY) continue;
     for(int jz=0;jz<=mx[1] && jb+jz<=DMAX;jz++){ if(G[1].best[jz]==-INFINITY) continue;
      for(int jf=0;jf<=mx[2] && jb+jz+jf<=DMAX;jf++){ if(G[2].best[jf]==-INFINITY) continue;
       for(int jw=0;jw<=mx[3] && jb+jz+jf+jw<=DMAX;jw++){ if(G[3].best[jw]==-INFINITY) continue;
        for(int jh=0;jh<=mx[4] && jb+jz+jf+jw+jh<=DMAX;jh++){ if(G[4].best[jh]==-INFINITY) continue;
          const int J=jb+jz+jf+jw+jh; const int js[NGRUP]={jb,jz,jf,jw,jh};
          double v=G[0].best[jb]+G[1].best[jz]+G[2].best[jf]+G[3].best[jw]+G[4].best[jh]+lgamma((double)J+1.0)/log(2.0)-lg21-cl;
          int esit=0; if(v>d->amac+ETOL) esit=0; else if(v>=d->amac-ETOL) esit=1; else continue;
          int gl[DMAX], ng=0; for(int k=0;k<NGRUP;k++){ uint32_t S=G[k].arg[js[k]]; while(S){ int b=__builtin_ctz(S); S&=S-1; gl[ng++]=GOFS[k]+G[k].pos[b]; } }
          if(esit){ if(J>d->n) continue; if(J==d->n){ if(L>d->L) continue; if(L==d->L && !liste_kucuk(gl,ng,d->g,d->n)) continue; } }
          d->amac=v; d->L=L; d->n=ng; memcpy(d->g,gl,sizeof(int)*ng); double bilgi=0, bed=0;
          for(int k=0;k<NGRUP;k++){ uint32_t S=G[k].arg[js[k]]; double c=0; for(int b=0;b<G[k].m;b++) if(S>>b&1) c+=G[k].cc[b]; const double ik=G[k].best[js[k]]+c; d->ig[k]=ik; bilgi+=ik; bed+=c; }
          d->bilgi=bilgi; d->bedel=lg21+bed-lgamma((double)J+1.0)/log(2.0); } } } } } }
}
#endif
