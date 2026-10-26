#!/usr/bin/env python3
"""Certified necessary union of positive C conjunctions for block6 tail.

This is a relaxed-score covering certificate, not an event probability.
Partitions and all other bindings remain free in the upper-bound calculation.
"""
import functools
import json
import math
import pathlib

import score_gate_bounds as b

THRESHOLD=22.69418342742572
tr=b.Training(b.ROOT/'results/train_block6.bin')
weights=b.policy_weights(1);logq=list(map(math.log2,weights))
result=[];per_variant=[]
for v in range(2):
    best=[b.NEG]*16
    configs=[]
    for f in range(14):
        for pi,p in enumerate(b.PRIMES):
            gm,cm,wm,logic=b.constraints('block6',v,f,pi)
            cs={c:tr.utility(v,1,f,pi,0,c) for c in cm}
            ws={w:tr.utility(v,2,f,pi,0,w) for w in wm}
            for g in gm:
                gu=tr.utility(v,0,f,None,0,g)
                opts=[]
                for view in range(8):
                    possible=(b.BAL if g&2 else b.FREE)[v,f,pi][view]
                    opts.extend(tr.utility(v,3,f,pi,view,q) for q in range(16) if possible>>q&1)
                mu=max(opts)
                config=dict(f=f,pi=pi,g=g,gu=gu,mu=mu,cs=cs,ws=ws,logic=logic)
                configs.append(config)
                for block in range(1,16):
                    value=(gu if block&1 else 0)+(mu if block&8 else 0)-b.FORM_COST
                    if block!=1:value+=logq[pi]
                    if block&6==6:
                        value+=max(cu+wu for c,cu in cs.items() for w,wu in ws.items() if b.cw_compatible(c,w,p,logic))
                    else:
                        if block&2:value+=max(cs.values())
                        if block&4:value+=max(ws.values())
                    best[block]=max(best[block],value)
    dp=[b.NEG]*16;dp[0]=0
    for active in range(1,16):
        dp[active]=max(sum(best[q] for q in part) for part in b.partitions(active))
    allowances={}
    for block in range(1,16):
        if not block&2:continue
        remaining=15^block;s=remaining;choices=[]
        while True:
            choices.append((dp[s]-5-math.log2(b.BELL[(s|block).bit_count()]),s))
            if s==0:break
            s=(s-1)&remaining
        allowances[block]=max(choices)
    maximum=max(dp[a]-5-math.log2(b.BELL[a.bit_count()]) for a in range(1,16))
    per_variant.append(dict(variant=127 if v else 129,relaxed_score_upper_bound=maximum))
    anchor_bounds={}
    for z in configs:
        f,pi,g=z['f'],z['pi'],z['g'];p=b.PRIMES[pi]
        for c,cu in z['cs'].items():
            if c==0:continue
            key=(f,pi,c)
            for block,(allowance,other) in allowances.items():
                value=cu-b.FORM_COST+logq[pi]+allowance
                if block&1:value+=z['gu']
                if block&8:value+=z['mu']
                if block&4:value+=max(wu for w,wu in z['ws'].items() if b.cw_compatible(c,w,p,z['logic']))
                if key not in anchor_bounds or value>anchor_bounds[key][0]:anchor_bounds[key]=(value,block,other,g)
    for f in range(14):
        for pi,p in enumerate(b.PRIMES):
            eligible=[c for c in range(1,16) if (f,pi,c) in anchor_bounds and anchor_bounds[f,pi,c][0]>=THRESHOLD-1e-10]
            minimal=[c for c in eligible if not any(q!=c and q&c==q for q in eligible)]
            if not minimal:continue
            result.append(dict(variant=127 if v else 129,form_index=f,alpha=b.AL[f],beta=b.BE[f],prime=p,
                               positive_C_masks=minimal,relaxed_details=[dict(mask=c,upper_bound=anchor_bounds[f,pi,c][0],anchored_block=anchor_bounds[f,pi,c][1],other_active_modules=anchor_bounds[f,pi,c][2],G_mask_at_anchor=anchor_bounds[f,pi,c][3],training_count=tr.stats(v,1,f,pi,0,c)['training_successes']) for c in minimal]))
out=dict(model='block6',target=THRESHOLD,scope='Every frozen primary exceedance lies in the union of these positive C conjunctions; not sufficient, and no probability is claimed.',
         further_necessary_conditions='For the same global variant, some G balance bit and some W disjoint-union-cardinality bit must also hold; their forms/primes need not match the C anchor.',
         per_variant=per_variant,tuples=len(result),conjunction_components=sum(len(x['positive_C_masks']) for x in result),cover=result)
pathlib.Path(__file__).with_suffix('.json').write_text(json.dumps(out,indent=2)+'\n')
print('tuple count',out['tuples'],'conjunction components',out['conjunction_components'])
for r in result:print(r['variant'],r['form_index'],r['prime'],r['positive_C_masks'])
