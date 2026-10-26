#!/usr/bin/env python3
from fractions import Fraction
import json
import math
from pathlib import Path
from scipy.stats import beta,chisquare,norm

HERE=Path(__file__).resolve().parent
read=lambda name:json.loads((HERE/name).read_text())
F=read('F_matching_count_127.json');MF=int(F['F_partial_matching_total'])
factor=Fraction(MF,math.prod(range(103,115)))
files=['pilot_smoke.json','pilot_sis_moments.json','pilot_rejection.json','pilot_fullscore_timing.json','pilot_uppergate_timing.json','pilot_alias.json']
results={}
for name in files:
    r=read(name);n=r['n'];pats=r['pattern_counts']
    expected=[n*int(c)/MF for c in F['F_partial_matching_counts_by_pattern']]
    chi=chisquare(pats,expected)
    metrics={}
    for m in r['metrics']:
        total=float(m['weight_sum']);square=float(m['weight_square_sum']);mean=total/n
        variance=max(0,(square-total*total/n)/(n*(n-1)))
        se=math.sqrt(variance)
        metrics[m['name']]={'raw_hits':m['hits'],'estimate':mean,'variance_of_mean':variance,'SE':se,
                            'normal_approx_95_interval':[max(0,mean-1.959963984540054*se),mean+1.959963984540054*se],
                            'ESS':total*total/square if square else 0,'max_weight':m['max_weight'],
                            'max_fraction_of_sum':m['max_weight']/total if total else None}
    results[name]={'phase':'pilot only','n':n,'seed':r['seed'],'method':r['method'],'seconds':r['seconds'],
                   'tail_scope':r['tail_scope'],'scoremode':r['score_mode'],'metrics':metrics,
                   'success_fraction':r['successes']/n,'F_bag_support_check':{'chi2':float(chi.statistic),'df':19,'p':float(chi.pvalue),'min_count':min(pats),'all20bags_observed':all(pats)},
                   'direct_feature_validations':r['validations'],'max_direct_product_vs_log_weight_error':r['max_direct_product_vs_log_weight_error']}
r=read('pilot_rejection.json');h=r['successes'];n=r['n']
lo=float(beta.ppf(.025,h,n-h+1));hi=float(beta.ppf(.975,h+1,n-h))
rejection_interval=[float(factor)*lo,float(factor)*hi]
alias=results['pilot_alias.json']['metrics']['anchor_C15_W_127']
out={'phase':'feasibility and pilot diagnostics, not final rarity estimates',
     'F_matching_normalizer':str(MF),'marked_rejection_factor_exact':str(factor),
     'all_runs':results,
     'independent_anchor_comparison':{'rejection_hits':h,'rejection_n':n,'rejection_estimate':float(factor)*h/n,
       'rejection_exact_binomial_scaled_CP95':rejection_interval,'SIS_alias_anchor_estimate':alias['estimate'],
       'SIS_alias_anchor_SE':alias['SE'],'SIS_estimate_inside_rejection_CP95':rejection_interval[0]<=alias['estimate']<=rejection_interval[1]},
     'production_budget_recommendation':{'total_n':100000000,'runs':2,'n_per_run':50000000,
       'reason':'Roughly100 natural matrix hits expected from pilots; enough to resolve rarity with explicit weighted uncertainty. Budget selected before fresh production outcomes.',
       'score_scope':'Only the intersection of the full frozen score tail with natural Geither and the127 C15/W anchor. Full-score evaluation on every anchor proposal would take hours at the measured timing.'},
     'uncertainty_warning':'Importance weights are nonconstant. Weighted normal/CLT intervals are approximations; they are not Clopper-Pearson. Pilot matrix estimates with one hit are not reportable final rarity estimates.',
     'support_validation':['sis_toy_validation.json','F_forward_validation.json','every integer alias mass checked against exact CDF counts']}
(HERE/'pilot_summary.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps(out['independent_anchor_comparison'],indent=2))
print('Alias ESS:',alias['ESS'],'maxfraction:',alias['max_fraction_of_sum'])
