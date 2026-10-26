#!/usr/bin/env python3
"""Independent support checks for the uniform natural-event SIS proposal."""
from collections import Counter
from hashlib import sha256
from itertools import combinations_with_replacement,product
import json
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
HERE=Path(__file__).resolve().parent

def compositions(total,groups):
    if groups==1:
        yield (total,)
    else:
        for first in range(total+1):
            for rest in compositions(total-first,groups-1):
                yield (first,)+rest

def main():
    original=list(map(int,(ROOT/'data/counts.txt').read_text().split()))
    base=original.copy();base[8]=127
    maxq=(114+max(base))//19
    patterns=set()
    count_by_cell_size={}
    # W forces4k<=19. C's row sums and parity force the even-q row to be
    #18/20 and the odd-q row19/19. Enumerate all possible positive bags;
    #do not assume in advance which cell is constant or that k must be3.
    for k in range(1,5):
        even=list(combinations_with_replacement(range(2,maxq+1,2),k))
        odd=[x for x in combinations_with_replacement(range(1,maxq+1,2),k) if sum(x)==19]
        e18=[x for x in even if sum(x)==18]
        e20=[x for x in even if sum(x)==20]
        added=set()
        for a,b,c,d in product(e18,e20,odd,odd):
            values=set(a+b+c+d)
            if max(values)-min(values)+1!=len(values):
                continue
            if not any(len(cell)>=2 and len(set(cell))==1 for cell in (a,b,c,d)):
                continue
            added.add((a,b,c,d));added.add((b,a,c,d))
        patterns.update(added);count_by_cell_size[k]=len(added)
    recorded=json.loads((ROOT/'rare_event/uniform_anchor/F_matching_count_127.json').read_text())
    forward=json.loads((ROOT/'rare_event/uniform_anchor/F_forward_validation.json').read_text())
    recorded_patterns={tuple(tuple(cell) for cell in p) for p in recorded['patterns']}
    assert len(recorded['patterns'])==len(recorded_patterns)==20
    assert patterns==recorded_patterns
    assert count_by_cell_size=={1:0,2:0,3:20,4:0}
    assert sum(map(int,recorded['F_partial_matching_counts_by_pattern']))==int(recorded['F_partial_matching_total'])
    assert recorded['F_partial_matching_total']==forward['F_matching_total']
    assert recorded['F_partial_matching_counts_by_pattern']==forward['pattern_counts']

    color_counts=Counter(min(a%19,19-a%19) for a in base)
    initial_hall_upper=max((6 if color==0 else 12)+color_counts[color] for color in range(10))
    assert initial_hall_upper==28<95
    feasible_states=transitions=0
    for groups in range(2,5):
        for remaining in range(1,7):
            all_counts=list(compositions(remaining,groups))
            for rows,cols in product(all_counts,repeat=2):
                if any(r+c>remaining for r,c in zip(rows,cols)):
                    continue
                feasible_states+=1
                maximum=max(rows[g]+cols[g] for g in range(groups) if rows[g])
                chosen=next(g for g in range(groups) if rows[g] and rows[g]+cols[g]==maximum)
                eligible=[g for g in range(groups) if g!=chosen and cols[g]]
                assert eligible
                for column in eligible:
                    r=list(rows);c=list(cols)
                    r[chosen]-=1;c[column]-=1
                    assert all(x+y<=remaining-1 for x,y in zip(r,c))
                    transitions+=1
    source_names=['rare_event/uniform_anchor/f_matchings.py','rare_event/uniform_anchor/uniform_sis.hpp',
                  'rare_event/uniform_anchor/uniform_sis_alias.hpp','rare_event/uniform_anchor/F_conditional_sampler_127.bin',
                  'rare_event/uniform_anchor/validate_sis_toys.py']
    result={
        'status':'passed',
        'independently_enumerated_complete_bags':20,
        'bags_by_equal_positive_cell_size':count_by_cell_size,
        'all_20_bags_match_forward_and_sampler_counts':True,
        'F_partial_matching_total':recorded['F_partial_matching_total'],
        'initial_remaining_completion_size':95,
        'universal_initial_Hall_upper_bound_Rg_plus_Cg':initial_hall_upper,
        'exhaustive_small_Hall_feasible_count_states':feasible_states,
        'all_allowed_greedy_transitions_verified_Hall_preserving':transitions,
        'source_hashes_at_audit':{name:sha256((ROOT/name).read_bytes()).hexdigest() for name in source_names},
        'scope':'Independent pattern completeness and Hall-support checks, accompanying the separate trace-weight proof. No new rarity estimate.',
    }
    (HERE/'uniform_sis_independent_support_validation.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,indent=2))

if __name__=='__main__':
    main()
