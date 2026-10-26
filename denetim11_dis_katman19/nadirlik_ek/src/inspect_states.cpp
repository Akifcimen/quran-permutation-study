#include "../../src/scorer.hpp"
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <iomanip>
using namespace study;
int main(int argc,char**argv){try{
 if(argc!=4)throw std::runtime_error("usage: inspect_states train states output");
 std::array<int,N>base;std::ifstream cf("data/counts.txt");for(auto&a:base)cf>>a;
 Hist h;h.load(argv[1]);Scorer score(h);MatrixEngine matrix;Extractor extract(base,matrix);
 std::ifstream in(argv[2],std::ios::binary);std::ofstream out(argv[3]);out<<std::setprecision(17)<<"[";
 std::set<std::string>unique;bool first=true;int count=0;
 while(true){Draw d;std::array<double,K> saved;in.read((char*)d.ids.data(),sizeof(int)*N);if(!in)break;in.read((char*)saved.data(),sizeof(double)*K);if(!in)throw std::runtime_error("truncated state");
  int inv129=0,inv127=0;
  for(int i=0;i<N;i++)for(int j=i+1;j<N;j++){inv129+=base[d.ids[i]]<base[d.ids[j]];inv127+=(d.ids[i]==8?127:base[d.ids[i]])<(d.ids[j]==8?127:base[d.ids[j]]);}
  const auto f=extract(d);const auto s=score(f);for(int k=0;k<K;k++)if(s.value[k]!=saved[k])throw std::runtime_error("saved score mismatch");
  std::ostringstream key;const auto&t=s.trace[1];key<<t.variant<<":"<<t.active;for(int b=0;b<t.block_count;b++){auto &z=t.blocks[b];key<<":"<<z.modules<<","<<z.form<<","<<z.prime_index<<","<<z.matrix_view;for(auto m:z.positive_masks)key<<","<<int(m);}
  if(unique.insert(key.str()).second){if(!first)out<<",";first=false;out<<"{\"sample_index\":"<<count<<",\"I129\":"<<inv129<<",\"I127\":"<<inv127<<",\"scores\":";write_scores_json(out,s);out<<",\"ids\":[";for(int i=0;i<N;i++){if(i)out<<",";out<<d.ids[i];}out<<"]}";}count++;
 }
 out<<"]\n";std::cout<<"verified "<<count<<" states, "<<unique.size()<<" primary programs\n";
}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}}
