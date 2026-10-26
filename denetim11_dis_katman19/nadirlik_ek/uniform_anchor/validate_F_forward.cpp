// Independent row-subset/column-injection enumeration and forward polynomial
// convolution. The production counter instead uses row recursion + suffix DP.
#include "uniform_sis.hpp"
#include <functional>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
using Count=__uint128_t;
constexpr uint64_t HIGH=0x88888888888888ULL;
bool fits(uint64_t a,uint64_t b){return (((a|HIGH)-b)&HIGH)==HIGH;}
std::string decimal(Count x){if(!x)return "0";std::string s;while(x){s.push_back('0'+x%10);x/=10;}std::reverse(s.begin(),s.end());return s;}
int category(int n,int a){int q=(n+a)/19;if((n+a)%19||q<3||q>9)return -1;int c=2*(q%2)+n%2;return c<2?3*c+(q-4)/2:6+4*(c-2)+(q-3)/2;}
int main(int argc,char**argv){try{
 if(argc!=3)return 2;std::string root=argv[1];std::array<int,114> base{};{std::ifstream f(root+"/data/counts.txt");for(auto& a:base)f>>a;}
 uniform_anchor::Sampler p(root+"/rare_event/uniform_anchor/F_conditional_sampler_127.bin",base);base[8]=127;
 std::array<std::unordered_map<uint64_t,uint64_t>,18> polynomials;
 uint64_t all_local=0;
 for(int g=0;g<18;g++){
   int r=g+1;std::vector<int> labels;for(int id=0;id<114;id++)if(base[id]%19==19-r)labels.push_back(id);
   for(int rowmask=0;rowmask<64;rowmask++){
     std::vector<int> rows;for(int k=0;k<6;k++)if(rowmask&(1<<k))rows.push_back(r+19*k);
     std::function<void(int,unsigned,uint64_t)> inject=[&](int at,unsigned used,uint64_t code){
       if(at==int(rows.size())){polynomials[g][code]++;return;}
       for(unsigned i=0;i<labels.size();i++)if(!(used&(1u<<i))){
         int c=category(rows[at],base[labels[i]]);if(c<0||((code>>(4*c))&15)>=3)continue;
         inject(at+1,used|(1u<<i),code+(uint64_t(1)<<(4*c)));
       }
     };inject(0,0,0);
   }
   if(polynomials[g].size()!=p.local[g].size())throw std::runtime_error("Local signature support mismatch");
   std::unordered_set<uint64_t> seen;
   for(auto& loc:p.local[g]){
     if(polynomials[g].at(loc.signature)!=loc.assignments.size())throw std::runtime_error("Local multiplicity mismatch");
     for(auto& assignment:loc.assignments){
       uint64_t packed=0,code=0;bool used[114]{};
       for(int k=0;k<6;k++){
         int id=assignment[k];packed|=uint64_t(id)<<(8*k);if(id==255)continue;
         if(id>=114||used[id])throw std::runtime_error("Invalid local label injection");used[id]=true;
         int c=category(r+19*k,base[id]);if(c<0)throw std::runtime_error("Invalid F edge");code+=uint64_t(1)<<(4*c);
       }
       if(code!=loc.signature||!seen.insert(packed).second)throw std::runtime_error("Duplicate/misclassified local matching");
     }
   }
   all_local+=seen.size();
 }
 uint64_t capacity[19]{};int cap[14]{};
 for(int g=17;g>=0;g--){for(int j=0;j<14;j++){int maximum=0;for(auto& pair:polynomials[g])maximum=std::max(maximum,int((pair.first>>(4*j))&15));cap[j]=std::min(3,cap[j]+maximum);capacity[g]|=uint64_t(cap[j])<<(4*j);}}
 std::vector<Count> answers;Count sum=0;size_t max_states=0;
 for(auto rootnode:p.roots){
   const uint64_t target=p.nodes[rootnode].remaining;std::unordered_map<uint64_t,Count> current;current[0]=1;
   for(int g=0;g<18;g++){
     std::unordered_map<uint64_t,Count> next;
     for(auto& prefix:current){uint64_t remaining=target-prefix.first;for(auto& local:polynomials[g]){
       if(!fits(remaining,local.first))continue;uint64_t rest=remaining-local.first;
       if(!fits(capacity[g+1],rest))continue;
       next[prefix.first+local.first]+=prefix.second*local.second;
     }}current=std::move(next);max_states=std::max(max_states,current.size());
   }
   Count count=current[target];if(count!=p.nodes[rootnode].count)throw std::runtime_error("Forward/suffix root count mismatch");answers.push_back(count);sum+=count;
 }
 if(sum!=p.MF)throw std::runtime_error("Full F normalizer mismatch");
 std::ofstream out(argv[2]);out<<"{\"status\":\"exact_agreement\",\"method\":\"Independent row-subset and column-injection enumeration plus forward polynomial convolution\",\"variant\":127,\"local_labeled_matchings_verified\":"<<all_local<<",\"F_matching_total\":\""<<decimal(sum)<<"\",\"maximum_forward_states\":"<<max_states<<",\"pattern_counts\":[";
 for(size_t i=0;i<answers.size();i++){if(i)out<<',';out<<'"'<<decimal(answers[i])<<'"';}out<<"]}\n";
 std::cout<<"Validated local="<<all_local<<" totalMF="<<decimal(sum)<<" patterns="<<answers.size()<<"\n";
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;} }
