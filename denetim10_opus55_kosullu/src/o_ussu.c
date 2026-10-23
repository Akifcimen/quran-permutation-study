// o_ussu — O' = O ∧ O4 (terazi) koşullu kitaplar: O-havuzundaki her kitabın iskeleti sabit tutulur, tamamlaması
// koşullu dağılımından yeniden çekilir ve terazi (A) tutanlar kaydedilir. İskelet başına SABİT emek harcandığından
// iskeletin beklenen katkısı P(A | iskelet) ile orantılıdır → çıktı akışı P(· | O') dağılımındadır.
//   U: kesin tamamlama (ret; erken kesme), iskelet başına R deneme.
//   M, L: tamamlama zinciri (tamamla.h), kitabın kendi tamamlamasından başlar, iskelet başına R kayıt × K adım.
// Kullanım: ./o_ussu <kodeks> <U|M|L> <seed> <R> <K> <cikti.bin> <O-havuz.bin>...
#include "tamamla.h"
typedef struct __attribute__((packed)) { uint16_t a[114]; uint32_t f; uint32_t zincir; uint64_t sira; } kayit_t;
// kesin tekdüze tamamlama: aile (t≡0) sabit, kalan değerler kalan konumlara, yeni üye olmadan düzgün
static void u_tamamla(rng_t *rg, const int *a0, int *a){
  int P[114], V[114], np=0; for(int i=0;i<114;i++){ if(((i+1)+a0[i])%19==0) a[i]=a0[i]; else { P[np]=i; V[np]=a0[i]; np++; } }
  for(;;){ int ok=1; for(int i=0;i<np;i++){ int j=i+(int)bounded(rg,(uint32_t)(np-i)); int t=V[i]; V[i]=V[j]; V[j]=t; if((P[i]+1+V[i])%19==0){ ok=0; break; } } if(ok) break; }
  for(int i=0;i<np;i++) a[P[i]]=V[i]; }
int main(int argc, char **argv){
  if(argc<8){ fprintf(stderr,"kullanım: %s <kodeks> <U|M|L> <seed> <R> <K> <cikti> <havuz>...\n",argv[0]); return 1; }
  int kod=atoi(argv[1]); char nul=argv[2][0]; uint64_t seed=strtoull(argv[3],NULL,10); long R=atol(argv[4]); int Kk=atoi(argv[5]); init_ortak(kod);
  double q=0; if(nul=='L'){ ref_hazirla(); q=mallows_q_ayarla(ters_kesin(BASE)); }
  char tmp[600]; snprintf(tmp,600,"%s.yazılıyor",argv[6]); FILE *fo=fopen(tmp,"wb");
  rng_t rg; rseed(&rg,seed); long nisk=0, nA=0, ndeneme=0; uint64_t kab=0, on=0;
  for(int d=7; d<argc; d++){ FILE *fi=fopen(argv[d],"rb"); if(!fi){ fprintf(stderr,"açılamadı: %s\n",argv[d]); return 1; } kayit_t r;
    while(fread(&r,sizeof r,1,fi)==1){ int a0[114], a[114]; for(int i=0;i<114;i++) a0[i]=r.a[i]; nisk++;
      if(nul=='U'){ for(long t=0;t<R;t++){ u_tamamla(&rg,a0,a); ndeneme++; int f=olaylar(a);
          if((f&EV_A) && OK_O(f)){ kayit_t K; for(int i=0;i<114;i++) K.a[i]=a[i]; K.f=f; K.zincir=(uint32_t)nisk; K.sira=(uint64_t)t; fwrite(&K,sizeof K,1,fo); nA++; } } }
      else { tz_t Z; tz_init(&Z,nul,a0,seed*1000003ULL+(uint64_t)nisk,q);
        for(long t=0;t<R;t++){ for(int s=0;s<Kk;s++) tz_adim(&Z); ndeneme++;
          if(tz_A(&Z)){ int f=olaylar(Z.a); if(!OK_O(f)||!(f&EV_A)){ fprintf(stderr,"HATA: artımlı A/O tutarsız\n"); return 2; }
            kayit_t K; for(int i=0;i<114;i++) K.a[i]=Z.a[i]; K.f=f; K.zincir=(uint32_t)nisk; K.sira=(uint64_t)t; fwrite(&K,sizeof K,1,fo); nA++; } }
        kab+=Z.kabul; on+=Z.oneri; } }
    fclose(fi); }
  fclose(fo); rename(tmp,argv[6]);
  char sf[600]; snprintf(sf,600,"%s.say",argv[6]); FILE *fs=fopen(sf,"w");
  fprintf(fs,"kodeks %d sifir %c seed %llu iskelet %ld deneme %ld A_isabet %ld P(A|O)_tahmin %.4e kabul_orani %.3f\n",kod,nul,(unsigned long long)seed,nisk,ndeneme,nA,nA/(double)ndeneme,on?kab/(double)on:1.0);
  fclose(fs); return 0; }
