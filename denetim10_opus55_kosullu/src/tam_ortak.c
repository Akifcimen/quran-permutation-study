// tam_ortak — Kur'an'ı geçen ya da ona yakın kitapların (ve Kur'an'ın) tekrarsız hikâyelerinin TAM BİRLİKTE-OLASILIĞI,
// koşullu sıfırda doğrudan sayımla (bağımsızlık varsayımı yok):
//   düzey 1: havuzda (LOO) hikâyenin bütün olgularını taşıyan kitap sayısı h1; h1 ≥ HMIN ise P = h1/(N−1).
//   düzey 2 (h1 < HMIN): hikâye = S_F (yalnız aileye bağlı olgular) ∪ S_R. P(S) = (n_F/N_s)·ort_{s∈örnek}[ h_s / R ],
//     n_F = S_F'yi taşıyan iskelet sayısı; her örnek iskelet için R tamamlama (tamamla.h zinciri, K adım arayla) sayılır.
//     Bir iskeletin tamamlamaları o iskeletin koşullu dağılımındandır → kestirici yansızdır (bağımsızlık varsayımı yok).
// Kullanım: ./tam_ortak <kodeks> <O|P> <U|M|L> <kitap_kapali> <seed> <R> <K> <DELTA> <NBANT> <MCAP> <cikti> <puan.bin> <kayit.bin>...
#include "kurallar.h"
#include "tamamla.h"
typedef struct __attribute__((packed)) { uint16_t a[114]; uint32_t f; uint32_t zincir; uint64_t sira; } kayit_t;
typedef struct __attribute__((packed)) { double t2r, t2t; uint64_t hr_lo, hr_hi, ht_lo, ht_hi; uint32_t dosya; uint32_t sira; } cikti_t;
#define HMIN 30
static u128 FSEV;  // yalnız aileye (iskelete) bağlı olgular
static void fsev_kur(void){ FSEV=0; for(int f=0;f<NFACT;f++){ int n=FNODE[f]; if(n>=1&&n<=5) FSEV|=BIT(f); }
  FSEV|=BIT(RP_F)|BIT(RP_MODEN)|BIT(MD_F0)|BIT(MD_FH)|BIT(MQ_F0)|BIT(MQ_FH); }
typedef struct { u128 S, SF, SR; long sahip; double t2t, t2r; long h1; int duzey; long nF; long m; double h2sum; long *isk; int nisk; } hik_t;
int main(int argc, char **argv){
  if(argc<14){ fprintf(stderr,"kullanım bkz. kaynak\n"); return 1; }
  int kod=atoi(argv[1]); char kos=argv[2][0], nul=argv[3][0]; int kk=atoi(argv[4]); uint64_t seed=strtoull(argv[5],NULL,10);
  long R=atol(argv[6]); int Kk=atoi(argv[7]); double DELTA=atof(argv[8]); int NBANT=atoi(argv[9]); long MCAP=atol(argv[10]); const char *out=argv[11];
  init_ortak(kod); fsev_kur(); kurallar_t K; kurallar_kur(&K,kos,kk);
  double q=0; if(nul=='L'){ ref_hazirla(); q=mallows_q_ayarla(ters_kesin(BASE)); }
  long cap=1<<20, N=0; int16_t (*A)[114]=malloc(sizeof(*A)*cap);
  for(int d=13; d<argc; d++){ FILE *fi=fopen(argv[d],"rb"); if(!fi){ fprintf(stderr,"açılamadı: %s\n",argv[d]); return 1; } kayit_t r; while(fread(&r,sizeof r,1,fi)==1){ if(N==cap){ cap*=2; A=realloc(A,sizeof(*A)*cap);} for(int i=0;i<114;i++) A[N][i]=r.a[i]; N++; } fclose(fi); }
  cikti_t *P=malloc(sizeof(cikti_t)*N); FILE *fp=fopen(argv[12],"rb"); if(!fp){ fprintf(stderr,"puan dosyası açılamadı\n"); return 1; } if(fread(P,sizeof(cikti_t),N,fp)!=(size_t)N){ fprintf(stderr,"puan dosyası uyuşmuyor\n"); return 1; } fclose(fp);
  u128 *bits=malloc(16*N); tablo_t *T=calloc(1,sizeof(tablo_t)); fres_t *Rr=malloc(sizeof(fres_t)*N);
  for(long i=0;i<N;i++){ int a[114]; for(int j=0;j<114;j++) a[j]=A[i][j]; facts19(a,&Rr[i],0); bits[i]=Rr[i].bits; tablo_ekle(T,&Rr[i]); }
  fres_t RQ; facts19(BASE,&RQ,0); puan_t PQ; puanla(T,&RQ,0,&K,&PQ);
  // seçim: yakın (T2* ≥ Q − DELTA) + bant [Q−DELTA−1, Q−DELTA) içinden NBANT rastgele
  rng_t rg; rseed(&rg,seed);
  long nh=0, hcap=1024; hik_t *H=calloc(hcap,sizeof(hik_t));
  H[nh].S=PQ.hik_t; H[nh].sahip=-1; H[nh].t2t=PQ.t2t; H[nh].t2r=PQ.t2r; nh++;
  long *bant=malloc(8*N), nb=0;
  for(long i=0;i<N;i++){ double t=P[i].t2t; u128 S=((u128)P[i].ht_hi<<64)|P[i].ht_lo;
    if(getenv("BANT_TAMAMI")){ if(t>=PQ.t2t-DELTA-1.0 && t<PQ.t2t-DELTA){ if(nh==hcap){ hcap*=2; H=realloc(H,sizeof(hik_t)*hcap); memset(H+nh,0,sizeof(hik_t)*(hcap-nh)); } H[nh].S=S; H[nh].sahip=i; H[nh].t2t=t; H[nh].t2r=P[i].t2r; nh++; } continue; }
    if(t>=PQ.t2t-DELTA){ if(nh==hcap){ hcap*=2; H=realloc(H,sizeof(hik_t)*hcap); memset(H+nh,0,sizeof(hik_t)*(hcap-nh)); } H[nh].S=S; H[nh].sahip=i; H[nh].t2t=t; H[nh].t2r=P[i].t2r; nh++; }
    else if(t>=PQ.t2t-DELTA-1.0) bant[nb++]=i; }
  long nyakin=nh-1;
  if(getenv("SADECE_KURAN")){ nh=1; nyakin=0; }   // çözünürlük eki: yalnız Kur'an'ın hikâyesi (yöntem aynı)
  long nsec=NBANT<nb?NBANT:nb; if(getenv("SADECE_KURAN")) nsec=0;
  for(long t=0;t<nsec;t++){ long j=t+(long)bounded(&rg,(uint32_t)(nb-t)); long x=bant[t]; bant[t]=bant[j]; bant[j]=x; long i=bant[t];
    if(nh==hcap){ hcap*=2; H=realloc(H,sizeof(hik_t)*hcap); memset(H+nh,0,sizeof(hik_t)*(hcap-nh)); } H[nh].S=((u128)P[i].ht_hi<<64)|P[i].ht_lo; H[nh].sahip=i; H[nh].t2t=P[i].t2t; H[nh].t2r=P[i].t2r; nh++; }
  // düzey 1
  for(long j=0;j<nh;j++){ H[j].SF=H[j].S&FSEV; H[j].SR=H[j].S&~FSEV; long c=0; for(long i=0;i<N;i++) if(i!=H[j].sahip && (bits[i]&H[j].S)==H[j].S) c++; H[j].h1=c; H[j].duzey=1; }
  // düzey 2 gerekenler: iskelet listeleri
  long nskel=(N), need=0; uint8_t *gerek=calloc(N,1);
  int yalniz1=getenv("DUZEY1")?1:0;   // O'-havuzları: yalnız düzey 1 (ONKAYIT §4.8)
  for(long j=0;j<nh;j++){ if(H[j].h1>=HMIN || yalniz1) continue; H[j].duzey=2; long nF=0; for(long i=0;i<N;i++) if(i!=H[j].sahip && (bits[i]&H[j].SF)==H[j].SF) nF++; H[j].nF=nF;
    long m=nF<MCAP?nF:MCAP; H[j].isk=malloc(8*(m>0?m:1)); H[j].nisk=0;
    // nF içinden m tane düzgün (rezervuar)
    long seen=0; for(long i=0;i<N;i++){ if(i==H[j].sahip || (bits[i]&H[j].SF)!=H[j].SF) continue; seen++; if(H[j].nisk<m) H[j].isk[H[j].nisk++]=i; else { long r=(long)bounded(&rg,(uint32_t)seen); if(r<m) H[j].isk[r]=i; } }
    for(int t=0;t<H[j].nisk;t++){ if(!gerek[H[j].isk[t]]) need++; gerek[H[j].isk[t]]=1; } }
  // iskelet → onu isteyen hikâyeler
  long *ofs=calloc(N+1,sizeof(long)); for(long j=0;j<nh;j++) if(H[j].duzey==2) for(int t=0;t<H[j].nisk;t++) ofs[H[j].isk[t]+1]++;
  for(long i=0;i<N;i++) ofs[i+1]+=ofs[i]; long *lst=malloc(8*(ofs[N]+1)); long *pos=calloc(N,sizeof(long));
  for(long j=0;j<nh;j++) if(H[j].duzey==2) for(int t=0;t<H[j].nisk;t++){ long i=H[j].isk[t]; lst[ofs[i]+pos[i]++]=j; }
  double *h2=calloc(nh,sizeof(double)), *h2q=calloc(nh,sizeof(double)); long *h2n=calloc(nh,sizeof(long));
  if(getenv("KURU")){ long n2=0; for(long j=0;j<nh;j++) n2+=(H[j].duzey==2); printf("KURU: hikâye %ld (yakın %ld), düzey-2 hikâye %ld, düzey-2 iskelet %ld, liste uzunluğu %ld\n",nh,nyakin,n2,need,ofs[N]); return 0; }
  long done=0; int parca_i=0, parca_n=1; if(getenv("PARCA")){ sscanf(getenv("PARCA"),"%d/%d",&parca_i,&parca_n); }
  for(long i=0;i<N;i++){ if(!gerek[i]) continue; if((i%parca_n)!=parca_i) continue; int a0[114]; for(int j=0;j<114;j++) a0[j]=A[i][j];
    tz_t Z; tz_init(&Z,nul,a0,seed*1000003ULL+(uint64_t)i,q); long *cnt=calloc(ofs[i+1]-ofs[i],sizeof(long));
    for(long t=0;t<R;t++){ for(int s=0;s<Kk;s++) tz_adim(&Z); fres_t Rc; facts19(Z.a,&Rc,0);
      for(long u=ofs[i];u<ofs[i+1];u++){ long j=lst[u]; if((Rc.bits&H[j].SR)==H[j].SR) cnt[u-ofs[i]]++; } }
    for(long u=ofs[i];u<ofs[i+1];u++){ double x=cnt[u-ofs[i]]/(double)R; h2[lst[u]]+=x; h2q[lst[u]]+=x*x; h2n[lst[u]]+=cnt[u-ofs[i]]; }
    free(cnt); done++; if(done%2000==0){ fprintf(stderr,"  iskelet %ld/%ld\n",done,need); } }
  // sonuçlar
  char fn[600]; if(parca_n>1) snprintf(fn,600,"%s.tam.parca%d.txt",out,parca_i); else snprintf(fn,600,"%s.tam.txt",out); FILE *fo=fopen(fn,"w");
  fprintf(fo,"# kodeks %d koşul %c sıfır %c N=%ld | Kur'an T2*=%.4f | yakın (≥Q−%.2f) %ld kitap, bant [Q−%.2f, Q−%.2f) %ld örnek | düzey-2 iskelet %ld, R=%ld, K=%d, MCAP=%ld\n",
    kod,kos,nul,N,PQ.t2t,DELTA,nyakin,DELTA+1,DELTA,nh-1-nyakin,need,R,Kk,MCAP);
  fprintf(fo,"# sahip T2* T2 düzey h1 nF m h2toplam h2kare h2isabet P_tam S_tam=-log10(P) | hikâye (onaltılık)\n");
  for(long j=0;j<nh;j++){ double Pt, Ns=(double)(H[j].sahip>=0?N-1:N);
    if(H[j].duzey==1) Pt=H[j].h1/Ns; else Pt=(H[j].nF/Ns)*(H[j].nisk>0?h2[j]/H[j].nisk:0);
    fprintf(fo,"%ld %.4f %.4f %d %ld %ld %d %.6f %.6f %ld %.4e %.4f | %016llx%016llx\n",H[j].sahip,H[j].t2t,H[j].t2r,H[j].duzey,H[j].h1,H[j].nF,H[j].nisk,h2[j],h2q[j],h2n[j],Pt,Pt>0?-log10(Pt):99.0,
      (unsigned long long)(uint64_t)(H[j].S>>64),(unsigned long long)(uint64_t)H[j].S); }
  fclose(fo); printf("tam_ortak: %ld hikâye (yakın %ld), düzey-2 iskelet %ld\n",nh,nyakin,need); return 0; }
