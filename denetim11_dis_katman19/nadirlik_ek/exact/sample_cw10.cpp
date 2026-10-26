#include "cw10_sampler.hpp"
#include "../../src/scorer.hpp"
#include <omp.h>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <sstream>

constexpr int NC10=16;
const char* counter_names[NC10]={"Ecount57","G129","G127","G_either","G_both","G129_M_view0","G127_M_view0","G129_M_any","G127_M_any","G_either_M_any","tail_uniform_intersect_Geither","tail_mixture_intersect_Geither","tail_literal_intersect_Geither","tail_discussion_intersect_Geither","tail_anchored19_intersect_Geither","tail_frequency_intersect_Geither"};
struct Worker {
  std::array<uint64_t,NC10> counts{};
  std::array<std::array<uint64_t,NC10>,16> by_mask{};
  std::array<uint64_t,16> mask_n{};
  uint64_t seed=0,processed=0,calls=0,final_state=0,validated=0;
  std::string example;
};
void write_counts(std::ostream& out,const std::array<uint64_t,NC10>& c){
  out<<'{';for(int k=0;k<NC10;k++){if(k)out<<',';out<<'"'<<counter_names[k]<<"\":"<<c[k];}out<<'}';
}
int main(int argc,char**argv){try{
  const std::string root=argc>1?argv[1]:".";
  const uint64_t n=argc>2?std::stoull(argv[2]):100000;
  const uint64_t seed=argc>3?std::stoull(argv[3]):202610250751ULL;
  const int threads=argc>4?std::stoi(argv[4]):8;
  const std::string outpath=argc>5?argv[5]:root+"/rare_event/exact/block6_C10W_pilot.json";
  const auto start=std::chrono::steady_clock::now();
  exact_block6_c10::Sampler sampler(root+"/rare_event/exact/block6_C10W_sampler.bin");
  MatrixEngine mat;std::array<int,study::N> base{};
  {std::ifstream in(root+"/data/counts.txt");for(auto& x:base)in>>x;if(!in)throw std::runtime_error("Missing counts");}
  study::Extractor extract(base,mat);study::Hist hist;hist.load(root+"/results/train_block6.bin");study::Scorer scorer(hist);
  study::Draw observed;std::iota(observed.ids.begin(),observed.ids.end(),0);const auto obs=scorer(extract(observed));
  uint8_t m_any[2][19][19]{},m0[2][19][19]{};
  for(int v=0;v<2;v++)for(int x=0;x<19;x++)for(int y=0;y<19;y++){
    int t=v?2:4;uint32_t bits=mat.eval(6,{x,t-x,-x,x},{y,-y,-y,t+y});m0[v][x][y]=(bits&15)==15;
    for(int view=0;view<8;view++,bits>>=4)m_any[v][x][y]|=(bits&15)==15;
  }
  std::vector<Worker> workers(threads);omp_set_dynamic(0);omp_set_num_threads(threads);int actual_threads=0;
  #pragma omp parallel
  {
    const int tid=omp_get_thread_num(),team=omp_get_num_threads();
    #pragma omp single
    actual_threads=team;
    auto& worker=workers[tid];study::RNG mix(seed^(0xd1b54a32d192ed03ULL*uint64_t(tid+1)));
    worker.seed=mix.next();study::RNG rng(worker.seed);const uint64_t begin=n*tid/team,end=n*(tid+1)/team;
    for(uint64_t j=begin;j<end;j++){
      auto s=sampler.draw(rng,true,&worker.calls);
      if(s.hn!=s.h_target||s.hd!=0||s.overlap!=0||(s.c_mask&10)!=10)throw std::runtime_error("C10W sampler constraint failure");
      const bool g[2]={s.ec==57&&s.es==3118,s.ec==57&&s.es-s.edit_even==3117};
      if(j-begin<125){
        study::Draw d;d.ids=s.ids;const auto f=extract(d);
        if(f.c[0][6]!=s.c_mask||f.c[1][6]!=s.c_mask||f.w[0][6]!=3||f.w[1][6]!=3||
           ((f.g[0][0]==3)!=g[0])||((f.g[1][0]==3)!=g[1]))throw std::runtime_error("Direct C10W feature validation failed");
        worker.validated++;
      }
      std::array<bool,NC10> flags{};
      flags[0]=s.ec==57;flags[1]=g[0];flags[2]=g[1];flags[3]=g[0]||g[1];flags[4]=g[0]&&g[1];
      flags[5]=g[0]&&m0[0][s.en][s.fn];flags[6]=g[1]&&m0[1][s.en][s.fn];
      flags[7]=g[0]&&m_any[0][s.en][s.fn];flags[8]=g[1]&&m_any[1][s.en][s.fn];flags[9]=flags[7]||flags[8];
      if(flags[3]){
        study::Draw d;d.ids=s.ids;const auto f=extract(d);const auto scores=scorer(f);
        for(int v=0;v<2;v++)if(g[v]){
          bool any=false;auto bits=f.m[v][6];for(int r=0;r<8;r++,bits>>=4)any|=(bits&15)==15;
          if(any!=bool(m_any[v][s.en][s.fn])||((f.m[v][6]&15)==15)!=bool(m0[v][s.en][s.fn]))throw std::runtime_error("C10W matrix reduction mismatch");
        }
        for(int k=0;k<6;k++)flags[10+k]=scores.value[k]>=obs.value[k];
        if(worker.example.empty()&&flags[11]){
          std::ostringstream os;os<<"{\"Cmask\":"<<s.c_mask<<",\"ids\":[";
          for(int i=0;i<114;i++){if(i)os<<',';os<<s.ids[i];}os<<"],\"scores\":";study::write_scores_json(os,scores);os<<'}';worker.example=os.str();
        }
      }
      worker.mask_n[s.c_mask]++;
      for(int k=0;k<NC10;k++){worker.counts[k]+=flags[k];worker.by_mask[s.c_mask][k]+=flags[k];}
    }
    worker.processed=end-begin;worker.final_state=rng.state;
  }
  Worker total;
  for(const auto& w:workers){total.processed+=w.processed;total.validated+=w.validated;for(int k=0;k<NC10;k++)total.counts[k]+=w.counts[k];for(int mask=0;mask<16;mask++){total.mask_n[mask]+=w.mask_n[mask];for(int k=0;k<NC10;k++)total.by_mask[mask][k]+=w.by_mask[mask][k];}}
  if(total.processed!=n)throw std::runtime_error("CW10 production count mismatch");
  const double seconds=std::chrono::duration<double>(std::chrono::steady_clock::now()-start).count();
  std::ofstream out(outpath);out<<std::setprecision(17);
  out<<"{\"model\":\"block6\",\"condition\":\"natural (Cmask&10)==10 and W3\",\"n\":"<<n<<",\"seed\":"<<seed<<",\"threads\":"<<actual_threads<<",\"integer_conditional_sampler\":true,\"count\":\""<<sampler.total<<"\",\"workers\":[";
  for(int i=0;i<actual_threads;i++){
    if(i)out<<',';const auto& w=workers[i];out<<"{\"seed\":"<<w.seed<<",\"processed\":"<<w.processed<<",\"rng_calls\":"<<w.calls<<",\"final_rng_state\":"<<w.final_state<<'}';
  }
  out<<"],\"counts\":";write_counts(out,total.counts);out<<",\"by_Cmask\":{";
  bool first=true;for(int mask=0;mask<16;mask++)if(total.mask_n[mask]){
    if(!first)out<<',';first=false;out<<'"'<<mask<<"\":{\"n\":"<<total.mask_n[mask]<<",\"counts\":";write_counts(out,total.by_mask[mask]);out<<'}';
  }
  out<<"},\"direct_feature_validations\":"<<total.validated<<",\"matrix_reduction_validated_on_every_G_draw\":true,\"observed\":";study::write_scores_json(out,obs);
  out<<",\"examples\":[";first=true;for(auto& w:workers)if(!w.example.empty()){if(!first)out<<',';first=false;out<<w.example;}out<<']';
  out<<",\"seconds\":"<<seconds<<",\"scope\":\"Natural event probabilities and lower-bound contributions to each full score tail; not complete tail probabilities\"}\n";
  std::cout<<"C10Wconditional n="<<n<<" seconds="<<seconds<<" samples_per_second="<<n/seconds<<"\n";
  for(int k=0;k<NC10;k++)std::cout<<counter_names[k]<<' '<<total.counts[k]<<"\n";
  for(int mask=0;mask<16;mask++)if(total.mask_n[mask])std::cout<<"Cmask "<<mask<<" n="<<total.mask_n[mask]<<" primarytail="<<total.by_mask[mask][11]<<" G127M0="<<total.by_mask[mask][6]<<"\n";
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;} }
