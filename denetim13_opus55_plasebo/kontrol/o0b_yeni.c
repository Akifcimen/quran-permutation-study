// o0b_yeni.c — yeni referans tablosundan (bu oturum) Kur'an'ın aynı aramaları ve el dilindeki tam bilgi puanı (ONKAYIT §3.3 kuralı).
#include "../src/dtab.h"
static atablo_t T;
void yeni_kur(char **f, int n){ kutuphane_init(); mercek_bedeli_hazirla(); atablo_kur(&T,f,n); }
void yeni_kuran(uint64_t *N, uint64_t *cB, uint64_t *nF, uint64_t *cF, uint64_t *cW, int *kF, double *S, int *Lb){
  static prof_t P; kdegerlendir(BASE,&P,0x3FFF); const int L=0*NPR+6; const int cf=T.cF[L][kkutu(P.kF[L])];
  const uint64_t fm=P.F[L]&0x7FFull, wm=P.W[L]&0x3Full; const uint8_t bm=P.B[L]&3u;
  uint64_t c1=0,c2=0; for(int kb=0;kb<KC;kb++){ if(T.cF[L][kb]!=cf) continue; for(int g=0;g<2;g++){ const uint32_t lk=LKG(L,kb,g); const uint64_t s=g?wm:fm;
      for(uint64_t e=T.bas[lk];e<T.bas[lk+1];e++) if((T.E[e].mask&s)==s){ if(g) c2+=T.E[e].c; else c1+=T.E[e].c; } } }
  *N=T.N; *cB=T.BA[L][bm]; *nF=T.nnF[L][cf]; *cF=c1; *cW=c2; *kF=P.kF[L];
  static const int EL[20]={0,1,7,11,12,13,14,15,16,17,18,19,20,21,65,66,67,68,69,70}; static dtab_t D; dtab_kur(&D,&T,EL,20,0); *S=dtab_puan(&D,&T,&P,Lb); }
