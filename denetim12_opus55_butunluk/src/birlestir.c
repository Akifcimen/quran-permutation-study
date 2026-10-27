// birlestir — referans bölüt deltalarını (seyrek .ref) bir ana tabloya toplar. Kullanım: ./birlestir <kodeks> <ana.ref> <delta.ref>...
// Ana dosya yoksa oluşturulur. Yazım atomiktir; deltaları silmek sürücünün işidir (ana yazıldıktan sonra).
#include "tablo.h"
int main(int argc,char**argv){ int kod=atoi(argv[1]); butunluk_init(kod); ref_t R; ref_ayir(&R);
  FILE *t=fopen(argv[2],"rb"); if(t){ fclose(t); if(!ref_oku_ekle(&R,argv[2])){ fprintf(stderr,"ana okunamadı\n"); return 2; } }
  for(int i=3;i<argc;i++) if(!ref_oku_ekle(&R,argv[i])){ fprintf(stderr,"delta okunamadı: %s\n",argv[i]); return 2; }
  ref_yaz(&R,argv[2]); fprintf(stderr,"ana tablo N = %llu\n",(unsigned long long)R.N); return 0; }
