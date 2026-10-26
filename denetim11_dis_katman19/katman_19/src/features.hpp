#ifndef JOINT_FEATURES_HPP
#define JOINT_FEATURES_HPP
#include "common.hpp"
#include "matrix.hpp"
#include "sampler.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
namespace study {
struct Family {
 int count=0,sn=0,sa=0,overlap=0,overlap_a=0;
 int celln[4]{},cells[4]{},lo[4]{},hi[4]{};
 int qmin=10000,qmax=-10000;
 uint64_t seen[16]{};
 void add(int n,int a,int q,int cell,bool overlap_){
  count++;sn+=n;sa+=a;overlap+=overlap_;if(overlap_)overlap_a+=a;
  cells[cell]+=q;
  if(celln[cell]++==0)lo[cell]=hi[cell]=q;else{lo[cell]=std::min(lo[cell],q);hi[cell]=std::max(hi[cell],q);}
  qmin=std::min(qmin,q);qmax=std::max(qmax,q);
  int b=q+512;if(b<0||b>=1024)throw std::runtime_error("coefficient outside storage range");
  seen[b>>6]|=uint64_t(1)<<(b&63);
 }
 uint8_t mask(int p)const{
  bool equal=celln[0]>0&&celln[0]==celln[1]&&celln[0]==celln[2]&&celln[0]==celln[3];
  auto row=[](int a,int b,int c,int d){return a==b&&((c==a-1&&d==a+1)||(c==a+1&&d==a-1));};
  bool pattern=row(cells[0],cells[1],cells[2],cells[3])||row(cells[2],cells[3],cells[0],cells[1]);
  int distinct=0;for(auto w:seen)distinct+=__builtin_popcountll(w);
  bool uniform=false;for(int c=0;c<4;c++)uniform|=celln[c]>=2&&lo[c]==hi[c];
  bool band=count>0&&distinct==qmax-qmin+1&&uniform;
  bool centre=cells[0]+cells[1]+cells[2]+cells[3]==4*p;
  return equal|(pattern<<1)|(band<<2)|(centre<<3);
 }
};
struct Features {
 uint8_t g[V][F]{};
 uint8_t c[V][C]{};
 uint8_t w[V][C]{};
 uint32_t m[V][C]{};
};
struct Extractor {
 const std::array<int,N>& base;
 const MatrixEngine& mat;
 std::array<uint32_t,1201> divisors{};
 Extractor(const std::array<int,N>& b,const MatrixEngine& e):base(b),mat(e){
  for(int x=0;x<=1200;x++)for(int pi=0;pi<P;pi++)if(x%MatrixEngine::kPrimes[pi]==0)divisors[x]|=1u<<pi;
 }
 Features operator()(const Draw& d,bool matrices=true)const{
  Features z;
  for(int v=0;v<V;v++){
   std::array<int,N>a;int total=0;for(int i=0;i<N;i++){a[i]=(v==1&&d.ids[i]==8)?127:base[d.ids[i]];total+=a[i];}
   for(int f=0;f<F;f+=2){
    Family fam[2][P];int en=0,ea=0,ec=0;
    int alpha=AL[f],beta=BE[f];
    for(int i=0;i<N;i++){
     int n=i+1,aa=a[i],tp=alpha*n+beta*aa,tm=alpha*n-beta*aa;
     int parity=tp&1;if(!parity){ec++;en+=n;ea+=aa;}
     uint32_t dp=divisors[std::abs(tp)],dm=divisors[std::abs(tm)];
     int cell=parity*2+(n&1);
     uint32_t bits=dp;while(bits){int pi=__builtin_ctz(bits);bits&=bits-1;fam[0][pi].add(n,aa,tp/MatrixEngine::kPrimes[pi],cell,(dm>>pi)&1);}
     bits=dm;while(bits){int pi=__builtin_ctz(bits);bits&=bits-1;fam[1][pi].add(n,aa,tm/MatrixEngine::kPrimes[pi],cell,(dp>>pi)&1);}
    }
    int gm=(ec==57)|((en==total-ea)<<1);z.g[v][f]=z.g[v][f+1]=gm;
    MatrixEngine::Matrix pm={en,ea,6555-en,total-ea};
    for(int pi=0;pi<P;pi++){
     int p=MatrixEngine::kPrimes[pi];const Family& one=fam[0][pi];const Family& two=fam[1][pi];
     int unionn=one.count+two.count-one.overlap,uniona=one.sa+two.sa-one.overlap_a;
     int wm=((one.overlap==0&&unionn==p)?1:0)|((uniona%p==0)?2:0);
     for(int sign=0;sign<2;sign++){
      const Family& ff=fam[sign][pi];int cx=(f+sign)*P+pi;
      z.c[v][cx]=ff.mask(p);z.w[v][cx]=wm;
      if(matrices){MatrixEngine::Matrix rm={ff.sn,ff.sa,6555-ff.sn,total-ff.sa};z.m[v][cx]=mat.eval(pi,pm,rm);}
     }
    }
   }
  }
  return z;
 }
};
}
#endif
