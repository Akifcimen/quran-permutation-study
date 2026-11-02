// duz_kart_emul.c — duz_ortak.h yürüyüşünün işlemci arka ucu (duz_kart_cihaz.cu ile aynı arayüz; eşdeğerlik sınaması için).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "duz_ortak.h"
static const ksabit_t *hC; static const dolay_t *hE; static yur_t *hY; static kprof_t *hP; static int NW; static double LQ_;
const char *duz_aciklama(void){ return "işlemci öykünmesi (duz_ortak.h)"; }
int duz_hazirla(const ksabit_t *C, double LQ, const dolay_t *E, int ne, const yur_t *Y, int nw){ (void)ne;
  hC=C; hE=E; hY=malloc(sizeof(yur_t)*nw); memcpy(hY,Y,sizeof(yur_t)*nw); hP=malloc(sizeof(kprof_t)); NW=nw; LQ_=LQ; return 0; }
int duz_yurut(int64_t parca){ for(int w=0;w<NW;w++) d_yuru(&hY[w],&hE[hY[w].e],hC,hP,LQ_,parca); return 0; }
int duz_al(yur_t *Y){ memcpy(Y,hY,sizeof(yur_t)*NW); return 0; }
