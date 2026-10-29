// dil_cikar — her hedefe D(x) uygulanır (ONKAYIT §4); diller dondurulmak üzere yazılır.
// Kullanım: ./dil_cikar <hedef.bin> <onsel 0|1> <ad_onek> <cikti.txt> <ayrinti.txt> <ref>...
//   cikti satırı: <ad> <hedef_no> <onsel> <L*> <n> <g1> ... <gn>   (ad: hedef 0 için "Q"/"QP2", ötekiler "<onek><no>")
//   ayrıntı: amaç, bilgi, dil bedeli, grup bilgileri, mercek, yüklem adları
#include "dil_sec.h"
int main(int argc,char**argv){ kutuphane_init(); mercek_bedeli_hazirla(); lmask_hazirla(); int onsel=atoi(argv[2]); const char *on=argv[3];
  static atablo_t T; atablo_kur(&T,argv+6,argc-6); genum_t *G=genum_ayir();
  FILE *f=fopen(argv[1],"rb"); fseek(f,0,SEEK_END); long sz=ftell(f); fseek(f,0,SEEK_SET); int NH=(int)(sz/232);
  FILE *o=fopen(argv[4],"w"), *ay=fopen(argv[5],"w"); static prof_t P; int maxh=getenv("HEDEF_SON")?atoi(getenv("HEDEF_SON")):NH-1;
  for(int h=0;h<NH;h++){ int32_t id; int16_t a16[114]; if(fread(&id,4,1,f)!=1||fread(a16,2,114,f)!=114||id!=h){ fprintf(stderr,"hedef\n"); return 2; } if(h>maxh) break;
    int a[114]; for(int i=0;i<114;i++) a[i]=a16[i]; kdegerlendir(a,&P,0x3FFF); dil_t d; dil_bul(&T,&P,onsel,G,&d);
    char ad[32]; if(h==0) snprintf(ad,32,onsel?"QP2":"Q"); else snprintf(ad,32,"%s%03d",on,h);
    fprintf(o,"%s %d %d %d %d",ad,h,onsel,d.L,d.n); for(int i=0;i<d.n;i++) fprintf(o," %d",d.g[i]); fprintf(o,"\n");
    const int f_=d.L/NPR, p=PR[d.L%NPR];
    fprintf(ay,"%s hedef %d mercek (%d,%+d,%d) amac %.6f bilgi %.6f dil_bedeli %.6f mercek_bedeli %.6f |Λ| %d grup_bilgi B %.4f Z %.4f F %.4f W %.4f H %.4f :",
      ad,h,AL[f_],BE[f_],p,d.amac,d.bilgi,d.bedel,LBEDEL[onsel][d.L],d.n,d.ig[0],d.ig[1],d.ig[2],d.ig[3],d.ig[4]);
    for(int i=0;i<d.n;i++) fprintf(ay," %s",PADI[d.g[i]]); fprintf(ay,"\n"); }
  fclose(o); fclose(ay); fclose(f); return 0; }
