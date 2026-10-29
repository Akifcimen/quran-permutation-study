// o0_karsilastir — Ö0 çapası (ONKAYIT §0, §3.1): (i) örnekleyiciler U, M20, Mallows aynı tohumda 12. denetiminkilerle bit-bit aynı mı;
// (ii) kütüphane 20 özgün yükleme sınırlandırılınca maskeler (ve k_F, k_H, en küçük hücre) butunluk.h ile bit-bit aynı mı.
// Rastgele kitaplar kod-doğrulama tohumlarından (tür 4) gelir; hiçbir hedef, kontrol ya da Kur'an kullanılmaz. Yalnız uyuşmazlık sayıları basılır.
// Kullanım: ./o0_karsilastir <kitap_sayisi_sifir_basina>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define NLENS 406
void b12_init(void); void b12_eval(const int*,uint32_t*,uint8_t*,uint8_t*,uint8_t*); void b12_orn_init(char,uint64_t); void b12_orn_sonraki(int*);
void kut_init(void); void kut_eval20(const int*,uint32_t*,uint8_t*,uint8_t*,uint8_t*); void kut_orn_init(char,uint64_t); void kut_orn_sonraki(int*);
uint64_t kut_tohum(char,int,long);
int main(int argc,char**argv){ long N=atol(argv[1]); b12_init(); kut_init();
  const char S[3]={'U','M','L'}; long top_orn=0, top_mask=0, top_k=0, top_kit=0;
  for(int si=0;si<3;si++){ char s=S[si]; uint64_t sd=kut_tohum(s,4,1);
    b12_orn_init(s,sd); kut_orn_init(s,sd); long horn=0, hmask=0, hk=0;
    static uint32_t m1[NLENS], m2[NLENS]; static uint8_t a1[NLENS],b1[NLENS],c1[NLENS],a2[NLENS],b2[NLENS],c2[NLENS];
    for(long b=0;b<N;b++){ int x[114], y[114]; b12_orn_sonraki(x); kut_orn_sonraki(y);
      if(memcmp(x,y,sizeof x)) horn++;
      b12_eval(x,m1,a1,b1,c1); kut_eval20(x,m2,a2,b2,c2);
      for(int L=0;L<NLENS;L++){ if(m1[L]!=m2[L]) hmask++; if(a1[L]!=a2[L]||b1[L]!=b2[L]||c1[L]!=c2[L]) hk++; } }
    printf("sifir %c tohum %llu kitap %ld: örnekleyici uyuşmazlığı %ld kitap; maske uyuşmazlığı %ld (mercek-kitap); kF/kH/hücre uyuşmazlığı %ld\n",
      s,(unsigned long long)sd,N,horn,hmask,hk);
    top_orn+=horn; top_mask+=hmask; top_k+=hk; top_kit+=N; }
  printf("TOPLAM %ld kitap × 406 mercek: örnekleyici %ld, maske %ld, kF/kH/hücre %ld\n",top_kit,top_orn,top_mask,top_k);
  return (top_orn||top_mask||top_k)?1:0; }
