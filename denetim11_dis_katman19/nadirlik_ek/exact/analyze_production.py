#!/usr/bin/env python3
"""Analyze fixed-budget iid conditional experiments with exact anchor masses."""
from __future__ import annotations
from fractions import Fraction
import hashlib
import json
from pathlib import Path
from scipy.stats import beta, chisquare

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[1]
MASK=(1<<64)-1
GAMMA=0x9e3779b97f4a7c15
SALT=0xd1b54a32d192ed03

def read(name):return json.loads((HERE/name).read_text())
def cp(h,n):
    if not n:return None
    return [0.0 if h==0 else float(beta.ppf(.025,h,n-h+1)),
            1.0 if h==n else float(beta.ppf(.975,h+1,n-h))]
def estimate(h,n,mass):
    interval=cp(h,n)
    return {'hits':h,'conditional_n':n,'conditional_estimate':h/n if n else None,
            'conditional_CP95':interval,'exact_anchor_probability':float(mass),
            'unconditional_estimate':float(mass)*h/n if n else None,
            'unconditional_CP95':[float(mass)*x for x in interval]if interval else None}
def sm(seed):
    z=(seed+GAMMA)&MASK;z=((z^(z>>30))*0xbf58476d1ce4e5b9)&MASK
    z=((z^(z>>27))*0x94d049bb133111eb)&MASK
    return (z^(z>>31))&MASK

e15=read('block6_CW_exact.json');e10=read('block6_C10_exact.json')
r15=read('block6_CW_production.json');r10=read('block6_C10W_production.json')
mass15=Fraction(int(e15['count']),int(e15['space']))
mass10=Fraction(int(e10['total_CW_count']),int(e10['sample_space']))
out={'scope':'Block6 model only; conditional natural-event probabilities and subset contributions, not full scanned-score tail probabilities',
     'conditional_sampling':'Uniform labeled block-preserving orders conditional on each exactly counted anchor; independent random ranks, no MCMC or mixing assumption',
     'intervals':'Two-sided95% Clopper-Pearson for fixed-n conditional Bernoulli counts, multiplied by an exactly known anchor probability. A component interval upper endpoint is not an upper bound on the full score tail.',
     'experiments':{},'same_sample_ratio':{},'validation':{}}

for label,result,mass,planfile in [('C15_and_W',r15,mass15,'production_plan.json'),('Cpattern_centre_and_W',r10,mass10,'production_C10_plan.json')]:
    plan=read(planfile)
    assert result['n']==plan['n']==100_000_000 and result['seed']==plan['seed']
    for path,expected in plan['input_hashes'].items():
        assert hashlib.sha256((HERE/path).read_bytes()).hexdigest()==expected,(label,path)
    record={'n':result['n'],'seed':result['seed'],'threads':result['threads'],
            'exact_anchor_numerator':str(mass.numerator),'exact_anchor_denominator':str(mass.denominator),
            'seconds':result['seconds'],
            'estimates':{k:estimate(v,result['n'],mass)for k,v in result['counts'].items()},
            'production_plan_hashes_match':True}
    if 'by_Cmask'in result:
        record['by_Cmask']={m:{'conditional_draws':row['n'],'estimates':{k:estimate(v,result['n'],mass)for k,v in row['counts'].items()}}for m,row in result['by_Cmask'].items()}
        assert sum(row['n']for row in result['by_Cmask'].values())==result['n']
        for k,v in result['counts'].items():assert sum(row['counts'][k]for row in result['by_Cmask'].values())==v
    out['experiments'][label]=record

num=r10['by_Cmask']['15']['counts']['G127_M_view0'];den=r10['counts']['G127_M_view0']
out['same_sample_ratio']={'meaning':'P(C15,G127,W,Mview0) / P(Cpattern-centre,G127,W,Mview0)',
                          'numerator_hits':num,'denominator_hits':den,'estimate':num/den,
                          'CP95':cp(num,den),'reciprocal_reduction_factor':den/num,
                          'interpretation':'The ratio conditions on denominator-event hits within the SAME fixed-budget sample; it is not a product of independent predicate probabilities.'}
cg=r15['counts']['G127'];cm=r15['counts']['G127_M_view0']
c_exact=read('block6_C_exact.json')
out['conditional_chain_C15_127']={
    'P_C15_exact':c_exact['probability'],
    'P_W_given_C15_exact':e15['W_given_C_probability'],
    'P_G127_given_C15_W':{'hits':cg,'n':r15['n'],'estimate':cg/r15['n'],'CP95':cp(cg,r15['n'])},
    'P_Mview0_given_G127_C15_W':{'hits':cm,'n':cg,'estimate':cm/cg,'CP95':cp(cm,cg)},
    'P_Manyview_given_G127_C15_W':{'hits':r15['counts']['G127_M_any'],'n':cg,'estimate':r15['counts']['G127_M_any']/cg,'CP95':cp(r15['counts']['G127_M_any'],cg)},
    'interpretation':'A sequential conditional-probability factorization, with no independence assumption between G,C,W,M; matrix numerator and G denominator come from the SAME conditional sample.'}
equal=sum(r10['by_Cmask'][str(m)]['counts']['G127_M_view0']for m in (11,15))
band=sum(r10['by_Cmask'][str(m)]['counts']['G127_M_view0']for m in (14,15))
out['same_sample_added_C_conditions']={
    'base':'G127,W,Mview0,and Cpattern-centre',
    'equal_counts':{'hits':equal,'n':den,'estimate':equal/den,'CP95':cp(equal,den)},
    'band_and_constant_cell':{'hits':band,'n':den,'estimate':band/den,'CP95':cp(band,den)},
    'both':out['same_sample_ratio'],
    'band_given_equal':{'hits':num,'n':equal,'estimate':num/equal,'CP95':cp(num,equal)},
    'equal_given_band':{'hits':num,'n':band,'estimate':num/band,'CP95':cp(num,band)}}

# Independent matrix reference proves this structural zero, rather than merely
# finding no conditional successes. The 127 natural view0 event is x16,y1.
matrix=read('matrix_residue_validation.json')
assert not any(0 in x['views']for x in matrix['all_passing_pairs']['129'])
assert [(x['E_N_mod19'],x['F_N_mod19'])for x in matrix['all_passing_pairs']['127']if 0 in x['views']]==[(16,1)]
out['validation']['matrix_reference']=matrix

exact_mask={m:sum(int(x['CW_count'])for x in e10['patterns']if str(x['Cmask'])==m)for m in r10['by_Cmask']}
observed=[r10['by_Cmask'][m]['n']for m in exact_mask]
expected=[r10['n']*n/int(e10['total_CW_count'])for n in exact_mask.values()]
chi=chisquare(observed,expected)
out['validation']['Cmask_distribution']={'masks':list(exact_mask),'observed':observed,'exact_expected':expected,
    'chi2':float(chi.statistic),'df':len(exact_mask)-1,'p':float(chi.pvalue),
    'purpose':'Sampler distribution check against exact anchor stratum masses; not a Quran numerical-structure p value.'}

for filename in ('forward_count_validation.json','forward_C10_count_validation.json'):
    v=read(filename);assert v['status']=='exact_agreement'
    out['validation'][filename]={'status':v['status'],'method':v['method'],'C_count':v['C_count'],'CW_count':v['CW_count']}
out['validation']['exact_integer_division']={'python_integer_reference_cases':100000,
    'input_sha256':hashlib.sha256((HERE/'integer_test_vectors.bin').read_bytes()).hexdigest(),
    'coverage':'Random1..171bit denominators, quotients0..719, and remainders0,d−1,random; exact correction of floating quotient guesses.'}

previous=json.loads((ROOT/'review/results_validation.json').read_text())['seed_audit']
records=[dict(r,source='original_frozen_study',calls_bound=previous['total_rng_steps_conservative_upper_bound_per_worker'])for r in previous['worker_records']]
for filename in ('C15_pilot_rng_replay.json','C15_production_rng_replay.json','block6_C10W_validation_pilot.json','block6_C10W_production.json'):
    d=read(filename)
    assert sum(w['processed']for w in d['workers'])==d['n']
    for tid,w in enumerate(d['workers']):
        assert w['seed']==sm(d['seed']^((SALT*(tid+1))&MASK))
        assert w['final_rng_state']==(w['seed']+GAMMA*w['rng_calls'])&MASK
        records.append(dict(w,source=filename,thread=tid,calls_bound=w['rng_calls']))
assert r15['worker_seeds']==[w['seed']for w in read('C15_production_rng_replay.json')['workers']]
seeds=[r['seed']for r in records];assert len(seeds)==len(set(seeds))
inverse=pow(GAMMA,-1,1<<64)
coordinates=sorted((r['seed']*inverse)&MASK for r in records)
gaps=[(coordinates[(i+1)%len(coordinates)]-coordinates[i])&MASK for i in range(len(coordinates))]
minimum=min(gaps);maximum=max(r['calls_bound']for r in records)
assert minimum>maximum
out['validation']['rng_streams']={'distinct_worker_streams_including_original_training_testing':len(records),
    'all_new_seed_formulas_and_final_states_checked':True,'minimum_circular_counter_gap':minimum,
    'maximum_consumed_or_proved_upper_bound':maximum,'consumed_counter_intervals_disjoint':True,
    'records':records,'caveat':'Finite nonoverlap of the deterministic pseudorandom streams is verified; this is not a mathematical proof of statistical independence of a PRNG.'}

observed=json.loads((ROOT/'results/observed_block6_scores.json').read_text())
for r in (r15,r10):
    assert r['observed']==observed
out['validation']['observed_frozen_scores_exactly_match_saved_block6']=True
out['validation']['scope_limit']='The unconditional uniform and exact-inversion models have not been evaluated by these block6-specific conditional calculations.'
(HERE/'conditional_results.json').write_text(json.dumps(out,indent=2)+'\n')
print('Analysis and validation saved:',HERE/'conditional_results.json')
for label,data in out['experiments'].items():
    for key in ('G127','G127_M_view0','tail_mixture_intersect_Geither'):
        x=data['estimates'][key];print(label,key,x['hits'],x['unconditional_estimate'],x['unconditional_CP95'])
print('Ratio',out['same_sample_ratio'])
print('RNG streams',len(records),'minimumgap',minimum,'maxbound',maximum)
