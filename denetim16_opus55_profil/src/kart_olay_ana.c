// kart_olay_ana.c — profil çalışmasının sayacı (kart kip 2; ONKAYIT §4): her kontrol için, olay listesindeki her olayın tutup tutmadığı.
// Bir kontrol bir olayı, olayın merceğinde kapı G (Gc: her hücre ≥ 2) tutuyor ve olayın her grup maskesi o mercekteki profilinin alt
// kümesiyse taşır. Kısmi puan, bit modeli ve mercek araması yoktur; referans tabloları kullanılmaz. Profil, kontrol_b/kontrol_h ile
// bayt bayt eşdeğerliği kabul edilmiş kart çekirdeğiyle (kart_ortak.h k_mercek) hesaplanır; örnekleyiciler ve tohumlar (tür 2) aynıdır.
// Kullanım: ./kart_olay <sifir> <hedef.bin> <olaylar.txt> <ilk_bolut> <son_bolut> <cikti_dizini>
//   olaylar.txt satırı (olay_kur çıktısı): <ad> <hedef_no> <L> <mB> <mZ> <mF> <mW> <mH> [başka sütunlar yok sayılır]
// Bölüt çıktısı <dizin>/b<bolut>.txt: başlık satırı ("olay <sayı>" ile) ve yalnız sayımı sıfırdan büyük olayların "D <ad> <k>" satırları.
// Kur'an'ın (hedef 0) olaylarının bütün isabetleri <dizin>/b<bolut>.q'ya yazılır (kontrol_b aday kaydıyla aynı biçim; puan 0, L olayın merceği).
// Ortam: KART_PARTI, KART_MOD/KART_ALT/KART_UST, KART_CH (kart_ana.c ile aynı).
#include "kutuphane.h"
#include "kart_ortak.h"
#include <time.h>
#include <unistd.h>
#ifndef BN
#define BN 250000
#endif
#define KONTROL_TUR 2
#define ADAYMAX 100000
typedef struct __attribute__((packed)) { uint32_t sira; float puan; int16_t L; uint8_t sinif, dil; uint16_t a[114]; } aday_t;
int kart_hazirla(const ksabit_t *C, const kdil_t *D, int nd, const float *havuz, uint64_t nhavuz);
int kart_parti(const int *bolut, int nb, int bn, uint64_t *k, uint64_t *kc, uint64_t *nb2, kkayit_t **kay, uint64_t *nkay);
const char *kart_aciklama(void);
static int kayit_kiyas(const void *x, const void *y){ const kkayit_t *a=x, *b=y;
  if(a->bolut_ix!=b->bolut_ix) return a->bolut_ix<b->bolut_ix?-1:1; if(a->sira!=b->sira) return a->sira<b->sira?-1:1; if(a->dil!=b->dil) return a->dil<b->dil?-1:1; return 0; }
int main(int argc,char**argv){
  if(argc<7){ fprintf(stderr,"kullanım\n"); return 1; }
  const char SIF=argv[1][0]; const int B1_=atoi(argv[4]), B2_=atoi(argv[5]); const char *DIZ=argv[6];
  kutuphane_init();
  int NH; { FILE *f=fopen(argv[2],"rb"); if(!f){ perror(argv[2]); return 2; } fseek(f,0,SEEK_END); NH=(int)(ftell(f)/232); fclose(f); }
  FILE *f=fopen(argv[3],"r"); if(!f){ perror(argv[3]); return 2; }
  int cap=1024, ND=0; kdil_t *KD_=calloc(cap,sizeof(kdil_t)); char (*AD)[24]=calloc(cap,24); char sat[4096];
  while(fgets(sat,sizeof sat,f)){ char ad[64]; int hed, L; unsigned long long m[5];
    if(sscanf(sat,"%63s %d %d %llx %llx %llx %llx %llx",ad,&hed,&L,&m[0],&m[1],&m[2],&m[3],&m[4])!=8){ fprintf(stderr,"olay satırı: %s",sat); return 2; }
    if(L<0||L>=K_NLENS||hed<0||hed>=NH){ fprintf(stderr,"olay: %s\n",ad); return 2; }
    if(ND==cap){ cap*=2; KD_=realloc(KD_,sizeof(kdil_t)*cap); AD=realloc(AD,24*(size_t)cap); memset(KD_+ND,0,sizeof(kdil_t)*(cap-ND)); }
    kdil_t *K=&KD_[ND]; K->hed=hed; K->Lsabit=L; for(int q=0;q<5;q++) K->om[q]=m[q]; snprintf(AD[ND],24,"%s",ad); ND++; }
  fclose(f); if(ND==0){ fprintf(stderr,"olay yok\n"); return 2; }
  ksabit_t *C=calloc(1,sizeof(ksabit_t)); C->sif=SIF; C->kip=2; C->KE=KAPI_ESIK; C->KH=KAPI_HUCRE;
  for(int q=0;q<114;q++) C->base[q]=(int16_t)BASE[q];
  { int srt[114]; memcpy(srt,BASE,sizeof srt); for(int i=1;i<114;i++){ int x=srt[i]; int j=i-1; while(j>=0&&srt[j]<x){srt[j+1]=srt[j];j--;} srt[j+1]=x; } for(int q=0;q<114;q++) C->srt[q]=(int16_t)srt[q]; }
  C->s_obs=S_OBS; C->sn_tot=(int32_t)SN_TOT; C->sa_tot=(int32_t)SA_TOT;
  memcpy(C->divm,DIVM,sizeof C->divm); memcpy(C->inv32,INV32,sizeof C->inv32);
  for(int q=0;q<NFORM;q++){ C->al[q]=AL[q]; C->be[q]=BE[q]; } for(int q=0;q<NPR;q++) C->pr[q]=PR[q];
  memcpy(C->exb,EXB,sizeof C->exb); memcpy(C->exz,EXZ,sizeof C->exz); memcpy(C->exh,EXH,sizeof C->exh); memcpy(C->exf,EXF,sizeof C->exf); memcpy(C->exw,EXW,sizeof C->exw);
  for(int d=0;d<ND;d++) C->fm_olay|=1u<<(KD_[d].Lsabit/K_NPR);
  if(SIF=='L'){ orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,'L',0); memcpy(C->thr,Z->thr,sizeof C->thr); memcpy(C->ali,Z->ali,sizeof C->ali);
    for(int q=0;q<114;q++) C->ref[q]=(int16_t)Z->ref[q]; free(Z); }
  else if(SIF!='M' && SIF!='U'){ fprintf(stderr,"sıfır?\n"); return 2; }
  if(kart_hazirla(C,KD_,ND,NULL,0)){ fprintf(stderr,"kart_hazirla\n"); return 4; }
  { char fn[1100]; snprintf(fn,sizeof fn,"%s/kart_gunlugu.txt",DIZ); FILE *g=fopen(fn,"a"); if(g){ time_t now=time(NULL); char ts[64]; strftime(ts,sizeof ts,"%Y-%m-%dT%H:%M:%SZ",gmtime(&now));
      fprintf(g,"[%s] kip 2 (olay) sıfır %c bölüt %d–%d olay %d biçim maskesi %#x BN %d arka uç: %s\n",ts,SIF,B1_,B2_,ND,C->fm_olay,BN,kart_aciklama()); fclose(g); } }
  const int MOD=getenv("KART_MOD")?atoi(getenv("KART_MOD")):1, ALT=getenv("KART_ALT")?atoi(getenv("KART_ALT")):0, UST=getenv("KART_UST")?atoi(getenv("KART_UST")):MOD-1;
  int NP=getenv("KART_PARTI")?atoi(getenv("KART_PARTI")):512; if(NP<1) NP=1;
  int *liste=malloc(sizeof(int)*(B2_-B1_+1>0?B2_-B1_+1:1)); int nl=0;
  for(int b=B1_;b<=B2_;b++){ if(b%MOD<ALT || b%MOD>UST) continue; char fn[1100]; snprintf(fn,sizeof fn,"%s/b%04d.txt",DIZ,b); FILE *t=fopen(fn,"r"); if(t){ fclose(t); continue; } liste[nl++]=b; }
  aday_t *AQ=malloc(sizeof(aday_t)*ADAYMAX);
  uint64_t *k=malloc(sizeof(uint64_t)*(size_t)NP*ND), *kc=malloc(sizeof(uint64_t)*(size_t)NP*ND), *nb2=malloc(sizeof(uint64_t)*NP);
  for(int p0=0;p0<nl;p0+=NP){ const int nb=(nl-p0<NP)?nl-p0:NP; struct timespec t0,t1; clock_gettime(CLOCK_MONOTONIC,&t0);
    kkayit_t *kay=NULL; uint64_t nkay=0;
    if(kart_parti(liste+p0,nb,BN,k,kc,nb2,&kay,&nkay)){ fprintf(stderr,"kart_parti hatası (bölüt %d…)\n",liste[p0]); return 5; }
    clock_gettime(CLOCK_MONOTONIC,&t1); const double sn=((t1.tv_sec-t0.tv_sec)+1e-9*(t1.tv_nsec-t0.tv_nsec))/nb;
    qsort(kay,nkay,sizeof(kkayit_t),kayit_kiyas); uint64_t r=0;
    for(int j=0;j<nb;j++){ const int b=liste[p0+j]; int nad=0, tasma=0;
      while(r<nkay && kay[r].bolut_ix<j) r++;
      for(;r<nkay && kay[r].bolut_ix==j;r++){ if(nad<ADAYMAX){ aday_t *A=&AQ[nad++]; A->sira=kay[r].sira; A->puan=0.0f; A->L=kay[r].L; A->sinif=1; A->dil=(uint8_t)kay[r].dil; memcpy(A->a,kay[r].a,sizeof A->a); } else tasma++; }
      char fn[1100], tmp[1100], fq[1100], tq[1100]; snprintf(fn,sizeof fn,"%s/b%04d.txt",DIZ,b); snprintf(tmp,sizeof tmp,"%s.yaziliyor",fn);
      snprintf(fq,sizeof fq,"%s/b%04d.q",DIZ,b); snprintf(tq,sizeof tq,"%s.yaziliyor",fq);
      { FILE *g=fopen(tq,"wb"); fwrite(AQ,sizeof(aday_t),nad,g); fclose(g); rename(tq,fq); }
      FILE *g=fopen(tmp,"w"); fprintf(g,"# sifir %c bolut %d tohum %llu N %d sure_s %.1f aday %d aday_tasma %d olay %d\n",SIF,b,(unsigned long long)k_tohum(SIF,KONTROL_TUR,b),BN,sn,nad,tasma,ND);
      for(int d=0;d<ND;d++){ const uint64_t x=k[(size_t)j*ND+d]; if(x) fprintf(g,"D %s %llu\n",AD[d],(unsigned long long)x); }
      fclose(g); rename(tmp,fn); }
    free(kay); }
  return 0; }
