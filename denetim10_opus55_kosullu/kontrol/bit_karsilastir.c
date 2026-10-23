// robot_yan çıktısını okur: (1) kendi örnekleyici portumla aynı tohumdan aynı kitapları üretiyor muyum,
// (2) facts19() robotun facts(LI_Q) bitleriyle birebir aynı mı. Kullanım: ./bit_karsilastir <kodeks> <null> <seed> <N> <robot.bin>
#include "../src/ortak.h"
int main(int argc, char **argv){
  int kod=atoi(argv[1]); init_ortak(kod);
  char null=argv[2][0]; double par=strlen(argv[2])>1?atof(argv[2]+1):0; uint64_t seed=strtoull(argv[3],NULL,10), N=strtoull(argv[4],NULL,10);
  FILE *fi=fopen(argv[5],"rb"); sampler_t *Z=malloc(sizeof(sampler_t)); sampler_init(Z,null,par,seed);
  long kfark=0, bfark=0, kkfark=0; int16_t b[114]; uint64_t lo,hi; int k[3];
  for(uint64_t it=0; it<=N; it++){
    if(fread(b,2,114,fi)!=114) { printf("dosya kısa\n"); return 1; } fread(&lo,8,1,fi); fread(&hi,8,1,fi); fread(k,4,3,fi);
    int a[114]; if(it==0) memcpy(a,BASE,sizeof a); else { sampler_next(Z); memcpy(a,Z->a,sizeof a); }
    int same=1; for(int i=0;i<114;i++) if(a[i]!=b[i]) same=0; if(!same) kfark++;
    int bb[114]; for(int i=0;i<114;i++) bb[i]=b[i];
    fres_t R; facts19(bb,&R,1); u128 rb=((u128)hi<<64)|lo;
    if(R.bits!=rb){ bfark++; if(bfark<5){ u128 d=R.bits^rb; printf("bit farkı kitap %llu:",(unsigned long long)it); while(d){ int f=ctz128(d); d&=d-1; printf(" [%s]",FNAME[f]); } printf("\n"); } }
    if(R.kF!=k[0]||R.kG!=k[1]||R.same!=k[2]) kkfark++;
  }
  printf("kodeks %d null %s: %llu kitap | örnekleyici farkı %ld | bit farkı %ld | kF/kG/same farkı %ld | olgu sayısı %d\n",kod,argv[2],(unsigned long long)N+1,kfark,bfark,kkfark,NFACT);
  return (kfark||bfark||kkfark)?1:0; }
