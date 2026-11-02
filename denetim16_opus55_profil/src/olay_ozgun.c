// olay_ozgun — kart_olay'ın bağımsız sınaması: aynı olayları, özgün kutuphane.h yoluyla (orn_init/orn_sonraki, kdegerlendir, kapiG; tek
// iş parçacığı) sayar. Çıktı kart_olay'ın bölüt dosyasıyla aynı biçimdedir ("D ad k", yalnız k > 0) ve ondan bağımsız üretilir.
// Kullanım: ./olay_ozgun <sifir> <olaylar.txt> <bolut>   (BN derleme parametresi)
#include "kutuphane.h"
#ifndef BN
#define BN 250000
#endif
int main(int argc,char**argv){ if(argc<4) return 1; const char S=argv[1][0]; const int b=atoi(argv[3]); kutuphane_init();
  FILE *f=fopen(argv[2],"r"); if(!f) return 2; int cap=1024, n=0; char (*ad)[24]=malloc(24*cap); int *L=malloc(sizeof(int)*cap); uint64_t (*m)[5]=malloc(sizeof(*m)*cap); char s[4096];
  while(fgets(s,sizeof s,f)){ unsigned long long x[5]; int h, l; char a[64]; if(sscanf(s,"%63s %d %d %llx %llx %llx %llx %llx",a,&h,&l,&x[0],&x[1],&x[2],&x[3],&x[4])!=8) return 3;
    if(n==cap){ cap*=2; ad=realloc(ad,24*cap); L=realloc(L,sizeof(int)*cap); m=realloc(m,sizeof(*m)*cap); } snprintf(ad[n],24,"%s",a); L[n]=l; for(int q=0;q<5;q++) m[n][q]=x[q]; n++; }
  fclose(f); long *k=calloc(n,sizeof(long)); orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,S,tohum(S,2,b)); static prof_t P;
  for(int i=0;i<BN;i++){ orn_sonraki(Z); kdegerlendir(Z->a,&P,0x3FFF);
    for(int d=0;d<n;d++){ const int l=L[d]; if(!kapiG(&P,l)) continue;
      if(((uint64_t)P.B[l]&m[d][0])==m[d][0] && ((uint64_t)P.Z[l]&m[d][1])==m[d][1] && (P.F[l]&m[d][2])==m[d][2] && (P.W[l]&m[d][3])==m[d][3] && ((uint64_t)P.H[l]&m[d][4])==m[d][4]) k[d]++; } }
  for(int d=0;d<n;d++) if(k[d]) printf("D %s %ld\n",ad[d],k[d]); return 0; }
