#!/usr/bin/env python3
"""Exhaustive independent tests of F anchoring, ordered-H SIS and completion.

For each small labeled problem, enumerate every target permutation and every
proposal trace. Verify support, H! multiplicity, exact probability-weight
identity, adaptive row-selection validity, and dead-end mass accounting.
"""
from collections import Counter, defaultdict
from fractions import Fraction
import itertools
import json
import math
from pathlib import Path
import time

HERE=Path(__file__).resolve().parent

def validate(values,p,f_target,h_target):
    n=len(values);rows=tuple(range(1,n+1));labels=tuple(range(n));factorial=math.factorial(n)
    def is_f(row,label):return (row+values[label])%p==0
    def is_h(row,label):return (row-values[label])%p==0
    def color(residue):return min(residue,p-residue)
    def rowcolor(row):return color(row%p)
    def colcolor(label):return color(values[label]%p)
    F_edges=[(row,label)for row in rows for label in labels if is_f(row,label)and not is_h(row,label)]
    matchings=[]
    for edges in itertools.combinations(F_edges,f_target):
        if len({x[0]for x in edges})==len({x[1]for x in edges})==f_target:matchings.append(edges)
    MF=len(matchings);assert MF
    targets=set()
    for perm in itertools.permutations(labels):
        F=[row for row,label in zip(rows,perm)if is_f(row,label)]
        H=[row for row,label in zip(rows,perm)if is_h(row,label)]
        if len(F)==f_target and len(H)==h_target and not(set(F)&set(H))and(sum(H)-sum(F))%p==0:targets.add(perm)
    probability_mass=defaultdict(Fraction);trace_counts=Counter();proposal_mass=Fraction();dead_mass=Fraction();second_moment=Fraction()
    dead_h=0;dead_completion=0;terminal_traces=0;hall_forced_steps=0
    def complete(assignment,q,path_product):
        nonlocal proposal_mass,dead_mass,second_moment,dead_completion,terminal_traces,hall_forced_steps
        remaining_rows=[x for x in rows if x not in assignment]
        remaining_labels=[x for x in labels if x not in assignment.values()]
        if not remaining_rows:
            perm=tuple(assignment[r]for r in rows)
            assert perm in targets
            w=Fraction(MF*path_product,factorial*math.factorial(h_target))
            probability_mass[perm]+=q*w;trace_counts[perm]+=1;proposal_mass+=q;second_moment+=q*w*w;terminal_traces+=1
            return
        R=Counter(map(rowcolor,remaining_rows));C=Counter(map(colcolor,remaining_labels));left=len(remaining_rows)
        if any(R[g]+C[g]>left for g in R.keys()|C.keys()):
            dead_mass+=q;dead_completion+=1;return
        group=min(R,key=lambda g:(-(R[g]+C[g]),g))
        row=max(r for r in remaining_rows if rowcolor(r)==group)
        choices=[label for label in remaining_labels if colcolor(label)!=group]
        assert choices
        if R[group]+C[group]==left:hall_forced_steps+=1
        for label in choices:
            assert not is_f(row,label)and not is_h(row,label)
            following=dict(assignment);following[row]=label
            # Every chosen completion edge preserves the Hall condition.
            R2=R.copy();C2=C.copy();R2[group]-=1;C2[colcolor(label)]-=1
            assert all(R2[g]+C2[g]<=left-1 for g in R2.keys()|C2.keys())
            complete(following,q/len(choices),path_product*len(choices))
    def select_h(assignment,selected,f_sum,h_sum,q,path_product):
        nonlocal dead_mass,dead_h
        if selected==h_target:
            assert(h_sum-f_sum)%p==0
            complete(assignment,q,path_product);return
        choices=[]
        for row in rows:
            if row in assignment:continue
            for label in labels:
                if label in assignment.values()or not is_h(row,label)or is_f(row,label):continue
                if selected+1==h_target and(h_sum+row-f_sum)%p:continue
                choices.append((row,label))
        if not choices:
            dead_mass+=q;dead_h+=1;return
        for row,label in choices:
            following=dict(assignment);following[row]=label
            select_h(following,selected+1,f_sum,h_sum+row,q/len(choices),path_product*len(choices))
    for matching in matchings:
        select_h(dict(matching),0,sum(x[0]for x in matching),0,Fraction(1,MF),1)
    assert set(probability_mass)==targets
    assert all(x==Fraction(1,factorial)for x in probability_mass.values())
    assert all(x==math.factorial(h_target)for x in trace_counts.values())
    assert proposal_mass+dead_mass==1
    expectation=sum(probability_mass.values(),Fraction())
    assert expectation==Fraction(len(targets),factorial)
    return {'values':values,'p':p,'Fcount':f_target,'Hcount':h_target,'F_matchings':MF,
            'target_permutations':len(targets),'total_permutations':factorial,
            'target_probability':float(expectation),'exact_target_fraction':str(expectation),
            'all_target_orders_have_exact_H_factorial_traces':True,
            'per_target_order_weighted_mass':str(Fraction(1,factorial)),
            'terminal_traces':terminal_traces,'proposal_success_mass':str(proposal_mass),'dead_end_mass':str(dead_mass),
            'dead_H_prefixes':dead_h,'initially_infeasible_completion_prefixes':dead_completion,
            'Hall_forced_steps_exercised':hall_forced_steps,
            'importance_second_moment':float(second_moment),'importance_variance':float(second_moment-expectation*expectation)}

start=time.perf_counter()
cases=[([1,1,2,2,4,5],3,2,2),([1,2,3,4,6,7,8],5,2,2),([1,2,3,4,5,6,7,8],5,3,2)]
results=[validate(*case)for case in cases]
out={'status':'all_exact_checks_pass','method':'Full enumeration of labeled permutations and all SIS random-choice traces, exact Fraction arithmetic','cases':results,'seconds':time.perf_counter()-start}
(HERE/'sis_toy_validation.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps(out,indent=2))
