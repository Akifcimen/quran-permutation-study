// o0_b12.c — 12. denetimin butunluk.h'si (değiştirilmemiş kopya src/b12/) ayrı derleme biriminde: maske ve örnekleyici.
#include "../src/b12/butunluk.h"
void b12_init(void){ butunluk_init(129); }
void b12_eval(const int *a, uint32_t *m, uint8_t *kF, uint8_t *kH, uint8_t *mc){ static profil_t P; degerlendir(a,&P);
  for(int L=0;L<NLENS;L++){ m[L]=P.m[L]; kF[L]=P.kF[L]; kH[L]=P.kH[L]; mc[L]=P.mc[L]; } }
static orn_t ZB;
void b12_orn_init(char s, uint64_t seed){ orn_init(&ZB,s,seed); }
void b12_orn_sonraki(int *a){ orn_sonraki(&ZB); memcpy(a,ZB.a,114*sizeof(int)); }
