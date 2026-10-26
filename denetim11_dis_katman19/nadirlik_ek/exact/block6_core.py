#!/usr/bin/env python3
"""Exact block6 counting and uniform conditional sampling of natural C=15.

This is a supplementary calculation for the frozen natural tuple (1,1), p=19.
It does not redefine or substitute for the frozen maximum-score tail.
"""
from __future__ import annotations

import argparse
from bisect import bisect_right
from collections import Counter, defaultdict
from fractions import Fraction
from functools import lru_cache
import hashlib
import itertools
import json
from pathlib import Path
import random
import time

ROOT = Path(__file__).resolve().parents[2]
CLASSES = ((0, 6), (0, 8), (1, 6), (2, 5), (2, 7), (2, 9), (3, 5), (3, 7))
TARGETS = ((2, 1, 3, 2, 0, 1, 1, 2), (2, 1, 3, 1, 2, 0, 1, 2))
TARGET_MAX = tuple(max(x[j] for x in TARGETS) for j in range(len(CLASSES)))
ZERO = (0,) * len(CLASSES)


def feature_c(ids, base):
    """Independent literal implementation of the four C predicates."""
    cells = [[] for _ in range(4)]
    for i, j in enumerate(ids):
        n, a = i + 1, base[j]
        if (n + a) % 19 == 0:
            cells[((n + a) & 1) * 2 + (n & 1)].append((n + a) // 19)
    sizes = list(map(len, cells))
    sums = list(map(sum, cells))
    equal = bool(sizes[0]) and len(set(sizes)) == 1
    pattern = any(sums[a] == sums[b] and sorted((sums[c], sums[d])) ==
                  [sums[a] - 1, sums[a] + 1]
                  for a, b, c, d in ((0, 1, 2, 3), (2, 3, 0, 1)))
    values = set(itertools.chain.from_iterable(cells))
    band = bool(values) and max(values) - min(values) + 1 == len(values)
    band &= any(len(c) >= 2 and len(set(c)) == 1 for c in cells)
    centre = sum(sums) == 76
    return int(equal) + 2 * pattern + 4 * band + 8 * centre


def validate_only_target(base):
    """Enumerate coefficient multisets, an independent exhaustive proof.

    All possible block-preserving F edges supply cell-specific capacities.
    Ignore assignment incompatibilities: this larger set still has exactly
    one C=15 coefficient multiset, proving necessity. Observed order supplies
    sufficiency. Multiplicities here are labeled edge capacities, so duplicate
    verse counts cannot invalidate the upper envelope.
    """
    cap = [Counter() for _ in range(4)]
    edges = []
    for bi in range(19):
        for n in range(6 * bi + 1, 6 * bi + 7):
            for j in range(6 * bi, 6 * bi + 6):
                a = base[j]
                if (n + a) % 19 == 0:
                    c, q = ((n + a) & 1) * 2 + (n & 1), (n + a) // 19
                    cap[c][q] += 1
                    edges.append((n, j, c, q))
    by_cell = []
    for c in range(4):
        opts = []
        keys = sorted(cap[c])
        for ns in itertools.product(*(range(cap[c][q] + 1) for q in keys)):
            size, total = sum(ns), sum(q * m for q, m in zip(keys, ns))
            if 0 < size and total <= 20:
                expanded = tuple(q for q, m in zip(keys, ns) for _ in range(m))
                opts.append((size, total, expanded))
        by_cell.append(opts)
    possible = []
    for rows in itertools.product(*by_cell):
        if len({r[0] for r in rows}) != 1:
            continue
        sums = [r[1] for r in rows]
        if sum(sums) != 76:
            continue
        pattern = any(sums[a] == sums[b] and sorted((sums[c], sums[d])) ==
                      [sums[a] - 1, sums[a] + 1]
                      for a, b, c, d in ((0, 1, 2, 3), (2, 3, 0, 1)))
        if not pattern:
            continue
        values = set(itertools.chain.from_iterable(r[2] for r in rows))
        band = max(values) - min(values) + 1 == len(values)
        uniform = any(r[0] >= 2 and len(set(r[2])) == 1 for r in rows)
        if band and uniform:
            possible.append([list(r[2]) for r in rows])
    expected = [[[6, 6, 8], [6, 6, 6], [5, 5, 9], [5, 7, 7]],
                [[6, 6, 8], [6, 6, 6], [5, 7, 7], [5, 7, 7]]]
    assert sorted(possible) == sorted(expected), possible
    assert feature_c(tuple(range(114)), base) == 15
    return {"possible_cell_coefficient_multisets": possible,
            "edge_capacity_by_cell": [dict(sorted(x.items())) for x in cap],
            "allowed_edges": edges,
            "proof": "Exhaustive cell-multiset envelope; every assignment is inside the envelope."}


class ConditionalCore:
    def __init__(self, base):
        self.base = base
        self.groups = []
        self.local_profiles = []
        for bi in range(19):
            groups = defaultdict(list)
            profiles = []
            for ids in itertools.permutations(range(6 * bi, 6 * bi + 6)):
                sig = [0] * len(CLASSES)
                ok = True
                eg, es, en, fn, hn, hs, overlap, edit_even = 0, 0, 0, 0, 0, 0, 0, 0
                for offset, j in enumerate(ids):
                    n, a = 6 * bi + offset + 1, base[j]
                    even = (n + a) % 2 == 0
                    eg += even
                    es += (n + a) * even // 2
                    en += n * even
                    edit_even |= int(j == 8 and even)
                    f, h = (n + a) % 19 == 0, (n - a) % 19 == 0
                    fn += n * f
                    hn += h
                    hs += n * h
                    overlap += f and h
                    if f:
                        key = (((n + a) & 1) * 2 + (n & 1), (n + a) // 19)
                        if key not in CLASSES:
                            ok = False
                        else:
                            sig[CLASSES.index(key)] += 1
                if ok and all(s <= t for s, t in zip(sig, TARGET_MAX)):
                    profile = {"ids": ids, "signature": tuple(sig), "ec": eg,
                               "es_half_129": es, "en_mod19": en % 19,
                               "fn_mod19": fn % 19, "hcount": hn,
                               "h_minus_f_mod19": (hs - fn) % 19,
                               "overlap": overlap, "edit_in_E": edit_even}
                    profiles.append(profile)
                    groups[tuple(sig)].append(len(profiles) - 1)
            self.groups.append([(sig, tuple(indexes)) for sig, indexes in sorted(groups.items())])
            self.local_profiles.append(profiles)
        self.transition_cache = {}

    @lru_cache(None)
    def completions(self, block, remaining):
        if block == 19:
            return int(remaining == ZERO)
        count = 0
        for sig, indexes in self.groups[block]:
            rest = tuple(a - b for a, b in zip(remaining, sig))
            if min(rest) >= 0:
                count += len(indexes) * self.completions(block + 1, rest)
        return count

    @property
    def count(self):
        return sum(self.completions(0, target) for target in TARGETS)

    def transitions(self, block, remaining):
        key = (block, remaining)
        if key not in self.transition_cache:
            choices, cumulative, total = [], [], 0
            for sig, indexes in self.groups[block]:
                rest = tuple(a - b for a, b in zip(remaining, sig))
                if min(rest) < 0:
                    continue
                child = self.completions(block + 1, rest)
                if child:
                    choices.append((rest, indexes, child))
                    total += len(indexes) * child
                    cumulative.append(total)
            assert total == self.completions(block, remaining)
            self.transition_cache[key] = (choices, cumulative)
        return self.transition_cache[key]

    def unrank(self, rank, include_profiles=False):
        assert 0 <= rank < self.count
        remaining = None
        for target in TARGETS:
            count = self.completions(0, target)
            if rank < count:
                remaining = target
                break
            rank -= count
        assert remaining is not None
        ids, chosen = [], []
        for bi in range(19):
            choices, cumulative = self.transitions(bi, remaining)
            index = bisect_right(cumulative, rank)
            if index:
                rank -= cumulative[index - 1]
            remaining, indexes, child = choices[index]
            local_index, rank = divmod(rank, child)
            profile = self.local_profiles[bi][indexes[local_index]]
            ids.extend(profile["ids"])
            chosen.append(profile)
        assert remaining == ZERO and rank == 0
        return (ids, chosen) if include_profiles else ids

    def export(self):
        # Every positive-completion reachable state, suitable for a separate
        # C++ sampler without carrying irrelevant DP states.
        todo, seen, nodes = [(0, target) for target in TARGETS], set(), []
        while todo:
            bi, remaining = todo.pop()
            if (bi, remaining) in seen or bi == 19:
                continue
            seen.add((bi, remaining))
            choices, cumulative = self.transitions(bi, remaining)
            nodes.append({"block": bi, "remaining": remaining,
                          "count": str(self.completions(bi, remaining)),
                          "choices": [{"remaining": rest, "indexes": indexes,
                                       "child_count": str(child)}
                                      for rest, indexes, child in choices]})
            todo.extend((bi + 1, rest) for rest, _, _ in choices)
        return {"classes": CLASSES, "targets": TARGETS,
                "target_counts": [str(self.completions(0, target)) for target in TARGETS],
                "count": str(self.count), "denominator": str(720 ** 19),
                "blocks": self.local_profiles,
                "states": sorted(nodes, key=lambda x: (x["block"], x["remaining"]))}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--draws", type=int, default=10000)
    ap.add_argument("--seed", type=int, default=202610250501)
    ap.add_argument("--out", type=Path, default=Path(__file__).resolve().parent)
    args = ap.parse_args()
    args.out.mkdir(parents=True, exist_ok=True)
    start = time.perf_counter()
    count_path = ROOT / "data" / "counts.txt"
    base = list(map(int, count_path.read_text().split()))
    proofs = [validate_only_target(base)]
    edited = base.copy()
    edited[8] = 127
    proofs.append(validate_only_target(edited))
    assert proofs[0] == proofs[1]
    core = ConditionalCore(base)
    probability = Fraction(core.count, 720 ** 19)
    export = core.export()
    (args.out / "block6_C_sampler.json").write_text(json.dumps(export, separators=(",", ":")))
    rng = random.Random(args.seed)
    checksum = hashlib.sha256()
    counts = Counter()
    with (args.out / "block6_C_validation_draws.bin").open("wb") as fh:
        for j in range(args.draws):
            ids, profiles = core.unrank(rng.randrange(core.count), True)
            data = bytes(ids)
            fh.write(data)
            checksum.update(data)
            if j < 1000:
                assert sorted(ids) == list(range(114))
                assert all(i // 6 == v // 6 for i, v in enumerate(ids))
                assert feature_c(ids, base) == feature_c(ids, edited) == 15
                assert sum(p["ec"] for p in profiles) == sum((i + 1 + base[v]) % 2 == 0 for i, v in enumerate(ids))
            ec = sum(p["ec"] for p in profiles)
            es = sum(p["es_half_129"] for p in profiles)
            ei = sum(p["edit_in_E"] for p in profiles)
            w = sum(p["hcount"] for p in profiles) == 7 and sum(p["h_minus_f_mod19"] for p in profiles) % 19 == 0 and not any(p["overlap"] for p in profiles)
            g0 = ec == 57 and es == sum(base) // 2
            g1 = ec == 57 and es - ei == (sum(base) - 2) // 2
            counts["W"] += w
            counts["G129"] += g0
            counts["G127"] += g1
            counts["G129_W"] += g0 and w
            counts["G127_W"] += g1 and w
    result = {"event": "Natural C=15 at alpha=beta=1, prime19, block6 model",
              "new_statistic": False,
              "is_full_score_tail_probability": False,
              "count": str(core.count), "sample_space": str(720 ** 19),
              "reduced_numerator": str(probability.numerator),
              "reduced_denominator": str(probability.denominator),
              "probability": float(probability),
              "both_variants_identical_C": True,
              "proofs": proofs,
              "dp_cache": core.completions.cache_info()._asdict(),
              "reachable_positive_states": len(export["states"]),
              "block_eligible_permutation_counts": list(map(len, core.local_profiles)),
              "block_distinct_signature_counts": list(map(len, core.groups)),
              "validation_draws": args.draws, "validation_seed": args.seed,
              "validation_counts": counts,
              "validation_draws_sha256": checksum.hexdigest(),
              "source_counts_sha256": hashlib.sha256(count_path.read_bytes()).hexdigest(),
              "seconds": time.perf_counter() - start}
    (args.out / "block6_C_exact.json").write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps({k: v for k, v in result.items() if k != "proofs"}, indent=2))


if __name__ == "__main__":
    main()
