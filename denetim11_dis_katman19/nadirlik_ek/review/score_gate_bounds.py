#!/usr/bin/env python3
"""Safe, deliberately relaxed upper bounds for proposed exact C event gates.

No score, trained probability, or inference definition is changed. A bound
below the observed threshold certifies exclusion; a bound above it proves
nothing about realizability. Module dependence is relaxed except for exact
modular M feasibility when G balance is true, parity, and assignment bounds.
"""
import functools
import json
import math
import pathlib
import sys
import itertools

import numpy as np
from scipy.optimize import linear_sum_assignment

ROOT = pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "review"))
from observed_decomposition import (Training, PRIMES, policy_weights,
                                     partitions, FORM_COST, BELL)

AL=(1,1,1,1,1,1,2,2,2,2,3,3,3,3)
BE=(1,-1,2,-2,3,-3,1,-1,3,-3,1,-1,2,-2)
BASE=json.loads((ROOT/'data/verse_counts.json').read_text())
NEG=float('-inf')

BAL={};FREE={}
for line in (pathlib.Path(__file__).with_name('matrix_balance_masks.txt')).read_text().splitlines():
    v,f,pi,*views=map(int,line.split());BAL[v,f,pi]=views
for line in (pathlib.Path(__file__).with_name('matrix_free_masks.txt')).read_text().splitlines():
    v,f,pi,*views=map(int,line.split());FREE[v,f,pi]=views

def extrema(value, allowed):
    ans=[]
    for direction in (1,-1):
        cost=np.where(allowed,direction*value,1e12)
        r,c=linear_sum_assignment(cost)
        if not np.all(allowed[r,c]):return None
        ans.append(int(value[r,c].sum()))
    return tuple(ans)

@functools.lru_cache(None)
def constraints(model,v,f,pi):
    a=np.array(BASE['counts_127' if v else 'counts_129'])
    n=np.arange(1,115)
    p=PRIMES[pi];alpha,beta=AL[f],BE[f]
    allowed=np.ones((114,114),bool)
    if model=='block6': allowed=(np.arange(114)[:,None]//6==np.arange(114)[None,:]//6)
    t=alpha*n[:,None]+beta*a[None,:]
    u=alpha*n[:,None]-beta*a[None,:]
    F=t%p==0;H=u%p==0
    cnt=extrema(F.astype(int),allowed)
    weighted=extrema(np.where(F,t,0),allowed)
    centre=weighted[0]<=4*p*p<=weighted[1]
    equal=beta%2!=0 and any(cnt[0]<=k<=cnt[1] for k in range(4,115,4))
    pattern=beta%2!=0
    unions=extrema((F|H).astype(int),allowed&~(F&H))
    w0=unions is not None and unions[0]<=p<=unions[1]
    # Assignment bounds on G balance, respecting the parity class.
    e=(alpha*n[:,None]+beta*a[None,:])%2==0
    gbal=extrema(np.where(e,n[:,None]+a[None,:],0),allowed)
    possible_balance=gbal[0]<=int(a.sum())<=gbal[1]
    if alpha%2==0: gm=[0,2] if possible_balance else [0]
    elif beta%2==0: gm=[1,3] if possible_balance else [1]
    else:gm=[0,1,2,3] if possible_balance else [0,1]
    cm=[q for q in range(16) if (not(q&1) or equal) and (not(q&2) or pattern) and (not(q&8) or centre)]
    wm=[q for q in range(4) if not(q&1) or w0]
    return gm,cm,wm,dict(F_count_bounds=cnt,F_weighted_bounds=weighted,union_count_bounds=unions,G_balance_sum_bounds=gbal)

@functools.lru_cache(None)
def centre_pattern_band_possible(p):
    # Generic integer certificate: <=p quotient entries, complete integer
    # support, a constant cell with >=2 entries, even/odd row parity, and
    # cell sums (p-1,p+1,p,p). No positivity of individual q is assumed.
    for k in range(5,p+1):
        for width in range(1,k):
            triangular=width*(width-1)//2
            lower=math.ceil((4*p-k*(width-1)+triangular)/k)
            upper=(4*p-triangular)//k
            for lo in range(lower,upper+1):
                support=set(range(lo,lo+width))
                choices=[]
                for cell,target in enumerate((p-1,p+1,p,p)):
                    values=sorted(x for x in support if x%2==cell//2)
                    entries=[]
                    for count in range(1,k-2):
                        for qs in itertools.combinations_with_replacement(values,count):
                            if sum(qs)==target:entries.append(qs)
                    choices.append(entries)
                def visit(cell,used,seen,constant):
                    if cell==4:return used==k and seen==support and constant
                    for qs in choices[cell]:
                        if used+len(qs)+3-cell>k:continue
                        if visit(cell+1,used+len(qs),seen|set(qs),constant or(len(qs)>=2 and len(set(qs))==1)):return True
                    return False
                if visit(0,0,set(),False):return True
    return False

def cw_compatible(c,w,p,logic):
    if not w&1:return True
    lo,hi=logic['F_count_bounds'];hi=min(hi,p)
    if c&1:
        ks=range(1,29)
        if c&4:ks=[k for k in ks if k>=2]
        if c&2:ks=[k for k in ks if k%2]
        if not any(lo<=4*k<=hi for k in ks):return False
    elif c&14==14 and hi<5:return False
    if c&14==14 and p<=7 and not centre_pattern_band_possible(p):return False
    return True

def run(model,scenario):
    tr=Training(ROOT/'results'/f'train_{model}.bin')
    weights=policy_weights(1);logq=list(map(math.log2,weights))
    global_best=(0,None)
    by_active={q:NEG for q in range(1,16)}
    for v in range(2):
        block_best=[(NEG,None) for _ in range(16)]
        for f in range(14):
            for pi,p in enumerate(PRIMES):
                gm,cm,wm,logic=constraints(model,v,f,pi)
                cm=list(cm)
                if scenario=='no_C':cm=[0]
                elif scenario=='no_C15':cm=[q for q in cm if q!=15]
                elif scenario=='no_natural_C15' and f==0 and p==19:cm=[q for q in cm if q!=15]
                elif scenario=='no_natural_C' and f==0 and p==19:cm=[0]
                elif scenario=='no_Cpattern':cm=[q for q in cm if not q&2]
                elif scenario=='no_Cband':cm=[q for q in cm if not q&4]
                elif scenario=='no_Ccentre':cm=[q for q in cm if not q&8]
                elif scenario=='no_Cequal':cm=[q for q in cm if not q&1]
                cu,carg=max((tr.utility(v,1,f,pi,0,q),q) for q in cm)
                wu,warg=max((tr.utility(v,2,f,pi,0,q),q) for q in wm)
                cw=max((tr.utility(v,1,f,pi,0,c)+tr.utility(v,2,f,pi,0,w),c,w)
                       for c in cm for w in wm if cw_compatible(c,w,p,logic))
                for g in gm:
                    gu=tr.utility(v,0,f,None,0,g)
                    opts=[]
                    for view in range(8):
                        possible=(BAL if g&2 else FREE)[v,f,pi][view]
                        masks=[q for q in range(16) if possible>>q&1]
                        opts.extend((tr.utility(v,3,f,pi,view,q),q,view) for q in masks)
                    mu,marg,view=max(opts)
                    us=[gu,cu,wu,mu]
                    for block in range(1,16):
                        local_us=us.copy();local_c=carg;local_w=warg
                        if block&6==6:
                            _,local_c,local_w=cw
                            local_us[1]=tr.utility(v,1,f,pi,0,local_c)
                            local_us[2]=tr.utility(v,2,f,pi,0,local_w)
                        val=sum(local_us[j] for j in range(4) if block>>j&1)-FORM_COST
                        if block!=1:val+=logq[pi]
                        if val>block_best[block][0]:
                            block_best[block]=(val,dict(modules=block,form=f,prime=p,G=g,C=local_c,W=local_w,M=marg,view=view,utilities=[u if math.isfinite(u) else None for u in local_us],logical_bounds=logic))
        for active in range(1,16):
            penalty=5+math.log2(BELL[active.bit_count()])
            for part in partitions(active):
                score=sum(block_best[b][0] for b in part)-penalty
                by_active[active]=max(by_active[active],score)
                if score>global_best[0]:global_best=(score,dict(variant=127 if v else 129,active=active,partition=part,blocks=[block_best[b][1] for b in part]))
    return dict(upper_bound=global_best[0],by_active_upper_bound={str(k):(v if math.isfinite(v) else None) for k,v in by_active.items()},maximizing_relaxed_configuration=global_best[1],warning='Upper bound only; maximizing modules/residues need not be jointly realizable.')

if __name__=='__main__':
    out={}
    for model in ('uniform','block6','inversion_exact'):
        out[model]={}
        for scenario in ('unrestricted','no_C','no_C15','no_natural_C15','no_natural_C'):
            out[model][scenario]=run(model,scenario)
            print(model,scenario,out[model][scenario]['upper_bound'],flush=True)
    pathlib.Path(__file__).with_suffix('.json').write_text(json.dumps(out,indent=2,allow_nan=False)+'\n')
