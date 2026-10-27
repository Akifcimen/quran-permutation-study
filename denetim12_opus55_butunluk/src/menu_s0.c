// menu_s0 — SONRADAN S0 duyarlılığının (ek/EK_20261026T172158Z_sifir_toplam_yozlasmasi.md) bölme menüsüne uygulanması (§8.4).
// Her çiftte, o çiftin hücreleriyle toplamı sıfır olan karşılaştırmalar kredi almaz: F2 (iki yarı 0), R0e (s00 = s01 = 0),
// R1e (s10 = s11 = 0), QS (Σq = 0), Qh (bir yarı 0), QH (Σq(H) = 0). Başka hiçbir şey değişmez (kapı, kurallar, öteki yüklemler).
// Ön-kayıtlı menu.h değiştirilmez; menu_degerlendir'in bir kopyasına yalnız sıfır bayrakları eklenmiştir (SIFIRSIZ=1 ile bayraklar
// kapatılınca menu_degerlendir'le eşitliği denetlenir). S0 ≤ C olduğundan menü adayları (tum ≥ Q−1 ya da t2siz ≥ Q−1) yeterlidir.
// Kullanım: ./menu_s0 <kodeks> <cikti.txt> <menü aday dosyaları...>
#include "menu.h"
typedef struct __attribute__((packed)) { uint16_t a[114]; uint8_t tum, t2siz, bir; uint8_t tumC, t2sizC; int16_t tumL, t2sizL; uint64_t sira; } mkayit_t;
static int SIFIR_ACIK=1;
static void menu_s0(const int *a, menu_t *o){
  static mfam_t FF[NPR]; static hfam_t HH[NPR];
  o->tum=o->t2siz=o->bir=-1; o->tumC=o->tumL=o->t2sizC=o->t2sizL=-1;
  for(int f=0;f<NFORM;f++){ const int al=AL[f], be=BE[f], abe=abs(be);
    for(int i=0;i<NPR;i++){ mfam_t *F=&FF[i]; F->k=0; memset(F->sc,0,sizeof F->sc); memset(F->ss,0,sizeof F->ss); F->qmin=1<<30; F->qmax=-(1<<30); F->b0=F->b1=0; F->sa=F->sn=0;
      HH[i].k=0; HH[i].sq=HH[i].sa=HH[i].sn=0; HH[i].b0=HH[i].b1=0; }
    int e0[ND]={0}; long sn0[ND]={0}, sa0[ND]={0}, st0[ND]={0}; long st=0;
    for(int n=1;n<=114;n++){ const int x=a[n-1]; const int t=al*n+be*x, u=al*n-be*x; st+=t;
      int sig=0; for(int d=0;d<ND;d++){ int c=dsin(d,n,x,t); sig|=c<<d; if(!c){ e0[d]++; sn0[d]+=n; sa0[d]+=x; st0[d]+=t; } }
      uint32_t mt=DIVM[t<0?-t:t];
      while(mt){ int i=__builtin_ctz(mt); mt&=mt-1; mfam_t *F=&FF[i]; const int q=(int32_t)((uint32_t)t*INV32[i]);
        F->sc[sig]++; F->ss[sig]+=q; F->qv[F->k++]=q; if(q<F->qmin) F->qmin=q; if(q>F->qmax) F->qmax=q; F->sa+=x; F->sn+=n;
        if(n<=64) F->b0|=1ULL<<(n-1); else F->b1|=1ULL<<(n-65); }
      uint32_t mu=DIVM[u<0?-u:u];
      while(mu){ int i=__builtin_ctz(mu); mu&=mu-1; hfam_t *H=&HH[i]; H->k++; H->sq+=(int32_t)((uint32_t)u*INV32[i]); H->sa+=x; H->sn+=n;
        if(n<=64) H->b0|=1ULL<<(n-1); else H->b1|=1ULL<<(n-65); } }
    uint32_t mbD[ND];
    for(int d=0;d<ND;d++){ uint32_t mb=0; const int e1=114-e0[d]; const long sn1=SN_TOT-sn0[d], sa1=SA_TOT-sa0[d], st1=st-st0[d];
      if(e0[d]==e1 && !b1_sabit(d,f)) mb|=1u<<P_B1;
      if(e0[d]>0 && e1>0 && ((long)al*sn0[d]==(long)abe*sa1 || (long)al*sn1==(long)abe*sa0[d] || st0[d]==st1)) mb|=1u<<P_B2;
      mbD[d]=mb; }
    for(int i=0;i<NPR;i++){ const int p=PR[i]; const int L=f*NPR+i; mfam_t *F=&FF[i]; hfam_t *H=&HH[i];
      uint32_t m0=0; long sq=0; for(int s=0;s<32;s++) sq+=F->ss[s];
      { fam_t tmp; tmp.k=F->k; tmp.qmin=F->qmin; tmp.qmax=F->qmax; memcpy(tmp.qv,F->qv,sizeof(int)*F->k); if(kosu_q(&tmp)) m0|=1u<<P_K2; }
      if(F->k>=1 && sq%p==0) m0|=1u<<P_QS;
      if(F->k==p) m0|=1u<<P_QkF;
      const uint64_t i0=F->b0&H->b0, i1=F->b1&H->b1; const int ni=__builtin_popcountll(i0)+__builtin_popcountll(i1); const int kU=F->k+H->k-ni;
      long saU=F->sa+H->sa, snU=F->sn+H->sn;
      if(ni){ uint64_t w=i0; while(w){ int b=__builtin_ctzll(w); w&=w-1; saU-=a[b]; snU-=b+1; } w=i1; while(w){ int b=__builtin_ctzll(w); w&=w-1; saU-=a[b+64]; snU-=b+65; } }
      if(F->k>=1 && H->k>=1 && ni==0) m0|=1u<<P_W1;
      if(kU==p) m0|=1u<<P_QU; if(H->k==p) m0|=1u<<P_QkH;
      if(kU>=1 && saU%p==0) m0|=1u<<P_Qa; if(kU>=1 && snU%p==0) m0|=1u<<P_Qn; if(H->k>=1 && H->sq%p==0) m0|=1u<<P_QH;
      uint32_t z0=0; if(SIFIR_ACIK){ if(sq==0) z0|=1u<<P_QS; if(H->sq==0) z0|=1u<<P_QH; }        // çiftten bağımsız sıfır bayrakları
      for(int c=0;c<NCIFT;c++){ const int d1=CIFT[c][0], d2=CIFT[c][1];
        int cn[4]={0,0,0,0}; long cs[4]={0,0,0,0};
        for(int s=0;s<32;s++){ if(!F->sc[s]) continue; int ci=((s>>d1)&1)*2+((s>>d2)&1); cn[ci]+=F->sc[s]; cs[ci]+=F->ss[s]; }
        const int c0=cn[0],c1=cn[1],c2=cn[2],c3=cn[3]; const long s0=cs[0],s1=cs[1],s2=cs[2],s3=cs[3];
        const int h0=c0+c1, h1=c2+c3; const long q0=s0+s1, q1=s2+s3; const int dolu=(c0&&c1&&c2&&c3);
        uint32_t m=m0|mbD[d1];
        if(h0==h1 && h0>=1) m|=1u<<P_F1; if(h0>=1 && h1>=1 && q0==q1) m|=1u<<P_F2;
        if(c0==c1 && c0>=1) m|=1u<<P_R0c; if(c0>=1 && c1>=1 && s0==s1) m|=1u<<P_R0e;
        if(c2==c3 && c2>=1) m|=1u<<P_R1c; if(c2>=1 && c3>=1 && s2==s3) m|=1u<<P_R1e;
        if(dolu && kosu4(s0,s1,s2,s3)) m|=1u<<P_K1; if(dolu && sq==4L*p) m|=1u<<P_Qc;
        if(h0>=1 && h1>=1 && q0%p==0 && q1%p==0) m|=1u<<P_Qh;
        uint32_t ex=EXCL[L]&~(1u<<P_B1); m&=~ex;
        if(SIFIR_ACIK){ uint32_t z=z0; if(q0==0 && q1==0) z|=1u<<P_F2; if(s0==0 && s1==0) z|=1u<<P_R0e; if(s2==0 && s3==0) z|=1u<<P_R1e;
          if(q0==0 || q1==0) z|=1u<<P_Qh; m&=~z; }
        int mc=c0; if(c1<mc) mc=c1; if(c2<mc) mc=c2; if(c3<mc) mc=c3;
        if(!(F->k*p>=228 && mc>=2)) m&=~GATED_MASK;
        const int kr=mkredi(m,f,i,yapi(d1,d2,f));
        if(kr>o->tum){ o->tum=kr; o->tumC=c; o->tumL=L; }
        if(d1!=D_t2 && d2!=D_t2 && kr>o->t2siz){ o->t2siz=kr; o->t2sizC=c; o->t2sizL=L; }
        if(d1==D_t2 && d2==D_n2){ if(kr>o->bir) o->bir=kr; } } } } }
int main(int argc,char**argv){ int kod=atoi(argv[1]); butunluk_init(kod); menu_hazirla(); if(getenv("SIFIRSIZ")) SIFIR_ACIK=0;
  FILE *fo=fopen(argv[2],"w"); menu_t Q; menu_s0(BASE,&Q); menu_t Qc; menu_degerlendir(BASE,&Qc,-1,NULL);
  fprintf(fo,"KURAN S0 tum %d (cift %s,%s mercek (%d,%+d,%d)) t2siz %d bir %d | C tum %d t2siz %d bir %d\n",Q.tum,DAD[CIFT[Q.tumC][0]],DAD[CIFT[Q.tumC][1]],
    AL[Q.tumL/NPR],BE[Q.tumL/NPR],PR[Q.tumL%NPR],Q.t2siz,Q.bir,Qc.tum,Qc.t2siz,Qc.bir);
  long esitsiz=0, n=0;
  for(int fi=3;fi<argc;fi++){ FILE *f=fopen(argv[fi],"rb"); if(!f) continue; mkayit_t K;
    while(fread(&K,sizeof K,1,f)==1){ int a[114]; for(int i=0;i<114;i++) a[i]=K.a[i]; menu_t M; menu_s0(a,&M); n++;
      if(!SIFIR_ACIK && (M.tum!=K.tum || M.t2siz!=K.t2siz || M.bir!=K.bir)) esitsiz++;
      fprintf(fo,"%s %llu C %d %d %d S0 %d %d %d cift %s,%s mercek (%d,%+d,%d)\n",argv[fi],(unsigned long long)K.sira,K.tum,K.t2siz,K.bir,M.tum,M.t2siz,M.bir,
        DAD[CIFT[M.tumC][0]],DAD[CIFT[M.tumC][1]],AL[M.tumL/NPR],BE[M.tumL/NPR],PR[M.tumL%NPR]); }
    fclose(f); }
  if(!SIFIR_ACIK) fprintf(fo,"DENETIM (SIFIRSIZ): %ld kayıtta menu_degerlendir ile eşitsiz %ld\n",n,esitsiz);
  fclose(fo); return 0; }
