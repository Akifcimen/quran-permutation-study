#!/usr/bin/env python3
"""Rebuild numeral tables and join the separately reviewed semantic annotations."""
import hashlib
import json
from collections import Counter
from datetime import datetime, timezone
from pathlib import Path

import parser as numeral_parser

BASE = Path(__file__).resolve().parent

def main():
    numeral_parser.main()
    audit = json.loads((BASE / "salience_audit.json").read_text())
    rows = json.loads((BASE / "inventory.json").read_text())
    by_ref = {r["verse"]: r for r in audit["prime_context_review"]}
    observed = Counter((r["verse_key"], r["value"]) for r in rows if r["eligible_odd_prime_under_114"])
    independently_reviewed = Counter((r["verse"], p) for r in audit["prime_context_review"] for p in r["prime_values_with_multiplicity"])
    assert observed == independently_reviewed
    for row in rows:
        refs = []
        if row["eligible_odd_prime_under_114"]:
            a = by_ref[row["verse_key"]]
            refs.append({"tier": a["tier"], "status": a["status"], "rationale": a["rationale"]})
        else:
            for a in audit["other_number_discussion_review"]:
                if row["verse_key"] not in a["verses"]:
                    continue
                eligible_values = a["literal_integer_values_with_multiplicity"] if row["primary_literal_integer"] else a["fraction_values"] if row["kind"] == "fraction" else []
                if row["value"] in eligible_values:
                    refs.append({"tier": a["tier"], "status": "see_context_and_rationale", "rationale": a["rationale"]})
        row["semantic_annotations"] = refs
        row["semantic_annotation_coverage"] = "fully_reviewed_eligible_prime_context" if row["eligible_odd_prime_under_114"] else "additional_nonprime_context_review" if refs else "not_individually_semantically_classified"
        row["semantic_annotation_provenance"] = "Model-assisted independent annotator reviewed all eligible-prime contexts; observed 19 hypothesis known; numerical simulation outcomes not accessed. This is not blinded human validation."
    for name, data in [
        ("inventory", rows),
        ("literal_integer_inventory", [r for r in rows if r["primary_literal_integer"]]),
        ("eligible_prime_inventory", [r for r in rows if r["eligible_odd_prime_under_114"]]),
    ]:
        (BASE / f"{name}.json").write_text(json.dumps(data, ensure_ascii=False, indent=2), encoding="utf-8")
        numeral_parser.dump_csv(BASE / f"{name}.csv", data)
    summary = json.loads((BASE / "numeral_summary.json").read_text())
    primes = summary["eligible_prime_universe"]
    literal = summary["literal_eligible_prime_values"]
    core = sorted({p for r in audit["prime_context_review"] if r["tier"] == "core_number_discussion" for p in r["prime_values_with_multiplicity"]})
    assert literal == [3, 5, 7, 11, 19]
    assert core == [3, 5, 7, 19]
    assert not any(r["primary_literal_integer"] and r["verse_key"] in {"9:128", "9:129"} for r in rows)
    def uniform(values):
        return {str(p): (1 / len(values) if p in values else 0.0) for p in primes}
    branches = {
        "all_primes_reference": {"values": primes, "weights": uniform(primes)},
        "literal_distinct_values": {"values": literal, "weights": uniform(literal)},
        "core_number_discussion_distinct_values": {"values": core, "weights": uniform(core)},
        "broad_symbolic_epistemic_distinct_values_sensitivity": {"values": literal, "weights": uniform(literal)},
        "literal_frequency_sensitivity": {"values": literal, "weights": summary["literal_frequency_weights_sensitivity_only"]},
    }
    for branch in branches.values():
        assert abs(sum(branch["weights"].values()) - 1) < 1e-12
    weights_path = BASE / "semantic_weights.json"
    if weights_path.exists():
        old = json.loads(weights_path.read_text())
        # Regeneration must not silently move the original protocol timestamp.
        frozen_time = old["frozen_utc"]
        assert old["branches"] == branches, "Frozen semantic weights changed; explicit versioning required."
    else:
        frozen_time = datetime.now(timezone.utc).isoformat()
    weights = {
        "frozen_utc": frozen_time,
        "scope": "Conditional text-anchor weights for odd-prime candidates3..113; values are literal semantic amounts, never verse addresses.",
        "selection_history": "Observed19 numerical hypothesis known when protocol designed. Sets communicated to parent before any simulation results were seen by the semantic team. Current freeze is not a retroactive preregistration of the original discovery.",
        "primary_count_rule": "Explicit lexical cardinal numeral or distributive numeral in original Arabic; compose grammatical number phrases; include reported or rejected claims with status flags; exclude ordinal/fraction/indefinite pronouns, grammatical dual nouns and inferred totals.",
        "core_salience_rule": audit["tiers"]["core_number_discussion"],
        "default_weights": "Uniform over distinct eligible values, not occurrences or subjective annotation frequency.",
        "variant_127_vs_129": "Dropping9:128-129 removes no primary literal integer mention; semantic candidate sets and weights are identical. Source text remains unchanged in this directory.",
        "branches": branches,
        "literal_occurrence_counts": summary["literal_eligible_prime_frequency"],
        "semantic_audit_sha256": hashlib.sha256((BASE / "salience_audit.json").read_bytes()).hexdigest(),
        "numeral_source_sha256": summary["source_hashes"],
    }
    weights_path.write_text(json.dumps(weights, ensure_ascii=False, indent=2), encoding="utf-8")
    wrows = [{"prime": p, **{name: branch["weights"][str(p)] for name, branch in branches.items()}, "literal_mentions": int(summary["literal_eligible_prime_frequency"].get(str(p), 0))} for p in primes]
    numeral_parser.dump_csv(BASE / "semantic_weights.csv", wrows)
    print(json.dumps({"frozen_utc": frozen_time, "literal_values": literal, "core_values": core, "prime_mentions": len([r for r in rows if r["eligible_odd_prime_under_114"]]), "prime_verse_contexts": len(by_ref)}, indent=2))

if __name__ == "__main__":
    main()
