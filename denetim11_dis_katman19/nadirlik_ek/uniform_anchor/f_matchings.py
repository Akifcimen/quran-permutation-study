#!/usr/bin/env python3
"""Exact partial-F matching counts for the uniform permutation natural anchor.

This does NOT count complete C/W permutations: additional H selection and
the complement completion remain in the importance proposal.
"""
from __future__ import annotations
import argparse
from collections import defaultdict
from functools import lru_cache
import hashlib
import itertools
import json
from pathlib import Path
import struct
import time

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[1]

def patterns():
    even=[x for x in itertools.combinations_with_replacement(range(2,21,2),3)if sum(x)==20]
    odd=[x for x in itertools.combinations_with_replacement(range(1,20,2),3)if sum(x)==19]
    result=[]
    for e,a,b,flip in itertools.product(even,odd,odd,range(2)):
        cells=[e,(6,6,6),a,b]
        if flip:cells[0],cells[1]=cells[1],cells[0]
        values=set(itertools.chain.from_iterable(cells))
        if max(values)-min(values)+1==len(values):result.append(tuple(cells))
    assert len(result)==20
    return result

PATTERNS=patterns()
CLASSES=tuple(sorted(set((c,q)for p in PATTERNS for c,qs in enumerate(p)for q in qs)))
INDEX={x:i for i,x in enumerate(CLASSES)}
HIGH=sum(8<<(4*j)for j in range(len(CLASSES)))
def encode_pattern(p):return sum(1<<(4*INDEX[c,q])for c,qs in enumerate(p)for q in qs)
TARGETS=tuple(map(encode_pattern,PATTERNS))
def fits(a,b):return (((a|HIGH)-b)&HIGH)==HIGH

class FMatchings:
    def __init__(self,base):
        self.base=base;self.groups=[];self.local_stats=[]
        for residue in range(1,19):
            rows=list(range(residue,115,19))
            labels=[i for i,a in enumerate(base)if a%19==(-residue)%19]
            edges=[]
            for n in rows:
                here=[]
                for local,label in enumerate(labels):
                    a=base[label];q=(n+a)//19;key=((q%2)*2+n%2,q)
                    if key in INDEX:here.append((local,label,INDEX[key]))
                edges.append(here)
            by_signature=defaultdict(list);assignment=[255]*6
            def walk(row,used,code):
                if row==6:
                    by_signature[code].append(bytes(assignment));return
                assignment[row]=255;walk(row+1,used,code)
                for local,label,c in edges[row]:
                    if used&(1<<local)or((code>>(4*c))&15)>=3:continue
                    assignment[row]=label;walk(row+1,used|(1<<local),code+(1<<(4*c)))
                assignment[row]=255
            walk(0,0,0)
            groups=tuple((code,tuple(matches))for code,matches in sorted(by_signature.items()))
            self.groups.append(groups)
            self.local_stats.append({'residue':residue,'row_count':len(rows),'column_count':len(labels),
                                     'allowed_F_edges':sum(map(len,edges)),'partial_matchings':sum(len(m)for c,m in groups),
                                     'distinct_signatures':len(groups)})
        self.suffix_capacity=[0]*19
        cap=[0]*len(CLASSES)
        for group in range(17,-1,-1):
            for j in range(len(CLASSES)):
                cap[j]=min(3,cap[j]+max((code>>(4*j))&15 for code,_ in self.groups[group]))
            self.suffix_capacity[group]=sum(x<<(4*j)for j,x in enumerate(cap))

    @lru_cache(None)
    def count(self,group,remaining):
        if remaining==0:return 1
        if group==18 or not fits(self.suffix_capacity[group],remaining):return 0
        total=0
        for code,matches in self.groups[group]:
            if fits(remaining,code):total+=len(matches)*self.count(group+1,remaining-code)
        return total

    def export_locals(self,path):
        with path.open('wb')as fh:
            fh.write(b'UNIFMF01')
            fh.write(struct.pack('<II',len(CLASSES),len(TARGETS)))
            for cell,q in CLASSES:fh.write(struct.pack('<II',cell,q))
            for target in TARGETS:fh.write(struct.pack('<Q',target))
            for r,groups in enumerate(self.groups,1):
                fh.write(struct.pack('<II',r,len(groups)))
                for code,matches in groups:
                    fh.write(struct.pack('<QI',code,len(matches)))
                    for matching in matches:fh.write(matching)

    def export_sampler(self,path,variant):
        nodes=[(18,0,1,())];index={}
        def visit(group,remaining):
            if remaining==0:return 0
            key=(group,remaining)
            if key in index:return index[key]
            ident=len(nodes);index[key]=ident;nodes.append(None)
            choices=[];cumulative=0
            for local,(code,matches)in enumerate(self.groups[group]):
                if not fits(remaining,code):continue
                child_count=self.count(group+1,remaining-code)
                if not child_count:continue
                child=visit(group+1,remaining-code)
                cumulative+=len(matches)*child_count
                choices.append((cumulative,child_count,child,local))
            assert cumulative==self.count(group,remaining)>0
            nodes[ident]=(group,remaining,cumulative,tuple(choices))
            return ident
        roots=[visit(0,t)for t in TARGETS]
        total=sum(self.count(0,t)for t in TARGETS)
        assert total<1<<64
        with path.open('wb')as fh:
            fh.write(b'UNIFCS01');fh.write(struct.pack('<IIQ',variant,len(roots),total))
            fh.write(struct.pack('<'+'I'*len(roots),*roots))
            for residue,groups in enumerate(self.groups,1):
                fh.write(struct.pack('<II',residue,len(groups)))
                for code,matches in groups:
                    fh.write(struct.pack('<QI',code,len(matches)))
                    for matching in matches:fh.write(matching)
            fh.write(struct.pack('<I',len(nodes)))
            for group,remaining,count,choices in nodes:
                fh.write(struct.pack('<IQQI',group,remaining,count,len(choices)))
                for choice in choices:fh.write(struct.pack('<QQII',*choice))
        return {'positive_sampler_states':len(nodes),'cdf_choices':sum(len(n[3])for n in nodes),'sampler_file_bytes':path.stat().st_size}

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--variant',type=int,choices=(129,127),default=127)
    ap.add_argument('--locals-only',action='store_true');ap.add_argument('--export-sampler',action='store_true');args=ap.parse_args()
    base=list(map(int,(ROOT/'data/counts.txt').read_text().split()));base[8]=args.variant
    start=time.perf_counter();f=FMatchings(base)
    f.export_locals(HERE/f'F_local_matchings_{args.variant}.bin')
    result={'phase':'feasibility, no production importance estimates','variant':args.variant,
            'patterns':PATTERNS,'classes':CLASSES,'local_groups':f.local_stats,
            'excluded_residue0_reason':'Every F edge with n and a divisible by19 also belongs to H, violating W disjointness.',
            'local_enumeration_seconds':time.perf_counter()-start}
    print('LOCAL_ENUM',json.dumps(result['local_groups']),flush=True)
    if not args.locals_only:
        target_counts=[]
        for i,t in enumerate(TARGETS):
            c=f.count(0,t);target_counts.append(c)
            print('PATTERN',i,c,'cache',f.count.cache_info(),'seconds',time.perf_counter()-start,flush=True)
        result['F_partial_matching_counts_by_pattern']=[str(c)for c in target_counts]
        result['F_partial_matching_total']=str(sum(target_counts))
        result['count_cache']=f.count.cache_info()._asdict()
        if args.export_sampler:
            result['sampler_export']=f.export_sampler(HERE/f'F_conditional_sampler_{args.variant}.bin',args.variant)
            print('EXPORT',json.dumps(result['sampler_export']),flush=True)
    result['seconds']=time.perf_counter()-start
    result['source_sha256']=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    (HERE/f'F_matching_count_{args.variant}.json').write_text(json.dumps(result,indent=2)+'\n')

if __name__=='__main__':main()
