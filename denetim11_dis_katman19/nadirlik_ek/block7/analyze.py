#!/usr/bin/env python3
"""Fixed-proposal binomial analysis of the frozen two-batch block7 study."""
import hashlib
import json
import math
import pathlib
from fractions import Fraction

from scipy.stats import beta

HERE=pathlib.Path(__file__).resolve().parent
ROOT=HERE.parents[1]
POLICIES=('uniform','mixture','literal','discussion','anchored19','frequency')

def cp(k,n,alpha=.05):
    return (0.0 if k==0 else float(beta.ppf(alpha/2,k,n-k+1)),
            1.0 if k==n else float(beta.ppf(1-alpha/2,k+1,n-k)))

def main():
    plan=json.loads((HERE/'production_plan.json').read_text())
    runs=[json.loads((HERE/b['output']).read_text()) for b in plan['batches']]
    independent=json.loads((HERE/'independent_intersections.json').read_text())
    denominator=int(independent['sample_space_size'])
    B=Fraction(int(independent['sum_anchor_counts']),denominator)
    union=Fraction(int(independent['union_count']),denominator)
    scale=float(B);acceptance=float(union/B)
    for spec,run in zip(plan['batches'],runs):
        assert run['n']==spec['n'] and run['seed']==spec['seed']
        assert run['processed']==run['n'] and run['primary_complete_within_union']
        assert math.isclose(run['mixture_mass_sum_b'],scale,rel_tol=2e-15)
    n=sum(r['n'] for r in runs)
    assert n==plan['total_proposals']
    accepted=sum(r['thinning_accepted'] for r in runs)
    accept_z=(accepted/n-acceptance)/math.sqrt(acceptance*(1-acceptance)/n)
    per_policy=[]
    for policy,name in enumerate(POLICIES):
        k=sum(r['tail_estimates'][policy]['thinned_hits'] for r in runs)
        raw=sum(r['tail_estimates'][policy]['unthinned_hits'] for r in runs)
        sums=sum(r['tail_estimates'][policy]['sum_weighted_hits'] for r in runs)
        squares=sum(r['tail_estimates'][policy]['sum_squared_weighted_hits'] for r in runs)
        weighted=sums/n
        weighted_se=math.sqrt(max(0.0,(squares-sums*sums/n)/(n-1)/n))
        intervals=[scale*x for x in cp(k,n)]
        diff_variance=0.0
        for run in runs:
            for row in run['membership']:
                multiplicity=int(row['mask']).bit_count()
                if multiplicity:
                    diff_variance+=scale*scale*row['tail_counts'][policy]*(multiplicity-1)/(multiplicity*multiplicity)
        diff_se=math.sqrt(diff_variance)/n
        point=scale*k/n
        per_policy.append(dict(policy=policy,name=name,n=n,thinned_hits=k,unthinned_hits=raw,
                               estimate=point,CP95=intervals,weighted_estimate=weighted,
                               weighted_standard_error=weighted_se,
                               weighted_minus_thinned_standardized_difference=(weighted-point)/diff_se if diff_se else None,
                               complete_within_W7_union=(policy==1),
                               scope='Full frozen primary tail intersect W7 union' if policy==1 else 'Policy tail intersect W7 union and any G balance; completeness within union not asserted'))
    primary=per_policy[1]
    ordinary=json.loads((ROOT/'results/summary.json').read_text())['models']['block6']['policies']['mixture']
    assert ordinary['hits']==0 and ordinary['n']==1000000
    # This is a one-sided97.5% upper endpoint, not the earlier95% endpoint.
    full_upper=-math.expm1(math.log(.025)/ordinary['n'])
    lower=primary['CP95'][0]
    full_ci=[lower,full_upper]
    full_wait=[1/full_upper,1/lower]
    natural=json.loads((ROOT/'rare_event/exact/conditional_results.json').read_text())['experiments']['C15_and_W']['estimates']['G127_M_view0']
    natural_point=natural['unconditional_estimate'];natural_ci=natural['unconditional_CP95']
    ratio_point=primary['estimate']/natural_point
    ratio_lower=lower/natural_ci[1]
    batches=[]
    for r in runs:
        p=r['tail_estimates'][1]
        batches.append(dict(n=r['n'],seed=r['seed'],hits=p['thinned_hits'],estimate=p['estimate'],weighted_estimate=p['weighted_estimate'],seconds=r['seconds']))
    pooled=primary['thinned_hits']/n
    batch_z=((batches[0]['hits']/batches[0]['n'])-(batches[1]['hits']/batches[1]['n']))/math.sqrt(pooled*(1-pooled)*(1/batches[0]['n']+1/batches[1]['n']))
    # Exact counter positions for each actual SplitMix stream, including pilot.
    rng_runs=[json.loads((HERE/'pilot.json').read_text()),*runs]
    modulus=1<<64;increment=0x9e3779b97f4a7c15;inverse=pow(increment,-1,modulus)
    streams=[]
    for run in rng_runs:
        for worker in run['workers']:
            calls=((worker['final_rng_state']-worker['seed'])*inverse)%modulus
            assert calls>=4*worker['processed']
            streams.append(dict(seed=worker['seed'],counter=(worker['seed']*inverse)%modulus,calls=calls,processed=worker['processed']))
    streams.sort(key=lambda x:x['counter'])
    gaps=[]
    for i,s in enumerate(streams):
        gap=(streams[(i+1)%len(streams)]['counter']-s['counter'])%modulus
        assert s['calls']<=gap
        gaps.append(gap)
    out=dict(scope='Block6 model; rigorous component estimate and rigorous full-tail bounds, not a precise full-tail point estimate',
             n=n,primary_hits=primary['thinned_hits'],B_exact={'numerator':str(B.numerator),'denominator':str(B.denominator)},B=scale,
             p_component=primary['estimate'],CP95=primary['CP95'],weighted_component=primary['weighted_estimate'],weighted_component_SE=primary['weighted_standard_error'],
             exact_union_mass={'numerator':str(union.numerator),'denominator':str(union.denominator),'probability':float(union)},
             thinning_acceptance_validation={'expected':acceptance,'accepted':accepted,'observed':accepted/n,'standardized_difference':accept_z},
             full_block6_primary_tail_CI95=full_ci,
             full_block6_expected_waiting_CI95=full_wait,
             full_tail_interval_justification='P_full>=P_component. Component CP95 lower endpoint is a one-sided97.5% lower bound; original0/1m one-sided97.5% upper is1-.025^(1/1m). Bonferroni failure<=.025+.025 gives joint coverage>=95%.',
             component_expected_waiting_point=1/primary['estimate'],component_expected_waiting_CI95=[1/primary['CP95'][1],1/primary['CP95'][0]],
             component_waiting_scope='Waiting for this subset event only; NOT a point estimate of full-tail waiting',
             natural19_pattern={'estimate':natural_point,'CP95':natural_ci,'hits':natural['hits'],'n':natural['conditional_n'],'event':'127 natural G3,C15,W3,M15 in view0'},
             component_to_natural19_point_ratio=ratio_point,
             full_tail_to_natural19_ratio_one_sided_lower95=ratio_lower,
             ratio_justification='P_full/P_natural>=P_component/P_natural. Divide component one-sided97.5% lower by natural one-sided97.5% upper; Bonferroni gives>=95% lower-bound coverage. This is not a point estimate of the full-tail ratio.',
             per_policy=per_policy,batches=batches,batch_agreement_standardized_difference=batch_z,
             direct_feature_validations=sum(r['direct_validations'] for r in runs),
             rng_nonoverlap={'streams_checked_including_pilot':len(streams),'maximum_actual_calls':max(x['calls'] for x in streams),'minimum_counter_gap':min(gaps),'all_consumed_counter_intervals_disjoint':True},
             pilot_excluded=True,all_fixed_proposals_retained=True,
             source_sha256={p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in [HERE/'production_plan.json',HERE/'production_batch0.json',HERE/'production_batch1.json',HERE/'independent_counts.json',HERE/'independent_intersections.json']})
    (HERE/'final_summary.json').write_text(json.dumps(out,indent=2)+'\n')
    text=f'''# Block7 component result

Across **{n:,} fixed production proposals**, the exact size-weighted mixture and1/K thinning produced **{primary['thinned_hits']:,} retained primary exceedances**. The estimated probability of the frozen primary tail intersecting the six-anchor W7 union is **{primary['estimate']:.10g}**, with exact scaled-binomial95% interval **[{primary['CP95'][0]:.10g}, {primary['CP95'][1]:.10g}]**.

The importance-weighted cross-check is {primary['weighted_estimate']:.10g} (standard error {primary['weighted_standard_error']:.5g}). The two fixed batches differ by {batch_z:.3f} binomial standard deviations. The observed thinning acceptance differs from its independently known exact value by {accept_z:.3f} standard deviations. The pilot was excluded.

## Rigorous bounds on the full frozen block6 tail

The component is a subset of the full event. Combining its one-sided97.5% lower endpoint with the original0/1m one-sided97.5% upper endpoint gives the conservative **95% full-tail interval [{lower:.10g}, {full_upper:.10g}]**. This uses the union bound for the two endpoint errors; no independence assumption is needed.

The corresponding mean waiting-time interval is **[{full_wait[0]:,.3f}, {full_wait[1]:,.3f}] independent block6 draws**. It is a confidence interval for the mean waiting time of a geometric distribution,1/p, not a promise about when an individual hit will occur. The full-tail probability still has no precise point estimate from this calculation.

For the W7 subset event alone, the plug-in mean waiting estimate is {1/primary['estimate']:,.3f}, with interval [{1/primary['CP95'][1]:,.3f}, {1/primary['CP95'][0]:,.3f}]. This must not be reported as the full-tail mean.

## Comparison with the natural19 pattern

The separately estimated natural127 G3,C15,W3,M15(view0) event has probability {natural_point:.10g} and95% interval [{natural_ci[0]:.10g}, {natural_ci[1]:.10g}]. The W7 component point estimate is {ratio_point:.6g} times as frequent. A conservative **95% one-sided lower bound on the full-tail/natural-pattern probability ratio is {ratio_lower:.6g}**, using the component lower and natural-pattern upper97.5% endpoints.

The primary result retains the complete frozen scorer, both variants, and all grammar alternatives. Only the auxiliary proposal is conditioned on the W7 union. The complement is not assumed empty. Secondary policy counters additionally intersect any G balance and are labeled accordingly in the JSON.

Detailed exact masses, per-policy counters, individual batch values, confidence formulas, source hashes, and validation results are in `final_summary.json`.
'''
    (HERE/'final_summary.md').write_text(text)
    print(json.dumps({k:out[k] for k in ('n','primary_hits','B','p_component','CP95','full_block6_primary_tail_CI95','full_block6_expected_waiting_CI95','component_to_natural19_point_ratio','full_tail_to_natural19_ratio_one_sided_lower95')},indent=2))

if __name__=='__main__':main()
