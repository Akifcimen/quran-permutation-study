// o0_kuran — Ö0'ın Kur'an kısmı (ONKAYIT §7.1; dondurmadan sonra): kütüphane 20 özgün yükleme sınırlandırılınca Kur'an'ın 406
// mercekteki maskeleri butunluk.h ile bit-bit aynı mı; el dili profili (1,+1,19)'da 15 yüklem mi.
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#define NLENS 406
void b12_init(void); void b12_eval(const int*,uint32_t*,uint8_t*,uint8_t*,uint8_t*);
void kut_init(void); void kut_eval20(const int*,uint32_t*,uint8_t*,uint8_t*,uint8_t*);
static const int AY[114]={7,286,200,176,120,165,206,75,129,109,123,111,43,52,99,128,111,110,98,135,112,78,118,64,77,227,93,88,69,60,34,30,73,54,45,83,182,88,75,85,
  54,53,89,59,37,35,38,29,18,45,60,49,62,55,78,96,29,22,24,13,14,11,11,18,12,12,30,52,52,44,28,28,20,56,40,31,50,40,46,42,29,19,36,25,22,17,19,26,30,20,15,21,11,8,8,19,5,8,8,11,
  11,8,3,9,5,4,7,3,6,3,5,4,5,6};
static const char *AD[20]={"B1","B2","F1","F2","R0c","R0e","R1c","R1e","K1","K2","Qc","QS","Qh","QkF","W1","QU","QkH","Qa","Qn","QH"};
int main(void){ b12_init(); kut_init(); static uint32_t m1[NLENS], m2[NLENS]; static uint8_t a1[NLENS],b1[NLENS],c1[NLENS],a2[NLENS],b2[NLENS],c2[NLENS];
  b12_eval(AY,m1,a1,b1,c1); kut_eval20(AY,m2,a2,b2,c2); long hm=0, hk=0; for(int L=0;L<NLENS;L++){ if(m1[L]!=m2[L]) hm++; if(a1[L]!=a2[L]||b1[L]!=b2[L]||c1[L]!=c2[L]) hk++; }
  const int L0=0*29+6; int n=0; printf("Kur'an (129): maske uyuşmazlığı %ld / 406 mercek; kF/kH/hücre uyuşmazlığı %ld\n(1,+1,19) el dili profili:",hm,hk);
  for(int b=0;b<20;b++) if(m2[L0]>>b&1){ printf(" %s",AD[b]); n++; } printf("  → %d yüklem; kF=%d kH=%d en küçük hücre=%d\n",n,a2[L0],b2[L0],c2[L0]);
  return (hm||hk||n!=15)?1:0; }
