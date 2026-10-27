// cikarim_tara — gerektirme ADAYLARI ve sabit yüklemler için birlikte-görülme taraması (puan hesaplanmaz).
// Çıktı: (1) 20 bitlik maske histogramı, bütün mercekler havuzlanmış (H_tum) ve yalnız kapı G'nin tuttuğu mercekler (H_kapi);
//        (2) mercek başına "hiç doğru olmadı" / "hep doğru" maskeleri. Kullanım: ./cikarim_tara <kodeks> <sifir> <tohum> <N> <cikti>
#include "../src/butunluk.h"
int main(int argc,char**argv){ int kod=atoi(argv[1]); char s=argv[2][0]; uint64_t seed=strtoull(argv[3],0,10); long N=atol(argv[4]);
  butunluk_init(kod); orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,s,seed); static profil_t P;
  uint64_t *Ht=calloc(1u<<NPRED,8), *Hg=calloc(1u<<NPRED,8); uint32_t ever[NLENS], always[NLENS];
  for(int L=0;L<NLENS;L++){ ever[L]=0; always[L]=0xFFFFFFFFu; }
  for(long b=0;b<N;b++){ orn_sonraki(Z); degerlendir(Z->a,&P);
    for(int L=0;L<NLENS;L++){ uint32_t m=P.m[L]; Ht[m]++; if(kapi_tut(&P,L,0)) Hg[m]++; ever[L]|=m; always[L]&=m; } }
  FILE *fo=fopen(argv[5],"wb"); fwrite(Ht,8,1u<<NPRED,fo); fwrite(Hg,8,1u<<NPRED,fo); fwrite(ever,4,NLENS,fo); fwrite(always,4,NLENS,fo); fclose(fo); return 0; }
