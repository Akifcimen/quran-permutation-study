"""Independent exact transition audit of the mutation construction.

Run after compiling/running test_kernels.cpp for the production-kernel checks.
This audit enumerates a six-label tied multiset and uses exact rational
probabilities for the state-dependent paired-repair proposal.
"""
from collections import Counter, defaultdict, deque
from fractions import Fraction
from itertools import combinations, permutations, product
from pathlib import Path
import json
import hashlib

import numpy as np

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT/"rare_event/tests"
VALUES = (10,10,7,4,3,1)
EDITED = (10,5,7,4,3,1)
STATES = list(permutations(range(6)))
INDEX = {x:i for i,x in enumerate(STATES)}
PAIRS = list(combinations(range(6),2))
PARITY_PAIRS = [(a,b) for a,b in PAIRS if (a-b)%2==0]


def swap(x,pair):
    y=list(x);a,b=pair;y[a],y[b]=y[b],y[a]
    return tuple(y)


def inversions(x,values):
    return sum(values[x[a]]<values[x[b]] for a,b in PAIRS)


def filter_q(matrix, selected, denominator):
    """Reject every exit as a self-transition; keep exact integer weights."""
    q=matrix[np.ix_(selected,selected)].copy()
    for i in range(len(selected)):
        q[i,i]+=denominator-int(q[i].sum())
    assert np.all(q==q.T)
    assert np.all(q.sum(axis=1)==denominator)
    return q


def components(q):
    unseen=set(range(len(q)));sizes=[]
    while unseen:
        component={unseen.pop()};todo=list(component)
        while todo:
            a=todo.pop()
            neighbors={int(j) for j in np.flatnonzero(q[a])}&unseen
            unseen-=neighbors;component|=neighbors;todo.extend(neighbors)
        sizes.append(len(component))
    return sorted(sizes,reverse=True)


def run():
    pairs=np.array([[INDEX[swap(x,p)] for p in PAIRS] for x in STATES])
    counts=[(inversions(x,VALUES),inversions(x,EDITED)) for x in STATES]
    support_counts=Counter(counts)
    target,max_size=max(support_counts.items(),key=lambda item:(item[1],item[0]))
    selected=[i for i,c in enumerate(counts) if c==target]
    pos={v:i for i,v in enumerate(selected)}
    # A further deterministic level set, independent of transition construction.
    event=[i for i in selected if STATES[i].index(0)%2==0]
    assert len(event)>1 and len(event)<len(selected)
    symmetry_checks={}
    full=np.zeros((720,720),dtype=np.int64)
    adjacent=np.zeros_like(full)
    parity=np.zeros_like(full)
    residue=np.zeros_like(full)
    cycles=np.zeros_like(full)
    for i,x in enumerate(STATES):
        for j in pairs[i]: full[i,j]+=1
        for edge in range(5): adjacent[i,INDEX[swap(x,(edge,edge+1))]]+=1
        for a,b in PARITY_PAIRS:
            y=swap(x,(a,b)) if (VALUES[x[a]]-VALUES[x[b]])%2==0 and (EDITED[x[a]]-EDITED[x[b]])%2==0 else x
            parity[i,INDEX[y]]+=1
            y=swap(x,(a,b)) if (VALUES[x[a]]-VALUES[x[b]])%6==0 and (EDITED[x[a]]-EDITED[x[b]])%6==0 else x
            residue[i,INDEX[y]]+=1
        for edge in range(4):
            for reverse in [False,True]:
                order=[(edge,edge+1),(edge+1,edge+2)]
                if reverse:order.reverse()
                y=x
                for p in order:y=swap(y,p)
                cycles[i,INDEX[y]]+=1
    for name,matrix,denominator in [
        ("all_transpositions",full,15),("adjacent_transpositions",adjacent,5),
        ("parity_filtered_transpositions",parity,len(PARITY_PAIRS)),
        ("residue_filtered_transpositions",residue,len(PARITY_PAIRS)),
        ("adjacent_inverse_paired_three_cycles",cycles,8),
    ]:
        assert np.all(matrix==matrix.T)
        assert np.all(matrix.sum(axis=1)==denominator)
        symmetry_checks[name]={"states":720,"integer_row_mass":denominator,"symmetric":True}
    full2=full@full
    full4=full2@full2
    constrained={}
    for name,matrix,denominator in [
        ("global_two",full2,15**2),("global_four",full4,15**4),
        ("adjacent_two",adjacent@adjacent,5**2),
        ("parity_two",parity@parity,len(PARITY_PAIRS)**2),
        ("residue_two",residue@residue,len(PARITY_PAIRS)**2),
        ("adjacent_three_cycle",cycles,8),
    ]:
        q=filter_q(matrix,selected,denominator)
        qe=filter_q(matrix,event,denominator)
        constrained[name]={"fiber_states":len(selected),"fiber_components":components(q),
                           "event_states":len(event),"event_components":components(qe),
                           "row_mass":denominator,"exact_detailed_balance":True}

    # Repair choices depend only on the off-fiber intermediate z and the fixed
    # target fiber. A uniform first pair and uniform non-undo restoring pair
    # therefore have exactly the same reverse-path probability.
    restoring={z:[u for u in range(15) if int(pairs[z,u]) in pos] for z in range(720)}
    q=[[Fraction(0) for _ in selected] for _ in selected]
    choices=[]
    for row,x in enumerate(selected):
        for u in range(15):
            z=int(pairs[x,u])
            candidates=restoring[z]
            assert u in candidates
            if len(candidates)>1:candidates=[v for v in candidates if v!=u]
            choices.append(len(candidates))
            for v in candidates:
                y=int(pairs[z,v]);col=pos[y]
                q[row][col]+=Fraction(1,15*len(candidates))
    assert all(sum(row)==1 for row in q)
    assert all(q[i][j]==q[j][i] for i in range(len(q)) for j in range(len(q)))
    epos=[pos[i] for i in event]
    qe=[[q[i][j] for j in epos] for i in epos]
    for i in range(len(qe)):qe[i][i]+=1-sum(qe[i])
    assert all(sum(row)==1 for row in qe)
    assert all(qe[i][j]==qe[j][i] for i in range(len(qe)) for j in range(len(qe)))
    # Positive independent refresh gives every pair a positive entry. It is
    # uniform on the base fiber; level exits are still rejected as self moves.
    refresh=Fraction(1,100)
    qrefresh=[[((1-refresh)*q[i][j]+refresh/Fraction(len(q))) for j in range(len(q))] for i in range(len(q))]
    assert all(sum(row)==1 for row in qrefresh)
    assert all(v>0 for row in qrefresh for v in row)
    assert all(qrefresh[i][j]==qrefresh[j][i] for i in range(len(q)) for j in range(len(q)))
    result={
        "status":"passed",
        "toy_values":VALUES,"edited_values":EDITED,
        "labeled_states":720,"target_inversions":target,
        "fiber_states":len(selected),"additional_event_states":len(event),
        "primitive_symmetry":symmetry_checks,"constrained_kernels":constrained,
        "paired_repair":{"exact_rational_detailed_balance":True,
                         "row_mass_exactly_one":True,
                         "additional_level_restriction_validated":True,
                         "min_second_choices":min(choices),"max_second_choices":max(choices),
                         "fiber_components":components(np.array([[v>0 for v in row] for row in q],dtype=np.int64)),
                         "with_refresh_all_transitions_positive":True},
        "production_header_sha256":hashlib.sha256((ROOT/"rare_event/src/kernels.hpp").read_bytes()).hexdigest(),
        "important_limit":"Stationarity and full support do not establish useful mixing in the actual rare-event tail. Report numeric motion and independent-replicate convergence separately."
    }
    (OUT/"kernel_exact_validation.json").write_text(json.dumps(result,indent=2)+"\n")
    print(json.dumps(result,indent=2))


if __name__=="__main__":
    run()

