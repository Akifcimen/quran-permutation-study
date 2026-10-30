// hedef_ayrinti_b — her hedef için kendi kolunun dilinde betimsel ayrıntı (ONKAYIT §9; yalnız raporlama, sonuçları değiştirmez):
// eşik (kontrol_b ile aynı tablo ve işlev), en iyi mercek, o mercekte k_F, k_H, |F ∩ H|, hücre boyları ve q-toplamları (00, 01, 10, 11:
// t-paritesi, n-paritesi), dilin grup başına en-az sayısı c ve bağlam n (c < 10 → çözülmemiş), dilin orada sağlanan yüklemleri ve
// kapsamı (tür/düzey), o mercekteki bütün L* profili. Dilsiz hedefte kapı G mercek sayısı basılır.
// Kullanım: ./hedef_ayrinti_b <hedef.bin> <diller.txt> <cikti.txt> <ref>...
#include "dtab_b.h"
static void grup_sayim(const atablo_t *T, const prof_t *P, const dtab_t *D, int L, uint64_t c[NGRUP], uint64_t n[NGRUP], int var[NGRUP]){
  const int cf=T->cF[L][kkutu(P->kF[L])], ch=T->cH[L][kkutu(P->kH[L])];
  const uint64_t s[NGRUP]={P->B[L]&D->gm[0],P->Z[L]&D->gm[1],P->F[L]&D->gm[2],P->W[L]&D->gm[3],P->H[L]&D->gm[4]};
  for(int k=0;k<NGRUP;k++){ var[k]=s[k]!=0; c[k]=0; n[k]=0; }
  if(s[0]){ c[0]=T->BA[L][s[0]]; n[0]=T->N; } if(s[1]){ c[1]=T->ZA[L][s[1]]; n[1]=T->N; }
  if(s[4]){ c[4]=T->HA[((size_t)L*NCX+ch)*128+s[4]]; n[4]=T->nnH[L][ch]; }
  for(int g=0;g<2;g++){ const int k=g?3:2; if(!s[k]) continue; uint64_t cc=0; for(int kb=0;kb<KC;kb++){ if(T->cF[L][kb]!=cf) continue; const uint32_t lk=LKG(L,kb,g);
      for(uint64_t e=T->bas[lk];e<T->bas[lk+1];e++) if((T->E[e].mask&s[k])==s[k]) cc+=T->E[e].c; } c[k]=cc; n[k]=T->nnF[L][cf]; } }
int main(int argc,char**argv){ if(argc<5) return 1; kutuphane_init(); mercek_bedeli_hazirla(); static atablo_t T; atablo_kur(&T,argv+4,argc-4);
  FILE *f=fopen(argv[1],"rb"); fseek(f,0,SEEK_END); int NH=(int)(ftell(f)/232); fseek(f,0,SEEK_SET); int16_t (*H)[114]=malloc(sizeof(*H)*NH);
  for(int h=0;h<NH;h++){ int32_t id; if(fread(&id,4,1,f)!=1||fread(H[h],2,114,f)!=114) return 2; } fclose(f);
  FILE *fd=fopen(argv[2],"r"), *o=fopen(argv[3],"w"); char ad[64]; int hed, kod, L, n; static prof_t P; static dtab_t D; int GL[NLENS];
  while(fscanf(fd,"%63s %d %d %d %d",ad,&hed,&kod,&L,&n)==5){ int g[DMAX]; for(int i=0;i<n;i++) if(fscanf(fd,"%d",&g[i])!=1) return 2;
    const int kol=kod/10, ons=kod%10; int a[114]; for(int q=0;q<114;q++) a[q]=H[hed][q]; kdegerlendir(a,&P,0x3FFF); const int nGL=kapi_listesi(&P,GL);
    if(kol>=1 && kol<=3 && (L<0||n==0)){ fprintf(o,"%s hedef %d kol %d dil_yok Gmercek %d\n",ad,hed,kol,nGL); continue; }
    dtab_kur(&D,&T,g,n,ons); dek_t E={0}; E.kol=kol; E.tmin=KOL_TMIN[kol]; E.dmin=KOL_DMIN[kol]; int Lb=-1; double S;
    if(kol>=1 && kol<=3){ int bv; S=puan_B(&D,&E,&T,&P,GL,nGL,&Lb,&bv); } else S=dtab_puan(&D,&T,&P,&Lb);
    uint64_t c[NGRUP], nn[NGRUP]; int var[NGRUP]; grup_sayim(&T,&P,&D,Lb,c,nn,var); int coz=1; for(int k=0;k<NGRUP;k++) if(var[k] && c[k]<T_COZ) coz=0;
    const int f_=Lb/NPR, p=PR[Lb%NPR], al=AL[f_], be=BE[f_];
    int cc[4]={0,0,0,0}, kH=0, ni=0; long ss[4]={0,0,0,0};
    for(int m=1;m<=114;m++){ const int x=a[m-1], t=al*m+be*x, u=al*m-be*x; const int inF=(t%p==0), inH=(u%p==0); if(inH) kH++; if(inF&&inH) ni++;
      if(inF){ const int ci=(t&1)*2+(m&1); cc[ci]++; ss[ci]+=t/p; } }
    uint8_t sig=kapsam(&D,&P,Lb);
    fprintf(o,"%s hedef %d kol %d esik %.6f mercek (%d,%+d,%d) Ldil %d cozulmus %d kF %d kH %d FnH %d mc %d kapiG %d hucre_boy %d/%d/%d/%d hucre_q %ld/%ld/%ld/%ld kapsam_tur %d kapsam_duzey %d |",
      ad,hed,kol,S,al,be,p,L,coz,P.kF[Lb],P.kH[Lb],ni,P.mc[Lb],kapiG(&P,Lb),cc[0],cc[1],cc[2],cc[3],ss[0],ss[1],ss[2],ss[3],__builtin_popcount((unsigned)sig>>4),__builtin_popcount(sig&15u));
    const char *GA="BZFWH"; for(int k=0;k<NGRUP;k++) if(var[k]) fprintf(o," %c:%llu/%llu",GA[k],(unsigned long long)c[k],(unsigned long long)nn[k]);
    fprintf(o," | dil:"); for(int i=0;i<n;i++) fprintf(o," %s",PADI[g[i]]);
    fprintf(o," | sağlanan:"); { const uint64_t s[NGRUP]={P.B[Lb],P.Z[Lb],P.F[Lb],P.W[Lb],P.H[Lb]}; for(int k=0;k<NGRUP;k++){ uint64_t x=s[k]&D.gm[k]; while(x){ int b=__builtin_ctzll(x); x&=x-1; fprintf(o," %s",PADI[GOFS[k]+b]); } } }
    fprintf(o," | L*@mercek:"); { const uint64_t s[NGRUP]={P.B[Lb],P.Z[Lb],P.F[Lb],P.W[Lb],P.H[Lb]}; for(int k=0;k<NGRUP;k++){ uint64_t x=s[k]; while(x){ int b=__builtin_ctzll(x); x&=x-1; if(LUYE[GOFS[k]+b]) fprintf(o," %s",PADI[GOFS[k]+b]); } } }
    fprintf(o," | Gmercek %d\n",nGL); dtab_birak(&D); }
  fclose(o); fclose(fd); return 0; }
