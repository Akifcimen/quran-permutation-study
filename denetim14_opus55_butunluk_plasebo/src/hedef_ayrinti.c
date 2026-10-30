// hedef_ayrinti — her hedef için kendi dilinde (ve el dilinde) betimsel ayrıntı (ONKAYIT §3.3, §9; dondurmadan sonra):
// eşik S_x(Λ_x) (kontrol programıyla aynı tablo ve işlev), en iyi mercek, o mercekte dilin grup başına en-az sayısı c ve bağlam n
// (c < 10 → çözülmemiş bayrağı), sınıf durumu ve o mercekteki bütün L* profili. Sonuçları değiştirmez; yalnız raporlama içindir.
// Kullanım: ./hedef_ayrinti <hedef.bin> <diller.txt> <cikti.txt> <ref>...
#include "dtab.h"
static void grup_sayim(const atablo_t *T, const prof_t *P, const dtab_t *D, int L, uint64_t c[NGRUP], uint64_t n[NGRUP], int var[NGRUP]){
  const int cf=T->cF[L][kkutu(P->kF[L])], ch=T->cH[L][kkutu(P->kH[L])];
  const uint64_t s[NGRUP]={P->B[L]&D->gm[0],P->Z[L]&D->gm[1],P->F[L]&D->gm[2],P->W[L]&D->gm[3],P->H[L]&D->gm[4]};
  for(int k=0;k<NGRUP;k++){ var[k]=s[k]!=0; c[k]=0; n[k]=0; }
  if(s[0]){ c[0]=T->BA[L][s[0]]; n[0]=T->N; } if(s[1]){ c[1]=T->ZA[L][s[1]]; n[1]=T->N; }
  if(s[4]){ c[4]=T->HA[((size_t)L*NCX+ch)*128+s[4]]; n[4]=T->nnH[L][ch]; }
  for(int g=0;g<2;g++){ const int k=g?3:2; if(!s[k]) continue; uint64_t cc=0; for(int kb=0;kb<KC;kb++){ if(T->cF[L][kb]!=cf) continue; const uint32_t lk=LKG(L,kb,g);
      for(uint64_t e=T->bas[lk];e<T->bas[lk+1];e++) if((T->E[e].mask&s[k])==s[k]) cc+=T->E[e].c; } c[k]=cc; n[k]=T->nnF[L][cf]; } }
int main(int argc,char**argv){ kutuphane_init(); mercek_bedeli_hazirla(); static atablo_t T; atablo_kur(&T,argv+4,argc-4);
  FILE *f=fopen(argv[1],"rb"); fseek(f,0,SEEK_END); int NH=(int)(ftell(f)/232); fseek(f,0,SEEK_SET); int16_t (*H)[114]=malloc(sizeof(*H)*NH);
  for(int h=0;h<NH;h++){ int32_t id; if(fread(&id,4,1,f)!=1||fread(H[h],2,114,f)!=114) return 2; } fclose(f);
  FILE *fd=fopen(argv[2],"r"), *o=fopen(argv[3],"w"); char ad[64]; int hed, ons, L, n; static prof_t P; static dtab_t D; dtab_t EL; int elvar=0;
  // el dili satırını önce bul
  { long pos=ftell(fd); while(fscanf(fd,"%63s %d %d %d %d",ad,&hed,&ons,&L,&n)==5){ int g[DMAX]; for(int i=0;i<n;i++) fscanf(fd,"%d",&g[i]); if(!strcmp(ad,"EL")){ dtab_kur(&EL,&T,g,n,0); elvar=1; } } fseek(fd,pos,SEEK_SET); }
  while(fscanf(fd,"%63s %d %d %d %d",ad,&hed,&ons,&L,&n)==5){ int g[DMAX]; for(int i=0;i<n;i++) fscanf(fd,"%d",&g[i]); if(!strcmp(ad,"EL")) continue;
    int a[114]; for(int q=0;q<114;q++) a[q]=H[hed][q]; kdegerlendir(a,&P,0x3FFF); dtab_kur(&D,&T,g,n,ons); int Lb; const double S=dtab_puan(&D,&T,&P,&Lb); const int sn=dtab_sinif(&D,&P,Lb);
    uint64_t c[NGRUP], nn[NGRUP]; int var[NGRUP]; grup_sayim(&T,&P,&D,Lb,c,nn,var); int coz=1; for(int k=0;k<NGRUP;k++) if(var[k] && c[k]<T_COZ) coz=0;
    const int f_=Lb/NPR, p=PR[Lb%NPR];
    fprintf(o,"%s hedef %d esik %.6f mercek (%d,%+d,%d) Ldil %d sinif %d cozulmus %d kF %d kH %d mc %d |",ad,hed,S,AL[f_],BE[f_],p,L,sn,coz,P.kF[Lb],P.kH[Lb],P.mc[Lb]);
    const char *GA="BZFWH"; for(int k=0;k<NGRUP;k++) if(var[k]) fprintf(o," %c:%llu/%llu",GA[k],(unsigned long long)c[k],(unsigned long long)nn[k]); fprintf(o," | dil:");
    for(int i=0;i<n;i++) fprintf(o," %s",PADI[g[i]]); fprintf(o," | L*@mercek:");
    const uint64_t s[NGRUP]={P.B[Lb],P.Z[Lb],P.F[Lb],P.W[Lb],P.H[Lb]}; for(int k=0;k<NGRUP;k++){ uint64_t x=s[k]; while(x){ int b=__builtin_ctzll(x); x&=x-1; if(LUYE[GOFS[k]+b]) fprintf(o," %s",PADI[GOFS[k]+b]); } }
    if(elvar){ int Le; const double Se=dtab_puan(&EL,&T,&P,&Le); const int f2=Le/NPR; fprintf(o," | el: %.6f (%d,%+d,%d) sinif %d",Se,AL[f2],BE[f2],PR[Le%NPR],dtab_sinif(&EL,&P,Le)); }
    fprintf(o,"\n"); dtab_birak(&D); }
  fclose(o); fclose(fd); return 0; }
