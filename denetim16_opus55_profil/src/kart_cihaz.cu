// kart_cihaz.cu — kart_ortak.h çekirdeğinin CUDA arka ucu (kart_emul.c ile aynı arayüz ve aynı sonuç).
// Bölüt başına bir iş parçacığı örnekleyiciyi sırayla yürütür (RNG akışı özgünle aynı); her kontrol bir warp'ta değerlendirilir:
// 14 şerit kitap düzeyini (biçim başına), 29 şerit aileleri (asal başına) hesaplar; puan şerit başına mercekler üzerinde, sonra warp
// indirgemesiyle (en büyük değer; eşitlikte en küçük mercek) bulunur. Sayaçlar bölüt × dil başına atomik toplanır; kayıtlar ortak
// tampona yazılır, ana programda (bölüt, tür, sıra, dil) sırasına dizilir.
#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
__constant__ uint32_t d_divm[1201];
#ifdef __CUDA_ARCH__
#define K_DIVM(C,x) d_divm[x]
#endif
#include "kart_ortak.h"

#define KIS 128                         // blok: 4 warp
typedef struct { int32_t bolut_ix; uint32_t sira; uint16_t fm; int16_t a[114]; } kgirdi_t;

static const ksabit_t *hC; static int ND; static int dev=-1; static char ACIK[256];
static ksabit_t *dC; static kdil_t *dD; static float *dH;
static korn_t *dZ=NULL; static int capZ=0; static kgirdi_t *dG=NULL; static uint64_t capG=0;
static unsigned long long *dK=NULL, *dKC=NULL, *dNB2=NULL; static uint64_t capK=0; static kkayit_t *dR=NULL; static uint64_t capR=0;
static unsigned long long *dSay=NULL;   // [0] girdi sayısı, [1] kayıt sayısı
static kprof_t *dP=NULL; static int nW=0;

#define CK(x) do{ cudaError_t e_=(x); if(e_!=cudaSuccess){ fprintf(stderr,"CUDA: %s (%s:%d)\n",cudaGetErrorString(e_),__FILE__,__LINE__); return 1; } }while(0)

__global__ void k_baslat(korn_t *Z, const ksabit_t *C, const int *bolut, int nb){
  const int j=blockIdx.x*blockDim.x+threadIdx.x; if(j>=nb) return; k_orn_init(&Z[j],C,k_tohum(C->sif,2,bolut[j])); }

// bölüt j'nin sira0..sira0+n−1 kontrolleri; kip 0: hepsi girdi tamponuna (j·CH + t); kip 1: yalnız B2'nin tuttuğu biçimi olanlar (ortak sayaç)
__global__ void k_ornekle(korn_t *Z, const ksabit_t *C, int nb, int sira0, int n, int CH, kgirdi_t *G, uint64_t capG, unsigned long long *say, unsigned long long *nb2){
  const int j=blockIdx.x*blockDim.x+threadIdx.x; if(j>=nb) return; korn_t *z=&Z[j]; int16_t a[114]; unsigned long long b2=0;
  for(int t=0;t<n;t++){ k_orn_sonraki(z,C,a);
    if(C->kip!=1){ kgirdi_t *g=&G[(uint64_t)j*CH+t]; g->bolut_ix=j; g->sira=(uint32_t)(sira0+t); g->fm=(uint16_t)(C->kip==2?C->fm_olay:0x3FFF); for(int q=0;q<114;q++) g->a[q]=a[q]; }
    else { uint32_t fm=0; for(int f=0;f<K_NFORM;f++){ kbk_t bk; k_kitap(a,C->al[f],C->be[f],&bk); if(k_mb0(&bk,C->al[f],C->be[f],C)&2u) fm|=1u<<f; }
      if(!fm) continue; b2++; const unsigned long long ix=atomicAdd(&say[0],1ull);
      if(ix<capG){ kgirdi_t *g=&G[ix]; g->bolut_ix=j; g->sira=(uint32_t)(sira0+t); g->fm=(uint16_t)fm; for(int q=0;q<114;q++) g->a[q]=a[q]; } } }
  if(C->kip==1) nb2[j]+=b2; }

__global__ void __launch_bounds__(KIS) k_degerlendir(const kgirdi_t *G, uint64_t nG, const ksabit_t *C, const kdil_t *D, int ND, const float *H, kprof_t *Pw,
    unsigned long long *K, unsigned long long *KC, kkayit_t *R, uint64_t capR, unsigned long long *say){
  __shared__ int16_t sa[KIS/32][114]; __shared__ kbk_t sbk[KIS/32][K_NFORM]; __shared__ uint8_t smb[KIS/32][K_NFORM];
  const int w=threadIdx.x>>5, ln=threadIdx.x&31; const int gw=(blockIdx.x*blockDim.x+threadIdx.x)>>5, NWt=(gridDim.x*blockDim.x)>>5;
  kprof_t *P=&Pw[gw];
  for(uint64_t e=gw;e<nG;e+=NWt){ const kgirdi_t *g=&G[e]; const uint32_t fm=g->fm;
    for(int q=ln;q<114;q+=32) sa[w][q]=g->a[q];
    __syncwarp();
    if(ln<K_NFORM){ kbk_t bk; k_kitap(sa[w],C->al[ln],C->be[ln],&bk); sbk[w][ln]=bk; smb[w][ln]=k_mb0(&bk,C->al[ln],C->be[ln],C); }
    __syncwarp();
    if(ln<K_NPR){ for(int f=0;f<K_NFORM;f++){ if(!(fm>>f&1)) continue; k_mercek(sa[w],f,ln,&sbk[w][f],smb[w][f],C,P); } }
    __syncwarp(); __threadfence_block();
    if(C->kip==2){                                                                   // kip 2: olaylar şeritlere bölünür
      for(int d=ln;d<ND;d+=32){ const kdil_t *Dd=&D[d]; if(!k_olay(C,P,Dd)) continue; atomicAdd(&K[(uint64_t)g->bolut_ix*ND+d],1ull);
        if(Dd->hed==0){ const unsigned long long r=atomicAdd(&say[1],1ull);
          if(r<capR){ kkayit_t *x=&R[r]; x->bolut_ix=g->bolut_ix; x->sira=g->sira; x->puan=0; x->L=(int16_t)Dd->Lsabit; x->dil=(uint16_t)d; x->tur=0;
            for(int q=0;q<114;q++) x->a[q]=(uint16_t)sa[w][q]; } } }
      __syncwarp(); continue; }
    for(int d=0;d<ND;d++){ const kdil_t *Dd=&D[d]; double best=-INFINITY; int bl=-1, bv=0;
      for(int L=ln;L<K_NLENS;L+=32){ if(!(fm>>(L/K_NPR)&1)) continue; if(!k_kapi(C,P,L)) continue; if(!k_kapsam_tamam(C,Dd,P,L)) continue; bv=1;
        const double v=k_mercek_puan(C,Dd,H,P,L); if(v>best){ best=v; bl=L; } }
      for(int o=16;o>0;o>>=1){ const double ob=__shfl_down_sync(0xffffffffu,best,o); const int ol=__shfl_down_sync(0xffffffffu,bl,o); const int ov=__shfl_down_sync(0xffffffffu,bv,o);
        if(ob>best || (ob==best && ol>=0 && (bl<0 || ol<bl))){ best=ob; bl=ol; } bv|=ov; }
      if(ln==0 && bv){ const uint64_t ix=(uint64_t)g->bolut_ix*ND+d; atomicAdd(&KC[ix],1ull); if(best>=Dd->esik_k) atomicAdd(&K[ix],1ull);
        if(Dd->hed==0){ for(int tur=0;tur<=C->kip;tur++){ if(tur==0 && !(best>=Dd->esik_q)) continue; const unsigned long long r=atomicAdd(&say[1],1ull);
            if(r<capR){ kkayit_t *x=&R[r]; x->bolut_ix=g->bolut_ix; x->sira=g->sira; x->puan=best; x->L=(int16_t)bl; x->dil=(uint16_t)d; x->tur=(uint8_t)tur;
              for(int q=0;q<114;q++) x->a[q]=(uint16_t)sa[w][q]; } } } } }
    __syncwarp(); } }

extern "C" const char *kart_aciklama(void){ return ACIK; }
extern "C" int kart_hazirla(const ksabit_t *C, const kdil_t *D, int nd, const float *havuz, uint64_t nhavuz){
  hC=C; ND=nd; CK(cudaSetDeviceFlags(cudaDeviceScheduleBlockingSync)); CK(cudaGetDevice(&dev)); cudaDeviceProp pr; CK(cudaGetDeviceProperties(&pr,dev));
  int drv=0, rt=0; cudaDriverGetVersion(&drv); cudaRuntimeGetVersion(&rt);
  snprintf(ACIK,sizeof ACIK,"CUDA kart %d: %s (sm_%d%d, %d SM), sürücü %d, çalışma zamanı %d",dev,pr.name,pr.major,pr.minor,pr.multiProcessorCount,drv,rt);
  CK(cudaMemcpyToSymbol(d_divm,C->divm,sizeof C->divm));
  CK(cudaMalloc(&dC,sizeof(ksabit_t))); CK(cudaMemcpy(dC,C,sizeof(ksabit_t),cudaMemcpyHostToDevice));
  CK(cudaMalloc(&dD,sizeof(kdil_t)*(nd?nd:1))); CK(cudaMemcpy(dD,D,sizeof(kdil_t)*nd,cudaMemcpyHostToDevice));
  CK(cudaMalloc(&dH,sizeof(float)*(nhavuz?nhavuz:1))); if(nhavuz) CK(cudaMemcpy(dH,havuz,sizeof(float)*nhavuz,cudaMemcpyHostToDevice));
  CK(cudaMalloc(&dSay,sizeof(unsigned long long)*2));
  nW=pr.multiProcessorCount*16*(KIS/32);  // kalıcı warp sayısı (blok başına 4 warp, SM başına 16 blok)
  CK(cudaMalloc(&dP,sizeof(kprof_t)*(size_t)nW));
  return 0; }

extern "C" int kart_parti(const int *bolut, int nb, int bn, uint64_t *k, uint64_t *kc, uint64_t *nb2, kkayit_t **kay, uint64_t *nkay){
  const int CH = hC->kip!=1 ? (getenv("KART_CH")?atoi(getenv("KART_CH")):1000) : (getenv("KART_CH")?atoi(getenv("KART_CH")):25000);
  const uint64_t needG = hC->kip!=1 ? (uint64_t)nb*CH : (uint64_t)(getenv("KART_GKAP")?atoll(getenv("KART_GKAP")):4000000);
  if(nb>capZ){ if(dZ) cudaFree(dZ); CK(cudaMalloc(&dZ,sizeof(korn_t)*(size_t)nb)); capZ=nb; }
  if(needG>capG){ if(dG) cudaFree(dG); CK(cudaMalloc(&dG,sizeof(kgirdi_t)*needG)); capG=needG; }
  const uint64_t nK=(uint64_t)nb*ND; if(nK>capK){ if(dK){ cudaFree(dK); cudaFree(dKC); } CK(cudaMalloc(&dK,8*(nK?nK:1))); CK(cudaMalloc(&dKC,8*(nK?nK:1))); capK=nK; }
  if(dNB2) cudaFree(dNB2); CK(cudaMalloc(&dNB2,8*(size_t)nb));
  if(!dR){ capR=getenv("KART_RKAP")?atoll(getenv("KART_RKAP")):1000000; CK(cudaMalloc(&dR,sizeof(kkayit_t)*capR)); }
  CK(cudaMemset(dK,0,8*(nK?nK:1))); CK(cudaMemset(dKC,0,8*(nK?nK:1))); CK(cudaMemset(dNB2,0,8*(size_t)nb)); CK(cudaMemset(dSay,0,16));
  int *dB; CK(cudaMalloc(&dB,sizeof(int)*nb)); CK(cudaMemcpy(dB,bolut,sizeof(int)*nb,cudaMemcpyHostToDevice));
  k_baslat<<<(nb+63)/64,64>>>(dZ,dC,dB,nb); CK(cudaGetLastError());
  if(hC->kip!=1 && bn%CH!=0){ fprintf(stderr,"kip 0: BN, KART_CH'nin katı olmalı (BN %d, CH %d)\n",bn,CH); return 1; }   // kısa son tur bayat yuvaları sayardı
  const int nblok=nW/(KIS/32);
  for(int s0=0;s0<bn;s0+=CH){ const int n=(bn-s0<CH)?bn-s0:CH;
    if(hC->kip==1){ unsigned long long z=0; CK(cudaMemcpy(dSay,&z,8,cudaMemcpyHostToDevice)); }
    k_ornekle<<<(nb+63)/64,64>>>(dZ,dC,nb,s0,n,CH,dG,capG,dSay,dNB2); CK(cudaGetLastError());
    uint64_t nG;
    if(hC->kip!=1) nG=(uint64_t)nb*CH;
    else { unsigned long long s; CK(cudaMemcpy(&s,dSay,8,cudaMemcpyDeviceToHost)); if(s>capG){ fprintf(stderr,"girdi tamponu taştı (%llu > %llu); KART_GKAP büyütülmeli\n",s,(unsigned long long)capG); return 1; } nG=s; }
    if(nG) { k_degerlendir<<<nblok,KIS>>>(dG,nG,dC,dD,ND,dH,dP,dK,dKC,dR,capR,dSay); CK(cudaGetLastError()); } }
  CK(cudaDeviceSynchronize());
  unsigned long long say[2]; CK(cudaMemcpy(say,dSay,16,cudaMemcpyDeviceToHost));
  if(say[1]>capR){ fprintf(stderr,"kayıt tamponu taştı (%llu > %llu); KART_RKAP büyütülmeli\n",say[1],(unsigned long long)capR); return 1; }
  CK(cudaMemcpy(k,dK,8*nK,cudaMemcpyDeviceToHost)); CK(cudaMemcpy(kc,dKC,8*nK,cudaMemcpyDeviceToHost)); CK(cudaMemcpy(nb2,dNB2,8*(size_t)nb,cudaMemcpyDeviceToHost));
  *nkay=say[1]; *kay=(kkayit_t*)malloc(sizeof(kkayit_t)*(say[1]?say[1]:1)); if(say[1]) CK(cudaMemcpy(*kay,dR,sizeof(kkayit_t)*say[1],cudaMemcpyDeviceToHost));
  cudaFree(dB); return 0; }
