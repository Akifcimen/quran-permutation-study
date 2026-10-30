// kontrol_b — plasebo14 kontrol geçişi (ONKAYIT §5). plasebo12'nin kontrol.c'sinden türetildi; kol 0 dilleri (3. sütun 0/1) için
// davranış plasebo12 ile birebir aynıdır (Ö0 sınaması). Kontrol kitapları aynı sıfırdan, referans ve hedeflerden bağımsız tohumlarla
// (tür 2; plasebo12 ile aynı akış) üretilir; her kontrol, hedefin donmuş dilinde kendi en iyi merceğini seçer.
//   kol 1–3 (A, A−, A+): bütünlüklü puan, yalnız B'nin tuttuğu mercekler (dtab_b.h puan_B); B hiçbir mercekte tutmuyorsa "aşmaz".
//     Çıktı "D ad k kB": k = eşiği aşan/eşleyen kontrol, kB = Λ altında en az bir mercekte bütünlüklü kontrol (betimsel).
//   kol 4 (B): plasebo12 puanı (bütün mercekler), boy sınırıyla kesin budama; "D ad k kc", kc = aşanlardan plasebo12 sınıf kuralına
//     uyanlar (betimsel).
// Kullanım: ./kontrol_b <sifir> <hedef.bin> <diller.txt> <ilk_bolut> <son_bolut> <cikti_dizini> <ref>...
//   diller.txt satırı: <ad> <hedef_no> <onsel+10·kol> <L> <n> <g1> ... <gn>   (ad "EL": el dili, kol 0; hedef_no −1)
//   KONTROL_BELLEK=1 ortam değişkeniyle yalnız dil başına tablo belleği basılır ("bayt <ad> <bayt>") ve çıkılır.
// Bölüt çıktısı <dizin>/b<bolut>.txt (atomik). Kur'an dilleri (hedef 0) için eşiğin 2 bit altına kadar adaylar <dizin>/b<bolut>.q.
// İş parçacığı sayısı ESZAMANLI (≤ 4). Bölüt büyüklüğü 250 000.
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
static atablo_t T; static char SIF; static int ND, NH; static dtab_t *DT; static dek_t *DE; static char (*DAD)[16]; static int *DHED; static double *DESIK; static int *DSINIF, *DKOL, *DL;
static int ELNO=-1; static double *ELESIK; static int *ELSINIF; static int *ELSIRA;
static int *K4, NK4=0;                                                                 // kol 4 dilleri: kontrol partileri üzerinde değerlendirilir
#define NPB 1024
static int16_t (*HED)[114]; static int B1_, B2_; static const char *DIZ; static int SONRAKI; static pthread_mutex_t KILIT=PTHREAD_MUTEX_INITIALIZER;
static void *isci(void *arg){ (void)arg; orn_t *Z=malloc(sizeof(orn_t)); int GL[NLENS];
  prof_t *PB=malloc(sizeof(prof_t)*NPB); uint32_t *PI=malloc(sizeof(uint32_t)*NPB); uint16_t (*PA)[114]=malloc(sizeof(*PA)*NPB);
  int16_t (*CF)[NLENS]=malloc(sizeof(*CF)*NPB), (*CH)[NLENS]=malloc(sizeof(*CH)*NPB); int nb=0;
  long *k=malloc(sizeof(long)*ND), *kc=malloc(sizeof(long)*ND), *ek=malloc(sizeof(long)*NH), *ekc=malloc(sizeof(long)*NH);
  aday_t *AD=malloc(sizeof(aday_t)*ADAYMAX);
  for(;;){ pthread_mutex_lock(&KILIT); int b=SONRAKI++; pthread_mutex_unlock(&KILIT); if(b>B2_) break;
    char fn[1100], tmp[1100], fq[1100], tq[1100]; snprintf(fn,sizeof fn,"%s/b%04d.txt",DIZ,b); snprintf(tmp,sizeof tmp,"%s.yaziliyor",fn);
    snprintf(fq,sizeof fq,"%s/b%04d.q",DIZ,b); snprintf(tq,sizeof tq,"%s.yaziliyor",fq);
    FILE *t=fopen(fn,"r"); if(t){ fclose(t); continue; }
    struct timespec t0,t1; clock_gettime(CLOCK_MONOTONIC,&t0);
    memset(k,0,sizeof(long)*ND); memset(kc,0,sizeof(long)*ND); memset(ek,0,sizeof(long)*NH); memset(ekc,0,sizeof(long)*NH); int nad=0, tasma=0;
    orn_init(Z,SIF,tohum(SIF,KONTROL_TUR,b));
    for(int i=0;i<BN;i++){ orn_sonraki(Z); prof_t *const Pp=&PB[nb]; kdegerlendir(Z->a,Pp,0x3FFF); const int nGL=kapi_listesi(Pp,GL);
      #define P (*Pp)
      if(NK4){ PI[nb]=(uint32_t)i; for(int q=0;q<114;q++) PA[nb][q]=(uint16_t)Z->a[q]; baglam_listesi(&T,Pp,CF[nb],CH[nb]); }
      for(int d=0;d<ND;d++){
        const int kol=DKOL[d];
        if(kol==0){                                                                   // plasebo12 ile birebir
          if(d==ELNO){ int L; const double v=dtab_puan(&DT[d],&T,&P,&L); const int sn=dtab_sinif(&DT[d],&P,L);
            for(int j=0;j<NH;j++){ const int h=ELSIRA[j]; if(v>=ELESIK[h]-1e-6){ ek[h]++; if(sn) ekc[h]++; } else break; }
            if(v>=ELESIK[0]-2.0 && nad<ADAYMAX){ aday_t *A=&AD[nad++]; A->sira=(uint32_t)i; A->puan=(float)v; A->L=(int16_t)L; A->sinif=(uint8_t)sn; A->dil=(uint8_t)d; for(int q=0;q<114;q++) A->a[q]=(uint16_t)Z->a[q]; }
            else if(v>=ELESIK[0]-2.0) tasma++;
            continue; }
          if(DHED[d]==0){ int L; const double v=dtab_puan(&DT[d],&T,&P,&L);
            if(v>=DESIK[d]-1e-6){ k[d]++; if(dtab_sinif(&DT[d],&P,L)) kc[d]++; }
            if(v>=DESIK[d]-2.0){ if(nad<ADAYMAX){ aday_t *A=&AD[nad++]; A->sira=(uint32_t)i; A->puan=(float)v; A->L=(int16_t)L; A->sinif=(uint8_t)dtab_sinif(&DT[d],&P,L); A->dil=(uint8_t)d;
                for(int q=0;q<114;q++) A->a[q]=(uint16_t)Z->a[q]; } else tasma++; }
            continue; }
          if(dtab_asar(&DT[d],&T,&P,DESIK[d])){ int L; dtab_puan(&DT[d],&T,&P,&L); k[d]++; if(dtab_sinif(&DT[d],&P,L)) kc[d]++; }
          continue; }
        if(kol<=3){                                                                   // Kol A, A−, A+: bütünlüklü puan
          if(nGL==0) continue; int L, bv; const double v=puan_B(&DT[d],&DE[d],&T,&P,GL,nGL,&L,&bv); if(!bv) continue; kc[d]++;
          if(v>=DESIK[d]-1e-6) k[d]++;
          if(DHED[d]==0 && v>=DESIK[d]-2.0){ if(nad<ADAYMAX){ aday_t *A=&AD[nad++]; A->sira=(uint32_t)i; A->puan=(float)v; A->L=(int16_t)L; A->sinif=1; A->dil=(uint8_t)d;
              for(int q=0;q<114;q++) A->a[q]=(uint16_t)Z->a[q]; } else tasma++; }
          continue; }
        /* kol 4: aşağıda, parti üzerinde */ }
      #undef P
      if(NK4 && (++nb==NPB || i==BN-1)){                                              // kol 4 (B): bütün mercekler, boy sınırıyla budama
        for(int e=0;e<NK4;e++){ const int d=K4[e]; const dtab_t *D=&DT[d]; const dek_t *E=&DE[d];
          for(int j=0;j<nb;j++){ const prof_t *Pq=&PB[j];
            if(DHED[d]==0){ int L; const double v=puan_G2(D,E,Pq,CF[j],CH[j],DESIK[d]-2.0,&L); if(v==-INFINITY) continue;
              const int sn=dtab_sinif(D,Pq,L); if(v>=DESIK[d]-1e-6){ k[d]++; if(sn) kc[d]++; }
              if(nad<ADAYMAX){ aday_t *A=&AD[nad++]; A->sira=PI[j]; A->puan=(float)v; A->L=(int16_t)L; A->sinif=(uint8_t)sn; A->dil=(uint8_t)d; memcpy(A->a,PA[j],sizeof A->a); } else tasma++;
              continue; }
            if(asar_G2(D,E,Pq,CF[j],CH[j],DESIK[d])){ int L; puan_G2(D,E,Pq,CF[j],CH[j],DESIK[d]-1e-6,&L); k[d]++; if(dtab_sinif(D,Pq,L)) kc[d]++; } } }
        nb=0; }
      else if(!NK4) nb=0; }
    clock_gettime(CLOCK_MONOTONIC,&t1); const double sn=(t1.tv_sec-t0.tv_sec)+1e-9*(t1.tv_nsec-t0.tv_nsec);
    FILE *g=fopen(tq,"wb"); fwrite(AD,sizeof(aday_t),nad,g); fclose(g); rename(tq,fq);
    FILE *f=fopen(tmp,"w"); fprintf(f,"# sifir %c bolut %d tohum %llu N %d sure_s %.1f aday %d aday_tasma %d\n",SIF,b,(unsigned long long)tohum(SIF,KONTROL_TUR,b),BN,sn,nad,tasma);
    for(int d=0;d<ND;d++) if(d!=ELNO) fprintf(f,"D %s %ld %ld\n",DAD[d],k[d],kc[d]);
    if(ELNO>=0) for(int h=0;h<NH;h++) fprintf(f,"E %d %ld %ld\n",h,ek[h],ekc[h]);
    fclose(f); rename(tmp,fn); }
  free(Z); free(k); free(kc); free(ek); free(ekc); free(AD); free(PB); free(PI); free(PA); free(CF); free(CH); return NULL; }
static int esik_kiyas(const void *a, const void *b){ const int x=*(const int*)a, y=*(const int*)b; if(ELESIK[x]<ELESIK[y]) return -1; if(ELESIK[x]>ELESIK[y]) return 1; return x<y?-1:(x>y); }
static size_t ub_bayt(const atablo_t *T_, const int *g, int n){ int ng[NGRUP]={0}; for(int i=0;i<n;i++) ng[pgrup(g[i])]++; size_t cf=0, ch=0; for(int L=0;L<NLENS;L++){ cf+=T_->nF[L]; ch+=T_->nH[L]; }
  return 4*((size_t)NLENS*(ng[0]+1+ng[1]+1)+cf*(ng[2]+1+ng[3]+1)+ch*(ng[4]+1)); }
int main(int argc,char**argv){
  if(argc<8){ fprintf(stderr,"kullanım\n"); return 1; }
  SIF=argv[1][0]; B1_=atoi(argv[4]); B2_=atoi(argv[5]); DIZ=argv[6]; SONRAKI=B1_; const int bellek=getenv("KONTROL_BELLEK")!=NULL;
  kutuphane_init(); mercek_bedeli_hazirla(); atablo_kur(&T,argv+7,argc-7);
  { FILE *f=fopen(argv[2],"rb"); fseek(f,0,SEEK_END); long sz=ftell(f); fseek(f,0,SEEK_SET); NH=(int)(sz/(4+228)); HED=malloc(sizeof(*HED)*NH);
    for(int h=0;h<NH;h++){ int32_t id; if(fread(&id,4,1,f)!=1||fread(HED[h],2,114,f)!=114||id!=h){ fprintf(stderr,"hedef dosyası\n"); return 2; } } fclose(f); }
  FILE *f=fopen(argv[3],"r"); char ad[64]; int hed, kod, L, n; int cap=1024; DT=calloc(cap,sizeof(dtab_t)); DE=calloc(cap,sizeof(dek_t)); DAD=calloc(cap,16); DHED=calloc(cap,sizeof(int));
  DESIK=calloc(cap,sizeof(double)); DSINIF=calloc(cap,sizeof(int)); DKOL=calloc(cap,sizeof(int)); DL=calloc(cap,sizeof(int));
  size_t top=0; ND=0;
  while(fscanf(f,"%63s %d %d %d %d",ad,&hed,&kod,&L,&n)==5){ int g[DMAX]; if(n>DMAX){ fprintf(stderr,"dil DMAX'ı aşıyor: %s\n",ad); return 2; }
    for(int i=0;i<n;i++) if(fscanf(f,"%d",&g[i])!=1){ fprintf(stderr,"dil satırı\n"); return 2; }
    const int kol=kod/10, ons=kod%10; if(kol<0||kol>4||ons<0||ons>1){ fprintf(stderr,"kol kodu: %s\n",ad); return 2; }
    if(kol>=1 && kol<=3 && (L<0||n==0)) continue;                                   // dilsiz hedef: p = 1, kontrol geçişine girmez
    if(ND==cap){ fprintf(stderr,"çok dil\n"); return 2; }
    { int ng[NGRUP]={0}; for(int i=0;i<n;i++) ng[pgrup(g[i])]++; for(int q=0;q<NGRUP;q++) if(ng[q]>20){ fprintf(stderr,"grup boyu > 20: %s\n",ad); return 2; } }
    const size_t by=dtab_bayt(&T,g,n)+(kol==4?ub_bayt(&T,g,n):0);
    if(bellek){ printf("bayt %s %zu\n",ad,by); continue; }
    top+=by; if(top>(size_t)3500*1024*1024){ fprintf(stderr,"dil tabloları bellek sınırını aşıyor (%zu MB)\n",top>>20); return 3; }
    dtab_kur(&DT[ND],&T,g,n,ons); DE[ND].kol=kol; DE[ND].tmin=KOL_TMIN[kol]; DE[ND].dmin=KOL_DMIN[kol]; if(kol==4) ub_kur(&DT[ND],&T,&DE[ND]);
    snprintf(DAD[ND],16,"%s",ad); DHED[ND]=hed; DKOL[ND]=kol; if(!strcmp(ad,"EL")) ELNO=ND; ND++; }
  fclose(f); if(bellek) return 0;
  K4=malloc(sizeof(int)*(ND+1)); for(int d=0;d<ND;d++) if(DKOL[d]==4) K4[NK4++]=d;
  static prof_t P; int GL[NLENS];
  for(int d=0;d<ND;d++){ if(d==ELNO) continue; int a[114]; for(int q=0;q<114;q++) a[q]=HED[DHED[d]][q]; kdegerlendir(a,&P,0x3FFF); int Lx=-1;
    if(DKOL[d]==0 || DKOL[d]==4){ DESIK[d]=dtab_puan(&DT[d],&T,&P,&Lx); DSINIF[d]=dtab_sinif(&DT[d],&P,Lx); }
    else { int bv; const int nGL=kapi_listesi(&P,GL); DESIK[d]=puan_B(&DT[d],&DE[d],&T,&P,GL,nGL,&Lx,&bv);
      if(!bv || DESIK[d]==-INFINITY){ fprintf(stderr,"hedef kendi dilinde bütünlüklü değil: %s\n",DAD[d]); return 2; } DSINIF[d]=1; }
    DL[d]=Lx; }
  if(ELNO>=0){ ELESIK=malloc(sizeof(double)*NH); ELSINIF=malloc(sizeof(int)*NH); ELSIRA=malloc(sizeof(int)*NH);
    for(int h=0;h<NH;h++){ int a[114]; for(int q=0;q<114;q++) a[q]=HED[h][q]; kdegerlendir(a,&P,0x3FFF); int Lx; ELESIK[h]=dtab_puan(&DT[ELNO],&T,&P,&Lx); ELSINIF[h]=dtab_sinif(&DT[ELNO],&P,Lx); ELSIRA[h]=h; }
    qsort(ELSIRA,NH,sizeof(int),esik_kiyas); }
  { char fn[1100]; snprintf(fn,sizeof fn,"%s/esikler.txt",DIZ); FILE *g=fopen(fn,"w");
    for(int d=0;d<ND;d++) if(d!=ELNO){ fprintf(g,"D %s %d %.9f %d",DAD[d],DHED[d],DESIK[d],DSINIF[d]); if(DKOL[d]) fprintf(g," kol %d mercek %d",DKOL[d],DL[d]); fprintf(g,"\n"); }
    if(ELNO>=0) for(int h=0;h<NH;h++) fprintf(g,"E %d %.9f %d\n",h,ELESIK[h],ELSINIF[h]);
    fprintf(g,"# dil sayısı %d, tablo belleği %zu MB\n",ND,top>>20); fclose(g); }
  free(T.E); free(T.HA); T.E=NULL; T.HA=NULL;
  int nt=atoi(getenv("ESZAMANLI")?getenv("ESZAMANLI"):"4"); if(nt<1) nt=1; if(nt>4) nt=4;
  pthread_t th[4]; for(int i=0;i<nt;i++) pthread_create(&th[i],NULL,isci,NULL); for(int i=0;i<nt;i++) pthread_join(th[i],NULL);
  return 0; }
