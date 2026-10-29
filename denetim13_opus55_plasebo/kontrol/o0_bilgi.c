// o0_bilgi — Ö0 bilgi puanı çapası (ONKAYIT §7.2). Kullanım: ./o0_bilgi <12.den_tablo.tab> <S_12> [<P_adanmis_F> <isabet> <oneri>] -- <ref>...
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
int b12t_oku(const char*); void b12t_kuran(uint64_t*,uint64_t*,uint64_t*,uint64_t*,uint64_t*,int*);
void yeni_kur(char**,int); void yeni_kuran(uint64_t*,uint64_t*,uint64_t*,uint64_t*,uint64_t*,int*,double*,int*);
// iki yanlı koşullu binom sınaması: x ~ Bin(x+y, n1/(n1+n2))
static double lbinom(int n,int k){ return lgamma(n+1.0)-lgamma(k+1.0)-lgamma(n-k+1.0); }
static double binom_iki(long x, long y, double n1, double n2){ long n=x+y; double p=n1/(n1+n2); if(n==0) return 1.0;
  double px=exp(lbinom(n,x)+x*log(p)+(n-x)*log1p(-p)), s=0; for(long k=0;k<=n;k++){ double pk=exp(lbinom(n,k)+k*log(p)+(n-k)*log1p(-p)); if(pk<=px*(1+1e-9)) s+=pk; } return s>1?1:s; }
static double poisson_iki(long x, double lam){ double px=exp(-lam+x*log(lam)-lgamma(x+1.0)), s=0; for(long k=0;k<2000;k++){ double pk=exp(-lam+k*log(lam)-lgamma(k+1.0)); if(pk<=px*(1+1e-9)) s+=pk; } return s>1?1:s; }
static double bit(uint64_t c, uint64_t n){ if(n==0) return 0; if(c>=10) return log2((double)n)-log2((double)c); return log2((double)n+1)-log2((double)c+0.5); }
int main(int argc,char**argv){ int sep=0; for(int i=1;i<argc;i++) if(!strcmp(argv[i],"--")) sep=i; if(!sep){ fprintf(stderr,"kullanım\n"); return 1; }
  const char *tb=argv[1]; double S12=atof(argv[2]); double Pad=-1; long had=0, nad=0; if(sep>4){ Pad=atof(argv[3]); had=atol(argv[4]); nad=atol(argv[5]); }
  if(!b12t_oku(tb)){ fprintf(stderr,"12. den. tablosu okunamadı\n"); return 2; }
  uint64_t N1,b1,n1,f1,w1, N2,b2,n2,f2,w2; int k1,k2; double S; int Lb; b12t_kuran(&N1,&b1,&n1,&f1,&w1,&k1);
  yeni_kur(argv+sep+1,argc-sep-1); yeni_kuran(&N2,&b2,&n2,&f2,&w2,&k2,&S,&Lb);
  printf("Kur'an (1,+1,19), k_F 12. den. %d / yeni %d\n",k1,k2);
  const char *ad[3]={"B (B1∧B2)","F (11 bit)","W (6 bit)"}; uint64_t c12[3]={b1,f1,w1}, cy[3]={b2,f2,w2}, n12[3]={N1,n1,n1}, ny[3]={N2,n2,n2}; int gecti=1; double d=0, v=0;
  for(int g=0;g<3;g++){ double p=binom_iki((long)cy[g],(long)c12[g],(double)ny[g],(double)n12[g]); if(p<0.001) gecti=0;
    double bo=bit(c12[g],n12[g]), by=bit(cy[g],ny[g]); d+=by-bo; double s1=c12[g]>0?1/(log(2)*sqrt((double)c12[g])):0, s2=cy[g]>0?1/(log(2)*sqrt((double)cy[g])):0; v+=s1*s1+s2*s2;
    printf("  %-11s 12. den. %llu / %llu (%.3f bit) — yeni %llu / %llu (%.3f bit); koşullu binom p = %.4f\n",ad[g],(unsigned long long)c12[g],(unsigned long long)n12[g],bo,(unsigned long long)cy[g],(unsigned long long)ny[g],by,p); }
  if(Pad>0){ double lam=Pad*(double)n2; double p=poisson_iki((long)f2,lam); if(p<0.001) gecti=0;
    printf("  F adanmış kestirimle: 12. den. P = %.4g (%ld / %ld); beklenen %.2f, gözlenen %llu; Poisson iki yanlı p = %.4f\n",Pad,had,nad,lam,(unsigned long long)f2,p); }
  printf("  el dili bilgi puanı (en iyi mercek L=%d): yeni %.3f bit; 12. den. %.3f bit; fark %+.3f (arama bileşenleri toplamı farkı %+.3f, σ ≈ %.3f)\n",Lb,S,S12,S-S12,d,sqrt(v));
  printf("  ÇAPA: %s\n",gecti?"TUTTU (bütün aramalar p ≥ 0,001 ile tutarlı)":"TUTMADI");
  return gecti?0:1; }
