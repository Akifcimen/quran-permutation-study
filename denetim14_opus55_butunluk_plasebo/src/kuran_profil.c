// kuran_profil — Kur'an'ın verilen mercekteki L* profili (dondurmadan sonra; ONKAYIT §9 b). Kullanım: ./kuran_profil [alfa beta p]
#include "kutuphane.h"
#include "agac_uretim.h"
int main(int argc,char**argv){ kutuphane_init(); int al=1, be=1, p=19; if(argc>3){ al=atoi(argv[1]); be=atoi(argv[2]); p=atoi(argv[3]); }
  int f=-1, pi=-1; for(int i=0;i<NFORM;i++) if(AL[i]==al&&BE[i]==be) f=i; for(int i=0;i<NPR;i++) if(PR[i]==p) pi=i; if(f<0||pi<0) return 1; const int L=f*NPR+pi;
  static prof_t P; kdegerlendir(BASE,&P,0x3FFF); const uint64_t s[NGRUP]={P.B[L],P.Z[L],P.F[L],P.W[L],P.H[L]}; const char *GA="BZFWH"; int n=0, nh=0;
  printf("Kur'an (129) @ (%d,%+d,%d): kF=%d kH=%d en küçük hücre=%d kapı G=%d\n",al,be,p,P.kF[L],P.kH[L],P.mc[L],kapiG(&P,L));
  for(int k=0;k<NGRUP;k++){ printf("  %c:",GA[k]); uint64_t x=s[k]; while(x){ int b=__builtin_ctzll(x); x&=x-1; const int g=GOFS[k]+b; if(!LUYE[g]) continue; printf(" %s",PADI[g]); n++; } printf("\n"); }
  printf("  L* yüklemlerinden sağlanan: %d / %d\n",n,NLSTAR); return 0; }
