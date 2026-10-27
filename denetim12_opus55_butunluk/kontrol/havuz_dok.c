// havuz_dok — O-havuzu kayıtlarından ilk N kitabın c_dok biçiminde dökümü (C↔Python eşitliği için). ./havuz_dok <kodeks> <N> <cikti> <havuz.bin>
#include "../src/butunluk.h"
int main(int argc,char**argv){ int kod=atoi(argv[1]); long N=atol(argv[2]); butunluk_init(kod); static profil_t P; FILE *fo=fopen(argv[3],"wb");
  FILE *f=fopen(argv[4],"rb"); unsigned char rec[244]; long b=0;
  while(b<N && fread(rec,244,1,f)==1){ int a[114]; int16_t a16[114]; for(int i=0;i<114;i++){ uint16_t v; memcpy(&v,rec+2*i,2); a[i]=v; a16[i]=(int16_t)v; }
    degerlendir(a,&P); fwrite(a16,2,114,fo); fwrite(P.m,4,NLENS,fo); fwrite(P.kF,1,NLENS,fo); fwrite(P.kH,1,NLENS,fo); fwrite(P.mc,1,NLENS,fo);
    uint8_t kr[NLENS]; for(int L=0;L<NLENS;L++) kr[L]=(uint8_t)sayim_mercek(&P,L,0); fwrite(kr,1,NLENS,fo); b++; }
  fclose(f); fclose(fo); return 0; }
