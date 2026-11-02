// kart_ana.c — ekran kartı sayacının ana programı (işlemci tarafı). Kip KART_KIP = 0: kontrol_b'nin kol 1–3 yolu; 1: kontrol_h.
// Tabloları, dilleri ve eşikleri özgün başlıklarla (dtab_b.h ve altı, değiştirilmeden) ve özgün programların main() gövdesiyle aynı
// adımlarla kurar; esikler.txt, b<bolut>.txt, b<bolut>.q (ve kip 1'de .s) dosyalarını özgün biçimle yazar. Sayım arka uçta yapılır:
// kart_cihaz.cu (CUDA) ya da kart_emul.c (işlemci; eşdeğerlik sınaması için). Kullanım ve argümanlar özgün programlarla aynıdır:
//   ./kart_<b|h> <sifir> <hedef.bin> <diller.txt> <ilk_bolut> <son_bolut> <cikti_dizini> <ref>...
// Ortam: KART_PARTI (aynı anda işlenen bölüt sayısı), KART_MOD/KART_ALT/KART_UST (yalnız b mod MOD ∈ [ALT, UST] bölütleri; iki kart için).
// Bölüt dosyalarının biçimi özgünle aynıdır; sure_s alanı partinin duvar süresinin bölüt başına payıdır. Kartın kimliği <dizin>/kart_gunlugu.txt'e yazılır.
#include "dtab_b.h"
#include "kart_ortak.h"
#include <time.h>
#include <unistd.h>
#ifndef BN
#define BN 250000
#endif
#ifndef KONTROL_TUR
#define KONTROL_TUR 2
#endif
#ifndef KART_KIP
#define KART_KIP 0
#endif
#define ADAYMAX 100000
typedef struct __attribute__((packed)) { uint32_t sira; float puan; int16_t L; uint8_t sinif, dil; uint16_t a[114]; } aday_t;

int kart_hazirla(const ksabit_t *C, const kdil_t *D, int nd, const float *havuz, uint64_t nhavuz);
int kart_parti(const int *bolut, int nb, int bn, uint64_t *k, uint64_t *kc, uint64_t *nb2, kkayit_t **kay, uint64_t *nkay);
const char *kart_aciklama(void);

static atablo_t T; static char SIF; static int ND, NH; static dtab_t *DT; static dek_t *DE; static char (*DAD)[16]; static int *DHED; static double *DESIK; static int *DKOL, *DL;
static int16_t (*HED)[114];

static int kayit_kiyas(const void *x, const void *y){ const kkayit_t *a=x, *b=y;
  if(a->bolut_ix!=b->bolut_ix) return a->bolut_ix<b->bolut_ix?-1:1; if(a->tur!=b->tur) return a->tur<b->tur?-1:1;
  if(a->sira!=b->sira) return a->sira<b->sira?-1:1; if(a->dil!=b->dil) return a->dil<b->dil?-1:1; return 0; }
static void aday_doldur(aday_t *A, const kkayit_t *r){ A->sira=r->sira; A->puan=(float)r->puan; A->L=r->L; A->sinif=1; A->dil=(uint8_t)r->dil; memcpy(A->a,r->a,sizeof A->a); }

int main(int argc,char**argv){
  if(argc<8){ fprintf(stderr,"kullanım\n"); return 1; }
  SIF=argv[1][0]; const int B1_=atoi(argv[4]), B2_=atoi(argv[5]); const char *DIZ=argv[6];
  kutuphane_init(); mercek_bedeli_hazirla(); atablo_kur(&T,argv+7,argc-7);
  { FILE *f=fopen(argv[2],"rb"); if(!f){ fprintf(stderr,"hedef dosyası\n"); return 2; } fseek(f,0,SEEK_END); long sz=ftell(f); fseek(f,0,SEEK_SET); NH=(int)(sz/(4+228)); HED=malloc(sizeof(*HED)*NH);
    for(int h=0;h<NH;h++){ int32_t id; if(fread(&id,4,1,f)!=1||fread(HED[h],2,114,f)!=114||id!=h){ fprintf(stderr,"hedef dosyası\n"); return 2; } } fclose(f); }
  FILE *f=fopen(argv[3],"r"); if(!f){ fprintf(stderr,"diller\n"); return 2; } char ad[64]; int hed, kod, L, n; const int cap=32768;   // K×100 (1 Kas): 4.096 → 32.768 (düzey 1 partisi ~20.000 dil)
  DT=calloc(cap,sizeof(dtab_t)); DE=calloc(cap,sizeof(dek_t)); DAD=calloc(cap,16); DHED=calloc(cap,sizeof(int)); DESIK=calloc(cap,sizeof(double)); DKOL=calloc(cap,sizeof(int)); DL=calloc(cap,sizeof(int));
  size_t top=0; ND=0;
  while(fscanf(f,"%63s %d %d %d %d",ad,&hed,&kod,&L,&n)==5){ int g[DMAX]; if(n>DMAX){ fprintf(stderr,"dil DMAX'ı aşıyor: %s\n",ad); return 2; }
    for(int i=0;i<n;i++) if(fscanf(f,"%d",&g[i])!=1){ fprintf(stderr,"dil satırı\n"); return 2; }
    const int kol=kod/10, ons=kod%10;
#if KART_KIP==0
    if(kol<0||kol>4||ons<0||ons>1){ fprintf(stderr,"kol kodu: %s\n",ad); return 2; }
    if(kol>=1 && kol<=3 && (L<0||n==0)) continue;                                   // kontrol_b: dilsiz hedef kontrol geçişine girmez
    if(kol==0 || kol==4 || !strcmp(ad,"EL")){ fprintf(stderr,"kart: yalnız kol 1–3 desteklenir: %s\n",ad); return 2; }
    if(ND==cap){ fprintf(stderr,"çok dil\n"); return 2; }
    { int ng[NGRUP]={0}; for(int i=0;i<n;i++) ng[pgrup(g[i])]++; for(int q=0;q<NGRUP;q++) if(ng[q]>20){ fprintf(stderr,"grup boyu > 20: %s\n",ad); return 2; } }
    const size_t by=dtab_bayt(&T,g,n); top+=by; if(top>(size_t)3500*1024*1024){ fprintf(stderr,"dil tabloları bellek sınırını aşıyor (%zu MB)\n",top>>20); return 3; }
#else
    if(kol<1||kol>3||ons<0||ons>1||!strcmp(ad,"EL")){ fprintf(stderr,"yalnız kol 1–3: %s\n",ad); return 2; }
    if(L<0||n==0){ fprintf(stderr,"dilsiz hedef: %s\n",ad); return 2; }
    if(ND==cap){ fprintf(stderr,"çok dil\n"); return 2; }
    { uint8_t s=0; int nb=0, b2=0; for(int i=0;i<n;i++){ if(pgrup(g[i])==G_B){ nb++; if(g[i]==GOFS[G_B]+B_B2) b2=1; } else s|=imza_b(g[i]); }
      if(nb!=1 || !b2 || kapsam_tamam_b(s,KOL_TMIN[kol],KOL_DMIN[kol])){ fprintf(stderr,"kısayol bu dil için geçerli değil: %s\n",ad); return 2; } }
#endif
    dtab_kur(&DT[ND],&T,g,n,ons); DE[ND].kol=kol; DE[ND].tmin=KOL_TMIN[kol]; DE[ND].dmin=KOL_DMIN[kol];
    snprintf(DAD[ND],16,"%s",ad); DHED[ND]=hed; DKOL[ND]=kol; ND++; }
  fclose(f);
  static prof_t P; int GL[NLENS];
  for(int d=0;d<ND;d++){ int a[114]; for(int q=0;q<114;q++) a[q]=HED[DHED[d]][q]; kdegerlendir(a,&P,0x3FFF); int Lx=-1, bv;
    const int nGL=kapi_listesi(&P,GL); DESIK[d]=puan_B(&DT[d],&DE[d],&T,&P,GL,nGL,&Lx,&bv);
    if(!bv || DESIK[d]==-INFINITY){ fprintf(stderr,"hedef kendi dilinde bütünlüklü değil: %s\n",DAD[d]); return 2; } DL[d]=Lx; }
  // esikler.txt: dizinde varsa (ör. işlemci sayacının yazdığı) bayt bayt aynı olmalı, yoksa yazılır (geçici dosya + rename)
  char fes[1100], tes[1100]; snprintf(fes,sizeof fes,"%s/esikler.txt",DIZ); snprintf(tes,sizeof tes,"%s/esikler.txt.kart%d",DIZ,(int)getpid());
  { char *fn=tes; FILE *g=fopen(fn,"w"); if(!g){ perror(fn); return 2; }
#if KART_KIP==0
    for(int d=0;d<ND;d++){ fprintf(g,"D %s %d %.9f %d",DAD[d],DHED[d],DESIK[d],1); if(DKOL[d]) fprintf(g," kol %d mercek %d",DKOL[d],DL[d]); fprintf(g,"\n"); }
    fprintf(g,"# dil sayısı %d, tablo belleği %zu MB\n",ND,top>>20);
#else
    for(int d=0;d<ND;d++) fprintf(g,"D %s %d %.9f %d kol %d mercek %d\n",DAD[d],DHED[d],DESIK[d],1,DKOL[d],DL[d]);
    fprintf(g,"# dil sayısı %d\n",ND);
#endif
    fclose(g); }
  { FILE *a=fopen(fes,"rb");
    if(!a){ if(rename(tes,fes)!=0){ perror("rename esikler"); return 2; } }
    else { FILE *b=fopen(tes,"rb"); int ca, cb, ayni=1; do { ca=fgetc(a); cb=fgetc(b); if(ca!=cb){ ayni=0; break; } } while(ca!=EOF); fclose(a); fclose(b); remove(tes);
      if(!ayni){ fprintf(stderr,"esikler.txt dizindekiyle aynı değil: eşikler farklı hesaplandı\n"); return 2; } } }
  // ---- sabitler ----
  ksabit_t *C=calloc(1,sizeof(ksabit_t)); C->sif=SIF; C->kip=KART_KIP; C->KE=KAPI_ESIK; C->KH=KAPI_HUCRE;
  for(int q=0;q<114;q++) C->base[q]=(int16_t)BASE[q];
  { int srt[114]; memcpy(srt,BASE,sizeof srt); for(int i=1;i<114;i++){ int x=srt[i]; int j=i-1; while(j>=0&&srt[j]<x){srt[j+1]=srt[j];j--;} srt[j+1]=x; } for(int q=0;q<114;q++) C->srt[q]=(int16_t)srt[q]; }
  C->s_obs=S_OBS; C->sn_tot=(int32_t)SN_TOT; C->sa_tot=(int32_t)SA_TOT;
  memcpy(C->divm,DIVM,sizeof C->divm); memcpy(C->inv32,INV32,sizeof C->inv32);
  for(int q=0;q<NFORM;q++){ C->al[q]=AL[q]; C->be[q]=BE[q]; } for(int q=0;q<NPR;q++) C->pr[q]=PR[q];
  memcpy(C->exb,EXB,sizeof C->exb); memcpy(C->exz,EXZ,sizeof C->exz); memcpy(C->exh,EXH,sizeof C->exh); memcpy(C->exf,EXF,sizeof C->exf); memcpy(C->exw,EXW,sizeof C->exw);
  memcpy(C->cF,T.cF,sizeof C->cF); memcpy(C->cH,T.cH,sizeof C->cH);
  for(int d=0;d<ND;d++) for(int q=0;q<=NLENS;q++) if(DT[d].ofs[q]!=DT[0].ofs[q] || DT[d].ofsH[q]!=DT[0].ofsH[q]){ fprintf(stderr,"bağlam ofsetleri diller arasında farklı\n"); return 2; }
  memcpy(C->ofs,DT[0].ofs,sizeof C->ofs); memcpy(C->ofsH,DT[0].ofsH,sizeof C->ofsH);
  memcpy(C->lbedel,LBEDEL,sizeof C->lbedel); for(int q=0;q<NPRED;q++) C->imza[q]=imza_b(q); for(int q=0;q<NGRUP;q++) C->gofs[q]=GOFS[q];
  for(int d=0;d<ND;d++) for(int q=0;q<NLENS;q++) if(DT[d].lc[q]!=LBEDEL[DT[d].onsel][q]){ fprintf(stderr,"mercek bedeli\n"); return 2; }
  if(SIF=='L'){ orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,'L',0); memcpy(C->thr,Z->thr,sizeof C->thr); memcpy(C->ali,Z->ali,sizeof C->ali);
    for(int q=0;q<114;q++) C->ref[q]=(int16_t)Z->ref[q]; free(Z); }
  else if(SIF!='M' && SIF!='U'){ fprintf(stderr,"sıfır?\n"); return 2; }
  // ---- diller ----
  kdil_t *KD_=calloc(ND,sizeof(kdil_t)); uint64_t nh=0;
  for(int d=0;d<ND;d++){ const dtab_t *D=&DT[d]; const uint64_t nx[5]={(uint64_t)NLENS<<D->ng[0],(uint64_t)NLENS<<D->ng[1],(uint64_t)D->ofs[NLENS]<<D->ng[2],(uint64_t)D->ofs[NLENS]<<D->ng[3],(uint64_t)D->ofsH[NLENS]<<D->ng[4]}; for(int q=0;q<5;q++) nh+=nx[q]; }
  float *HV=malloc(sizeof(float)*(nh?nh:1)); uint64_t o=0;
  for(int d=0;d<ND;d++){ const dtab_t *D=&DT[d]; kdil_t *K=&KD_[d];
    for(int q=0;q<5;q++){ K->ng[q]=D->ng[q]; K->gm[q]=D->gm[q]; for(int j=0;j<D->ng[q];j++) K->pos[q][j]=D->pos[q][j]; }
    K->tmin=DE[d].tmin; K->dmin=DE[d].dmin; K->onsel=D->onsel; K->hed=DHED[d]; K->esik_k=DESIK[d]-1e-6; K->esik_q=DESIK[d]-2.0;
    const float *src[5]={D->TB,D->TZ,D->TF,D->TW,D->TH}; const uint64_t nx[5]={(uint64_t)NLENS<<D->ng[0],(uint64_t)NLENS<<D->ng[1],(uint64_t)D->ofs[NLENS]<<D->ng[2],(uint64_t)D->ofs[NLENS]<<D->ng[3],(uint64_t)D->ofsH[NLENS]<<D->ng[4]};
    uint64_t *of[5]={&K->oTB,&K->oTZ,&K->oTF,&K->oTW,&K->oTH};
    for(int q=0;q<5;q++){ *of[q]=o; memcpy(HV+o,src[q],sizeof(float)*nx[q]); o+=nx[q]; }
    dtab_birak(&DT[d]); }
  free(T.E); free(T.HA); T.E=NULL; T.HA=NULL;
  if(kart_hazirla(C,KD_,ND,HV,nh)){ fprintf(stderr,"kart_hazirla\n"); return 4; }
  { char fn[1100]; snprintf(fn,sizeof fn,"%s/kart_gunlugu.txt",DIZ); FILE *g=fopen(fn,"a"); if(g){ time_t now=time(NULL); char ts[64]; strftime(ts,sizeof ts,"%Y-%m-%dT%H:%M:%SZ",gmtime(&now));
      fprintf(g,"[%s] kip %d sıfır %c bölüt %d–%d dil %d BN %d arka uç: %s\n",ts,KART_KIP,SIF,B1_,B2_,ND,BN,kart_aciklama()); fclose(g); } }
  // ---- bölütler ----
  const int MOD=getenv("KART_MOD")?atoi(getenv("KART_MOD")):1, ALT=getenv("KART_ALT")?atoi(getenv("KART_ALT")):0, UST=getenv("KART_UST")?atoi(getenv("KART_UST")):MOD-1;
  int NP=getenv("KART_PARTI")?atoi(getenv("KART_PARTI")):512; if(NP<1) NP=1;
  int *liste=malloc(sizeof(int)*(B2_-B1_+1>0?B2_-B1_+1:1)); int nl=0;
  for(int b=B1_;b<=B2_;b++){ if(b%MOD<ALT || b%MOD>UST) continue; char fn[1100]; snprintf(fn,sizeof fn,"%s/b%04d.txt",DIZ,b); FILE *t=fopen(fn,"r"); if(t){ fclose(t); continue; } liste[nl++]=b; }
  aday_t *AD=malloc(sizeof(aday_t)*ADAYMAX), *AS=malloc(sizeof(aday_t)*ADAYMAX);
  uint64_t *k=malloc(sizeof(uint64_t)*(size_t)NP*ND), *kc=malloc(sizeof(uint64_t)*(size_t)NP*ND), *nb2=malloc(sizeof(uint64_t)*NP);
  for(int p0=0;p0<nl;p0+=NP){ const int nb=(nl-p0<NP)?nl-p0:NP; struct timespec t0,t1; clock_gettime(CLOCK_MONOTONIC,&t0);
    kkayit_t *kay=NULL; uint64_t nkay=0;
    if(kart_parti(liste+p0,nb,BN,k,kc,nb2,&kay,&nkay)){ fprintf(stderr,"kart_parti hatası (bölüt %d…)\n",liste[p0]); return 5; }
    clock_gettime(CLOCK_MONOTONIC,&t1); const double sn=((t1.tv_sec-t0.tv_sec)+1e-9*(t1.tv_nsec-t0.tv_nsec))/nb;
    qsort(kay,nkay,sizeof(kkayit_t),kayit_kiyas); uint64_t r=0;
    for(int j=0;j<nb;j++){ const int b=liste[p0+j]; int nad=0, tasma=0, nas=0, stasma=0;
      while(r<nkay && kay[r].bolut_ix<j) r++;
      for(;r<nkay && kay[r].bolut_ix==j;r++){ if(kay[r].tur==0){ if(nad<ADAYMAX) aday_doldur(&AD[nad++],&kay[r]); else tasma++; }
        else { if(nas<ADAYMAX) aday_doldur(&AS[nas++],&kay[r]); else stasma++; } }
      char fn[1100], tmp[1100], fq[1100], tq[1100]; snprintf(fn,sizeof fn,"%s/b%04d.txt",DIZ,b); snprintf(tmp,sizeof tmp,"%s.yaziliyor",fn);
      snprintf(fq,sizeof fq,"%s/b%04d.q",DIZ,b); snprintf(tq,sizeof tq,"%s.yaziliyor",fq);
#if KART_KIP==0
      { FILE *g=fopen(tq,"wb"); fwrite(AD,sizeof(aday_t),nad,g); fclose(g); rename(tq,fq); }
      FILE *g=fopen(tmp,"w"); fprintf(g,"# sifir %c bolut %d tohum %llu N %d sure_s %.1f aday %d aday_tasma %d\n",SIF,b,(unsigned long long)tohum(SIF,KONTROL_TUR,b),BN,sn,nad,tasma);
#else
      char fs[1100], ts[1100]; snprintf(fs,sizeof fs,"%s/b%04d.s",DIZ,b); snprintf(ts,sizeof ts,"%s.yaziliyor",fs);
      if(nad){ FILE *g=fopen(tq,"wb"); fwrite(AD,sizeof(aday_t),nad,g); fclose(g); rename(tq,fq); }
      if(nas){ FILE *g=fopen(ts,"wb"); fwrite(AS,sizeof(aday_t),nas,g); fclose(g); rename(ts,fs); }
      FILE *g=fopen(tmp,"w"); fprintf(g,"# sifir %c bolut %d tohum %llu N %d sure_s %.1f aday %d aday_tasma %d sinifli %d sinifli_tasma %d b2 %ld\n",
        SIF,b,(unsigned long long)tohum(SIF,KONTROL_TUR,b),BN,sn,nad,tasma,nas,stasma,(long)nb2[j]);
#endif
      for(int d=0;d<ND;d++) fprintf(g,"D %s %ld %ld\n",DAD[d],(long)k[(size_t)j*ND+d],(long)kc[(size_t)j*ND+d]);
      fclose(g); rename(tmp,fn); }
    free(kay); }
  return 0; }
