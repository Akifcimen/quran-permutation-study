// olay_kur — profil çalışması (ONKAYIT §3): her hedefin üç olayını kurar ve olayların model bilgisini (bit) hesaplar.
//   O1: hedefin Kol A dili Λ_A, hedefin Kol A merceği L_A'da, hepsi birden (kısmi puan yok).
//   O2: hedefin L_A'daki bütün profili Π(L_A): kütüphanenin o mercekte doğru olan bütün yüklemleri (dışlamalardan sonra).
//   O3: hedefin yalnız p = 19 mercekleri arasından seçilmiş Kol A merceği L_19'daki bütün profili Π(L_19).
// Bir kontrol bir olayı, olayın merceğinde kapı G tutuyor ve olayın her yüklemi o mercekte doğruysa taşır (kart_ana kip 2).
// Model bilgisi S (betimsel; ONKAYIT §5): olayın grup maskelerinin referans tablolarındaki üst-küme sayımlarından, dtab.h ile aynı bağlam
// ve bitf kuralıyla, gruplar toplanarak; S = Σ_g bitf(c_g, n_g) − c(L). O1 için S, Kol A eşiğinin (DESIK) aynısıdır (sınama).
// Kullanım: ./olay_kur <hedef.bin> <diller_A.txt> <diller_A19.txt> <cikti_olaylar.txt> <ref>...
// Çıktı satırı: <ad>.<tür> <hedef_no> <L> <mB> <mZ> <mF> <mW> <mH> <S> <bilgi> <yüklem_sayısı>   (maskeler onaltılı; S = bilgi − c(L))
#include "dtab_b.h"
static atablo_t T; static int16_t (*HED)[114]; static int NH;
static uint64_t ust_FW(int L, int cf, int g, uint64_t M){ uint64_t c=0;   // (L, bağlam cf, grup g) içinde maskesi M'yi kapsayan referans sayısı
  for(int kb=0;kb<KC;kb++){ if(T.cF[L][kb]!=cf) continue; const uint32_t lk=LKG(L,kb,g); for(uint64_t e=T.bas[lk];e<T.bas[lk+1];e++) if((T.E[e].mask&M)==M) c+=T.E[e].c; }
  return c; }
static double bilgi(const prof_t *P, int L, const uint64_t m[5]){
  const int cf=T.cF[L][kkutu(P->kF[L])], ch=T.cH[L][kkutu(P->kH[L])]; double b=0;   // dtab_mercek ile aynı toplama sırası (B, Z, F, W, H)
  if(m[0]) b+=(float)bitf(T.BA[L][m[0]],T.N);
  if(m[1]) b+=(float)bitf(T.ZA[L][m[1]],T.N);
  if(m[2]) b+=(float)bitf(ust_FW(L,cf,0,m[2]),T.nnF[L][cf]);
  if(m[3]) b+=(float)bitf(ust_FW(L,cf,1,m[3]),T.nnF[L][cf]);
  if(m[4]) b+=(float)bitf(T.HA[((size_t)L*NCX+ch)*128+m[4]],T.nnH[L][ch]);
  return b; }   // bilgi (bit); S = bilgi − c(L) çağıranda
static int satir_oku(FILE *f, char *ad, int *hed, int *L, int *g, int *n){ int kod; if(fscanf(f,"%63s %d %d %d %d",ad,hed,&kod,L,n)!=5) return 0;
  for(int i=0;i<*n;i++) if(fscanf(f,"%d",&g[i])!=1) return -1; return 1; }
int main(int argc,char**argv){
  if(argc<6){ fprintf(stderr,"kullanım\n"); return 1; }
  kutuphane_init(); mercek_bedeli_hazirla(); atablo_kur(&T,argv+5,argc-5);
  { FILE *f=fopen(argv[1],"rb"); if(!f){ perror(argv[1]); return 2; } fseek(f,0,SEEK_END); long sz=ftell(f); fseek(f,0,SEEK_SET); NH=(int)(sz/232); HED=malloc(sizeof(*HED)*NH);
    for(int h=0;h<NH;h++){ int32_t id; if(fread(&id,4,1,f)!=1||fread(HED[h],2,114,f)!=114||id!=h){ fprintf(stderr,"hedef\n"); return 2; } } fclose(f); }
  FILE *A=fopen(argv[2],"r"), *A19=fopen(argv[3],"r"), *o=fopen(argv[4],"w"); if(!A||!A19||!o){ fprintf(stderr,"dosya\n"); return 2; }
  static prof_t P; char ad[64], ad19[64]; int hed, L, n, g[DMAX], hed19, L19, n19, g19[DMAX]; long say[4]={0,0,0,0};
  for(;;){ const int r1=satir_oku(A,ad,&hed,&L,g,&n), r2=satir_oku(A19,ad19,&hed19,&L19,g19,&n19);
    if(r1==0 && r2==0) break; if(r1!=1 || r2!=1 || hed!=hed19 || strcmp(ad,ad19)){ fprintf(stderr,"diller dosyaları uyuşmuyor (%s / %s)\n",ad,ad19); return 2; }
    int a[114]; for(int q=0;q<114;q++) a[q]=HED[hed][q]; kdegerlendir(a,&P,0x3FFF); say[0]++;
    if(L>=0 && n>0){ uint64_t m[5]={0,0,0,0,0}; for(int i=0;i<n;i++){ const int k=pgrup(g[i]); m[k]|=1ull<<(g[i]-GOFS[k]); }
      const uint64_t pm[5]={P.B[L],P.Z[L],P.F[L],P.W[L],P.H[L]};
      for(int k=0;k<5;k++) if((pm[k]&m[k])!=m[k]){ fprintf(stderr,"%s: dil merceğinde tutmuyor\n",ad); return 2; }
      if(!kapiG(&P,L)){ fprintf(stderr,"%s: kapı tutmuyor\n",ad); return 2; }
      fprintf(o,"%s.1 %d %d %llx %llx %llx %llx %llx %.9f %.9f %d\n",ad,hed,L,(unsigned long long)m[0],(unsigned long long)m[1],(unsigned long long)m[2],(unsigned long long)m[3],(unsigned long long)m[4],bilgi(&P,L,m)-LBEDEL[0][L],bilgi(&P,L,m),n);
      int np=0; for(int k=0;k<5;k++) np+=__builtin_popcountll(pm[k]);
      fprintf(o,"%s.2 %d %d %llx %llx %llx %llx %llx %.9f %.9f %d\n",ad,hed,L,(unsigned long long)pm[0],(unsigned long long)pm[1],(unsigned long long)pm[2],(unsigned long long)pm[3],(unsigned long long)pm[4],bilgi(&P,L,pm)-LBEDEL[0][L],bilgi(&P,L,pm),np);
      say[1]++; say[2]++; }
    if(L19>=0 && n19>0){ if(L19%NPR!=6){ fprintf(stderr,"%s: L19 asalı 19 değil\n",ad); return 2; } if(!kapiG(&P,L19)){ fprintf(stderr,"%s: kapı (19)\n",ad); return 2; }
      const uint64_t pm[5]={P.B[L19],P.Z[L19],P.F[L19],P.W[L19],P.H[L19]}; int np=0; for(int k=0;k<5;k++) np+=__builtin_popcountll(pm[k]);
      fprintf(o,"%s.3 %d %d %llx %llx %llx %llx %llx %.9f %.9f %d\n",ad,hed,L19,(unsigned long long)pm[0],(unsigned long long)pm[1],(unsigned long long)pm[2],(unsigned long long)pm[3],(unsigned long long)pm[4],bilgi(&P,L19,pm)-LBEDEL[0][L19],bilgi(&P,L19,pm),np);
      say[3]++; } }
  fclose(A); fclose(A19); fclose(o); fprintf(stderr,"hedef %ld; O1 %ld, O2 %ld, O3 %ld olay\n",say[0],say[1],say[2],say[3]); return 0; }
