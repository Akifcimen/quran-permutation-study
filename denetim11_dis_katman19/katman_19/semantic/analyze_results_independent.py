#!/usr/bin/env python3
"""Independent audit of raw final score shards and production run metadata.

Does not import the production analyzer or read its summaries. Diagnostics are
producer aggregate counts: we independently validate and analyze them but cannot
reconstruct all20 features from six saved score columns alone.
"""
from __future__ import annotations
from collections import Counter
from datetime import datetime, timezone
from pathlib import Path
import hashlib
import json
import math
import struct
import sys

import numpy as np
from scipy.stats import beta

ROOT = Path(__file__).resolve().parents[1]
RESULTS = ROOT / "results"
REVIEW = ROOT / "review"
MODELS = ("uniform", "block6", "inversion_exact")
POLICIES = ("uniform_29", "primary_mixture", "literal_integer_values", "number_discussion_values", "conditional_19", "literal_frequency")
DIAGNOSTICS = (
    "E1_modules_exist_separately_within_one_variant",
    "E2_same_form_and_prime_G_Cbase_W_M",
    "E3_same_form_and_prime_G_Ccenter_W_M",
    "E4_E3_and_number_discussion_prime",
    "any_Cbase", "any_Ccenter", "any_shared_Cbase_M", "any_shared_Ccenter_M",
    "canonical_129_Cbase", "canonical_129_Ccenter", "canonical_129_M", "canonical_129_Cbase_M", "canonical_129_G", "canonical_129_W",
    "canonical_127_Cbase", "canonical_127_Ccenter", "canonical_127_M", "canonical_127_Cbase_M", "canonical_127_G", "canonical_127_W",
)
MASK = (1 << 64) - 1
GAMMA = 0x9e3779b97f4a7c15

def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def read(path):
    return json.loads(Path(path).read_text())

def splitmix_once(state):
    z = (state + GAMMA) & MASK
    z = ((z ^ (z >> 30)) * 0xbf58476d1ce4e5b9) & MASK
    z = ((z ^ (z >> 27)) * 0x94d049bb133111eb) & MASK
    return (z ^ (z >> 31)) & MASK

def interval(k, n):
    assert 0 <= k <= n and n > 0
    lower = 0.0 if k == 0 else float(beta.ppf(0.025, k, n - k + 1))
    upper = 1.0 if k == n else float(beta.ppf(0.975, k + 1, n - k))
    upper_one = 1.0 if k == n else float(beta.ppf(0.95, k + 1, n - k))
    if k == 0:
        exact_upper = -math.expm1(math.log(0.025) / n)
        exact_upper_one = -math.expm1(math.log(0.05) / n)
        assert math.isclose(upper, exact_upper, rel_tol=1e-10)
        assert math.isclose(upper_one, exact_upper_one, rel_tol=1e-10)
    return {"clopper_pearson_two_sided_95": [lower, upper], "one_sided_95_upper": upper_one}

def conditional(name, numerator, denominator):
    assert 0 <= numerator <= denominator
    if denominator == 0:
        return {"name": name, "numerator": numerator, "denominator": denominator, "estimate": None, "status": "unestimable_zero_denominator", "clopper_pearson_two_sided_95": None, "one_sided_95_upper": None}
    return {"name": name, "numerator": numerator, "denominator": denominator, "estimate": numerator / denominator, "status": "estimated", **interval(numerator, denominator)}

def check_training(model, run):
    path = RESULTS / f"train_{model}.bin"
    payload = path.read_bytes()
    n, proposals = struct.unpack_from("<QQ", payload)
    assert n == 200000 == run["n"] == run["processed"]
    assert proposals == run["proposals"]
    layouts = (("g", (2, 14, 4)), ("c", (2, 406, 16)), ("w", (2, 406, 4)), ("m", (2, 406, 8, 16)))
    expected_bytes = 16 + sum(math.prod(shape) for _, shape in layouts) * 8
    assert len(payload) == expected_bytes == 962320
    offset = 16
    cells_checked = 0
    for _, shape in layouts:
        count = math.prod(shape)
        a = np.frombuffer(payload, dtype="<u8", count=count, offset=offset).reshape(shape)
        sums = a.sum(axis=-1)
        assert np.all(sums == n)
        cells_checked += sums.size
        offset += count * 8
    assert offset == len(payload)
    return {"file": path.name, "bytes": len(payload), "n": n, "proposals": proposals, "categorical_distributions_sum_checked": cells_checked, "sha256": sha(path)}

def audit_model(model):
    train = read(RESULTS / f"train_{model}_run.json")
    test = read(RESULTS / f"test_{model}_run.json")
    assert train["phase"] == "train" and test["phase"] == "test"
    assert train["model"] == test["model"] == model
    assert test["n"] == test["processed"] == 1000000
    assert test["threads"] == test["requested_threads"] == 8
    assert train["threads"] == train["requested_threads"] == 8
    training = check_training(model, train)
    observed_file = RESULTS / f"observed_{model}_scores.json"
    observed_record = read(observed_file)
    assert observed_record["training_n"] == 200000
    assert tuple(p["name"] for p in observed_record["policies"]) == POLICIES
    assert tuple(p["policy"] for p in observed_record["policies"]) == tuple(range(6))
    observed = np.array([p["value"] for p in observed_record["policies"]], dtype=np.float64)
    assert np.isfinite(observed).all()
    shards = sorted(RESULTS.glob(f"test_{model}_scores_*.bin"))
    assert [p.name for p in shards] == [f"test_{model}_scores_{i}.bin" for i in range(8)]
    arrays, shard_records = [], []
    for tid, path in enumerate(shards):
        count = test["n"] * (tid + 1) // 8 - test["n"] * tid // 8
        assert path.stat().st_size == count * 6 * 8 == 6000000
        a = np.fromfile(path, dtype="<f8").reshape(-1, 6)
        assert len(a) == count and np.isfinite(a).all()
        arrays.append(a)
        shard_records.append({"thread": tid, "file": path.name, "bytes": path.stat().st_size, "rows": len(a), "sha256": sha(path)})
    scores = np.concatenate(arrays, axis=0)
    assert scores.shape == (1000000, 6)
    hits = (scores >= observed).sum(axis=0)
    equalities = (scores == observed).sum(axis=0)
    policy_results = []
    for k, policy in enumerate(POLICIES):
        r = int(hits[k]); n = len(scores)
        policy_results.append({"policy": policy, "observed": float(observed[k]), "observed_variant_index": observed_record["policies"][k]["variant_index"], "hits_ge_observed": r, "exact_score_ties_with_observed": int(equalities[k]), "n": n, "plus_one_p": (r + 1) / (n + 1), "empirical_exceedance_rate": r / n, "maximum_control_score": float(scores[:, k].max()), "minimum_control_score": float(scores[:, k].min()), "control_quantiles_50_90_99_999": np.quantile(scores[:, k], [0.5, 0.9, 0.99, 0.999]).tolist(), **interval(r, n)})
    # Validate retained top50 against all one-million primary-column values.
    top = read(RESULTS / f"test_{model}_top.json")
    assert len(top) == 50
    top_values = np.array([r["scores"]["policies"][1]["value"] for r in top])
    exact_top = np.sort(np.partition(scores[:, 1], -50)[-50:])[::-1]
    assert np.array_equal(top_values, exact_top)
    for row in top:
        assert sorted(row["ids"]) == list(range(114))
    d = read(RESULTS / f"test_{model}_diagnostics.json")
    assert d["n"] == 1000000 and len(d["counts"]) == 20
    c = d["counts"]
    assert all(isinstance(v, int) and 0 <= v <= d["n"] for v in c)
    assert c[3] <= c[2] <= c[1] <= c[0]
    assert c[5] <= c[4] and c[7] <= c[6] <= c[4] and c[7] <= c[5]
    for b in (8, 14):
        assert c[b+1] <= c[b]
        assert c[b+3] <= min(c[b], c[b+2])
    conditional_results = [
        conditional("E2_given_E1", c[1], c[0]),
        conditional("E3_given_E2", c[2], c[1]),
        conditional("E4_given_E3", c[3], c[2]),
        conditional("centered_C_given_any_bare_C", c[5], c[4]),
        conditional("matrix_M_given_any_shared_bare_C", c[6], c[4]),
        conditional("matrix_M_given_any_shared_centered_C", c[7], c[5]),
    ]
    for b, variant in ((8, 129), (14, 127)):
        conditional_results.extend([
            conditional(f"canonical_{variant}_M_given_Cbase", c[b+3], c[b]),
            conditional(f"canonical_{variant}_Cbase_given_M", c[b+3], c[b+2]),
            conditional(f"canonical_{variant}_Ccenter_given_Cbase", c[b+1], c[b]),
        ])
    diagnostics = [{"index": i, "name": name, "count": c[i], "n": d["n"], "empirical_rate": c[i]/d["n"], **interval(c[i], d["n"])} for i, name in enumerate(DIAGNOSTICS)]
    return {"model": model, "train_run": train, "test_run": test, "training_histogram_validation": training, "observed_scores_sha256": sha(observed_file), "shards": shard_records, "total_test_rows": len(scores), "policies": policy_results, "top50_primary_matches_binary_scores": True, "diagnostics": diagnostics, "conditional_diagnostics": conditional_results, "diagnostic_scope": "Postprocessing and nesting validation of producer aggregate counts; six raw score columns do not encode the20 diagnostic flags, so those counts were not independently replayed from every sampled ID order."}

def seed_audit(models):
    records = []
    for model in models:
        for kind in ("train_run", "test_run"):
            run = model[kind]
            expected = [splitmix_once(run["seed"] ^ ((0xd1b54a32d192ed03 * (tid + 1)) & MASK)) for tid in range(run["threads"])]
            assert expected == run["worker_seeds"]
            for tid, seed in enumerate(expected):
                records.append({"model": run["model"], "phase": run["phase"], "thread": tid, "seed": seed})
    seeds = [r["seed"] for r in records]
    assert len(seeds) == len(set(seeds)) == 48
    inverse_gamma = pow(GAMMA, -1, 1 << 64)
    coords = sorted((s * inverse_gamma) & MASK for s in seeds)
    gaps = [coords[i+1] - coords[i] for i in range(len(coords)-1)] + [(1 << 64) + coords[0] - coords[-1]]
    min_gap = min(gaps)
    # Every bounded range in the sampler is<=114. For a given range r, the
    # multiply/reject algorithm rejects precisely (2^32 mod r) possible32-bit
    # inputs. Union-bound all possible rejected inputs over r=1..114. SplitMix's
    # output permutation visits each top32 input2^32 times perfull64-bit cycle.
    # This intentionally enormous bound covers every retry without observing
    # final RNG counters and remains much smaller than the seed-cycle spacing.
    reject_inputs_bound = sum((1 << 32) % r for r in range(1, 115))
    retry_states_bound = reject_inputs_bound * (1 << 32)
    max_proposals = max(m[phase]["proposals"] for m in models for phase in ("train_run", "test_run"))
    normal_calls_bound = 3 * 114 * max_proposals
    consumed_steps_bound = normal_calls_bound + retry_states_bound
    assert consumed_steps_bound < min_gap
    return {"all_worker_seed_formulas_recomputed": True, "distinct_worker_seeds": len(set(seeds)), "worker_records": records, "minimum_circular_splitmix_counter_gap": min_gap, "max_whole_run_proposals_used_as_per_worker_upper_bound": max_proposals, "nonretry_calls_upper_bound_per_worker": normal_calls_bound, "possible_rejected_top32_values_union_bound": reject_inputs_bound, "whole_cycle_retry_states_upper_bound": retry_states_bound, "total_rng_steps_conservative_upper_bound_per_worker": consumed_steps_bound, "counter_intervals_disjoint_under_implemented_bounded_ranges": True, "caveat": "Disjoint pseudorandom counter intervals do not establish mathematical independence of a deterministic generator. Bound assumes the audited sampler code and its ranges1..114, and the run manifests accurately record proposals."}

def write_notes(result):
    lines = ["# Independent final-results validation", "", f"Completed: {result['created_utc']}", "", "## Scope", "", "This audit read the24 raw binary score shards, three observed-score files, six final run manifests, three training histograms and three diagnostic aggregates. It did not import or read the main analyzer or its summary before completing these calculations. The analyst had been informed that the main analysis found zero exceedances; the calculations themselves were recomputed from the raw shards.", "", "## Raw-score results", "", "Each model has200,000 training draws and exactly1,000,000 fresh test draws. Each of its eight score shards contains125,000 rows of six float64 values, or6,000,000 bytes. All values are finite. Every training categorical histogram sums to200,000 and all binary headers, file lengths and runtime totals agree.", "", "| Model | Primary observed score | Largest primary control | Exceedances / test draws | Plus-one p |", "| --- | ---: | ---: | ---: | ---: |"]
    for model in result["models"]:
        p = model["policies"][1]
        lines.append(f"| {model['model']} | {p['observed']:.12g} | {p['maximum_control_score']:.12g} | {p['hits_ge_observed']} / {p['n']:,} | {p['plus_one_p']:.12g} |")
    first = result["models"][0]["policies"][0]
    lines += ["", "All six declared policies have zero exceedances in each of the three models. Comparisons use score >= observed score, including ties. The saved top50 primary scores match the50 largest scores recomputed from every raw test row.", "", f"For each individual zero-hit run, the plus-one Monte Carlo value is{first['plus_one_p']:.12g}; the exact two-sided95% Clopper–Pearson interval for the conditional tail probability is[0,{first['clopper_pearson_two_sided_95'][1]:.12g}]; the one-sided95% upper bound is{first['one_sided_95_upper']:.12g}. Zero hits do not mean zero probability or an arbitrarily tiny estimated tail probability.", "", "The three null models are not pooled into a3,000,000-draw denominator. Robustness is summarized by the maximum model-specific p-value. The six related policies are not treated as six independent confirmations.", "", "##20 diagnostics", "", "| Diagnostic | Uniform | Block6 | Exact inversion |", "| --- | ---: | ---: | ---: |"]
    for i, name in enumerate(DIAGNOSTICS):
        counts = [m["diagnostics"][i]["count"] for m in result["models"]]
        lines.append(f"| {name} | {counts[0]} | {counts[1]} | {counts[2]} |")
    lines += ["", "E1 through E4 all have count 0 under every model. Thus E2/E1, E3/E2 and E4/E3 are 0/0 and **unestimable**. The incremental rarity of common-rule, center and textual-anchor constraints within the complete four-module family cannot be estimated from these zero denominators.", "", "The simpler C-only event has nonzero counts in two models. For centered-C given any bare-C, uniform is 0/0 (unestimable), block6 is 0/48, and exact inversion is 4/75 = 0.0533333. Those limited conditional samples support the intervals in the machine-readable report; they must not be confused with the unobserved complete-family conditioning events.", "", "The20 diagnostic numbers are producer aggregates. Their bounds, nesting and conditional-denominator calculations were independently checked. They cannot all be reconstructed from the six saved score columns; this audit did not replay all3,000,000 feature evaluations.", "", "## Seeds and runtime bookkeeping", "", f"All48 final worker seeds match an independent implementation of the hashed seed formula and are globally distinct. Minimum circular distance between starting states in SplitMix's counter coordinates is {result['seed_audit']['minimum_circular_splitmix_counter_gap']:,} outputs.", "", f"For ranges 1..114 the union bound on all potentially rejected32-bit inputs is {result['seed_audit']['possible_rejected_top32_values_union_bound']:,}. Because SplitMix's full output map is bijective, this bounds retry states across an entire64-bit cycle by {result['seed_audit']['whole_cycle_retry_states_upper_bound']:,}. Adding342 times the largest whole-run proposal count bounds any worker's total consumed counter interval by {result['seed_audit']['total_rng_steps_conservative_upper_bound_per_worker']:,}, below the minimum starting-state gap. Thus the final worker counter intervals cannot overlap under the audited code and recorded proposal counts. This establishes stream separation, not mathematical independence of a deterministic PRNG.", "", "Training and test manifests use different master seeds, with no worker-state collision across phases or models. The initial defective arithmetic seed spacing is not present in the final manifests.", "", "## Reproduction", "", "Run `python semantic/analyze_results_independent.py` from any working directory. Machine-readable details, confidence bounds, policy-specific maxima, shard hashes and metadata checks are saved in `review/results_validation.json`."]
    # Keep narrative spacing readable without changing paths, code or verse IDs.
    replacements = {"the24": "the 24", "has200": "has 200", "exactly1": "exactly 1", "contains125": "contains 125", "or6,": "or 6,", "to200": "to 200", "top50": "top 50", "the50": "the 50", "is[0,": "is [0,", "is9.": "is 9.", "sided95": "sided 95", "two-sided95": "two-sided 95", "into a3": "into a 3", "##20": "## 20", "count0": "count 0", "are0/0": "are 0/0", "The20": "The 20", "all3,": "all 3,", "All48": "All 48", "entire64": "entire 64", "rejected32": "rejected 32", "Adding342": "Adding 342"}
    text = "\n".join(lines) + "\n"
    for a, b in replacements.items(): text = text.replace(a, b)
    (REVIEW / "results_validation.md").write_text(text)

def main():
    assert sys.byteorder == "little", "Production shard byte order must be checked on non-little-endian host"
    models = [audit_model(name) for name in MODELS]
    seeds = seed_audit(models)
    robust = {name: max(m["policies"][k]["plus_one_p"] for m in models) for k, name in enumerate(POLICIES)}
    result = {"status": "passed", "created_utc": datetime.now(timezone.utc).isoformat(), "independence_scope": "Separate implementation reading raw binary score shards; no production analyzer import or summary consultation. Prior zero-exceedance claim was disclosed by parent before audit.", "models": models, "seed_audit": seeds, "max_over_models_plus_one_p_by_policy": robust, "pooling": "No pooling across null models; no multiplication across policies.", "source_hashes": {"study_engine.cpp": sha(ROOT / "src" / "study_engine.cpp"), "sampler.hpp": sha(ROOT / "src" / "sampler.hpp"), "scorer.hpp": sha(ROOT / "src" / "scorer.hpp")}}
    REVIEW.mkdir(exist_ok=True)
    (REVIEW / "results_validation.json").write_text(json.dumps(result, indent=2))
    write_notes(result)
    print(json.dumps({"status": "passed", "models": [{"model": m["model"], "n": m["total_test_rows"], "hits": [p["hits_ge_observed"] for p in m["policies"]], "primary_observed": m["policies"][1]["observed"], "primary_maximum_control": m["policies"][1]["maximum_control_score"], "diagnostic_counts": [d["count"] for d in m["diagnostics"]]} for m in models], "common_zero_hit_bounds": interval(0, 1000000), "max_model_primary_plus_one_p": robust["primary_mixture"], "seed_counter_intervals_disjoint": seeds["counter_intervals_disjoint_under_implemented_bounded_ranges"], "output": str(REVIEW / "results_validation.json")}, indent=2))

if __name__ == "__main__":
    main()
