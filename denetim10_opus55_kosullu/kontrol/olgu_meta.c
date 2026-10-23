#include "../src/ortak.h"
int main(){ init_ortak(129); printf("{\"ad\": ["); for(int f=0;f<NFACT;f++) printf("%s\"%s\"",f?",":"",FNAME[f]); printf("], \"dugum\": ["); for(int f=0;f<NFACT;f++) printf("%s%d",f?",":"",FNODE[f]);
  printf("], \"ctx\": ["); for(int f=0;f<NFACT;f++) printf("%s%d",f?",":"",FCTX[f]); printf("], \"dugum_ad\": ["); for(int i=0;i<NNODE;i++) printf("%s\"%s\"",i?",":"",NODENAME[i]); printf("]}\n"); return 0; }
