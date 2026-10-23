// cekirdek — Ö2: P(A ∧ W | G-iskeleti) her tam-kristal (G) iskeleti için tamamlama zinciriyle (tamamla.h) doğrudan sayım.
// G-iskeletleri koşullu havuzlardan (G ⊂ O) gelir: o sıfırın P(iskelet | G) dağılımından çekilişlerdir.
// Sonuç: P(A∩G∩W | sıfır) = P(G | sıfır) × ortalama_iskelet P(A∧W | iskelet)  (aktarım yok: her çarpan o sıfırda ölçülür).
// Kullanım: ./cekirdek <kodeks> <U|M|L> <seed> <adim> <K> <cikti.txt> <havuz.bin>...   (her K adımda bir sayım)
// U için ayrıca KESIN=1 ortam değişkeniyle kesin (ret) tamamlama kullanılabilir (zincirin sınaması).
#include "tamamla.h"
typedef struct __attribute__((packed)) { uint16_t a[114]; uint32_t f; uint32_t zincir; uint64_t sira; } kayit_t;
static void u_tamamla(rng_t *rg, const int *a0, int *a){
  int P[114], V[114], np=0; for(int i=0;i<114;i++){ if(((i+1)+a0[i])%19==0) a[i]=a0[i]; else { P[np]=i; V[np]=a0[i]; np++; } }
  for(;;){ int ok=1; for(int i=0;i<np;i++){ int j=i+(int)bounded(rg,(uint32_t)(np-i)); int t=V[i]; V[i]=V[j]; V[j]=t; if((P[i]+1+V[i])%19==0){ ok=0; break; } } if(ok) break; }
  for(int i=0;i<np;i++) a[P[i]]=V[i]; }
int main(int argc, char **argv){
  if(argc<8){ fprintf(stderr,"kullanım bkz. kaynak\n"); return 1; }
  int kod=atoi(argv[1]); char nul=argv[2][0]; uint64_t seed=strtoull(argv[3],NULL,10); long L=atol(argv[4]); int Kk=atoi(argv[5]); init_ortak(kod);
  int kesin=getenv("KESIN")?atoi(getenv("KESIN")):0;
  double q=0; if(nul=='L'){ ref_hazirla(); q=mallows_q_ayarla(ters_kesin(BASE)); }
  FILE *fo=fopen(argv[6],"w"); fprintf(fo,"# iskelet dosya sira sayim A W AW kabul_orani\n");
  // G-iskeletlerini topla; MAXISK'ten (varsayılan 1000) fazlaysa düzgün örnekle (ONKAYIT §5)
  long maxisk=getenv("MAXISK")?atol(getenv("MAXISK")):1000; long cap=1024, ng=0; kayit_t *GB=malloc(sizeof(kayit_t)*cap); int *gd=malloc(sizeof(int)*cap); uint32_t *gs=malloc(4*cap);
  for(int d=7; d<argc; d++){ FILE *fi=fopen(argv[d],"rb"); if(!fi){ fprintf(stderr,"açılamadı: %s\n",argv[d]); return 1; } kayit_t r; uint32_t s=0;
    while(fread(&r,sizeof r,1,fi)==1){ s++; if(!(r.f&EV_G)) continue; if(ng==cap){ cap*=2; GB=realloc(GB,sizeof(kayit_t)*cap); gd=realloc(gd,sizeof(int)*cap); gs=realloc(gs,4*cap); } GB[ng]=r; gd[ng]=d-7; gs[ng]=s-1; ng++; }
    fclose(fi); }
  long nsec=ng; if(ng>maxisk){ rng_t rs; rseed(&rs,seed^0x5EEDULL); for(long t=0;t<maxisk;t++){ long j=t+(long)bounded(&rs,(uint32_t)(ng-t)); kayit_t x=GB[t]; GB[t]=GB[j]; GB[j]=x; int y=gd[t]; gd[t]=gd[j]; gd[j]=y; uint32_t z=gs[t]; gs[t]=gs[j]; gs[j]=z; } nsec=maxisk; }
  fprintf(fo,"# G-iskeleti toplam %ld, işlenen %ld (MAXISK %ld)\n",ng,nsec,maxisk);
  long nisk=0; double topAW=0, topA=0, topW=0;
  for(long g=0; g<nsec; g++){ kayit_t r=GB[g]; int d=gd[g]+7; uint32_t s=gs[g]+1; { int a0[114]; for(int i=0;i<114;i++) a0[i]=r.a[i]; nisk++;
      long nA=0,nW=0,nAW=0,ns=0; double kab=1;
      if(kesin && nul=='U'){ rng_t rg; rseed(&rg,seed*1000003ULL+nisk); int a[114];
        for(long t=0;t<L;t++){ u_tamamla(&rg,a0,a); int f=olaylar(a); if(!(f&EV_G)){ fprintf(stderr,"HATA G\n"); return 2; } ns++; if(f&EV_A) nA++; if(f&EV_W) nW++; if((f&EV_A)&&(f&EV_W)) nAW++; } }
      else { tz_t Z; tz_init(&Z,nul,a0,seed*1000003ULL+(uint64_t)nisk,q);
        for(long t=0;t<L;t++){ for(int k=0;k<Kk;k++) tz_adim(&Z); ns++; int A=tz_A(&Z), W=tz_W(&Z); if(A) nA++; if(W) nW++;
          if(A&&W){ nAW++; int f=olaylar(Z.a); if(!(f&EV_G)||!(f&EV_A)||!(f&EV_W)){ fprintf(stderr,"HATA: artımlı A/W/G tutarsız\n"); return 2; } } }
        kab=Z.kabul/(double)Z.oneri; }
      fprintf(fo,"%ld %d %u %ld %ld %ld %ld %.4f\n",nisk,d-7,s-1,ns,nA,nW,nAW,kab); fflush(fo);
      topAW+=nAW/(double)ns; topA+=nA/(double)ns; topW+=nW/(double)ns; } }
  fprintf(fo,"# TOPLAM iskelet %ld  ort P(A|G)=%.4e  P(W|G)=%.4e  P(A∧W|G)=%.4e\n",nisk,topA/nisk,topW/nisk,topAW/nisk); fclose(fo); return 0; }
