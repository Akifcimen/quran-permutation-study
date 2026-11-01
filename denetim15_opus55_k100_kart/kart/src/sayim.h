// sayim.h — referans sayımlarının yapıları (ONKAYIT §3.1): tam kütüphane maskelerinin seyrek sayımı ve yoğun tablolar.
// F ve W grupları: (mercek, k_F kutusu, grup, tam maske) → sayı, seyrek karma tablo; dosyada (lkg, maske) sıralı dizi.
// B: mercek başına 7 bitlik maske sayıları; H: (mercek, k_H kutusu) başına 7 bitlik maske sayıları; NK, NKH: kesin k_F, k_H.
#ifndef SAYIM_H
#define SAYIM_H
#include "kutuphane.h"
#define KC 80                                  // bağlam kutuları 0..79 (k ≥ 79 → 79), 12. denetimle aynı
#define KX 115                                 // kesin k 0..114 (Z grubu)
static inline int kkutu(int k){ return k<KC-1?k:KC-1; }
typedef struct { uint64_t mask; uint32_t lkg; uint32_t c; } ent_t;
static inline uint32_t LKG(int L,int kb,int g){ return ((uint32_t)L*KC+(uint32_t)kb)*2u+(uint32_t)g; }   // g: 0 = F, 1 = W
typedef struct { ent_t *e; uint64_t cap, n; } karma_t;
static inline uint64_t hkar(uint64_t m, uint32_t lkg){ uint64_t x=m^((uint64_t)lkg*0x9E3779B97F4A7C15ULL)^((uint64_t)lkg<<40);
  x^=x>>33; x*=0xff51afd7ed558ccdULL; x^=x>>33; x*=0xc4ceb9fe1a85ec53ULL; x^=x>>33; return x; }
static void k_ayir(karma_t *H, uint64_t cap){ H->cap=cap; H->n=0; H->e=calloc(cap,sizeof(ent_t)); if(!H->e){ fprintf(stderr,"bellek (karma)\n"); exit(2); } }
static void k_ekle(karma_t *H, uint64_t m, uint32_t lkg, uint32_t c);
static void k_buyut(karma_t *H){ karma_t Y; k_ayir(&Y,H->cap*2); for(uint64_t i=0;i<H->cap;i++) if(H->e[i].c) k_ekle(&Y,H->e[i].mask,H->e[i].lkg,H->e[i].c);
  free(H->e); *H=Y; }
static void k_ekle(karma_t *H, uint64_t m, uint32_t lkg, uint32_t c){
  uint64_t h=hkar(m,lkg)&(H->cap-1);
  for(;;){ ent_t *E=&H->e[h]; if(E->c==0){ E->mask=m; E->lkg=lkg; E->c=c; H->n++; if(H->n*10>H->cap*6) k_buyut(H); return; }
    if(E->mask==m && E->lkg==lkg){ uint64_t s=(uint64_t)E->c+c; if(s>0xFFFFFFFFull){ fprintf(stderr,"taşma\n"); exit(2); } E->c=(uint32_t)s; return; }
    h=(h+1)&(H->cap-1); } }
typedef struct { uint64_t N; uint32_t BC[NLENS][128]; uint32_t NK[NLENS][KX], NKH[NLENS][KX]; uint32_t *HC; karma_t FW; } ref_t;   // HC[(L*KC+kb)*128+m]
static void ref_ayir(ref_t *R, uint64_t cap){ memset(R,0,sizeof *R); R->HC=calloc((size_t)NLENS*KC*128,4); k_ayir(&R->FW,cap); }
static inline void ref_ekle(ref_t *R, const prof_t *P, uint32_t fbit){
  for(int f=0;f<NFORM;f++){ if(!(fbit>>f&1)) continue;
    for(int i=0;i<NPR;i++){ const int L=f*NPR+i; const int kF=P->kF[L], kH=P->kH[L]; const int kb=kkutu(kF), hb=kkutu(kH);
      R->BC[L][P->B[L]]++; R->NK[L][kF]++; R->NKH[L][kH]++; R->HC[((size_t)L*KC+hb)*128+P->H[L]]++;
      k_ekle(&R->FW,P->F[L],LKG(L,kb,0),1); k_ekle(&R->FW,P->W[L],LKG(L,kb,1),1); } }
  R->N++; }
// dosya biçimi: imza | N | BC | NK | NKH | HC | n_ent | ent[] ((lkg, mask) sıralı)
#define REF_IMZA 0x31525050u
static int ent_kiyas(const void *a, const void *b){ const ent_t *x=a, *y=b; if(x->lkg!=y->lkg) return x->lkg<y->lkg?-1:1; if(x->mask!=y->mask) return x->mask<y->mask?-1:1; return 0; }
static void ref_yaz(const ref_t *R, const char *fn, uint64_t ek){   // ek: dosyaya yazılan ek bilgi (tamamlanan bölüt)
  char tmp[1100]; snprintf(tmp,sizeof tmp,"%s.yaziliyor",fn); FILE *f=fopen(tmp,"wb"); if(!f){ perror(tmp); exit(2); }
  uint32_t im=REF_IMZA; fwrite(&im,4,1,f); fwrite(&ek,8,1,f); fwrite(&R->N,8,1,f); fwrite(R->BC,4,(size_t)NLENS*128,f); fwrite(R->NK,4,(size_t)NLENS*KX,f);
  fwrite(R->NKH,4,(size_t)NLENS*KX,f); fwrite(R->HC,4,(size_t)NLENS*KC*128,f);
  ent_t *E=malloc(sizeof(ent_t)*(R->FW.n?R->FW.n:1)); uint64_t n=0; for(uint64_t i=0;i<R->FW.cap;i++) if(R->FW.e[i].c) E[n++]=R->FW.e[i];
  qsort(E,n,sizeof(ent_t),ent_kiyas); fwrite(&n,8,1,f); fwrite(E,sizeof(ent_t),n,f); free(E);
  if(fclose(f)!=0){ perror("fclose"); exit(2); } if(rename(tmp,fn)!=0){ perror("rename"); exit(2); } }
static int ref_oku(ref_t *R, const char *fn, uint64_t *ek){   // R ayrılmış olmalı; sayımlar R'ye EKLENİR (biçim kümeleri ayrık);
                                                              // N bütün dosyalarda aynı olmalı (aynı kitap akışı)
  FILE *f=fopen(fn,"rb"); if(!f) return 0; uint32_t im; uint64_t N, e;
  if(fread(&im,4,1,f)!=1||im!=REF_IMZA||fread(&e,8,1,f)!=1||fread(&N,8,1,f)!=1){ fclose(f); return 0; }
  static uint32_t BC[NLENS][128], NK[NLENS][KX], NKH[NLENS][KX]; uint32_t *HC=malloc((size_t)NLENS*KC*128*4);
  int ok=fread(BC,4,(size_t)NLENS*128,f)==(size_t)NLENS*128 && fread(NK,4,(size_t)NLENS*KX,f)==(size_t)NLENS*KX && fread(NKH,4,(size_t)NLENS*KX,f)==(size_t)NLENS*KX
       && fread(HC,4,(size_t)NLENS*KC*128,f)==(size_t)NLENS*KC*128;
  uint64_t n=0; if(ok && fread(&n,8,1,f)!=1) ok=0;
  if(ok){ ent_t buf[4096]; uint64_t kal=n; while(kal){ size_t b=kal>4096?4096:(size_t)kal; if(fread(buf,sizeof(ent_t),b,f)!=b){ ok=0; break; }
      for(size_t j=0;j<b;j++) k_ekle(&R->FW,buf[j].mask,buf[j].lkg,buf[j].c); kal-=b; } }
  fclose(f); if(!ok){ free(HC); return 0; }
  if(R->N==0) R->N=N; else if(R->N!=N){ fprintf(stderr,"N uyuşmazlığı: %llu / %llu (%s)\n",(unsigned long long)R->N,(unsigned long long)N,fn); exit(2); }
  for(int L=0;L<NLENS;L++){ for(int m=0;m<128;m++) R->BC[L][m]+=BC[L][m]; for(int k=0;k<KX;k++){ R->NK[L][k]+=NK[L][k]; R->NKH[L][k]+=NKH[L][k]; } }
  for(size_t j=0;j<(size_t)NLENS*KC*128;j++) R->HC[j]+=HC[j]; free(HC); if(ek) *ek=e; return 1; }
#endif
