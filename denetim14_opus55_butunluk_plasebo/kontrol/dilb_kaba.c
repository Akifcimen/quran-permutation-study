// dilb_kaba — D_B'nin ve en geniş dilin kaba-kuvvet sınaması (ONKAYIT §0, §7.3): HEDEF OLMAYAN kitaplarda (hedef_dogrulama, tür 4)
// her kolda (3/3, 2/2, 4/4) kapı G'nin tuttuğu ve sağlanan L* yüklemi ≤ NKABA olan her mercekte bütün alt kümeler (|S| ≤ 20) bağımsız
// yoldan puanlanır: grup katkısı doğrudan seyrek listelerden/yoğun tablodan sayılır (üst-küme dönüşümü kullanılmaz), tür ve düzey
// etiketleri PDUZEY/PTUR'dan doğrudan okunur, B elle sınanır. Karşılaştırılanlar: (i) D_B'nin amacı taranan mercekler üzerindeki en
// büyük değerden küçük değil; (ii) D_B'nin merceği taranmışsa orada eşit; (iii) bütün kapı-G mercekleri taranmışsa dilsizlik kararı
// aynı; (iv) D_B dilinin doğrudan yeniden puanı amaçla aynı ve dil orada B'yi sağlıyor. En geniş dil için: (v) her mercekte doğrudan
// puan, en iyi mercek ve dil aynı. Yalnız uyuşmazlık sayıları basılır.
// Kullanım: ./dilb_kaba <dogrulama.bin> <ilk> <son> <ref>...
#include "../src/dil_sec_b.h"
#define NKABA 16
#ifndef KMER
#define KMER 1000
#endif
#ifndef KKAR
#define KKAR 300
#endif
static uint64_t say_dogrudan(const atablo_t *T, const prof_t *P, int L, int k, uint64_t s){
  if(!s) return 0; const int cf=T->cF[L][kkutu(P->kF[L])], ch=T->cH[L][kkutu(P->kH[L])];
  if(k==G_B) return T->BA[L][s]; if(k==G_Z) return T->ZA[L][s];
  if(k==G_H) return T->HA[((size_t)L*NCX+ch)*128+s];       // B, Z, H: plasebo12'nin yoğun üst-küme tabloları (plasebo12'de doğrulandı)
  uint64_t c=0; for(int kb=0;kb<KC;kb++){ if(T->cF[L][kb]!=cf) continue; const uint32_t lk=LKG(L,kb,k==G_W); for(uint64_t e=T->bas[lk];e<T->bas[lk+1];e++) if((T->E[e].mask&s)==s) c+=T->E[e].c; }
  return c; }
static uint64_t n_baglam(const atablo_t *T, const prof_t *P, int L, int k){ const int cf=T->cF[L][kkutu(P->kF[L])], ch=T->cH[L][kkutu(P->kH[L])];
  if(k==G_B||k==G_Z) return T->N; if(k==G_H) return T->nnH[L][ch]; return T->nnF[L][cf]; }
static int B_tutar(const int *g, int n, int tmin, int dmin){ int t=0, d=0; for(int i=0;i<n;i++){ t|=1<<PTUR[g[i]]; d|=1<<PDUZEY[g[i]]; } return __builtin_popcount(t)>=tmin && __builtin_popcount(d)>=dmin; }
static int kapi_dogrudan(const prof_t *P, int L){ return (int)P->kF[L]*PR[L%NPR]>=228 && P->mc[L]>=2; }
// bir (mercek, dil) çiftinin amacı, doğrudan
static double amac_dogrudan(const atablo_t *T, const prof_t *P, int L, const int *g, int n){
  uint64_t s[NGRUP]={0}; for(int i=0;i<n;i++){ int k=pgrup(g[i]); s[k]|=1ull<<(g[i]-GOFS[k]); } double b=0;
  const uint64_t m[NGRUP]={P->B[L],P->Z[L],P->F[L],P->W[L],P->H[L]};
  for(int k=0;k<NGRUP;k++){ const uint64_t x=s[k]&m[k]; if(x) b+=bitf(say_dogrudan(T,P,L,k,x),n_baglam(T,P,L,k)); }
  double bed=0; for(int i=0;i<n;i++) bed+=PBEDEL[g[i]]; bed+=log2(21.0)-lgamma((double)n+1.0)/log(2.0); return b-bed-LBEDEL[0][L]; }
int main(int argc,char**argv){ if(argc<5) return 1; kutuphane_init(); mercek_bedeli_hazirla(); lmask_hazirla();
  int ilk=atoi(argv[2]), son=atoi(argv[3]); static atablo_t T; atablo_kur(&T,argv+4,argc-4); genum_t *G=genum_ayir(); dilb_tampon_t *BT=dilb_ayir();
  FILE *f=fopen(argv[1],"rb"); fseek(f,0,SEEK_END); int NH=(int)(ftell(f)/232); fseek(f,0,SEEK_SET); int16_t (*H)[114]=malloc(sizeof(*H)*NH);
  for(int h=0;h<NH;h++){ int32_t id; if(fread(&id,4,1,f)!=1||fread(H[h],2,114,f)!=114) return 2; } fclose(f);
  static prof_t P; const int TM[3]={3,2,4}, DM[3]={3,2,4};
  long n_kitap=0, n_mer=0, n_atla=0, h1=0, h2=0, h3=0, h4=0, h5=0, n_dilsiz[3]={0,0,0}, n_tam=0, hg=0, n_esit_mer=0;
  static double VG[NGRUP][1<<NKABA]; static uint64_t AD[1<<NKABA];
  for(int h=ilk;h<=son && h<NH;h++){ int a[114]; for(int i=0;i<114;i++) a[i]=H[h][i]; kdegerlendir(a,&P,0x3FFF); n_kitap++;
    dilb_t d[3]; for(int r=0;r<3;r++){ dil_bul_b(&T,&P,TM[r],DM[r],G,BT,&d[r]); if(d[r].L<0) n_dilsiz[r]++; }
    double enb[3]={-INFINITY,-INFINITY,-INFINITY}, Dmer_enb[3]={-INFINITY,-INFINITY,-INFINITY}; int atlandi=0, Dmer_tarandi[3]={0,0,0};
    double bv_[3]={-INFINITY,-INFINITY,-INFINITY}; int bn_[3]={0,0,0}, bL_[3]={-1,-1,-1}, bg_[3][DMAX];       // eşitlik kurallı en iyi (plasebo12 kuralı)
    for(int L=0;L<NLENS;L++){ if(!kapi_dogrudan(&P,L)) continue;
      const uint64_t s[NGRUP]={P.B[L]&LMASK[0],P.Z[L]&LMASK[1],P.F[L]&LMASK[2],P.W[L]&LMASK[3],P.H[L]&LMASK[4]};
      int u[64], ug[64], ub[64], nu=0, mg[NGRUP], gpos[NGRUP][64];
      for(int k=0;k<NGRUP;k++){ mg[k]=0; uint64_t x=s[k]; while(x){ int b=__builtin_ctzll(x); x&=x-1; gpos[k][mg[k]]=b; ug[nu]=k; ub[nu]=mg[k]; mg[k]++; u[nu++]=GOFS[k]+b; } }
      if(nu>NKABA){ atlandi=1; n_atla++; continue; } n_mer++;
      // grup başına bütün alt kümelerin bit değeri, doğrudan: B/Z/H yoğun tablo; F/W her girdinin (maske ∩ s) alt maskeleri sayılarak
      const int cf=T.cF[L][kkutu(P.kF[L])];
      for(int k=0;k<NGRUP;k++){ const uint32_t top=1u<<mg[k]; const uint64_t nb=n_baglam(&T,&P,L,k);
        if(k==G_F || k==G_W){ memset(AD,0,sizeof(uint64_t)*top);
          for(int kb=0;kb<KC;kb++){ if(T.cF[L][kb]!=cf) continue; const uint32_t lk=LKG(L,kb,k==G_W);
            for(uint64_t e=T.bas[lk];e<T.bas[lk+1];e++){ uint32_t M=0; for(int j=0;j<mg[k];j++) if(T.E[e].mask>>gpos[k][j]&1) M|=1u<<j;
              uint32_t sub=M; for(;;){ AD[sub]+=T.E[e].c; if(sub==0) break; sub=(sub-1)&M; } } }
          for(uint32_t S=1;S<top;S++) VG[k][S]=bitf(AD[S],nb); }
        else for(uint32_t S=1;S<top;S++){ uint64_t M=0; for(int j=0;j<mg[k];j++) if(S>>j&1) M|=1ull<<gpos[k][j]; VG[k][S]=bitf(say_dogrudan(&T,&P,L,k,M),nb); }
        VG[k][0]=0.0; }
      double lb[3]={-INFINITY,-INFINITY,-INFINITY};
      for(uint32_t S=1;S<(1u<<nu);S++){ const int ng=__builtin_popcount(S); if(ng>DMAX) continue; uint32_t part[NGRUP]={0,0,0,0,0}; int g[DMAX], q=0, tt=0, dd=0; double bed=0;
        for(int j=0;j<nu;j++) if(S>>j&1){ part[ug[j]]|=1u<<ub[j]; g[q++]=u[j]; tt|=1<<PTUR[u[j]]; dd|=1<<PDUZEY[u[j]]; bed+=PBEDEL[u[j]]; }
        double b=0; for(int k=0;k<NGRUP;k++) b+=VG[k][part[k]];
        const double v=b-bed-log2(21.0)+lgamma((double)ng+1.0)/log(2.0)-LBEDEL[0][L];
        for(int r=0;r<3;r++) if(__builtin_popcount(tt)>=TM[r] && __builtin_popcount(dd)>=DM[r]){ if(v>lb[r]) lb[r]=v;
          int al=0; if(v>bv_[r]+ETOL) al=1; else if(v>=bv_[r]-ETOL){ if(ng<bn_[r]) al=1; else if(ng==bn_[r]){ if(L<bL_[r]) al=1; else if(L==bL_[r] && liste_kucuk(g,ng,bg_[r],bn_[r])) al=1; } }
          if(al){ bv_[r]=v; bn_[r]=ng; bL_[r]=L; memcpy(bg_[r],g,sizeof(int)*ng); } } }
      for(int r=0;r<3;r++){ if(lb[r]>enb[r]) enb[r]=lb[r]; if(L==d[r].L){ Dmer_tarandi[r]=1; Dmer_enb[r]=lb[r]; } } }
    for(int r=0;r<3;r++){
      if(d[r].L>=0){ if(enb[r]>d[r].amac+1e-6) h1++; if(Dmer_tarandi[r]){ n_esit_mer++; if(fabs(Dmer_enb[r]-d[r].amac)>1e-6) h2++; }
        const double v=amac_dogrudan(&T,&P,d[r].L,d[r].g,d[r].n); if(fabs(v-d[r].amac)>1e-6 || !B_tutar(d[r].g,d[r].n,TM[r],DM[r]) || !kapi_dogrudan(&P,d[r].L)) h4++;
        for(int i=0;i<d[r].n;i++){ const int k=pgrup(d[r].g[i]); const uint64_t m[NGRUP]={P.B[d[r].L],P.Z[d[r].L],P.F[d[r].L],P.W[d[r].L],P.H[d[r].L]}; if(!(m[k]>>(d[r].g[i]-GOFS[k])&1)) h4++; } }
      if(!atlandi){ n_tam++; if((d[r].L<0)!=(enb[r]==-INFINITY)) h3++; else if(d[r].L>=0 && fabs(enb[r]-d[r].amac)>1e-6) h3++;
        if(d[r].L>=0 && (bL_[r]!=d[r].L || bn_[r]!=d[r].n || memcmp(bg_[r],d[r].g,sizeof(int)*d[r].n))) h5++; } }
    // en geniş dil: her mercekte doğrudan puan (bütün sağlanan L* yüklemleri), en iyi mercek (eşitlikte küçük indeks) ve dil
    dilg_t e; dil_genis(&T,&P,&e); double best=-INFINITY; int bl=-1;
    for(int L=0;L<NLENS;L++){ const uint64_t m[NGRUP]={P.B[L]&LMASK[0],P.Z[L]&LMASK[1],P.F[L]&LMASK[2],P.W[L]&LMASK[3],P.H[L]&LMASK[4]}; double b=0;
      for(int k=0;k<NGRUP;k++) if(m[k]) b+=bitf(say_dogrudan(&T,&P,L,k,m[k]),n_baglam(&T,&P,L,k)); const double v=b-LBEDEL[0][L]; if(v>best+ETOL){ best=v; bl=L; } }
    { int g[DMAXG], ng=0; const uint64_t m[NGRUP]={P.B[bl]&LMASK[0],P.Z[bl]&LMASK[1],P.F[bl]&LMASK[2],P.W[bl]&LMASK[3],P.H[bl]&LMASK[4]};
      for(int k=0;k<NGRUP;k++){ uint64_t x=m[k]; while(x){ int b=__builtin_ctzll(x); x&=x-1; g[ng++]=GOFS[k]+b; } }
      if(bl!=e.L || fabs(best-e.puan)>1e-9 || ng!=e.n || memcmp(g,e.g,sizeof(int)*ng)) hg++; } }
  // dondurma öncesi kural: yalnız uyuşmazlık sayıları; kapsam yalnız önceden yazılmış alt sınırlarla evet/hayır olarak basılır
  (void)n_dilsiz; (void)n_atla;
  printf("kaba-kuvvet D_B (%ld hedef olmayan kitap, 3 kol): uyuşmazlık (i) D_B'den iyi alt küme %ld; (ii) D_B merceğinde eşitlik %ld; (iii) tam taramada amaç/dilsizlik %ld; (iv) dilin doğrudan puanı/B/sağlanma %ld; (v) en geniş dil %ld; (vi) tam taramada seçilen dil (liste, mercek) %ld\n",n_kitap,h1,h2,h3,h4,hg,h5);
  printf("  kapsam: taranan mercek ≥ %d: %s; (ii) karşılaştırması ≥ %d: %s; (iii) tam taranan (kitap, kol) ≥ %d: %s\n",KMER,n_mer>=KMER?"evet":"hayır",KKAR,n_esit_mer>=KKAR?"evet":"hayır",KKAR,n_tam>=KKAR?"evet":"hayır");
  return (h1||h2||h3||h4||hg||h5)?1:0; }
