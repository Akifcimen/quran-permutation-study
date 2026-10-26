"""Direct arithmetic audit of the observed n+a, p=19 structure."""
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / "src"))
from matrix_reference import matrix_order, multiply, IDENTITY


def structure(counts):
    records = [(n, a) for n, a in enumerate(counts, 1)]
    E = [(n, a) for n, a in records if (n+a) % 2 == 0]
    notE = [(n, a) for n, a in records if (n+a) % 2]
    F = [(n, a) for n, a in records if (n+a) % 19 == 0]
    H = [(n, a) for n, a in records if (n-a) % 19 == 0]
    U = sorted(set(F) | set(H))
    cells = []
    for t_parity in range(2):
        for n_parity in range(2):
            rs = [(n, a) for n, a in F if (n+a) % 2 == t_parity and n % 2 == n_parity]
            qs = [(n+a)//19 for n, a in rs]
            cells.append({"t_parity": t_parity, "n_parity": n_parity, "records": rs, "q": qs, "q_sum": sum(qs)})
    def aggregate(group):
        other = [r for r in records if r not in group]
        return [sum(n for n,a in group), sum(a for n,a in group), sum(n for n,a in other), sum(a for n,a in other)]
    Praw, Rraw = aggregate(E), aggregate(F)
    P, R = [x % 19 for x in Praw], [x % 19 for x in Rraw]
    pr, rp = multiply(P,R,19), multiply(R,P,19)
    plus = [(x+y)%19 for x,y in zip(pr,rp)]
    minus = [(x-y)%19 for x,y in zip(pr,rp)]
    return {
        "total_a": sum(counts), "total_n": sum(range(1,115)),
        "E_count": len(E), "E_sum_n": sum(n for n,a in E), "notE_sum_a": sum(a for n,a in notE),
        "F": F, "H": H, "F_count": len(F), "H_count": len(H), "overlap_count": len(set(F)&set(H)),
        "union_count": len(U), "union_sum_a": sum(a for n,a in U), "union_sum_a_quotient19": sum(a for n,a in U)/19,
        "cells": cells, "coefficient_sum": sum(c["q_sum"] for c in cells),
        "P_raw": Praw, "R_raw": Rraw, "P_mod19": P, "R_mod19": R,
        "anticommutator": plus, "commutator": minus,
        "commutator_squared": multiply(minus,minus,19),
        "orders": {"P": matrix_order(P,19), "R": matrix_order(R,19), "PR_plus_RP": matrix_order(plus,19)},
        "commutator_square_is_I": multiply(minus,minus,19) == IDENTITY,
        "strict_descending_disorder": sum(counts[i] < counts[j] for i in range(114) for j in range(i+1,114)),
    }


def main():
    data = json.loads((ROOT / "data/verse_counts.json").read_text())
    result = {str(v): structure(data[f"counts_{v}"]) for v in [129,127]}
    assert result["129"]["total_a"] == 6236
    assert result["127"]["total_a"] == 6234
    assert result["127"]["orders"] == {"P":360,"R":360,"PR_plus_RP":360}
    assert result["127"]["commutator_square_is_I"]
    assert result["127"]["union_sum_a"] == 1159
    (ROOT / "results/observed_structure.json").write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
    print(json.dumps(result,ensure_ascii=False,indent=2))


if __name__ == '__main__':
    main()
