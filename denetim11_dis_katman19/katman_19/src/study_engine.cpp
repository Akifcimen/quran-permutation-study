#include "scorer.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <queue>
#include <sstream>
#include <omp.h>

namespace study {
struct Diagnostic {
 std::array<uint64_t,20> counts{};
 void add(const Features& x){
  bool e[4]={false,false,false,false};
  bool bareC=false,centredC=false,bothCM=false,centredCM=false;
  for(int v=0;v<V;v++){
   bool gg=false,cc=false,ww=false,mm=false;
   for(int f=0;f<F;f++){
    gg|=x.g[v][f]==3;
    for(int pi=0;pi<P;pi++){
     int cx=f*P+pi;bool M=false;uint32_t bits=x.m[v][cx];
     for(int r=0;r<8;r++,bits>>=4)M|=(bits&15)==15;
     bool C7=(x.c[v][cx]&7)==7,C15=x.c[v][cx]==15,W=x.w[v][cx]==3,G=x.g[v][f]==3;
     cc|=C7;ww|=W;mm|=M;
     e[1]|=G&&C7&&W&&M;
     e[2]|=G&&C15&&W&&M;
     e[3]|=G&&C15&&W&&M&&(pi==0||pi==1||pi==2||pi==6);
     bareC|=C7;centredC|=C15;bothCM|=C7&&M;centredCM|=C15&&M;
    }
   }
   e[0]|=gg&&cc&&ww&&mm;
  }
  for(int j=0;j<4;j++)counts[j]+=e[j];
  counts[4]+=bareC;counts[5]+=centredC;counts[6]+=bothCM;counts[7]+=centredCM;
  for(int v=0;v<V;v++){
   int cx=6;bool M=false;uint32_t bits=x.m[v][cx];for(int r=0;r<8;r++,bits>>=4)M|=(bits&15)==15;
   int b=8+6*v;
   counts[b]+=(x.c[v][cx]&7)==7;counts[b+1]+=x.c[v][cx]==15;counts[b+2]+=M;
   counts[b+3]+=((x.c[v][cx]&7)==7)&&M;counts[b+4]+=x.g[v][0]==3;counts[b+5]+=x.w[v][cx]==3;
  }
 }
};
struct Top {
 double key;Draw draw;Scores scores;
 bool operator<(const Top& b)const{return key>b.key;}
};
void write_feature_json(const Features& x,const std::string& path){
 std::ofstream o(path);o<<"{\"variants\":[";
 for(int v=0;v<V;v++){
  if(v)o<<",";
  o<<"{\"variant\":"<<(v==0?129:127)<<",\"candidates\":[";
  for(int f=0;f<F;f++)for(int pi=0;pi<P;pi++){
   int cx=f*P+pi;if(cx)o<<",";
   o<<"{\"alpha\":"<<AL[f]<<",\"beta\":"<<BE[f]<<",\"p\":"<<MatrixEngine::kPrimes[pi]<<",\"G\":"<<int(x.g[v][f])<<",\"C\":"<<int(x.c[v][cx])<<",\"W\":"<<int(x.w[v][cx])<<",\"matrix_packed\":"<<x.m[v][cx]<<"}";
  }
  o<<"]}";
 }
 o<<"]}\n";
}
}

int main(int argc,char**argv){using namespace study;try{
 std::string phase="bench",modelname="uniform",out="results/pilot",trainpath="",countspath="data/counts.txt",inputpath="";
 uint64_t nsamp=1000,seed=20261025;int threads=8;
 for(int i=1;i<argc;i+=2){
  std::string k=argv[i],val=i+1<argc?argv[i+1]:"";
  if(k=="--phase")phase=val;else if(k=="--model")modelname=val;else if(k=="--out")out=val;else if(k=="--train")trainpath=val;
  else if(k=="--counts")countspath=val;else if(k=="--input")inputpath=val;else if(k=="--n")nsamp=std::stoull(val);
  else if(k=="--seed")seed=std::stoull(val);else if(k=="--threads")threads=std::stoi(val);else throw std::runtime_error("unknown argument "+k);
 }
 int model=modelname=="uniform"?0:modelname=="block6"?1:modelname=="inversion_exact"?2:-1;
 if(model<0)throw std::runtime_error("unknown model");
 if(threads<1||nsamp<1)throw std::runtime_error("threads and n must be positive");
 if(phase!="bench"&&phase!="train"&&phase!="test"&&phase!="observed")throw std::runtime_error("unknown phase");
 std::array<int,N>base{};std::ifstream data(countspath);for(auto& a:base)data>>a;if(!data)throw std::runtime_error("count data missing");
 auto start=std::chrono::steady_clock::now();MatrixEngine matrix;Extractor extract(base,matrix);
 Draw real;std::iota(real.ids.begin(),real.ids.end(),0);
 if(!inputpath.empty()){std::ifstream f(inputpath);for(auto& id:real.ids)f>>id;if(!f)throw std::runtime_error("permutation input missing");}
 if(phase=="observed"){
  auto f=extract(real);write_feature_json(f,out+"_features.json");
  if(!trainpath.empty()){Hist h;h.load(trainpath);Scorer scorer(h);auto s=scorer(f);std::ofstream o(out+"_scores.json");write_scores_json(o,s);o<<"\n";}
  return 0;
 }
 omp_set_dynamic(0);
 omp_set_num_threads(threads);
 int actual_threads=0;
 std::vector<Hist> hist(threads);std::vector<uint64_t> proposals(threads),processed(threads),worker_seeds(threads);std::vector<Diagnostic> diag(threads);std::vector<std::vector<Top>> top(threads);
 std::unique_ptr<Scorer> scorer;
 if(phase=="test"){Hist trained;trained.load(trainpath);scorer=std::make_unique<Scorer>(trained);}
 #pragma omp parallel
 {
  int tid=omp_get_thread_num(),team_size=omp_get_num_threads();
  #pragma omp single
  actual_threads=team_size;
  uint64_t begin=nsamp*tid/team_size,end=nsamp*(tid+1)/team_size;
  // Hash each worker identifier into a starting state. Spacing starting states
  // by SplitMix's increment would instead create overlapping shifted streams.
  RNG seed_mixer(seed^(0xd1b54a32d192ed03ULL*uint64_t(tid+1)));
  worker_seeds[tid]=seed_mixer.next();
  Sampler sampler(base,model,worker_seeds[tid]);
  std::ofstream scores;if(phase=="test")scores.open(out+"_scores_"+std::to_string(tid)+".bin",std::ios::binary);
  std::priority_queue<Top> heap;
  for(uint64_t j=begin;j<end;j++){
   auto draw=sampler.next();auto features=extract(draw);
   if(phase=="train")hist[tid].add(features);
   else if(phase=="test"){
    auto ss=(*scorer)(features);scores.write((char*)ss.value.data(),K*sizeof(double));diag[tid].add(features);
    if(heap.size()<50||ss.value[1]>heap.top().key){heap.push(Top{ss.value[1],draw,ss});if(heap.size()>50)heap.pop();}
   }
  }
  processed[tid]=end-begin;
  proposals[tid]=sampler.proposals;hist[tid].proposals=sampler.proposals;
  while(!heap.empty()){top[tid].push_back(heap.top());heap.pop();}
 }
 if(std::accumulate(processed.begin(),processed.end(),uint64_t(0))!=nsamp)throw std::runtime_error("processed sample count mismatch");
 if(phase=="train"){Hist h;for(const auto& a:hist)h.merge(a);h.save(out+".bin");}
 if(phase=="test"){
  std::ofstream d(out+"_diagnostics.json");d<<"{\"n\":"<<nsamp<<",\"counts\":[";
  for(int i=0;i<20;i++){uint64_t c=0;for(auto& a:diag)c+=a.counts[i];if(i)d<<",";d<<c;}d<<"]}\n";
  std::vector<Top> all;for(auto& a:top)all.insert(all.end(),a.begin(),a.end());
  std::sort(all.begin(),all.end(),[](const Top&a,const Top&b){return a.key>b.key;});
  std::ofstream t(out+"_top.json");t<<"[";
  for(size_t i=0;i<std::min<size_t>(50,all.size());i++){
   if(i)t<<",";
   t<<"{\"ids\":[";for(int j=0;j<N;j++){if(j)t<<",";t<<all[i].draw.ids[j];}
   t<<"],\"scores\":";write_scores_json(t,all[i].scores);t<<"}";
  }
  t<<"]\n";
 }
 uint64_t nprop=std::accumulate(proposals.begin(),proposals.end(),uint64_t(0));
 double sec=std::chrono::duration<double>(std::chrono::steady_clock::now()-start).count();
 std::ofstream log(out+"_run.json");log<<"{\"phase\":\""<<phase<<"\",\"model\":\""<<modelname<<"\",\"n\":"<<nsamp<<",\"seed\":"<<seed<<",\"threads\":"<<actual_threads<<",\"requested_threads\":"<<threads<<",\"worker_seeds\":[";
 for(int tid=0;tid<actual_threads;tid++){if(tid)log<<",";log<<worker_seeds[tid];}
 log<<"],\"processed\":"<<std::accumulate(processed.begin(),processed.end(),uint64_t(0))<<",\"proposals\":"<<nprop<<",\"seconds\":"<<sec<<"}\n";
 std::cout<<phase<<" "<<modelname<<" n="<<nsamp<<" proposals="<<nprop<<" seconds="<<sec<<" samples_per_second="<<nsamp/sec<<"\n";
 }catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}return 0;
}
