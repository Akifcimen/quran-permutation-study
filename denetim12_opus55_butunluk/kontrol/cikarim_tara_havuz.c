// cikarim_tara_havuz — O-havuzu kitaplarında (yapılı aileler) birlikte-görülme taraması; çıktı biçimi cikarim_tara ile aynı.
// Kullanım: ./cikarim_tara_havuz <kodeks> <cikti> <havuz.bin> ...   (kayıt: uint16 a[114] | u32 | u32 | u64 = 244 bayt)
#include "../src/butunluk.h"
int main(int argc,char**argv){ int kod=atoi(argv[1]); butunluk_init(kod); static profil_t P;
  uint64_t *Ht=calloc(1u<<NPRED,8), *Hg=calloc(1u<<NPRED,8); uint32_t ever[NLENS], always[NLENS];
  for(int L=0;L<NLENS;L++){ ever[L]=0; always[L]=0xFFFFFFFFu; } long nb=0;
  for(int fi=3;fi<argc;fi++){ FILE *f=fopen(argv[fi],"rb"); unsigned char rec[244];
    while(fread(rec,244,1,f)==1){ int a[114]; for(int i=0;i<114;i++){ uint16_t v; memcpy(&v,rec+2*i,2); a[i]=v; } nb++;
      degerlendir(a,&P); for(int L=0;L<NLENS;L++){ uint32_t m=P.m[L]; Ht[m]++; if(kapi_tut(&P,L,0)) Hg[m]++; ever[L]|=m; always[L]&=m; } }
    fclose(f); }
  FILE *fo=fopen(argv[2],"wb"); fwrite(Ht,8,1u<<NPRED,fo); fwrite(Hg,8,1u<<NPRED,fo); fwrite(ever,4,NLENS,fo); fwrite(always,4,NLENS,fo); fclose(fo);
  fprintf(stderr,"%ld kitap\n",nb); return 0; }
