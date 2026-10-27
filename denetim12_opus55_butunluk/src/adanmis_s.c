// adanmis_s — sıralı (ve öteki) sıfırlarda adanmış kestirim (ONKAYIT §4.3): tek mercekli doğrudan örnekleme. Sıfırın kendi
// örnekleyicisiyle (yeni tohum) kitap üretilir; yalnız hedef mercekte aile/iki-el maskeleri hesaplanır; k_F hedef kümesindeyse
// hedef maskelerin en-az isabetleri sayılır. Tek mercek değerlendirmesi degerlendir() ile aynı tanımlardır (DENETIM=1 ile sınanır).
// Kullanım: ./adanmis_s <kodeks> <sifir U|M|L|J> <bicim> <p> <K listesi, virgüllü> <kitap_sayisi> <tohum> <F|W|B> <maske1> [...]
// (B: kitap maskesi; biçim düzeyinde, p ve K yok sayılır)
#include "butunluk.h"
static inline void tek_mercek(const int *a, int f, int pi, uint32_t *fm, uint32_t *wm, int *kF){
  const int al=AL[f], be=BE[f], p=PR[pi]; int c[4]={0,0,0,0}; long s[4]={0,0,0,0}; fam_t F; F.k=0; F.qmin=1<<30; F.qmax=-(1<<30);
  int kH=0; long sqH=0, saU=0, snU=0; int kU=0, ni=0;
  for(int n=1;n<=114;n++){ const int x=a[n-1]; const int t=al*n+be*x, u=al*n-be*x; const int inF=(t%p==0), inH=(u%p==0);
    if(inF){ int q=t/p; int ci=(t&1)*2+(n&1); c[ci]++; s[ci]+=q; F.qv[F.k++]=q; if(q<F.qmin) F.qmin=q; if(q>F.qmax) F.qmax=q; }
    if(inH){ kH++; sqH+=u/p; } if(inF||inH){ kU++; saU+=x; snU+=n; } if(inF&&inH) ni++; }
  const int c0=c[0],c1=c[1],c2=c[2],c3=c[3]; const long s0=s[0],s1=s[1],s2=s[2],s3=s[3]; const int h0=c0+c1,h1=c2+c3; const long q0=s0+s1,q1=s2+s3,sq=q0+q1; const int dolu=c0&&c1&&c2&&c3;
  uint32_t m=0;
  if(h0==h1&&h0>=1) m|=1u<<P_F1; if(h0>=1&&h1>=1&&q0==q1) m|=1u<<P_F2; if(c0==c1&&c0>=1) m|=1u<<P_R0c; if(c0>=1&&c1>=1&&s0==s1) m|=1u<<P_R0e;
  if(c2==c3&&c2>=1) m|=1u<<P_R1c; if(c2>=1&&c3>=1&&s2==s3) m|=1u<<P_R1e; if(dolu&&kosu4(s0,s1,s2,s3)) m|=1u<<P_K1; if(kosu_q(&F)) m|=1u<<P_K2;
  if(dolu&&sq==4L*p) m|=1u<<P_Qc; if(F.k>=1&&sq%p==0) m|=1u<<P_QS; if(h0>=1&&h1>=1&&q0%p==0&&q1%p==0) m|=1u<<P_Qh; if(F.k==p) m|=1u<<P_QkF;
  if(F.k>=1&&kH>=1&&ni==0) m|=1u<<P_W1; if(kU==p) m|=1u<<P_QU; if(kH==p) m|=1u<<P_QkH; if(kU>=1&&saU%p==0) m|=1u<<P_Qa; if(kU>=1&&snU%p==0) m|=1u<<P_Qn; if(kH>=1&&sqH%p==0) m|=1u<<P_QH;
  m&=~EXCL[f*NPR+pi]; *fm=fmask11(m); *wm=wmask6(m); *kF=F.k; }
static inline uint32_t kitap_maske(const int *a, int f){ const int al=AL[f], be=BE[f], abe=abs(be); int e0=0; long sn0=0, sa0=0, st0=0, st=0;
  for(int n=1;n<=114;n++){ const int x=a[n-1]; const int t=al*n+be*x; st+=t; if(!(t&1)){ e0++; sn0+=n; sa0+=x; st0+=t; } }
  uint32_t mb=0; const int e1=114-e0; const long sn1=SN_TOT-sn0, sa1=SA_TOT-sa0, st1=st-st0;
  if(e0==e1) mb|=1u<<P_B1; if(e0>0 && e1>0 && ((long)al*sn0==(long)abe*sa1 || (long)al*sn1==(long)abe*sa0 || st0==st1)) mb|=1u<<P_B2;
  mb&=~EXCL[f*NPR]; return mb; }
int main(int argc,char**argv){
  if(argc<10){ fprintf(stderr,"kullanım\n"); return 1; }
  int kod=atoi(argv[1]); char sif=argv[2][0]; int f=atoi(argv[3]); int p=atoi(argv[4]); char *Ks=argv[5]; long NB=atol(argv[6]); uint64_t seed=strtoull(argv[7],0,10); char grp=argv[8][0];
  int nm=argc-9; uint32_t hedef[64]; for(int i=0;i<nm;i++) hedef[i]=(uint32_t)atoi(argv[9+i]);
  butunluk_init(kod); int pi=-1; for(int i=0;i<NPR;i++) if(PR[i]==p) pi=i; int Kset[128]={0}; { char *t=strtok(Ks,","); while(t){ Kset[atoi(t)]=1; t=strtok(NULL,","); } }
  orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,sif,seed); long n=0, h[64]={0}; long den_hata=0; static long nK[128], hK[128][64]; const int den=getenv("DENETIM")!=NULL; static profil_t P;
  if(grp=='B'){ long hb[64]={0}; long hata=0;
    for(long b=0;b<NB;b++){ orn_sonraki(Z); uint32_t mb=kitap_maske(Z->a,f);
      if(den && b<20000){ degerlendir(Z->a,&P); if(bmask2(P.m[f*NPR])!=mb) hata++; }
      for(int i=0;i<nm;i++) if((mb&hedef[i])==hedef[i]) hb[i]++; }
    printf("adanmis_s kodeks %d sifir %c bicim (%d,%+d) grup B: kitap %ld\n",kod,sif,AL[f],BE[f],NB);
    if(den) printf("  denetim (ilk 20000 kitap): kitap maskesi ↔ degerlendir uyuşmazlığı %ld\n",hata);
    for(int i=0;i<nm;i++) printf("  maske %u: isabet %ld / %ld, P^ = %.6g\n",hedef[i],hb[i],NB,(double)hb[i]/NB);
    return 0; }
  for(long b=0;b<NB;b++){ orn_sonraki(Z); uint32_t fm,wm; int kF; tek_mercek(Z->a,f,pi,&fm,&wm,&kF);
    if(den && b<20000){ degerlendir(Z->a,&P); int L=f*NPR+pi; if(fmask11(P.m[L])!=fm || wmask6(P.m[L])!=wm || P.kF[L]!=kF) den_hata++; }
    if(kF>=128 || !Kset[kF]) continue; n++; nK[kF]++; uint32_t mk=(grp=='F')?fm:wm; for(int i=0;i<nm;i++) if((mk&hedef[i])==hedef[i]){ h[i]++; hK[kF][i]++; } }
  printf("adanmis_s kodeks %d sifir %c bicim (%d,%+d) p %d K {%s} grup %c: kitap %ld, bağlamda %ld%s\n",kod,sif,AL[f],BE[f],p,argv[5],grp,NB,n,den?"":"");
  if(den) printf("  denetim (ilk 20000 kitap): tek-mercek ↔ degerlendir uyuşmazlığı %ld\n",den_hata);
  for(int i=0;i<nm;i++) printf("  maske %u: isabet %ld / %ld, P^ = %.6g\n",hedef[i],h[i],n,n?(double)h[i]/n:0.0);
  for(int k=0;k<128;k++) if(Kset[k] && nK[k]) for(int i=0;i<nm;i++) printf("  K=%d maske %u: isabet %ld / %ld, P^ = %.6g\n",k,hedef[i],hK[k][i],nK[k],(double)hK[k][i]/nK[k]);
  return 0; }
