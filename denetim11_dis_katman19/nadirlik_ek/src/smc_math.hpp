#ifndef RARE_SMC_MATH_HPP
#define RARE_SMC_MATH_HPP
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>
namespace rare {
struct WeightSummary { double log_mean=0,ess=0,max_weight=0; };
template<class Particle,class RNG,class LogWeight>
WeightSummary reweight_resample(std::vector<Particle>& particles,RNG& rng,LogWeight log_weight) {
 const int n=int(particles.size());if(n<1)throw std::runtime_error("empty SMC population");
 std::vector<double> logs(n),weights(n);double top=-INFINITY;
 for(int i=0;i<n;i++){logs[i]=log_weight(particles[i]);top=std::max(top,logs[i]);}
 if(!std::isfinite(top))throw std::runtime_error("all weights zero");
 double sum=0,sumsq=0,mx=0;
 for(int i=0;i<n;i++){weights[i]=std::exp(logs[i]-top);sum+=weights[i];sumsq+=weights[i]*weights[i];mx=std::max(mx,weights[i]);}
 WeightSummary out{top+std::log(sum/n),sum*sum/sumsq,mx/sum};
 for(int i=1;i<n;i++)weights[i]+=weights[i-1];
 auto previous=particles;
 for(int i=0;i<n;i++){
  double u=rng.unif()*sum;auto it=std::upper_bound(weights.begin(),weights.end(),u);
  int k=std::min(n-1,int(it-weights.begin()));particles[i]=previous[k];
 }
 return out;
}
}
#endif

