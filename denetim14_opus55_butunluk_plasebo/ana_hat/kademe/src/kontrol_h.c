// kontrol_h — kademeli ölçüm (~/opus5.5/kademe): plasebo14 kontrol_b.c'nin kol 1–3 (A, A−, A+) yolunun hızlı ve birebir eşdeğer sürümü.
// Başlıklar (kutuphane.h, sayim.h, agac_uretim.h, tablo.h, dtab.h, dtab_b.h) plasebo14'ün donmuş dosyalarıdır, değiştirilmeden içerilir.
// Kısayol (kesin; başlangıçta her dil için sınanır):
//   Yüklenen her dilde B grubunun tek yüklemi B2'dir ve B2 olmadan dilin sınıf şartı (≥ tmin tür, ≥ dmin düzey) hiçbir mercekte
//   sağlanamaz. B2 biçim düzeyindedir (kdegerlendir'deki mb0; dışlama EXB[L] ancak düşürür). Öyleyse B2'nin tutmadığı biçimlerin
//   mercekleri puan_B'de bütünlüklü olamaz: kdegerlendir yalnız B2'nin tuttuğu biçimlerde çağrılır, kapı G listesi o biçimlerin
//   merceklerinden (artan L sırasıyla) kurulur. k, kB ve aday kayıtları kontrol_b ile aynıdır (ana hat sınaması: bölüt 1–400).
// Kullanım: ./kontrol_h <sifir> <hedef.bin> <diller.txt> <ilk_bolut> <son_bolut> <cikti_dizini> <ref>...
//   diller.txt satırı kontrol_b ile aynı; yalnız kol 1–3 kabul edilir (kol 0, kol 4 ve EL reddedilir).
// Bölüt çıktısı <dizin>/b<bolut>.txt (atomik; biçim kontrol_b ile aynı). Kur'an dilleri (hedef 0) için eşiğin 2 bit altına kadar adaylar
// <dizin>/b<bolut>.q (kontrol_b ile aynı kayıt; yalnız aday varsa yazılır); ayrıca bütünlüklü (bv = 1) bütün Kur'an kontrolleri
// <dizin>/b<bolut>.s (aynı kayıt biçimi; betimsel, bağımsız yeniden sınama için).
// İş parçacığı sayısı ESZAMANLI (≤ 24). Bölüt büyüklüğü 250 000; tohumlar kontrol_b ile aynı (tür 2).
#include "dtab_b.h"
#include <pthread.h>
#include <time.h>
#ifndef BN
#define BN 250000
#endif
#ifndef KONTROL_TUR
#define KONTROL_TUR 2
#endif
#define ADAYMAX 100000
typedef struct __attribute__((packed)) { uint32_t sira; float puan; int16_t L; uint8_t sinif, dil; uint16_t a[114]; } aday_t;
static atablo_t T; static char SIF; static int ND, NH; static dtab_t *DT; static dek_t *DE; static char (*DAD)[16]; static int *DHED; static double *DESIK; static int *DKOL, *DL;
static int16_t (*HED)[114]; static int B1_, B2_; static const char *DIZ; static int SONRAKI; static pthread_mutex_t KILIT=PTHREAD_MUTEX_INITIALIZER;
// B2'nin tuttuğu biçimler (bit f): kdegerlendir'deki e0, sn0, sa0, st0, st ve mb0 B2 satırının aynısı
static inline uint32_t b2_bicimler(const int *a){ uint32_t m=0;
  for(int f=0;f<NFORM;f++){ const int al=AL[f], be=BE[f], abe=abs(be); int e0=0; long sn0=0, sa0=0, st0=0, st=0;
    for(int n=1;n<=114;n++){ const int x=a[n-1]; const int t=al*n+be*x; const int d1=t&1; st+=t; if(!d1){ e0++; sn0+=n; sa0+=x; st0+=t; } }
    const int e1=114-e0; const long sn1=SN_TOT-sn0, sa1=SA_TOT-sa0, st1=st-st0;
    if(e0>0 && e1>0 && ((long)al*sn0==(long)abe*sa1 || (long)al*sn1==(long)abe*sa0 || st0==st1)) m|=1u<<f; }
  return m; }
static void kayit(aday_t *A, uint32_t i, double v, int L, int d, const int *a){ A->sira=i; A->puan=(float)v; A->L=(int16_t)L; A->sinif=1; A->dil=(uint8_t)d;
  for(int q=0;q<114;q++) A->a[q]=(uint16_t)a[q]; }
static void *isci(void *arg){ (void)arg; orn_t *Z=malloc(sizeof(orn_t)); int GL[NLENS]; prof_t *Pp=malloc(sizeof(prof_t));
  long *k=malloc(sizeof(long)*ND), *kc=malloc(sizeof(long)*ND); aday_t *AD=malloc(sizeof(aday_t)*ADAYMAX), *AS=malloc(sizeof(aday_t)*ADAYMAX);
  for(;;){ pthread_mutex_lock(&KILIT); int b=SONRAKI++; pthread_mutex_unlock(&KILIT); if(b>B2_) break;
    char fn[1100], tmp[1100], fq[1100], tq[1100], fs[1100], ts[1100]; snprintf(fn,sizeof fn,"%s/b%04d.txt",DIZ,b); snprintf(tmp,sizeof tmp,"%s.yaziliyor",fn);
    snprintf(fq,sizeof fq,"%s/b%04d.q",DIZ,b); snprintf(tq,sizeof tq,"%s.yaziliyor",fq); snprintf(fs,sizeof fs,"%s/b%04d.s",DIZ,b); snprintf(ts,sizeof ts,"%s.yaziliyor",fs);
    FILE *t=fopen(fn,"r"); if(t){ fclose(t); continue; }
    struct timespec t0,t1; clock_gettime(CLOCK_MONOTONIC,&t0);
    memset(k,0,sizeof(long)*ND); memset(kc,0,sizeof(long)*ND); int nad=0, tasma=0, nas=0, stasma=0; long nb2=0;
    orn_init(Z,SIF,tohum(SIF,KONTROL_TUR,b));
    for(int i=0;i<BN;i++){ orn_sonraki(Z); const uint32_t fm=b2_bicimler(Z->a); if(!fm) continue; nb2++;
      kdegerlendir(Z->a,Pp,fm); int nGL=0;
      for(int f=0;f<NFORM;f++){ if(!(fm>>f&1)) continue; for(int j=0;j<NPR;j++){ const int L=f*NPR+j; if(kapiG(Pp,L)) GL[nGL++]=L; } }
      if(nGL==0) continue;
      for(int d=0;d<ND;d++){ int L, bv; const double v=puan_B(&DT[d],&DE[d],&T,Pp,GL,nGL,&L,&bv); if(!bv) continue; kc[d]++;
        if(v>=DESIK[d]-1e-6) k[d]++;
        if(DHED[d]==0){ if(v>=DESIK[d]-2.0){ if(nad<ADAYMAX) kayit(&AD[nad++],(uint32_t)i,v,L,d,Z->a); else tasma++; }
          if(nas<ADAYMAX) kayit(&AS[nas++],(uint32_t)i,v,L,d,Z->a); else stasma++; } } }
    clock_gettime(CLOCK_MONOTONIC,&t1); const double sn=(t1.tv_sec-t0.tv_sec)+1e-9*(t1.tv_nsec-t0.tv_nsec);
    if(nad){ FILE *g=fopen(tq,"wb"); fwrite(AD,sizeof(aday_t),nad,g); fclose(g); rename(tq,fq); }
    if(nas){ FILE *g=fopen(ts,"wb"); fwrite(AS,sizeof(aday_t),nas,g); fclose(g); rename(ts,fs); }
    FILE *f=fopen(tmp,"w"); fprintf(f,"# sifir %c bolut %d tohum %llu N %d sure_s %.1f aday %d aday_tasma %d sinifli %d sinifli_tasma %d b2 %ld\n",
      SIF,b,(unsigned long long)tohum(SIF,KONTROL_TUR,b),BN,sn,nad,tasma,nas,stasma,nb2);
    for(int d=0;d<ND;d++) fprintf(f,"D %s %ld %ld\n",DAD[d],k[d],kc[d]);
    fclose(f); rename(tmp,fn); }
  free(Z); free(Pp); free(k); free(kc); free(AD); free(AS); return NULL; }
int main(int argc,char**argv){
  if(argc<8){ fprintf(stderr,"kullanım\n"); return 1; }
  SIF=argv[1][0]; B1_=atoi(argv[4]); B2_=atoi(argv[5]); DIZ=argv[6]; SONRAKI=B1_;
  kutuphane_init(); mercek_bedeli_hazirla(); atablo_kur(&T,argv+7,argc-7);
  { FILE *f=fopen(argv[2],"rb"); fseek(f,0,SEEK_END); long sz=ftell(f); fseek(f,0,SEEK_SET); NH=(int)(sz/(4+228)); HED=malloc(sizeof(*HED)*NH);
    for(int h=0;h<NH;h++){ int32_t id; if(fread(&id,4,1,f)!=1||fread(HED[h],2,114,f)!=114||id!=h){ fprintf(stderr,"hedef dosyası\n"); return 2; } } fclose(f); }
  FILE *f=fopen(argv[3],"r"); char ad[64]; int hed, kod, L, n; int cap=64; DT=calloc(cap,sizeof(dtab_t)); DE=calloc(cap,sizeof(dek_t)); DAD=calloc(cap,16); DHED=calloc(cap,sizeof(int));
  DESIK=calloc(cap,sizeof(double)); DKOL=calloc(cap,sizeof(int)); DL=calloc(cap,sizeof(int)); ND=0;
  while(fscanf(f,"%63s %d %d %d %d",ad,&hed,&kod,&L,&n)==5){ int g[DMAX]; if(n>DMAX){ fprintf(stderr,"dil DMAX'ı aşıyor: %s\n",ad); return 2; }
    for(int i=0;i<n;i++) if(fscanf(f,"%d",&g[i])!=1){ fprintf(stderr,"dil satırı\n"); return 2; }
    const int kol=kod/10, ons=kod%10; if(kol<1||kol>3||ons<0||ons>1||!strcmp(ad,"EL")){ fprintf(stderr,"yalnız kol 1–3: %s\n",ad); return 2; }
    if(L<0||n==0){ fprintf(stderr,"dilsiz hedef: %s\n",ad); return 2; }
    if(ND==cap){ fprintf(stderr,"çok dil\n"); return 2; }
    // kısayol sınaması: B grubu = {B2} ve B2'siz kapsam sınıf şartını sağlayamaz
    { uint8_t s=0; int nb=0, b2=0; for(int i=0;i<n;i++){ if(pgrup(g[i])==G_B){ nb++; if(g[i]==GOFS[G_B]+B_B2) b2=1; } else s|=imza_b(g[i]); }
      if(nb!=1 || !b2 || kapsam_tamam_b(s,KOL_TMIN[kol],KOL_DMIN[kol])){ fprintf(stderr,"kısayol bu dil için geçerli değil: %s\n",ad); return 2; } }
    dtab_kur(&DT[ND],&T,g,n,ons); DE[ND].kol=kol; DE[ND].tmin=KOL_TMIN[kol]; DE[ND].dmin=KOL_DMIN[kol];
    snprintf(DAD[ND],16,"%s",ad); DHED[ND]=hed; DKOL[ND]=kol; ND++; }
  fclose(f);
  static prof_t P; int GL[NLENS];
  for(int d=0;d<ND;d++){ int a[114]; for(int q=0;q<114;q++) a[q]=HED[DHED[d]][q]; kdegerlendir(a,&P,0x3FFF); int Lx=-1, bv;   // eşik: kontrol_b ile aynı yol
    const int nGL=kapi_listesi(&P,GL); DESIK[d]=puan_B(&DT[d],&DE[d],&T,&P,GL,nGL,&Lx,&bv);
    if(!bv || DESIK[d]==-INFINITY){ fprintf(stderr,"hedef kendi dilinde bütünlüklü değil: %s\n",DAD[d]); return 2; } DL[d]=Lx; }
  { char fn[1100]; snprintf(fn,sizeof fn,"%s/esikler.txt",DIZ); FILE *g=fopen(fn,"w");
    for(int d=0;d<ND;d++) fprintf(g,"D %s %d %.9f %d kol %d mercek %d\n",DAD[d],DHED[d],DESIK[d],1,DKOL[d],DL[d]);
    fprintf(g,"# dil sayısı %d\n",ND); fclose(g); }
  free(T.E); free(T.HA); T.E=NULL; T.HA=NULL;
  int nt=atoi(getenv("ESZAMANLI")?getenv("ESZAMANLI"):"4"); if(nt<1) nt=1; if(nt>24) nt=24;
  pthread_t th[24]; for(int i=0;i<nt;i++) pthread_create(&th[i],NULL,isci,NULL); for(int i=0;i<nt;i++) pthread_join(th[i],NULL);
  return 0; }
