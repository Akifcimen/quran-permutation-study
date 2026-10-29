// kontrol_sinama — kontrol programının bağımsız yeniden sayımı (ONKAYIT §0, mekanik): aynı tür-8 kontrol kitapları, aynı diller;
// puanlar doğrudan seyrek listelerden (double) hesaplanır; k sayıları kontrol çıktısıyla karşılaştırılır. Yalnız uyuşmazlıklar basılır.
#include "../src/dil_sec.h"
#ifndef BN
#define BN 4000
#endif
static double puan_dogrudan(const atablo_t *T, const prof_t *P, const int *g, int ng, int onsel){
  uint64_t lm[NGRUP]={0}; for(int i=0;i<ng;i++){ int k=pgrup(g[i]); lm[k]|=1ull<<(g[i]-GOFS[k]); }
  double best=-INFINITY;
  for(int L=0;L<NLENS;L++){ const int cf=T->cF[L][kkutu(P->kF[L])], ch=T->cH[L][kkutu(P->kH[L])]; double b=0;
    uint64_t sB=P->B[L]&lm[0], sZ=P->Z[L]&lm[1], sF=P->F[L]&lm[2], sW=P->W[L]&lm[3], sH=P->H[L]&lm[4];
    if(sB) b+=bitf(T->BA[L][sB],T->N); if(sZ) b+=bitf(T->ZA[L][sZ],T->N); if(sH) b+=bitf(T->HA[((size_t)L*NCX+ch)*128+sH],T->nnH[L][ch]);
    for(int gg=0;gg<2;gg++){ uint64_t s=gg?sW:sF; if(!s) continue; uint64_t c=0; for(int k=0;k<KC;k++){ if(T->cF[L][k]!=cf) continue; uint32_t lk=LKG(L,k,gg);
        for(uint64_t e=T->bas[lk];e<T->bas[lk+1];e++) if((T->E[e].mask&s)==s) c+=T->E[e].c; } b+=bitf(c,T->nnF[L][cf]); }
    const double v=b-LBEDEL[onsel][L]; if(v>best) best=v; }
  return best; }
int main(int argc,char**argv){ char s=argv[1][0]; const char *hf=argv[2], *df=argv[3]; int b1=atoi(argv[4]), b2=atoi(argv[5]); const char *diz=argv[6];
  kutuphane_init(); mercek_bedeli_hazirla(); static atablo_t T; atablo_kur(&T,argv+7,argc-7);
  FILE *f=fopen(hf,"rb"); fseek(f,0,SEEK_END); int NH=(int)(ftell(f)/232); fseek(f,0,SEEK_SET); int16_t (*H)[114]=malloc(sizeof(*H)*NH);
  for(int h=0;h<NH;h++){ int32_t id; fread(&id,4,1,f); fread(H[h],2,114,f); } fclose(f);
  char ad[100][16]; int hed[100], ons[100], n[100], g[100][20], nd=0; int el=-1; f=fopen(df,"r"); int L;
  while(nd<100 && fscanf(f,"%15s %d %d %d %d",ad[nd],&hed[nd],&ons[nd],&L,&n[nd])==5){ for(int i=0;i<n[nd];i++) fscanf(f,"%d",&g[nd][i]); if(!strcmp(ad[nd],"EL")) el=nd; nd++; } fclose(f);
  static prof_t P; double esik[100]; double elesik[1000];
  for(int d=0;d<nd;d++){ if(d==el) continue; int a[114]; for(int q=0;q<114;q++) a[q]=H[hed[d]][q]; kdegerlendir(a,&P,0x3FFF); esik[d]=puan_dogrudan(&T,&P,g[d],n[d],ons[d]); }
  if(el>=0) for(int h=0;h<NH;h++){ int a[114]; for(int q=0;q<114;q++) a[q]=H[h][q]; kdegerlendir(a,&P,0x3FFF); elesik[h]=puan_dogrudan(&T,&P,g[el],n[el],0); }
  long k[100]={0}, ek[1000]={0}; orn_t *Z=malloc(sizeof(orn_t)); long sinir=0;
  for(int b=b1;b<=b2;b++){ orn_init(Z,s,tohum(s,8,b)); for(int i=0;i<BN;i++){ orn_sonraki(Z); kdegerlendir(Z->a,&P,0x3FFF);
      for(int d=0;d<nd;d++){ if(d==el){ double v=puan_dogrudan(&T,&P,g[d],n[d],0); for(int h=0;h<NH;h++){ if(v>=elesik[h]-1e-6) ek[h]++; if(fabs(v-elesik[h])<1e-4) sinir++; } continue; }
        double v=puan_dogrudan(&T,&P,g[d],n[d],ons[d]); if(v>=esik[d]-1e-6) k[d]++; if(fabs(v-esik[d])<1e-4) sinir++; } } }
  // kontrol çıktısını oku
  long kk[100]={0}, kek[1000]={0}; for(int b=b1;b<=b2;b++){ char fn[1100]; snprintf(fn,sizeof fn,"%s/b%04d.txt",diz,b); FILE *x=fopen(fn,"r"); char line[512];
    while(fgets(line,512,x)){ char a1[32]; long v1,v2; int h; if(line[0]=='D' && sscanf(line,"D %31s %ld %ld",a1,&v1,&v2)==3){ for(int d=0;d<nd;d++) if(!strcmp(ad[d],a1)) kk[d]+=v1; }
      else if(line[0]=='E' && sscanf(line,"E %d %ld %ld",&h,&v1,&v2)==3) kek[h]+=v1; } fclose(x); }
  long hd=0, he=0, top=0; for(int d=0;d<nd;d++){ if(d==el) continue; top+=k[d]; if(k[d]!=kk[d]) hd++; } for(int h=0;h<NH;h++) if(el>=0 && ek[h]!=kek[h]) he++;
  printf("mekanik: %d dil, %d hedef, %d×%d kontrol; k uyuşmazlığı dil %ld, el-dili hedef %ld; toplam isabet %ld; eşiğe 1e-4 bitten yakın puan %ld\n",nd,NH,b2-b1+1,BN,hd,he,top,sinir);
  return (hd||he)?1:0; }
