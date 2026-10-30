// akis_ozet — örnekleyici akışlarının platformlar arası eşitliği: bir bölütün ilk N kitabının FNV-1a özeti ve B2 biçim sayısı.
// Kullanım: ./akis_ozet <sifir> <bolut> <N>
#include "kutuphane.h"
int main(int argc, char **argv){
  if(argc<4){ fprintf(stderr,"kullanım\n"); return 1; }
  const char s=argv[1][0]; const long b=atol(argv[2]); const int N=atoi(argv[3]);
  kutuphane_init(); orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,s,tohum(s,2,b));
  uint64_t h=1469598103934665603ULL; long nb2=0;
  for(int i=0;i<N;i++){ orn_sonraki(Z); for(int q=0;q<114;q++){ h^=(uint64_t)(uint16_t)Z->a[q]; h*=1099511628211ULL; }
    for(int f=0;f<NFORM;f++){ const int al=AL[f], be=BE[f], abe=abs(be); int e0=0; long sn0=0, sa0=0, st0=0, st=0;
      for(int n=1;n<=114;n++){ const int x=Z->a[n-1]; const int t=al*n+be*x; st+=t; if(!(t&1)){ e0++; sn0+=n; sa0+=x; st0+=t; } }
      const int e1=114-e0; const long sn1=SN_TOT-sn0, sa1=SA_TOT-sa0, st1=st-st0;
      if(e0>0 && e1>0 && ((long)al*sn0==(long)abe*sa1 || (long)al*sn1==(long)abe*sa0 || st0==st1)) nb2++; } }
  printf("%c bolut %ld N %d ozet %016llx b2_bicim %ld q %.17g\n",s,b,N,(unsigned long long)h,nb2,s=='L'?Z->q:0.0);
  free(Z); return 0; }
