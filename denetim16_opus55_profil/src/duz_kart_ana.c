// duz_kart_ana.c — duz.c'nin ekran kartı sürümünün ana programı. Olaylar, tohumlar, başlangıç dizilimi (orn_init + orn_sonraki) ve
// başlangıç D'si duz.c ile aynı yoldan hazırlanır; yürüyüş duz_ortak.h'de (kart: duz_kart_cihaz.cu, işlemci: duz_kart_emul.c).
// Çıktı satırları duz.c ile aynı biçimdedir.
// Kullanım: ./duz_kart <sifir> <olaylar.txt> <secim.txt> <wl_adım> <ölçüm_adım> <tekrar> <parça k/n> <çıktı> [ilk_tekrar]
//   wl_adım "k:<c>:<en_az>" biçimindeyse olay başına W = max(en_az, c·düzey²), ölçüm = W/2 (duz.c ile aynı kural).
//   parça k/n: iş sırası q için q mod n = k olan işler (iki kart için iki süreç). ilk_tekrar (varsayılan 1): tekrar r = ilk … ilk+tekrar−1.
#include "kutuphane.h"
#include "tek_mercek.h"
#include "duz_ortak.h"
#include <math.h>
#include <time.h>
int duz_hazirla(const ksabit_t *C, double LQ, const dolay_t *E, int ne, const yur_t *Y, int nw);
int duz_yurut(int64_t parca);
int duz_al(yur_t *Y);
const char *duz_aciklama(void);
typedef struct { char ad[24]; int no; } ad_t;
static int bozuk_h(const dolay_t *E, const int *a){ static prof_t P; kdeg_tek(a,&P,E->L); const int L=E->L; int d=kapiG(&P,L)?0:1;
  d+=__builtin_popcountll(E->om[0]&~(uint64_t)P.B[L])+__builtin_popcountll(E->om[1]&~(uint64_t)P.Z[L])+__builtin_popcountll(E->om[2]&~P.F[L])
    +__builtin_popcountll(E->om[3]&~P.W[L])+__builtin_popcountll(E->om[4]&~(uint64_t)P.H[L]); return d; }
static double logsum(const double *v, int n){ double m=-1e300; for(int i=0;i<n;i++) if(v[i]>m) m=v[i]; double s=0; for(int i=0;i<n;i++) s+=exp(v[i]-m); return m+log(s); }
int main(int argc,char**argv){
  if(argc<9){ fprintf(stderr,"kullanım: duz_kart <sifir> <olaylar> <secim> <wl> <ölçüm> <tekrar> <k/n> <çıktı> [ilk_tekrar]\n"); return 1; }
  const char SIF=argv[1][0]; int64_t WL=0, OL=0; double BC=0; long BMIN=0; if(strchr(argv[4],':')){ if(sscanf(argv[4],"k:%lf:%ld",&BC,&BMIN)!=2) return 1; } else { WL=atoll(argv[4]); OL=atoll(argv[5]); } const int TEK=atoi(argv[6]); int PK=0, PN=1; sscanf(argv[7],"%d/%d",&PK,&PN);
  const int R0=argc>9?atoi(argv[9]):1; kutuphane_init();
  ksabit_t *C=calloc(1,sizeof(ksabit_t)); C->sif=SIF; C->kip=2; C->KE=KAPI_ESIK; C->KH=KAPI_HUCRE;
  for(int q=0;q<114;q++) C->base[q]=(int16_t)BASE[q];
  { int srt[114]; memcpy(srt,BASE,sizeof srt); for(int i=1;i<114;i++){ int x=srt[i]; int j=i-1; while(j>=0&&srt[j]<x){srt[j+1]=srt[j];j--;} srt[j+1]=x; } for(int q=0;q<114;q++) C->srt[q]=(int16_t)srt[q]; }
  C->s_obs=S_OBS; C->sn_tot=(int32_t)SN_TOT; C->sa_tot=(int32_t)SA_TOT;
  memcpy(C->divm,DIVM,sizeof C->divm); memcpy(C->inv32,INV32,sizeof C->inv32);
  for(int q=0;q<NFORM;q++){ C->al[q]=AL[q]; C->be[q]=BE[q]; } for(int q=0;q<NPR;q++) C->pr[q]=PR[q];
  memcpy(C->exb,EXB,sizeof C->exb); memcpy(C->exz,EXZ,sizeof C->exz); memcpy(C->exh,EXH,sizeof C->exh); memcpy(C->exf,EXF,sizeof C->exf); memcpy(C->exw,EXW,sizeof C->exw);
  double LQ=0;
  if(SIF=='L'){ orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,'L',1); LQ=log(Z->q); memcpy(C->thr,Z->thr,sizeof C->thr); memcpy(C->ali,Z->ali,sizeof C->ali);
    for(int q=0;q<114;q++) C->ref[q]=(int16_t)Z->ref[q]; free(Z); }
  else if(SIF!='M' && SIF!='U'){ fprintf(stderr,"sıfır?\n"); return 2; }
  int cap=1<<16; char (*TUM)[24]=calloc(cap,24); int *TL=calloc(cap,sizeof(int)); uint64_t (*TM)[5]=calloc(cap,40); int nt=0;
  { FILE *f=fopen(argv[2],"r"); if(!f){ perror(argv[2]); return 2; } char s[4096];
    while(fgets(s,sizeof s,f)){ char a[64]; int h,L; unsigned long long m[5]; if(sscanf(s,"%63s %d %d %llx %llx %llx %llx %llx",a,&h,&L,&m[0],&m[1],&m[2],&m[3],&m[4])!=8) return 3;
      if(nt==cap) return 4; snprintf(TUM[nt],24,"%s",a); TL[nt]=L; for(int k=0;k<5;k++) TM[nt][k]=m[k]; nt++; } fclose(f); }
  dolay_t *E=calloc(cap,sizeof(dolay_t)); ad_t *AD=calloc(cap,sizeof(ad_t)); int ne=0;
  { FILE *f=fopen(argv[3],"r"); if(!f){ perror(argv[3]); return 2; } char a[64];
    while(fscanf(f,"%63s",a)==1){ int i=0; while(i<nt && strcmp(TUM[i],a)) i++; if(i==nt){ fprintf(stderr,"olay yok: %s\n",a); return 5; }
      dolay_t *e=&E[ne]; e->L=TL[i]; int nc=1; for(int k=0;k<5;k++){ e->om[k]=TM[i][k]; nc+=__builtin_popcountll(TM[i][k]); } if(nc>DKMAX) return 6; e->nl=nc+1;
      snprintf(AD[ne].ad,24,"%s",a); AD[ne].no=i; ne++; } fclose(f); }
  int nw=0; yur_t *Y=calloc((size_t)ne*TEK,sizeof(yur_t)); int *REP=calloc((size_t)ne*TEK,sizeof(int)); orn_t *Z=malloc(sizeof(orn_t)); int q=0;
  for(int r=R0;r<R0+TEK;r++) for(int e=0;e<ne;e++,q++){ if(q%PN!=PK) continue;
    const uint64_t sd=tohum(SIF,9,16L*AD[e].no+r); orn_init(Z,SIF,sd); orn_sonraki(Z); rng_t rg; rseed(&rg,sd^0x5DEECE66DULL);
    yur_t *y=&Y[nw]; memset(y,0,sizeof *y); for(int k=0;k<4;k++) y->r.s[k]=rg.s[k]; long S=0; for(int k=0;k<114;k++){ y->a[k]=(int16_t)Z->a[k]; S+=(long)(k+1)*Z->a[k]; }
    y->S=S; y->D=bozuk_h(&E[e],Z->a); y->e=e; y->min_d=y->D; y->lf=1.0; REP[nw]=r;
    y->WL=WL; y->OL=OL; if(BC>0){ const int nl=E[e].nl; long w=(long)(BC*nl*nl); if(w<BMIN) w=BMIN; y->WL=w; y->OL=w/2; } nw++; }
  if(nw==0){ fprintf(stderr,"iş yok\n"); return 0; }
  if(duz_hazirla(C,LQ,E,ne,Y,nw)){ fprintf(stderr,"duz_hazirla\n"); return 7; }
  FILE *O=fopen(argv[8],"a"); if(!O){ perror(argv[8]); return 2; }
  fprintf(O,"# duz_kart sıfır %c wl %s ölçüm %lld tekrar %d (ilk %d) olay %d yürüyüş %d parça %d/%d; %s\n",SIF,argv[4],(long long)OL,TEK,R0,ne,nw,PK,PN,duz_aciklama()); fflush(O);
  const int64_t PARCA=getenv("DUZ_PARCA")?atoll(getenv("DUZ_PARCA")):200000; int64_t top=0; for(int w=0;w<nw;w++) if(Y[w].WL+Y[w].OL>top) top=Y[w].WL+Y[w].OL; time_t t0=time(NULL);
  for(int64_t s=0;s<top;s+=PARCA){ if(duz_yurut(PARCA)){ fprintf(stderr,"duz_yurut\n"); return 8; }
    if(getenv("DUZ_GUNLUK")) fprintf(stderr,"[%lds] %lld / %lld\n",(long)(time(NULL)-t0),(long long)(s+PARCA),(long long)top); }
  if(duz_al(Y)) return 9;
  for(int w=0;w<nw;w++){ const yur_t *y=&Y[w]; const int nl=E[y->e].nl; double lp[DKMAX+2];
    for(int d=0;d<nl;d++) lp[d]=y->V[d]?y->lg[d]+log((double)y->V[d]):-1e300;
    const double ltop=logsum(lp,nl); const double p0=y->V[0]?exp(lp[0]-ltop):0.0;
    fprintf(O,"%s r %d p %.6e lnf %.2e asama %d en_az_D %d adim %lld | V",AD[y->e].ad,REP[w],p0,y->lf,y->asama,y->min_d,(long long)(y->WL+y->OL)); for(int d=0;d<nl;d++) fprintf(O," %lld",(long long)y->V[d]);
    fprintf(O," | lnpi"); for(int d=0;d<nl;d++) fprintf(O," %.3f",y->V[d]?lp[d]-ltop:-999.0); fprintf(O,"\n"); }
  fclose(O); fprintf(stderr,"bitti: %d yürüyüş, %lds\n",nw,(long)(time(NULL)-t0)); return 0; }
