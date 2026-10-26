// Independent Monte Carlo check of P(C) and P(C∩W) under the 6-block null (natural n±a, p = 19).
// C: four (t-parity × n-parity) cells of F = {19 | n+a} have equal positive counts; one t-row has equal q-sums b,b and the
//    other row b−1,b+1 (either order); distinct q values form a consecutive band and some cell with ≥2 members has all-equal q;
//    Σq = 4·19.  W: F ∩ H = ∅ (H = {19 | n−a}), |F ∪ H| = 19, 19 | Σa(F ∪ H).
// Usage: blok6_cw <counts file> <samples> <seed> <threads>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

#define NB 19
#define NPERM 720
typedef struct { int nf; int q[6]; int cell[6]; int fa; int nh; int ha; int ov; int ova; } blk_t;
static blk_t TAB[NB][NPERM];
static int A[114];
static long long NS; static uint64_t SEED; static int NT;

static inline uint64_t rotl(const uint64_t x, int k){ return (x << k) | (x >> (64 - k)); }
typedef struct { uint64_t s[4]; } rng_t;
static inline uint64_t nextr(rng_t *r){ uint64_t *s=r->s; const uint64_t res = rotl(s[1]*5,7)*9; const uint64_t t = s[1]<<17;
  s[2]^=s[0]; s[3]^=s[1]; s[1]^=s[2]; s[0]^=s[3]; s[2]^=t; s[3]=rotl(s[3],45); return res; }
static uint64_t splitmix(uint64_t *x){ uint64_t z=(*x+=0x9e3779b97f4a7c15ULL); z=(z^(z>>30))*0xbf58476d1ce4e5b9ULL; z=(z^(z>>27))*0x94d049bb133111ebULL; return z^(z>>31); }

static int PERMS[NPERM][6];
static void gen_perms(void){ int idx=0; int p[6]={0,1,2,3,4,5};
  // Heap's algorithm alternative: iterate all 6^6 and keep permutations
  for(int a=0;a<6;a++)for(int b=0;b<6;b++)for(int c=0;c<6;c++)for(int d=0;d<6;d++)for(int e=0;e<6;e++)for(int f=0;f<6;f++){
    int v[6]={a,b,c,d,e,f}, seen=0, ok=1; for(int i=0;i<6;i++){ if(seen&(1<<v[i])){ok=0;break;} seen|=1<<v[i]; }
    if(ok){ memcpy(PERMS[idx++],v,sizeof v); } }
  (void)p; if(idx!=NPERM){ fprintf(stderr,"perm count %d\n",idx); exit(1);} }

static void build(void){
  for(int b=0;b<NB;b++) for(int k=0;k<NPERM;k++){
    blk_t *t=&TAB[b][k]; memset(t,0,sizeof *t);
    for(int i=0;i<6;i++){ int n=6*b+i+1; int a=A[6*b+PERMS[k][i]]; int tp=n+a, tm=n-a;
      int inF = (tp%19==0), inH = (tm%19==0);
      if(inF){ t->q[t->nf]=tp/19; t->cell[t->nf]=(tp&1)*2+(n&1); t->nf++; t->fa+=a; }
      if(inH){ t->nh++; t->ha+=a; }
      if(inF&&inH){ t->ov++; t->ova+=a; } } } }

typedef struct { int id; long long n; long long c, cw, eq, pat, band, cen, w; } job_t;
static void *work(void *arg){
  job_t *J=(job_t*)arg; rng_t r; uint64_t x=SEED*1000003ULL+(uint64_t)J->id*7919ULL+1; for(int i=0;i<4;i++) r.s[i]=splitmix(&x);
  for(long long it=0; it<J->n; it++){
    int celln[4]={0,0,0,0}, cells[4]={0,0,0,0}, lo[4]={1000,1000,1000,1000}, hi[4]={-1,-1,-1,-1};
    int count=0, fa=0, nh=0, ha=0, ov=0, ova=0; uint32_t seen=0; int qmin=1000,qmax=-1;
    for(int b=0;b<NB;b++){ uint64_t rr=nextr(&r); int k=(int)(((unsigned __int128)rr*NPERM)>>64); const blk_t *t=&TAB[b][k];
      for(int j=0;j<t->nf;j++){ int q=t->q[j], c=t->cell[j]; celln[c]++; cells[c]+=q; if(q<lo[c])lo[c]=q; if(q>hi[c])hi[c]=q;
        seen|=1u<<q; if(q<qmin)qmin=q; if(q>qmax)qmax=q; }
      count+=t->nf; fa+=t->fa; nh+=t->nh; ha+=t->ha; ov+=t->ov; ova+=t->ova; }
    int equal = celln[0]>0 && celln[0]==celln[1] && celln[0]==celln[2] && celln[0]==celln[3];
    #define ROW(a,b,c,d) ((a)==(b) && (((c)==(a)-1 && (d)==(a)+1) || ((c)==(a)+1 && (d)==(a)-1)))
    int pattern = ROW(cells[0],cells[1],cells[2],cells[3]) || ROW(cells[2],cells[3],cells[0],cells[1]);
    int uniform=0; for(int c=0;c<4;c++) if(celln[c]>=2 && lo[c]==hi[c]) uniform=1;
    int band = count>0 && __builtin_popcount(seen)==qmax-qmin+1 && uniform;
    int centre = (cells[0]+cells[1]+cells[2]+cells[3])==76;
    int w = (ov==0 && count+nh-ov==19) && ((fa+ha-ova)%19==0);
    J->eq+=equal; J->pat+=pattern; J->band+=band; J->cen+=centre; J->w+=w;
    if(equal&&pattern&&band&&centre){ J->c++; if(w) J->cw++; } }
  return NULL; }

int main(int argc,char**argv){
  if(argc<5){ fprintf(stderr,"usage\n"); return 1; }
  FILE *f=fopen(argv[1],"r"); for(int i=0;i<114;i++) if(fscanf(f,"%d",&A[i])!=1){ fprintf(stderr,"read\n"); return 1; } fclose(f);
  NS=atoll(argv[2]); SEED=strtoull(argv[3],0,10); NT=atoi(argv[4]);
  gen_perms(); build();
  // sanity: identity permutation index 0 must reproduce the observed book
  { int cnt=0; for(int b=0;b<NB;b++) cnt+=TAB[b][0].nf; fprintf(stderr,"observed |F| via identity perms = %d\n",cnt); }
  pthread_t th[64]; job_t J[64]; long long per=NS/NT;
  for(int i=0;i<NT;i++){ memset(&J[i],0,sizeof J[i]); J[i].id=i; J[i].n=(i==NT-1)?NS-per*(NT-1):per; pthread_create(&th[i],0,work,&J[i]); }
  long long c=0,cw=0,eq=0,pat=0,band=0,cen=0,w=0; for(int i=0;i<NT;i++){ pthread_join(th[i],0); c+=J[i].c; cw+=J[i].cw; eq+=J[i].eq; pat+=J[i].pat; band+=J[i].band; cen+=J[i].cen; w+=J[i].w; }
  printf("N=%lld seed=%llu  C=%lld (%.6e)  C∩W=%lld (%.6e)\n", NS,(unsigned long long)SEED, c,(double)c/NS, cw,(double)cw/NS);
  printf("marginals: equal %.4e  pattern %.4e  band %.4e  centre %.4e  W %.4e\n",(double)eq/NS,(double)pat/NS,(double)band/NS,(double)cen/NS,(double)w/NS);
  return 0; }
