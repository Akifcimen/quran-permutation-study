// o0_kut.c — kütüphane (src/kutuphane.h) ayrı derleme biriminde: 20 özgün yükleme izdüşüm ve örnekleyici.
#include "../src/kutuphane.h"
void kut_init(void){ kutuphane_init(); }
// 12. denetimin bit sırasına izdüşüm: B1 0, B2 1, F1..Qh 2..12, QkF 13, W1..QH 14..19
void kut_eval20(const int *a, uint32_t *m, uint8_t *kF, uint8_t *kH, uint8_t *mc){ static prof_t P; kdegerlendir(a,&P,0x3FFF);
  for(int L=0;L<NLENS;L++){ uint32_t x=0; x|=(uint32_t)(P.B[L]&3u); x|=(uint32_t)(P.F[L]&0x7FFull)<<2; x|=(uint32_t)(P.Z[L]&1u)<<13;
    x|=(uint32_t)(P.W[L]&0x3Full)<<14; m[L]=x; kF[L]=P.kF[L]; kH[L]=P.kH[L]; mc[L]=P.mc[L]; } }
static orn_t ZK;
void kut_orn_init(char s, uint64_t seed){ orn_init(&ZK,s,seed); }
void kut_orn_sonraki(int *a){ orn_sonraki(&ZK); memcpy(a,ZK.a,114*sizeof(int)); }
uint64_t kut_tohum(char s, int tur, long sira){ return tohum(s,tur,sira); }
