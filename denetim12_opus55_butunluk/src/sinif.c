// sinif — keşif (ek/EK_20261026T181111Z_sinif_kisitli_kesif.md): sınıf-kısıtlı puanlar. Her kitap için yalnız ön-kayıtlı
// Kur'an-sınıfı mercekleri (kapı G ∧ ≥ 3 motif türü ∧ ≥ 3 düzey, kapılı profilde) üzerinden: en büyük bilgi puanı (v = 0 nokta;
// tablo verilirse) ve en büyük sayım kredisi (kapı G) ve S0 kredisi. Kullanım: ./sinif <kodeks> <tablo|-> <R|T> <cikti> <aday>...
#include "bilgi.h"
#define T_TUR(m) ( (((m)&((1u<<P_B1)|(1u<<P_B2)|(1u<<P_F1)|(1u<<P_F2)|(1u<<P_R0c)|(1u<<P_R0e)|(1u<<P_R1c)|(1u<<P_R1e)))!=0) \
               + (((m)&((1u<<P_Qc)|(1u<<P_QS)|(1u<<P_Qh)|(1u<<P_QkF)|(1u<<P_QU)|(1u<<P_QkH)|(1u<<P_Qa)|(1u<<P_Qn)|(1u<<P_QH)))!=0) \
               + (((m)&(1u<<P_W1))!=0) + (((m)&((1u<<P_K1)|(1u<<P_K2)))!=0) )
#define T_DUZ(m) ( (((m)&((1u<<P_B1)|(1u<<P_B2)))!=0) + (((m)&((1u<<P_F1)|(1u<<P_F2)|(1u<<P_QS)|(1u<<P_Qh)|(1u<<P_QkF)|(1u<<P_K2)))!=0) \
               + (((m)&((1u<<P_R0c)|(1u<<P_R0e)|(1u<<P_R1c)|(1u<<P_R1e)|(1u<<P_K1)|(1u<<P_Qc)))!=0) \
               + (((m)&((1u<<P_W1)|(1u<<P_QU)|(1u<<P_QkH)|(1u<<P_Qa)|(1u<<P_Qn)|(1u<<P_QH)))!=0) )
static uint32_t sifir_maske(const int *a, int L){ const int f=L/NPR, p=PR[L%NPR], al=AL[f], be=BE[f]; long s[4]={0,0,0,0}, sH=0;
  for(int n=1;n<=114;n++){ int x=a[n-1]; int t=al*n+be*x, u=al*n-be*x; if(t%p==0){ int ci=(t&1)*2+(n&1); s[ci]+=t/p; } if(u%p==0) sH+=u/p; }
  uint32_t z=0; long h0=s[0]+s[1], h1=s[2]+s[3]; if(h0==0&&h1==0) z|=1u<<P_F2; if(s[0]==0&&s[1]==0) z|=1u<<P_R0e; if(s[2]==0&&s[3]==0) z|=1u<<P_R1e;
  if(h0+h1==0) z|=1u<<P_QS; if(h0==0||h1==0) z|=1u<<P_Qh; if(sH==0) z|=1u<<P_QH; return z; }
static void kitap(const tablo_t *T, int tab, const int *a, double *bi, int *ci, int *si, int *Lb){
  static profil_t P; degerlendir(a,&P); *bi=-1e300; *ci=-1; *si=-1; *Lb=-1;
  for(int L=0;L<NLENS;L++){ if(!kapi_tut(&P,L,0)) continue; uint32_t m=P.m[L]; if(T_TUR(m)<3 || T_DUZ(m)<3) continue;
    int c=kredi(m,L); if(c>*ci) *ci=c; uint32_t ms=m&~sifir_maske(a,L); int s=kredi(ms,L); if(s>*si) *si=s;
    if(tab){ bilesen_t b; mercek_bilesen(T,&P,L,&b); double v=b.B.pt+b.S.pt+b.F.pt+b.W.pt-COST[0][L]; if(v>*bi){ *bi=v; *Lb=L; } } } }
int main(int argc,char**argv){ int kod=atoi(argv[1]); butunluk_init(kod); bedel_hazirla(); tablo_t T; int tab=strcmp(argv[2],"-")!=0;
  if(tab && !tablo_oku(&T,argv[2])){ fprintf(stderr,"tablo\n"); return 2; } char tur=argv[3][0]; FILE *fo=fopen(argv[4],"w");
  double bq; int cq, sq, Lq; kitap(&T,tab,BASE,&bq,&cq,&sq,&Lq); fprintf(fo,"KURAN %.4f %d %d %d\n",bq,cq,sq,Lq);
  int R = tur=='R'?242:362;
  for(int fi=5;fi<argc;fi++){ FILE *f=fopen(argv[fi],"rb"); if(!f) continue; unsigned char r[400];
    while(fread(r,R,1,f)==1){ int a[114]; for(int i=0;i<114;i++){ uint16_t v; memcpy(&v,r+2*i,2); a[i]=v; } uint64_t sira; memcpy(&sira,r+R-8,8);
      double b; int c,s,L; kitap(&T,tab,a,&b,&c,&s,&L); fprintf(fo,"%s %llu %.4f %d %d %d\n",argv[fi],(unsigned long long)sira,b,c,s,L); } fclose(f); }
  fclose(fo); return 0; }
