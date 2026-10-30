// dil_sec_b.h — plasebo14: bütünlük kısıtlı keşif yordamı D_B(x) (ONKAYIT §4) ve bedelsiz en geniş dil (Kol B, ONKAYIT §6).
// plasebo12'nin dil_sec.h'si değiştirilmeden içerilir: kısıtsız D (dil_bul), aday seçimi (aday_sec), üst-küme sayımları
// (fw_hazirla, yogun_hazirla) ve bit işlevi (bitf) aynen kullanılır.
//
// D_B(x) = argmax { I_Λ(λ) − c(λ) − c(Λ) : |Λ| ≤ 20, Λ ⊆ sağlanan(x, λ), B(x, λ, Λ) }, B = kapı G ∧ ≥ tmin tür ∧ ≥ dmin düzey.
// Arama (tam): kapı G'nin tuttuğu her mercekte, her grubun sağlanan adayları üzerinde bütün alt kümeler taranır ve her
// (boy j, kapsam imzası σ) kovası için en iyi b_g(S) − Σc(π) tutulur (σ: 4 bit düzey | 4 bit tür). Gruplar B, Z, F, W, H sırasıyla
// (toplam boy J, birleşik kapsam σ) durumları üzerinde dinamik programlamayla tam olarak birleştirilir. Amaç log₂(J!) − log₂21 − c(λ)
// terimiyle yalnız J'ye bağlıdır ve B yalnız σ'ya bağlıdır; bu yüzden kova başına en iyiyi tutmak kesindir.
// Eşitlik bozma (plasebo12 ile aynı; 1e-9 bit): küçük |Λ|, sonra küçük mercek indeksi, sonra sıralı global indeks listesi sözlükte
// küçük. Kova ve durum içi eşitliklerde de sözlükte küçük liste tutulur (aynı boylu listeler; ayrıntı ONKAYIT §4.3).
#ifndef DIL_SEC_B_H
#define DIL_SEC_B_H
#include "dil_sec.h"
#define KSIG 256
static inline uint8_t imza(int g){ return (uint8_t)((1u<<PDUZEY[g])|(1u<<(4+PTUR[g]))); }      // düzey: bit 0–3, tür: bit 4–7
static inline int kapsam_tamam(uint8_t s, int tmin, int dmin){ return __builtin_popcount(s&15u)>=dmin && __builtin_popcount((unsigned)s>>4)>=tmin; }
typedef struct { double v; uint32_t S; } kova_t;
typedef struct { double v; int n; int g[DMAX]; uint32_t Sg[NGRUP]; double vg[NGRUP]; } durum_t;
typedef struct { int L, n, g[DMAX]; double amac, bilgi, bedel, ig[NGRUP]; uint8_t sig; int nG, nB, asim; } dilb_t;
// G->A (üst-küme sayımları) ve G->C (bedel toplamları) fw_hazirla/yogun_hazirla'dan sonra geçerlidir (genum_tara bütün S'yi doldurur).
static void kova_tara(const genum_t *G, uint64_t n, int grup_ofs, kova_t K[DMAX+1][KSIG], uint8_t *SG){
  for(int j=0;j<=DMAX;j++) for(int s=0;s<KSIG;s++){ K[j][s].v=-INFINITY; K[j][s].S=0; }
  K[0][0].v=0.0; K[0][0].S=0; uint8_t sig[GADAY]; for(int b=0;b<G->m;b++) sig[b]=imza(grup_ofs+G->pos[b]);
  const uint32_t top=1u<<G->m; SG[0]=0;
  for(uint32_t S=1;S<top;S++){ const int b=__builtin_ctz(S); SG[S]=SG[S&(S-1)]|sig[b]; const int j=__builtin_popcount(S); if(j>DMAX) continue;
    const double v=bitf(G->A[S],n)-G->C[S]; kova_t *k=&K[j][SG[S]];
    if(v>k->v+ETOL){ k->v=v; k->S=S; }
    else if(v>=k->v-ETOL){ const uint32_t d=S^k->S; if(S&(d&(~d+1))){ k->v=v; k->S=S; } } } }
// sıralı listeler (aynı boy): a, b'den sözlükte küçük mü
static inline int esboy_kucuk(const int *a, const int *b, int n){ for(int i=0;i<n;i++) if(a[i]!=b[i]) return a[i]<b[i]; return 0; }
#define NDUR ((DMAX+1)*KSIG)
typedef struct { kova_t K[NGRUP][DMAX+1][KSIG]; durum_t D[2][DMAX+1][KSIG]; uint8_t *SG; uint16_t kl[NDUR], al[2][NDUR]; } dilb_tampon_t;
static dilb_tampon_t *dilb_ayir(void){ dilb_tampon_t *B=calloc(1,sizeof(dilb_tampon_t)); if(!B){ fprintf(stderr,"bellek (dilb)\n"); exit(2); }
  B->SG=malloc((size_t)1<<GADAY); if(!B->SG){ fprintf(stderr,"bellek (dilb)\n"); exit(2); }
  for(int i=0;i<2;i++) for(int J=0;J<=DMAX;J++) for(int z=0;z<KSIG;z++) B->D[i][J][z].v=-INFINITY; return B; }
// D_B(x): tmin, dmin = en az tür ve düzey sayısı (Kol A: 3, 3; A−: 2, 2; A+: 4, 4). Dil yoksa d->L = −1, d->n = 0.
static void dil_bul_b(const atablo_t *T, const prof_t *P, int tmin, int dmin, genum_t *G, dilb_tampon_t *BT, dilb_t *d){
  d->amac=-INFINITY; d->n=0; d->L=-1; d->sig=0; d->nG=0; d->nB=0; d->asim=0; for(int k=0;k<NGRUP;k++) d->ig[k]=0; d->bilgi=0; d->bedel=0;
  const double lg21=log2(21.0);
  for(int L=0;L<NLENS;L++){ if(!kapiG(P,L)) continue; d->nG++;
    const int kb=kkutu(P->kF[L]), hb=kkutu(P->kH[L]); const int cf=T->cF[L][kb], ch=T->cH[L][hb];
    const uint64_t s[NGRUP]={(uint64_t)P->B[L]&LMASK[G_B],(uint64_t)P->Z[L]&LMASK[G_Z],P->F[L]&LMASK[G_F],P->W[L]&LMASK[G_W],(uint64_t)P->H[L]&LMASK[G_H]};
    for(int k=0;k<NGRUP;k++) if(__builtin_popcountll(s[k])>GADAY) d->asim=1;
    // gruplar: aday kümeleri, üst-küme sayımları, kovalar (A/C tamponu paylaşıldığı için her gruptan hemen sonra taranır)
    yogun_hazirla(T->BA[L],T->N,s[G_B],&G[G_B],GOFS[G_B]); kova_tara(&G[G_B],T->N,GOFS[G_B],BT->K[G_B],BT->SG);
    yogun_hazirla(T->ZA[L],T->N,s[G_Z],&G[G_Z],GOFS[G_Z]); kova_tara(&G[G_Z],T->N,GOFS[G_Z],BT->K[G_Z],BT->SG);
    fw_hazirla(T,L,cf,0,s[G_F],&G[G_F],GOFS[G_F]); kova_tara(&G[G_F],T->nnF[L][cf],GOFS[G_F],BT->K[G_F],BT->SG);
    fw_hazirla(T,L,cf,1,s[G_W],&G[G_W],GOFS[G_W]); kova_tara(&G[G_W],T->nnF[L][cf],GOFS[G_W],BT->K[G_W],BT->SG);
    yogun_hazirla(T->HA+((size_t)L*NCX+ch)*128,T->nnH[L][ch],s[G_H],&G[G_H],GOFS[G_H]); kova_tara(&G[G_H],T->nnH[L][ch],GOFS[G_H],BT->K[G_H],BT->SG);
    // dinamik programlama: (J, σ) durumları; etkin durum ve dolu kova listeleri (durum indeksi J*KSIG + σ)
    durum_t (*cur)[KSIG]=BT->D[0], (*nxt)[KSIG]=BT->D[1]; uint16_t *acur=BT->al[0], *anxt=BT->al[1]; int nacur=1, nanxt=0;
    cur[0][0].v=0.0; cur[0][0].n=0; for(int k=0;k<NGRUP;k++){ cur[0][0].Sg[k]=0; cur[0][0].vg[k]=0.0; } acur[0]=0;
    for(int k=0;k<NGRUP;k++){ const int mk=G[k].m<DMAX?G[k].m:DMAX; int nkl=0;
      for(int j=0;j<=mk;j++) for(int y=0;y<KSIG;y++) if(BT->K[k][j][y].v!=-INFINITY) BT->kl[nkl++]=(uint16_t)(j*KSIG+y);
      nanxt=0;
      for(int ia=0;ia<nacur;ia++){ const int J=acur[ia]/KSIG, z=acur[ia]%KSIG; const durum_t *u=&cur[J][z];
        for(int ik=0;ik<nkl;ik++){ const int j=BT->kl[ik]/KSIG, y=BT->kl[ik]%KSIG; if(J+j>DMAX) continue; const kova_t *q=&BT->K[k][j][y];
          const double v=u->v+q->v; const int J2=J+j, z2=z|y; durum_t *w=&nxt[J2][z2];
          int gl[DMAX]; memcpy(gl,u->g,sizeof(int)*J); { uint32_t S=q->S; int t=J; while(S){ int b=__builtin_ctz(S); S&=S-1; gl[t++]=GOFS[k]+G[k].pos[b]; } }
          int al=0; if(w->v==-INFINITY){ al=1; anxt[nanxt++]=(uint16_t)(J2*KSIG+z2); } else if(v>w->v+ETOL) al=1; else if(v>=w->v-ETOL && esboy_kucuk(gl,w->g,J2)) al=1;
          if(al){ w->v=v; w->n=J2; memcpy(w->g,gl,sizeof(int)*J2); memcpy(w->Sg,u->Sg,sizeof w->Sg); memcpy(w->vg,u->vg,sizeof w->vg); w->Sg[k]=q->S; w->vg[k]=q->v; } } }
      for(int ia=0;ia<nacur;ia++){ cur[acur[ia]/KSIG][acur[ia]%KSIG].v=-INFINITY; }      // eski durumları temizle
      { durum_t (*t)[KSIG]=cur; cur=nxt; nxt=t; uint16_t *ta=acur; acur=anxt; anxt=ta; nacur=nanxt; } }
    const double cl=LBEDEL[0][L]; int bvar=0;
    for(int ia=0;ia<nacur;ia++){ const int J=acur[ia]/KSIG, z=acur[ia]%KSIG; const durum_t *u=&cur[J][z];
      if(J<1 || u->v==-INFINITY || !kapsam_tamam((uint8_t)z,tmin,dmin)) continue; bvar=1;
      const double v=u->v+lgamma((double)J+1.0)/log(2.0)-lg21-cl;
      int esit=0; if(v>d->amac+ETOL) esit=0; else if(v>=d->amac-ETOL) esit=1; else continue;
      if(esit){ if(J>d->n) continue; if(J==d->n){ if(L>d->L) continue; if(L==d->L && !liste_kucuk(u->g,J,d->g,d->n)) continue; } }
      d->amac=v; d->L=L; d->n=J; memcpy(d->g,u->g,sizeof(int)*J); d->sig=(uint8_t)z; double bilgi=0, bed=0;
      for(int k=0;k<NGRUP;k++){ double c=0; for(int b=0;b<G[k].m;b++) if(u->Sg[k]>>b&1) c+=G[k].cc[b]; d->ig[k]=u->vg[k]+c; bilgi+=d->ig[k]; bed+=c; }
      d->bilgi=bilgi; d->bedel=lg21+bed-lgamma((double)J+1.0)/log(2.0); }
    for(int ia=0;ia<nacur;ia++){ cur[acur[ia]/KSIG][acur[ia]%KSIG].v=-INFINITY; }        // son durumları temizle (sonraki mercek)
    d->nB+=bvar; }
  if(d->L<0){ d->amac=-INFINITY; d->n=0; } }

// Kol B: bedelsiz en geniş dil. Her mercekte sağlanan bütün L* yüklemlerinin bilgisi I(λ) (grup içi tam birlikte en-az sayısı) eksi
// mercek bedeli; en büyüğünü veren mercekte (eşitlikte küçük indeks) sağlanan L* yüklemlerinin hepsi dildir. Boy sınırı yoktur.
#define DMAXG 64
typedef struct { int L, n, g[DMAXG]; double puan, bilgi, ig[NGRUP]; uint64_t c[NGRUP], nn[NGRUP]; } dilg_t;
static uint64_t fw_ensay(const atablo_t *T, int L, int cf, int g, uint64_t s){ uint64_t c=0;
  for(int kb=0;kb<KC;kb++){ if(T->cF[L][kb]!=cf) continue; const uint32_t lk=LKG(L,kb,g); for(uint64_t e=T->bas[lk];e<T->bas[lk+1];e++) if((T->E[e].mask&s)==s) c+=T->E[e].c; }
  return c; }
static double genis_mercek(const atablo_t *T, const prof_t *P, int L, double ig[NGRUP], uint64_t cc[NGRUP], uint64_t nn[NGRUP]){
  const int cf=T->cF[L][kkutu(P->kF[L])], ch=T->cH[L][kkutu(P->kH[L])];
  const uint64_t s[NGRUP]={(uint64_t)P->B[L]&LMASK[G_B],(uint64_t)P->Z[L]&LMASK[G_Z],P->F[L]&LMASK[G_F],P->W[L]&LMASK[G_W],(uint64_t)P->H[L]&LMASK[G_H]};
  double b=0; for(int k=0;k<NGRUP;k++){ ig[k]=0; cc[k]=0; nn[k]=0; }
  if(s[0]){ cc[0]=T->BA[L][s[0]]; nn[0]=T->N; ig[0]=bitf(cc[0],nn[0]); }
  if(s[1]){ cc[1]=T->ZA[L][s[1]]; nn[1]=T->N; ig[1]=bitf(cc[1],nn[1]); }
  if(s[2]){ cc[2]=fw_ensay(T,L,cf,0,s[2]); nn[2]=T->nnF[L][cf]; ig[2]=bitf(cc[2],nn[2]); }
  if(s[3]){ cc[3]=fw_ensay(T,L,cf,1,s[3]); nn[3]=T->nnF[L][cf]; ig[3]=bitf(cc[3],nn[3]); }
  if(s[4]){ cc[4]=T->HA[((size_t)L*NCX+ch)*128+s[4]]; nn[4]=T->nnH[L][ch]; ig[4]=bitf(cc[4],nn[4]); }
  for(int k=0;k<NGRUP;k++) b+=ig[k]; return b; }
static void dil_genis(const atablo_t *T, const prof_t *P, dilg_t *d){
  d->puan=-INFINITY; d->L=-1; d->n=0; double ig[NGRUP]; uint64_t cc[NGRUP], nn[NGRUP];
  for(int L=0;L<NLENS;L++){ const double b=genis_mercek(T,P,L,ig,cc,nn); const double v=b-LBEDEL[0][L];
    if(v>d->puan+ETOL){ d->puan=v; d->L=L; d->bilgi=b; memcpy(d->ig,ig,sizeof ig); memcpy(d->c,cc,sizeof cc); memcpy(d->nn,nn,sizeof nn); } }
  const int L=d->L; const uint64_t s[NGRUP]={(uint64_t)P->B[L]&LMASK[G_B],(uint64_t)P->Z[L]&LMASK[G_Z],P->F[L]&LMASK[G_F],P->W[L]&LMASK[G_W],(uint64_t)P->H[L]&LMASK[G_H]};
  for(int k=0;k<NGRUP;k++){ uint64_t x=s[k]; while(x){ int b=__builtin_ctzll(x); x&=x-1; if(d->n>=DMAXG){ fprintf(stderr,"en geniş dil DMAXG'yi aşıyor\n"); exit(2); } d->g[d->n++]=GOFS[k]+b; } } }
#endif
