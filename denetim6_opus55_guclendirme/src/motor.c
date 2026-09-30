// Bağımsız permütasyon motoru (xoshiro256++; yazarın SplitMix64'ünden farklı aile).
// Yazarın olayları (kesif4_motor.c ile birebir tanım) + sınıf-düzeyi kristal G* +
// farklı SIFIR modelleri:
//   U        : tekdüze permütasyon (yazarın sıfırı)
//   J<sigma> : "titreşim" sıfırı — gerçek dizilişte her sûrenin ayet sayısı konumu
//              n + sigma*Z ile yeniden sıralanır (uzunluk-sırası korunur, ince yapı rastgele)
//   B<w>     : blok karıştırma — ardışık w'lik bloklar içinde tekdüze permütasyon
//   M        : S = Σ n·a ≤ S_obs kısıtlı MCMC (yazarın kesif16 sıfırı), transpozisyon önerisi
// Kullanım: ./motor <null> <seed> <trials> <threads>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

static const int AYAT127[114] = {
  7,286,200,176,120,165,206,75,127,109,123,111,43,52,99,128,111,110,98,135,
  112,78,118,64,77,227,93,88,69,60,34,30,73,54,45,83,182,88,75,85,
  54,53,89,59,37,35,38,29,18,45,60,49,62,55,78,96,29,22,24,13,
  14,11,11,18,12,12,30,52,52,44,28,28,20,56,40,31,50,40,46,42,
  29,19,36,25,22,17,19,26,30,20,15,21,11,8,8,19,5,8,8,11,
  11,8,3,9,5,4,7,3,6,3,5,4,5,6};

typedef struct { uint64_t s[4]; } rng_t;
static inline uint64_t rotl(const uint64_t x, int k){ return (x << k) | (x >> (64 - k)); }
static inline uint64_t xnext(rng_t *r){
  uint64_t *s = r->s;
  const uint64_t result = rotl(s[0] + s[3], 23) + s[0];
  const uint64_t t = s[1] << 17;
  s[2] ^= s[0]; s[3] ^= s[1]; s[1] ^= s[2]; s[0] ^= s[3]; s[2] ^= t; s[3] = rotl(s[3], 45);
  return result;
}
static void rseed(rng_t *r, uint64_t seed){
  uint64_t z = seed;
  for(int i=0;i<4;i++){
    z += 0x9E3779B97F4A7C15ULL; uint64_t x = z;
    x = (x ^ (x >> 30)) * 0xBF58476D1CE4E5B9ULL; x = (x ^ (x >> 27)) * 0x94D049BB133111EBULL;
    r->s[i] = x ^ (x >> 31);
  }
}
static inline uint32_t bounded(rng_t *r, uint32_t n){ return (uint32_t)(((unsigned __int128)xnext(r) * n) >> 64); }
static inline double unif(rng_t *r){ return (xnext(r) >> 11) * (1.0/9007199254740992.0); }
static inline double gauss(rng_t *r){
  double u1 = unif(r), u2 = unif(r); if(u1 < 1e-300) u1 = 1e-300;
  return sqrt(-2.0*log(u1)) * cos(6.283185307179586*u2);
}

enum { cA, cB, cC, cD, cE, cG, cGs, cW, cAB, cAC, cAD, cAG, cAGs, cGW, cGsW, cAW, cAGW, cAGsW, cDW, cK12, NC };
static const char *CN[NC] = {"A","B","C","D","E","G","Gs","W","AB","AC","AD","AG","AGs","GW","GsW","AW","AGW","AGsW","DW","K12"};

static inline void evaluate(const int *a, uint64_t *K){
  int evenCnt=0; long sumTEven=0; int famCnt=0, coeffSum=0;
  int cellCnt[4]={0,0,0,0}, cellSum[4]={0,0,0,0};
  int histE[40], histO[40]; memset(histE,0,sizeof histE); memset(histO,0,sizeof histO);
  int nu=0, uniCnt=0; long uniSa=0; int cmin=1000, cmax=-1;
  int hist[40]; memset(hist,0,sizeof hist);
  for(int i=0;i<114;i++){
    int n=i+1, t=n+a[i], u=n-a[i];
    int fu=(u%19==0), ft=(t%19==0);
    if(fu) nu++;
    if(ft||fu){ uniCnt++; uniSa+=a[i]; }
    int te=((t&1)==0);
    if(te){ evenCnt++; sumTEven+=t; }
    if(ft){
      int c=t/19; famCnt++; coeffSum+=c;
      int idx=(te?0:2)+((n&1)?1:0); cellCnt[idx]++; cellSum[idx]+=c;
      if(c<40){ if(te) histE[c]++; else histO[c]++; hist[c]++; }
      if(c<cmin) cmin=c; if(c>cmax) cmax=c;
    }
  }
  int A_=(evenCnt==57 && sumTEven==6234);
  int B_=(famCnt==12 && coeffSum==76);
  int C_=(B_ && (cellCnt[0]+cellCnt[1])==6 && (cellSum[0]+cellSum[1])==38);
  int D_=0,E_=0,G_=0,Gs=0;
  if(C_){
    int cnts_ok=(cellCnt[0]==3&&cellCnt[1]==3&&cellCnt[2]==3&&cellCnt[3]==3);
    int odd_ok=(cellSum[2]==19&&cellSum[3]==19);
    int even_ok=((cellSum[0]==18&&cellSum[1]==20)||(cellSum[0]==20&&cellSum[1]==18));
    D_=cnts_ok&&odd_ok&&even_ok;
    E_=(histE[6]==5&&histE[8]==1&&histO[5]==3&&histO[7]==2&&histO[9]==1);
    G_=D_&&E_;   // hücre içerikleri, kefe torbaları + hücre toplamlarıyla zorunlu (yazarın notu)
    if(D_ && cmax<40){ int band=1; for(int c=cmin;c<=cmax;c++) if(!hist[c]){band=0;break;} Gs=band; }
  }
  int W_=(nu==7 && uniCnt==19 && (uniSa%19)==0);
  if(A_) K[cA]++; if(B_) K[cB]++; if(C_) K[cC]++; if(D_) K[cD]++; if(E_) K[cE]++; if(G_) K[cG]++; if(Gs) K[cGs]++;
  if(W_) K[cW]++; if(A_&&B_) K[cAB]++; if(A_&&C_) K[cAC]++; if(A_&&D_) K[cAD]++; if(A_&&G_) K[cAG]++; if(A_&&Gs) K[cAGs]++;
  if(G_&&W_) K[cGW]++; if(Gs&&W_) K[cGsW]++; if(A_&&W_) K[cAW]++; if(A_&&G_&&W_) K[cAGW]++; if(A_&&Gs&&W_) K[cAGsW]++;
  if(D_&&W_) K[cDW]++; if(famCnt==12) K[cK12]++;
}

typedef struct { char null; double par; uint64_t seed, trials; uint64_t K[NC]; uint64_t T; } job_t;

static long S_OBS;
static void *worker(void *arg){
  job_t *J=(job_t*)arg; rng_t r; rseed(&r, J->seed);
  int a[114]; memcpy(a, AYAT127, sizeof a);
  memset(J->K,0,sizeof J->K); J->T=0;
  if(J->null=='U'){
    for(uint64_t it=0; it<J->trials; it++){
      for(int i=113;i>0;i--){ int j=(int)bounded(&r,(uint32_t)(i+1)); int tmp=a[i]; a[i]=a[j]; a[j]=tmp; }
      evaluate(a, J->K); J->T++;
    }
  } else if(J->null=='J'){
    double key[114]; int idx[114];
    for(uint64_t it=0; it<J->trials; it++){
      for(int i=0;i<114;i++){ key[i]=(i+1)+J->par*gauss(&r); idx[i]=i; }
      // eklemeli sıralama (idx'i key'e göre)
      for(int i=1;i<114;i++){ int x=idx[i]; double kx=key[x]; int j=i-1; while(j>=0 && key[idx[j]]>kx){ idx[j+1]=idx[j]; j--; } idx[j+1]=x; }
      for(int k=0;k<114;k++) a[k]=AYAT127[idx[k]];
      evaluate(a, J->K); J->T++;
    }
  } else if(J->null=='B'){
    int w=(int)J->par;
    for(uint64_t it=0; it<J->trials; it++){
      memcpy(a, AYAT127, sizeof a);
      for(int s=0;s<114;s+=w){ int e=s+w; if(e>114) e=114; int L=e-s;
        for(int i=L-1;i>0;i--){ int j=(int)bounded(&r,(uint32_t)(i+1)); int tmp=a[s+i]; a[s+i]=a[s+j]; a[s+j]=tmp; } }
      evaluate(a, J->K); J->T++;
    }
  } else if(J->null=='M' || J->null=='C'){
    // kesif16 sıfırı: S ≤ S_obs üzerinde tekdüze; başlangıç tam-azalan; transpozisyon Metropolis.
    // 'M': iplik başına 1 zincir; 'C': iplik başına 64 zincir (sırayla örneklenir, ardışık örnek bağımlılığı azalır)
    int sorted_[114]; memcpy(sorted_, AYAT127, sizeof sorted_);
    for(int i=1;i<114;i++){ int x=sorted_[i]; int j=i-1; while(j>=0 && sorted_[j]<x){ sorted_[j+1]=sorted_[j]; j--; } sorted_[j+1]=x; }
    int NCH = (J->null=='C') ? 64 : 1;
    static __thread int ch[64][114]; static __thread long chS[64];
    for(int c=0;c<NCH;c++){ memcpy(ch[c], sorted_, sizeof sorted_); long S=0; for(int i=0;i<114;i++) S+=(long)(i+1)*ch[c][i];
      for(int b=0;b<30000;b++){ int i=(int)bounded(&r,114), j=(int)bounded(&r,114); long dS=(long)((i+1)-(j+1))*(ch[c][j]-ch[c][i]); if(S+dS<=S_OBS){ int tmp=ch[c][i]; ch[c][i]=ch[c][j]; ch[c][j]=tmp; S+=dS; } }
      chS[c]=S; }
    int thin=(int)J->par; if(thin<1) thin=1;
    for(uint64_t it=0; it<J->trials; it++){
      int c=(int)(it%NCH); int *b=ch[c]; long S=chS[c];
      for(int st=0; st<thin; st++){ int i=(int)bounded(&r,114), j=(int)bounded(&r,114); long dS=(long)((i+1)-(j+1))*(b[j]-b[i]); if(S+dS<=S_OBS){ int tmp=b[i]; b[i]=b[j]; b[j]=tmp; S+=dS; } }
      chS[c]=S; evaluate(b, J->K); J->T++;
    }
  }
  return NULL;
}

int main(int argc, char **argv){
  S_OBS=0; for(int i=0;i<114;i++) S_OBS+=(long)(i+1)*AYAT127[i];
  if(argc>=2 && strcmp(argv[1],"selftest")==0){
    uint64_t K[NC]; memset(K,0,sizeof K); evaluate(AYAT127,K);
    for(int c=0;c<NC;c++) printf("%s=%llu ",CN[c],(unsigned long long)K[c]); printf("\nS_obs(127)=%ld\n",S_OBS); return 0;
  }
  if(argc<5){ fprintf(stderr,"kullanım: %s <U|J<s>|B<w>|M<thin>> <seed> <trials> <threads>\n",argv[0]); return 1; }
  char null=argv[1][0]; double par=(strlen(argv[1])>1)?atof(argv[1]+1):0;
  uint64_t seed=strtoull(argv[2],NULL,10), trials=strtoull(argv[3],NULL,10); int th=atoi(argv[4]);
  job_t *J=calloc(th,sizeof(job_t)); pthread_t *P=calloc(th,sizeof(pthread_t));
  for(int i=0;i<th;i++){ J[i].null=null; J[i].par=par; J[i].seed=seed*1000003ULL+i*7919ULL+1; J[i].trials=trials/th; pthread_create(&P[i],NULL,worker,&J[i]); }
  uint64_t K[NC]; memset(K,0,sizeof K); uint64_t T=0;
  for(int i=0;i<th;i++){ pthread_join(P[i],NULL); for(int c=0;c<NC;c++) K[c]+=J[i].K[c]; T+=J[i].T; }
  printf("null=%s seed=%llu T=%llu", argv[1], (unsigned long long)seed, (unsigned long long)T);
  for(int c=0;c<NC;c++) printf(" %s=%llu",CN[c],(unsigned long long)K[c]);
  printf("\n");
  return 0;
}
