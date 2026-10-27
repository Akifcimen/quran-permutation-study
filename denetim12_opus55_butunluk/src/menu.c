// menu — bölme menüsü koşusu (ONKAYIT §8.4; yalnız sayım puanı, kapı G). Kullanım: ./menu <kodeks> <sifir> <tohum> <N> <cikti_onek>
// Kur'an'ın değerleri hesaplanır; her kitap için: tum (20 çift), t2siz (t2 içermeyen 12 çift), bir (yalnız (t2,n2)).
#include "menu.h"
#include <time.h>
typedef struct __attribute__((packed)) { uint16_t a[114]; uint8_t tum, t2siz, bir; uint8_t tumC, t2sizC; int16_t tumL, t2sizL; uint64_t sira; } mkayit_t;
int main(int argc,char**argv){ int kod=atoi(argv[1]); char s=argv[2][0]; uint64_t seed=strtoull(argv[3],0,10); long N=atol(argv[4]); const char *on=argv[5];
  butunluk_init(kod); menu_hazirla(); menu_t Q; menu_degerlendir(BASE,&Q,-1,NULL);
  orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,s,seed); long h[3][NPRED+1]; memset(h,0,sizeof h); long k[3]={0,0,0}; long esitsiz=0;
  char fa[1024]; snprintf(fa,1024,"%s.aday.yaziliyor",on); FILE *fo=fopen(fa,"wb"); long nyaz=0; mkayit_t top[30]; int nt=0;
  struct timespec t0,t1; clock_gettime(CLOCK_MONOTONIC,&t0); static profil_t P; const int dene=getenv("MENU_DENETIM")!=NULL;
  for(long b=0;b<N;b++){ orn_sonraki(Z); menu_t M; int birL[NLENS]; menu_degerlendir(Z->a,&M,-1,dene?birL:NULL);
    if(dene){ degerlendir(Z->a,&P); for(int L=0;L<NLENS;L++) if(birL[L]!=sayim_mercek(&P,L,0)) esitsiz++; }
    h[0][M.tum]++; h[1][M.t2siz]++; h[2][M.bir]++; if(M.tum>=Q.tum) k[0]++; if(M.t2siz>=Q.t2siz) k[1]++; if(M.bir>=Q.bir) k[2]++;
    mkayit_t K; for(int i=0;i<114;i++) K.a[i]=(uint16_t)Z->a[i]; K.tum=M.tum; K.t2siz=M.t2siz; K.bir=M.bir; K.tumC=M.tumC; K.t2sizC=M.t2sizC; K.tumL=M.tumL; K.t2sizL=M.t2sizL; K.sira=b;
    if(M.tum>=Q.tum-1 || M.t2siz>=Q.t2siz-1){ fwrite(&K,sizeof K,1,fo); nyaz++; }
    if(nt<30) top[nt++]=K; else { int mi=0; for(int j=1;j<30;j++) if(top[j].tum<top[mi].tum) mi=j; if(K.tum>top[mi].tum) top[mi]=K; } }
  for(int j=0;j<nt;j++) fwrite(&top[j],sizeof(mkayit_t),1,fo); fclose(fo); clock_gettime(CLOCK_MONOTONIC,&t1); double sn=(t1.tv_sec-t0.tv_sec)+1e-9*(t1.tv_nsec-t0.tv_nsec);
  char fa2[1024]; snprintf(fa2,1024,"%s.aday",on); rename(fa,fa2);
  char fs[1024], ft[1024]; snprintf(fs,1024,"%s.say",on); snprintf(ft,1024,"%s.say.yaziliyor",on); FILE *f=fopen(ft,"w");
  fprintf(f,"kodeks %d sifir %c tohum %llu N %ld sure_s %.1f aday %ld\n",kod,s,(unsigned long long)seed,N,sn,nyaz);
  fprintf(f,"Kuran tum %d (cift %s,%s mercek (%d,%+d,%d)) t2siz %d (cift %s,%s mercek (%d,%+d,%d)) bir %d\n",Q.tum,DAD[CIFT[Q.tumC][0]],DAD[CIFT[Q.tumC][1]],AL[Q.tumL/NPR],BE[Q.tumL/NPR],PR[Q.tumL%NPR],
    Q.t2siz,DAD[CIFT[Q.t2sizC][0]],DAD[CIFT[Q.t2sizC][1]],AL[Q.t2sizL/NPR],BE[Q.t2sizL/NPR],PR[Q.t2sizL%NPR],Q.bir);
  const char *ad[3]={"tum","t2siz","bir"}; for(int j=0;j<3;j++){ fprintf(f,"%s k %ld hist",ad[j],k[j]); for(int c=0;c<=NPRED;c++) fprintf(f," %ld",h[j][c]); fprintf(f,"\n"); }
  if(dene) fprintf(f,"denetim: (t2,n2) kredileri ana puanlayıcıyla esitsiz mercek-kitap %ld\n",esitsiz);
  fclose(f); rename(ft,fs); return 0; }
