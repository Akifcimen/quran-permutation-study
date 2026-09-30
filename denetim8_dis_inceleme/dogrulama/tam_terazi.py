# Exact P(A) for the parity balance under uniform permutation (independent check of review 8, round 1, §3).
# A: |E| = 57 and sum over E of t = total verses, E = {n : n + a(n) even}. Equivalent: sum of E positions = sum of E^c values.
# Usage (repo root): python3 denetim8_dis_inceleme/dogrulama/tam_terazi.py quran_meta.json
import json, sys
from math import comb
from fractions import Fraction
K0=[s['numberOfAyahs'] for s in json.load(open(sys.argv[1]))['data']['surahs']['references']]
def subset_counts(items, k):
    M=sum(sorted(items)[-k:]); dp=[[0]*(M+1) for _ in range(k+1)]; dp[0][0]=1
    for x in items:
        for j in range(k,0,-1):
            prev=dp[j-1]; cur=dp[j]
            for s in range(M-x,-1,-1):
                if prev[s]: cur[s+x]+=prev[s]
    return dp[k]
def conv(a,b):
    r=[0]*(len(a)+len(b)-1)
    for i,x in enumerate(a):
        if x:
            for j,y in enumerate(b):
                if y: r[i+j]+=x*y
    return r
oddpos=list(range(1,115,2)); evenpos=list(range(2,115,2))
P=conv(subset_counts(oddpos,27),subset_counts(evenpos,30))
for v9 in (127,129):
    a=[v9 if n==9 else K0[n-1] for n in range(1,115)]
    ov=[x for x in a if x%2]; ev=[x for x in a if x%2==0]
    assert len(ov)+len(ev)==114
    no,ne=len(ov),len(ev)
    # x = odd values at odd positions; |E| = x + (57 - (no - x)) = 2x + 57 - no, so |E| = 57 needs x = no/2
    assert no%2==0; x=no//2
    k_vo=no-x; k_ve=57-x   # E^c odd values (at even positions), E^c even values (at odd positions)
    kpo=x; kpe=57-(no-x)
    Pp=conv(subset_counts(oddpos,kpo),subset_counts(evenpos,kpe)) if (kpo,kpe)!=(27,30) else P
    V=conv(subset_counts(ov,k_vo),subset_counts(ev,k_ve))
    F=sum(Pp[s]*V[s] for s in range(min(len(Pp),len(V))))
    pA=Fraction(comb(57,x)*comb(57,no-x),comb(114,no))*Fraction(F,comb(57,kpo)*comb(57,kpe)*comb(no,k_vo)*comb(ne,k_ve))
    print(v9, sum(a), 'odd values',no, 'P(A)=',float(pA))
