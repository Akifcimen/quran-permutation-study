# Independent check of the matrix-layer claims (RAPOR.md §3.3–3.4; katman_19 §3.2).
import json, sys, itertools
K=[int(x) for x in open(sys.argv[1]).read().split()]
def mul(A,B,p): return [(A[0]*B[0]+A[1]*B[2])%p,(A[0]*B[1]+A[1]*B[3])%p,(A[2]*B[0]+A[3]*B[2])%p,(A[2]*B[1]+A[3]*B[3])%p]
I=[1,0,0,1]
def order(A,p):
    X=A[:]
    for k in range(1,p*p):
        if X==I: return k
        X=mul(X,A,p)
    return None
def mats(a,p=19):
    recs=list(enumerate(a,1)); tot=sum(a)
    E=[(n,x) for n,x in recs if (n+x)%2==0]; F=[(n,x) for n,x in recs if (n+x)%19==0]
    L=lambda K:[sum(n for n,_ in K), sum(x for _,x in K), 6555-sum(n for n,_ in K), tot-sum(x for _,x in K)]
    P=[v%p for v in L(E)]; R=[v%p for v in L(F)]
    pr,rp=mul(P,R,p),mul(R,P,p); plus=[(u+v)%p for u,v in zip(pr,rp)]; minus=[(u-v)%p for u,v in zip(pr,rp)]
    return L(E),L(F),P,R,order(P,p),order(R,p),order(plus,p),mul(minus,minus,p)
for v in (129,127):
    a=[127 if (n==9 and v==127) else K[n-1] for n in range(1,115)]
    LE,LF,P,R,oP,oR,oS,D2=mats(a)
    print(v,'raw P',LE,'raw R',LF,'P mod19',P,'R mod19',R,'ord P,R,PR+RP =',oP,oR,oS,'(PR-RP)^2 =',D2)
# identity (PR-RP)^2 = s^2 (x^2 + s y - y^2) I under P=[[y,s-y],[-y,y]], R=[[x,-x],[-x,s+x]] (mod 19), all 19^3 triples
p=19; bad=0
for s,x,y in itertools.product(range(p),repeat=3):
    P=[y%p,(s-y)%p,(-y)%p,y%p]; R=[x%p,(-x)%p,(-x)%p,(s+x)%p]
    pr,rp=mul(P,R,p),mul(R,P,p); minus=[(u-v)%p for u,v in zip(pr,rp)]
    D2=mul(minus,minus,p); c=(s*s*(x*x+s*y-y*y))%p
    if D2!=[c,0,0,c]: bad+=1
print('identity violations over 19^3 triples:',bad)
# how many (x,y) give full natural matrix architecture with s=2 (127) and s=4 (129)
for s in (2,4):
    good=[]; ordPR=[]
    for x,y in itertools.product(range(p),repeat=2):
        P=[y,(s-y)%p,(-y)%p,y]; R=[x,(-x)%p,(-x)%p,(s+x)%p]
        oP,oR=order(P,p),order(R,p)
        if oP==360 and oR==360: ordPR.append((x,y))
        pr,rp=mul(P,R,p),mul(R,P,p); plus=[(u+v)%p for u,v in zip(pr,rp)]; minus=[(u-v)%p for u,v in zip(pr,rp)]
        if oP==360 and oR==360 and order(plus,p)==360 and mul(minus,minus,p)==I: good.append((x,y))
    print('s =',s,': pairs with ord P = ord R = 360:',len(ordPR),' full architecture:',good)
