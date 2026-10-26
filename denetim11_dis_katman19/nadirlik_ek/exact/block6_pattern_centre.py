#!/usr/bin/env python3
"""Independent exact superset: C has pattern and centre bits (mask10).

This calculation is diagnostic. The frozen score can assign the same fitted
C utility to different masks when training counts are all zero. Thus it is
incorrect to assume that crossing its tail requires all four C predicates.
"""
import itertools
import json
import struct
from collections import Counter
from fractions import Fraction
from pathlib import Path
import block6_core as core_module
import block6_cw as cw_module

OUT = Path(__file__).resolve().parent
base = list(map(int, (core_module.ROOT / "data" / "counts.txt").read_text().split()))
capacities = Counter()
for bi in range(19):
    maximum = Counter()
    for ids in itertools.permutations(range(6*bi,6*bi+6)):
        counts = Counter()
        for off,j in enumerate(ids):
            n,a=6*bi+off+1,base[j]
            if (n+a)%19==0:
                counts[(((n+a)&1)*2+(n&1),(n+a)//19)] += 1
        for key,count in counts.items():
            maximum[key] = max(maximum[key],count)
    capacities.update(maximum)
options=[]
for cell in range(4):
    vals=sorted(q for c,q in capacities if c==cell)
    here=[]
    for multiplicities in itertools.product(*(range(capacities[cell,q]+1)for q in vals)):
        total=sum(q*m for q,m in zip(vals,multiplicities))
        if total in (18,19,20):
            here.append(tuple(q for q,m in zip(vals,multiplicities)for _ in range(m)))
    options.append(here)
patterns=[]
for cells in itertools.product(*options):
    sums=list(map(sum,cells))
    if sum(sums)==76 and any(sums[a]==sums[b] and sorted((sums[c],sums[d]))==[sums[a]-1,sums[a]+1]for a,b,c,d in ((0,1,2,3),(2,3,0,1))):
        patterns.append(cells)
assert len(patterns)==18,patterns
classes=tuple(sorted(set((c,q)for pat in patterns for c,cell in enumerate(pat)for q in cell)))
targets=tuple(tuple(Counter((c,q)for c,cell in enumerate(pat)for q in cell)[key]for key in classes)for pat in patterns)
core_module.CLASSES=classes
core_module.TARGETS=targets
core_module.TARGET_MAX=tuple(max(t[j]for t in targets)for j in range(len(classes)))
core_module.ZERO=(0,)*len(classes)
cw_module.TARGETS=targets
core=core_module.ConditionalCore(base)
cw=cw_module.ConditionalCW(core)
denominator=720**19
entries=[]
for target,pattern,root in zip(targets,patterns,cw.roots):
    ccount=core.completions(0,target)
    h=19-sum(target)
    cwcount=cw.count(root,h,0)
    ids=core.unrank(sum(core.completions(0,t)for t in targets[:targets.index(target)])) if ccount else None
    sizes=list(map(len,pattern));values=set(itertools.chain.from_iterable(pattern))
    mask=10+int(len(set(sizes))==1)+4*int(max(values)-min(values)+1==len(values)and any(len(c)>=2 and len(set(c))==1 for c in pattern))
    if ids is not None:assert mask==core_module.feature_c(ids,base)
    assert mask&10==10
    entries.append({"cell_coefficients":pattern,"Cmask":mask,"family_count":sum(target),
                    "C_count":str(ccount),"C_probability":float(Fraction(ccount,denominator)),
                    "CW_count":str(cwcount),"CW_probability":float(Fraction(cwcount,denominator)),
                    "example_ids":ids})
totalc=sum(int(x["C_count"])for x in entries)
totalcw=sum(int(x["CW_count"])for x in entries)
result={"event":"Natural C has pattern-and-centre bits, (Cmask&10)==10, block6",
        "frozen_statistic_unchanged":True,"is_full_score_tail_probability":False,
        "proof":"Enumerate every per-cell coefficient multiset within exact per-block assignment capacities. Eighteen pattern-and-centre allocations remain in this envelope; DP enforces all assignment compatibilities.",
        "capacities":[{"cell":c,"q":q,"capacity":m}for(c,q),m in sorted(capacities.items())],
        "patterns":entries,"total_C_count":str(totalc),"total_CW_count":str(totalcw),
        "sample_space":str(denominator),"total_C_probability":float(Fraction(totalc,denominator)),
        "total_CW_probability":float(Fraction(totalcw,denominator)),
        "positive_C_states":len(cw.nodes),"CW_cache":cw.count.cache_info()._asdict()}
(OUT/'block6_C10_exact.json').write_text(json.dumps(result,indent=2)+'\n')

# Version02 carries each disjoint root's H target and complete C mask.
def u32(fh,*values):fh.write(struct.pack('<'+'I'*len(values),*values))
with (OUT/'block6_C10W_sampler.bin').open('wb')as fh:
    fh.write(b'BLK6CW02')
    u32(fh,len(cw.nodes),cw.terminal,len(cw.roots))
    for root,target,entry in zip(cw.roots,targets,entries):
        u32(fh,root,19-sum(target),entry['Cmask'])
    fh.write(totalcw.to_bytes(32,'little'))
    for profiles in core.local_profiles:
        u32(fh,len(profiles))
        for p in profiles:
            u32(fh,p['ec'],p['es_half_129'],p['en_mod19'],p['fn_mod19'],p['hcount'],p['h_minus_f_mod19'],p['overlap'],p['edit_in_E'],*p['ids'])
    for bi,edges in cw.nodes:
        u32(fh,bi,len(edges))
        for child,h,d,indexes in edges:u32(fh,child,h,d,len(indexes),*indexes)
    for node in range(cw.terminal+1):
        for h in range(8):
            for residue in range(19):fh.write(cw.count(node,h,residue).to_bytes(32,'little'))
print(json.dumps({k:v for k,v in result.items()if k not in('patterns','capacities')},indent=2))
for x in entries:print(x['Cmask'],x['cell_coefficients'],x['C_probability'],x['CW_probability'])
