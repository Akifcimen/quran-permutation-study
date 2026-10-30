// kontrol — kontrol geçişi (ONKAYIT §5): kontrol kitapları aynı sıfırdan, referans ve hedeflerden bağımsız tohumlarla (tür 2) üretilir;
// her kontrol her etkin dilde kendi en iyi merceğini seçer; hedefin eşiği hedefin kendi dilindeki puanıdır.
// Kullanım: ./kontrol <sifir> <hedef.bin> <diller.txt> <ilk_bolut> <son_bolut> <cikti_dizini> <ref>...
//   diller.txt satırı: <ad> <hedef_no> <onsel> <L> <n> <g1> ... <gn>   (ad "EL": el dili, bütün hedeflere karşı; hedef_no -1)
// Bölüt çıktısı <dizin>/b<bolut>.txt (atomik): "D ad k kc" ve "E hedef_no k kc"; Kur'an dilleri (hedef 0) için eşiğin 2 bit altına
// kadar adaylar <dizin>/b<bolut>.q (ikili). İş parçacığı sayısı ESZAMANLI (≤ 4). Bölüt büyüklüğü 250 000.
#include "dtab.h"
#include <pthread.h>
#include <time.h>
#ifndef BN
#define BN 250000
#endif
#ifndef KONTROL_TUR
#define KONTROL_TUR 2           // üretim kontrol tohumları (tür 2); mekanik sınama derlemesi tür 8 kullanır
#endif
typedef struct __attribute__((packed)) { uint32_t sira; float puan; int16_t L; uint8_t sinif, dil; uint16_t a[114]; } aday_t;
static atablo_t T; static char SIF; static int ND, NH; static dtab_t *DT; static char (*DAD)[16]; static int *DHED; static double *DESIK; static int *DSINIF;
static int ELNO=-1; static double *ELESIK; static int *ELSINIF; static int *ELSIRA;     // el dili: hedef eşikleri (artan sıralı)
static int16_t (*HED)[114]; static int B1_, B2_; static const char *DIZ; static int SONRAKI; static pthread_mutex_t KILIT=PTHREAD_MUTEX_INITIALIZER;
static void *isci(void *arg){ (void)arg; static __thread prof_t P; orn_t *Z=malloc(sizeof(orn_t));
  long *k=malloc(sizeof(long)*ND), *kc=malloc(sizeof(long)*ND), *ek=malloc(sizeof(long)*NH), *ekc=malloc(sizeof(long)*NH);
  aday_t *AD=malloc(sizeof(aday_t)*20000);
  for(;;){ pthread_mutex_lock(&KILIT); int b=SONRAKI++; pthread_mutex_unlock(&KILIT); if(b>B2_) break;
    char fn[1100], tmp[1100], fq[1100], tq[1100]; snprintf(fn,sizeof fn,"%s/b%04d.txt",DIZ,b); snprintf(tmp,sizeof tmp,"%s.yaziliyor",fn);
    snprintf(fq,sizeof fq,"%s/b%04d.q",DIZ,b); snprintf(tq,sizeof tq,"%s.yaziliyor",fq);
    FILE *t=fopen(fn,"r"); if(t){ fclose(t); continue; }
    struct timespec t0,t1; clock_gettime(CLOCK_MONOTONIC,&t0);
    memset(k,0,sizeof(long)*ND); memset(kc,0,sizeof(long)*ND); memset(ek,0,sizeof(long)*NH); memset(ekc,0,sizeof(long)*NH); int nad=0, tasma=0;
    pthread_mutex_lock(&KILIT); orn_init(Z,SIF,tohum(SIF,KONTROL_TUR,b)); pthread_mutex_unlock(&KILIT);   // yarış düzeltmesi (29 Eki)
    for(int i=0;i<BN;i++){ orn_sonraki(Z); kdegerlendir(Z->a,&P,0x3FFF);
      for(int d=0;d<ND;d++){
        if(d==ELNO){ int L; const double v=dtab_puan(&DT[d],&T,&P,&L); const int sn=dtab_sinif(&DT[d],&P,L);
          // eşikleri aşan hedefler: ELSIRA artan eşik sırası; v ≥ esik − tol olanların hepsi
          for(int j=0;j<NH;j++){ const int h=ELSIRA[j]; if(v>=ELESIK[h]-1e-6){ ek[h]++; if(sn) ekc[h]++; } else break; }
          if(v>=ELESIK[0]-2.0 && nad<20000){ aday_t *A=&AD[nad++]; A->sira=(uint32_t)i; A->puan=(float)v; A->L=(int16_t)L; A->sinif=(uint8_t)sn; A->dil=(uint8_t)d; for(int q=0;q<114;q++) A->a[q]=(uint16_t)Z->a[q]; }
          else if(v>=ELESIK[0]-2.0) tasma++;
          continue; }
        if(DHED[d]==0){ int L; const double v=dtab_puan(&DT[d],&T,&P,&L);           // Kur'an dilleri: tam puan ve adaylar
          if(v>=DESIK[d]-1e-6){ k[d]++; if(dtab_sinif(&DT[d],&P,L)) kc[d]++; }
          if(v>=DESIK[d]-2.0){ if(nad<20000){ aday_t *A=&AD[nad++]; A->sira=(uint32_t)i; A->puan=(float)v; A->L=(int16_t)L; A->sinif=(uint8_t)dtab_sinif(&DT[d],&P,L); A->dil=(uint8_t)d;
              for(int q=0;q<114;q++) A->a[q]=(uint16_t)Z->a[q]; } else tasma++; }
          continue; }
        if(dtab_asar(&DT[d],&T,&P,DESIK[d])){ int L; dtab_puan(&DT[d],&T,&P,&L); k[d]++; if(dtab_sinif(&DT[d],&P,L)) kc[d]++; } } }
    clock_gettime(CLOCK_MONOTONIC,&t1); const double sn=(t1.tv_sec-t0.tv_sec)+1e-9*(t1.tv_nsec-t0.tv_nsec);
    FILE *g=fopen(tq,"wb"); fwrite(AD,sizeof(aday_t),nad,g); fclose(g); rename(tq,fq);
    FILE *f=fopen(tmp,"w"); fprintf(f,"# sifir %c bolut %d tohum %llu N %d sure_s %.1f aday %d aday_tasma %d\n",SIF,b,(unsigned long long)tohum(SIF,KONTROL_TUR,b),BN,sn,nad,tasma);
    for(int d=0;d<ND;d++) if(d!=ELNO) fprintf(f,"D %s %ld %ld\n",DAD[d],k[d],kc[d]);
    if(ELNO>=0) for(int h=0;h<NH;h++) fprintf(f,"E %d %ld %ld\n",h,ek[h],ekc[h]);
    fclose(f); rename(tmp,fn); }
  free(Z); free(k); free(kc); free(ek); free(ekc); free(AD); return NULL; }
static int esik_kiyas(const void *a, const void *b){ const int x=*(const int*)a, y=*(const int*)b; if(ELESIK[x]<ELESIK[y]) return -1; if(ELESIK[x]>ELESIK[y]) return 1; return x<y?-1:(x>y); }
int main(int argc,char**argv){
  if(argc<8){ fprintf(stderr,"kullanım\n"); return 1; }
  SIF=argv[1][0]; B1_=atoi(argv[4]); B2_=atoi(argv[5]); DIZ=argv[6]; SONRAKI=B1_;
  kutuphane_init(); mercek_bedeli_hazirla(); atablo_kur(&T,argv+7,argc-7);
  { FILE *f=fopen(argv[2],"rb"); fseek(f,0,SEEK_END); long sz=ftell(f); fseek(f,0,SEEK_SET); NH=(int)(sz/(4+228)); HED=malloc(sizeof(*HED)*NH);
    for(int h=0;h<NH;h++){ int32_t id; if(fread(&id,4,1,f)!=1||fread(HED[h],2,114,f)!=114||id!=h){ fprintf(stderr,"hedef dosyası\n"); return 2; } } fclose(f); }
  // dilleri oku
  FILE *f=fopen(argv[3],"r"); char ad[64]; int hed, ons, L, n; int cap=1024; DT=calloc(cap,sizeof(dtab_t)); DAD=calloc(cap,16); DHED=calloc(cap,sizeof(int)); DESIK=calloc(cap,sizeof(double)); DSINIF=calloc(cap,sizeof(int));
  size_t top=0; ND=0;
  while(fscanf(f,"%63s %d %d %d %d",ad,&hed,&ons,&L,&n)==5){ int g[DMAX]; for(int i=0;i<n;i++) if(fscanf(f,"%d",&g[i])!=1){ fprintf(stderr,"dil satırı\n"); return 2; }
    if(ND==cap){ fprintf(stderr,"çok dil\n"); return 2; }
    top+=dtab_bayt(&T,g,n); if(top>(size_t)3500*1024*1024){ fprintf(stderr,"dil tabloları bellek sınırını aşıyor (%zu MB)\n",top>>20); return 3; }
    dtab_kur(&DT[ND],&T,g,n,ons); snprintf(DAD[ND],16,"%s",ad); DHED[ND]=hed; if(!strcmp(ad,"EL")) ELNO=ND; ND++; }
  fclose(f);
  // eşikler: hedefin kendi dilindeki puanı (aynı tablo ve aynı işlevle)
  static prof_t P;
  for(int d=0;d<ND;d++){ if(d==ELNO) continue; int a[114]; for(int q=0;q<114;q++) a[q]=HED[DHED[d]][q]; kdegerlendir(a,&P,0x3FFF); int Lx; DESIK[d]=dtab_puan(&DT[d],&T,&P,&Lx); DSINIF[d]=dtab_sinif(&DT[d],&P,Lx); }
  if(ELNO>=0){ ELESIK=malloc(sizeof(double)*NH); ELSINIF=malloc(sizeof(int)*NH); ELSIRA=malloc(sizeof(int)*NH);
    for(int h=0;h<NH;h++){ int a[114]; for(int q=0;q<114;q++) a[q]=HED[h][q]; kdegerlendir(a,&P,0x3FFF); int Lx; ELESIK[h]=dtab_puan(&DT[ELNO],&T,&P,&Lx); ELSINIF[h]=dtab_sinif(&DT[ELNO],&P,Lx); ELSIRA[h]=h; }
    qsort(ELSIRA,NH,sizeof(int),esik_kiyas); }
  { char fn[1100]; snprintf(fn,sizeof fn,"%s/esikler.txt",DIZ); FILE *g=fopen(fn,"w");
    for(int d=0;d<ND;d++) if(d!=ELNO) fprintf(g,"D %s %d %.9f %d\n",DAD[d],DHED[d],DESIK[d],DSINIF[d]);
    if(ELNO>=0) for(int h=0;h<NH;h++) fprintf(g,"E %d %.9f %d\n",h,ELESIK[h],ELSINIF[h]);
    fprintf(g,"# dil sayısı %d, tablo belleği %zu MB\n",ND,top>>20); fclose(g); }
  free(T.E); free(T.HA); T.E=NULL; T.HA=NULL;
  int nt=atoi(getenv("ESZAMANLI")?getenv("ESZAMANLI"):"4"); if(nt<1) nt=1; if(nt>4) nt=4;
  pthread_t th[4]; for(int i=0;i<nt;i++) pthread_create(&th[i],NULL,isci,NULL); for(int i=0;i<nt;i++) pthread_join(th[i],NULL);
  return 0; }
