// kuran_puan — Kur'an'ın bilgi puanı (8 değişke) ve sayım puanı (4 kapı); en iyi mercekler ve bileşenleri; çözülmemiş aramalar.
// Kullanım: ./kuran_puan <kodeks> <tablo.tab> [esik_cikti]
#include "bilgi.h"
static const char *KAPI[4]={"G","GE","Gc","G0"};
static void yaz_y(const char *ad, yuzey_t y){ if(y.coz) printf(" %s=%.3f", ad, y.pt); else printf(" %s=[%.3f,%s] (ÇÖZÜLMEDİ)", ad, y.lo, isinf(y.hi)?"inf":"…"); }
int main(int argc,char**argv){ int kod=atoi(argv[1]); butunluk_init(kod); bedel_hazirla(); tablo_t T;
  if(!tablo_oku(&T,argv[2])){ fprintf(stderr,"tablo okunamadı\n"); return 2; }
  static profil_t P; degerlendir(BASE,&P); bilgi_t B; bilgi_puan(&T,&P,&B);
  printf("Kur'an (%d), referans N = %llu\n",kod,(unsigned long long)T.N);
  for(int v=0;v<NVAR;v++){ int L=B.L[v]; printf("  değişke %d (kapı %s, önsel %d): nokta %.3f bit  [alt %.3f, üst %s]  mercek (%d,%+d,%d) çözülmüş=%d\n",
      v, v>=4?"G":"yok", v&3, B.pt[v], B.lo[v], isinf(B.hi[v])?"inf":"sonlu", AL[L/NPR], BE[L/NPR], PR[L%NPR], B.coz[v]); }
  for(int g=0;g<4;g++){ int L; int c=sayim_puani(&P,g,&L); printf("  sayım puanı (kapı %s): %d, mercek (%d,%+d,%d)\n",KAPI[g],c,AL[L/NPR],BE[L/NPR],PR[L%NPR]); }
  // birincil değişkede en iyi 8 mercek
  double sc[NLENS]; int ix[NLENS]; for(int L=0;L<NLENS;L++){ bilesen_t b; mercek_bilesen(&T,&P,L,&b); sc[L]=b.B.pt+b.S.pt+b.F.pt+b.W.pt-COST[0][L]; ix[L]=L; }
  for(int i=0;i<NLENS;i++) for(int j=i+1;j<NLENS;j++) if(sc[ix[j]]>sc[ix[i]]){ int t=ix[i]; ix[i]=ix[j]; ix[j]=t; }
  printf("  birincil değişkede en iyi 8 mercek (bileşenler, bit):\n");
  for(int r=0;r<8;r++){ int L=ix[r]; bilesen_t b; mercek_bilesen(&T,&P,L,&b);
    printf("   (%d,%+d,%d) puan %.3f  kF=%d bağlam n=%llu bedel %.3f |", AL[L/NPR],BE[L/NPR],PR[L%NPR], sc[L], P.kF[L], (unsigned long long)b.n, COST[0][L]);
    yaz_y("B",b.B); yaz_y("boy",b.S); yaz_y("F",b.F); yaz_y("W",b.W); printf("  maske=0x%05x\n",P.m[L]); }
  if(argc>3){ FILE *f=fopen(argv[3],"w"); for(int v=0;v<NVAR;v++) fprintf(f,"SQ %d %.9f\n",v,B.pt[v]); for(int g=0;g<4;g++) fprintf(f,"CQ %d %d\n",g,sayim_puani(&P,g,NULL)); fclose(f); }
  return 0; }
