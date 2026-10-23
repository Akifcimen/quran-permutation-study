// Ö0(b) — gerektirme taraması (yalnız ADAY üretir; kesinlik kanıtla ya da karşı-örnekle belirlenir).
// Kaynaklar: U, M20, J10, L(Mallows), O (tekdüze kesin O-koşullu), X (aile boyları zorlanmış çeşitli kitaplar).
// Her kaynak için n(f), n(f∧g), n(f∧g∧h) sayılarını yazar (99 olgu; MX hesaplanmaz).
// Kullanım: ./cikarim <kodeks> <kaynak U|M|J|L|O|X> <seed> <N> <cikti.bin>
#include "kosul_u.h"
#define NFX 99
static uint64_t C1[NFX], C2[NFX][NFX], C3[NFX][NFX][NFX];
static void say(u128 b){ int ix[NFX], m=0; while(b){ int f=ctz128(b); b&=b-1; ix[m++]=f; }
  for(int i=0;i<m;i++){ C1[ix[i]]++; for(int j=i+1;j<m;j++){ C2[ix[i]][ix[j]]++; for(int k=j+1;k<m;k++) C3[ix[i]][ix[j]][ix[k]]++; } } }
int main(int argc, char **argv){
  int kod=atoi(argv[1]); init_ortak(kod); char src=argv[2][0]; uint64_t seed=strtoull(argv[3],NULL,10), N=strtoull(argv[4],NULL,10);
  int a[114]; sampler_t *Z=malloc(sizeof(sampler_t)); usamp_t *U=NULL; rng_t rg; rseed(&rg,seed*7919+11); ustat_t st; memset(&st,0,sizeof st);
  if(src=='U') sampler_init(Z,'U',0,seed); else if(src=='M') sampler_init(Z,'M',20,seed); else if(src=='J') sampler_init(Z,'J',10,seed);
  else if(src=='L'){ ref_hazirla(); double q=mallows_q_ayarla(ters_kesin(BASE)); sampler_init(Z,'L',q,seed); }
  else if(src=='O'){ U=malloc(sizeof(usamp_t)); usamp_init(U,12); char bf[256]; snprintf(bf,256,"sonuc/u_B_%d_K12.txt",kod); usamp_B_oku(U,bf); }
  long nO=0; FILE *fr=NULL; if(src=='R'){ fr=fopen(getenv("KAYIT"),"rb"); if(!fr){ fprintf(stderr,"KAYIT dosyası?\n"); return 1; } }
  for(uint64_t it=0; it<N; it++){
    if(src=='R'){ uint16_t kb[122]; if(fread(kb,2,122,fr)!=122){ N=it; break; } for(int i=0;i<114;i++) a[i]=kb[i]; }
    else if(src=='O'){ int fi[12],fl[12]; usamp_kitap(U,&rg,'O',a,fi,fl,&st); }
    else if(src=='X'){ // çeşitli: her kitapta rastgele ρF, ρG; konum sırası rastgele, uyumlu değer olasılıkla seçilir
      double rF=unif(&rg)*0.6, rG=unif(&rg)*0.6; int pool[114]; memcpy(pool,BASE,sizeof pool); int np=114; int ord[114]; for(int i=0;i<114;i++) ord[i]=i;
      for(int i=113;i>0;i--){ int j=(int)bounded(&rg,i+1); int t=ord[i]; ord[i]=ord[j]; ord[j]=t; }
      for(int s=0;s<114;s++){ int i=ord[s], n=i+1; double u=unif(&rg); int want=-1; if(u<rF) want=(19-n%19)%19; else if(u<rF+rG) want=n%19;
        int pick=-1; if(want>=0){ int cand[114],nc=0; for(int k=0;k<np;k++) if(pool[k]%19==want) cand[nc++]=k; if(nc) pick=cand[bounded(&rg,nc)]; }
        if(pick<0) pick=(int)bounded(&rg,np); a[i]=pool[pick]; pool[pick]=pool[--np]; } }
    else { sampler_next(Z); memcpy(a,Z->a,sizeof a); }
    fres_t R; facts19(a,&R,0); say(R.bits); if(OK_O(olaylar(a))) nO++;
  }
  FILE *fo=fopen(argv[5],"wb"); uint64_t hdr[3]={N,(uint64_t)nO,NFX}; fwrite(hdr,8,3,fo); fwrite(C1,8,NFX,fo); fwrite(C2,8,NFX*NFX,fo); fwrite(C3,8,(size_t)NFX*NFX*NFX,fo); fclose(fo);
  printf("kaynak %c: %llu kitap, O-kitap %ld\n",src,(unsigned long long)N,nO); return 0; }
