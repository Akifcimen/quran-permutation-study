// orn_ref — denetim10 örnekleyicileriyle (kontrol/ref_d10 = depodan kopya) kitap dökümü. ./orn_ref <kodeks> <N> <cikti>
#include "ref_d10/hizli.h"
int main(int argc,char**argv){ int kod=atoi(argv[1]); long N=atol(argv[2]); FILE *fo=fopen(argv[3],"wb");
  init_ortak(kod); comp_hazirla(); int a[114];
  { sampler_t *A=malloc(sizeof(sampler_t)); sampler_init(A,'U',0,4242); for(long i=0;i<N;i++){ sampler_next(A); fwrite(A->a,4,114,fo);} }
  { sampler_t *A=malloc(sizeof(sampler_t)); sampler_init(A,'M',20,4343); for(long i=0;i<N;i++){ sampler_next(A); fwrite(A->a,4,114,fo);} }
  { ref_hazirla(); double q=mallows_q_ayarla(ters_kesin(BASE)); lz_t *A=malloc(sizeof(lz_t)); lz_init(A,q,4444); for(long i=0;i<N;i++){ lz_kitap(A,a); fwrite(a,4,114,fo);} fprintf(stderr,"ref q=%.12f\n",q); }
  { jz_t *A=malloc(sizeof(jz_t)); jz_init(A,10.0,4545); for(long i=0;i<N;i++){ jz_kitap(A,a); fwrite(a,4,114,fo);} }
  fclose(fo); return 0; }
