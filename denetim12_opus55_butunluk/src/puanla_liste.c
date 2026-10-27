// puanla_liste — metin listesindeki kitapları (satır: kodeks \t ad \t 114 virgüllü değer) puanlar: sayım (4 kapı), S0 (sonradan),
// bilgi (8 değişke, verilen tablo ile; tablo "-" ise atlanır). Yalnız verilen kodeksin satırları. ./puanla_liste <kodeks> <tablo|-> <liste>
#include "bilgi.h"
static uint32_t sifir_maske(const int *a, int L){ const int f=L/NPR, p=PR[L%NPR], al=AL[f], be=BE[f]; long s[4]={0,0,0,0}, sH=0;
  for(int n=1;n<=114;n++){ int x=a[n-1]; int t=al*n+be*x, u=al*n-be*x; if(t%p==0){ int ci=(t&1)*2+(n&1); s[ci]+=t/p; } if(u%p==0) sH+=u/p; }
  uint32_t z=0; long h0=s[0]+s[1], h1=s[2]+s[3]; if(h0==0&&h1==0) z|=1u<<P_F2; if(s[0]==0&&s[1]==0) z|=1u<<P_R0e; if(s[2]==0&&s[3]==0) z|=1u<<P_R1e;
  if(h0+h1==0) z|=1u<<P_QS; if(h0==0||h1==0) z|=1u<<P_Qh; if(sH==0) z|=1u<<P_QH; return z; }
int main(int argc,char**argv){ int kod=atoi(argv[1]); butunluk_init(kod); bedel_hazirla(); tablo_t T; int tab=strcmp(argv[2],"-")!=0; if(tab && !tablo_oku(&T,argv[2])){ fprintf(stderr,"tablo\n"); return 2; }
  FILE *f=fopen(argv[3],"r"); char line[8192]; static profil_t P;
  while(fgets(line,sizeof line,f)){ char *k=strtok(line,"\t"), *ad=strtok(NULL,"\t"), *cs=strtok(NULL,"\t\n"); if(!k||!ad||!cs||atoi(k)!=kod) continue;
    int a[114], n=0; char *t=strtok(cs,","); while(t&&n<114){ a[n++]=atoi(t); t=strtok(NULL,","); } if(n!=114) continue;
    degerlendir(a,&P); int C[4], CL; for(int g=0;g<4;g++){ int L; C[g]=sayim_puani(&P,g,&L); if(!g) CL=L; }
    int s0=-1, s0L=-1; for(int L=0;L<NLENS;L++){ uint32_t m=P.m[L]&~sifir_maske(a,L); if(!kapi_tut(&P,L,0)) m&=~GATED_MASK; int c=kredi(m,L); if(c>s0){ s0=c; s0L=L; } }
    printf("%s\tsayım G/GE/Gc/G0 = %d/%d/%d/%d (mercek (%d,%+d,%d), maske 0x%05x, kF=%d, hücre-min %d)\tS0 = %d (%d,%+d,%d)",ad,C[0],C[1],C[2],C[3],AL[CL/NPR],BE[CL/NPR],PR[CL%NPR],P.m[CL],P.kF[CL],P.mc[CL],s0,AL[s0L/NPR],BE[s0L/NPR],PR[s0L%NPR]);
    if(tab){ bilgi_t B; bilgi_puan(&T,&P,&B); printf("\tbilgi v0 %.3f [%.3f, %s] (%d,%+d,%d) çözülmüş=%d; v4(G) %.3f",B.pt[0],B.lo[0],isinf(B.hi[0])?"inf":"sonlu",AL[B.L[0]/NPR],BE[B.L[0]/NPR],PR[B.L[0]%NPR],B.coz[0],B.pt[4]); }
    printf("\n"); }
  return 0; }
