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
classes=((0,6),(0,8),(1,6),(2,5),(2,7),(2,9),(3,5),(3,7))
targets=((2,1,3,2,0,1,1,2),(2,1,3,1,2,0,1,2))
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
    records.append({'target':target,'C_count':str(current[target]),'CW_count':str(weighted[target][7,0]),'forward_states_by_block':states})
c_total=sum(int(x['C_count'])for x in records)
cw_total=sum(int(x['CW_count'])for x in records)
c_saved=json.loads((HERE/'block6_C_exact.json').read_text())
cw_saved=json.loads((HERE/'block6_CW_exact.json').read_text())
assert str(c_total)==c_saved['count']
assert str(cw_total)==cw_saved['count']
assert [x['CW_count']for x in records]==cw_saved['target_counts']
result={'status':'exact_agreement','method':'Independent forward polynomial convolution, independently enumerated all720 labeled permutations of each block','C_count':str(c_total),'CW_count':str(cw_total),'records':records,'seconds':time.perf_counter()-start,'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest()}
(HERE/'forward_count_validation.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps({k:v for k,v in result.items()if k!='records'},indent=2))
