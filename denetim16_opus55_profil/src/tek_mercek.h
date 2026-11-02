// tek_mercek.h — tek_uret.py ile kutuphane.h kdegerlendir'den üretildi; elle düzenlenmez.
#pragma once
static void kdeg_tek(const int *a, prof_t *P, int L0){
  static __thread kfam_t FF[NPR]; static __thread khfam_t HH[NPR]; const int i0=L0%NPR;
  for(int f=L0/NPR;f<=L0/NPR;f++){
    const int al=AL[f], be=BE[f], abe=abs(be);
    for(int i=i0;i<=i0;i++){ kfam_t *F=&FF[i]; F->k=0; for(int c=0;c<4;c++){ F->c[c]=0; F->s[c]=0; F->eq[c]=1; F->qf[c]=0; }
      F->qmin=1<<30; F->qmax=-(1<<30); F->b0=F->b1=0; F->sa=F->sn=0;
      khfam_t *H=&HH[i]; H->k=0; for(int c=0;c<4;c++){ H->c[c]=0; H->r[c]=0; } H->rmin=1<<30; H->rmax=-(1<<30); H->b0=H->b1=0; H->sa=H->sn=H->st=H->sr=0; }
    int e0=0; long sn0=0, sa0=0, st0=0, st=0;
    for(int n=1;n<=114;n++){ const int x=a[n-1]; const int t=al*n+be*x, u=al*n-be*x;
      const int d1=t&1, d2=n&1; st+=t; if(!d1){ e0++; sn0+=n; sa0+=x; st0+=t; }
      const int ci=d1*2+d2;
      uint32_t mt=DIVM[t<0?-t:t]&(1u<<i0);
      while(mt){ int i=__builtin_ctz(mt); mt&=mt-1; kfam_t *F=&FF[i]; const int q=(int32_t)((uint32_t)t*INV32[i]);
        if(F->c[ci]==0) F->qf[ci]=q; else if(q!=F->qf[ci]) F->eq[ci]=0;
        F->c[ci]++; F->s[ci]+=q; F->qv[F->k++]=q; if(q<F->qmin) F->qmin=q; if(q>F->qmax) F->qmax=q; F->sa+=x; F->sn+=n;
        if(n<=64) F->b0|=1ULL<<(n-1); else F->b1|=1ULL<<(n-65); }
      uint32_t mu=DIVM[u<0?-u:u]&(1u<<i0);
      while(mu){ int i=__builtin_ctz(mu); mu&=mu-1; khfam_t *H=&HH[i]; const int r=(int32_t)((uint32_t)u*INV32[i]);
        H->c[ci]++; H->r[ci]+=r; H->rv[H->k++]=r; if(r<H->rmin) H->rmin=r; if(r>H->rmax) H->rmax=r; H->sa+=x; H->sn+=n; H->st+=t; H->sr+=r;
        if(n<=64) H->b0|=1ULL<<(n-1); else H->b1|=1ULL<<(n-65); } }
    // kitap düzeyi
    const int e1=114-e0; const long sn1=SN_TOT-sn0, sa1=SA_TOT-sa0, st1=st-st0;
    uint8_t mb0=0;
    if(e0==e1) mb0|=1u<<B_B1;
    if(e0>0 && e1>0 && ((long)al*sn0==(long)abe*sa1 || (long)al*sn1==(long)abe*sa0 || st0==st1)) mb0|=1u<<B_B2;
    if(e0>0 && e1>0 && sa0==sa1) mb0|=1u<<B_Ba;
    if(e0>0 && e1>0 && st0==st1) mb0|=1u<<B_Bt;
    for(int i=i0;i<=i0;i++){ const int p=PR[i]; const long pp=(long)p*p; const int L=f*NPR+i; kfam_t *F=&FF[i]; khfam_t *H=&HH[i];
      uint8_t mb=mb0;
      if(e0>0 && e1>0){ if(sn0%p==0 && sn1%p==0) mb|=1u<<B_BMn; if(st0%p==0 && st1%p==0) mb|=1u<<B_BMt; if(st0%pp==0 && st1%pp==0) mb|=1u<<B_BMt2; }
      const int c0=F->c[0],c1=F->c[1],c2=F->c[2],c3=F->c[3]; const long s0=F->s[0],s1=F->s[1],s2=F->s[2],s3=F->s[3];
      const int h0=c0+c1, h1=c2+c3, k=F->k; const long q0=s0+s1, q1=s2+s3, sq=q0+q1; const int dolu=(c0&&c1&&c2&&c3);
      const int k0=c0+c2, k1=c1+c3; const long cs0=s0+s2, cs1=s1+s3;          // sütunlar (n çift / tek)
      const int g0=c0+c3, g1=c1+c2; const long ds0=s0+s3, ds1=s1+s2;          // köşegenler (00∪11 / 01∪10)
      uint64_t m=0;
      if(h0==h1 && h0>=1) m|=1ull<<F_F1;
      if(h0>=1 && h1>=1 && q0==q1) m|=1ull<<F_F2;
      if(c0==c1 && c0>=1) m|=1ull<<F_R0c;
      if(c0>=1 && c1>=1 && s0==s1) m|=1ull<<F_R0e;
      if(c2==c3 && c2>=1) m|=1ull<<F_R1c;
      if(c2>=1 && c3>=1 && s2==s3) m|=1ull<<F_R1e;
      if(dolu && kosu4(s0,s1,s2,s3)) m|=1ull<<F_K1;
      if(bant(F->qv,k,F->qmin,F->qmax)) m|=1ull<<F_K2;
      if(dolu && sq==4L*p) m|=1ull<<F_Qc;
      if(k>=1 && sq%p==0) m|=1ull<<F_QS;
      if(h0>=1 && h1>=1 && q0%p==0 && q1%p==0) m|=1ull<<F_Qh;
      if(k0==k1 && k0>=1) m|=1ull<<F_FC1;
      if(k0>=1 && k1>=1 && cs0==cs1) m|=1ull<<F_FC2;
      if(g0==g1 && g0>=1) m|=1ull<<F_FD1;
      if(g0>=1 && g1>=1 && ds0==ds1) m|=1ull<<F_FD2;
      if(k>=1){ if(sq==p) m|=1ull<<F_FSp; if(sq==2L*p) m|=1ull<<F_FS2p; if(sq%pp==0) m|=1ull<<F_FSd2;
        if(F->sa==p) m|=1ull<<F_FAp; if(F->sa==2L*p) m|=1ull<<F_FA2p; if(F->sa%p==0) m|=1ull<<F_FAd; if(F->sa%pp==0) m|=1ull<<F_FAd2;
        if(F->sn==p) m|=1ull<<F_FNp; if(F->sn==2L*p) m|=1ull<<F_FN2p; if(F->sn%p==0) m|=1ull<<F_FNd; if(F->sn%pp==0) m|=1ull<<F_FNd2; }
      if(h0>=1 && h1>=1){
        if(q0==p && q1==p) m|=1ull<<F_FHp; if(q0==2L*p && q1==2L*p) m|=1ull<<F_FH2p; if(sq==2L*p) m|=1ull<<F_FHm;
        if(q0%pp==0 && q1%pp==0) m|=1ull<<F_FHd2;
        if(h0==p && h1==p) m|=1ull<<F_FHSp; if(h0==2*p && h1==2*p) m|=1ull<<F_FHS2p; if(h0+h1==2*p) m|=1ull<<F_FHSm;
        if(h0%p==0 && h1%p==0) m|=1ull<<F_FHSd; if(h0%pp==0 && h1%pp==0) m|=1ull<<F_FHSd2;
        if(labs(q0-q1)==1) m|=1ull<<F_FRK; }
      if(k0>=1 && k1>=1){
        if(cs0==p && cs1==p) m|=1ull<<F_FCp; if(cs0==2L*p && cs1==2L*p) m|=1ull<<F_FC2p; if(cs0+cs1==2L*p) m|=1ull<<F_FCm;
        if(cs0%p==0 && cs1%p==0) m|=1ull<<F_FCd; if(cs0%pp==0 && cs1%pp==0) m|=1ull<<F_FCd2;
        if(labs(cs0-cs1)==1) m|=1ull<<F_FCK; }
      if(dolu){
        if(s0==p && s1==p && s2==p && s3==p) m|=1ull<<F_FKp; if(s0==2L*p && s1==2L*p && s2==2L*p && s3==2L*p) m|=1ull<<F_FK2p;
        if(s0%p==0 && s1%p==0 && s2%p==0 && s3%p==0) m|=1ull<<F_FKd; if(s0%pp==0 && s1%pp==0 && s2%pp==0 && s3%pp==0) m|=1ull<<F_FKd2;
        if(c0==p && c1==p && c2==p && c3==p) m|=1ull<<F_FKSp; if(c0==2*p && c1==2*p && c2==2*p && c3==2*p) m|=1ull<<F_FKS2p;
        if(c0+c1+c2+c3==4*p) m|=1ull<<F_FKSm;
        if(c0%p==0 && c1%p==0 && c2%p==0 && c3%p==0) m|=1ull<<F_FKSd; if(c0%pp==0 && c1%pp==0 && c2%pp==0 && c3%pp==0) m|=1ull<<F_FKSd2;
        if(ardisik_dizi4(s0,s1,s2,s3)) m|=1ull<<F_FAP;
        if(kosu4(c0,c1,c2,c3)) m|=1ull<<F_FKS; }
      if((c0>=2&&F->eq[0]) || (c1>=2&&F->eq[1]) || (c2>=2&&F->eq[2]) || (c3>=2&&F->eq[3])) m|=1ull<<F_FSC;
      uint8_t mz=0;
      if(k==p) mz|=1u<<Z_QkF; if(k==2*p) mz|=1u<<Z_k2p; if(k>=1 && k%p==0) mz|=1u<<Z_kd; if(k>=1 && k%pp==0) mz|=1u<<Z_kd2;
      // iki el, birleşim
      const uint64_t i0=F->b0&H->b0, i1=F->b1&H->b1; const int ni=__builtin_popcountll(i0)+__builtin_popcountll(i1);
      const int kH=H->k, kU=k+kH-ni;
      long saU=F->sa+H->sa, snU=F->sn+H->sn, stU=(long)p*sq+H->st;
      if(ni){ uint64_t w=i0; while(w){ int b=__builtin_ctzll(w); w&=w-1; const int nn=b+1, x=a[b]; saU-=x; snU-=nn; stU-=al*nn+be*x; }
              w=i1; while(w){ int b=__builtin_ctzll(w); w&=w-1; const int nn=b+65, x=a[b+64]; saU-=x; snU-=nn; stU-=al*nn+be*x; } }
      uint64_t mw=0;
      if(k>=1 && kH>=1 && ni==0) mw|=1ull<<W_W1;
      if(kU==p) mw|=1ull<<W_QU;
      if(kH==p) mw|=1ull<<W_QkH;
      if(kU>=1 && saU%p==0) mw|=1ull<<W_Qa;
      if(kU>=1 && snU%p==0) mw|=1ull<<W_Qn;
      if(kH>=1 && H->sr%p==0) mw|=1ull<<W_QH;
      if(ni==1) mw|=1ull<<W_WI1;
      if(k>=1 && kH>=1 && k==kH) mw|=1ull<<W_WEq;
      if(k>=1 && kH>=1 && k+kH==p) mw|=1ull<<W_WSum;
      if(kU==2*p) mw|=1ull<<W_WU2p; if(kU>=1 && kU%p==0) mw|=1ull<<W_WUd; if(kU>=1 && kU%pp==0) mw|=1ull<<W_WUd2;
      if(kH==2*p) mw|=1ull<<W_WH2p; if(kH>=1 && kH%p==0) mw|=1ull<<W_WHd; if(kH>=1 && kH%pp==0) mw|=1ull<<W_WHd2;
      if(ni==p) mw|=1ull<<W_WIp; if(ni==2*p) mw|=1ull<<W_WI2p; if(ni>=1 && ni%p==0) mw|=1ull<<W_WId; if(ni>=1 && ni%pp==0) mw|=1ull<<W_WId2;
      if(kU>=1){ if(saU==p) mw|=1ull<<W_WUAp; if(saU==2L*p) mw|=1ull<<W_WUA2p; if(saU%pp==0) mw|=1ull<<W_WUAd2;
        if(snU==p) mw|=1ull<<W_WUNp; if(snU==2L*p) mw|=1ull<<W_WUN2p; if(snU%pp==0) mw|=1ull<<W_WUNd2;
        if(stU==p) mw|=1ull<<W_WUTp; if(stU==2L*p) mw|=1ull<<W_WUT2p; if(stU%p==0) mw|=1ull<<W_WUTd; if(stU%pp==0) mw|=1ull<<W_WUTd2; }
      if(kH>=1){ if(H->sa==p) mw|=1ull<<W_WHAp; if(H->sa==2L*p) mw|=1ull<<W_WHA2p; if(H->sa%p==0) mw|=1ull<<W_WHAd; if(H->sa%pp==0) mw|=1ull<<W_WHAd2;
        if(H->sn==p) mw|=1ull<<W_WHNp; if(H->sn==2L*p) mw|=1ull<<W_WHN2p; if(H->sn%p==0) mw|=1ull<<W_WHNd; if(H->sn%pp==0) mw|=1ull<<W_WHNd2;
        if(H->sr==p) mw|=1ull<<W_WHRp; if(H->sr==2L*p) mw|=1ull<<W_WHR2p; if(H->sr%pp==0) mw|=1ull<<W_WHRd2; }
      // karşı aile içi
      const int hc0=H->c[0],hc1=H->c[1],hc2=H->c[2],hc3=H->c[3]; const long r0=H->r[0],r1=H->r[1],r2=H->r[2],r3=H->r[3];
      const int hh0=hc0+hc1, hh1=hc2+hc3; uint8_t mh=0;
      if(hh0==hh1 && hh0>=1) mh|=1u<<H_HH1;
      if(hh0>=1 && hh1>=1 && r0+r1==r2+r3) mh|=1u<<H_HH2;
      if(hc0==hc1 && hc0>=1) mh|=1u<<H_HR0c;
      if(hc0>=1 && hc1>=1 && r0==r1) mh|=1u<<H_HR0e;
      if(hc2==hc3 && hc2>=1) mh|=1u<<H_HR1c;
      if(hc2>=1 && hc3>=1 && r2==r3) mh|=1u<<H_HR1e;
      if(bant(H->rv,kH,H->rmin,H->rmax)) mh|=1u<<H_HK2;
      P->B[L]=mb&~EXB[L]; P->Z[L]=mz&~EXZ[L]; P->F[L]=m&~EXF[L]; P->W[L]=mw&~EXW[L]; P->H[L]=mh&~EXH[L];
      P->kF[L]=(uint8_t)k; P->kH[L]=(uint8_t)kH;
      int mc=c0; if(c1<mc) mc=c1; if(c2<mc) mc=c2; if(c3<mc) mc=c3; P->mc[L]=(uint8_t)mc; }
  }
}
