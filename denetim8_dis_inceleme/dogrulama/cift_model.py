# Adjacent-pair flip model: exact P(A∩G∩W) (independent check of review 8, round 2, §4).
# Pairs (1,2),(3,4),...,(113,114) each flipped by a fair coin. G (crystal) depends only on pairs that can hold a t≡0 (mod 19)
# member, W also on pairs that can hold a u≡0 member; the balance A is counted over the remaining pairs by exact DP.
# Usage (repo root): python3 denetim8_dis_inceleme/dogrulama/cift_model.py quran_meta.json
import json, sys, itertools
from fractions import Fraction
from collections import Counter
K0=[s['numberOfAyahs'] for s in json.load(open(sys.argv[1]))['data']['surahs']['references']]
ORI=[{1:(6,6,6),0:(6,6,8)},{1:(6,6,8),0:(6,6,6)}]; ORO=[{0:(5,5,9),1:(5,7,7)},{0:(5,7,7),1:(5,5,9)}]
def G(fam):  # fam: list of (n,a) with t%19==0
    if len(fam)!=12: return False
    cell=lambda tp,npar: tuple(sorted((n+a)//19 for n,a in fam if (n+a)%2==tp and n%2==npar))
    return any(cell(0,1)==e[1] and cell(0,0)==e[0] and cell(1,0)==o[0] and cell(1,1)==o[1] for e in ORI for o in ORO)
for v9 in (127,129):
    a=[None]+[v9 if n==9 else K0[n-1] for n in range(1,115)]; SA=sum(a[1:])
    pairs=[(i,i+1) for i in range(1,114,2)]
    def opts(p):
        i,j=p; return [((i,a[i]),(j,a[j])),((i,a[j]),(j,a[i]))]
    def hitT(o): return any((n+x)%19==0 for n,x in o)
    def hitU(o): return any((n-x)%19==0 for n,x in o)
    Tb=[p for p in pairs if any(hitT(o) for o in opts(p))]
    Ub=[p for p in pairs if p not in Tb and any(hitU(o) for o in opts(p))]
    neu=[p for p in pairs if p not in Tb and p not in Ub]
    sat=[]  # active configs satisfying G∧W
    for bits in itertools.product((0,1),repeat=len(Tb)):
        ch=[opts(p)[b] for p,b in zip(Tb,bits)]
        fam=[(n,x) for o in ch for n,x in o if (n+x)%19==0]
        if not G(fam): continue
        for ub in itertools.product((0,1),repeat=len(Ub)):
            ch2=ch+[opts(p)[b] for p,b in zip(Ub,ub)]
            allv=[(n,x) for o in ch2 for n,x in o]
            U=[(n,x) for n,x in allv if (n-x)%19==0]   # u-family can also sit in T-bit pairs
            Tn={n for n,x in fam}; Un={n for n,x in U}
            if len(U)==7 and not (Tn&Un) and len(Tn|Un)==19 and (sum(x for n,x in fam)+sum(x for n,x in U))%19==0:
                e=[(n+x) for n,x in allv if (n+x)%2==0]; sat.append((len(e),sum(e)))
    # neutral DP over (count of even t, sum of even t); weight = labeled outcomes
    dp=Counter({(0,0):1})
    for p in neu:
        nd=Counter()
        for o in opts(p):
            e=[n+x for n,x in o if (n+x)%2==0]
            for (c,s),w in dp.items(): nd[(c+len(e),s+sum(e))]+=w
        dp=nd
    good=sum(dp.get((57-c,SA-s),0) for c,s in sat)
    p=Fraction(good,2**(len(Tb)+len(Ub)+len(neu)))
    print(v9,'T-bits',len(Tb),'U-only bits',len(Ub),'neutral',len(neu),'G∧W configs',len(sat),'A-successes(labeled,neutral)',good,'P=',p,float(p))
