#!/usr/bin/env python3
"""Exact intersections and union of six labeled block7 anchors.

Independent implementation using 18-block count DPs plus direct enumeration
of the one block affected by the count-convention edit.
"""
from collections import Counter
from decimal import Decimal, localcontext
from hashlib import sha256
from itertools import combinations, permutations
import json
from math import comb
from pathlib import Path

HERE=Path(__file__).resolve().parent
ROOT=HERE.parent.parent
ANCHORS=[(variant,ratio) for variant in (129,127) for ratio in (1,2,3)]

def main():
    data=ROOT/'data/counts.txt'
    values=list(map(int,data.read_text().split()))
    assert len(values)==114 and values[8]==129
    assert sum(x%7==0 for x in values)==14
    assert sum(n%7==0 for n in range(1,115))==16
    distributions=[]
    checked=0
    for block in range(19):
        start=6*block
        histogram=Counter()
        expected_three_union=6-sum(n%7==0 for n in range(start+1,start+7))-sum(x%7==0 for x in values[start:start+6])
        for ids in permutations(range(start,start+6)):
            overlap=sum((start+j+1)%7==0 and values[identity]%7==0 for j,identity in enumerate(ids))
            if overlap:
                continue
            counts=[]
            for variant,ratio in ANCHORS:
                count=0
                for j,identity in enumerate(ids):
                    n=start+j+1
                    a=127 if variant==127 and identity==8 else values[identity]
                    count+=((a-ratio*n)%7==0 or (a+ratio*n)%7==0)
                counts.append(count)
            assert sum(counts[:3])==sum(counts[3:])==expected_three_union
            if block!=1:
                assert counts[:3]==counts[3:]
            histogram[tuple(counts)]+=1
            checked+=1
        distributions.append(histogram)
    # Each ratio or pair has at most8^2 truncated count states. Counts from
    # the edited block are supplied separately below.
    outside={}
    for width in (1,2):
        for ratios in combinations((1,2,3),width):
            dp={(0,)*width:1}
            for block,histogram in enumerate(distributions):
                if block==1:
                    continue
                collapsed=Counter()
                for counts,frequency in histogram.items():
                    collapsed[tuple(counts[r-1] for r in ratios)]+=frequency
                nxt=Counter()
                for previous,frequency in dp.items():
                    for additions,multiplicity in collapsed.items():
                        total=tuple(x+y for x,y in zip(previous,additions))
                        if max(total)<=7:
                            nxt[total]+=frequency*multiplicity
                dp=nxt
            outside[ratios]=dp
    intersections={}
    for mask in range(1,64):
        indices=[i for i in range(6) if mask>>i&1]
        ratios=tuple(sorted({ANCHORS[i][1] for i in indices}))
        if len(ratios)==3:
            # One selected variant count<=7 at each ratio implies that the
            #129 convention counts are each<=8 (a single item is edited),
            #contradicting their exact total84 on zero-overlap support.
            intersections[mask]=0
            continue
        result=0
        for counts,frequency in distributions[1].items():
            residual={}
            consistent=True
            for i in indices:
                ratio=ANCHORS[i][1]
                target=7-counts[i]
                if target<0 or (ratio in residual and residual[ratio]!=target):
                    consistent=False
                    break
                residual[ratio]=target
            if consistent:
                result+=frequency*outside[ratios].get(tuple(residual[r] for r in ratios),0)
        intersections[mask]=result
    singles=json.loads((HERE/'independent_counts.json').read_text())['anchors']
    for i,entry in enumerate(singles):
        assert (entry['variant'],entry['ratio'])==ANCHORS[i]
        assert intersections[1<<i]==int(entry['anchor_count'])
    for mask,count in intersections.items():
        for i in range(6):
            if mask>>i&1 and mask!=(1<<i):
                assert count<=intersections[mask^(1<<i)]
    total=720**19
    union=sum((1 if mask.bit_count()%2 else -1)*count for mask,count in intersections.items())
    sums=[total]+[sum(count for mask,count in intersections.items() if mask.bit_count()==k) for k in range(1,7)]
    exact_memberships=[0]*7
    for k in range(6,-1,-1):
        exact_memberships[k]=sums[k]-sum(comb(j,k)*exact_memberships[j] for j in range(k+1,7))
        assert exact_memberships[k]>=0
    assert sum(exact_memberships)==total
    assert sum(exact_memberships[1:])==union
    assert sum(k*count for k,count in enumerate(exact_memberships))==sums[1]
    assert max(intersections[1<<i] for i in range(6))<=union<=sums[1]
    with localcontext() as ctx:
        ctx.prec=70
        result={
            'status':'passed',
            'method':'Independent integer DP on18 unaffected blocks, direct edited-block enumeration, and exact inclusion-exclusion.',
            'counts_sha256':sha256(data.read_bytes()).hexdigest(),
            'script_sha256':sha256(Path(__file__).read_bytes()).hexdigest(),
            'anchor_order':[{'variant':v,'ratio':r} for v,r in ANCHORS],
            'zero_overlap_block_assignments_checked':checked,
            'sample_space_size':str(total),
            'union_count':str(union),
            'union_probability':str(Decimal(union)/Decimal(total)),
            'sum_anchor_counts':str(sums[1]),
            'sum_anchor_probabilities':str(Decimal(sums[1])/Decimal(total)),
            'size_weighted_mixture_accept_1_over_m_acceptance':str(Decimal(union)/Decimal(sums[1])),
            'exact_membership_counts':[str(x) for x in exact_memberships],
            'intersections':[{'mask':mask,'cardinality':mask.bit_count(),'count':str(count),'probability':str(Decimal(count)/Decimal(total))} for mask,count in intersections.items()],
            'validation':{'singles_match_prior_independent_enumeration':True,'intersection_monotonicity':True,'all_three_ratio_intersections_zero':True,'exact_multiplicity_inversion_nonnegative':True,'mass_and_first_moment_verified':True},
        }
    (HERE/'independent_intersections.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:result[k] for k in ('status','union_count','union_probability','sum_anchor_probabilities','size_weighted_mixture_accept_1_over_m_acceptance','exact_membership_counts')},indent=2))

if __name__=='__main__':
    main()
