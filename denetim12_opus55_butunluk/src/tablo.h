// tablo.h — bilgi-ağırlıklı puanın sıklık tabloları (ONKAYIT §3).
// Referans örnekleminden: aile boyu kF'ye koşullu aile-içi maske (11 bit) ve iki-el maskesi (6 bit) KESİN maske sayıları,
// biçim başına kitap maskesi (2 bit), mercek başına kF dağılımı. Arama: "en az" (üst-küme toplamı) sayıları; bağlam = kF
// (çekirdek aralıkta kesin kF, kuyruklarda kF mod 4 havuzu). Çözülmemiş hücre: en-az sayısı < T_COZ → Poisson–CP sınırları.
#ifndef TABLO_H
#define TABLO_H
#include "butunluk.h"
#define KC 80                       // kF kutuları 0..79 (kF ≥ 79 → 79)
#define NFM 2048
#define NWM 64
#define T_COZ 10                    // çözülmüş: en-az sayısı ≥ 10
static inline int kkutu(int k){ return k<KC-1?k:KC-1; }
static inline size_t FIDX(int L,int k,uint32_t m){ return ((size_t)L*KC+k)*NFM+m; }
static inline size_t WIDX(int L,int k,uint32_t m){ return ((size_t)L*KC+k)*NWM+m; }

// GH: kapı G'yi geçen (kF ≥ 2E ∧ her hücre ≥ 2) mercek-kitapların aile maskesi sayıları — seyrek karma (sayım puanının
// model-tabanlı kuyruğu için; ONKAYIT §6.2). Anahtar = ((L·KC + k) << 12) | (aile maskesi 11 bit | QkF << 11); bellekte +1 (0 = boş).
#define GH_BIT 22
typedef struct { uint64_t *anahtar; uint32_t *sayi; uint64_t n; } karma_t;
static void karma_ayir(karma_t *H){ H->anahtar=calloc(1ull<<GH_BIT,8); H->sayi=calloc(1ull<<GH_BIT,4); H->n=0; }
static inline void karma_ekle(karma_t *H, uint64_t key, uint32_t c){ key+=1; uint64_t h=(key*0x9E3779B97F4A7C15ULL)>>(64-GH_BIT);
  for(;;){ if(H->anahtar[h]==key){ H->sayi[h]+=c; return; } if(H->anahtar[h]==0){ H->anahtar[h]=key; H->sayi[h]=c; H->n++;
      if(H->n>(1ull<<(GH_BIT-1))){ fprintf(stderr,"karma doldu\n"); exit(2); } return; } h=(h+1)&((1ull<<GH_BIT)-1); } }
typedef struct { uint32_t *FC, *WC, *NK; uint64_t BC[NFORM][4]; uint64_t N; karma_t GH; } ref_t;
static void ref_ayir(ref_t *R){ R->FC=calloc((size_t)NLENS*KC*NFM,4); R->WC=calloc((size_t)NLENS*KC*NWM,4); R->NK=calloc((size_t)NLENS*KC,4);
  memset(R->BC,0,sizeof R->BC); R->N=0; karma_ayir(&R->GH); if(!R->FC||!R->WC||!R->NK||!R->GH.anahtar){ fprintf(stderr,"bellek\n"); exit(2); } }
static inline void ref_ekle(ref_t *R, const profil_t *P){
  for(int L=0;L<NLENS;L++){ const uint32_t m=P->m[L]; const int k=kkutu(P->kF[L]);
    R->FC[FIDX(L,k,fmask11(m))]++; R->WC[WIDX(L,k,wmask6(m))]++; R->NK[(size_t)L*KC+k]++;
    if(kapi_tut(P,L,0)) karma_ekle(&R->GH,((((uint64_t)L*KC+k))<<12)|(fmask11(m)|(((m>>P_QkF)&1u)<<11)),1); }
  for(int f=0;f<NFORM;f++) R->BC[f][bmask2(P->m[f*NPR])]++;
  R->N++; }
// seyrek yazım: başlık | N | BC | (indeks u32, sayı u32)* ayrı bölümler F, W, NK
static void seyrek_yaz_dizi(FILE *fo, const uint32_t *A, size_t n){ uint64_t nz=0; for(size_t i=0;i<n;i++) if(A[i]) nz++; fwrite(&nz,8,1,fo);
  for(size_t i=0;i<n;i++) if(A[i]){ uint32_t ix=(uint32_t)i; fwrite(&ix,4,1,fo); fwrite(&A[i],4,1,fo); } }
static int seyrek_oku_dizi(FILE *fi, uint32_t *A, size_t n){ uint64_t nz; if(fread(&nz,8,1,fi)!=1) return 0;
  for(uint64_t j=0;j<nz;j++){ uint32_t ix,c; if(fread(&ix,4,1,fi)!=1||fread(&c,4,1,fi)!=1) return 0; if(ix>=n) return 0;
    uint64_t s=(uint64_t)A[ix]+c; if(s>0xFFFFFFFFull){ fprintf(stderr,"taşma\n"); exit(2); } A[ix]=(uint32_t)s; } return 1; }
#define REF_IMZA 0x42544C53u   // "SLTB" (GH bölümlü sürüm)
static void ref_yaz(const ref_t *R, const char *fn){ char tmp[1024]; snprintf(tmp,1024,"%s.yaziliyor",fn); FILE *fo=fopen(tmp,"wb");
  uint32_t im=REF_IMZA, kod=(uint32_t)KODEKS; fwrite(&im,4,1,fo); fwrite(&kod,4,1,fo); fwrite(&R->N,8,1,fo); fwrite(R->BC,8,NFORM*4,fo);
  seyrek_yaz_dizi(fo,R->FC,(size_t)NLENS*KC*NFM); seyrek_yaz_dizi(fo,R->WC,(size_t)NLENS*KC*NWM); seyrek_yaz_dizi(fo,R->NK,(size_t)NLENS*KC);
  fwrite(&R->GH.n,8,1,fo); for(uint64_t h=0;h<(1ull<<GH_BIT);h++) if(R->GH.anahtar[h]){ uint64_t k=R->GH.anahtar[h]-1; fwrite(&k,8,1,fo); fwrite(&R->GH.sayi[h],4,1,fo); }
  fclose(fo); rename(tmp,fn); }
static int ref_oku_ekle(ref_t *R, const char *fn){ FILE *fi=fopen(fn,"rb"); if(!fi) return 0; uint32_t im,kod; uint64_t N; uint64_t BC[NFORM][4];
  if(fread(&im,4,1,fi)!=1||im!=REF_IMZA||fread(&kod,4,1,fi)!=1||(int)kod!=KODEKS||fread(&N,8,1,fi)!=1||fread(BC,8,NFORM*4,fi)!=NFORM*4){ fclose(fi); return 0; }
  int ok=seyrek_oku_dizi(fi,R->FC,(size_t)NLENS*KC*NFM)&&seyrek_oku_dizi(fi,R->WC,(size_t)NLENS*KC*NWM)&&seyrek_oku_dizi(fi,R->NK,(size_t)NLENS*KC);
  uint64_t gn=0; if(ok && fread(&gn,8,1,fi)!=1) ok=0;
  for(uint64_t j=0;ok&&j<gn;j++){ uint64_t k; uint32_t c; if(fread(&k,8,1,fi)!=1||fread(&c,4,1,fi)!=1){ ok=0; break; } karma_ekle(&R->GH,k,c); }
  fclose(fi); if(!ok) return 0; R->N+=N; for(int f=0;f<NFORM;f++) for(int b=0;b<4;b++) R->BC[f][b]+=BC[f][b]; return 1; }

// ---------------- arama tablosu ----------------
#define NMIN 10000                  // çekirdek bağlam: kesin kF kutusunun referans sayısı ≥ NMIN
typedef struct { uint64_t N; int nctx; int32_t *ctx;          // ctx[L*KC+k] → bağlam
  uint64_t *cn; uint32_t *FA, *WA;                            // bağlam başına: n, F en-az (2048), W en-az (64)
  uint64_t BA[NFORM][4]; uint64_t *NKU;                         // biçim başına B en-az; mercek başına kF sayıları (koşulsuz)
} tablo_t;
static void ust_kume(uint32_t *A, int bits){ int n=1<<bits; for(int b=0;b<bits;b++) for(int m=0;m<n;m++) if(!(m>>b&1)) A[m]+=A[m|(1<<b)]; }
static void tablo_kur(const ref_t *R, tablo_t *T){
  T->N=R->N; T->ctx=malloc(sizeof(int32_t)*NLENS*KC); T->NKU=calloc((size_t)NLENS*KC,8);
  for(size_t i=0;i<(size_t)NLENS*KC;i++) T->NKU[i]=R->NK[i];
  // bağlamlar: her mercekte çekirdek = modu içeren, NK ≥ NMIN olan kesintisiz aralık; kuyruklar (alt/üst) kF mod 4 havuzları
  int nctx=0;
  for(int L=0;L<NLENS;L++){ const uint32_t *nk=R->NK+(size_t)L*KC; int mod=0; for(int k=1;k<KC;k++) if(nk[k]>nk[mod]) mod=k;
    int lo=mod, hi=mod; if(nk[mod]>=NMIN){ while(lo>0 && nk[lo-1]>=NMIN) lo--; while(hi<KC-1 && nk[hi+1]>=NMIN) hi++; } else { lo=mod; hi=mod-1; }
    int alt[4], ust[4]; for(int r=0;r<4;r++){ alt[r]=-1; ust[r]=-1; }
    for(int k=0;k<KC;k++){ int c;
      if(k>=lo && k<=hi) c=nctx++;
      else if(k<lo){ if(alt[k&3]<0) alt[k&3]=nctx++; c=alt[k&3]; }
      else { if(ust[k&3]<0) ust[k&3]=nctx++; c=ust[k&3]; }
      T->ctx[L*KC+k]=c; } }
  T->nctx=nctx; T->cn=calloc(nctx,8); T->FA=calloc((size_t)nctx*NFM,4); T->WA=calloc((size_t)nctx*NWM,4);
  for(int L=0;L<NLENS;L++) for(int k=0;k<KC;k++){ int c=T->ctx[L*KC+k]; T->cn[c]+=R->NK[(size_t)L*KC+k];
    uint32_t *fa=T->FA+(size_t)c*NFM; const uint32_t *fc=R->FC+FIDX(L,k,0); for(int m=0;m<NFM;m++) fa[m]+=fc[m];
    uint32_t *wa=T->WA+(size_t)c*NWM; const uint32_t *wc=R->WC+WIDX(L,k,0); for(int m=0;m<NWM;m++) wa[m]+=wc[m]; }
  for(int c=0;c<nctx;c++){ ust_kume(T->FA+(size_t)c*NFM,11); ust_kume(T->WA+(size_t)c*NWM,6); }
  for(int f=0;f<NFORM;f++){ uint64_t b[4]; for(int i=0;i<4;i++) b[i]=R->BC[f][i]; T->BA[f][0]=b[0]+b[1]+b[2]+b[3]; T->BA[f][1]=b[1]+b[3]; T->BA[f][2]=b[2]+b[3]; T->BA[f][3]=b[3]; }
  }
#define TAB_IMZA 0x42415454u
static void tablo_yaz(const tablo_t *T, const char *fn){ char tmp[1024]; snprintf(tmp,1024,"%s.yaziliyor",fn); FILE *fo=fopen(tmp,"wb");
  uint32_t im=TAB_IMZA, kod=(uint32_t)KODEKS; fwrite(&im,4,1,fo); fwrite(&kod,4,1,fo); fwrite(&T->N,8,1,fo); fwrite(&T->nctx,4,1,fo);
  fwrite(T->ctx,4,NLENS*KC,fo); fwrite(T->cn,8,T->nctx,fo); fwrite(T->FA,4,(size_t)T->nctx*NFM,fo); fwrite(T->WA,4,(size_t)T->nctx*NWM,fo);
  fwrite(T->BA,8,NFORM*4,fo); fwrite(T->NKU,8,(size_t)NLENS*KC,fo); fclose(fo); rename(tmp,fn); }
static int tablo_oku(tablo_t *T, const char *fn){ FILE *fi=fopen(fn,"rb"); if(!fi) return 0; uint32_t im,kod;
  if(fread(&im,4,1,fi)!=1||im!=TAB_IMZA||fread(&kod,4,1,fi)!=1||(int)kod!=KODEKS){ fclose(fi); return 0; }
  if(fread(&T->N,8,1,fi)!=1||fread(&T->nctx,4,1,fi)!=1){ fclose(fi); return 0; }
  T->ctx=malloc(4*NLENS*KC); T->cn=malloc(8*(size_t)T->nctx); T->FA=malloc(4*(size_t)T->nctx*NFM); T->WA=malloc(4*(size_t)T->nctx*NWM); T->NKU=malloc(8*(size_t)NLENS*KC);
  int ok = fread(T->ctx,4,NLENS*KC,fi)==(size_t)NLENS*KC && fread(T->cn,8,T->nctx,fi)==(size_t)T->nctx && fread(T->FA,4,(size_t)T->nctx*NFM,fi)==(size_t)T->nctx*NFM
        && fread(T->WA,4,(size_t)T->nctx*NWM,fi)==(size_t)T->nctx*NWM && fread(T->BA,8,NFORM*4,fi)==NFORM*4 && fread(T->NKU,8,(size_t)NLENS*KC,fi)==(size_t)NLENS*KC;
  fclose(fi); return ok; }

// ---------------- sınırlar ve yüzey (bit) ----------------
// Tek yanlı %95 Poisson–Clopper–Pearson: üst = χ²(0,95; 2c+2)/2, alt = χ²(0,05; 2c)/2 (n ≫ c yaklaşımı), c = 0..9.
static const double CP_UST[T_COZ]={2.995732,4.743865,6.295794,7.753657,9.153519,10.513035,11.842396,13.148114,14.434650,15.705216};
static const double CP_ALT[T_COZ]={0.0,0.051293,0.355362,0.817691,1.366318,1.970150,2.613015,3.285316,3.980823,4.695228};
typedef struct { double pt, lo, hi; int coz; } yuzey_t;      // pt = nokta (bit); lo/hi = puan alt/üst sınırına katkı (bit)
static inline yuzey_t yuzey(uint64_t c, uint64_t n){ yuzey_t y;
  if(n==0){ y.pt=y.lo=0; y.hi=INFINITY; y.coz=0; return y; }
  if(c>=T_COZ){ double v=log2((double)n)-log2((double)c); y.pt=y.lo=y.hi=v; y.coz=1; return y; }
  y.coz=0; y.lo=log2((double)n)-log2(CP_UST[c]); y.lo=y.lo>0?y.lo:0;          // olasılık üst sınırı → bit alt sınırı
  y.hi=(c==0)?INFINITY:log2((double)n)-log2(CP_ALT[c]);
  y.pt=log2((double)n+1.0)-log2((double)c+0.5); return y; }
#endif
