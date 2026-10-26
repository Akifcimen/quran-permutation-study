# RAPOR §3.2 check: under O (family 12, Σq = 76, halves 38/38, odd-t cells 19/19), even-t bag = 6 even q summing 38,
# odd-t bag = 6 odd q split into two triples of 19. Keep pairs with equal 3/3 cells, a consecutive band of distinct q,
# and some q repeated at least five times. RAPOR reports 2 survivors; the bag counts depend on the q upper bound.
from itertools import combinations_with_replacement as cwr
from collections import Counter
even=[c for c in cwr(range(2,40,2),6) if sum(c)==38]
odd=sorted({tuple(sorted(t1+t2)) for t1 in [t for t in cwr(range(1,40,2),3) if sum(t)==19] for t2 in [t for t in cwr(range(1,40,2),3) if sum(t)==19]})
keep=[]
for e in even:
    for o in odd:
        cnt=Counter(e+o); d=sorted(cnt)
        if d==list(range(d[0],d[-1]+1)) and max(cnt.values())>=5: keep.append((e,o))
print('even bags',len(even),'odd bags',len(odd),'survivors',len(keep)); [print(k) for k in keep]
