// ayrinti — kitap ayrıntısı (ONKAYIT §4.3, §7): sınama aday kayıtlarını (362 B) ya da Kur'an'ı okur; birincil bilgi puanında
// (v = 0) en iyi merceği, bileşenlerini, çözülmemiş aramaları ve S_Q'ya ulaşabilecek bütün mercekleri listeler.
// Kullanım: ./ayrinti <kodeks> <tablo.tab> <S_Q> <cikti.tsv> <aday.bin>...   (aday yoksa yalnız Kur'an)
// Çıktı satırları: KITAP <dosya> <sira> pt lo hi C_G | MERCEK <L> f p kF ctx n puan_pt puan_lo puan_hi B(c,n) S(c,n) F(m,c) W(m,c) maske kapi
#include "bilgi.h"
typedef struct __attribute__((packed)) { uint16_t a[114]; float pt[NVAR], lo[NVAR], hi[NVAR]; int16_t L[NVAR]; uint8_t coz[NVAR]; uint8_t C[4]; int16_t CL; uint64_t sira; } kayit_t;
static void kitap(FILE *fo, const tablo_t *T, const int *a, const char *ad, uint64_t sira, double SQ){
  static profil_t P; degerlendir(a,&P); bilgi_t B; bilgi_puan(T,&P,&B); int C=sayim_puani(&P,0,NULL);
  fprintf(fo,"KITAP\t%s\t%llu\t%.4f\t%.4f\t%.4f\t%d\n",ad,(unsigned long long)sira,B.pt[0],B.lo[0],isinf(B.hi[0])?1e9:B.hi[0],C);
  for(int L=0;L<NLENS;L++){ bilesen_t b; mercek_bilesen(T,&P,L,&b); double c=COST[0][L];
    double sp=b.B.pt+b.S.pt+b.F.pt+b.W.pt-c, sl=b.B.lo+b.S.lo+b.F.lo+b.W.lo-c, sh=b.B.hi+b.S.hi+b.F.hi+b.W.hi-c;
    if(L!=B.L[0] && sh<SQ-1e-6) continue;   // yalnız en iyi mercek ve S_Q'ya ulaşabilecek mercekler
    const uint32_t m=P.m[L]; const int f=L/NPR, p=PR[L%NPR]; const int k=kkutu(P.kF[L]);
    uint64_t cB=bmask2(m)?T->BA[f][bmask2(m)]:T->N, cS=(m&(1u<<P_QkF))?(p<KC?T->NKU[(size_t)L*KC+p]:0):T->N;
    uint64_t cF=fmask11(m)?T->FA[(size_t)b.ctx*NFM+fmask11(m)]:b.n, cW=wmask6(m)?T->WA[(size_t)b.ctx*NWM+wmask6(m)]:b.n;
    fprintf(fo,"MERCEK\t%d\t%d\t%d\t%d\t%d\t%llu\t%.4f\t%.4f\t%.4f\t%llu\t%llu\t%u\t%llu\t%u\t%llu\t0x%05x\t%d\t%d\n",
      L,f,p,P.kF[L],b.ctx,(unsigned long long)b.n,sp,sl,isinf(sh)?1e9:sh,(unsigned long long)cB,(unsigned long long)cS,fmask11(m),(unsigned long long)cF,wmask6(m),(unsigned long long)cW,m,b.kapi,k); } }
int main(int argc,char**argv){ int kod=atoi(argv[1]); butunluk_init(kod); bedel_hazirla(); tablo_t T; if(!tablo_oku(&T,argv[2])){ fprintf(stderr,"tablo\n"); return 2; }
  double SQ=atof(argv[3]); FILE *fo=fopen(argv[4],"w");
  if(argc==5){ kitap(fo,&T,BASE,"KURAN",0,SQ); fclose(fo); return 0; }
  for(int fi=5;fi<argc;fi++){ FILE *f=fopen(argv[fi],"rb"); kayit_t K;
    while(fread(&K,sizeof K,1,f)==1){ int a[114]; for(int i=0;i<114;i++) a[i]=K.a[i]; kitap(fo,&T,a,argv[fi],K.sira,SQ); } fclose(f); }
  fclose(fo); return 0; }
