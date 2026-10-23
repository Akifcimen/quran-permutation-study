// Sınama için koşulsuz U kitapları (kayıt biçiminde).
#include "../src/ortak.h"
typedef struct __attribute__((packed)) { uint16_t a[114]; uint32_t f; uint32_t zincir; uint64_t sira; } kayit_t;
int main(int argc,char **argv){ init_ortak(atoi(argv[1])); long N=atol(argv[2]); sampler_t *Z=malloc(sizeof(sampler_t)); sampler_init(Z,'U',0,atoi(argv[3])); FILE *f=fopen(argv[4],"wb");
  kayit_t K; for(long i=0;i<N;i++){ sampler_next(Z); for(int j=0;j<114;j++) K.a[j]=Z->a[j]; K.f=olaylar(Z->a); K.zincir=0; K.sira=i; fwrite(&K,sizeof K,1,f); } fclose(f); return 0; }
