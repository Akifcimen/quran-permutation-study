#!/usr/bin/env python3
"""Independent exact-rational checks plus compiled production-sampler validation.

No numerical-structure feature scores are computed or inspected here.
"""
from collections import defaultdict
from fractions import Fraction
from itertools import permutations
from pathlib import Path
import hashlib
import json
import subprocess

ROOT = Path(__file__).resolve().parents[1]

def inv(ids, values):
    return sum(values[ids[i]] < values[ids[j]] for i in range(len(ids)) for j in range(i + 1, len(ids)))

def exact_checks():
    q = Fraction(2, 3)
    values = (3, 3, 2, 1)
    all_orders = list(permutations(range(4)))
    numeric_word_weights = defaultdict(Fraction)
    raw = {}
    for ids in all_orders:
        labeled_j = sum(ids[i] > ids[j] for i in range(4) for j in range(i + 1, 4))
        raw[ids] = q ** labeled_j
        numeric_word_weights[tuple(values[i] for i in ids)] += raw[ids]
    assert len(numeric_word_weights) == 12
    projection_ratios = set()
    for word, weight in numeric_word_weights.items():
        i = sum(word[a] < word[b] for a in range(4) for b in range(a + 1, 4))
        projection_ratios.add(weight / q**i)
    assert projection_ratios == {Fraction(5, 3)}
    mean_numerator = Fraction(0)
    normalizer = sum(numeric_word_weights.values())
    for word, weight in numeric_word_weights.items():
        i = sum(word[a] < word[b] for a in range(4) for b in range(a + 1, 4))
        mean_numerator += i * weight
    exact_mean = mean_numerator / normalizer
    assert exact_mean == Fraction(2164, 1235)
    mu = lambda m: sum(k * q**k for k in range(m)) / sum(q**k for k in range(m))
    assert sum(mu(m) for m in range(1, 5)) - mu(2) == exact_mean
    uniformized = {ids: numeric_word_weights[tuple(values[i] for i in ids)] / 2 for ids in all_orders}
    changed = (3, 3, 0, 1)
    support = [ids for ids in all_orders if inv(ids, values) == 2 and inv(ids, changed) == 3]
    assert len(support) == 4
    assert {raw[ids] for ids in support} == {Fraction(4, 9), Fraction(8, 27)}
    assert {uniformized[ids] for ids in support} == {Fraction(10, 27)}
    # Include edits to a label from the tied class, not just a unique value.
    checked_groups = 0
    for special in range(4):
        for replacement in range(-1, 5):
            altered = list(values)
            altered[special] = replacement
            groups = defaultdict(list)
            for ids in all_orders:
                groups[(inv(ids, values), inv(ids, altered))].append(uniformized[ids])
            for weights in groups.values():
                assert len(set(weights)) == 1
                checked_groups += 1
    return {
        "rational_projection_factor": str(next(iter(projection_ratios))),
        "rational_projected_mean": str(exact_mean),
        "labeled_orders_enumerated": len(all_orders),
        "numeric_words_enumerated": len(numeric_word_weights),
        "joint_support": ["".join("abcd"[i] for i in ids) for ids in support],
        "raw_joint_weights": [str(raw[ids]) for ids in support],
        "uniformized_joint_weights": [str(uniformized[ids]) for ids in support],
        "edited_label_and_value_conditional_groups_verified": checked_groups,
    }

def main():
    exact = exact_checks()
    binary = ROOT / "tests" / "test_sampler"
    command = ["g++", "-std=c++17", "-O3", "-Wall", "-Wextra", "-Wpedantic", "-I", str(ROOT / "src"), str(ROOT / "tests" / "test_sampler.cpp"), "-o", str(binary)]
    compilation = subprocess.run(command, text=True, capture_output=True, check=True)
    run = subprocess.run([str(binary), str(ROOT / "data" / "counts.txt")], text=True, capture_output=True, check=True)
    compiled = json.loads(run.stdout)
    result = {
        "status": "passed",
        "purpose": "Validate sampling distributions and constraints only; no feature results or inferential study outcomes were inspected.",
        "exact_rational_checks": exact,
        "compiled_production_code_checks": compiled,
        "compiler_command": command,
        "compiler_diagnostics": compilation.stderr,
        "sampler_sha256": hashlib.sha256((ROOT / "src" / "sampler.hpp").read_bytes()).hexdigest(),
        "cpp_test_sha256": hashlib.sha256((ROOT / "tests" / "test_sampler.cpp").read_bytes()).hexdigest(),
        "floating_point_note": "Alias probabilities and q use floating-point arithmetic. Exact statements concern the mathematical proposal/conditioning construction; empirical checks exercise its finite-precision implementation.",
    }
    output = ROOT / "tests" / "test_sampler_validation.json"
    output.write_text(json.dumps(result, indent=2))
    print(json.dumps(result, indent=2))

if __name__ == "__main__":
    main()
