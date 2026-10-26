#ifndef BLOCK7_CONDITIONAL_MIXTURE_SAMPLER_HPP
#define BLOCK7_CONDITIONAL_MIXTURE_SAMPLER_HPP

#include "../exact/cw_sampler.hpp"
#include <array>
#include <cstdint>
#include <numeric>
#include <vector>

namespace block7 {
using Integer=exact_block6::U256; // independently validated exact192-bit integer
constexpr int A=6,MAX_BLOCKS=19,TARGET=7;
struct Profile {
 std::array<std::uint8_t,6> ids{},union_count{};
 std::array<std::uint16_t,3> g_sum{};
 std::array<std::uint8_t,3> g_count{};
 std::uint8_t edit_mask=0,overlap=0;
};
struct Choice {Integer cumulative,child_count;int used=0;};
struct Summary {
 std::array<std::uint16_t,MAX_BLOCKS> profile{};
 std::array<int,3> g_sum{},g_count{};
 std::array<int,A> union_count{};
 unsigned edit_mask=0,membership=0;
 int anchor=-1;
 long double weight=0;
 bool any_g_balance=false;
};

inline long double as_long_double(const Integer& x){
 return std::ldexp((long double)x.w[2],128)+std::ldexp((long double)x.w[1],64)+(long double)x.w[0];
}

class Sampler {
public:
 int blocks,target;
 std::array<std::vector<Profile>,MAX_BLOCKS> profiles;
 std::array<std::array<std::array<std::vector<std::uint16_t>,7>,MAX_BLOCKS>,A> groups;
 std::array<std::array<std::array<Integer,TARGET+1>,MAX_BLOCKS+1>,A> counts{};
 std::array<std::array<std::array<std::vector<Choice>,TARGET+1>,MAX_BLOCKS>,A> choices;
 std::array<Integer,A> total{};
 Integer base_count=1,count_sum=0;
 long double mixture_mass=0;
 std::array<long double,A> probability{};
 std::array<long double,1u<<A> weights{};

 explicit Sampler(const std::array<int,study::N>& base,int block_count=19,int requested=7)
     :blocks(block_count),target(requested){
  if(blocks<1||blocks>19||target<0||target>7||base[8]!=129)
   throw std::runtime_error("Invalid block7 configuration");
  for(int block=0;block<blocks;++block){
   base_count=base_count*720;
   std::array<int,6> ids;std::iota(ids.begin(),ids.end(),6*block);
   do{
    Profile p;
    for(int j=0;j<6;++j){
     int n=6*block+j+1,id=ids[j],a=base[id];p.ids[j]=std::uint8_t(id);
     bool selected[3]={((n+a)%2)==0,(n%2)==0,(a%2)==0};
     for(int type=0;type<3;++type)if(selected[type]){
      ++p.g_count[type];p.g_sum[type]+=n+a;
      if(id==8)p.edit_mask|=1u<<type;
     }
     p.overlap+=(n%7==0&&a%7==0);
     for(int v=0;v<2;++v){
      int av=(v==1&&id==8)?127:a;
      for(int r=1;r<=3;++r){
       bool selected7=(av-r*n)%7==0||(av+r*n)%7==0;
       p.union_count[3*v+r-1]+=selected7;
      }
     }
    }
    unsigned index=profiles[block].size();profiles[block].push_back(p);
    if(p.overlap==0)for(int j=0;j<A;++j)groups[j][block][p.union_count[j]].push_back(index);
   }while(std::next_permutation(ids.begin(),ids.end()));
   if(profiles[block].size()!=720)throw std::logic_error("Block permutation count mismatch");
  }
  for(int j=0;j<A;++j){
   counts[j][blocks][0]=1;
   for(int block=blocks-1;block>=0;--block)for(int remaining=0;remaining<=target;++remaining){
    Integer cumulative=0;
    for(int u=0;u<=6&&u<=remaining;++u){
     const auto child=counts[j][block+1][remaining-u];
     const auto number=groups[j][block][u].size();
     if(child==0||number==0)continue;
     cumulative+=child*number;
     choices[j][block][remaining].push_back({cumulative,child,u});
    }
    counts[j][block][remaining]=cumulative;
   }
   total[j]=counts[j][0][target];
   if(total[j]==0)throw std::runtime_error("An anchor has empty support");
   probability[j]=as_long_double(total[j])/as_long_double(base_count);
   count_sum+=total[j];
  }
  mixture_mass=as_long_double(count_sum)/as_long_double(base_count);
  for(unsigned mask=1;mask<weights.size();++mask){
   weights[mask]=mixture_mass/__builtin_popcount(mask);
  }
 }

 template<class RNG>Integer random_below(RNG& rng,const Integer& upper)const{
  const unsigned bits=upper.bits();
  for(;;){
   Integer x;
   for(unsigned limb=0;limb<(bits+63)/64;++limb)x.w[limb]=rng.next();
   if(bits%64)x.w[bits/64]&=(UINT64_C(1)<<(bits%64))-1;
   if(x<upper)return x;
  }
 }

 Summary unrank(int anchor,Integer rank)const{
  if(anchor<0||anchor>=A||!(rank<total[anchor]))throw std::runtime_error("Bad block7 rank");
  Summary out;out.anchor=anchor;int remaining=target;
  for(int block=0;block<blocks;++block){
   const auto& available=choices[anchor][block][remaining];
   auto it=std::upper_bound(available.begin(),available.end(),rank,
                          [](const Integer& x,const Choice& c){return x<c.cumulative;});
   if(it==available.end())throw std::logic_error("Block7 rank outside CDF");
   if(it!=available.begin())rank-=(it-1)->cumulative;
   auto index=rank.divide_small_quotient(it->child_count);
   const auto profile=groups[anchor][block][it->used].at(index);
   out.profile[block]=profile;remaining-=it->used;
   const auto& p=profiles[block][profile];
   for(int type=0;type<3;++type){out.g_sum[type]+=p.g_sum[type];out.g_count[type]+=p.g_count[type];}
   out.edit_mask|=p.edit_mask;
   for(int j=0;j<A;++j)out.union_count[j]+=p.union_count[j];
  }
  if(remaining!=0||rank!=0)throw std::logic_error("Block7 unranking did not terminate");
  for(int j=0;j<A;++j)if(out.union_count[j]==target)out.membership|=1u<<j;
  if(!(out.membership>>anchor&1u))throw std::logic_error("Selected anchor missing from draw");
  out.weight=weights[out.membership];
  if(blocks==19){
   for(int type=0;type<3;++type){
    out.any_g_balance|=out.g_sum[type]==6236;
    out.any_g_balance|=out.g_sum[type]-2*int(out.edit_mask>>type&1u)==6234;
   }
  }
  return out;
 }

 template<class RNG>Summary draw(RNG& rng)const{
  // One exact integer rank selects anchor proportional to its count and a
  // uniform conditional permutation jointly. The marginal density is pi0*K/B.
  Integer rank=random_below(rng,count_sum);
  for(int anchor=0;anchor<A;++anchor){
   if(rank<total[anchor])return unrank(anchor,rank);
   rank-=total[anchor];
  }
  throw std::logic_error("Block7 joint anchor rank did not terminate");
 }
 study::Draw full_draw(const Summary& s)const{
  study::Draw out;std::iota(out.ids.begin(),out.ids.end(),0);
  for(int block=0;block<blocks;++block)for(int j=0;j<6;++j)
   out.ids[6*block+j]=profiles[block][s.profile[block]].ids[j];
  return out;
 }
};
} // namespace block7
#endif
