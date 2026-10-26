#include "uniform_sis_production.hpp"
#include "../../src/scorer.hpp"
#include <omp.h>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <sstream>

constexpr int NM=11;
const char* metric_names[NM]={"anchor_C15_W_127","G127","G127_Mview0","G127_Manyview","both_variant_CW_anchors","tail_uniform","tail_mixture","tail_literal","tail_discussion","tail_anchored19","tail_frequency"};
struct Metric {uint64_t hits=0;long double sum=0,sumsq=0;double maximum=0;void add(double w){hits++;sum+=w;sumsq+=static_cast<long double>(w)*w;maximum=std::max(maximum,w);}};
struct Worker {
  std::array<Metric,NM> metrics{};
  std::array<uint64_t,20> patterns{};
  std::array<long double,20> pattern_weights{};
  std::array<std::array<Metric,NM>,20> pattern_metrics{};
  std::array<uint64_t,8> h_dead{};
  uint64_t seed=0,processed=0,success=0,validations=0,full_scores=0,upper_gates=0;
  uint64_t rng_calls=0,final_rng_state=0;
  double max_logweight_error=0;
  std::string example;
  std::string max_target_example;
};
void write_metrics(std::ostream& out,const std::array<Metric,NM>& metrics){out<<'[';for(int k=0;k<NM;k++){if(k)out<<',';const auto&m=metrics[k];out<<"{\"name\":\""<<metric_names[k]<<"\",\"hits\":"<<m.hits<<",\"weight_sum\":"<<m.sum<<",\"weight_square_sum\":"<<m.sumsq<<",\"max_weight\":"<<m.maximum<<'}';}out<<']';}
int main(int argc,char**argv){try{
  if(argc!=10){std::cerr<<"usage ROOT N SEED THREADS sis SCOREMODE OUTFILE VARIANT PHASE\n";return 2;}
  const std::string root=argv[1],method=argv[5],output=argv[7];const uint64_t n=std::stoull(argv[2]),seed=std::stoull(argv[3]);
  const int threads=std::stoi(argv[4]),scoremode=std::stoi(argv[6]),variant=std::stoi(argv[8]);
  const std::string phase=argv[9];
  if(method!="sis"||scoremode!=1)throw std::runtime_error("Production scope requires SIS and Geither-intersected score evaluation");
  if(variant!=127)throw std::runtime_error("Current summaries explicitly target127 anchor");
  auto start=std::chrono::steady_clock::now();
  std::array<int,114> base{};{std::ifstream f(root+"/data/counts.txt");for(auto&x:base)f>>x;if(!f)throw std::runtime_error("Missing counts");}
  uniform_anchor::Sampler proposal(root+"/rare_event/uniform_anchor/F_conditional_sampler_127.bin",base);
  MatrixEngine matrix;study::Extractor extract(base,matrix);study::Hist hist;hist.load(root+"/results/train_uniform.bin");study::Scorer scorer(hist);
  study::Draw identity;std::iota(identity.ids.begin(),identity.ids.end(),0);auto observed=scorer(extract(identity));
  std::vector<Worker> workers(threads);omp_set_dynamic(0);omp_set_num_threads(threads);int actual=0;
  #pragma omp parallel
  {
    int tid=omp_get_thread_num(),team=omp_get_num_threads();
    #pragma omp single
    actual=team;
    auto&w=workers[tid];study::RNG mix(seed^(0xd1b54a32d192ed03ULL*uint64_t(tid+1)));w.seed=mix.next();uniform_anchor::CountedRNG rng(w.seed);
    uint64_t begin=n*tid/team,end=n*(tid+1)/team;
    for(uint64_t j=begin;j<end;j++){
      auto x=method=="sis"?proposal.sis(rng):proposal.rejection(rng);w.patterns[x.pattern]++;
      if(!x.success){if(x.h_dead_step>=0)w.h_dead[x.h_dead_step]++;continue;}w.success++;w.pattern_weights[x.pattern]+=x.weight;
      bool g127=x.ec==57&&x.es==3117,g129=x.ec==57&&x.es+x.edit_even==3118;
      int editposition=0;for(int i=0;i<114;i++)if(x.ids[i]==8){editposition=i+1;break;}
      const int editr=editposition%19;const bool bothanchors=editr!=4&&editr!=6&&editr!=13&&editr!=15;
      std::array<bool,NM> flags{};flags[0]=true;flags[1]=g127;flags[2]=g127&&x.en==16&&x.fn==1;
      flags[3]=g127&&((x.en==16&&x.fn==1)||(x.en==18&&x.fn==3));flags[4]=bothanchors;
      study::Draw draw;draw.ids=x.ids;
      if(w.validations<125){
        auto f=extract(draw);if(f.c[1][6]!=15||f.w[1][6]!=3||(f.g[1][0]==3)!=g127||(f.g[0][0]==3)!=g129||((f.c[0][6]==15&&f.w[0][6]==3)!=bothanchors))throw std::runtime_error("Uniform SIS direct feature mismatch");
        if(g127&&(((f.m[1][6]&15)==15)!=flags[2]))throw std::runtime_error("Uniform SIS natural matrix reduction mismatch");
        if(method=="sis")w.max_logweight_error=std::max(w.max_logweight_error,std::abs(std::log(x.weight)-x.log_weight));
        w.validations++;
      }
      bool need_score=scoremode==2||(scoremode==1&&(g127||g129));
      if(scoremode==3){
        auto boundfeatures=extract(draw,false);for(int v=0;v<2;v++)for(int c=0;c<406;c++)boundfeatures.m[v][c]=0xffffffffu;
        auto upper=scorer(boundfeatures);w.upper_gates++;
        for(int k=0;k<6;k++)need_score|=upper.value[k]>=observed.value[k]-1e-12;
      }
      if(need_score){
        auto features=extract(draw);auto scores=scorer(features);w.full_scores++;
        if(g127&&(((features.m[1][6]&15)==15)!=flags[2]))throw std::runtime_error("Production matrix reduction disagrees with full extractor");
        for(int k=0;k<6;k++)flags[5+k]=scores.value[k]>=observed.value[k];
        if(flags[6]&&w.example.empty()){
          std::ostringstream os;os<<"{\"weight\":"<<std::setprecision(17)<<x.weight<<",\"pattern\":"<<x.pattern<<",\"ids\":[";
          for(int i=0;i<114;i++){if(i)os<<',';os<<x.ids[i];}os<<"],\"scores\":";study::write_scores_json(os,scores);os<<'}';w.example=os.str();
        }
        if(flags[2]&&x.weight>w.metrics[2].maximum){
          std::ostringstream os;os<<"{\"weight\":"<<std::setprecision(17)<<x.weight<<",\"pattern\":"<<x.pattern<<",\"H_rows_in_sampling_order\":[";
          for(int k=0;k<7;k++){if(k)os<<',';os<<x.h_rows[k];}os<<"],\"ids\":[";
          for(int i=0;i<114;i++){if(i)os<<',';os<<x.ids[i];}os<<"],\"scores\":";study::write_scores_json(os,scores);os<<'}';w.max_target_example=os.str();
        }
      }
      for(int k=0;k<NM;k++)if(flags[k]){w.metrics[k].add(x.weight);w.pattern_metrics[x.pattern][k].add(x.weight);}
    }
    w.processed=end-begin;
    w.rng_calls=rng.calls;w.final_rng_state=rng.state;
  }
  Worker all;
  for(const auto&w:workers){
    all.processed+=w.processed;all.success+=w.success;all.validations+=w.validations;all.full_scores+=w.full_scores;all.upper_gates+=w.upper_gates;all.max_logweight_error=std::max(all.max_logweight_error,w.max_logweight_error);
    for(int k=0;k<NM;k++){all.metrics[k].hits+=w.metrics[k].hits;all.metrics[k].sum+=w.metrics[k].sum;all.metrics[k].sumsq+=w.metrics[k].sumsq;all.metrics[k].maximum=std::max(all.metrics[k].maximum,w.metrics[k].maximum);}
    for(int p=0;p<20;p++){
      all.patterns[p]+=w.patterns[p];all.pattern_weights[p]+=w.pattern_weights[p];
      for(int k=0;k<NM;k++){auto& a=all.pattern_metrics[p][k];const auto& b=w.pattern_metrics[p][k];a.hits+=b.hits;a.sum+=b.sum;a.sumsq+=b.sumsq;a.maximum=std::max(a.maximum,b.maximum);}
    }for(int k=0;k<8;k++)all.h_dead[k]+=w.h_dead[k];
  }
  if(all.processed!=n)throw std::runtime_error("Uniform pilot budget mismatch");
  double seconds=std::chrono::duration<double>(std::chrono::steady_clock::now()-start).count();
  std::ofstream out(output);out<<std::setprecision(21);
  out<<"{\"phase\":\""<<phase<<"\",\"method\":\""<<method<<"\",\"variant_anchor\":127,\"n\":"<<n<<",\"seed\":"<<seed<<",\"threads\":"<<actual<<",\"F_partial_matching_count\":"<<proposal.MF<<",\"F_marked_rejection_factor\":"<<proposal.f_marked_probability<<",\"score_mode\":"<<scoremode<<",\"tail_scope\":\"tail intersection with anchored CW127 and natural Geither, not the full tail\",\"successes\":"<<all.success<<",\"validations\":"<<all.validations<<",\"max_direct_product_vs_log_weight_error\":"<<all.max_logweight_error<<",\"full_score_evaluations\":"<<all.full_scores<<",\"matrix_upper_bound_gates\":"<<all.upper_gates<<",\"metrics\":";write_metrics(out,all.metrics);
  out<<",\"pattern_counts\": [";for(int p=0;p<20;p++){if(p)out<<',';out<<all.patterns[p];}out<<"],\"pattern_weight_sums\": [";for(int p=0;p<20;p++){if(p)out<<',';out<<all.pattern_weights[p];}out<<"],\"H_dead_prefix_counts\": [";for(int k=0;k<8;k++){if(k)out<<',';out<<all.h_dead[k];}out<<"],\"workers\": [";
  for(int t=0;t<actual;t++){
    if(t)out<<',';const auto&w=workers[t];out<<"{\"seed\":"<<w.seed<<",\"processed\":"<<w.processed<<",\"success\":"<<w.success<<",\"rng_calls\":"<<w.rng_calls<<",\"final_rng_state\":"<<w.final_rng_state<<",\"metrics\":";write_metrics(out,w.metrics);
    out<<",\"pattern_counts\": [";for(int p=0;p<20;p++){if(p)out<<',';out<<w.patterns[p];}out<<']';
    if(!w.max_target_example.empty())out<<",\"max_target_draw\":"<<w.max_target_example;out<<'}';
  }out<<"],\"by_pattern\": [";
  for(int p=0;p<20;p++){if(p)out<<',';out<<"{\"pattern\":"<<p<<",\"n\":"<<all.patterns[p]<<",\"metrics\":";write_metrics(out,all.pattern_metrics[p]);out<<'}';}out<<"],\"observed\":";study::write_scores_json(out,observed);
  out<<",\"examples\": [";bool first=true;for(auto&w:workers)if(!w.example.empty()){if(!first)out<<',';first=false;out<<w.example;}out<<"],\"seconds\":"<<seconds<<"}\n";
  std::cout<<method<<" n="<<n<<" success="<<all.success<<" seconds="<<seconds<<" fullscores="<<all.full_scores<<"\n";
  for(int k=0;k<NM;k++){auto&m=all.metrics[k];std::cout<<metric_names[k]<<" hits="<<m.hits<<" estimate="<<std::setprecision(12)<<double(m.sum/n)<<" ESS="<<double(m.sumsq?m.sum*m.sum/m.sumsq:0)<<"\n";}
}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;} }
