// dI_sina.c — duz.c'deki ΔI (yer değiştirmeyle ters sıra farkı) formülünün kaba kuvvetle sınaması.
#include "kutuphane.h"
int main(void){ kutuphane_init(); orn_t *Z=malloc(sizeof(orn_t)); orn_init(Z,'L',tohum('L',4,991)); rng_t r; rseed(&r,5); long fark=0;
  for(int it=0;it<200000;it++){ orn_sonraki(Z); int *a=Z->a; int i=bounded(&r,114), j=bounded(&r,113); if(j>=i) j++; if(i>j){int t=i;i=j;j=t;}
    const int x=a[i], y=a[j]; int dI=(y<x)-(x<y); for(int k=i+1;k<j;k++){ const int z=a[k]; dI+=(y<z)-(x<z)+(z<x)-(z<y); }
    long I0=ters_kesin(a); a[i]=y; a[j]=x; long I1=ters_kesin(a); if(I1-I0!=dI) fark++; }
  printf("ΔI sınaması: 200000 yer değiştirme, fark %ld; q = %.9f\n",fark,Z->q); return 0; }
