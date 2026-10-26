// Replay only the random-rank draws from the C15 production. Unranking never
// consumes RNG values, so this recovers exact consumed counter intervals.
#include "cw_sampler.hpp"
#include <omp.h>
#include <iostream>
int main(int argc,char**argv){
  if(argc!=6)return 2;
  exact_block6::Sampler sampler(argv[1]);const uint64_t n=std::stoull(argv[2]),seed=std::stoull(argv[3]);const int nt=std::stoi(argv[4]);
  std::vector<uint64_t> calls(nt),starts(nt),ends(nt),draws(nt);omp_set_dynamic(0);omp_set_num_threads(nt);
  #pragma omp parallel
  {
    int t=omp_get_thread_num(),team=omp_get_num_threads();study::RNG mix(seed^(0xd1b54a32d192ed03ULL*uint64_t(t+1)));
    starts[t]=mix.next();study::RNG rng(starts[t]);const uint64_t count=n*(t+1)/team-n*t/team;draws[t]=count;
    for(uint64_t j=0;j<count;j++)for(;;){
      exact_block6::U256 x;for(unsigned i=0;i<(sampler.bits+63)/64;i++){x.w[i]=rng.next();calls[t]++;}
      if(sampler.bits%64)x.w[sampler.bits/64]&=(uint64_t(1)<<(sampler.bits%64))-1;
      if(x<sampler.total)break;
    }
    ends[t]=rng.state;
  }
  std::ofstream out(argv[5]);out<<"{\"n\":"<<n<<",\"seed\":"<<seed<<",\"workers\":[";
  for(int t=0;t<nt;t++){if(t)out<<',';out<<"{\"seed\":"<<starts[t]<<",\"processed\":"<<draws[t]<<",\"rng_calls\":"<<calls[t]<<",\"final_rng_state\":"<<ends[t]<<'}';}out<<"],\"method\":\"Exact replay of all random rank draws; unranking consumes no RNG words\"}\n";
}
