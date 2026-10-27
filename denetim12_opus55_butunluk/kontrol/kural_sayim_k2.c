// kural_sayim_k2 — K2 (katsayı bandı) dahil 11 aile-içi yüklem için açık katsayılı kapsamlı sayım.
// Hücre başına 0..cmax üye; satır 0 çift q, satır 1 tek q; q ∈ [qlo, qhi]. Çıktı: 2048-bin maske histogramı.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
static int p, qlo, qhi, cmax; static uint64_t H[2048];
static int vals[2][64], nv[2];
static int ms[4][200][8], mc[4][200], nm[4];   // hücre başına çoklu kümeler
static void ms_uret(int cell){ int row=cell/2; int idx[8]; nm[cell]=0;
  for(int c=0;c<=cmax;c++){ // c elemanlı azalmayan diziler
    if(c==0){ mc[cell][nm[cell]]=0; nm[cell]++; continue; }
    for(int i=0;i<c;i++) idx[i]=0;
    for(;;){ int k=nm[cell]; for(int i=0;i<c;i++) ms[cell][k][i]=vals[row][idx[i]]; mc[cell][k]=c; nm[cell]++;
      int j=c-1; while(j>=0 && idx[j]==nv[row]-1) j--; if(j<0) break; idx[j]++; for(int i=j+1;i<c;i++) idx[i]=idx[j]; } } }
static int kosu4(long a,long b,long c,long d){ long v[4]={a,b,c,d}; for(int i=1;i<4;i++){ long x=v[i]; int j=i-1; while(j>=0&&v[j]>x){v[j+1]=v[j];j--;} v[j+1]=x; }
  int nd=1; for(int i=1;i<4;i++) if(v[i]!=v[i-1]) nd++; return nd>=2 && v[3]-v[0]+1==nd; }
int main(int argc,char**argv){ p=atoi(argv[1]); qlo=atoi(argv[2]); qhi=atoi(argv[3]); cmax=atoi(argv[4]);
  nv[0]=nv[1]=0; for(int q=qlo;q<=qhi;q++){ int r=((q%2)+2)%2; vals[r][nv[r]++]=q; }
  for(int c=0;c<4;c++) ms_uret(c);
  for(int a=0;a<nm[0];a++) for(int b=0;b<nm[1];b++) for(int c=0;c<nm[2];c++) for(int d=0;d<nm[3];d++){
    int id[4]={a,b,c,d}; int cn[4]; long s[4]; int qv[32], k=0;
    for(int x=0;x<4;x++){ cn[x]=mc[x][id[x]]; s[x]=0; for(int i=0;i<cn[x];i++){ s[x]+=ms[x][id[x]][i]; qv[k++]=ms[x][id[x]][i]; } }
    int h0=cn[0]+cn[1], h1=cn[2]+cn[3]; long q0=s[0]+s[1], q1=s[2]+s[3], sq=q0+q1; int dolu=cn[0]&&cn[1]&&cn[2]&&cn[3]; unsigned m=0;
    if(h0==h1&&h0>=1) m|=1; if(h0>=1&&h1>=1&&q0==q1) m|=2; if(cn[0]==cn[1]&&cn[0]>=1) m|=4; if(cn[0]>=1&&cn[1]>=1&&s[0]==s[1]) m|=8;
    if(cn[2]==cn[3]&&cn[2]>=1) m|=16; if(cn[2]>=1&&cn[3]>=1&&s[2]==s[3]) m|=32; if(dolu&&kosu4(s[0],s[1],s[2],s[3])) m|=64;
    if(k>=2){ int mn=qv[0],mx=qv[0]; for(int i=1;i<k;i++){ if(qv[i]<mn)mn=qv[i]; if(qv[i]>mx)mx=qv[i]; } int span=mx-mn+1; if(span>=2 && span<=k){ int seen[64]={0}, nd=0; for(int i=0;i<k;i++){ if(!seen[qv[i]-mn]){ seen[qv[i]-mn]=1; nd++; } } if(nd==span) m|=128; } }
    if(dolu&&sq==4L*p) m|=256; if(k>=1&&sq%p==0) m|=512; if(h0>=1&&h1>=1&&q0%p==0&&q1%p==0) m|=1024;
    H[m]++; }
  FILE *fo=fopen(argv[5],"wb"); fwrite(H,8,2048,fo); fclose(fo); return 0; }
