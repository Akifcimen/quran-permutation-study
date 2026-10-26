#include "sampler.hpp"
#include "../../src/scorer.hpp"
#include <omp.h>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

struct Worker {
 std::uint64_t seed=0,processed=0,final_state=0,validated=0,scored=0,accepted=0;
 std::array<std::uint64_t,6> selected{};
 std::array<std::uint64_t,64> membership{},any_g{};
 std::array<std::array<std::uint64_t,64>,6> tail{};
 std::array<std::array<std::uint64_t,64>,6> thinned_tail{};
 std::string example;
};

int main(int argc,char**argv){try{
 std::string root=argc>1?argv[1]:".";
 std::uint64_t n=argc>2?std::stoull(argv[2]):1000000;
 std::uint64_t seed=argc>3?std::stoull(argv[3]):202610251701ULL;
 int threads=argc>4?std::stoi(argv[4]):8;
 std::string outpath=argc>5?argv[5]:root+"/rare_event/block7/pilot.json";
 if(n==0||threads<1)throw std::runtime_error("invalid sample size or team");
 std::array<int,study::N> base;std::ifstream in(root+"/data/counts.txt");for(auto&a:base)in>>a;
 if(!in)throw std::runtime_error("missing verse counts");
 block7::Sampler sampler(base);MatrixEngine matrix;study::Extractor extractor(base,matrix);
 study::Hist hist;hist.load(root+"/results/train_block6.bin");study::Scorer scorer(hist);
 study::Draw observed;std::iota(observed.ids.begin(),observed.ids.end(),0);
 auto thresholds=scorer(extractor(observed));
 std::vector<Worker> workers(threads);int actual=0;
 omp_set_dynamic(0);omp_set_num_threads(threads);
 auto started=std::chrono::steady_clock::now();
 #pragma omp parallel
 {
  int tid=omp_get_thread_num(),team=omp_get_num_threads();
  #pragma omp single
  actual=team;
  auto&w=workers[tid];study::RNG mixer(seed^(UINT64_C(0xd1b54a32d192ed03)*std::uint64_t(tid+1)));
  w.seed=mixer.next();study::RNG rng(w.seed);
  std::uint64_t begin=n*tid/team,end=n*(tid+1)/team;
  for(std::uint64_t i=begin;i<end;++i){
   const auto s=sampler.draw(rng);++w.selected[s.anchor];++w.membership[s.membership];
   const bool accepted=rng.bounded(__builtin_popcount(s.membership))==0;
   w.accepted+=accepted;
   if(s.any_g_balance)++w.any_g[s.membership];
   const bool validate=i-begin<125;
   if(s.any_g_balance||validate){
    const auto d=sampler.full_draw(s);const auto f=extractor(d);
    if(validate){
     bool balance=false;for(int v=0;v<2;++v)for(int form=0;form<14;++form)balance|=(f.g[v][form]&2)!=0;
     if(balance!=s.any_g_balance)throw std::runtime_error("Any-G screen disagrees with original extractor");
     unsigned direct=0;
     for(int v=0;v<2;++v)for(int ratio=1;ratio<=3;++ratio){
      int form=ratio==1?0:ratio==2?6:10;
      if(f.w[v][form*29+2]&1)direct|=1u<<(3*v+ratio-1);
     }
     if(direct!=s.membership)throw std::runtime_error("Direct W7 anchor membership mismatch");
     for(int k=0;k<114;++k)if(d.ids[k]/6!=k/6)throw std::runtime_error("Block membership violated");
     ++w.validated;
    }
    if(s.any_g_balance){
     ++w.scored;auto scores=scorer(f);
     for(int k=0;k<6;++k)if(scores.value[k]>=thresholds.value[k]){
      ++w.tail[k][s.membership];w.thinned_tail[k][s.membership]+=accepted;
     }
     if(w.example.empty()&&scores.value[1]>=thresholds.value[1]){
      std::ostringstream os;os<<"{\"membership\":"<<s.membership<<",\"ids\":[";
      for(int k=0;k<114;++k){if(k)os<<',';os<<d.ids[k];}
      os<<"],\"scores\":";study::write_scores_json(os,scores);os<<'}';w.example=os.str();
     }
    }
   }
  }
  w.processed=end-begin;w.final_state=rng.state;
 }
 Worker all;
 for(const auto&w:workers){
  all.processed+=w.processed;all.scored+=w.scored;all.validated+=w.validated;all.accepted+=w.accepted;
  for(int j=0;j<6;++j)all.selected[j]+=w.selected[j];
  for(int m=0;m<64;++m){all.membership[m]+=w.membership[m];all.any_g[m]+=w.any_g[m];for(int k=0;k<6;++k){all.tail[k][m]+=w.tail[k][m];all.thinned_tail[k][m]+=w.thinned_tail[k][m];}}
 }
 if(all.processed!=n)throw std::runtime_error("Processed count mismatch");
 long double union_estimate=0;
 for(unsigned mask=1;mask<64;++mask)union_estimate+=sampler.weights[mask]*all.membership[mask]/n;
 double seconds=std::chrono::duration<double>(std::chrono::steady_clock::now()-started).count();
 std::ofstream out(outpath);out<<std::setprecision(22);
 out<<"{\"model\":\"block6\",\"proposal\":\"exact size-weighted mixture of six W0(p7) anchors, followed by independent1/K thinning\",\"scope\":\"Primary tail intersect union of six W7 anchors; no claim about complement\",\"primary_complete_within_union\":true,\"secondary_scope\":\"Other policy tails additionally intersect any G balance; completeness not asserted\",\"n\":"<<n<<",\"seed\":"<<seed<<",\"actual_threads\":"<<actual<<",\"processed\":"<<all.processed<<",\"scored\":"<<all.scored<<",\"thinning_accepted\":"<<all.accepted<<",\"mixture_mass_sum_b\":"<<sampler.mixture_mass<<",\"direct_validations\":"<<all.validated<<",\"seconds\":"<<seconds<<",\"weighted_union_estimate\":"<<union_estimate<<",\"thinned_union_estimate\":"<<sampler.mixture_mass*all.accepted/n<<",\"anchors\":[";
 for(int j=0;j<6;++j){if(j)out<<',';out<<"{\"variant\":"<<(j/3?127:129)<<",\"ratio\":"<<j%3+1<<",\"count\":\""<<sampler.total[j]<<"\",\"probability\":"<<sampler.probability[j]<<",\"selected\":"<<all.selected[j]<<'}';}
 out<<"],\"membership\":[";
 for(unsigned mask=0;mask<64;++mask){
  if(mask)out<<',';out<<"{\"mask\":"<<mask<<",\"weight\":"<<sampler.weights[mask]<<",\"n\":"<<all.membership[mask]<<",\"any_G_balance\":"<<all.any_g[mask]<<",\"tail_counts\":[";
  for(int k=0;k<6;++k){if(k)out<<',';out<<all.tail[k][mask];}out<<"],\"thinned_tail_counts\":[";
  for(int k=0;k<6;++k){if(k)out<<',';out<<all.thinned_tail[k][mask];}out<<"]}";
 }
 out<<"],\"tail_estimates\":[";
 for(int k=0;k<6;++k){
  long double sum=0,sumsq=0;std::uint64_t hits=0,thinned_hits=0;
  for(unsigned mask=1;mask<64;++mask){long double w=sampler.weights[mask];hits+=all.tail[k][mask];thinned_hits+=all.thinned_tail[k][mask];sum+=w*all.tail[k][mask];sumsq+=w*w*all.tail[k][mask];}
  long double mean=sum/n,variance=(sumsq-sum*sum/n)/(n-1),se=std::sqrt(std::max(0.L,variance)/n);
  if(k)out<<',';out<<"{\"policy\":"<<k<<",\"threshold\":"<<thresholds.value[k]<<",\"unthinned_hits\":"<<hits<<",\"thinned_hits\":"<<thinned_hits<<",\"estimate\":"<<sampler.mixture_mass*thinned_hits/n<<",\"weighted_estimate\":"<<mean<<",\"weighted_standard_error\":"<<se<<",\"sum_weighted_hits\":"<<sum<<",\"sum_squared_weighted_hits\":"<<sumsq<<'}';
  if(k==1)std::cout<<std::setprecision(12)<<"primary_thinned_hits="<<thinned_hits<<" p_intersect_union="<<sampler.mixture_mass*thinned_hits/n<<" weighted="<<mean<<" weighted_SE="<<se<<'\n';
 }
 out<<"],\"workers\":[";
 for(int i=0;i<actual;++i){if(i)out<<',';const auto&w=workers[i];out<<"{\"seed\":"<<w.seed<<",\"processed\":"<<w.processed<<",\"final_rng_state\":"<<w.final_state<<'}';}
 out<<"],\"examples\":[";bool first=true;for(const auto&w:workers)if(!w.example.empty()){if(!first)out<<',';first=false;out<<w.example;}out<<"]}\n";
 std::cout<<"n="<<n<<" scored="<<all.scored<<" seconds="<<seconds<<"\n";
 return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
