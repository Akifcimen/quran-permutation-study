// orn_biz — bu oturumun örnekleyicileriyle (src/butunluk.h) aynı tohumlarla kitap dökümü. ./orn_biz <kodeks> <N> <cikti>
#include "../src/butunluk.h"
int main(int argc,char**argv){ int kod=atoi(argv[1]); long N=atol(argv[2]); FILE *fo=fopen(argv[3],"wb"); butunluk_init(kod);
  const char *S="UMLJ"; uint64_t sd[4]={4242,4343,4444,4545};
  for(int k=0;k<4;k++){ orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,S[k],sd[k]); if(S[k]=='L') fprintf(stderr,"biz q=%.12f\n",Z->q);
    for(long i=0;i<N;i++){ orn_sonraki(Z); fwrite(Z->a,4,114,fo);} free(Z); }
  fclose(fo); return 0; }
