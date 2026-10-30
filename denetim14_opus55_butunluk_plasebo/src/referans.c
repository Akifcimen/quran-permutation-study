// referans — referans örneklemi (ONKAYIT §3.1, §6): kitap üret, seçilen biçimlerde 406 merceğin ilgili kısmını değerlendir,
// tam kütüphane maskelerinin sayımlarını biriktir. Biçimler süreçlere bölünür (aynı kitap akışı her süreçte yeniden üretilir);
// böylece sayımlar süreçler arasında ayrıktır ve birleştirme gerekmez.
// Kullanım: ./referans <sifir U|M|L> <tur> <ilk_bolut> <son_bolut> <bolut_N> <bicim_bitleri_onaltilik> <cikti_onek> [kontrol_noktasi_araligi]
//   tur 1 = üretim referansı; tur 6 = hız/bellek ölçümü (ONKAYIT §0). Kontrol noktası: <onek>.ck (atomik); bitince <onek>.ref ve <onek>.say.
#include "sayim.h"
#include <time.h>
int main(int argc,char**argv){
  if(argc<8){ fprintf(stderr,"kullanım\n"); return 1; }
  char s=argv[1][0]; int tur=atoi(argv[2]); long b1=atol(argv[3]), b2=atol(argv[4]); long BN=atol(argv[5]);
  uint32_t fbit=(uint32_t)strtoul(argv[6],0,16); const char *on=argv[7]; int ck=argc>8?atoi(argv[8]):5;
  kutuphane_init(); static ref_t R; ref_ayir(&R,1ull<<22);
  char fck[1024], fref[1024], fsay[1024]; snprintf(fck,1024,"%s.ck",on); snprintf(fref,1024,"%s.ref",on); snprintf(fsay,1024,"%s.say",on);
  long bas=b1; uint64_t done=0;
  if(ref_oku(&R,fck,&done)){ bas=(long)done+1; fprintf(stderr,"kontrol noktası: bölüt %llu'e kadar yüklendi (N=%llu)\n",(unsigned long long)done,(unsigned long long)R.N); }
  orn_t *Z=malloc(sizeof(orn_t)); static prof_t P;
  struct timespec t0,t1; clock_gettime(CLOCK_MONOTONIC,&t0);
  for(long b=bas;b<=b2;b++){ orn_init(Z,s,tohum(s,tur,b));
    for(long j=0;j<BN;j++){ orn_sonraki(Z); kdegerlendir(Z->a,&P,fbit); ref_ekle(&R,&P,fbit); }
    if(b==b2 || (b-b1+1)%ck==0) ref_yaz(&R,fck,(uint64_t)b); }
  clock_gettime(CLOCK_MONOTONIC,&t1); double sn=(t1.tv_sec-t0.tv_sec)+1e-9*(t1.tv_nsec-t0.tv_nsec);
  ref_yaz(&R,fref,(uint64_t)b2);
  FILE *f=fopen(fsay,"w"); fprintf(f,"sifir %c tur %d bolut %ld-%ld bolut_N %ld bicim 0x%x N %llu sure_s %.1f girdi_FW %llu kapasite %llu\n",
    s,tur,b1,b2,BN,fbit,(unsigned long long)R.N,sn,(unsigned long long)R.FW.n,(unsigned long long)R.FW.cap); fclose(f);
  remove(fck); return 0; }
