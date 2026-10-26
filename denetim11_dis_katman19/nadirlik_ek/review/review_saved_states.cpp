#include "../../src/scorer.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace {
struct Record {std::array<double,study::K> scores{};unsigned flags=0;};
void require(bool condition,const std::string& message){if(!condition)throw std::runtime_error(message);}
std::string program(const study::ScoreTrace& trace){
 std::ostringstream out;out<<trace.variant<<":"<<trace.active;
 for(int i=0;i<trace.block_count;++i){
  const auto&b=trace.blocks[i];out<<"|"<<b.modules<<":"<<b.form<<":"<<b.prime_index<<":"<<b.matrix_view;
  for(auto mask:b.positive_masks)out<<":"<<int(mask);
 }
 return out.str();
}
}

int main(int argc,char**argv){try{
 static_assert(sizeof(int)==4&&sizeof(double)==8,"Saved record sizes differ");
 require(argc==4,"Usage: review_saved_states study_root model states_file");
 const std::string root=argv[1],model=argv[2],path=argv[3];
 require(model=="uniform"||model=="block6"||model=="inversion_exact","Unknown null model");
 std::array<int,study::N> base{};
 std::ifstream counts(root+"/data/counts.txt");for(int&x:base)counts>>x;
 require(bool(counts),"Missing input counts");
 study::Hist histogram;histogram.load(root+"/results/train_"+model+".bin");
 MatrixEngine matrix;study::Extractor extract(base,matrix);study::Scorer scorer(histogram);
 study::Draw observed;std::iota(observed.ids.begin(),observed.ids.end(),0);
 const auto targets=scorer(extract(observed)).value;
 std::ifstream input(path,std::ios::binary|std::ios::ate);
 require(bool(input),"Missing states file");
 const auto bytes=input.tellg();
 constexpr std::size_t record_bytes=sizeof(int)*study::N+sizeof(double)*study::K;
 require(bytes>=0&&std::size_t(bytes)%record_bytes==0,"Truncated saved state record");
 input.seekg(0);
 const int expected=std::size_t(bytes)/record_bytes;
 std::vector<Record> records;records.reserve(expected);
 std::set<std::array<int,study::N>> distinct;
 std::map<std::string,std::pair<int,int>> programs;
 std::array<int,4> natural_counts{};
 std::array<int,study::K> policy_hits{};
 int inv129_min=1000000,inv129_max=-1,inv127_min=1000000,inv127_max=-1;
 bool all_block_membership=true;
 for(int index=0;index<expected;++index){
  study::Draw draw;Record record;
  input.read(reinterpret_cast<char*>(draw.ids.data()),sizeof(draw.ids));
  input.read(reinterpret_cast<char*>(record.scores.data()),sizeof(record.scores));
  require(bool(input),"Incomplete state read");
  std::array<bool,study::N> seen{};bool block_membership=true;
  for(int position=0;position<study::N;++position){
   const int id=draw.ids[position];
   require(id>=0&&id<study::N,"Out-of-range identity");
   require(!seen[id],"Repeated identity in saved permutation");seen[id]=true;
   block_membership&=id/6==position/6;
  }
  all_block_membership&=block_membership;
  if(model=="block6")require(block_membership,"Saved state leaves original six-item block");
  int i129=0,i127=0;
  for(int i=0;i<study::N;++i)for(int j=i+1;j<study::N;++j){
   const int left=draw.ids[i],right=draw.ids[j];
   i129+=base[left]<base[right];
   i127+=(left==8?127:base[left])<(right==8?127:base[right]);
  }
  if(model=="inversion_exact")require(i129==993&&i127==994,"Saved state leaves joint inversion fiber");
  inv129_min=std::min(inv129_min,i129);inv129_max=std::max(inv129_max,i129);
  inv127_min=std::min(inv127_min,i127);inv127_max=std::max(inv127_max,i127);
  const auto features=extract(draw);
  const auto recomputed=scorer(features);
  for(int k=0;k<study::K;++k){
   require(std::isfinite(record.scores[k]),"Nonfinite saved score");
   require(std::memcmp(&record.scores[k],&recomputed.value[k],sizeof(double))==0,"Saved score differs bit for bit from original scorer");
   policy_hits[k]+=record.scores[k]>=targets[k];
  }
  for(int variant=0;variant<study::V;++variant){
   const bool scalar=features.g[variant][0]==3&&features.c[variant][6]==15&&features.w[variant][6]==3;
   if(scalar)record.flags|=1u<<(2*variant);
   if(scalar&&(features.m[variant][6]&15)==15)record.flags|=2u<<(2*variant);
  }
  for(int bit=0;bit<4;++bit)natural_counts[bit]+=(record.flags>>bit)&1;
  auto& frequency=programs[program(recomputed.trace[1])];
  ++frequency.first;frequency.second+=record.scores[1]>=targets[1];
  distinct.insert(draw.ids);records.push_back(record);
 }
 std::cout<<std::setprecision(17)<<"{\"status\":\"passed\",\"model\":\""<<model<<"\",\"states\":"<<records.size()
  <<",\"valid_permutations_checked\":"<<records.size()<<",\"null_support_checks\":"<<records.size()
  <<",\"score_values_compared_bit_exactly\":"<<records.size()*study::K<<",\"training_n\":"<<histogram.n
  <<",\"inversion129_range\":["<<inv129_min<<","<<inv129_max<<"],\"inversion127_range\":["<<inv127_min<<","<<inv127_max
  <<"],\"all_block_membership\":"<<(all_block_membership?"true":"false")<<",\"distinct_permutations\":"<<distinct.size()
  <<",\"distinct_primary_programs\":"<<programs.size()<<",\"targets\":[";
 for(int k=0;k<study::K;++k){if(k)std::cout<<",";std::cout<<targets[k];}
 std::cout<<"],\"policy_hit_counts\":[";
 for(int k=0;k<study::K;++k){if(k)std::cout<<",";std::cout<<policy_hits[k];}
 std::cout<<"],\"natural_flag_counts\":[";
 for(int k=0;k<4;++k){if(k)std::cout<<",";std::cout<<natural_counts[k];}
 std::cout<<"],\"primary_program_frequencies\":[";
 bool first=true;
 for(const auto&entry:programs){
  if(!first)std::cout<<",";
  first=false;
  std::cout<<"{\"program\":\""<<entry.first<<"\",\"count\":"<<entry.second.first<<",\"primary_hits\":"<<entry.second.second<<"}";
 }
 std::cout<<"],\"records\":[";
 for(std::size_t i=0;i<records.size();++i){
  if(i)std::cout<<",";
  std::cout<<"{\"scores\":[";
  for(int k=0;k<study::K;++k){if(k)std::cout<<",";std::cout<<records[i].scores[k];}
  std::cout<<"],\"natural_flags\":"<<records[i].flags<<"}";
 }
 std::cout<<"]}\n";
 return 0;
}catch(const std::exception&e){std::cerr<<"Saved-state review failed: "<<e.what()<<"\n";return 1;}}
