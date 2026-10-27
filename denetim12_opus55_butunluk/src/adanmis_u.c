// adanmis_u — tekdüze sıfırda adanmış kestirim (ONKAYIT §4.3): P(en az M | k_F = K) aile-içi maskeler için, KESİN koşullu
// önem örneklemesiyle. Etiketli permütasyon düzgün ⇔ aile yapısı γ ve tamamlama τ; P(γ) ∝ Φ(k), Φ = yeni aile üyesi doğurmayan
// tamamlama sayısı (ayrık blok yasak tahtasının kale polinomu; 10. denetim kosul_u.h'nin herhangi bir merceğe genellemesi).
// Öneri: K sabit, γ düzgün (sınıf-sayı vektörü ∝ Π w_r, sınıf içinde düzgün seçim) → ağırlık ∝ Φ(k)/M!. MCMC yok.
// İsteğe bağlı iki-el (W) maskesi: γ'ye ek olarak τ, geçerli tamamlamalar arasından düzgün (ret) çekilir.
// Kullanım: ./adanmis_u <kodeks> <bicim> <p> <K> <oneri_sayisi> <tohum> <F|W> <maske1> [maske2 ...]   (maske ondalık; F: 11 bit, W: 6 bit)
// Sınır: p ≥ 11 (Φ'nin değişken işaretli toplamı çift duyarlıkta kararlı; p < 11 için ayrı yöntem gerekir).
#include "butunluk.h"
#define MAXP 113
static int al,be,p,K,M; static int PC[MAXP][120], ps[MAXP], VC[MAXP][120], vs[MAXP];
static double W[MAXP][120];          // w_r(k) = C(ps,k) C(vs,k) k!  (log ölçekli değil; küçük)
static double D[MAXP+1][120];
static double lbin(int n,int k){ return lgamma(n+1.0)-lgamma(k+1.0)-lgamma(n-k+1.0); }
static double phi_oran(const int *k){ // Φ(k)/M! = Σ_j (−1)^j R_j / (M)_j
  double P[130]; int deg=0; P[0]=1;
  for(int r=0;r<p;r++){ int a=ps[r]-k[r], b=vs[r]-k[r]; int mn=a<b?a:b; if(mn<=0) continue;
    double Rr[120]; for(int i=0;i<=mn;i++) Rr[i]=exp(lbin(a,i)+lbin(b,i)+lgamma(i+1.0));
    double Q[130]; for(int j=0;j<=deg+mn;j++) Q[j]=0; for(int x=0;x<=deg;x++) for(int y=0;y<=mn;y++) Q[x+y]+=P[x]*Rr[y]; deg+=mn; for(int j=0;j<=deg;j++) P[j]=Q[j]; }
  double s=0, ff=1; for(int j=0;j<=deg;j++){ if(j>0) ff*=(M-j+1); s+=((j&1)?-1.0:1.0)*P[j]/ff; } return s; }
int main(int argc,char**argv){
  if(argc<9){ fprintf(stderr,"kullanım\n"); return 1; }
  int kod=atoi(argv[1]); int f=atoi(argv[2]); p=atoi(argv[3]); K=atoi(argv[4]); long NO=atol(argv[5]); uint64_t seed=strtoull(argv[6],0,10); char grp=argv[7][0];
  int nm=argc-8; uint32_t hedef[64]; for(int i=0;i<nm;i++) hedef[i]=(uint32_t)atoi(argv[8+i]);
  butunluk_init(kod); al=AL[f]; be=BE[f]; M=114-K; int pi=-1; for(int i=0;i<NPR;i++) if(PR[i]==p) pi=i;
  if(p<11 || (abs(be)%p==0) || (al%p==0)){ fprintf(stderr,"p < 11 ya da p | αβ: bu kestirici kapsamıyor\n"); return 3; }
  int binv=1; { int bb=((be%p)+p)%p; while((bb*binv)%p!=1) binv++; }
  for(int r=0;r<p;r++){ ps[r]=vs[r]=0; }
  for(int n=1;n<=114;n++){ int r=((-al*binv*n)%p+p)%p; PC[r][ps[r]++]=n; }     // n ∈ F ⇔ a ≡ r_F(n)
  for(int j=0;j<114;j++){ int r=BASE[j]%p; VC[r][vs[r]++]=j; }
  for(int r=0;r<p;r++){ int mx=ps[r]<vs[r]?ps[r]:vs[r]; for(int k=0;k<120;k++) W[r][k]=(k<=mx)?exp(lbin(ps[r],k)+lbin(vs[r],k)+lgamma(k+1.0)):0; }
  for(int s=0;s<120;s++) D[p][s]=(s==0); for(int r=p-1;r>=0;r--) for(int s=0;s<120;s++){ double a=0; for(int k=0;k<=s;k++) a+=W[r][k]*D[r+1][s-k]; D[r][s]=a; }
  if(D[0][K]<=0){ fprintf(stderr,"K olanaksız\n"); return 3; }
  rng_t rg; rseed(&rg,seed); double tw=0, tw2=0; double hw[64]={0}; long hn[64]={0}; long nkab=0, tau_den=0;
  int kv[MAXP]; int pos[120], lab[120];
  for(long it=0;it<NO;it++){
    int s=K; for(int r=0;r<p;r++){ double tot=D[r][s], u=unif(&rg)*tot, acc=0; int x=-1, last=0; for(int y=0;y<=s&&y<120;y++){ double wy=W[r][y]*D[r+1][s-y]; if(wy>0) last=y; acc+=wy; if(x<0&&u<acc) x=y; } if(x<0) x=last; kv[r]=x; s-=x; }
    double w=phi_oran(kv); if(!(w>0)) continue;
    // γ: sınıf içinde düzgün konum ve etiket seçimi
    int m=0; for(int r=0;r<p;r++){ int kr=kv[r]; if(!kr) continue; int pp[120], vv[120]; memcpy(pp,PC[r],sizeof(int)*ps[r]); memcpy(vv,VC[r],sizeof(int)*vs[r]);
      for(int j=0;j<kr;j++){ int x=j+(int)bounded(&rg,(uint32_t)(ps[r]-j)); int t=pp[j]; pp[j]=pp[x]; pp[x]=t; int y=j+(int)bounded(&rg,(uint32_t)(vs[r]-j)); t=vv[j]; vv[j]=vv[y]; vv[y]=t; pos[m]=pp[j]; lab[m]=vv[j]; m++; } }
    int a[114]; uint32_t mask;
    if(grp=='F'){ // aile-içi maske yalnız γ'den: hücreler ve katsayılar
      int c[4]={0,0,0,0}; long sc[4]={0,0,0,0}; int qmin=1<<30,qmax=-(1<<30); int qv[120];
      for(int i=0;i<m;i++){ int n=pos[i], x=BASE[lab[i]]; int t=al*n+be*x; int q=t/p; int ci=(t&1)*2+(n&1); c[ci]++; sc[ci]+=q; qv[i]=q; if(q<qmin) qmin=q; if(q>qmax) qmax=q; }
      uint32_t mm=0; int h0=c[0]+c[1], h1=c[2]+c[3]; long q0=sc[0]+sc[1], q1=sc[2]+sc[3], sq=q0+q1; int dolu=c[0]&&c[1]&&c[2]&&c[3];
      if(h0==h1&&h0>=1) mm|=1u<<0; if(h0>=1&&h1>=1&&q0==q1) mm|=1u<<1; if(c[0]==c[1]&&c[0]>=1) mm|=1u<<2; if(c[0]>=1&&c[1]>=1&&sc[0]==sc[1]) mm|=1u<<3;
      if(c[2]==c[3]&&c[2]>=1) mm|=1u<<4; if(c[2]>=1&&c[3]>=1&&sc[2]==sc[3]) mm|=1u<<5; if(dolu&&kosu4(sc[0],sc[1],sc[2],sc[3])) mm|=1u<<6;
      { fam_t F; F.k=m; F.qmin=qmin; F.qmax=qmax; memcpy(F.qv,qv,sizeof(int)*m); if(kosu_q(&F)) mm|=1u<<7; }
      if(dolu&&sq==4L*p) mm|=1u<<8; if(m>=1&&sq%p==0) mm|=1u<<9; if(h0>=1&&h1>=1&&q0%p==0&&q1%p==0) mm|=1u<<10;
      mask=mm;
    } else { // τ: kalan etiketleri kalan konumlara yeni üye doğurmadan düzgün dağıt (ret, erken kesme), sonra tam profil
      int up[115]={0}, ul[114]={0}; for(int i=0;i<m;i++){ up[pos[i]]=1; ul[lab[i]]=1; a[pos[i]-1]=BASE[lab[i]]; }
      int P_[114], V_[114], np=0, nv=0; for(int n=1;n<=114;n++) if(!up[n]) P_[np++]=n; for(int j=0;j<114;j++) if(!ul[j]) V_[nv++]=BASE[j];
      for(;;){ tau_den++; int ok=1; for(int i=0;i<nv;i++){ int j=i+(int)bounded(&rg,(uint32_t)(nv-i)); int t=V_[i]; V_[i]=V_[j]; V_[j]=t; if((al*P_[i]+be*V_[i])%p==0){ ok=0; break; } } if(ok) break; }
      for(int i=0;i<np;i++) a[P_[i]-1]=V_[i];
      static profil_t P; degerlendir(a,&P); mask=wmask6(P.m[f*NPR+pi]);
      if(P.kF[f*NPR+pi]!=K){ fprintf(stderr,"HATA: kF %d != K %d\n",P.kF[f*NPR+pi],K); return 4; } }
    nkab++; tw+=w; tw2+=w*w; for(int i=0;i<nm;i++) if((mask&hedef[i])==hedef[i]){ hw[i]+=w; hn[i]++; } }
  double ess=tw*tw/tw2; printf("adanmis_u kodeks %d bicim (%d,%+d) p %d K %d grup %c: oneri %ld, ESS %.0f, tau_deneme %ld\n",kod,al,be,p,K,grp,nkab,ess,tau_den);
  for(int i=0;i<nm;i++){ double ph=hw[i]/tw; double se=sqrt(ph*(1-ph)/ess); printf("  maske %u: isabet %ld, P^ = %.6g (yaklaşık SE %.2g; ESS tabanlı)\n",hedef[i],hn[i],ph,se); }
  return 0; }
