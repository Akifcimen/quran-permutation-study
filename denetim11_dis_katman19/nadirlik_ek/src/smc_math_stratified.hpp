#ifndef RARE_SMC_MATH_STRATIFIED_HPP
#define RARE_SMC_MATH_STRATIFIED_HPP

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

namespace rare {
struct WeightSummary { double log_mean=0,ess=0,max_weight=0; };

// API-compatible alternative to smc_math.hpp; include exactly one header.
// One independent uniform draw in each of N equal CDF strata. Conditional
// expected offspring counts are N times the normalized particle weights,
// which is the unbiased-resampling property needed by the SMC normalizer.
// Equal weights retain each particle exactly once (apart from harmless order).
template<class Particle,class RNG,class LogWeight>
WeightSummary reweight_resample(std::vector<Particle>& particles,RNG& rng,
                               LogWeight log_weight) {
 const int n=int(particles.size());
 if(n<1)throw std::runtime_error("empty SMC population");
 std::vector<double> logs(n),weights(n);
 double top=-INFINITY;
 // Preserve the production API's exactly-once, in-order callback contract.
 for(int i=0;i<n;i++){logs[i]=log_weight(particles[i]);top=std::max(top,logs[i]);}
 if(!std::isfinite(top))throw std::runtime_error("all weights zero");
 double sum=0,sumsq=0,mx=0;
 for(int i=0;i<n;i++){
  weights[i]=std::exp(logs[i]-top);
  sum+=weights[i];sumsq+=weights[i]*weights[i];mx=std::max(mx,weights[i]);
 }
 WeightSummary out{top+std::log(sum/n),sum*sum/sumsq,mx/sum};
 for(int i=1;i<n;i++)weights[i]+=weights[i-1];
 const auto previous=particles;
 int ancestor=0;
 for(int i=0;i<n;i++){
  const double lower=(double(i)*sum)/n;
  const double upper=(double(i+1)*sum)/n;
  double u=((double(i)+rng.unif())*sum)/n;
  // Floating addition can round a [0,1) draw to the stratum's upper endpoint.
  // Keep it inside its own half-open stratum, including for equal weights.
  u=std::max(lower,std::min(u,std::nextafter(upper,lower)));
  while(ancestor<n-1&&u>=weights[ancestor])++ancestor;
  particles[i]=previous[ancestor];
 }
 return out;
}
} // namespace rare
#endif
