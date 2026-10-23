// kosullu_puan — Ö1 puanlaması: bir koşullu kitap havuzu (bölüt dosyaları) için LOO tablo, robotun T2'si ve tekrarsız T2*;
// Kur'an'ın sırası p = (k+1)/(N+1), k = #{T2*_i ≥ T2*_Kur'an − 1e-6}.
// Kullanım: ./kosullu_puan <kodeks> <kosul O|P> <kitap_kapali 0|1> <cikti_prefix> <kayit.bin>...
//   kosul O: her kitap O = O1∧O2∧O3'ü sağlamalı; P (= O'): ayrıca O4 (terazi, A); '-': koşulsuz (yalnız kod sınaması).
// Çıktılar: <prefix>.ozet.txt (özet), <prefix>.puan.bin (her kitap: t2r, t2t float64; hik_r, hik_t u128; kaynak dosya/sıra),
//           <prefix>.kuran.txt (Kur'an'ın hikâyesi, olgu olasılıkları).
#include "kurallar.h"
typedef struct __attribute__((packed)) { uint16_t a[114]; uint32_t f; uint32_t zincir; uint64_t sira; } kayit_t;
typedef struct __attribute__((packed)) { double t2r, t2t; uint64_t hr_lo, hr_hi, ht_lo, ht_hi; uint32_t dosya; uint32_t sira; } cikti_t;
static int cmpd(const void *a, const void *b){ double x=*(const double*)a, y=*(const double*)b; return (x<y)?-1:(x>y); }
// Clopper–Pearson
static double betainc_cf(double a,double b,double x){ double qab=a+b,qap=a+1,qam=a-1,c=1,d=1-qab*x/qap; if(fabs(d)<1e-300)d=1e-300; d=1/d; double h=d;
  for(int m=1;m<=300;m++){ int m2=2*m; double aa=m*(b-m)*x/((qam+m2)*(a+m2)); d=1+aa*d; if(fabs(d)<1e-300)d=1e-300; c=1+aa/c; if(fabs(c)<1e-300)c=1e-300; d=1/d; h*=d*c;
    aa=-(a+m)*(qab+m)*x/((a+m2)*(qap+m2)); d=1+aa*d; if(fabs(d)<1e-300)d=1e-300; c=1+aa/c; if(fabs(c)<1e-300)c=1e-300; d=1/d; double del=d*c; h*=del; if(fabs(del-1)<1e-12) break; } return h; }
static double ibeta(double a,double b,double x){ if(x<=0) return 0; if(x>=1) return 1; double bt=exp(lgamma(a+b)-lgamma(a)-lgamma(b)+a*log(x)+b*log(1-x));
  if(x<(a+1)/(a+b+2)) return bt*betainc_cf(a,b,x)/a; return 1-bt*betainc_cf(b,a,1-x)/b; }
static double cp_q(double a,double b,double p){ double lo=0,hi=1; for(int i=0;i<200;i++){ double m=0.5*(lo+hi); if(ibeta(a,b,m)<p) lo=m; else hi=m; } return 0.5*(lo+hi); }
static void cp(long k,long n,double *lo,double *hi){ *lo=(k==0)?0:cp_q(k,n-k+1,0.025); *hi=(k==n)?1:cp_q(k+1,n-k,0.975); }
static void hikaye_yaz(FILE *f, const tablo_t *T, const fres_t *R, int loo, u128 h){ u128 b=h; while(b){ int ff=ctz128(b); b&=b-1; double p=pfact_t(T,ff,R,loo); fprintf(f,"      [%-17s] %-44s p=%.3e (−log10=%.3f)\n",NODENAME[FNODE[ff]],FNAME[ff],p,-log10(p)); } }
int main(int argc, char **argv){
  if(argc<6){ fprintf(stderr,"kullanım: %s <kodeks> <O|P> <kitap_kapali> <prefix> <kayit.bin>...\n",argv[0]); return 1; }
  int kod=atoi(argv[1]); char kos=argv[2][0]; int kk=atoi(argv[3]); const char *pre=argv[4]; init_ortak(kod);
  kurallar_t K; kurallar_kur(&K,kos,kk);
  // yükle
  long cap=1<<20, N=0; int16_t (*A)[114]=malloc(sizeof(*A)*cap); uint32_t *dsy=malloc(4*cap), *sira_=malloc(4*cap);
  for(int d=5; d<argc; d++){ FILE *fi=fopen(argv[d],"rb"); if(!fi){ fprintf(stderr,"açılamadı %s\n",argv[d]); return 1; } kayit_t r; uint32_t s=0;
    while(fread(&r,sizeof r,1,fi)==1){ if(N==cap){ cap*=2; A=realloc(A,sizeof(*A)*cap); dsy=realloc(dsy,4*cap); sira_=realloc(sira_,4*cap); } for(int i=0;i<114;i++) A[N][i]=(int16_t)r.a[i]; dsy[N]=d-5; sira_[N]=s++; N++; } fclose(fi); }
  fres_t *R=malloc(sizeof(fres_t)*N); tablo_t *T=calloc(1,sizeof(tablo_t)); long kosul_hata=0, kapali_hata=0;
  for(long i=0;i<N;i++){ int a[114]; for(int j=0;j<114;j++) a[j]=A[i][j]; int f=olaylar(a); if(!OK_O(f) || (kos=='P' && !(f&EV_A))) kosul_hata++;
    if(kos=='-') kosul_hata=0;
    facts19(a,&R[i],0); if((R[i].bits & K.kapali_kosul)!=K.kapali_kosul) kapali_hata++; tablo_ekle(T,&R[i]); }
  if(kosul_hata||kapali_hata){ fprintf(stderr,"HATA: koşul tutmayan %ld, koşulun gerektirdiği olgusu eksik %ld kitap\n",kosul_hata,kapali_hata); return 2; }
  // Kur'an
  fres_t RQ; facts19(BASE,&RQ,0); puan_t PQ; puanla(T,&RQ,0,&K,&PQ);
  cikti_t *O=malloc(sizeof(cikti_t)*N); double *v2r=malloc(8*N), *v2t=malloc(8*N); long kr=0, kt=0, ndup=0;
  for(long i=0;i<N;i++){ puan_t P; puanla(T,&R[i],1,&K,&P); O[i].t2r=P.t2r; O[i].t2t=P.t2t; O[i].hr_lo=(uint64_t)P.hik_r; O[i].hr_hi=(uint64_t)(P.hik_r>>64); O[i].ht_lo=(uint64_t)P.hik_t; O[i].ht_hi=(uint64_t)(P.hik_t>>64); O[i].dosya=dsy[i]; O[i].sira=sira_[i];
    v2r[i]=P.t2r; v2t[i]=P.t2t; if(P.t2r>=PQ.t2r-1e-6) kr++; if(P.t2t>=PQ.t2t-1e-6) kt++; if(P.t2t<P.t2r-1e-9) ndup++; }
  char fn[600]; snprintf(fn,600,"%s.puan.bin",pre); FILE *fo=fopen(fn,"wb"); fwrite(O,sizeof(cikti_t),N,fo); fclose(fo);
  snprintf(fn,600,"%s.ozet.txt",pre); FILE *fs=fopen(fn,"w");
  double lo,hi; fprintf(fs,"kodeks %d, koşul %s, terazi olguları %s, N = %ld koşullu kitap (%d dosya)\n",kod,kos=='O'?"O = O1∧O2∧O3":"O' = O∧O4",kk?"kapalı":"AÇIK (duyarlılık)",N,argc-5);
  fprintf(fs,"Kur'an: robot T2 = %.4f, tekrarsız T2* = %.4f\n",PQ.t2r,PQ.t2t);
  cp(kt,N,&lo,&hi); fprintf(fs,"T2* (birincil): k = %ld → p = (k+1)/(N+1) = %.4e ; k/N %%95 Clopper–Pearson [%.3e, %.3e]\n",kt,(kt+1.0)/(N+1.0),lo,hi);
  cp(kr,N,&lo,&hi); fprintf(fs,"robot T2 (karşılaştırma): k = %ld → p = %.4e ; k/N %%95 [%.3e, %.3e]\n",kr,(kr+1.0)/(N+1.0),lo,hi);
  fprintf(fs,"T2* < T2 olan (tekrarı ayıklanan) kitap: %ld (%.2f%%)\n",ndup,100.0*ndup/N);
  qsort(v2r,N,8,cmpd); qsort(v2t,N,8,cmpd); double qq[6]={0.5,0.9,0.99,0.999,0.9999,1.0};
  fprintf(fs,"dağılım T2*: "); for(int j=0;j<6;j++){ long ix=(long)(qq[j]*(N-1)); fprintf(fs,"q%.4g=%.3f ",qq[j],v2t[ix]); } fprintf(fs,"\n");
  fprintf(fs,"dağılım T2 : "); for(int j=0;j<6;j++){ long ix=(long)(qq[j]*(N-1)); fprintf(fs,"q%.4g=%.3f ",qq[j],v2r[ix]); } fprintf(fs,"\n");
  fprintf(fs,"Kur'an'ın tekrarsız hikâyesi:\n"); hikaye_yaz(fs,T,&RQ,0,PQ.hik_t);
  // en yüksek 40 kitap (T2*)
  long *ix=malloc(8*N); for(long i=0;i<N;i++) ix[i]=i;
  for(int t=0;t<40&&t<N;t++){ long b=t; for(long i=t+1;i<N;i++) if(O[ix[i]].t2t>O[ix[b]].t2t) b=i; long tmp=ix[t]; ix[t]=ix[b]; ix[b]=tmp; }
  fprintf(fs,"\nT2*'a göre en yüksek 40 kitap (dosya/sıra, T2*, T2):\n");
  for(int t=0;t<40&&t<N;t++){ long i=ix[t]; fprintf(fs,"  #%d  %u/%u  T2*=%.3f  T2=%.3f\n",t+1,O[i].dosya,O[i].sira,O[i].t2t,O[i].t2r); u128 h=((u128)O[i].ht_hi<<64)|O[i].ht_lo; hikaye_yaz(fs,T,&R[i],1,h); }
  fclose(fs);
  snprintf(fn,600,"%s.kuran.txt",pre); FILE *fq=fopen(fn,"w"); fprintf(fq,"%.6f %.6f %016llx%016llx %016llx%016llx\n",PQ.t2r,PQ.t2t,(unsigned long long)(uint64_t)(PQ.hik_r>>64),(unsigned long long)(uint64_t)PQ.hik_r,(unsigned long long)(uint64_t)(PQ.hik_t>>64),(unsigned long long)(uint64_t)PQ.hik_t);
  for(int n=0;n<NNODE;n++) if(PQ.nodev[n]>0) fprintf(fq,"  düğüm %-17s %.4f\n",NODENAME[n],PQ.nodev[n]); fclose(fq);
  printf("N=%ld  Kur'an T2*=%.3f  k=%ld  p=%.3e\n",N,PQ.t2t,kt,(kt+1.0)/(N+1.0)); return 0; }
