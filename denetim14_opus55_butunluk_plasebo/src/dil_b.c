// dil_b — her hedefe dil kurma yordamı uygulanır (ONKAYIT §4, §6); diller dondurulmak üzere yazılır.
// Kullanım: ./dil_b <kip> <hedef.bin> <onek|onsel> <cikti.txt> <ayrinti.txt> <ref>...
//   kip P  : plasebo12'nin kısıtsız D'si (dil_cikar.c ile birebir aynı yol ve biçim; 3. argüman önsel 0|1, önek "x"). Ö0 sınaması için.
//   kip A, Am, Ap : bütünlük kısıtlı D_B (≥ 3/3, ≥ 2/2, 4/4 tür/düzey); kip B : bedelsiz en geniş dil (Kol B).
//   cikti satırı: <ad> <hedef_no> <onsel+10·kol> <L> <n> <g1> ... <gn>   (ad: <kip><hedef_no, 3 hane>; dilsiz hedefte L = −1, n = 0)
//   HEDEF_SON ortam değişkeni: son hedef numarası (P kipinde plasebo12 ile aynı anlam).
#include "dil_sec_b.h"
static const char *TURAD="abcd";
static void maske_yaz(char *s, int m, int ofs, const char *ad){ int k=0; for(int b=0;b<4;b++) if(m>>(b+ofs)&1) s[k++]=ad[b]; s[k]=0; }
int main(int argc,char**argv){ if(argc<7){ fprintf(stderr,"kullanım\n"); return 1; }
  const char *kip=argv[1]; kutuphane_init(); mercek_bedeli_hazirla(); lmask_hazirla();
  static atablo_t T; atablo_kur(&T,argv+6,argc-6); genum_t *G=genum_ayir();
  FILE *f=fopen(argv[2],"rb"); if(!f){ perror(argv[2]); return 2; } fseek(f,0,SEEK_END); long sz=ftell(f); fseek(f,0,SEEK_SET); int NH=(int)(sz/232);
  FILE *o=fopen(argv[4],"w"), *ay=fopen(argv[5],"w"); static prof_t P; int maxh=getenv("HEDEF_SON")?atoi(getenv("HEDEF_SON")):NH-1;
  int kol=-1, tmin=0, dmin=0; if(!strcmp(kip,"A")){ kol=1; tmin=3; dmin=3; } else if(!strcmp(kip,"Am")){ kol=2; tmin=2; dmin=2; } else if(!strcmp(kip,"Ap")){ kol=3; tmin=4; dmin=4; }
  else if(!strcmp(kip,"B")) kol=4; else if(!strcmp(kip,"P")) kol=0; else { fprintf(stderr,"kip?\n"); return 1; }
  dilb_tampon_t *BT=(kol>=1&&kol<=3)?dilb_ayir():NULL;
  for(int h=0;h<NH;h++){ int32_t id; int16_t a16[114]; if(fread(&id,4,1,f)!=1||fread(a16,2,114,f)!=114||id!=h){ fprintf(stderr,"hedef\n"); return 2; } if(h>maxh) break;
    int a[114]; for(int i=0;i<114;i++) a[i]=a16[i]; kdegerlendir(a,&P,0x3FFF);
    if(kol==0){                                                                       // plasebo12 dil_cikar.c ile aynı
      const int onsel=atoi(argv[3]); const char *on="x"; dil_t d; dil_bul(&T,&P,onsel,G,&d);
      char ad[32]; if(h==0) snprintf(ad,32,onsel?"QP2":"Q"); else snprintf(ad,32,"%s%03d",on,h);
      fprintf(o,"%s %d %d %d %d",ad,h,onsel,d.L,d.n); for(int i=0;i<d.n;i++) fprintf(o," %d",d.g[i]); fprintf(o,"\n");
      const int f_=d.L/NPR, p=PR[d.L%NPR];
      fprintf(ay,"%s hedef %d mercek (%d,%+d,%d) amac %.6f bilgi %.6f dil_bedeli %.6f mercek_bedeli %.6f |Λ| %d grup_bilgi B %.4f Z %.4f F %.4f W %.4f H %.4f :",
        ad,h,AL[f_],BE[f_],p,d.amac,d.bilgi,d.bedel,LBEDEL[onsel][d.L],d.n,d.ig[0],d.ig[1],d.ig[2],d.ig[3],d.ig[4]);
      for(int i=0;i<d.n;i++) fprintf(ay," %s",PADI[d.g[i]]); fprintf(ay,"\n"); continue; }
    char ad[32]; snprintf(ad,32,"%s%03d",kip,h);
    if(kol<=3){ dilb_t d; dil_bul_b(&T,&P,tmin,dmin,G,BT,&d);
      fprintf(o,"%s %d %d %d %d",ad,h,10*kol,d.L,d.n); for(int i=0;i<d.n;i++) fprintf(o," %d",d.g[i]); fprintf(o,"\n");
      if(d.L<0){ fprintf(ay,"%s hedef %d kol %d dil_yok Gmercek %d Bmercek %d aday_asimi %d\n",ad,h,kol,d.nG,d.nB,d.asim); continue; }
      const int f_=d.L/NPR, p=PR[d.L%NPR]; char ts[8], ds[8]; maske_yaz(ts,d.sig,4,TURAD); maske_yaz(ds,d.sig,0,"KAHB");
      fprintf(ay,"%s hedef %d kol %d mercek (%d,%+d,%d) amac %.6f bilgi %.6f dil_bedeli %.6f mercek_bedeli %.6f esik %.6f |Λ| %d grup_bilgi B %.4f Z %.4f F %.4f W %.4f H %.4f tur %s duzey %s Gmercek %d Bmercek %d aday_asimi %d :",
        ad,h,kol,AL[f_],BE[f_],p,d.amac,d.bilgi,d.bedel,LBEDEL[0][d.L],d.bilgi-LBEDEL[0][d.L],d.n,d.ig[0],d.ig[1],d.ig[2],d.ig[3],d.ig[4],ts,ds,d.nG,d.nB,d.asim);
      for(int i=0;i<d.n;i++) fprintf(ay," %s",PADI[d.g[i]]); fprintf(ay,"\n"); continue; }
    dilg_t d; dil_genis(&T,&P,&d);
    fprintf(o,"%s %d %d %d %d",ad,h,40,d.L,d.n); for(int i=0;i<d.n;i++) fprintf(o," %d",d.g[i]); fprintf(o,"\n");
    const int f_=d.L/NPR, p=PR[d.L%NPR];
    fprintf(ay,"%s hedef %d kol 4 mercek (%d,%+d,%d) puan %.6f bilgi %.6f mercek_bedeli %.6f |Λ| %d grup_bilgi B %.4f Z %.4f F %.4f W %.4f H %.4f grup_sayim B %llu/%llu Z %llu/%llu F %llu/%llu W %llu/%llu H %llu/%llu :",
      ad,h,AL[f_],BE[f_],p,d.puan,d.bilgi,LBEDEL[0][d.L],d.n,d.ig[0],d.ig[1],d.ig[2],d.ig[3],d.ig[4],
      (unsigned long long)d.c[0],(unsigned long long)d.nn[0],(unsigned long long)d.c[1],(unsigned long long)d.nn[1],(unsigned long long)d.c[2],(unsigned long long)d.nn[2],
      (unsigned long long)d.c[3],(unsigned long long)d.nn[3],(unsigned long long)d.c[4],(unsigned long long)d.nn[4]);
    for(int i=0;i<d.n;i++) fprintf(ay," %s",PADI[d.g[i]]); fprintf(ay,"\n"); }
  fclose(o); fclose(ay); fclose(f); return 0; }
