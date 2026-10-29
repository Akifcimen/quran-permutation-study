// sabit_tarama — sabit ya da hep-doğru yüklem–mercek çiftlerinin taraması (ONKAYIT §2.5). Tür 5 tohumlarıyla rastgele kitaplar;
// dışlama UYGULANMADAN ham maskeler. Yalnız bayraklar basılır: (1) analitik listede olmayan hep-doğru çiftler, (2) analitik
// listede olup taramada iki değeri de alan (yani sabit olmayan) çiftler, (3) hiçbir mercekte hiç doğru olmayan yüklemler (adlar).
// Ham özet ikili dosyaya yazılır (birleştirme için). Kullanım: ./sabit_tarama <sifir> <sira> <N> <cikti.bin>
#include "../src/kutuphane.h"
int main(int argc,char**argv){ char s=argv[1][0]; long sira=atol(argv[2]); long N=atol(argv[3]); kutuphane_init();
  static uint8_t xB[NLENS],xZ[NLENS],xH[NLENS]; static uint64_t xF[NLENS],xW[NLENS];
  memcpy(xB,EXB,sizeof xB); memcpy(xZ,EXZ,sizeof xZ); memcpy(xH,EXH,sizeof xH); memcpy(xF,EXF,sizeof xF); memcpy(xW,EXW,sizeof xW);
  memset(EXB,0,sizeof EXB); memset(EXZ,0,sizeof EXZ); memset(EXH,0,sizeof EXH); memset(EXF,0,sizeof EXF); memset(EXW,0,sizeof EXW);
  static uint8_t orB[NLENS],orZ[NLENS],orH[NLENS],anB[NLENS],anZ[NLENS],anH[NLENS]; static uint64_t orF[NLENS],orW[NLENS],anF[NLENS],anW[NLENS];
  memset(anB,0xFF,sizeof anB); memset(anZ,0xFF,sizeof anZ); memset(anH,0xFF,sizeof anH); memset(anF,0xFF,sizeof anF); memset(anW,0xFF,sizeof anW);
  orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,s,tohum(s,5,sira)); static prof_t P;
  for(long b=0;b<N;b++){ orn_sonraki(Z); kdegerlendir(Z->a,&P,0x3FFF);
    for(int L=0;L<NLENS;L++){ orB[L]|=P.B[L]; anB[L]&=P.B[L]; orZ[L]|=P.Z[L]; anZ[L]&=P.Z[L]; orH[L]|=P.H[L]; anH[L]&=P.H[L];
      orF[L]|=P.F[L]; anF[L]&=P.F[L]; orW[L]|=P.W[L]; anW[L]&=P.W[L]; } }
  FILE *f=fopen(argv[4],"wb"); fwrite(orB,1,NLENS,f); fwrite(anB,1,NLENS,f); fwrite(orZ,1,NLENS,f); fwrite(anZ,1,NLENS,f); fwrite(orH,1,NLENS,f); fwrite(anH,1,NLENS,f);
  fwrite(orF,8,NLENS,f); fwrite(anF,8,NLENS,f); fwrite(orW,8,NLENS,f); fwrite(anW,8,NLENS,f);
  fwrite(xB,1,NLENS,f); fwrite(xZ,1,NLENS,f); fwrite(xH,1,NLENS,f); fwrite(xF,8,NLENS,f); fwrite(xW,8,NLENS,f); fclose(f); return 0; }
