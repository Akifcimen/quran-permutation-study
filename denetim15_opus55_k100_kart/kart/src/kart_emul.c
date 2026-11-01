// kart_emul.c — kart_ortak.h çekirdeğinin işlemci üzerinde sırayla çalışan arka ucu. Amacı yalnız eşdeğerlik sınamasıdır:
// yeniden düzenlenmiş çekirdeğin (mercek başına profil, budamasız puan, kayıt sıralaması) özgün kontrol_b / kontrol_h ile aynı
// dosyaları ürettiğini kartsız bir makinede göstermek. Kart arka ucu (kart_cihaz.cu) aynı çekirdek işlevleri kullanır.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "kart_ortak.h"
static const ksabit_t *C; static const kdil_t *D; static int ND; static const float *HV;
const char *kart_aciklama(void){ return "işlemci öykünmesi (kart_emul.c, tek iş parçacığı)"; }
int kart_hazirla(const ksabit_t *C_, const kdil_t *D_, int nd, const float *havuz, uint64_t nhavuz){ (void)nhavuz; C=C_; D=D_; ND=nd; HV=havuz; return 0; }
int kart_parti(const int *bolut, int nb, int bn, uint64_t *k, uint64_t *kc, uint64_t *nb2, kkayit_t **kay, uint64_t *nkay){
  korn_t *Z=malloc(sizeof(korn_t)); kprof_t *P=malloc(sizeof(kprof_t)); uint64_t cap=1024, n=0; kkayit_t *R=malloc(sizeof(kkayit_t)*cap);
  memset(k,0,sizeof(uint64_t)*(size_t)nb*ND); memset(kc,0,sizeof(uint64_t)*(size_t)nb*ND); memset(nb2,0,sizeof(uint64_t)*nb);
  for(int j=0;j<nb;j++){ k_orn_init(Z,C,k_tohum(C->sif,2,bolut[j]));
    for(int i=0;i<bn;i++){ int16_t a[114]; k_orn_sonraki(Z,C,a);
      kbk_t bk[K_NFORM]; uint8_t mb0[K_NFORM]; uint32_t fm=0;
      for(int f=0;f<K_NFORM;f++){ k_kitap(a,C->al[f],C->be[f],&bk[f]); mb0[f]=k_mb0(&bk[f],C->al[f],C->be[f],C); if(mb0[f]&2u) fm|=1u<<f; }
      if(C->kip==1){ if(!fm) continue; nb2[j]++; } else fm=0x3FFF;
      int GL[K_NLENS], nGL=0;
      for(int f=0;f<K_NFORM;f++){ if(!(fm>>f&1)) continue; for(int q=0;q<K_NPR;q++){ k_mercek(a,f,q,&bk[f],mb0[f],C,P); const int L=f*K_NPR+q; if(k_kapi(C,P,L)) GL[nGL++]=L; } }
      if(nGL==0) continue;
      for(int d=0;d<ND;d++){ double best=-INFINITY; int bl=-1, bv=0;
        for(int x=0;x<nGL;x++){ const int L=GL[x]; if(!k_kapsam_tamam(C,&D[d],P,L)) continue; bv=1; const double v=k_mercek_puan(C,&D[d],HV,P,L); if(v>best){ best=v; bl=L; } }
        if(!bv) continue; kc[(size_t)j*ND+d]++; if(best>=D[d].esik_k) k[(size_t)j*ND+d]++;
        if(D[d].hed==0){ for(int tur=0;tur<=C->kip;tur++){ if(tur==0 && !(best>=D[d].esik_q)) continue;
            if(n==cap){ cap*=2; R=realloc(R,sizeof(kkayit_t)*cap); } kkayit_t *r=&R[n++]; r->bolut_ix=j; r->sira=(uint32_t)i; r->puan=best; r->L=(int16_t)bl; r->dil=(uint16_t)d; r->tur=(uint8_t)tur;
            for(int q=0;q<114;q++) r->a[q]=(uint16_t)a[q]; } } } } }
  free(Z); free(P); *kay=R; *nkay=n; return 0; }
