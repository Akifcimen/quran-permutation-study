#include "../../src/scorer.hpp"
#include "kernels.hpp"
#include "smc_math.hpp"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <set>
#include <sstream>
#include <unordered_set>
#include <vector>
#include <omp.h>

using namespace study;
struct Particle {
 Draw draw;
 std::array<double,K> scores{};
 double distance=0;
 int founder=0;
 uint64_t permutation_hash=0,program_hash=0,natural_hash=0;
 unsigned natural_flags=0;
};
uint64_t mix(uint64_t z){RNG rng(z);return rng.next();}
uint64_t hash_ids(const std::array<int,N>& ids){
 uint64_t h=1469598103934665603ULL;for(auto id:ids){h^=uint64_t(id+1);h*=1099511628211ULL;}return h;
}
uint64_t trace_hash(const ScoreTrace& s){
 uint64_t h=1469598103934665603ULL;
 auto add=[&](uint64_t x){h^=x;h*=1099511628211ULL;};
 add(s.variant+1);add(s.active);
 for(int i=0;i<s.block_count;i++){const auto& b=s.blocks[i];add(b.modules);add(b.form+1);add(b.prime_index+1);add(b.matrix_view+1);for(auto m:b.positive_masks)add(m);}
 return h;
}
uint64_t natural_hash(const Draw& d,const std::array<int,N>& base){
 uint64_t h=1469598103934665603ULL;int ec=0,es=0,en=0,fs=0,hn=0,ha=0;
 for(int i=0;i<N;i++){
  int n=i+1,a=d.ids[i]==8?127:base[d.ids[i]];
  if((n+a)%2==0){ec++;es+=n+a;en+=n;}
  if((n+a)%19==0){fs+=n;h^=uint64_t(n);h*=1099511628211ULL;}
  if((n-a)%19==0){hn++;ha+=a;h^=uint64_t(200+n);h*=1099511628211ULL;}
 }
 for(int a:{ec,es,en,fs,hn,ha}){h^=uint64_t(a);h*=1099511628211ULL;}return h;
}
struct Evaluator {
 const std::array<int,N>& base;const Extractor& extract;const Scorer& scorer;
 std::array<double,K> target;
 Particle operator()(const Draw& d,int founder)const{
  auto f=extract(d);auto s=scorer(f);Particle p;p.draw=d;p.scores=s.value;p.founder=founder;
  p.distance=std::max(0.0,target[1]-p.scores[1]);p.permutation_hash=hash_ids(d.ids);
  p.program_hash=trace_hash(s.trace[1]);p.natural_hash=natural_hash(d,base);
  for(int v=0;v<V;v++){
   bool scalar=f.g[v][0]==3&&f.c[v][6]==15&&f.w[v][6]==3;
   if(scalar)p.natural_flags|=1u<<(2*v);
   if(scalar&&(f.m[v][6]&15)==15)p.natural_flags|=2u<<(2*v);
  }
  return p;
 }
};
struct MoveCounts {
 uint64_t attempted=0,null_ok=0,changed=0,numeric=0,evaluated=0,accepted=0,score_changed=0,program_changed=0,natural_changed=0,refresh_attempts=0;
};
void write_counts(std::ostream& o,const MoveCounts& c){
 o<<"{\"attempted\":"<<c.attempted<<",\"null_ok\":"<<c.null_ok<<",\"changed\":"<<c.changed<<",\"numeric\":"<<c.numeric<<",\"evaluated\":"<<c.evaluated<<",\"accepted\":"<<c.accepted<<",\"score_changed\":"<<c.score_changed<<",\"program_changed\":"<<c.program_changed<<",\"natural_changed\":"<<c.natural_changed<<",\"refresh_attempts\":"<<c.refresh_attempts<<"}";
}
int main(int argc,char**argv){try{
 std::string modelname="uniform",out="rare_event/pilot/smc",betapath="",trainpath="",counts="data/counts.txt",init="iid";
 int population=256,reps=2,moves=64,threads=8,refresh=10;
 uint64_t seed=202610251001;double override_threshold=-1;
 for(int i=1;i<argc;i+=2){
  std::string k=argv[i],v=i+1<argc?argv[i+1]:"";
  if(k=="--model")modelname=v;else if(k=="--out")out=v;else if(k=="--betas")betapath=v;else if(k=="--train")trainpath=v;
  else if(k=="--counts")counts=v;else if(k=="--population")population=std::stoi(v);else if(k=="--reps")reps=std::stoi(v);
  else if(k=="--moves")moves=std::stoi(v);else if(k=="--threads")threads=std::stoi(v);else if(k=="--refresh")refresh=std::stoi(v);
  else if(k=="--seed")seed=std::stoull(v);else if(k=="--threshold")override_threshold=std::stod(v);else if(k=="--initialization")init=v;
  else throw std::runtime_error("unknown argument "+k);
 }
 int model=modelname=="uniform"?0:modelname=="block6"?1:modelname=="inversion_exact"?2:-1;
 if(model<0||population<1||reps<1||moves<0||threads<1)throw std::runtime_error("invalid configuration");
 if(init!="iid"&&init!="observed_diagnostic")throw std::runtime_error("invalid initialization");
 std::array<int,N> base;std::ifstream cfile(counts);for(auto&a:base)cfile>>a;if(!cfile)throw std::runtime_error("missing counts");
 std::vector<double> betas;std::ifstream bf(betapath);double b;while(bf>>b)betas.push_back(b);
 if(betas.empty()||betas.front()<=0||!std::is_sorted(betas.begin(),betas.end()))throw std::runtime_error("invalid beta sequence");
 Hist hist;hist.load(trainpath);Scorer scorer(hist);MatrixEngine matrix;Extractor extractor(base,matrix);
 Draw observed;std::iota(observed.ids.begin(),observed.ids.end(),0);auto targets=scorer(extractor(observed)).value;
 if(override_threshold>=0)targets[1]=override_threshold;
 Evaluator evaluate{base,extractor,scorer,targets};
 omp_set_dynamic(0);omp_set_num_threads(threads);
 auto allstart=std::chrono::steady_clock::now();
 std::vector<double> estimates(reps),maxima(reps),seconds(reps);
 #pragma omp parallel for schedule(dynamic)
 for(int rep=0;rep<reps;rep++){
  auto start=std::chrono::steady_clock::now();
  uint64_t repseed=mix(seed^(0xd1b54a32d192ed03ULL*uint64_t(rep+1)));
  Sampler sampler(base,model,mix(repseed^0x6638432a1e527e09ULL));
  rare::MutationKernel kernel(base,model,mix(repseed^0xc7f2a0a17e987bd9ULL),refresh);
  RNG rng(mix(repseed^0x3b4c508724f1ab63ULL));
  std::vector<Particle> particles;particles.reserve(population);
  for(int i=0;i<population;i++)particles.push_back(evaluate(init=="iid"?sampler.next():observed,i));
  std::ofstream log(out+"_rep"+std::to_string(rep)+".json");log<<std::setprecision(17);
  log<<"{\"model\":\""<<modelname<<"\",\"initialization\":\""<<init<<"\",\"is_production_estimator\":"<<(init=="iid"?"true":"false")<<",\"replicate\":"<<rep<<",\"seed\":"<<repseed<<",\"population\":"<<population<<",\"moves_per_level\":"<<moves<<",\"target\":"<<targets[1]<<",\"levels\":[";
  double logz=0,previous_beta=0;
  for(size_t li=0;li<betas.size();li++){
   double beta=betas[li],delta=beta-previous_beta;
   auto ws=rare::reweight_resample(particles,rng,[&](const Particle&p){return-delta*p.distance;});
   logz+=ws.log_mean;MoveCounts mc;
   for(auto& p:particles)for(int step=0;step<moves;step++){
    mc.attempted++;auto proposal=kernel.propose(p.draw.ids);mc.refresh_attempts+=proposal.refresh_proposals;
    if(!proposal.null_ok)continue;mc.null_ok++;
    if(!proposal.changed)continue;mc.changed++;
    if(!proposal.numeric_changed){
     p.draw=proposal.draw;p.permutation_hash=hash_ids(p.draw.ids);mc.accepted++;continue;
    }
    mc.numeric++;mc.evaluated++;auto candidate=evaluate(proposal.draw,p.founder);
    double loga=-beta*(candidate.distance-p.distance);
    if(loga>=0||std::log(rng.unif())<loga){
     mc.accepted++;mc.score_changed+=candidate.scores[1]!=p.scores[1];
     mc.program_changed+=candidate.program_hash!=p.program_hash;mc.natural_changed+=candidate.natural_hash!=p.natural_hash;
     p=std::move(candidate);
    }
   }
   double lo=INFINITY,hi=-INFINITY,mean=0;int hit=0;
   std::unordered_set<uint64_t> distinct,programs,naturals;std::vector<int> founders(population);
   for(const auto&p:particles){lo=std::min(lo,p.scores[1]);hi=std::max(hi,p.scores[1]);mean+=p.scores[1];hit+=p.distance==0;distinct.insert(p.permutation_hash);programs.insert(p.program_hash);naturals.insert(p.natural_hash);founders[p.founder]++;}
   int nf=0,mf=0;for(int f:founders){nf+=f>0;mf=std::max(mf,f);}
   if(li)log<<",";log<<"{\"beta\":"<<beta<<",\"log_z\":"<<logz<<",\"ess_before_resampling\":"<<ws.ess<<",\"max_weight\":"<<ws.max_weight<<",\"minimum_score\":"<<lo<<",\"mean_score\":"<<mean/population<<",\"maximum_score\":"<<hi<<",\"target_hits\":"<<hit<<",\"distinct_permutations\":"<<distinct.size()<<",\"distinct_programs\":"<<programs.size()<<",\"distinct_natural_aggregates\":"<<naturals.size()<<",\"founders\":"<<nf<<",\"largest_founder_fraction\":"<<double(mf)/population<<",\"moves\":";
   write_counts(log,mc);log<<"}";log.flush();
   previous_beta=beta;maxima[rep]=hi;
  }
  int hits=0;std::array<int,4> naturalhits{};std::array<long double,K> weighted{};
  for(const auto&p:particles){
   hits+=p.distance==0;
   for(int bit=0;bit<4;bit++)naturalhits[bit]+=(p.natural_flags>>bit)&1;
   for(int k=0;k<K;k++)if(p.scores[k]>=targets[k])weighted[k]+=std::exp((long double)betas.back()*p.distance);
  }
  double estimate=std::exp(logz)*double(hits)/population;estimates[rep]=estimate;
  seconds[rep]=std::chrono::duration<double>(std::chrono::steady_clock::now()-start).count();
  log<<"],\"log_z\":"<<logz<<",\"target_hits\":"<<hits<<",\"estimate\":"<<estimate<<",\"policy_estimates\":[";
  for(int k=0;k<K;k++){if(k)log<<",";log<<(double)(std::exp((long double)logz)*weighted[k]/population);}
  log<<"],\"natural_flag_counts\":[";
  for(int bit=0;bit<4;bit++){if(bit)log<<",";log<<naturalhits[bit];}
  log<<"],\"seconds\":"<<seconds[rep]<<",\"initial_sampler_proposals\":"<<sampler.proposals<<"}\n";
  std::ofstream state(out+"_states"+std::to_string(rep)+".bin",std::ios::binary);
  for(const auto&p:particles){state.write((const char*)p.draw.ids.data(),sizeof(int)*N);state.write((const char*)p.scores.data(),sizeof(double)*K);}
  #pragma omp critical
  std::cout<<"rep="<<rep<<" model="<<modelname<<" max="<<maxima[rep]<<" hits="<<hits<<"/"<<population<<" estimate="<<std::setprecision(10)<<estimate<<" seconds="<<seconds[rep]<<std::endl;
 }
 double sec=std::chrono::duration<double>(std::chrono::steady_clock::now()-allstart).count(),mean=std::accumulate(estimates.begin(),estimates.end(),0.0)/reps;
 std::ofstream result(out+"_run.json");result<<std::setprecision(17)<<"{\"model\":\""<<modelname<<"\",\"initialization\":\""<<init<<"\",\"population\":"<<population<<",\"reps\":"<<reps<<",\"moves_per_level\":"<<moves<<",\"master_seed\":"<<seed<<",\"target\":"<<targets[1]<<",\"beta_file\":\""<<betapath<<"\",\"arithmetic_mean_estimate\":"<<mean<<",\"seconds\":"<<sec<<",\"estimates\":[";
 for(int r=0;r<reps;r++){if(r)result<<",";result<<estimates[r];}result<<"]}\n";
 return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}}

