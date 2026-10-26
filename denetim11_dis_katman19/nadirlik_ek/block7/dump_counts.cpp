#include "sampler.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
int main(int argc,char**argv){try{
 std::string root=argc>1?argv[1]:".";std::array<int,study::N> base;
 std::ifstream in(root+"/data/counts.txt");for(auto&a:base)in>>a;if(!in)throw std::runtime_error("missing counts");
 block7::Sampler sampler(base);
 std::cout<<std::setprecision(22)<<"{\"base_count\":\""<<sampler.base_count<<"\",\"anchors\":[";
 for(int j=0;j<6;++j){
  if(j)std::cout<<',';
  std::cout<<"{\"variant\":"<<(j/3?127:129)<<",\"ratio\":"<<j%3+1<<",\"count\":\""<<sampler.total[j]<<"\",\"probability\":"<<sampler.probability[j]<<",\"block_histograms\":[";
  for(int block=0;block<19;++block){if(block)std::cout<<',';std::cout<<'[';for(int k=0;k<7;++k){if(k)std::cout<<',';std::cout<<sampler.groups[j][block][k].size();}std::cout<<']';}
  std::cout<<"]}";
 }
 std::cout<<"],\"membership_weights\":[";
 for(unsigned mask=0;mask<64;++mask){if(mask)std::cout<<',';std::cout<<sampler.weights[mask];}
 std::cout<<"]}\n";
 return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
