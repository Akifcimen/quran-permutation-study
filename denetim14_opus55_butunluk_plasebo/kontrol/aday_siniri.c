// aday_siniri — ONKAYIT §4.3'ün raporlama yükümlülüğü: D'nin grup başına 24 aday sınırının (tek yaklaşık adım) hangi
// hedef-merceklerde devreye girdiğini sayar. Her hedefin 406 mercekte her grupta sağladığı L* yüklemi sayılır (dışlananlar hariç);
// 24'ü aşan (hedef, mercek, grup) üçlüleri ve hedefin seçilmiş merceğinde aşım olup olmadığı basılır. Sonuçları değiştirmez.
// Kullanım: ./aday_siniri <hedef.bin> <diller.txt>
#include "../src/dil_sec.h"   // GADAY (D'nin grup başına aday sınırı)
#include "../src/dtab.h"
int main(int argc,char**argv){ kutuphane_init(); if(argc<3) return 1;
  FILE *f=fopen(argv[1],"rb"); fseek(f,0,SEEK_END); int NH=(int)(ftell(f)/232); fseek(f,0,SEEK_SET); int16_t (*H)[114]=malloc(sizeof(*H)*NH);
  for(int h=0;h<NH;h++){ int32_t id; if(fread(&id,4,1,f)!=1||fread(H[h],2,114,f)!=114) return 2; } fclose(f);
  int *Lsec=malloc(sizeof(int)*NH); for(int h=0;h<NH;h++) Lsec[h]=-1;
  FILE *fd=fopen(argv[2],"r"); char ad[64]; int hed, ons, L, n;
  while(fscanf(fd,"%63s %d %d %d %d",ad,&hed,&ons,&L,&n)==5){ int g; for(int i=0;i<n;i++) fscanf(fd,"%d",&g); if(ons==0 && hed>=0 && hed<NH) Lsec[hed]=L; }
  fclose(fd);
  static prof_t P; long asim=0, asim_hedef=0, asim_secili=0; int enb[NGRUP]={0}; const char *GA="BZFWH";
  for(int h=0;h<NH;h++){ int a[114]; for(int q=0;q<114;q++) a[q]=H[h][q]; kdegerlendir(a,&P,0x3FFF); int bu=0;
    for(int L2=0;L2<NLENS;L2++){ const uint64_t s[NGRUP]={P.B[L2],P.Z[L2],P.F[L2],P.W[L2],P.H[L2]};
      for(int k=0;k<NGRUP;k++){ int c=0; uint64_t x=s[k]; while(x){ int b=__builtin_ctzll(x); x&=x-1; if(LUYE[GOFS[k]+b]) c++; }
        if(c>enb[k]) enb[k]=c;
        if(c>GADAY){ asim++; bu=1; const int f_=L2/NPR; printf("aşım: hedef %d mercek (%d,%+d,%d) grup %c sağlanan %d%s\n",h,AL[f_],BE[f_],PR[L2%NPR],GA[k],c,L2==Lsec[h]?" (seçilmiş mercek)":"");
          if(L2==Lsec[h]) asim_secili++; } } }
    asim_hedef+=bu; }
  printf("hedef %d; 24'ü aşan (hedef, mercek, grup) %ld; aşımlı hedef %d'den %ld; seçilmiş mercekte aşım %ld\n",NH,asim,NH,asim_hedef,asim_secili);
  printf("grup başına en çok sağlanan:"); for(int k=0;k<NGRUP;k++) printf(" %c %d",GA[k],enb[k]); printf("\n");
  return 0; }
