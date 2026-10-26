#!/usr/bin/env python3
"""Audit documented SplitMix64 starts and recorded/conservative stream windows.

This does not pretend that unrecorded production RNG call counts are known.
Conditional bounds for rejection loops are reported as such.
"""
import json
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
MOD=1<<64
MASK=MOD-1
GAMMA=0x9E3779B97F4A7C15
INV_GAMMA=pow(GAMMA,-1,MOD)
WORKER_MIX=0xD1B54A32D192ED03
MAX_BOUNDED_TRIALS=8
MAX_SUBSET_TRIALS=1024

def mix(seed):
    z=(seed+GAMMA)&MASK
    z=((z^(z>>30))*0xBF58476D1CE4E5B9)&MASK
    z=((z^(z>>27))*0x94D049BB133111EB)&MASK
    return z^(z>>31)

def worker_seed(master,index):
    return mix(master^((WORKER_MIX*(index+1))&MASK))

def read_complete(path):
    try:
        return json.loads(path.read_text())
    except (FileNotFoundError,json.JSONDecodeError):
        return None

def make_stream(name,seed,calls=None,kind='unrecorded',bounded_invocations=0,subset_selections=0):
    return {'name':name,'seed':seed,'counter_start':(seed*INV_GAMMA)&MASK,
            'call_window_upper_bound':calls,'bound_kind':kind,
            'bounded_invocation_cap':bounded_invocations,'subset_selection_cap':subset_selections}

def sampler_window(model,proposals):
    if proposals is None:
        return None,0
    # At most113 bounded calls in a full shuffle;95 in nineteen block
    # shuffles; at most226 rank RNG words plus37 tie-label calls per full
    # Mallows proposal. Treat all263 words as bounded calls conservatively.
    per_proposal={'uniform':113,'block6':95,'inversion_exact':263}[model]
    invocations=per_proposal*proposals
    return MAX_BOUNDED_TRIALS*invocations,invocations

def smc_streams(label,model,master,rep,population,moves,levels,record):
    repseed=worker_seed(master,rep)
    if record is not None:
        assert record['seed']==repseed
        assert record['replicate']==rep
    kernel_seed=mix(repseed^0xc7f2a0a17e987bd9)
    initial=mix(repseed^0x6638432a1e527e09)
    local=mix(kernel_seed^0x848b50ccb25c0e7d)
    refresh=mix(kernel_seed^0x53e9f55e0d02b4cb)
    resample=mix(repseed^0x3b4c508724f1ab63)
    steps=population*moves*levels
    initial_proposals=record.get('initial_sampler_proposals') if record else None
    refresh_proposals=sum(x['moves']['refresh_attempts'] for x in record['levels']) if record else None
    initial_bound,initial_invocations=sampler_window(model,initial_proposals)
    refresh_bound,refresh_invocations=sampler_window(model,refresh_proposals)
    # Paired repair has at most6440 restoring alternatives; all other moves
    # fit below7000 bounded invocations when each selected-position retry
    # loop uses at most1024 draws. This is a conditional upper bound.
    local_invocations=7000*steps
    prefix=f'{label}/{model}/rep{rep}'
    return [
        make_stream(prefix+'/initial',initial,initial_bound,'conditional_rejection_cap' if initial_bound is not None else 'unrecorded_pending',initial_invocations),
        make_stream(prefix+'/kernel',local,MAX_BOUNDED_TRIALS*local_invocations,'conditional_rejection_and_subset_caps',local_invocations,6*steps),
        make_stream(prefix+'/refresh',refresh,refresh_bound,'conditional_rejection_cap' if refresh_bound is not None else 'unrecorded_pending',refresh_invocations),
        make_stream(prefix+'/resample_MH',resample,population*levels+steps,'deterministic_upper_bound'),
    ]

def audit(root=ROOT):
    streams=[]
    protocol=json.loads((root/'protocol.json').read_text())
    original_names=[]
    for phase,seeds in [('train',protocol['training_seeds']),('test',protocol['testing_seeds'])]:
        for model,master in zip(protocol['models'],seeds):
            record=json.loads((root/f'results/{phase}_{model}_run.json').read_text())
            expected=[worker_seed(master,i) for i in range(record['threads'])]
            assert record['worker_seeds']==expected
            for worker,seed in enumerate(expected):
                if model=='inversion_exact':
                    # Total proposals for all workers is a conservative cap
                    # for any one worker; individual totals were not logged.
                    proposals=record['proposals']
                else:
                    proposals=record['n']*(worker+1)//record['threads']-record['n']*worker//record['threads']
                bound,invocations=sampler_window(model,proposals)
                name=f'original/{phase}/{model}/worker{worker}'
                original_names.append(name)
                streams.append(make_stream(name,seed,bound,'conditional_rejection_cap',invocations))
    assert len(original_names)==48

    exact_files=[
        'rare_event/exact/C15_pilot_rng_replay.json',
        'rare_event/exact/C15_production_rng_replay.json',
        'rare_event/exact/block6_C10W_validation_pilot.json',
        'rare_event/exact/block6_C10W_production.json',
    ]
    exact_names=[]
    for relative in exact_files:
        record=json.loads((root/relative).read_text())
        for worker,entry in enumerate(record['workers']):
            seed=worker_seed(record['seed'],worker)
            assert entry['seed']==seed
            assert (seed+GAMMA*entry['rng_calls'])&MASK==entry['final_rng_state']
            name=f'conditional/{Path(relative).stem}/worker{worker}'
            exact_names.append(name)
            streams.append(make_stream(name,seed,entry['rng_calls'],'recorded_exact_calls'))
    assert len(exact_names)==32

    # Include earlier SMC pilots as well, although the requested comparison
    # only requires the original48 and the conditional32 streams.
    for pilot in ('score','guided'):
        for model,short in [('uniform','uniform'),('block6','block6'),('inversion_exact','inversion')]:
            run=read_complete(root/f'rare_event/pilot/{pilot}_{short}_run.json')
            if run is None:
                continue
            for rep in range(run['reps']):
                record=read_complete(root/f'rare_event/pilot/{pilot}_{short}_rep{rep}.json')
                if record is None:
                    continue
                streams+=smc_streams('pilot_'+pilot,model,run['master_seed'],rep,run['population'],run['moves_per_level'],len(record['levels']),record)

    production=json.loads((root/'rare_event/production/plan.json').read_text())
    levels=len((root/production['schedule_file']).read_text().splitlines())
    production_names=[]
    for model,master in production['master_seeds'].items():
        for rep in range(production['replicates_per_model']):
            record=read_complete(root/f'rare_event/production/{model}_rep{rep}.json')
            added=smc_streams('production',model,master,rep,production['population'],production['moves_per_level'],levels,record)
            production_names.extend(x['name'] for x in added)
            streams+=added

    assert len({x['seed'] for x in streams})==len(streams),'Duplicate RNG start state'
    ordered=sorted(streams,key=lambda x:x['counter_start'])
    for i,entry in enumerate(ordered):
        following=ordered[(i+1)%len(ordered)]
        gap=(following['counter_start']-entry['counter_start'])&MASK
        entry['forward_counter_gap_to_next_start']=gap
        entry['next_start_name']=following['name']
        bound=entry['call_window_upper_bound']
        entry['window_below_next_start']=None if bound is None else bound<gap
        entry['window_margin']=None if bound is None else gap-bound
    pending=[x['name'] for x in ordered if x['call_window_upper_bound'] is None]
    failed=[x['name'] for x in ordered if x['window_below_next_start'] is False]
    total_bounded=sum(x['bounded_invocation_cap'] for x in streams)
    total_subsets=sum(x['subset_selection_cap'] for x in streams)
    # Under ideal independent uniform words, bounded(r) has rejection
    # probability <(r-1)/2^32, with r<=6440 in these paths. A selected-position
    # retry has rejection probability at most5/6. These are NOT claims of
    # mathematical independence for fixed finite PRNG streams.
    bounded_failure=total_bounded*(6440/(2**32))**MAX_BOUNDED_TRIALS
    subset_failure=total_subsets*(5/6)**MAX_SUBSET_TRIALS
    nearest=min(ordered,key=lambda x:x['forward_counter_gap_to_next_start'])
    return {
        'status':'failed' if failed else ('pending_call_bounds' if pending else 'conditional_windows_separated'),
        'original_streams':len(original_names),'recorded_conditional_streams':len(exact_names),
        'production_streams':len(production_names),'all_streams_including_SMC_pilots':len(streams),
        'all_start_states_distinct':True,
        'all_documented_seed_derivations_reproduced':True,
        'all_32_recorded_conditional_final_states_match_calls':True,
        'minimum_circular_counter_gap':nearest['forward_counter_gap_to_next_start'],
        'closest_pair':[nearest['name'],nearest['next_start_name']],
        'windows_exceeding_gap':failed,'windows_waiting_for_completed_proposal_totals':pending,
        'bounds':{
            'bounded_trials_cap':MAX_BOUNDED_TRIALS,'selected_position_trials_cap':MAX_SUBSET_TRIALS,
            'maximum_local_bounded_invocations_per_move_under_cap':7000,
            'total_bounded_invocations_in_completed_or_fixed_budgets':total_bounded,
            'ideal_uniform_word_union_bound_for_bounded_cap_failure':bounded_failure,
            'ideal_uniform_word_union_bound_for_subset_cap_failure':subset_failure,
        },
        'interpretation':'Start-state separation and recorded conditional call windows are exact arithmetic checks. Production call counts are unrecorded; the other windows use explicit conservative caps on rejection loops, not fabricated exact call counts. Bounds relying on uniform independent words do not prove independence of fixed PRNG streams.',
        'streams':ordered,
    }

if __name__=='__main__':
    result=audit()
    path=ROOT/'rare_event/review/production_seed_review.json'
    path.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:result[k] for k in ('status','all_streams_including_SMC_pilots','minimum_circular_counter_gap','closest_pair','bounds')},indent=2))
