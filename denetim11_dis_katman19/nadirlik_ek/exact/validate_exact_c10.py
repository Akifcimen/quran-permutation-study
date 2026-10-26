#!/usr/bin/env python3
"""Independent forward polynomial count, without importing sampler/DP code."""
from collections import Counter, defaultdict
import hashlib
import itertools
import json
from pathlib import Path
import time
import numpy as np

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[1]
base=list(map(int,(ROOT/'data/counts.txt').read_text().split()))
even_rows=(((6,6,8),(6,6,6)),((6,6,8),(4,4,4,6)),
           ((6,6,6),(4,4,6,6)),((4,6,8),(4,4,6,6)),
           ((4,4,6,6),(6,6,6)),((4,4,6,6),(4,4,4,6)))
odd_first=((5,7,7),(5,5,9),(3,7,9))
patterns=[(a,b,c,(5,7,7))for a,b in even_rows for c in odd_first]
classes=tuple(sorted(set((c,q)for pattern in patterns for c,cell in enumerate(pattern)for q in cell)))
targets=tuple(tuple(Counter((c,q)for c,cell in enumerate(pattern)for q in cell)[key]for key in classes)for pattern in patterns)
start=time.perf_counter()
cgroups=[];wgroups=[]
for bi in range(19):
    cgroup=Counter();wgroup=Counter()
    for assignment in itertools.permutations(range(6*bi,6*bi+6)):
        F=[];H=[];cell_counts=Counter()
        for offset,label in enumerate(assignment):
            position=6*bi+offset+1;value=base[label]
            if (position+value)%19==0:
                F.append(position)
                cell_counts[(((position+value)%2)*2+position%2,(position+value)//19)]+=1
            if (position-value)%19==0:H.append(position)
        if set(cell_counts)-set(classes):continue
        sig=tuple(cell_counts[c]for c in classes)
        cgroup[sig]+=1
        if not(set(F)&set(H)):
            wgroup[(sig,len(H),(sum(H)-sum(F))%19)]+=1
    cgroups.append(cgroup);wgroups.append(wgroup)

records=[]
for target in targets:
    # Necessary per-coordinate remaining capacity prunes impossible prefixes;
    # it does not rely on the backwards memoization being validated.
    caps=[[0]*len(classes)for _ in range(20)]
    for bi in range(18,-1,-1):
        for j in range(len(classes)):
            caps[bi][j]=caps[bi+1][j]+max(s[j]for s in cgroups[bi])
    current={tuple([0]*len(classes)):1}
    zero=np.zeros((8,19),dtype=object);zero[0,0]=1
    weighted={tuple([0]*len(classes)):zero}
    states=[]
    for bi in range(19):
        next_c=defaultdict(int);next_w={}
        for prefix,count in current.items():
            for sig,multiplicity in cgroups[bi].items():
                dest=tuple(x+y for x,y in zip(prefix,sig))
                if all(dest[j]<=target[j]<=dest[j]+caps[bi+1][j]for j in range(len(target))):
                    next_c[dest]+=count*multiplicity
        for prefix,array in weighted.items():
            for (sig,h,residue),multiplicity in wgroups[bi].items():
                if h>7:continue
                dest=tuple(x+y for x,y in zip(prefix,sig))
                if not all(dest[j]<=target[j]<=dest[j]+caps[bi+1][j]for j in range(len(target))):continue
                if dest not in next_w:next_w[dest]=np.zeros((8,19),dtype=object)
                next_w[dest][h:,:]+=multiplicity*np.roll(array[:8-h,:],residue,axis=1)
        current,weighted=next_c,next_w
        states.append([len(current),len(weighted)])
    htarget=19-sum(target)
    records.append({'target':target,'C_count':str(current[target]),'CW_count':str(weighted[target][htarget,0]),'forward_states_by_block':states})
c_total=sum(int(x['C_count'])for x in records)
cw_total=sum(int(x['CW_count'])for x in records)
saved=json.loads((HERE/'block6_C10_exact.json').read_text())
assert str(c_total)==saved['total_C_count']
assert str(cw_total)==saved['total_CW_count']
assert [x['CW_count']for x in records]==[x['CW_count']for x in saved['patterns']]
result={'status':'exact_agreement','method':'Independent forward polynomial convolution for all18 Cpattern-centre allocations, independently enumerated all720 labeled permutations of each block','C_count':str(c_total),'CW_count':str(cw_total),'records':records,'seconds':time.perf_counter()-start,'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest()}
(HERE/'forward_C10_count_validation.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps({k:v for k,v in result.items()if k!='records'},indent=2))
