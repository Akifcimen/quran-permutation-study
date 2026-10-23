// zincir_sina — Ç4: iskelet sabit tamamlama zincirinin (tamamla.h) doğrulanması.
//  mod 'U': ilk NS O-iskeletinde, iskelet başına R kesin (ret) tamamlama ile R zincir kaydının (K adım arayla) frekansları.
//  mod 'S' (M20/Mallows): havuzun ilk NS iskeleti; zincir-ortalamalı frekans (iskelet başına R kayıt) ile havuzun doğrudan frekansı
//          (her iskeletin kendi tamamlaması = o sıfırın koşullu çekilişi).
// Karşılaştırılan: A (terazi), W (İki El) ve tamamlamaya bağlı bütün olgular (ön-kayıtlı beşli: A, W, U: |U|=m, U: m|Σa(U), MQ: 14/15).
// Kullanım: ./zincir_sina <kodeks> <U|M|L> <mod U|S> <seed> <NS> <R> <K> <cikti.txt> <havuz.bin>...
#include "tamamla.h"
typedef struct __attribute__((packed)) { uint16_t a[114]; uint32_t f; uint32_t zincir; uint64_t sira; } kayit_t;
static void u_tamamla(rng_t *rg, const int *a0, int *a){
  int P[114], V[114], np=0; for(int i=0;i<114;i++){ if(((i+1)+a0[i])%19==0) a[i]=a0[i]; else { P[np]=i; V[np]=a0[i]; np++; } }
  for(;;){ int ok=1; for(int i=0;i<np;i++){ int j=i+(int)bounded(rg,(uint32_t)(np-i)); int t=V[i]; V[i]=V[j]; V[j]=t; if((P[i]+1+V[i])%19==0){ ok=0; break; } } if(ok) break; }
  for(int i=0;i<np;i++) a[P[i]]=V[i]; }
#define NX (NF+2)   // olgular + A + W
static void say(const int *a, long *acc, long w){ fres_t R; facts19(a,&R,0); u128 b=R.bits; while(b){ int f=ctz128(b); b&=b-1; acc[f]+=w; } int e=olaylar(a); if(e&EV_A) acc[NF]+=w; if(e&EV_W) acc[NF+1]+=w; }
int main(int argc, char **argv){
  if(argc<10){ fprintf(stderr,"kullanım bkz. kaynak\n"); return 1; }
  int kod=atoi(argv[1]); char nul=argv[2][0], mod=argv[3][0]; uint64_t seed=strtoull(argv[4],NULL,10); long NS=atol(argv[5]), R=atol(argv[6]); int Kk=atoi(argv[7]);
  init_ortak(kod); double q=0; if(nul=='L'){ ref_hazirla(); q=mallows_q_ayarla(ters_kesin(BASE)); }
  static double c1[NX], c2[NX], c1q[NX], c2q[NX], dd[NX], ddq[NX]; static int degisken[NX]; long ns=0; rng_t rg; rseed(&rg,seed);
  for(int d=9; d<argc && ns<NS; d++){ FILE *fi=fopen(argv[d],"rb"); if(!fi){ fprintf(stderr,"açılamadı: %s\n",argv[d]); return 1; } kayit_t r;
    while(ns<NS && fread(&r,sizeof r,1,fi)==1){ int a0[114], a[114]; for(int i=0;i<114;i++) a0[i]=r.a[i]; ns++;
      long i1[NX]={0}, i2[NX]={0};   // tamsayı sayımlar (iskelet başına, R ölçeğinde)
      if(mod=='U'){ for(long t=0;t<R;t++){ u_tamamla(&rg,a0,a); say(a,i1,1); } }
      else say(a0,i1,R);   // doğrudan: iskeletin kendi tamamlaması (R ağırlıklı)
      tz_t Z; tz_init(&Z,nul,a0,seed*1000003ULL+(uint64_t)ns,q); for(long t=0;t<R;t++){ for(int s=0;s<Kk;s++) tz_adim(&Z); say(Z.a,i2,1); }
      for(int f=0;f<NX;f++){ double t1=i1[f]/(double)R, t2=i2[f]/(double)R; c1[f]+=t1; c2[f]+=t2; c1q[f]+=t1*t1; c2q[f]+=t2*t2;
        long di=i2[f]-i1[f]; if(di!=0) degisken[f]=1; double d=di/(double)R; dd[f]+=d; ddq[f]+=d*d; } }
    fclose(fi); }
  FILE *fo=fopen(argv[8],"w"); fprintf(fo,"# kodeks %d sıfır %c mod %c iskelet %ld R %ld K %d | sütunlar: olgu, %s ort, zincir ort, z_eşleşmemiş, z_eşleşmiş (aynı iskeletlerde fark; ölçüt)\n",kod,nul,mod,ns,R,Kk,mod=='U'?"kesin":"doğrudan");
  const char *ad[NX]; for(int f=0;f<NF;f++) ad[f]=f<NFACT?FNAME[f]:"-"; ad[NF]="OLAY A (terazi)"; ad[NF+1]="OLAY W (İki El)";
  double zmax=0; for(int f=0;f<NX;f++){ if(f<NF && (f>=NFACT)) continue; double m1=c1[f]/ns, m2=c2[f]/ns; if(m1==0&&m2==0) continue;
    double v1=(c1q[f]/ns-m1*m1)/ns, v2=(c2q[f]/ns-m2*m2)/ns; double z=(m2-m1)/sqrt(v1+v2+1e-300);
    double md=dd[f]/ns, vd=(ddq[f]/ns-md*md)/ns; double zp=degisken[f]?md/sqrt(vd+1e-300):0.0;
    fprintf(fo,"%-44s %.5e %.5e %+.2f %+.2f%s\n",ad[f],m1,m2,z,zp,degisken[f]?"":"   (iskelete bağlı: tamamlamayla değişmez)"); if(degisken[f] && fabs(zp)>zmax && (m1*ns>=50)) zmax=fabs(zp); }
  fprintf(fo,"# en büyük |z_eşleşmiş| (beklenen sayı ≥ 50 olanlar) = %.2f\n",zmax); fclose(fo); return 0; }
