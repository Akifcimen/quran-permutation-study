// tablo_kur — ana referans tablosundan arama tablosu (bağlamlar, en-az sayıları). Kullanım: ./tablo_kur <kodeks> <ana.ref> <cikti.tab>
#include "tablo.h"
int main(int argc,char**argv){ int kod=atoi(argv[1]); butunluk_init(kod); ref_t R; ref_ayir(&R);
  if(!ref_oku_ekle(&R,argv[2])){ fprintf(stderr,"okunamadı\n"); return 2; } tablo_t T; tablo_kur(&R,&T); tablo_yaz(&T,argv[3]);
  long cek=0; for(int L=0;L<NLENS;L++) for(int k=0;k<KC;k++){ int c=T.ctx[L*KC+k]; (void)c; }
  for(int c=0;c<T.nctx;c++) if(T.cn[c]>=NMIN) cek++;
  fprintf(stderr,"N=%llu bağlam %d (n≥NMIN: %ld)\n",(unsigned long long)T.N,T.nctx,cek); return 0; }
