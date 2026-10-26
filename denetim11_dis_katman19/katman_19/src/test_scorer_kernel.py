#!/usr/bin/env python3
"""Validate the scorer against exhaustive independent partition enumeration.

Uses deterministic artificial histograms and feature masks, never study data or
simulation outcomes. The Python reference retains negative-utility modules and
enumerates all partitions, independently checking the C++ pruning and DP.
"""

import functools
import json
import math
import pathlib
import subprocess
import tempfile

from matrix_reference import PRIMES

V, F, P, C, K = 2, 14, 29, 406, 6
NTRAIN = 1_000_000
BELL = (1, 1, 2, 5, 15)
NEG = float("-inf")


def utilities(groups, states, salt, cost):
    result = []
    scale = (1, 10, 100, 1000, 2000)
    for group in range(groups):
        counts = [0] + [((group * 17 + state * 13 + salt) % 31) * scale[group % 5]
                        for state in range(1, states)]
        counts[0] = NTRAIN - sum(counts)
        assert counts[0] >= 0
        row = [NEG]
        for mask in range(1, states):
            count = sum(counts[state] for state in range(states) if state & mask == mask)
            row.append(-math.log2((count + 0.5) / (NTRAIN + 1)) - cost)
        result.append(row)
    return result


def policies():
    result = [[] for _ in range(K)]
    for p in PRIMES:
        uniform = 1 / P
        literal = 1 / 5 if p in (3, 5, 7, 11, 19) else 0
        discussion = 1 / 4 if p in (3, 5, 7, 19) else 0
        frequency = {3: 19, 5: 2, 7: 24, 11: 1, 19: 1}.get(p, 0) / 47
        weights = (uniform, (uniform + literal + discussion) / 3,
                   literal, discussion, int(p == 19), frequency)
        for k, weight in enumerate(weights):
            result[k].append(math.log2(weight) if weight else NEG)
    for row in result:
        assert math.isclose(sum(2 ** x for x in row), 1.0)
    return result


def state_hash(seed, variant, index, module, view):
    x = (seed + 104729 * (variant + 1) + 7919 * (index + 1) +
         1543 * (module + 1) + 32749 * (view + 1)) & 0xFFFFFFFF
    x = ((x ^ (x >> 16)) * 0x7FEB352D) & 0xFFFFFFFF
    x = ((x ^ (x >> 15)) * 0x846CA68B) & 0xFFFFFFFF
    return x ^ (x >> 16)


@functools.lru_cache(None)
def partitions(mask):
    if mask == 0:
        return ((),)
    first = mask & -mask
    answer = set()
    for old in partitions(mask ^ first):
        answer.add(tuple(sorted((first, *old))))
        for j in range(len(old)):
            joined = list(old)
            joined[j] |= first
            answer.add(tuple(sorted(joined)))
    return tuple(sorted(answer))


def make_candidates(seed, allowed, ug, uc, uw, um):
    variants = []
    for v in range(V):
        candidates = []
        global_values = []
        for f in range(F):
            gm = state_hash(seed, v, f, 0, 0) & 3 if allowed & 1 else 0
            gu = ug[v * F + f][gm]
            global_values.append((gu, gm))
            for pi in range(P):
                cx = f * P + pi
                cm = state_hash(seed, v, cx, 1, 0) & 15 if allowed & 2 else 0
                wm = state_hash(seed, v, cx, 2, 0) & 3 if allowed & 4 else 0
                m_options = []
                for view in range(8):
                    mm = state_hash(seed, v, cx, 3, view) & 15 if allowed & 8 else 0
                    m_options.append((um[(v * C + cx) * 8 + view][mm], view, mm))
                mu, best_view, mm = max(m_options, key=lambda item: item[0])
                u = (gu, uc[v * C + cx][cm], uw[v * C + cx][wm], mu)
                candidates.append((f, pi, u, (gm, cm, wm, mm), best_view))
        variants.append((global_values, candidates))
    return variants


def exhaustive_scores(variants, logq):
    scores = [0.0] * K
    form_cost = math.log2(F)
    for global_values, candidates in variants:
        for policy in range(K):
            block_best = [NEG] * 16
            block_best[1] = max(u for u, _ in global_values) - form_cost
            for block in range(2, 16):
                modules = [j for j in range(4) if block & (1 << j)]
                best = NEG
                for _, pi, u, _, _ in candidates:
                    candidate = sum(u[j] for j in modules) - form_cost + logq[policy][pi]
                    best = max(best, candidate)
                block_best[block] = best
            for active in range(1, 16):
                best = max(sum(block_best[block] for block in partition)
                           for partition in partitions(active))
                score = best - 5 - math.log2(BELL[active.bit_count()])
                scores[policy] = max(scores[policy], score)
    return scores


def check_trace(record, variants, policy, logq):
    score = record["value"]
    if score == 0:
        assert record["variant_index"] is None
        assert record["active_mask"] == 0 and record["blocks"] == []
        return
    v = record["variant_index"]
    global_values, candidates = variants[v]
    active = record["active_mask"]
    union = 0
    block_values = []
    for block in record["blocks"]:
        mask, f = block["module_mask"], block["form_index"]
        assert union & mask == 0
        union |= mask
        if mask == 1:
            assert block["prime_index"] is None
            gu, gm = global_values[f]
            expected = gu - math.log2(F)
            expected_masks = [gm, 0, 0, 0]
        else:
            pi = block["prime_index"]
            assert block["prime"] == PRIMES[pi]
            _, _, u, masks, view = candidates[f * P + pi]
            expected = sum(u[j] for j in range(4) if mask & (1 << j)) - math.log2(F) + logq[policy][pi]
            expected_masks = [masks[j] if mask & (1 << j) else 0 for j in range(4)]
            if mask & 8:
                assert block["matrix_view"] == view
        assert expected_masks == block["positive_masks"]
        assert math.isclose(expected, block["value"], abs_tol=1e-10)
        block_values.append(block["value"])
    assert union == active
    penalty = 5 + math.log2(BELL[active.bit_count()])
    assert math.isclose(penalty, record["program_penalty"], abs_tol=1e-10)
    assert math.isclose(sum(block_values), record["blocks_value"], abs_tol=1e-10)
    assert math.isclose(sum(block_values) - penalty, score, abs_tol=1e-10)


def main():
    for active in range(16):
        assert len(partitions(active)) == BELL[active.bit_count()]
    ug = utilities(V * F, 4, 3, 2)
    uc = utilities(V * C, 16, 5, 4)
    uw = utilities(V * C, 4, 7, 2)
    um = utilities(V * C * 8, 16, 11, 7)
    logq = policies()
    cases = [(20261025 + allowed * 37, allowed) for allowed in range(16)]
    cases.extend([(17, 15), (987654321, 15), (4294967295, 15)])
    src = pathlib.Path(__file__).resolve().parent
    with tempfile.TemporaryDirectory(prefix="joint-scorer-check-") as temp:
        driver = pathlib.Path(temp) / "scorer_driver"
        subprocess.run(["g++", "-std=c++17", "-O2", "-Wall", "-Wextra", "-pedantic",
                        str(src / "scorer_driver.cpp"), "-o", str(driver)], check=True)
        inputs = "".join(f"{seed} {allowed}\n" for seed, allowed in cases)
        outputs = subprocess.check_output([str(driver)], input=inputs, text=True).splitlines()
        assert len(outputs) == len(cases)
        nonzero = 0
        partition_sizes = set()
        for (seed, allowed), output in zip(cases, outputs):
            parsed = json.loads(output)
            assert parsed["training_n"] == NTRAIN
            variants = make_candidates(seed, allowed, ug, uc, uw, um)
            expected = exhaustive_scores(variants, logq)
            for policy, record in enumerate(parsed["policies"]):
                assert record["policy"] == policy
                assert math.isclose(record["value"], expected[policy], abs_tol=1e-10), (
                    seed, allowed, policy, record["value"], expected[policy])
                check_trace(record, variants, policy, logq)
                nonzero += record["value"] > 0
                partition_sizes.add(len(record["blocks"]))

    # The ideal code weights sum to <=1 over variants, activations, partitions,
    # parameter choices, view choices, and nonempty positive-predicate masks.
    mask_masses = (3 / 4, 15 / 16, 3 / 4, 15 / 16)
    total_mass = sum(math.prod(mask_masses[j] for j in range(4) if mask & (1 << j)) / 16
                     for mask in range(1, 16))
    assert total_mass <= 1
    print(f"PASS: {len(cases) * K} scores matched exhaustive form/prime/partition enumeration")
    print(f"PASS: {nonzero} nonzero traces reconstructed from independent module utilities")
    print(f"PASS: all 16 activation masks; observed partition sizes in artificial fixtures: {sorted(partition_sizes)}")
    print("PASS: negative-utility pruning and policy-independent form maximization")
    print(f"PASS: ideal program-code mass = {total_mass:.12f} <= 1")


if __name__ == "__main__":
    main()
