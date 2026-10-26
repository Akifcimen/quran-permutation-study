#!/usr/bin/env python3
"""Independent exact labeled block6 enumeration of six W0(p7) anchors."""
from collections import Counter
from decimal import Decimal, localcontext
from hashlib import sha256
from itertools import permutations
import json
from pathlib import Path

HERE=Path(__file__).resolve().parent
ROOT=HERE.parent.parent

def anchor(values,variant,ratio):
    distributions=[]
    overlap_rejected=[]
    for block in range(19):
        start=6*block
        frequencies=Counter()
        rejected=0
        for identities in permutations(range(start,start+6)):
            union=overlap=0
            for offset,identity in enumerate(identities):
                position=start+offset+1
                value=127 if variant==127 and identity==8 else values[identity]
                plus=(value-ratio*position)%7==0
                minus=(value+ratio*position)%7==0
                union+=plus or minus
                overlap+=plus and minus
            if overlap:
                rejected+=1
            else:
                frequencies[union]+=1
        assert sum(frequencies.values())+rejected==720
        distributions.append([frequencies[k] for k in range(7)])
        overlap_rejected.append(rejected)
    distribution=[1]
    for frequencies in distributions:
        nxt=[0]*(len(distribution)+6)
        for n,count in enumerate(distribution):
            for k,frequency in enumerate(frequencies):
                nxt[n+k]+=count*frequency
        distribution=nxt
    # Independent evaluation of all integer DP coefficients at z=1.
    product=1
    for frequencies in distributions:
        product*=sum(frequencies)
    assert sum(distribution)==product
    count=distribution[7]
    with localcontext() as ctx:
        ctx.prec=70
        probability=str(Decimal(count)/Decimal(720**19))
    return {
        'variant':variant,'ratio':ratio,
        'definition':'No item lies in both a=+r*n (mod7) and a=-r*n (mod7), and their union has exactly7 items.',
        'labeled_permutations_enumerated':19*720,
        'block_zero_overlap_union_count_histograms':distributions,
        'block_overlap_rejections':overlap_rejected,
        'total_zero_overlap_count':str(product),
        'anchor_count':str(count),
        'block6_sample_space_size':str(720**19),
        'probability':probability,
        'complete_union_count_coefficients':[str(x) for x in distribution],
    }

def main():
    data=ROOT/'data/counts.txt'
    values=list(map(int,data.read_text().split()))
    assert len(values)==114 and values[8]==129
    results=[anchor(values,v,r) for v in (129,127) for r in (1,2,3)]
    result={
        'status':'passed','method':'Exhaustive labeled block enumeration and exact integer coefficient convolution; independent Python implementation.',
        'counts_sha256':sha256(data.read_bytes()).hexdigest(),
        'script_sha256':sha256(Path(__file__).read_bytes()).hexdigest(),
        'anchors':results,
    }
    path=HERE/'independent_counts.json'
    path.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({'status':'passed','anchors':[{'variant':x['variant'],'ratio':x['ratio'],'count':x['anchor_count'],'probability':x['probability']} for x in results]},indent=2))

if __name__=='__main__':
    main()
