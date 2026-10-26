#ifndef BLOCK6_EXACT_CW_SAMPLER_HPP
#define BLOCK6_EXACT_CW_SAMPLER_HPP
#include "../../src/sampler.hpp"
#include <array>
#include <cmath>
#include <fstream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>

namespace exact_block6 {
// The largest integer is720^19<2^181. Three limbs suffice. Floating point is
// used only to guess a small quotient; exact correction always determines it.
struct U256 {
  uint64_t w[3]{};
  U256(uint64_t x=0):w{x,0,0}{}
  bool operator<(const U256& b)const {for(int i=2;i>=0;i--)if(w[i]!=b.w[i])return w[i]<b.w[i];return false;}
  bool operator==(const U256& b)const{return w[0]==b.w[0]&&w[1]==b.w[1]&&w[2]==b.w[2];}
  bool operator!=(const U256& b)const{return !(*this==b);}
  U256& operator+=(const U256& b){__uint128_t c=0;for(int i=0;i<3;i++){c+=__uint128_t(w[i])+b.w[i];w[i]=uint64_t(c);c>>=64;}if(c)throw std::runtime_error("192bit overflow");return *this;}
  U256& operator-=(const U256& b){uint64_t borrow=0;for(int i=0;i<3;i++){__uint128_t rhs=__uint128_t(b.w[i])+borrow;borrow=__uint128_t(w[i])<rhs;w[i]=uint64_t(__uint128_t(w[i])-rhs);}if(borrow)throw std::runtime_error("192bit negative");return *this;}
  U256 operator*(uint64_t b)const{U256 x;__uint128_t c=0;for(int i=0;i<3;i++){c+=__uint128_t(w[i])*b;x.w[i]=uint64_t(c);c>>=64;}if(c)throw std::runtime_error("192bit multiply overflow");return x;}
  double approx()const{return double(w[2])*0x1p128+double(w[1])*0x1p64+double(w[0]);}
  unsigned bits()const{for(int i=2;i>=0;i--)if(w[i])return 64*i+64-__builtin_clzll(w[i]);return 0;}
  uint32_t divide_small_quotient(const U256& denominator){
    uint32_t q=uint32_t(approx()/denominator.approx());
    U256 product=denominator*q;
    while(*this<product){q--;product-=denominator;}
    U256 next=product;next+=denominator;
    while(!( *this<next)){q++;product=next;next+=denominator;}
    *this-=product;return q;
  }
  std::string str()const{
    U256 x=*this;std::vector<uint32_t> parts;
    do{__uint128_t rem=0;for(int i=2;i>=0;i--){__uint128_t z=(rem<<64)|x.w[i];x.w[i]=uint64_t(z/1000000000);rem=z%1000000000;}parts.push_back(uint32_t(rem));}while(x!=U256(0));
    std::string out=std::to_string(parts.back());
    for(int i=int(parts.size())-2;i>=0;i--){auto p=std::to_string(parts[i]);out+=std::string(9-p.size(),'0')+p;}return out;
  }
};
inline std::ostream& operator<<(std::ostream& os,const U256& x){return os<<x.str();}
struct Profile {
  uint32_t ec, es, en, fn, hn, hd, overlap, edit_even;
  std::array<uint32_t,6> ids;
};
struct Edge {uint32_t child, h, d; std::vector<uint32_t> indexes;};
struct Node {uint32_t block; std::vector<Edge> edges;};
struct Choice {U256 cumulative, child_count; uint32_t child_state; const Edge* edge;};
struct Summary {
  uint32_t ec=0, es=0, en=0, fn=0, hn=0, hd=0, overlap=0, edit_even=0;
  std::array<int,study::N> ids{};
};

class Sampler {
 public:
  std::array<std::vector<Profile>,19> profiles;
  std::vector<Node> nodes;
  std::vector<U256> counts;
  std::vector<std::vector<Choice>> choices;
  std::vector<uint32_t> roots;
  uint32_t terminal;
  U256 total;
  unsigned bits;

  explicit Sampler(const std::string& path) {
    std::ifstream in(path,std::ios::binary);
    if(!in)throw std::runtime_error("Missing CW sampler binary");
    char magic[8];in.read(magic,8);
    if(std::string(magic,8)!="BLK6CW01")throw std::runtime_error("CW format mismatch");
    const uint32_t nnode=read32(in);terminal=read32(in);
    const uint32_t nroot=read32(in);roots.resize(nroot);
    for(auto& r:roots)r=read32(in);
    total=read256(in);
    for(auto& block:profiles){
      block.resize(read32(in));
      for(auto& p:block){
        p.ec=read32(in);p.es=read32(in);p.en=read32(in);p.fn=read32(in);
        p.hn=read32(in);p.hd=read32(in);p.overlap=read32(in);p.edit_even=read32(in);
        for(auto& id:p.ids)id=read32(in);
      }
    }
    nodes.resize(nnode);
    for(auto& node:nodes){
      node.block=read32(in);node.edges.resize(read32(in));
      for(auto& edge:node.edges){
        edge.child=read32(in);edge.h=read32(in);edge.d=read32(in);
        edge.indexes.resize(read32(in));for(auto& x:edge.indexes)x=read32(in);
      }
    }
    if(terminal!=nnode)throw std::runtime_error("Bad terminal index");
    counts.resize((nnode+1)*8*19);
    for(auto& x:counts)x=read256(in);
    if(!in || in.peek()!=std::char_traits<char>::eof())throw std::runtime_error("CW binary length mismatch");
    U256 check=0;for(auto root:roots)check+=counts[state(root,7,0)];
    if(check!=total)throw std::runtime_error("CW root count mismatch");
    bits=total.bits();
    choices.resize(counts.size());
    for(uint32_t node=0;node<nnode;node++)for(uint32_t h=0;h<8;h++)for(uint32_t r=0;r<19;r++){
      const uint32_t current=state(node,h,r);
      if(counts[current]==0)continue;
      U256 cumulative=0;
      for(const auto& e:nodes[node].edges){
        if(e.h>h)continue;
        uint32_t child=state(e.child,h-e.h,(r+19-e.d)%19);
        const U256 child_count=counts[child];
        if(child_count==0)continue;
        cumulative+=child_count*e.indexes.size();
        choices[current].push_back(Choice{cumulative,child_count,child,&e});
      }
      if(cumulative!=counts[current])throw std::runtime_error("CW integer recurrence failed");
    }
  }

  Summary draw(study::RNG& rng,bool save_ids=false)const {
    U256 rank=random_below(rng);
    uint32_t current=0;
    for(auto r:roots){
      const auto s=state(r,7,0);
      if(rank<counts[s]){current=s;break;}
      rank-=counts[s];
    }
    Summary out;
    for(int step=0;step<19;step++){
      const auto& cs=choices[current];
      auto it=std::upper_bound(cs.begin(),cs.end(),rank,[](const U256& x,const Choice& c){return x<c.cumulative;});
      if(it==cs.end())throw std::runtime_error("CW unranking outside interval");
      if(it!=cs.begin())rank-=(it-1)->cumulative;
      const auto index=rank.divide_small_quotient(it->child_count);
      const uint32_t block=nodes[current/(8*19)].block;
      const Profile& p=profiles[block][it->edge->indexes[index]];
      out.ec+=p.ec;out.es+=p.es;out.en+=p.en;out.fn+=p.fn;
      out.hn+=p.hn;out.hd+=p.hd;out.overlap+=p.overlap;out.edit_even+=p.edit_even;
      if(save_ids)for(int i=0;i<6;i++)out.ids[6*block+i]=p.ids[i];
      current=it->child_state;
    }
    if(current!=state(terminal,0,0)||rank!=0)throw std::runtime_error("CW unranking did not terminate");
    out.en%=19;out.fn%=19;out.hd%=19;
    return out;
  }

 private:
  static uint32_t read32(std::istream& in){
    unsigned char b[4];in.read(reinterpret_cast<char*>(b),4);
    return uint32_t(b[0])|(uint32_t(b[1])<<8)|(uint32_t(b[2])<<16)|(uint32_t(b[3])<<24);
  }
  static U256 read256(std::istream& in){
    U256 x;for(unsigned i=0;i<3;i++){x.w[i]=read32(in);x.w[i]|=uint64_t(read32(in))<<32;}
    const uint32_t upper0=read32(in),upper1=read32(in);
    if(upper0||upper1)throw std::runtime_error("Integer exceeds192bits");return x;
  }
  static uint32_t state(uint32_t node,uint32_t h,uint32_t residue){return (node*8+h)*19+residue;}
  U256 random_below(study::RNG& rng)const {
    for(;;){
      U256 x;for(unsigned i=0;i<(bits+63)/64;i++)x.w[i]=rng.next();
      if(bits%64)x.w[bits/64]&=(uint64_t(1)<<(bits%64))-1;
      if(x<total)return x;
    }
  }
};
}
#endif
