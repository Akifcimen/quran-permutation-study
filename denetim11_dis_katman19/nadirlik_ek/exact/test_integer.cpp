#include "cw_sampler.hpp"
#include <iostream>
int main(int argc,char**argv){
  if(argc!=2)return 2;
  std::ifstream f(argv[1],std::ios::binary);
  uint64_t cases=0;
  for(;;){
    uint64_t data[10];f.read(reinterpret_cast<char*>(data),sizeof(data));
    if(f.gcount()==0)break;
    if(f.gcount()!=sizeof(data))return 3;
    exact_block6::U256 denominator,numerator,expected;
    for(int j=0;j<3;j++){denominator.w[j]=data[j];numerator.w[j]=data[3+j];expected.w[j]=data[7+j];}
    auto q=numerator.divide_small_quotient(denominator);
    if(q!=data[6]||numerator!=expected){std::cerr<<"failure "<<cases<<"\n";return 1;}
    cases++;
  }
  std::cout<<"exact_quotient_remainder_cases "<<cases<<"\n";
}
