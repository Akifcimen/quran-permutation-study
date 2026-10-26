#include "sampler.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <unordered_set>

int main(int argc,char**argv){try{
 std::string root=argc>1?argv[1]:".";std::array<int,study::N> base;
 std::ifstream in(root+"/data/counts.txt");for(auto&a:base)in>>a;if(!in)throw std::runtime_error("missing counts");
 block7::Sampler sampler(base,2,7);
 std::array<std::uint64_t,6> brute{};std::uint64_t union_count=0,event_count=0;
 for(int left=0;left<720;++left)for(int right=0;right<720;++right){
  const auto&a=sampler.profiles[0][left];const auto&b=sampler.profiles[1][right];
  if(a.overlap||b.overlap)continue;
  bool selected=false;for(int j=0;j<6;++j)if(a.union_count[j]+b.union_count[j]==7){++brute[j];selected=true;}
  if(selected){++union_count;event_count+=a.ids[0]==0;}
 }
 std::uint64_t ranked=0;
 for(int anchor=0;anchor<6;++anchor){
  if(sampler.total[anchor]!=block7::Integer(brute[anchor]))throw std::runtime_error("Toy DP differs from enumeration");
  std::unordered_set<unsigned> seen;
  for(std::uint64_t rank=0;rank<brute[anchor];++rank){
   const auto s=sampler.unrank(anchor,block7::Integer(rank));
   unsigned key=720*s.profile[0]+s.profile[1];
   if(!seen.insert(key).second)throw std::runtime_error("Unranking duplicate");
   auto d=sampler.full_draw(s);unsigned membership=0;
   for(int v=0;v<2;++v)for(int r=1;r<=3;++r){
    int count=0,overlap=0;
    for(int pos=0;pos<12;++pos){int n=pos+1,id=d.ids[pos],a=(v&&id==8)?127:base[id];
     bool f=(a-r*n)%7==0,h=(a+r*n)%7==0;count+=f||h;overlap+=f&&h;
    }
    if(count==7&&!overlap)membership|=1u<<(3*v+r-1);
   }
   if(membership!=s.membership)throw std::runtime_error("Raw toy membership mismatch");
   ++ranked;
  }
 }
 constexpr int reps=200000;
 study::RNG rng(UINT64_C(0x7bc1ac5fa00e7291));int accepted=0,hits=0;
 long double sum=0,sumsq=0;
 for(int i=0;i<reps;++i){
  auto s=sampler.draw(rng);bool keep=rng.bounded(__builtin_popcount(s.membership))==0;
  bool event=sampler.profiles[0][s.profile[0]].ids[0]==0;
  accepted+=keep;hits+=keep&&event;long double value=event?s.weight:0;sum+=value;sumsq+=value*value;
 }
 long double denom=block7::as_long_double(sampler.count_sum),space=720.L*720;
 long double expected_accept=union_count/denom,expected_hit=event_count/denom;
 long double za=(accepted/(long double)reps-expected_accept)/std::sqrt(expected_accept*(1-expected_accept)/reps);
 long double zh=(hits/(long double)reps-expected_hit)/std::sqrt(expected_hit*(1-expected_hit)/reps);
 long double weighted_se=std::sqrt((sumsq-sum*sum/reps)/(reps-1)/reps);
 long double zw=(sum/reps-event_count/space)/weighted_se;
 if(std::abs(za)>6||std::abs(zh)>6||std::abs(zw)>6)throw std::runtime_error("Toy Monte Carlo discrepancy over6SE");
 std::cout<<std::setprecision(20)<<"{\"status\":\"passed\",\"brute_states\":518400,\"all_anchor_ranks_verified\":"<<ranked<<",\"union_count\":"<<union_count<<",\"event_count\":"<<event_count<<",\"mixture_total_count\":\""<<sampler.count_sum<<"\",\"mixture_repetitions\":"<<reps<<",\"acceptance_standardized_difference\":"<<za<<",\"event_standardized_difference\":"<<zh<<",\"weighted_event_standardized_difference\":"<<zw<<"}\n";
 return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
