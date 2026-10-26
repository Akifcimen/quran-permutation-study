#include "cw_sampler.hpp"
#include "../../src/scorer.hpp"
#include <omp.h>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <sstream>

int main(int argc,char** argv){try{
  const std::string root=argc>1?argv[1]:".";
  const uint64_t n=argc>2?std::stoull(argv[2]):100000;
  const uint64_t seed=argc>3?std::stoull(argv[3]):202610250601ULL;
  const int threads=argc>4?std::stoi(argv[4]):8;
  const std::string outpath=argc>5?argv[5]:root+"/rare_event/exact/block6_CW_pilot.json";
  const auto start=std::chrono::steady_clock::now();
  exact_block6::Sampler sampler(root+"/rare_event/exact/block6_CW_sampler.bin");
  MatrixEngine mat;std::array<int,study::N> base{};
  {std::ifstream in(root+"/data/counts.txt");for(auto& x:base)in>>x;if(!in)throw std::runtime_error("Missing counts");}
  study::Extractor extract(base,mat);
  study::Hist hist;hist.load(root+"/results/train_block6.bin");study::Scorer scorer(hist);
  study::Draw observed;std::iota(observed.ids.begin(),observed.ids.end(),0);
  const auto obs=scorer(extract(observed));
  // Given G=3 and C=15, these two residues determine the two matrices.
  uint8_t m_any[2][19][19]{},m0[2][19][19]{};
  int possible[2]{},possible0[2]{};
  for(int v=0;v<2;v++)for(int x=0;x<19;x++)for(int y=0;y<19;y++){
    int t=v?2:4;
    uint32_t bits=mat.eval(6,{x,t-x,-x,x},{y,-y,-y,t+y});
    m0[v][x][y]=(bits&15)==15;possible0[v]+=m0[v][x][y];
    for(int view=0;view<8;view++,bits>>=4)m_any[v][x][y]|=(bits&15)==15;
    possible[v]+=m_any[v][x][y];
  }
  constexpr int NC=15;
  const char* names[NC]={"Ecount57","G129","G127","G_either","G_both","G129_M_view0","G127_M_view0","G129_M_any","G127_M_any","G_either_M_any","tail_uniform_intersect_Geither","tail_mixture_intersect_Geither","tail_literal_intersect_Geither","tail_discussion_intersect_Geither","tail_anchored19_intersect_Geither"};
  std::vector<std::array<uint64_t,NC>> local(threads);
  std::vector<uint64_t> frequency_tail(threads),seeds(threads),validated(threads);
  std::vector<std::string> examples(threads);
  omp_set_dynamic(0);omp_set_num_threads(threads);
  int actual_threads=0;
  #pragma omp parallel
  {
    int tid=omp_get_thread_num(),team=omp_get_num_threads();
    #pragma omp single
    actual_threads=team;
    study::RNG mix(seed^(0xd1b54a32d192ed03ULL*uint64_t(tid+1)));
    seeds[tid]=mix.next();study::RNG rng(seeds[tid]);
    uint64_t begin=n*tid/team,end=n*(tid+1)/team;
    for(uint64_t j=begin;j<end;j++){
      // Retaining all114IDs adds negligible cost; it permits independent
      // arithmetic validation and full scorer evaluation for G-containing hits.
      auto s=sampler.draw(rng,true);
      if(s.hn!=7||s.hd!=0||s.overlap!=0)throw std::runtime_error("W sampler error");
      bool g[2]={s.ec==57&&s.es==3118,s.ec==57&&s.es-s.edit_even==3117};
      if(j-begin<125){
        study::Draw d;d.ids=s.ids;const auto f=extract(d);
        if(f.c[0][6]!=15||f.c[1][6]!=15||f.w[0][6]!=3||f.w[1][6]!=3||
           ((f.g[0][0]==3)!=g[0])||((f.g[1][0]==3)!=g[1]))throw std::runtime_error("Direct feature validation failed");
        for(int v=0;v<2;v++)if(g[v]){
          bool any=false;auto bits=f.m[v][6];for(int r=0;r<8;r++,bits>>=4)any|=(bits&15)==15;
          if(any!=bool(m_any[v][s.en][s.fn])||((f.m[v][6]&15)==15)!=bool(m0[v][s.en][s.fn]))throw std::runtime_error("Matrix reduction validation failed");
        }
        validated[tid]++;
      }
      auto& c=local[tid];c[0]+=s.ec==57;c[1]+=g[0];c[2]+=g[1];c[3]+=g[0]||g[1];c[4]+=g[0]&&g[1];
      const bool a0=g[0]&&m_any[0][s.en][s.fn],a1=g[1]&&m_any[1][s.en][s.fn];
      c[5]+=g[0]&&m0[0][s.en][s.fn];c[6]+=g[1]&&m0[1][s.en][s.fn];c[7]+=a0;c[8]+=a1;c[9]+=a0||a1;
      if(g[0]||g[1]){
        study::Draw d;d.ids=s.ids;const auto scores=scorer(extract(d));
        for(int k=0;k<5;k++)c[10+k]+=scores.value[k]>=obs.value[k];
        frequency_tail[tid]+=scores.value[5]>=obs.value[5];
        if(examples[tid].empty()&&scores.value[1]>=obs.value[1]){
          std::ostringstream os;os<<"{\"ids\":[";for(int i=0;i<114;i++){if(i)os<<',';os<<s.ids[i];}os<<"],\"scores\":";study::write_scores_json(os,scores);os<<'}';examples[tid]=os.str();
        }
      }
    }
  }
  std::ofstream out(outpath);out<<std::setprecision(17);
  out<<"{\"model\":\"block6\",\"condition\":\"natural C15 and W3\",\"n\":"<<n<<",\"seed\":"<<seed<<",\"threads\":"<<actual_threads<<",\"worker_seeds\":[";
  for(int i=0;i<actual_threads;i++){if(i)out<<',';out<<seeds[i];}
  out<<"],\"integer_conditional_sampler\":true,\"count\":\""<<sampler.total<<"\",\"counts\":{";
  for(int i=0;i<NC;i++){uint64_t value=0;for(auto& a:local)value+=a[i];if(i)out<<',';out<<'"'<<names[i]<<"\":"<<value;}
  out<<",\"tail_frequency_intersect_Geither\":"<<std::accumulate(frequency_tail.begin(),frequency_tail.end(),uint64_t(0));
  out<<"},\"matrix_residue_pairs_any_view\":["<<possible[0]<<','<<possible[1]<<"],\"matrix_residue_pairs_view0\":["<<possible0[0]<<','<<possible0[1]<<"],\"direct_feature_validations\":"<<std::accumulate(validated.begin(),validated.end(),uint64_t(0));
  out<<",\"observed\":";study::write_scores_json(out,obs);
  out<<",\"examples\":[";bool first=true;for(auto& ex:examples)if(!ex.empty()){if(!first)out<<',';out<<ex;first=false;}out<<']';
  const double seconds=std::chrono::duration<double>(std::chrono::steady_clock::now()-start).count();
  out<<",\"seconds\":"<<seconds<<",\"scope\":\"Natural event probabilities and a lower-bound contribution to the full score tail, not the full tail\"}\n";
  std::cout<<"CWconditional n="<<n<<" seconds="<<seconds<<" samples_per_second="<<n/seconds<<"\n";
  for(int i=0;i<NC;i++){uint64_t value=0;for(auto& a:local)value+=a[i];std::cout<<names[i]<<" "<<value<<"\n";}
  std::cout<<"matrixpossible "<<possible[0]<<" "<<possible[1]<<"\n";
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;} }
