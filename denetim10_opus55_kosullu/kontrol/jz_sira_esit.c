// kova sıralamasının ekleme sıralamasıyla aynı sırayı verdiğinin sınaması (aynı anahtarlar)
#include "../src/hizli.h"
int main(){ rng_t r; rseed(&r,5); long fark=0; for(long t=0;t<2000000;t++){ double key[114]; int i1[114], i2[114]; for(int i=0;i<114;i++){ key[i]=(i+1)+10.0*gauss(&r)*(t%1000==0?8:1); i1[i]=i; }
    for(int i=1;i<114;i++){ int x=i1[i]; double kx=key[x]; int j=i-1; while(j>=0&&key[i1[j]]>kx){ i1[j+1]=i1[j]; j--; } i1[j+1]=x; }
    jz_sirala(key,i2); if(memcmp(i1,i2,sizeof i1)) fark++; } printf("2e6 anahtar kümesinde (binde birinde σ×8 uç değerli) fark: %ld\n",fark); return fark?1:0; }
