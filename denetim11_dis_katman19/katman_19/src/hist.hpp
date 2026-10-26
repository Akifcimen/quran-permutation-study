#ifndef JOINT_HIST_HPP
#define JOINT_HIST_HPP
#include "features.hpp"
#include <fstream>
#include <vector>
namespace study {
struct Hist {
 uint64_t n=0,proposals=0;
 std::vector<uint64_t> g,c,w,m;
 Hist():g(V*F*4),c(V*C*16),w(V*C*4),m(V*C*8*16){}
 void add(const Features& x){
  n++;
  for(int v=0;v<V;v++){
   for(int f=0;f<F;f++)g[(v*F+f)*4+x.g[v][f]]++;
   for(int cx=0;cx<C;cx++){
    c[(v*C+cx)*16+x.c[v][cx]]++;w[(v*C+cx)*4+x.w[v][cx]]++;
    uint32_t bits=x.m[v][cx];for(int r=0;r<8;r++,bits>>=4)m[((v*C+cx)*8+r)*16+(bits&15)]++;
   }
  }
 }
 void merge(const Hist& b){n+=b.n;proposals+=b.proposals;for(auto pair:{std::make_pair(&g,&b.g),std::make_pair(&c,&b.c),std::make_pair(&w,&b.w),std::make_pair(&m,&b.m)})for(size_t i=0;i<pair.first->size();i++)(*pair.first)[i]+=(*pair.second)[i];}
 void save(const std::string& path)const{std::ofstream o(path,std::ios::binary);o.write((char*)&n,8);o.write((char*)&proposals,8);for(const auto* a:{&g,&c,&w,&m})o.write((char*)a->data(),a->size()*8);if(!o)throw std::runtime_error("hist write");}
 void load(const std::string& path){std::ifstream i(path,std::ios::binary);i.read((char*)&n,8);i.read((char*)&proposals,8);for(auto* a:{&g,&c,&w,&m})i.read((char*)a->data(),a->size()*8);if(!i)throw std::runtime_error("hist read");}
};
}
#endif
