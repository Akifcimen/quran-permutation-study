// kesif_cuda.cu — Derin permütasyon motoru, CUDA sürümü (RTX 4090 hedefli)
// kesif4_motor.c ile SAYAÇ-UYUMLU: aynı olay tanımları, aynı çıktı formatı.
// Kullanım:
//   ./kesif_cuda selftest
//   ./kesif_cuda <seed> <toplam_deneme>            (ör. ./kesif_cuda 7 10000000000000)
// Çıktı: her yığın (chunk) sonrası kümülatif "T=... A=... ... X6=..." satırı (kesilse bile son satır geçerlidir).
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cuda_runtime.h>

#define CHECK(x) do{ cudaError_t e=(x); if(e!=cudaSuccess){ \
  fprintf(stderr,"CUDA hata %s:%d: %s\n",__FILE__,__LINE__,cudaGetErrorString(e)); exit(1);} }while(0)

static const int AYAT127[114] = {
  7,286,200,176,120,165,206,75,127,109,123,111,43,52,99,128,111,110,98,135,
  112,78,118,64,77,227,93,88,69,60,34,30,73,54,45,83,182,88,75,85,
  54,53,89,59,37,35,38,29,18,45,60,49,62,55,78,96,29,22,24,13,
  14,11,11,18,12,12,30,52,52,44,28,28,20,56,40,31,50,40,46,42,
  29,19,36,25,22,17,19,26,30,20,15,21,11,8,8,19,5,8,8,11,
  11,8,3,9,5,4,7,3,6,3,5,4,5,6};

// sayaç indeksleri
enum { cT,cA,cB,cC,cD,cE,cF,cG,cAB,cAC,cH,cJ,
       cW,cW2,cW3,cBW,cCW,cDW,cGW,cAW,cADW,cAGW,
       cX1,cX2,cX3,cX4,cX5,cX6, NCTR };
static const char* CNAME[NCTR]={"T","A","B","C","D","E","F","G","AB","AC","H","J",
  "W","W2","W3","BW","CW","DW","GW","AW","ADW","AGW","X1","X2","X3","X4","X5","X6"};

__device__ __forceinline__ uint64_t splitmix64(uint64_t &s){
  uint64_t z = (s += 0x9E3779B97F4A7C15ULL);
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
  return z ^ (z >> 31);
}
__device__ __forceinline__ uint32_t bounded(uint64_t &s, uint32_t n){
  // ((128-bit) x*n) >> 64 == __umul64hi(x, n)  — Windows/Linux taşınabilir
  return (uint32_t)__umul64hi(splitmix64(s), (uint64_t)n);
}

__device__ int ord_is_360(int m00,int m01,int m10,int m11){
  int x00=1,x01=0,x10=0,x11=1;
  for(int k=1;k<=360;k++){
    int y00=(x00*m00+x01*m10)%19, y01=(x00*m01+x01*m11)%19;
    int y10=(x10*m00+x11*m10)%19, y11=(x10*m01+x11*m11)%19;
    x00=y00;x01=y01;x10=y10;x11=y11;
    if(x00==1&&x01==0&&x10==0&&x11==1) return k==360;
  }
  return 0;
}

// tek deneme değerlendirmesi — a_get(i): i. sûrenin ayet sayısı (0-indeksli)
template<typename GET>
__device__ void evaluate(GET a_get, unsigned int *L){
  int evenCnt=0; long sumTEven=0;
  long snE=0, saE=0, fn=0, fa=0;
  int famCnt=0, coeffSum=0;
  int cellCnt[4]={0,0,0,0}, cellSum[4]={0,0,0,0};
  unsigned char histE[24]={0}, histO[24]={0};
  int cellVals[4][4]; int cellN[4]={0,0,0,0};
  int nu=0, usum=0, uposs=0, uniCnt=0; long uniSa=0;
  for(int i=0;i<114;i++){
    int ai=a_get(i);
    int n=i+1, t=n+ai, u=n-ai;
    int fu=(u%19==0), ft=(t%19==0);
    if(fu){ nu++; usum+=u; if(u>0) uposs+=u; }
    if(ft||fu){ uniCnt++; uniSa+=ai; }
    int te=((t&1)==0);
    if(te){ evenCnt++; sumTEven+=t; snE+=n; saE+=ai; }
    if(ft){
      int c=t/19;
      famCnt++; coeffSum+=c; fn+=n; fa+=ai;
      int idx=(te?0:2)+((n&1)?1:0);
      cellCnt[idx]++; cellSum[idx]+=c;
      if(cellN[idx]<4) cellVals[idx][cellN[idx]++]=c;
      if(c<24){ if(te) histE[c]++; else histO[c]++; }
    }
  }
  L[cT]++;
  int A_=(evenCnt==57 && sumTEven==6234);
  int B_=(famCnt==12 && coeffSum==76);
  int ehc=cellCnt[0]+cellCnt[1], ehs=cellSum[0]+cellSum[1];
  int C_=(B_ && ehc==6 && ehs==38);
  int D_=0,E_=0,G_=0;
  if(C_){
    int cnts=(cellCnt[0]==3&&cellCnt[1]==3&&cellCnt[2]==3&&cellCnt[3]==3);
    int odd=(cellSum[2]==19&&cellSum[3]==19);
    int even=((cellSum[0]==18&&cellSum[1]==20)||(cellSum[0]==20&&cellSum[1]==18));
    D_=cnts&&odd&&even;
    E_=(histE[6]==5&&histE[8]==1&&histO[5]==3&&histO[7]==2&&histO[9]==1);
    if(D_&&E_){
      int id[4];
      for(int k=0;k<4;k++){ int s=0,q=0;
        for(int m=0;m<cellN[k];m++){ s+=cellVals[k][m]; q+=cellVals[k][m]*cellVals[k][m]; }
        id[k]=s*1000+q; }
      int ep=((id[0]==18108&&id[1]==20136)||(id[0]==20136&&id[1]==18108));
      int op=((id[2]==19131&&id[3]==19123)||(id[2]==19123&&id[3]==19131));
      G_=ep&&op;
    }
  }
  int W_=(nu==7 && uniCnt==19 && (uniSa%19)==0);
  int pOK=0,rOK=0;
  if(A_||C_){
    pOK=ord_is_360((int)(snE%19),(int)(saE%19),(int)((6555-snE)%19),(int)((6234-saE)%19));
  }
  if(B_){
    rOK=ord_is_360((int)(fn%19),(int)(fa%19),(int)((6555-fn)%19),(int)((6234-fa)%19));
  }
  if(A_)L[cA]++; if(B_)L[cB]++; if(C_)L[cC]++; if(D_)L[cD]++; if(E_)L[cE]++;
  if(D_&&E_)L[cF]++; if(G_)L[cG]++;
  if(A_&&B_)L[cAB]++; if(A_&&C_)L[cAC]++; if(A_&&D_)L[cH]++; if(A_&&G_)L[cJ]++;
  if(W_){
    L[cW]++;
    if(usum==133)L[cW2]++;
    if(uposs==266)L[cW3]++;
    if(B_)L[cBW]++; if(C_)L[cCW]++; if(D_)L[cDW]++; if(G_)L[cGW]++;
    if(A_)L[cAW]++; if(A_&&D_)L[cADW]++; if(A_&&G_)L[cAGW]++;
  }
  if(A_&&pOK)L[cX1]++; if(B_&&rOK)L[cX2]++;
  if(C_&&pOK&&rOK)L[cX3]++; if(D_&&pOK&&rOK)L[cX4]++;
  if(D_&&W_&&pOK&&rOK)L[cX5]++; if(G_&&pOK&&rOK)L[cX6]++;
}

__constant__ int d_ayat[114];

// ana çekirdek: her iş parçacığı kendi dizisiyle `iters` deneme koşar
__global__ void kernel_run(uint64_t seed, uint64_t chunkId, uint32_t iters,
                           unsigned long long *gctr){
  extern __shared__ int sdata[]; // [114 * blockDim.x], eleman j: sdata[j*blockDim.x+tid]
  const int tid=threadIdx.x, bs=blockDim.x;
  const uint64_t gid=(uint64_t)blockIdx.x*bs+tid;
  uint64_t st = seed ^ (gid*0xA24BAED4963EE407ULL) ^ (chunkId*0x9FB21C651E98DF25ULL) ^ 0xD6E8FEB86659FD93ULL;
  // diziyi yükle
  for(int j=0;j<114;j++) sdata[j*bs+tid]=d_ayat[j];
  unsigned int L[NCTR]; for(int k=0;k<NCTR;k++) L[k]=0;
  auto get=[&](int i){ return sdata[i*bs+tid]; };
  for(uint32_t it=0; it<iters; it++){
    for(int i=113;i>0;i--){
      int j=(int)bounded(st,(uint32_t)(i+1));
      int tmp=sdata[i*bs+tid]; sdata[i*bs+tid]=sdata[j*bs+tid]; sdata[j*bs+tid]=tmp;
    }
    evaluate(get,L);
  }
  for(int k=0;k<NCTR;k++) if(L[k]) atomicAdd(&gctr[k],(unsigned long long)L[k]);
}

__global__ void kernel_selftest(unsigned long long *gctr){
  if(blockIdx.x==0 && threadIdx.x==0){
    unsigned int L[NCTR]; for(int k=0;k<NCTR;k++) L[k]=0;
    auto get=[&](int i){ return d_ayat[i]; };
    evaluate(get,L);
    for(int k=0;k<NCTR;k++) gctr[k]=L[k];
  }
}

static void print_ctrs(const char* pfx, unsigned long long *h){
  printf("%s",pfx);
  for(int k=0;k<NCTR;k++) printf("%s%s=%llu", k?" ":"", CNAME[k], h[k]);
  printf("\n"); fflush(stdout);
}

int main(int argc,char**argv){
  CHECK(cudaMemcpyToSymbol(d_ayat,AYAT127,sizeof(AYAT127)));
  unsigned long long *dctr; CHECK(cudaMalloc(&dctr,NCTR*sizeof(unsigned long long)));
  unsigned long long h[NCTR];

  if(argc>=2 && !strcmp(argv[1],"selftest")){
    CHECK(cudaMemset(dctr,0,NCTR*sizeof(unsigned long long)));
    kernel_selftest<<<1,1>>>(dctr);
    CHECK(cudaDeviceSynchronize());
    CHECK(cudaMemcpy(h,dctr,sizeof h,cudaMemcpyDeviceToHost));
    print_ctrs("selftest ",h);
    return 0;
  }
  if(argc<3){ fprintf(stderr,"kullanım: %s <seed> <toplam_deneme>\n",argv[0]); return 1; }
  uint64_t seed=strtoull(argv[1],0,10), total=strtoull(argv[2],0,10);

  const int THREADS=128;
  int dev=0, sms=0; CHECK(cudaGetDevice(&dev));
  CHECK(cudaDeviceGetAttribute(&sms,cudaDevAttrMultiProcessorCount,dev));
  const int BLOCKS=sms*8;                       // 4090: 128 SM → 1024 blok
  const size_t SHMEM=(size_t)114*THREADS*sizeof(int); // 58.368 B
  CHECK(cudaFuncSetAttribute(kernel_run,cudaFuncAttributeMaxDynamicSharedMemorySize,(int)SHMEM));
  const uint64_t NTHREADS=(uint64_t)BLOCKS*THREADS;
  const uint32_t ITERS=4096;                    // yığın başına iş parçacığı denemesi
  const uint64_t CHUNK=NTHREADS*ITERS;          // 4090: ~537M deneme/yığın
  fprintf(stderr,"# GPU: %d SM, %d blok × %d thread, yığın=%llu deneme\n",
          sms,BLOCKS,THREADS,(unsigned long long)CHUNK);

  CHECK(cudaMemset(dctr,0,NCTR*sizeof(unsigned long long)));
  uint64_t done=0, chunkId=0;
  while(done<total){
    uint32_t it=ITERS;
    if(total-done < CHUNK){
      it=(uint32_t)((total-done+NTHREADS-1)/NTHREADS);
      if(!it) break;
    }
    kernel_run<<<BLOCKS,THREADS,SHMEM>>>(seed,chunkId++,it,dctr);
    CHECK(cudaDeviceSynchronize());
    done+=(uint64_t)NTHREADS*it;
    CHECK(cudaMemcpy(h,dctr,sizeof h,cudaMemcpyDeviceToHost));
    print_ctrs("",h);
  }
  return 0;
}
