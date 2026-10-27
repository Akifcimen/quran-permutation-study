// kuyruk — model-tabanlı kuyruk R2 (ONKAYIT §6.2). Mercekler üzerinde birleşim (toplam):
//   bilgi:  P(S ≥ s) ≈ Σ_L Σ_bağlam π Σ_{b,f,w} P_B(b) P_F(f|bağlam) P_W(w|bağlam) 1{I_B + I_boy + I_F + I_W − c(L) ≥ s}   (v = 0)
//   sayım:  P(C ≥ c) ≈ Σ_L Σ_k π_k Σ_{b,f',w} P_B(b) P(f' | k) P_W(w | k) 1{kredi(b ∪ f' ∪ w) ≥ c}  (kapı G; f' kapılı aile maskesi)
// Gruplar (kitap, aile-içi, iki-el) bağlam verildiğinde bağımsız varsayılır; kesin maske olasılıkları referans tablosundan.
// Kullanım: ./kuyruk <kodeks> <ana.ref> <tablo.tab> <cikti.txt>
#include "bilgi.h"
typedef struct { double v, p; } vp_t;
typedef struct { uint64_t key; double c; } gh_t;
static int gcmp(const void *a,const void *b){ uint64_t x=((const gh_t*)a)->key, y=((const gh_t*)b)->key; return x<y?-1:x>y?1:0; }
static int vp_cmp(const void *a, const void *b){ double x=((const vp_t*)a)->v, y=((const vp_t*)b)->v; return (x<y)?1:(x>y)?-1:0; } // azalan
static double yz(uint64_t c, uint64_t n){ if(c==0) return log2((double)n+1)-log2(0.5); if(c<T_COZ) return log2((double)n+1)-log2(c+0.5); return log2((double)n)-log2((double)c); }
#define NS 161
int main(int argc,char**argv){ int kod=atoi(argv[1]); butunluk_init(kod); bedel_hazirla(); ref_t R; ref_ayir(&R); if(!ref_oku_ekle(&R,argv[2])){ fprintf(stderr,"ref\n"); return 2; }
  tablo_t T; if(!tablo_oku(&T,argv[3])){ fprintf(stderr,"tab\n"); return 2; } const double N=(double)R.N;
  double s_grid[NS]; for(int i=0;i<NS;i++) s_grid[i]=i*0.25;     // 0..40 bit
  static double Pinfo[NS]; static double Pcount[NPRED+2]; memset(Pinfo,0,sizeof Pinfo); memset(Pcount,0,sizeof Pcount);
  // kitap maskesi dağılımı ve yüzeyi (biçim başına)
  double PB[NFORM][4], IB[NFORM][4]; for(int f=0;f<NFORM;f++) for(int b=0;b<4;b++){ PB[f][b]=R.BC[f][b]/N; IB[f][b]= b? yz(T.BA[f][b],T.N):0; }
  vp_t *fw=malloc(sizeof(vp_t)*NFM*NWM);
  // ---- bilgi R2 ----
  for(int L=0;L<NLENS;L++){ const int f=L/NPR, p=PR[L%NPR]; const double cost=COST[0][L];
    // bağlamları topla
    int ctxs[KC+8], nc=0; for(int k=0;k<KC;k++){ int c=T.ctx[L*KC+k]; int var=0; for(int j=0;j<nc;j++) if(ctxs[j]==c) var=1; if(!var) ctxs[nc++]=c; }
    for(int j=0;j<nc;j++){ const int c=ctxs[j]; const double nctx=(double)T.cn[c]; if(nctx<=0) continue;
      double Fe[NFM], We[NWM]; memset(Fe,0,sizeof Fe); memset(We,0,sizeof We); int kp=-1;
      for(int k=0;k<KC;k++) if(T.ctx[L*KC+k]==c){ for(int m=0;m<NFM;m++) Fe[m]+=R.FC[FIDX(L,k,m)]; for(int m=0;m<NWM;m++) We[m]+=R.WC[WIDX(L,k,m)]; if(k==p) kp=k; }
      double IS=0; if(kp>=0 && T.ctx[L*KC+kp]==c && T.cn[c]==R.NK[(size_t)L*KC+kp] && !(EXCL[L]&(1u<<P_QkF))) IS=yz(T.NKU[(size_t)L*KC+p],T.N);
      int n=0; for(int fm=0;fm<NFM;fm++){ if(Fe[fm]<=0) continue; double iF= fm? yz(T.FA[(size_t)c*NFM+fm],T.cn[c]):0;
        for(int wm=0;wm<NWM;wm++){ if(We[wm]<=0) continue; double iW= wm? yz(T.WA[(size_t)c*NWM+wm],T.cn[c]):0; fw[n].v=iF+iW; fw[n].p=(Fe[fm]/nctx)*(We[wm]/nctx); n++; } }
      qsort(fw,n,sizeof(vp_t),vp_cmp);
      // kümülatif: P(I_F+I_W ≥ x)
      for(int b=0;b<4;b++){ if(PB[f][b]<=0) continue; double base=IB[f][b]+IS-cost; double cum=0; int idx=0;
        for(int si=NS-1;si>=0;si--){ double need=s_grid[si]-base; while(idx<n && fw[idx].v>=need){ cum+=fw[idx].p; idx++; } Pinfo[si]+=(nctx/N)*PB[f][b]*cum; } } } }
  // ---- sayım R2 (kapı G) ----
  // kapılı aile maskeleri: GH karma tablosundan (L,k) → (12 bit maske → sayı); anahtar sırasına göre dizilir
  uint64_t ng=R.GH.n; gh_t *G=malloc(sizeof(gh_t)*(ng+1)); uint64_t gi=0;
  for(uint64_t h=0;h<(1ull<<GH_BIT);h++) if(R.GH.anahtar[h]){ G[gi].key=R.GH.anahtar[h]-1; G[gi].c=R.GH.sayi[h]; gi++; }
  qsort(G,gi,sizeof(gh_t),gcmp); uint64_t gp=0;
  for(int L=0;L<NLENS;L++){ const int f=L/NPR;
    for(int k=0;k<KC;k++){ const double nk=R.NK[(size_t)L*KC+k]; const uint64_t lk=(uint64_t)L*KC+k;
      while(gp<gi && (G[gp].key>>12)<lk) gp++; uint64_t g0=gp, g1=gp; while(g1<gi && (G[g1].key>>12)==lk) g1++;
      if(nk<=0) continue;
      double gtop=0; for(uint64_t j=g0;j<g1;j++) gtop+=G[j].c;
      for(int b=0;b<4;b++){ if(PB[f][b]<=0) continue;
        for(int wm=0;wm<NWM;wm++){ const double pw=R.WC[WIDX(L,k,wm)]/nk; if(pw<=0) continue;
          uint32_t mbw=((uint32_t)b)|((uint32_t)wm<<P_W1);
          double pn=(nk-gtop)/nk; if(pn>0){ int cr=kredi(mbw,L); for(int c=0;c<=cr;c++) Pcount[c]+=(nk/N)*PB[f][b]*pw*pn; }
          for(uint64_t j=g0;j<g1;j++){ uint32_t fm=(uint32_t)(G[j].key&4095u);
            uint32_t m=mbw|((fm&2047u)<<P_F1)|((fm>>11)<<P_QkF); int cr=kredi(m,L);
            for(int c=0;c<=cr;c++) Pcount[c]+=(nk/N)*PB[f][b]*pw*(G[j].c/nk); } } } } }
  FILE *fo=fopen(argv[4],"w"); fprintf(fo,"# R2 kodeks %d, referans N %.0f. bilgi (v=0): s P_R2(S>=s) [mercekler üzerinde toplam]\n",kod,N);
  for(int i=0;i<NS;i++) fprintf(fo,"bilgi %.2f %.6g\n",s_grid[i],Pinfo[i]);
  fprintf(fo,"# sayim (kapı G): c P_R2(C>=c)\n"); for(int c=0;c<=NPRED;c++) fprintf(fo,"sayim %d %.6g\n",c,Pcount[c]); fclose(fo); return 0; }
