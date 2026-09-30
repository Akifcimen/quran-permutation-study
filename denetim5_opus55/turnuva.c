// kesif19_catallanma.py'nin score() fonksiyonunun BİREBİR C portu (katı + çatallı S1–S8),
// + sıfır modelleri (U tekdüze, J<sigma> titreşim, M<thin> kesif16 S≤S_obs MCMC, B<w> blok).
// Modlar:
//   fixed : sabit mercek (1,1,19): katı t-eli, çatallı t-eli, çatallı iki-el (maks) histogramları
//   free  : 8 biçim × 38 modül × 2 el (kesif19 run_free ile aynı: katı yalnız v-birincil; çatallı iki el)
//   real  : gerçek kitabın tam haritası (doğrulama)
// Kullanım: ./turnuva <mode> <null> <seed> <trials> <threads>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <pthread.h>

static const int AYAT127[114] = {
  7,286,200,176,120,165,206,75,127,109,123,111,43,52,99,128,111,110,98,135,
  112,78,118,64,77,227,93,88,69,60,34,30,73,54,45,83,182,88,75,85,
  54,53,89,59,37,35,38,29,18,45,60,49,62,55,78,96,29,22,24,13,
  14,11,11,18,12,12,30,52,52,44,28,28,20,56,40,31,50,40,46,42,
  29,19,36,25,22,17,19,26,30,20,15,21,11,8,8,19,5,8,8,11,
  11,8,3,9,5,4,7,3,6,3,5,4,5,6};
static const int FORMS[8][2] = {{0,1},{1,1},{1,2},{1,3},{2,1},{2,3},{3,1},{3,2}};

typedef struct { uint64_t s[4]; } rng_t;
static inline uint64_t rotl(const uint64_t x, int k){ return (x << k) | (x >> (64 - k)); }
static inline uint64_t xnext(rng_t *r){ uint64_t *s=r->s; const uint64_t res=rotl(s[0]+s[3],23)+s[0]; const uint64_t t=s[1]<<17;
  s[2]^=s[0]; s[3]^=s[1]; s[1]^=s[2]; s[0]^=s[3]; s[2]^=t; s[3]=rotl(s[3],45); return res; }
static void rseed(rng_t *r, uint64_t seed){ uint64_t z=seed; for(int i=0;i<4;i++){ z+=0x9E3779B97F4A7C15ULL; uint64_t x=z;
  x=(x^(x>>30))*0xBF58476D1CE4E5B9ULL; x=(x^(x>>27))*0x94D049BB133111EBULL; r->s[i]=x^(x>>31);} }
static inline uint32_t bounded(rng_t *r, uint32_t n){ return (uint32_t)(((unsigned __int128)xnext(r)*n)>>64); }
static inline double unif(rng_t *r){ return (xnext(r)>>11)*(1.0/9007199254740992.0); }
static inline double gauss(rng_t *r){ double u1=unif(r),u2=unif(r); if(u1<1e-300)u1=1e-300; return sqrt(-2.0*log(u1))*cos(6.283185307179586*u2); }

static inline long labs_(long x){ return x<0?-x:x; }
static inline int is_square(long x){ x=labs_(x); long r=(long)floor(sqrt((double)x)); return (r*r==x)||((r+1)*(r+1)==x); }
static long PP[200]; static int nPP=0;
static inline int is_pp(long x){ x=labs_(x); for(int i=0;i<nPP;i++) if(PP[i]==x) return 1; return 0; }
static inline int pretty(long x, int m){ return (x%((long)m*m)==0)||is_square(x)||(x%114==0)||is_pp(x); }
static inline long fmod_pos(long x, long m){ long r=x%m; return r<0?r+m:r; }
static inline long floordiv(long x, long m){ long q=x/m; if((x%m!=0)&&((x<0)!=(m<0))) q--; return q; }

// Bir mercek-el için (katı, çatallı) puan
static void score1(const int *P, int al, int be, int m, int prim, int *strict, int *forked){
  int V[114], W[114];
  for(int i=0;i<114;i++){ int n=i+1; int v=al*n+be*P[i], w=al*n-be*P[i]; if(prim){V[i]=v;W[i]=w;} else {V[i]=w;W[i]=v;} }
  double E=114.0/m; long mm=(long)m*m;
  int k=0,ke=0,kn=0,ku=0,kuni=0,inter=0; long sv=0,sve=0,svn=0;
  long sa_u=0,sn_u=0,sv_u=0,sw_u=0, saU=0,snU=0,swU=0;
  long cmin=LONG_MAX, cmax=LONG_MIN; int seen_cnt=0; long cvals[114];
  for(int i=0;i<114;i++){
    int n=i+1; int F=(V[i]%m==0); int U=(W[i]%m==0);
    if(F){ k++; sv+=V[i]; long c=floordiv(V[i],m); cvals[seen_cnt++]=c; if(c<cmin)cmin=c; if(c>cmax)cmax=c;
           if(V[i]%2==0){ ke++; sve+=V[i]; } if(n%2==0){ kn++; svn+=V[i]; } }
    if(U){ ku++; saU+=P[i]; snU+=n; swU+=W[i]; }
    if(F||U){ kuni++; sa_u+=P[i]; sn_u+=n; sv_u+=V[i]; sw_u+=W[i]; }
    if(F&&U) inter++;
  }
  int kpos=k>0; long svo=sv-sve, svn2=sv-svn;
  int s1=(k>=2*E); int f1=s1||(k==m);
  int s2=kpos&&(sv%mm==0); int f2=kpos&&pretty(sv,m);
  int eqcnt=(2*ke==k)&&kpos; int s3=eqcnt&&(sve==svo); long d=labs_(sve-svo);
  int f3v=eqcnt&&((d==0)||(d==mm)||(d==2*mm));
  int eqn=(2*kn==k)&&kpos; long dn=labs_(svn-svn2);
  int f3n=eqn&&((dn==0)||(dn==mm)||(dn==2*mm));
  int f3=f3v||f3n;
  int s4=kpos&&(sve%mm==0)&&(svo%mm==0);
  int f4=s4||(kpos&&is_square(sve)&&is_square(svo))||(kpos&&(svn%mm==0)&&(svn2%mm==0));
  int band=0, ap2=0;
  if(kpos){
    // farklı değer sayısı
    // basit: sırala
    for(int i=1;i<seen_cnt;i++){ long x=cvals[i]; int j=i-1; while(j>=0&&cvals[j]>x){cvals[j+1]=cvals[j];j--;} cvals[j+1]=x; }
    int nd=1; for(int i=1;i<seen_cnt;i++) if(cvals[i]!=cvals[i-1]) nd++;
    band=(nd==cmax-cmin+1);
    int allpar=1; long pm=fmod_pos(cmin,2); for(int i=0;i<seen_cnt;i++) if(fmod_pos(cvals[i],2)!=pm){allpar=0;break;}
    ap2=((cmax-cmin)%2==0)&&(nd==(cmax-cmin)/2+1)&&(nd>=2)&&allpar;
  }
  int s5=band, f5=band||ap2;
  int s6=(kuni==m); int f6=s6||(kuni==2*m)||((ku==k)&&kpos)||(ku==m)||(ku>=2*E);
  int s7=(sa_u%m==0);
  int f7=s7||(sn_u%m==0)||(sv_u%m==0)||(sw_u%m==0)||pretty(sa_u,m)||pretty(sn_u,m)||pretty(sv_u,m)||pretty(sw_u,m)
         ||(saU%m==0)||(snU%m==0)||((ku>0)&&(swU==(long)m*ku));
  int s8=(inter==0); int f8=s8||(inter==1);
  *strict=s1+s2+s3+s4+s5+s6+s7+s8; *forked=f1+f2+f3+f4+f5+f6+f7+f8;
}

// terazi (t-paritesi): 57/57 ∧ Σt(çift)=Σa  ve çatalları
static inline int balance(const int *P, int *forked){
  int ne=0; long tE=0,nE=0,aE=0; long SA=0,SN=6555;
  for(int i=0;i<114;i++){ int n=i+1,t=n+P[i]; SA+=P[i]; if((t&1)==0){ ne++; tE+=t; nE+=n; aE+=P[i]; } }
  long tO=(SN+SA)-tE, nO=SN-nE, aO=SA-aE;
  int cnt=(ne==57);
  int strict=cnt&&(tE==SA);
  int f=cnt&&((tE==SA)||(tE==SN)||(tO==SA)||(tO==SN)||(tE==tO)||(aE==aO)||(nE==nO)||(nE==aE)||(nO==aO));
  *forked=f; return strict;
}

static long S_OBS;
typedef struct { char mode; char null; double par; uint64_t seed, trials; uint64_t hs[9], hf[9], hft[9], hbalS, hbalF, hjointS8bal, hjointF8bal; uint64_t T; } job_t;

static void sample(job_t *J, rng_t *r, int *a, long *S, double *key, int *idx, const int *sorted_){
  if(J->null=='U'){ for(int i=113;i>0;i--){ int j=(int)bounded(r,(uint32_t)(i+1)); int tmp=a[i]; a[i]=a[j]; a[j]=tmp; } }
  else if(J->null=='J'){ for(int i=0;i<114;i++){ key[i]=(i+1)+J->par*gauss(r); idx[i]=i; }
    for(int i=1;i<114;i++){ int x=idx[i]; double kx=key[x]; int j=i-1; while(j>=0&&key[idx[j]]>kx){ idx[j+1]=idx[j]; j--; } idx[j+1]=x; }
    for(int k=0;k<114;k++) a[k]=AYAT127[idx[k]]; }
  else if(J->null=='B'){ int w=(int)J->par; memcpy(a,AYAT127,114*sizeof(int));
    for(int s=0;s<114;s+=w){ int e=s+w; if(e>114)e=114; int L=e-s; for(int i=L-1;i>0;i--){ int j=(int)bounded(r,(uint32_t)(i+1)); int tmp=a[s+i]; a[s+i]=a[s+j]; a[s+j]=tmp; } } }
  else if(J->null=='M'){ int thin=(int)J->par; if(thin<1)thin=1;
    for(int st=0;st<thin;st++){ int i=(int)bounded(r,114), j=(int)bounded(r,114); long dS=(long)((i+1)-(j+1))*(a[j]-a[i]); if(*S+dS<=S_OBS){ int tmp=a[i]; a[i]=a[j]; a[j]=tmp; *S+=dS; } } }
}

static void *worker(void *arg){
  job_t *J=(job_t*)arg; rng_t r; rseed(&r,J->seed);
  int a[114]; memcpy(a,AYAT127,sizeof a); double key[114]; int idx[114]; int sorted_[114];
  long S=0;
  if(J->null=='M'){ memcpy(sorted_,AYAT127,sizeof sorted_);
    for(int i=1;i<114;i++){ int x=sorted_[i]; int j=i-1; while(j>=0&&sorted_[j]<x){ sorted_[j+1]=sorted_[j]; j--; } sorted_[j+1]=x; }
    memcpy(a,sorted_,sizeof a); for(int i=0;i<114;i++) S+=(long)(i+1)*a[i];
    for(int b=0;b<30000;b++){ int i=(int)bounded(&r,114), j=(int)bounded(&r,114); long dS=(long)((i+1)-(j+1))*(a[j]-a[i]); if(S+dS<=S_OBS){ int tmp=a[i]; a[i]=a[j]; a[j]=tmp; S+=dS; } } }
  for(uint64_t it=0; it<J->trials; it++){
    sample(J,&r,a,&S,key,idx,sorted_);
    int bf; int bs=balance(a,&bf); if(bs) J->hbalS++; if(bf) J->hbalF++;
    if(J->mode=='x'){ // fixed
      int st,ft,su,fu; score1(a,1,1,19,1,&st,&ft); score1(a,1,1,19,0,&su,&fu);
      J->hs[st]++; J->hft[ft]++; int fm=ft>fu?ft:fu; J->hf[fm]++;
      if(st==8&&bs) J->hjointS8bal++; if(fm==8&&bf) J->hjointF8bal++;
    } else { // free
      int bsS=0,bfF=0;
      for(int f=0;f<8;f++) for(int m=3;m<=40;m++) for(int prim=1;prim>=0;prim--){
        int s,fk; score1(a,FORMS[f][0],FORMS[f][1],m,prim,&s,&fk);
        if(prim && s>bsS) bsS=s; if(fk>bfF) bfF=fk; }
      J->hs[bsS]++; J->hf[bfF]++;
      if(bsS==8){ // katı 8/8'e ulaşan kitabı ve merceğini dök
        for(int f=0;f<8;f++) for(int m=3;m<=40;m++){ int s,fk; score1(a,FORMS[f][0],FORMS[f][1],m,1,&s,&fk); if(s==8){
          flockfile(stdout); printf("KATI8 tohum-iplik=%llu örnek=%llu mercek=(%d,%d,%d) diziliş=",(unsigned long long)J->seed,(unsigned long long)it,FORMS[f][0],FORMS[f][1],m);
          for(int k=0;k<114;k++) printf("%d%s",a[k],k<113?",":"\n"); funlockfile(stdout); } } }
      if(bfF==8&&bf) J->hjointF8bal++; if(bsS==8&&bs) J->hjointS8bal++;
    }
    J->T++;
  }
  return NULL;
}

int main(int argc, char **argv){
  for(int p=2;p<=7;p++){ if(p==4||p==6) continue; long v=(long)p*p*p; while(v<1000000){ PP[nPP++]=v; v*=p; } }
  S_OBS=0; for(int i=0;i<114;i++) S_OBS+=(long)(i+1)*AYAT127[i];
  if(argc>=2 && strcmp(argv[1],"real")==0){
    printf("GERÇEK KİTAP (kodeks 127): en yüksek çatallı puanlı mercekler\n");
    for(int f=0;f<8;f++) for(int m=3;m<=40;m++) for(int prim=1;prim>=0;prim--){
      int s,fk; score1(AYAT127,FORMS[f][0],FORMS[f][1],m,prim,&s,&fk);
      if(fk>=5||s>=4) printf("  çatallı %d/8 katı %d/8  v=%dn+%da m=%d el=%s\n",fk,s,FORMS[f][0],FORMS[f][1],m,prim?"t":"u"); }
    int bf; int bs=balance(AYAT127,&bf); printf("terazi katı=%d çatallı=%d\n",bs,bf); return 0;
  }
  if(argc<6){ fprintf(stderr,"kullanım: %s <fixed|free> <U|J<s>|M<thin>|B<w>> <seed> <trials> <threads>\n",argv[0]); return 1; }
  char mode=(strcmp(argv[1],"fixed")==0)?'x':'f';
  char null=argv[2][0]; double par=(strlen(argv[2])>1)?atof(argv[2]+1):0;
  uint64_t seed=strtoull(argv[3],NULL,10), trials=strtoull(argv[4],NULL,10); int th=atoi(argv[5]);
  job_t *J=calloc(th,sizeof(job_t)); pthread_t *P=calloc(th,sizeof(pthread_t));
  for(int i=0;i<th;i++){ J[i].mode=mode; J[i].null=null; J[i].par=par; J[i].seed=seed*1000003ULL+i*7919ULL+7; J[i].trials=trials/th; pthread_create(&P[i],NULL,worker,&J[i]); }
  uint64_t hs[9]={0},hf[9]={0},hft[9]={0},bS=0,bF=0,jS=0,jF=0,T=0;
  for(int i=0;i<th;i++){ pthread_join(P[i],NULL); for(int c=0;c<9;c++){hs[c]+=J[i].hs[c];hf[c]+=J[i].hf[c];hft[c]+=J[i].hft[c];} bS+=J[i].hbalS; bF+=J[i].hbalF; jS+=J[i].hjointS8bal; jF+=J[i].hjointF8bal; T+=J[i].T; }
  printf("mode=%s null=%s seed=%llu T=%llu\n",argv[1],argv[2],(unsigned long long)seed,(unsigned long long)T);
  printf("  katı   :"); for(int c=0;c<9;c++) printf(" %d:%llu",c,(unsigned long long)hs[c]); printf("  | >=7 %.3e  8/8 %.3e\n",(double)(hs[7]+hs[8])/T,(double)hs[8]/T);
  if(mode=='x'){ printf("  çat-t  :"); for(int c=0;c<9;c++) printf(" %d:%llu",c,(unsigned long long)hft[c]); printf("  | >=7 %.3e  8/8 %.3e\n",(double)(hft[7]+hft[8])/T,(double)hft[8]/T); }
  printf("  çatallı:"); for(int c=0;c<9;c++) printf(" %d:%llu",c,(unsigned long long)hf[c]); printf("  | >=7 %.3e  8/8 %.3e\n",(double)(hf[7]+hf[8])/T,(double)hf[8]/T);
  printf("  terazi katı %.3e  çatallı %.3e | 8/8∧terazi: katı %llu  çatallı %llu\n",(double)bS/T,(double)bF/T,(unsigned long long)jS,(unsigned long long)jF);
  return 0;
}
