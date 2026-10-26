#!/usr/bin/env python3
"""Post-freeze descriptive decomposition; never changes the registered test.

Reads regenerated training histograms and saved observed feature masks. Repeats
the primary optimization independently for each count variant. Completed top
controls are checked against direct raw-data scalar and matrix arithmetic.
"""

import array
import functools
import hashlib
import json
import math
import pathlib
import struct
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "src"))
from matrix_reference import PRIMES, flags as matrix_flags, matrix_order

MODELS = ("uniform", "block6", "inversion_exact")
V, F, P, C = 2, 14, 29, 406
MODULES = ("G", "C", "W", "M")
WIDTHS = (2, 4, 2, 4)
FULL = (3, 15, 3, 15)
BELL = (1, 1, 2, 5, 15)
NEG = float("-inf")
FORM_COST = math.log2(F)


def policy_weights(policy):
    result = []
    for p in PRIMES:
        uniform = 1 / 29
        literal = 1 / 5 if p in (3, 5, 7, 11, 19) else 0
        discussion = 1 / 4 if p in (3, 5, 7, 19) else 0
        frequency = {3: 19, 5: 2, 7: 24, 11: 1, 19: 1}.get(p, 0) / 47
        result.append((uniform, (uniform + literal + discussion) / 3,
                       literal, discussion, int(p == 19), frequency)[policy])
    assert math.isclose(sum(result), 1)
    return result


@functools.lru_cache(None)
def partitions(mask):
    if mask == 0:
        return ((),)
    first = mask & -mask
    result = set()
    for old in partitions(mask ^ first):
        result.add(tuple(sorted((first, *old))))
        for j in range(len(old)):
            joined = list(old)
            joined[j] |= first
            result.add(tuple(sorted(joined)))
    return tuple(sorted(result))


class Training:
    def __init__(self, path):
        raw = path.read_bytes()
        self.sha256 = hashlib.sha256(raw).hexdigest()
        self.n, self.proposals = struct.unpack_from("=QQ", raw)
        sizes = (V * F * 4, V * C * 16, V * C * 4, V * C * 8 * 16)
        assert len(raw) == 16 + 8 * sum(sizes)
        data = array.array("Q")
        data.frombytes(raw[16:])
        self.tables = []
        offset = 0
        for size in sizes:
            self.tables.append(data[offset:offset + size])
            offset += size
        self.cache = {}

    def stats(self, variant, module, form, prime, view, mask):
        key = (variant, module, form, prime, view, mask)
        if key in self.cache:
            return self.cache[key]
        states = 1 << WIDTHS[module]
        cx = form * P + prime if prime is not None else None
        group = (variant * F + form if module == 0 else
                 (variant * C + cx) * 8 + view if module == 3 else
                 variant * C + cx)
        row = self.tables[module][group * states:(group + 1) * states]
        assert sum(row) == self.n
        count = sum(row[state] for state in range(states) if state & mask == mask)
        probability = (count + 0.5) / (self.n + 1)
        info = -math.log2(probability)
        result = {"positive_mask": mask, "training_successes": count,
                  "training_n": self.n, "fitted_probability": probability,
                  "fitted_surprisal_bits": info,
                  "predicate_code_bits": WIDTHS[module],
                  "view_code_bits": 3 if module == 3 else 0,
                  "utility": info - WIDTHS[module] - (3 if module == 3 else 0)}
        self.cache[key] = result
        return result

    def utility(self, variant, module, form, prime, view, mask):
        return self.stats(variant, module, form, prime, view, mask)["utility"] if mask else NEG


def candidate_values(training, records, variant):
    candidates = []
    for cx, rec in enumerate(records):
        f, pi = divmod(cx, P)
        masks = [rec["G"], rec["C"], rec["W"], 0]
        options = []
        for view in range(8):
            mask = (rec["matrix_packed"] >> (4 * view)) & 15
            options.append((training.utility(variant, 3, f, pi, view, mask), view, mask))
        mu, view, mm = max(options, key=lambda x: x[0])
        masks[3] = mm
        u = [training.utility(variant, j, f, pi, 0, masks[j]) for j in range(3)] + [mu]
        candidates.append({"form_index": f, "prime_index": pi, "prime": PRIMES[pi],
                           "matrix_view": view, "masks": masks, "utilities": u})
    return candidates


def bind_block(candidate, mask, logq):
    if any(candidate["masks"][j] == 0 for j in range(4) if mask & (1 << j)):
        return None
    pi = candidate["prime_index"]
    value = sum(candidate["utilities"][j] for j in range(4) if mask & (1 << j)) - FORM_COST
    if mask != 1:
        value += logq[pi]
    if not math.isfinite(value):
        return None
    return {"module_mask": mask, "modules": [MODULES[j] for j in range(4) if mask & (1 << j)],
            "form_index": candidate["form_index"], "prime_index": None if mask == 1 else pi,
            "prime": None if mask == 1 else PRIMES[pi],
            "matrix_view": candidate["matrix_view"] if mask & 8 else None,
            "positive_masks": [candidate["masks"][j] if mask & (1 << j) else 0 for j in range(4)],
            "block_value": value}


def program(training, variant, active, blocks, weights):
    if any(block is None for block in blocks):
        return None
    predicate_bits = sum(WIDTHS[j] for j in range(4) if active & (1 << j))
    view_bits = 3 if active & 8 else 0
    prime_bits = sum(-math.log2(weights[b["prime_index"]]) for b in blocks if b["prime_index"] is not None)
    costs = {"predicate_mask_bits": predicate_bits, "matrix_view_bits": view_bits,
             "form_binding_bits": len(blocks) * FORM_COST, "prime_binding_bits": prime_bits,
             "activation_bits": 4, "global_variant_bits": 1,
             "partition_bits": math.log2(BELL[active.bit_count()])}
    costs["total_bits"] = sum(costs.values())
    penalty = 5 + math.log2(BELL[active.bit_count()])
    value = sum(b["block_value"] for b in blocks) - penalty
    return {"variant": 129 if variant == 0 else 127, "active_mask": active,
            "active_modules": [MODULES[j] for j in range(4) if active & (1 << j)],
            "blocks": blocks, "raw_program_score": value, "costs": costs,
            "sum_fitted_module_surprisals_bits": value + costs["total_bits"]}


def optimize_variant(training, records, variant, weights):
    logq = [math.log2(q) if q else NEG for q in weights]
    candidates = candidate_values(training, records, variant)
    best_blocks = [None] * 16
    for mask in range(1, 16):
        for candidate in candidates:
            block = bind_block(candidate, mask, logq)
            if block is not None and (best_blocks[mask] is None or block["block_value"] > best_blocks[mask]["block_value"]):
                best_blocks[mask] = block
    best = None
    best_score = 0.0
    for active in range(1, 16):
        for partition in partitions(active):
            current = program(training, variant, active, [best_blocks[b] for b in partition], weights)
            if current is not None and current["raw_program_score"] > best_score:
                best, best_score = current, current["raw_program_score"]
    shared = program(training, variant, 15, [best_blocks[15]], weights)
    separate = program(training, variant, 15, [best_blocks[b] for b in (1, 2, 4, 8)], weights)
    return {"maximum_score": best_score, "winning_program": best,
            "four_module_shared_tuple_maximum": shared,
            "four_module_fully_separate_bindings_maximum": separate}, candidates


def direct_tuple(base, ids, variant, alpha, beta, prime):
    a = [127 if variant == 1 and identity == 8 else base[identity] for identity in ids]
    n = list(range(1, 115))
    t = [alpha * i + beta * aa for i, aa in zip(n, a)]
    u = [alpha * i - beta * aa for i, aa in zip(n, a)]
    even = [j for j, value in enumerate(t) if value % 2 == 0]
    fset = {j for j, value in enumerate(t) if value % prime == 0}
    hset = {j for j, value in enumerate(u) if value % prime == 0}
    total = sum(a)
    en, ea = sum(n[j] for j in even), sum(a[j] for j in even)
    gm = int(len(even) == 57) | (int(en == total - ea) << 1)
    cells = [[] for _ in range(4)]
    for j in fset:
        cells[(t[j] % 2) * 2 + n[j] % 2].append(t[j] // prime)
    counts, sums = list(map(len, cells)), list(map(sum, cells))
    equal = counts[0] > 0 and len(set(counts)) == 1
    def row(a0, b0, c0, d0):
        return a0 == b0 and sorted((c0, d0)) == [a0 - 1, a0 + 1]
    pattern = row(*sums) or row(*(sums[2:] + sums[:2]))
    support = {q for cell in cells for q in cell}
    uniform = any(len(cell) >= 2 and len(set(cell)) == 1 for cell in cells)
    band = bool(support) and len(support) == max(support) - min(support) + 1 and uniform
    centre = sum(sums) == 4 * prime
    cm = int(equal) | (int(pattern) << 1) | (int(band) << 2) | (int(centre) << 3)
    union = fset | hset
    ua = sum(a[j] for j in union)
    wm = int(not (fset & hset) and len(union) == prime) | (int(ua % prime == 0) << 1)
    fn, fa = sum(n[j] for j in fset), sum(a[j] for j in fset)
    pm = (en, ea, 6555 - en, total - ea)
    rm = (fn, fa, 6555 - fn, total - fa)
    matrix_masks = [matrix_flags(pm, rm, prime, 0, pc, rr, rc)
                    for pc in range(2) for rr in range(2) for rc in range(2)]
    return {"G": gm, "C": cm, "W": wm, "matrix_masks": matrix_masks,
            "cell_counts": counts, "cell_sums": sums,
            "P": list(pm), "R": list(rm), "union_cardinality": len(union),
            "union_a": ua, "even_count": len(even)}


def natural_decomposition(training, records, variant, weights, raw):
    pi = PRIMES.index(19)
    record = records[pi]
    actual = [record["G"], record["C"], record["W"], record["matrix_packed"] & 15]
    assert actual == [raw["G"], raw["C"], raw["W"], raw["matrix_masks"][0]]
    stats = []
    for j in range(4):
        info = dict(training.stats(variant, j, 0, pi, 0, FULL[j]))
        info.update({"module": MODULES[j], "actual_mask": actual[j],
                     "full_conjunction_satisfied": actual[j] == FULL[j]})
        stats.append(info)
    candidate = {"form_index": 0, "prime_index": pi, "matrix_view": 0,
                 "masks": list(FULL), "utilities": [x["utility"] for x in stats]}
    logq = [math.log2(q) if q else NEG for q in weights]
    eligible = sum(1 << j for j in range(4) if actual[j] == FULL[j])
    result = {"form": [1, 1], "prime": 19, "matrix_view": 0,
              "raw_arithmetic": raw, "full_mask_module_statistics": stats,
              "full_four_module_program_eligible": eligible == 15,
              "maximum_eligible_full_module_mask": eligible}
    for active, label in ((15, "four_full_modules"), (eligible, "all_satisfied_full_modules")):
        if active == 0 or active & eligible != active:
            result[label] = None
            continue
        shared = program(training, variant, active, [bind_block(candidate, active, logq)], weights)
        singleton_blocks = [bind_block(candidate, 1 << j, logq) for j in range(4) if active & (1 << j)]
        separate = program(training, variant, active, singleton_blocks, weights)
        credit = separate["costs"]["total_bits"] - shared["costs"]["total_bits"]
        assert math.isclose(shared["raw_program_score"] - separate["raw_program_score"], credit)
        result[label] = {"shared": shared, "fixed_identical_predicates_separately_bound": separate,
                         "code_saving_only_bits": credit}
    if eligible & 7 == 7:
        base = program(training, variant, 7, [bind_block(candidate, 7, logq)], weights)
        result["G_C_W_shared_program"] = base
        if eligible == 15:
            full = result["four_full_modules"]["shared"]
            result["matrix_extension_from_GCW"] = {
                "score_increment": full["raw_program_score"] - base["raw_program_score"],
                "matrix_fitted_surprisal_bits": stats[3]["fitted_surprisal_bits"],
                "additional_predicate_bits": 4, "additional_view_bits": 3,
                "additional_partition_bits": math.log2(3),
                "additional_form_or_prime_bits": 0,
                "not_an_independent_probability_multiplier": True}
        else:
            result["matrix_extension_from_GCW"] = None
    return result


def verify_top_control(model, training, base, forms):
    results = ROOT / "results"
    run_path = results / f"test_{model}_run.json"
    top_path = results / f"test_{model}_top.json"
    if not run_path.exists() or not top_path.exists():
        return {"status": "pending", "reason": "Completed run manifest/top file not yet available"}
    run = json.loads(run_path.read_text())
    top = json.loads(top_path.read_text())[0]
    ids = top["ids"]
    assert sorted(ids) == list(range(114))
    if model == "block6":
        assert all(sorted(ids[k:k + 6]) == list(range(k, k + 6)) for k in range(0, 114, 6))
    inversions = []
    for variant in range(2):
        a = [127 if variant == 1 and identity == 8 else base[identity] for identity in ids]
        inversions.append(sum(a[i] < a[j] for i in range(114) for j in range(i + 1, 114)))
    if model == "inversion_exact":
        assert inversions == [993, 994]
    cache = {}
    checks = []
    for policy, trace in enumerate(top["scores"]["policies"]):
        if trace["value"] == 0:
            assert trace["variant_index"] is None and trace["active_mask"] == 0 and trace["blocks"] == []
            checks.append({"policy": policy, "score_reconstructed": 0.0, "blocks_checked": 0})
            continue
        variant = trace["variant_index"]
        weights = policy_weights(policy)
        total = 0
        active_union = 0
        for block in trace["blocks"]:
            active = block["module_mask"]
            assert not (active_union & active)
            active_union |= active
            form = block["form_index"]
            pi = block["prime_index"]
            actual_pi = PRIMES.index(19) if pi is None else pi
            key = (variant, form, actual_pi)
            if key not in cache:
                cache[key] = direct_tuple(base, ids, variant, *forms[form], PRIMES[actual_pi])
            raw = cache[key]
            value = -FORM_COST
            if pi is not None:
                value += math.log2(weights[pi])
            for module in range(4):
                if not (active & (1 << module)):
                    assert block["positive_masks"][module] == 0
                    continue
                view = block["matrix_view"] if module == 3 else 0
                actual_mask = raw["matrix_masks"][view] if module == 3 else raw[MODULES[module]]
                assert block["positive_masks"][module] == actual_mask
                value += training.utility(variant, module, form, actual_pi, view, actual_mask)
                if module == 3:
                    utilities = [training.utility(variant, 3, form, actual_pi, j, mask)
                                 for j, mask in enumerate(raw["matrix_masks"])]
                    assert math.isclose(utilities[view], max(utilities), abs_tol=1e-10)
            assert math.isclose(value, block["value"], abs_tol=1e-10)
            total += value
        assert active_union == trace["active_mask"]
        penalty = 5 + math.log2(BELL[active_union.bit_count()])
        assert math.isclose(total - penalty, trace["value"], abs_tol=1e-10)
        checks.append({"policy": policy, "score_reconstructed": total - penalty,
                       "blocks_checked": len(trace["blocks"])})
    return {"status": "pass", "scope": "Saved strongest primary control; all six stored policy traces reconstructed directly, not a new search",
            "run_n": run["n"], "permutation_sha256": hashlib.sha256(bytes(ids)).hexdigest(),
            "strict_ascending_pair_counts_129_127": inversions,
            "raw_tuple_contexts_checked": len(cache), "policy_checks": checks}


def short_program(program_value, forms):
    if program_value is None:
        return "none"
    parts = []
    for block in program_value["blocks"]:
        names = "+".join(block["modules"])
        alpha, beta = forms[block["form_index"]]
        prime = "none" if block["prime"] is None else str(block["prime"])
        parts.append(f"{names}: ({alpha},{beta}), p={prime}")
    return "; ".join(parts)


def make_markdown(report, forms):
    lines = ["# Observed score decomposition after freezing the study", "",
             "This is a descriptive diagnostic. It does not modify any feature, weight, score, selection rule, test budget, or null model.", "",
             "Each variant-specific maximum retains the original one-bit variant-selection charge, making the two columns comparable to the registered combined score. These are surrogate scores, not new p values.", "",
             "## Maximum primary score by variant", "",
             "| Null training model | Variant | Maximum score | Winning program |", "|---|---:|---:|---|"]
    for model in MODELS:
        for row in report["models"][model]["variants"]:
            lines.append(f"| {model} | {row['variant']} | {row['maximum_score']:.6f} | {short_program(row['winning_program'], forms)} |")
    lines += ["", "## Natural tuple: full positive conjunctions", "",
              "The natural tuple is form (1,1), p=19, matrix view 0. A conjunction is eligible only when its full mask is true in that variant. Fitted probabilities use the frozen Jeffreys pseudocount; a zero training count is not a claim of zero true probability.", "",
              "| Model | Variant | Module | Full mask true? | Training successes / n | Fitted probability | Fitted surprise, bits |", "|---|---:|---|---|---:|---:|---:|"]
    for model in MODELS:
        for row in report["models"][model]["variants"]:
            for stat in row["natural_tuple"]["full_mask_module_statistics"]:
                lines.append(f"| {model} | {row['variant']} | {stat['module']} | {'yes' if stat['full_conjunction_satisfied'] else 'no'} | {stat['training_successes']} / {stat['training_n']} | {stat['fitted_probability']:.8g} | {stat['fitted_surprisal_bits']:.6f} |")
    lines += ["", "## Four-module sharing versus separate bindings", "",
              "All comparisons below keep the four modules activated. The separately optimized program may use different forms, primes and positive-predicate masks for the modules. It therefore does not isolate a pure reuse effect; the fixed-predicate comparison does.", "",
              "| Model | Variant | Best shared tuple | Best fully separate bindings | Natural identical-predicate shared | Natural identical-predicate separate |", "|---|---:|---:|---:|---:|---:|"]
    def score_or_na(obj):
        return "not eligible" if obj is None else f"{obj['raw_program_score']:.6f}"
    for model in MODELS:
        for row in report["models"][model]["variants"]:
            natural = row["natural_tuple"]["four_full_modules"]
            lines.append(f"| {model} | {row['variant']} | {score_or_na(row['four_module_shared_tuple_maximum'])} | {score_or_na(row['four_module_fully_separate_bindings_maximum'])} | {score_or_na(natural['shared'] if natural else None)} | {score_or_na(natural['fixed_identical_predicates_separately_bound'] if natural else None)} |")
    credit = report["fixed_parameter_reuse"]
    lines += ["", "## Isolated code-cost saving", "",
              f"For all four modules with the same natural predicates and parameters, sharing saves **{credit['total_code_saving_bits']:.6f} ideal code bits** under the primary mixture policy:", "",
              f"- Three repeated form bindings: 3 log2(14) = {credit['form_saving_bits']:.6f} bits.",
              f"- Two repeated prime bindings: 2[-log2(q(19))] = {credit['prime_saving_bits']:.6f} bits. G alone does not introduce a prime.", "",
              "Predicate-mask costs, matrix-view cost, activation cost, variant cost, and the partition code are unchanged in this fixed comparison. The saving is a property of the declared code and grammar. It is **not** an independent probability multiplier, and exponentiating it would not produce additional statistical evidence.", "",
              "Precisely, this saving measures reuse of the form parameters (alpha,beta) and the prime p. L(K), the plus/minus construction, and the module templates are fixed primitives of the grammar. They are reused by construction for every null dataset too. This diagnostic does not compare arbitrary aggregation operators or assign literal source-code lengths to alternative definitions of L.", "",
              "The condition b=p contributes through the joint C-module frequency. It is not counted again as a separate binding-cost bonus.", "",
              f"For a one-prime natural program, changing the uniform prime policy to the frozen primary mixture changes the prime cost by {report['textual_prior_score_shift_natural_one_prime_bits']:.6f} bits. This is a score-policy effect, not an independent event probability.", "",
              "## Matrix extension within the 127 convention", "",
              "Keeping the same 127 data and natural G+C+W program fixed isolates the fitted score contribution of adding M. The additional costs are 4 predicate-mask bits, 3 view bits, and log2(3) partition bits; no extra form or prime is introduced.", "",
              "| Model | Natural 127 G+C+W score | Natural 127 G+C+W+M score | Net matrix score increment |", "|---|---:|---:|---:|"]
    for model in MODELS:
        row = next(x for x in report["models"][model]["variants"] if x["variant"] == 127)
        natural = row["natural_tuple"]
        lines.append(f"| {model} | {natural['G_C_W_shared_program']['raw_program_score']:.6f} | {natural['four_full_modules']['shared']['raw_program_score']:.6f} | {natural['matrix_extension_from_GCW']['score_increment']:.6f} |")
    lines += ["", "These are within-score contributions. They are not independent probabilities. The 129 variant preserves G, C, and W at the natural tuple but fails the full M conjunction; it therefore does not have the same four-module program.", "",
              "## Direct verification of strongest saved controls", ""]
    for model in MODELS:
        check = report["models"][model]["top_control_verification"]
        lines.append(f"- {model}: {check['status']}. " +
                     (f"All {len(check['policy_checks'])} policy traces reconstructed from {check['raw_tuple_contexts_checked']} direct arithmetic contexts." if check['status'] == 'pass' else check['reason']))
    lines += ["", "Full module counts, cost ledgers, variant-specific winners, independent-binding programs, and control checks are in observed_decomposition.json. The accompanying script regenerates this diagnostic from the frozen inputs.", ""]
    return "\n".join(lines)


def main():
    protocol = json.loads((ROOT / "protocol.json").read_text())
    forms = protocol["forms"]
    base = list(map(int, (ROOT / "data" / "counts.txt").read_text().split()))
    assert len(base) == 114 and base[8] == 129
    observed = [json.loads((ROOT / "results" / f"observed_{model}_features.json").read_text()) for model in MODELS]
    assert all(item == observed[0] for item in observed)
    weights = policy_weights(1)
    pi19 = PRIMES.index(19)
    prime_cost = -math.log2(weights[pi19])
    report = {"kind": "post-freeze descriptive diagnostic", "test_definition_changed": False,
              "primary_policy": "equal mixture of uniform, literal-value, discussion-value policies",
              "variant_charge_retained": True,
              "score_is_not_log_joint_probability": True,
              "reuse_scope": "Shared form and prime bindings within the finite grammar; L(K), plus/minus construction, and module templates are fixed primitives, not alternative operators searched by this diagnostic.",
              "fixed_parameter_reuse": {"q19": weights[pi19], "form_saving_bits": 3 * FORM_COST,
                                         "prime_saving_bits": 2 * prime_cost,
                                         "total_code_saving_bits": 3 * FORM_COST + 2 * prime_cost},
              "textual_prior_score_shift_natural_one_prime_bits": math.log2(weights[pi19] * P),
              "models": {}}
    identity = list(range(114))
    raw_natural = [direct_tuple(base, identity, variant, 1, 1, 19) for variant in range(2)]
    for model in MODELS:
        training = Training(ROOT / "results" / f"train_{model}.bin")
        recorded = json.loads((ROOT / "results" / f"observed_{model}_scores.json").read_text())
        rows = []
        for variant in range(2):
            records = observed[0]["variants"][variant]["candidates"]
            row, _ = optimize_variant(training, records, variant, weights)
            row["variant"] = 129 if variant == 0 else 127
            row["natural_tuple"] = natural_decomposition(training, records, variant, weights, raw_natural[variant])
            rows.append(row)
        assert math.isclose(max(row["maximum_score"] for row in rows), recorded["policies"][1]["value"], abs_tol=1e-10)
        report["models"][model] = {"training_n": training.n, "training_sha256": training.sha256,
                                  "variants": rows,
                                  "joint_observed_primary_score_reproduced": recorded["policies"][1]["value"],
                                  "top_control_verification": verify_top_control(model, training, base, forms)}
    destination = ROOT / "review"
    (destination / "observed_decomposition.json").write_text(json.dumps(report, indent=2, allow_nan=False) + "\n")
    (destination / "observed_decomposition.md").write_text(make_markdown(report, forms))
    for model in MODELS:
        rows = report["models"][model]["variants"]
        print(model, "; ".join(f"{row['variant']}: {row['maximum_score']:.6f}" for row in rows),
              "top-control:", report["models"][model]["top_control_verification"]["status"])
    print(f"Fixed natural four-module code saving: {report['fixed_parameter_reuse']['total_code_saving_bits']:.9f} bits; no probability multiplier")


if __name__ == "__main__":
    main()
