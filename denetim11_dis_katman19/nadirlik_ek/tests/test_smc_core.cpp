// Known finite-state checks using the identical production resampling core.
#ifdef RARE_USE_STRATIFIED
#include "../src/smc_math_stratified.hpp"
#else
#include "../src/smc_math.hpp"
#endif
#include "../../src/sampler.hpp"
#include <array>
#include <iomanip>
#include <iostream>
#include <vector>

struct State { unsigned bits=0; int distance=0; };
struct Moments {
  long double sum=0,sumsq=0;
  void add(double x){sum+=x;sumsq+=(long double)x*x;}
  double mean(int n)const{return double(sum/n);}
  double se(int n)const{return std::sqrt(double((sumsq-sum*sum/n)/(n-1)/n));}
};

int distance(unsigned bits,bool two_modes){
  int k=__builtin_popcount(bits);
  return two_modes?std::min(k,10-k):10-k;
}

int main(){
  constexpr int repetitions=8192,population=64;
  const std::array<double,9> betas={.1,.25,.5,.8,1.1,1.5,2.,2.5,3.};
  int failures=0;
  std::cout<<std::setprecision(17)<<"[";
  bool comma=false;
  for(bool multimodal:{false,true})for(int moves:{0,1,4}){
    Moments normalizer,event,secondary;
    int zero=0;
    for(int rep=0;rep<repetitions;++rep){
      study::RNG seed(UINT64_C(0x7ab35c912c862dd1)^(UINT64_C(0xd1b54a32d192ed03)*(rep+1))^
                      (UINT64_C(0x99ef19a277e74929)*(1+moves+10*multimodal)));
      study::RNG rng(seed.next());
      std::vector<State> particles(population);
      for(auto& p:particles){p.bits=rng.bounded(1024);p.distance=distance(p.bits,multimodal);}
      double logz=0,old=0;
      for(double beta:betas){
        auto w=rare::reweight_resample(particles,rng,[&](const State&p){return-(beta-old)*p.distance;});
        logz+=w.log_mean;
        for(auto&p:particles)for(int j=0;j<moves;++j){
          unsigned candidate=p.bits^(1u<<rng.bounded(10));
          int d=distance(candidate,multimodal);
          double a=-beta*(d-p.distance);
          if(a>=0||std::log(rng.unif())<a)p={candidate,d};
        }
        old=beta;
      }
      int hits=0;
      double importance_sum=0;
      for(const auto&p:particles){
        hits+=p.distance==0;
        if(p.distance<=2)importance_sum+=std::exp(betas.back()*p.distance);
      }
      double z=std::exp(logz),prob=z*hits/population;
      normalizer.add(z);event.add(prob);secondary.add(z*importance_sum/population);
      zero+=hits==0;
    }
    double exact_z=0,exact_event=0,exact_secondary=0;
    for(unsigned x=0;x<1024;++x){int d=distance(x,multimodal);exact_z+=std::exp(-betas.back()*d)/1024.;exact_event+=(d==0)/1024.;exact_secondary+=(d<=2)/1024.;}
    if(comma)std::cout<<",";comma=true;
    std::cout<<"{\"two_modes\":"<<(multimodal?"true":"false")<<",\"moves\":"<<moves<<",\"repetitions\":"<<repetitions<<",\"population\":"<<population<<",\"zero_estimates\":"<<zero<<",\"checks\":[";
    std::array<Moments,3> mm={normalizer,event,secondary};
    std::array<double,3> exact={exact_z,exact_event,exact_secondary};
    const char* names[3]={"soft_normalizer","primary_event","secondary_importance_event"};
    for(int j=0;j<3;++j){
      if(j)std::cout<<",";
      double mean=mm[j].mean(repetitions),se=mm[j].se(repetitions);
      double standardized=(mean-exact[j])/se;
      bool pass=std::abs(mean-exact[j])<=6*se+1e-12;
      failures+=!pass;
      std::cout<<"{\"quantity\":\""<<names[j]<<"\",\"exact\":"<<exact[j]<<",\"mean\":"<<mean<<",\"standard_error\":"<<se<<",\"standardized_difference\":"<<standardized<<",\"pass\":"<<(pass?"true":"false")<<"}";
    }
    std::cout<<"]}";
  }
  std::cout<<"]\n";
  return failures?1:0;
}
