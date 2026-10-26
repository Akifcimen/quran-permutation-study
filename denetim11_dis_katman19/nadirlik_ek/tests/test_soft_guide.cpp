#include "../src/soft_guide.hpp"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <set>
#include <string>

namespace {
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
int nearest_pattern(const study::Family& family){
 int low=family.cells[0],high=low;
 for(int x:family.cells){low=std::min(low,x);high=std::max(high,x);}
 int best=1000000000;
 for(int b=low-2;b<=high+2;++b)
  for(int row=0;row<2;++row)for(int orientation:{-1,1}){
   int desired[4]={b,b,b,b};desired[2*row]=b-orientation;desired[2*row+1]=b+orientation;
   int distance=0;for(int k=0;k<4;++k)distance+=std::abs(family.cells[k]-desired[k]);
   best=std::min(best,distance);
  }
 return best;
}
void compare_features(const study::Features&a,const study::Features&b){
 for(int v=0;v<study::V;++v){
  for(int f=0;f<study::F;++f)check(a.g[v][f]==b.g[v][f],"G extraction changed");
  for(int c=0;c<study::C;++c){
   check(a.c[v][c]==b.c[v][c],"C extraction changed");
   check(a.w[v][c]==b.w[v][c],"W extraction changed");
   check(a.m[v][c]==b.m[v][c],"M extraction changed");
  }
 }
}
std::uint64_t verify_zero_equivalence(const rare::SoftPrepared& p){
 std::uint64_t checked=0;
 for(int v=0;v<study::V;++v){
  for(int f=0;f<study::F;++f)for(int bit=0;bit<2;++bit){
   check((p.g[v][f][bit]==0)==bool((p.features.g[v][f]>>bit)&1),"G zero distance mismatch");++checked;
  }
  for(int c=0;c<study::C;++c){
   for(int bit=0;bit<4;++bit){
    check(p.c[v][c][bit]>=0,"C distance negative");
    check((p.c[v][c][bit]==0)==bool((p.features.c[v][c]>>bit)&1),"C zero distance mismatch");++checked;
   }
   for(int bit=0;bit<2;++bit){
    check(p.w[v][c][bit]>=0,"W distance negative");
    check((p.w[v][c][bit]==0)==bool((p.features.w[v][c]>>bit)&1),"W zero distance mismatch");++checked;
   }
  }
 }
 return checked;
}
}

int main(int argc,char**argv){
 try{
  check(argc==2,"Provide study root");
  const std::string root=argv[1];
  std::array<int,study::N> base{};
  std::ifstream input(root+"/data/counts.txt");for(int&a:base)check(bool(input>>a),"Missing counts");
  MatrixEngine matrix;study::Extractor original(base,matrix);
  study::Draw observed;std::iota(observed.ids.begin(),observed.ids.end(),0);
  study::RNG family_rng(202610252001ULL);
  std::uint64_t family_checks=0;
  std::uint64_t exhaustive_pattern_checks=0;
  // Independently validate the closed-form pattern distance over all small
  // integer four-cell sums, including configurations absent from IID draws.
  for(int a=-5;a<=5;++a)for(int b=-5;b<=5;++b)
   for(int c=-5;c<=5;++c)for(int d=-5;d<=5;++d){
    study::Family family;
    family.cells[0]=a;family.cells[1]=b;family.cells[2]=c;family.cells[3]=d;
    const auto distance=rare::SoftGuide::family_distances(family,19);
    check(distance[1]==std::log2(1.0+nearest_pattern(family)),"Closed-form pattern distance changed");
    ++exhaustive_pattern_checks;
   }
  // Random raw families exercise empty supports, singleton cells, gaps, ties,
  // negative q, and nearest-pattern distances independently of permutation data.
  for(int example=0;example<10000;++example){
   study::Family family;
   const int count=int(family_rng.bounded(25));
   std::set<int> qs;
   for(int k=0;k<count;++k){
    const int q=int(family_rng.bounded(21))-10;
    family.add(k+1,k+3,q,int(family_rng.bounded(4)),false);qs.insert(q);
   }
   const int p=MatrixEngine::kPrimes[family_rng.bounded(29)];
   const auto d=rare::SoftGuide::family_distances(family,p);
   const int mask=family.mask(p);
   for(int bit=0;bit<4;++bit){
    check(d[bit]>=0,"Negative family distance");
    check((d[bit]==0)==bool((mask>>bit)&1),"Random family zero equivalence failed");++family_checks;
   }
   check(std::fabs(d[1]-std::log2(1.0+nearest_pattern(family)))<1e-12,"Nearest-centre median minimization failed");
   ++family_checks;
  }
  // Explicit complete crystal, empty family, and one-hole support examples.
  for(int which=0;which<3;++which){
   study::Family family;
   const int cells[4][3]={{6,6,8},{6,6,6},{5,5,9},{5,7,7}};
   if(which!=1)for(int cell=0;cell<4;++cell)for(int j=0;j<3;++j)
    family.add(3*cell+j+1,7,(which==2&&cells[cell][j]==8)?10:cells[cell][j],cell,false);
   const auto d=rare::SoftGuide::family_distances(family,19);
   for(int bit=0;bit<4;++bit)check((d[bit]==0)==bool((family.mask(19)>>bit)&1),"Constructed family zero mismatch");
   if(which==0){check(family.mask(19)==15,"Constructed crystal is not full C");for(double x:d)check(x==0,"Crystal positive distance");}
   if(which==1)check(d[2]>0,"Empty band distance must be positive");
  }

  const double lambdas[]={0,.05,.1,.25,.5,1,2,4,8,16,32,1000000,std::numeric_limits<double>::infinity()};
  constexpr int L=sizeof(lambdas)/sizeof(double);
  const char* names[]={"uniform","block6","inversion_exact"};
  std::uint64_t zero_checks=0,feature_checks=0,hard_limit_checks=0,monotonicity_checks=0,upper_checks=0;
  std::cout<<std::setprecision(17)<<"{\"status\":\"passed\",\"random_family_checks\":"<<family_checks
           <<",\"exhaustive_pattern_checks\":"<<exhaustive_pattern_checks<<",\"models\":[";
  for(int model=0;model<3;++model){
   const auto started=std::chrono::steady_clock::now();
   study::Hist hist;hist.load(root+"/results/train_"+names[model]+".bin");
   rare::SoftGuide guide(base,hist,matrix);
   study::Scorer hard_scorer(hist);
   study::Sampler iid(base,model,202610252101ULL+model);
   study::RNG near_rng(202610252201ULL+model);
   const auto observed_prepared=guide.prepare(observed);
   const double exact_observed=hard_scorer(original(observed)).value[1];
   const double s0=guide.evaluate(observed_prepared,0);
   const auto retained=guide.retained_mask_counts();
   double mean[L]{},maximum[L]{};
   std::ofstream csv(root+"/rare_event/tests/soft_guide_snapshot_"+names[model]+".csv");
   csv<<"sample,hard";for(double lambda:lambdas)csv<<",lambda_"<<lambda;csv<<"\n";
   double max_hard=0;
   for(int sample=-101;sample<500;++sample){
    study::Draw draw;
    if(sample==-101)draw=observed;
    else if(sample<0){
     draw=observed;
     const int swaps=(-sample)%12;
     for(int j=0;j<swaps;++j){int a=int(near_rng.bounded(study::N)),b=int(near_rng.bounded(study::N));std::swap(draw.ids[a],draw.ids[b]);}
    }else draw=iid.next();
    auto p=guide.prepare(draw);
    const auto frozen=original(draw);
    compare_features(p.features,frozen);feature_checks+=2*(study::F+3*study::C);
    zero_checks+=verify_zero_equivalence(p);
    const double hard=hard_scorer(frozen).value[1];
    check(guide.hard(p)==hard,"Original frozen score through prepared features changed");
    check(guide.evaluate(p,guide.exact_hard_lambda())==hard,"Finite exact-hard endpoint mismatch");++hard_limit_checks;
    double previous=std::numeric_limits<double>::infinity();
    if(sample>=0)csv<<sample<<","<<std::setprecision(17)<<hard;
    for(int j=0;j<L;++j){
     const double soft=guide.evaluate(p,lambdas[j]);
     check(std::isfinite(soft),"Nonfinite soft score");
     check(soft+1e-12>=hard,"Soft guide is below frozen hard score");++upper_checks;
     check(soft<=previous+1e-12,"Soft guide is not monotone");++monotonicity_checks;
     if(lambdas[j]>=1000000){check(soft==hard,"Large-lambda guide is not bit-exact hard score");++hard_limit_checks;}
     if(j==0)check(soft==s0,"Lambda-zero guide is not constant across states");
     previous=soft;
     if(sample>=0){csv<<","<<soft;mean[j]+=soft/500;maximum[j]=std::max(maximum[j],soft);}
    }
    if(sample>=0){csv<<"\n";max_hard=std::max(max_hard,hard);}
   }
   if(model)std::cout<<",";
   std::cout<<"{\"model\":\""<<names[model]<<"\",\"iid_samples\":500,\"near_observed_cases\":101,\"observed_hard\":"<<exact_observed
            <<",\"lambda_zero_constant\":"<<s0<<",\"exact_hard_lambda\":"<<guide.exact_hard_lambda()<<",\"retained_positive_masks\":[";
   for(int j=0;j<4;++j){if(j)std::cout<<",";std::cout<<retained[j];}
   std::cout<<"],\"iid_max_hard\":"<<max_hard<<",\"lambda_summaries\":[";
   for(int j=0;j<L;++j){
    if(j)std::cout<<",";
    std::cout<<"{\"lambda\":";
    if(std::isinf(lambdas[j]))std::cout<<"\"infinity\"";else std::cout<<lambdas[j];
    std::cout<<",\"mean\":"<<mean[j]<<",\"maximum\":"<<maximum[j]<<"}";
   }
   std::cout<<"],\"seconds\":"<<std::chrono::duration<double>(std::chrono::steady_clock::now()-started).count()<<"}";
  }
  std::cout<<"],\"zero_equivalence_checks\":"<<zero_checks<<",\"feature_mask_comparisons\":"<<feature_checks
           <<",\"hard_limit_checks\":"<<hard_limit_checks<<",\"upper_envelope_checks\":"<<upper_checks
           <<",\"monotonicity_checks\":"<<monotonicity_checks<<"}\n";
  return 0;
 }catch(const std::exception&e){std::cerr<<"Soft-guide validation failed: "<<e.what()<<"\n";return 1;}
}

