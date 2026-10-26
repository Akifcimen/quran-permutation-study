#include "../src/soft_guide.hpp"
#include "soft_guide_reference.hpp"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

namespace {
volatile double sink=0;
inline void consume(const void* pointer){asm volatile("" : : "g"(pointer) : "memory");}
struct Timing{double cpu_us,wall_us;};
template<class Function> Timing measure(int number,Function function){
 const std::clock_t cpu=std::clock();
 const auto wall=std::chrono::steady_clock::now();
 for(int i=0;i<number;++i)function(i);
 return {1e6*double(std::clock()-cpu)/CLOCKS_PER_SEC/number,
         1e6*std::chrono::duration<double>(std::chrono::steady_clock::now()-wall).count()/number};
}
void timing(const char*name,Timing t,bool comma=true){
 if(comma)std::cout<<",";
 std::cout<<"\""<<name<<"\":{\"cpu_us\":"<<t.cpu_us<<",\"wall_us\":"<<t.wall_us<<"}";
}
void equal_prepared(const rare_reference::SoftPrepared&a,const rare::SoftPrepared&b){
 auto check=[](bool ok){if(!ok)throw std::runtime_error("Optimized prepare changed a value");};
 for(int v=0;v<study::V;++v){
  for(int f=0;f<study::F;++f){
   check(a.features.g[v][f]==b.features.g[v][f]);
   for(int j=0;j<2;++j)check(a.g[v][f][j]==b.g[v][f][j]);
  }
  for(int c=0;c<study::C;++c){
   check(a.features.c[v][c]==b.features.c[v][c]);
   check(a.features.w[v][c]==b.features.w[v][c]);
   check(a.features.m[v][c]==b.features.m[v][c]);
   for(int j=0;j<4;++j)check(a.c[v][c][j]==b.c[v][c][j]);
   for(int j=0;j<2;++j)check(a.w[v][c][j]==b.w[v][c][j]);
   for(int j=0;j<5;++j)check(a.m[v][c][j]==b.m[v][c][j]);
  }
 }
}
}

int main(int argc,char**argv){try{
 if(argc!=2)throw std::runtime_error("Need study root");
 const std::string root=argv[1];
 std::array<int,study::N> base{};
 std::ifstream input(root+"/data/counts.txt");for(int&a:base)input>>a;
 MatrixEngine matrix;
 study::Extractor extractor(base,matrix);
 const char*names[]={"uniform","block6","inversion_exact"};
 const char*pilot_names[]={"uniform","block6","inversion"};
 std::cout<<std::setprecision(10)<<"{\"status\":\"completed\",\"cohorts\":[";
 bool comma=false;
 for(int model=0;model<3;++model){
  study::Hist hist;hist.load(root+"/results/train_"+names[model]+".bin");
  rare_reference::SoftGuide guide(base,hist,matrix);study::Scorer scorer(hist);
  rare::SoftGuide fast(base,hist,matrix);
  for(int cohort=0;cohort<2;++cohort){
   std::vector<study::Draw> draws;
   if(cohort==0){study::Sampler iid(base,model,202610253301ULL+model);for(int i=0;i<128;++i)draws.push_back(iid.next());}
   else{
    std::ifstream file(root+"/rare_event/pilot/guided_"+pilot_names[model]+"_states0.bin",std::ios::binary);
    for(int i=0;i<128;++i){study::Draw d;std::array<double,study::K> scores;
     file.read(reinterpret_cast<char*>(d.ids.data()),sizeof(d.ids));
     file.read(reinterpret_cast<char*>(scores.data()),sizeof(scores));
     if(!file)throw std::runtime_error("Incomplete pilot states");
     draws.push_back(d);
    }
   }
   std::vector<rare_reference::SoftPrepared> prepared;prepared.reserve(draws.size());
   double hard_min=1e9,hard_max=0;
   for(const auto&d:draws){
    prepared.push_back(guide.prepare(d));const double t=guide.hard(prepared.back());hard_min=std::min(hard_min,t);hard_max=std::max(hard_max,t);
    auto candidate=fast.prepare(d);equal_prepared(prepared.back(),candidate);
    for(double lambda:{0.,1.,2.,4.,8.,32.})
     if(guide.evaluate(prepared.back(),lambda)!=fast.evaluate(candidate,lambda))throw std::runtime_error("Optimized guide changed a score");
   }
   const int count=draws.size(),repeats=8;
   if(comma)std::cout<<",";
   comma=true;
   std::cout<<"{\"model\":\""<<names[model]<<"\",\"cohort\":\""<<(cohort?"guided_pilot_final":"iid")<<"\",\"states\":"<<count<<",\"hard_min\":"<<hard_min<<",\"hard_max\":"<<hard_max;
   timing("original_extraction",measure(count*repeats,[&](int i){auto p=extractor(draws[i%count]);consume(&p);}));
   timing("soft_prepare",measure(count*repeats,[&](int i){auto p=guide.prepare(draws[i%count]);consume(&p);}));
   timing("fast_candidate_prepare",measure(count*repeats,[&](int i){auto p=fast.prepare(draws[i%count]);consume(&p);}));
   timing("original_six_policy_scorer",measure(count*repeats,[&](int i){auto p=scorer(prepared[i%count].features);consume(&p);sink+=p.value[1];}));
   for(double lambda:{0.,1.,2.,4.,8.,32.}){
    std::string name="evaluate_lambda_"+std::to_string(int(lambda));
    timing(name.c_str(),measure(count*repeats,[&](int i){sink+=guide.evaluate(prepared[i%count],lambda);}));
   }
   std::cout<<"}";
  }
 }
 std::cout<<"],\"checksum\":"<<sink<<"}\n";
 return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}}
