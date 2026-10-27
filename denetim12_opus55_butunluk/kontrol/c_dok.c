// c_dok — C puanlayıcısının profil dökümü (C ↔ Python eşitlik sınaması için).
// Kullanım: ./c_dok <kodeks> <sifir U|M|L|J|K> <tohum> <N> <cikti.bin>   (K = yalnız Kur'an'ın kendisi)
// Kayıt: int16 a[114] | uint32 m[406] | uint8 kF[406] | uint8 kH[406] | uint8 mc[406] | uint8 kredi_G[406]
#include "../src/butunluk.h"
int main(int argc,char**argv){
  if(argc<6){ fprintf(stderr,"kullanım\n"); return 1; }
  int kod=atoi(argv[1]); char s=argv[2][0]; uint64_t seed=strtoull(argv[3],0,10); long N=atol(argv[4]);
  butunluk_init(kod); FILE *fo=fopen(argv[5],"wb"); static profil_t P; orn_t *Z=malloc(sizeof(orn_t));
  if(s!='K') orn_init(Z,s,seed);
  for(long b=0;b<N;b++){ int a[114];
    if(s=='K') memcpy(a,BASE,sizeof a); else { orn_sonraki(Z); memcpy(a,Z->a,sizeof a); }
    degerlendir(a,&P); int16_t a16[114]; for(int i=0;i<114;i++) a16[i]=(int16_t)a[i];
    fwrite(a16,2,114,fo); fwrite(P.m,4,NLENS,fo); fwrite(P.kF,1,NLENS,fo); fwrite(P.kH,1,NLENS,fo); fwrite(P.mc,1,NLENS,fo); uint8_t kr[NLENS]; for(int L=0;L<NLENS;L++) kr[L]=(uint8_t)sayim_mercek(&P,L,0); fwrite(kr,1,NLENS,fo); }
  fclose(fo); return 0; }
