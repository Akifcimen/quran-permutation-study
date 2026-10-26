#!/usr/bin/env python3
"""Independent saved-state and replicate-level review of fixed SMC production.

Run --pilot to validate this reviewer against the six completed guided pilots.
Partial production files are identified as pending, never treated as zeros.
Completed zero estimates remain in every replicate mean and standard error.
"""
import argparse
from datetime import datetime,timezone
from hashlib import sha256
import json
import math
from pathlib import Path
import subprocess

from review_seed_streams import audit as seed_audit,worker_seed

ROOT=Path(__file__).resolve().parents[2]
HERE=Path(__file__).resolve().parent
MODELS=('uniform','block6','inversion_exact')
POLICIES=('uniform_29','primary_mixture','literal_integer_values','number_discussion_values','conditional_19','literal_frequency')
NATURAL=('129_scalar_G_C15_W','129_scalar_plus_M_view0','127_scalar_G_C15_W','127_scalar_plus_M_view0')
RECORD_BYTES=114*4+6*8

def digest(path):
    return sha256(path.read_bytes()).hexdigest()

def close(actual,expected,label):
    if actual==expected:
        return
    if not (math.isfinite(actual) and math.isfinite(expected) and math.isclose(actual,expected,rel_tol=2e-11,abs_tol=0)):
        raise AssertionError(f'{label}: {actual!r} != {expected!r}')

def source_review(plan,pilot):
    if pilot:
        declared={
            'rare_event/src/smc_guided.cpp':plan['source_sha256'],
            'rare_event/src/soft_guide_pilot_baseline.hpp':plan['guide_sha256'],
        }
    else:
        declared=plan['source_hashes']
    checks=[]
    for relative,expected in declared.items():
        actual=digest(ROOT/relative)
        checks.append({'path':relative,'expected':expected,'actual':actual,'matches':actual==expected})
    assert all(x['matches'] for x in checks),'Frozen source hash mismatch'
    supplementary=['data/counts.txt','src/common.hpp','src/sampler.hpp','src/matrix.hpp','src/hist.hpp']
    supplementary += [f'results/train_{model}.bin' for model in MODELS]
    return {
        'registered_hashes_all_match':True,'registered_hashes_checked':checks,
        'current_additional_dependency_hashes':{name:digest(ROOT/name) for name in supplementary},
        'additional_hash_scope':'Recorded at audit time; these additional hashes were not declared in the production plan and are not represented as prelaunch attestations.',
    }

def ensure_checker():
    source=HERE/'review_saved_states.cpp'
    binary=HERE/'review_saved_states'
    dependencies=[source]+[ROOT/f'src/{name}.hpp' for name in ('scorer','features','sampler','common','hist','matrix')]
    if not binary.exists() or binary.stat().st_mtime<max(x.stat().st_mtime for x in dependencies):
        subprocess.run(['g++','-O3','-std=c++17','-march=native','-Wall','-Wextra','-Wpedantic',str(source),'-o',str(binary)],check=True,capture_output=True,text=True)
    return binary

def inspect_states(binary,path,model,cache):
    source_names=['rare_event/review/review_saved_states.cpp']+[f'src/{name}.hpp' for name in ('scorer','features','sampler','common','hist','matrix')]
    fingerprints={'states':digest(path),'training':digest(ROOT/f'results/train_{model}.bin'),'counts':digest(ROOT/'data/counts.txt')}
    fingerprints.update({name:digest(ROOT/name) for name in source_names})
    if cache.exists():
        cached=json.loads(cache.read_text())
        if cached.get('review_fingerprints')==fingerprints:
            return cached
    process=subprocess.run([str(binary),str(ROOT),model,str(path)],check=True,capture_output=True,text=True)
    result=json.loads(process.stdout)
    assert result['status']=='passed'
    result['review_fingerprints']=fingerprints
    cache.write_text(json.dumps(result,indent=2)+'\n')
    return result

def weighted_event(log_z,logs,population):
    if not logs:
        return 0.0
    top=max(logs)
    log_mean=top+math.log(math.fsum(math.exp(x-top) for x in logs))-math.log(population)
    return math.exp(log_z+log_mean)

def summarize(values,gate,expected,eligible=True):
    count=len(values)
    if not count:
        return {'replicates':0,'status':'pending'}
    assert all(math.isfinite(x) and x>=0 for x in values)
    maximum=max(values)
    scaled=[x/maximum for x in values] if maximum else [0.0]*count
    total=math.fsum(scaled)
    scaled_mean=total/count
    mean=maximum*scaled_mean
    scaled_se=math.sqrt(math.fsum((x-scaled_mean)**2 for x in scaled)/(count-1)/count) if count>1 else None
    se=maximum*scaled_se if scaled_se is not None else None
    rse=scaled_se/scaled_mean if scaled_mean>0 and scaled_se is not None else None
    share=1/total if total else None
    nonzero=sum(x>0 for x in values)
    checks={
        'all_planned_replicates_complete':count==expected,
        'minimum_nonzero_replicates':nonzero>=gate['minimum_nonzero_replicates'],
        'relative_standard_error':rse is not None and rse<=gate['maximum_empirical_relative_standard_error'],
        'largest_replicate_share':share is not None and share<=gate['maximum_single_replicate_share'],
    }
    passes=eligible and all(checks.values())
    return {
        'replicates':count,'planned_replicates':expected,'nonzero_replicates':nonzero,
        'arithmetic_mean_including_zeros':mean,'between_replicate_standard_error':se,
        'empirical_relative_standard_error':rse,'maximum_single_replicate_share':share,
        'replicate_estimates':values,'reciprocal_of_mean':1/mean if mean>0 else None,
        'gate_checks':checks,'registered_production_gate_eligible':eligible,
        'passes_precision_gate':passes,
        'status':'precision_gate_passed' if passes else ('incomplete' if count!=expected else 'exploratory'),
        'uncertainty_scope':'Between-replicate empirical variation; no binomial particle interval and no guarantee of rare-mode coverage.',
    }

def review_rep(record,state,plan,master,rep,schedule,pilot):
    population=plan['population']
    assert record['replicate']==rep and record['seed']==worker_seed(master,rep)
    assert record['population']==population and record['moves_per_level']==plan['moves_per_level']
    assert record['initialization']=='iid' and record['is_production_estimator'] is True
    assert state['states']==population and state['training_n']==200000
    assert record['target']==state['targets'][1]
    assert len(record['levels'])==len(schedule)
    previous_logz=0.0
    for level,expected in zip(record['levels'],schedule):
        assert [level['lambda'],level['beta']]==expected
        assert level['log_z']<=previous_logz+1e-10
        previous_logz=level['log_z']
        assert 1-1e-8<=level['ess_before_resampling']<=population+1e-8
        assert 1/population-1e-12<=level['max_weight']<=1+1e-12
        moves=level['moves']
        assert moves['attempted']==population*plan['moves_per_level']
        assert 0<=moves['numeric']==moves['evaluated']<=moves['changed']<=moves['null_ok']<=moves['attempted']
        assert 0<=moves['accepted']<=moves['changed']
        assert all(0<=moves[key]<=moves['accepted'] for key in ('score_changed','program_changed','natural_changed'))
        assert 1<=level['founders']<=population
        assert 1/population-1e-12<=level['largest_founder_fraction']<=1+1e-12
    last=record['levels'][-1]
    assert record['log_z']==last['log_z']
    assert last['lambda']>=16.609647687893986
    targets=state['targets']
    beta=last['beta']
    policy_logs=[[] for _ in POLICIES]
    natural_logs=[[] for _ in NATURAL]
    primary_values=[]
    for particle in state['records']:
        scores=particle['scores'];primary_values.append(scores[1])
        inverse_loggamma=beta*max(0.0,targets[1]-scores[1])
        for k in range(6):
            if scores[k]>=targets[k]:
                policy_logs[k].append(inverse_loggamma)
        for bit in range(4):
            if particle['natural_flags']>>bit&1:
                natural_logs[bit].append(inverse_loggamma)
    reconstructed_policies=[weighted_event(record['log_z'],logs,population) for logs in policy_logs]
    reconstructed_natural=[weighted_event(record['log_z'],logs,population) for logs in natural_logs]
    assert record['target_hits']==last['target_hits']==state['policy_hit_counts'][1]
    close(record['estimate'],reconstructed_policies[1],'Primary estimate')
    for k,value in enumerate(reconstructed_policies):
        close(record['policy_estimates'][k],value,f'Policy{k} inverse-gamma estimate')
    assert record['natural_flag_counts']==state['natural_flag_counts']
    if 'natural_event_estimates' in record:
        for bit,value in enumerate(reconstructed_natural):
            close(record['natural_event_estimates'][bit],value,f'Natural{bit} inverse-gamma estimate')
    else:
        assert pilot,'Production is missing natural inverse-gamma estimates'
    close(last['minimum_score'],min(primary_values),'Final minimum score')
    close(last['maximum_score'],max(primary_values),'Final maximum score')
    close(last['mean_score'],math.fsum(primary_values)/population,'Final mean score')
    assert last['distinct_permutations']==state['distinct_permutations']
    assert last['distinct_programs']==state['distinct_primary_programs']
    return {
        'replicate':rep,'seed':record['seed'],'status':'passed',
        'states_checked':population,'scores_compared_bit_exactly':state['score_values_compared_bit_exactly'],
        'null_support_verified':True,'target_hits':record['target_hits'],
        'estimate':record['estimate'],'policy_estimates':record['policy_estimates'],
        'natural_event_estimates':reconstructed_natural,
        'natural_estimate_source':'Reconstructed from original-score verification and final inverse-gamma weights; compared with reported values where present.',
        'natural_flag_counts':record['natural_flag_counts'],
        'largest_selected_inverse_gamma_logweight_by_policy':[max(x) if x else None for x in policy_logs],
        'largest_selected_inverse_gamma_logweight_by_natural_event':[max(x) if x else None for x in natural_logs],
        'final_founders':last['founders'],'largest_founder_fraction':last['largest_founder_fraction'],
        'final_distinct_permutations':state['distinct_permutations'],
        'final_distinct_primary_programs':state['distinct_primary_programs'],
        'minimum_ESS_before_resampling':min(x['ess_before_resampling'] for x in record['levels']),
        'maximum_single_particle_weight_before_resampling':max(x['max_weight'] for x in record['levels']),
        'inversion129_range':state['inversion129_range'],'inversion127_range':state['inversion127_range'],
        'log_z':record['log_z'],'seconds':record['seconds'],
    }

def main():
    parser=argparse.ArgumentParser()
    parser.add_argument('--pilot',action='store_true')
    parser.add_argument('--output',type=Path)
    parser.add_argument('--skip-seeds',action='store_true')
    args=parser.parse_args()
    production_plan=json.loads((ROOT/'rare_event/production/plan.json').read_text())
    plan_path=ROOT/('rare_event/pilot/guided_plan.json' if args.pilot else 'rare_event/production/plan.json')
    plan=json.loads(plan_path.read_text())
    source=source_review(plan,args.pilot)
    schedule=plan['schedule'] if args.pilot else [list(map(float,line.split())) for line in (ROOT/plan['schedule_file']).read_text().splitlines() if line.strip()]
    masters=plan['seeds'] if args.pilot else plan['master_seeds']
    expected=plan['replicates_per_model']
    gate=production_plan['reporting_gate']
    checker=ensure_checker()
    report={'reviewed_utc':datetime.now(timezone.utc).isoformat(),'scope':'guided_pilot_reviewer_validation' if args.pilot else 'fixed_production_review',
            'plan_file':str(plan_path.relative_to(ROOT)),'plan_sha256':digest(plan_path),'source_review':source,'reporting_gate':gate,'models':{}}
    report['review_implementation_hashes']={name:digest(HERE/name) for name in ('production_review.py','review_saved_states.cpp','review_seed_streams.py')}
    all_complete=True
    for model in MODELS:
        prefix=('rare_event/pilot/guided_'+('inversion' if model=='inversion_exact' else model)) if args.pilot else 'rare_event/production/'+model
        reviewed=[];pending=[]
        for rep in range(expected):
            path=ROOT/f'{prefix}_rep{rep}.json'
            states=ROOT/f'{prefix}_states{rep}.bin'
            try:
                record=json.loads(path.read_text())
            except (FileNotFoundError,json.JSONDecodeError):
                pending.append({'replicate':rep,'reason':'replicate JSON not complete'});continue
            if not states.exists() or states.stat().st_size!=plan['population']*RECORD_BYTES:
                pending.append({'replicate':rep,'reason':'saved state binary not complete'});continue
            cache=HERE/(('pilot' if args.pilot else 'production')+f'_{model}_rep{rep}_state_review.json')
            state=inspect_states(checker,states,model,cache)
            result=review_rep(record,state,plan,masters[model],rep,schedule,args.pilot)
            result.update({'replicate_json_sha256':digest(path),'state_binary_sha256':digest(states),'state_review_file':str(cache.relative_to(ROOT))})
            reviewed.append(result)
        policies={name:summarize([x['policy_estimates'][k] for x in reviewed],gate,expected,not args.pilot) for k,name in enumerate(POLICIES)}
        natural={name:summarize([x['natural_event_estimates'][k] for x in reviewed],gate,expected,not args.pilot) for k,name in enumerate(NATURAL)}
        aggregate_path=ROOT/f'{prefix}_run.json'
        aggregate_verified=False
        if len(reviewed)==expected:
            try:
                aggregate=json.loads(aggregate_path.read_text())
            except (FileNotFoundError,json.JSONDecodeError):
                aggregate=None
            if aggregate is not None:
                assert aggregate['reps']==expected and aggregate['population']==plan['population']
                assert aggregate['master_seed']==masters[model]
                assert aggregate['estimates']==[x['estimate'] for x in reviewed]
                close(aggregate['arithmetic_mean_estimate'],policies['primary_mixture']['arithmetic_mean_including_zeros'],'Published aggregate mean')
                aggregate_verified=True
        complete=len(reviewed)==expected and aggregate_verified
        all_complete&=complete
        report['models'][model]={'status':'complete_verified' if complete else 'incomplete','completed_replicates':len(reviewed),'planned_replicates':expected,
            'published_aggregate_verified':aggregate_verified,'pending':pending,'replicates':reviewed,'policies':policies,'natural_events':natural,
            'secondary_policy_caution':'The primary-score guide need not cover secondary-event regions well. Each policy receives its own replicate diagnostics; inverse-gamma validity does not ensure small variance.'}
    report['status']='passed_all_completed_outputs' if all_complete else 'incomplete_production'
    report['no_binomial_particle_intervals']=True
    if not args.skip_seeds:
        seeds=seed_audit(ROOT)
        (HERE/'production_seed_review.json').write_text(json.dumps(seeds,indent=2)+'\n')
        report['seed_review']={k:seeds[k] for k in ('status','all_start_states_distinct','all_documented_seed_derivations_reproduced','minimum_circular_counter_gap','closest_pair','bounds','interpretation')}
    # Verify that the declared sources were unchanged during the review itself.
    assert source_review(plan,args.pilot)['registered_hashes_checked']==source['registered_hashes_checked']
    output=args.output or HERE/('pilot_production_reviewer_validation.json' if args.pilot else 'production_review.json')
    output.write_text(json.dumps(report,indent=2,allow_nan=False)+'\n')
    compact={'status':report['status'],'scope':report['scope'],'models':{}}
    for model,entry in report['models'].items():
        summary=entry['policies']['primary_mixture']
        compact['models'][model]={k:summary.get(k) for k in ('replicates','nonzero_replicates','arithmetic_mean_including_zeros','between_replicate_standard_error','empirical_relative_standard_error','maximum_single_replicate_share','passes_precision_gate')}
    print(json.dumps(compact,indent=2))

if __name__=='__main__':
    main()
