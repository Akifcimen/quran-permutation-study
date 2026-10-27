// kosullu_uret — bir bölüt koşullu kitap üretir (kontrol noktası: bölüt dosyası tamamlanınca atomik yazılır).
// Kullanım: ./kosullu_uret <kodeks> <sifir U|M|L|J> <kosul O|C|G|B> <seed> <N_hedef> <cikti.bin>
//   U: tekdüze kesin koşullu örnekleyici (kosul_u.h; ret + Φ kabulü; çıktılar bağımsız ve tam dağılımlı)
//   M: robotun M20 sıfırı (64 zincir, tam azalandan 30000 adım yakma, 20 önerilik emisyon; birebir akış), emisyonlarda süzme
//   L: Mallows (q yalnız E[I] = I_gözlenen olacak biçimde ayarlı), kesin Lehmer örneklemesi, ret
//   J: robotun J10 sıfırının dağılımca aynısı (gerçek dizilişin σ=10 konum titreşimi; hizli.h jz_kitap), ret — yalnız ikincil, "veriye merkezli"
// Koşul: O = O1∧O2∧O3 (12 üye, Σt=1444, 6/6, 722/722, tek-t kolunun iki hücresi 361); C = O1∧O2; G = tam kristal; B = O1.
// Kayıt: uint16 a[114] | uint32 olay bayrakları | uint32 zincir (M) | uint64 emisyon/deneme sırası  (= 244 bayt)
// Bölüt sonunda <cikti>.say: deneme (emisyon) sayısı, isabet, ek sayaçlar.
#include "kosul_u.h"
#include "hizli.h"
#include <time.h>
#include <unistd.h>
typedef struct __attribute__((packed)) { uint16_t a[114]; uint32_t f; uint32_t zincir; uint64_t sira; } kayit_t;
static int kosul_tut(int f, char kos){ switch(kos){ case 'O': return OK_O(f); case 'C': return (f&EV_C)!=0; case 'G': return (f&EV_G)!=0; case 'B': return (f&EV_B)!=0; } return 0; }
int main(int argc, char **argv){
  if(argc<7){ fprintf(stderr,"kullanım: %s <kodeks> <U|M|L|J> <O|C|G|B> <seed> <N> <cikti>\n",argv[0]); return 1; }
  int kod=atoi(argv[1]); char sif=argv[2][0], kos=argv[3][0]; uint64_t seed=strtoull(argv[4],NULL,10); long N=atol(argv[5]); const char *out=argv[6];
  init_ortak(kod); comp_hazirla();
  char tmp[600]; snprintf(tmp,600,"%s.yazılıyor",out); FILE *fo=fopen(tmp,"wb"); if(!fo){ perror("fopen"); return 1; }
  long hit=0; uint64_t den=0; kayit_t K; int a[114]; struct timespec t0,t1; clock_gettime(CLOCK_MONOTONIC,&t0);
  ustat_t st; memset(&st,0,sizeof st); long ekB=0, ekC=0, ekG=0;
  if(sif=='U'){
    if(kos=='G'){ fprintf(stderr,"U için G doğrudan yok (C ya da O kullan)\n"); return 1; }
    usamp_t *U=malloc(sizeof(usamp_t)); usamp_init(U,12); char bf[256]; snprintf(bf,256,"sonuc/u_B_%d_K12.txt",kod); usamp_B_oku(U,bf);
    rng_t rg; rseed(&rg,seed); int fi[12],fl[12];
    while(hit<N){ usamp_kitap(U,&rg,kos=='B'?'B':(kos=='C'?'C':'O'),a,fi,fl,&st); int f=olaylar(a); if(!kosul_tut(f,kos)){ fprintf(stderr,"HATA: koşul tutmuyor\n"); return 2; }
      for(int i=0;i<114;i++) K.a[i]=(uint16_t)a[i]; K.f=(uint32_t)f; K.zincir=0; K.sira=(uint64_t)st.oneri; fwrite(&K,sizeof K,1,fo); hit++; }
    den=(uint64_t)st.oneri;
  } else if(sif=='M'){
    m20_t *M=malloc(sizeof(m20_t)); m20_init(M,seed,20);
    while(hit<N){ int c=m20_emit(M); den++; mz_t *z=&M->ch[c];
      if(z->kF==12 && z->cs==76){ ekB++; int f=olaylar(z->b); if(f&EV_C) ekC++; if(f&EV_G) ekG++;
        if(kosul_tut(f,kos)){ for(int i=0;i<114;i++) K.a[i]=(uint16_t)z->b[i]; K.f=(uint32_t)f; K.zincir=(uint32_t)c; K.sira=den; fwrite(&K,sizeof K,1,fo); hit++; } } }
  } else if(sif=='L'){
    ref_hazirla(); double q=mallows_q_ayarla(ters_kesin(BASE)); lz_t *L=malloc(sizeof(lz_t)); lz_init(L,q,seed);
    while(hit<N){ den++; if(!lz_kitap(L,a)) continue; ekB++; int f=olaylar(a); if(f&EV_C) ekC++; if(f&EV_G) ekG++;
      if(kosul_tut(f,kos)){ for(int i=0;i<114;i++) K.a[i]=(uint16_t)a[i]; K.f=(uint32_t)f; K.zincir=0; K.sira=den; fwrite(&K,sizeof K,1,fo); hit++; } }
  } else if(sif=='J'){
    jz_t *Z=malloc(sizeof(jz_t)); jz_init(Z,10.0,seed);
    while(hit<N){ den++; if(!jz_kitap(Z,a)) continue;
      ekB++; int f=olaylar(a); if(f&EV_C) ekC++; if(f&EV_G) ekG++;
      if(kosul_tut(f,kos)){ for(int i=0;i<114;i++) K.a[i]=(uint16_t)a[i]; K.f=(uint32_t)f; K.zincir=0; K.sira=den; fwrite(&K,sizeof K,1,fo); hit++; } }
  } else { fprintf(stderr,"sıfır?\n"); return 1; }
  fclose(fo); clock_gettime(CLOCK_MONOTONIC,&t1); double sn=(t1.tv_sec-t0.tv_sec)+1e-9*(t1.tv_nsec-t0.tv_nsec);
  char sf[600]; snprintf(sf,600,"%s.say",out); FILE *fs=fopen(sf,"w");
  fprintf(fs,"kodeks %d sifir %c kosul %c seed %llu isabet %ld deneme %llu sure_s %.1f B %ld C %ld G %ld",kod,sif,kos,(unsigned long long)seed,hit,(unsigned long long)den,sn,ekB,ekC,ekG);
  if(sif=='U') fprintf(fs," kosul_gecen %ld phi_kabul %ld tau_deneme %ld bihlal %ld phimaks %.10Lg",st.kosul_gecen,st.phi_kabul,st.tau_deneme,st.bihlal,st.phi_maks_gorulen);
  fprintf(fs,"\n"); fclose(fs); rename(tmp,out); return 0; }
