// kural_sayim — aile-içi yüklemler (F1,F2,R0c,R0e,R1c,R1e,K1,Qc,QS,Qh) için SOYUT yapılandırma uzayında kapsamlı sayım.
// D1 = t2: t-çift satırdaki (satır 0) q'lar çift, satır 1'deki q'lar tek. Hücre (c, s): c = üye sayısı, s = q toplamı.
// Gerçekleşebilirlik: c = 0 ⇒ s = 0; satır 0: s çift; satır 1: s ≡ c (mod 2); işaret kipi pozitifse satır 0: s ≥ 2c, satır 1: s ≥ c.
// Çıktı: her (p, kip) için maske histogramı → gerektirme adayları python ile. ./kural_sayim <p> <kip:0=pozitif,1=serbest> <cmax> <cikti> [desen]
// desen (isteğe bağlı): 4 karakter, hücre başına parite: 'e' çift q, 'o' tek q, 'x' serbest. Varsayılan "eeoo" (D1 = t2).
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
static int kosu4(long a,long b,long c,long d){ long v[4]={a,b,c,d}; for(int i=1;i<4;i++){ long x=v[i]; int j=i-1; while(j>=0&&v[j]>x){v[j+1]=v[j];j--;} v[j+1]=x; }
  int nd=1; for(int i=1;i<4;i++) if(v[i]!=v[i-1]) nd++; return nd>=2 && v[3]-v[0]+1==nd; }
int main(int argc,char**argv){ int p=atoi(argv[1]), kip=atoi(argv[2]), cmax=atoi(argv[3]); const char *desen=argc>5?argv[5]:"eeoo";
  int smin = kip? -(3*p+6) : 0, smax = 5*p+6;
  static uint64_t H[1024];
  int c[4]; long s[4];
  for(c[0]=0;c[0]<=cmax;c[0]++) for(c[1]=0;c[1]<=cmax;c[1]++) for(c[2]=0;c[2]<=cmax;c[2]++) for(c[3]=0;c[3]<=cmax;c[3]++){
    long lo[4],hi[4]; int st[4];
    for(int i=0;i<4;i++){ if(c[i]==0){ lo[i]=hi[i]=0; st[i]=1; continue; }
      char d=desen[i]; if(d=='e'){ st[i]=2; lo[i]= kip? smin : 2*c[i]; if(((lo[i]%2)+2)%2) lo[i]++; }
      else if(d=='o'){ st[i]=2; lo[i]= kip? smin : c[i]; if(((lo[i]-c[i])%2+2)%2) lo[i]++; }
      else { st[i]=1; lo[i]= kip? smin : c[i]; }
      hi[i]=smax; }
    for(s[0]=lo[0];s[0]<=hi[0];s[0]+=st[0]) for(s[1]=lo[1];s[1]<=hi[1];s[1]+=st[1]) for(s[2]=lo[2];s[2]<=hi[2];s[2]+=st[2]) for(s[3]=lo[3];s[3]<=hi[3];s[3]+=st[3]){
      int h0=c[0]+c[1], h1=c[2]+c[3], k=h0+h1; long q0=s[0]+s[1], q1=s[2]+s[3], sq=q0+q1; int dolu=c[0]&&c[1]&&c[2]&&c[3]; unsigned m=0;
      if(h0==h1 && h0>=1) m|=1;            // F1
      if(h0>=1&&h1>=1&&q0==q1) m|=2;       // F2
      if(c[0]==c[1]&&c[0]>=1) m|=4;        // R0c
      if(c[0]>=1&&c[1]>=1&&s[0]==s[1]) m|=8;   // R0e
      if(c[2]==c[3]&&c[2]>=1) m|=16;       // R1c
      if(c[2]>=1&&c[3]>=1&&s[2]==s[3]) m|=32;  // R1e
      if(dolu && kosu4(s[0],s[1],s[2],s[3])) m|=64;  // K1
      if(dolu && sq==4L*p) m|=128;         // Qc
      if(k>=1 && sq%p==0) m|=256;          // QS
      if(h0>=1&&h1>=1&&q0%p==0&&q1%p==0) m|=512; // Qh
      H[m]++; } }
  FILE *fo=fopen(argv[4],"wb"); fwrite(H,8,1024,fo); fclose(fo); return 0; }
