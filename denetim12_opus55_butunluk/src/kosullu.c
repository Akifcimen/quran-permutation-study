// kosullu — eski literatüre koşullu bütünlük (ONKAYIT §8.2). Mercek devralınmış: (α,β,p) = (1,+1,19).
// O altında kapalı: F1 F2 R1e Qc QS Qh (+ sayımda kapı G ile zorunlu R1c); O' altında ek olarak B1 B2.
// Sayım: kalan yüklemlerin kredisi (kapı G). Bilgi: −log2 P̂(B|·) − log2 P̂(F-kalan|·) − log2 P̂(W|·), olasılıklar havuzdan
// biri-dışarıda. Duyarlılık: serbest mercek (sayım) = max(kalan kredi @ (1,1,19), öteki 405 merceğin tam kredisi).
// Kullanım: ./kosullu <kodeks> <O|P> <cikti_onek> <havuz.bin>...   (kayıt 244 bayt: uint16 a[114] | u32 | u32 | u64)
#include "tablo.h"
#define L0 (0*NPR+6)
static uint32_t KAPALI_O, KAPALI_SAYIM_EK;
static inline uint32_t fkalan(uint32_t m){ // aile-içi kalan: R0c R1c K1 K2 → 4 bit
  return ((m>>P_R0c)&1u) | (((m>>P_R1c)&1u)<<1) | (((m>>P_K1)&1u)<<2) | (((m>>P_K2)&1u)<<3); }
typedef struct { uint32_t m; int kapi; int Cfix, Cfree; } kayit_t;
static void degerle(const int *a, char kos, kayit_t *K, profil_t *P){
  degerlendir(a,P); uint32_t m=P->m[L0]; K->m=m; K->kapi=kapi_tut(P,L0,0);
  uint32_t kap=KAPALI_O|(kos=='P'?BITB_MASK:0);
  uint32_t ms=m&~kap; if(K->kapi) ms&=~KAPALI_SAYIM_EK; else ms&=~GATED_MASK;
  K->Cfix=kredi(ms,L0); int best=K->Cfix;
  for(int L=0;L<NLENS;L++){ if(L==L0) continue; int c=sayim_mercek(P,L,0); if(c>best) best=c; }
  K->Cfree=best; }
static inline double bit_loo(long c_atleast, long n, int kendi){ long c=c_atleast-kendi, nn=n-1; if(c<=0) return INFINITY; return log2((double)nn)-log2((double)c); }
int main(int argc,char**argv){
  int kod=atoi(argv[1]); char kos=argv[2][0]; const char *on=argv[3]; butunluk_init(kod);
  KAPALI_O=(1u<<P_F1)|(1u<<P_F2)|(1u<<P_R1e)|(1u<<P_Qc)|(1u<<P_QS)|(1u<<P_Qh)|(1u<<P_QkF);
  KAPALI_SAYIM_EK=(1u<<P_R1c);
  // havuzu oku
  long cap=1<<20, nb=0; int16_t *A=malloc(sizeof(int16_t)*114*cap); uint64_t *sira=malloc(8*cap);
  for(int fi=4;fi<argc;fi++){ FILE *f=fopen(argv[fi],"rb"); unsigned char rec[244];
    while(fread(rec,244,1,f)==1){ if(nb==cap){ cap*=2; A=realloc(A,sizeof(int16_t)*114*cap); sira=realloc(sira,8*cap); }
      for(int i=0;i<114;i++){ uint16_t v; memcpy(&v,rec+2*i,2); A[nb*114+i]=(int16_t)v; } memcpy(&sira[nb],rec+236,8); nb++; } fclose(f); }
  kayit_t *K=malloc(sizeof(kayit_t)*nb); static profil_t P;
  long Bc[4]={0}, Fc[16]={0}, Wc[64]={0};
  for(long b=0;b<nb;b++){ int a[114]; for(int i=0;i<114;i++) a[i]=A[b*114+i]; degerle(a,kos,&K[b],&P);
    uint32_t m=K[b].m; Bc[bmask2(m)]++; Fc[fkalan(m)]++; Wc[wmask6(m)]++; }
  // en-az sayıları
  long Ba[4], Fa[16], Wa[64];
  for(int x=0;x<4;x++){ Ba[x]=0; for(int y=0;y<4;y++) if((y&x)==x) Ba[x]+=Bc[y]; }
  for(int x=0;x<16;x++){ Fa[x]=0; for(int y=0;y<16;y++) if((y&x)==x) Fa[x]+=Fc[y]; }
  for(int x=0;x<64;x++){ Wa[x]=0; for(int y=0;y<64;y++) if((y&x)==x) Wa[x]+=Wc[y]; }
  // Bilgi puanı: grup başına en-az sayısı; havuz kitapları için biri-dışarıda (c−1, n−1), Kur'an için havuz sayıları (c, n).
  // Çözülmemiş arama (c < 10): ONKAYIT §3.3'teki gibi Poisson–CP sınırları; nokta yalnız geçici.
  #define TOPLA(y,acc) do{ acc.pt+=y.pt; acc.lo+=y.lo; acc.hi+=y.hi; acc.coz=acc.coz&&y.coz; }while(0)
  kayit_t Q; degerle(BASE,kos,&Q,&P);
  yuzey_t IQ={0,0,0,1};
  { uint32_t m=Q.m; if(kos!='P' && bmask2(m)){ yuzey_t y=yuzey(Ba[bmask2(m)],nb); TOPLA(y,IQ); }
    if(fkalan(m)){ yuzey_t y=yuzey(Fa[fkalan(m)],nb); TOPLA(y,IQ); } if(wmask6(m)){ yuzey_t y=yuzey(Wa[wmask6(m)],nb); TOPLA(y,IQ); } }
  long kIl=0,kIp=0,kIh=0, kC=0, kCf=0; double *Ib=malloc(sizeof(double)*nb);
  for(long b=0;b<nb;b++){ uint32_t m=K[b].m; yuzey_t I={0,0,0,1};
    if(kos!='P' && bmask2(m)){ yuzey_t y=yuzey(Ba[bmask2(m)]-1,nb-1); TOPLA(y,I); }
    if(fkalan(m)){ yuzey_t y=yuzey(Fa[fkalan(m)]-1,nb-1); TOPLA(y,I); } if(wmask6(m)){ yuzey_t y=yuzey(Wa[wmask6(m)]-1,nb-1); TOPLA(y,I); }
    Ib[b]=I.pt; if(I.lo>=IQ.pt-1e-9) kIl++; if(I.pt>=IQ.pt-1e-9) kIp++; if(I.hi>=IQ.pt-1e-9) kIh++;
    if(K[b].Cfix>=Q.Cfix) kC++; if(K[b].Cfree>=Q.Cfree) kCf++; }
  char fn[1024]; snprintf(fn,1024,"%s.txt",on); FILE *fo=fopen(fn,"w");
  fprintf(fo,"kosul %c kodeks %d havuz N %ld\n",kos,kod,nb);
  fprintf(fo,"Kuran: maske@(1,1,19) 0x%05x kapi %d | sayim(sabit mercek) %d | sayim(serbest mercek) %d | bilgi nokta %.4f bit (alt %.4f, ust %.4f, cozulmus %d)\n",
    Q.m,Q.kapi,Q.Cfix,Q.Cfree,IQ.pt,IQ.lo,IQ.hi,IQ.coz);
  fprintf(fo,"sira bilgi (Kuran noktasina gore): k_alt %ld k_nokta %ld k_ust %ld -> p_nokta %.6g [%.6g, %.6g] | sayim sabit: k %ld p %.6g | sayim serbest: k %ld p %.6g\n",
    kIl,kIp,kIh,(kIp+1.0)/(nb+1.0),(kIl+1.0)/(nb+1.0),(kIh+1.0)/(nb+1.0),kC,(kC+1.0)/(nb+1.0),kCf,(kCf+1.0)/(nb+1.0));
  // yüklem sıklıkları (havuzda): kalan yüklemler
  const int kal[]={P_B1,P_B2,P_R0c,P_R1c,P_K1,P_K2,P_W1,P_QU,P_QkH,P_Qa,P_Qn,P_QH};
  fprintf(fo,"kalan yuklem sikliklari (havuz):"); for(unsigned i=0;i<sizeof kal/sizeof kal[0];i++){ long c=0; for(long b=0;b<nb;b++) if(K[b].m>>kal[i]&1) c++; fprintf(fo," %s=%.5g",PNAME[kal[i]],(double)c/nb); } fprintf(fo,"\n");
  // sayım histogramları
  long hs[NPRED+1]={0}, hf[NPRED+1]={0}; for(long b=0;b<nb;b++){ hs[K[b].Cfix]++; hf[K[b].Cfree]++; }
  fprintf(fo,"hist sayim sabit:"); for(int c=0;c<=NPRED;c++) fprintf(fo," %ld",hs[c]); fprintf(fo,"\nhist sayim serbest:"); for(int c=0;c<=NPRED;c++) fprintf(fo," %ld",hf[c]); fprintf(fo,"\n");
  // en yüksek 20 (bilgi)
  long ix[20]; int nt=0; for(long b=0;b<nb;b++){ if(nt<20){ ix[nt++]=b; } else { int mi=0; for(int j=1;j<20;j++) if(Ib[ix[j]]<Ib[ix[mi]]) mi=j; if(Ib[b]>Ib[ix[mi]]) ix[mi]=b; } }
  for(int i=0;i<nt;i++) for(int j=i+1;j<nt;j++) if(Ib[ix[j]]>Ib[ix[i]]){ long t=ix[i]; ix[i]=ix[j]; ix[j]=t; }
  fprintf(fo,"en yuksek 20 (bilgi): sira | bilgi | sayim sabit | maske | zincir/sira\n");
  for(int i=0;i<nt;i++){ long b=ix[i]; fprintf(fo,"  %ld %.4f %d 0x%05x %llu\n",b,Ib[b],K[b].Cfix,K[b].m,(unsigned long long)sira[b]); }
  fclose(fo); return 0; }
