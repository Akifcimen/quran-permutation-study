#include "../src/smc_math_stratified.hpp"
#include "../../src/sampler.hpp"
#include <array>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

struct UpperUniform {double unif(){return 1.-0x1.0p-53;}};
int main(){
 int failures=0,identity_cases=0;
 for(int n:{1,2,3,7,64,256,513})for(double shift:{0.,-1000.}){
  std::vector<int> p(n);std::iota(p.begin(),p.end(),0);UpperUniform rng;int calls=0;
  auto s=rare::reweight_resample(p,rng,[&](int identity){if(identity!=calls++)++failures;return shift;});
  for(int i=0;i<n;++i)failures+=p[i]!=i;
  failures+=std::abs(s.log_mean-shift)>1e-12;
  failures+=std::abs(s.ess-n)>1e-10;
  failures+=std::abs(s.max_weight-1./n)>1e-12;
  ++identity_cases;
 }
 constexpr int n=7,reps=200000;
 const std::array<double,n> weights={0.,1.,2.,3.,7.,0.,11.};
 std::array<long double,n> sums{},squares{};
 study::RNG rng(UINT64_C(0xe39b1a4d6f027895));
 for(int r=0;r<reps;++r){
  std::vector<int> p(n);std::iota(p.begin(),p.end(),0);
  rare::reweight_resample(p,rng,[&](int i){return weights[i]>0?std::log(weights[i])-1000:-INFINITY;});
  std::array<int,n> count{};for(int i:p)++count[i];
  for(int i=0;i<n;++i){sums[i]+=count[i];squares[i]+=(long double)count[i]*count[i];}
 }
 double total=std::accumulate(weights.begin(),weights.end(),0.);
 std::cout<<std::setprecision(17)<<"{\"identity_cases\":"<<identity_cases<<",\"offspring_repetitions\":"<<reps<<",\"offspring_checks\":[";
 for(int i=0;i<n;++i){
  double expected=n*weights[i]/total,mean=double(sums[i]/reps);
  double se=std::sqrt(double((squares[i]-sums[i]*sums[i]/reps)/(reps-1)/reps));
  bool pass=std::abs(mean-expected)<=6*se+1e-12;failures+=!pass;
  if(i)std::cout<<",";
  std::cout<<"{\"identity\":"<<i<<",\"expected_offspring\":"<<expected<<",\"mean_offspring\":"<<mean<<",\"standard_error\":"<<se<<",\"pass\":"<<(pass?"true":"false")<<"}";
 }
 std::cout<<"],\"failures\":"<<failures<<"}\n";
 return failures?1:0;
}
