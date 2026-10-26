#ifndef JOINT_RARE_EVENT_SOFT_GUIDE_HPP
#define JOINT_RARE_EVENT_SOFT_GUIDE_HPP

#include "../../src/scorer.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <vector>

namespace rare {

struct SoftPrepared {
    study::Features features{};
    double g[study::V][study::F][2]{};
    double c[study::V][study::C][4]{};
    double w[study::V][study::C][2]{};
    // Best trained matrix utility at each integer number of missing predicates.
    double m[study::V][study::C][5]{};
};

// An auxiliary sampling guide, NOT a replacement statistic.
//
// Each positive predicate mask is allowed with penalty lambda*distance. Every
// component distance is nonnegative and vanishes exactly when its frozen hard
// predicate holds. Consequently S_lambda >= the frozen primary score, S_lambda
// decreases with lambda, and its infinite-lambda limit is the hard score.
//
// All primary-policy costs, form/prime options, variants, views, activation
// choices and set partitions are retained. The final event must still use the
// original Scorer; hard(prepared) does precisely that.
class SoftGuide {
public:
    SoftGuide(const std::array<int,study::N>& base129,
              const study::Hist& hist, const ::MatrixEngine& matrix)
        : base_(base129), matrix_(matrix), frozen_scorer_(hist) {
        if (hist.n==0) throw std::invalid_argument("SoftGuide needs nonempty training");
        build(hist.g,study::V*study::F,4,2,hist.n,ug_);
        build(hist.c,study::V*study::C,16,4,hist.n,uc_);
        build(hist.w,study::V*study::C,4,2,hist.n,uw_);
        build(hist.m,study::V*study::C*8,16,7,hist.n,um_);
        for(const auto* groups:{&ug_,&uc_,&uw_,&um_})
            for(const auto& list:*groups)for(const auto& term:list)
                hard_lambda_=std::max(hard_lambda_,term.utility);
        for(int pi=0;pi<study::P;++pi) {
            const int p=::MatrixEngine::kPrimes[pi];
            const bool literal=p==3||p==5||p==7||p==11||p==19;
            const bool discussion=p==3||p==5||p==7||p==19;
            const double uniform=1.0/study::P;
            const double lw=literal?1.0/5:0;
            const double dw=discussion?1.0/4:0;
            logq_[pi]=std::log2((uniform+lw+dw)/3);
        }
        for(int value=0;value<=1200;++value)
            for(int pi=0;pi<study::P;++pi)
                if(value % ::MatrixEngine::kPrimes[pi]==0)
                    divisors_[value]|=std::uint32_t(1)<<pi;
        matrix_envelopes_.resize(um_.size());
        for(std::size_t group=0;group<um_.size();++group) {
            auto& table=matrix_envelopes_[group];
            table.fill(negative_infinity());
            for(int actual=0;actual<16;++actual)
                for(const auto& term:um_[group]) {
                    const int missing=__builtin_popcount(unsigned(term.mask)&unsigned(~actual)&15u);
                    table[actual*5+missing]=std::max(table[actual*5+missing],term.utility);
                }
        }
    }

    SoftPrepared prepare(const study::Draw& draw) const {
        SoftPrepared out;
        for(int variant=0;variant<study::V;++variant) {
            std::array<int,study::N> values{};
            int total=0;
            for(int i=0;i<study::N;++i) {
                values[i]=(variant==1&&draw.ids[i]==8)?127:base_[draw.ids[i]];
                total+=values[i];
            }
            for(int form=0;form<study::F;form+=2) {
                study::Family family[2][study::P];
                int even_n=0,even_a=0,even_count=0;
                const int alpha=study::AL[form],beta=study::BE[form];
                for(int i=0;i<study::N;++i) {
                    const int n=i+1,a=values[i];
                    const int plus=alpha*n+beta*a,minus=alpha*n-beta*a;
                    const int parity=plus&1;
                    if(!parity){++even_count;even_n+=n;even_a+=a;}
                    const auto dp=divisors_[std::abs(plus)],dm=divisors_[std::abs(minus)];
                    const int cell=2*parity+(n&1);
                    auto bits=dp;
                    while(bits) {
                        const int pi=__builtin_ctz(bits);bits&=bits-1;
                        family[0][pi].add(n,a,plus/::MatrixEngine::kPrimes[pi],cell,(dm>>pi)&1);
                    }
                    bits=dm;
                    while(bits) {
                        const int pi=__builtin_ctz(bits);bits&=bits-1;
                        family[1][pi].add(n,a,minus/::MatrixEngine::kPrimes[pi],cell,(dp>>pi)&1);
                    }
                }
                const int gmask=(even_count==57)|((even_n==total-even_a)<<1);
                for(int sign=0;sign<2;++sign) {
                    out.features.g[variant][form+sign]=gmask;
                    out.g[variant][form+sign][0]=log_distance(std::abs(even_count-57));
                    out.g[variant][form+sign][1]=log_distance(std::abs(even_n-(total-even_a)));
                }
                const ::MatrixEngine::Matrix pm={even_n,even_a,6555-even_n,total-even_a};
                for(int pi=0;pi<study::P;++pi) {
                    const int p=::MatrixEngine::kPrimes[pi];
                    const auto& one=family[0][pi];
                    const auto& two=family[1][pi];
                    const int union_n=one.count+two.count-one.overlap;
                    const int union_a=one.sa+two.sa-one.overlap_a;
                    const int rem=union_a%p;
                    const int wmask=((one.overlap==0&&union_n==p)?1:0)|((rem==0)?2:0);
                    for(int sign=0;sign<2;++sign) {
                        const auto& ff=family[sign][pi];
                        const int cx=(form+sign)*study::P+pi;
                        out.features.c[variant][cx]=ff.mask(p);
                        out.features.w[variant][cx]=wmask;
                        const auto distances=family_distances(ff,p);
                        for(int bit=0;bit<4;++bit)out.c[variant][cx][bit]=distances[bit];
                        out.w[variant][cx][0]=log_distance(std::abs(union_n-p)+one.overlap);
                        out.w[variant][cx][1]=log_distance(std::min(rem,p-rem));
                        const ::MatrixEngine::Matrix rm={ff.sn,ff.sa,6555-ff.sn,total-ff.sa};
                        const auto packed=matrix_.eval(pi,pm,rm);
                        out.features.m[variant][cx]=packed;
                        std::fill(out.m[variant][cx],out.m[variant][cx]+5,negative_infinity());
                        for(int view=0;view<8;++view) {
                            const unsigned actual=(packed>>(4*view))&15;
                            const auto& table=matrix_envelopes_[(variant*study::C+cx)*8+view];
                            for(int missing=0;missing<=4;++missing)
                                out.m[variant][cx][missing]=std::max(out.m[variant][cx][missing],table[actual*5+missing]);
                        }
                    }
                }
            }
        }
        return out;
    }

    double evaluate(const study::Draw& draw,double lambda) const {
        return evaluate(prepare(draw),lambda);
    }

    double evaluate(const SoftPrepared& features,double lambda) const {
        if(!(lambda>=0))throw std::invalid_argument("Guide lambda must be nonnegative");
        constexpr int bell[5]={1,1,2,5,15};
        const double form_cost=std::log2(double(study::F));
        double answer=0;
        for(int variant=0;variant<study::V;++variant) {
            std::array<std::array<double,16>,study::P> prime_best;
            for(auto& values:prime_best)values.fill(negative_infinity());
            double g_only=negative_infinity();
            for(int form=0;form<study::F;++form) {
                const double gu=utility(ug_[variant*study::F+form],features.g[variant][form],lambda);
                if(gu>0)g_only=std::max(g_only,gu-form_cost);
                for(int pi=0;pi<study::P;++pi) {
                    const int cx=form*study::P+pi,group=variant*study::C+cx;
                    const double cu=utility(uc_[group],features.c[variant][cx],lambda);
                    const double wu=utility(uw_[group],features.w[variant][cx],lambda);
                    double mu=negative_infinity();
                    for(int missing=0;missing<=4;++missing) {
                        const double intercept=features.m[variant][cx][missing];
                        if(std::isfinite(intercept))
                            mu=std::max(mu,penalize(intercept,double(missing),lambda));
                    }
                    const double u[4]={gu,cu,wu,mu};
                    unsigned available=0;
                    for(unsigned module=0;module<4;++module)if(u[module]>0)available|=1u<<module;
                    for(unsigned block=available;block;block=(block-1)&available) {
                        if(block==1)continue;
                        double sum=0;
                        for(int module=0;module<4;++module)if(block&(1u<<module))sum+=u[module];
                        prime_best[pi][block]=std::max(prime_best[pi][block],sum-form_cost);
                    }
                }
            }
            std::array<double,16> block_best,best;
            block_best.fill(negative_infinity());
            block_best[1]=g_only;
            for(int block=2;block<16;++block)
                for(int pi=0;pi<study::P;++pi)
                    block_best[block]=std::max(block_best[block],prime_best[pi][block]+logq_[pi]);
            best.fill(negative_infinity());best[0]=0;
            for(int active=1;active<16;++active) {
                const int anchor=active&-active;
                for(int block=active;block;block=(block-1)&active) {
                    if(!(block&anchor))continue;
                    best[active]=std::max(best[active],block_best[block]+best[active^block]);
                }
                const double penalty=5+std::log2(double(bell[__builtin_popcount(unsigned(active))]));
                answer=std::max(answer,best[active]-penalty);
            }
        }
        return answer;
    }

    // Exactly the original frozen statistic, including its numerical order of
    // operations and >= tie behavior; use this for the final rare-event flag.
    double hard(const SoftPrepared& prepared) const {
        return frozen_scorer_(prepared.features).value[1];
    }
    double hard(const study::Draw& draw) const { return hard(prepare(draw)); }

    // Every nonzero component distance is at least one. Once lambda reaches
    // the largest positive module utility, every unsatisfied mask has utility
    // <=0 and cannot improve a program. This finite endpoint is exactly T.
    double exact_hard_lambda() const {
        return std::nextafter(hard_lambda_,std::numeric_limits<double>::infinity());
    }

    // Public solely to permit independent exact-zero/nearest-pattern tests.
    static std::array<double,4> family_distances(const study::Family& family,int p) {
        const int minimum=*std::min_element(family.celln,family.celln+4);
        const int maximum=*std::max_element(family.celln,family.celln+4);
        const int equal_residual=maximum-minimum+(minimum==0?1:0);
        int pattern_residual=std::numeric_limits<int>::max();
        for(int equal_row=0;equal_row<2;++equal_row)
            for(int orientation:{-1,1}) {
                int offsets[4]={0,0,0,0};
                const int unequal_row=1-equal_row;
                offsets[2*unequal_row]=-orientation;
                offsets[2*unequal_row+1]=orientation;
                std::array<int,4> adjusted;
                for(int cell=0;cell<4;++cell)adjusted[cell]=family.cells[cell]-offsets[cell];
                auto sorted=adjusted;
                std::sort(sorted.begin(),sorted.end());
                const int centre=sorted[1];
                int residual=0;
                for(int cell=0;cell<4;++cell)residual+=std::abs(adjusted[cell]-centre);
                pattern_residual=std::min(pattern_residual,residual);
            }
        int distinct=0;
        for(auto word:family.seen)distinct+=__builtin_popcountll(word);
        const int holes=family.count>0?family.qmax-family.qmin+1-distinct:1;
        int constant_cell_residual=std::numeric_limits<int>::max();
        for(int cell=0;cell<4;++cell) {
            const int range=family.celln[cell]>0?family.hi[cell]-family.lo[cell]:0;
            constant_cell_residual=std::min(constant_cell_residual,
                range+std::max(0,2-family.celln[cell]));
        }
        const int band_residual=holes+constant_cell_residual;
        const int coefficient_sum=family.cells[0]+family.cells[1]+family.cells[2]+family.cells[3];
        return {log_distance(equal_residual),log_distance(pattern_residual),
                log_distance(band_residual),log_distance(std::abs(coefficient_sum-4*p))};
    }

    std::array<std::size_t,4> retained_mask_counts() const {
        std::array<std::size_t,4> counts{};
        const std::vector<std::vector<Term>>* groups[4]={&ug_,&uc_,&uw_,&um_};
        for(int module=0;module<4;++module)
            for(const auto& list:*groups[module])counts[module]+=list.size();
        return counts;
    }

private:
    struct Term {unsigned mask;double utility;};
    std::array<int,study::N> base_{};
    const ::MatrixEngine& matrix_;
    study::Scorer frozen_scorer_;
    std::array<std::uint32_t,1201> divisors_{};
    std::array<double,study::P> logq_{};
    std::vector<std::vector<Term>> ug_,uc_,uw_,um_;
    std::vector<std::array<double,16*5>> matrix_envelopes_;
    double hard_lambda_=0;

    static constexpr double negative_infinity() {
        return -std::numeric_limits<double>::infinity();
    }
    static double log_distance(int residual) {
        if(residual<0)throw std::logic_error("Negative guide distance");
        return std::log2(1.0+double(residual));
    }
    static double penalize(double value,double distance,double lambda) {
        return distance==0?value:value-lambda*distance;
    }
    static double utility(const std::vector<Term>& terms,const double* distances,double lambda) {
        double best=negative_infinity();
        for(const auto& term:terms) {
            double distance=0;
            for(unsigned bits=term.mask;bits;bits&=bits-1)
                distance+=distances[__builtin_ctz(bits)];
            best=std::max(best,penalize(term.utility,distance,lambda));
        }
        return best;
    }
    static void build(const std::vector<std::uint64_t>& counts,int groups,int states,
                      int code_cost,std::uint64_t n,std::vector<std::vector<Term>>& result) {
        if(counts.size()!=std::size_t(groups*states))
            throw std::invalid_argument("Wrong training histogram shape for SoftGuide");
        result.resize(groups);
        for(int group=0;group<groups;++group) {
            const int offset=group*states;
            std::uint64_t total=0;
            for(int state=0;state<states;++state)total+=counts[offset+state];
            if(total!=n)throw std::invalid_argument("Training histogram total mismatch");
            std::array<double,16> values;
            values.fill(negative_infinity());
            for(int mask=1;mask<states;++mask) {
                std::uint64_t count=0;
                for(int state=0;state<states;++state)
                    if((state&mask)==mask)count+=counts[offset+state];
                values[mask]=-std::log2((double(count)+0.5)/(double(n)+1))-code_cost;
            }
            for(int mask=1;mask<states;++mask) {
                if(!(values[mask]>0))continue;
                bool dominated=false;
                for(int sub=(mask-1)&mask;sub;sub=(sub-1)&mask)
                    if(values[sub]>=values[mask]){dominated=true;break;}
                if(!dominated)result[group].push_back({unsigned(mask),values[mask]});
            }
        }
    }
};

} // namespace rare
#endif

