// Relaxed, exhaustive modular feasibility bound conditional on G bit 1.
// This does not claim every residue pair comes from an actual permutation.
#include "../../src/matrix.hpp"
#include "../../src/common.hpp"
#include <array>
#include <iostream>

int main(){
  MatrixEngine engine;
  for(int v=0;v<2;++v) for(int f=0;f<14;++f) for(int pi=0;pi<29;++pi){
    int p=MatrixEngine::kPrimes[pi], a=study::AL[f], b=study::BE[f];
    int total=v?6234:6236, inverse=0;
    int bm=(b%p+p)%p;
    if(bm)for(int q=1;q<p;++q)if(q*bm%p==1){inverse=q;break;}
    std::array<unsigned,8> possible{};
    bool free_y=(a%2 && b%2);
    for(int yy=0;yy<(free_y?p:1);++yy){
      int y=free_y?yy:(a%2?3306:total-3514);
      int z=total-y;
      MatrixEngine::Matrix P={y,z,6555-y,total-z};
      for(int r=0;r<p;++r){
        int fn=bm?r:0, fa=bm?(((-a*inverse*r)%p)+p)%p:r;
        MatrixEngine::Matrix R={fn,fa,6555-fn,total-fa};
        auto packed=engine.eval(pi,P,R);
        for(int j=0;j<8;++j)possible[j]|=1u<<((packed>>(4*j))&15);
      }
    }
    std::cout<<v<<' '<<f<<' '<<pi;
    for(auto x:possible)std::cout<<' '<<x;
    std::cout<<'\n';
  }
}
