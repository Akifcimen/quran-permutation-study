// Ö0(a) — dokuzuncu incelemenin deneyinin yeniden üretimi (bizim taraf).
// Robotun kendi tablosunu (robot4_m table, ROBOT_MODS=19) okur; robotun score işçilerinin tohum düzeniyle aynı değerlendirme
// kitaplarını (bit-birebir örnekleyici portu) yeniden üretir; robotun sabit-mercek T2 çıktısıyla (sütun 4) karşılaştırır ve
// üç puan sayar: (1) özgün T2, (2) dokuzuncu incelemenin tek düzeltmesi (G-çapraz düğümünde kazanan "G: Σn=Σa" ise
// G-toplam düğümünde "G: m²|Σq" puan almaz), (3) Ö0(c)'nin tekrarsız T2*'ı (kurallar.h L0).
// Kullanım: ./tekrar9 <kodeks> <null U|J10|M20> <seed> <N> <th> <tablo.bin> <robot_sifir.f32|-> <kitap_kapali 0|1> <cikti_prefix>
#include "kurallar.h"
#define NLENS_R 646
#define LI_Q_R 16
static int rob_tab_oku(const char *fn, tablo_t *T){
  FILE *f=fopen(fn,"rb"); if(!f) return 0; uint64_t N; fread(&N,8,1,f); T->N=N;
  size_t hs=(size_t)NLENS_R*NF*NCTX; uint64_t *H=malloc(8*hs); fread(H,8,hs,f);
  uint64_t *TF=malloc(8*NLENS_R*NCTX), *TG=malloc(8*NLENS_R*NCTX); fread(TF,8,NLENS_R*NCTX,f); fread(TG,8,NLENS_R*NCTX,f); fclose(f);
  for(int ff=0;ff<NF;ff++) for(int c=0;c<NCTX;c++) T->hit[ff][c]=H[((size_t)LI_Q_R*NF+ff)*NCTX+c];
  for(int c=0;c<NCTX;c++){ T->trF[c]=TF[LI_Q_R*NCTX+c]; T->trG[c]=TG[LI_Q_R*NCTX+c]; }
  free(H); free(TF); free(TG); return 1; }
// dokuzuncu incelemenin tek düzeltmesi
static double t2_fix9(const tablo_t *T, const fres_t *R, const kurallar_t *K){
  u128 b=R->bits & ~K->kapali; if(R->same){ for(int f=0;f<NFACT;f++){ int n=FNODE[f]; if((n>=6&&n<=11)||n==14) b&=~BIT(f); } }
  double nodev[NNODE]; int nodef[NNODE]; for(int i=0;i<NNODE;i++){ nodev[i]=0; nodef[i]=-1; } double x[NF];
  u128 bb=b; while(bb){ int f=ctz128(bb); bb&=bb-1; double p=pfact_t(T,f,R,0); double v=-log10(p); if(v<0) v=0; x[f]=v; if(nodef[FNODE[f]]<0||v>nodev[FNODE[f]]){ nodev[FNODE[f]]=v; nodef[FNODE[f]]=f; } }
  if(nodef[8]==X_NA[1]){ double m=0; u128 c=b; while(c){ int f=ctz128(c); c&=c-1; if(FNODE[f]==7 && f!=S_M2[1] && x[f]>m) m=x[f]; } nodev[7]=m; }
  double s[NNODE]; memcpy(s,nodev,sizeof s); for(int i=1;i<NNODE;i++){ double y=s[i]; int j=i-1; while(j>=0&&s[j]<y){ s[j+1]=s[j]; j--; } s[j+1]=y; }
  double t=0; for(int i=0;i<8;i++) t+=s[i]; return t; }
int main(int argc, char **argv){
  if(argc<10){ fprintf(stderr,"kullanım bkz. kaynak\n"); return 1; }
  int kod=atoi(argv[1]); init_ortak(kod); char null=argv[2][0]; double par=strlen(argv[2])>1?atof(argv[2]+1):0;
  uint64_t seed=strtoull(argv[3],NULL,10), N=strtoull(argv[4],NULL,10); int th=atoi(argv[5]); int kk=atoi(argv[8]);
  tablo_t *T=calloc(1,sizeof(tablo_t)); if(!rob_tab_oku(argv[6],T)){ fprintf(stderr,"tablo?\n"); return 1; }
  kurallar_t K; kurallar_kur(&K,'-',kk);
  // keşif (EK): IDDIA=1 → iddia bataryası (robot_m'nin 60 olgusu = defter bataryasının 0–14. düğümleri; 15–22. düğümler kapalı)
  if(getenv("IDDIA")&&atoi(getenv("IDDIA"))){ for(int f=0;f<NFACT;f++) if(FNODE[f]>=15) K.kapali|=BIT(f); }
  FILE *fr=NULL; if(strcmp(argv[7],"-")) fr=fopen(argv[7],"rb");
  fres_t RQ; facts19(BASE,&RQ,0); puan_t PQ; puanla(T,&RQ,0,&K,&PQ); double fQ=t2_fix9(T,&RQ,&K);
  long k_or=0,k_fx=0,k_tt=0, n=0; double maxd=0; long nfark=0; long n_dup=0;
  uint64_t per=N/th; float *buf=malloc(sizeof(float)*9*per);
  for(int i=0;i<th;i++){ sampler_t *Z=malloc(sizeof(sampler_t)); sampler_init(Z,null,par,seed*1000003ULL+i*7919ULL+5);
    if(fr){ fseek(fr,(long)sizeof(float)*9*per*i,SEEK_SET); fread(buf,sizeof(float),9*per,fr); }
    for(uint64_t it=0; it<per; it++){ sampler_next(Z); fres_t R; facts19(Z->a,&R,0); puan_t P; puanla(T,&R,0,&K,&P); double fx=t2_fix9(T,&R,&K);
      if(fr){ double d=fabs((double)buf[it*9+4]-P.t2r); if(d>maxd) maxd=d; if(d>1e-3) nfark++; }
      if(P.t2r>=PQ.t2r-1e-6) k_or++; if(fx>=fQ-1e-6) k_fx++; if(P.t2t>=PQ.t2t-1e-6) k_tt++; if(P.t2t<P.t2r-1e-9) n_dup++; n++; }
    free(Z); }
  char fn[512]; snprintf(fn,512,"%s.txt",argv[9]); FILE *fo=fopen(fn,"w");
  fprintf(fo,"kodeks %d null %s seed %llu N %ld kitap_kapali %d iddia %d | Kuran T2=%.4f T2fix9=%.4f T2*=%.4f | k_ozgun %ld k_fix9 %ld k_tekrarsiz %ld | p_ozgun %.4e p_fix9 %.4e p_tekrarsiz %.4e | T2*<T2 kitap %ld | robot farkı maks %.2e (>1e-3: %ld)\n",
    kod,argv[2],(unsigned long long)seed,n,kk,getenv("IDDIA")?atoi(getenv("IDDIA")):0,PQ.t2r,fQ,PQ.t2t,k_or,k_fx,k_tt,(k_or+1.0)/(n+1.0),(k_fx+1.0)/(n+1.0),(k_tt+1.0)/(n+1.0),n_dup,maxd,nfark);
  fclose(fo); return 0; }
