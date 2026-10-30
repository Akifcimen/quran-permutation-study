// kontrolb_sinama — kontrol programının bağımsız yeniden sayımı (ONKAYIT §0, §7.3; mekanik): aynı tür-8 kontrol kitapları, HEDEF OLMAYAN
// kitapların dilleri (kol 1–4). Puanlar doğrudan hesaplanır (double): F/W grupları seyrek listelerden doğrudan en-az sayımıyla (önbellekli),
// B/Z/H plasebo12'nin yoğun üst-küme tablolarından; kapı G ve B (tür/düzey) etiketlerden elle; Kol B'de budama yok. Karşılaştırılanlar:
// k (ve kol 1–3'te kB) kontrol_b_test çıktısıyla; eşikler kontrol_b_test'in esikler.txt'siyle ve dil_b'nin ayrıntısıyla (|fark| < 1e-4).
// Yalnız uyuşmazlık sayıları basılır. Kullanım: ./kontrolb_sinama <sifir> <hedef.bin> <diller.txt> <ayrinti.txt> <b1> <b2> <dizin> <ref>...
#include "../src/dtab.h"
#ifndef BN
#define BN 4000
#endif
static const atablo_t *TT;
typedef struct { uint64_t mask; uint32_t key; uint32_t c; uint8_t dolu; } hc_t;
#define HCAP (1u<<24)
static hc_t *HCT; static uint64_t HDOLU=0;
static uint64_t sayF(int L, int cf, int g, uint64_t s){       // F (g=0) / W (g=1) en-az sayısı, doğrudan, önbellekli
  const uint32_t key=((uint32_t)L*NCX+(uint32_t)cf)*2u+(uint32_t)g; uint64_t h=(s*0x9E3779B97F4A7C15ULL)^((uint64_t)key*0xC2B2AE3D27D4EB4FULL); h^=h>>29;
  for(uint32_t i=(uint32_t)h&(HCAP-1);;i=(i+1)&(HCAP-1)){ hc_t *e=&HCT[i]; if(!e->dolu){ uint64_t c=0;
      for(int kb=0;kb<KC;kb++){ if(TT->cF[L][kb]!=cf) continue; const uint32_t lk=LKG(L,kb,g); for(uint64_t x=TT->bas[lk];x<TT->bas[lk+1];x++) if((TT->E[x].mask&s)==s) c+=TT->E[x].c; }
      e->dolu=1; e->mask=s; e->key=key; e->c=(uint32_t)c; if(++HDOLU>HCAP/4*3){ fprintf(stderr,"önbellek doldu\n"); exit(3); } return c; }
    if(e->mask==s && e->key==key) return e->c; } }
static double dogrudan(const prof_t *P, const uint64_t lm[NGRUP], int kol, int L){   // bir mercekteki değer (B tutmuyorsa −∞)
  const uint64_t x[NGRUP]={P->B[L]&lm[0],P->Z[L]&lm[1],P->F[L]&lm[2],P->W[L]&lm[3],P->H[L]&lm[4]};
  if(kol>=1 && kol<=3){ const int tm=kol==1?3:(kol==2?2:4); if(!((int)P->kF[L]*PR[L%NPR]>=228 && P->mc[L]>=2)) return -INFINITY;
    int t=0, d=0; for(int k=0;k<NGRUP;k++){ uint64_t y=x[k]; while(y){ int b=__builtin_ctzll(y); y&=y-1; t|=1<<PTUR[GOFS[k]+b]; d|=1<<PDUZEY[GOFS[k]+b]; } }
    if(__builtin_popcount(t)<tm || __builtin_popcount(d)<tm) return -INFINITY; }
  const int cf=TT->cF[L][kkutu(P->kF[L])], ch=TT->cH[L][kkutu(P->kH[L])]; double b=0;
  if(x[0]) b+=bitf(TT->BA[L][x[0]],TT->N); if(x[1]) b+=bitf(TT->ZA[L][x[1]],TT->N);
  if(x[2]) b+=bitf(sayF(L,cf,0,x[2]),TT->nnF[L][cf]); if(x[3]) b+=bitf(sayF(L,cf,1,x[3]),TT->nnF[L][cf]);
  if(x[4]) b+=bitf(TT->HA[((size_t)L*NCX+ch)*128+x[4]],TT->nnH[L][ch]);
  return b-LBEDEL[0][L]; }
static double puan(const prof_t *P, const uint64_t lm[NGRUP], int kol, int *bvar){ double best=-INFINITY; int bv=0;
  for(int L=0;L<NLENS;L++){ const double v=dogrudan(P,lm,kol,L); if(v>-INFINITY) bv=1; if(v>best) best=v; } if(bvar) *bvar=bv; return best; }
int main(int argc,char**argv){ if(argc<9) return 1; char s=argv[1][0]; const char *hf=argv[2], *df=argv[3], *af=argv[4]; int b1=atoi(argv[5]), b2=atoi(argv[6]); const char *diz=argv[7];
  kutuphane_init(); mercek_bedeli_hazirla(); static atablo_t T; atablo_kur(&T,argv+8,argc-8); TT=&T; HCT=calloc(HCAP,sizeof(hc_t));
  FILE *f=fopen(hf,"rb"); fseek(f,0,SEEK_END); int NH=(int)(ftell(f)/232); fseek(f,0,SEEK_SET); int16_t (*H)[114]=malloc(sizeof(*H)*NH);
  for(int h=0;h<NH;h++){ int32_t id; if(fread(&id,4,1,f)!=1||fread(H[h],2,114,f)!=114) return 2; } fclose(f);
  enum { MD=600 }; static char ad[MD][16]; static int hed[MD], kol[MD]; static uint64_t lm[MD][NGRUP]; int nd=0; f=fopen(df,"r"); int kod, L, n;
  while(nd<MD && fscanf(f,"%15s %d %d %d %d",ad[nd],&hed[nd],&kod,&L,&n)==5){ int g; for(int k=0;k<NGRUP;k++) lm[nd][k]=0;
    for(int i=0;i<n;i++){ if(fscanf(f,"%d",&g)!=1) return 2; const int k=pgrup(g); lm[nd][k]|=1ull<<(g-GOFS[k]); } kol[nd]=kod/10; if(L<0||n==0) continue; nd++; } fclose(f);
  static prof_t P; static double esik[MD];
  for(int d=0;d<nd;d++){ int a[114]; for(int q=0;q<114;q++) a[q]=H[hed[d]][q]; kdegerlendir(a,&P,0x3FFF); esik[d]=puan(&P,lm[d],kol[d],NULL); }
  static long k[MD], kb[MD]; long sinir=0; orn_t *Z=malloc(sizeof(orn_t));
  for(int b=b1;b<=b2;b++){ orn_init(Z,s,tohum(s,8,b)); for(int i=0;i<BN;i++){ orn_sonraki(Z); kdegerlendir(Z->a,&P,0x3FFF);
      for(int d=0;d<nd;d++){ int bv; const double v=puan(&P,lm[d],kol[d],&bv); if(kol[d]<=3 && bv) kb[d]++; if(v>=esik[d]-1e-6) k[d]++; if(fabs(v-esik[d])<1e-4) sinir++; } } }
  // kontrol_b_test çıktısı
  static long kk[MD], kkb[MD]; for(int b=b1;b<=b2;b++){ char fn[1100]; snprintf(fn,sizeof fn,"%s/b%04d.txt",diz,b); FILE *x=fopen(fn,"r"); if(!x){ printf("bölüt dosyası yok: %s\n",fn); return 2; } char line[512];
    while(fgets(line,512,x)){ char a1[32]; long v1,v2; if(line[0]=='D' && sscanf(line,"D %31s %ld %ld",a1,&v1,&v2)==3){ for(int d=0;d<nd;d++) if(!strcmp(ad[d],a1)){ kk[d]+=v1; kkb[d]+=v2; } } } fclose(x); }
  long hk=0, hb=0, he=0, ha=0, top=0, na=0; char fn[1100]; snprintf(fn,sizeof fn,"%s/esikler.txt",diz); FILE *x=fopen(fn,"r"); char line[1024];
  while(x && fgets(line,1024,x)){ char a1[32]; int hh; double e; if(line[0]=='D' && sscanf(line,"D %31s %d %lf",a1,&hh,&e)==3){ for(int d=0;d<nd;d++) if(!strcmp(ad[d],a1) && fabs(e-esik[d])>1e-4) he++; } } if(x) fclose(x);
  x=fopen(af,"r"); while(x && fgets(line,1024,x)){ char a1[32]; if(sscanf(line,"%31s",a1)!=1) continue; char *p=strstr(line," esik "); if(!p) p=strstr(line," puan "); if(!p) continue; const double e=atof(p+6);
    for(int d=0;d<nd;d++) if(!strcmp(ad[d],a1)){ na++; if(fabs(e-esik[d])>1e-4) ha++; } } if(x) fclose(x);
  int nA=0, nB=0; for(int d=0;d<nd;d++){ if(kol[d]<=3) nA++; else nB++; top+=k[d]; if(k[d]!=kk[d]) hk++; if(kol[d]<=3 && kb[d]!=kkb[d]) hb++; }
  printf("mekanik (sıfır %c): %d dil (kol 1–3: %d, kol 4: %d), %d×%d kontrol; uyuşmazlık: k %ld, kB %ld, eşik (kontrol_b) %ld, eşik (dil_b ayrıntısı) %ld / %ld; eşiğe 1e-4 bitten yakın puan %ld\n",
    s,nd,nA,nB,b2-b1+1,BN,hk,hb,he,ha,na,sinir);
  printf("  kapsam: toplam isabet ≥ 50: %s\n",top>=50?"evet":"hayır");
  return (hk||hb||he||ha)?1:0; }
