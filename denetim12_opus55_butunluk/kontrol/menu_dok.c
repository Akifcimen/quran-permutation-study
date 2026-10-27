// menu_dok — menü kredilerinin dökümü (C ↔ Python): kayıt = int16 a[114] | uint8 kredi[20][406]. ./menu_dok <kodeks> <sifir> <tohum> <N> <cikti>
#include "../src/menu.h"
int main(int argc,char**argv){ int kod=atoi(argv[1]); char s=argv[2][0]; uint64_t seed=strtoull(argv[3],0,10); long N=atol(argv[4]);
  butunluk_init(kod); menu_hazirla(); orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,s,seed); FILE *fo=fopen(argv[5],"wb"); static uint8_t D[20*NLENS]; MENU_DOK=D;
  for(long b=0;b<N;b++){ int a[114]; if(s=='K') memcpy(a,BASE,sizeof a); else { orn_sonraki(Z); memcpy(a,Z->a,sizeof a); } menu_t M; menu_degerlendir(a,&M,-1,NULL);
    int16_t a16[114]; for(int i=0;i<114;i++) a16[i]=(int16_t)a[i]; fwrite(a16,2,114,fo); fwrite(D,1,20*NLENS,fo); }
  fclose(fo); return 0; }
