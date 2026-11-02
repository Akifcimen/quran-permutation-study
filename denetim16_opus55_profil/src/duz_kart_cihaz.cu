// duz_kart_cihaz.cu — duz_ortak.h yürüyüşünün CUDA arka ucu: iş parçacığı başına bir yürüyüş (olay × tekrar); durum aygıt belleğinde,
// çekirdek parça parça çağrılır.
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
#include "duz_ortak.h"
#define CK(x) do{ cudaError_t e_=(x); if(e_!=cudaSuccess){ fprintf(stderr,"CUDA: %s (%s:%d)\n",cudaGetErrorString(e_),__FILE__,__LINE__); return 1; } }while(0)
static ksabit_t *dC; static dolay_t *dE; static yur_t *dY; static kprof_t *dP; static int NW; static double LQ_; static char ACIK[256];
__global__ void k_yuru(yur_t *Y, int nw, const dolay_t *E, const ksabit_t *C, kprof_t *P, double LQ, int64_t parca){
  const int w=blockIdx.x*blockDim.x+threadIdx.x; if(w>=nw) return;
  yur_t y=Y[w]; d_yuru(&y,&E[y.e],C,&P[w],LQ,parca); Y[w]=y; }
extern "C" const char *duz_aciklama(void){ return ACIK; }
extern "C" int duz_hazirla(const ksabit_t *C, double LQ, const dolay_t *E, int ne, const yur_t *Y, int nw){
  cudaSetDeviceFlags(cudaDeviceScheduleBlockingSync);   // ana iş parçacığı beklerken uyur (işlemciyi meşgul etmez); sonucu değiştirmez
  int dev=0; CK(cudaGetDevice(&dev)); cudaDeviceProp pr; CK(cudaGetDeviceProperties(&pr,dev)); int drv=0, rt=0; cudaDriverGetVersion(&drv); cudaRuntimeGetVersion(&rt);
  snprintf(ACIK,sizeof ACIK,"CUDA kart %d: %s (sm_%d%d, %d SM), sürücü %d, çalışma zamanı %d",dev,pr.name,pr.major,pr.minor,pr.multiProcessorCount,drv,rt);
  CK(cudaMemcpyToSymbol(d_divm,C->divm,sizeof C->divm));
  CK(cudaMalloc(&dC,sizeof(ksabit_t))); CK(cudaMemcpy(dC,C,sizeof(ksabit_t),cudaMemcpyHostToDevice));
  CK(cudaMalloc(&dE,sizeof(dolay_t)*ne)); CK(cudaMemcpy(dE,E,sizeof(dolay_t)*ne,cudaMemcpyHostToDevice));
  CK(cudaMalloc(&dY,sizeof(yur_t)*nw)); CK(cudaMemcpy(dY,Y,sizeof(yur_t)*nw,cudaMemcpyHostToDevice));
  CK(cudaMalloc(&dP,sizeof(kprof_t)*(size_t)nw)); NW=nw; LQ_=LQ; return 0; }
extern "C" int duz_yurut(int64_t parca){
  k_yuru<<<(NW+127)/128,128>>>(dY,NW,dE,dC,dP,LQ_,parca); CK(cudaGetLastError()); CK(cudaDeviceSynchronize()); return 0; }
extern "C" int duz_al(yur_t *Y){ CK(cudaMemcpy(Y,dY,sizeof(yur_t)*NW,cudaMemcpyDeviceToHost)); return 0; }
