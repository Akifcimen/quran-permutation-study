// o0b_b12.c — 12. denetimin arama tablosunu (tab/{U,M}129.tab, salt okunur) kendi kodlarıyla (src/b12/tablo.h kopyası) okur ve
// Kur'an'ın (1,+1,19) merceğindeki el dili aramalarının sayılarını verir.
#include "../src/b12/tablo.h"
static tablo_t TB;
int b12t_oku(const char *fn){ butunluk_init(129); return tablo_oku(&TB,fn); }
// çıktı: N, B en-az sayısı (B1∧B2, biçim (1,+1)), F bağlam n ve en-az sayısı, W en-az sayısı, k_F
void b12t_kuran(uint64_t *N, uint64_t *cB, uint64_t *nF, uint64_t *cF, uint64_t *cW, int *kF){
  static profil_t P; degerlendir(BASE,&P); const int L=0*NPR+6; const uint32_t m=P.m[L]; const int k=kkutu(P.kF[L]); const int c=TB.ctx[L*KC+k];
  *N=TB.N; *cB=TB.BA[0][bmask2(m)]; *nF=TB.cn[c]; *cF=TB.FA[(size_t)c*NFM+fmask11(m)]; *cW=TB.WA[(size_t)c*NWM+wmask6(m)]; *kF=P.kF[L]; }
