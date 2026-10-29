// tablo.h — analiz tarafı (ONKAYIT §3.2–3.4): referans dosyalarını birleştirme, bağlamlar, üst-küme ("en az") sayımları,
// bit işlevi ve mercek bedelleri. F ve W için seyrek tam-maske listeleri (lkg, maske) sıralı tutulur; bir dilin ya da bir hedefin
// ihtiyacı olan sayımlar bu listelerin izdüşümü ve üst-küme toplamıyla elde edilir.
#ifndef TABLO_H
#define TABLO_H
#include "sayim.h"
#include "agac_uretim.h"
#define NMIN 10000                     // çekirdek bağlam: kutunun referans sayısı ≥ NMIN (12. denetimle aynı)
#define NCX 88                         // mercek başına en çok bağlam (80 çekirdek + 4 alt + 4 üst)
#define T_COZ 10
typedef struct {
  uint64_t N;
  int16_t cF[NLENS][KC], cH[NLENS][KC];                 // (mercek, k kutusu) → mercek-yerel bağlam numarası
  int nF[NLENS], nH[NLENS];
  uint64_t nnF[NLENS][NCX], nnH[NLENS][NCX];           // bağlam büyüklükleri
  uint64_t BA[NLENS][128], ZA[NLENS][16];              // kitap ve boy grupları: üst-küme sayımları
  uint64_t *HA;                                        // H: [(L*NCX+ctx)*128 + m] üst-küme sayımları
  ent_t *E; uint64_t nE; uint64_t *bas;                // F/W seyrek girdiler; bas[lkg]..bas[lkg+1]
  uint32_t NK[NLENS][KX], NKH[NLENS][KX];
} atablo_t;
static inline double bitf(uint64_t c, uint64_t n){ if(n==0) return 0.0; if(c>=T_COZ) return log2((double)n)-log2((double)c); return log2((double)n+1.0)-log2((double)c+0.5); }
static double LBEDEL[2][NLENS];                        // mercek bedeli: 0 = P0 (birincil), 1 = P2 (yalnız Kur'an ek satırı)
static void mercek_bedeli_hazirla(void){
  for(int f=0;f<NFORM;f++){ double s=0; for(int g=0;g<NFORM;g++) s+=pow(2.0,-(AL[g]+abs(BE[g])-2)); const double bb=-log2(pow(2.0,-(AL[f]+abs(BE[f])-2))/s);
    for(int i=0;i<NPR;i++){ const int p=PR[i]; double u=1.0/87, l=1.0/15, d=1.0/12, q=u; if(p==3||p==5||p==7||p==19) q=u+l+d; else if(p==11) q=u+l;
      LBEDEL[0][f*NPR+i]=bb+log2(29.0); LBEDEL[1][f*NPR+i]=bb-log2(q); } } }
static int ent_kiyas2(const void *a, const void *b){ const ent_t *x=a, *y=b; if(x->lkg!=y->lkg) return x->lkg<y->lkg?-1:1; if(x->mask!=y->mask) return x->mask<y->mask?-1:1; return 0; }
static void baglam_kur(const uint32_t nk[KC], int16_t *c, int *nc){   // 12. denetimin tablo_kur kuralı
  int mod=0; for(int k=1;k<KC;k++) if(nk[k]>nk[mod]) mod=k;
  int lo=mod, hi=mod; if(nk[mod]>=NMIN){ while(lo>0 && nk[lo-1]>=NMIN) lo--; while(hi<KC-1 && nk[hi+1]>=NMIN) hi++; } else { lo=mod; hi=mod-1; }
  int alt[4], ust[4], n=0; for(int r=0;r<4;r++){ alt[r]=-1; ust[r]=-1; }
  for(int k=0;k<KC;k++){ int x; if(k>=lo && k<=hi) x=n++; else if(k<lo){ if(alt[k&3]<0) alt[k&3]=n++; x=alt[k&3]; } else { if(ust[k&3]<0) ust[k&3]=n++; x=ust[k&3]; } c[k]=(int16_t)x; }
  *nc=n; }
static inline uint8_t zmaske(int k, int p, int L){ uint8_t m=0; const int pp=p*p;
  if(k==p) m|=1u<<Z_QkF; if(k==2*p) m|=1u<<Z_k2p; if(k>=1 && k%p==0) m|=1u<<Z_kd; if(k>=1 && k%pp==0) m|=1u<<Z_kd2; return m&~EXZ[L]; }
// Referans dosyalarını (biçim kümeleri ayrık; aynı kitap akışı) birleştir ve analiz tablolarını kur.
static void atablo_kur(atablo_t *T, char **dosya, int nd){
  static uint32_t BC[NLENS][128]; uint32_t *HC=calloc((size_t)NLENS*KC*128,4); memset(BC,0,sizeof BC); memset(T->NK,0,sizeof T->NK); memset(T->NKH,0,sizeof T->NKH);
  uint64_t cap=1<<20, n=0; T->E=malloc(sizeof(ent_t)*cap); T->N=0;
  uint32_t *tmp=malloc((size_t)NLENS*KC*128*4); static uint32_t b2[NLENS][128], k2[NLENS][KX], h2[NLENS][KX];
  for(int i=0;i<nd;i++){ FILE *f=fopen(dosya[i],"rb"); if(!f){ fprintf(stderr,"referans yok: %s\n",dosya[i]); exit(2); }
    uint32_t im; uint64_t e, N, ne;
    if(fread(&im,4,1,f)!=1||im!=REF_IMZA||fread(&e,8,1,f)!=1||fread(&N,8,1,f)!=1){ fprintf(stderr,"başlık: %s\n",dosya[i]); exit(2); }
    if(T->N==0) T->N=N; else if(T->N!=N){ fprintf(stderr,"N uyuşmazlığı: %s\n",dosya[i]); exit(2); }
    if(fread(b2,4,(size_t)NLENS*128,f)!=(size_t)NLENS*128||fread(k2,4,(size_t)NLENS*KX,f)!=(size_t)NLENS*KX||fread(h2,4,(size_t)NLENS*KX,f)!=(size_t)NLENS*KX
       ||fread(tmp,4,(size_t)NLENS*KC*128,f)!=(size_t)NLENS*KC*128||fread(&ne,8,1,f)!=1){ fprintf(stderr,"gövde: %s\n",dosya[i]); exit(2); }
    for(int L=0;L<NLENS;L++){ for(int m=0;m<128;m++) BC[L][m]+=b2[L][m]; for(int k=0;k<KX;k++){ T->NK[L][k]+=k2[L][k]; T->NKH[L][k]+=h2[L][k]; } }
    for(size_t j=0;j<(size_t)NLENS*KC*128;j++) HC[j]+=tmp[j];
    while(n+ne>cap){ cap*=2; T->E=realloc(T->E,sizeof(ent_t)*cap); }
    if(fread(T->E+n,sizeof(ent_t),ne,f)!=ne){ fprintf(stderr,"girdiler: %s\n",dosya[i]); exit(2); } n+=ne; fclose(f); }
  free(tmp);
  for(int L=0;L<NLENS;L++){ uint32_t nk[KC], nh[KC]; memset(nk,0,sizeof nk); memset(nh,0,sizeof nh);
    for(int k=0;k<KX;k++){ nk[kkutu(k)]+=T->NK[L][k]; nh[kkutu(k)]+=T->NKH[L][k]; }
    baglam_kur(nk,T->cF[L],&T->nF[L]); baglam_kur(nh,T->cH[L],&T->nH[L]);
    for(int c=0;c<NCX;c++){ T->nnF[L][c]=0; T->nnH[L][c]=0; }
    for(int k=0;k<KC;k++){ T->nnF[L][T->cF[L][k]]+=nk[k]; T->nnH[L][T->cH[L][k]]+=nh[k]; }
    for(int S=0;S<128;S++){ uint64_t s=0; for(int m=0;m<128;m++) if((m&S)==S) s+=BC[L][m]; T->BA[L][S]=s; }
    const int p=PR[L%NPR];
    for(int S=0;S<16;S++){ uint64_t s=0; for(int k=0;k<KX;k++) if((zmaske(k,p,L)&S)==S) s+=T->NK[L][k]; T->ZA[L][S]=s; } }
  T->HA=calloc((size_t)NLENS*NCX*128,8);
  for(int L=0;L<NLENS;L++) for(int kb=0;kb<KC;kb++){ const int c=T->cH[L][kb]; uint64_t *h=T->HA+((size_t)L*NCX+c)*128; const uint32_t *src=HC+((size_t)L*KC+kb)*128;
    for(int m=0;m<128;m++) h[m]+=src[m]; }
  for(int L=0;L<NLENS;L++) for(int c=0;c<T->nH[L];c++){ uint64_t *h=T->HA+((size_t)L*NCX+c)*128; for(int b=0;b<7;b++) for(int m=0;m<128;m++) if(!(m>>b&1)) h[m]+=h[m|(1<<b)]; }
  free(HC);
  qsort(T->E,n,sizeof(ent_t),ent_kiyas2); T->nE=n;
  for(uint64_t j=1;j<n;j++) if(T->E[j].lkg==T->E[j-1].lkg && T->E[j].mask==T->E[j-1].mask){ fprintf(stderr,"yinelenen girdi (biçim kümeleri ayrık değil?)\n"); exit(2); }
  const uint32_t NLKG=(uint32_t)NLENS*KC*2; T->bas=calloc(NLKG+1,8); { uint64_t j=0; for(uint32_t g=0;g<=NLKG;g++){ while(j<n && T->E[j].lkg<g) j++; T->bas[g]=j; } }
}
#endif
