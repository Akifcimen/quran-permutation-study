// Two-parameter guide/temperature integration with the production math core.
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

struct State { unsigned bits=0;int distance=0;double guide=0,log_gamma=0; };
struct Moments {
 long double sum=0,sumsq=0;
 void add(double x){sum+=x;sumsq+=(long double)x*x;}
 double mean(int n)const{return double(sum/n);}
 double se(int n)const{return std::sqrt(double((sumsq-sum*sum/n)/(n-1)/n));}
};
int distance(unsigned x,bool two){int k=__builtin_popcount(x);return two?std::min(k,10-k):10-k;}
double guide(int d,double lambda){
 double hard=10-d;
 return std::max({hard,10-lambda*((d+1)/2),hard+std::max(0.,2-lambda)*(d%3)});
}
State evaluate(unsigned bits,bool two,double lambda,double beta){
 State x;x.bits=bits;x.distance=distance(bits,two);x.guide=guide(x.distance,lambda);
 x.log_gamma=-beta*std::max(0.,10-x.guide);return x;
}
int main(){
 constexpr int repetitions=8192,population=64;
 const std::array<std::pair<double,double>,9> schedule={{{.1,.2},{.25,.5},{.5,.8},{.7,1.1},{1.,1.4},{1.25,1.8},{1.5,2.2},{1.8,2.6},{2.,3.}}};
 int failures=0;bool comma=false;std::cout<<std::setprecision(17)<<"[";
 for(bool two:{false,true})for(int moves:{0,1,4}){
  Moments normalizer,event,secondary;int zero=0;
  for(int rep=0;rep<repetitions;++rep){
   study::RNG seed(UINT64_C(0x1a23d912c7862991)^(UINT64_C(0xd1b54a32d192ed03)*(rep+1))^(UINT64_C(0x29cf1ba2c3e74917)*(1+moves+10*two)));
   study::RNG rng(seed.next());std::vector<State> particles(population);
   for(auto&p:particles)p=evaluate(rng.bounded(1024),two,0,0);
   double logz=0;
   for(auto[lambda,beta]:schedule){
    std::vector<double> increments;increments.reserve(population);
    for(auto&p:particles){
     p.guide=guide(p.distance,lambda);
     double next=-beta*std::max(0.,10-p.guide);
     increments.push_back(next-p.log_gamma);p.log_gamma=next;
    }
    std::size_t index=0;
    auto summary=rare::reweight_resample(particles,rng,[&](const State&){return increments.at(index++);});
    if(index!=particles.size())return 2;
    logz+=summary.log_mean;
    for(auto&p:particles)for(int j=0;j<moves;++j){
     auto candidate=evaluate(p.bits^(1u<<rng.bounded(10)),two,lambda,beta);
     double loga=candidate.log_gamma-p.log_gamma;
     if(loga>=0||std::log(rng.unif())<loga)p=candidate;
    }
   }
   int hits=0;double corrected=0;
   for(const auto&p:particles){hits+=p.distance==0;if(p.distance<=2)corrected+=std::exp(-p.log_gamma);}
   double z=std::exp(logz);normalizer.add(z);event.add(z*hits/population);secondary.add(z*corrected/population);zero+=hits==0;
  }
  double exact_z=0,exact_p=0,exact_secondary=0;
  for(unsigned bits=0;bits<1024;++bits){auto x=evaluate(bits,two,2,3);exact_z+=std::exp(x.log_gamma)/1024.;exact_p+=(x.distance==0)/1024.;exact_secondary+=(x.distance<=2)/1024.;}
  if(comma)std::cout<<",";comma=true;
  std::cout<<"{\"two_modes\":"<<(two?"true":"false")<<",\"moves\":"<<moves<<",\"repetitions\":"<<repetitions<<",\"population\":"<<population<<",\"zero_estimates\":"<<zero<<",\"checks\":[";
  std::array<Moments,3> moments={normalizer,event,secondary};std::array<double,3> exact={exact_z,exact_p,exact_secondary};
  const char* names[3]={"soft_normalizer","primary_event","secondary_importance_event"};
  for(int i=0;i<3;++i){
   if(i)std::cout<<",";double mean=moments[i].mean(repetitions),se=moments[i].se(repetitions);bool pass=std::abs(mean-exact[i])<=6*se+1e-12;failures+=!pass;
   std::cout<<"{\"quantity\":\""<<names[i]<<"\",\"exact\":"<<exact[i]<<",\"mean\":"<<mean<<",\"standard_error\":"<<se<<",\"standardized_difference\":"<<(mean-exact[i])/se<<",\"pass\":"<<(pass?"true":"false")<<"}";
  }
  std::cout<<"]}";
 }
 std::cout<<"]\n";return failures?1:0;
}
