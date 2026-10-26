#include "../src/kernels.hpp"
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

namespace {
void check(bool condition,const char* message) {if(!condition)throw std::runtime_error(message);}
template<std::size_t M>
int direct(const std::array<int,M>& ids,const std::array<int,M>& values) {
 int total=0;
 for(std::size_t i=0;i<M;++i)for(std::size_t j=i+1;j<M;++j)
  total+=values[ids[i]]<values[ids[j]];
 return total;
}
void valid(const rare::Ids& ids,int model,const rare::Ids& base,const rare::Ids& alt) {
 std::array<bool,study::N> used{};
 for(int n=0;n<study::N;++n){
  check(ids[n]>=0&&ids[n]<study::N,"Out-of-range ID");
  check(!used[ids[n]],"Duplicate ID");used[ids[n]]=true;
  if(model==1)check(ids[n]/6==n/6,"Block-support violation");
 }
 if(model==2){check(direct(ids,base)==993,"Wrong inversion129");check(direct(ids,alt)==994,"Wrong inversion127");}
}
std::array<int,4> aggregate(const rare::Ids& ids,const rare::Ids& values,
                           int alpha,int beta,int p,bool parity) {
 std::array<int,4> result{};
 for(int i=0;i<study::N;++i){
  const int n=i+1,a=values[ids[i]],t=alpha*n+beta*a;
  const bool selected=parity?t%2==0:t%p==0;
  result[selected?0:2]+=n;result[selected?1:3]+=a;
 }
 return result;
}
std::array<double,6> moments(const rare::Ids& ids,const rare::Ids& values) {
 int pos0=0,pos8=0;
 for(int n=0;n<study::N;++n){if(ids[n]==0)pos0=n;if(ids[n]==8)pos8=n;}
 return {double(pos0),double(pos8),double(values[ids[3]]),double(values[ids[17]]),
         double(values[ids[55]]),double(values[ids[99]])};
}
struct Moments {
 std::array<double,6> sum{},square{};int n=0;
 void add(std::array<double,6> x){++n;for(int j=0;j<6;++j){sum[j]+=x[j];square[j]+=x[j]*x[j];}}
};
}
int main(int argc,char**argv){
 try{
  check(argc==2,"Provide counts.txt");
  const auto started=std::chrono::steady_clock::now();
  rare::Ids base{},identity{};
  std::ifstream input(argv[1]);for(int&a:base)check(bool(input>>a),"Missing counts");
  std::iota(identity.begin(),identity.end(),0);
  auto alt=base;alt[8]=127;
  std::uint64_t exhaustive=0;
  const std::array<int,6> toy={10,10,7,4,3,1};
  auto toy_alt=toy;toy_alt[1]=8;
  std::array<int,6> ids={0,1,2,3,4,5};
  const auto ranks=rare::value_ranks(toy),ranks_alt=rare::value_ranks(toy_alt);
  do{
   const rare::RankPrefix<6> prefix(ids,ranks),prefix_alt(ids,ranks_alt);
   const int original=direct(ids,toy),original_alt=direct(ids,toy_alt);
   for(int i=0;i<6;++i)for(int j=0;j<6;++j){
    auto next=ids;std::swap(next[i],next[j]);
    const int expected=direct(next,toy)-original;
    const int expected_alt=direct(next,toy_alt)-original_alt;
    check(expected==rare::swap_inversion_delta(ids,toy,i,j),"O(distance) delta mismatch");
    check(expected_alt==rare::swap_inversion_delta(ids,toy_alt,i,j),"Edited delta mismatch");
    check(expected==prefix.delta(i,j),"Rank-prefix delta mismatch");
    check(expected_alt==prefix_alt.delta(i,j),"Edited rank-prefix delta mismatch");
    exhaustive+=4;
   }
  }while(std::next_permutation(ids.begin(),ids.end()));

  std::cout<<std::setprecision(17)<<"{\"status\":\"passed\",\"delta_comparisons\":"<<exhaustive<<",\"models\":[";
  for(int model=0;model<3;++model){
   const auto model_start=std::chrono::steady_clock::now();
   rare::MutationKernel kernel(base,model,UINT64_C(202610251600)+model);
   auto state=identity;
   std::array<std::uint64_t,13> attempts{},accepted{},numeric{};
   std::uint64_t repair_alternatives=0,repair_nontrivial=0,invariant_checks=0,filter_checks=0;
   constexpr int STEPS=100000;
   for(int step=0;step<STEPS;++step){
    const auto proposal=kernel.propose(state);
    const unsigned kind=static_cast<unsigned>(proposal.kind);
    ++attempts[kind];accepted[kind]+=proposal.null_ok;
    numeric[kind]+=proposal.null_ok&&proposal.numeric_changed;
    if(proposal.kind==rare::MoveKind::PairedRepair){
     repair_alternatives+=proposal.repair_alternatives;
     repair_nontrivial+=proposal.repair_alternatives>0;
    }
    if(step<2000||step%1000==0){
     if(model==2){
      const int a=direct(proposal.draw.ids,base),b=direct(proposal.draw.ids,alt);
      check(proposal.null_ok==(a==993&&b==994),"Null flag differs from direct count");
      if(proposal.kind!=rare::MoveKind::Independent){
       check(proposal.delta129==a-993,"Accumulated129 delta wrong");
       check(proposal.delta127==b-994,"Accumulated127 delta wrong");
      }
     }
     if(proposal.null_ok)valid(proposal.draw.ids,model,base,alt);
     ++invariant_checks;
    }
    if(step<2000&&kind>=8&&kind<=11){
     for(const auto& values:{base,alt}){
      for(int f=0;f<study::F;++f){
       const auto beforeP=aggregate(state,values,study::AL[f],study::BE[f],19,true);
       const auto afterP=aggregate(proposal.draw.ids,values,study::AL[f],study::BE[f],19,true);
       check(beforeP==afterP,"Parity-filtered move changed aggregate P");
       ++filter_checks;
       if(proposal.filter_prime){
        const int p=proposal.filter_prime;
        const auto beforeR=aggregate(state,values,study::AL[f],study::BE[f],p,false);
        const auto afterR=aggregate(proposal.draw.ids,values,study::AL[f],study::BE[f],p,false);
        for(int j=0;j<4;++j)check((beforeR[j]-afterR[j])%p==0,"Residue move changed R modulo p");
        ++filter_checks;
       }
      }
     }
    }
    if(proposal.null_ok)state=proposal.draw.ids;
   }
   valid(state,model,base,alt);

   // Each experimental output starts from an independent null draw; arbitrary
   // finite mutation steps should preserve that null distribution. A distinct
   // independent group supplies the comparison for the inversion model.
   study::Sampler starts(base,model,UINT64_C(202610251700)+model);
   study::Sampler references(base,model,UINT64_C(202610251800)+model);
   rare::MutationKernel stationarity_kernel(base,model,UINT64_C(202610251900)+model);
   Moments reference,mutated;
   constexpr int INDEPENDENT_STARTS=6000;
   for(int draw=0;draw<INDEPENDENT_STARTS;++draw){
    auto current=starts.next().ids;
    for(int s=0;s<20;++s){
     auto next=stationarity_kernel.propose(current);
     if(next.null_ok)current=next.draw.ids;
    }
    mutated.add(moments(current,base));
    reference.add(moments(references.next().ids,base));
   }
   double max_z=0;
   for(int j=0;j<6;++j){
    const double m1=mutated.sum[j]/mutated.n,m0=reference.sum[j]/reference.n;
    const double v1=(mutated.square[j]-mutated.sum[j]*m1)/(mutated.n-1);
    const double v0=(reference.square[j]-reference.sum[j]*m0)/(reference.n-1);
    const double se=std::sqrt(v1/mutated.n+v0/reference.n);
    const double z=se>0?std::fabs(m1-m0)/se:0;
    max_z=std::max(max_z,z);check(z<6,"Independent-start stationarity moment check failed");
   }
   if(model)std::cout<<",";
   std::cout<<"{\"model\":"<<model<<",\"steps\":"<<STEPS<<",\"kinds\":[";
   for(int k=0;k<13;++k){
    if(k)std::cout<<",";
    std::cout<<"{\"kind\":\""<<rare::move_name(static_cast<rare::MoveKind>(k))<<"\",\"attempts\":"<<attempts[k]
             <<",\"null_valid\":"<<accepted[k]<<",\"numeric_moves\":"<<numeric[k]<<"}";
   }
   std::cout<<"],\"repair_alternatives_total\":"<<repair_alternatives<<",\"repair_nontrivial\":"<<repair_nontrivial
            <<",\"invariant_checks\":"<<invariant_checks<<",\"aggregate_preservation_checks\":"<<filter_checks
            <<",\"independent_starts_per_group\":"<<INDEPENDENT_STARTS<<",\"stationarity_max_abs_z\":"<<max_z
            <<",\"seconds\":"<<std::chrono::duration<double>(std::chrono::steady_clock::now()-model_start).count()<<"}";
  }
  std::cout<<"],\"total_seconds\":"<<std::chrono::duration<double>(std::chrono::steady_clock::now()-started).count()<<"}\n";
  return 0;
 }catch(const std::exception& error){std::cerr<<error.what()<<"\n";return 1;}
}

