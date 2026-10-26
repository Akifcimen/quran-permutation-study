"""Compare the C++ extractor against direct Python set/list operations."""
from pathlib import Path
import json
import random
import subprocess
import sys
import tempfile
from matrix_reference import eval8

ROOT=Path(__file__).resolve().parents[1]
BASE=json.loads((ROOT/'data/verse_counts.json').read_text())['counts_129']

def reference(ids,variant,alpha,beta,p,do_matrix=False):
    a=[127 if variant==127 and j==8 else BASE[j] for j in ids]
    n=list(range(1,115))
    t=[alpha*i+beta*j for i,j in zip(n,a)]
    u=[alpha*i-beta*j for i,j in zip(n,a)]
    E={i for i in range(114) if t[i]%2==0}
    F={i for i in range(114) if t[i]%p==0}
    H={i for i in range(114) if u[i]%p==0}
    g=(len(E)==57)|((sum(n[i] for i in E)==sum(a[i] for i in range(114) if i not in E))<<1)
    cells=[[t[i]//p for i in F if t[i]%2==tr and n[i]%2==nr] for tr in range(2) for nr in range(2)]
    sizes=list(map(len,cells));sums=list(map(sum,cells));q=[t[i]//p for i in F]
    equal=sizes[0]>0 and len(set(sizes))==1
    pattern=any(sums[r]==sums[r+1] and sorted([sums[s],sums[s+1]])==[sums[r]-1,sums[r]+1] for r,s in [(0,2),(2,0)])
    band=bool(q) and set(q)==set(range(min(q),max(q)+1)) and any(len(c)>=2 and len(set(c))==1 for c in cells)
    centre=sum(q)==4*p
    cm=equal|(pattern<<1)|(band<<2)|(centre<<3)
    w=(not(F&H) and len(F|H)==p)|((sum(a[i] for i in F|H)%p==0)<<1)
    result={'G':g,'C':cm,'W':w}
    if do_matrix:
        def table(K):
            sn=sum(n[i] for i in K);sa=sum(a[i] for i in K)
            return (sn,sa,sum(n)-sn,sum(a)-sa)
        result['matrix_packed']=eval8(table(E),table(F),p)
    return result

def main():
    rng=random.Random(861420)
    samples=[list(range(114))]
    for _ in range(3):
        ids=list(range(114));rng.shuffle(ids);samples.append(ids)
    checked=matrix_checked=0
    with tempfile.TemporaryDirectory(dir=ROOT/'logs') as folder:
        path=Path(folder)
        for j,ids in enumerate(samples):
            ipath=path/f'ids{j}.txt';ipath.write_text(' '.join(map(str,ids)))
            prefix=path/f'features{j}'
            subprocess.run([str(ROOT/'study_engine'),'--phase','observed','--input',str(ipath),'--out',str(prefix)],cwd=ROOT,check=True)
            data=json.loads(Path(str(prefix)+'_features.json').read_text())
            for variant in data['variants']:
                for candidate in variant['candidates']:
                    alpha,beta,p=(candidate[x] for x in ('alpha','beta','p'))
                    matrix=(p in [3,19,113] and (alpha,beta) in [(1,1),(1,-3),(2,3)])
                    expected=reference(ids,variant['variant'],alpha,beta,p,matrix)
                    for key,value in expected.items():
                        assert candidate[key]==value,(j,variant['variant'],alpha,beta,p,key,candidate[key],value)
                    checked+=1;matrix_checked+=matrix
    result={'permutations':len(samples),'scalar_candidates_checked':checked,'matrix_candidate_sets_checked':matrix_checked,'status':'pass'}
    (ROOT/'review/feature_validation.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result))

if __name__=='__main__':main()
