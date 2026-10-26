// Exact modular maximum candidates without G balance, with safe early exit
// when every view attains mask15 (which maximizes each fitted utility).
#include "../../src/matrix.hpp"
#include "../../src/common.hpp"
#include <array>
#include <iostream>

int main(){
  MatrixEngine engine;
  for(int v=0;v<2;++v)for(int f=0;f<14;++f)for(int pi=0;pi<29;++pi){
    int p=MatrixEngine::kPrimes[pi],a=study::AL[f],b=study::BE[f];
    int total=v?6234:6236,inverse=0,bm=(b%p+p)%p;
    if(bm)for(int q=1;q<p;++q)if(q*bm%p==1){inverse=q;break;}
    std::array<unsigned,8> possible{};
    int ny=a%2&&b%2?p:(a%2?1:p), nz=a%2?p:1;
    bool complete=false;
    for(int yy=0;yy<ny&&!complete;++yy)for(int zz=0;zz<nz&&!complete;++zz){
      int y=(a%2&&b%2)||a%2==0?yy:3306;
      int z=a%2?zz:3514;
      MatrixEngine::Matrix P={y,z,6555-y,total-z};
      for(int r=0;r<p&&!complete;++r){
        int fn=bm?r:0,fa=bm?(((-a*inverse*r)%p)+p)%p:r;
        MatrixEngine::Matrix R={fn,fa,6555-fn,total-fa};
        auto packed=engine.eval(pi,P,R);complete=true;
        for(int j=0;j<8;++j){possible[j]|=1u<<((packed>>(4*j))&15);complete&=(possible[j]&(1u<<15))!=0;}
      }
    }
    std::cout<<v<<' '<<f<<' '<<pi;
    for(auto x:possible)std::cout<<' '<<x;
    std::cout<<'\n';
  }
}
