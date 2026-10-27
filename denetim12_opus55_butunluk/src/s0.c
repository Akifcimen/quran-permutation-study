// s0 — SONRADAN duyarlılık S0 (ek/EK_20261026T172158Z_sifir_toplam_yozlasmasi.md): sayım puanında toplamı sıfır olan
// karşılaştırmalar kredi almaz (F2: iki yarı 0; R0e: s00 = s01 = 0; R1e: s10 = s11 = 0; QS: Σq = 0; Qh: bir yarı 0; QH: Σq(H) = 0).
// Ön-kayıtlı kod (butunluk.h) değiştirilmez; sıfır bayrakları burada ayrıca hesaplanır.
// Kullanım: ./s0 <kodeks> <R|T|M|K> <cikti.txt> <aday dosyalari...>   (R: referans adayı 242 B; T: sınama adayı; M: menü adayı; K: yalnız Kur'an)
#include "menu.h"
static uint32_t sifir_maske(const int *a, int L){ // L merceğinde sıfır toplam yüzünden kredisi düşecek yüklemler
  const int f=L/NPR, p=PR[L%NPR], al=AL[f], be=BE[f]; long s[4]={0,0,0,0}, sH=0; int c[4]={0,0,0,0};
  for(int n=1;n<=114;n++){ int x=a[n-1]; int t=al*n+be*x, u=al*n-be*x;
    if(t%p==0){ int ci=(t&1)*2+(n&1); c[ci]++; s[ci]+=t/p; } if(u%p==0) sH+=u/p; }
  uint32_t z=0; long h0=s[0]+s[1], h1=s[2]+s[3];
  if(h0==0 && h1==0) z|=1u<<P_F2; if(s[0]==0 && s[1]==0) z|=1u<<P_R0e; if(s[2]==0 && s[3]==0) z|=1u<<P_R1e;
  if(h0+h1==0) z|=1u<<P_QS; if(h0==0 || h1==0) z|=1u<<P_Qh; if(sH==0) z|=1u<<P_QH; return z; }
static int s0_puan(const int *a, int *enL){ static profil_t P; degerlendir(a,&P); int best=-1, bl=-1;
  for(int L=0;L<NLENS;L++){ uint32_t m=P.m[L]&~sifir_maske(a,L); if(!kapi_tut(&P,L,0)) m&=~GATED_MASK; int c=kredi(m,L); if(c>best){ best=c; bl=L; } }
  if(enL) *enL=bl; return best; }
int main(int argc,char**argv){ int kod=atoi(argv[1]); char tur=argv[2][0]; butunluk_init(kod); FILE *fo=fopen(argv[3],"w");
  int L; int q=s0_puan(BASE,&L); static profil_t P; degerlendir(BASE,&P); fprintf(fo,"KURAN S0 %d mercek (%d,%+d,%d) C %d\n",q,AL[L/NPR],BE[L/NPR],PR[L%NPR],sayim_puani(&P,0,NULL));
  int REC = tur=='R'?242 : tur=='T'? (228+4*8*3+2*8+8+4+2+8) : 0;
  if(tur=='M'){ REC=228+3+2+4+8; }
  for(int fi=4;fi<argc;fi++){ FILE *f=fopen(argv[fi],"rb"); if(!f) continue; unsigned char r[512]; long idx=0;
    while(fread(r,REC,1,f)==1){ int a[114]; for(int i=0;i<114;i++){ uint16_t v; memcpy(&v,r+2*i,2); a[i]=v; }
      uint64_t sira; memcpy(&sira,r+REC-8,8); int C; if(tur=='R') C=r[228]; else if(tur=='T') C=r[228+4*8*3+2*8+8]; else C=r[228];
      int Ls; int s=s0_puan(a,&Ls); fprintf(fo,"%s %llu C %d S0 %d mercek %d\n",argv[fi],(unsigned long long)sira,C,s,Ls); idx++; }
    fclose(f); }
  fclose(fo); return 0; }
