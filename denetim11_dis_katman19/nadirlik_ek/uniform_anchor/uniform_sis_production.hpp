#ifndef UNIFORM_NATURAL_ANCHOR_SIS_PRODUCTION_HPP
#define UNIFORM_NATURAL_ANCHOR_SIS_PRODUCTION_HPP
#include "../../src/sampler.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace uniform_anchor {
struct CountedRNG:study::RNG {
  uint64_t calls=0;
  explicit CountedRNG(uint64_t seed):study::RNG(seed){}
  uint64_t next(){calls++;return study::RNG::next();}
  uint32_t bounded(uint32_t r){
    if(!r)throw std::invalid_argument("CountedRNG bound0");
    uint32_t x=uint32_t(next()>>32);uint64_t product=uint64_t(x)*r;uint32_t low=uint32_t(product);
    if(low<r){const uint32_t threshold=(uint32_t(0)-r)%r;while(low<threshold){x=uint32_t(next()>>32);product=uint64_t(x)*r;low=uint32_t(product);}}
    return uint32_t(product>>32);
  }
};
struct Local {uint64_t signature=0;std::vector<std::array<uint8_t,6>> assignments;};
struct Choice {uint64_t cumulative,child_count;uint32_t child,local;};
struct Alias {uint64_t threshold=0;uint32_t alias=0,child=0,local=0;};
struct Node {uint32_t group=18;uint64_t remaining=0,count=1,rng_threshold=0;std::vector<Choice> choices;std::vector<Alias> aliases;};
struct Draw {
  std::array<int,study::N> ids{};
  std::array<int,7> h_rows{};
  int pattern=-1,fn=0,ec=0,es=0,en=0,edit_even=0;
  bool success=false;
  double weight=0,log_weight=0;
  int h_dead_step=-1;
};
class Sampler {
 public:
  int variant=127;
  uint64_t MF=0,threshold=0;
  std::array<int,study::N> base{};
  std::array<std::vector<Local>,18> local;
  std::vector<Node> nodes;
  std::vector<uint32_t> roots;
  std::vector<uint64_t> root_cdf;
  std::array<double,1025> logs{};
  double log_constant=0,f_marked_probability=0;
  long double initial_weight=0;

  Sampler(const std::string& path,const std::array<int,study::N>& original):base(original){
    std::ifstream in(path,std::ios::binary);char magic[8];in.read(magic,8);
    if(std::string(magic,8)!="UNIFCS01")throw std::runtime_error("Uniform F sampler format mismatch");
    variant=read32(in);base[8]=variant;uint32_t nr=read32(in);MF=read64(in);roots.resize(nr);for(auto& r:roots)r=read32(in);
    for(uint32_t group=0;group<18;group++){
      if(read32(in)!=group+1)throw std::runtime_error("F residue order mismatch");
      local[group].resize(read32(in));
      for(auto& x:local[group]){x.signature=read64(in);x.assignments.resize(read32(in));for(auto& a:x.assignments)in.read(reinterpret_cast<char*>(a.data()),6);}
    }
    nodes.resize(read32(in));
    for(auto& node:nodes){node.group=read32(in);node.remaining=read64(in);node.count=read64(in);node.choices.resize(read32(in));for(auto& c:node.choices){c.cumulative=read64(in);c.child_count=read64(in);c.child=read32(in);c.local=read32(in);}}
    if(!in||in.peek()!=std::char_traits<char>::eof())throw std::runtime_error("F sampler binary length mismatch");
    uint64_t cumulative=0;for(auto r:roots){cumulative+=nodes[r].count;root_cdf.push_back(cumulative);}if(cumulative!=MF)throw std::runtime_error("F root count mismatch");
    for(size_t i=1;i<nodes.size();i++){
      const auto& node=nodes[i];uint64_t before=0;
      for(auto& c:node.choices){
        if(c.child_count!=nodes[c.child].count||(__uint128_t(c.child_count)*local[node.group][c.local].assignments.size())!=c.cumulative-before)throw std::runtime_error("F integer recurrence mismatch");
        before=c.cumulative;
      }
      if(before!=node.count)throw std::runtime_error("F CDF count mismatch");
    }
    // Exact integer Walker alias tables. Their outcome masses are verified
    // algebraically before the original CDF storage is released.
    for(size_t ni=1;ni<nodes.size();ni++){
      auto& node=nodes[ni];const uint32_t size=node.choices.size();node.aliases.resize(size);node.rng_threshold=uint64_t(-node.count)%node.count;
      std::vector<__uint128_t> scaled(size),original(size),checked(size);
      std::vector<uint32_t> small,large;uint64_t before=0;
      for(uint32_t i=0;i<size;i++){
        const auto& c=node.choices[i];original[i]=scaled[i]=__uint128_t(c.cumulative-before)*size;before=c.cumulative;
        node.aliases[i].child=c.child;node.aliases[i].local=c.local;
        (scaled[i]<node.count?small:large).push_back(i);
      }
      while(!small.empty()&&!large.empty()){
        uint32_t a=small.back(),b=large.back();small.pop_back();large.pop_back();
        node.aliases[a].threshold=uint64_t(scaled[a]);node.aliases[a].alias=b;
        scaled[b]-=node.count-scaled[a];(scaled[b]<node.count?small:large).push_back(b);
      }
      for(auto i:small){node.aliases[i].threshold=node.count;node.aliases[i].alias=i;}
      for(auto i:large){node.aliases[i].threshold=node.count;node.aliases[i].alias=i;}
      for(uint32_t i=0;i<size;i++){checked[i]+=node.aliases[i].threshold;checked[node.aliases[i].alias]+=node.count-node.aliases[i].threshold;}
      if(checked!=original)throw std::runtime_error("Exact integer alias mass mismatch");
      std::vector<Choice>().swap(node.choices);
    }
    threshold=uint64_t(-MF)%MF;
    for(size_t i=1;i<logs.size();i++)logs[i]=std::log(double(i));
    log_constant=std::log(double(MF))-std::lgamma(115.0)-std::lgamma(8.0);
    initial_weight=MF;for(int i=2;i<=114;i++)initial_weight/=i;for(int i=2;i<=7;i++)initial_weight/=i;
    long double marked=MF;for(int i=103;i<=114;i++)marked/=i;f_marked_probability=double(marked);
  }

  template<class RNG> Draw partial_F(RNG& rng)const{
    Draw out;out.ids.fill(-1);uint64_t rank=below_MF(rng);
    out.pattern=std::upper_bound(root_cdf.begin(),root_cdf.end(),rank)-root_cdf.begin();
    uint32_t node=roots[out.pattern];
    while(node){
      const auto& x=nodes[node];uint32_t choice=0;
      if(x.aliases.size()>1){choice=rng.bounded(x.aliases.size());uint64_t u=below(rng,x.count,x.rng_threshold);if(u>=x.aliases[choice].threshold)choice=x.aliases[choice].alias;}
      const auto& selected=x.aliases[choice];const auto& assignments=local[x.group][selected.local].assignments;
      const auto& assignment=assignments[assignments.size()==1?0:rng.bounded(assignments.size())];
      for(int i=0;i<6;i++)if(assignment[i]!=255){const int n=x.group+1+19*i;out.ids[n-1]=assignment[i];out.fn+=n;}
      node=selected.child;
    }
    out.fn%=19;return out;
  }

  // Exact-F anchor plus ordered-H sequential importance proposal, followed by
  // a Hall-aware full-support completion avoiding all additional F/H edges.
  template<class RNG> Draw sis(RNG& rng)const{
    Draw out=partial_F(rng);bool used[114]{};int rows[19][6],cols[19][114],R[19]{},C[19]{};
    for(int i=0;i<114;i++)if(out.ids[i]>=0)used[out.ids[i]]=true;else rows[(i+1)%19][R[(i+1)%19]++]=i+1;
    for(int id=0;id<114;id++)if(!used[id])cols[base[id]%19][C[base[id]%19]++]=id;
    double logw=log_constant;long double weight=initial_weight;int hs=0;
    for(int step=0;step<7;step++){
      int r=0,available=0;
      if(step==6){r=(out.fn-hs+19*7)%19;available=r?R[r]*C[r]:0;}
      else for(int q=1;q<19;q++)available+=R[q]*C[q];
      if(!available){out.h_dead_step=step;return out;}
      logw+=logs[available];weight*=available;int chosen=rng.bounded(available);
      if(step<6){for(r=1;r<19;r++){int mass=R[r]*C[r];if(chosen<mass)break;chosen-=mass;}}
      const int ri=chosen/C[r],ci=chosen%C[r],n=rows[r][ri],id=cols[r][ci];
      out.h_rows[step]=n;
      rows[r][ri]=rows[r][--R[r]];cols[r][ci]=cols[r][--C[r]];out.ids[n-1]=id;hs=(hs+n)%19;
    }
    int grow[10][12],gcol[10][114],gr[10]{},gc[10]{};
    for(int r=0;r<19;r++){
      const int g=std::min(r,19-r);
      for(int i=0;i<R[r];i++)grow[g][gr[g]++]=rows[r][i];
      for(int i=0;i<C[r];i++)gcol[g][gc[g]++]=cols[r][i];
    }
    for(int left=95;left>0;left--){
      int g=-1,largest=-1;
      for(int j=0;j<10;j++)if(gr[j]&&gr[j]+gc[j]>largest){largest=gr[j]+gc[j];g=j;}
      if(g<0||largest>left)throw std::runtime_error("Hall invariant violated in actual uniform anchor");
      const int available=left-gc[g];if(!available)throw std::runtime_error("Unexpected completion deadend");
      logw+=logs[available];weight*=available;int chosen=rng.bounded(available),column_group=0;
      for(;column_group<10;column_group++){if(column_group==g)continue;if(chosen<gc[column_group])break;chosen-=gc[column_group];}
      const int n=grow[g][--gr[g]],id=gcol[column_group][chosen];
      gcol[column_group][chosen]=gcol[column_group][--gc[column_group]];out.ids[n-1]=id;
    }
    summarize(out);out.success=true;out.log_weight=logw;out.weight=double(weight);return out;
  }

  // Independent F-marked rejection comparator. Complete uniformly, with no
  // H-biased proposal and no avoiding-completion importance weights.
  template<class RNG> Draw rejection(RNG& rng)const{
    Draw out=partial_F(rng);bool used[114]{};int remaining[114],count=0,hn=0,hs=0;
    for(int id:out.ids)if(id>=0)used[id]=true;
    for(int id=0;id<114;id++)if(!used[id])remaining[count++]=id;
    for(int i=0;i<114;i++)if(out.ids[i]<0){
      int index=rng.bounded(count),id=remaining[index];remaining[index]=remaining[--count];out.ids[i]=id;
      int n=i+1,a=base[id];if((n+a)%19==0)return out;
      if((n-a)%19==0){if(++hn>7)return out;hs+=n;}
    }
    if(hn!=7||((hs-out.fn)%19+19)%19)return out;
    summarize(out);out.success=true;out.weight=f_marked_probability;out.log_weight=std::log(out.weight);return out;
  }

 private:
  static uint32_t read32(std::istream& in){unsigned char b[4];in.read(reinterpret_cast<char*>(b),4);return uint32_t(b[0])|(uint32_t(b[1])<<8)|(uint32_t(b[2])<<16)|(uint32_t(b[3])<<24);}
  static uint64_t read64(std::istream& in){uint64_t x=read32(in);return x|(uint64_t(read32(in))<<32);}
  template<class RNG> uint64_t below_MF(RNG& rng)const{
    return below(rng,MF,threshold);
  }
  template<class RNG> static uint64_t below(RNG& rng,uint64_t bound,uint64_t reject_threshold){for(;;){const __uint128_t product=__uint128_t(rng.next())*bound;const uint64_t low=uint64_t(product);if(low>=reject_threshold)return uint64_t(product>>64);}}
  void summarize(Draw& out)const{
    for(int i=0;i<114;i++){
      int n=i+1,id=out.ids[i],a=base[id];bool even=((n+a)%2)==0;
      out.ec+=even;out.es+=(n+a)*even/2;out.en+=n*even;out.edit_even|=(id==8&&even);
    }
    out.en%=19;
  }
};
}
#endif
