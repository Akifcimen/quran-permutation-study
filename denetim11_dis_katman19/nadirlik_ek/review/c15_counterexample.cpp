#include "../../src/scorer.hpp"
#include <fstream>
#include <iostream>
#include <numeric>
int main(int argc,char**){
  std::array<int,study::N> base;std::ifstream in("data/counts.txt");for(auto&a:base)in>>a;
  study::Draw d;std::iota(d.ids.begin(),d.ids.end(),0);
  d.ids[49]=87;d.ids[87]=106;d.ids[106]=49;
  if(argc>1){std::swap(d.ids[7],d.ids[25]);std::swap(d.ids[64],d.ids[76]);std::swap(d.ids[0],d.ids[34]);}
  MatrixEngine matrix;study::Extractor extract(base,matrix);auto f=extract(d);
  int inv=0,inv127=0;for(int i=0;i<114;++i)for(int j=i+1;j<114;++j){inv+=base[d.ids[i]]<base[d.ids[j]];inv127+=(d.ids[i]==8?127:base[d.ids[i]])<(d.ids[j]==8?127:base[d.ids[j]]);}
  std::cout<<"{\"inversions129\":"<<inv<<",\"inversions127\":"<<inv127<<",\"all_C15_candidates\":[";
  bool comma=false;for(int v=0;v<2;++v)for(int c=0;c<406;++c)if(f.c[v][c]==15){if(comma)std::cout<<',';comma=true;std::cout<<'['<<v<<','<<c/29<<','<<MatrixEngine::kPrimes[c%29]<<']';}
  std::cout<<"],\"scores\":{";comma=false;
  for(const char* model:{"uniform","block6","inversion_exact"}){
    study::Hist hist;hist.load(std::string("results/train_")+model+".bin");study::Scorer scorer(hist);
    if(comma)std::cout<<',';comma=true;std::cout<<'"'<<model<<"\":";write_scores_json(std::cout,scorer(f));
  }
  std::cout<<"}}\n";
}
